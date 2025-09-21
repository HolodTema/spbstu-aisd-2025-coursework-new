#include "ScopeGuard.hpp"

ScopeGuard::ScopeGuard(std::basic_ios<wchar_t> &stream):
	stream_(stream),
	flags_(stream.flags()),
	width_(stream.width()),
	precision_(stream.precision()),
	fill_(stream.fill())
{ }

ScopeGuard::~ScopeGuard() {
	stream_.flags(flags_);
	stream_.width(width_);
	stream_.precision(precision_);
	stream_.fill(fill_);
}
