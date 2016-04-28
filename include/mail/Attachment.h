#pragma once

#include <memory>
#include <string>

#ifdef MailLib_EXPORTS
#define EXPORT_MAIL_LIB __declspec( dllexport )
#else 
#define EXPORT_MAIL_LIB __declspec(dllimport)
#endif

namespace mail {

	class EXPORT_MAIL_LIB Attachment {
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
