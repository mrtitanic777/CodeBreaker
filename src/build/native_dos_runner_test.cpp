// Tests of build/native_dos_runner.hpp: BLD.BAT's lines as COMMAND.COM reads them, and a session run over a stand-in executive
// (what it was asked to run, with which drives, and where its output went).
#include "build/native_dos_runner.hpp"

#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build;
namespace stdfs = std::filesystem;

CB_TEST("build.native_dos_runner.a_batch_line_is_read_as_command_com_reads_it", [] {
    DosCommandLine c = parse_dos_command("cl /nologo /c /W0 /AL /FoF1.OBJ F1.C > F1.TXT");
    CB_CHECK(c.kind == DosCommandLine::Kind::Program);
    CB_CHECK_EQ(c.program, "cl");
    CB_CHECK_EQ(c.tail, " /nologo /c /W0 /AL /FoF1.OBJ F1.C");
    CB_CHECK(c.out && *c.out == "F1.TXT");
    CB_CHECK(!c.append && !c.with_errors);
    c = parse_dos_command("MASM T.ASM, T.OBJ, T.LST, NUL; > T.LOG 2>&1");
    CB_CHECK_EQ(c.program, "MASM");
    CB_CHECK_EQ(c.tail, " T.ASM, T.OBJ, T.LST, NUL;");
    CB_CHECK(c.with_errors && c.out && *c.out == "T.LOG");
    c = parse_dos_command("cl /c /AS m.c >> CL.TXT");
    CB_CHECK(c.append && *c.out == "CL.TXT");
    c = parse_dos_command("c:\\ml.exe /c /Ic:\\inc /Fom.obj m.asm > a.txt");
    CB_CHECK_EQ(c.program, "c:\\ml.exe");
    c = parse_dos_command("CL/c t.c");   // (a switch ends the name)
    CB_CHECK_EQ(c.program, "CL");
    CB_CHECK_EQ(c.tail, " /c t.c");
    CB_CHECK(parse_dos_command("del done.txt").kind == DosCommandLine::Kind::Delete);
    CB_CHECK_EQ(parse_dos_command("del done.txt").name, "done.txt");
    CB_CHECK(parse_dos_command("d:").kind == DosCommandLine::Kind::Drive);
    CB_CHECK(parse_dos_command("set LIB=C:\\LIB").kind == DosCommandLine::Kind::Set);
    CB_CHECK_EQ(parse_dos_command("set LIB=C:\\LIB").name, "LIB");
    CB_CHECK(parse_dos_command("@echo off").kind == DosCommandLine::Kind::Nothing);
    CB_CHECK(parse_dos_command("rem nothing").kind == DosCommandLine::Kind::Nothing);
});

// Answers each program with what it was asked: its name and command tail on standard output, "err" on standard error.
class FakeSessions : public native_dos::DosExecutive {
public:
    mutable std::vector<native_dos::SessionRun> runs;
    bool fail = false;
    bool available() const override { return true; }
    native_dos::Exit run(const stdfs::path&, const stdfs::path&, std::string_view) const override { return {}; }
    native_dos::MemoryResult run_dpmi(const native_dos::MemoryRun&) const override { return {}; }
    native_dos::SessionExit run_session(const native_dos::SessionRun& run) const override {
        if (fail) throw native_dos::NativeDosError("no such service");
        runs.push_back(run);
        native_dos::SessionExit e;
        e.code = 0;
        const std::string said = run.exe.filename().string() + run.cmdline + "\r\n";
        e.standard_out.assign(said.begin(), said.end());
        e.console = e.standard_out;
        e.console.insert(e.console.end(), {'e', 'r', 'r'});
        return e;
    }
};

class Recorder : public DosRunner {
public:
    mutable int runs = 0;
    DosRunResult run(const DosRunSpec&) const override {
        ++runs;
        DosRunResult r;
        r.ok = true;
        return r;
    }
};

CB_TEST("build.native_dos_runner.a_session_runs_each_program_from_its_drive_and_path_with_its_output_redirected", [] {
    fs::ScopedTempDir tools("ndr_tools"), work("ndr_work");
    fs::make_dirs(tools.path() / "BIN");
    fs::write_file(tools.path() / "BIN" / "CL.EXE", Bytes{'M', 'Z'});
    FakeSessions dos;
    const NativeDosRunner runner(dos);
    DosRunSpec spec;
    spec.mounts = {{"c", tools.path()}, {"d", work.path()}};
    spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}};
    spec.files["T.C"] = std::string("int f(void) { return 1; }\n");
    spec.commands = {"cl /c T.C > T.TXT", "cl /c T.C >> T.TXT 2>&1", "nothere /x", "del GONE.TXT"};
    spec.artifacts = {"T.TXT"};
    spec.work = work.path();
    fs::write_text(work.path() / "GONE.TXT", "x");
    const DosRunResult r = runner.run(spec);
    CB_CHECK(r.ok);
    CB_CHECK_EQ(dos.runs.size(), 2u);   // (a program not found does nothing, as under DOS)
    CB_CHECK_EQ(text::upper(fs::to_text(dos.runs[0].exe)), text::upper(fs::to_text(tools.path() / "BIN" / "CL.EXE")));   // (DOS names: any case)
    CB_CHECK_EQ(dos.runs[0].cmdline, " /c T.C");
    CB_CHECK_EQ(dos.runs[0].drive, 'D');   // (the last drive listed is the current one)
    CB_CHECK(dos.runs[0].drives.at('C') == tools.path());
    CB_CHECK(as_text(dos.runs[0].env).find("INCLUDE=C:\\INCLUDE") != std::string::npos);
    CB_CHECK_EQ(text::upper(as_text(r.made.at("T.TXT"))), "CL.EXE /C T.C\r\nCL.EXE /C T.C\r\nERR");   // (`>`: standard output; `>> 2>&1`: both)
    CB_CHECK(fs::read_text(work.path() / "T.C") == "int f(void) { return 1; }\r\n");   // (text with DOS line ends)
    CB_CHECK(!stdfs::exists(work.path() / "GONE.TXT"));
    CB_CHECK(stdfs::exists(work.path() / "done.txt"));   // (the end of the batch, as DosboxRunner's)
});

CB_TEST("build.native_dos_runner.a_program_the_executive_cannot_run_sends_the_run_to_the_fallback", [] {
    fs::ScopedTempDir tools("ndr_tools"), work("ndr_work");
    fs::write_file(tools.path() / "X.EXE", Bytes{'M', 'Z'});
    FakeSessions dos;
    dos.fail = true;
    const Recorder dosbox;
    DosRunSpec spec;
    spec.mounts = {{"c", tools.path()}, {"d", work.path()}};
    spec.commands = {"c:\\x.exe"};
    spec.work = work.path();
    CB_CHECK(NativeDosRunner(dos, &dosbox).run(spec).ok);
    CB_CHECK_EQ(dosbox.runs, 1);
    CB_CHECK(!NativeDosRunner(dos).run(spec).ok);   // (no fallback: nothing pretended)
});

}  // namespace
