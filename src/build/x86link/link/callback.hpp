// link/callback.hpp -- how the linker hands a routine to a walker (a ring's, a carver's, the sections', the modules' ...):
// FunctionRef<F>, a reference to anything callable as F (a function, or a lambda that holds what the routine needs:
// the Link among it), and bind_link(link, f), the routine f with the Link given first. A walker takes FunctionRef<F>
// where wlink took F *; a routine that needs the link's state takes Link& first and is handed as bind_link(link, f).
// (Force-included with cbcxx.h: every file of the linker has it.)
#ifndef CB_WLINK_CALLBACK_HPP
#define CB_WLINK_CALLBACK_HPP

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace cb_wlink {

struct Link;

template <class F>
class FunctionRef;

template <class R, class... A>
class FunctionRef<R(A...)> {
public:
    FunctionRef() noexcept = default;
    FunctionRef(std::nullptr_t) noexcept {}
    FunctionRef(R (*f)(A...)) noexcept
        : obj_(f == nullptr ? nullptr : reinterpret_cast<void*>(f)),
          call_(f == nullptr ? nullptr : [](void* o, A... a) -> R { return reinterpret_cast<R (*)(A...)>(o)(std::forward<A>(a)...); }) {}
    template <class G, std::enable_if_t<!std::is_same_v<std::remove_cvref_t<G>, FunctionRef> &&
                                            !std::is_function_v<std::remove_reference_t<G>> &&
                                            !std::is_pointer_v<std::remove_cvref_t<G>> && std::is_invocable_r_v<R, G&, A...>,
                                        int> = 0>
    FunctionRef(G&& g) noexcept   // (a lambda: it outlives the walk, the full expression it is made in)
        : obj_(const_cast<void*>(static_cast<const void*>(std::addressof(g)))),
          call_([](void* o, A... a) -> R { return (*static_cast<std::remove_reference_t<G>*>(o))(std::forward<A>(a)...); }) {}

    R operator()(A... a) const { return call_(obj_, std::forward<A>(a)...); }
    explicit operator bool() const noexcept { return call_ != nullptr; }
    friend bool operator==(const FunctionRef& f, std::nullptr_t) noexcept { return f.call_ == nullptr; }

private:
    void* obj_ = nullptr;
    R (*call_)(void*, A...) = nullptr;
};

// f, with the link given first: what a walker calls with its own arguments (each converted to f's, as wlink's casts of
// the routine to the walker's type had them: a ring's element is a void * the routine names as its type)
template <class R, class... A>
auto bind_link(Link& link, R (*f)(Link&, A...)) {
    return [&link, f](auto... a) -> R { return f(link, (A)(a)...); };
}

}  // namespace cb_wlink

using cb_wlink::FunctionRef;
using cb_wlink::bind_link;

#endif
