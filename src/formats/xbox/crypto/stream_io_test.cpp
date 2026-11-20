// Tests of cb/formats/xbox/crypto/stream_io.hpp. The expected bytes and values were recorded from a reference run of the same calls.
//
// Behaviour pinned here: a short integer read is TruncatedError;
// `read(n)` still answers what is there; a name never labelled and a seek before the start are UsageError.
#include "formats/xbox/crypto/stream_io.hpp"

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace stream_io_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;

CB_TEST("formats.xbox.crypto.stream_io.big_endian_integers_and_a_gap", [] {
    StreamIO s(Endian::Big);
    s.write_u32(0xDEADBEEF);
    s.write_u16(0x1234);
    s.write_i16(-2);
    s.write_u64(0x0102030405060708ull);
    s.write_i32(-5);
    s.write_u8(0xAB);
    s.write_i8(-1);
    s.write_i64(-7);
    s.write_at(0x40, as_bytes("tail"));   // past the end: zero-filled between
    CB_CHECK_EQ(s.position(), 30u);       // (a write at an offset leaves the position)
    CB_CHECK_EQ(s.length(), 68u);
    const Bytes all = s.contents();
    CB_CHECK_EQ(cb::crypto::to_hex(ByteView(all).subspan(0, 30)), "deadbeef1234fffe0102030405060708fffffffbabfffffffffffffffff9");
    for (std::size_t i = 30; i < 0x40; ++i) CB_CHECK_EQ(all[i], 0);   // the gap
    CB_CHECK_EQ(cb::crypto::to_hex(ByteView(all).subspan(0x40)), "7461696c");

    s.seek(0);
    CB_CHECK_EQ(s.read_u32(), 0xDEADBEEFu);
    CB_CHECK_EQ(s.read_u16(), 0x1234);
    CB_CHECK_EQ(s.read_i16(), -2);
    CB_CHECK_EQ(s.read_u64(), 0x0102030405060708ull);
    CB_CHECK_EQ(s.read_i32(), -5);
    CB_CHECK_EQ(s.read_u8(), 0xAB);
    CB_CHECK_EQ(s.read_i8(), -1);
    CB_CHECK_EQ(s.read_i64(), -7);
    CB_CHECK_EQ(s.position(), 30u);
});

CB_TEST("formats.xbox.crypto.stream_io.labels_and_functions_at_an_offset", [] {
    StreamIO s(Endian::Big);
    s.write_u64(0x0102030405060708ull);
    s.set_label("here", 6);
    s.goto_label("here", 2);
    CB_CHECK_EQ(s.position(), 8u);
    CB_CHECK_EQ(s.label("here"), 6u);
    s.seek(0);
    const Bytes flipped = s.perform_function_at(0, 4, [](ByteView b) {
        Bytes out(b.begin(), b.end());
        for (auto& x : out) x ^= 0xFF;
        return out;
    });
    CB_CHECK_EQ(cb::crypto::to_hex(flipped), "fefdfcfb");
    CB_CHECK_EQ(s.position(), 0u);   // left where it was
    CB_CHECK_EQ(cb::crypto::to_hex(s.read_at(0, 8)), "fefdfcfb05060708");
    CB_CHECK_EQ(s.read_u32_at(4), 0x05060708u);
    s.write_u16_at(0, 0x1234);
    CB_CHECK_EQ(s.read_u16_at(0), 0x1234);
    CB_CHECK_EQ(s.position(), 0u);
});

CB_TEST("formats.xbox.crypto.stream_io.little_endian_and_short_reads", [] {
    const Bytes eight = {1, 2, 3, 4, 5, 6, 7, 8};
    StreamIO s(eight, Endian::Little);
    CB_CHECK_EQ(s.read_u32(), 0x04030201u);
    CB_CHECK_EQ(s.read_u16(), 0x0605);
    CB_CHECK_EQ(s.read_u8(), 7);
    CB_CHECK_EQ(cb::crypto::to_hex(s.read(5)), "08");   // what is there
    CB_CHECK(s.read(5).empty());
    CB_CHECK_THROWS_AS(TruncatedError, (void)s.read_u16());
    CB_CHECK_THROWS_AS(TruncatedError, (void)s.read_exact(1));
    s.seek(-2, Whence::End);
    CB_CHECK_EQ(s.read_u16(), 0x0807);
    CB_CHECK_THROWS_AS(UsageError, (void)s.seek(-1));
    CB_CHECK_THROWS_AS(UsageError, (void)s.label("never set"));
});

CB_TEST("formats.xbox.crypto.stream_io.structures_as_they_lie", [] {
    struct Header {
        std::uint8_t magic[4];
        std::uint32_t size;   // (this machine's order: a real record uses its own field types)
    };
    StreamIO s;
    const Header h{{'X', 'E', 'X', '2'}, 0x1000};
    s.write_struct(h);
    CB_CHECK_EQ(s.length(), sizeof(Header));
    s.seek(0);
    const Header back = s.read_struct<Header>();
    CB_CHECK_EQ(back.size, 0x1000u);
    CB_CHECK_EQ(back.magic[3], '2');
    CB_CHECK_THROWS_AS(TruncatedError, (void)s.read_struct<Header>());
});

CB_TEST("formats.xbox.crypto.stream_io.a_file_stream_edits_in_place", [] {
    fs::ScopedTempDir dir("cb-streamio");
    const auto path = dir.path() / "image.bin";
    {
        StreamIO created = StreamIO::open(path, Endian::Big);   // not there: made
        created.write_u32(0x01020304);
        created.write_u32(0x05060708);
    }
    CB_CHECK_EQ(fs::read_file(path).size(), 8u);
    {
        StreamIO existing = StreamIO::open(path, Endian::Big);
        CB_CHECK_EQ(existing.length(), 8u);
        CB_CHECK_EQ(existing.read_u32_at(4), 0x05060708u);
        existing.write_u16_at(2, 0xAAAA);
    }
    CB_CHECK_EQ(cb::crypto::to_hex(fs::read_file(path)), "0102aaaa05060708");
});

}  // namespace stream_io_test
