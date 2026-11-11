// Tests of the EXEPACK writer and reader. The packed stream is checked by an independent unpacker written here
// from the format's description (the reverse of what the packer wrote), not by the packer's own reader alone.
#include "cb/build/exepack.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

Bytes stub_code() {
    Bytes code(20, 0x90);
    append(code, as_bytes(exepack::kMessage));
    return code;
}

std::uint16_t word(const Bytes& b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }

// What the real unpacker does: the stored program's first bytes, then the commands read backwards from the
// header's segment (`cs`:0), writing the program from its end downwards; and the relocation table behind the stub.
struct Unpacked {
    Bytes program;
    std::vector<std::uint32_t> relocs;
};
Unpacked unpack(const Bytes& module, std::uint16_t cs, unsigned words) {
    const std::size_t seg = std::size_t{cs} * 16;
    const std::size_t paragraphs = word(module, seg + 12);
    const std::size_t own = word(module, seg + 6);
    Unpacked out;
    out.program.assign(paragraphs * 16, 0xEE);
    std::size_t src = seg;
    std::size_t dst = out.program.size();
    while (module[src - 1] == 0xFF) --src;   // the padding
    for (;;) {
        const std::uint8_t command = module[src - 1];
        const std::size_t length = word(module, src - 3);
        src -= 3;
        if ((command & 0xFE) == 0xB0) {
            const std::uint8_t b = module[--src];
            dst -= length;
            for (std::size_t i = 0; i < length; ++i) out.program[dst + i] = b;
        } else {
            CB_CHECK_EQ(command & 0xFE, 0xB2);
            src -= length;
            dst -= length;
            for (std::size_t i = 0; i < length; ++i) out.program[dst + i] = module[src + i];
        }
        if (command & 1) break;
    }
    CB_CHECK_EQ(src, dst);                          // the program's first bytes were stored as they are
    for (std::size_t i = 0; i < src; ++i) out.program[i] = module[i];
    // the table: the last 16 lists of the segment (count, offsets) -- walk back from the segment's end
    std::size_t at = seg + own;
    // find where the lists start: read forwards from after the stub code (known length)
    const std::size_t table_at = seg + 2 * words + stub_code().size();
    std::size_t p = table_at;
    for (std::uint32_t window = 0; window < 16; ++window) {
        const std::size_t n = word(module, p);
        p += 2;
        for (std::size_t i = 0; i < n; ++i, p += 2) out.relocs.push_back((window << 16) | word(module, p));
    }
    CB_CHECK_EQ(p, at);
    return out;
}

Bytes sample_image() {
    Bytes image;
    for (int i = 0; i < 1024; ++i) image.push_back(static_cast<std::uint8_t>(i));
    image.insert(image.end(), 300, 0);
    image.insert(image.end(), 40, 0x41);
    for (int i = 0; i < 64; ++i) image.push_back(static_cast<std::uint8_t>(i));
    image.insert(image.end(), 16, 0xFF);
    return image;
}

const std::vector<exepack::Fill> kSampleFills{{1024, 300, 0}, {1324, 40, 0x41}, {1428, 16, 0xFF}};

}  // namespace

