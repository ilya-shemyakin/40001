#include "scopeGuard.hpp"

vladimirova::iofmtguard::iofmtguard(std::basic_ios<char>& stream) :
    stream_(stream),
    width_(stream.width()),
    fill_(stream.fill()),
    precision_(stream.precision()),
    flags_(stream.flags())
{
}

vladimirova::iofmtguard::~iofmtguard()
{
    stream_.width(width_);
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.flags(flags_);
}
