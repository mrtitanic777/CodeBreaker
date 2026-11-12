// cb/recompile/builders/operands.hpp -- what the instruction builders are given to read a PowerPC operand with.
//
// Every builder (floating, arithmetic, logical, memory, system, vector) turns one decoded instruction into the C++
// statements the recompiled function holds. The recompiler (recompile/powerpc) knows how an operand token is spelled
// and how a register is written in the generated code, so it supplies that knowledge, once, as an `Operands`; a
// builder asks for the number behind a token ("f3" -> 3) and writes the statement around it.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace cb::recompile::builders {

// A memory operand "-0x70(r1)": the displacement and the base register.
struct MemoryOperand {
    std::int64_t displacement = 0;
    int base = 0;
};

// How the recompiler reads operand tokens and spells registers and addresses in the generated code. The token readers
// give nullopt when the token is not that kind of operand.
class Operands {
public:
    virtual ~Operands() = default;

    virtual std::optional<int> reg(std::string_view token) const = 0;     // "r5"  -> 5
    virtual std::optional<int> vreg(std::string_view token) const = 0;    // "v5"  -> 5
    virtual std::optional<int> freg(std::string_view token) const = 0;    // "f5"  -> 5
    virtual std::optional<int> cr(std::string_view token) const = 0;      // "cr6" -> 6
    virtual std::optional<std::int64_t> imm(std::string_view token) const = 0;
    virtual std::optional<MemoryOperand> mem(std::string_view token) const = 0;
    // The register's value in the generated code: "ctx.r<n>.<field>".
    virtual std::string register_value(int n, std::string_view field) const = 0;
    // The expression for the address base register, and for base + displacement.
    virtual std::string base_expression(int base) const = 0;
    virtual std::string address(std::int64_t displacement, int base) const = 0;
};

// One instruction as the builders see it: the mnemonic, its operand tokens in the decoder's order, its address, and the raw
// instruction word (the trap builders decode TO, RA, RB and SIMM from it, so every spelling of a trap reaches one builder).
struct Instruction {
    std::string_view mnemonic;
    std::span<const std::string> operands;
    std::uint32_t address = 0;
    std::uint32_t word = 0;
};

}  // namespace cb::recompile::builders
