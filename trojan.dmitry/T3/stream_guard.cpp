#include "stream_guard.hpp"

trojan::StreamGuard::StreamGuard(std::basic_ios<char>& stream):
    stream_(stream),
    fill_(stream.fill()),
    precision_(stream.precision()),
    width_(stream.width()),
    flags_(stream.flags())
{}
trojan::StreamGuard::~StreamGuard()
{
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.width(width_);
    stream_.flags(flags_);
}
