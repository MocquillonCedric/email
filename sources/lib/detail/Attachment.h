#pragma once

#include <mail/Attachment.h>
#include <fstream>

namespace mail {
	class Attachment::Impl {
	public:
		Impl(std::string const& file_path);

		std::unique_ptr<std::istream> ContentStream() const;

	private:
		//std::ifstream stream_;
		std::string path_;
	};
}