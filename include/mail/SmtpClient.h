#pragma once

#include <memory>
#include <string>

#ifdef MailLib_EXPORTS
#define EXPORT_MAIL_LIB __declspec( dllexport )
#else 
#define EXPORT_MAIL_LIB __declspec(dllimport)
#endif

namespace mail {
	
	class Message;
	class Credential;

	/**
	 * Allows applications to send e-mail by using the Simple Mail Transfer Protocol (SMTP).
	 */
	class EXPORT_MAIL_LIB SmtpClient {
	public:

		/**
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

		Credential const& credentials() const;

		void credentials(Credential const& cred);

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