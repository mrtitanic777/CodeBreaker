// codebreaker/exact/identify.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/exact/identify.{hpp,cpp}; this header
// keeps the old py:: surface (Project with set_switches, Candidate, File, Loader, default_loader, candidates, score, main) for the ports and the
// dynamic exe that still use it. Conversion and forwarding only: the old project is seen by the new module through ledger::detail::NewView, the
// old toolchain by modules::detail::ToolchainAdapter, and the profiles are those of the old toolchain port's folder. Deleted with its last py::
// consumer.
//
// Not carried over: `shuffle` (the new module's sample is its own: a fixed-seed shuffle of the sources, not CPython's random.Random(0)).
//
// DIVERGES, for the old callers: the sample of source files is another fixed one of the same size (so a program's best toolchain may be
// chosen on other functions); the lines go to `out` without Python's formatting of a missing variant or switches; --write writes project.json
// with two-space indentation; a source without an address no longer makes the sort a TypeError.
#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/identify.hpp"
#include "codebreaker/exact/ledger.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::identify {

// a project, as identify reads and changes it
class Project : public ledger::Project {
  public:
    virtual void set_switches(const std::optional<py::str>& switches) = 0;   // prj.switches = ...
};

// (toolchain name, variant or None, switches or None)
using Candidate = std::tuple<py::str, std::optional<py::str>, std::optional<py::str>>;
// a source file as prj.sources() lists it: (address or None, path)
using File = std::tuple<std::optional<int64_t>, py::str>;

using Loader = std::function<std::shared_ptr<ledger::Toolchain>(const py::str& name, const std::optional<py::str>& variant)>;

// toolchain.load
inline Loader default_loader() {
    return [](const py::str& name, const std::optional<py::str>& variant) -> std::shared_ptr<ledger::Toolchain> {
        return toolchain::load(name, variant);
    };
}

namespace nl = ::cb::exact::identify;   // the new module

namespace shim_ {

namespace conv = ::cb::compat;

inline ::cb::exact::toolchain::Locations locations() {
    ::cb::exact::toolchain::Locations l;
    l.profiles = ::cb::fs::from_text(toolchain::HERE().utf8()) / "toolchains";
    return l;
}

inline nl::Services services(Project& prj, const Loader& load, std::ostream& out) {
    nl::Services s;
    s.locations = locations();
    s.set_switches = [&prj](const std::optional<std::string>& sw) { prj.set_switches(sw ? std::optional<py::str>(conv::to_py(*sw)) : std::nullopt); };
    s.say = [&out](std::string_view line) { out << line << "\n"; };
    s.load = [load](const std::string& name, const std::optional<std::string>& variant) -> std::shared_ptr<const ::cb::exact::ledger::Toolchain> {
        return std::make_shared<modules::detail::ToolchainAdapter>(
            load(conv::to_py(name), variant ? std::optional<py::str>(conv::to_py(*variant)) : std::nullopt));
    };
    return s;
}

inline std::optional<py::str> text_or_none(const std::optional<std::string>& s) { return s ? std::optional<py::str>(conv::to_py(*s)) : std::nullopt; }

}  // namespace shim_

inline py::list<Candidate> candidates(const Project& prj, const std::optional<py::list<py::str>>& tries = std::nullopt) {
    std::vector<std::string> named;
    if (tries)
        for (const py::str& t : *tries) named.push_back(::cb::compat::from_py(t));
    const ledger::detail::NewView view(prj);
    py::list<Candidate> out;
    for (const auto& c : nl::candidates(view, shim_::locations(), named))
        out.append(Candidate(::cb::compat::to_py(c.toolchain), shim_::text_or_none(c.variant), shim_::text_or_none(c.switches)));
    return out;
}

// (how many of the sample's functions come out the program's, "N of M" or the toolchain's error)
inline std::tuple<int64_t, py::str> score(Project& prj, const Candidate& cand, const py::list<File>& files, const Loader& load = default_loader()) {
    const ledger::detail::NewView view(prj);
    std::vector<::cb::exact::ledger::SourceFile> sources;
    for (const File& f : files) sources.push_back({std::get<0>(f), ::cb::fs::from_text(std::get<1>(f).utf8())});
    const nl::Candidate c{::cb::compat::from_py(std::get<0>(cand)),
                          std::get<1>(cand) ? std::optional<std::string>(::cb::compat::from_py(*std::get<1>(cand))) : std::nullopt,
                          std::get<2>(cand) ? std::optional<std::string>(::cb::compat::from_py(*std::get<2>(cand))) : std::nullopt};
    const auto result = nl::score(view, c, sources, shim_::services(prj, load, std::cout));
    return {result.same, ::cb::compat::to_py(result.text)};
}

inline int64_t main(Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout, const Loader& load = default_loader()) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(::cb::compat::from_py(a));
    const ledger::detail::NewView view(prj);
    return nl::command(view, args, shim_::services(prj, load, out));
}

}  // namespace codebreaker::exact::identify
