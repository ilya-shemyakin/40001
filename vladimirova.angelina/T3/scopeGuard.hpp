#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP

#include <iostream>
#include <iomanip>

struct iofmtguard
{
	std::ostream& s;
	std::streamsize precision;
	std::ios::fmtflags fmt;

	explicit iofmtguard(std::ostream& s_);
	~iofmtguard();
};

#endif