CB_TEST("build.exepack.pack_roundtrip_both_headers", [] {
    // the program of tests/test_exepack.py: runs of 0, 'A' and FF written as iterated data
    const Bytes image = sample_image();
    const std::vector<std::uint32_t> relocs{0x10, 0x20, 0x1F0, 0x10005, 0x3FFFF};
    for (const unsigned words : {8u, 9u}) {
        const exepack::Packed p = exepack::pack(image, relocs, {3, 0x44}, {0x50, 0x200}, kSampleFills, stub_code(), words);
        // the file's header: the unpacker's own entry, behind the stored program
        CB_CHECK_EQ(p.registers.ip, 2 * words);
        CB_CHECK_EQ(p.registers.sp, 0x80);
        // the program is stored padded to a whole paragraph (1444 bytes become 1456), the padding zero
        const std::size_t padded = (image.size() + 15) / 16 * 16;
        Bytes expected = image;
        expected.resize(padded, 0);
        const Unpacked u = unpack(p.module, p.registers.cs, words);
        CB_CHECK(u.program == expected);
        CB_CHECK(u.relocs == relocs);
        // and the packer's own reader recovers how it was packed
        const exepack::Layout l = exepack::layout(p.module, p.registers.cs, p.registers.ip);
        CB_CHECK(l.fills == kSampleFills);
        CB_CHECK(l.stub_code == stub_code());
        CB_CHECK_EQ(l.size, padded);
        CB_CHECK_EQ(l.words, words);
        // the header words: the program's IP, CS, scratch, own size, SP, SS, paragraphs, [skip count], "RB"
        const std::size_t seg = std::size_t{p.registers.cs} * 16;
        CB_CHECK_EQ(word(p.module, seg + 0), 0x44);
        CB_CHECK_EQ(word(p.module, seg + 2), 3);
        CB_CHECK_EQ(word(p.module, seg + 8), 0x200);
        CB_CHECK_EQ(word(p.module, seg + 10), 0x50);
        CB_CHECK_EQ(word(p.module, seg + 12), padded / 16);
        CB_CHECK_EQ(word(p.module, seg + 2 * words - 2), 0x4252);
    }
});

CB_TEST("build.exepack.stream_layout_is_exact", [] {
    // 16 bytes: 1 literal byte, then a 15-byte fill. The stream is [literal][fill byte][len=15][B0|last], padded
    // with FF to the paragraph; the unpacker segment follows.
    Bytes image(16, 0x77);
    image[0] = 0x12;
    const exepack::Packed p = exepack::pack(image, {}, {0, 0}, {0, 0}, std::vector<exepack::Fill>{{1, 15, 0x77}}, stub_code());
    CB_CHECK_EQ(p.registers.cs, 1);   // the stored program is one paragraph
    CB_CHECK_EQ(p.module[0], 0x12);
    CB_CHECK_EQ(p.module[1], 0x77);
    CB_CHECK_EQ(word(p.module, 2), 15);
    CB_CHECK_EQ(p.module[4], 0xB1);   // fill, last command
    for (std::size_t i = 5; i < 16; ++i) CB_CHECK_EQ(p.module[i], 0xFF);
    CB_CHECK_EQ(p.module[16 + 12], 1);   // one paragraph of program
    // without fills it is a plain copy, B2|last
    const exepack::Packed q = exepack::pack(image, {}, {0, 0}, {0, 0}, {}, stub_code());
    CB_CHECK_EQ(q.module[16], 0x10);
    CB_CHECK_EQ(q.module[17], 0x00);
    CB_CHECK_EQ(q.module[18], 0xB3);
    CB_CHECK_EQ(unpack(q.module, q.registers.cs, 8).program == image, true);
});

CB_TEST("build.exepack.long_runs_and_copies_split_at_64k", [] {
    Bytes image(0x20004, 0x07);
    for (std::size_t i = 0; i < 100; ++i) image[i] = static_cast<std::uint8_t>(i + 1);
    for (std::size_t i = 0x20000; i < 0x20004; ++i) image[i] = static_cast<std::uint8_t>(i);
    // two runs, the second the rest of the 64 KB-bounded run structure; the 4 bytes behind are copied
    const std::vector<exepack::Fill> fills{{100, 0xFFFF, 7}, {100 + 0xFFFF, 0x20000 - 100 - 0xFFFF, 7}};
    const exepack::Packed p = exepack::pack(image, {}, {1, 2}, {3, 4}, fills, stub_code());
    const Unpacked u = unpack(p.module, p.registers.cs, 8);
    CB_CHECK_EQ(u.program.size(), 0x20010u);   // padded to a paragraph
    CB_CHECK(std::equal(image.begin(), image.end(), u.program.begin()));
    const exepack::Layout l = exepack::layout(p.module, p.registers.cs, p.registers.ip);
    CB_CHECK(l.fills == fills);

    // DIVERGES: a program with no fill was stored with no command at all, a stream the unpacker cannot end on;
    // it is now copy commands alone, one per 64 KB at most.
    Bytes plain(0x18000);
    for (std::size_t i = 0; i < plain.size(); ++i) plain[i] = static_cast<std::uint8_t>(i * 7 + (i >> 8));
    const exepack::Packed q = exepack::pack(plain, {}, {1, 2}, {3, 4}, {}, stub_code());
    CB_CHECK(unpack(q.module, q.registers.cs, 8).program == plain);
    CB_CHECK(exepack::layout(q.module, q.registers.cs, q.registers.ip).fills.empty());
});

