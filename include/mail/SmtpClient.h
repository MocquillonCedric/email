#pragma once

#include <memory>
#include <string>

namespace mail {
	
	class Message;
	class Credential;

	/**
	 * Allows applications to send e-mail by using the Simple Mail Transfer Protocol (SMTP).
	 */
	class SmtpClient {
	public:

/*		SmtpClient(boost::asio::io_service& io);

		SmtpClient(boost::asio::io_service& io, std::string const& host, uint16_t port = 25);

		connect();
		connect(std::string const& host, uint16_t port = 25);
*/		/**
		 * Initializes a new instance of the SmtpClient class that sends e-mail
		 * by using the specified SMTP server and port.
		 *
		 * @param server: A std::string that contains the name or IP address of
		 * the host used for SMTP transactions.
		 * @param port: An uint16_t that contains the port to be used on host.
		 * The default value is 25.
		 */
		SmtpClient(std::string const& server, uint16_t port = 25);

		~SmtpClient();

		//client_certificates
		//credentials
		//delivery_format
		//delivery_method
		//enable_ssl
		//host
		//pickup_directory_location
		//(/) port
		//service_point <=> socket
		//target_name
		//timeout
		//user_default_credentials
		Credential const& credentials() const;

		void credentials(Credential const& cred);

//		void connect(std::string const& host = "localhost", uint16_t port = 25);

/*		void helo(std::string const& name = "");

		void ehlo(std::string const& name = "");

		void ehlo_or_helo_if_needed();
*/
		/**
		 * Gets the port used for SMTP transactions.
		 *
		 * @return An uint16_t that contains the port number on the SMTP host.
		 */
		uint16_t port() const;

		/**
		 * Sends the specified message to an SMTP server for delivery.
		 *
		 * @param message: A mail::Message that contains the message to send.
		 */
		void send(Message const& message) const;

		void send(Message const& m, Credential const& cred) const;

	private:

		class Impl;
		std::unique_ptr<Impl> impl_;
	};
}