#include "cb/recompile/powerpc.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <format>
#include <map>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/recompile/test_ppc.hpp"

using namespace cb;
namespace ppcgen = cb::recompile::powerpc;

// DIVERGES from the Python: an instruction whose operands the module cannot read is a PPC_UNIMPLEMENTED trap (the Python raised out
// of recompile_function, or wrote "None" into a statement it then trapped); the hand-written handlers the builders shadow (add, lwz,
// fmr ...) are gone; --all writes the mapping table beside the file by replacing its extension, not every ".cpp" of the path.

namespace {
namespace pptest {

constexpr std::uint32_t kSliceVa = 0x822B6E00;     // where the 16 KB of Minecraft code in ppc_slice.bin loads
constexpr std::uint32_t kProgramVa = 0x82100000;   // where the made program of config_code.bin loads

std::string slurp(std::string_view name) { return fs::read_text(testing::data_path("recompile/testdata/" + std::string(name))); }

// "[name]" sections of lines; the lines before the first section are kept under "".
std::map<std::string, std::vector<std::string>> sections_of(const std::string& body) {   // (lookup by name only)
    std::map<std::string, std::vector<std::string>> out;
    std::vector<std::string>* current = &out[""];
    for (const std::string_view line : text::lines(body)) {
        if (line.starts_with('[') && line.ends_with(']')) {
            current = &out[std::string(line.substr(1, line.size() - 2))];
        } else {
            current->emplace_back(line);
        }
    }
    return out;
}

// The text a section of "| "-led lines holds.
std::string bar_text(const std::vector<std::string>& lines) {
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i) out += '\n';
        out += lines[i].size() >= 2 ? lines[i].substr(2) : std::string();
    }
    return out;
}

std::uint32_t hex32(std::string_view s) { return *text::parse_int<std::uint32_t>(s, 16); }

// The Xenon's vector words of the slice, as the reference decoded them (ppc_slice.listing.txt); Capstone reads the rest.
recompile::disassemble::VectorDecoder slice_vectors() {
    std::map<std::uint32_t, std::pair<std::string, std::string>> table;   // lookup only
    const std::string body = slurp("ppc_slice.listing.txt");
    for (const std::string_view line : text::lines(body)) {
        if (line.empty()) continue;
        const auto parts = text::split_n(line, ' ', 3);
        if (parts[2] != "V") continue;
        const std::size_t bar = parts[3].find('|');
        table[hex32(parts[1])] = {std::string(parts[3].substr(0, bar)), std::string(parts[3].substr(bar + 1))};
    }
    return [table](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
        const auto it = table.find(word);
        if (it == table.end()) return std::nullopt;
        return it->second;
    };
}

recompile::disassemble::Disassembler& slice_dis() {
    static recompile::disassemble::Disassembler dis(slice_vectors());
    return dis;
}

recompile::disassemble::Disassembler& plain_dis() {
    static recompile::disassemble::Disassembler dis;
    return dis;
}

// The made program of config_code.bin as the recompiler reads an image.
ppcgen::LoadedImage program_image() {
    ppcgen::LoadedImage loaded;
    const Bytes code = fs::read_file(testing::data_path("recompile/testdata/config_code.bin"));
    const std::uint32_t end = kProgramVa + static_cast<std::uint32_t>(code.size());
    recompile::discover::Image& image = loaded.image;
    image.code = code;
    image.code_begin = kProgramVa;
    image.code_end = end;
    image.entry = kProgramVa;
    image.extents = {{kProgramVa, 0x40}, {kProgramVa + 0x40, 0x20}, {kProgramVa + 0x430, 0x40}, {kProgramVa + 0x4C0, 0x20}, {kProgramVa + 0x600, 0x20}};
    image.imports = {kProgramVa + 0x700};
    image.sections.push_back({".text", kProgramVa, 0x60000020, code});
    recompile::disassemble::ImageFacts& facts = loaded.facts;
    facts.data = code;
    facts.base = kProgramVa;
    facts.entry = kProgramVa;
    facts.code_begin = kProgramVa;
    facts.code_end = end;
    facts.chunks.push_back({kProgramVa, 0, static_cast<std::uint32_t>(code.size())});
    facts.sections.push_back({".text", kProgramVa, static_cast<std::uint32_t>(code.size()), 0x60000020});
    return loaded;
}

