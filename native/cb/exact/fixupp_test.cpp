#include "cb/exact/fixupp.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact::fixupp;

namespace {

ByteView payload_of(const Bytes& record) {
    // a record is type, length word, payload, checksum
    return ByteView(record).subspan(3, record.size() - 4);
}

}  // namespace

CB_TEST("exact.fixupp.index_forms", [] {
    CB_CHECK((index_bytes(0x7F) == Bytes{0x7F}));
    CB_CHECK((index_bytes(0x80) == Bytes{0x80, 0x80}));
    CB_CHECK((index_bytes(0x1234) == Bytes{0x92, 0x34}));
    CB_CHECK_THROWS_AS(FormatError, index_bytes(0x8000));
    const Bytes two{0x92, 0x34, 0x05};
    const Index i = read_index(two, 0);
    CB_CHECK_EQ(i.value, 0x1234);
    CB_CHECK_EQ(i.next, std::size_t{2});
    CB_CHECK_EQ(read_index(two, 2).value, 5);
    CB_CHECK_THROWS_AS(TruncatedError, read_index(Bytes{0x92}, 0));
    CB_CHECK_THROWS_AS(TruncatedError, read_index(two, 3));
});

CB_TEST("exact.fixupp.parse_explicit_fixup", [] {
    // segment-relative (M), location 3 (pointer), offset 0x105; frame by segment index 2, target by external index
    // 0x81 (a two-byte index), with a displacement
    const Bytes r{0xCD /*1 1 0011 01*/, 0x05, static_cast<std::uint8_t>((0 << 4) | (0 << 2) | 2), 0x02, 0x80, 0x81, 0x34, 0x12};
    Threads threads;
    const std::vector<Fixup> fs = parse(r, threads);
    CB_CHECK_EQ(fs.size(), std::size_t{1});
    CB_CHECK(fs[0].segment_relative);
    CB_CHECK_EQ(fs[0].location, 3);
    CB_CHECK_EQ(fs[0].offset, 0x105);
    CB_CHECK_EQ(fs[0].frame_method, 0);
    CB_CHECK_EQ(*fs[0].frame_index, 2);
    CB_CHECK_EQ(fs[0].target_method, 2);
    CB_CHECK_EQ(fs[0].target_index, 0x81);
    CB_CHECK(!fs[0].no_displacement);
    CB_CHECK_EQ(fs[0].displacement, 0x1234);
});

CB_TEST("exact.fixupp.threads_resolve_and_persist", [] {
    // a frame thread 1 (method 4: none) and a target thread 2 (method 0, index 3), then a fix-up naming both
    // THREAD: bit 7 clear, bit 6 = frame, bits 2-4 method, bits 0-1 thread number
    const Bytes thread_frame{0x40 | (4 << 2) | 1};              // frame thread 1: method 4, no index
    const Bytes thread_target{0x00 | (0 << 2) | 2, 0x03};        // target thread 2: method 0, index 3
    Threads threads;
    CB_CHECK(parse(thread_frame, threads).empty());
    CB_CHECK(parse(thread_target, threads).empty());
    // a later record: fix at 6 whose fix data names frame thread 1 (0x80 | 1 << 4) and target thread 2 (8 | 2), P set
    const Bytes use{0xC4, 0x06, static_cast<std::uint8_t>(0x80 | (1 << 4) | 8 | 4 | 2)};
    const std::vector<Fixup> fs = parse(use, threads);
    CB_CHECK_EQ(fs.size(), std::size_t{1});
    CB_CHECK_EQ(fs[0].frame_method, 4);
    CB_CHECK(!fs[0].frame_index.has_value());
    CB_CHECK_EQ(fs[0].target_method, 0);
    CB_CHECK_EQ(fs[0].target_index, 3);
    CB_CHECK(fs[0].no_displacement);
    // a thread nobody defined
    Threads empty;
    CB_CHECK_THROWS_AS(FormatError, parse(use, empty));
});

CB_TEST("exact.fixupp.damaged_records", [] {
    Threads threads;
    CB_CHECK_THROWS_AS(TruncatedError, parse(Bytes{0xC4}, threads));
    CB_CHECK_THROWS_AS(TruncatedError, parse(Bytes{0xC4, 0x04}, threads));
    CB_CHECK_THROWS_AS(TruncatedError, parse(Bytes{0xC4, 0x04, 0x02}, threads));         // no target index
    CB_CHECK_THROWS_AS(TruncatedError, parse(Bytes{0xC4, 0x04, 0x02, 0x01, 0x05}, threads));   // displacement short
    CB_CHECK(parse(Bytes{}, threads).empty());
});

CB_TEST("exact.fixupp.encode_is_what_parse_read", [] {
    // the fields survive a round trip, the 4-bit location included
    constexpr std::uint32_t kBase = 0x200;   // the data record the fix-ups follow begins here
    std::vector<Fixup> in;
    std::uint32_t at = kBase;
    for (const int loc : {0, 1, 2, 3, 4, 8, 9, 15}) {
        Fixup f;
        f.segment_relative = loc % 2 == 0;
        f.location = static_cast<std::uint8_t>(loc);
        f.frame_method = 0;
        f.frame_index = static_cast<std::uint16_t>(0x100 + loc);
        f.target_method = static_cast<std::uint8_t>(loc & 3);
        f.target_index = static_cast<std::uint16_t>(loc * 11);
        f.no_displacement = loc % 3 != 0;
        f.displacement = f.no_displacement ? 0 : 0xBEEF;
        f.at = at;
        at += 3;
        in.push_back(f);
    }
    Fixup own;   // a frame method that names no index
    own.location = 1;
    own.frame_method = 5;
    own.target_method = 1;
    own.target_index = 2;
    own.at = at;
    in.push_back(own);
    const Bytes rec = encode(in, kBase);
    CB_CHECK_EQ(rec.front(), 0x9C);
    Threads threads;
    const std::vector<Fixup> out = parse(payload_of(rec), threads);
    CB_CHECK_EQ(out.size(), in.size());
    for (std::size_t k = 0; k < in.size(); ++k) {
        CB_CHECK_EQ(out[k].offset, in[k].at - kBase);
        CB_CHECK_EQ(out[k].segment_relative, in[k].segment_relative);
        CB_CHECK_EQ(out[k].location, in[k].location);
        CB_CHECK_EQ(out[k].frame_method, in[k].frame_method);
        CB_CHECK(out[k].frame_index == in[k].frame_index);
        CB_CHECK_EQ(out[k].target_method, in[k].target_method);
        CB_CHECK_EQ(out[k].target_index, in[k].target_index);
        CB_CHECK_EQ(out[k].no_displacement, in[k].no_displacement);
        CB_CHECK_EQ(out[k].displacement, in[k].displacement);
    }
});

CB_TEST("exact.fixupp.encode_limits", [] {
    Fixup f;
    f.location = 2;
    f.frame_method = 0;
    f.frame_index = std::uint16_t{1};
    f.target_index = std::uint16_t{1};
    f.at = 1024;
    CB_CHECK_THROWS_AS(InternalError, encode(std::vector<Fixup>{f}, 0));   // past the data record the base names
    f.at = 1023;
    CB_CHECK_NOTHROW(encode(std::vector<Fixup>{f}, 0));
    f.frame_index.reset();
    CB_CHECK_THROWS_AS(FormatError, encode(std::vector<Fixup>{f}, 0));     // method 0 needs the index
    f.frame_method = 5;
    f.target_method = 6;
    CB_CHECK_THROWS_AS(FormatError, encode(std::vector<Fixup>{f}, 0));     // a target method wider than two bits
});
