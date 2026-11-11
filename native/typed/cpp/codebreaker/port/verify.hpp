// codebreaker/port/verify.hpp -- typed C++ port of codebreaker.port.verify.
//
// verify.py -- the original and its port run on the same inputs; what each shows and writes compared.
//
//     python -m codebreaker.port.verify CASES.json [--report OUT.md]
//
// The port is right when it does what the original did. The original runs under CodeBreaker's own emulator (DOS:
// exact/emu/dos.py - nothing of it runs on the host); the port runs natively. Each case runs both in folders of their
// own with the same input files, the same command line, the same keyboard input; then the console output (byte for
// byte), the exit code and every file left in the folder are compared.
//
// CASES.json (paths relative to it):
//     {"original": "LIB.EXE", "port": "../modern/program.exe", "system": "dos",
//      "inputs": {"HELLO.OBJ": "path", ...},                  the files every case may use
//      "env": {"DOSRT_CORE_PAGES": "1103"},                   for the port (the platform layer's settings)
//      "cases": [{"name": "make", "cmd": "NEW +HELLO;", "stdin": "", "files": ["HELLO.OBJ"],
//                 "then": [...more cases run in the same folders after it...]}]}
//     "programs": {"mc2": {"original": "MC2.EXE", "port": "../../mc2/modern/program.exe"}}
//                                                             another program of the same set (a compiler's next
//                                                             pass): a case names it, "program": "mc2"
//     a case's "ignore": ["*.Q"]                              files not compared (what the original left
//                                                             unwritten - its memory as it was - and the next pass
//                                                             does not read)
//
// Typing notes:
//   * cases.json is the struct Config (read by load_config); a case is the struct Case. "volatile" is a list of regular
//     expressions applied to bytes: here to the bytes read as latin-1 text with re.ASCII (\d \w \s as bytes patterns have them).
//   * The exit of an original is `Exit`: None (the program did not say), a number, or the text "did not end".
//   * difflib (unified_diff, SequenceMatcher) is not in the runtime: the module has its own (namespace difflib below), the
//     Python's algorithm with its autojunk heuristic, for lists of strings.
//   * _emulator_key(): in the Python the hash of the emulator's sources (and of the C++ executive's DLL); here the emulator is
//     the program itself: the hash of the running executable's file. set_emulator_key() fixes it (a test makes two processes
//     agree on it); the value cannot be the Python's.
//   * The port is run with CreateProcess (subprocess.run with cwd, input bytes, stdout and stderr in one pipe, an environment
//     of the process' own plus the case's, a timeout of 60 seconds): a run that does not end is killed and is TimeoutExpired,
//     which carries what it printed.
//   * The cases run at once on jobs::pool::threads, as the Python's pool.threads.
//   * main() prints to the stream it is given.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::port::verify {

// ---- difflib
namespace difflib {
using Opcode = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t>;     // (tag, i1, i2, j1, j2)
// SequenceMatcher(None, a, b).get_opcodes()
py::list<Opcode> get_opcodes(const py::list<py::str>& a, const py::list<py::str>& b);
// the groups SequenceMatcher(None, a, b).get_grouped_opcodes(n)
py::list<py::list<Opcode>> get_grouped_opcodes(const py::list<py::str>& a, const py::list<py::str>& b, int64_t n = 3);
// list(difflib.unified_diff(a, b, fromfile, tofile, lineterm=lineterm, n=n))
py::list<py::str> unified_diff(const py::list<py::str>& a, const py::list<py::str>& b, const py::str& fromfile, const py::str& tofile,
                               int64_t n = 3, const py::str& lineterm = py::str("\n"));
}  // namespace difflib

// ---- cases.json
struct Program {
    py::str original;
    py::str port;
};

struct Case {
    py::str name;
    py::str cmd;                                      // default ""
    py::str stdin_;                                   // "stdin", default ""
    py::list<py::str> files;                          // the inputs this case is given
    py::dict<py::str, py::str> make;                  // small inputs written out: {name: text (latin-1)}
    py::list<Case> then;                              // run after it, in the same folders
    py::list<py::str> ignore;                         // glob patterns of files not compared
    std::optional<py::str> program;                   // another program of the set
};

struct Config {
    std::optional<py::str> original;
    std::optional<py::str> port;
    py::dict<py::str, py::str> inputs;
    py::dict<py::str, py::str> env;
    py::dict<py::str, Program> programs;
    py::list<py::str> volatile_;                      // "volatile"
    py::list<Case> cases;
};

Config load_config(const py::str& path);              // json.load(open(path))

// ---- the exit of a run
using Exit = std::variant<std::monostate, int64_t, py::str>;      // None | a number | "did not end"

// what the emulator is (see the typing notes)
py::str _emulator_key();
void set_emulator_key(const std::optional<py::str>& key);

// sha256 of the file (kept by path, mtime and size)
py::str _file_hash(const py::str& path, double mtime, int64_t size);

// the original's run, by everything it depends on: the program, the emulator, the command, the input and every file it is given
py::str _original_key(const py::str& exe, const py::str& folder, const py::str& cmd, const py::str& stdin_);

// the original run under the emulator - once: what it showed and left, by what it was given, kept (it is the
// same every time); -> (console, exit)
std::tuple<py::bytes, Exit> run_original(const Config& cfg, const py::str& exe, const py::str& folder, const py::str& cmd, const py::str& stdin_);

// subprocess.TimeoutExpired of the port's run: what it printed before it was stopped
struct TimeoutExpired : py::Exception {
    py::bytes output;
    explicit TimeoutExpired(const py::bytes& out) : py::Exception("timed out after 60 seconds"), output(out) {}
    const char* type() const override { return "TimeoutExpired"; }
};

// the port run in `folder`: -> (console with stderr, the exit code)
std::tuple<py::bytes, int64_t> run_port(const Config& cfg, const py::str& exe, const py::str& folder, const py::str& cmd, const py::str& stdin_,
                                        const py::dict<py::str, py::str>& env);

// every case of CASES.json run by exe alone (the original not run): [(case name, its console)] - what the
// sanitizer pass reads
py::list<std::tuple<py::str, py::str>> run_cases(const py::str& path, const py::str& exe,
                                                 const std::optional<py::dict<py::str, py::str>>& env = std::nullopt);

// {NAME (upper case): contents} of the files of the folder
py::dict<py::str, py::bytes> files_of(const py::str& folder);

// the lines of a console, newlines as \n
py::list<py::str> show(const py::bytes& b);

// fnmatch.fnmatch (Windows: both sides lower-cased)
bool fnmatch(const py::str& name, const py::str& pattern);

// the program's main: -> the exit code, what it prints goes to `out`
int64_t main(const py::list<py::str>& argv, std::ostream& out);

}  // namespace codebreaker::port::verify