ppcgen::Program program() {
    const ppcgen::LoadedImage loaded = program_image();
    return {loaded.image, recompile::disassemble::flatten(loaded.facts)};
}

std::string aggregate_line(const ppcgen::Aggregate& a) {
    return std::format("agg funcs={} total={} translated={} unimpl={} discovered={} tail={} lo={:08X} hi={:08X}", a.funcs, a.total, a.translated,
                       a.unimplemented, a.discovered, a.tail_entries, a.lo, a.hi);
}

std::string hex_list(std::string_view name, const std::vector<std::uint32_t>& v) {
    std::string out = std::string(name) + " ";   // (the reference wrote "name " and the addresses with a blank between)
    for (std::size_t i = 0; i < v.size(); ++i) out += std::format("{}{:08X}", i ? " " : "", v[i]);
    return out;
}

}  // namespace pptest

using namespace pptest;

// ---- one instruction -------------------------------------------------------------------------------------------------------

CB_TEST("recompile.powerpc.every_instruction_as_the_reference_makes_it", [] {
    const std::string body = slurp("powerpc_insns.golden.txt");
    ppcgen::FunctionBounds bounds;
    ppcgen::JumpTables tables;
    recompile::disassemble::Row row;
    std::vector<std::string> expected;
    bool have_row = false;
    std::size_t rows = 0, traps = 0;
    const auto check = [&] {
        if (!have_row) return;
        ++rows;
        const std::vector<std::string> got = ppcgen::emit_insn(row, bounds, tables);
        if (expected.size() == 1 && expected[0] == "!EXC") {
            // DIVERGES: the Python raised; an instruction whose operands cannot be read is a trap here
            ++traps;
            std::string_view stem = row.mnemonic;   // (a record form's trap names its base operation)
            while (stem.ends_with('.')) stem.remove_suffix(1);
            const bool same = got.size() == 1 && got[0].starts_with(std::format("PPC_UNIMPLEMENTED(\"{}", stem));
            if (!same) std::fprintf(stderr, "EXC row %s | %s -> %s\n", row.mnemonic.c_str(), row.operands.c_str(), got.empty() ? "" : got[0].c_str());
            CB_CHECK(same);
        } else {
            if (got != expected) std::fprintf(stderr, "ROW %s | %s -> %s (want %s)\n", row.mnemonic.c_str(), row.operands.c_str(), got.empty() ? "" : got[0].c_str(), expected.empty() ? "" : expected[0].c_str());
            CB_CHECK(got == expected);
        }
    };
    for (const std::string_view line : text::lines(body)) {
        if (line.starts_with('#')) continue;
        if (line.starts_with("lo ")) {
            bounds.lo = hex32(line.substr(3));
        } else if (line.starts_with("hi ")) {
            bounds.hi = hex32(line.substr(3));
        } else if (line.starts_with("table ")) {
            const auto w = text::split_ws(line);
            recompile::jumptable::JumpTable t;
            t.index_reg = *text::parse_int<std::uint32_t>(w[2], 10);
            for (std::size_t i = 3; i < w.size(); ++i) t.targets.push_back(hex32(w[i]));
            tables[hex32(w[1])] = t;
        } else if (line.starts_with("@ ")) {
            check();
            const auto head = text::split_n(line.substr(2), ' ', 2);   // va, raw, "mn | ops"
            const std::size_t bar = head[2].find(" | ");
            row = {};
            row.address = hex32(head[0]);
            row.word = hex32(head[1]);
            row.mnemonic = std::string(head[2].substr(0, bar));
            row.operands = std::string(head[2].substr(bar + 3));
            expected.clear();
            have_row = true;
        } else if (line.starts_with("> ")) {
            expected.emplace_back(line.substr(2));
        }
    }
    check();
    CB_CHECK_EQ(rows, std::size_t{236});
    CB_CHECK_EQ(traps, std::size_t{11});
});

