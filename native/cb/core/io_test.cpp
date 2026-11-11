#include "cb/core/io.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::io;

namespace {
const Bytes kData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
}

CB_TEST("core.io.reader_little_endian", [] {
    ByteReader r(kData);
    CB_CHECK_EQ(r.u8(), 0x01);
    CB_CHECK_EQ(r.u16(), 0x0302);
    CB_CHECK_EQ(r.u32(), 0x07060504u);
    CB_CHECK_EQ(r.pos(), 7u);
    CB_CHECK_EQ(r.remaining(), 3u);
    r.seek(0);
    CB_CHECK_EQ(r.u64(), 0x0807060504030201ull);
});

CB_TEST("core.io.reader_big_endian", [] {
    ByteReader r(kData);
    CB_CHECK_EQ(r.u16_be(), 0x0102);
    CB_CHECK_EQ(r.u32_be(), 0x03040506u);
    r.seek(0);
    CB_CHECK_EQ(r.u64_be(), 0x0102030405060708ull);
    r.seek(0);
    CB_CHECK_EQ(r.i16_be(), 0x0102);
});

CB_TEST("core.io.reader_signed", [] {
    const Bytes d = {0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80};
    ByteReader r(d);
    CB_CHECK_EQ(r.i8(), -1);
    CB_CHECK_EQ(r.i16(), static_cast<std::int16_t>(0xFFFE));
    r.seek(0);
    CB_CHECK_EQ(r.i32(), static_cast<std::int32_t>(0xFFFFFEFFu));
    const Bytes ones(8, 0xFF);
    CB_CHECK_EQ(ByteReader(ones).i64(), -1);
});

CB_TEST("core.io.reader_far16", [] {
    const Bytes d = {0x34, 0x12, 0x00, 0xB8};   // offset 0x1234, segment 0xB800
    ByteReader r(d);
    Far16 p = r.far16();
    CB_CHECK_EQ(p.off, 0x1234);
    CB_CHECK_EQ(p.seg, 0xB800);
    CB_CHECK_EQ(p.linear(), 0xB8000u + 0x1234u);
});

CB_TEST("core.io.reader_bytes_and_cstr", [] {
    const Bytes d = {'a', 'b', 'c', 0, 'd', 'e', 0, 'x'};
    ByteReader r(d);
    ByteView s1 = r.cstr();
    CB_CHECK_EQ(as_text(s1), "abc");
    CB_CHECK_EQ(r.pos(), 4u);
    ByteView s2 = r.cstr();
    CB_CHECK_EQ(as_text(s2), "de");
    CB_CHECK_THROWS_AS(TruncatedError, r.cstr());   // "x" has no terminator
    r.seek(0);
    CB_CHECK_EQ(r.bytes(2).size(), 2u);
    CB_CHECK_EQ(r.bytes_copy(2)[1], 0);
});

CB_TEST("core.io.reader_truncated_everywhere", [] {
    // every width fails with TruncatedError when the data is one byte short
    const Bytes d = {1, 2, 3, 4, 5, 6, 7};
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(d).u64());
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(ByteView(d).first(3)).u32());
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(ByteView(d).first(1)).u16());
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(ByteView{}).u8());
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(ByteView(d).first(1)).u16_be());
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(d).bytes(8));
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(d).bytes(0xFFFFFFFFFFFFFFFFull));   // no overflow tricks
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(d).skip(8));
    CB_CHECK_THROWS_AS(TruncatedError, ByteReader(ByteView(d).first(3)).far16());
});

CB_TEST("core.io.reader_error_details", [] {
    ByteReader r(kData, "ne.header");
    r.skip(8);
    try {
        (void)r.u32();
        CB_CHECK(false);
    } catch (const TruncatedError& e) {
        CB_CHECK_EQ(e.offset(), 8u);
        CB_CHECK_EQ(e.wanted(), 4u);
        CB_CHECK_EQ(e.available(), 2u);
        CB_CHECK_EQ(e.reader(), "ne.header");
    }
    CB_CHECK_EQ(r.pos(), 8u);   // a failed read does not move the cursor
});

CB_TEST("core.io.reader_try_forms", [] {
    ByteReader r(ByteView(kData).first(3));
    CB_CHECK_EQ(*r.try_u8(), 1);
    CB_CHECK_EQ(*r.try_u16(), 0x0302);
    CB_CHECK(!r.try_u8().has_value());
    CB_CHECK(!r.try_u32().has_value());
    CB_CHECK_EQ(r.pos(), 3u);
    r.seek(0);
    CB_CHECK(!r.try_u32().has_value());
    CB_CHECK_EQ(r.pos(), 0u);   // unmoved
    CB_CHECK(!r.try_bytes(4).has_value());
    CB_CHECK_EQ(r.try_bytes(3)->size(), 3u);
    r.seek(0);
    CB_CHECK_EQ(*r.peek_u8(), 1);
    CB_CHECK_EQ(*r.peek_u16(), 0x0201);
    CB_CHECK_EQ(r.pos(), 0u);
});

