#include "cb/java/analyze.hpp"
#include "cb/java/strview.hpp"

#include <chrono>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace an = cb::java::analyze;
using cb::java::ledger::Category;
using cb::java::ledger::Ledger;
using cb::java::ledger::Severity;

// The converted output of a small project: a header naming its target, a source full of markers.
void write_converted_tree(const std::filesystem::path& dir) {
    cb::fs::write_text(dir / "Foo.h", "// Foo.h\n// Target: Xbox 360 (XDK)\n#pragma once\nclass Foo {};\n");
    cb::fs::write_text(dir / "Foo.cpp",
                       "#include \"Foo.h\"\n"
                       "void f() { } // PORT: value type assumed, check this\n"                      // 2 guessed
                       "// PORT-BLOCKED: no reflection\n"                                              // 3 blocked
                       "// PORT stub: needs D3D9\n"                                                    // 4 stubbed
                       "// TODO: later\n"                                                              // 5 warning
                       "/* FIXME clean this */\n"                                                      // 6 warning
                       "// PORT: plain note\n"                                                         // 7 annotated
                       "// PORT: XInput gamepad\n"                                                     // 8 stubbed (a hint)
                       "x = \"TODO\";\n"                                                               // 9 no comment: the line itself
                       "// PORT(post-vs-pre-multiply) blending\n"                                      // 10 annotated
                       "// PORT NOTES: tidy\n"                                                         // 11 annotated
                       "// PORT-WARNING check overflow\n"                                              // 12 warning
                       "// PORT\n"                                                                     // 13 annotated, empty message
                       "// nothing special here\n"
                       "load(\"/gui/furnace->png\");\n"                                                // 15 corrupted literal
                       "bool* a = new bool[4];\n");                                                    // 16 uninit-array
    cb::fs::write_text(dir / "notes.txt", "// PORT: in a text file is ignored\n");
}

CB_TEST("java.analyze.scan_markers_classifies_by_marker_and_by_content", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    write_converted_tree(tmp.path());
    Ledger l;
    CB_CHECK_EQ(an::scan_markers(l, tmp.path()), static_cast<std::size_t>(12));
    const auto& e = l.events();
    auto at = [&](std::size_t i) { return e[i].file + ":" + std::to_string(e[i].line); };
    CB_CHECK_EQ(at(0), "Foo.cpp:2");
    CB_CHECK(e[0].category == Category::Guessed && e[0].severity == Severity::High && e[0].source == "marker" && e[0].rule == "PORT");
    CB_CHECK_EQ(e[0].message, "value type assumed, check this");
    CB_CHECK_EQ(e[0].snippet, "void f() { } // PORT: value type assumed, check this");
    CB_CHECK(e[1].line == 3 && e[1].category == Category::Blocked && e[1].rule == "PORT-BLOCKED" && e[1].message == "no reflection");
    CB_CHECK(e[2].line == 4 && e[2].category == Category::Stubbed && e[2].rule == "PORT stub" && e[2].message == "needs D3D9");
    CB_CHECK(e[3].line == 5 && e[3].category == Category::Warning && e[3].rule == "TODO" && e[3].message == "later");
    CB_CHECK(e[4].line == 6 && e[4].category == Category::Warning && e[4].rule == "FIXME" && e[4].message == "clean this");
    CB_CHECK(e[5].line == 7 && e[5].category == Category::Annotated && e[5].severity == Severity::Low && e[5].message == "plain note");
    CB_CHECK(e[6].line == 8 && e[6].category == Category::Stubbed && e[6].severity == Severity::High);   // `XInput`/`gamepad` sharpen it
    CB_CHECK(e[7].line == 9 && e[7].category == Category::Warning && e[7].message == "\";");    // no comment: the whole line
    CB_CHECK(e[8].line == 10 && e[8].category == Category::Annotated && e[8].rule == "PORT(post-vs-pre-multiply)" && e[8].message == "blending");
    CB_CHECK(e[9].line == 11 && e[9].category == Category::Annotated && e[9].rule == "PORT NOTES" && e[9].message == "tidy");
    CB_CHECK(e[10].line == 12 && e[10].category == Category::Warning && e[10].rule == "PORT-WARNING");
    // a marker with nothing after it has the whole comment as its message
    Ledger bare;
    cb::fs::write_text(tmp.path() / "Bare.cpp", "// PORT\n");
    cb::fs::write_text(tmp.path() / "Foo.cpp", "");
    CB_CHECK_EQ(an::scan_markers(bare, tmp.path()), static_cast<std::size_t>(1));
    CB_CHECK(bare.events()[0].message == "PORT" && bare.events()[0].category == Category::Annotated);
});

