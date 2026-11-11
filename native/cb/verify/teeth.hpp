// cb/verify/teeth.hpp -- does the judge notice when a reconstruction is wrong?
//
//     codebreaker teeth <module> [--rounds N] [--json FILE]
//
// A verdict of "behaves as the original" is worth what the judge would have said of a wrong reconstruction. This
// asks it: functions the judge has passed are each broken in one small way that changes what they do -- a constant
// one higher, a comparison turned round, a plus made a minus, a store left out, two arguments swapped -- the module
// is built with them, and they are judged again. A broken function the judge still passes is a hole in the judge
// (or the break was in code no input reaches, which is the same thing said of the inputs).
//
// Works for every kind of module whose functions are rebuilt from C and run beside the original (32-bit and 16-bit
// Windows, PowerPC). The broken functions of one round never reach one another, so each is caught, or not, on its
// own account. Nothing is recorded in the function store.
//
//     caught      the judge now says the function differs from the original
//     survived    the judge still passes it
//
// The share caught is given for the functions it had called VERIFIED and for those it had called VERIFIED (weak)
// apart: the first is what the strong verdict is worth.
//
// The module and its judge are the lanes' business: they come in as a `Lane` (what to break, how to rebuild and
// judge a round), as the function store and the corpus come in as `Services`. `plant` is the loop every lane's
// self-check shares (C text broken and rebuilt, a 16-bit build's code, a driver's entry points).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::verify::teeth {

// ---- breaking C text ------------------------------------------------------------------------------------------

// The five small ways a function's C is broken, in the order they are tried.
enum class BreakKind { Constant, Comparison, Operator, Store, Arguments };
inline constexpr std::size_t kBreakKindCount = 5;
std::string_view word(BreakKind k); // "constant", "comparison", "operator", "store", "arguments"

// A function's text with one break made.
struct TextBreak {
    BreakKind kind;
    std::string text;
};

// Every kind of break that applies to the function's text (the declarations at the top of the body are left
// alone; breaks are made in the statements), each as the whole function's text with the break made.
std::vector<TextBreak> breaks(std::string_view text);

// "old line  ->  new line" for the one line a break changed (or "(removed)"), at most 200 bytes.
std::string changed(std::string_view before, std::string_view after);

// ---- which functions reach which ------------------------------------------------------------------------------

struct NamedText {
    std::string name;
    std::string text;
};

// For each function (in the order given): the indices of every function it can reach through the names its text
// mentions, itself included, ascending.
std::vector<std::vector<std::uint32_t>> reach(std::span<const NamedText> functions);

// ---- planting breaks ------------------------------------------------------------------------------------------

// One break in one function (or entry point): which, the kind of break (a word, or a lane's own description), and
// what the broken function is (the text, or the file).
struct Attempt {
    std::string name, what, made;
};
using Round = std::vector<Attempt>;

enum class Outcome { Caught, Missed, Other }; // the judge noticed / passed the break / said something else

template <class Said> struct Planted {
    const Attempt* attempt; // valid for the call of `each`
    Said said;
    Outcome outcome;
};

// The judge judged, whatever the lane. `next_round` gives the next round of breaks (the breaks of a round never
// reaching one another; nullopt at the end); `judge` judges a round together, giving what it said of each attempt,
// in order; `outcome` says whether it noticed. `each` is told, round by round, what came of it. The next round is
// asked for only after `each` returns.
template <class Said>
void plant(const std::function<std::optional<Round>()>& next_round,
           const std::function<std::vector<Said>(const Round&)>& judge,
           const std::function<Outcome(const Said&)>& outcome,
           const std::function<void(std::span<const Planted<Said>>)>& each) {
    while (std::optional<Round> round = next_round()) {
        std::vector<Said> said = judge(*round);
        std::vector<Planted<Said>> planted;
        planted.reserve(round->size());
        for (std::size_t i = 0; i < round->size(); ++i)
            planted.push_back({&(*round)[i], said[i], outcome(said[i])});
        each(planted);
    }
}

// ---- running it on a module -----------------------------------------------------------------------------------

// A function of the module as the lane rebuilds it.
struct Function {
    std::string name;
    std::string text;            // the C it is built from
    std::optional<Class> passed; // Verified or Weak: what the judge had called it; empty if it had not passed
};

// What the judge said of one rebuilt function.
struct Said {
    std::optional<Verdict> verdict; // nullopt: the function was not built (`reason` says why)
    std::string reason;
    std::vector<std::string> limits;
};

// A function of the module with its text replaced by a broken one.
struct Replacement {
    std::size_t function; // index into Lane::functions()
    std::string text;
};

// The module as a lane breaks it.
class Lane {
public:
    virtual ~Lane() = default;
    virtual std::string kind() const = 0;        // "ne", "pe", "powerpc"
    virtual std::string module_name() const = 0; // the report is named after it
    // Every function, in build order, with the C it is built from. The same list for the life of the lane.
    virtual const std::vector<Function>& functions() const = 0;
    // Build the module with those functions' texts replaced (the rest as they were) and judge those functions:
    // what the judge said of each, in order. Nothing is recorded in the function store.
    virtual std::vector<Said> judge(std::span<const Replacement> broken) = 0;
};

// One function's result.
enum class Fate { Caught, Survived, NotJudged, DoesNotCompile };
std::string_view word(Fate f); // "caught", "survived", "not judged", "does not compile"

struct Result {
    std::string name;
    Class was = Class::Verified; // Verified or Weak: what the judge had called it
    std::string made;            // the kind of break
    std::string reason;
    Fate now = Fate::NotJudged;
    std::string change; // 'old line  ->  new line'; only when it compiled
    std::string limits; // the verdict's limits, joined by ","; only when it compiled
};

// How many of the broken functions of one group were caught. Groups are "verified", "weak" (what the judge had
// called them) and "by break: <kind>".
struct Group {
    std::string name;
    std::size_t caught = 0, of = 0;
};

struct Report {
    std::string module, kind;
    int rounds = 0;
    std::vector<Result> results;
    std::vector<Group> caught; // sorted by name
};

using Log = std::function<void(std::string_view line)>; // an empty function: nothing logged

// Break the functions the judge passed, `rounds` rounds of them, and judge again.
Report run(Lane& lane, int rounds = 3, const Log& log = {});

// The report as the file `teeth` writes (JSON, results in the order they were judged).
std::string report_json(const Report& report);

struct Services {
    // A corpus name or a file as the file the lanes read (a .xex: its basefile); nullopt if there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> find_module;
    // Judge the module if it has not been, and prepare to break it. UsageError for a kind of module that is
    // rebuilt whole and compared byte for byte (there is no judge of behaviour to test).
    std::function<std::unique_ptr<Lane>(const std::filesystem::path&)> open;
    std::filesystem::path report_dir; // where a report goes by default: <report_dir>/<module>.json
};

// The declared command (`teeth <module> [--rounds N] [--json FILE]`).
cli::Command command(Services services);

} // namespace cb::verify::teeth
