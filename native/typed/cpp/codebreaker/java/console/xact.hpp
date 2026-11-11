// codebreaker/java/console/xact.hpp -- SHIM over cb/java/console/xact.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer (run).
//
// The new module takes the sounds the game plays as a SoundDemand; the old callers hand it the Java folder and the
// shim asks the old content scanner for them. COMPAT(py): remove when projects/minecraft/content is refactored.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/java/console/xact.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/projects/minecraft/content.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::xact {

namespace detail_ {
namespace nw = cb::java::console::xact;

// The sounds the game plays (content.scan_assets over the Java source), keys in the scanner's order.
inline nw::SoundDemand demand_of(const py::str& java_dir) {
    const auto assets = projects::minecraft::content::scan_assets(java_dir);
    nw::SoundDemand demand;
    for (const auto& kv : assets.sounds.items()) demand.sounds.push_back(std::get<0>(kv).utf8());
    for (const auto& kv : assets.step_sounds.items()) demand.step_sounds.push_back(std::get<0>(kv).utf8());
    return demand;
}
}  // namespace detail_

// Human-readable decompile of an .xwb or .xsb.
inline py::str dump(const py::str& path) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::java::console::xact::dump(shim::path_of(path))); });
}

// The XACT cues the converted game will request: {cue_name: [source_event(s)]}.
inline py::dict<py::str, py::list<py::str>> needed_cues(const py::str& java_dir) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::java::console::xact::needed_cues(detail_::demand_of(java_dir))); });
}

// What cue_coverage returns: the (needed, authored, missing, unused) dict.
struct CueCoverage {
    py::dict<py::str, py::list<py::str>> needed;
    py::list<py::str> authored;
    py::list<py::str> missing;
    py::list<py::str> unused;
};

namespace detail_ {
inline CueCoverage to_old(const nw::CueCoverage& c) {
    CueCoverage o;
    o.needed = cb::compat::to_py(c.needed);
    o.authored = cb::compat::to_py(c.authored);
    o.missing = cb::compat::to_py(c.missing);
    o.unused = cb::compat::to_py(c.unused);
    return o;
}
inline nw::CueCoverage to_new(const CueCoverage& c) {
    nw::CueCoverage o;
    for (const auto& kv : c.needed.items()) o.needed[std::get<0>(kv).utf8()] = cb::compat::from_py(std::get<1>(kv));
    o.authored = cb::compat::from_py(c.authored);
    o.missing = cb::compat::from_py(c.missing);
    o.unused = cb::compat::from_py(c.unused);
    return o;
}
}  // namespace detail_

// Diff the cues the game NEEDS against those AUTHORED in an existing sound bank.
inline CueCoverage cue_coverage(const py::str& java_dir, const py::str& xsb_path) {
    return cb::compat::guard([&] {
        const auto bank = cb::java::console::xact::read_xsb(shim::path_of(xsb_path));
        return detail_::to_old(cb::java::console::xact::cue_coverage(detail_::demand_of(java_dir), bank));
    });
}

inline py::str render_coverage(const CueCoverage& cov) {
    return cb::compat::to_py(cb::java::console::xact::render_coverage(detail_::to_new(cov)));
}

// One matched cue: {'event':.., 'files':[..]}.
struct Matched {
    py::str event;
    py::list<py::str> files;
};

// What match_sources returns: {'matched': {cue: {'event':.., 'files':[..]}}, 'missing': {cue:[events]}, 'index_keys': n}.
struct Match {
    py::dict<py::str, Matched> matched;
    py::dict<py::str, py::list<py::str>> missing;
    int64_t index_keys = 0;
};

// Map each needed cue to the original .ogg file(s) that supply it.
inline Match match_sources(const py::str& java_dir, const py::str& resources_dir) {
    return cb::compat::guard([&] {
        const auto m = cb::java::console::xact::match_sources(detail_::demand_of(java_dir), shim::path_of(resources_dir));
        Match out;
        for (const auto& [cue, source] : m.matched) out.matched[cb::compat::to_py(cue)] = Matched{cb::compat::to_py(source.event), cb::compat::to_py(source.files)};
        out.missing = cb::compat::to_py(m.missing);
        out.index_keys = static_cast<int64_t>(m.index_keys);
        return out;
    });
}

// Emit convert_audio.bat: an ffmpeg command per matched cue. Returns the .bat path.
inline py::str write_ffmpeg_script(const Match& match, const py::str& out_dir, const py::str& resources_dir,
                                   const py::str& wav_subdir = py::str("wav"), const py::str& bat_name = py::str("convert_audio.bat")) {
    return cb::compat::guard([&] {
        cb::java::console::xact::Match m;
        for (const auto& kv : match.matched.items()) {
            const Matched& source = std::get<1>(kv);
            m.matched[std::get<0>(kv).utf8()] = {source.event.utf8(), cb::compat::from_py(source.files)};
        }
        return shim::native(cb::java::console::xact::write_ffmpeg_script(m, shim::path_of(out_dir), shim::path_of(resources_dir),
                                                                         wav_subdir.utf8(), bat_name.utf8()));
    });
}

// One-shot XACT3 audio build for a converted game. Returns (xap_path, cues).
inline std::tuple<py::str, py::list<py::str>> build(const py::str& java_dir, const py::str& out_dir,
                                                    const std::optional<py::str>& resources_dir = std::nullopt, bool autorun = false,
                                                    const std::optional<py::str>& sdk_root = std::nullopt,
                                                    const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        cb::java::console::xact::BuildOptions options;
        options.resources_dir = shim::given(resources_dir);
        options.java_dir = shim::path_of(java_dir);
        options.autorun = autorun;
        options.sdk_root = shim::given(sdk_root);
        if (log) options.log = [&](std::string_view line) { log(cb::compat::to_py(line)); };
        const auto done = cb::java::console::xact::build(detail_::demand_of(java_dir), shim::path_of(out_dir), options);
        return std::make_tuple(shim::native(done.xap), cb::compat::to_py(done.cues));
    });
}

}  // namespace codebreaker::java::console::xact
