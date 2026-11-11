// cb/java/projects/minecraft/achievements.hpp -- analyze the program's achievements and map them onto the console's
// achievement system.
//
// On a PC/Java program, achievements are just objects created and unlocked in code. On a console they are NOT
// free-form: every achievement must be declared up front in the title's configuration (on Xbox 360, an XLAST
// project compiled to an SPA blob and embedded in the build) and unlocked through a platform call that writes to the
// signed-in profile over the platform's online service (XUserWriteAchievements on 360). This module SCANS the
// converted code for the program's achievement definitions, MAPS them to a platform achievement table, and EMITS a
// report (ACHIEVEMENTS.md), an XLAST-style config skeleton and a code seam (Achievements.h/.cpp). It also crops each
// achievement's tile art out of the game's own texture atlases.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::projects::minecraft::achievements {

// ---- title id ----------------------------------------------------------------------------------------------
// A 360 title id is 32-bit: high 16 = publisher id, low 16 = title number. A self-published/homebrew title picks its
// own and just has to avoid colliding with an existing one.
inline constexpr std::uint32_t kHomebrewPublisher = 0xFFFE;

// 'FFFE1234', '0xFFFE1234', 'fffe1234' (surrounding blanks allowed) -> the canonical 8-hex-digit UPPER-case text;
// nullopt for anything that is not 1 to 8 hex digits (so also for empty text).
std::optional<std::string> normalize_title_id(std::string_view tid);

// A stable, self-issued title id derived from `seed` (e.g. the game name; "title" if empty): the homebrew publisher
// and a title number from the seed's CRC-32. NOT guaranteed unique -- the user must check it against a title-id
// list. Returns 8 upper-case hex digits.
std::string suggest_title_id(std::string_view seed);

// The project's title_id from <out_dir>/platform.config.json (a string, or a whole number read as its decimal
// digits), or nullopt if the file is missing, is not JSON, or holds no usable title id. Shared with the vcxproj
// writer so the XLAST/SPA and the XEX are stamped from the same value.
std::optional<std::string> title_id_from_config(const std::filesystem::path& out_dir);

// ---- scanning the program ----------------------------------------------------------------------------------
// One achievement the program defines: `new Achievement(<id>, "<key>", <col>, <row>, <icon>, <parent>)`, optionally
// followed by `->setSpecial()`.
struct Achievement {
    std::int64_t id = 0;
    std::string key;
    std::string name;                    // from the .lang file, else the key made readable
    std::string desc;
    bool hidden = false;                 // the program's 'special/secret' flag
    std::optional<std::string> parent;   // the key's variable of the parent achievement, if any
    std::optional<std::string> icon;     // e.g. "Item::s_book" / "Block::s_wood"
};

// camelCase / snake key -> a readable title: openInventory -> Open Inventory, onARail -> On A Rail.
std::string humanize(std::string_view key);

// What a .lang file says of one achievement.
struct LangName {
    std::optional<std::string> name;   // achievement.<key>=Name
    std::optional<std::string> desc;   // achievement.<key>.desc=Description
};

// Parse a .lang file: key -> its name and description. Empty if the file is missing.
std::map<std::string, LangName> load_lang_names(const std::filesystem::path& lang_file);

// Find every achievement the program defines in the `.cpp`/`.java` files under `code_dir` (a later definition of the
// same id replaces an earlier one). Sorted by id.
std::vector<Achievement> scan_achievements(const std::filesystem::path& code_dir,
                                           const std::optional<std::filesystem::path>& lang_file = std::nullopt);

// ---- icon resolution: symbol -> atlas sprite ---------------------------------------------------------------
enum class Atlas {
    Items,     // gui/items.png
    Terrain,   // terrain.png
};
std::string_view atlas_name(Atlas a);   // "items" / "terrain"

// A 16x16 sprite of an atlas: column and row in sprites.
struct Sprite {
    Atlas atlas = Atlas::Items;
    std::uint32_t col = 0;
    std::uint32_t row = 0;
};

// Map each icon symbol of the game ("Item::s_book", "Block::s_wood") to its sprite by reading the game's Item and
// Block definitions: an item places its sprite with setIconCoord(col,row) or setIconIndex(n); a block carries a
// terrain.png index as the literal 2nd argument of its base constructor, or -- for a subclass -- as a literal
// `blockIndexInTexture = N;` in the subclass's constructor. Symbols that cannot be resolved are absent.
std::map<std::string, Sprite> resolve_icon_atlas(const std::filesystem::path& code_dir);

// ---- the platform mapping ----------------------------------------------------------------------------------
// The two kinds of Xbox 360 title: retail (up to 99 achievements totalling 1000 gamerscore) and arcade (up to 12,
// 200 gamerscore).
enum class Tier { Retail, Arcade };
struct TierLimits {
    std::int64_t max_count = 0;
    std::int64_t score = 0;
};
TierLimits limits(Tier tier);
std::string_view tier_name(Tier tier);               // "retail" / "arcade"
std::optional<Tier> parse_tier(std::string_view name);

