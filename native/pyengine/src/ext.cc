// ext.cc -- the engine's extensions over the old front end.
#include "infer.hpp"

namespace pyengine {

TR Eval::ext_std(const A::Call*, const std::string&, const std::vector<std::optional<T>>&, const OMap<std::optional<T>>&, const State&,
                 bool& handled) {
    handled = false;
    return BOTTOM();
}

TR Eval::ext_method(const A::Call*, const T&, const std::string&, const std::vector<std::optional<T>>&, const OMap<std::optional<T>>&,
                    const State&, bool& handled) {
    handled = false;
    return BOTTOM();
}

}  // namespace pyengine