CB_TEST("recompile.powerpc.a_trap_names_the_instruction", [] {
    recompile::disassemble::Row row;
    row.address = kProgramVa;
    row.mnemonic = "frobnicate";
    row.operands = "r3, r4";
    CB_CHECK((ppcgen::emit_insn(row, {kProgramVa, kProgramVa + 16})) == (std::vector<std::string>{"PPC_UNIMPLEMENTED(\"frobnicate r3, r4\");"}));
    row.mnemonic = ".long";
    row.operands = "0x00000000";
    CB_CHECK((ppcgen::emit_insn(row, {kProgramVa, kProgramVa + 16})) == (std::vector<std::string>{"PPC_UNIMPLEMENTED(\".long 0x00000000\");"}));
});

CB_TEST("recompile.powerpc.a_branch_is_a_goto_inside_the_function_and_a_tail_call_outside", [] {
    recompile::disassemble::Row row;
    row.address = kProgramVa + 4;
    row.mnemonic = "bne";
    row.operands = "cr6, 0x82100010";
    const ppcgen::FunctionBounds bounds{kProgramVa, kProgramVa + 0x20};
    CB_CHECK((ppcgen::emit_insn(row, bounds)) == (std::vector<std::string>{"if (!ctx.cr6.eq) { goto loc_82100010; }"}));
    row.operands = "cr6, 0x82100020";   // (the end of the function is outside it)
    CB_CHECK((ppcgen::emit_insn(row, bounds)) == (std::vector<std::string>{"if (!ctx.cr6.eq) { sub_82100020(ctx, base); return; }"}));
    row.operands = "cr6, 0x820FFFFC";
    CB_CHECK((ppcgen::emit_insn(row, bounds)) == (std::vector<std::string>{"if (!ctx.cr6.eq) { sub_820FFFFC(ctx, base); return; }"}));
    // a link register set to the return address
    row.mnemonic = "bl";
    row.operands = "0x82300000";
    CB_CHECK((ppcgen::emit_insn(row, bounds)) == ((std::vector<std::string>{"ctx.lr = 0x82100008;", "sub_82300000(ctx, base);"})));
});

// ---- one function --------------------------------------------------------------------------------------------------------------

CB_TEST("recompile.powerpc.function_edges", [] {
    // no code: the frame of a function and nothing in it
    const ppcgen::Recompiled none = ppcgen::recompile_function(plain_dis(), ByteView(), kProgramVa, 0);
    CB_CHECK_EQ(none.cpp, "PPC_FUNC_IMPL(sub_82100000) {\n\tPPC_FUNC_PROLOGUE();\n\tuint32_t ea{};\n\tPPCRegister temp{};\n}\n");
    CB_CHECK_EQ(none.stats.total, std::size_t{0});
    CB_CHECK(none.called.empty());
    // a size that is not a whole number of words is read down to one; a body shorter than its size gives what it has
    const Bytes blr_and_a_half = {0x4E, 0x80, 0x00, 0x20, 0x4E, 0x80};
    const ppcgen::Recompiled one = ppcgen::recompile_function(plain_dis(), blr_and_a_half, kProgramVa, 6);
    CB_CHECK_EQ(one.stats.total, std::size_t{1});
    CB_CHECK(one.cpp.find("\treturn;\n") != std::string::npos);
    const ppcgen::Recompiled shorter = ppcgen::recompile_function(plain_dis(), blr_and_a_half, kProgramVa, 64);
    CB_CHECK_EQ(shorter.stats.total, std::size_t{1});
});

CB_TEST("recompile.powerpc.a_function_with_its_calls_labels_and_stats", [] {
    const Bytes code = fs::read_file(testing::data_path("recompile/testdata/config_code.bin"));
    // F1 of the made program: calls X, tail-jumps to Y, conditionally to Z, and has a branch inside itself
    const ppcgen::Recompiled f1 = ppcgen::recompile_function(plain_dis(), ByteView(code).subspan(0, 0x40), kProgramVa, 0x40);
    CB_CHECK(f1.called == (std::set<std::uint32_t>{kProgramVa + 0x100, kProgramVa + 0x180, kProgramVa + 0x1C0}));
    CB_CHECK_EQ(f1.stats.total, std::size_t{16});
    CB_CHECK_EQ(f1.stats.labels, std::size_t{1});
    CB_CHECK(f1.cpp.find("loc_82100030:\n") != std::string::npos);
    CB_CHECK_EQ(f1.stats.translated + f1.stats.unimplemented, f1.stats.total);
    // the forward declarations of what it calls, in address order
    CB_CHECK_EQ(ppcgen::forward_declarations(f1.called), "extern PPC_FUNC(sub_82100100);\nextern PPC_FUNC(sub_82100180);\nextern PPC_FUNC(sub_821001C0);");
    CB_CHECK_EQ(ppcgen::forward_declarations({}), "");
});

