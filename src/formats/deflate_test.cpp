#include "formats/deflate.hpp"

#include "core/error.hpp"
#include "core/testing.hpp"

namespace cb::formats::deflate {
namespace {

Bytes sample(std::size_t n) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>((i * 7 + i / 13) % 251);
    return b;
}

CB_TEST("formats.deflate.round_trip_in_each_wrapping", [] {
    const Bytes data = sample(100000);
    for (const Wrap wrap : {Wrap::zlib, Wrap::raw, Wrap::gzip}) {
        const Bytes packed = compress(data, -1, wrap);
        CB_CHECK(packed.size() < data.size());
        const Inflated all = inflate_all(packed, wrap);
        CB_CHECK(all.status == Status::end);
        CB_CHECK(all.data == data);
        CB_CHECK_EQ(all.consumed, packed.size());
        const Inflated exact = inflate_exact(packed, data.size(), wrap);
        CB_CHECK(exact.status == Status::end);
        CB_CHECK(exact.data == data);
    }
    // a zlib stream starts 78 xx; a gzip stream 1F 8B
    CB_CHECK_EQ(compress(data)[0], std::uint8_t{0x78});
    CB_CHECK_EQ(compress(data, -1, Wrap::gzip)[0], std::uint8_t{0x1F});
});

CB_TEST("formats.deflate.short_and_damaged_streams", [] {
    const Bytes data = sample(5000);
    const Bytes packed = compress(data, 9, Wrap::raw);
    // cut short: what there is is given, the stream has not ended
    const Inflated cut = inflate_all(ByteView(packed).first(packed.size() / 2), Wrap::raw);
    CB_CHECK(cut.status == Status::ok);
    CB_CHECK(!cut.data.empty() && cut.data.size() < data.size());
    // a buffer that is too small: Status::buffer with what fits
    const Inflated small = inflate_exact(packed, 100, Wrap::raw);
    CB_CHECK(small.status == Status::buffer);
    CB_CHECK_EQ(small.data.size(), std::size_t{100});
    // a buffer that is too big: the stream ends, the rest is not produced
    const Inflated big = inflate_exact(packed, 9000, Wrap::raw);
    CB_CHECK(big.status == Status::end);
    CB_CHECK_EQ(big.data.size(), data.size());
    // not a deflate stream at all
    const Bytes junk(64, 0xFF);
    CB_CHECK(inflate_all(junk, Wrap::raw).status == Status::bad);
    CB_CHECK(inflate_all(junk).status == Status::bad);
});

CB_TEST("formats.deflate.a_dictionary_primes_a_raw_stream", [] {
    const Bytes before = sample(3000);
    Bytes next = before;
    next.push_back(1);
    const Bytes plain = compress(next, 9, Wrap::raw);
    const Bytes primed = compress(next, 9, Wrap::raw, before);
    CB_CHECK(primed.size() < plain.size());
    const Inflated got = inflate_all(primed, Wrap::raw, before);
    CB_CHECK(got.status == Status::end);
    CB_CHECK(got.data == next);
});

}  // namespace
}  // namespace cb::formats::deflate
