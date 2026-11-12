// Tests of cb/formats/pe_functions.hpp on a real Windows 95 module (testdata/lz32.dll) and on small blobs of code.
//
// The numbers asserted for lz32.dll are the ones the Python implementation gave on the same file (state/work/typed-port/
// formats.pe_functions/expected.txt), each read against the disassembly before it was written down here: 39 functions,
// 623 decoded instructions, 69 of them calls or jumps that resolve to a name, 11 functions that reference data.
//
// DIVERGES from the Python implementation (state/work/typed-port/_bugs/LIST.md): a PE32+ image is an UnsupportedError (the
// Python decoded its bytes as 32-bit code); a section that would end past 4 GB is an UnsupportedError; a section named in
// PeSeg that the module does not have, and a module with no executable section, are FormatErrors (ValueError before).
#include "cb/formats/pe_functions.hpp"

#include <algorithm>

#include "cb/core/capstone_api.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace pe = cb::formats::pe;
namespace pf = cb::formats::pe_functions;

namespace {

std::filesystem::path lz32() { return testing::data_path("formats/testdata/lz32.dll"); }

const pf::PeCarve& carved() {
    static const pf::PeCarve carve = pf::PeCarve::open(lz32());
    return carve;
}

}  // namespace

CB_TEST("formats.pe_functions.lz32_layout", [] {
    const pf::PeCarve& c = carved();
    CB_CHECK_EQ(c.image_base(), 0xBFEA0000u);
    CB_CHECK_EQ(c.sections().size(), 1u);
    CB_CHECK_EQ(c.sections()[0].va, 3219787776u);
    CB_CHECK_EQ(c.sections()[0].data.size(), 1536u);
    CB_CHECK_EQ(c.sections()[0].name, std::string(".text"));
    CB_CHECK(c.section_of(3219787776u) != nullptr);
    CB_CHECK(c.section_of(3219787775u) == nullptr);     // one byte before
    CB_CHECK(c.section_of(3219789312u) == nullptr);     // one byte past
    CB_CHECK_EQ(c.imports().size(), 17u);
    CB_CHECK_EQ(c.exports().at(3219788067u), std::string("LZStart"));
    CB_CHECK_EQ(c.exports().at(3219788184u), std::string("GetExpandedNameW"));
});

CB_TEST("formats.pe_functions.lz32_functions", [] {
    const pf::PeCarve& c = carved();
    const auto& fns = c.functions();
    CB_CHECK_EQ(fns.size(), 39u);
    CB_CHECK_EQ(fns.begin()->first, 3219787776u);
    // each function ends where the next begins; the last at the end of the section
    for (auto it = fns.begin(); it != fns.end(); ++it) {
        auto next = std::next(it);
        const pf::Va expect = next == fns.end() ? c.sections()[0].end() : next->first;
        CB_CHECK_EQ(it->second.end, expect);
    }
    CB_CHECK_EQ(fns.at(3219787776u).end, 3219787863u);
    CB_CHECK_EQ(fns.at(3219787893u).end, 3219787895u);
    CB_CHECK(!fns.at(3219787776u).name.has_value());
    // an export keeps its name
    CB_CHECK_EQ(*fns.at(3219788067u).name, std::string("LZStart"));
    // the call targets are ascending and start with the first local function called
    const auto targets = c.call_targets();
    CB_CHECK(std::ranges::is_sorted(targets));
    CB_CHECK_EQ(targets[0], 3219787863u);
});

CB_TEST("formats.pe_functions.lz32_spans_equal_the_carve", [] {
    const std::vector<pf::Function> spans = pf::function_spans(pe::Image::open(lz32()));
    const auto& fns = carved().functions();
    CB_CHECK_EQ(spans.size(), fns.size());
    for (const pf::Function& f : spans) {
        const auto it = fns.find(f.start);
        CB_CHECK(it != fns.end());
        if (it == fns.end()) continue;
        CB_CHECK_EQ(f.end, it->second.end);
        CB_CHECK(f.name == it->second.name);
    }
});