CB_TEST("recompile.powerpc.the_slice_of_real_code_as_the_reference_makes_it", [] {
    const Bytes code = fs::read_file(testing::data_path("recompile/testdata/ppc_slice.bin"));
    const auto sections = sections_of(slurp("powerpc_slice.golden.txt"));
    const std::uint32_t hi = kSliceVa + static_cast<std::uint32_t>(code.size());
    const ppcgen::GuestImage image{code, kSliceVa, [hi](std::uint32_t a) { return (a & 3) == 0 && kSliceVa <= a && a < hi; }};

    // the whole slice as one function, with the image (switch tables recovered) and without
    const ppcgen::Recompiled whole = ppcgen::recompile_function(slice_dis(), code, kSliceVa, static_cast<std::uint32_t>(code.size()), &image);
    const auto& w = sections.at("whole_slice");
    CB_CHECK_EQ("sha1 " + crypto::to_hex(crypto::sha1(as_bytes(whole.cpp))), w[0]);
    CB_CHECK_EQ(std::format("lines {}", std::ranges::count(whole.cpp, '\n')), w[1]);
    CB_CHECK_EQ(std::format("stats total={} unimpl={} translated={} labels={}", whole.stats.total, whole.stats.unimplemented, whole.stats.translated, whole.stats.labels), w[2]);
    CB_CHECK_EQ(std::format("called {} first={:08X} last={:08X}", whole.called.size(), *whole.called.begin(), *whole.called.rbegin()), w[3]);

    const ppcgen::Recompiled no_image = ppcgen::recompile_function(slice_dis(), code, kSliceVa, static_cast<std::uint32_t>(code.size()));
    const auto& n = sections.at("whole_slice_no_image");
    CB_CHECK_EQ("sha1 " + crypto::to_hex(crypto::sha1(as_bytes(no_image.cpp))), n[0]);
    CB_CHECK_EQ(std::format("stats total={} unimpl={} translated={} labels={}", no_image.stats.total, no_image.stats.unimplemented, no_image.stats.translated,
                            no_image.stats.labels), n[1]);
    CB_CHECK_EQ(whole.cpp, no_image.cpp);   // (the slice has no switch the recogniser takes: the image changes nothing)

    // what the module cannot translate, by instruction: the reference's list
    std::map<std::string, std::size_t> traps;   // lookup only
    for (const std::string_view line : text::lines(whole.cpp)) {
        if (line.find("PPC_UNIMPLEMENTED") != std::string_view::npos) ++traps[std::string(text::trim(line))];
    }
    std::map<std::string, std::size_t> want;
    for (const std::string& line : sections.at("unimplemented_in_slice")) {
        const std::size_t space = line.find(' ');
        want[line.substr(space + 1)] = *text::parse_int<std::size_t>(line.substr(0, space), 10);
    }
    CB_CHECK(traps == want);
    CB_CHECK_EQ(traps.size(), whole.stats.unimplemented == 0 ? std::size_t{0} : traps.size());

    // some whole functions, as text
    for (const auto& [name, lines] : sections) {
        if (!name.starts_with("window_")) continue;
        const std::uint32_t lo = hex32(name.substr(7, 4)), end = hex32(name.substr(12, 4));
        const ppcgen::Recompiled r = ppcgen::recompile_function(slice_dis(), ByteView(code).subspan(lo, end - lo), kSliceVa + lo, end - lo, &image);
        std::string calls = "calls";
        for (const std::uint32_t a : r.called) calls += std::format(" {:08X}", a);
        CB_CHECK_EQ(calls, lines[0]);
        CB_CHECK_EQ(std::format("stats total={} unimpl={} translated={} labels={}", r.stats.total, r.stats.unimplemented, r.stats.translated, r.stats.labels), lines[1]);
        CB_CHECK_EQ(r.cpp, bar_text(std::vector<std::string>(lines.begin() + 2, lines.end())) + "\n");
    }
});

