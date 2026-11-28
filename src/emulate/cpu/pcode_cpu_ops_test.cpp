// The p-code operations blocks run written out (decompile/engine/pcode_emulator.cc) against the description's behaviour table: the same result for every operation, every
// pair of sizes up to 8 bytes, and edge and random values (inputs are masked to their size, as the emulator hands them over).
#include <array>
#include <cstdio>
#include <random>
#include <vector>

#include "core/testing.hpp"
#include "decompile/engine/core/cpp/opcodes.hh"
#include "decompile/engine/pcode_emulator.hh"
#include "emulate/cpu/pcode_cpu_internal.hpp"

using namespace cb;

CB_TEST("emulate.pcode_cpu.written_out_p_code_operations_match_the_behaviour_table", [] {
    namespace gh = ghidra;
    decompile::sleigh::Emulator& emu = emulate::pcode_cpu::emulator_named("6502.sla");
    const std::array<unsigned, 27> ops = {gh::CPUI_COPY,      gh::CPUI_INT_ZEXT,      gh::CPUI_INT_SEXT,  gh::CPUI_INT_ADD,   gh::CPUI_INT_SUB,       gh::CPUI_INT_MULT,
                                          gh::CPUI_INT_AND,   gh::CPUI_INT_OR,        gh::CPUI_INT_XOR,   gh::CPUI_INT_EQUAL, gh::CPUI_INT_NOTEQUAL,  gh::CPUI_INT_LESS,
                                          gh::CPUI_INT_LESSEQUAL, gh::CPUI_INT_SLESS, gh::CPUI_INT_SLESSEQUAL, gh::CPUI_INT_LEFT, gh::CPUI_INT_RIGHT, gh::CPUI_SUBPIECE,
                                          gh::CPUI_INT_CARRY, gh::CPUI_INT_SCARRY, gh::CPUI_INT_SBORROW, gh::CPUI_INT_NEGATE, gh::CPUI_INT_2COMP, gh::CPUI_BOOL_NEGATE, gh::CPUI_BOOL_AND,
                                          gh::CPUI_BOOL_OR, gh::CPUI_BOOL_XOR};
    constexpr std::array<std::uint64_t, 15> edges = {0, 1, 2, 0x7F, 0x80, 0xFF, 0x7FFF, 0x8000, 0xFFFF, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0x7FFFFFFFFFFFFFFFull, 0x8000000000000000ull,
                                                     ~std::uint64_t{0}};
    const auto mask = [](int size) { return size >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * size)) - 1; };
    std::mt19937_64 rng(20261122);
    unsigned checked = 0, different = 0;
    for (const unsigned op : ops) {
        const bool unary = op == gh::CPUI_COPY || op == gh::CPUI_INT_ZEXT || op == gh::CPUI_INT_SEXT || op == gh::CPUI_INT_NEGATE || op == gh::CPUI_INT_2COMP || op == gh::CPUI_BOOL_NEGATE;
        for (int out = 1; out <= 8; ++out)
            for (int in = 1; in <= 8; ++in) {
                std::vector<std::uint64_t> values(edges.begin(), edges.end());
                for (int k = 0; k < 20; ++k) values.push_back(rng());
                for (const std::uint64_t x : values)
                    for (const std::uint64_t y : values) {
                        const std::uint64_t a = x & mask(in);
                        // (a shift count is any width, a SUBPIECE offset is in bytes; the others' second input is the first's size)
                        const std::uint64_t c = (op == gh::CPUI_INT_LEFT || op == gh::CPUI_INT_RIGHT) ? (y & 0xFF) % 80 : op == gh::CPUI_SUBPIECE ? (y & 0xFF) % 16 : y & mask(in);
                        std::uint64_t fast = 0, slow = 0;
                        CB_CHECK(emu.evaluateOp(op, out, in, a, c, fast, true));
                        CB_CHECK(emu.evaluateOp(op, out, in, a, c, slow, false));
                        ++checked;
                        if (fast != slow && ++different <= 10)
                            std::printf("  op %u out %d in %d: a=%llx c=%llx: written out %llx, table %llx\n", op, out, in, static_cast<unsigned long long>(a), static_cast<unsigned long long>(c),
                                        static_cast<unsigned long long>(fast), static_cast<unsigned long long>(slow));
                        if (unary) break;
                    }
            }
    }
    std::printf("pcode ops: %u evaluations, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});