CB_TEST("build.exepack.pack_rejects_what_is_not_a_fill", [] {
    const Bytes image = sample_image();
    auto pack = [&](std::vector<exepack::Fill> fills, unsigned words = 8) {
        return exepack::pack(image, {}, {1, 2}, {3, 4}, fills, stub_code(), words);
    };
    CB_CHECK_THROWS_AS(UsageError, (void)pack({{0, 10, 0}}));                  // not a run of zeros
    CB_CHECK_THROWS_AS(UsageError, (void)pack({{1024, 301, 0}}));              // one byte too many
    CB_CHECK_THROWS_AS(UsageError, (void)pack({{1024, 0, 0}}));                // no bytes
    CB_CHECK_THROWS_AS(UsageError, (void)pack({{100000, 5, 0}}));              // past the program
    CB_CHECK_THROWS_AS(UsageError, (void)pack({{1024, 20, 0}, {1030, 20, 0}}));   // overlapping (the port wrote garbage)
    CB_CHECK_THROWS_AS(UsageError, (void)pack({}, 7));
    CB_CHECK_THROWS_AS(UsageError, (void)pack({}, 10));
    // fills in any order are sorted
    CB_CHECK_NOTHROW((void)pack({{1428, 16, 0xFF}, {1024, 300, 0}}));
    // values that do not fit their words
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::pack(Bytes(0x10010 * 16, 0), {}, {1, 2}, {3, 4}, {}, stub_code()));
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::pack(image, std::vector<std::uint32_t>{16u << 16}, {1, 2}, {3, 4}, {}, stub_code()));
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::pack(image, {}, {1, 2}, {3, 4}, {}, Bytes(70000, 0)));
});

CB_TEST("build.exepack.layout_rejects_what_is_not_exepack", [] {
    const Bytes image = sample_image();
    const exepack::Packed p = exepack::pack(image, {}, {3, 0x44}, {0x50, 0x200}, kSampleFills, stub_code());
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::layout(p.module, p.registers.cs, 0x14));          // no such header
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::layout(p.module, p.registers.cs, 0x12));          // 9 words, but it has 8
    CB_CHECK_THROWS_AS(TruncatedError, (void)exepack::layout(Bytes(8, 0), 0, 0x10));                 // too short
    Bytes broken = p.module;
    broken[std::size_t{p.registers.cs} * 16 + 14] = 'X';                                             // "RB" damaged
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::layout(broken, p.registers.cs, p.registers.ip));
    Bytes bad_command = p.module;
    bad_command[std::size_t{p.registers.cs} * 16 - 1] = 0xFF - 0x80;                                 // an unknown command byte
    bad_command[std::size_t{p.registers.cs} * 16 - 2] = 0x00;
    CB_CHECK_THROWS_AS(Error, (void)exepack::layout(bad_command, p.registers.cs, p.registers.ip));
    Bytes no_message = p.module;
    for (std::size_t i = std::size_t{p.registers.cs} * 16; i + 4 < no_message.size(); ++i) {
        if (no_message[i] == 'P' && no_message[i + 1] == 'a' && no_message[i + 2] == 'c') no_message[i] = 'Q';
    }
    CB_CHECK_THROWS_AS(FormatError, (void)exepack::layout(no_message, p.registers.cs, p.registers.ip));
});