// A real switch: `cmplwi / bgt / lis / addi / slwi / lwzx / mtctr / bctr` over a table of four cases in the image.
CB_TEST("recompile.powerpc.a_switch_is_a_cxx_switch", [] {
    constexpr std::uint32_t kBase = 0x82000000;
    recompile::test::Mem m(kBase, 0x10000);
    using namespace recompile::test;
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40), slwi(0, 11, 2),
                                                      lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kBase + 0x1000 + 4 * i, kBase + 0x200 + 0x10 * i);
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kBase + 0x200 + 0x10 * i, blr());
    const auto is_exec = [kBase](std::uint32_t a) { return a >= kBase && a < kBase + 0x400; };
    const ppcgen::GuestImage image{m.bytes, kBase, is_exec};
    const auto make = [&](std::uint32_t size, const ppcgen::GuestImage* guest) {
        return ppcgen::recompile_function(plain_dis(), ByteView(m.bytes).subspan(0x100, size), kBase + 0x100, size, guest);
    };
    // cases outside the function are tail calls
    const ppcgen::Recompiled outside = make(end - (kBase + 0x100), &image);
    CB_CHECK(outside.cpp.find("\tswitch (ctx.r11.u32) {\n\tcase 0u: sub_82000200(ctx, base); return;\n\tcase 1u: sub_82000210(ctx, base); return;\n"
                              "\tcase 2u: sub_82000220(ctx, base); return;\n\tcase 3u: sub_82000230(ctx, base); return;\n"
                              "\tdefault: PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;\n\t}\n") != std::string::npos);
    CB_CHECK_EQ(outside.called.size(), std::size_t{5});   // (the four cases, and the default of the bounds check: a tail call out of the function)
    CB_CHECK(outside.called.contains(kBase + 0x14C));
    // cases inside it are gotos to labels
    const ppcgen::Recompiled inside = make(0x140, &image);
    CB_CHECK(inside.cpp.find("\tcase 0u: goto loc_82000200;\n") != std::string::npos);
    CB_CHECK(inside.cpp.find("loc_82000230:\n") != std::string::npos);
    CB_CHECK(inside.stats.labels >= 4);
    // with no image the bctr is the honest indirect call
    const ppcgen::Recompiled indirect = make(end - (kBase + 0x100), nullptr);
    CB_CHECK(indirect.cpp.find("switch") == std::string::npos);
    CB_CHECK(indirect.cpp.find("\tPPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;\n") != std::string::npos);
    // a table case that is not code ends the table, so the image's is_exec decides
    const ppcgen::GuestImage narrow{m.bytes, kBase, [](std::uint32_t) { return false; }};
    CB_CHECK(make(end - (kBase + 0x100), &narrow).cpp.find("switch") == std::string::npos);
});

// ---- the runtime ----------------------------------------------------------------------------------------------------------------

CB_TEST("recompile.powerpc.runtime_header", [] {
    const std::string header = ppcgen::runtime_header();
    CB_CHECK_EQ(header, slurp("runtime_header.golden.h"));
    CB_CHECK(header.find("PPCRegister r31;") != std::string::npos);
    CB_CHECK(header.find("PPCVRegister v127;") != std::string::npos);
    CB_CHECK(header.find("#define PPC_LOAD_U32(x)  (__builtin_bswap32(*(volatile uint32_t*)(base + (uint32_t)(x))))") != std::string::npos);
    CB_CHECK(header.find("ppc_vperm") != std::string::npos);
    CB_CHECK(header.find('@') == std::string::npos);   // (every slot of the text is filled)
    CB_CHECK(ppcgen::vector_helpers().find("ppc_vsel") != std::string_view::npos);
});

// ---- every function of an image -------------------------------------------------------------------------------------------------

