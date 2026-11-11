#include "cb/java/projects/minecraft/achievements.hpp"

#include <algorithm>
#include <array>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/rgba_png.hpp"
#include "cb/java/textutil.hpp"
#include "cb/java/tree.hpp"

namespace cb::java::projects::minecraft::achievements {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kUnsetTitleId = "00000000";

bool is_lower(char c) { return c >= 'a' && c <= 'z'; }
bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool is_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

// `separator` between every lower-case letter and an upper-case one that follows it (the old ([a-z])([A-Z]) rule).
std::string split_camel(std::string_view s, char separator) {
    std::string out;
    out.reserve(s.size() + 4);
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (i > 0 && is_lower(s[i - 1]) && is_upper(s[i])) out += separator;
        out += s[i];
    }
    return out;
}

// `new Achievement(<id>, "<key>"`
const Regex& new_achievement() {
    static const Regex re(R"rx(new\s+Achievement\s*\(\s*(\d+)\s*,\s*"([^"]+)")rx");
    return re;
}
const Regex& special_flag() {
    static const Regex re(R"rx(setSpecial\s*\(\s*\))rx");
    return re;
}
// the 6th argument: the parent's variable (or NULL), then the closing parenthesis
const Regex& parent_arg() {
    static const Regex re(R"rx("[^"]+",[^,]+,[^,]+,[^,]+,\s*([A-Za-z_]\w*)\s*\))rx");
    return re;
}
// the 5th argument: the icon source, Item::s_book / Block::s_wood / ...
const Regex& icon_arg() {
    static const Regex re(R"rx("[^"]+"\s*,[^,]+,[^,]+,\s*((?:Item|Block)::s_\w+))rx");
    return re;
}

// `Item::s_book = <definition>;`  (the definition on one line)
const Regex& item_definition() {
    static const Regex re(R"rx(Item::(s_\w+)\s*=\s*(.+?);)rx");
    return re;
}
const Regex& icon_coord() {
    static const Regex re(R"rx(setIconCoord\(\s*(\d+)\s*,\s*(\d+)\s*\))rx");
    return re;
}
const Regex& icon_index() {
    static const Regex re(R"rx(setIconIndex\(\s*(\d+)\s*\))rx");
    return re;
}
// `Block::s_wood = (new BlockClass(args)...`
const Regex& block_assignment() {
    static const Regex re(R"rx(Block::(s_\w+)\s*=\s*\(new\s+(\w+)\(([^)]*)\))rx");
    return re;
}
const Regex& block_index_literal() {
    static const Regex re(R"rx(blockIndexInTexture\s*=\s*(\d+)\s*;)rx");
    return re;
}

std::string read_or_empty(const stdfs::path& path) { return textutil::try_read_text(path).value_or(std::string()); }

std::vector<std::string_view> lines_of(std::string_view text) { return text::split(text, '\n'); }

std::string xml_escape(std::string_view s) {
    std::string out = text::replace_all(s, "&", "&amp;");
    out = text::replace_all(out, "<", "&lt;");
    out = text::replace_all(out, ">", "&gt;");
    return text::replace_all(out, "\"", "&quot;");
}

std::string_view or_default(const std::optional<std::string>& value, std::string_view fallback) {
    return value && !value->empty() ? std::string_view(*value) : fallback;
}

}  // namespace

// ---- title id ----------------------------------------------------------------------------------------------

std::optional<std::string> normalize_title_id(std::string_view tid) {
    std::string s = text::lower(text::trim(tid));
    std::string_view digits = s;
    if (digits.starts_with("0x")) digits.remove_prefix(2);
    if (digits.empty() || digits.size() > 8 || !std::all_of(digits.begin(), digits.end(), is_hex)) return std::nullopt;
    const auto value = text::parse_int<std::uint32_t>(digits, 16);
    if (!value) return std::nullopt;
    return std::format("{:08X}", *value);
}

std::string suggest_title_id(std::string_view seed) {
    const std::uint32_t number = crypto::crc32(as_bytes(seed.empty() ? std::string_view("title") : seed)) & 0xFFFFu;
    return std::format("{:04X}{:04X}", kHomebrewPublisher, number);
}

std::optional<std::string> title_id_from_config(const stdfs::path& out_dir) {
    const stdfs::path path = out_dir / "platform.config.json";
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return std::nullopt;
    const std::optional<std::string> source = textutil::try_read_text(path);
    if (!source) return std::nullopt;
    Json config;
    if (!json::try_parse(*source, config) || !config.is_object()) return std::nullopt;
    const auto it = config.find("title_id");
    if (it == config.end()) return std::nullopt;
    if (it->is_string()) {
        const std::string text = it->get<std::string>();
        return text.empty() ? std::nullopt : std::optional<std::string>(text);
    }
    // A whole number is read as its decimal digits, which normalize_title_id then takes as hexadecimal (a quirk of
    // the config reader that the vcxproj writer shares: the two must agree on the id). Zero means "none".
    if (it->is_number_integer()) {
        const auto n = it->get<std::int64_t>();
        return n == 0 ? std::nullopt : std::optional<std::string>(std::to_string(n));
    }
    return std::nullopt;   // null, a float, a list, an object, a boolean: never a title id
}

// ---- scanning the program ----------------------------------------------------------------------------------

std::string humanize(std::string_view key) {
    std::string s = text::replace_all(key, "_", " ");
    s = split_camel(s, ' ');
    // an upper-case letter that starts a capitalised word after another upper-case letter: HTMLParser -> HTML Parser
    std::string spaced;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (i > 0 && i + 1 < s.size() && is_upper(s[i - 1]) && is_upper(s[i]) && is_lower(s[i + 1])) spaced += ' ';
        spaced += s[i];
    }
    if (spaced.empty()) return std::string(key);
    if (is_lower(spaced[0])) spaced[0] = static_cast<char>(spaced[0] - 'a' + 'A');
    return spaced;
}

std::map<std::string, LangName> load_lang_names(const stdfs::path& lang_file) {
    std::map<std::string, LangName> out;
    const std::optional<std::string> source = textutil::try_read_text(lang_file);
    if (!source) return out;
    constexpr std::string_view kPrefix = "achievement.";
    for (const std::string_view line : lines_of(*source)) {
        const std::size_t eq = line.find('=');
        if (eq == std::string_view::npos || !line.starts_with(kPrefix)) continue;
        const std::string_view rest = line.substr(kPrefix.size(), eq - kPrefix.size());
        const std::string value(line.substr(eq + 1));
        if (rest.ends_with(".desc")) {
            out[std::string(rest.substr(0, rest.size() - 5))].desc = value;
        } else {
            out[std::string(rest)].name = value;
        }
    }
    return out;
}

std::vector<Achievement> scan_achievements(const stdfs::path& code_dir, const std::optional<stdfs::path>& lang_file) {
    const std::map<std::string, LangName> names = lang_file ? load_lang_names(*lang_file) : std::map<std::string, LangName>();
    std::map<std::int64_t, Achievement> found;
    for (const stdfs::path& file : tree::files_with_extension(code_dir, {".cpp", ".java"})) {
        const std::string source = read_or_empty(file);   // (an unreadable file has no achievements)
        if (source.find("new Achievement(") == std::string::npos) continue;
        for (const std::string_view line : lines_of(source)) {
            const std::optional<RegexMatch> m = new_achievement().search(line);
            if (!m) continue;
            const auto id = text::parse_int<std::int64_t>(m->group(1));
            if (!id) continue;   // an id that does not fit 64 bits is not an achievement id
            Achievement a;
            a.id = *id;
            a.key = std::string(m->group(2));
            const auto lang = names.find(a.key);
            const std::optional<std::string> lang_name = lang != names.end() ? lang->second.name : std::nullopt;
            const std::optional<std::string> lang_desc = lang != names.end() ? lang->second.desc : std::nullopt;
            a.name = std::string(or_default(lang_name, humanize(a.key)));
            a.desc = lang_desc && !lang_desc->empty() ? *lang_desc : std::format("Earn the {} achievement.", humanize(a.key));
            a.hidden = special_flag().test(line);
            if (const auto pm = parent_arg().search(line)) {
                const std::string parent(pm->group(1));
                if (parent != "NULL" && parent != "null") a.parent = parent;
            }
            if (const auto im = icon_arg().search(line)) a.icon = std::string(im->group(1));
            found.insert_or_assign(a.id, std::move(a));
        }
    }
    std::vector<Achievement> result;
    result.reserve(found.size());
    for (auto& entry : found) result.push_back(std::move(entry.second));
    return result;
}

// ---- icon resolution: symbol -> atlas sprite ---------------------------------------------------------------

std::string_view atlas_name(Atlas a) { return a == Atlas::Items ? "items" : "terrain"; }

namespace {

// {BlockSubclass: terrain index} from each subclass constructor's literal `blockIndexInTexture = N;`. The base
// `Block` sets it from a constructor argument (no literal) and is intentionally absent.
std::map<std::string, std::uint32_t> block_class_textures(const stdfs::path& code_dir) {
    std::map<std::string, std::uint32_t> out;
    for (const stdfs::path& file : tree::files_with_extension(code_dir, {".cpp"})) {
        const std::string name = fs::to_text(file.filename());
        if (!name.starts_with("Block") || name == "Block.cpp") continue;
        const std::string source = read_or_empty(file);
        if (const auto m = block_index_literal().search(source)) {
            if (const auto n = text::parse_int<std::uint32_t>(m->group(1))) out[name.substr(0, name.size() - 4)] = *n;
        }
    }
    return out;
}

Sprite sprite_of_index(Atlas atlas, std::uint32_t index) { return Sprite{atlas, index % 16, index / 16}; }

}  // namespace

std::map<std::string, Sprite> resolve_icon_atlas(const stdfs::path& code_dir) {
    std::map<std::string, Sprite> out;
    const std::map<std::string, std::uint32_t> class_tex = block_class_textures(code_dir);
    std::optional<stdfs::path> item_cpp, block_cpp;
    for (const stdfs::path& file : tree::files_with_extension(code_dir, {".cpp"})) {
        const std::string name = fs::to_text(file.filename());
        if (name == "Item.cpp") item_cpp = file;
        if (name == "Block.cpp") block_cpp = file;
    }
    if (item_cpp) {
        const std::string source = read_or_empty(*item_cpp);
        item_definition().for_each_match(source, [&](const RegexMatch& m) {
            const std::string symbol = "Item::" + std::string(m[1]);
            const std::string_view body = m[2];
            if (const auto c = icon_coord().search(body)) {
                const auto col = text::parse_int<std::uint32_t>(c->group(1)), row = text::parse_int<std::uint32_t>(c->group(2));
                if (col && row) out[symbol] = Sprite{Atlas::Items, *col, *row};
            } else if (const auto i = icon_index().search(body)) {
                if (const auto n = text::parse_int<std::uint32_t>(i->group(1))) out[symbol] = sprite_of_index(Atlas::Items, *n);
            }
            return true;
        });
    }
    if (block_cpp) {
        const std::string source = read_or_empty(*block_cpp);
        block_assignment().for_each_match(source, [&](const RegexMatch& m) {
            const std::string symbol = "Block::" + std::string(m[1]);
            std::optional<std::uint32_t> tex;
            if (const auto it = class_tex.find(std::string(m[2])); it != class_tex.end()) {
                tex = it->second;   // a subclass: the literal index in its constructor
            } else {
                // the base Block(id, tex, mat): the second argument, if it is a literal
                const std::vector<std::string_view> args = text::split(m[3], ',');
                if (args.size() >= 2) tex = text::parse_int<std::uint32_t>(text::trim(args[1]));
            }
            if (tex) out[symbol] = sprite_of_index(Atlas::Terrain, *tex);
            return true;
        });
    }
    return out;
}

// ---- the platform mapping ----------------------------------------------------------------------------------

TierLimits limits(Tier tier) { return tier == Tier::Retail ? TierLimits{99, 1000} : TierLimits{12, 200}; }

std::string_view tier_name(Tier tier) { return tier == Tier::Retail ? "retail" : "arcade"; }

std::optional<Tier> parse_tier(std::string_view name) {
    if (name == "retail") return Tier::Retail;
    if (name == "arcade") return Tier::Arcade;
    return std::nullopt;
}

std::vector<std::int64_t> distribute_score(std::int64_t n, std::int64_t total) {
    if (n <= 0) return {};
    const std::int64_t base = total / n / 5 * 5;
    std::vector<std::int64_t> scores(static_cast<std::size_t>(n), base);
    std::int64_t remaining = total - base * n;
    for (std::size_t i = 0; remaining > 0 && i < scores.size(); ++i, remaining -= 5) scores[i] += 5;
    // if the step was 0 (n huge), fall back to whole shares so nothing is zero-valued
    if (base == 0 && total >= n) {
        std::fill(scores.begin(), scores.end(), total / n);
        std::int64_t sum = 0;
        for (const std::int64_t s : scores) sum += s;
        scores.back() += total - sum;
    }
    return scores;
}

Mapping map_to_platform(std::span<const Achievement> achievements, Tier tier) {
    const TierLimits lim = limits(tier);
    const auto n = static_cast<std::int64_t>(achievements.size());
    Mapping m;
    m.tier = tier;
    m.count = n;
    m.limit = lim.max_count;
    if (n > lim.max_count) {
        m.warnings.push_back(std::format("{} achievements exceeds the {} limit of {} -- split across titles or raise the profile.",
                                         n, tier_name(tier), lim.max_count));
    }
    const std::vector<std::int64_t> scores = distribute_score(n, lim.score);
    for (std::size_t i = 0; i < achievements.size(); ++i) {
        const Achievement& a = achievements[i];
        Entry e;
        e.platform_id = static_cast<std::int64_t>(i) + 1;   // consoles number achievements from 1
        e.source_id = a.id;
        e.key = a.key;
        e.name = a.name;
        e.desc = a.desc;
        e.gamerscore = scores[i];
        e.hidden = a.hidden;
        e.icon = a.icon;
        m.total_score += e.gamerscore;
        m.entries.push_back(std::move(e));
    }
    if (m.total_score != lim.score && n > 0) {
        m.warnings.push_back(std::format("gamerscore sums to {}, not the {} target of {}.", m.total_score, tier_name(tier), lim.score));
    }
    return m;
}

// ---- tile art ----------------------------------------------------------------------------------------------

namespace {

// A locked-achievement tile: desaturated (the luma of ITU-R 601-2, as Pillow's greyscale conversion) and darkened to
// 40%, alpha kept.
rgba_png::Image locked_variant(const rgba_png::Image& img) {
    rgba_png::Image out = img;
    for (std::uint32_t y = 0; y < out.height; ++y) {
        for (std::uint32_t x = 0; x < out.width; ++x) {
            std::uint8_t* p = out.pixel(x, y);
            const std::uint32_t luma = (p[0] * 19595u + p[1] * 38470u + p[2] * 7471u + 0x8000u) >> 16;
            p[0] = p[1] = p[2] = static_cast<std::uint8_t>(luma * 40 / 100);
        }
    }
    return out;
}

stdfs::path atlas_file(Atlas a) { return a == Atlas::Items ? stdfs::path("gui") / "items.png" : stdfs::path("terrain.png"); }

// `path` relative to `base`, with backslashes (as the XLAST project wants them); the absolute path if the two
// have no common root.
std::string relative_with_backslashes(const stdfs::path& path, const stdfs::path& base) {
    const stdfs::path abs_path = stdfs::absolute(path).lexically_normal();
    const stdfs::path rel = abs_path.lexically_relative(stdfs::absolute(base).lexically_normal());
    return text::replace_all(fs::to_text(rel.empty() ? abs_path : rel), "/", "\\");
}

}  // namespace

IconWiring wire_icons(std::vector<Entry>& entries, const stdfs::path& code_dir, const stdfs::path& content_dir,
                      const stdfs::path& out_dir, std::uint32_t tile) {
    IconWiring result;
    const std::map<std::string, Sprite> sprites = resolve_icon_atlas(code_dir);
    // an atlas is read once; one that cannot be read leaves its reason for every entry that needs it
    struct Loaded {
        std::optional<rgba_png::Image> image;
        std::string problem;
    };
    std::map<Atlas, Loaded> atlases;
    // The PNGs go UNDER the deploy Content dir (Debug\Content\achievement_images), never the project root; the
    // references stored in each entry are made relative to out_dir (where achievements.xlast lives) so the XLAST
    // tool still resolves them.
    const stdfs::path image_dir = content_dir / "achievement_images";

    for (Entry& e : entries) {
        const auto sprite = e.icon ? sprites.find(*e.icon) : sprites.end();
        if (sprite == sprites.end()) {
            result.unresolved.push_back({e.key, std::format("icon {} not resolvable to an atlas sprite", or_default(e.icon, "(none)"))});
            continue;
        }
        const Atlas atlas = sprite->second.atlas;
        auto loaded = atlases.find(atlas);
        if (loaded == atlases.end()) {
            Loaded l;
            const stdfs::path path = content_dir / atlas_file(atlas);
            std::error_code ec;
            if (!stdfs::is_regular_file(path, ec)) {
                l.problem = std::format("atlas {} missing at {}", atlas_name(atlas), fs::to_text(path));
            } else {
                try {   // one atlas failing is a verdict on its entries, not on the others
                    l.image = rgba_png::decode(fs::read_file(path));
                } catch (const Error& error) {
                    l.problem = std::format("atlas {} unreadable at {}: {}", atlas_name(atlas), fs::to_text(path), error.what());
                }
            }
            loaded = atlases.emplace(atlas, std::move(l)).first;
        }
        if (!loaded->second.image) {
            result.unresolved.push_back({e.key, loaded->second.problem});
            continue;
        }
        const std::uint32_t left = sprite->second.col * 16, top = sprite->second.row * 16;
        const rgba_png::Image tile_image = rgba_png::resize_nearest(rgba_png::crop(*loaded->second.image, left, top, left + 16, top + 16), tile);
        const stdfs::path unlocked = image_dir / (e.key + "_unlocked.png");
        const stdfs::path locked = image_dir / (e.key + "_locked.png");
        fs::write_file(unlocked, rgba_png::encode(tile_image));
        fs::write_file(locked, rgba_png::encode(locked_variant(tile_image)));
        e.art = TileArt{relative_with_backslashes(unlocked, out_dir), relative_with_backslashes(locked, out_dir)};
        result.wired.push_back(e.key);
    }
    return result;
}

std::optional<stdfs::path> guess_content_dir(const stdfs::path& out_dir) {
    // Debug is checked first: the asset pipeline writes assets to <out_dir>\Debug\Content, which IS `game:\Content`
    // (the DeployContent target mirrors the other configurations from it).
    const std::array<stdfs::path, 5> candidates = {out_dir / "Debug" / "Content", out_dir / "Content", out_dir,
                                                   out_dir / "Release" / "Content", out_dir / "Release_LTCG" / "Content"};
    std::error_code ec;
    for (const stdfs::path& dir : candidates) {
        if (stdfs::is_regular_file(dir / "terrain.png", ec) || stdfs::is_regular_file(dir / "gui" / "items.png", ec)) return dir;
    }
    return std::nullopt;
}

// ---- emission ----------------------------------------------------------------------------------------------

std::string const_name(std::string_view key) {
    std::string out = split_camel(key, '_');
    for (char& c : out) c = is_lower(c) ? static_cast<char>(c - 'a' + 'A') : c;
    return "ACH_" + out;
}

std::string render_report(const Mapping& mapping, std::string_view delivery, const IconWiring* icons,
                          std::string_view title_id, bool title_suggested) {
    std::vector<std::string> L = {
        "# ACHIEVEMENTS.md -- achievement conversion",
        "",
        std::format("The program defines {} achievement(s). A console does NOT let code invent achievements at runtime -- "
                    "each must be declared in the title configuration and unlocked through a platform call that writes "
                    "to the signed-in profile over the online service. This maps the program's achievements onto that "
                    "model.",
                    mapping.count),
        "",
        std::format("- profile: **{}** (max {}, {} gamerscore)   assigned total: **{}**   delivery: **{}**", tier_name(mapping.tier),
                    mapping.limit, limits(mapping.tier).score, mapping.total_score, delivery),
        "",
    };
    for (const std::string& w : mapping.warnings) L.push_back("> WARNING: " + w);
    if (!mapping.warnings.empty()) L.push_back("");
    for (const char* line : {"## Achievement table", "", "| # | Key | Name | Gamerscore | Hidden | Tile art |",
                             "|---:|---|---|---:|:---:|---|"}) {
        L.emplace_back(line);
    }
    for (const Entry& x : mapping.entries) {
        const std::string art = x.art ? "`" + x.art->unlocked + "`" : "TODO";
        L.push_back(std::format("| {} | `{}` | {} | {} | {} | {} |", x.platform_id, x.key, x.name, x.gamerscore, x.hidden ? "yes" : "", art));
    }
    if (icons != nullptr) {
        L.push_back("");
        L.push_back("### Tile art");
        L.push_back("");
        L.push_back("Achievement tiles were auto-wired from the game's own item/block icons (cropped from the texture "
                    "atlas, scaled to 64x64; a darkened/desaturated locked variant is generated). They land in "
                    "`achievement_images/` and are referenced by the XLAST project.");
        L.push_back("");
        L.push_back(std::format("- auto-wired: **{}**   needs manual art: **{}**", icons->wired.size(), icons->unresolved.size()));
        if (!icons->unresolved.empty()) {
            L.push_back("");
            L.push_back("Icons not auto-resolved (add art by hand in the XLAST project):");
            L.push_back("");
            for (const Unresolved& u : icons->unresolved) L.push_back(std::format("- `{}` \xE2\x80\x94 {}", u.key, u.reason));
        }
        L.push_back("");
    }
    const std::string id(title_id.empty() ? kUnsetTitleId : title_id);
    for (const char* line : {
             "",
             "## What the console requires (and the source program does not provide)",
             "",
             "1. **Declared up front.** Every achievement above must be entered into the title's configuration tool "
             "(Xbox 360: an XLAST project, compiled to an SPA blob embedded in the build). A skeleton is emitted as "
             "`achievements.xlast` -- import it, add the locked/unlocked tile art, and build the SPA.",
             "2. **Unlocked via a networked write.** Each in-code unlock becomes a call to the platform achievement write "
             "(Xbox 360: `XUserWriteAchievements`). This is the telemetry/networking path you flagged: it needs",
             "   - a **Title ID** assigned to the build,",
             "   - a **signed-in profile** (tie this to the *User / profile* decision),",
             "   - the **network** for immediate sync -- **offline it queues and syncs on next connect**.",
             "3. **Budget + limits.** Gamerscore must sum to the profile target and the count must stay under the limit "
             "(validated above).",
             "4. **Hidden achievements.** The program's \"special/secret\" flag maps to the console \"hidden\" attribute "
             "(shown above).",
             "",
             "## Title ID",
             "",
             "The achievement config (SPA) and the built XEX are both keyed to a **32-bit Title ID** (high 16 bits = "
             "publisher, low 16 = title number). It must be the SAME in the XLAST project and on the XEX, or unlocks "
             "will not bind.",
             ""}) {
        L.emplace_back(line);
    }
    L.push_back(std::format("- Title ID: **0x{}**{}", id, title_suggested ? "  _(suggested \xE2\x80\x94 see below)_" : ""));
    for (const char* line : {
             "",
             "Since Xbox 360 titles are no longer submitted to Microsoft, you **self-issue** this: there is no database to "
             "request one from. Pick a value and make sure it does not collide with an existing title (community "
             "title-ID lists exist to check against). The suggested id uses a high publisher prefix (`0xFFFE`) unlikely "
             "to clash and a title number derived from the name; override it with your own if you prefer.",
             "",
             "Set the same value in **Visual Studio -> project properties -> Xbox 360 -> Title ID** (it is written into "
             "the XEX at build), and in the XLAST project (already set in the emitted `achievements.xlast`).",
             "",
             "## Code seam",
             "",
             "Emitted `Achievements.h/.cpp` provides `Achievement_Unlock(userIndex, ACH_*)`. The converter routes each "
             "in-code unlock (e.g. the program's `triggerAchievement`) to it; on hardware it performs the platform "
             "write, off-hardware it is a safe no-op. The `ACH_*` id constants match the table above.",
             ""}) {
        L.emplace_back(line);
    }
    if (delivery != kDeliveryOnline) {
        L.push_back(std::format("> NOTE: delivery is **{}** -- unlocks are tracked locally only, no online write. Switch the "
                                "*Achievements* decision to `xbox_live` for real profile achievements.", delivery));
        L.push_back("");
    }
    return text::join(L, "\n") + "\n";
}

void emit_xlast(const Mapping& mapping, const stdfs::path& out_path, std::string_view title_id) {
    const std::string tid = normalize_title_id(title_id).value_or(std::string(kUnsetTitleId));
    std::vector<std::string> L = {
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>",
        "<!-- Achievement config skeleton emitted by CodeBreaker. Import into the platform submission/config",
        "     tool (Xbox 360: XLAST), then build the SPA. The titleId below MUST match the Title ID set",
        "     on the XEX in the Visual Studio project (Xbox 360 property pages). -->",
        std::format("<XLASTProject titleId=\"0x{}\">", tid),
        "  <Configuration>",
        "    <Achievements>",
    };
    for (const Entry& x : mapping.entries) {
        L.push_back(std::format("      <Achievement id=\"{}\" name=\"{}\" gamerscore=\"{}\" hidden=\"{}\">", x.platform_id,
                                xml_escape(x.name), x.gamerscore, x.hidden ? "true" : "false"));
        L.push_back(std::format("        <LockedDescription>{}</LockedDescription>", xml_escape(x.desc)));
        L.push_back(std::format("        <UnlockedDescription>{}</UnlockedDescription>", xml_escape(x.desc)));
        if (x.art) {
            L.push_back(std::format("        <Image type=\"unlocked\">{}</Image>", xml_escape(x.art->unlocked)));
            L.push_back(std::format("        <Image type=\"locked\">{}</Image>", xml_escape(x.art->locked)));
        } else {
            L.emplace_back("        <!-- TODO: add locked/unlocked 64x64 tile (icon not auto-resolved) -->");
        }
        L.emplace_back("      </Achievement>");
    }
    for (const char* line : {"    </Achievements>", "  </Configuration>", "</XLASTProject>", ""}) L.emplace_back(line);
    textutil::write_text(out_path, text::join(L, "\n"));
}

std::vector<stdfs::path> emit_code(const Mapping& mapping, const stdfs::path& out_dir, std::string_view delivery) {
    std::vector<std::string> h = {
        "// Achievements.h -- CodeBreaker-emitted achievement ids + unlock seam. See ACHIEVEMENTS.md.",
        "#ifndef J2X_ACHIEVEMENTS_H",
        "#define J2X_ACHIEVEMENTS_H",
        "",
        "// Platform achievement ids (match the title config / XLAST project).",
    };
    for (const Entry& x : mapping.entries) h.push_back(std::format("#define {:<28} {}", const_name(x.key), x.platform_id));
    for (const char* line : {"", "// Unlock an achievement for a signed-in user. On hardware this writes to the profile over",
                             "// the online service (queues offline); off-hardware it is a no-op returning true.",
                             "bool Achievement_Unlock(int userIndex, int achievementId);", "", "#endif // J2X_ACHIEVEMENTS_H", ""}) {
        h.emplace_back(line);
    }

    std::vector<std::string> c = {
        "// Achievements.cpp -- CodeBreaker-emitted. Routes in-code unlocks to the platform write.",
        "#include \"stdafx.h\"",
        "#include \"Achievements.h\"",
        "",
        "#if defined(_XBOX) || defined(_XENON)",
        "  #include <xtl.h>",
        "#endif",
        "#include <string.h>",
        "",
        "bool Achievement_Unlock(int userIndex, int achievementId)",
        "{",
    };
    if (delivery == kDeliveryOnline) {
        for (const char* line : {"#if defined(_XBOX) || defined(_XENON)", "    XUSER_ACHIEVEMENT ach;",
                                 "    ach.dwUserIndex = (DWORD)userIndex;", "    ach.dwAchievementId = (DWORD)achievementId;",
                                 "    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));", "    // async LIVE write; queues + syncs later if offline.",
                                 "    DWORD r = XUserWriteAchievements(1, &ach, &ov);",
                                 "    if (r != ERROR_IO_PENDING && r != ERROR_SUCCESS) return false;",
                                 "    while (!XHasOverlappedIoCompleted(&ov)) { }",
                                 "    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;", "#else",
                                 "    (void)userIndex; (void)achievementId; return true;", "#endif"}) {
            c.emplace_back(line);
        }
    } else {
        c.push_back(std::format("    // J2X-STUB[achievements-local]: {} delivery -- track locally, no online write.", delivery));
        c.emplace_back("    (void)userIndex; (void)achievementId; return true;");
    }
    c.emplace_back("}");
    c.emplace_back("");

    const stdfs::path header_path = out_dir / "Achievements.h";
    const stdfs::path source_path = out_dir / "Achievements.cpp";
    textutil::write_text(header_path, text::join(h, "\n"));
    textutil::write_text(source_path, text::join(c, "\n"));
    return {header_path, source_path};
}

// ---- the whole pass ----------------------------------------------------------------------------------------

RunResult run(const stdfs::path& code_dir, const stdfs::path& out_dir, const RunOptions& options) {
    fs::make_dirs(out_dir);
    const std::vector<Achievement> achievements = scan_achievements(code_dir, options.lang_file);
    RunResult r;
    r.mapping = map_to_platform(achievements, options.tier);
    r.count = r.mapping.count;
    r.content_dir = options.content_dir ? options.content_dir : guess_content_dir(out_dir);
    if (r.content_dir) {
        r.icons = wire_icons(r.mapping.entries, code_dir, *r.content_dir, out_dir);
    } else {
        // Say WHY, per entry. Reporting zero wired AND zero unresolved would read as "nothing to do" -- exactly how 16
        // TODO tiles once went unnoticed. A missing atlas source is an unresolved icon, not an absence of work.
        for (const Entry& e : r.mapping.entries) {
            r.icons.unresolved.push_back({e.key, std::format("no Content dir with terrain.png / gui/items.png found near {} (run "
                                                              "`codebreaker java assets` first, or pass --content-dir)", fs::to_text(out_dir))});
        }
    }
    // Title ID, in precedence order: an explicit one, else the project's platform.config.json (the ONE place it
    // lives -- `codebreaker java vcxproj` stamps the SAME value into the XEX, and the two MUST match or unlocks never
    // bind to a profile), else the conventional "unset" placeholder 00000000. One is never issued on the user's
    // behalf: they pick it (the Suggest button / --title-id).
    std::optional<std::string> tid = options.title_id ? normalize_title_id(*options.title_id) : std::nullopt;
    if (!tid) {
        const std::optional<std::string> configured = title_id_from_config(out_dir);
        tid = configured ? normalize_title_id(*configured) : std::nullopt;
    }
    r.title_id = tid.value_or(std::string(kUnsetTitleId));
    r.report = out_dir / "ACHIEVEMENTS.md";
    textutil::write_text(r.report, render_report(r.mapping, options.delivery, &r.icons, r.title_id, r.title_suggested));
    r.xlast = out_dir / "achievements.xlast";
    emit_xlast(r.mapping, r.xlast, r.title_id);
    r.code = emit_code(r.mapping, out_dir, options.delivery);
    return r;
}

}  // namespace cb::java::projects::minecraft::achievements
