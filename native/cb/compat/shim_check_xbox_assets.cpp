// Compile-only check of the shims at codebreaker/formats/xbox/{xur,bink,chd,package}.hpp: the parts that start programs or reach the old settings (and so are not run in cb_tests). Nothing here
// runs; it is in cb_shim_check, so the shims stay honest until their last consumer is refactored.
#include "codebreaker/formats/xbox/bink.hpp"
#include "codebreaker/formats/xbox/chd.hpp"
#include "codebreaker/formats/xbox/package.hpp"
#include "codebreaker/formats/xbox/xur.hpp"

#include "cb/compat/xbox_assets_shim_test.cpp"   // the shim tests are compiled here too: they run in cb_tests, which the area builds leave out

namespace shim_check_xbox_assets {

inline void touch() {
    namespace xb = codebreaker::formats::xbox;
    (void)xb::bink::decode(py::str("a.bik"), py::str("a.wav"), true, false);
    (void)xb::bink::main(py::list<py::str>());
    (void)xb::chd::extract(py::str("a.chd"), py::str("a.iso"));
    (void)xb::chd::build(py::str("a.iso"), py::str("a.chd"));
    (void)xb::chd::verify(py::str("a.chd"));
    xb::chd::main(py::list<py::str>());
    xb::xur::main(py::list<py::str>());
    xb::set_hooks(xb::Hooks{});
    (void)xb::_container(py::str("x.iso"));
}

}  // namespace shim_check_xbox_assets
