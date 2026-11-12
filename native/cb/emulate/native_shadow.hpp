// cb/emulate/native_shadow.hpp -- the path explorer's symbolic shadow of an x86 run: the conditions the run's path went by, as
// expressions over its inputs.
//
//     pcode::Lifter& lifter = *lifters.get(pcode::Mode::X86Flat32);
//     Shadow shadow(Setup::for_lifter(Kind::Flat32, lifter, inputs));     // inputs: the memory bytes that are inputs until written
//     shadow.enter({{.where = std::uint64_t{0x7FFF0004}, .value = 5, .input = 0}}, 32);   // argument 0, 32 bits, is at that address
//     for each instruction:  shadow.step(uc, address);  ... run it ...
//     const Run& run = *shadow.run();                                     // path, trail, vars, lost ...
//
// The shadow is the recorder's own (emulate/tracer shadow: stepped before each instruction, told of the calls the environment
// answered, of memory it wrote and of registers it set); this is its typed face. A recorder (native_trace, the 16-bit and driver
// recorders) is handed `handle()` and steps the shadow itself.
//
// What the run made is read back from the recorder's tables on first use after it (`run()`), as symbolic expressions of an
// `ExprPool` the Run owns: hold the `shared_ptr<const Run>` for as long as its conditions are used. A shadow that is stepped, told of
// a call or entered again makes the next `run()` read the tables again.
//
// The p-code of an instruction the shadow has not seen is asked of the lifter, through a function the shadow is given (`Setup::line`):
// the recorder's callback carries no context, so the lifters asked live in one table of this module, by mode.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core16_types.hpp"
#include "cb/emulate/symbolic.hpp"

namespace cb::emulate::pcode {
class Lifter;
}

namespace cb::emulate::native_shadow {

// What kind of run: a 16-bit one (a selector's base is read from the GDT, or is selector * 16 in real mode), a flat 32-bit one, or a
// driver's flat ring-0 one (a service call also sets the carry flag).
enum class Kind : std::int32_t { Real16 = 16, Flat32 = 32, Driver32 = 33 };

// ---- what a run made --------------------------------------------------------------------------------------------------------

// One condition the run's path went by: a conditional jump (taken or not), one inside an instruction's p-code (a string
// instruction's count), or an address or selector held at its value.
enum class PathKind : std::uint8_t { Jump, Inner, Pin };
struct PathEntry {
    std::uint64_t site = 0;
    bool taken = false;
    const symbolic::Expr* condition = nullptr;
    PathKind kind = PathKind::Jump;
};

// Every conditional jump, in order.
struct Step {
    std::uint64_t site = 0;
    bool taken = false;
};

// An input of the run: an argument (`where` is its number), a register (its name), memory (its address), what an unmodelled call
// returned (no place), or a carry flag.
enum class VarKind : std::uint8_t { Arg, Reg, Mem, Ret, Carry };
struct Var {
    VarKind kind = VarKind::Arg;
    std::variant<std::monostate, std::uint64_t, std::string> where;
    std::uint32_t bits = 0;
    std::uint64_t value = 0;
};

struct Run {
    std::vector<PathEntry> path;
    std::vector<Step> trail;
    OrderedMap<std::string, Var> vars;       // in the order the inputs were made
    std::set<std::string> lost;              // why the conditions may not be all there is (empty: they are)
    std::int64_t pins = 0;
    std::int64_t steps = 0;
    bool off = false;
    symbolic::ExprPool pool;                 // owns the conditions' nodes
};

// The recorder's tables as it hands them over: the node table (8 words a node), the path (4), the trail (2), the vars (7), the lost
// (a string index each) and the strings, NUL ended.
struct RawRun {
    std::span<const std::uint64_t> nodes, path, trail, vars, lost;
    std::string_view strings;
    std::int64_t string_count = 0;
    std::int64_t pins = 0, steps = 0, off = 0;
};
// The run the tables describe. FormatError for tables that lie (an operation, kind or string that is not there, a constant wider than
// 64 bits, a node made of one that comes after it).
std::shared_ptr<const Run> decode_run(const RawRun& raw);

// ---- making a shadow -----------------------------------------------------------------------------------------------------------

struct RegisterSpec {
    std::string name;
    std::uint64_t offset = 0;                // in the lifter's register space
    std::uint32_t size = 0;
};

// pcodecheck's line for the instruction at the start of `code`, at `offset` (pcode::Lifter::line); throws what the lifter throws.
using LineFn = std::function<std::string(ByteView code, std::uint64_t offset)>;

struct Setup {
    Kind kind = Kind::Flat32;
    std::string mode;                        // the lift mode ("x86-32-flat"): the lifters asked are kept by it
    std::vector<RegisterSpec> registers;     // the lifter's registers, in its order
    LineFn line;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> inputs;   // (low, high): memory bytes that are inputs of the run until written
    std::optional<std::pair<std::uint64_t, std::uint64_t>> span;   // (low, high) of the function's own code: its jumps are the path's
    bool real_mode = false;                  // 16-bit: selector * 16 is a selector's base
    std::uint64_t gdt_addr = 0x1000;         // 16-bit: where the GDT is

    // The lifter's mode, registers and lines.
    static Setup for_lifter(Kind kind, pcode::Lifter& lifter, std::vector<std::pair<std::uint64_t, std::uint64_t>> inputs = {},
                            std::optional<std::pair<std::uint64_t, std::uint64_t>> span = std::nullopt, bool real_mode = false);
};

// One input to enter(): its place (a memory address, or a register's name), its value, and its name -- an argument's number, or a
// register input's own name; none: the place's position in the list.
struct Place {
    std::variant<std::uint64_t, std::string> where;
    std::uint64_t value = 0;
    std::optional<std::variant<std::int64_t, std::string>> input;
};

// What the environment answered for a call it modelled.
struct CallResult {
    bool symbolic = false;                   // the result is a new input
    std::uint64_t value = 0;
    std::optional<std::uint64_t> args_at;    // where the call's arguments were
    std::int64_t args = 0;
    std::string why;                         // why the conditions may not be all there is, when something was left out
    std::int32_t word = 2;                   // the width of a word of the run, in bytes
    std::uint64_t carry = 0;
};

class Shadow {
public:
    // UsageError for a setup the recorder cannot take (a lift mode kept for too many modes at once).
    explicit Shadow(Setup setup);
    Shadow(const Shadow&) = delete;
    Shadow& operator=(const Shadow&) = delete;
    ~Shadow();

    const Setup& setup() const { return setup_; }
    Kind kind() const { return setup_.kind; }
    // The recorder's handle of this shadow: what a recorder is given to step it itself. Marks the shadow's read-back stale (the
    // recorder steps it from here on).
    void* handle();

    // ---- what the runner calls
    // The inputs of the run: each place and its value, `bits` wide.
    void enter(std::span<const Place> places, std::int32_t bits = 16);
    void step(core16::Uc& machine, std::uint64_t address, std::uint64_t cs_base = 0);
    void after_call(core16::Uc& machine, const CallResult& result);
    void written(std::uint64_t at, std::uint64_t size);
    void forget(std::span<const std::string> names);
    std::uint64_t segbase(core16::Uc& machine, std::uint64_t selector);

    // ---- what the run made (read back on first use after the run)
    std::shared_ptr<const Run> run();

private:
    Setup setup_;
    void* shadow_ = nullptr;                 // the recorder's
    std::shared_ptr<const Run> read_;        // what run() read; cleared by whatever changes the shadow
};

}  // namespace cb::emulate::native_shadow
