#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace ct_string {

namespace detail {
struct ct_string_base {};
struct ct_string_holder_base {};

template <typename T>
constexpr bool is_ct_string_holder =
    std::is_base_of_v<detail::ct_string_holder_base, T>;

template <typename T>
concept ct_string_holder_type = is_ct_string_holder<T>;

} // namespace detail

template <typename T>
constexpr bool is_ct_string = std::is_base_of_v<detail::ct_string_base, T>;

template <typename T>
concept ct_string_type = is_ct_string<T>;

template <typename Current, typename T>
concept DifferentTypeString =
    !std::is_same_v<Current, T> && !std::is_base_of_v<T, Current>;

// N does not count null-terminator
template <typename Char, std::size_t N>
struct basic_string_holder : detail::ct_string_holder_base {

  using view_t = std::basic_string_view<Char>;
  using char_t = Char;
  static constexpr auto length_v = N;

  constexpr basic_string_holder(view_t str) {
    std::copy(str.begin(), str.end(), &m_chars[0]);
    m_chars[N] = Char{'\0'};
  }

  constexpr basic_string_holder(const Char (&str)[N + 1]) {
    std::copy_n(str, N, &m_chars[0]);
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

template <typename Char, std::size_t N>
basic_string_holder(const Char (&)[N]) -> basic_string_holder<Char, N - 1>;

template <detail::ct_string_holder_type auto StringHolder>
struct basic_ct_string : detail::ct_string_base {

  using string_holder_t = decltype(StringHolder);
  using char_t = typename string_holder_t::char_t;
  using view_t = std::basic_string_view<char_t>;
  using string_t = std::basic_string<char_t>;

  constexpr bool operator<=>(const basic_ct_string &) const = default;

  template <auto Other>
  constexpr auto operator+(const basic_ct_string<Other> &) const {
    constexpr auto combined = StringHolder + Other;
    return basic_ct_string<combined>();
  }

  template <detail::ct_string_holder_type auto OtherStringHolder>
  constexpr bool
  operator==(const basic_ct_string<OtherStringHolder> &other) const {
    return str_view() == other.str_view();
  }

  template <std::size_t U>
  constexpr bool operator==(const char_t (&other)[U]) const {
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
  constexpr bool operator!=(const char_t (&other)[U]) const {
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
    return StringHolder.length_v;
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

namespace literals {
template <basic_string_holder StringHolder> consteval auto operator""_cts() {
  return basic_ct_string<StringHolder>{};
}
} // namespace literals

} // namespace ct_string
