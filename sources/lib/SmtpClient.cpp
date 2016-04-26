#include <mail/SmtpClient.h>
//#include <mail/Message.h>
#include "detail/SmtpClient.h"

namespace mail {

	SmtpClient::SmtpClient(std::string const& server, uint16_t port) :
		impl_(std::make_unique<SmtpClient::Impl>(server, port))
	{}

	SmtpClient::~SmtpClient() = default;

	Credential const& SmtpClient::credentials() const {
		return impl_->credentials();
	}

	void SmtpClient::credentials(Credential const& cred) {
		impl_->credentials(cred);
	}

	uint16_t SmtpClient::port() const {
		return impl_->port();
	}

/*	void SmtpClient::connect(std::string const& host, uint16_t port) {
		return impl_->connect(host, port);
	}

	void SmtpClient::helo(std::string const& name = "");

	void SmtpClient::ehlo(std::string const& name = "");

	void SmtpClient::ehlo_or_helo_if_needed();
*/
	void SmtpClient::send(Message const& m) const {
		return impl_->send(m);
	}

	void SmtpClient::send(Message const& m, Credential const& cred) const {
		return impl_->send(m, cred);
	}
}