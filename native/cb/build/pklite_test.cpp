#include "cb/build/pklite.hpp"

#include <algorithm>
#include <random>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::build;

namespace {

// EXTRACT.EXE of the Win95 media, a real PKLITE 1.15 (large) load module: the part after its MZ header.
Bytes real_module() { return fs::read_file(testing::data_path("build/testdata/extract.mod")); }

Bytes pseudo_random(std::size_t n, unsigned seed, unsigned alphabet) {
    std::mt19937 rng(seed);
    Bytes out(n);
    for (auto& b : out) b = static_cast<std::uint8_t>(rng() % alphabet);
    return out;
}

Bytes text(std::string_view s) { return to_bytes(s); }

using pklite::Token;

}  // namespace

CB_TEST("build.pklite.reads_a_real_module", [] {
    const Bytes module = real_module();
    CB_CHECK_EQ(pklite::stream_start(module), 656u);
    const pklite::Stored s = pklite::read(module);
    CB_CHECK_EQ(s.start, 656u);
    CB_CHECK_EQ(s.end, 45573u);
    CB_CHECK_EQ(s.image.size(), 86150u);
    CB_CHECK_EQ(s.tokens.size(), 33011u);
    CB_CHECK_EQ(crypto::crc32(s.image), 0x0f5a1005u);   // (as the original Python read it)
    CB_CHECK(s.tokens.back() == Token::end());
    CB_CHECK_EQ(std::count(s.tokens.begin(), s.tokens.end(), Token::mark()), 2);   // 86150 bytes: marks at 32491 and 73451
    CB_CHECK(s.regs == (pklite::Registers{.cs = 2260, .ip = 30, .ss = 5568, .sp = 5120}));
    CB_CHECK(s.tail.empty());
    // the program's relocations: 28 runs, 431 in all, in the order the table lists them
    CB_CHECK_EQ(s.relocs.size(), 28u);
    std::size_t total = 0;
    for (const auto& run : s.relocs) total += run.offsets.size();
    CB_CHECK_EQ(total, 431u);
    CB_CHECK_EQ(s.relocs.at(0).segment, 0);
    CB_CHECK_EQ(s.relocs.at(0).offsets.at(0), 923);
    CB_CHECK_EQ(s.relocs.at(0).offsets.at(1), 886);
    // the expanded program is a DOS program: its first bytes are code, here `enter 0x0004, 0` (C8 24 04 00) ...
    CB_CHECK_EQ(s.image.at(0), 0xC8);
    CB_CHECK_EQ(s.image.at(1), 0x24);
});

CB_TEST("build.pklite.the_prefix_codes_are_the_decompressors", [] {
    const Bytes module = real_module();
    const pklite::Stub stub = pklite::Stub::of(module, pklite::stream_start(module));
    CB_CHECK_EQ(stub.head.size(), 656u);
    CB_CHECK_EQ(stub.lengths.size(), 24u);   // lengths 2..25
    CB_CHECK_EQ(stub.highs.size(), 31u);     // high bytes 1..31
    const auto bits = [](const pklite::Code& c) {
        std::string s;
        for (int i = c.length - 1; i >= 0; --i) s += ((c.bits >> i) & 1) ? '1' : '0';
        return s;
    };
    CB_CHECK_EQ(bits(stub.lengths.at(2)), "10");
    CB_CHECK_EQ(bits(stub.lengths.at(3)), "11");
    CB_CHECK_EQ(bits(stub.lengths.at(4)), "000");
    CB_CHECK_EQ(bits(stub.lengths.at(25)), "011100");   // 25: a copy whose length byte follows
    CB_CHECK_EQ(bits(stub.lengths.at(24)), "011111111");
    CB_CHECK_EQ(bits(stub.highs.at(1)), "000");
    CB_CHECK_EQ(bits(stub.highs.at(14)), "101110");
    CB_CHECK_EQ(bits(stub.highs.at(31)), "111111");
    // it is a prefix code: no code is the start of another
    for (const pklite::Codes* codes : {&stub.lengths, &stub.highs})
        for (const auto& a : *codes)
            for (const auto& b : *codes) {
                if (&a == &b || a.second.length > b.second.length) continue;
                const unsigned shift = b.second.length - a.second.length;
                CB_CHECK((b.second.bits >> shift) != a.second.bits);
            }
});

