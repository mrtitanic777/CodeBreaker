#include "cb/recompile/builders/system.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <format>

#include "cb/core/error.hpp"
#include "cb/recompile/builders/reading.hpp"

namespace cb::recompile::builders::system {
namespace {

using Statements = std::vector<std::string>;

// the field names of a condition register, by bit within the field
constexpr std::array<std::string_view, 4> kCrBit = {"lt", "gt", "eq", "so"};

// no statements: canonical nops, barriers, the Xenon's hints, cache hints
constexpr std::array kNops = std::to_array<std::string_view>({"nop", "attn", "sync", "lwsync", "eieio", "db16cyc", "cctpl", "cctpm", "cctph",
                                                              "dcbf", "dcbt", "dcbtst", "dcbst"});

constexpr std::array kTrapWordImmediate = std::to_array<std::string_view>({"twi", "twlgti", "twllti", "tweqi", "twlgei", "twlnli", "twllei", "twlngi",
                                                                           "twgti", "twgei", "twnli", "twlti", "twlei", "twngi", "twnei"});
constexpr std::array kTrapDoubleImmediate = std::to_array<std::string_view>({"tdi", "tdlgti", "tdllti", "tdeqi", "tdlgei", "tdlnli", "tdllei", "tdlngi",
                                                                             "tdgti", "tdgei", "tdnli", "tdlti", "tdlei", "tdngi", "tdnei"});
constexpr std::array kTrapWord = std::to_array<std::string_view>({"tw", "twge", "twgt", "twle", "twlt", "tweq", "twne", "twlge", "twlgt", "twlle", "twllt"});
constexpr std::array kTrapDouble = std::to_array<std::string_view>({"td", "tdge", "tdgt", "tdle", "tdlt", "tdeq", "tdne", "tdlge", "tdlgt", "tdlle", "tdllt"});

// A condition-register bit operation: dest = [!](A op B), A and B each optionally complemented.
struct CrLogical {
    std::string_view mnemonic;
    std::string_view op;
    bool invert_a, invert_b, invert_result;
};
constexpr std::array kCrLogical = std::to_array<CrLogical>({
    {"crand", "&", false, false, false}, {"crandc", "&", false, true, false}, {"creqv", "^", false, false, true}, {"crnand", "&", false, false, true},
    {"crnor", "|", false, false, true},  {"cror", "|", false, false, false},  {"crorc", "|", false, true, false}, {"crxor", "^", false, false, false},
});

template <typename Names>
bool contains(const Names& names, std::string_view mnemonic) {
    return std::ranges::find(names, mnemonic) != names.end();
}

// ctx.cr<field>.<bit> of condition-register bit number `bit` (0..31).
std::string cr_bit_name(const Instruction& insn, std::size_t operand_index, int bit) {
    if (bit < 0) throw ParseError(insn.mnemonic, 0, static_cast<std::uint32_t>(operand_index + 1), "condition register bit " + std::to_string(bit) + " is negative");
    return std::format("ctx.cr{}.{}", bit / 4, kCrBit[static_cast<std::size_t>(bit % 4)]);
}

// The trap condition over `to` (the TO field): a trap when any selected comparison of A and B holds.
Statements trap_statements(int to, const std::string& a_signed, const std::string& a_unsigned, const std::string& b_signed, const std::string& b_unsigned) {
    if (to == 0) return {};
    if (to == 0x1F) return {"ppc_trap(ctx, base, 0);"};
    std::string condition;
    auto add = [&](bool selected, const std::string& test) {
        if (!selected) return;
        if (!condition.empty()) condition += " || ";
        condition += test;
    };
    add(to & 0x10, a_signed + " < " + b_signed);
    add(to & 0x08, a_signed + " > " + b_signed);
    add(to & 0x04, a_signed + " == " + b_signed);
    add(to & 0x02, a_unsigned + " < " + b_unsigned);
    add(to & 0x01, a_unsigned + " > " + b_unsigned);
    return {"if (" + condition + ") ppc_trap(ctx, base, 0);"};
}

Statements trap(std::string_view mnemonic, std::uint32_t word) {
    const int to = static_cast<int>((word >> 21) & 0x1F);
    const std::string ra = "ctx.r" + std::to_string((word >> 16) & 0x1F);
    const std::string rb = "ctx.r" + std::to_string((word >> 11) & 0x1F);
    const std::int64_t simm = static_cast<std::int16_t>(word & 0xFFFF);   // sign-extended
    if (contains(kTrapWordImmediate, mnemonic)) {
        // twi 31, r0, <imm> is the unconditional service trap whose code is the immediate
        if (to == 0x1F && ((word >> 16) & 0x1F) == 0) return {"ppc_trap(ctx, base, " + std::to_string(word & 0xFFFF) + ");"};
        return trap_statements(to, ra + ".s32", ra + ".u32", std::to_string(simm), std::to_string(as_u32(simm)) + "u");
    }
    if (contains(kTrapDoubleImmediate, mnemonic)) {
        return trap_statements(to, ra + ".s64", ra + ".u64", std::to_string(simm) + "ll", std::to_string(static_cast<std::uint64_t>(simm)) + "ull");
    }
    const std::string width = contains(kTrapWord, mnemonic) ? "32" : "64";
    return trap_statements(to, ra + ".s" + width, ra + ".u" + width, rb + ".s" + width, rb + ".u" + width);
}

std::string cr0_compare(int d) { return std::format("ctx.cr0.compare<int32_t>(ctx.r{}.s32, 0, ctx.xer);", d); }

Statements cr_logical(const CrLogical& op, const Instruction& insn, const Operands& ops) {
    const int dest = crf_at(insn, ops, 0);
    const int a = crf_at(insn, ops, 1);
    const int b = crf_at(insn, ops, 2);
    std::string a_expr = cr_bit_name(insn, 1, a);
    std::string b_expr = cr_bit_name(insn, 2, b);
    if (op.invert_a) a_expr = "!(" + a_expr + ")";
    if (op.invert_b) b_expr = "!(" + b_expr + ")";
    std::string expr = a_expr + " " + std::string(op.op) + " " + b_expr;
    if (op.invert_result) expr = "!(" + expr + ")";
    return {cr_bit_name(insn, 0, dest) + " = " + expr + ";"};
}

}  // namespace

std::optional<Statements> emit(const Instruction& insn, const Operands& ops) {
    // Only mr and clrldi honour the record bit; nothing else of this category legitimately ends with '.'.
    const bool record = insn.mnemonic.ends_with('.');
    const std::string_view mn = record ? insn.mnemonic.substr(0, insn.mnemonic.size() - 1) : insn.mnemonic;

    if (contains(kNops, mn)) return Statements{};
    if (contains(kTrapWordImmediate, mn) || contains(kTrapDoubleImmediate, mn) || contains(kTrapWord, mn) || contains(kTrapDouble, mn)) {
        return trap(mn, insn.word);
    }

    // cache blocks that have an effect: zero 32 (dcbz) or 128 (dcbzl) bytes
    if (mn == "dcbz" || mn == "dcbzl") {
        const int a = gpr_at(insn, ops, 0);
        const int b = gpr_at(insn, ops, 1);
        const std::string base = a != 0 ? "ctx.r" + std::to_string(a) + ".u32 + " : std::string();
        const int size = mn == "dcbz" ? 32 : 128;
        return Statements{std::format("ea = ({}ctx.r{}.u32) & ~{};", base, b, size - 1), std::format("memset((void*)PPC_RAW_ADDR(ea), 0, {});", size)};
    }

    if (mn == "mr") {
        const int d = gpr_at(insn, ops, 0);
        const int s = gpr_at(insn, ops, 1);
        Statements out{std::format("ctx.r{}.u64 = ctx.r{}.u64;", d, s)};
        if (record) out.push_back(cr0_compare(d));
        return out;
    }
    if (mn == "mcrf") return Statements{std::format("ctx.cr{} = ctx.cr{};", crf_at(insn, ops, 0), crf_at(insn, ops, 1))};

    // move from special registers
    if (mn == "mfxer") return Statements{std::format("ctx.r{}.u64 = (ctx.xer.so << 31) | (ctx.xer.ov << 30) | (ctx.xer.ca << 29);", gpr_at(insn, ops, 0))};
    if (mn == "mfctr") return Statements{std::format("ctx.r{}.u64 = ctx.ctr.u64;", gpr_at(insn, ops, 0))};
    if (mn == "mfcr") {
        const int d = gpr_at(insn, ops, 0);
        Statements out;
        for (int i = 0; i < 32; ++i) {
            out.push_back(std::format("ctx.r{}.u64 {}= ctx.cr{}.{} ? {} : 0;", d, i == 0 ? "" : "|", i / 4, kCrBit[static_cast<std::size_t>(i % 4)],
                                      hex(std::uint64_t{1} << (31 - i))));
        }
        return out;
    }
    if (mn == "mfocrf") {
        const int d = gpr_at(insn, ops, 0);
        const std::int64_t fxm = imm_at(insn, ops, 1);
        int field = 0;   // the first field the mask selects
        for (int i = 0; i < 8; ++i) {
            if (fxm & (0x80 >> i)) {
                field = i;
                break;
            }
        }
        const int shift = 28 - 4 * field;
        return Statements{std::format("ctx.r{}.u64 = (ctx.cr{}.lt << {}) | (ctx.cr{}.gt << {}) | (ctx.cr{}.eq << {}) | (ctx.cr{}.so << {});", d, field,
                                      shift + 3, field, shift + 2, field, shift + 1, field, shift)};
    }
    if (mn == "mflr") return Statements{std::format("ctx.r{}.u64 = ctx.lr;", gpr_at(insn, ops, 0))};
    if (mn == "mfmsr") {
        return Statements{"std::atomic_thread_fence(std::memory_order_seq_cst);", std::format("ctx.r{}.u64 = PPC_CHECK_GLOBAL_LOCK();", gpr_at(insn, ops, 0))};
    }
    if (mn == "mffs") return Statements{std::format("ctx.f{}.u64 = ctx.fpscr.loadFromHost();", fpr_at(insn, ops, 0))};
    if (mn == "mftb") return Statements{std::format("ctx.r{}.u64 = PPC_QUERY_TIMEBASE();", gpr_at(insn, ops, 0))};
    if (mn == "mftbu") return Statements{std::format("ctx.r{}.u64 = PPC_QUERY_TIMEBASE() >> 32;", gpr_at(insn, ops, 0))};

    // move to special registers
    if (mn == "mtcr") {
        const int s = gpr_at(insn, ops, 0);
        Statements out;
        for (int i = 0; i < 32; ++i) {
            out.push_back(std::format("ctx.cr{}.{} = (ctx.r{}.u32 & {}) != 0;", i / 4, kCrBit[static_cast<std::size_t>(i % 4)], s, hex(std::uint64_t{1} << (31 - i))));
        }
        return out;
    }
    if (mn == "mtcrf" || mn == "mtocrf") {
        const std::int64_t fxm = imm_at(insn, ops, 0);
        const int s = gpr_at(insn, ops, 1);
        Statements out;
        for (int field = 0; field < 8; ++field) {
            if (!(fxm & (0x80 >> field))) continue;
            const int base_bit = 28 - 4 * field;
            for (int bit = 0; bit < 4; ++bit) {
                out.push_back(std::format("ctx.cr{}.{} = (ctx.r{}.u32 & {}) != 0;", field, kCrBit[static_cast<std::size_t>(bit)], s,
                                          hex(std::uint64_t{1} << (base_bit + 3 - bit))));
            }
        }
        return out;
    }
    if (mn == "mtctr") return Statements{std::format("ctx.ctr.u64 = ctx.r{}.u64;", gpr_at(insn, ops, 0))};
    if (mn == "mtlr") return Statements{std::format("ctx.lr = ctx.r{}.u64;", gpr_at(insn, ops, 0))};
    if (mn == "mtmsrd") {
        const int s = gpr_at(insn, ops, 0);
        // r13 = enter the global lock (interrupts off); any other register = leave it
        return Statements{"std::atomic_thread_fence(std::memory_order_seq_cst);", std::format("ctx.msr = (ctx.r{}.u32 & 0x8020) | (ctx.msr & ~0x8020);", s),
                          s == 13 ? "PPC_ENTER_GLOBAL_LOCK();" : "PPC_LEAVE_GLOBAL_LOCK();"};
    }
    if (mn == "mtfsf") {
        const std::int64_t fm = imm_at(insn, ops, 0);
        const int b = fpr_at(insn, ops, 1);
        std::uint64_t mask = 0;
        for (int j = 0; j < 8; ++j) {
            if (fm & (std::int64_t{1} << (7 - j))) mask |= std::uint64_t{0xF} << (4 * j);
        }
        if (mask == 0xFFFFFFFFull) return Statements{std::format("ctx.fpscr.storeFromGuest(ctx.f{}.u32);", b)};
        return Statements{std::format("ctx.fpscr.storeFromGuest((ctx.fpscr.loadFromHost() & 0x{:08X}) | (ctx.f{}.u32 & 0x{:08X}));", ~mask & 0xFFFFFFFFull, b, mask)};
    }
    if (mn == "mtxer") {
        const int s = gpr_at(insn, ops, 0);
        return Statements{std::format("ctx.xer.so = (ctx.r{}.u64 & 0x80000000) != 0;", s), std::format("ctx.xer.ov = (ctx.r{}.u64 & 0x40000000) != 0;", s),
                          std::format("ctx.xer.ca = (ctx.r{}.u64 & 0x20000000) != 0;", s)};
    }

    // clear left doubleword immediate (a simplified rldicl)
    if (mn == "clrldi") {
        const int d = gpr_at(insn, ops, 0);
        const int s = gpr_at(insn, ops, 1);
        const std::int64_t n = imm_at(insn, ops, 2);
        if (n > 64) throw ParseError(insn.mnemonic, 0, 3, "clear count " + std::to_string(n) + " is above 64");
        const std::uint64_t mask = n <= 0 ? ~std::uint64_t{0} : n == 64 ? 0 : (std::uint64_t{1} << (64 - n)) - 1;
        Statements out{std::format("ctx.r{}.u64 = ctx.r{}.u64 & {};", d, s, hex(mask))};
        if (record) out.push_back(cr0_compare(d));
        return out;
    }

    for (const CrLogical& op : kCrLogical) {
        if (op.mnemonic == mn) return cr_logical(op, insn, ops);
    }
    return std::nullopt;
}

std::string_view runtime_helpers() { return ""; }

std::span<const std::string_view> mnemonics() {
    static const std::vector<std::string_view> names = [] {
        std::vector<std::string_view> out(kNops.begin(), kNops.end());
        for (const std::string_view m : {"dcbz", "dcbzl", "mr", "mcrf", "mfxer", "mfctr", "mfcr", "mfocrf", "mflr", "mfmsr", "mffs", "mftb", "mftbu", "mtcr",
                                         "mtcrf", "mtocrf", "mtctr", "mtlr", "mtmsrd", "mtfsf", "mtxer", "clrldi"}) {
            out.push_back(m);
        }
        for (const CrLogical& op : kCrLogical) out.push_back(op.mnemonic);
        for (const auto* group : {&kTrapWordImmediate, &kTrapDoubleImmediate}) out.insert(out.end(), group->begin(), group->end());
        out.insert(out.end(), kTrapWord.begin(), kTrapWord.end());
        out.insert(out.end(), kTrapDouble.begin(), kTrapDouble.end());
        return out;
    }();
    return names;
}

}  // namespace cb::recompile::builders::system
