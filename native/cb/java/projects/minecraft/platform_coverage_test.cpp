// platform_coverage: the symbol scanners on small headers and game code, the classification, and the report.
#include <set>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/platform_coverage.hpp"

namespace {

using namespace cb;
namespace pc = cb::java::projects::minecraft::platform_coverage;
namespace stdfs = std::filesystem;

// (two string literals in braces would be read as an iterator range)
std::set<std::string> cov_runtime_files() { return std::set<std::string>{std::string("GL11.h"), std::string("Keyboard.h")}; }

bool cov_test_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

CB_TEST("java.minecraft.platform_coverage.subsystems_table", [] {
    const auto subs = pc::subsystems();
    CB_CHECK_EQ(subs.size(), std::size_t{8});
    CB_CHECK_EQ(subs[0].key, "video");
    CB_CHECK_EQ(subs[7].key, "user");
    CB_CHECK_EQ(subs[0].holders.size(), std::size_t{8});
    CB_CHECK_EQ(subs[0].reference.size(), std::size_t{6});
    CB_CHECK_EQ(subs[0].notes.size(), std::size_t{1});
    CB_CHECK(subs[1].holders.size() == 3 && subs[1].holders[0] == "Keyboard");
    CB_CHECK(subs[4].holders.empty());   // fileio: types only
    CB_CHECK(subs[6].types.empty());     // memory
});

CB_TEST("java.minecraft.platform_coverage.provided_symbols", [] {
    const std::set<std::string> got = pc::provided_symbols(
        "#define GL_FOO_BAR 1\n"
        "const int GL_BAZ_QUX = 3;\n"
        "enum { ABC_DEF = 2, GHI_JKL=4 };\n"
        "void f(int x); int g (int y);\n"
        "if (x) while(y) for(;;) return(1); switch(a) sizeof(b);\n");
    // functions, the three kinds of constant, and none of the keywords
    CB_CHECK((got == std::set<std::string>{"GL_FOO_BAR", "GL_BAZ_QUX", "ABC_DEF", "GHI_JKL", "f", "g"}));
});

CB_TEST("java.minecraft.platform_coverage.provided_symbols_edges", [] {
    // a #define with spaces after the hash; a name that is too short or is not ALL_CAPS is no constant
    const std::set<std::string> got = pc::provided_symbols(
        "#  define MAX_LEN 4\n#define AB 1\n#define Mixed_Case 2\nconst unsigned int TWO_WORDS = 1;\n"
        "const std::string kName;\nconst float PI_VALUE = 3;\nx = FOO_BAR_BAZ;\n9abc(\nabc9(\n");
    CB_CHECK(got.contains("MAX_LEN"));
    CB_CHECK(!got.contains("AB"));
    CB_CHECK(!got.contains("Mixed_Case"));
    CB_CHECK(got.contains("PI_VALUE"));
    CB_CHECK(!got.contains("FOO_BAR_BAZ"));   // no `=` after it
    CB_CHECK(got.contains("TWO_WORDS"));   // `const unsigned int NAME` is no const declaration, but `NAME =` is an enumerator
    CB_CHECK(got.contains("abc9"));
    CB_CHECK(!got.contains("9abc"));
    CB_CHECK(pc::provided_symbols("").empty());
});

CB_TEST("java.minecraft.platform_coverage.used_holder_symbols", [] {
    const auto used = pc::used_holder_symbols(
        "GL11::glBegin(1); GL11 :: glEnd(); GL11::glBegin(2);\n  MyGL11::glNope(); GL11::\nglLate();\nGL11x::glNo(); GL11::9bad();", "GL11");
    // counts, in the order each symbol first appears; a longer name or a non-identifier is not a use
    CB_CHECK_EQ(used.size(), std::size_t{3});
    CB_CHECK_EQ(used.entries()[0].first, "glBegin");
    CB_CHECK_EQ(used.entries()[0].second, std::size_t{2});
    CB_CHECK_EQ(used.entries()[1].first, "glEnd");
    CB_CHECK_EQ(used.entries()[2].first, "glLate");
    CB_CHECK(pc::used_holder_symbols("", "GL11").empty());
});

CB_TEST("java.minecraft.platform_coverage.type_usage_count", [] {
    // `new T`, `T*`, `T(`, `<T>` count; a longer name, a member of another class and a qualified use do not
    CB_CHECK_EQ(pc::type_usage_count("new Socket(1); Socket* s; List<Socket> l; MySocket m; Socketry r; a::Socket q;", "Socket"), std::size_t{3});
    CB_CHECK_EQ(pc::type_usage_count("Socket", "Socket"), std::size_t{1});
    CB_CHECK_EQ(pc::type_usage_count("", "Socket"), std::size_t{0});
    CB_CHECK_EQ(pc::type_usage_count("Socket_x _Socket", "Socket"), std::size_t{0});
});

CB_TEST("java.minecraft.platform_coverage.scan_stub_markers", [] {
    fs::ScopedTempDir dir("cb-cov");
    fs::write_text(dir.path() / "B.h", "int x;\nvoid f() {} // J2X-STUB[net-socket]: no-op placeholder  \n");
    fs::write_text(dir.path() / "A.cpp",
                   "// J2X-STUB[video-1]: first\n// J2X-STUB[net-socket]: second\n// J2X-STUB[BAD TAG]: x\n"
                   "J2X-STUB[a]  :   spaced note\n// J2X-STUB[x] no colon\n");
    fs::write_text(dir.path() / "notes.txt", "// J2X-STUB[ignored]: not a source file");
    const pc::StubMarkers m = pc::scan_stub_markers(dir.path());
    // tags in the order met (A.cpp before B.h), sites in order
    CB_CHECK_EQ(m.size(), std::size_t{3});
    CB_CHECK_EQ(m.entries()[0].first, "video-1");
    CB_CHECK_EQ(m.entries()[1].first, "net-socket");
    CB_CHECK_EQ(m.entries()[2].first, "a");
    const auto& net = m.at("net-socket");
    CB_CHECK_EQ(net.size(), std::size_t{2});
    CB_CHECK_EQ(net[0].file, "A.cpp");
    CB_CHECK_EQ(net[0].line, std::size_t{2});
    CB_CHECK_EQ(net[0].note, "second");
    CB_CHECK_EQ(net[1].file, "B.h");
    CB_CHECK_EQ(net[1].line, std::size_t{2});
    CB_CHECK_EQ(net[1].note, "no-op placeholder");   // trimmed
    CB_CHECK_EQ(m.at("a")[0].note, "spaced note");
    CB_CHECK(pc::scan_stub_markers(dir.path() / "missing").empty());
});

CB_TEST("java.minecraft.platform_coverage.stub_marker_notes_are_clipped_to_100_bytes", [] {
    fs::ScopedTempDir dir("cb-cov");
    fs::write_text(dir.path() / "A.h", "// J2X-STUB[long]: " + std::string(150, 'x') + "\n");
    const pc::StubMarkers m = pc::scan_stub_markers(dir.path());
    CB_CHECK_EQ(m.at("long")[0].note.size(), std::size_t{100});
});

// A converted tree: stub headers (the runtime files), game code, and a reference folder.
struct CovTree {
    fs::ScopedTempDir dir{"cb-cov"};
    stdfs::path out, ref;
    CovTree() {
        out = dir.path() / "out";
        ref = dir.path() / "reference";
        fs::make_dirs(out);
        fs::make_dirs(ref);
        // stubs: GL11 provides glBegin and glEnd (no-ops); Keyboard provides isKeyDown
        fs::write_text(out / "GL11.h", "namespace GL11 {\ninline void glBegin(int m) {}\ninline void glEnd() {}\n}\n");
        fs::write_text(out / "Keyboard.h", "namespace Keyboard { inline bool isKeyDown(int k) { return false; } }\n");
        // the reference implements glBegin for real, and glFog
        fs::write_text(ref / "GL11.reference.h", "namespace GL11 { void glBegin(int); void glFog(int, float); }\n");
        fs::write_text(ref / "GL11.reference.cpp", "void GL11::glBegin(int m) { /* real */ }\n");
        // the game code
        fs::write_text(out / "Tessellator.cpp",
                       "void t() { GL11::glBegin(7); GL11::glBegin(4); GL11::glEnd(); GL11::glFog(1, 2.0F); "
                       "GL11::glNeverHeardOfIt(); Keyboard::isKeyDown(1); Keyboard::getEventKey(); }\n"
                       "Tessellator* x = new Tessellator(); // the platform type\n");
        fs::write_text(out / "Game.h", "GL11::glBegin(1); Socket* s;\n");
    }
};

CB_TEST("java.minecraft.platform_coverage.analyze_classifies_each_symbol", [] {
    CovTree t;
    const std::vector<pc::Coverage> r = pc::analyze(t.out, t.ref, cov_runtime_files());
    CB_CHECK_EQ(r.size(), std::size_t{8});
    const pc::Coverage& video = r[0];
    CB_CHECK_EQ(video.sub->key, "video");
    CB_CHECK(video.ref_exists);
    // most used first; the reference wins over the stub (glBegin), glFog is only in the reference, glEnd only the stub
    CB_CHECK_EQ(video.symbols.size(), std::size_t{4});
    CB_CHECK_EQ(video.symbols[0].name, "GL11::glBegin");
    CB_CHECK_EQ(video.symbols[0].count, std::size_t{3});
    CB_CHECK(video.symbols[0].status == pc::Status::Reusable);
    CB_CHECK(video.with_status(pc::Status::Stub).size() == 1 && video.with_status(pc::Status::Stub)[0]->name == "GL11::glEnd");
    CB_CHECK(video.with_status(pc::Status::Reusable).size() == 2);
    CB_CHECK(video.with_status(pc::Status::Missing).size() == 1 &&
             video.with_status(pc::Status::Missing)[0]->name == "GL11::glNeverHeardOfIt");
    // the types: Tessellator is used (a video type), GLAllocation is not
    CB_CHECK_EQ(video.types.size(), std::size_t{1});
    CB_CHECK_EQ(video.types[0].name, "Tessellator");
    CB_CHECK_EQ(video.types[0].count, std::size_t{2});
    CB_CHECK(!video.types[0].in_reference);
    const pc::Coverage& input = r[1];
    CB_CHECK(!input.ref_exists);   // no Input reference files here
    CB_CHECK_EQ(input.symbols.size(), std::size_t{2});
    CB_CHECK(input.with_status(pc::Status::Missing)[0]->name == "Keyboard::getEventKey");
    const pc::Coverage& network = r[5];
    CB_CHECK_EQ(network.types.size(), std::size_t{1});   // Socket in Game.h
    CB_CHECK_EQ(network.types[0].name, "Socket");
});

CB_TEST("java.minecraft.platform_coverage.runtime_files_are_not_game_code", [] {
    CovTree t;
    // GL11.h mentions GL11:: only inside its own namespace line; naming it as a runtime file leaves it out of the scan
    fs::write_text(t.out / "GL11.h", "namespace GL11 {\ninline void glBegin(int m) {}\ninline void glEnd() { GL11::glEnd(); }\n}\n");
    const auto with_runtime = pc::analyze(t.out, t.ref, cov_runtime_files());
    const auto without = pc::analyze(t.out, t.ref, {"Keyboard.h"});
    std::size_t end_with = 0, end_without = 0;
    for (const auto& s : with_runtime[0].symbols) end_with += s.name == "GL11::glEnd" ? s.count : 0;
    for (const auto& s : without[0].symbols) end_without += s.name == "GL11::glEnd" ? s.count : 0;
    CB_CHECK_EQ(end_with, std::size_t{1});
    CB_CHECK_EQ(end_without, std::size_t{2});
});

CB_TEST("java.minecraft.platform_coverage.no_reference_marks_nothing_reusable", [] {
    CovTree t;
    const auto r = pc::analyze(t.out, std::nullopt, cov_runtime_files());
    CB_CHECK(!r[0].ref_exists);
    CB_CHECK(r[0].with_status(pc::Status::Reusable).empty());
    CB_CHECK_EQ(r[0].with_status(pc::Status::Stub).size(), std::size_t{2});   // glBegin and glEnd
    const auto missing_dir = pc::analyze(t.out / "nope", std::nullopt, {});
    CB_CHECK_EQ(missing_dir.size(), std::size_t{8});
    CB_CHECK(missing_dir[0].symbols.empty());
});

CB_TEST("java.minecraft.platform_coverage.report_sections", [] {
    CovTree t;
    fs::write_text(t.out / "Stubby.h", "void f() {} // J2X-STUB[video-x]: placeholder\n");
    const auto results = pc::analyze(t.out, t.ref, cov_runtime_files());
    const std::string md = pc::render_report(results, t.ref, pc::scan_stub_markers(t.out));
    CB_CHECK(md.starts_with("# PLATFORM_COVERAGE.md -- platform-layer coverage & decisions\n"));
    CB_CHECK(cov_test_has(md, "| Video | yes | 2 | 1 | 1 |"));   // reusable, stub, missing
    CB_CHECK(cov_test_has(md, "| Input | no | 0 | 1 | 1 |"));
    CB_CHECK(cov_test_has(md, "symbol(s) are MISSING**"));
    CB_CHECK(cov_test_has(md, "### MISSING -- new work for this version (1)"));
    CB_CHECK(cov_test_has(md, "| `GL11::glNeverHeardOfIt` | 1 | not in the stub OR the Alpha reference -- implement it |"));
    CB_CHECK(cov_test_has(md, "### REUSABLE -- already implemented in the Alpha reference (2)"));
    CB_CHECK(cov_test_has(md, "| `GL11::glBegin` | 3 |"));
    CB_CHECK(cov_test_has(md, "### STUB -- placeholder only, verify it is not load-bearing (1)"));
    CB_CHECK(cov_test_has(md, "**Reference implementation:** `GL11.reference.*`, `Shaders.reference.*`"));
    CB_CHECK(cov_test_has(md, "- ⚠️ HALF-PIXEL OFFSET"));
    CB_CHECK(cov_test_has(md, "### Platform types referenced"));
    CB_CHECK(cov_test_has(md, "## J2X-STUB markers -- the explicit placeholder surface (1)"));
    CB_CHECK(cov_test_has(md, "| `video-x` | 1 | `Stubby.h:1` -- placeholder |"));
    // no SDK was scanned
    CB_CHECK(cov_test_has(md, "> No SDK was scanned, so requirements could not be auto-detected."));
    // the XDK equivalents table is always there
    CB_CHECK(cov_test_has(md, "## Xbox 360 XDK equivalents -- the real API for every platform symbol"));
    CB_CHECK(cov_test_has(md, "| Subsystem | Java symbol | Xbox 360 XDK API | Lib | Status |"));
    CB_CHECK(md.ends_with("\n") && !md.ends_with("\n\n\n"));
    // a subsystem with nothing used and no notes is left out
    CB_CHECK(!cov_test_has(md, "## Display / window / timing"));
    CB_CHECK(!cov_test_has(md, "> NOTE: the Alpha reference layer was not found"));
});

CB_TEST("java.minecraft.platform_coverage.report_without_a_reference_says_so", [] {
    CovTree t;
    const auto results = pc::analyze(t.out, std::nullopt, {"GL11.h"});
    const std::string md = pc::render_report(results, std::nullopt, {});
    CB_CHECK(cov_test_has(md, "> NOTE: the Alpha reference layer was not found, so nothing could be marked REUSABLE."));
    CB_CHECK(!cov_test_has(md, "J2X-STUB markers"));
    const std::string md2 = pc::render_report(results, t.dir.path() / "no_such_reference", {});
    CB_CHECK(cov_test_has(md2, "> NOTE: the Alpha reference layer was not found"));
});

CB_TEST("java.minecraft.platform_coverage.report_with_sdk_requirements_and_opportunities", [] {
    fs::ScopedTempDir sdk("cb-cov-sdk");
    fs::make_dirs(sdk.path() / "include" / "xbox");
    fs::write_text(sdk.path() / "include" / "xbox" / "xam.h",
                   "DWORD XShowDeviceSelectorUI(DWORD user, DWORD type, DWORD flags, ULARGE_INTEGER bytes, DWORD* id, XOVERLAPPED* ov);\n"
                   "DWORD XNetStartup(const XNetStartupParams* p);\n");
    const auto scan = cb::java::console::xdk_scan::detect(sdk.path());
    CB_CHECK(scan.include.has_value());
    CovTree t;
    const auto results = pc::analyze(t.out, t.ref, cov_runtime_files());
    const std::string md = pc::render_report(results, t.ref, {}, scan);
    CB_CHECK(cov_test_has(md, "## Xbox 360 platform requirements -- auto-detected from the installed SDK"));
    CB_CHECK(cov_test_has(md, "`XShowDeviceSelectorUI` -- xam.h | CERT |"));
    CB_CHECK(cov_test_has(md, "Storage device must be selected before saving"));
    CB_CHECK(cov_test_has(md, "`XNetStartup` -- xam.h | COMPAT |"));
    // a requirement the SDK does not declare is not claimed
    CB_CHECK(!cov_test_has(md, "GPU resources should come from physical memory"));
    CB_CHECK(!cov_test_has(md, "No SDK was scanned"));
});

CB_TEST("java.minecraft.platform_coverage.write_report_writes_the_file", [] {
    CovTree t;
    fs::ScopedTempDir empty_sdk("cb-cov-sdk");   // a folder with no headers: nothing to detect
    pc::WriteOptions options;
    options.reference_dir = t.ref;
    options.runtime_files = cov_runtime_files();
    options.sdk_root = empty_sdk.path();
    const pc::Written w = pc::write_report(t.out, options);
    CB_CHECK_EQ(w.path.filename().string(), "PLATFORM_COVERAGE.md");
    CB_CHECK_EQ(w.results.size(), std::size_t{8});
    const std::string md = fs::read_text(w.path);
    CB_CHECK(cov_test_has(md, "| Video | yes | 2 | 1 | 1 |"));
    CB_CHECK(cov_test_has(md, "# PLATFORM_COVERAGE.md"));
    // the report is written next to the sources it describes, and not counted as game code by the next run
    CB_CHECK(stdfs::exists(t.out / "PLATFORM_COVERAGE.md"));
});

}  // namespace
