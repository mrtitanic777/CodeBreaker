// Tests of cb/recompile/jumptable: hand-made instruction sequences (every shape the recogniser has a path for) and
// real tables of Minecraft's Xbox 360 image (testdata/switch_samples.txt, with what the original produced).

#include "cb/recompile/jumptable.hpp"

#include "cb/core/testing.hpp"
#include "cb/recompile/test_ppc.hpp"

using namespace cb::recompile;
using namespace cb::recompile::jumptable;
using namespace cb::recompile::test;

namespace {

constexpr std::uint32_t kBase = 0x82000000;
constexpr std::uint32_t kTable = kBase + 0x1000;

// The test image as guest memory: mapped inside the buffer, big-endian; code is [exec_lo, exec_hi).
class ImageMemory final : public GuestMemory {
public:
    ImageMemory(const Mem& m, std::uint32_t exec_lo, std::uint32_t exec_hi) : base_(m.base), bytes_(m.bytes), lo_(exec_lo), hi_(exec_hi) {}
    ImageMemory(std::uint32_t base, std::vector<std::uint8_t> bytes, std::uint32_t exec_lo, std::uint32_t exec_hi)
        : base_(base), bytes_(std::move(bytes)), lo_(exec_lo), hi_(exec_hi) {}
    std::optional<std::uint8_t> u8(std::uint32_t a) const override { return narrow<std::uint8_t>(read(a, 1)); }
    std::optional<std::uint16_t> u16(std::uint32_t a) const override { return narrow<std::uint16_t>(read(a, 2)); }
    std::optional<std::uint32_t> u32(std::uint32_t a) const override { return read(a, 4); }
    bool is_exec(std::uint32_t a) const override { return a >= lo_ && a < hi_; }

private:
    template <class T>
    static std::optional<T> narrow(const std::optional<std::uint32_t>& v) {
        return v ? std::optional<T>(static_cast<T>(*v)) : std::nullopt;
    }
    std::optional<std::uint32_t> read(std::uint32_t a, std::size_t n) const {
        if (a < base_ || std::uint64_t{a} - base_ + n > bytes_.size()) return std::nullopt;
        std::uint32_t v = 0;
        for (std::size_t i = 0; i < n; ++i) v = (v << 8) | bytes_[a - base_ + i];
        return v;
    }
    std::uint32_t base_;
    std::vector<std::uint8_t> bytes_;
    std::uint32_t lo_, hi_;
};

// Memory that answers for every address: the words of `m` where it has them, `elsewhere` otherwise (an aligned
// code address, as table entries read); every address is code.
class FilledMemory final : public GuestMemory {
public:
    FilledMemory(const Mem& m, std::uint32_t elsewhere, std::uint64_t mapped_below = 0x100000000ull)
        : m_(m), elsewhere_(elsewhere), mapped_below_(mapped_below) {}
    std::optional<std::uint8_t> u8(std::uint32_t) const override { return std::uint8_t{4}; }
    std::optional<std::uint16_t> u16(std::uint32_t) const override { return std::uint16_t{4}; }
    std::optional<std::uint32_t> u32(std::uint32_t a) const override {
        if (a >= m_.base && std::uint64_t{a} + 4 <= std::uint64_t{m_.base} + m_.bytes.size()) {
            std::uint32_t v = 0;
            for (int i = 0; i < 4; ++i) v = (v << 8) | m_.bytes[a - m_.base + i];
            return v;
        }
        if (a >= mapped_below_) return std::nullopt;
        return elsewhere_;
    }
    bool is_exec(std::uint32_t) const override { return true; }

private:
    const Mem& m_;
    std::uint32_t elsewhere_;
    std::uint64_t mapped_below_;
};

std::optional<JumpTable> detect_at_end(Mem& m, std::uint32_t end, std::uint32_t exec_lo = kBase, std::uint32_t exec_hi = kBase + 0x400) {
    m.u32(end - 4, kBctrWord);
    const ImageMemory mem(m, exec_lo, exec_hi);
    return detect_one(mem, end - 4);
}

std::vector<std::uint32_t> at(std::initializer_list<std::uint32_t> offsets) {
    std::vector<std::uint32_t> out;
    for (const std::uint32_t o : offsets) out.push_back(kBase + o);
    return out;
}

}  // namespace

CB_TEST("recompile.jumptable.absolute_table_with_cmplwi_bound", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40),
                                                      slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->index_reg, 11u);
    CB_CHECK(t->targets == at({0x200, 0x210, 0x220, 0x230}));
});

CB_TEST("recompile.jumptable.byte_offsets_from_a_base", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(10, 0x8200), addi(10, 10, 0x1100), lis(12, 0x8200), addi(12, 12, 0x200),
                                                      cmplwi(6, 11, 4), bgt(6, 0x40), lbzx(0, 10, 11), add(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 5; ++i) m.u8(kBase + 0x1100 + i, static_cast<std::uint8_t>(4 * i));
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->index_reg, 11u);
    CB_CHECK(t->targets == at({0x200, 0x204, 0x208, 0x20C, 0x210}));
});

