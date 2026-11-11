// Tests of cb/recompile/switches: hand-made code (the shapes the symbolic scan classifies, and the ways it gives up),
// the XenonRecomp file it writes, and real tables of Minecraft's Xbox 360 image (testdata/switch_samples.txt, with
// what the original produced on exactly those bytes).

#include "cb/recompile/switches.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/recompile/test_ppc.hpp"

using namespace cb::recompile::switches;
using namespace cb::recompile::test;

namespace {

constexpr std::uint32_t kBase = 0x82000000;
constexpr std::uint32_t kTable = kBase + 0x1000;

CodeImage image_of(const Mem& m, std::uint32_t code_begin, std::uint32_t code_end, std::vector<Extent> functions = {}) {
    CodeImage image;
    image.base = m.base;
    image.data = m.bytes;
    image.code_begin = code_begin;
    image.code_end = code_end;
    image.functions = std::move(functions);
    return image;
}

std::vector<std::uint32_t> at(std::initializer_list<std::uint32_t> offsets) {
    std::vector<std::uint32_t> out;
    for (const std::uint32_t o : offsets) out.push_back(kBase + o);
    return out;
}

// the code of a guarded absolute switch on r11 (the counter register is r10, distinct from the index), table at kTable
std::uint32_t guarded_absolute(Mem& m, std::uint32_t at_addr) {
    const std::uint32_t end = m.code(at_addr, {cmplwi(6, 11, 3), bgt(6, 0x40), lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2),
                                               lwzx(10, 12, 0), mtctr(10), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + 0x200 + 0x10 * i);
    return end;
}

}  // namespace

CB_TEST("recompile.switches.guarded_absolute_table", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = guarded_absolute(m, kBase + 0x100);
    const Found f = find(image_of(m, kBase + 0x100, end));
    CB_CHECK_EQ(f.tables.size(), 1u);
    const SwitchTable& t = f.tables.at(0);
    CB_CHECK_EQ(t.bctr, end - 4);
    CB_CHECK_EQ(t.index_reg, 11u);
    CB_CHECK(t.kind == TableKind::Absolute);
    CB_CHECK(t.labels == at({0x200, 0x210, 0x220, 0x230}));
    CB_CHECK_EQ(t.default_target, kBase + 0x144);   // bgt at 0x104 + 0x40
    CB_CHECK(t.guard.has_value());
    CB_CHECK_EQ(t.guard.value_or(0), kBase + 0x100);
    CB_CHECK(!t.function.has_value());
    CB_CHECK_EQ(f.report.tables, 1u);
    CB_CHECK_EQ(f.report.kinds.at(TableKind::Absolute), 1u);
    CB_CHECK(f.report.unresolved.empty() && f.report.outside.empty() && f.report.pointer_calls.empty());
});

CB_TEST("recompile.switches.byte_table_with_a_base", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {cmplwi(6, 11, 4), bgt(6, 0x40), lis(10, 0x8200), addi(10, 10, 0x1100), lis(12, 0x8200),
                                                      addi(12, 12, 0x200), lbzx(0, 10, 11), add(9, 12, 0), mtctr(9), kBctrWord});
    for (std::uint32_t i = 0; i < 5; ++i) m.u8(kBase + 0x1100 + i, static_cast<std::uint8_t>(4 * i));
    const Found f = find(image_of(m, kBase + 0x100, end));
    CB_CHECK_EQ(f.tables.size(), 1u);
    CB_CHECK(f.tables.at(0).kind == TableKind::Byte);
    CB_CHECK(f.tables.at(0).labels == at({0x200, 0x204, 0x208, 0x20C, 0x210}));
    CB_CHECK_EQ(f.report.kinds.at(TableKind::Byte), 1u);
});

CB_TEST("recompile.switches.unguarded_table_ends_at_the_function_bounds", [] {
    // no bound check: the entries run while they are word-aligned addresses inside the function; an entry equal to
    // the function's end is the compiler's slot for the unreachable default and is kept
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2), lwzx(10, 12, 0), mtctr(10), kBctrWord});
    const std::uint32_t entries[] = {0x200, 0x210, 0x220, 0x240};
    for (int i = 0; i < 4; ++i) m.u32(kTable + 4 * i, kBase + entries[i]);
    const Found f = find(image_of(m, kBase + 0x100, end, {{kBase + 0x100, kBase + 0x240}}));
    CB_CHECK_EQ(f.tables.size(), 1u);
    const SwitchTable& t = f.tables.at(0);
    CB_CHECK(!t.guard.has_value());
    CB_CHECK_EQ(t.table, kTable);
    CB_CHECK(!t.inline_table);
    CB_CHECK_EQ(t.index_reg, 11u);
    CB_CHECK(t.labels == at({0x200, 0x210, 0x220, 0x240}));
    CB_CHECK(t.function.has_value() && *t.function == Extent{kBase + 0x100, kBase + 0x240});
    CB_CHECK(f.report.outside.empty());
});

