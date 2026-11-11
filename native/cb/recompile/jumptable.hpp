// cb/recompile/jumptable.hpp -- the switch tables behind a `bctr`, so a recompiled switch is a C++ switch.
//
// The Xbox 360 compiler builds a switch four ways (a table of addresses; of byte, halfword or word offsets from a
// base). Each is recognised by scanning back from the `bctr` for the bounds check, the table's address (lis/addi,
// also when the table register is the base register -- the base is found first) and the load; the table is then
// read from the image. A target outside the function becomes a tail call in the recompiler.
//
// Derived from the rexglue SDK's function scanner (src/codegen/function_scanner.cpp; BSD-3-Clause -- see NOTICE),
// with its register-reuse and RA/RB-swap handling. switches.hpp is the other, symbolic recogniser; this one is the
// recompiler's own, run over the instructions of one function.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <vector>

namespace cb::recompile::jumptable {

// The guest memory of the image under analysis. Reads are big-endian; an unmapped address answers nullopt.
class GuestMemory {
public:
    virtual ~GuestMemory() = default;
    virtual std::optional<std::uint8_t> u8(std::uint32_t addr) const = 0;
    virtual std::optional<std::uint16_t> u16(std::uint32_t addr) const = 0;
    virtual std::optional<std::uint32_t> u32(std::uint32_t addr) const = 0;
    // Does a candidate target land in executable code?
    virtual bool is_exec(std::uint32_t addr) const = 0;
};

// One instruction of the function: its address and its (big-endian decoded) 32-bit word.
struct Instruction {
    std::uint32_t va = 0;
    std::uint32_t word = 0;
};

// A recovered switch: the register holding the case index and the case targets in table order.
struct JumpTable {
    std::uint32_t index_reg = 0;
    std::vector<std::uint32_t> targets;
};

// The longest table read when the bounds check gives no length (a real switch is never this wide; the scan of
// such a table ends at the first entry that is not code, which a runaway scan over data would not reach).
inline constexpr std::size_t kMaxTableEntries = 65536;

// The jump table of the `bctr` at `bctr_addr`, if the code before it is one of the four shapes and the table has
// at least two targets.
std::optional<JumpTable> detect_one(const GuestMemory& memory, std::uint32_t bctr_addr);

// For each `bctr` (not `bctrl`) among the instructions, its jump table if there is one, keyed by the bctr's address.
std::map<std::uint32_t, JumpTable> detect_jump_tables(std::span<const Instruction> insns, const GuestMemory& memory);

}  // namespace cb::recompile::jumptable
