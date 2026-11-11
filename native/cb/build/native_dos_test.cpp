// Tests of the native DOS runner's driving code over a fake executive: what is written, what command line MASM
// gets, what is read back, the switch, and how a refusal of the executive reaches the caller. (The real executive,
// exact/emu/dos, is tested with its own module.)
//
// DIVERGES from the Python:
//   * a stem with an empty, `.` or `..` part is a UsageError (the Python joined it to the work folder as given, so
//   `../x`
//     wrote outside it);
//   * the work folder is deleted when the call returns (the products are already read into the result; the Python left
//   it
//     for the caller to remove, and the result named it);
//   * a .LST / .LOG keeps the CRLF line ends the tool wrote (the Python's text-mode read translated them to LF);
//   * an unknown value of CB_NATIVE_DOS means "auto", as it did.
#include "cb/build/native_dos.hpp"

#include <functional>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// (a container is compared with ==; the failure message of CB_CHECK_EQ cannot render one)
#define CHECK_SAME(a, ...) CB_CHECK((a) == (__VA_ARGS__))

namespace {

using namespace cb;
using namespace cb::build;
namespace nd = cb::build::native_dos;
namespace stdfs = std::filesystem;

// Stands in for MASM: for the command line `A.ASM,A.OBJ,A.LST,NUL;` it reads A.ASM from the work folder and writes
// A.OBJ (the source's bytes reversed) and A.LST (its text, upper-cased). A source that contains "NOLOG" makes no
// .LOG; one that contains "SILENT" makes no .OBJ either. The console always says "console".
class FakeDos : public nd::DosExecutive {
public:
    bool available_ = true;
    std::optional<std::string> refuse; // throw this from run
    mutable std::vector<std::string> command_lines;
    mutable std::vector<stdfs::path> work_folders;
    mutable nd::MemoryRun last_memory_run;
    std::function<nd::MemoryResult(const nd::MemoryRun&)> dpmi;

    bool available() const override { return available_; }

    nd::Exit run(const stdfs::path& exe, const stdfs::path& work, std::string_view cmdline) const override {
        if (refuse)
            throw EmulationError(*refuse);
        CB_CHECK(stdfs::is_regular_file(exe));
        command_lines.emplace_back(cmdline);
        work_folders.push_back(work);
        const std::string name = std::string(cmdline.substr(0, cmdline.find(".ASM,")));
        std::string folder_name = text::replace_all(name, "\\", "/");
        const Bytes source = fs::read_file(work / (folder_name + ".ASM"));
        const std::string text(as_text(source));
        if (text.find("SILENT") == std::string::npos) {
            Bytes obj(source.rbegin(), source.rend());
            fs::write_file(work / (folder_name + ".OBJ"), obj);
        }
        fs::write_text(work / (folder_name + ".LST"), text::upper(text));
        if (text.find("NOLOG") == std::string::npos)
            fs::write_text(work / (folder_name + ".LOG"), "log of " + name);
        return {0, to_bytes("console")};
    }

    nd::MemoryResult run_dpmi(const nd::MemoryRun& r) const override {
        if (refuse)
            throw EmulationError(*refuse);
        last_memory_run = r;
        return dpmi ? dpmi(r) : nd::MemoryResult{};
    }
};

stdfs::path fake_masm_root(const fs::ScopedTempDir& dir) {
    fs::write_text(dir.path() / "masm" / "BIN" / "MASM.EXE", "MZ-not-really");
    return dir.path() / "masm";
}

std::string text_of(const std::optional<Bytes>& b) {
    return b ? std::string(as_text(*b)) : "<none>";
}

} // namespace

CB_TEST("build.native_dos.assemble_reads_back_the_products", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    const dos_sessions::Product p = nd::assemble(dos, "mov ax,1\r\nint 21h\n", "t", fake_masm_root(dir));
    // the source reached DOS as clean CRLF text; the command line names stem.ASM,.OBJ,.LST and no cross-reference
    CB_CHECK_EQ(dos.command_lines.size(), 1u);
    CB_CHECK_EQ(dos.command_lines[0], "t.ASM,t.OBJ,t.LST,NUL;");
    const std::string want_source = "mov ax,1\r\nint 21h\r\n";
    const Bytes obj(want_source.rbegin(), want_source.rend());
    CHECK_SAME(p.obj, std::optional<Bytes>(obj));
    CB_CHECK_EQ(p.lst.value_or("<none>"), "MOV AX,1\r\nINT 21H\r\n");
    CB_CHECK_EQ(p.log.value_or("<none>"), "log of t");
    // the work folder is gone when the call returns
    CB_CHECK(!stdfs::exists(dos.work_folders.at(0)));
});