CB_TEST("java.analyze.scan_markers_clips_long_snippets_and_skips_unreadable_names", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    cb::fs::write_text(tmp.path() / "A.cpp", "// TODO " + std::string(300, 'x') + "\n");
    std::filesystem::create_directories(tmp.path() / "Dir.cpp");   // a folder that looks like a source
    Ledger l;
    CB_CHECK_EQ(an::scan_markers(l, tmp.path()), static_cast<std::size_t>(1));
    CB_CHECK_EQ(l.events()[0].snippet.size(), static_cast<std::size_t>(160));
    CB_CHECK_EQ(an::scan_markers(l, tmp.path() / "missing"), static_cast<std::size_t>(0));
});

CB_TEST("java.analyze.scan_corrupted_strings", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    write_converted_tree(tmp.path());
    cb::fs::write_text(tmp.path() / "Ok.cpp", "load(\"/gui/furnace.png\"); a->b; \"x->y\";\n");
    Ledger l;
    CB_CHECK_EQ(an::scan_corrupted_strings(l, tmp.path()), static_cast<std::size_t>(1));
    const auto& e = l.events()[0];
    CB_CHECK(e.file == "Foo.cpp" && e.line == 15 && e.category == Category::Risk && e.severity == Severity::High);
    CB_CHECK(e.source == "lint" && e.rule == "high/corrupt-string-literal" && e.snippet == "load(\"/gui/furnace->png\");");
    CB_CHECK(cb::java::contains(e.message, "\"/gui/furnace->png\""));
});

CB_TEST("java.analyze.scan_build_log_parses_msvc_errors_once", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    const std::filesystem::path log = tmp.path() / "build.log";
    cb::fs::write_text(log,
                       "1>Foo.cpp(12): error C2065: 'x' : undeclared identifier\r\n"
                       "2>Foo.cpp(12): error C2065: 'x' : undeclared identifier\r\n"          // the same error again (/MP)
                       "Foo.h(3) : fatal error C1083: Cannot open include file: 'a.h'\r\n"
                       "Foo.cpp(40): warning C4101: unused\r\n"
                       "Bar.cpp(7): error C2143: syntax error : missing ';' before '}'  \r\n"
                       "Foo.cpp(12): error C2065: 'y' : undeclared identifier\n"             // another message: another event
                       "not an error line\n"
                       "Dir/Baz.cpp(1): error C1: x\n");                                       // a path prefix: matched on its file name
    Ledger l;
    CB_CHECK_EQ(an::scan_build_log(l, log), static_cast<std::size_t>(5));
    const auto& e = l.events();
    CB_CHECK(e[0].file == "Foo.cpp" && e[0].line == 12 && e[0].rule == "C2065" && e[0].category == Category::Error && e[0].source == "build");
    CB_CHECK_EQ(e[0].message, "'x' : undeclared identifier");
    CB_CHECK(e[1].file == "Foo.h" && e[1].line == 3 && e[1].rule == "C1083");
    CB_CHECK(e[2].file == "Bar.cpp" && e[2].message == "syntax error : missing ';' before '}'");
    CB_CHECK(e[3].message == "'y' : undeclared identifier");
    CB_CHECK(e[4].file == "Baz.cpp");
    CB_CHECK_EQ(an::scan_build_log(l, tmp.path() / "nope.log"), static_cast<std::size_t>(0));
    CB_CHECK_EQ(an::scan_build_log(l, tmp.path()), static_cast<std::size_t>(0));   // a folder is not a log
});