CB_TEST("core.io.reader_seek_align_sub", [] {
    ByteReader r(kData);
    r.seek(10);   // seeking to the end is allowed
    CB_CHECK(r.eof());
    CB_CHECK_THROWS_AS(TruncatedError, r.seek(11));
    r.seek(3);
    r.align(4);
    CB_CHECK_EQ(r.pos(), 4u);
    r.align(4);
    CB_CHECK_EQ(r.pos(), 4u);
    ByteReader inner = r.sub(4, "inner");
    CB_CHECK_EQ(r.pos(), 8u);
    CB_CHECK_EQ(inner.u8(), 5);
    CB_CHECK_EQ(inner.size(), 4u);
    CB_CHECK_THROWS_AS(TruncatedError, inner.u64());
    CB_CHECK_THROWS_AS(TruncatedError, r.sub(3));
    CB_CHECK_THROWS_AS(InternalError, r.align(3));
});

CB_TEST("core.io.checked_range", [] {
    CB_CHECK_EQ(checked_range(kData, 2, 3).size(), 3u);
    CB_CHECK_EQ(checked_range(kData, 10, 0).size(), 0u);    // empty range at the very end is fine
    CB_CHECK_EQ(checked_range(kData, 0, 10).size(), 10u);
    CB_CHECK_THROWS_AS(FormatError, checked_range(kData, 0, 11));
    CB_CHECK_THROWS_AS(FormatError, checked_range(kData, 11, 0));
    CB_CHECK_THROWS_AS(FormatError, checked_range(kData, 8, 3));
    // offsets near 2^64 must not wrap around
    CB_CHECK_THROWS_AS(FormatError, checked_range(kData, 0xFFFFFFFFFFFFFFFFull, 2));
    CB_CHECK_THROWS_AS(FormatError, checked_range(kData, 2, 0xFFFFFFFFFFFFFFFFull));
    CB_CHECK(!try_range(kData, 5, 6).has_value());
    CB_CHECK(try_range(kData, 5, 5).has_value());
});

CB_TEST("core.io.writer_roundtrip", [] {
    ByteWriter w;
    w.u8(0xAB);
    w.u16(0x1234);
    w.u32(0xDEADBEEF);
    w.u64(0x0102030405060708ull);
    w.i16(-2);
    w.u16_be(0x1234);
    w.u32_be(0x01020304);
    w.far16(Far16{0xB800, 0x0010});
    w.cstr("hi");
    w.zeros(2);
    ByteReader r(w.data());
    CB_CHECK_EQ(r.u8(), 0xAB);
    CB_CHECK_EQ(r.u16(), 0x1234);
    CB_CHECK_EQ(r.u32(), 0xDEADBEEFu);
    CB_CHECK_EQ(r.u64(), 0x0102030405060708ull);
    CB_CHECK_EQ(r.i16(), -2);
    CB_CHECK_EQ(r.u16_be(), 0x1234);
    CB_CHECK_EQ(r.u32_be(), 0x01020304u);
    Far16 p = r.far16();
    CB_CHECK(p == (Far16{0xB800, 0x0010}));
    CB_CHECK_EQ(as_text(r.cstr()), "hi");
    CB_CHECK_EQ(r.remaining(), 2u);
});

CB_TEST("core.io.writer_patch_and_align", [] {
    ByteWriter w;
    w.u32(0);          // size placeholder
    w.cstr("body");
    w.patch_u32(0, static_cast<std::uint32_t>(w.size()));
    ByteReader r(w.data());
    CB_CHECK_EQ(r.u32(), 9u);
    w.align(8);
    CB_CHECK_EQ(w.size(), 16u);
    w.patch_u16(0, 0xBEEF);
    CB_CHECK_EQ(w.data()[0], 0xEF);
    CB_CHECK_EQ(w.data()[1], 0xBE);
    CB_CHECK_THROWS_AS(InternalError, w.patch_u32(14, 1));   // past the written bytes
    Bytes taken = w.take();
    CB_CHECK_EQ(taken.size(), 16u);
    CB_CHECK_EQ(w.size(), 0u);
});
