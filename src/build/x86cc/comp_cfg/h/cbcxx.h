/* cbcxx.h -- what the compilers' C relied on that C++ asks to be said, force-included into every file of cb_wcc_i86 and
 * cb_wcc_386 (CMakeLists.txt).
 *
 * C treats an enumeration as an int: `flags |= TYPE_CONST`, `kind++` and `flags & ~MASK` keep the enumeration's type there,
 * and only an int is stored back. C++ gives an enumeration no arithmetic of its own; these operators give the compilers'
 * enumerations C's: the value computed on the underlying integer, kept as the enumeration. */
#ifndef CB_WCC_CXX_H
#define CB_WCC_CXX_H
#ifdef __cplusplus

#include <type_traits>

namespace {   // (internal linkage: each target's objects keep the instantiations of their own enumerations; no merging between wcc and wcc386)
namespace cb_wcc_cxx {
template <class E>
using if_enum = std::enable_if_t<std::is_enum_v<E>, int>;
template <class E>
constexpr std::underlying_type_t<E> raw(E e) { return static_cast<std::underlying_type_t<E>>(e); }
}  // namespace cb_wcc_cxx

template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator|(E a, E b) { return static_cast<E>(cb_wcc_cxx::raw(a) | cb_wcc_cxx::raw(b)); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator&(E a, E b) { return static_cast<E>(cb_wcc_cxx::raw(a) & cb_wcc_cxx::raw(b)); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator^(E a, E b) { return static_cast<E>(cb_wcc_cxx::raw(a) ^ cb_wcc_cxx::raw(b)); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator~(E a) { return static_cast<E>(~cb_wcc_cxx::raw(a)); }

template <class E, class I, cb_wcc_cxx::if_enum<E> = 0, std::enable_if_t<(std::is_integral_v<I> || std::is_enum_v<I>), int> = 0>
constexpr E& operator|=(E& a, I b) { return a = static_cast<E>(cb_wcc_cxx::raw(a) | b); }
template <class E, class I, cb_wcc_cxx::if_enum<E> = 0, std::enable_if_t<(std::is_integral_v<I> || std::is_enum_v<I>), int> = 0>
constexpr E& operator&=(E& a, I b) { return a = static_cast<E>(cb_wcc_cxx::raw(a) & b); }
template <class E, class I, cb_wcc_cxx::if_enum<E> = 0, std::enable_if_t<(std::is_integral_v<I> || std::is_enum_v<I>), int> = 0>
constexpr E& operator^=(E& a, I b) { return a = static_cast<E>(cb_wcc_cxx::raw(a) ^ b); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E& operator|=(E& a, E b) { return a = a | b; }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E& operator&=(E& a, E b) { return a = a & b; }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E& operator^=(E& a, E b) { return a = a ^ b; }
template <class E, class I, cb_wcc_cxx::if_enum<E> = 0, std::enable_if_t<(std::is_integral_v<I> || std::is_enum_v<I>), int> = 0>
constexpr E& operator+=(E& a, I b) { return a = static_cast<E>(cb_wcc_cxx::raw(a) + b); }
template <class E, class I, cb_wcc_cxx::if_enum<E> = 0, std::enable_if_t<(std::is_integral_v<I> || std::is_enum_v<I>), int> = 0>
constexpr E& operator-=(E& a, I b) { return a = static_cast<E>(cb_wcc_cxx::raw(a) - b); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E& operator++(E& a) { return a = static_cast<E>(cb_wcc_cxx::raw(a) + 1); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator++(E& a, int) { E was = a; ++a; return was; }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E& operator--(E& a) { return a = static_cast<E>(cb_wcc_cxx::raw(a) - 1); }
template <class E, cb_wcc_cxx::if_enum<E> = 0>
constexpr E operator--(E& a, int) { E was = a; --a; return was; }

}  // namespace

#endif
#endif
