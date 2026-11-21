/* cbcxx.h -- what wlink's C relied on that C++ asks to be said, force-included into every file of wlink (CMakeLists.txt).
 *
 * C treats an enumeration as an int: `flags |= ORL_SEC_FLAG_READ`, `kind++` and `flags & ~MASK` keep the enumeration's
 * type there, and only an int is stored back. C++ gives an enumeration no arithmetic of its own; these operators give
 * wlink's enumerations C's: the value computed on the underlying integer, kept as the enumeration. (In C the file
 * includes nothing.) */
#ifndef CB_WLINK_CXX_H
#define CB_WLINK_CXX_H
#ifdef __cplusplus

#include <type_traits>

namespace cb_wlink { struct Link; }
using cb_wlink::Link;   // (the state of one link: every function of the linker that reads or changes it is given it, `Link& link`)
#include "build/x86link/link/callback.hpp"   // (FunctionRef, bind_link: a routine handed to a walker)

namespace cb_wlink_cxx {
template <class E>
using if_enum = std::enable_if_t<std::is_enum_v<E>, int>;
template <class E>
constexpr std::underlying_type_t<E> raw(E e) { return static_cast<std::underlying_type_t<E>>(e); }
}  // namespace cb_wlink_cxx

template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator|(E a, E b) { return static_cast<E>(cb_wlink_cxx::raw(a) | cb_wlink_cxx::raw(b)); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator&(E a, E b) { return static_cast<E>(cb_wlink_cxx::raw(a) & cb_wlink_cxx::raw(b)); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator^(E a, E b) { return static_cast<E>(cb_wlink_cxx::raw(a) ^ cb_wlink_cxx::raw(b)); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator~(E a) { return static_cast<E>(~cb_wlink_cxx::raw(a)); }

template <class E, class I, cb_wlink_cxx::if_enum<E> = 0, std::enable_if_t<std::is_integral_v<I>, int> = 0>
constexpr E& operator|=(E& a, I b) { return a = static_cast<E>(cb_wlink_cxx::raw(a) | b); }
template <class E, class I, cb_wlink_cxx::if_enum<E> = 0, std::enable_if_t<std::is_integral_v<I>, int> = 0>
constexpr E& operator&=(E& a, I b) { return a = static_cast<E>(cb_wlink_cxx::raw(a) & b); }
template <class E, class I, cb_wlink_cxx::if_enum<E> = 0, std::enable_if_t<std::is_integral_v<I>, int> = 0>
constexpr E& operator^=(E& a, I b) { return a = static_cast<E>(cb_wlink_cxx::raw(a) ^ b); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E& operator|=(E& a, E b) { return a = a | b; }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E& operator&=(E& a, E b) { return a = a & b; }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E& operator^=(E& a, E b) { return a = a ^ b; }
template <class E, class I, cb_wlink_cxx::if_enum<E> = 0, std::enable_if_t<std::is_integral_v<I>, int> = 0>
constexpr E& operator+=(E& a, I b) { return a = static_cast<E>(cb_wlink_cxx::raw(a) + b); }
template <class E, class I, cb_wlink_cxx::if_enum<E> = 0, std::enable_if_t<std::is_integral_v<I>, int> = 0>
constexpr E& operator-=(E& a, I b) { return a = static_cast<E>(cb_wlink_cxx::raw(a) - b); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E& operator++(E& a) { return a = static_cast<E>(cb_wlink_cxx::raw(a) + 1); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator++(E& a, int) { E was = a; ++a; return was; }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E& operator--(E& a) { return a = static_cast<E>(cb_wlink_cxx::raw(a) - 1); }
template <class E, cb_wlink_cxx::if_enum<E> = 0>
constexpr E operator--(E& a, int) { E was = a; --a; return was; }

#endif
#endif
