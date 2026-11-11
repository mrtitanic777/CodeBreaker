#include "cb/java/console/assets.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/java/console/png_rgba.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

std::string list_text(const std::vector<std::string>& v) {
    std::string out = "[";
    for (std::size_t i = 0; i < v.size(); ++i) out += (i ? ", '" : "'") + v[i] + "'";
    return out + "]";
}

// A folder's files as the golden lists them: a PNG by its size and pixels, any other file by its size and SHA-1.
std::string listing(const stdfs::path& dir) {
    std::vector<std::string> files;
    test::files_in_walk_order(dir, dir, files);
    std::string out;
    for (const std::string& rel : files) {
        const Bytes data = fs::read_file(dir / fs::from_text(rel));
        const bool is_png = text::iends_with(rel, ".png") && png::color_type(data).has_value();
        if (!out.empty()) out += "\n";
        if (is_png) {
            const png::Image img = png::decode_rgba(data);
            out += std::format("  {} png {}x{} {}", rel, img.width, img.height, crypto::to_hex(img.rgba));
        } else {
            out += std::format("  {} {} {}", rel, data.size(), crypto::to_hex(crypto::sha1(data)));
        }
    }
    return out;
}

CB_TEST("java.console.assets.matches_the_original", [] {
    const stdfs::path jar_path = test::data("assets/game.jar");
    const stdfs::path code = test::data("assets/code");
    const stdfs::path bundle = test::data("assets/bundle");
    const ZipArchive jar = ZipArchive::open(jar_path);
    std::vector<std::string> out;

    out.push_back("jar assets: " + list_text(assets::list_jar_assets(jar)));
    out.push_back("console assets: " + list_text(assets::list_console_assets(bundle)));
    const assets::ResourceRefs refs = assets::code_resource_refs(code);
    std::string refs_text = "refs:";
    for (const auto& [path, files] : refs) refs_text += std::format("\n  {} <- {}", path, list_text(files));
    out.push_back(refs_text);
    const assets::Coverage cov = assets::coverage(jar, code, bundle);
    out.push_back(std::format("coverage present={}\n  missing={}\n  extra={}\n  jar_count={}", list_text(cov.present), list_text(cov.missing),
                              list_text(cov.extra), cov.supply));

    fs::ScopedTempDir tmp("cbassets");
    struct Run {
        const char* name;
        bool only_needed;
        bool normalize;
    };
    for (const Run& r : {Run{"all", false, true}, Run{"needed", true, true}, Run{"raw", false, false}}) {
        const stdfs::path run_dir = tmp.path() / (std::string("run_") + r.name);
        assets::RunOptions options;
        options.code_dir = code;
        options.bundle_dir = bundle;
        options.only_needed = r.only_needed;
        options.normalize_png = r.normalize;
        const assets::RunResult res = assets::run(jar_path, run_dir / "Content", options);
        out.push_back(std::format("run {}: written={}\n  converted={} unconverted=[] has_pil=True console={}", r.name, list_text(res.written),
                                  list_text(res.converted), list_text(res.console)));
        out.push_back("  report: " + test::rel(res.report, run_dir));
        out.push_back("  report text:\n" + test::lf(fs::read_text(*res.report)));
        out.push_back(listing(run_dir / "Content"));
    }
    {   // without a code folder: everything is extracted, nothing is audited
        assets::RunOptions options;
        options.bundle_dir = bundle;
        const assets::RunResult res = assets::run(jar_path, tmp.path() / "nocode" / "Content", options);
        out.push_back(std::format("run nocode: written={} report={}", res.written.size(), res.report ? "x" : "None"));
        CB_CHECK(!res.coverage.has_value());
    }
    assets::Coverage c2;
    c2.needed = {{"a.png", {"A.java"}}};
    c2.present = {"a.png"};
    c2.extra = {"b.txt"};
    c2.supply = 2;
    out.push_back(assets::render_coverage(c2));
    out.push_back(assets::render_coverage(c2, {"x.png", "y.png"}));
    out.push_back("strip: '" + text::replace_all(assets::strip_comments("a /* x */ b // c\nd \"/p//q.png\" e /* open"), "\n", "\\n") + "'");

    // DIVERGES: a name that occurs twice in a jar is listed once; the original listed it twice
    std::string golden = test::golden("assets.golden.txt");
    golden = text::replace_all(golden, "jar assets: ['font/default.png', 'gui/gui.png', 'gui/gui.png',", "jar assets: ['font/default.png', 'gui/gui.png',");
    CB_CHECK_EQ(text::join(out, "\n") + "\n", golden);
});

