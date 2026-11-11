// DIVERGES from the Python (the fixture's expected/ files are what the Python module wrote for the same inputs and are
// compared byte for byte, the PNG tiles pixel for pixel):
//   * icons are keyed by the whole symbol ("Item::s_sign" and "Block::s_sign" are two icons); the Python keyed them by the
//     bare symbol, so a block silently took the sprite of an item of the same name.
//   * the profile is an enum (retail | arcade): an unknown name used to give retail limits in map_to_platform and then a
//     KeyError in the report; the caller now has to say which one (parse_tier).
//   * an atlas that is not a readable PNG leaves ITS entries unresolved, with the reason, instead of ending the whole run with
//     an exception; the other atlas is still used.
//   * an achievement id that does not fit 64 bits is skipped (the Python's int was unbounded).
//   * Pillow clips a 16-bit grey to 255 when it converts to RGBA; the PNG reader keeps the high byte (rgba_png_test).
//   * kept on purpose: a whole-number title_id in platform.config.json is read as its decimal digits and then as hexadecimal,
//     the same rule the vcxproj writer uses -- the two must agree on the id.

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/achievements.hpp"
#include "cb/java/rgba_png.hpp"

namespace {

using namespace cb;
using namespace cb::java::projects::minecraft::achievements;
namespace png = cb::java::rgba_png;
namespace stdfs = std::filesystem;
using Scores = std::vector<std::int64_t>;

// testdata/achievements: a converted tree in the style of the Alpha port (AchievementList.cpp, Item.cpp, Block.cpp,
// BlockWorkbench.cpp, plus a Java file), a .lang file, and the two atlases (a grey ramp: pixel (x, y) is
// ((x + y) & 255) with alpha 200). testdata/achievements/expected holds what the Python module wrote for the same
// inputs (read and checked by hand); the PNG tiles compare by pixel, not by file bytes.
stdfs::path fixture(const std::string& rel = "") { return testing::data_path("java/projects/minecraft/testdata/achievements/" + rel); }

// A copy of the fixture as code/, content/, lang/ side by side in a temp folder (run() writes into it).
void copy_fixture(const stdfs::path& to) {
    for (const char* part : {"code", "content", "lang"}) fs::copy_tree(fixture(part), to / part);
}

const Achievement* find(const std::vector<Achievement>& all, std::string_view key) {
    for (const Achievement& a : all) {
        if (a.key == key) return &a;
    }
    return nullptr;
}

CB_TEST("java.minecraft.achievements.normalize_title_id", [] {
    // (every row checked against the Python module)
    CB_CHECK((normalize_title_id("FFFE1234")) == (std::optional<std::string>("FFFE1234")));
    CB_CHECK((normalize_title_id("0xFFFE1234")) == (std::optional<std::string>("FFFE1234")));
    CB_CHECK((normalize_title_id("fffe1234")) == (std::optional<std::string>("FFFE1234")));
    CB_CHECK((normalize_title_id("  0XaB  ")) == (std::optional<std::string>("000000AB")));   // blanks go, short ids are padded
    CB_CHECK((normalize_title_id("1")) == (std::optional<std::string>("00000001")));
    CB_CHECK((normalize_title_id("00000000")) == (std::optional<std::string>("00000000")));
    CB_CHECK((normalize_title_id("ffffffff")) == (std::optional<std::string>("FFFFFFFF")));
    for (const char* bad : {"123456789", "xyz", "", "0x", "FFFE 12", "0x0x1", "-1", "+5", "0xFFFE12345"}) {
        CB_CHECK(!normalize_title_id(bad));
    }
});

CB_TEST("java.minecraft.achievements.suggest_title_id", [] {
    // the homebrew publisher and a CRC-32 derived title number (Python's zlib.crc32 & 0xFFFF)
    CB_CHECK_EQ(suggest_title_id("Minecraft"), "FFFE5FA6");
    CB_CHECK_EQ(suggest_title_id("MinecraftAlpha"), "FFFE88C9");
    CB_CHECK_EQ(suggest_title_id("Caf\xC3\xA9"), "FFFE6DB1");     // the CRC is over the UTF-8 bytes
    CB_CHECK_EQ(suggest_title_id(""), "FFFE786B");
    CB_CHECK_EQ(suggest_title_id("title"), "FFFE786B");        // an empty seed is "title"
    CB_CHECK(normalize_title_id(suggest_title_id("Anything")).has_value());
});

CB_TEST("java.minecraft.achievements.humanize_and_const_name", [] {
    struct Row {
        const char* key;
        const char* human;
        const char* constant;
    };
    for (const Row& r : {Row{"openInventory", "Open Inventory", "ACH_OPEN_INVENTORY"}, Row{"onARail", "On A Rail", "ACH_ON_ARAIL"},
                         Row{"HTMLParser", "HTML Parser", "ACH_HTMLPARSER"}, Row{"a_b_c", "A b c", "ACH_A_B_C"},
                         Row{"killEnemy", "Kill Enemy", "ACH_KILL_ENEMY"}, Row{"x", "X", "ACH_X"},
                         Row{"ABC", "ABC", "ACH_ABC"}, Row{"getABCdef", "Get AB Cdef", "ACH_GET_ABCDEF"},
                         Row{"snake_caseKey", "Snake case Key", "ACH_SNAKE_CASE_KEY"}, Row{"iron_Tools", "Iron Tools", "ACH_IRON_TOOLS"},
                         Row{"a1B2", "A1B2", "ACH_A1B2"}, Row{"findDiamonds2", "Find Diamonds2", "ACH_FIND_DIAMONDS2"}}) {
        CB_CHECK_EQ(humanize(r.key), r.human);
        CB_CHECK_EQ(const_name(r.key), r.constant);
    }
    CB_CHECK_EQ(humanize(""), "");
});

CB_TEST("java.minecraft.achievements.distribute_score", [] {
    // steps of 5 that add up to the total exactly; the first few take the remainder (Python's results)
    CB_CHECK((distribute_score(7, 1000)) == ((Scores{145, 145, 145, 145, 140, 140, 140})));
    CB_CHECK((distribute_score(12, 200)) == ((Scores{20, 20, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15})));
    CB_CHECK((distribute_score(13, 200)) == ((Scores{20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15})));
    CB_CHECK((distribute_score(3, 1000)) == ((Scores{335, 335, 330})));
    CB_CHECK((distribute_score(1, 1000)) == (Scores{1000}));
    CB_CHECK((distribute_score(7, 200)) == ((Scores{30, 30, 30, 30, 30, 25, 25})));
    CB_CHECK(distribute_score(0, 1000).empty());
    CB_CHECK(distribute_score(-4, 1000).empty());
    for (const std::int64_t n : {2, 5, 30, 50, 99, 100, 199, 200, 201, 500, 999, 1000}) {
        const Scores s = distribute_score(n, 1000);
        CB_CHECK_EQ(s.size(), static_cast<std::size_t>(n));
        std::int64_t sum = 0;
        for (const std::int64_t v : s) sum += v;
        CB_CHECK_EQ(sum, std::int64_t{1000});
    }
    // so many achievements that the step would be 0: whole shares, the last one takes the rest
    const Scores many = distribute_score(1000, 1000);
    CB_CHECK(many.front() == 1 && many.back() == 1);
    CB_CHECK_EQ(distribute_score(1001, 1000).back(), std::int64_t{0});    // more achievements than points: some get nothing
});

CB_TEST("java.minecraft.achievements.tiers", [] {
    CB_CHECK_EQ(limits(Tier::Retail).max_count, std::int64_t{99});
    CB_CHECK_EQ(limits(Tier::Retail).score, std::int64_t{1000});
    CB_CHECK_EQ(limits(Tier::Arcade).max_count, std::int64_t{12});
    CB_CHECK_EQ(limits(Tier::Arcade).score, std::int64_t{200});
    CB_CHECK_EQ(tier_name(Tier::Arcade), "arcade");
    CB_CHECK(parse_tier("retail") == Tier::Retail && parse_tier("arcade") == Tier::Arcade);
    CB_CHECK(!parse_tier("Retail") && !parse_tier("gold") && !parse_tier(""));
});

CB_TEST("java.minecraft.achievements.load_lang_names", [] {
    const auto names = load_lang_names(fixture("lang/en_US.lang"));
    CB_CHECK_EQ(names.size(), std::size_t{6});         // only `achievement.*` lines count
    CB_CHECK_EQ(*names.at("openInventory").name, "Taking Inventory");
    CB_CHECK_EQ(*names.at("openInventory").desc, "Press 'E' to open your inventory.");
    CB_CHECK(!names.at("mineWood").desc);
    CB_CHECK(!names.at("diamonds").name && names.at("diamonds").desc);
    CB_CHECK_EQ(*names.at("flyPig").name, "When Pigs Fly = yes");        // the value is what follows the FIRST '='
    CB_CHECK_EQ(*names.at("cookFish").name, "");                          // present but empty: scan treats it as absent
    CB_CHECK(names.contains("unknown"));
    CB_CHECK(!names.contains("menu.singleplayer"));
    CB_CHECK(load_lang_names(fixture("lang/missing.lang")).empty());
    CB_CHECK(load_lang_names(fixture("lang")).empty());                   // a folder is not a lang file
});

CB_TEST("java.minecraft.achievements.scan_achievements", [] {
    const auto all = scan_achievements(fixture("code"), fixture("lang/en_US.lang"));
    // ids 0..6, once each and in id order; the Java file's id 0 replaces the C++ one (a later file wins)
    CB_CHECK_EQ(all.size(), std::size_t{7});
    for (std::size_t i = 0; i < all.size(); ++i) CB_CHECK_EQ(all[i].id, static_cast<std::int64_t>(i));

    const Achievement& inv = all[0];
    CB_CHECK_EQ(inv.key, "openInventory");
    CB_CHECK_EQ(inv.name, "Taking Inventory");                 // from the .lang file
    CB_CHECK_EQ(inv.desc, "Press 'E' to open your inventory.");
    CB_CHECK(!inv.icon && !inv.parent && !inv.hidden);        // (Extra.java's definition: the Java icon is not a C++ symbol)

    const Achievement* wood = find(all, "mineWood");
    CB_CHECK(wood && wood->name == "Getting Wood");
    CB_CHECK_EQ(wood->desc, "Earn the Mine Wood achievement.");   // no description in the file: made from the key
    CB_CHECK_EQ(*wood->icon, "Block::s_wood");
    CB_CHECK_EQ(*wood->parent, "openInventory");

    const Achievement* bench = find(all, "buildWorkBench");
    CB_CHECK(bench && bench->name == "Build Work Bench");         // no name in the file: made from the key
    CB_CHECK_EQ(*bench->parent, "mineWood");
    CB_CHECK_EQ(*bench->icon, "Block::s_workbench");

    const Achievement* diamonds = find(all, "diamonds");
    CB_CHECK(diamonds && diamonds->hidden);                      // setSpecial()
    CB_CHECK_EQ(diamonds->desc, "Acquire diamonds with your iron tools");
    CB_CHECK_EQ(*diamonds->icon, "Item::s_diamond");

    const Achievement* rail = find(all, "onARail");
    CB_CHECK(rail && !rail->parent);                               // NULL is no parent
    CB_CHECK_EQ(rail->name, "On A Rail");
    const Achievement* fish = find(all, "cookFish");
    CB_CHECK(fish && !fish->parent);                               // nor is null
    CB_CHECK_EQ(fish->name, "Cook Fish");                           // an empty name in the file counts as none
    CB_CHECK_EQ(fish->desc, "Earn the Cook Fish achievement.");

    const Achievement* pig = find(all, "flyPig");
    CB_CHECK(pig && pig->hidden && *pig->parent == "killEnemy");   // a Java-style bare parent variable
    CB_CHECK_EQ(pig->name, "When Pigs Fly = yes");
    CB_CHECK(find(all, "notThis") == nullptr);                       // the words alone are not a definition

    // without a .lang file every name is made from its key
    const auto bare = scan_achievements(fixture("code"));
    CB_CHECK_EQ(bare[0].name, "Open Inventory");
    CB_CHECK(scan_achievements(fixture("nowhere")).empty());
});

CB_TEST("java.minecraft.achievements.resolve_icon_atlas", [] {
    const auto icons = resolve_icon_atlas(fixture("code"));
    CB_CHECK_EQ(icons.size(), std::size_t{6});
    auto check = [&](const char* symbol, Atlas atlas, std::uint32_t col, std::uint32_t row) {
        const auto it = icons.find(symbol);
        CB_CHECK(it != icons.end());
        if (it == icons.end()) return;
        CB_CHECK(it->second.atlas == atlas);
        CB_CHECK_EQ(it->second.col, col);
        CB_CHECK_EQ(it->second.row, row);
    };
    check("Item::s_book", Atlas::Items, 11, 3);          // setIconCoord(11, 3)
    check("Item::s_diamond", Atlas::Items, 7, 3);        // setIconIndex(55): column 55 % 16, row 55 / 16
    check("Item::s_fishCooked", Atlas::Items, 10, 5);
    check("Block::s_wood", Atlas::Terrain, 4, 0);        // the base constructor's literal 2nd argument
    check("Block::s_workbench", Atlas::Terrain, 11, 3);  // BlockWorkbench's `blockIndexInTexture = 59;`
    check("Block::s_stone", Atlas::Terrain, 1, 0);
    CB_CHECK(icons.find("Item::s_stick") == icons.end());    // no icon call: unresolvable
    CB_CHECK(icons.find("Item::s_missing") == icons.end());
    CB_CHECK_EQ(atlas_name(Atlas::Items), "items");
    CB_CHECK_EQ(atlas_name(Atlas::Terrain), "terrain");
    CB_CHECK(resolve_icon_atlas(fixture("nowhere")).empty());
});

// The same symbol in an item and a block definition no longer collides: the entries are keyed by the whole icon.
CB_TEST("java.minecraft.achievements.resolve_icon_atlas_same_symbol", [] {
    fs::ScopedTempDir tmp("cbach");
    fs::write_text(tmp.path() / "Item.cpp", "Item::s_sign = (new Item(323))->setIconCoord(10, 1);\n");
    fs::write_text(tmp.path() / "Block.cpp", "Block::s_sign = (new Block(63, 4, M))->x();\n");
    const auto icons = resolve_icon_atlas(tmp.path());
    CB_CHECK_EQ(icons.size(), std::size_t{2});
    CB_CHECK(icons.at("Item::s_sign").atlas == Atlas::Items && icons.at("Item::s_sign").col == 10);
    CB_CHECK(icons.at("Block::s_sign").atlas == Atlas::Terrain && icons.at("Block::s_sign").col == 4);
});

CB_TEST("java.minecraft.achievements.map_to_platform", [] {
    const auto all = scan_achievements(fixture("code"), fixture("lang/en_US.lang"));
    const Mapping m = map_to_platform(all);
    CB_CHECK_EQ(m.count, std::int64_t{7});
    CB_CHECK_EQ(m.limit, std::int64_t{99});
    CB_CHECK_EQ(m.total_score, std::int64_t{1000});
    CB_CHECK(m.warnings.empty());
    CB_CHECK(m.tier == Tier::Retail);
    for (std::size_t i = 0; i < m.entries.size(); ++i) {
        CB_CHECK_EQ(m.entries[i].platform_id, static_cast<std::int64_t>(i) + 1);    // consoles count from 1
        CB_CHECK_EQ(m.entries[i].source_id, all[i].id);
        CB_CHECK_EQ(m.entries[i].key, all[i].key);
    }
    CB_CHECK_EQ(m.entries[3].gamerscore, std::int64_t{145});
    CB_CHECK_EQ(m.entries[6].gamerscore, std::int64_t{140});
    CB_CHECK(m.entries[3].hidden && !m.entries[2].hidden);
    CB_CHECK(!m.entries[0].art);

    // arcade: 7 achievements fit under 12, scoring 200
    const Mapping arcade = map_to_platform(all, Tier::Arcade);
    CB_CHECK_EQ(arcade.total_score, std::int64_t{200});
    CB_CHECK(arcade.warnings.empty());
    CB_CHECK_EQ(arcade.limit, std::int64_t{12});
});

CB_TEST("java.minecraft.achievements.map_to_platform_warnings", [] {
    std::vector<Achievement> many;
    for (std::int64_t i = 0; i < 13; ++i) {
        Achievement a;
        a.id = i;
        a.key = "ach" + std::to_string(i);
        many.push_back(a);
    }
    const Mapping m = map_to_platform(many, Tier::Arcade);
    CB_CHECK((m.warnings) == ((std::vector<std::string>{"13 achievements exceeds the arcade limit of 12 -- split across titles or raise the profile."})));
    CB_CHECK_EQ(m.total_score, std::int64_t{200});

    const Mapping none = map_to_platform({});
    CB_CHECK(none.entries.empty() && none.warnings.empty());      // nothing to score is not a warning
    CB_CHECK_EQ(none.total_score, std::int64_t{0});

    // a tier that cannot add up: 1001 achievements for 1000 points leave some with none, but the sum still holds
    std::vector<Achievement> lots(99);
    const Mapping big = map_to_platform(lots);
    CB_CHECK_EQ(big.total_score, std::int64_t{1000});
    CB_CHECK(big.warnings.empty());
    const Mapping over = map_to_platform(std::vector<Achievement>(100), Tier::Arcade);
    CB_CHECK_EQ(over.warnings.size(), std::size_t{1});
});

CB_TEST("java.minecraft.achievements.title_id_from_config", [] {
    fs::ScopedTempDir tmp("cbach");
    CB_CHECK(!title_id_from_config(tmp.path()));                                         // no file
    auto with = [&](const char* json) {
        fs::write_text(tmp.path() / "platform.config.json", json);
        return title_id_from_config(tmp.path());
    };
    CB_CHECK((with(R"({"title_id": "FFFE0042"})")) == (std::optional<std::string>("FFFE0042")));
    CB_CHECK((with(R"({"other": 1, "title_id": "0xABCD"})")) == (std::optional<std::string>("0xABCD")));
    CB_CHECK((with(R"({"title_id": 1234})")) == (std::optional<std::string>("1234")));      // a number: its decimal digits
    CB_CHECK(!with(R"({"title_id": 0})"));
    CB_CHECK(!with(R"({"title_id": ""})"));
    CB_CHECK(!with(R"({"title_id": null})"));
    CB_CHECK(!with(R"({"title_id": 1.5})"));
    CB_CHECK(!with(R"({"title_id": true})"));
    CB_CHECK(!with(R"({"title_id": ["FFFE0042"]})"));
    CB_CHECK(!with(R"({"title_id": {"a": 1}})"));
    CB_CHECK(!with(R"({"name": "x"})"));
    CB_CHECK(!with(R"(["title_id"])"));                                                   // not an object
    CB_CHECK(!with("not json at all"));
    CB_CHECK(!with(""));
});

CB_TEST("java.minecraft.achievements.guess_content_dir", [] {
    fs::ScopedTempDir tmp("cbach");
    CB_CHECK(!guess_content_dir(tmp.path()));
    fs::write_text(tmp.path() / "Release" / "Content" / "terrain.png", "x");
    CB_CHECK(fs::same_path(*guess_content_dir(tmp.path()), tmp.path() / "Release" / "Content"));
    fs::write_text(tmp.path() / "gui" / "items.png", "x");                                // the folder itself
    CB_CHECK(fs::same_path(*guess_content_dir(tmp.path()), tmp.path()));
    fs::write_text(tmp.path() / "Content" / "gui" / "items.png", "x");
    CB_CHECK(fs::same_path(*guess_content_dir(tmp.path()), tmp.path() / "Content"));
    fs::write_text(tmp.path() / "Debug" / "Content" / "terrain.png", "x");                // the deploy dir wins
    CB_CHECK(fs::same_path(*guess_content_dir(tmp.path()), tmp.path() / "Debug" / "Content"));
});

void check_tile(const stdfs::path& got, const stdfs::path& want) {
    const png::Image a = png::decode(fs::read_file(got));
    const png::Image b = png::decode(fs::read_file(want));
    CB_CHECK(a.width == 64 && a.height == 64);
    CB_CHECK(a.width == b.width && a.height == b.height);
    CB_CHECK(a.rgba == b.rgba);
}

CB_TEST("java.minecraft.achievements.run_matches_the_python_module", [] {
    fs::ScopedTempDir tmp("cbach");
    copy_fixture(tmp.path());
    RunOptions opt;
    opt.lang_file = tmp.path() / "lang" / "en_US.lang";
    opt.content_dir = tmp.path() / "content";
    opt.title_id = "fffe00a1";
    const RunResult r = run(tmp.path() / "code", tmp.path() / "out", opt);

    CB_CHECK_EQ(r.count, std::int64_t{7});
    CB_CHECK_EQ(r.title_id, "FFFE00A1");
    CB_CHECK(!r.title_suggested);
    CB_CHECK((r.icons.wired) == ((std::vector<std::string>{"mineWood", "buildWorkBench", "diamonds", "cookFish"})));
    CB_CHECK_EQ(r.icons.unresolved.size(), std::size_t{3});
    CB_CHECK_EQ(r.icons.unresolved[1].key, "onARail");
    CB_CHECK_EQ(r.icons.unresolved[1].reason, "icon Item::s_missing not resolvable to an atlas sprite");
    CB_CHECK_EQ(r.icons.unresolved[0].reason, "icon (none) not resolvable to an atlas sprite");
    CB_CHECK(r.content_dir && fs::same_path(*r.content_dir, tmp.path() / "content"));
    CB_CHECK_EQ(r.code.size(), std::size_t{2});

    for (const char* name : {"ACHIEVEMENTS.md", "achievements.xlast", "Achievements.h", "Achievements.cpp"}) {
        CB_CHECK_EQ(fs::read_text(tmp.path() / "out" / name), fs::read_text(fixture(std::string("expected/") + name)));
    }
    for (const char* tile : {"mineWood", "buildWorkBench", "diamonds", "cookFish"}) {
        for (const char* kind : {"_unlocked.png", "_locked.png"}) {
            check_tile(tmp.path() / "content" / "achievement_images" / (std::string(tile) + kind),
                       fixture(std::string("expected/achievement_images/") + tile + kind));
        }
    }
    // an unresolved icon leaves no tile behind
    CB_CHECK(!stdfs::exists(tmp.path() / "content" / "achievement_images" / "onARail_unlocked.png"));
});

CB_TEST("java.minecraft.achievements.tile_pixels_by_hand", [] {
    // the wired tiles, checked against the atlas arithmetic rather than the Python's files: mineWood = Block::s_wood =
    // terrain sprite (4, 0), cut at x 64..80, y 0..16 and scaled 4x. The atlas is a grey ramp v = (x + y) & 255 with
    // alpha 200; a locked tile is 40% of the grey (the luma of a grey is the grey itself).
    fs::ScopedTempDir tmp("cbach");
    copy_fixture(tmp.path());
    RunOptions opt;
    opt.content_dir = tmp.path() / "content";
    (void)run(tmp.path() / "code", tmp.path() / "out", opt);
    const png::Image unlocked = png::decode(fs::read_file(tmp.path() / "content" / "achievement_images" / "mineWood_unlocked.png"));
    const png::Image locked = png::decode(fs::read_file(tmp.path() / "content" / "achievement_images" / "mineWood_locked.png"));
    for (const auto& [x, y] : std::vector<std::pair<std::uint32_t, std::uint32_t>>{{0, 0}, {3, 3}, {4, 0}, {63, 63}, {17, 40}}) {
        const std::uint32_t v = (64 + x / 4 + y / 4) & 255;
        CB_CHECK_EQ(unlocked.pixel(x, y)[0], static_cast<std::uint8_t>(v));
        CB_CHECK_EQ(unlocked.pixel(x, y)[1], static_cast<std::uint8_t>(v));
        CB_CHECK_EQ(unlocked.pixel(x, y)[2], static_cast<std::uint8_t>(v));
        CB_CHECK_EQ(unlocked.pixel(x, y)[3], std::uint8_t{200});
        CB_CHECK_EQ(locked.pixel(x, y)[0], static_cast<std::uint8_t>(v * 40 / 100));
        CB_CHECK_EQ(locked.pixel(x, y)[3], std::uint8_t{200});
    }
});

CB_TEST("java.minecraft.achievements.run_without_art_says_why", [] {
    fs::ScopedTempDir tmp("cbach");
    copy_fixture(tmp.path());
    // no content folder anywhere near `out`: every entry is unresolved, with the reason -- never "zero of both"
    RunOptions opt;
    opt.lang_file = tmp.path() / "lang" / "en_US.lang";
    opt.tier = Tier::Arcade;
    opt.delivery = "local";
    const RunResult r = run(tmp.path() / "code", tmp.path() / "out", opt);
    CB_CHECK(!r.content_dir);
    CB_CHECK(r.icons.wired.empty());
    CB_CHECK_EQ(r.icons.unresolved.size(), std::size_t{7});
    CB_CHECK(r.icons.unresolved[0].reason.starts_with("no Content dir with terrain.png / gui/items.png found near "));
    CB_CHECK(r.icons.unresolved[0].reason.ends_with("(run `codebreaker java assets` first, or pass --content-dir)"));
    CB_CHECK_EQ(r.title_id, "00000000");                                  // never issued for the user
    CB_CHECK_EQ(r.mapping.total_score, std::int64_t{200});

    const std::string report = fs::read_text(r.report);
    CB_CHECK(report.find("- profile: **arcade** (max 12, 200 gamerscore)   assigned total: **200**   delivery: **local**") != std::string::npos);
    CB_CHECK(report.find("> NOTE: delivery is **local** -- unlocks are tracked locally only") != std::string::npos);
    CB_CHECK(report.find("- Title ID: **0x00000000**\n") != std::string::npos);
    const std::string code = fs::read_text(tmp.path() / "out" / "Achievements.cpp");
    CB_CHECK(code.find("// J2X-STUB[achievements-local]: local delivery -- track locally, no online write.") != std::string::npos);
    CB_CHECK(code.find("XUserWriteAchievements") == std::string::npos);
});

CB_TEST("java.minecraft.achievements.title_id_precedence", [] {
    fs::ScopedTempDir tmp("cbach");
    copy_fixture(tmp.path());
    const stdfs::path out = tmp.path() / "out";
    RunOptions opt;
    CB_CHECK_EQ(run(tmp.path() / "code", out, opt).title_id, "00000000");                  // nothing says
    fs::write_text(out / "platform.config.json", R"({"title_id": "0xfffe0001"})");
    CB_CHECK_EQ(run(tmp.path() / "code", out, opt).title_id, "FFFE0001");                  // the project's config
    opt.title_id = "abcd1234";
    CB_CHECK_EQ(run(tmp.path() / "code", out, opt).title_id, "ABCD1234");                  // an explicit one wins
    opt.title_id = "not hex";
    CB_CHECK_EQ(run(tmp.path() / "code", out, opt).title_id, "FFFE0001");                  // an unusable one is passed over
    fs::write_text(out / "platform.config.json", R"({"title_id": "zz"})");
    CB_CHECK_EQ(run(tmp.path() / "code", out, opt).title_id, "00000000");
    const std::string xlast = fs::read_text(out / "achievements.xlast");
    CB_CHECK(xlast.find("<XLASTProject titleId=\"0x00000000\">") != std::string::npos);
});

CB_TEST("java.minecraft.achievements.wire_icons_problems", [] {
    fs::ScopedTempDir tmp("cbach");
    copy_fixture(tmp.path());
    const stdfs::path code = tmp.path() / "code";
    auto entries = map_to_platform(scan_achievements(code)).entries;

    // a Content folder without atlases
    fs::ScopedTempDir empty("cbach");
    auto none = entries;
    const IconWiring w1 = wire_icons(none, code, empty.path(), tmp.path() / "out");
    CB_CHECK(w1.wired.empty());
    bool saw_items = false, saw_terrain = false;
    for (const Unresolved& u : w1.unresolved) {
        saw_items |= u.reason.starts_with("atlas items missing at ");
        saw_terrain |= u.reason.starts_with("atlas terrain missing at ");
    }
    CB_CHECK(saw_items && saw_terrain);
    CB_CHECK(!stdfs::exists(empty.path() / "achievement_images"));       // nothing is created for nothing

    // a damaged atlas: its entries say so, the other atlas still works
    fs::copy_tree(tmp.path() / "content", empty.path());
    fs::write_text(empty.path() / "terrain.png", "this is not a PNG");
    auto mixed = entries;
    const IconWiring w2 = wire_icons(mixed, code, empty.path(), tmp.path() / "out");
    bool terrain_unreadable = false;
    for (const Unresolved& u : w2.unresolved) terrain_unreadable |= u.reason.starts_with("atlas terrain unreadable at ");
    CB_CHECK(terrain_unreadable);
    CB_CHECK((w2.wired) == ((std::vector<std::string>{"diamonds", "cookFish"})));   // the items ones
    for (const Entry& e : mixed) CB_CHECK_EQ(e.art.has_value(), e.key == "diamonds" || e.key == "cookFish");
});

CB_TEST("java.minecraft.achievements.wire_icons_sprite_outside_the_atlas", [] {
    // a sprite that overhangs a small atlas is cropped like a picture crop: the missing part is transparent black
    fs::ScopedTempDir tmp("cbach");
    fs::write_text(tmp.path() / "code" / "Item.cpp", "Item::s_far = (new Item(1))->setIconCoord(1, 1);\n");
    png::Image atlas = png::blank(24, 24);
    for (std::uint32_t y = 0; y < 24; ++y) {
        for (std::uint32_t x = 0; x < 24; ++x) {
            std::uint8_t* p = atlas.pixel(x, y);
            p[0] = p[1] = p[2] = 100;
            p[3] = 255;
        }
    }
    fs::write_file(tmp.path() / "content" / "gui" / "items.png", png::encode(atlas));
    std::vector<Entry> entries(1);
    entries[0].key = "far";
    entries[0].icon = "Item::s_far";
    const IconWiring w = wire_icons(entries, tmp.path() / "code", tmp.path() / "content", tmp.path() / "out", 32);
    CB_CHECK((w.wired) == (std::vector<std::string>{"far"}));
    const png::Image tile = png::decode(fs::read_file(tmp.path() / "content" / "achievement_images" / "far_unlocked.png"));
    CB_CHECK(tile.width == 32 && tile.height == 32);
    CB_CHECK_EQ(tile.pixel(0, 0)[3], std::uint8_t{255});     // inside the atlas (x, y 16..24)
    CB_CHECK_EQ(tile.pixel(15, 15)[3], std::uint8_t{255});
    CB_CHECK_EQ(tile.pixel(16, 0)[3], std::uint8_t{0});      // x past 24: outside
    CB_CHECK_EQ(tile.pixel(0, 16)[3], std::uint8_t{0});
    CB_CHECK_EQ(entries[0].art->unlocked, "..\\content\\achievement_images\\far_unlocked.png");
    CB_CHECK_EQ(entries[0].art->locked, "..\\content\\achievement_images\\far_locked.png");
});

CB_TEST("java.minecraft.achievements.emitters_without_entries", [] {
    // a project with no achievements still gets a well-formed (empty) table, config and seam
    fs::ScopedTempDir tmp("cbach");
    const Mapping m = map_to_platform({});
    const std::string report = render_report(m);
    CB_CHECK(report.starts_with("# ACHIEVEMENTS.md -- achievement conversion\n\nThe program defines 0 achievement(s)."));
    CB_CHECK(report.find("| # | Key | Name | Gamerscore | Hidden | Tile art |\n|---:|---|---|---:|:---:|---|\n\n") != std::string::npos);
    CB_CHECK(report.find("### Tile art") == std::string::npos);            // no icon wiring given: no section
    CB_CHECK(report.find("- Title ID: **0x00000000**\n") != std::string::npos);
    CB_CHECK(render_report(m, "online", nullptr, "FFFE0001", true).find("- Title ID: **0xFFFE0001**  _(suggested \xE2\x80\x94 see below)_") !=
             std::string::npos);

    emit_xlast(m, tmp.path() / "x.xlast", "bad id");
    CB_CHECK_EQ(fs::read_text(tmp.path() / "x.xlast"),
                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                "<!-- Achievement config skeleton emitted by CodeBreaker. Import into the platform submission/config\n"
                "     tool (Xbox 360: XLAST), then build the SPA. The titleId below MUST match the Title ID set\n"
                "     on the XEX in the Visual Studio project (Xbox 360 property pages). -->\n"
                "<XLASTProject titleId=\"0x00000000\">\n"
                "  <Configuration>\n    <Achievements>\n    </Achievements>\n  </Configuration>\n</XLASTProject>\n");
    const auto code = emit_code(m, tmp.path());
    CB_CHECK_EQ(code.size(), std::size_t{2});
    CB_CHECK(fs::read_text(code[0]).find("#define J2X_ACHIEVEMENTS_H\n\n// Platform achievement ids (match the title config / XLAST project).\n\n// Unlock") !=
             std::string::npos);
});

CB_TEST("java.minecraft.achievements.xml_is_escaped", [] {
    Mapping m;
    Entry e;
    e.platform_id = 1;
    e.key = "tom";
    e.name = "Tom & \"Jerry\" <3";
    e.desc = "a > b";
    e.art = TileArt{"..\\a&b\\x.png", "..\\a&b\\y.png"};
    m.entries.push_back(e);
    fs::ScopedTempDir tmp("cbach");
    emit_xlast(m, tmp.path() / "x.xlast", "FFFE0001");
    const std::string xml = fs::read_text(tmp.path() / "x.xlast");
    CB_CHECK(xml.find("name=\"Tom &amp; &quot;Jerry&quot; &lt;3\"") != std::string::npos);
    CB_CHECK(xml.find("<LockedDescription>a &gt; b</LockedDescription>") != std::string::npos);
    CB_CHECK(xml.find("<Image type=\"unlocked\">..\\a&amp;b\\x.png</Image>") != std::string::npos);
    CB_CHECK(xml.find("<Image type=\"locked\">..\\a&amp;b\\y.png</Image>") != std::string::npos);
});

}  // namespace
