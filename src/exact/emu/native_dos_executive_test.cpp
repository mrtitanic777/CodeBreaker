// Tests of exact/emu/native_dos_executive.hpp on the real executive: a session over two drives, and the period compilers
// making the object files DOSBox-X made of the same sources (their SHA-1 sums were recorded from DOSBox-X on 2026-11-21, when the
// executive was proven against it) -- skipped where the compiler is not set up.
#include "exact/emu/native_dos_executive.hpp"

#include "build/native_dos_runner.hpp"
#include "core/crypto.hpp"
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

std::string sha1_hex(const Bytes& data) { return crypto::to_hex(crypto::sha1(data)); }

// Microsoft C 6's CL, its passes started through EXEC: the object file and the console text are what DOSBox-X made.
CB_TEST("exact.emu.native_dos_executive.microsoft_c_6_makes_the_object_dosbox_made", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const config::Config& config = config::current();
    const auto msc6 = config.path("msc6");
    std::error_code ec;
    if (!msc6 || !stdfs::is_regular_file(*msc6 / "BIN" / "CL.EXE", ec)) return;
    const exact::emu::NativeDosExecutive dos(config);
    if (!dos.available()) return;
    fs::ScopedTempDir work("nde_native");
    build::DosRunSpec spec;
    spec.mounts = {{"c", *msc6}, {"d", work.path()}};
    spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}, {"LIB", "C:\\LIB"}};
    spec.files["T.C"] = std::string("int twice(int a) { return a + a; }\nint far *pick(int far *p, int i) { return p + i; }\n");
    spec.commands = {"cl /nologo /c /W0 /AL /Gs /FoT.OBJ T.C > T.TXT"};
    spec.artifacts = {"T.OBJ", "T.TXT"};
    spec.work = work.path();
    const build::DosRunResult native = build::NativeDosRunner(dos).run(spec);
    CB_CHECK(native.ok);
    if (!native.ok) return;
    CB_CHECK_EQ(native.made.at("T.OBJ").size(), std::size_t{254});
    CB_CHECK_EQ(sha1_hex(native.made.at("T.OBJ")), "b22de419495786fa7f44af76f0c8910392117496");
    CB_CHECK_EQ(native.made.at("T.TXT").size(), std::size_t{5});
    CB_CHECK_EQ(sha1_hex(native.made.at("T.TXT")), "892b68be141859cdcff0e03a2978b20272a2be23");
});

// Visual C++ 1.52's CL, a DOS-extended program: its extender starts each pass under a PSP of its own (55h, 50h) and ends it with 4Ch,
// which returns to that PSP's terminate address. The executive makes the object and the listing DOSBox-X made.
CB_TEST("exact.emu.native_dos_executive.visual_cpp_1_52_makes_the_object_dosbox_made", [] {
    CB_WINDOWS_ONLY("the DOS executive is built on Windows only");
    const config::Config& config = config::current();
    const auto msvc = config.path("msvc152");
    std::error_code ec;
    if (!msvc || !stdfs::is_regular_file(*msvc / "BIN" / "CL.EXE", ec)) return;
    const exact::emu::NativeDosExecutive dos(config);
    if (!dos.available()) return;
    fs::ScopedTempDir work("nde_vc_native");
    build::DosRunSpec spec;
    spec.mounts = {{"c", *msvc}, {"d", work.path()}};
    spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}, {"LIB", "C:\\LIB"}};
    spec.files["T.C"] = std::string("int twice(int a) { return a + a; }\n");
    spec.commands = {"CL /AM /c /FcT.COD /FoT.OBJ T.C > T.LOG 2>&1"};
    spec.artifacts = {"T.OBJ", "T.COD"};
    spec.work = work.path();
    const build::DosRunResult native = build::NativeDosRunner(dos).run(spec);
    CB_CHECK(native.ok);
    if (!native.ok) return;
    CB_CHECK_EQ(native.made.at("T.OBJ").size(), std::size_t{357});
    CB_CHECK_EQ(sha1_hex(native.made.at("T.OBJ")), "2b1c0ce412fc46300d47e05e94b0489fe3d3147d");
    CB_CHECK_EQ(native.made.at("T.COD").size(), std::size_t{608});
    CB_CHECK_EQ(sha1_hex(native.made.at("T.COD")), "c2292d61a9d72e636c3fad6960d91f95c745a3f1");
});

}  // namespace
