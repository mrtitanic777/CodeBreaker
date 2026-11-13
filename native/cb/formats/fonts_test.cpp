// Tests of cb/formats/fonts.hpp on real Windows 95 .FON files (testdata/fonts/): COURE.FON (fixed-pitch, 2.x), 8514SYS.FON
// (proportional), MODERN.FON (a vector font), VGAFIX.FON. The command's output is compared with what `codebreaker fonts ...`
// printed for the same files (the *.txt goldens, made by the translated Python: stdout and `exit=N`).
#include "cb/formats/fonts.hpp"

#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/ne.hpp"

using namespace cb;
namespace fonts = cb::formats::fonts;

namespace {

std::filesystem::path font_path(const char* name) { return testing::data_path(std::string("formats/testdata/fonts/") + name); }

std::string golden(const char* name) { return fs::read_text(font_path(name)); }

// What the command prints and `exit=N`, as the goldens have it.
std::string run(std::vector<std::string> args, const fonts::Environment& env = {}) {
    cli::Cli cli("codebreaker", {fonts::command(env)});
    testing::CapturedStd cap;
    args.insert(args.begin(), "fonts");
    const int code = cli.run(args);
    return cap.out() + "exit=" + std::to_string(code) + "\n";
}

std::string text_of(const char* name) { return fs::to_text(font_path(name)); }

}  // namespace

CB_TEST("formats.fonts.a_fon_lists_its_fonts", [] {
    const auto found = fonts::fonts_in(font_path("coure.fon"));
    CB_CHECK_EQ(found.size(), std::size_t{3});
    CB_CHECK_EQ(found[0].name, "Courier");
    CB_CHECK_EQ(found[0].version, 0x200);
    CB_CHECK_EQ(found[0].height, 13);
    CB_CHECK(found[0].kind() == fonts::Kind::Fixed);
    CB_CHECK_EQ(found[2].describe(), "Courier        15pt 20px  fixed        chars 32..255  version 2.00");
    CB_CHECK(fonts::fonts_in(font_path("modern.fon"))[0].kind() == fonts::Kind::Vector);
    CB_CHECK(fonts::fonts_in(font_path("8514sys.fon"))[0].kind() == fonts::Kind::Proportional);
});

CB_TEST("formats.fonts.glyphs_are_drawn_from_the_bitmaps", [] {
    const fonts::Font font = fonts::fonts_in(font_path("coure.fon"))[0];
    const std::vector<std::string> h = font.glyph('H');
    CB_CHECK_EQ(h.size(), std::size_t{13});
    CB_CHECK_EQ(h[0], std::string(font.places.at('H').width, ' '));
    bool any = false;
    for (const std::string& row : h) any = any || row.find('#') != std::string::npos;
    CB_CHECK(any);
    // a character the font does not have is its default character
    CB_CHECK(font.glyph(0x20AC) == font.glyph(font.first + font.default_char));
    // a vector font draws nothing
    const fonts::Font vector = fonts::fonts_in(font_path("modern.fon"))[0];
    CB_CHECK(vector.glyph('x') == std::vector<std::string>(static_cast<std::size_t>(vector.height)));
});

CB_TEST("formats.fonts.render_puts_a_column_after_each_glyph", [] {
    const fonts::Font font = fonts::fonts_in(font_path("8514sys.fon"))[0];
    const auto rows = font.render("Wi");
    const std::size_t width = static_cast<std::size_t>(font.places.at('W').width + font.places.at('i').width + 2);
    CB_CHECK_EQ(rows.size(), static_cast<std::size_t>(font.height));
    for (const std::string& row : rows) CB_CHECK_EQ(row.size(), width);
});

CB_TEST("formats.fonts.a_bare_fnt_is_one_font_and_other_files_none", [] {
    // the first FONT resource of COURE.FON is a .FNT image: written out bare it is the file's one font
    const auto module = formats::ne::Module::open_file(font_path("coure.fon"));
    std::filesystem::path bare;
    for (const auto& resource : module.resources()) {
        if (resource.type != "FONT") continue;
        const ByteView image = module.data().subspan(resource.file_offset, resource.length);
        static const fs::ScopedTempDir dir{"cb-fonts"};
        bare = dir.path() / "one.fnt";
        fs::write_file(bare, image);
        break;
    }
    const auto found = fonts::fonts_in(bare);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK_EQ(found[0].name, "Courier");
    CB_CHECK_EQ(fonts::fonts_in(testing::data_path("formats/testdata/xcopy.exe")).size(), std::size_t{0});
    CB_CHECK_THROWS_AS(FormatError, fonts::Font::parse(Bytes(0x200, 0)));
});

CB_TEST("formats.fonts.the_command_prints_what_the_python_printed", [] {
    CB_CHECK_EQ(run({text_of("coure.fon"), "--show", "Hi"}), golden("coure_hi.txt"));
    CB_CHECK_EQ(run({text_of("coure.fon"), "--index", "1", "--show", "Aq~ #"}), golden("coure_second.txt"));
    CB_CHECK_EQ(run({text_of("modern.fon"), "--show", "x"}), golden("modern_vector.txt"));
    CB_CHECK_EQ(run({text_of("8514sys.fon"), "--show", "Wiq."}), golden("sys_proportional.txt"));
    CB_CHECK_EQ(run({text_of("vgafix.fon")}), golden("vgafix_list.txt"));
});

CB_TEST("formats.fonts.a_corpus_name_is_found_through_the_environment_and_a_missing_file_exits_one", [] {
    fonts::Environment env;
    env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
        if (name == "vgafix.fon") return font_path("vgafix.fon");
        return std::nullopt;
    };
    CB_CHECK_EQ(run({"vgafix.fon"}, env), golden("vgafix_list.txt"));
    CB_CHECK_EQ(run({text_of("nosuch.fon")}, env), golden("missing.txt"));
});
