#include <ct_string/ct_string.hpp>

#include <gtest/gtest.h>
#include <ostream>

namespace ct_string {
template <typename Char, std::size_t N>
std::ostream &operator<<(std::ostream &out,
                         const basic_ct_string<Char, N> &str) {
  return out << str.str_view();
}
} // namespace ct_string

namespace ct_string::tests {

using namespace std::literals;
using namespace ::ct_string::literals;

TEST(CtString, Constructor) {
  { constexpr ct_string cts{"foo"}; }
  { constexpr ct_wstring cts{L"foo"}; }
  { constexpr auto cts = "foo"_cts; }
  { constexpr auto cts = L"foo"_cts; }
  {
    constexpr const char foo[] = "foo";
    constexpr ct_string cts23{foo};
  }
}

TEST(CtString, Comparison) {

  constexpr ct_string foo{"foo"};
  constexpr ct_string foo2{"foo"};
  constexpr ct_string bar{"bar"};
  constexpr ct_string long_baz{"long baz"};

  EXPECT_EQ(foo, "foo");
  EXPECT_EQ("foo", foo);
  EXPECT_EQ(foo, "foo"s);
  EXPECT_EQ("foo"s, foo);
  EXPECT_EQ(foo, "foo"sv);
  EXPECT_EQ("foo"sv, foo);
  EXPECT_EQ(foo, foo);
  EXPECT_EQ(foo, foo2);
  EXPECT_EQ(foo2, foo);
  static_assert(foo == foo);
  static_assert(foo == foo2);
  static_assert(foo2 == foo);

  EXPECT_NE(foo, "bar");
  EXPECT_NE("bar", foo);
  EXPECT_NE(foo, "bar"s);
  EXPECT_NE("bar"s, foo);
  EXPECT_NE(foo, "bar"sv);
  EXPECT_NE("bar"sv, foo);
  EXPECT_NE(foo, bar);
  EXPECT_NE(bar, foo);
  static_assert(foo != bar);
  static_assert(bar != foo);

  EXPECT_NE(foo, "long baz");
  EXPECT_NE("long baz", foo);
  EXPECT_NE(foo, "long baz"s);
  EXPECT_NE("long baz"s, foo);
  EXPECT_NE(foo, "long baz"sv);
  EXPECT_NE("long baz"sv, foo);
  EXPECT_NE(foo, long_baz);
  EXPECT_NE(long_baz, foo);
  static_assert(foo != long_baz);
  static_assert(long_baz != foo);
}

} // namespace ct_string::tests