CB_TEST("build.native_dos.missing_parts_and_console_log", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    const dos_sessions::Product p = nd::assemble(dos, "SILENT NOLOG", "x", fake_masm_root(dir));
    CB_CHECK(!p.obj.has_value()); // MASM made no object
    CB_CHECK(p.lst.has_value());
    CB_CHECK_EQ(p.log.value_or("<none>"), "console"); // no .LOG: what MASM printed
});

CB_TEST("build.native_dos.stem_with_folders", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    const dos_sessions::Product p = nd::assemble(dos, "abc", "asm/lead00", fake_masm_root(dir));
    CB_CHECK_EQ(dos.command_lines.at(0), "asm\\lead00.ASM,asm\\lead00.OBJ,asm\\lead00.LST,NUL;");
    CB_CHECK_EQ(text_of(p.obj), "cba");
    // a stem may not leave the work folder
    CB_CHECK_THROWS_AS(UsageError, (void)nd::assemble(dos, "abc", "../evil", fake_masm_root(dir)));
    CB_CHECK_THROWS_AS(UsageError, (void)nd::assemble(dos, "abc", "a//b", fake_masm_root(dir)));
    CB_CHECK_THROWS_AS(UsageError, (void)nd::assemble(dos, "abc", "", fake_masm_root(dir)));
});

CB_TEST("build.native_dos.assemble_many_in_order", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    const std::vector<dos_sessions::Source> items{{"one", "111"}, {"two", "22 NOLOG"}, {"three", "3"}};
    const dos_sessions::Finished done = nd::assemble_many(dos, items, fake_masm_root(dir));
    CB_CHECK_EQ(done.results.size(), 3u);
    CB_CHECK_EQ(done.results[0].first, "one");
    CB_CHECK_EQ(done.results[1].first, "two");
    CB_CHECK_EQ(done.results[2].first, "three");
    CB_CHECK_EQ(text_of(done.results[0].second.obj), "111");
    CB_CHECK_EQ(done.results[1].second.log.value_or("<none>"), "console");
    CB_CHECK_EQ(done.results[2].second.log.value_or("<none>"), "log of three");
    // one folder for the whole batch, deleted at the end
    CB_CHECK_EQ(dos.work_folders.size(), 3u);
    CB_CHECK(dos.work_folders[0] == dos.work_folders[2]);
    CB_CHECK(!stdfs::exists(dos.work_folders[0]));
    CB_CHECK(done.work.empty());
});

CB_TEST("build.native_dos.a_refusal_becomes_native_dos_error", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    dos.refuse = "unimplemented INT 21h AH=5Fh";
    bool threw = false;
    try {
        (void)nd::assemble(dos, "x", "t", fake_masm_root(dir));
    } catch (const nd::NativeDosError& e) {
        threw = true;
        CB_CHECK(std::string(e.what()).find("unimplemented INT 21h") != std::string::npos);
        CB_CHECK(std::string(e.what()).find("EmulationError") != std::string::npos);
    }
    CB_CHECK(threw);
    const std::vector<dos_sessions::Source> items{{"a", "1"}};
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)nd::assemble_many(dos, items, fake_masm_root(dir)));
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)nd::run_extended(dos, {}, "ML.EXE", "", {}, {}));
});

CB_TEST("build.native_dos.missing_masm_is_native_dos_error", [] {
    fs::ScopedTempDir dir("nd");
    FakeDos dos;
    CB_CHECK_THROWS_AS(nd::NativeDosError, (void)nd::assemble(dos, "x", "t", dir.path() / "no-masm-here"));
});

CB_TEST("build.native_dos.the_switch", [] {
    FakeDos dos;
    // unset or auto: the proven tools only
    CB_CHECK(nd::enabled(dos, "masm", nd::Mode::Auto));
    CB_CHECK(nd::enabled(dos, "ml", nd::Mode::Auto));
    CB_CHECK(!nd::enabled(dos, "link", nd::Mode::Auto));
    CB_CHECK(nd::enabled(dos, "link", nd::Mode::On));
    CB_CHECK(!nd::enabled(dos, "masm", nd::Mode::Off));
    dos.available_ = false; // no engine: never, even forced
    CB_CHECK(!nd::enabled(dos, "masm", nd::Mode::On));
    CB_CHECK(!nd::enabled(dos, "masm", nd::Mode::Auto));
    {
        fs::ScopedEnv unset("CB_NATIVE_DOS", std::nullopt);
        CB_CHECK(nd::mode_from_environment() == nd::Mode::Auto);
    }
    for (const char* v : {"0", "off", "No", "FALSE"}) {
        fs::ScopedEnv env("CB_NATIVE_DOS", v);
        CB_CHECK(nd::mode_from_environment() == nd::Mode::Off);
    }
    for (const char* v : {"1", "ON", "yes", "True"}) {
        fs::ScopedEnv env("CB_NATIVE_DOS", v);
        CB_CHECK(nd::mode_from_environment() == nd::Mode::On);
    }
    {
        fs::ScopedEnv env("CB_NATIVE_DOS", "auto");
        CB_CHECK(nd::mode_from_environment() == nd::Mode::Auto);
    }
    {
        fs::ScopedEnv env("CB_NATIVE_DOS", "perhaps"); // not understood: auto
        CB_CHECK(nd::mode_from_environment() == nd::Mode::Auto);
    }
});

