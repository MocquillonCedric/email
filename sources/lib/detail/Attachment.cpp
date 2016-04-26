#include "Attachment.h"

namespace mail {
	Attachment::Impl::Impl(std::string const& file_path) :
		path_(file_path)
		//stream_(file_path)
	{}

	std::unique_ptr<std::istream> Attachment::Impl::ContentStream() const {
		return std::make_unique<std::ifstream>(path_);
	}
}