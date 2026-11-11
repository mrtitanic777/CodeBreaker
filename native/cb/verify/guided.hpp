// cb/verify/guided.hpp -- inputs aimed at the branches no input has taken yet.
//
// A function verified on inputs that ran under half of its instructions is "verified (weak)": the rest was never
// compared. The fixed inputs (verify/lanes/win32 _vectors, _confirm_vectors) and the directed ones (constants and
// globals read off the code) stop at the first test they do not happen to satisfy. This generator watches each
// conditional jump of the ORIGINAL function -- which way it went, and what the comparison before it compared --
// and makes inputs for the side not taken yet:
//
//   * input-to-state: when a value the comparison saw is also in the input (an argument, the value imported calls
//     return, a word of the memory handed in), that place gets the value it was compared with, one either side
//     of it, or for a zero test the opposite of zero;
//   * otherwise the compared-with value is tried in each argument, and a few fixed changes (interesting integers,
//     what imported calls return) are tried on the inputs kept so far.
//
// An input is kept when the original then takes a branch direction, or runs an instruction, that no input before
// it did. Kept inputs are run on both sides by the comparer like any other.
//
//     guided_inputs(run, seeds, options) -> the new inputs
//
// `run(input)` traces the original and returns an Observation, or nothing when it cannot be observed. Nothing
// here knows the processor: the lanes (win32, win16, powerpc) supply `run`, and say in `Machine` how wide a word
// and which byte order theirs is.
//
// Words are `std::uint64_t` holding a value of the machine's word width (2, 4 or 8 bytes); arithmetic on them wraps
// at the width of the comparison being aimed at, as the emulated instruction would.
//
// Randomness: the "fixed changes" stage draws from a small generator (SplitMix64) seeded from `Options::seed`, so
// a run is reproducible; the draws are NOT those of any other language's generator.
#pragma once

#include <array>
#include <compare>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::verify::guided {

enum class ByteOrder : std::uint8_t { Little, Big };

// The processor's argument layout: byte order and the bytes in one argument word (2 for a 16-bit stack, 4, 8).
struct Machine {
    ByteOrder order = ByteOrder::Little;
    unsigned word = 4;
};

// A block of memory the input sets before the run.
struct MemInit {
    std::uint64_t address = 0;
    Bytes data;
    auto operator<=>(const MemInit&) const = default;
};

// What a lane adds to an input that guided search carries untouched: an integer or a list of integers (a lane's
// "ecx", "edx", "given", ...).
using Extra = std::variant<std::int64_t, std::vector<std::int64_t>>;

// One input to a function: its arguments, memory set beforehand, the value imported calls return.
struct Input {
    std::vector<std::uint64_t> args;
    std::optional<std::vector<MemInit>>
        mem_init; // absent: the lane's own memory; present (even empty): the input sets it
    std::optional<std::uint64_t> stub_ret;
    std::map<std::string, Extra> extra; // sorted by key: part of the input's identity
    auto operator<=>(const Input&) const = default;
};

// The instruction that produced a comparison's flags. Only these change how a comparison is turned round; every
// other instruction (cmp, sub, ...) is `Other`.
enum class CompareOp : std::uint8_t { Test, And, Or, Dec, Inc, Add, Xor, Other };
CompareOp parse_compare_op(std::string_view mnemonic); // "test", "and", "or", "dec", "inc", "add", "xor"; else Other

// What a comparison before a jump compared: the instruction, the operand size in bytes, the two values, and
// where each was read from (a stack slot or memory address), when known.
struct Compare {
    CompareOp op = CompareOp::Other;
    unsigned size = 4;
    std::uint64_t a = 0, b = 0;
    std::optional<std::uint64_t> a_at, b_at;
};

// One conditional jump of a traced run.
struct Branch {
    std::uint64_t address = 0;
    bool taken = false;
    std::optional<Compare> compare; // what was compared just before it, when the lane could tell
};

// One trace of the original.
struct Observation {
    std::vector<Branch> branches;
    std::unordered_set<std::uint64_t> reach; // every address that ran (membership only)
    std::optional<std::uint64_t> args_at;    // where the first argument is on the stack
    std::uint64_t stack_low = 0, stack_high = 0;
};

// An input already run, and what was seen: taken as a seed without running it again.
struct Seen {
    Input input;
    Observation observation;
};

using Run = std::function<std::optional<Observation>(const Input&)>;

struct Options {
    int budget = 96; // runs of the original spent on new inputs
    int keep = 16;   // inputs kept at most
    std::uint64_t seed = 0;
    Machine machine;
    std::span<const Seen> observed; // inputs already run, taken as seeds without running them again
};

// Inputs (beyond `seeds`) that take branch directions, or run instructions, the seeds did not.
std::vector<Input> guided_inputs(const Run& run, std::span<const Input> seeds, const Options& options = {});

// ---- the pieces, exposed for the lanes' tests and for tests of their own ------------------------------------

// The values a few fixed changes draw from.
extern const std::array<std::uint64_t, 12> kInteresting;

// A change to one operand that would send a comparison the other way: which operand (0 or 1), the value it had,
// the value to give it.
struct Flip {
    int operand = 0;
    std::uint64_t seen = 0, wanted = 0;
    friend bool operator==(const Flip&, const Flip&) = default;
};
std::vector<Flip> flips(const Compare& cmp);

// Where in an input a value can be put.
enum class PlaceKind : std::uint8_t {
    Arg,     // `index` = the argument, `offset` = the byte within its word
    Addr,    // `index` = an address of memory the input does not set yet (added to mem_init)
    StubRet, // the value imported calls return
    Mem,     // `index` = the mem_init block, `offset` = the byte within it
};
struct Place {
    PlaceKind kind = PlaceKind::Arg;
    std::uint64_t index = 0;
    std::uint64_t offset = 0;
    friend bool operator==(const Place&, const Place&) = default;
};

// Places in input `v` to put a new value for operand `k` of comparison `cmp`: first where that operand was read
// from (an argument's stack slot -- also one past those given -- or any other memory the input can set), then
// every place in the input that holds the value it had. At most six.
std::vector<Place> places(const Input& v, const Observation& obs, const Compare& cmp, int operand, std::uint64_t value,
                          unsigned size, const Machine& machine);

// The input with `value` (of `size` bytes) put at `place`.
Input put(const Input& v, const Place& place, std::uint64_t value, unsigned size, const Machine& machine);

} // namespace cb::verify::guided
