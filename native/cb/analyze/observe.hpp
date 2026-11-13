// cb/analyze/observe.hpp -- what running a function shows that reading it does not.
//
//     Image image = image::load_pe(formats::pe::Image::open(path));
//     const auto graph = call_graph::build(image);
//     const auto facts = conventions::functions(image, graph);
//     const auto structs = structures::recover(image, graph, conventions::summary(facts));
//     observe::Setting setting{.module = path, .argc = my_import_argc};
//     const observe::Seen seen = observe::run(image, facts, structs, setting);      // {function: observation}
//     const observe::Report report = observe::analyse(image, facts, structs, seen);
//
// Each function is run in the emulator with every argument pointing at a buffer of its own, filled so that any value
// read from it says which buffer and which offset it came from; and once more with only the arguments the static
// analysis took for pointers. From those runs:
//
//     fields      {argument: {offset: cell}}                 what it touches through each pointer (a cell: the widest
//                                                            access, how often it was read, how often written)
//     nested      {(argument, offset): {offset: cell}}       ... and through the pointers it finds there
//     indirect    [(call site, function reached)]            where `call [reg+8]` and the like really went (addresses of
//                                                            the module's own functions the static call graph cannot have)
//     passes      [(import, parameter, argument, offset, address | value)]
//                                                            a field, or the address of one, handed to a system function
//                                                            -- which says what the field is
//     removed     bytes of arguments the function removed on return (none when it did not return)
//     returns     the result is argument k, or the field at an offset of argument k
//     idiom       strlen, memcpy ... when the function behaves as that one on test inputs
//
// The idiom check runs the small functions that loop and call nothing on a battery of strings and buffers and compares
// what they return and leave behind with what strlen, wcslen, strcpy, strcat, strcmp, stricmp, strchr, strrchr, strncpy,
// memcpy, memset, memcmp, memicmp and strncmp would. A function that passes every case of one of them behaves as it on
// those cases; it is evidence for a name, not a proof.
//
// 32-bit modules only: the 16-bit emulator is entered with far pointers and a data segment, and nothing here drives it
// yet. The runs go through the job pool (one function lost to a native fault is one function lost, not the analysis), at
// most `jobs` at a time, each worker on a Tracer of its own.
//
// Unicorn keeps state per process and cb::emulate serialises every call into it, so threads never use more than about one
// core. Given a worker launcher (Setting::launcher) the functions are shared out over `jobs` worker PROCESSES of the host
// program instead (cb/jobs/workers.hpp): each loads the module, runs its share and hands the observations back as a line
// of tokens. The host registers the kind with `register_worker_kinds` at the top of its main.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/analyze/conventions.hpp"
#include "cb/analyze/facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/core/bytes.hpp"
#include "cb/emulate/emulator.hpp"
#include "cb/jobs/workers.hpp"