CB_TEST("build.pklite.encoding_what_was_read_gives_the_stream_again", [] {
    const Bytes module = real_module();
    const pklite::Stored s = pklite::read(module);
    const Bytes stream = pklite::encode(s.tokens, s.stub);
    CB_CHECK_EQ(stream.size(), s.end - s.start);
    CB_CHECK(std::equal(stream.begin(), stream.end(), module.begin() + static_cast<std::ptrdiff_t>(s.start)));
});

CB_TEST("build.pklite.parse_makes_the_choices_the_real_packer_made", [] {
    // 86 KB, 33011 tokens: every copy's length and distance, every put-off literal and both marks as PKLITE chose them
    const pklite::Stored s = pklite::read(real_module());
    CB_CHECK(pklite::parse(s.image) == s.tokens);
});

CB_TEST("build.pklite.pack_rebuilds_the_real_module", [] {
    const Bytes module = real_module();
    const pklite::Stored s = pklite::read(module);
    std::vector<pklite::Relocated> relocs;
    for (const auto& run : s.relocs)
        for (std::uint16_t off : run.offsets) relocs.push_back({run.segment, off});
    const ByteView head(module.data(), s.start);
    const pklite::Packed p = pklite::pack(s.image, relocs, s.regs, head, 0);
    CB_CHECK_EQ(p.module.size(), module.size());
    // all but the two words the entry code holds: the memory it needs, and the size of what is stored
    CB_CHECK(std::equal(p.module.begin() + 6, p.module.end(), module.begin() + 6));
    CB_CHECK_EQ(p.module[1] | (p.module[2] << 8), 0x1593);   // mov ax, top (for `memory` = 0)
    CB_CHECK_EQ(p.module[4] | (p.module[5] << 8), 0x0B5C);   // mov dx, paragraphs: 46528 bytes
    CB_CHECK(p.header.cs == 0xFFF0 && p.header.ip == 0x100);
    CB_CHECK_EQ(p.header.ss, 2917);
    CB_CHECK_EQ(p.header.sp, 0x200);
    CB_CHECK_EQ(p.header.min_alloc, 2631);
    CB_CHECK_EQ(p.header.relocs.size(), 1u);
    CB_CHECK(p.header.relocs[0].segment == 7 && p.header.relocs[0].offset == 0);
    // more memory asked for: the entry code is told more, nothing else changes
    const pklite::Packed more = pklite::pack(s.image, relocs, s.regs, head, 5000);
    CB_CHECK_EQ(more.module[1] | (more.module[2] << 8), 0x2881);
    CB_CHECK_EQ(more.header.min_alloc, 7477);
    CB_CHECK(std::equal(more.module.begin() + 6, more.module.end(), module.begin() + 6));
});

