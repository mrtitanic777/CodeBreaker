#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/debug.hpp"

namespace {

using namespace cb::java;
namespace stdfs = std::filesystem;

std::string golden(const char* relative) { return cb::fs::read_text(cb::testing::data_path(std::string("java/testdata/") + relative)); }

// testdata/debug_on/*, debug_off/Debug.h: the three files exactly as the Python module's constants and
// write_debug() produce them.
CB_TEST("java.debug.text_matches_python", [] {
    CB_CHECK_EQ(debug::debug_header(true), golden("debug_on/Debug.h"));
    CB_CHECK_EQ(debug::debug_header(false), golden("debug_off/Debug.h"));
    CB_CHECK_EQ(std::string(debug::debug_cpp()), golden("debug_on/Debug.cpp"));
    CB_CHECK_EQ(std::string(debug::debug_readme()), golden("debug_on/DEBUG.md"));
    // the retail switch differs in exactly one line
    const std::string on = debug::debug_header(true), off = debug::debug_header(false);
    CB_CHECK(on.find("#define DEBUG_ENABLED 1          // 1 = debug build") != std::string::npos);
    CB_CHECK(off.find("#define DEBUG_ENABLED 0          // 1 = debug build") != std::string::npos);
    CB_CHECK_EQ(on.size(), off.size());
});

CB_TEST("java.debug.write", [] {
    cb::fs::ScopedTempDir tmp("cbdebug");
    auto paths = debug::write_debug(tmp.path() / "out", false);
    CB_CHECK_EQ(paths.size(), std::size_t{3});
    CB_CHECK_EQ(paths[0].filename().string(), "Debug.h");
    CB_CHECK_EQ(paths[1].filename().string(), "Debug.cpp");
    CB_CHECK_EQ(paths[2].filename().string(), "DEBUG.md");
    CB_CHECK_EQ(cb::fs::read_text(paths[0]), golden("debug_off/Debug.h"));
    CB_CHECK_EQ(cb::fs::read_text(paths[1]), golden("debug_on/Debug.cpp"));
    CB_CHECK_EQ(cb::fs::read_text(paths[2]), golden("debug_on/DEBUG.md"));
});

CB_TEST("java.debug.scan_markers", [] {
    cb::fs::ScopedTempDir tmp("cbdebug");
    const stdfs::path dir = tmp.path();
    cb::fs::write_text(dir / "b.cpp", "int x;\r\nvoid f() {}  // J2X-DEBUG[leak-impl]\nint y;  // J2X-DEBUG[once] and J2X-DEBUG[net]\n");
    cb::fs::write_text(dir / "a.h", "// J2X-DEBUG[once]\n// J2X-DEBUG[Bad] J2X-DEBUG[ok-1]\n// J2X-DEBUG[] x\n// J2X-DEBUG[unterminated\n");
    cb::fs::write_text(dir / "c.txt", "J2X-DEBUG[ignored]\n");                 // wrong extension
    cb::fs::write_text(dir / "sub" / "d.cpp", "J2X-DEBUG[nested]\n");            // not scanned: top level only
    cb::fs::write_text(dir / "plain.cpp", "no marker here\n");

    auto found = debug::scan_debug_markers(dir);
    CB_CHECK_EQ(found.size(), std::size_t{3});
    CB_CHECK_EQ(found.at("once").size(), std::size_t{2});
    CB_CHECK_EQ(found.at("once")[0].file, "a.h");                  // files in name order
    CB_CHECK_EQ(found.at("once")[0].line, std::uint32_t{1});
    CB_CHECK_EQ(found.at("once")[1].file, "b.cpp");
    CB_CHECK_EQ(found.at("once")[1].line, std::uint32_t{3});
    CB_CHECK_EQ(found.at("leak-impl")[0].line, std::uint32_t{2});  // CRLF counted as one line break
    CB_CHECK_EQ(found.at("ok-1")[0].file, "a.h");                  // "Bad" is not a tag, the next marker on the line is
    CB_CHECK_EQ(found.at("ok-1")[0].line, std::uint32_t{2});
    CB_CHECK(!found.contains("net"));                              // only the first marker on a line counts
    CB_CHECK(!found.contains("ignored") && !found.contains("nested") && !found.contains("Bad"));
    CB_CHECK_THROWS_AS(cb::NotFoundError, debug::scan_debug_markers(dir / "no-such-folder"));
});

CB_TEST("java.debug.scan_of_generated_files", [] {
    // the emitted Debug.h/Debug.cpp carry the markers the catalogue is built from
    cb::fs::ScopedTempDir tmp("cbdebug");
    debug::write_debug(tmp.path());
    auto found = debug::scan_debug_markers(tmp.path());
    for (const char* tag : {"dbg", "once", "every", "throttle", "assert", "crash-soft", "crash-hard", "leak-new",
                            "leak-free", "net", "net-event", "summary", "printf", "throttle-impl", "leak-impl",
                            "leak-free-impl", "net-impl", "summary-impl", "crash-impl"}) {
        CB_CHECK(found.contains(tag));
    }
});

}  // namespace

