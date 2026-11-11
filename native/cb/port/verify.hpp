// cb/port/verify.hpp -- the original and its port run on the same inputs; what each shows and writes compared.
//
//     codebreaker verify CASES.json [--report OUT.md] [--port EXE]
//
// The port is right when it does what the original did. The original, a DOS program, runs under CodeBreaker's own
// emulator (an `OriginalRunner`: nothing of it runs on the host); the port runs natively. Each case runs both in
// folders of their own with the same input files, the same command line, the same keyboard input; then the console
// output (byte for byte), the exit code and every file left in the folder are compared.
//
// CASES.json (paths relative to it):
//     {"original": "LIB.EXE", "port": "../modern/program.exe",
//      "inputs": {"HELLO.OBJ": "path", ...},                  the files every case may use
//      "env": {"DOSRT_CORE_PAGES": "1103"},                   for the port (the platform layer's settings)
//      "volatile": ["Free memory: \\d+"],                      what depends on the machine: masked, alike, on both
//                                                              sides (the console, and the files ending .LST .MAP .TXT)
//      "cases": [{"name": "make", "cmd": "NEW +HELLO;", "stdin": "", "files": ["HELLO.OBJ"],
//                 "then": [...more cases run in the same folders after it...]}]}
//     "programs": {"mc2": {"original": "MC2.EXE", "port": "../../mc2/modern/program.exe"}}
//                                                             another program of the same set (a compiler's next
//                                                             pass): a case names it, "program": "mc2"
//     a case's "make": {"IN.TXT": "text"}                    small inputs written out (the text is the bytes, one
//                                                             character a byte: latin-1)
//     a case's "ignore": ["*.Q"]                              files not compared (what the original left
//                                                             unwritten - its memory as it was - and the next pass
//                                                             does not read)
//
// The original's run is the slow part and is the same every time, so what it showed and left is kept in the cache,
// by everything it depends on: the program, the emulator, the command, the input and every file it was given.
//
// A port is run with a timeout of 60 seconds; one that does not end is stopped and recorded as "did not end". The
// cases run at once, each case with the cases that follow it (`then`) in one pair of folders.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/base/parallel.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::port::verify {

// ---- CASES.json -----------------------------------------------------------------------------------------------

struct Program {
    std::string original;
    std::string port;
};

struct Case {
    std::string name;
    std::string cmd;                        // the command tail, split at blanks for the port
    std::string stdin_text;                 // what is typed: one character a byte (latin-1)
    std::vector<std::string> files;         // the inputs (named in Config::inputs) this case is given
    std::map<std::string, std::string> make;   // small inputs written out: name -> contents (latin-1)
    std::vector<Case> then;                 // run after it, in the same folders
    std::vector<std::string> ignore;        // glob patterns of files not compared
    std::optional<std::string> program;     // another program of the set (a key of Config::programs)
};

struct Config {
    std::optional<std::string> original;
    std::optional<std::string> port;
    std::map<std::string, std::string> inputs;     // name -> path
    std::map<std::string, std::string> env;        // set for the port
    std::map<std::string, Program> programs;
    std::vector<std::string> volatile_patterns;    // regular expressions over bytes
    std::vector<Case> cases;
};

// Throws ParseError for text that is not a CASES.json, FormatError where it is one that cannot be run: a case that
// names an input or a program the file does not define. IoError for a file that cannot be read.
Config parse_config(std::string_view json, std::string_view file_name = "cases.json");
Config load_config(const std::filesystem::path& path);

// ---- the runs ---------------------------------------------------------------------------------------------------

// How a run ended: with the code it gave, without saying (a DOS program that ended and gave none), or not at all
// (it was stopped).
struct Exit {
    enum class Kind : std::uint8_t { Unsaid, Code, DidNotEnd };
    Kind kind = Kind::Unsaid;
    std::uint32_t code = 0;

    static Exit unsaid() { return {}; }
    static Exit of(std::uint32_t code) { return {Kind::Code, code}; }
    static Exit did_not_end() { return {Kind::DidNotEnd, 0}; }
    // What a report shows: the number, "(none)" or "did not end".
    std::string text() const;
    // The same ending: no code and code 0 are the same (the program was not asked to say); a run that did not end
    // is the same as another that did not.
    friend bool operator==(const Exit& a, const Exit& b) {
        if (a.kind == Kind::DidNotEnd || b.kind == Kind::DidNotEnd) return a.kind == b.kind;
        return (a.kind == Kind::Code ? a.code : 0) == (b.kind == Kind::Code ? b.code : 0);
    }
};

// What a program showed on the console (its stdout and stderr, in order) and how it ended.
struct Run {
    Bytes console;
    Exit exit;
};

// Runs the original: a DOS program under the emulator. `folder` holds the case's input files and the program may
// leave files in it; its working folder. Called from several threads at once.
class OriginalRunner {
public:
    virtual ~OriginalRunner() = default;
    // Text that changes whenever what the emulator would do changes (its build): part of the key a run is kept under.
    virtual std::string identity() const = 0;
    // `cmd` is the command tail as typed (no leading blank). A program that does not end is a Run that says so, with
    // what it showed; an emulator that cannot go on is an EmulationError.
    virtual Run run(const std::filesystem::path& exe, const std::filesystem::path& folder, std::string_view cmd,
                    ByteView stdin_data) const = 0;
};

