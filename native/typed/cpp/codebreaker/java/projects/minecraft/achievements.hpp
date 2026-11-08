// codebreaker/java/projects/minecraft/achievements.hpp -- typed C++ port of
// codebreaker.java.projects.minecraft.achievements.
//
// achievements.py -- analyze the program's achievements and map them onto the console's achievement system.
//
// On a PC/Java program, achievements are just objects created and unlocked in code. On a console they are
// NOT free-form: every achievement must be declared up front in the title's configuration (on Xbox 360,
// an XLAST project compiled to an SPA blob and embedded in the build) and unlocked through a platform call
// that writes to the signed-in profile over the platform's online service (XUserWriteAchievements on 360).
// This module SCANS the converted code for the program's achievement definitions, MAPS them to a platform
// achievement table, and EMITS a report (ACHIEVEMENTS.md), an XLAST-style config skeleton and a code seam
// (Achievements.h/.cpp).
//
// Notes on the port:
//   * the dicts with fixed keys are records: Achievement (scan_achievements' rows), LangName, Entry (a row of
//     the mapping), Mapping, IconWiring, RunResult.  Entry is held as a py::ref because wire_icons mutates the
//     rows in place (it sets image_unlocked / image_locked);
//   * PIL: `try: from PIL import Image` -- the PNG reader/writer inside the .cpp stands in for Pillow
//     (RGBA 8/16-bit, interlaced or not, read directly; every other PNG colour type goes through
//     assets::normalize_png_to_rgba first, as assets.py does).  The PNG files wire_icons writes hold Pillow's
//     pixels; their compressed bytes differ.  A file that is not a PNG is an UnidentifiedImageError (Pillow
//     would also read JPEG, BMP ... atlases; none is used here);
//   * suggest_title_id / normalize_title_id take an optional str (Python takes any object: only str / None
//     are used);
//   * json.load of platform.config.json uses py::json_parse; a title_id that is neither a str nor an int
//     reads as "no title id" (str(x) of a float / list / dict / bool never is 1-8 hex digits).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::achievements {

inline constexpr bool _HAS_PIL = true;   // the PNG code in the .cpp is the stand-in for Pillow

// --- title id --------------------------------------------------------------
// A 360 title id is 32-bit: high 16 = publisher id, low 16 = title number. A self-published/homebrew title
// picks its own and just has to avoid colliding with an existing one.
inline constexpr int64_t _HOMEBREW_PUBLISHER = 0xFFFE;

// Accept 'FFFE1234', '0xFFFE1234', 'fffe1234' -> canonical 8-hex-digit UPPER string, or None.
std::optional<py::str> normalize_title_id(const std::optional<py::str>& tid);

// A stable, self-issued title id derived from `seed` (e.g. the game name). NOT guaranteed unique --
// the user must check it against a title-id list. Returns an 8-hex-digit UPPER string.
py::str suggest_title_id(const std::optional<py::str>& seed);

// `new Achievement(<id>, "<key>", <col>, <row>, <icon>, <parent>)`, optional `->setSpecial()`.
const py::re::Pattern& NEW_ACH_();
const py::re::Pattern& SPECIAL_();
const py::re::Pattern& PARENT_();   // 6th arg (parent var/NULL)
const py::re::Pattern& ICON_();     // the 5th arg is the icon source: Item::s_book / Block::s_wood / ...

// camelCase / snake key -> a readable title. openInventory -> Open Inventory, onARail -> On A Rail.
py::str _humanize(const py::str& key);

// {'name': ..., 'desc': ...} of one key of a .lang file (either may be absent)
struct LangName {
    std::optional<py::str> name;
    std::optional<py::str> desc;
};

// Parse a .lang file for `achievement.<key>=Name` and `achievement.<key>.desc=Desc`. Returns
// {key: {'name':..., 'desc':...}}. Empty if the file is missing.
py::dict<py::str, LangName> load_lang_names(const std::optional<py::str>& lang_file);

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
py::list<Achievement> scan_achievements(const py::str& code_dir, const std::optional<py::str>& lang_file = std::nullopt);

// --- icon resolution: symbol -> atlas sprite -------------------------------
py::str _read(const py::str& path);

