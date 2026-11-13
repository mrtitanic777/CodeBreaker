// Tests of cb/formats/xbox/pck.hpp on a 286-byte container made for the purpose: a property, four entries (a PNG in a folder, a file with
// a backslash in its name, an empty one, a last payload the file cuts one byte short) and the padding the format shows between payloads.
// The Python's pck.parse / pck.payloads (state/work/typed-port/xbox-deferred/vol_golden.py) give: version 3, properties [(7, 'ver')], the entries
// (33, 0, 'tex/a.png') (35, 1, 'b\\c.loc') (0, 2, 'empty') (40, 3, 'd.grf'), table_end 146; the payloads tex_a.png (33), b_c.loc (35) and
// d.grf (39 of 40), reading stopping at 287 of 286 bytes.
//
// DIVERGES from the Python: a table that runs past the end of the file is TruncatedError (struct.error or a clamped name followed by one).
#include "cb/formats/xbox/pck.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace pck_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes sample() {
    return crypto::from_hex(
        "000000030000000100000007000000030076006500720000000000000004000000210000000000000009007400650078002f0061002e0070006e0067"
        "000000000000002300000001000000070062005c0063002e006c006f0063000000000000000000000002000000050065006d007000740079000000"
        "000000002800000003000000050064002e0067007200660000000000000000000000000000000089504e470d0a1a0a494844520000000000000000"
        "000000000049454e4400000000000000004c4f43444154414c4f43444154414c4f43444154414c4f43444154414c4f434441544100000000000000"
        "000000000000000000000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2021222324252627");
}

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

CB_TEST("formats.xbox.pck.the_table_reads_as_the_pythons_does", [] {
    const Bytes file = sample();
    CB_CHECK_EQ(file.size(), 286u);
    const pck::Archive a = pck::parse(file);
    CB_CHECK_EQ(a.version, 3u);
    CB_CHECK_EQ(a.properties.size(), 1u);
    CB_CHECK_EQ(a.properties[0].first, 7u);
    CB_CHECK_EQ(a.properties[0].second, "ver");
    CB_CHECK_EQ(a.entries.size(), 4u);
    CB_CHECK_EQ(a.entries[0].size, 33u);
    CB_CHECK_EQ(a.entries[0].type, 0u);
    CB_CHECK_EQ(a.entries[0].name, "tex/a.png");
    CB_CHECK_EQ(a.entries[1].name, "b\\c.loc");
    CB_CHECK_EQ(a.entries[2].size, 0u);
    CB_CHECK_EQ(a.entries[3].type, 3u);
    CB_CHECK_EQ(a.table_end, 146u);
});

CB_TEST("formats.xbox.pck.payloads_are_found_past_their_padding", [] {
    const Bytes file = sample();
    const pck::Payloads p = pck::payloads(file);
    CB_CHECK_EQ(p.found.size(), 3u);   // the empty entry has none
    CB_CHECK_EQ(p.found[0].name, "tex_a.png");   // '/' made '_'
    CB_CHECK_EQ(p.found[0].size, 33u);
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(p.found[0].offset, p.found[0].size)), "637f7e4314d99ae81cf1b1983f6537a7b146c0c9");
    CB_CHECK_EQ(p.found[1].name, "b_c.loc");     // '\\' made '_'
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(p.found[1].offset, p.found[1].size)), "667832bc4767a398ec994b9c4255cd3eb01eb91a");
    CB_CHECK_EQ(p.found[2].name, "d.grf");
    CB_CHECK_EQ(p.found[2].size, 39u);           // one byte short: the file ends there
    CB_CHECK_EQ(sha1_hex(ByteView(file).subspan(p.found[2].offset, p.found[2].size)), "86065e5e4249c0ee7157c72f0e86ff60d5fe2fb0");
    CB_CHECK_EQ(p.end, 287u);
    CB_CHECK_EQ(p.file_size, 286u);
});

CB_TEST("formats.xbox.pck.a_table_the_file_cuts_short_is_truncated", [] {
    const Bytes file = sample();
    CB_CHECK_THROWS_AS(TruncatedError, (void)pck::parse(ByteView(file).subspan(0, 4)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)pck::parse(ByteView(file).subspan(0, 60)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)pck::parse(ByteView(file).subspan(0, 120)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)pck::parse(Bytes{}));
    // a property name of more than 8192 units
    Bytes big = file;
    big[8 + 4] = 0x00;
    big[8 + 5] = 0x00;
    big[8 + 6] = 0xFF;
    big[8 + 7] = 0xFF;
    CB_CHECK_THROWS_AS(FormatError, (void)pck::parse(big));
});

}  // namespace pck_test
