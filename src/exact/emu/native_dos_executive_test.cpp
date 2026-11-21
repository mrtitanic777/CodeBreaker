// Tests of exact/emu/native_dos_executive.hpp on the real executive: a session over two drives, and the period compiler
// making the same object file as under DOSBox-X (skipped where Microsoft C 6 or DOSBox-X is not set up).
#include "exact/emu/native_dos_executive.hpp"

#include "build/dos_tools.hpp"
#include "build/native_dos_runner.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

using namespace cb;
namespace stdfs = std::filesystem;

CB_TEST("exact.emu.native_dos_executive.a_session_runs_a_program_from_its_drive_and_writes_its_output", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const exact::emu::NativeDosExecutive dos(config::current());
    if (!dos.available()) return;
    fs::ScopedTempDir tools("nde_tools"), work("nde_work");
    fs::write_file(tools.path() / "HELLO.EXE", fs::read_file(testing::data_path("port/testdata/verify/hello.exe")));
    const build::NativeDosRunner runner(dos);
    build::DosRunSpec spec;
    spec.mounts = {{"c", tools.path()}, {"d", work.path()}};
    spec.variables = {{"PATH", "C:\\"}};
    spec.commands = {"hello > OUT.TXT"};
    spec.artifacts = {"OUT.TXT"};
    spec.work = work.path();
    const build::DosRunResult r = runner.run(spec);
    CB_CHECK(r.ok);
    CB_CHECK_EQ(as_text(r.made.at("OUT.TXT")), "Hello from DOS\r\n");
});

// The same compile in both: CL, its passes started through EXEC, the object file byte for byte.
CB_TEST("exact.emu.native_dos_executive.microsoft_c_6_makes_the_same_object_as_under_dosbox", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const config::Config& config = config::current();
    const auto msc6 = config.path("msc6");
    std::error_code ec;
    if (!msc6 || !stdfs::is_regular_file(*msc6 / "BIN" / "CL.EXE", ec)) return;
    const build::dos_tools::Emulator emulator = build::dos_tools::Emulator::from_config(config);
    if (!emulator.program()) return;
    const exact::emu::NativeDosExecutive dos(config);
    if (!dos.available()) return;
    const auto compile = [&](const build::DosRunner& runner, const stdfs::path& work) {
        build::DosRunSpec spec;
        spec.mounts = {{"c", *msc6}, {"d", work}};
        spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}, {"LIB", "C:\\LIB"}};
        spec.files["T.C"] = std::string("int twice(int a) { return a + a; }\nint far *pick(int far *p, int i) { return p + i; }\n");
        spec.commands = {"cl /nologo /c /W0 /AL /Gs /FoT.OBJ T.C > T.TXT"};
        spec.artifacts = {"T.OBJ", "T.TXT"};
        spec.cpu = {{"core", "dynamic"}, {"cputype", "386"}, {"cycles", "max"}};
        spec.work = work;
        return runner.run(spec);
    };
    fs::ScopedTempDir a("nde_native"), b("nde_dosbox");
    const build::DosRunResult native = compile(build::NativeDosRunner(dos), a.path());
    const build::DosRunResult dosbox = compile(build::dos_tools::DosboxRunner(emulator), b.path());
    CB_CHECK(native.ok);
    CB_CHECK(dosbox.ok);
    if (!native.ok || !dosbox.ok) return;
    CB_CHECK(native.made.at("T.OBJ") == dosbox.made.at("T.OBJ"));
    CB_CHECK_EQ(as_text(native.made.at("T.TXT")), as_text(dosbox.made.at("T.TXT")));
});

// Visual C++ 1.52's CL, a DOS-extended program: its extender starts each pass under a PSP of its own (55h, 50h) and ends it with 4Ch,
// which returns to that PSP's terminate address. No fallback here: the executive alone makes the object DOSBox-X makes.
CB_TEST("exact.emu.native_dos_executive.visual_cpp_1_52_makes_the_same_object_as_under_dosbox", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const config::Config& config = config::current();
    const auto msvc = config.path("msvc152");
    std::error_code ec;
    if (!msvc || !stdfs::is_regular_file(*msvc / "BIN" / "CL.EXE", ec)) return;
    const build::dos_tools::Emulator emulator = build::dos_tools::Emulator::from_config(config);
    if (!emulator.program()) return;
    const exact::emu::NativeDosExecutive dos(config);
    if (!dos.available()) return;
    const auto compile = [&](const build::DosRunner& runner, const stdfs::path& work) {
        build::DosRunSpec spec;
        spec.mounts = {{"c", *msvc}, {"d", work}};
        spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}, {"LIB", "C:\\LIB"}};
        spec.files["T.C"] = std::string("int twice(int a) { return a + a; }\n");
        spec.commands = {"CL /AM /c /FcT.COD /FoT.OBJ T.C > T.LOG 2>&1"};
        spec.artifacts = {"T.OBJ", "T.COD"};
        spec.work = work;
        return runner.run(spec);
    };
    fs::ScopedTempDir a("nde_vc_native"), b("nde_vc_dosbox");
    const build::dos_tools::DosboxRunner dosbox_runner(emulator);
    const build::DosRunResult dosbox = compile(dosbox_runner, b.path());
    if (!dosbox.ok) return;   // (DOSBox-X does not run it here either: nothing to hold the runner to)
    const build::DosRunResult native = compile(build::NativeDosRunner(dos), a.path());
    CB_CHECK(native.ok);
    if (!native.ok) return;
    CB_CHECK(native.made.at("T.OBJ") == dosbox.made.at("T.OBJ"));
    CB_CHECK_EQ(as_text(native.made.at("T.COD")), as_text(dosbox.made.at("T.COD")));
});

}  // namespace
