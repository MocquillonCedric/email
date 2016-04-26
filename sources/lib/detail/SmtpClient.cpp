#include "SmtpClient.h"
#include <mail/Credentials.h>
#include <mail/Message.h>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/utility/string_ref.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

namespace {

	namespace base64 {
		std::string encode(std::string const& data) {
			static char const* base64_padding[] = { "", "==","=" };

			using namespace boost::archive::iterators;
			using base64_enc = base64_from_binary<transform_width<char const*, 6, 8>>;

			std::stringstream os;
			std::copy(base64_enc(data.c_str()), base64_enc(data.c_str() + data.size()), std::ostream_iterator<char>(os));
			os << base64_padding[data.size() % 3];
			return os.str();
		}
	}

	static const std::string newline = "\r\n";

	template<typename socket_t>
	std::string check_response(std::string const& responseWaited, socket_t& socket) {
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");
		std::string const srv_response((std::istreambuf_iterator<char>(&response)), std::istreambuf_iterator<char>());

		if (responseWaited != srv_response.substr(0, 3)) {
			throw std::runtime_error("Unexpected server response:\n" + srv_response);
		}
		return srv_response;
	}

	void helo(boost::asio::ip::tcp::socket& socket, std::string const& server) {
		using namespace boost::asio;
		write(socket, buffer("EHLO " + server + newline));
		auto response = check_response("250", socket);

		std::vector<std::string> extensions;

		for (size_t prev = 0, curr = response.find(newline);
			 curr != std::string::npos;
			 prev = curr + 2, curr = response.find(newline, prev)) 
		{
			boost::string_ref const e(&response[0] + prev, curr - prev);
			if (!e.starts_with("250")) {
				std::cerr << e;
			}
			else {
				extensions.emplace_back(e);
			}
		}
		extensions.erase(extensions.begin());	//the first reponse is not an extension
	}

	template<typename socket_t>
	void handle_message(socket_t& socket, mail::Message const& message) {
		using namespace boost::asio;

		std::string const uuid = boost::uuids::to_string(boost::uuids::random_generator()());

		write(socket, buffer("MIME-version: 1.0" + newline));
		write(socket, buffer("Content-Type: multipart/mixed; boundary=\"" + uuid + "\"" + newline));
		write(socket, buffer("From: " + message.from + newline));
		for (auto const& e : message.to) {
			write(socket, buffer("To: " + e + newline));
		}
		write(socket, buffer("SUBJECT: " + message.subject + newline));
		write(socket, buffer(newline + "body: " + message.body + newline));
		write(socket, buffer("--" + uuid + newline));
		write(socket, buffer("Content-Type: text/plain" + newline));
		write(socket, buffer(newline + message.body + newline));
		for (auto& a : message.attachments) {
			write(socket, buffer(newline + "--" + uuid + newline));
			write(socket, buffer("Content-Type: application/octet-stream" + newline));
			write(socket, buffer("Content-Disposition: attachment; filename=\"foo.txt\"" + newline));
			write(socket, buffer("Content-transfer-encoding: base64" + newline));
			auto stream = a.ContentStream();
			auto content = std::string(
				std::istreambuf_iterator<char>(*stream),
				std::istreambuf_iterator<char>());
			write(socket, buffer(newline + base64::encode(content) + newline));

		}
		write(socket, buffer(newline + "--" + uuid + "--" + newline));
	}

	template<typename socket_t>
	void handle_smtp(socket_t& socket, mail::Message const& message) {
		using namespace boost::asio;
		write(socket, buffer("MAIL FROM:<" + message.from + ">" + newline));
		std::cout << check_response("250", socket) << std::endl;

		for (auto const& e : message.to) {
			write(socket, buffer("RCPT TO:<" + e + ">" + newline));
			std::cout << check_response("250", socket) << std::endl;
		}

		write(socket, buffer("DATA" + newline));
		std::cout << check_response("354", socket) << std::endl;

		handle_message(socket, message);
		write(socket, buffer(newline + "." + newline));
		std::cout << check_response("250", socket) << std::endl;

		write(socket, buffer("QUIT" + newline));
		std::cout << check_response("221", socket) << std::endl;
	}

	template<typename socket_t>
	void handle_auth(socket_t& socket, mail::Credential const& cred) {
		using namespace boost::asio;

		write(socket, buffer("AUTH LOGIN" + newline));
		std::cout << check_response("334", socket) << std::endl;

		write(socket, buffer(base64::encode(cred.user) + newline));
		std::cout << check_response("334", socket) << std::endl;

		write(socket, buffer(base64::encode(cred.password) + newline));
		std::cout << check_response("235", socket) << std::endl;
	}

	template<typename socket_t>
	void handle_starttls(socket_t& socket, mail::Credential const& cred, mail::Message const& message) {
		using namespace boost::asio;
		write(socket, buffer("STARTTLS" + newline));
		std::cout << check_response("220", socket) << std::endl;

		ssl::context ctx(ssl::context::tlsv12_client);
		ctx.set_verify_mode(ssl::verify_none);
		ssl::stream<ip::tcp::socket&> ssl_sock(socket, ctx);
		ssl_sock.handshake(ssl::stream_base::client);

		handle_auth(ssl_sock, cred);
		handle_smtp(ssl_sock, message);
	}

	//tls_connection()
}

namespace mail {
	//SmtpClient::Impl::Impl(boost::asio::io_service& io) :
	//	io_service_(io)
	//{}
	SmtpClient::Impl::Impl(std::string const& server, uint16_t port) :
		server_(server), port_(port)
	{}

	Credential const& SmtpClient::Impl::credentials() const {
		return credentials_;
	}

	void SmtpClient::Impl::credentials(Credential const& cred) {
		credentials_ = cred;
	}

	uint16_t SmtpClient::Impl::port() const {
		return port_;
	}

	//void SmtpClient::Impl::connect(std::string const& host, uint16_t port) {
	//	using namespace boost::asio;
	//	ip::tcp::socket s;
	//}

	void SmtpClient::Impl::send(Message const& message) const {
		using namespace boost::asio;

		io_service io_service;
		ip::tcp::socket socket(io_service);
		ip::tcp::resolver resolver(io_service);
		ip::tcp::resolver::query query(server_, std::to_string(port_));
		ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		connect(socket, endpoint_iterator);
		std::cout << check_response("220", socket) << std::endl;
		helo(socket, server_);

		handle_smtp(socket, message);
	}

	void SmtpClient::Impl::send(Message const& message, Credential const& cred) const {
		using namespace boost::asio;

		io_service io_service;
		ip::tcp::socket socket(io_service);
		ip::tcp::resolver resolver(io_service);
		ip::tcp::resolver::query query(server_, std::to_string(port_));
		ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		connect(socket, endpoint_iterator);
		std::cout << check_response("220", socket) << std::endl;
		helo(socket, server_);

		handle_starttls(socket, cred, message);
	}
}