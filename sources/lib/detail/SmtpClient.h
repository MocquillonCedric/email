#pragma once

#include <mail/SmtpClient.h>
#include <mail/Credentials.h>

namespace boost {
	namespace asio {
		class io_service;
	}
}

namespace mail {
	class SmtpClient::Impl {
	public:

//		Impl(boost::asio::io_service& io);

		Impl(std::string const& server, uint16_t port);

		Credential const& credentials() const;

		void credentials(Credential const& cred);

		uint16_t port() const;

//		void connect(std::string const& host, uint16_t port);

		void send(Message const& m) const;

		void send(Message const& m, Credential const& cred) const;

	private:
//		boost::asio::io_service& io_service_;
		std::string const server_;
		uint16_t const port_;
		Credential credentials_;
	};
}