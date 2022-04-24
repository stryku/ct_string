#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#ifndef CT_STRING_MAX_STRING_LENGTH
#define CT_STRING_MAX_STRING_LENGTH 1024
#endif

namespace ct_string {

namespace detail {
struct ct_string_base {};
} // namespace detail

template <typename T>
constexpr bool is_ct_string = std::is_base_of_v<detail::ct_string_base, T>;

template <typename T>
concept ct_string_type = is_ct_string<T>;

template <typename Current, typename T>
concept DifferentTypeString =
    !std::is_same_v<Current, T> && !std::is_base_of_v<T, Current>;

// N does not count null-terminator
template <typename Char, std::size_t N> struct basic_string_holder {

  using view_t = std::basic_string_view<Char>;
  using string_t = std::basic_string<Char>;
  using char_t = Char;
  static constexpr auto length_v = N;

  constexpr basic_string_holder(view_t str) {
    std::copy(str.begin(), str.end(), &m_chars[0]);
    m_chars[N] = Char{'\0'};
  }

  template <std::size_t U>
  constexpr auto operator+(const basic_string_holder<char_t, U> &other) const {
    std::array<char_t, N + U> chars{};
    const auto self = this->str_view();
    const auto other_view = other.str_view();

    std::copy(self.begin(), self.end(), chars.data());
    std::copy(other_view.begin(), other_view.end(),
              chars.data() + self.length());
    return basic_string_holder<char_t, N + U>{
        view_t{chars.data(), chars.size()}};
  }

  constexpr view_t str_view() const {
    return view_t{m_chars, N};
  }

  Char m_chars[N + 1]{};
};

template <auto StringHolder> struct basic_ct_string2 : detail::ct_string_base {

  using string_holder_t = decltype(StringHolder);
  using char_t = typename string_holder_t::char_t;
  using view_t = std::basic_string_view<char_t>;
  using string_t = std::basic_string<char_t>;

  constexpr bool operator<=>(const basic_ct_string2 &) const = default;

  template <auto Other>
  constexpr auto operator+(const basic_ct_string2<Other> &) const {
    constexpr auto combined = StringHolder + Other;
    return basic_ct_string2<combined>();
  }

  template <std::size_t U>
  constexpr bool operator==(const char_t (&other)[U]) const {
    // Assume string literal. -1 for null-terminator
    return view_t{other, U - 1} == str_view();
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator==(const Other &other) const {
    return view_t{other} == str_view();
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator!=(const Other &other) const {
    return !(view_t{other} == str_view());
  }

  template <std::size_t U>
  constexpr bool operator!=(const char_t (&other)[U]) const {
    // Assume string literal. -1 for null-terminator
    return !(*this == other);
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator<(const Other &other) const {
    return view_t{other} < str_view();
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator<=(const Other &other) const {
    return view_t{other} <= str_view();
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator>(const Other &other) const {
    return !(view_t{other} <= str_view());
  }

  template <DifferentTypeString<basic_ct_string2> Other>
  constexpr bool operator>=(const Other &other) const {
    return !(view_t{other} < str_view());
  }

  constexpr std::size_t length() const {
    return str_view().length();
  }

  constexpr view_t str_view() const {
    return StringHolder.str_view();
  }

  constexpr string_t str() const {
    return string_t{str_view()};
  }

  constexpr const char_t *c_str() const {
    return str_view().data();
  }
};

template <typename Char, std::size_t N>
struct basic_ct_string : detail::ct_string_base {

  using view_t = std::basic_string_view<Char>;
  using string_t = std::basic_string<Char>;

  constexpr basic_ct_string(view_t str) {
    std::copy(str.begin(), str.end(), &m_chars[0]);
    m_chars[N] = Char{'\0'};
  }

  constexpr bool operator<=>(const basic_ct_string &) const = default;

  template <std::size_t U>
  constexpr auto operator+(const basic_ct_string<Char, U> &other) const {
    std::array<Char, N + U> chars{};
    const auto self = this->str_view();
    const auto other_view = other.str_view();

    std::copy(self.begin(), self.end(), chars.data());
    std::copy(other_view.begin(), other_view.end(),
              chars.data() + self.length());
    return basic_ct_string<Char, N + U>{view_t{chars.data(), chars.size()}};
  }

  template <std::size_t U>
  constexpr bool operator==(const Char (&other)[U]) const {
    // Assume string literal. -1 for null-terminator
    return view_t{other, U - 1} == str_view();
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator==(const Other &other) const {
    return view_t{other} == str_view();
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator!=(const Other &other) const {
    return !(view_t{other} == str_view());
  }

  template <std::size_t U>
  constexpr bool operator!=(const Char (&other)[U]) const {
    // Assume string literal. -1 for null-terminator
    return !(*this == other);
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator<(const Other &other) const {
    return view_t{other} < str_view();
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator<=(const Other &other) const {
    return view_t{other} <= str_view();
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator>(const Other &other) const {
    return !(view_t{other} <= str_view());
  }

  template <DifferentTypeString<basic_ct_string> Other>
  constexpr bool operator>=(const Other &other) const {
    return !(view_t{other} < str_view());
  }

  constexpr std::size_t length() const {
    return N;
  }

  constexpr view_t str_view() const {
    return view_t{m_chars, N};
  }

  constexpr string_t str() const {
    return string_t{m_chars, N};
  }

  constexpr const Char *c_str() const {
    return m_chars;
  }

  Char m_chars[N + 1]{};
};

template <typename Char, std::size_t N>
basic_ct_string(const Char (&)[N])
    -> basic_ct_string<Char,
                       N - 1>; // Assume string literal. -1 for null-terminator

template <std::size_t N> struct ct_string : basic_ct_string<char, N> {
  consteval ct_string(std::string_view str) : basic_ct_string<char, N>{str} {}
};

template <std::size_t N>
ct_string(const char (&)[N])
    -> ct_string<N - 1>; // Assume string literal. -1 for null-terminator

template <std::size_t N> struct ct_wstring : basic_ct_string<wchar_t, N> {
  consteval ct_wstring(std::wstring_view str)
      : basic_ct_string<wchar_t, N>{str} {}
};

template <std::size_t N>
ct_wstring(const wchar_t (&)[N])
    -> ct_wstring<N - 1>; // Assume string literal. -1 for null-terminator

namespace detail {

template <typename Char> struct oversized_ct_string {
  using value_type = Char;

  constexpr oversized_ct_string(const Char *str) {
    Char *out = m_str;
    Char *end = m_str + CT_STRING_MAX_STRING_LENGTH - 1; // without the '\0'

    while (*str && out != end) {
      *out++ = *str++;
    }

    *out = '\0';

    m_length = out - m_str;
  }

  constexpr bool operator==(const oversized_ct_string &) const = default;

  constexpr std::basic_string_view<Char> str_view() const {
    return std::basic_string_view<Char>{m_str, m_length};
  }

  Char m_str[CT_STRING_MAX_STRING_LENGTH]{};
  std::size_t m_length = 0;
};

} // namespace detail

template <typename Char>
using ct_string_acceptor = detail::oversized_ct_string<Char>;

namespace literals {
template <detail::oversized_ct_string<char> StringHolder>
consteval auto operator""_cts() {
  return basic_ct_string<char, StringHolder.m_length>{StringHolder.m_str};
}

template <detail::oversized_ct_string<wchar_t> StringHolder>
consteval auto operator""_cts() {
  return basic_ct_string<wchar_t, StringHolder.m_length>{StringHolder.m_str};
}

template <detail::oversized_ct_string<char> OversizedString>
consteval auto operator""_cts2() {
  constexpr basic_string_holder<char, OversizedString.m_length> str{
      OversizedString.m_str};
  return basic_ct_string2<str>{};
}
} // namespace literals

} // namespace ct_string
