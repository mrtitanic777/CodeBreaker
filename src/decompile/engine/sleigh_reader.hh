// decompile/engine/sleigh_reader.hh -- the decompiler core's SLEIGH translator as CodeBreaker's one instruction reader.
//
// A processor is a SLEIGH description (languages/<cpu>.sla, compiled from languages/src/<cpu>/*.slaspec by sleigh.exe); the same
// description the decompiler lifts to p-code reads the instructions here. Nothing of the core shows through this header.
//
//     sleigh::Reader z80(languages / "z80.sla", {});
//     for (const sleigh::Instruction& i : z80.read(bytes.data(), bytes.size(), 0x0000, 16)) ...    // "LD A,0x12"
//     sleigh::Lifted l = xenon.pcode(bytes.data(), bytes.size(), base, address);                    // its p-code, as data
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace cb::decompile::sleigh {

// How control leaves an instruction, as its p-code says: the last branch, call or return it holds. A branch inside the instruction
// (to a p-code label: a conditional RET's test, a REP's loop) is not a way out of it, so a conditional return falls through (Next).
enum class Flow {
    Next,      // falls through
    Branch,    // may go to `target` or fall through
    Jump,      // goes to `target` only
    Call,      // calls `target` (or through a register: CallIndirect), then falls through
    CallIndirect,
    Return,
    Indirect,  // goes through a register or memory cell
};

struct Instruction {
    std::uint64_t address = 0;
    std::size_t length = 0;     // 0: the bytes there are no instruction of the processor's
    std::string mnemonic;
    std::string operands;
    Flow flow = Flow::Next;
    std::optional<std::uint64_t> target;   // where a branch, jump or call goes, when it is a constant address
};

// ---- p-code: what an instruction does, as the description says it (the recompiler writes each operation as C++) ----------------

// The operations, numbered as the core numbers them (sleigh_reader.cc checks that they agree).
enum class Op : unsigned {
    Copy = 1, Load, Store, Branch, CBranch, BranchInd, Call, CallInd, CallOther, Return,
    IntEqual, IntNotEqual, IntSLess, IntSLessEqual, IntLess, IntLessEqual, IntZext, IntSext, IntAdd, IntSub, IntCarry, IntSCarry,
    IntSBorrow, Int2Comp, IntNegate, IntXor, IntAnd, IntOr, IntLeft, IntRight, IntSRight, IntMult, IntDiv, IntSDiv, IntRem, IntSRem,
    BoolNegate, BoolXor, BoolAnd, BoolOr,
    FloatEqual, FloatNotEqual, FloatLess, FloatLessEqual, FloatNan = 46, FloatAdd, FloatDiv, FloatMult, FloatSub, FloatNeg, FloatAbs,
    FloatSqrt, FloatInt2Float, FloatFloat2Float, FloatTrunc, FloatCeil, FloatFloor, FloatRound,
    Piece = 62, Subpiece, Insert = 70, ZPull, PopCount, LzCount, SPull,
};
// The operation's name as the core prints it ("INT_ADD", "CALLOTHER" ...).
const char* op_name(Op op);

// Where a varnode is. `Constant`: the offset is the value (a branch's target inside the instruction is a Constant too: the number of
// operations to move by, signed). `Memory`: a processor space (ram); `space` says which, by its index among the description's spaces.
enum class Space : std::uint8_t { Constant, Register, Unique, Memory };

struct Varnode {
    Space kind = Space::Constant;
    std::uint64_t offset = 0;   // in bytes (a register's place in the register space, a temporary's number, an address)
    unsigned size = 0;          // bytes
    unsigned space = 0;         // Memory: the space's index (space_name() names it)
    bool operator==(const Varnode& o) const { return kind == o.kind && offset == o.offset && size == o.size && space == o.space; }
    bool operator!=(const Varnode& o) const { return !(*this == o); }
};

// One operation. LOAD and STORE take the space as their first input (a Constant whose offset is the space's index), then the address
// (and STORE the value). CALLOTHER's first input is the user operation's number (user_op_name() names it). Branches and calls name
// their target as a Memory varnode (an address of the code space) or, inside the instruction, as a Constant.
struct PcodeOp {
    Op op = Op::Copy;
    std::optional<Varnode> output;
    std::vector<Varnode> inputs;
};

// One instruction's p-code: its length in bytes (0: no instruction of the processor's) and its operations in order.
struct Lifted {
    std::uint64_t address = 0;
    std::size_t length = 0;
    std::vector<PcodeOp> ops;
};

class Reader {
public:
    // `sla`: a compiled description; `context`: the description's context variables to start from (x86's "addrsize", ARM's
    // "TMode" ...). Error (std::runtime_error) for a file that is not a description.
    Reader(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context);
    ~Reader();
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;

    // `count` instructions (0: all) of the `size` bytes at `code`, the first at `base`. A place that does not decode is one
    // Instruction of length 0; the caller decides how far to step over it.
    std::vector<Instruction> read(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::size_t count) const;
    // The same with context variables set over the whole of the bytes read (the 65816's register widths `m16` and `x16`): what an
    // earlier read left in the translator's context at these addresses (a REP's or SEP's change for the instructions after it) is
    // replaced, so the instruction is read in the context given, whatever was read before.
    std::vector<Instruction> read(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::size_t count,
                                  const std::vector<std::pair<std::string, int>>& context) const;
    // The one instruction at `address` of the `size` bytes at `code` (the first at `base`), as read() reads it. Reading again in
    // the same bytes keeps the translator's parses (a decoder following code calls this for every instruction of one image).
    Instruction at(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::uint64_t address) const;
    // The smallest step the processor's instructions take (its alignment), in bytes.
    std::size_t alignment() const;

    // The p-code of the one instruction at `address` of the `size` bytes at `code` (the first at `base`), as the decompiler and the
    // emulator get it from the description. A place that does not decode (or runs past the bytes) is a Lifted of length 0, no ops.
    Lifted pcode(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::uint64_t address) const;
    // The description's name of the register that holds `size` bytes at `offset` of the register space ("r3", "cr0", "vs32"), or empty
    // when none does. A part of a bigger register is named by it (the low word of r3, 4 bytes at r3's offset + 4, is "r3"): compare
    // with register_named() to tell the part.
    std::string register_name(std::uint64_t offset, unsigned size) const;
    // A register's place by its name: nullopt when the description has no such register.
    std::optional<Varnode> register_named(const std::string& name) const;
    // A user operation's name by its number (CALLOTHER's first input); empty for a number the description has none for.
    std::string user_op_name(std::uint64_t index) const;
    // A space's name by its index (a Memory varnode's `space`, LOAD's and STORE's first input).
    std::string space_name(unsigned index) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::decompile::sleigh
