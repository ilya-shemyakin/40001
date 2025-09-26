#ifndef FORMAT_GUARD_HPP
#define FORMAT_GUARD_HPP

#include <ios>

namespace andriuschin
{
  class FormatGuard
  {
  public:
    FormatGuard(std::basic_ios< char >& stream);
    ~FormatGuard();
  private:
    std::basic_ios< char >& stream_;
    char fill_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
  };
}

#endif
