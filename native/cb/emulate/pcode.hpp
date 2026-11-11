// cb/emulate/pcode.hpp -- an instruction's p-code, from the decompiler's own SLEIGH specification.
//
//     LifterSet lifters(PcodeConfig::from_environment(decompiler_bin));
//     Lifter* lifter = lifters.get(Mode::X86Real16);      // null when the lifting program is not there, or has not
//                                                         // that mode
//     const Lifted* ins = lifter->lift(code_bytes, offset);   // null if it does not decode
//
// The judge's path explorer follows what an instruction does to the values made from a function's inputs. What an
// instruction does is already written down once, and checked against the processor instruction by instruction
// (docs/SLEIGH_VERIFY.md): the specification the decompiler lifts with. So the explorer asks for the same p-code --
// native/decompiler/pcodecheck.cc in its lift mode, one long-running process per Lifter, every instruction lifted
// once (kept by its bytes and offset).
//
// An op is (opcode, out, inputs): a varnode is (space, offset, size); a user operation's first input is its name.
// A 16-bit instruction is lifted at its offset in its segment: a near jump's target is an offset there too (the lift
// mode puts each in a 64K window of its own, the window dropped here). A flat 32-bit one and a PowerPC one are
// lifted at their address (the PowerPC's from ppc_64_xenon_be.sla, registers named as pcodecheck's ppc-be-64 mode
// names them, big-endian).
//
// A Lifter talks to its child through a pipe and is shared by the threads of a gate: its calls are serialised.
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/process.hpp"

namespace cb::emulate::pcode {

enum class Mode { X86Real16, X86Flat32, PpcBe64 };
std::string_view mode_name(Mode m);       // "x86-16-real", "x86-32-flat", "ppc-be-64"
std::string_view sla_file(Mode m);        // the specification the mode reads: "x86.sla", "ppc_64_xenon_be.sla"

enum class Space { Register, Unique, Constant, Ram };   // r, u, c, m

struct Varnode {
    Space space = Space::Register;
    std::uint64_t offset = 0;
    std::uint32_t size = 0;
    friend bool operator==(const Varnode&, const Varnode&) = default;
};

// One input of an op: a varnode, a user operation's name, or nothing ("-" in the line).
using Input = std::variant<std::monostate, Varnode, std::string>;

struct Op {
    std::string opcode;                   // SLEIGH's names: INT_ADD, LOAD, CBRANCH, CALLOTHER ...
    std::optional<Varnode> out;
    std::vector<Input> inputs;
};

// One instruction's p-code, and what the shadow needs to know of it without running it: the register bytes it reads
// and writes, whether it reaches memory, a user operation or a branch (anything that must be followed op by op).
struct Lifted {
    std::uint32_t length = 0;
    std::vector<Op> ops;
    std::set<std::uint64_t> reads;        // register bytes (by offset in the register space)
    std::set<std::uint64_t> writes;
    bool full = false;

    Lifted(std::uint32_t length, std::vector<Op> ops);   // works out reads / writes / full from the ops
};

struct RegisterSlot {
    std::uint64_t offset = 0;
    std::uint32_t size = 0;
};

class Lifter {
public:
    // Starts `exe` on `sla` in `mode`'s lift form. Throws ToolError when it cannot start, or does not answer with
    // its register table ("not a lifting pcodecheck").
    Lifter(const std::filesystem::path& exe, const std::filesystem::path& sla, Mode mode);
    // The same, answering null instead of throwing: for probing whether a program can lift.
    static std::unique_ptr<Lifter> try_start(const std::filesystem::path& exe, const std::filesystem::path& sla,
                                             Mode mode);

    Mode mode() const { return mode_; }
    // The lifter's registers by name, in the order the program lists them.
    const OrderedMap<std::string, RegisterSlot>& registers() const { return regs_; }

    // pcodecheck's line for the instruction at the start of `code` at `offset` (the C++ shadow parses it, and keeps
    // it for the process).
    std::string line(ByteView code, std::uint64_t offset);

    // The p-code of the instruction at the start of `code` (16 bytes are enough for x86), at `offset`; null when it
    // does not decode. The answer lives as long as the Lifter, and the same instruction gets the same answer.
    const Lifted* lift(ByteView code, std::uint64_t offset);

private:
    std::string line_locked(ByteView code, std::uint64_t offset);
    std::string read_line();

    Mode mode_;
    std::unique_ptr<process::Process> child_;
    std::string pending_;                 // bytes read from the child past the last line
    OrderedMap<std::string, RegisterSlot> regs_;
    std::mutex lock_;
    std::unordered_map<std::string, std::optional<Lifted>> memo_;   // by hex bytes + offset; empty: does not decode
};

// Where the lifting program and the specifications are.
struct PcodeConfig {
    std::filesystem::path exe;            // pcodecheck.exe
    std::filesystem::path languages;      // the folder of the .sla files (native/decompiler/languages)

    // CB_PCODECHECK names the program when it is set, else `decompiler_bin`/pcodecheck.exe.
    static PcodeConfig from_environment(const std::filesystem::path& decompiler_bin,
                                        const std::filesystem::path& languages);
    std::filesystem::path sla(Mode m) const { return languages / sla_file(m); }
};

// The lifters of a process, one per mode, started on first use. Owned by whoever runs the path explorer.
class LifterSet {
public:
    explicit LifterSet(PcodeConfig config) : config_(std::move(config)) {}
    // The lifter for `mode`; null when there is none to start (no program, no specification, or a program without
    // the lift mode). A failed start is not retried.
    Lifter* get(Mode mode);
    const PcodeConfig& config() const { return config_; }

private:
    PcodeConfig config_;
    std::mutex lock_;
    std::vector<std::pair<Mode, std::unique_ptr<Lifter>>> started_;
};

}  // namespace cb::emulate::pcode