CB_TEST("java.console.assets.comment_stripping", [] {
    CB_CHECK_EQ(assets::strip_comments("a /* b */ c"), "a  c");
    CB_CHECK_EQ(assets::strip_comments("a // b\nc"), "a \nc");
    CB_CHECK_EQ(assets::strip_comments("a // b"), "a ");
    CB_CHECK_EQ(assets::strip_comments("/* x\ny */z"), "z");                       // a block comment may span lines
    CB_CHECK_EQ(assets::strip_comments("/* a */ /* b */x"), " x");
    CB_CHECK_EQ(assets::strip_comments("/* open"), "/* open");                      // a comment that does not end is not one
    CB_CHECK_EQ(assets::strip_comments("x /* a // b */ y"), "x  y");                // a line comment inside a block goes with it
    CB_CHECK_EQ(assets::strip_comments("// a /* b\nc */ d"), "");                    // the block pass comes first, as it always did: it
                                                                                     // takes the "/* b\nc */" that starts inside a line comment
    CB_CHECK_EQ(assets::strip_comments("s = \"http://x\"; t"), "s = \"http:");      // plain, not a lexer: a "//" in a string cuts the line
    CB_CHECK_EQ(assets::strip_comments(""), "");
});

CB_TEST("java.console.assets.resource_literals", [] {
    fs::ScopedTempDir tmp("cbassets");
    fs::write_text(tmp.path() / "A.java",
                   "x(\"/a/b.png\"); y(\"/c.txt\"); \"/d/e.lang\" \"/f.properties\" \"/g.fsh\" \"/h.vsh\" \"/i.jpg\" \"/j.jpeg\"\n"
                   "\"a/b.png\" \"/B.PNG\" \"/x y.png\" \"/z.png \" \"/q.pn\" \"/.png\" \"/a.b.png\" \"/-_.9.png\"\n"
                   "\"/dir/\" \"\" \"/\" \"/a.png\"junk\"/b.png\" \"//lead.png\"\n");
    const assets::ResourceRefs refs = assets::code_resource_refs(tmp.path());
    std::string keys;
    for (const auto& [k, files] : refs) keys += k + ";";
    // a literal is "/" + at least one path character + a shipped extension in lower case; the quote that closes one
    // string can open the next ("junk\"/b.png" finds /b.png); slashes in front are not part of the key
    CB_CHECK_EQ(keys, "-_.9.png;a.b.png;a.png;a/b.png;b.png;c.txt;d/e.lang;f.properties;g.fsh;h.vsh;i.jpg;j.jpeg;");
    // file kinds: .java .cpp .h .hpp .cc .cxx in any case, nothing else; files by name, sorted
    fs::write_text(tmp.path() / "sub" / "B.CPP", "\"/shared.png\"");
    fs::write_text(tmp.path() / "sub" / "C.hxx", "\"/not_scanned.png\"");
    fs::write_text(tmp.path() / "D.cc", "\"/shared.png\"");
    fs::write_text(tmp.path() / "E.cxx", "\"/shared.png\" \"/shared.png\"");
    const assets::ResourceRefs more = assets::code_resource_refs(tmp.path());
    CB_CHECK_EQ(list_text(more.at("shared.png")), "['B.CPP', 'D.cc', 'E.cxx']");
    CB_CHECK(!more.contains("not_scanned.png"));
    CB_CHECK(assets::code_resource_refs(tmp.path() / "nowhere").empty());
    // a resource path named only in a comment is not a demand
    fs::write_text(tmp.path() / "F.java", "// \"/only/in/comment.png\"\n/* \"/also/in.png\" */\n");
    CB_CHECK(!assets::code_resource_refs(tmp.path()).contains("only/in/comment.png"));
});

CB_TEST("java.console.assets.case_drift_is_tolerated", [] {
    const ZipArchive jar = ZipArchive::open(test::data("assets/drift.jar"));
    const stdfs::path code = test::data("assets/code_drift");
    const assets::Coverage cov = assets::coverage(jar, code, {});
    // the code writes /Gui/Items.png, the jar has gui/items.png: present, and not also an "extra"
    CB_CHECK_EQ(list_text(cov.present), "['Gui/Items.png', 'lang/en.lang']");
    CB_CHECK_EQ(list_text(cov.missing), "['Missing/One.png']");
    CB_CHECK_EQ(list_text(cov.extra), "['other/unused.png']");
    CB_CHECK_EQ(cov.supply, std::size_t{3});
    // only_needed extracts the jar's own spelling of what the code asks for (the original compared exactly and so
    // left the drifting path out of Content although it had counted it present)
    fs::ScopedTempDir tmp("cbassets");
    assets::RunOptions options;
    options.code_dir = code;
    options.only_needed = true;
    const assets::RunResult res = assets::run(test::data("assets/drift.jar"), tmp.path() / "Content", options);
    CB_CHECK_EQ(list_text(res.written), "['gui/items.png', 'lang/en.lang']");
    CB_CHECK(stdfs::exists(tmp.path() / "Content" / "gui" / "items.png"));
    CB_CHECK(!stdfs::exists(tmp.path() / "Content" / "other"));
    CB_CHECK_EQ(list_text(res.converted), "['gui/items.png']");
    // the report sits next to Content
    CB_CHECK_PATH(*res.report, tmp.path() / "TEXTURE_COVERAGE.md");
});