CB_TEST("recompile.powerpc.an_image_as_the_reference_makes_it", [] {
    const auto sections = sections_of(slurp("powerpc_image.golden.txt"));
    const ppcgen::Program prog = program();
    const auto check = [&](const std::string& name, const ppcgen::ImageOptions& options) {
        const ppcgen::ImageResult made = ppcgen::recompile_image(prog, {}, options);
        const auto& head = sections.at(name);
        CB_CHECK_EQ(aggregate_line(made.agg), head[0]);
        CB_CHECK_EQ(hex_list("decls", made.agg.decls), head[1]);
        CB_CHECK_EQ(hex_list("chosen", made.agg.chosen), head[2]);
        CB_CHECK_EQ(hex_list("skipped", made.agg.skipped), head[3]);
        CB_CHECK_EQ(made.body, bar_text(sections.at(name + ".body")));
        CB_CHECK_EQ(made.mapping, bar_text(sections.at(name + ".mapping")));
        CB_CHECK_EQ(made.agg.parts.size(), made.agg.funcs);
    };
    ppcgen::ImageOptions single;
    single.jobs = 1;
    check("image_default", single);
    ppcgen::ImageOptions skip = single;
    skip.skip = {kProgramVa + 0x700, kProgramVa + 0x100};
    check("image_skip", skip);
    ppcgen::ImageOptions limit = single;
    limit.limit = 2;
    check("image_limit", limit);
    ppcgen::ImageOptions seeds = single;
    seeds.extra = {kProgramVa + 0x300, kProgramVa + 0x320, kProgramVa};
    check("image_seeds", seeds);
    // threads change nothing
    ppcgen::ImageOptions many;
    many.jobs = 4;
    check("image_default", many);
});

CB_TEST("recompile.powerpc.an_image_meaning", [] {
    const ppcgen::ImageResult made = ppcgen::recompile_image(program(), {});
    const ppcgen::Aggregate& agg = made.agg;
    const auto has = [&](const std::vector<std::uint32_t>& v, std::uint32_t a) { return std::ranges::find(v, a) != v.end(); };
    // the tail-jump targets (Y and Z, reached only by `b`) are functions too, and are counted as such
    CB_CHECK(has(agg.chosen, kProgramVa + 0x180) && has(agg.chosen, kProgramVa + 0x1C0));
    CB_CHECK_EQ(agg.tail_entries, agg.chosen.size() - agg.discovered);
    CB_CHECK(agg.lo == agg.chosen.front() && agg.hi == agg.chosen.back());
    // every function mapped is declared, and every call is declared
    for (const std::uint32_t a : agg.chosen) CB_CHECK(has(agg.decls, a));
    CB_CHECK(made.body.starts_with("#include \"rex_ppc_min.h\"\n\nPPC_FUNC(sub_82100000);\n"));
    CB_CHECK(made.mapping.ends_with("\t{ 0, nullptr },\n};\n"));
    // the import thunk is declared (a function calls it) and not recompiled when skipped; a skipped function stays mapped
    ppcgen::ImageOptions options;
    options.skip = {kProgramVa + 0x100, kProgramVa + 0x180};
    const ppcgen::ImageResult skipped = ppcgen::recompile_image(program(), {}, options);
    CB_CHECK(has(skipped.agg.skipped, kProgramVa + 0x100) && has(skipped.agg.chosen, kProgramVa + 0x100) && has(skipped.agg.decls, kProgramVa + 0x100));
    CB_CHECK(!has(skipped.agg.chosen, kProgramVa + 0x180) && has(skipped.agg.decls, kProgramVa + 0x180));   // (a skipped tail-jump target is called, so declared, but never made a function)
    CB_CHECK_EQ(skipped.agg.funcs + 2, made.agg.funcs);   // (X, and Y, the tail-jump target)
    // no functions: an empty program
    ppcgen::Program empty;
    const ppcgen::ImageResult nothing = ppcgen::recompile_image(empty, {});
    CB_CHECK_EQ(nothing.agg.funcs, std::size_t{0});
    CB_CHECK_EQ(nothing.body, "#include \"rex_ppc_min.h\"\n\n\n\n");
});

// ---- checking the output compiles --------------------------------------------------------------------------------------------------