CB_TEST("recompile.jumptable.halfword_offsets_and_a_table_address_with_a_negative_low_half", [] {
    // lis 0x8201 + addi 0x8010 (sign-extended: -0x7FF0) is 0x82008010
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(10, 0x8201), addi(10, 10, 0x8010), lis(12, 0x8200), addi(12, 12, 0x200),
                                                      cmplwi(6, 11, 2), bgt(6, 0x40), slwi(0, 11, 1), lhzx(0, 10, 0), add(11, 12, 0),
                                                      mtctr(11), kBctrWord});
    const std::uint16_t offsets[] = {0, 0x10, 0x20};
    for (int i = 0; i < 3; ++i) m.u16(kBase + 0x8010 + 2 * i, offsets[i]);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x210, 0x220}));
});

CB_TEST("recompile.jumptable.computed_offsets_shifted_left", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(10, 0x8200), addi(10, 10, 0x1100), lis(12, 0x8200), addi(12, 12, 0x200),
                                                      cmplwi(6, 11, 3), bgt(6, 0x40), lbzx(9, 10, 11), slwi(0, 9, 2), add(11, 12, 0),
                                                      mtctr(11), kBctrWord});
    const std::uint8_t offsets[] = {0, 1, 2, 4};
    for (int i = 0; i < 4; ++i) m.u8(kBase + 0x1100 + i, offsets[i]);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x204, 0x208, 0x210}));
});

CB_TEST("recompile.jumptable.table_register_reused_as_the_base", [] {
    // the same register holds the table's address for the load and then the base for the add
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {cmplwi(6, 11, 2), bgt(6, 0x40), lis(12, 0x8200), addi(12, 12, 0x1100), lbzx(0, 12, 11),
                                                      lis(12, 0x8200), addi(12, 12, 0x200), add(11, 12, 0), mtctr(11), kBctrWord});
    const std::uint8_t offsets[] = {0, 8, 16};
    for (int i = 0; i < 3; ++i) m.u8(kBase + 0x1100 + i, offsets[i]);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x208, 0x210}));
});

CB_TEST("recompile.jumptable.lwzx_operands_the_wrong_way_round_are_swapped", [] {
    // lwzx rCTR, rIndex, rTable: the first guess (RA is the table) is wrong and a lis on RB corrects it; the
    // index register is then the scaled one (r0), as the original reports it
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40),
                                                      slwi(0, 11, 2), lwzx(11, 0, 12), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->index_reg, 0u);
    CB_CHECK(t->targets == at({0x200, 0x210, 0x220, 0x230}));
});

CB_TEST("recompile.jumptable.clrlwi_mask_gives_the_entry_count", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), clrlwi(11, 11, 30), slwi(0, 11, 2),
                                                      lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    m.u32(kTable + 16, kBase + 0x300);   // a fifth entry the mask rules out
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->targets.size(), 4u);
});

CB_TEST("recompile.jumptable.no_bound_reads_until_the_entries_stop_being_code", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 3; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    m.u32(kTable + 12, 0x83000000);   // outside the code
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x210, 0x220}));
});

CB_TEST("recompile.jumptable.a_misaligned_entry_ends_the_table", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 5), bgt(6, 0x40),
                                                      slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    const std::uint32_t entries[] = {0x200, 0x210, 0x222, 0x230};
    for (int i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + entries[i]);
    const auto t = detect_at_end(m, end);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x210}));
});

CB_TEST("recompile.jumptable.rejected_shapes", [] {
    {   // one target is not a switch
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
        m.u32(kTable, kBase + 0x200);
        CB_CHECK(!detect_at_end(m, end).has_value());
    }
    {   // a return between the setup and the bctr ends the straight-line code
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40),
                                                          slwi(0, 11, 2), blr(), lwzx(11, 12, 0), mtctr(11), kBctrWord});
        for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
        CB_CHECK(!detect_at_end(m, end).has_value());
    }
    {   // a bctr with nothing before it
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {kBctrWord});
        CB_CHECK(!detect_at_end(m, end).has_value());
    }
    {   // the table lies outside the mapped memory
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x9000), addi(12, 12, 0), cmplwi(6, 11, 3), bgt(6, 0x40),
                                                          slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
        CB_CHECK(!detect_at_end(m, end).has_value());
    }
    {   // a bctr at the very start of the image: the scan back runs out of addresses
        Mem m(0, 0x20);
        m.u32(4, kBctrWord);
        const ImageMemory mem(m, 0, 0x20);
        CB_CHECK(!detect_one(mem, 4).has_value());
        CB_CHECK(!detect_one(mem, 0).has_value());
    }
});

