#include "cb/decompile/vmx128.hpp"

#include <algorithm>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace v = cb::decompile::vmx128;
using std::string;

// DIVERGES from the Python module: none in the decoding or the generated SLEIGH (the old harness decoded all 77 rows
// with random free bits, 4 real games' vector words, and wrote the SLEIGH with and without declared operations; the
// text matches the original byte for byte, and the checked-in languages/src/ppc/vmx128.sinc is its output). The
// command-line shell (`main` with --coverage) is not here: it belongs to the command table; `coverage` takes the code
// section's words, which the PowerPC image reader supplies.

namespace {

// A checkout may hold the language file with CR LF ends (git's autocrlf); the generator writes LF.
string lf(string text) {
    text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
    return text;
}

string show(std::uint32_t word) {
    const auto t = v::text(word);
    return t ? *t : "(none)";
}

}  // namespace

CB_TEST("decompile.vmx128.fields", [] {
    // each register number is five bits from the usual place and two or three more from inside the extended opcode:
    // vD v100 = 0b1100100 -> 4 in bits 21..25 (IBM 6..10), 3 in the bits at 2..3
    CB_CHECK_EQ(v::vd128((4u << 21) | (3u << 2)), 100u);
    CB_CHECK_EQ(v::vb128(3u | (7u << 11)), 103u);                          // 3 in bits 0..1, 7 in 11..15
    CB_CHECK_EQ(v::va128((1u << 10) | (1u << 5) | (9u << 16)), 105u);      // bit 10 is 64, bit 5 is 32, 9 in 16..20
    CB_CHECK_EQ(v::vc128(7u << 6), 7u);
    CB_CHECK_EQ(v::vd128(0x102320C3), 1u);
});

CB_TEST("decompile.vmx128.decode.words", [] {
    CB_CHECK_EQ(show(0x102320C3), "lvx128 v1,r3,r4");   // (llvm-mc reads this word as vucmprlb 1,3,4)
    CB_CHECK_EQ(show(0x14000000), "vperm128 v0,v0,v0,v0");
    CB_CHECK_EQ(show(0x180000D0), "vslw128 v0,v0,v0");
    CB_CHECK_EQ(show(0x140000D0), "vmaddfp128 v0,v0,v0,v0");   // (vD again, as the addend)
    CB_CHECK_EQ(show(0x14000110), "vmaddcfp128 v0,v0,v0,v0");
    CB_CHECK_EQ(show(0x18000180), "vcmpbfp128 v0,v0,v0");
    CB_CHECK_EQ(show(0x180001C0), "vcmpbfp128. v0,v0,v0");   // the record bit
    CB_CHECK_EQ(show(0x18000140), "vcmpgtfp128. v0,v0,v0");
    CB_CHECK_EQ(show(0x181F0770), "vspltisw128 v0,-1");     // the 5-bit immediate is signed
    CB_CHECK_EQ(show(0x18030770), "vspltisw128 v0,3");
    CB_CHECK_EQ(show(0x18070350), "vpermwi128 v0,v0,167");   // the 8-bit control comes in two pieces
    CB_CHECK_EQ(show(0x18000610), "vpkd3d128 v0,v0,0,0,0");
    CB_CHECK_EQ(show(0x18000710), "vrlimi128 v0,v0,0,0");
    // an rA of 0 is the literal zero in an address
    CB_CHECK_EQ(show(0x102020C3), "lvx128 v1,0,r4");
});

CB_TEST("decompile.vmx128.decode.opcode_31_unaligned_loads_and_stores", [] {
    const auto xform = [](std::uint32_t xo, std::uint32_t d, std::uint32_t a, std::uint32_t b) {
        return 0x7C000000u | (xo << 1) | (d << 21) | (a << 16) | (b << 11);
    };
    CB_CHECK_EQ(show(xform(519, 3, 4, 5)), "lvlx v3,r4,r5");
    CB_CHECK_EQ(show(xform(903, 3, 0, 5)), "stvlxl v3,0,r5");
    CB_CHECK_EQ(show(xform(519, 3, 4, 5) | 1), "(none)");   // the record bit makes it another instruction
    CB_CHECK_EQ(show(xform(20, 3, 4, 5)), "(none)");        // an opcode-31 word that is not one of the eight
});

