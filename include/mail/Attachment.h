#pragma once

#include <memory>
#include <string>

namespace mail {

	class Attachment {
	public:
		explicit Attachment(std::string const& file_path);

		Attachment(Attachment const& src);

		~Attachment();

		std::unique_ptr<std::istream> ContentStream() const;

	private:

		class Impl;
		std::unique_ptr<Impl> impl_;
	};
}