CB_TEST("recompile.switches.a_label_outside_its_function_is_reported", [] {
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {cmplwi(6, 11, 1), bgt(6, 0x40), lis(12, 0x8200), addi(12, 12, 0x1000), slwi(0, 11, 2),
                                                      lwzx(10, 12, 0), mtctr(10), kBctrWord});
    m.u32(kTable, kBase + 0x200);
    m.u32(kTable + 4, kBase + 0x300);
    const Found f = find(image_of(m, kBase + 0x100, end, {{kBase + 0x100, kBase + 0x240}}));
    CB_CHECK_EQ(f.tables.size(), 1u);
    CB_CHECK_EQ(f.report.outside.size(), 1u);
    CB_CHECK_EQ(f.report.outside.at(0).bctr, end - 4);
    CB_CHECK(f.report.outside.at(0).function == Extent{kBase + 0x100, kBase + 0x240});
});

CB_TEST("recompile.switches.unresolved_and_pointer_calls", [] {
    {   // the counter comes from a load the scan cannot follow
        Mem m(kBase, 0x10000);
        const std::uint32_t lwz_r10 = (32u << 26) | (10u << 21) | (3u << 16);
        const std::uint32_t end = m.code(kBase + 0x100, {cmplwi(6, 11, 3), bgt(6, 0x40), lwz_r10, mtctr(10), kBctrWord});
        const Found f = find(image_of(m, kBase + 0x100, end));
        CB_CHECK(f.tables.empty());
        CB_CHECK_EQ(f.report.unresolved.size(), 1u);
        CB_CHECK_EQ(f.report.unresolved.at(0).bctr, end - 4);
        CB_CHECK(!f.report.unresolved.at(0).why.empty());
    }
    {   // an indexed load before the bctr, no bound and no table: an indirect call through a pointer table
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {lwzx(10, 3, 4), mtctr(10), kBctrWord});
        const Found f = find(image_of(m, kBase + 0x100, end));
        CB_CHECK(f.tables.empty());
        CB_CHECK(f.report.unresolved.empty());
        CB_CHECK(f.report.pointer_calls == std::vector<std::uint32_t>{end - 4});
    }
    {   // a bctr with no load before it is neither
        Mem m(kBase, 0x10000);
        const std::uint32_t end = m.code(kBase + 0x100, {mtctr(10), kBctrWord});
        const Found f = find(image_of(m, kBase + 0x100, end));
        CB_CHECK(f.tables.empty() && f.report.unresolved.empty() && f.report.pointer_calls.empty());
    }
});

CB_TEST("recompile.switches.every_bctr_of_the_code_is_scanned_in_address_order", [] {
    Mem m(kBase, 0x10000);
    std::uint32_t end = guarded_absolute(m, kBase + 0x100);
    end = m.code(end, {nop(), nop(), kBctrWord});   // a second bctr, with the same guard and table before it
    const Found f = find(image_of(m, kBase + 0x100, end));
    CB_CHECK_EQ(f.tables.size(), 2u);
    CB_CHECK(f.tables.at(0).bctr < f.tables.at(1).bctr);
    CB_CHECK_EQ(f.report.tables, 2u);
    CB_CHECK_EQ(f.report.kinds.at(TableKind::Absolute), 2u);
});

CB_TEST("recompile.switches.code_before_the_image_reads_as_padding", [] {
    // DIVERGES: the original raised a struct error when the scan reached below the start of the image, losing the
    // whole scan; here the words there read as zero (padding), which ends every recogniser
    Mem m(kBase, 0x1000);
    m.code(kBase, {cmplwi(6, 11, 3), kBctrWord});
    const Found f = find(image_of(m, kBase, kBase + 8));
    CB_CHECK(f.tables.empty());
    CB_CHECK_EQ(f.report.unresolved.size() + f.report.pointer_calls.size(), 0u);
    // an empty image and an empty code range
    CB_CHECK(find(CodeImage{}).tables.empty());
});

CB_TEST("recompile.switches.kind_names", [] {
    CB_CHECK_EQ(kind_name(TableKind::Absolute), "absolute");
    CB_CHECK_EQ(kind_name(TableKind::Byte), "byte");
    CB_CHECK_EQ(kind_name(TableKind::Short), "short");
    CB_CHECK_EQ(kind_name(TableKind::XenonAnalyse), "XenonAnalyse");
});

