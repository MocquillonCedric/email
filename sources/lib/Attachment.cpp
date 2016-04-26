#include <mail/Attachment.h>
#include "detail/Attachment.h"

namespace mail {
	Attachment::Attachment(std::string const& file_path) :
		impl_(std::make_unique<Attachment::Impl>(file_path))
	{}

	Attachment::Attachment(Attachment const& src) :
		impl_(std::make_unique<Attachment::Impl>(*src.impl_)) 
	{}

	Attachment::~Attachment() = default;

	std::unique_ptr<std::istream> Attachment::ContentStream() const {
		return impl_->ContentStream();
	}
}