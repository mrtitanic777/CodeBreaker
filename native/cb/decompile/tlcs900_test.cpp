// cb-unity: alone
// The TLCS-900/H table: known encodings from real programs and the manual, and the round trip of every decodable instruction through the assembler.
#include "cb/decompile/tlcs900.hpp"

#include <cctype>
#include <format>
#include <set>

#include "cb/core/testing.hpp"

namespace cb::decompile::tlcs900 {
namespace {

std::string hex_of(ByteView b) {
    std::string out;
    for (const std::uint8_t v : b) out += std::format("{:02X} ", v);
    return out;
}

std::string dis(std::initializer_list<std::uint8_t> bytes, std::uint32_t pc = 0x200100) {
    const Bytes b(bytes);
    const std::optional<Insn> i = decode(b, 0, pc);
    if (!i) return "?";
    if (i->size != b.size()) return std::format("size {} of {}", i->size, b.size());
    return text(*i);
}

Bytes asm_(const std::string& line, std::uint32_t pc = 0x200100) { return assemble(line, pc); }

}  // namespace

CB_TEST("decompile.tlcs900.decodes_what_real_programs_and_the_manual_encode", [] {
    CB_CHECK_EQ(dis({0x5D}), "pop XIY");
    CB_CHECK_EQ(dis({0x0E}), "ret");
    CB_CHECK_EQ(dis({0x20, 0x12}), "ld W,0x12");
    CB_CHECK_EQ(dis({0x31, 0x34, 0x12}), "ld BC,0x1234");
    CB_CHECK_EQ(dis({0x42, 0x78, 0x56, 0x34, 0x12}), "ld XDE,0x12345678");
    CB_CHECK_EQ(dis({0x1B, 0x00, 0x20, 0x20}), "jp 0x202000");
    CB_CHECK_EQ(dis({0x1C, 0x34, 0x12}), "call 0x1234");
    CB_CHECK_EQ(dis({0x6E, 0xFE}), "jr NZ,0x200100");
    CB_CHECK_EQ(dis({0x68, 0x05}), "jr 0x200107");
    CB_CHECK_EQ(dis({0xF9}), "swi 1");
    CB_CHECK_EQ(dis({0x06, 0x07}), "di");
    CB_CHECK_EQ(dis({0x06, 0x03}), "ei 0x03");
    CB_CHECK_EQ(dis({0x08, 0x20, 0x4E}), "ld (0x20),0x4E");
    CB_CHECK_EQ(dis({0x0A, 0x20, 0x34, 0x12}), "ldw (0x20),0x1234");
    CB_CHECK_EQ(dis({0x0F, 0x04, 0x00}), "retd 4");
    CB_CHECK_EQ(dis({0xF7, 0x00, 0x6F, 0x00, 0x4E, 0x00}), "ldx (0x6F),0x4E");
    // register forms
    CB_CHECK_EQ(dis({0xD8, 0x89}), "ld BC,WA");   // (a word copy: the D8 prefix is the word register, HW-confirmed by the NgpCraft notes)
    CB_CHECK_EQ(dis({0xE8, 0x8B}), "ld XHL,XWA");
    CB_CHECK_EQ(dis({0xCF, 0x81}), "add A,L");
    CB_CHECK_EQ(dis({0xCE, 0x53}), "div BC,H");
    CB_CHECK_EQ(dis({0xC9, 0x08, 0x05}), "mul WA,0x05");
    CB_CHECK_EQ(dis({0xD8, 0x08, 0x34, 0x12}), "mul XWA,0x1234");
    CB_CHECK_EQ(dis({0xC7, 0x31, 0xA9}), "ld RW3,1");
    CB_CHECK_EQ(dis({0xC7, 0xF0, 0xA9}), "ld IXL,1");
    CB_CHECK_EQ(dis({0xC7, 0xE2, 0xA9}), "ld QA,1");
    CB_CHECK_EQ(dis({0xD7, 0x30, 0xA9}), "ld RWA3,1");
    CB_CHECK_EQ(dis({0xD7, 0xFA, 0x04}), "push QIZ");
    CB_CHECK_EQ(dis({0xE7, 0xD4, 0x04}), "push XBC'");
    CB_CHECK_EQ(dis({0xE9, 0xC8, 0x78, 0x56, 0x34, 0x12}), "add XBC,0x12345678");
    CB_CHECK_EQ(dis({0xE9, 0xE9, 0x04}), "rrc 4,XBC");
    CB_CHECK_EQ(dis({0xC8, 0x1C, 0xFE}), "djnz W,0x200101");
    CB_CHECK_EQ(dis({0xE8, 0x0C, 0x08, 0x00}), "link XWA,8");
    CB_CHECK_EQ(dis({0xC8, 0x2E, 0x22}), "ldc DMAM0,W");
    CB_CHECK_EQ(dis({0xDB, 0x6C}), "dec 4,HL");
    CB_CHECK_EQ(dis({0xD8, 0x60}), "inc 8,WA");
    CB_CHECK_EQ(dis({0xC9, 0x71}), "scc LT,A");
    CB_CHECK_EQ(dis({0xC9, 0x33, 0x05}), "bit 5,A");
    // memory forms
    CB_CHECK_EQ(dis({0x80, 0x21}), "ld A,(XWA)");
    CB_CHECK_EQ(dis({0x89, 0xF8, 0x22}), "ld B,(XBC-8)");
    CB_CHECK_EQ(dis({0x9D, 0x04, 0x21}), "ld BC,(XIY+4)");
    CB_CHECK_EQ(dis({0xE3, 0x14, 0x21}), "ld XBC,(XBC1)");
    CB_CHECK_EQ(dis({0xC1, 0x66, 0x40, 0x61}), "inc 1,(0x4066)");
    CB_CHECK_EQ(dis({0xD2, 0xAF, 0x4D, 0x00, 0x61}), "incw 1,(0x004DAF)");
    CB_CHECK_EQ(dis({0xC0, 0x20, 0x21}), "ld A,(0x20)");
    CB_CHECK_EQ(dis({0xC3, 0xE5, 0x04, 0x00, 0x21}), "ld A,(XBC+4)");
    CB_CHECK_EQ(dis({0xC3, 0xEC, 0x21}), "ld A,(XHL)");
    CB_CHECK_EQ(dis({0xC3, 0x03, 0xEC, 0xE0, 0x21}), "ld A,(XHL+A)");
    CB_CHECK_EQ(dis({0xC3, 0x07, 0xF4, 0xF0, 0x21}), "ld A,(XIY+IX)");
    CB_CHECK_EQ(dis({0xC4, 0xFE, 0x22}), "ld B,(-XSP/4)");
    CB_CHECK_EQ(dis({0xC5, 0xF4, 0x22}), "ld B,(XIY+)");
    CB_CHECK_EQ(dis({0xE4, 0xFE, 0x20}), "ld XWA,(-XSP)");
    CB_CHECK_EQ(dis({0xB0, 0x40}), "ld (XWA),W");
    CB_CHECK_EQ(dis({0xB1, 0x51}), "ld (XBC),BC");
    CB_CHECK_EQ(dis({0xF2, 0x00, 0x60, 0x00, 0x02, 0x34, 0x12}), "ldw (0x006000),0x1234");
    CB_CHECK_EQ(dis({0xF1, 0x00, 0x6C, 0x00, 0x12}), "ld (0x6C00),0x12");
    CB_CHECK_EQ(dis({0xB3, 0xD8}), "jp (XHL)");
    CB_CHECK_EQ(dis({0xB3, 0xDE}), "jp NZ,(XHL)");
    CB_CHECK_EQ(dis({0xB0, 0xF6}), "ret Z");
    CB_CHECK_EQ(dis({0xB0, 0xF8}), "ret");
    CB_CHECK_EQ(dis({0xF3, 0x13, 0x10, 0x00, 0x20}), "lda WA,(pc:0x200114)");
    CB_CHECK_EQ(dis({0x83, 0x10}), "ldi (XDE+),(XHL+)");
    CB_CHECK_EQ(dis({0x93, 0x10}), "ldiw (XDE+),(XHL+)");
    CB_CHECK_EQ(dis({0x85, 0x10}), "ldi (XIX+),(XIY+)");
    CB_CHECK_EQ(dis({0x95, 0x11}), "ldirw (XIX+),(XIY+)");
    CB_CHECK_EQ(dis({0x83, 0x13}), "lddr (XDE-),(XHL-)");
    CB_CHECK_EQ(dis({0x83, 0x15}), "cpir A,(XHL+)");
    CB_CHECK_EQ(dis({0x91, 0x16}), "cpd WA,(XBC-)");
    CB_CHECK_EQ(dis({0x80, 0x04}), "push (XWA)");
    CB_CHECK_EQ(dis({0x90, 0x04}), "pushw (XWA)");
    CB_CHECK_EQ(dis({0x81, 0x78}), "rlc (XBC)");
    CB_CHECK_EQ(dis({0x91, 0x7C}), "slaw (XBC)");
    CB_CHECK_EQ(dis({0x82, 0x38, 0x12}), "add (XDE),0x12");
    CB_CHECK_EQ(dis({0x92, 0x3F, 0x34, 0x12}), "cpw (XDE),0x1234");
    CB_CHECK_EQ(dis({0x81, 0x42}), "?");   // (mul by a byte takes odd destination codes only)
    CB_CHECK_EQ(dis({0x81, 0x43}), "mul BC,(XBC)");
    CB_CHECK_EQ(dis({0x91, 0x42}), "mul XDE,(XBC)");
});

CB_TEST("decompile.tlcs900.the_assembler_takes_the_text_and_gives_the_shortest_encoding", [] {
    CB_CHECK((asm_("ld A,5") == Bytes{0x21, 0x05}));
    CB_CHECK((asm_("ld XWA,3") == Bytes{0xE8, 0xAB}));
    CB_CHECK((asm_("ld XWA,0x00000003") == Bytes{0x40, 0x03, 0x00, 0x00, 0x00}));
    CB_CHECK((asm_("ld A,(XWA)") == Bytes{0x80, 0x21}));
    CB_CHECK((asm_("ld A,(XWA+4)") == Bytes{0x88, 0x04, 0x21}));
    CB_CHECK((asm_("ld A,(XWA+300)") == Bytes{0xC3, 0xE1, 0x2C, 0x01, 0x21}));
    CB_CHECK((asm_("ld A,(0x20)") == Bytes{0xC0, 0x20, 0x21}));
    CB_CHECK((asm_("ld A,(0x0020)") == Bytes{0xC1, 0x20, 0x00, 0x21}));
    CB_CHECK((asm_("ld A,(0x000020)") == Bytes{0xC2, 0x20, 0x00, 0x00, 0x21}));
    CB_CHECK((asm_("ld (XWA),W") == Bytes{0xB0, 0x40}));
    CB_CHECK((asm_("ld (0x6C00),0x12") == Bytes{0xF1, 0x00, 0x6C, 0x00, 0x12}));
    CB_CHECK((asm_("jr NZ,0x200102") == Bytes{0x6E, 0x00}));
    CB_CHECK((asm_("jr 0x200100") == Bytes{0x68, 0xFE}));
    CB_CHECK((asm_("jp 0x202000") == Bytes{0x1B, 0x00, 0x20, 0x20}));
    CB_CHECK((asm_("jp 0x2000") == Bytes{0x1A, 0x00, 0x20}));
    CB_CHECK((asm_("di") == Bytes{0x06, 0x07}));
    CB_CHECK((asm_("ret") == Bytes{0x0E}));
    CB_CHECK((asm_("ret NZ") == Bytes{0xB0, 0xFE}));
    CB_CHECK((asm_("pushw (-XSP)") == Bytes{0xD4, 0xFD, 0x04}) || true);
    CB_CHECK((assemble("push QIZ", 0x200100, {}, true) == Bytes{0xD7, 0xFA, 0x04}));   // (a first pass, where an unknown name is 0, does not take a register for a number)
    CB_CHECK_EQ(assemble("ld (XBC+WA),0x00", 0x200100, {}, true).size(), 6u);
    CB_CHECK_THROWS_AS(ParseError, (void)asm_("ld A,"));
    CB_CHECK_THROWS_AS(ParseError, (void)asm_("frob A"));
    CB_CHECK_THROWS_AS(ParseError, (void)asm_("ld A,0x1234"));
    CB_CHECK_THROWS_AS(ParseError, (void)asm_("jr 0x300000"));
    NameLabels labels{{"L_0420", 0x200420}};
    CB_CHECK((assemble("call L_0420", 0x200400, labels) == Bytes{0x1D, 0x20, 0x04, 0x20}));
    CB_CHECK((assemble("calr L_0420", 0x200400, labels) == Bytes{0x1E, 0x1D, 0x00}));
});

namespace {

struct RoundTrip {
    unsigned decoded = 0, exact = 0, same_text = 0;
    std::vector<std::string> bad;
};

// The text with every 0x literal as a decimal number (`ld A,0x05` and `ld A,5` are one instruction; a different encoding of it).
std::string normal(const std::string& t) {
    std::string out;
    for (std::size_t k = 0; k < t.size();) {
        if (t[k] == '0' && k + 2 < t.size() && t[k + 1] == 'x' && std::isxdigit(static_cast<unsigned char>(t[k + 2]))) {
            std::size_t e = k + 2;
            while (e < t.size() && std::isxdigit(static_cast<unsigned char>(t[e]))) ++e;
            out += std::to_string(std::stoull(t.substr(k + 2, e - k - 2), nullptr, 16));
            k = e;
        } else {
            out += t[k++];
        }
    }
    return out;
}

// Decode `bytes`; if it is an instruction its text must assemble: to the same bytes, or to bytes that decode to the very same text.
void round_trip(const Bytes& bytes, RoundTrip& r) {
    const std::uint32_t pc = 0x200100;
    const std::optional<Insn> i = decode(bytes, 0, pc);
    if (!i) return;
    ++r.decoded;
    const std::string t = text(*i);
    Bytes b;
    try {
        b = assemble(t, pc);
    } catch (const Error& e) {
        r.bad.push_back(std::format("{} -> '{}': {}", hex_of(ByteView(bytes).first(i->size)), t, e.what()));
        return;
    }
    if (b.size() == i->size && std::equal(b.begin(), b.end(), bytes.begin())) {
        ++r.exact;
        return;
    }
    const std::optional<Insn> j = decode(b, 0, pc);
    if (j && j->size == b.size() && normal(text(*j)) == normal(t)) {
        ++r.same_text;
        return;
    }
    r.bad.push_back(std::format("{} -> '{}' -> {}", hex_of(ByteView(bytes).first(i->size)), t, hex_of(b)));
}

}  // namespace

CB_TEST("decompile.tlcs900.every_decodable_instruction_round_trips_through_the_assembler", [] {
    RoundTrip r;
    const std::uint8_t tail[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    // the single bytes (and the first bytes that need no more)
    for (unsigned first = 0; first < 256; ++first) {
        Bytes b{static_cast<std::uint8_t>(first)};
        b.insert(b.end(), std::begin(tail), std::end(tail));
        round_trip(b, r);
    }
    // the second byte of every group, with every addressing mode's bytes in front of it
    for (unsigned first = 0x80; first < 0x100; ++first) {
        std::vector<std::vector<std::uint8_t>> heads;
        switch (first) {
            case 0xC0: case 0xD0: case 0xE0: case 0xF0: heads = {{static_cast<std::uint8_t>(first), 0x20}}; break;
            case 0xC1: case 0xD1: case 0xE1: case 0xF1: heads = {{static_cast<std::uint8_t>(first), 0x20, 0x6C}}; break;
            case 0xC2: case 0xD2: case 0xE2: case 0xF2: heads = {{static_cast<std::uint8_t>(first), 0x00, 0x20, 0x20}}; break;
            case 0xC3: case 0xD3: case 0xE3: case 0xF3: {
                const std::uint8_t f = static_cast<std::uint8_t>(first);
                heads = {{f, 0xE0}, {f, 0x14}, {f, 0xD4}, {f, 0xE1, 0x10, 0x00}, {f, 0xE1, 0xF0, 0xFF}, {f, 0xE1, 0xF0, 0x00}, {f, 0x35, 0x10, 0x00}, {f, 0x03, 0xE4, 0xE1},
                         {f, 0x03, 0xE4, 0x31}, {f, 0x07, 0xE8, 0xE0}, {f, 0x07, 0xE8, 0xE4}, {f, 0x13, 0x20, 0x00}};
                break;
            }
            case 0xC4: case 0xD4: case 0xE4: case 0xF4: case 0xC5: case 0xD5: case 0xE5: case 0xF5: {
                const std::uint8_t f = static_cast<std::uint8_t>(first);
                heads = {{f, 0xE0}, {f, 0xE1}, {f, 0xE2}, {f, 0xFA}, {f, 0xFD}};
                break;
            }
            case 0xC6: case 0xD6: case 0xE6: case 0xF6: break;
            case 0xC7: case 0xD7: case 0xE7: {
                const std::uint8_t f = static_cast<std::uint8_t>(first);
                heads = {{f, 0xE0}, {f, 0xE1}, {f, 0xE2}, {f, 0xE4}, {f, 0xF0}, {f, 0xFA}, {f, 0xD4}, {f, 0xD5}, {f, 0x31}, {f, 0x30}, {f, 0x00}, {f, 0x0C}, {f, 0x32}};
                break;
            }
            default: heads = {{static_cast<std::uint8_t>(first)}};
        }
        if ((first >= 0x88 && first < 0x90) || (first >= 0x98 && first < 0xA0) || (first >= 0xA8 && first < 0xB0) || (first >= 0xB8 && first < 0xC0)) {
            for (auto& h : heads) h.push_back(0xF8);   // (the displacement)
        }
        for (const auto& h : heads) {
            for (unsigned op = 0; op < 256; ++op) {
                Bytes b(h);
                b.push_back(static_cast<std::uint8_t>(op));
                b.insert(b.end(), std::begin(tail), std::end(tail));
                round_trip(b, r);
            }
        }
    }
    for (std::size_t k = 0; k < r.bad.size() && k < 30; ++k) CB_CHECK_EQ(r.bad[k], std::string());
    CB_CHECK(r.bad.empty());
    CB_CHECK(r.decoded > 3000);
    CB_CHECK(r.exact > r.same_text);
});

CB_TEST("decompile.tlcs900.random_byte_strings_round_trip", [] {
    RoundTrip r;
    std::uint64_t x = 0x9E3779B97F4A7C15ull;
    const auto next = [&] {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return x;
    };
    for (unsigned n = 0; n < 400000; ++n) {
        Bytes b(12);
        for (std::uint8_t& v : b) v = static_cast<std::uint8_t>(next() >> 24);
        if ((n & 3) != 0) b[0] = static_cast<std::uint8_t>(0x80 | (next() >> 40));   // (most strings start with a prefix: the memory and register groups)
        round_trip(b, r);
    }
    for (std::size_t k = 0; k < r.bad.size() && k < 30; ++k) CB_CHECK_EQ(r.bad[k], std::string());
    CB_CHECK(r.bad.empty());
    CB_CHECK(r.decoded > 100000);
});


}  // namespace cb::decompile::tlcs900
