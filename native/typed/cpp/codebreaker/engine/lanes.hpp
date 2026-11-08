// codebreaker/engine/lanes.hpp -- interface of codebreaker.engine.lanes (typed C++; implementation pending).
//
// lanes.py -- every lane's reconstruct-and-judge as steps of the engine, and the 32-bit module build.
//
//     auto rows = lanes::judged(path);               // -> Judged: the rows reconstruct.run records
//
// A lane (a verify::reconstruct::Kind) is four calls: profile, prepare, build, judge.  The same steps serve every
// lane, filed under the name its results are (pe, ne, le, dos, powerpc, nes, ...): lane.K.profile, .module, .function,
// .keys, .texts, .build, .built, .verdict, .rows, and pe.image for the 32-bit module build.
//
// Typing notes:
//   * Python's platform object (platforms.Platform: name, id, lane, packager.tools, lane_module()) has no port; the
//     fields engine.lanes reads are `LanePlatform`.  `tools` may be empty (the Python: packager.tools is None).
//   * The lane module of the Python (profile/prepare/build/judge) is verify::reconstruct::Kind.
//   * A step's values are the engine's facts (engine::Value); Judged is the typed form of the dict lane.K.rows gives
//     (`rows` entries are JudgedRow, the dict with keys addr, sig, name, source, verdict, reason, limits, detail,
//     problem), `made` is engine::last_run.
//   * build() answers a 32-bit module's Built (build::module::Built, group 6): forward declared here.
//   * KINDS (the Python's list of declared lane names) is `kinds()`.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>

#include "codebreaker/engine/engine.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"

namespace codebreaker::build::module {
class Built;
}

namespace codebreaker::engine::lanes {

// The fields of a platform that this module reads.
struct LanePlatform {
    py::str name;   // the name results are filed under (K)
    py::str id;     // the platform id (steps are declared under it)
    py::str lane;   // the lane's module name (a stamp digests its file)
    // packager.tools(): {name: path} ({} / empty function when the platform has none)
    std::function<py::dict<py::str, py::str>()> tools;
    // lane_module(): the platform's Kind
    std::function<std::shared_ptr<verify::reconstruct::Kind>()> lane_module;
};

// One row reconstruct.run records in the function store.
struct JudgedRow {
    py::str addr;
    py::str sig;
    py::str name;
    std::optional<py::str> source;     // the C (None: nothing produced)
    py::str verdict, reason;
    py::str limits, detail;            // limits: comma-joined
    std::optional<py::str> problem;    // "none", "compile" ...
};

// What lane.K.rows gives, and how the run went.
struct Judged {
    py::str profile;
    py::str settings;
    py::str header;
    py::list<py::str> all_keys;
    py::list<JudgedRow> rows;
    py::dict<py::str, py::str> built;               // {key: built signature}
    py::dict<py::str, py::list<int64_t>> made;      // {kind: [kept, made]}
};

// The lane names declared so far (Python's KINDS).
py::list<py::str> kinds();

// Declare the lane's steps (lane.K.*) for a platform. -> K  (used by tests.test_engine for a made-up lane)
py::str _declare(const LanePlatform& p);

// {name: signature} as reconstruct.run gives them: per function for a lane that builds per function, else the
// module's own for each.
py::dict<py::str, py::str> reconstruct_signatures(const verify::reconstruct::Prepared& m, const verify::reconstruct::Kind& kind);

// Is this module's lane on these steps?
bool on(const py::str& path);

// The module's database: a binary's by its bytes; a folder (a Java program's source) by the lane's sha.
std::shared_ptr<native_engine::Database> open_module(const py::str& path);

// reconstruct.run's decompile, build and judge of a module, made by the engine.  `log` defaults to None.
Judged judged(const py::str& path, const std::function<void(const py::str&)>& log = nullptr);

// module.build of a module whose platform has these steps: a 32-bit module made by the engine. `outdir` None: the
// database's build folder; `log` defaults to print.  Raises py::SystemExit for a platform that is not 32-bit Windows.
std::shared_ptr<build::module::Built> build(const py::str& path, const std::optional<py::str>& outdir = std::nullopt,
                                            bool call = true, const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::engine::lanes