CB_TEST("decompile.vmx128.decode.not_vmx128", [] {
    CB_CHECK_EQ(show(0x38000000), "(none)");
    CB_CHECK_EQ(show(0x10000000), "(none)");
    CB_CHECK_EQ(show(0x1FE000D0), "(none)");   // primary opcode 7
    CB_CHECK(!v::decode(0).has_value());
    CB_CHECK(!v::decode(0xFFFFFFFFu).has_value());
});

CB_TEST("decompile.vmx128.table", [] {
    CB_CHECK_EQ(v::rows().size(), 77u);
    CB_CHECK_EQ(v::x31_rows().size(), 8u);
    for (const v::Row& r : v::rows()) {
        // every row decodes to its own mnemonic at its pattern
        const auto d = v::decode(r.pattern);
        CB_CHECK(d.has_value());
        if (d) CB_CHECK_EQ(d->mnemonic, string(r.mnemonic));
    }
});

CB_TEST("decompile.vmx128.coverage", [] {
    const std::vector<std::uint32_t> words = {0x102320C3, 0x38000000, 0x10000000, 0x14000000, 0x7C642C0E, 0x18000000};
    const v::Coverage c = v::coverage(words);
    // 0x10000000 is primary opcode 4 and a legal AltiVec word that is not VMX128; 0x7C... and 0x38... are not vector words
    CB_CHECK_EQ(c.vector_words, 4u);
    CB_CHECK_EQ(c.vmx128, 3u);
    CB_CHECK_EQ(c.other, 1u);
    CB_CHECK_EQ(v::coverage({}).vector_words, 0u);
});

CB_TEST("decompile.vmx128.sleigh_is_the_checked_in_language", [] {
    // the generated text is what languages/src/ppc/vmx128.sinc holds, given the operations its sibling files define
    const std::filesystem::path sinc = cb::testing::data_path("../decompiler/languages/src/ppc/vmx128.sinc");
    if (!std::filesystem::exists(sinc)) return;   // (a checkout without the decompiler's languages)
    std::set<string> declared;
    for (const auto& entry : std::filesystem::directory_iterator(sinc.parent_path())) {
        if (entry.path().extension() == ".sinc" && entry.path().filename() != "vmx128.sinc") {
            const std::set<string> ops = v::declared_operations(cb::fs::read_text(entry.path()));
            declared.insert(ops.begin(), ops.end());
        }
    }
    CB_CHECK(!declared.empty());
    CB_CHECK_EQ(v::sleigh(declared) + "\n", lf(cb::fs::read_text(sinc)));
    // write_sleigh_file does the same from a folder holding the siblings
    cb::fs::ScopedTempDir dir("vmx128");
    for (const auto& entry : std::filesystem::directory_iterator(sinc.parent_path())) {
        if (entry.path().extension() == ".sinc" && entry.path().filename() != "vmx128.sinc") {
            std::filesystem::copy_file(entry.path(), dir.path() / entry.path().filename());
        }
    }
    CB_CHECK_EQ(v::write_sleigh_file(dir.path() / "vmx128.sinc"), 77u);
    CB_CHECK_EQ(cb::fs::read_text(dir.path() / "vmx128.sinc"), lf(cb::fs::read_text(sinc)));   // (the generated file has LF ends)
});

CB_TEST("decompile.vmx128.sleigh_without_siblings_defines_every_operation", [] {
    const string all = v::sleigh();
    CB_CHECK(all.find("define pcodeop vectorPermute;") != string::npos);
    CB_CHECK(all.find("define pcodeop loadVectorLeftIndexed;") != string::npos);
    CB_CHECK(all.find("define pcodeop vectorCompareSetCR6;") != string::npos);
    const string without = v::sleigh({"vectorPermute"});
    CB_CHECK(without.find("define pcodeop vectorPermute;") == string::npos);
    CB_CHECK(without.find(":vperm128 vD128,vA128,vB128,vC128 is") != string::npos);   // (still used by the constructor)
    CB_CHECK_EQ(v::declared_operations("define pcodeop a;\n  define pcodeop b;\ndefine pcodeop c_1;\ndefine pcodeop bad name;\r\ndefine pcodeop d;").size(), 3u);   // a, c_1, d: only lines that begin with it
});
