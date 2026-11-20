// Tests of the sanitized run's key and of what the loop does without a compiler that has the sanitizer. (The rounds themselves need
// clang and a built port; they are what `codebreaker port --cases` runs.)

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "port/sanitize_run.hpp"

using namespace cb;
using namespace cb::port::tool;
namespace sanitize = cb::port::sanitize;

CB_TEST("port.sanitize_run.the_key_is_all_the_run_depends_on", [] {
    fs::ScopedTempDir tmp("cb-sanrun");
    fs::write_text(tmp.path() / "a.c", "int a;\n");
    fs::write_text(tmp.path() / "a.h", "int h;\n");
    fs::write_text(tmp.path() / "notes.txt", "not part of it");
    fs::write_text(tmp.path() / "in.txt", "input one");
    fs::write_text(tmp.path() / "cases.json", R"({"inputs": {"IN.TXT": "in.txt"}, "cases": [{"name": "a", "cmd": "X"}]})");
    const std::string key = sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe");
    CB_CHECK_EQ(key, sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe"));
    // not a file the build reads
    fs::write_text(tmp.path() / "notes.txt", "changed");
    CB_CHECK_EQ(key, sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe"));
    // the port, the compiler, the cases, an input they name
    fs::write_text(tmp.path() / "a.c", "int a2;\n");
    const std::string after_c = sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe");
    CB_CHECK(after_c != key);
    CB_CHECK(after_c != sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/other/clang.exe"));
    fs::write_text(tmp.path() / "in.txt", "input two");
    const std::string after_input = sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe");
    CB_CHECK(after_input != after_c);
    fs::write_text(tmp.path() / "cases.json", R"({"inputs": {"IN.TXT": "in.txt"}, "cases": [{"name": "b", "cmd": "X"}]})");
    CB_CHECK(sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe") != after_input);
    // cases that cannot be read still make a key (of their own)
    fs::write_text(tmp.path() / "cases.json", "{ not json");
    const std::string broken = sanitize_key(tmp.path(), tmp.path() / "cases.json", "C:/llvm/bin/clang.exe");
    CB_CHECK(broken != after_input);
});

CB_TEST("port.sanitize_run.without_a_compiler_nothing_is_run", [] {
    fs::ScopedTempDir tmp("cb-sanrun");
    Compilers compilers;
    compilers["clang"].find = {"definitely-not-a-clang-anywhere"};
    compilers["gcc"].find = {"definitely-not-a-gcc-anywhere"};
    int written = 0;
    SanitizeSetup setup;
    setup.compilers = &compilers;
    setup.compiler = "gcc";
    setup.out = tmp.path();
    SanitizeHooks hooks{[&] { ++written; }, [](const std::vector<sanitize::Report>&) { return std::vector<sanitize::Mended>{}; }};
    const std::vector<std::string> notes = sanitize_loop(setup, hooks);
    CB_CHECK_EQ(notes.size(), std::size_t{1});
    CB_CHECK_EQ(notes[0], "no compiler with AddressSanitizer found: the cases were not run sanitized");
    CB_CHECK_EQ(written, 0);
    // msvc is not tried first (it has no sanitizer of this kind)
    setup.compiler = "msvc";
    CB_CHECK_EQ(sanitize_loop(setup, hooks).size(), std::size_t{1});
});
