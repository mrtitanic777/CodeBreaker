// codebreaker/base/tidy_state.cpp -- the shim's translation unit (see tidy_state.hpp): the old arguments in, the new
// cb::base::tidy_state called. COMPAT(py): the cache's own clean-up is the old cache port's (base/cache), given to the new module
// as hooks; deleted with tidy_state.hpp.

#include "codebreaker/base/tidy_state.hpp"

#include "cb/base/tidy_state.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/core/log.hpp"
#include "codebreaker/base/cache.hpp"

namespace codebreaker::base::tidy_state {

int64_t main(const py::list<py::str>& argv) {
    namespace newt = ::cb::base::tidy_state;
    if (!argv.empty() && (argv[0] == py::str("-h") || argv[0] == py::str("--help"))) {
        py::print(py::str(DOC));
        return 0;
    }
    newt::Options options;
    for (const py::str& a : argv) {
        if (a == py::str("--dry")) options.dry = true;
        if (a == py::str("--stray")) options.remove_stray = true;
        if (a == py::str("--keep-builds")) options.keep_builds = true;
    }
    newt::Layout layout;
    layout.state = ::cb::compat::legacy::state_dir();
    const py::str converted = py::settings::get_str(py::str("converted"));
    layout.converted = converted.empty() ? ::cb::compat::legacy::path_of(py::settings::checkout_root()) / "Converted"
                                         : ::cb::compat::legacy::path_of(converted);
    newt::CacheHooks hooks;
    hooks.prune = [] {
        std::vector<newt::CachePruned> out;
        for (const auto& [kind, names] : codebreaker::base::cache::prune()) {
            newt::CachePruned pruned;
            pruned.kind = ::cb::compat::from_py(kind);
            for (const py::str& n : names) pruned.generations.push_back(::cb::compat::from_py(n));
            out.push_back(std::move(pruned));
        }
        return out;
    };
    hooks.max_bytes = [] { return static_cast<std::uint64_t>(codebreaker::base::cache::max_bytes()); };
    const int code = newt::run(layout, options, hooks);
    ::cb::flush_out();   // (the old layer prints through its own stream: keep the two in order)
    return code;
}

}  // namespace codebreaker::base::tidy_state