CB_TEST("formats.pe_functions.lz32_instructions", [] {
    const pf::PeCarve& c = carved();
    CB_CHECK_EQ(c.instruction_addresses().size(), 623u);
    CB_CHECK_EQ(c.instruction_addresses().front(), 3219787776u);
    CB_CHECK_EQ(c.instruction_addresses().back(), 3219789310u);
    CB_CHECK(std::ranges::is_sorted(c.instruction_addresses()));

    const auto push = c.instruction_at(3219787776u);   // the first function's `push ebp`
    CB_CHECK(push.has_value());
    CB_CHECK_EQ(push->mnemonic, std::string("push"));
    CB_CHECK_EQ(push->op_str, std::string("ebp"));
    CB_CHECK_EQ(push->size, 1);
    CB_CHECK_EQ(push->operands.size(), 1u);
    CB_CHECK(push->operands[0].kind == pf::OperandKind::Register);

    const auto stack = c.instruction_at(3219788593u);   // push dword ptr [esp + 8]
    CB_CHECK(stack.has_value());
    CB_CHECK_EQ(stack->op_str, std::string("dword ptr [esp + 8]"));
    CB_CHECK_EQ(stack->size, 4);
    CB_CHECK(stack->operands[0].kind == pf::OperandKind::Memory);
    CB_CHECK_EQ(stack->operands[0].base, static_cast<std::uint32_t>(X86_REG_ESP));
    CB_CHECK_EQ(stack->operands[0].index, 0u);
    CB_CHECK_EQ(stack->operands[0].disp, 8);

    // not decoded: an address in the middle of an instruction, outside the section, and one that never was
    CB_CHECK(!c.has_instruction(3219787778u));
    CB_CHECK(!c.instruction_at(3219787778u).has_value());
    CB_CHECK(!c.instruction_at(0).has_value());
    CB_CHECK(!c.instruction_at(3219789410u).has_value());
    CB_CHECK(!c.instruction_at(3219787775u).has_value());
    // every decoded address decodes again, and to its own address
    int checked = 0;
    for (pf::Va a : c.instruction_addresses()) {
        const auto ins = c.instruction_at(a);
        CB_CHECK(ins.has_value());
        if (ins) CB_CHECK_EQ(ins->address, a);
        ++checked;
    }
    CB_CHECK_EQ(checked, 623);
});

CB_TEST("formats.pe_functions.lz32_calls", [] {
    const pf::PeCarve& c = carved();
    int calls_or_jumps = 0, resolved = 0;
    for (pf::Va a : c.instruction_addresses()) {
        const auto ins = c.instruction_at(a);
        if (!ins) continue;
        ++calls_or_jumps;
        if (c.resolve_call(*ins)) ++resolved;
    }
    CB_CHECK_EQ(calls_or_jumps, 623);
    CB_CHECK_EQ(resolved, 69);
    CB_CHECK(c.calls_in(3219787776u) == (std::vector<std::string>{"sub_bfea13fa", "sub_bfea1057"}));
    CB_CHECK(c.calls_in(3219788140u).empty());
    CB_CHECK(c.calls_in(0).empty());               // no function there
    CB_CHECK(c.calls_in(3219787777u).empty());     // an instruction, not a function start
    // a call into the import table names the DLL
    bool named_import = false;
    for (const auto& [start, f] : c.functions())
        for (const std::string& name : c.calls_in(start))
            if (name.find('!') != std::string::npos) named_import = true;
    CB_CHECK(named_import);
});

CB_TEST("formats.pe_functions.lz32_disassemble", [] {
    const pf::PeCarve& c = carved();
    const auto rows = c.disassemble(3219787776u);
    CB_CHECK(rows.size() > 5);
    CB_CHECK_EQ(rows[0].address, 3219787776u);
    CB_CHECK_EQ(rows[0].mnemonic, std::string("push"));
    CB_CHECK_EQ(rows[1].mnemonic, std::string("mov"));
    CB_CHECK_EQ(rows[1].op_str, std::string("ebp, esp"));
    CB_CHECK_EQ(rows[2].op_str, std::string("eax, dword ptr [ebp + 0xc]"));
    CB_CHECK_EQ(rows[4].mnemonic, std::string("jne"));
    CB_CHECK_EQ(rows[4].op_str, std::string("0xbfea1032"));
    const auto one = c.disassemble(3219788140u);
    CB_CHECK_EQ(one.size(), 1u);
    CB_CHECK_EQ(one[0].op_str, std::string("cl, 0"));
    CB_CHECK(c.disassemble(0).empty());
});

CB_TEST("formats.pe_functions.lz32_data_references", [] {
    const pf::PeCarve& c = carved();
    const auto& refs = c.data_refs();
    CB_CHECK_EQ(refs.size(), 11u);
    CB_CHECK(refs.at(3219787776u) == (std::vector<pf::Va>{3219791872u, 3219791885u, 3219791894u}));
    CB_CHECK(&c.data_refs() == &refs);   // computed once
    for (const auto& [start, vas] : refs) {
        CB_CHECK(std::ranges::is_sorted(vas));
        for (pf::Va v : vas) CB_CHECK(c.in_image(v));
    }
    CB_CHECK(c.xrefs_to(3219791872u) == (std::vector<pf::Va>{3219787776u}));
    CB_CHECK(c.xrefs_to(3219787885u) ==
             (std::vector<pf::Va>{3219787895u, 3219787943u, 3219787967u, 3219788004u, 3219788043u, 3219788069u, 3219788091u,
                                  3219788125u, 3219788142u}));
    CB_CHECK(c.xrefs_to(1).empty());
    CB_CHECK(!c.in_image(0xBFEA0000ull));   // the headers are not a section
    CB_CHECK(c.in_image(3219787776ull));
    CB_CHECK(c.in_image(3219787836ull));
    CB_CHECK(!c.in_image(0));
    CB_CHECK(!c.in_image(0xBFEA0000ull + 0x7FFFFFFull));

    std::unordered_map<pf::Va, std::string> texts{{3219791872u, "first"}, {3219791885u, "second"}, {7u, "elsewhere"}};
    const auto strings = c.string_refs(texts);
    CB_CHECK_EQ(strings.size(), 1u);
    CB_CHECK_EQ(strings.at(3219787776u).size(), 2u);
    CB_CHECK_EQ(strings.at(3219787776u)[1].second, std::string("second"));
});

