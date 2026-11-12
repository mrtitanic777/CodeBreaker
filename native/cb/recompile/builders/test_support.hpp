// cb/recompile/builders/test_support.hpp -- what the builders' tests share: operand tokens as the recompiler spells them, and
// the reader of the golden files (testdata/<module>.golden.txt, made by testdata/gen_builders_golden.py from the reference
// builders over a spread of operands, and spot-checked against the PowerPC manual's definitions).
#pragma once

#include <charconv>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/recompile/builders/operands.hpp"

namespace cb::recompile::builders::test {

// Operand tokens as powerpc's helpers read them: r3 f4 v5 cr6 (a field), cr1eq (a bit, field * 4 + bit), 0x10, "-0x70(r1)".
class TokenOperands : public Operands {
public:
    std::optional<int> reg(std::string_view t) const override { return t == "0" ? std::optional<int>(0) : number(t, "r"); }
    std::optional<int> vreg(std::string_view t) const override { return number(t, "v"); }
    std::optional<int> freg(std::string_view t) const override { return number(t, "f"); }
    std::optional<int> cr(std::string_view t) const override {
        if (!t.starts_with("cr") || t.size() < 3) return std::nullopt;
        if (t.size() == 3) return number(t, "cr");
        const std::optional<int> field = number(t.substr(0, 3), "cr");
        const std::string_view bit = t.substr(3);
        if (!field || bit.size() != 2) return std::nullopt;
        for (int i = 0; i < 5; ++i) {
            static constexpr std::string_view kNames[] = {"lt", "gt", "eq", "un", "so"};
            if (bit == kNames[i]) return *field * 4 + (i == 4 ? 3 : i);
        }
        return std::nullopt;
    }
    std::optional<std::int64_t> imm(std::string_view t) const override {
        bool negative = false;
        if (t.starts_with('-')) {
            negative = true;
            t.remove_prefix(1);
        }
        int base = 10;
        if (t.starts_with("0x") || t.starts_with("0X")) {
            base = 16;
            t.remove_prefix(2);
        }
        std::int64_t value = 0;
        const auto [end, error] = std::from_chars(t.data(), t.data() + t.size(), value, base);
        if (error != std::errc() || end != t.data() + t.size()) return std::nullopt;
        return negative ? -value : value;
    }
    std::optional<MemoryOperand> mem(std::string_view t) const override {
        const std::size_t open = t.find('(');
        if (open == std::string_view::npos || !t.ends_with(')')) return std::nullopt;
        const std::optional<std::int64_t> disp = imm(t.substr(0, open));
        const std::string_view base = t.substr(open + 1, t.size() - open - 2);
        const std::optional<int> base_register = reg(base);
        if (!disp || !base_register) return std::nullopt;
        return MemoryOperand{*disp, *base_register};
    }
    std::string register_value(int n, std::string_view field) const override { return "ctx.r" + std::to_string(n) + "." + std::string(field); }
    std::string base_expression(int base) const override { return base == 0 ? "0" : "ctx.r" + std::to_string(base) + ".u32"; }
    std::string address(std::int64_t displacement, int base) const override {
        return base == 0 ? std::to_string(displacement) : base_expression(base) + " + " + std::to_string(displacement);
    }

private:
    static std::optional<int> number(std::string_view token, std::string_view prefix) {
        if (!token.starts_with(prefix)) return std::nullopt;
        token.remove_prefix(prefix.size());
        return text::parse_int<int>(token);
    }
};

// One case of a golden file: "## <mnemonic> <operands> [@0xWORD]" and the statements the builder must give.
struct GoldenCase {
    std::string mnemonic;
    std::vector<std::string> operands;
    std::uint32_t word = 0;
    std::vector<std::string> statements;
};

inline std::vector<GoldenCase> read_golden(std::string_view file) {
    std::vector<GoldenCase> cases;
    const std::string body = fs::read_text(testing::data_path(std::string("recompile/testdata/") + std::string(file)));
    for (std::string_view line : text::lines(body)) {
        if (line.starts_with("## ")) {
            const auto words = text::split_ws(line.substr(3));
            GoldenCase c;
            c.mnemonic = std::string(words[0]);
            for (std::size_t i = 1; i < words.size(); ++i) {
                if (words[i].starts_with('@')) {
                    c.word = static_cast<std::uint32_t>(std::stoul(std::string(words[i].substr(1)), nullptr, 16));
                } else {
                    c.operands.emplace_back(words[i]);
                }
            }
            cases.push_back(std::move(c));
        } else {
            cases.back().statements.emplace_back(line);
        }
    }
    return cases;
}

// Runs `emit` (a function from Instruction and Operands to optional statements) over every case of the golden file and
// compares; returns the number of cases.
template <class Emit>
std::size_t check_golden(std::string_view file, Emit emit) {
    const std::vector<GoldenCase> cases = read_golden(file);
    const TokenOperands tokens;
    for (const GoldenCase& c : cases) {
        const std::optional<std::vector<std::string>> got = emit(Instruction{c.mnemonic, c.operands, 0x82000000, c.word}, tokens);
        CB_CHECK(got.has_value());
        if (!got) continue;
        const std::string label = c.mnemonic + " " + text::join(c.operands, " ") + "\n";   // (shown with the statements when they differ)
        CB_CHECK_EQ(label + text::join(*got, "\n"), label + text::join(c.statements, "\n"));
    }
    return cases.size();
}

}  // namespace cb::recompile::builders::test
