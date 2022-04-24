#pragma once

#include <algorithm>
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

template <typename Current, typename T>
concept DifferentTypeString =
    !std::is_same_v<Current, T> && !std::is_base_of_v<T, Current>;

// N does not count null-terminator
template <typename Char, std::size_t N>
struct basic_ct_string : detail::ct_string_base {

  using view_t = std::basic_string_view<Char>;
  using string_t = std::basic_string<Char>;

  consteval basic_ct_string(view_t str) {
    std::copy(str.begin(), str.end(), &m_chars[0]);
    m_chars[N] = Char{'\0'};
  }

  constexpr bool operator<=>(const basic_ct_string &) const = default;

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

template <typename Char> struct huge_string_holder {
  using value_type = Char;

  constexpr huge_string_holder(const Char *str) {
    Char *out = m_str;
    Char *end = m_str + CT_STRING_MAX_STRING_LENGTH - 1; // without the '\0'

    while (*str && out != end) {
      *out++ = *str++;
    }

    *out = '\0';

    m_length = out - m_str;
  }

  constexpr bool operator==(const huge_string_holder &) const = default;

  Char m_str[CT_STRING_MAX_STRING_LENGTH]{};
  std::size_t m_length = 0;
};

} // namespace detail

namespace literals {
template <detail::huge_string_holder<char> StringHolder>
consteval auto operator""_cts() {
  return basic_ct_string<char, StringHolder.m_length>{StringHolder.m_str};
}

template <detail::huge_string_holder<wchar_t> StringHolder>
consteval auto operator""_cts() {
  return basic_ct_string<wchar_t, StringHolder.m_length>{StringHolder.m_str};
}
} // namespace literals

} // namespace ct_string