namespace cb::analyze::observe {

inline constexpr std::int64_t kBuffers = 0x40000000, kBufferSize = 0x1000;   // argument k points at kBuffers + k * 0x10000
inline constexpr std::int64_t kMark = 0x50000000;     // the dword at offset o of buffer k is kMark + (k << 24) + (o << 12)
inline constexpr std::int64_t kTextA = 0x41000000, kTextB = 0x41010000;      // the idiom battery's two buffers
inline constexpr int kMaxArgs = 8;
inline constexpr std::int64_t kInstructions = 2500;   // one run's cap

// What a function does to one place it was handed: the widest access and how many reads and writes there were.
struct Cell {
    std::int64_t size = 0, reads = 0, writes = 0;
    friend bool operator==(const Cell&, const Cell&) = default;
};
using Cells = std::map<Va, Cell>;   // by offset

// How a run ended: reached the end of the function / was cut off by the instruction cap / faulted (or could not be run).
enum class Exit { Returned, Cut, Fault };
std::string_view name_of(Exit exit);   // "ret" "cut" "fault"

// A field or an argument handed to a system function: the system function (as "MODULE!name"), which of its parameters got
// it, and what: the address of a place in argument k's buffer, or the value found there.
struct Pass {
    std::string import;
    int parameter = 0;
    int argument = 0;
    Va offset = 0;
    enum class How { Address, Value } how = How::Address;
    friend bool operator==(const Pass&, const Pass&) = default;
};

// A call site and where the call really went.
struct Indirect {
    Va site = 0, target = 0;
    friend bool operator==(const Indirect&, const Indirect&) = default;
};

// The result of a function is one of its arguments (the pointer itself) or the dword at an offset of one.
struct Returns {
    enum class Kind { Argument, Field } kind = Kind::Argument;
    int k = 0;
    std::optional<Va> offset;   // Field
    friend bool operator==(const Returns&, const Returns&) = default;
};

enum class Idiom { Strlen, Wcslen, Strcpy, Strcat, Strcmp, Stricmp, Strchr, Strrchr, Strncpy, Memcpy, Memset, Memcmp, Memicmp, Strncmp };
std::string_view name_of(Idiom idiom);   // "strlen" ... "strncmp"

struct Observation {
    std::map<int, Cells> fields;                      // argument -> its cells
    std::map<std::pair<int, Va>, Cells> nested;       // (argument, offset of the pointer in its buffer) -> cells
    std::vector<Indirect> indirect;                   // in the order met, each once
    std::vector<Pass> passes;                         // in the order met, each once
    std::optional<Exit> exit;                         // none: nothing was run
    std::optional<std::int64_t> removed;
    std::optional<Returns> returns;
    std::optional<Idiom> idiom;
};

// ---- the buffers ---------------------------------------------------------------------------------------------------------

// The contents of argument k's buffer: the dword at offset o says which buffer and offset it came from.
Bytes fill(int k);

// What an address in one of our buffers is.
struct Where {
    enum class Kind { Argument, Field } kind = Kind::Argument;   // a place in a buffer / a value read from one
    int k = 0;                  // the buffer
    Va o = 0;                   // Argument: the offset in the buffer; Field: the offset the value was read from
    Va offset = 0;              // Field: the offset in that dword (a pointer to the field's middle)
};
std::optional<Where> where(std::int64_t address);

// The run taken into the observation. A run that could not be made at all (`nullptr`) counts as a fault.
void digest(const emulate::emulator::Trace* run, Observation& into);

// The contents of a buffer after a run: the bytes it started with and the run's writes on it.
Bytes after(const emulate::emulator::Trace& run, std::int64_t base, ByteView initial);

// ---- running -------------------------------------------------------------------------------------------------------------

struct Setting {
    // The file the Image was made from: the emulator loads that module.
    std::filesystem::path module;
    // The emulator's environment but for `resolve` (the headers' prototypes: what an import gives back through a pointer).
    emulate::emulator::Environment environment;
    // How many arguments an imported function pops; empty: not known.
    emulate::emulator::ImportArgc argc;
    std::optional<std::size_t> limit = 400;   // the functions run (the most informative first); none: all of them
    int jobs = 4;                             // at most this many at a time
    // Worker processes of the host program to run the functions in (see above); none: threads of this process.
    std::optional<jobs::WorkerLauncher> launcher;
    // The fewest functions per worker process worth starting one for (a process costs the module's load); fewer than two such shares: threads.
    std::size_t per_process = 6;
};

// One function to run: its address, how many arguments it takes (none: not known), which of them the static analysis took
// for pointers, and whether it is worth the idiom battery.
struct Candidate {
    Va function = 0;
    std::optional<std::int64_t> args;
    std::vector<int> pointers;   // ascending
    bool battery = false;
};

// The functions worth running, the informative ones first (the battery's, then those with pointer arguments, then by
// address), at most `limit`. A function that is a thunk, empty, not code, or reads a register but ecx before writing it
// is left out.
std::vector<Candidate> candidates(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found,
                                  std::optional<std::size_t> limit);

// Runs the module's functions. Thread-safe: each thread that asks takes a Tracer of its own from the Observer's idle ones.
class Observer {
public:
    // Loads the module once (NotFoundError, FormatError) so that a bad setting is one error, not a lost function each.
    explicit Observer(Setting setting);

