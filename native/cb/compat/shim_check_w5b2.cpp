// Compile-only check of the shim at the old header path for analyze/observe (the old py:: API over cb/analyze/observe.hpp and the bridge that
// makes a cb Image of an old analyze::image::Image). Nothing here runs; it is in cb_shim_check so the shim stays honest until its last
// consumer is refactored.
#include "codebreaker/analyze/observe.hpp"

namespace shim_check {

inline void touch_w5b2() {
    namespace observe = codebreaker::analyze::observe;
    (void)&observe::run;
    (void)&observe::analyse;
    (void)sizeof(observe::Observation);
    (void)sizeof(observe::Analysis);
}

}  // namespace shim_check
