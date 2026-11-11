// cb/emulate/run.hpp -- one record of a run, whichever emulator made it.
//
// Every platform's emulator returns a `Run`: the same fields under the same names, whether the run was the 32-bit
// tracer's, the 16-bit one's, a driver's, the PowerPC interpreter's or one of the console machines'. A judge
// (cb/verify/lanes) turns it into what the comparer compares and gives only what is its own: how it names a call,
// which places it leaves out, what of the result is a result.
//
//     exit        how it ended, in the emulator's own words ("ret", "exception: ...", "return")
//     ended()     the same as the comparer says it: "ret", "fault", "truncated", or the word before a colon
//     results     the result registers as the run left them ("EAX", "AX" and "DX:AX", "r3" and "f1", a machine's
//                 A X Y ...), in the order the emulator named them
//     writes      the memory it wrote, in order; `harness` marks what the emulator wrote for the run, not the code
//                 (a stand-in's return through zeros)
//     capped      more writes were made than were recorded: the writes are not all there
//     calls       the calls out of the code, in order
//     io          accesses to devices, in order (a machine's bus log too)
//     stack       how far the stack pointer moved from entry to the end (absent: not known)
//     preserved   the registers a function must hand back, on entry and at the end (absent when not read)
//     path        the addresses of the instructions that ran (in order where the emulator keeps the order)
//     steps       instructions it took
//     truncated   cut off at the step limit (or another of the emulator's caps)
//     endian      how a written value's bytes lie in memory
//     raw         the emulator's own record, for what only one lane asks of it: opaque here
//
// A Run is built by its emulator's `record(...)` from the record the emulator always made.
#pragma once

#include <any>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::emulate {

// A result register: an integer, a float (PowerPC f1), or absent (a driver's missing EAX).
using Register = std::variant<std::monostate, std::uint64_t, double>;

struct MemoryWrite {
    std::uint64_t address = 0;
    std::uint32_t size = 0;
    std::uint64_t value = 0;
    std::optional<std::uint64_t> old;     // what the bytes held just before, when known
    bool harness = false;
};

// A call into a Windows API the 16-bit tracer answered with a stand-in: what the NE import table says of it, and
// what the call did (the stack words it was given, the instruction count then).
struct ImportHit {
    std::string api;                              // "MODULE.FUNC", "far ssss:oooo" or "callback name"
    std::string kind;                             // the fixup kind (IMPORTORD, IMPORTNAME, OSFIXUP), when from the table
    std::optional<std::string> module;
    std::optional<std::int64_t> ordinal;
    std::optional<std::int64_t> name_off;
    std::optional<std::int64_t> slot;
    std::vector<std::int64_t> args_stack;         // the words above the far return address
    std::int64_t n = 0;                           // the instruction count when it was entered
    std::optional<std::int64_t> ret_ip;           // where it returns to ...
    std::optional<std::int64_t> ret_cs;           // ... in which code segment
    std::optional<std::int64_t> arg_bytes;        // bytes of arguments it pops
    std::optional<std::int64_t> interrupt;        // the interrupt that was answered as this call (21h: DOS3Call)
    bool foreign = false;                         // a far call into the recorded system's own code
    bool unmodelled = false;                      // the stand-in knew nothing of it
    // up to 32 bytes at the code a stack word pointed into (to the first zero), by the word's place
    std::vector<std::pair<std::size_t, Bytes>> pointed;
};

// A virtual device driver's call: INT 20h service `number` of `device`, or -- with no device -- a call out through
// a pointer, `number` being the address it reached.
struct ServiceCall {
    std::optional<std::uint32_t> device;
    std::uint32_t number = 0;
};

// A call's own note of itself: none, a text, a driver's service, or the 16-bit tracer's import hit.
using CallDetail = std::variant<std::monostate, std::string, ServiceCall, ImportHit>;

struct Call {
    std::optional<std::string> name;
    std::optional<std::vector<std::int64_t>> args;   // absent when their number is not known
    CallDetail detail;
};

// An access to a device: a port, or a machine's bus. `kind` is the emulator's word for it ("in", "out", ...).
struct IoAccess {
    std::optional<std::uint64_t> step;
    std::string kind;
    std::uint64_t port = 0;
    std::uint64_t value = 0;
    std::optional<std::uint32_t> size;
};

struct SavedRegister {                             // a register a function must hand back
    std::optional<std::uint64_t> on_entry;
    std::optional<std::uint64_t> at_end;
    bool kept() const { return on_entry == at_end; }
};
using Preserved = OrderedMap<std::string, SavedRegister>;

enum class Endian { Little, Big };

// How a run ended, as the comparer says it: "none" (no word), "ret", "fault" (an exception), else the word before
// any colon ("truncated", "wild", "int:21" -> "int").
std::string ended(const std::optional<std::string>& exit);

struct Run {
    std::optional<std::string> exit;
    OrderedMap<std::string, Register> results;
    std::vector<MemoryWrite> writes;
    bool capped = false;
    std::vector<Call> calls;
    std::vector<IoAccess> io;
    std::optional<std::int64_t> stack;
    std::optional<Preserved> preserved;
    std::vector<std::uint64_t> path;
    std::optional<std::uint64_t> steps;
    bool truncated = false;
    Endian endian = Endian::Little;
    std::any raw;

    std::string ended() const { return emulate::ended(exit); }

    // Whether the registers a function must hand back came back as they went in (those of `names`, else every one
    // recorded); false when they were not read. A name that was not recorded counts as unread on both sides: kept.
    bool kept() const;
    bool kept(std::span<const std::string> names) const;

    // One line for a log: the exit, the steps, the calls and writes, and whether the run was cut off.
    std::string summary() const;
};

}  // namespace cb::emulate