// The emulator of native/dosexec (cbdos.dll, with the Unicorn it loads at run time).
class CbdosRunner final : public OriginalRunner {
public:
    struct Settings {
        std::filesystem::path dll;       // cbdos.dll
        std::filesystem::path unicorn;   // the unicorn.dll it runs on
        std::uint64_t max_blocks = 60'000'000;         // a run is stopped after this many basic blocks (when it counts them)
        std::chrono::seconds time_limit{20};           // and after this long
    };
    // Throws NotFoundError when a file is missing, ToolError when the emulator does not start.
    static std::unique_ptr<CbdosRunner> load(const Settings& settings);
    ~CbdosRunner() override;
    std::string identity() const override;
    Run run(const std::filesystem::path& exe, const std::filesystem::path& folder, std::string_view cmd,
            ByteView stdin_data) const override;

private:
    struct Impl;
    explicit CbdosRunner(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

// The port, a native program, run in `folder` with `cmd` split at blanks, `stdin_data` typed, and `env` set on top of
// the environment this process has. `timeout`: a port that has not ended by then is killed; its Run says so and holds
// what it showed up to then.
Run run_port(const std::filesystem::path& exe, const std::filesystem::path& folder, std::string_view cmd,
             ByteView stdin_data, const std::map<std::string, std::string>& env,
             std::chrono::milliseconds timeout = std::chrono::seconds(60));

// What a case may use of the machine: how the original is run, where its results are kept, how much memory to leave.
struct Services {
    const OriginalRunner* original = nullptr;                 // required by compare()
    base::cache::Cache* cache = nullptr;                      // none: every original run is made
    const base::budget::Limits* limits = nullptr;             // none: no waiting for memory, workers as the machine says
    std::chrono::milliseconds port_timeout = std::chrono::seconds(60);   // a port not ended by then is killed
};

// The kind of cache entry the original's runs are kept in (a State kind).
inline constexpr std::string_view kCacheKind = "port-originals";
inline constexpr std::string_view kCacheVersion = "originals-1";   // change when what is kept changes
void declare_cache(base::cache::Cache& cache);

// The original run in `folder` - once: what it showed and left, by what it was given, kept. The folder is left as the
// run left it (as it is when the run is not made, from what was kept).
Run run_original(const OriginalRunner& runner, base::cache::Cache* cache, const base::budget::Limits* limits,
                 const std::filesystem::path& exe, const std::filesystem::path& folder, std::string_view cmd,
                 ByteView stdin_data);

// ---- comparing ----------------------------------------------------------------------------------------------------

// A row of the report.
struct CaseRow {
    std::string name;
    std::string cmd;
    ::cb::verify::Verdict verdict = ::cb::verify::Verdict::Equivalent;
    std::string what;
};

struct Report {
    std::string original_name;       // the original's file name
    std::string port_name;           // the port's path, relative to the cases file when it can be
    std::vector<CaseRow> rows;       // in the order of the cases, each case's `then` after it
    std::vector<std::string> details;   // the console differences, one markdown block per case

    std::size_t same() const;
    bool all_same() const { return same() == rows.size(); }
    // The report as markdown: a line of totals, a table of cases, and the differences.
    std::string markdown() const;
};

// Runs every case (the cases at once) with the original and with the port and compares them. `base` is the folder
// the paths of the cases file are relative to. Throws what the runs throw (an emulator that cannot go on).
Report compare(const Config& config, const std::filesystem::path& base, const std::filesystem::path& original_exe,
               const std::filesystem::path& port_exe, const Services& services);

struct CaseConsole {
    std::string name;
    std::string console;     // as text: one character a byte (latin-1 made UTF-8)
};

// Every case run by the port alone (the original not run), in the order of the cases: what each showed. A case
// that names another program whose port is not built is left out. A port that does not end gives what it showed
// (the last 200000 bytes) and "(did not end)". `more_env` is set on top of the cases file's own.
std::vector<CaseConsole> run_cases(const Config& config, const std::filesystem::path& base,
                                   const std::filesystem::path& port_exe, const std::map<std::string, std::string>& more_env,
                                   const Services& services);

// The files of a folder, by name in upper case (a DOS name has no case): name -> contents.
std::map<std::string, Bytes> files_of(const std::filesystem::path& folder);

// The lines of a console: bytes read one a character, line ends of either kind ended.
std::vector<std::string> console_lines(ByteView console);

// A unified diff of two lists of lines, as the lines of the text: "--- from", "+++ to", then each hunk with `context`
// lines around its changes. Equal lists give none. A difference too large to align gives the whole middle as one
// change.
std::vector<std::string> unified_diff(const std::vector<std::string>& a, const std::vector<std::string>& b,
                                      std::string_view from_name, std::string_view to_name, std::size_t context = 3);

// Windows file-name pattern matching: letters of either case, `*` and `?`, `[abc]`, `[a-c]`, `[!abc]`, and '/' or '\'.
bool glob_match(std::string_view name, std::string_view pattern);

// What the command did: the exit code, and what it prints to stdout and to stderr.
struct Outcome {
    int exit_code = 0;
    std::string out;
    std::string err;
};

// `verify CASES.json [--report OUT.md] [--port EXE]` without printing: exit 0 when every case is the same, 1 when not, 3
// when the port is not built (`err` says so). `port` replaces the port the cases file names. The report is written to
// `report_path` when given (and `out` says how many cases were the same), else it is `out`.
Outcome execute(const std::filesystem::path& cases_path, const std::optional<std::filesystem::path>& report_path,
                const std::optional<std::filesystem::path>& port, const Services& services);

// The command: `verify CASES.json [--report OUT.md] [--port EXE]`. Exit 0 when every case is the same, 1 when not, 3
// when the port is not built. `services.original` must be set.
cli::Command command(const Services& services);

}  // namespace cb::port::verify