CB_TEST("build.native_dos.run_extended_files_in_memory", [] {
    fs::ScopedTempDir dir("nd");
    fs::write_file(dir.path() / "tool.exe", Bytes{1, 2, 3});
    FakeDos dos;
    dos.dpmi = [](const nd::MemoryRun& r) {
        nd::MemoryResult out;
        out.console = to_bytes("ok");
        for (const std::string& a : r.artifacts)
            out.made[a] = r.files.at("M.ASM"); // "makes" each artifact: the source
        return out;
    };
    const std::map<std::string, DosFile> inputs{{"m.asm", std::string("a\nb\r\nc")}, {"data.bin", Bytes{0x0A, 0xFF}}};
    const std::vector<std::string> artifacts{"m.obj"};
    const nd::MemoryResult r =
        nd::run_extended(dos, {{"tool.exe", dir.path() / "tool.exe"}}, "tool.exe", "/c m.asm", inputs, artifacts);
    const nd::MemoryRun& seen = dos.last_memory_run;
    CB_CHECK_EQ(seen.exe, "TOOL.EXE");
    CB_CHECK_EQ(seen.cmdline, "/c m.asm");
    CHECK_SAME(seen.files.at("TOOL.EXE"), (Bytes{1, 2, 3}));
    CHECK_SAME(seen.files.at("M.ASM"), to_bytes("a\r\nb\r\nc")); // text gets CRLF ends
    CHECK_SAME(seen.files.at("DATA.BIN"), (Bytes{0x0A, 0xFF}));  // bytes are left alone
    CHECK_SAME(seen.env, nd::default_extended_env());
    CB_CHECK_EQ(std::string(as_text(nd::default_extended_env())), std::string("PATH=C:\\\0", 9));
    CHECK_SAME(r.made.at("m.obj"), to_bytes("a\r\nb\r\nc"));
    CB_CHECK_EQ(std::string(as_text(r.console)), "ok");
});

CB_TEST("build.native_dos.text_files_are_latin1", [] {
    FakeDos dos;
    const std::map<std::string, DosFile> inputs{{"m.asm", std::string("; caf\xC3\xA9")}}; // "café" in UTF-8
    (void)nd::run_extended(dos, {}, "ML.EXE", "", inputs, {});
    CHECK_SAME(dos.last_memory_run.files.at("M.ASM"), (Bytes{';', ' ', 'c', 'a', 'f', 0xE9})); // the Latin-1 byte
});

CB_TEST("build.native_dos.ml_toolchain", [] {
    fs::ScopedTempDir dir("nd");
    const nd::MlToolchain tc{dir.path() / "ddk", dir.path() / "msvc"};
    try {
        (void)nd::ml_files(tc);
        CB_CHECK(false);
    } catch (const nd::NativeDosError& e) {
        const std::string what = e.what();
        CB_CHECK(what.find("ML.EXE") != std::string::npos);
        CB_CHECK(what.find("ML.ERR") != std::string::npos);
        CB_CHECK(what.find("DOSXNT.EXE") != std::string::npos);
    }
    fs::write_text(tc.ml_dir / "ML.EXE", "ml");
    fs::write_text(tc.ml_dir / "ML.ERR", "err");
    fs::write_text(tc.msvc_root / "BIN" / "DOSXNT.EXE", "dosxnt");
    const auto files = nd::ml_files(tc);
    CB_CHECK_EQ(files.size(), 3u);

    FakeDos dos;
    dos.dpmi = [](const nd::MemoryRun& r) {
        nd::MemoryResult out;
        out.console = to_bytes("Assembling: m.asm\r\n");
        out.made["m.obj"] = r.files.at("ML.EXE");
        return out;
    };
    const std::vector<std::string> artifacts{"m.obj", "m.lst"};
    const nd::MlResult r = nd::ml(dos, tc, "/c /Fom.obj m.asm", {{"m.asm", std::string(".386")}}, artifacts);
    CB_CHECK_EQ(r.made.size(), 1u); // m.lst was not made
    CHECK_SAME(r.made.at("m.obj"), to_bytes("ml"));
    CB_CHECK_EQ(r.console, "Assembling: m.asm\r\n");
    CHECK_SAME(dos.last_memory_run.files.at("DOSXNT.EXE"), to_bytes("dosxnt"));
    CB_CHECK_EQ(dos.last_memory_run.cmdline, "/c /Fom.obj m.asm");
});