    // The runs of one function. A function whose runs fault is an observation that says so.
    Observation observe(const Candidate& candidate) const;

    // The library function `function` behaves as on the battery, or none. `args`: how many arguments it takes, if known
    // (only the idioms of that many arguments are tried).
    std::optional<Idiom> idiom(Va function, std::optional<std::int64_t> args) const;

private:
    // One run; none when the emulator could not run it at all (a function it cannot lay out, a fault of its own).
    std::optional<emulate::emulator::Trace> trace(Va function, const std::vector<std::uint32_t>& args,
                                                  const std::vector<emulate::emulator::MemoryInit>& memory) const;

    Setting setting_;
    mutable std::mutex mutex_;
    mutable std::vector<std::unique_ptr<emulate::emulator::Tracer>> idle_;
};

// What a worker process needs of the Setting that cannot cross the process boundary -- the environment's functions and the imports' argument counts:
// the host says how it makes them (the same as it gives the parent's Setting).
using PrepareSetting = std::function<void(Setting&)>;

// Registers the worker kind that runs a share of the functions (kind "observe") with the host's way of preparing a Setting.
void register_worker_kinds(jobs::WorkerKinds& kinds, PrepareSetting prepare);

// One Observation as tokens, and back (what travels between a worker and its parent).
void write_observation(jobs::TokenWriter& out, const Observation& observation);
Observation read_observation(jobs::TokenReader& in);

// {function: observation} for the candidates; none where the pool lost the function. Empty for an image that is not a flat
// 32-bit module.
using Seen = std::map<Va, std::optional<Observation>>;
Seen run(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found, const Setting& setting);

// ---- the analysis ---------------------------------------------------------------------------------------------------------

struct Counts {
    std::int64_t functions_run = 0, returned = 0, faulted = 0, cut_off = 0, with_pointer_arguments = 0;
    std::int64_t fields_observed = 0, nested_pointers_followed = 0;
    std::int64_t static_fields_confirmed = 0, fields_only_seen_running = 0, structures_with_a_field_confirmed = 0;
    std::int64_t indirect_calls_resolved = 0, function_starts_found = 0, fields_passed_to_system_calls = 0;
    std::int64_t removed_bytes_agree_with_static = 0, removed_bytes_differ = 0, library_idioms = 0;
};

// What one function's runs showed (the fact kind "observed").
struct ObservedFact {
    Va addr = 0;
    Exit exit = Exit::Fault;
    std::map<int, Cells> fields;
    std::map<std::pair<int, Va>, Cells> nested;
    std::vector<Pass> passes;
    std::vector<std::string> indirect_calls;   // "<function> at <site> -> <function reached>": the sites in the module's own code
    std::optional<std::int64_t> removed;
    std::optional<Returns> returns;
    std::optional<Idiom> behaves_like;
};

// What the runs say about a recovered structure (the fact kind "structure_observed").
struct StructureObservedFact {
    std::string name;
    std::vector<Va> fields_confirmed;       // ascending: the fields the static recovery had and the runs touched
    std::vector<Va> fields_added;           // ascending: touched by the runs and not in the recovered structure
    std::int64_t fields_not_reached = 0;    // the recovered fields no run touched
};

struct Report {
    Counts counts;
    std::vector<ObservedFact> observed;                 // by function address
    std::vector<StructureObservedFact> structures;      // by structure name
    std::vector<std::string> text;
};

// What the runs found, set against what the static analyses said (`facts`: the function's removal, `found`: the structures).
Report analyse(Image& image, const conventions::Facts& facts, const std::vector<structures::Structure>& found, const Seen& seen);

// The sentence a pass is told in: "<import> parameter <n> gets the address of | the value at argument <k> +<offset>".
std::string describe(const Pass& pass);

// The document of one observed function (`addr` and `kind` included), and the rows the fact table keeps of a report: an
// `observed` row per function and a `structure_observed` row per structure (no address).
knowledge::Value to_value(const ObservedFact& fact);
std::vector<facts::Row> fact_rows(const Report& report, ImageKind kind);

}  // namespace cb::analyze::observe
