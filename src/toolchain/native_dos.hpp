// toolchain/native_dos.hpp -- run the period real-mode DOS tools on CodeBreaker's OWN emulator, in-process.
//
// CodeBreaker has a real-mode DOS executive on Unicorn (exact/emu/dos: a PSP, the MCB memory chain, program load and
// INT 20h/21h over a filesystem that maps a working directory, and a DPMI host), all in-process. This module is what the
// toolchain drivers (build/masm, source/vxd/masm) ask for an assembly: the same MASM.EXE and the same DDK ML.EXE, run on
// that executive, headless and parallel. It is the only way DOS tools run: a run the executive cannot complete fails with
// a NativeDosError that says why; nothing is pretended.
//
//     const DosExecutive& dos = ...;                         // exact/emu/dos, behind this interface
//     Product p = native_dos::assemble(dos, source, "t", masm_root);       // .obj, .lst, .log
//     auto made = native_dos::ml(dos, toolchain, "/c /Fom.obj m.asm", {{"m.asm", text}}, {"m.obj"});
//
// THE DPMI TOOL: the DDK's ML.EXE is a Phar Lap TNT bound program: its real-mode stub loads DOSXNT.EXE, which enters
// protected mode through a DPMI host and raw-switches to real mode for every DOS call. It runs entirely in memory
// (`DosExecutive::run_dpmi`): nothing touches the disk.
//
// Nothing here is global: the executive and the tool folders are passed in. Everything a run makes is read
// into the result and its work folder is deleted when the call returns.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "toolchain/dos_run.hpp"
#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::toolchain::native_dos {

// The executive could not complete a run (a service it lacks, a CPU fault, a limit). A clean run that merely reports errors
// of the tool is NOT this.
class NativeDosError : public EmulationError {
public:
    using EmulationError::EmulationError;
    CB_ERROR_BOILERPLATE(NativeDosError)
};

// One source to assemble: `stem` names the DOS files (STEM.ASM -> STEM.OBJ, STEM.LST, STEM.LOG): 1-8 letters, digits,
// underscore, dollar sign.
struct Source {
    std::string stem;
    std::string text;
};

// The files an assembly produced; each is present only when the file exists.
struct Product {
    std::optional<Bytes> obj;
    std::optional<std::string> lst; // Latin-1 text, as UTF-8
    std::optional<std::string> log;
};

// What a batch answers: a product per source, in the order the sources were given.
struct Finished {
    std::vector<std::pair<std::string, Product>> results;
    std::filesystem::path work; // the folder the run used, when it is kept
};

// What a real-mode run answers.
struct Exit {
    std::optional<std::int32_t> code; // the program's exit code; nullopt when it did not end with one
    Bytes console;                    // everything it wrote to the console
};

// A run of a DOS-extended program, all in memory.
struct MemoryRun {
    std::map<std::string, Bytes>
        files;       // virtual file name (upper case) -> contents: the tool's own files and the inputs
    std::string exe; // the program to start (upper case): one of `files`
    std::string cmdline;
    Bytes env;                          // the environment block: NAME=value strings, each NUL-ended
    std::vector<std::string> artifacts; // names of files to hand back, if the run made them
};
struct MemoryResult {
    std::map<std::string, Bytes> made; // the artifacts the run made (by the name given in `artifacts`)
    Bytes console;
};

// One program of a DOS session (native_dos_runner: BLD.BAT's lines, each run on the executive): its drives, each its own
// host folder, the current one, the command tail and the environment.
struct SessionRun {
    std::filesystem::path exe;                       // the program, on the host
    std::map<char, std::filesystem::path> drives;    // 'C' -> folder ...
    char drive = 'D';                                // the current drive
    std::string cmdline;                             // the command tail, as typed (a leading blank is the caller's)
    Bytes env;                                       // NAME=value strings, each NUL-ended
    bool dpmi = false;                               // a DPMI host for a DOS-extended program
};
// What it did: its exit code, everything it wrote to the console, and what of that was standard output (`>`).
struct SessionExit {
    std::optional<std::int32_t> code;
    Bytes console;
    Bytes standard_out;
};

// The DOS executive on the emulator (exact/emu/dos, with the DPMI host). Implementations throw a cb::Error of any kind
// when a run cannot complete; this module turns that into NativeDosError.
class DosExecutive {
public:
    virtual ~DosExecutive() = default;
    // One program of a session over the session's drives (the files it writes are written to those folders). The default
    // says the executive cannot: NativeDosError.
    virtual SessionExit run_session(const SessionRun& run) const;
    // Whether an emulator engine can be opened at all (the program has handed over unicorn.dll).
    virtual bool available() const = 0;
    // Run the real-mode program `exe` with `work` as the DOS working directory (every file it names is there).
    virtual Exit run(const std::filesystem::path& exe, const std::filesystem::path& work,
                     std::string_view cmdline) const = 0;
    // Run a DOS-extended program with the DPMI host over an in-memory file system.
    virtual MemoryResult run_dpmi(const MemoryRun& run) const = 0;
};

// One source with MASM.EXE from `masm_root` (BIN/MASM.EXE). `stem` names the DOS files (STEM.ASM -> STEM.OBJ, .LST); it
// may carry sub-folders (`asm/lead00`) but never `..`. The log is MASM's own, or the console when it made none.
// NativeDosError when the executive cannot complete or MASM.EXE is missing; UsageError for a bad stem.
Product assemble(const DosExecutive& dos, std::string_view source, std::string_view stem,
                               const std::filesystem::path& masm_root);

// One MASM run per source, in a shared work folder (MASM.EXE copied in once), results in the order given.
// NativeDosError the moment any run cannot complete.
Finished assemble_many(const DosExecutive& dos, std::span<const Source> items,
                                     const std::filesystem::path& masm_root);

// The environment of an extended run: PATH=C:\ .
const Bytes& default_extended_env();

// Run a DOS-extended tool entirely in memory: `tool_files` ({name -> path}, read once) and the inputs `files` (text is
// stored with CRLF line ends as a text-mode write would) are put in a virtual folder and `exe` is run in it. Returns
// the artifacts the run made and the console. NativeDosError when the run cannot complete.
MemoryResult run_extended(const DosExecutive& dos, const std::map<std::string, std::filesystem::path>& tool_files,
                          std::string_view exe, std::string_view cmdline, const std::map<std::string, DosFile>& files,
                          std::span<const std::string> artifacts, const Bytes& env = default_extended_env());

// Where the DDK's ML.EXE and the extender are.
struct MlToolchain {
    std::filesystem::path ml_dir;    // ML.EXE and ML.ERR (the DDK's MASM611C folder)
    std::filesystem::path msvc_root; // Microsoft C/C++ 1.52c: BIN/DOSXNT.EXE
};
// {ML.EXE, ML.ERR, DOSXNT.EXE -> path}. NativeDosError naming the files that are missing.
std::map<std::string, std::filesystem::path> ml_files(const MlToolchain& toolchain);

// What `ml` made and printed.
struct MlResult {
    std::map<std::string, Bytes> made;
    std::string console; // Latin-1 as UTF-8
};
// TNT ML.EXE natively: ml(dos, tc, "/c /Fom.obj m.asm", {{"m.asm", text}}, {"m.obj"}).
MlResult ml(const DosExecutive& dos, const MlToolchain& toolchain, std::string_view args,
            const std::map<std::string, DosFile>& files, std::span<const std::string> artifacts);

} // namespace cb::toolchain::native_dos