CB_TEST("recompile.switches.the_switch_file_is_what_XenonRecomp_reads", [] {
    SwitchTable guarded;
    guarded.bctr = 0x8200011C;
    guarded.index_reg = 11;
    guarded.kind = TableKind::Absolute;
    guarded.labels = at({0x200, 0x210});
    guarded.default_target = 0x82000144;
    guarded.guard = 0x82000100;
    SwitchTable inl;
    inl.bctr = 0x82000300;
    inl.index_reg = 5;
    inl.kind = TableKind::Short;
    inl.labels = at({0x400});
    inl.table = 0x82000304;
    inl.inline_table = true;
    SwitchTable far;
    far.bctr = 0x82000500;
    far.index_reg = 9;
    far.kind = TableKind::Byte;
    far.labels = at({0x600, 0x604});
    far.table = 0x82001000;
    // a table XenonAnalyse found, added by the project code: the guard it names is 0
    SwitchTable analysed;
    analysed.bctr = 0x82000700;
    analysed.index_reg = 3;
    analysed.kind = TableKind::XenonAnalyse;
    analysed.labels = at({0x800});
    analysed.default_target = 0x82000900;
    analysed.guard = 0;
    const std::vector<SwitchTable> tables = {guarded, inl, far, analysed};
    const std::string expected =
        "# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)\n"
        "\n"
        "# absolute table, guard at 0x82000100\n"
        "[[switch]]\n"
        "base = 0x8200011C\n"
        "r = 11\n"
        "default = 0x82000144\n"
        "labels = [\n"
        "    0x82000200,\n"
        "    0x82000210,\n"
        "]\n"
        "\n"
        "# short table inline after the bctr, no bounds check (length from its entries)\n"
        "[[switch]]\n"
        "base = 0x82000300\n"
        "r = 5\n"
        "default = 0x0\n"
        "labels = [\n"
        "    0x82000400,\n"
        "]\n"
        "\n"
        "# byte table at 0x82001000, no bounds check (length from its entries)\n"
        "[[switch]]\n"
        "base = 0x82000500\n"
        "r = 9\n"
        "default = 0x0\n"
        "labels = [\n"
        "    0x82000600,\n"
        "    0x82000604,\n"
        "]\n"
        "\n"
        "# XenonAnalyse table, guard at 0x0\n"
        "[[switch]]\n"
        "base = 0x82000700\n"
        "r = 3\n"
        "default = 0x82000900\n"
        "labels = [\n"
        "    0x82000800,\n"
        "]\n"
        "\n";
    CB_CHECK_EQ(format_file(tables), expected);
    CB_CHECK_EQ(format_file({}), std::string("# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)\n\n"));

    cb::fs::ScopedTempDir dir("cb-switches");
    write(dir.path() / "out" / "switches.toml", tables);
    CB_CHECK_EQ(cb::fs::read_text(dir.path() / "out" / "switches.toml"), expected);
});

CB_TEST("recompile.switches.real_tables_of_minecraft_match_the_original", [] {
    const std::vector<Sample> samples = load_samples();
    CB_CHECK(samples.size() >= 15);
    std::size_t tables = 0, unresolved = 0, pointers = 0;
    for (const Sample& s : samples) {
        CodeImage image;
        image.base = s.span().first;
        image.data = s.flat();
        image.code_begin = s.code_lo;
        image.code_end = s.code_hi;
        for (const auto& [b, e] : s.funcs) image.functions.push_back({b, e});
        const Found f = find(image);
        CB_CHECK_EQ(f.tables.size(), s.tables.size());
        for (std::size_t i = 0; i < f.tables.size() && i < s.tables.size(); ++i) {
            const SwitchTable& got = f.tables[i];
            const ExpectedTable& want = s.tables[i];
            CB_CHECK_EQ(got.bctr, want.bctr);
            CB_CHECK_EQ(got.index_reg, want.r);
            CB_CHECK_EQ(std::string(kind_name(got.kind)), want.kind);
            CB_CHECK(got.labels == want.labels);
            CB_CHECK_EQ(got.guard.has_value(), want.guard.has_value());
            if (want.guard) {
                CB_CHECK_EQ(got.guard.value_or(0), *want.guard);
                CB_CHECK_EQ(got.default_target, want.default_target);
            } else {
                CB_CHECK_EQ(got.table, want.table);
                CB_CHECK_EQ(got.inline_table, want.inline_table);
            }
            CB_CHECK_EQ(got.function.has_value(), want.func.has_value());
            if (got.function && want.func) CB_CHECK(*got.function == Extent{want.func->first, want.func->second});
            ++tables;
        }
        std::vector<std::uint32_t> got_unresolved, got_outside;
        for (const Unresolved& u : f.report.unresolved) got_unresolved.push_back(u.bctr);
        for (const Outside& o : f.report.outside) got_outside.push_back(o.bctr);
        CB_CHECK(got_unresolved == s.unresolved);
        CB_CHECK(got_outside == s.outside);
        CB_CHECK(f.report.pointer_calls == s.pointers);
        unresolved += got_unresolved.size();
        pointers += f.report.pointer_calls.size();
    }
    // the samples hold every kind (guarded absolute/byte/short, unguarded, inline), unresolved bctrs and pointer calls
    CB_CHECK(tables >= 15);
    CB_CHECK(unresolved >= 3);
    CB_CHECK(pointers >= 2);
});
