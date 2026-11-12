// The six builders together, on real code: the instructions of 16 KB of Minecraft (Xbox 360 Edition), as Capstone and the
// VMX128 decoder spell them, handed to the builders in the order the recompiler tries them.
#include <algorithm>
#include <map>

#include "cb/recompile/builders/arith.hpp"
#include "cb/recompile/builders/floating.hpp"
#include "cb/recompile/builders/logical.hpp"
#include "cb/recompile/builders/memory.hpp"
#include "cb/recompile/builders/system.hpp"
#include "cb/recompile/builders/test_support.hpp"
#include "cb/recompile/builders/vector.hpp"

using namespace cb;
using cb::recompile::builders::Instruction;
using cb::recompile::builders::Operands;
namespace rb = cb::recompile::builders;

namespace {

using Statements = std::optional<std::vector<std::string>>;

// The first builder that answers, in the recompiler's order (a builder that refuses an operand is skipped).
Statements first_answer(const Instruction& insn, const Operands& operands) {
    using Emit = Statements (*)(const Instruction&, const Operands&);
    static constexpr Emit kBuilders[] = {rb::floating::emit, rb::arith::emit, rb::logical::emit, rb::system::emit, rb::memory::emit, rb::vector::emit};
    for (const Emit emit : kBuilders) {
        try {
            if (Statements got = emit(insn, operands)) return got;
        } catch (const ParseError&) {
            continue;
        }
    }
    return std::nullopt;
}

}  // namespace

CB_TEST("recompile.builders.real_minecraft_code_matches_the_reference", [] {
    const std::size_t cases = rb::test::check_golden("real.golden.txt", first_answer);
    CB_CHECK_EQ(cases, 1404u);
});

CB_TEST("recompile.builders.no_mnemonic_belongs_to_two_builders", [] {
    std::map<std::string, std::string> owner;
    auto claim = [&](std::string_view builder, std::span<const std::string_view> mnemonics) {
        for (const std::string_view m : mnemonics) {
            const auto [it, fresh] = owner.emplace(std::string(m), std::string(builder));
            CB_CHECK(fresh);
            if (!fresh) CB_CHECK_EQ(it->second, std::string(builder));   // (names the other builder)
        }
    };
    claim("floating", rb::floating::mnemonics());
    claim("arith", rb::arith::mnemonics());
    claim("logical", rb::logical::mnemonics());
    claim("system", rb::system::mnemonics());
    claim("memory", rb::memory::mnemonics());
    claim("vector", rb::vector::mnemonics());
    CB_CHECK_EQ(owner.size(), 32u + 34u + 41u + 95u + 91u + 172u);
});

CB_TEST("recompile.builders.each_builder_answers_every_mnemonic_it_lists", [] {
    // a listed mnemonic is never "not ours": with no operands it is a ParseError (an operand is missing), or statements for
    // the instructions that take none (nop, sync ...)
    const rb::test::TokenOperands tokens;
    const std::vector<std::string> nothing;
    auto answers_all = [&](std::span<const std::string_view> names, auto emit) {
        for (const std::string_view m : names) {
            try {
                CB_CHECK(emit(Instruction{m, nothing, 0, 0}, tokens).has_value());
            } catch (const ParseError&) {
            }
        }
    };
    answers_all(rb::floating::mnemonics(), rb::floating::emit);
    answers_all(rb::arith::mnemonics(), rb::arith::emit);
    answers_all(rb::logical::mnemonics(), rb::logical::emit);
    answers_all(rb::memory::mnemonics(), rb::memory::emit);
    answers_all(rb::system::mnemonics(), rb::system::emit);
    answers_all(rb::vector::mnemonics(), rb::vector::emit);
});
