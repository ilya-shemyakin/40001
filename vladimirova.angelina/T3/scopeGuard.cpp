#include "scopeGuard.hpp"

iofmtguard::iofmtguard(std::ostream& s_) :
	s(s_),
	precision(s.precision()),
	fmt(s.flags())
{
}

iofmtguard::~iofmtguard()
{
	s.precision(precision);
	s.flags(fmt);
}
