// cb/build/native_dos.hpp -- run the period real-mode DOS tools on CodeBreaker's OWN emulator, in-process, instead of
// launching DOSBox-X.
//
// DOSBox-X boots a whole emulated PC in an external process to run one 16-bit DOS program: it is slow, serial,
// heavyweight and an external dependency. CodeBreaker has a real-mode DOS executive on Unicorn (exact/emu/dos: a PSP,
// the MCB memory chain, program load and INT 20h/21h over a filesystem that maps a working directory, and a DPMI
// host), all in-process. This module is what the toolchain drivers (build/masm, source/vxd/masm) ask for an
// assembly: the same MASM.EXE and the same DDK ML.EXE, run on that executive, about 20x faster, headless and parallel.
//
//     const DosExecutive& dos = ...;                         // exact/emu/dos, behind this interface
//     if (native_dos::enabled(dos, "masm")) {
//         Product p = native_dos::assemble(dos, source, "t", masm_root);       // .obj, .lst, .log
//     }
//     auto made = native_dos::ml(dos, toolchain, "/c /Fom.obj m.asm", {{"m.asm", text}}, {"m.obj"});
//
// THE SWITCH -- environment variable CB_NATIVE_DOS (`mode_from_environment`):
//     unset / "auto"   the native runner for the tools PROVEN byte-identical to DOSBox (MASM and the DDK's TNT ML);
//                      the caller falls back to DOSBox when a native run cannot complete (NativeDosError)
//     "1" / "on" ...   force the native runner (no DOSBox)
//     "0" / "off" ...  force DOSBox
// MASM is proven byte-identical on the objects and listings it makes; ML on real Win95 VxD sources, OMF and COFF (a
// COFF header's time stamp is the run's clock -- ours a fixed date -- the one field that differs, as it does between
// two DOSBox runs).
//
// THE DPMI TOOL: the DDK's ML.EXE is a Phar Lap TNT bound program: its real-mode stub loads DOSXNT.EXE, which enters
// protected mode through a DPMI host and raw-switches to real mode for every DOS call. It runs entirely in memory
// (`DosExecutive::run_dpmi`): nothing touches the disk.
//
// Nothing here is global: the executive, the tool folders and the mode are passed in. Everything a run makes is read
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

#include "cb/build/dos_run.hpp"
#include "cb/build/dos_sessions.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::build::native_dos {

// The native runner could not complete a run (a service the executive lacks, a CPU fault, a limit): the caller falls
// back to DOSBox. A clean run that merely reports errors of the tool is NOT this.
class NativeDosError : public EmulationError {
public:
    using EmulationError::EmulationError;
    CB_ERROR_BOILERPLATE(NativeDosError)
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

// The DOS executive on the emulator (exact/emu/dos, with the DPMI host). Implementations throw a cb::Error of any kind
// when a run cannot complete; this module turns that into NativeDosError.
class DosExecutive {
public:
    virtual ~DosExecutive() = default;
    // Whether an emulator engine can be opened at all (the program has handed over unicorn.dll).
    virtual bool available() const = 0;
    // Run the real-mode program `exe` with `work` as the DOS working directory (every file it names is there).
    virtual Exit run(const std::filesystem::path& exe, const std::filesystem::path& work,
                     std::string_view cmdline) const = 0;
    // Run a DOS-extended program with the DPMI host over an in-memory file system.
    virtual MemoryResult run_dpmi(const MemoryRun& run) const = 0;
};

// CB_NATIVE_DOS.
enum class Mode { Auto, On, Off };
Mode mode_from_environment();

// Tools proven byte-identical to DOSBox.
bool is_proven(std::string_view tool);

// Whether the native runner should be used for `tool` ("masm", "ml"): never when the mode is Off or no engine can be
// opened; always when it is On; else only for the proven tools.
bool enabled(const DosExecutive& dos, std::string_view tool = "masm", Mode mode = mode_from_environment());

// One source with MASM.EXE from `masm_root` (BIN/MASM.EXE). `stem` names the DOS files (STEM.ASM -> STEM.OBJ, .LST); it
// may carry sub-folders (`asm/lead00`) but never `..`. The log is MASM's own, or the console when it made none.
// NativeDosError when the executive cannot complete or MASM.EXE is missing; UsageError for a bad stem.
dos_sessions::Product assemble(const DosExecutive& dos, std::string_view source, std::string_view stem,
                               const std::filesystem::path& masm_root);

// One MASM run per source, in a shared work folder (MASM.EXE copied in once), results in the order given.
// NativeDosError the moment any run cannot complete, so the caller falls the WHOLE batch back to DOSBox.
dos_sessions::Finished assemble_many(const DosExecutive& dos, std::span<const dos_sessions::Source> items,
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

} // namespace cb::build::native_dos
