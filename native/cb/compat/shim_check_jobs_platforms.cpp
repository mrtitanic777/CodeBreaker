// Compile-only check of the staged shims for cb/jobs/pool and cb/platforms/{win16,vxd}: the old py:: API over the new
// modules, including the parts that call old ports which cannot be linked into cb_tests (the toolchain's paths, the
// corpus). Nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumer is refactored.
#include "shims/codebreaker/jobs/pool.hpp"
#include "shims/codebreaker/platforms/vxd.hpp"
#include "shims/codebreaker/platforms/win16.hpp"

namespace shim_check {

inline void touch_jobs_platforms() {
    namespace pool = codebreaker::jobs::pool;
    namespace win16 = codebreaker::platforms::win16;
    namespace vxd = codebreaker::platforms::vxd;
    py::list<int64_t> items;
    items.append(1);
    pool::Options<int64_t, int64_t> o;
    (void)pool::run_first([](const int&, const int64_t& i) { return i; }, items, 0, o);
    (void)pool::threads([](const int64_t& i) { return i; }, items, 2);
    pool::as_done([](const int64_t& i) { return i; }, items, [](const int64_t&, const int64_t&) {});
    (void)win16::tools();
    (void)win16::library();
    (void)win16::recognises(py::str("x"));
    (void)win16::segments(py::str("x"));
    (void)win16::entries(py::str("x"));
    (void)vxd::tools();
    (void)vxd::recognises(py::str("x"));
    (void)vxd::entries(py::str("x"));
}

}  // namespace shim_check