CB_TEST("recompile.powerpc.finding_a_compiler", [] {
    fs::ScopedTempDir dir("cb-ppc-compiler");
    fs::write_text(dir.path() / "g++.exe", "not a compiler");
    fs::write_text(dir.path() / "mine.exe", "not a compiler either");
    ppcgen::CompilerEnv env;
    env.path_variable = fs::to_text(dir.path() / "nowhere") + ";" + fs::to_text(dir.path());
    CB_CHECK_EQ(fs::to_text(*ppcgen::find_compiler(env)), fs::to_text(dir.path() / "g++.exe"));
    env.gpp = dir.path() / "mine.exe";   // the setting wins when it is a file
    CB_CHECK_EQ(fs::to_text(*ppcgen::find_compiler(env)), fs::to_text(dir.path() / "mine.exe"));
    env.gpp = dir.path() / "no-such.exe";   // ... and is passed over when it is not
    CB_CHECK_EQ(fs::to_text(*ppcgen::find_compiler(env)), fs::to_text(dir.path() / "g++.exe"));
    env.path_variable = fs::to_text(dir.path() / "empty");
    CB_CHECK(!ppcgen::find_compiler(env).has_value());
    const ppcgen::CompileCheck check = ppcgen::check_compiles("int x;", env);
    CB_CHECK(!check.compiles.has_value());
    CB_CHECK(check.message.find("no C++ compiler") != std::string::npos);
});

CB_TEST("recompile.powerpc.kernel_includes", [] {
    fs::ScopedTempDir dir("cb-ppc-kernel");
    CB_CHECK(ppcgen::kernel_includes(dir.path()).empty());   // (not in the tree)
    std::filesystem::create_directories(dir.path() / "native" / "rexglue" / "kernel");
    const auto includes = ppcgen::kernel_includes(dir.path());
    CB_CHECK_EQ(includes.size(), std::size_t{8});
    CB_CHECK_EQ(fs::to_text(includes.front()), fs::to_text(dir.path() / "native" / "rexglue" / "kernel" / "shim"));
    CB_CHECK_EQ(fs::to_text(includes[2]).substr(fs::to_text(includes[2]).size() - 22), "thirdparty/fmt/include");
});

// Timing, not a check: set CB_BENCH to see what recompiling the 16 KB slice costs (docs/TYPED_CPP_SPEED.md).
CB_TEST("recompile.powerpc.bench", [] {
    if (!fs::get_env("CB_BENCH")) return;
    const Bytes code = fs::read_file(testing::data_path("recompile/testdata/ppc_slice.bin"));
    const std::uint32_t hi = kSliceVa + static_cast<std::uint32_t>(code.size());
    const ppcgen::GuestImage image{code, kSliceVa, [hi](std::uint32_t a) { return (a & 3) == 0 && kSliceVa <= a && a < hi; }};
    constexpr int kRounds = 20;
    const auto t0 = std::chrono::steady_clock::now();
    std::size_t total = 0;
    for (int i = 0; i < kRounds; ++i) total += ppcgen::recompile_function(slice_dis(), code, kSliceVa, static_cast<std::uint32_t>(code.size()), &image).stats.total;
    const auto t1 = std::chrono::steady_clock::now();
    std::fprintf(stderr, "powerpc bench: %zu instructions per round, %.2f ms per round (%d rounds)" "\n", total / kRounds,
                 std::chrono::duration<double, std::milli>(t1 - t0).count() / kRounds, kRounds);
});

// ---- the command ---------------------------------------------------------------------------------------------------------------------

namespace pptest {

ppcgen::Hooks hooks() {
    ppcgen::Hooks h;
    h.load_image = [](const std::filesystem::path&) { return program_image(); };
    h.compiler.path_variable = "";   // no compiler to be found
    return h;
}

int run(const ppcgen::Hooks& h, std::vector<std::string> args) { return ppcgen::main(h, args); }

}  // namespace pptest

