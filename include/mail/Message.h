#pragma once

#include <mail/Address.h>
#include <mail/Attachment.h>
#include <string>
#include <vector>

namespace mail {
	class Message {
	public:
		Address from;
		std::vector<Address> to;
		std::string subject;
		std::string body;
		std::vector<Attachment> attachments;
	};
}