// codebreaker/java/projects/minecraft/achievements.hpp -- SHIM over cb/java/projects/minecraft/achievements.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// What the old consumers use: the title-id helpers (console/vcxproj), scan_achievements and run (the Minecraft
// project's reports step, `codebreaker java achievements`). The records of run()'s result keep their old shape. The
// port's intermediate steps (resolve_icon_atlas, wire_icons, map_to_platform, render_report, emit_xlast, emit_code
// and the private helpers) had no caller outside the module and are not carried: they are the new module's API.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/projects/minecraft/achievements.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::achievements {

namespace shim_ {
namespace cba = cb::java::projects::minecraft::achievements;
inline std::filesystem::path path(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }
inline std::optional<std::filesystem::path> opt_path(const std::optional<py::str>& s) {
    if (!s || s->empty()) return std::nullopt;   // (an empty folder name meant "none")
    return path(*s);
}
}  // namespace shim_

// Accept 'FFFE1234', '0xFFFE1234', 'fffe1234' -> canonical 8-hex-digit UPPER string, or None.
inline std::optional<py::str> normalize_title_id(const std::optional<py::str>& tid) {
    if (!tid) return std::nullopt;
    return cb::compat::to_py(shim_::cba::normalize_title_id(cb::compat::from_py(*tid)));
}

// A stable, self-issued title id derived from `seed` (e.g. the game name). Returns an 8-hex-digit UPPER string.
inline py::str suggest_title_id(const std::optional<py::str>& seed) {
    return cb::compat::to_py(shim_::cba::suggest_title_id(seed ? cb::compat::from_py(*seed) : std::string()));
}

// One row of scan_achievements' result.
struct Achievement {
    int64_t id = 0;
    py::str key;
    py::str name;
    py::str desc;
    bool hidden = false;                 // the program's 'special/secret' flag
    std::optional<py::str> parent;
    std::optional<py::str> icon;         // e.g. 'Item::s_book' / 'Block::s_wood'
};

// Find every achievement the program defines. Returns a list sorted by id.
inline py::list<Achievement> scan_achievements(const py::str& code_dir, const std::optional<py::str>& lang_file = std::nullopt) {
    return cb::compat::guard([&] {
        py::list<Achievement> out;
        for (const auto& a : shim_::cba::scan_achievements(shim_::path(code_dir), shim_::opt_path(lang_file))) {
            out.append(Achievement{a.id, cb::compat::to_py(a.key), cb::compat::to_py(a.name), cb::compat::to_py(a.desc), a.hidden,
                                   cb::compat::to_py(a.parent), cb::compat::to_py(a.icon)});
        }
        return out;
    });
}

// A row of the platform mapping.
struct Entry {
    int64_t platform_id = 0;             // consoles number achievements from 1
    int64_t source_id = 0;
    py::str key;
    py::str name;
    py::str desc;
    int64_t gamerscore = 0;
    bool hidden = false;
    std::optional<py::str> icon;
    std::optional<py::str> image_unlocked;   // set when the tile art was wired
    std::optional<py::str> image_locked;
};

// What wire_icons returns: {'wired': [keys], 'unresolved': [(key, reason)]}.
struct IconWiring {
    py::list<py::str> wired;
    py::list<std::tuple<py::str, py::str>> unresolved;
};

// What map_to_platform returns.
struct Mapping {
    py::list<py::ref<Entry>> entries;
    py::str profile;
    int64_t total_score = 0;
    py::list<py::str> warnings;
    int64_t count = 0;
    int64_t limit = 0;
};

// What run() returns (a dict with these fixed keys in Python).
struct RunResult {
    Mapping mapping;
    py::str report;
    py::str xlast;
    py::list<py::str> code;
    std::optional<py::str> content_dir;
    int64_t count = 0;
    IconWiring icons;
    py::str title_id;
    bool title_suggested = false;
};

// Scan, map, wire icons, and emit ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp. `content_dir` (with the
// game's atlases) enables auto-wiring the achievement tile art; `title_id` (self-issued) is written into the XLAST.
// (`title_name` was never read.) A `profile` that is not 'retail' or 'arcade' is a KeyError, as it was when the report
// looked its limits up.
inline RunResult run(const py::str& code_dir, const py::str& out_dir, const std::optional<py::str>& lang_file = std::nullopt,
                     const py::str& profile = py::str("retail"), const py::str& delivery = py::str("online"),
                     const std::optional<py::str>& content_dir = std::nullopt, const std::optional<py::str>& title_id = std::nullopt,
                     const std::optional<py::str>& /*title_name*/ = std::nullopt) {
    const std::optional<shim_::cba::Tier> tier = shim_::cba::parse_tier(cb::compat::from_py(profile));
    if (!tier) throw py::KeyError(py::repr(profile).utf8());
    return cb::compat::guard([&] {
        shim_::cba::RunOptions opt;
        opt.tier = *tier;
        opt.delivery = cb::compat::from_py(delivery);
        opt.lang_file = shim_::opt_path(lang_file);
        opt.content_dir = shim_::opt_path(content_dir);
        if (title_id && !title_id->empty()) opt.title_id = cb::compat::from_py(*title_id);
        const shim_::cba::RunResult r = shim_::cba::run(shim_::path(code_dir), shim_::path(out_dir), opt);

        RunResult out;
        for (const shim_::cba::Entry& e : r.mapping.entries) {
            auto row = std::make_shared<Entry>();
            row->platform_id = e.platform_id;
            row->source_id = e.source_id;
            row->key = cb::compat::to_py(e.key);
            row->name = cb::compat::to_py(e.name);
            row->desc = cb::compat::to_py(e.desc);
            row->gamerscore = e.gamerscore;
            row->hidden = e.hidden;
            row->icon = cb::compat::to_py(e.icon);
            if (e.art) {
                row->image_unlocked = cb::compat::to_py(e.art->unlocked);
                row->image_locked = cb::compat::to_py(e.art->locked);
            }
            out.mapping.entries.append(row);
        }
        out.mapping.profile = cb::compat::to_py(shim_::cba::tier_name(r.mapping.tier));
        out.mapping.total_score = r.mapping.total_score;
        out.mapping.warnings = cb::compat::to_py(r.mapping.warnings);
        out.mapping.count = r.mapping.count;
        out.mapping.limit = r.mapping.limit;
        out.report = cb::compat::to_py(cb::fs::to_text(r.report));
        out.xlast = cb::compat::to_py(cb::fs::to_text(r.xlast));
        for (const auto& p : r.code) out.code.append(cb::compat::to_py(cb::fs::to_text(p)));
        if (r.content_dir) out.content_dir = cb::compat::to_py(cb::fs::to_text(*r.content_dir));
        out.count = r.count;
        out.icons.wired = cb::compat::to_py(r.icons.wired);
        for (const shim_::cba::Unresolved& u : r.icons.unresolved) {
            out.icons.unresolved.append(std::make_tuple(cb::compat::to_py(u.key), cb::compat::to_py(u.reason)));
        }
        out.title_id = cb::compat::to_py(r.title_id);
        out.title_suggested = r.title_suggested;
        return out;
    });
}

}  // namespace codebreaker::java::projects::minecraft::achievements