// {BlockSubclass: terrainIndex} from each subclass ctor's literal `blockIndexInTexture = N;`.
// The base `Block` sets it from a ctor arg (no literal) and is intentionally absent here.
py::dict<py::str, int64_t> _block_class_textures(const py::str& code_dir);

// Map an icon symbol (s_book / s_wood) to (atlas, col, row), atlas in {'items','terrain'}, by
// reading the game's Item/Block definitions. Unresolvable symbols are simply absent.
py::dict<py::str, std::tuple<py::str, int64_t, int64_t>> resolve_icon_atlas(const py::str& code_dir);

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
    std::optional<py::str> image_unlocked;   // set by wire_icons
    std::optional<py::str> image_locked;
};

// What wire_icons returns: {'wired': [keys], 'unresolved': [(key, reason)]}.
struct IconWiring {
    py::list<py::str> wired;
    py::list<std::tuple<py::str, py::str>> unresolved;
};

// For each achievement whose icon resolves, crop its 16x16 sprite from the atlas, scale to
// `tile`x`tile` (nearest, crisp), and write unlocked + locked PNGs under content_dir/achievement_images/.
// Mutates each entry with image_unlocked / image_locked (relative to out_dir) when wired.
IconWiring wire_icons(const py::list<py::ref<Entry>>& entries, const py::str& code_dir, const py::str& content_dir,
                      const py::str& out_dir, int64_t tile = 64);

// Console achievement constraints (Xbox 360). Retail titles: up to 99 achievements totalling 1000
// gamerscore; Arcade titles: up to 12 totalling 200. Used to validate the mapping.
struct Profile {
    int64_t max_count = 0;
    int64_t score = 0;
};
const py::dict<py::str, Profile>& PROFILES();

// Split `total` gamerscore across n achievements as evenly as possible in steps of 5, summing
// EXACTLY to total (the last few absorb the remainder).
py::list<int64_t> _distribute_score(int64_t n, int64_t total);

// What map_to_platform returns.
struct Mapping {
    py::list<py::ref<Entry>> entries;
    py::str profile;
    int64_t total_score = 0;
    py::list<py::str> warnings;
    int64_t count = 0;
    int64_t limit = 0;
};

// Assign a platform achievement id (1-based, as consoles expect) + gamerscore to each.
Mapping map_to_platform(const py::list<Achievement>& achievements, const py::str& profile = py::str("retail"),
                        const std::optional<py::str>& title_profile = std::nullopt);

// --- emission ---------------------------------------------------------------
py::str _const_name(const py::str& key);

py::str render_report(const Mapping& mapping, const py::str& delivery = py::str("online"),
                      const std::optional<IconWiring>& icons = std::nullopt,
                      const std::optional<py::str>& title_id = std::nullopt, bool title_suggested = false);

// Write an XLAST-style achievement config skeleton (XML) to import into the submission tool.
py::str emit_xlast(const Mapping& mapping, const py::str& out_path, const std::optional<py::str>& title_id = std::nullopt);

py::str _xml(const py::str& s);

// Write Achievements.h + Achievements.cpp: the ACH_* id constants and the unlock seam.
py::list<py::str> emit_code(const Mapping& mapping, const py::str& out_dir, const py::str& delivery = py::str("online"));

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

// Scan, map, wire icons, and emit ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp.
// `content_dir` (with the game's atlases) enables auto-wiring the achievement tile art. `title_id`
// (self-issued, no MS submission needed) is written into the XLAST.
RunResult run(const py::str& code_dir, const py::str& out_dir, const std::optional<py::str>& lang_file = std::nullopt,
              const py::str& profile = py::str("retail"), const py::str& delivery = py::str("online"),
              const std::optional<py::str>& content_dir = std::nullopt,
              const std::optional<py::str>& title_id = std::nullopt,
              const std::optional<py::str>& title_name = std::nullopt);

// The project's title_id from <out_dir>/platform.config.json, or None.
std::optional<py::str> _title_id_from_config(const py::str& out_dir);

// Find the game's Content folder (the one with the texture atlases) near out_dir.
std::optional<py::str> _guess_content_dir(const py::str& out_dir);

}  // namespace codebreaker::java::projects::minecraft::achievements
