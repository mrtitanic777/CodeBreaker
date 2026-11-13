// Tests of cb/formats/xbox/xzp.hpp on a v3 (dashboard) and a v1 (Minecraft) container made for the purpose; the Python's xzp.parse and
// xzp.payloads (state/work/xbox-deferred/vol_golden.py) give for the v3: version 3, size 135, base 93, table end 93, entries
// (18, 0, '..\\media\\a.png') (21, 18, 'skin\\b.xur') (2, 39, 'c:evil') (1, 41, '..\\..'), payloads media/a.png and skin/b.xur (the entry
// with a colon and the one that is only `..` parts are dropped); for the v1: version 1, size 134, base 97, entries
// (17, 0, 'media\\a.png') (20, 17, 'media\\de-DE\\s.xus').
//
// DIVERGES from the Python: a header cut short is FormatError (struct.error / IndexError).
#include "cb/formats/xbox/xzp.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace xzp_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes v3_file() {
    return crypto::from_hex(
        "5855495a00000003000000870000000000000047000400000012000000000e2e2e5c6d656469615c612e706e6700000015000000120a736b696e5c622e"
        "787572000000020000002706633a6576696c0000000100000029052e2e5c2e2e89504e470d0a1a0a787878787878787878785855525855525855525855"
        "525855525855525855527a7a71");
}

static Bytes v1_file() {
    return crypto::from_hex(
        "5855495a0000000100000086000000000000004b000000000011000000000b006d0065006400690061005c0061002e0070006e0067000000140000001111"
        "006d0065006400690061005c00640065002d00440045005c0073002e0078007500730089504e470d0a1a0a7979797979797979797465787474657874746578"
        "747465787474657874");
}

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

CB_TEST("formats.xbox.xzp.a_v3_pack_reads_as_the_pythons_does", [] {
    const Bytes file = v3_file();
    const xzp::Pack pack = xzp::parse(file);
    CB_CHECK_EQ(pack.version, 3);
    CB_CHECK_EQ(pack.file_size, 135u);
    CB_CHECK_EQ(pack.base, 93u);
    CB_CHECK_EQ(pack.table_end, 93u);
    CB_CHECK_EQ(pack.entries.size(), 4u);
    CB_CHECK_EQ(pack.entries[0].size, 18u);
    CB_CHECK_EQ(pack.entries[0].offset, 0u);
    CB_CHECK_EQ(pack.entries[0].name, "..\\media\\a.png");
    CB_CHECK_EQ(pack.entries[1].name, "skin\\b.xur");
    CB_CHECK_EQ(pack.entries[1].offset, 18u);
    CB_CHECK_EQ(pack.entries[2].name, "c:evil");
    CB_CHECK_EQ(pack.entries[3].name, "..\\..");

    const std::vector<xzp::Payload> payloads = xzp::payloads(file);
    CB_CHECK_EQ(payloads.size(), 2u);   // the colon and the `..`-only paths are dropped
    CB_CHECK_EQ(payloads[0].path, "media/a.png");
    CB_CHECK_EQ(payloads[0].size, 18u);
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(payloads[0].offset, payloads[0].size)), "9400754922a1ed3063249f1bbd2079c969bab5d8");
    CB_CHECK_EQ(payloads[1].path, "skin/b.xur");
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(payloads[1].offset, payloads[1].size)), "619d126d92e557fbfd0f3d85b9df4d1fa189de3e");
});

CB_TEST("formats.xbox.xzp.a_v1_pack_reads_as_the_pythons_does", [] {
    const Bytes file = v1_file();
    const xzp::Pack pack = xzp::parse(file);
    CB_CHECK_EQ(pack.version, 1);
    CB_CHECK_EQ(pack.file_size, 134u);
    CB_CHECK_EQ(pack.base, 97u);
    CB_CHECK_EQ(pack.table_end, 97u);
    CB_CHECK_EQ(pack.entries.size(), 2u);
    CB_CHECK_EQ(pack.entries[0].size, 17u);
    CB_CHECK_EQ(pack.entries[0].name, "media\\a.png");
    CB_CHECK_EQ(pack.entries[1].size, 20u);
    CB_CHECK_EQ(pack.entries[1].offset, 17u);
    CB_CHECK_EQ(pack.entries[1].name, "media\\de-DE\\s.xus");
    const std::vector<xzp::Payload> payloads = xzp::payloads(file);
    CB_CHECK_EQ(payloads.size(), 2u);
    CB_CHECK_EQ(payloads[0].path, "media/a.png");
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(payloads[0].offset, payloads[0].size)), "73e26ca693eb01377f876eeee76c61ff2160c174");
    CB_CHECK_EQ(payloads[1].path, "media/de-DE/s.xus");
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(payloads[1].offset, payloads[1].size)), "3a8521aec2ef9a44e4f0cd246fe1fb8445ce90f8");
});

CB_TEST("formats.xbox.xzp.a_file_that_is_not_xuiz_or_is_cut_short", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)xzp::parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)xzp::parse(to_bytes("PIRS and more bytes than a header")));
    const Bytes file = v3_file();
    CB_CHECK_THROWS_AS(FormatError, (void)xzp::parse(ByteView(file).subspan(0, 10)));
    // a v3 table that the file ends inside gives the entries before it; a v1 table stops at its first entry that does not hold together
    const xzp::Pack cut = xzp::parse(ByteView(file).subspan(0, 50));
    CB_CHECK(cut.entries.size() < 4u);
    Bytes v1 = v1_file();
    v1[0x17 + 7] = 0;   // a name of no characters
    v1[0x17 + 8] = 0;
    CB_CHECK(xzp::parse(v1).entries.empty());
    // an entry whose payload lies outside the file ends a v1 table
    Bytes outside = v1_file();
    outside[0x17 + 3] = 0x7F;
    CB_CHECK(xzp::parse(outside).entries.empty());
});

}  // namespace xzp_test