CB_TEST("java.analyze.newest_build_log_picks_the_most_recent", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    CB_CHECK(!an::newest_build_log(tmp.path()).has_value());
    CB_CHECK(!an::newest_build_log(tmp.path() / "missing").has_value());
    cb::fs::write_text(tmp.path() / "build_alpha_old.log", "x");
    cb::fs::write_text(tmp.path() / "build_alpha_new.log", "x");
    cb::fs::write_text(tmp.path() / "other.log", "x");
    cb::fs::write_text(tmp.path() / "build_alpha_x.txt", "x");
    const auto now = std::filesystem::file_time_type::clock::now();
    std::filesystem::last_write_time(tmp.path() / "build_alpha_old.log", now - std::chrono::hours(2));
    std::filesystem::last_write_time(tmp.path() / "build_alpha_new.log", now - std::chrono::hours(1));
    std::filesystem::last_write_time(tmp.path() / "other.log", now);   // newer, but not a build_alpha log
    const auto newest = an::newest_build_log(tmp.path());
    CB_CHECK(newest && newest->filename() == "build_alpha_new.log");
});

CB_TEST("java.analyze.detect_platform_reads_the_target_comment_of_a_header", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    CB_CHECK(!an::detect_platform(tmp.path()).has_value());
    cb::fs::write_text(tmp.path() / "A.h", "// no target here\n");
    CB_CHECK(!an::detect_platform(tmp.path()).has_value());
    cb::fs::write_text(tmp.path() / "B.h", "// Target: Not A Platform\n");   // a target no platform has: keep looking
    CB_CHECK(!an::detect_platform(tmp.path()).has_value());
    cb::fs::write_text(tmp.path() / "C.h", std::string(11, '\n') + "// Target: Xbox 360\n");   // past the first eleven lines
    CB_CHECK(!an::detect_platform(tmp.path()).has_value());
    cb::fs::write_text(tmp.path() / "D.h", std::string(10, '\n') + "// Target: Xbox 360\n");   // the eleventh line
    CB_CHECK(an::detect_platform(tmp.path()) == std::optional<std::string>("xbox360"));
});

CB_TEST("java.analyze.analyze_all_runs_every_analyzer", [] {
    cb::fs::ScopedTempDir tmp("cbanalyze");
    const auto out = tmp.path() / "out";
    write_converted_tree(out);
    const auto root = tmp.path() / "root";
    cb::fs::write_text(root / "build_alpha1.log", "Foo.cpp(2): error C2065: 'z' : undeclared identifier\n");
    Ledger l;
    const an::Counts c = an::analyze_all(l, out, {.root = root});
    CB_CHECK_EQ(c.markers, static_cast<std::size_t>(12));
    CB_CHECK_EQ(c.corrupt_strings, static_cast<std::size_t>(1));
    CB_CHECK_EQ(c.build_log, static_cast<std::size_t>(1));
    CB_CHECK(c.build_log_path && c.build_log_path->filename() == "build_alpha1.log");
    // the rules ran for the platform the header names (xbox360): uninit-array on line 16
    CB_CHECK_EQ(c.rules, static_cast<std::size_t>(1));
    CB_CHECK_EQ(l.size(), c.markers + c.rules + c.corrupt_strings + c.build_log);
    // the order of the passes: markers, rules, corrupt strings, build log
    CB_CHECK(l.events()[0].source == "marker" && l.events()[12].source == "rule" && l.events()[13].source == "lint" && l.events()[14].source == "build");
    // an explicit log and platform win over what is found; no log at all is fine
    Ledger l2;
    const an::Counts c2 = an::analyze_all(l2, out, {.build_log = root / "missing.log", .root = root, .platform = "ps3"});
    CB_CHECK(c2.build_log == 0 && c2.build_log_path && c2.build_log_path->filename() == "missing.log");
    Ledger l3;
    const an::Counts c3 = an::analyze_all(l3, out, {.root = tmp.path() / "empty"});
    CB_CHECK(!c3.build_log_path.has_value() && c3.build_log == 0);
});

}  // namespace