CB_TEST("build.pklite.parse_small_images", [] {
    const auto lit = [](char c) { return Token::literal(static_cast<std::uint8_t>(c)); };
    CB_CHECK(pklite::parse({}) == std::vector<Token>{Token::end()});
    CB_CHECK(pklite::parse(text("a")) == (std::vector<Token>{lit('a'), Token::end()}));
    CB_CHECK(pklite::parse(text("ab")) == (std::vector<Token>{lit('a'), lit('b'), Token::end()}));
    CB_CHECK(pklite::parse(text("aaaa")) == (std::vector<Token>{lit('a'), Token::copy(3, 1), Token::end()}));
    CB_CHECK(pklite::parse(text("abcabcabcabc")) ==
             (std::vector<Token>{lit('a'), lit('b'), lit('c'), Token::copy(9, 3), Token::end()}));
    // two copies of the same bytes: the nearest wins at a tie, and a copy is put off by a byte (a literal) when the
    // next one is two longer
    CB_CHECK(pklite::parse(text("xabxxabxabx")) ==
             (std::vector<Token>{lit('x'), lit('a'), lit('b'), lit('x'), Token::copy(4, 4), Token::copy(3, 7), Token::end()}));
    CB_CHECK(pklite::parse(text("abcdXabcdYabcdabcdZ")) ==
             (std::vector<Token>{lit('a'), lit('b'), lit('c'), lit('d'), lit('X'), Token::copy(4, 5), lit('Y'),
                                 Token::copy(4, 5), Token::copy(4, 4), lit('Z'), Token::end()}));
    // a run of one byte: copies of the longest length (277), each from the FARTHEST place that gives it, so the copy
    // comes from where the run began
    CB_CHECK(pklite::parse(Bytes(1000, 0)) ==
             (std::vector<Token>{Token::literal(0), Token::copy(277, 1), Token::copy(277, 278), Token::copy(277, 555),
                                 Token::copy(168, 832), Token::end()}));
    Bytes pairs;
    for (int i = 0; i < 300; ++i) {
        pairs.push_back(1);
        pairs.push_back(2);
    }
    CB_CHECK(pklite::parse(pairs) == (std::vector<Token>{Token::literal(1), Token::literal(2), Token::copy(277, 2),
                                                          Token::copy(277, 278), Token::copy(44, 556), Token::end()}));
});

CB_TEST("build.pklite.marks_come_before_the_first_token_past_each_threshold", [] {
    const Bytes image = pseudo_random(80000, 7, 3);   // compressible enough for long copies, with copies across the thresholds
    const auto tokens = pklite::parse(image);
    std::uint64_t out = 0, next_mark = 0x8000;
    int marks = 0;
    for (const Token& t : tokens) {
        if (t.kind == Token::Kind::Mark) {
            // the mark is placed where output has reached the threshold less the longest copy
            CB_CHECK(out + pklite::kMaxLength >= next_mark);
            next_mark += 0xA000;
            ++marks;
        } else {
            CB_CHECK(out + pklite::kMaxLength < next_mark);   // no token starts at or past a threshold without its mark
            if (t.kind == Token::Kind::Literal) out += 1;
            if (t.kind == Token::Kind::Copy) out += t.length;
        }
    }
    CB_CHECK_EQ(out, image.size());
    CB_CHECK_EQ(marks, 2);
});

CB_TEST("build.pklite.pack_then_read_gives_the_program_back", [] {
    const Bytes real = real_module();
    const std::size_t start = pklite::stream_start(real);
    const ByteView head(real.data(), start);   // PKLITE's own entry code and decompressor
    struct Case {
        const char* name;
        Bytes image;
    };
    Bytes repeated;
    for (int i = 0; i < 700; ++i)
        for (int b : {0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10, 0xC9, 0xCB}) repeated.push_back(static_cast<std::uint8_t>(b + i / 100));
    const std::vector<Case> cases = {
        {"empty", {}},
        {"one byte", {0x90}},
        {"two bytes", {1, 2}},
        {"zeros", Bytes(5000, 0)},
        {"two-letter noise", pseudo_random(3000, 1, 2)},
        {"eight-letter noise", pseudo_random(20000, 2, 8)},
        {"random", pseudo_random(9000, 3, 256)},
        {"code-like", repeated},
        {"past the first mark", pseudo_random(40000, 4, 5)},
        {"past the second mark", pseudo_random(76000, 5, 4)},
    };
    for (const Case& c : cases) {
        const pklite::Registers regs{.cs = 0x1234, .ip = 0x56, .ss = 0x78, .sp = 0x9A};
        const std::vector<pklite::Relocated> relocs = {{3, 0x10}, {3, 0x20}, {9, 1}, {3, 5}};
        const pklite::Packed packed = pklite::pack(c.image, relocs, regs, head, 17);
        const pklite::Stored back = pklite::read(packed.module);
        CB_CHECK(back.image == c.image);
        CB_CHECK(back.regs == regs);
        CB_CHECK(back.tail.empty());
        CB_CHECK(back.tokens == pklite::parse(c.image));
        // the table keeps the order, and a run is one entry per segment change
        CB_CHECK_EQ(back.relocs.size(), 3u);
        CB_CHECK_EQ(back.relocs[0].segment, 3);
        CB_CHECK((back.relocs[0].offsets == std::vector<std::uint16_t>{0x10, 0x20}));
        CB_CHECK_EQ(back.relocs[1].segment, 9);
        CB_CHECK_EQ(back.relocs[2].segment, 3);
        CB_CHECK_EQ(pklite::encode(back.tokens, back.stub).size(), back.end - back.start);
    }
});

