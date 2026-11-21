// app/convert.hpp -- the conversion pipeline behind the CodeBreaker window and `codebreaker convert`: one file in, a folder of source code out.
//
//     convert::Request request{.input = "game.gb", .converted_root = "Converted"};
//     convert::Outcome done = convert::convert(request, convert::standard_planner(config), report, cancel);
//     done.state     Done | Unsupported | Failed | Cancelled;  done.output the folder;  done.message what to tell the person
//
// The run is a state machine. It always starts Idle -> Identifying; then, depending on what the file is, it passes through some of
//
//     Unpacking      only for containers and compressed images (a disc, a cabinet, an archive): its programs are taken out
//     Opening        the file is read and taken apart
//     Disassembling  its code is turned back into assembly source
//     Decompiling    only for lanes whose source is C (Windows 32-bit and 16-bit, DOS, drivers): the functions are decompiled
//     Translating    only when a language other than Original is asked for: the source goes through the translator's hub into that language
//                    (a source file of C# or Python is opened, then translated; counts are functions)
//     Checking       only after Translating where the translator can prove the result: the translation is built and run beside the original and
//                    what both print is compared; the run is Done only when they agree (translate/translate_file.hpp)
//     Writing        the source is written into the output folder
//
// and ends in exactly one of Done, Unsupported (CodeBreaker has no way to convert this file; `Outcome::what` says what it is), Failed
// (something went wrong; `message` says what) or Cancelled. Which stages a file passes through is decided by its Route (planner), not by
// the state machine: the machine only enforces the order, keeps the percentage, and cleans up.
//
// Progress is honest. Every stage has a weight (what share of the run it normally takes); inside a stage a route reports real counts
// (functions decompiled of the total, files written of the total) and the percentage follows them. A stage that has no count to give
// stays where it began until it ends; nothing is animated. A route writes into a staging folder next to the output; the Writing stage
// moves the finished files into the output folder one by one, so a run that fails or is cancelled leaves nothing behind, and a folder
// that already exists is never written into (the output is a numbered sibling: lz32, lz32_2, lz32_3 ...).
//
// This module uses no operating-system API: it runs the same on Linux. The routes themselves (convert_backends.cpp) call the writers and the
// decompiler of the other areas.
#pragma once

#include <atomic>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "translate/capability.hpp"