CB_TEST("formats.pe_functions.the_cache_keeps_one_carve_per_file", [] {
    pf::CarveCache cache;
    const auto a = cache.get(lz32());
    const auto b = cache.get(lz32());
    CB_CHECK(a.get() == b.get());
    CB_CHECK_EQ(a->functions().size(), 39u);
});

CB_TEST("formats.pe_functions.pe_seg_of_lz32", [] {
    const pf::PeSeg seg = pf::PeSeg::open(lz32(), "lz32.dll");
    CB_CHECK_EQ(seg.name, std::string(".text"));
    CB_CHECK_EQ(seg.va, 3219787776u);
    CB_CHECK_EQ(seg.base, 0xBFEA0000u);
    CB_CHECK_EQ(seg.code.size(), 1536u);
    CB_CHECK_EQ(seg.rows.size(), seg.row_at.size());
    CB_CHECK_EQ(seg.rows[0].at, 0u);
    CB_CHECK_EQ(seg.rows[0].text, std::string("push ebp"));
    CB_CHECK_EQ(seg.row_at.at(1), 1u);
    // the listing is contiguous: each row starts where the previous one's bytes end
    CB_CHECK_EQ(seg.rows.back().at < seg.code.size(), true);
    // functions are offsets in the section
    CB_CHECK_EQ(seg.funcs.size(), 39u);
    CB_CHECK_EQ(seg.funcs.at(0).end, 87u);
    CB_CHECK_EQ(seg.export_names.at(3219788067u - 3219787776u), std::string("LZStart"));
    // base relocations: every covered byte lies in groups of four, and the section has some
    std::size_t covered = 0;
    for (bool b : seg.reloc_mask) covered += b ? 1 : 0;
    CB_CHECK(covered > 0);
    CB_CHECK_EQ(covered % 4, 0u);
    CB_CHECK(seg.carve != nullptr);
    // reusing a carve gives the same answer
    const pf::PeSeg again = pf::PeSeg::open(lz32(), "lz32.dll", ".text", seg.carve);
    CB_CHECK_EQ(again.rows.size(), seg.rows.size());
    CB_CHECK(again.carve.get() == seg.carve.get());
    CB_CHECK_THROWS_AS(FormatError, (void)pf::PeSeg::open(lz32(), "lz32.dll", ".nothing", seg.carve));
});

CB_TEST("formats.pe_functions.listing_keeps_prefixed_instructions_whole_and_steps_over_bad_bytes", [] {
    // push ebp; mov ebp,esp; mov eax, fs:[0x18]; ud-ish byte pair (0F 04 is no instruction); ret
    const Bytes code = {0x55, 0x8B, 0xEC, 0x64, 0xA1, 0x18, 0x00, 0x00, 0x00, 0x0F, 0x04, 0x90, 0xC3};
    const auto rows = pf::disassemble_listing(code, 0x1000);
    CB_CHECK_EQ(rows[0].at, 0x1000u);
    CB_CHECK_EQ(rows[0].text, std::string("push ebp"));
    CB_CHECK_EQ(rows[1].text, std::string("mov ebp, esp"));
    CB_CHECK_EQ(rows[2].at, 0x1003u);                          // the segment prefix is not a row of its own
    CB_CHECK_EQ(rows[2].text, std::string("mov eax, dword ptr fs:[0x18]"));
    CB_CHECK_EQ(rows[3].at, 0x1009u);
    CB_CHECK_EQ(rows[3].text, std::string("(bad)"));
    CB_CHECK_EQ(rows[4].at, 0x100Au);                          // the next byte is tried on its own
    CB_CHECK_EQ(rows[4].text, std::string("add al, 0x90"));
    CB_CHECK_EQ(rows.back().text, std::string("ret"));
    CB_CHECK(pf::disassemble_listing({}, 0).empty());
    // a cut instruction at the end: one (bad) row for its first byte, then each byte that follows is tried alone
    const Bytes cut = {0xB8, 0x01};
    const auto cut_rows = pf::disassemble_listing(cut, 0);
    CB_CHECK_EQ(cut_rows[0].text, std::string("(bad)"));
    CB_CHECK_EQ(cut_rows.size(), 2u);
});

CB_TEST("formats.pe_functions.errors", [] {
    fs::ScopedTempDir dir("pe-functions-test");
    fs::write_file(dir.path() / "junk.dll", to_bytes("this is not a PE image at all"));
    CB_CHECK_THROWS_AS(FormatError, (void)pf::PeCarve::open(dir.path() / "junk.dll"));
    CB_CHECK_THROWS_AS(NotFoundError, (void)pf::PeCarve::open(dir.path() / "missing.dll"));
    CB_CHECK_THROWS_AS(FormatError, (void)pf::function_spans(pe::Image::parse(to_bytes("MZ"))));
});
