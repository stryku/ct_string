#include <ct_string/ct_string.hpp>

#include <gtest/gtest.h>
#include <ostream>

namespace ct_string {
// template <typename Char, std::size_t N>
// std::ostream &operator<<(std::ostream &out,
//                          const basic_ct_string<Char, N> &str) {
//   return out << str.str_view();
// }
} // namespace ct_string

namespace ct_string::tests {

using namespace std::literals;
using namespace ::ct_string::literals;

TEST(CtString, Constructor) {
  { constexpr auto cts = "foo"_cts; }
  { constexpr auto cts = L"foo"_cts; }
}

TEST(CtString, Comparison) {

  constexpr auto foo = "foo"_cts;
  constexpr auto foo2 = "foo"_cts;
  constexpr auto bar = "bar"_cts;
  constexpr auto long_baz = "long baz"_cts;

  static_assert(foo == "foo");
  static_assert("foo" == foo);
  static_assert(foo == "foo"sv);
  static_assert("foo"sv == foo);
  EXPECT_EQ(foo, "foo"s);
  EXPECT_EQ("foo"s, foo);
  static_assert(foo == foo);
  static_assert(foo == foo2);
  static_assert(foo2 == foo);

  static_assert(foo != "bar");
  static_assert("bar" != foo);
  static_assert(foo != "bar"sv);
  static_assert("bar"sv != foo);
  EXPECT_NE(foo, "bar"s);
  EXPECT_NE("bar"s, foo);
  static_assert(foo != bar);
  static_assert(bar != foo);

  static_assert(foo != "long baz");
  static_assert("long baz" != foo);
  static_assert(foo != "long baz"sv);
  static_assert("long baz"sv != foo);
  EXPECT_NE(foo, "long baz"s);
  EXPECT_NE("long baz"s, foo);
  static_assert(foo != long_baz);
  static_assert(long_baz != foo);
}

} // namespace ct_string::tests