CB_TEST("recompile.powerpc.command.runtime_header", [] {
    fs::ScopedTempDir dir("cb-ppc-main");
    testing::CapturedStd captured;
    CB_CHECK_EQ(run(hooks(), {"--runtime", fs::to_text(dir.path() / "rex_ppc_min.h")}), 0);
    CB_CHECK_EQ(fs::read_text(dir.path() / "rex_ppc_min.h"), ppcgen::runtime_header());
    CB_CHECK(captured.out().starts_with("wrote "));
});

CB_TEST("recompile.powerpc.command.one_function", [] {
    fs::ScopedTempDir dir("cb-ppc-main");
    testing::CapturedStd captured;
    const std::filesystem::path out = dir.path() / "f1.cpp";
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--at", "0x82100000", "--size", "0x40", "-o", fs::to_text(out)}), 0);
    const Bytes code = fs::read_file(testing::data_path("recompile/testdata/config_code.bin"));
    const ppcgen::Recompiled f1 = ppcgen::recompile_function(plain_dis(), ByteView(code).subspan(0, 0x40), kProgramVa, 0x40);
    CB_CHECK_EQ(captured.out(), std::format("sub_82100000: {}/{} instructions translated, {} not, {} labels, {} calls\n", f1.stats.translated, f1.stats.total,
                                           f1.stats.unimplemented, f1.stats.labels, f1.called.size()));
    CB_CHECK_EQ(fs::read_text(out), "#include \"rex_ppc_min.h\"\n\n" + ppcgen::forward_declarations(f1.called) + "\n\n" + f1.cpp);
    // the size defaults to 0x100
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--at", "0x82100000"}), 0);
});

CB_TEST("recompile.powerpc.command.every_function", [] {
    fs::ScopedTempDir dir("cb-ppc-main");
    testing::CapturedStd captured;
    const std::filesystem::path out = dir.path() / "game.cpp";
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--all", "-o", fs::to_text(out), "-j", "2"}), 0);
    const auto sections = sections_of(slurp("powerpc_image.golden.txt"));
    CB_CHECK_EQ(fs::read_text(out), bar_text(sections.at("image_default.body")));
    CB_CHECK_EQ(fs::read_text(dir.path() / "game_init.cpp"), bar_text(sections.at("image_default.mapping")));
    const ppcgen::ImageResult made = ppcgen::recompile_image(program(), {});
    CB_CHECK(captured.out().starts_with(std::format("{} functions ({} found, {} tail-jump entries), {}/{} instructions translated (", made.agg.funcs, made.agg.discovered,
                                                    made.agg.tail_entries, made.agg.translated, made.agg.total)));
    CB_CHECK(captured.out().ends_with("wrote " + fs::to_text(out) + " and its mapping table\n"));
});

CB_TEST("recompile.powerpc.command.into_the_project_folder", [] {
    fs::ScopedTempDir dir("cb-ppc-main");
    testing::CapturedStd captured;
    ppcgen::Hooks h = hooks();
    std::vector<std::filesystem::path> recorded;
    h.project_output = [&](const std::filesystem::path&) { return dir.path() / "game.cpp"; };
    h.record = [&](const std::filesystem::path&, const std::vector<std::filesystem::path>& written, std::string_view) { recorded = written; };
    CB_CHECK_EQ(run(h, {"game.xex", "--all", "--limit", "1"}), 0);
    CB_CHECK_EQ(recorded.size(), std::size_t{2});
    CB_CHECK(std::filesystem::exists(dir.path() / "game_init.cpp"));
    // without a project to write into, -o is needed
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--all"}), 2);
});

CB_TEST("recompile.powerpc.command.check_without_a_compiler", [] {
    testing::CapturedStd captured;
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--at", "0x82100000", "--size", "0x40", "--check"}), 1);
    CB_CHECK(captured.out().find("no C++ compiler") != std::string::npos);
});

CB_TEST("recompile.powerpc.command.usage", [] {
    testing::CapturedStd captured;
    CB_CHECK_EQ(run(hooks(), {}), 2);                                           // no image
    CB_CHECK_EQ(run(hooks(), {"game.xex"}), 2);                                 // nothing to recompile
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--at", "zz"}), 2);                   // not a number
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--at", "0x10"}), 2);                 // not in the code
    CB_CHECK_EQ(run(hooks(), {"game.xex", "--all", "--bogus"}), 2);
});

}  // namespace