CB_TEST("java.console.assets.extract_options", [] {
    const ZipArchive jar = ZipArchive::open(test::data("assets/game.jar"));
    fs::ScopedTempDir tmp("cbassets");
    assets::ExtractOptions options;
    options.only = std::set<std::string>{"terrain.png", "lang/en_US.lang", "not/in/the.jar"};
    options.normalize_png = false;
    const assets::Extracted raw = assets::extract(jar, tmp.path() / "raw", options);
    CB_CHECK_EQ(list_text(raw.written), "['lang/en_US.lang', 'terrain.png']");
    CB_CHECK(raw.normalized.empty());
    CB_CHECK(equal_bytes(fs::read_file(tmp.path() / "raw" / "terrain.png"), jar.read("terrain.png")));   // byte for byte
    options.normalize_png = true;
    const assets::Extracted conv = assets::extract(jar, tmp.path() / "conv", options);
    CB_CHECK_EQ(list_text(conv.normalized), "['terrain.png']");
    CB_CHECK(!png::needs_rgba(fs::read_file(tmp.path() / "conv" / "terrain.png")));
    CB_CHECK_EQ(png::decode_rgba(fs::read_file(tmp.path() / "conv" / "terrain.png")).width, 5u);
    // (the extension is read in any case: Mixed.PNG is converted too)
    options.only = std::set<std::string>{"gui/items.png", "misc/Mixed.PNG"};
    const assets::Extracted both = assets::extract(jar, tmp.path() / "both", options);
    CB_CHECK_EQ(list_text(both.normalized), "['gui/items.png', 'misc/Mixed.PNG']");         // the extension is read in any case
    // an empty selection extracts nothing; non-assets are never extracted
    options.only = std::set<std::string>{};
    CB_CHECK(assets::extract(jar, tmp.path() / "none", options).written.empty());
    const assets::Extracted all = assets::extract(jar, tmp.path() / "all");
    for (const std::string& w : all.written) CB_CHECK(assets::is_asset_name(w));
    CB_CHECK(!stdfs::exists(tmp.path() / "all" / "readme.html"));
    CB_CHECK(!stdfs::exists(tmp.path() / "all" / "com"));
    CB_CHECK(assets::is_asset_name("A/B.PNG") && assets::is_asset_name("x.Properties") && !assets::is_asset_name("x.png.bak"));
});

CB_TEST("java.console.assets.a_jar_cannot_write_outside_content", [] {
    fs::ScopedTempDir tmp("cbassets");
    const ZipArchive evil = ZipArchive::open(test::data("assets/evil.jar"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)assets::extract(evil, tmp.path() / "Content"));
    CB_CHECK(!stdfs::exists(tmp.path() / "evil.png"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)assets::run(test::data("assets/evil.jar"), tmp.path() / "Content2"));
    CB_CHECK(!stdfs::exists(tmp.path() / "evil.png"));
});

CB_TEST("java.console.assets.the_console_bundle", [] {
    fs::ScopedTempDir tmp("cbassets");
    CB_CHECK(assets::list_console_assets(tmp.path() / "nowhere").empty());
    CB_CHECK(assets::list_console_assets({}).empty());
    CB_CHECK(assets::place_console_assets(tmp.path() / "nowhere", tmp.path() / "out").empty());
    CB_CHECK_EQ(list_text(assets::list_console_assets(test::data("assets/bundle"))), "['cursor.png', 'extra/readme.txt']");
    const auto placed = assets::place_console_assets(test::data("assets/bundle"), tmp.path() / "Content");
    CB_CHECK_EQ(list_text(placed), "['cursor.png', 'extra/readme.txt']");
    CB_CHECK_EQ(fs::read_text(tmp.path() / "Content" / "extra" / "readme.txt"), "bundled\n");
    // a bundle file is part of the supply: a code reference to it is present even when the jar lacks it
    const ZipArchive jar = ZipArchive::open(test::data("assets/drift.jar"));
    fs::write_text(tmp.path() / "code" / "A.java", "x(\"/cursor.png\");");
    CB_CHECK_EQ(list_text(assets::coverage(jar, tmp.path() / "code", test::data("assets/bundle")).present), "['cursor.png']");
    CB_CHECK_EQ(list_text(assets::coverage(jar, tmp.path() / "code", {}).missing), "['cursor.png']");
});

}  // namespace