namespace cb::app::convert {

enum class State {
    Idle,
    Identifying,
    Unpacking,
    Opening,
    Disassembling,
    Decompiling,
    Translating,
    Checking,
    Writing,
    Done,
    Unsupported,
    Failed,
    Cancelled,
};

// "identifying", "unpacking" ... "done", "unsupported", "failed", "cancelled": stable names for logs, the command line and tests.
std::string_view state_name(State state);
// What a person is told, in plain words: "Identifying the file", "Unpacking", "Opening", "Disassembling", "Decompiling", "Translating", "Checking",
// "Writing the source".
std::string_view state_label(State state);
// Identifying .. Writing.
bool is_stage(State state);
// Done, Unsupported, Failed, Cancelled.
bool is_final(State state);

// One report of progress, handed to the listener from the thread that runs the conversion.
struct Progress {
    State state = State::Idle;
    std::string label;         // state_label(state)
    std::string detail;        // "12 of 39 functions", "gb.asm": what is being worked on; may be empty
    std::uint64_t done = 0;    // real counts inside the stage; total is 0 when the stage has none
    std::uint64_t total = 0;
    int percent = 0;           // 0..100 over the whole run; never goes down
    // New lines of what the run is producing, since the last report: the file's first bytes as hex while it is identified, then the text of
    // each source file as it lands in the staging folder, then the names of the files written. The window types them out behind the
    // progress (its character stream); nothing else reads them. Only a few are attached to each report, so the stream trickles.
    std::vector<std::string> lines;
};
using ProgressFn = std::function<void(const Progress&)>;

// Thrown by a route's reporter when the person cancelled; routes let it pass.
class Cancelled final : public std::exception {
public:
    const char* what() const noexcept override { return "cancelled"; }
};

// Thrown by a route that finds, after it has begun, that it cannot convert the file (a 64-bit program, an image it does not know): the run ends
// Unsupported with `what` and this message.
class NotConvertible final : public std::exception {
public:
    explicit NotConvertible(std::string message) : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

// Whether a function of the result behaves like the original: Matches only when a judge ran both and they agreed, Differs when they did not, NotChecked
// when nothing ran it (most routes today never judge a function; the window then says "Not checked" rather than guess).
enum class FunctionVerdict { NotChecked, Matches, Differs };

// One function of the result: what the Source card's Functions, Check and Compare views list.
struct FunctionRecord {
    std::string name;                          // "LZRead", "sub_00024"
    std::string address;                       // "0x11F0" as the original's address; empty when unknown
    FunctionVerdict verdict = FunctionVerdict::NotChecked;
    std::string source_file;                   // the file of the output folder that holds its source ("lz32.c"); empty when unknown
    int source_line = 0;                       // 1-based line of its definition there; 0 when unknown
    friend bool operator==(const FunctionRecord&, const FunctionRecord&) = default;
};

// What a route tells the machine while it works.
class Reporter {
public:
    virtual ~Reporter() = default;
    // Enter a stage (stages are entered in the order of Route::stages; one not entered is skipped). Throws Cancelled when cancelled.
    virtual void enter(State stage) = 0;
    // Real counts inside the current stage. Throws Cancelled when cancelled.
    virtual void count(std::uint64_t done, std::uint64_t total, std::string_view detail = {}) = 0;
    // A cancelled run stops at the next call of enter() or count(); a route that works a long time between them calls this.
    virtual void check() = 0;
    // A sentence for the person about how the result was proved (it reaches Outcome::checked). Only the last one is kept.
    virtual void note(std::string_view) {}
    // A line of what is being produced right now, for the window's character stream (Progress::lines). The machine already streams the
    // source files a route writes into its staging folder; a route calls this only for output that never becomes a file.
    virtual void preview(std::string_view) {}
    // The functions of the result and what the route knows of each (it reaches Outcome::functions); `can_recheck`: the route can judge them again.
    virtual void functions(std::vector<FunctionRecord>, bool /*can_recheck*/ = false) {}
};

// How one kind of file is converted: the stages it passes through (between Identifying and Writing) and the work itself. `run` reads `input`
// and writes the source into the folder `work` (never the final folder); it enters its stages through the reporter.
struct Route {
    std::string name;                         // "pe", "machine:gb", "container": for logs and tests
    std::vector<State> stages;                // some of Unpacking, Opening, Disassembling, Decompiling, in order; Writing is always last, added by the machine
    std::map<State, double> weights;          // the share of the run a stage takes; those not named have the default (see default_weight)
    std::function<void(Reporter&, const std::filesystem::path& input, const std::filesystem::path& work)> run;
};

// What identification found: a sentence for the person ("a PNG image"), and the route when CodeBreaker can convert it.
struct Plan {
    std::string what;                         // "a Windows program (PE, 32-bit)"; empty when the file is not recognised
    std::optional<Route> route;               // none: Unsupported
    std::string refusal;                      // none: the sentence to show instead of "This looks like ... CodeBreaker cannot convert it." (a target that is not supported)
    std::string folder_suffix;                // added to the output folder's name when it is made from the file's name: "_cpp17" gives Converted/Bank_cpp17
};
using Planner = std::function<Plan(const std::filesystem::path& input)>;

// The weight a stage has unless the route names one.
double default_weight(State stage);

struct Request {
    std::filesystem::path input;
    std::optional<std::filesystem::path> out_dir;      // where the source goes; default <converted_root>/<file stem>
    std::filesystem::path converted_root;              // the "converted" setting's folder; empty: next to the input, <file stem>-converted
};

struct Outcome {
    State state = State::Idle;                 // the final state
    std::string what;                          // what the file is, as identification said ("a PNG image"); empty when unknown
    std::string message;                       // Unsupported / Failed / Cancelled: the sentence to show
    std::filesystem::path output;              // Done: the folder written
    std::vector<State> stages;                 // every state passed through, in order (Identifying first, the final state last)
    std::uint64_t files = 0;                   // Done: files written
    std::string route;                         // the route's name; empty when none was chosen
    std::string checked;                       // Done after a translation: how it was proved ("... printed the same output and exited with the same code")
    std::vector<FunctionRecord> functions;     // Done: the functions of the result a route could name, with what it knows of each (empty: the route does not say)
    bool can_recheck = false;                  // the route can judge the result again ("Check again" in the window)
};

// Run the conversion. `cancel` is read between steps; set it from another thread to stop. Never throws: every failure is an Outcome.
Outcome convert(const Request& request, const Planner& planner, const ProgressFn& progress, const std::atomic<bool>& cancel);

// <root>/<stem>, or <stem>_2, <stem>_3 ... when that exists. The folder is not made.
std::filesystem::path unique_output_folder(const std::filesystem::path& root, std::string_view stem);

// "a PNG image" from "PNG image": the right article before a format's name.
std::string with_article(std::string_view what);

// The folder setting "converted" names; empty when it is not set (convert then writes next to the input file).
std::filesystem::path converted_root();

// `codebreaker convert FILE [--to LANG] [--out DIR]`: the words after `convert`; the exit code (0 done, 1 failed, 2 usage, 3 unsupported, 130 cancelled).
int run_command(std::span<const std::string> args);

// The file extensions the window's file picker lists first (".exe", ".gba" ...): the Windows and DOS ones and every registered lane's. The source
// languages' (".cs", ".py" ...) are listed with them.
std::vector<std::string> program_extensions();

// The planner of the program: every route CodeBreaker has (convert_backends.cpp). Needs the settings (cb::config::current()).
// `target` is the language asked for: "original" (or empty) is today's behaviour exactly; any other id of translate/capability.hpp ("cpp17", "csharp")
// plans a translation of a source file, and a target the translator does not support for the file is refused (Plan::refusal) before any work.
Planner standard_planner(const std::string& target = "original");

// The targets for a file, asked of the translator (translate::capability): a source file by its language, a program by the language of its decompiled
// output, anything else the plain list. Not-supported ones carry their reason.
std::vector<translate::capability::Target> targets_for(const std::filesystem::path& file);

// The language last chosen in the window ("original" when none), kept per user (a small JSON file, config/app.json; the
// environment variable CB_APP_SETTINGS names another file). remember_target writes it; failures to write are ignored.
std::string remembered_target();
void remember_target(const std::string& id);
// The per-user file above (config/app.json). The recent-conversions list is kept beside it.
std::filesystem::path settings_file();

// The hint under "Drag a file here": what the window takes, from what the translator reads (".exe, .dll, ROMs, disc images, source code (.cs .py ...)").
std::string drop_hint();

// The routes' platform list as the window's status bar shows it ("Windows · DOS · Xbox · 45 consoles · Java · .NET"); the count of consoles is
// the lane registry's.
std::string platform_summary();

}  // namespace cb::app::convert
