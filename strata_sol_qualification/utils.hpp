//
// Created by dym on 06.05.24.
//

#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

#include <QMetaEnum>
#include <QtGlobal>

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>


namespace ssq {


auto map(auto x, auto in_min, auto in_max, auto out_min, auto out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};


template<bool Cond_, typename Then, typename Else>
[[nodiscard]] inline constexpr decltype(auto) eval_if(Then&& then_, Else&& else_) {
  if constexpr (Cond_) {
    if constexpr (std::is_invocable_v<Then>) {
      return std::invoke(std::forward<Then>(then_));
    } else {
      return std::forward<Then>(then_);
    }
  } else {
    if constexpr (std::is_invocable_v<Else>) {
      return std::invoke(std::forward<Else>(else_));
    } else {
      return std::forward<Else>(else_);
    }
  }
}


template<typename T>
[[nodiscard]] int enum_count() {
  using U                = std::remove_cvref_t<T>;
  static int const value = QMetaEnum::fromType<U>().keyCount();
  return value;
}


template<typename T>
[[nodiscard]] const char* enum_to_string(T const in) {
  assert(std::in_range<int>(static_cast<std::underlying_type_t<T>>(in)));
  return QMetaEnum::fromType<T>().valueToKey(static_cast<int>(in));
}


template<typename Enum, typename T>
Enum enum_from_string(T&& in) {
#ifndef NDEBUG
  bool ok = false;
#endif

  Enum const e = static_cast<Enum>(QMetaEnum::fromType<Enum>().keyToValue(
    eval_if<std::is_same_v<std::decay_t<T>, char const*>>(
      in,
      [&in] { return qPrintable(std::forward<T>(in)); })
  // clang-format off
#ifndef NDEBUG
    , &ok
#endif
    // clang-format on
    ));
#ifndef NDEBUG
  assert(ok);
#endif
  return e;
}


template<typename T>
const char* enum_name() {
  using U                 = std::remove_cvref_t<T>;
  static auto const value = QMetaEnum::fromType<U>().enumName();
  return value;
}


template<typename T>
auto enum_names() {
  namespace rv = ranges::views;
  return rv::iota(0, enum_count<T>())
    | rv::transform([](auto i) {
        return enum_to_string(static_cast<T>(i));
      });
}


}// namespace ssq