// Split `total` gamerscore across n achievements as evenly as possible in steps of 5, summing EXACTLY to total (the
// first few take the remainder). When n is so large that the step would be 0, every achievement gets total/n and the
// last one the rest. Empty for n <= 0.
std::vector<std::int64_t> distribute_score(std::int64_t n, std::int64_t total);

// Where an achievement's tile art went: the unlocked and locked PNGs, as paths relative to the output folder with
// backslashes (as the XLAST project wants them).
struct TileArt {
    std::string unlocked;
    std::string locked;
};

// A row of the platform mapping.
struct Entry {
    std::int64_t platform_id = 0;   // consoles number achievements from 1
    std::int64_t source_id = 0;
    std::string key;
    std::string name;
    std::string desc;
    std::int64_t gamerscore = 0;
    bool hidden = false;
    std::optional<std::string> icon;
    std::optional<TileArt> art;     // set by wire_icons
};

struct Mapping {
    std::vector<Entry> entries;
    Tier tier = Tier::Retail;
    std::int64_t total_score = 0;
    std::vector<std::string> warnings;
    std::int64_t count = 0;
    std::int64_t limit = 0;         // the tier's maximum count
};

// Assign a platform achievement id (1-based, as consoles expect) and a gamerscore to each achievement, with a
// warning for an over-full tier or a score that does not add up.
Mapping map_to_platform(std::span<const Achievement> achievements, Tier tier = Tier::Retail);

// ---- tile art ----------------------------------------------------------------------------------------------
struct Unresolved {
    std::string key;
    std::string reason;
};

struct IconWiring {
    std::vector<std::string> wired;        // keys whose art was written
    std::vector<Unresolved> unresolved;    // keys that need art by hand, and why
};

// For each achievement whose icon resolves, crop its 16x16 sprite from the atlas under `content_dir`, scale it to
// `tile` x `tile` (nearest, crisp), and write an unlocked and a locked (desaturated, darkened) PNG into
// content_dir/achievement_images/. Each wired entry gets its `art`, relative to `out_dir` (where achievements.xlast
// lives). An atlas that is missing or not a readable PNG leaves its entries unresolved, with the reason.
IconWiring wire_icons(std::vector<Entry>& entries, const std::filesystem::path& code_dir,
                      const std::filesystem::path& content_dir, const std::filesystem::path& out_dir,
                      std::uint32_t tile = 64);

// The game's Content folder (the one with the texture atlases) near `out_dir`: Debug/Content (the canonical deploy
// dir, which is `game:\Content`), Content, the folder itself, Release/Content, Release_LTCG/Content. nullopt if none
// has terrain.png or gui/items.png.
std::optional<std::filesystem::path> guess_content_dir(const std::filesystem::path& out_dir);

// ---- emission ----------------------------------------------------------------------------------------------
inline constexpr std::string_view kDeliveryOnline = "online";

// `openInventory` -> `ACH_OPEN_INVENTORY`.
std::string const_name(std::string_view key);

// ACHIEVEMENTS.md. `delivery` is how unlocks reach the profile ("online", or another name for local tracking);
// `icons` adds the tile-art section; `title_id` (8 hex digits; "00000000" when empty) says which title the
// achievements belong to; `title_suggested` marks a self-issued id.
std::string render_report(const Mapping& mapping, std::string_view delivery = kDeliveryOnline,
                          const IconWiring* icons = nullptr, std::string_view title_id = {}, bool title_suggested = false);

// An XLAST-style achievement config skeleton (XML) to import into the submission tool, written to `out_path`.
// A title id that does not normalise is written as 00000000.
void emit_xlast(const Mapping& mapping, const std::filesystem::path& out_path, std::string_view title_id = {});

// Achievements.h + Achievements.cpp in `out_dir`: the ACH_* id constants and the unlock seam. Returns their paths.
std::vector<std::filesystem::path> emit_code(const Mapping& mapping, const std::filesystem::path& out_dir,
                                             std::string_view delivery = kDeliveryOnline);

// ---- the whole pass ----------------------------------------------------------------------------------------
struct RunOptions {
    Tier tier = Tier::Retail;
    std::string delivery{kDeliveryOnline};
    std::optional<std::filesystem::path> lang_file;     // achievement names and descriptions
    std::optional<std::filesystem::path> content_dir;   // with the game's atlases; found near out_dir if absent
    std::optional<std::string> title_id;                // self-issued; else the project's platform.config.json
};

struct RunResult {
    Mapping mapping;
    std::filesystem::path report;
    std::filesystem::path xlast;
    std::vector<std::filesystem::path> code;
    std::optional<std::filesystem::path> content_dir;
    std::int64_t count = 0;
    IconWiring icons;
    std::string title_id;                // 8 hex digits
    bool title_suggested = false;        // always false: a title id is never issued on the user's behalf
};

// Scan `code_dir`, map, wire icons, and write ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp into
// `out_dir` (created if needed). The title id is `options.title_id` if it is one, else the project's
// platform.config.json title id, else the conventional unset value 00000000.
RunResult run(const std::filesystem::path& code_dir, const std::filesystem::path& out_dir, const RunOptions& options = {});

}  // namespace cb::java::projects::minecraft::achievements
