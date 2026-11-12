// Compile-only check of the shim at the old header path for coverage/reach: the surface the translated Python and the exe's
// command dispatcher use, over native/cb/coverage/reach, with the old layer's volumes, store and corpus wired in. Nothing here
// runs; it is in cb_shim_check so the shim stays honest until its last consumer is refactored.
#include "codebreaker/coverage/reach.hpp"

namespace codebreaker::shim_check {

py::str coverage_shims(const py::list<py::str>& inputs, const py::bytes& head) {
    namespace reach = coverage::reach;
    py::str out;
    reach::platform_identify() = [](const py::str&) -> std::optional<reach::PlatformInfo> { return std::nullopt; };
    reach::measure_argv() = [](const py::str& p) { return py::list<py::str>{p}; };
    reach::clock() = [] { return 0.0; };
    if (const auto gap = reach::_gap(py::str("x.nds"), head)) out += std::get<0>(*gap);
    out += reach::describe(inputs[0]).found;
    for (const auto& f : reach::programs(inputs, 3)) out += std::get<0>(f);
    for (const py::str& p : reach::_unpack(inputs[0], 2, nullptr)) out += p;
    reach::Judged j = reach::judge(inputs[0], 5);
    out += j.stopped;
    out += std::get<1>(reach::tally_of(inputs[0]));
    py::list<reach::Row> rows = reach::read_results(inputs[0]);
    for (const reach::Row& r : reach::spread(rows, 2, 100)) out += r.sha;
    auto ran = reach::run(inputs, 5, false, 300, 20, py::str("r"), false, 1 << 20);
    out += std::get<1>(ran) + reach::report(std::get<0>(ran));
    out += py::str(std::to_string(reach::main(inputs)));
    return out;
}

}  // namespace codebreaker::shim_check
