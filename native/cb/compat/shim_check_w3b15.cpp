// Compile-only check of the shims at the old header paths for verify/reconstruct and verify/sweep (the old lane interface over py:: records, the registry
// functions, run and the sweep's three functions) and of engine/lanes_wiring.cpp, which installs the hooks those shims ask the old world for. Nothing here
// runs; it is in cb_shim_check so the shims stay honest until their last consumers are refactored.
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/translate/frontends/python/methods.hpp"
#include "codebreaker/translate/frontends/python/package.hpp"
#include "codebreaker/verify/sweep.hpp"

namespace shim_check {

// an old lane: the interface the old lanes (win16, win32, driver, powerpc) still implement
class OldLane final : public codebreaker::verify::reconstruct::Kind {
  public:
    py::str KIND() const override { return py::str("old"); }
    std::tuple<py::str, py::str> profile(const py::str&) const override { return {py::str("d"), py::str("s")}; }
    codebreaker::verify::reconstruct::Prepared prepare(const py::str&, const py::str&) const override { return {}; }
    codebreaker::verify::reconstruct::Built build(const codebreaker::verify::reconstruct::Prepared&, const py::str&, const py::str&) const override { return {}; }
    codebreaker::verify::reconstruct::Verdict judge(const codebreaker::verify::reconstruct::Context&,
                                                    const codebreaker::verify::reconstruct::Item&) const override {
        return codebreaker::verify::reconstruct::NOT_RUN();
    }
};

inline void touch_w3b15() {
    namespace rec = codebreaker::verify::reconstruct;
    namespace sw = codebreaker::verify::sweep;
    rec::register_kind(py::str("old"), std::make_shared<OldLane>());
    (void)&rec::kinds;
    (void)&rec::kind_named;
    (void)&rec::kind_name;
    (void)&rec::kind_of;
    (void)&rec::digests;
    (void)&rec::identity;
    (void)&rec::module_sha;
    (void)&rec::_reaches;
    (void)&rec::signatures;
    (void)&rec::run;
    (void)&rec::judge_worker;
    (void)&rec::hooks;
    (void)&sw::_wanted;
    (void)&sw::status;
    (void)&sw::main;
}

}  // namespace shim_check