CB_TEST("build.pklite.relocation_table_runs", [] {
    CB_CHECK(pklite::relocation_table({}) == Bytes{0});
    const std::vector<pklite::Relocated> few = {{0x0102, 0x0304}, {0x0102, 0x0506}, {7, 8}};
    const Bytes expected = {2, 0x02, 0x01, 0x04, 0x03, 0x06, 0x05, 1, 7, 0, 8, 0, 0};
    CB_CHECK(pklite::relocation_table(few) == expected);
    // a run is cut at 255 offsets
    std::vector<pklite::Relocated> many;
    for (int i = 0; i < 300; ++i) many.push_back({4, static_cast<std::uint16_t>(i)});
    const Bytes table = pklite::relocation_table(many);
    CB_CHECK_EQ(table.size(), 3u + 255 * 2 + 3 + 45 * 2 + 1);
    CB_CHECK_EQ(table[0], 255);
    CB_CHECK_EQ(table[3 + 255 * 2], 45);
    CB_CHECK_EQ(table.back(), 0);
});

CB_TEST("build.pklite.damaged_modules_are_errors_not_crashes", [] {
    const Bytes real = real_module();
    // not PKLITE at all
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::read(Bytes(100, 0x90)));
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::stream_start(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::read(Bytes{}));
    // cut anywhere: through the entry code, through the stream, through the relocation table
    int threw = 0, cuts = 0;
    for (std::size_t cut : {5u, 20u, 100u, 0x44u, 0x200u, 656u, 657u, 700u, 1000u, 20000u, 45000u, 45572u, 45573u, 45574u, 45800u,
                            46000u, 46500u, 46527u}) {
        ++cuts;
        try {
            (void)pklite::read(ByteView(real.data(), cut));
        } catch (const FormatError&) {
            ++threw;
        }
    }
    CB_CHECK_EQ(threw, cuts);   // the stream is cut in every one of them, or the table, or the registers
    // the entry code names a stream start before the module
    Bytes entry = {0x8C, 0xDB, 0x53, 0x81, 0xC3, 0x03, 0x00};
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::stream_start(entry));   // 3 * 16 - 0x100 < 0
    // a copy from before the start of the program
    const pklite::Stub stub = pklite::Stub::of(real, pklite::stream_start(real));
    const std::vector<Token> bad = {Token::copy(5, 9), Token::end()};
    Bytes stream = pklite::encode(bad, stub);
    Bytes module(real.begin(), real.begin() + 656);
    module.insert(module.end(), stream.begin(), stream.end());
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::read(module));
    // a length the decompressor has no code for
    CB_CHECK_THROWS_AS(UnsupportedError, (void)pklite::encode(std::vector<Token>{Token::copy(26, 1)}, pklite::Stub{}));
});

CB_TEST("build.pklite.a_program_that_does_not_fit_is_unsupported", [] {
    const Bytes real = real_module();
    const ByteView head(real.data(), pklite::stream_start(real));
    const Bytes image(100, 0);
    // the memory it asks for does not fit the entry code's 16-bit field
    CB_CHECK_THROWS_AS(UnsupportedError, (void)pklite::pack(image, {}, {}, head, 70000));
    // a head that is not even the entry code
    CB_CHECK_THROWS_AS(FormatError, (void)pklite::pack(image, {}, {}, Bytes(3, 0), 0));
});
