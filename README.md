# Compile-time string

Simple C++20 compile-time string library.

# Known supported compilers
* gcc-11

# Docs

## Creation
```cpp
using namespace ct_string::literals;
constexpr auto foo = "foo"_cts;
```

## Concatenation
```cpp
using namespace ct_string::literals;
constexpr auto foo_bar = "foo"_cts + " "_cts + "bar";
```

## Joining
```cpp
using namespace ct_string::literals;
constexpr auto foo_bar = " "_cts.join("foo"_cts, "bar");
```