CB_TEST("recompile.jumptable.detect_jump_tables_takes_only_bctr_not_bctrl", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40),
                                                      slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    const ImageMemory mem(m, kBase, kBase + 0x400);
    const std::uint32_t bctr = end - 4;
    // the same instructions, but the last one is bctrl (link bit set)
    const std::vector<Instruction> with_link = {{bctr, kBctrWord | 1}};
    CB_CHECK(detect_jump_tables(with_link, mem).empty());
    const std::vector<Instruction> plain = {{kBase + 0x100, lis(12, 0x8200)}, {bctr, kBctrWord}};
    const auto found = detect_jump_tables(plain, mem);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK(found.contains(bctr));
    CB_CHECK_EQ(found.at(bctr).targets.size(), 4u);
});

CB_TEST("recompile.jumptable.a_table_without_a_bound_is_capped", [] {
    // DIVERGES: the original read an unbounded table as long as the memory kept answering; a table that never
    // stops looking like code is cut at kMaxTableEntries (a real switch is nowhere near it)
    Mem m(kBase, 0x40);
    m.code(kBase, {lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    const FilledMemory mem(m, 0x82000200);   // the table entries (outside the 0x40 bytes of code) are all code addresses
    const auto t = detect_one(mem, kBase + 0x14);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->targets.size(), kMaxTableEntries);
});

CB_TEST("recompile.jumptable.table_addresses_do_not_wrap_past_4gb", [] {
    // The table is at 0xFFFFFFF8 (lis 0 + addi -8) with no bound: two entries are mapped; the third would be at
    // 0x1_0000_0000, which is unmapped -- not address 0, where this memory would answer with a code address.
    Mem m(0xFFFFFF00, 0x100);
    const std::uint32_t end = m.code(0xFFFFFF00, {lis(12, 0), addi(12, 12, 0xFFF8), slwi(0, 11, 2), lwzx(11, 12, 0), mtctr(11), kBctrWord});
    m.u32(0xFFFFFFF8, 0x82000200);
    m.u32(0xFFFFFFFC, 0x82000210);
    const FilledMemory mem(m, 0x82000220, 0x100000000ull);   // below the image, every word is a code address
    const auto t = detect_one(mem, end - 4);
    CB_CHECK(t.has_value());
    CB_CHECK(t->targets == at({0x200, 0x210}));
});

// The memory of a sample: only the windows that were kept are mapped (the original was run on exactly that).
class SampleMemory final : public GuestMemory {
public:
    explicit SampleMemory(const Sample& s) : s_(s) {}
    std::optional<std::uint8_t> u8(std::uint32_t a) const override { return narrow<std::uint8_t>(read(a, 1)); }
    std::optional<std::uint16_t> u16(std::uint32_t a) const override { return narrow<std::uint16_t>(read(a, 2)); }
    std::optional<std::uint32_t> u32(std::uint32_t a) const override { return read(a, 4); }
    bool is_exec(std::uint32_t a) const override { return a >= s_.exec_lo && a < s_.exec_hi; }

private:
    template <class T>
    static std::optional<T> narrow(const std::optional<std::uint32_t>& v) {
        return v ? std::optional<T>(static_cast<T>(*v)) : std::nullopt;
    }
    std::optional<std::uint32_t> read(std::uint32_t a, std::size_t n) const {
        for (const auto& [base, bytes] : s_.windows) {
            if (a >= base && std::uint64_t{a} - base + n <= bytes.size()) {
                std::uint32_t v = 0;
                for (std::size_t i = 0; i < n; ++i) v = (v << 8) | bytes[a - base + i];
                return v;
            }
        }
        return std::nullopt;
    }
    const Sample& s_;
};

CB_TEST("recompile.jumptable.real_tables_of_minecraft_match_the_original", [] {
    const std::vector<Sample> samples = load_samples();
    CB_CHECK(samples.size() >= 15);
    std::size_t tables = 0;
    for (const Sample& s : samples) {
        const SampleMemory mem(s);
        std::vector<Instruction> insns;
        for (std::uint32_t a = s.code_lo; a < s.code_hi; a += 4) insns.push_back({a, *mem.u32(a)});
        const auto found = detect_jump_tables(insns, mem);
        CB_CHECK_EQ(found.size(), s.jumps.size());
        for (const ExpectedJump& j : s.jumps) {
            const auto it = found.find(j.bctr);
            CB_CHECK(it != found.end());
            if (it == found.end()) continue;
            CB_CHECK_EQ(it->second.index_reg, j.reg);
            if (it->second.targets != j.targets) {
                cb::testing::record_failure(__FILE__, __LINE__, std::format("{}: {} targets at {:X}, the original found {}", s.name, it->second.targets.size(), j.bctr, j.targets.size()));
            }
            ++tables;
        }
    }
    CB_CHECK(tables >= 5);   // (the recompiler's recogniser finds six of the twenty windows' bctrs)
});
