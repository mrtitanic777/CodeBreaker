// verify/gate16/isolated16.hpp -- each function of a rebuilt 16-bit module run beside the original's, on its own.
//
//     Isolated16 gate(spec);
//     for (const Isolated16Answer& a : gate.check("USER.EXE", "build/user", names)) { a.name, a.verdict, a.why }
//
// Both are entered the way the function is called (far or near), on the same arguments, with the original's data segment on both sides. What one
// function calls -- another of the module's functions, a Windows function, an interrupt -- is not run: it is recorded (who, with which argument words)
// and answered at once with the same made-up result on both sides (emulate/windows16, `callees`). So a function is judged on what it does itself: a
// callee rebuilt wrongly, or one that would need the whole of Windows behind it, cannot make it look wrong, or right.
//
// Compared, for each set of inputs: how the run ends; the calls it makes, in order, with their arguments; the result (AX, and DX for a function that
// returns four bytes); what is left in the data segment and anywhere else but the stack (a buffer reached through a far pointer); how many bytes of
// arguments were removed; and whether SI, DI and DS were kept. Where the two differ is the first thing a reader needs: it says which.
//
// The inputs are the 16-bit judge's (verify/lanes/win16): the fixed ones, the directed ones (the constants the code compares with) and the guided ones
// (found path by path where the lane has the p-code lifters, else guessed: arguments, words of the data segment and what an unmodelled Windows call
// returns, chosen to take branches of the original no other input took), and the scattered ones once more over a patterned data segment. CB_NO_GUIDED=1
// leaves the guided ones out. The guided inputs are looked for only once the others all agree: a function found to differ needs no more.
//
// The work is shared over threads, each with a Runner and a governor of its own (the emulators are not process-global);
// the checker's runs of the original are kept within one check.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/ordered_map.hpp"
#include "core16_types.hpp"
#include "emulate/pcode.hpp"
#include "emulate/windows16.hpp"
#include "knowledge/value.hpp"
#include "verify/lanes/win16.hpp"
#include "store/verdicts.hpp"

namespace cb::verify::gate16 {

// What a check needs of the decompiled module: how each function is called (the bytes of arguments it removes, -1 where that is not known, and whether it
// is far), which have a four-byte result, and where each is in the original.
struct IsolatedTables {
    struct Function {
        std::int64_t removes = -1;
        bool far = false;
        friend bool operator==(const Function&, const Function&) = default;
    };
    OrderedMap<std::string, Function> funcs;
    std::vector<std::string> wide;                       // sorted
    std::map<std::string, core16::SegOff> place;         // name -> (segment, offset) in the original

    // The tables as a document (the form the engine hands the gate: {"funcs": {name: [removes, far]}, "wide": [...], "place": {name: [seg, off]}}).
    knowledge::Value to_value() const;
    // FormatError for a document that is not one.
    static IsolatedTables from_value(const knowledge::Value& doc);
};

// {name: (segment, offset)} of the functions in a build, from its linker map (`m.map` in the build's folder): the first place a name is given.
std::map<std::string, core16::SegOff> built_places(const std::filesystem::path& folder);

struct Isolated16Answer {
    std::string name;
    store::Verdict verdict = store::Verdict::Inconclusive;
    std::string why;
};

struct Isolated16Spec {
    // The runner's surroundings: the models of Windows' functions and their argument sizes. The governor is the caller's, one thread to one.
    std::function<emulate::windows16::Environment(emulate::native_trace::Governor&)> environment;
    // The p-code lifters of the path explorer; none: the guided inputs are guessed.
    emulate::pcode::LifterSet* lifters = nullptr;
    // What check needs of the decompiled module of `original` (the decompiler's work: made once and kept by whoever runs it). Used when a check is not given the tables.
    std::function<IsolatedTables(const std::filesystem::path& original)> tables;
};

struct Isolated16Options {
    std::optional<std::filesystem::path> image;                   // the built module (default: the file of the folder named as the original)
    std::optional<std::vector<lanes::Vector16>> inputs;           // replaces the usual inputs (calls recorded in the running system, replayed)
    std::optional<IsolatedTables> tables;                         // default: Isolated16Spec::tables of the original
    int workers = 0;                                              // threads (0: the machine's budget)
    bool chain = false;                                           // the module's other functions run for real on both sides: what one function leaves for another is seen
};

class Isolated16 {
public:
    explicit Isolated16(Isolated16Spec spec);

    // Each named function (one the build has as C) beside the original's, in the order of `names`: equivalent, divergent, or inconclusive (not run) --
    // the judge's words. Many at once: shared out over threads.
    std::vector<Isolated16Answer> check(const std::filesystem::path& original, const std::filesystem::path& folder, const std::vector<std::string>& names,
                                        const Isolated16Options& options = {}) const;

    const Isolated16Spec& spec() const { return spec_; }

private:
    Isolated16Spec spec_;
};

// ---- the parts of a comparison, for the tests ----------------------------------------------------------------------------------------------------

// What one run did, in a form two builds can be compared on.
struct GatePrint {
    std::string exit;                                              // "ret", "truncated", "fault", ...
    std::vector<std::string> calls;                                // the callees and the Windows functions it called, in order
    std::map<std::int64_t, std::uint8_t> data;                     // what it left in the data segment, by offset (bytes that changed)
    std::map<std::int64_t, std::uint8_t> far;                      // what it wrote anywhere else but the stack and the descriptor table, by address
    // The result, as two builds agree on it: a number; "DS" (its own data selector, a leftover); none (AX as it came in); a byte (AL set).
    enum class ResultKind { Absent, Number, DataSelector, NoResult, Byte };
    ResultKind result_kind = ResultKind::Absent;
    std::int64_t result = 0;
    std::optional<std::int64_t> pop;                               // bytes the stack moved
    std::optional<std::array<bool, 3>> kept;                       // SI, DI, DS
};

// The first way two prints differ, in words, or nullopt.
std::optional<std::string> gate_difference(const GatePrint& original, const GatePrint& ours);

// What the run `r` of a function did; `wide`: four-byte result; `data_length`: bytes of the data segment; `places`: the module's own functions by the
// (segment, offset) a far pointer to one names, as callbacks are told.
GatePrint gate_print(const emulate::Run& r, bool wide, std::size_t data_length, const std::map<std::pair<std::uint32_t, std::uint32_t>, std::string>& places);

// An input as a verdict names it: its first argument words, and for a guided one what else it sets.
std::string gate_input_text(const lanes::Vector16& v);

}  // namespace cb::verify::gate16
