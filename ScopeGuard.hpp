#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

#include <iostream>

class ScopeGuard {
public:
	explicit ScopeGuard(std::basic_ios<wchar_t> &stream);

	ScopeGuard(const ScopeGuard &other) = delete;

	ScopeGuard(ScopeGuard &&other) = delete;

	~ScopeGuard();

private:
	std::basic_ios<wchar_t> &stream_;
	std::basic_ios<wchar_t>::fmtflags flags_;
	std::streamsize width_;
	std::streamsize precision_;
	char fill_;
};

#endif
