#ifndef LITERALS_HPP
#define LITERALS_HPP

#include <cstddef>
#include <iosfwd>
#include <utility>
#include <string>

namespace andriuschin
{
  template< class T, size_t Id = 0 >
  class Literal;
  template< class T, size_t Id >
  extern std::ostream& operator<<(std::ostream& out, const Literal< T, Id >&& value);
  template< class T, size_t Id >
  extern std::istream& operator>>(std::istream& in, Literal< T, Id >&& value);

  template< class T, size_t Id >
  class Literal
  {
  public:
    using value_type = T;
    static constexpr size_t id = Id;
    Literal(const value_type& value):
      data_(value),
      link_(data_)
    {}
    Literal(value_type& link):
      data_(),
      link_(link)
    {}
    Literal(const Literal&) = delete;

    friend std::ostream& operator<<< T, Id >(std::ostream& out, const Literal&& value);
    friend std::istream& operator>>< T, Id >(std::istream& in, Literal&& value);
  private:
    value_type data_;
    value_type& link_;
  };
  using CharLiteral = Literal< char >;
  using RationalLiteral = Literal< std::pair< long long, unsigned long long > >;
  using StringLiteral = Literal< std::string >;
}

#endif
