#include "stream_guard.hpp"

shapes::StreamGuard::StreamGuard(std::basic_ios<char>& s) :
    stream_(s),
    precision_(s.precision()),
    flags_(s.flags())
{
}

shapes::StreamGuard::~StreamGuard() {
    stream_.precision(precision_);
    stream_.flags(flags_);
}
