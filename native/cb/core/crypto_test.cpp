#include "cb/core/crypto.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::crypto;

CB_TEST("core.crypto.known_digests", [] {
    const ByteView abc = as_bytes("abc");
    CB_CHECK_EQ(to_hex(md5(abc)), "900150983cd24fb0d6963f7d28e17f72");
    CB_CHECK_EQ(to_hex(sha1(abc)), "a9993e364706816aba3e25717850c26c9cd0d89d");
    CB_CHECK_EQ(to_hex(sha256(abc)), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    CB_CHECK_EQ(to_hex(sha512(abc)).substr(0, 32), "ddaf35a193617abacc417349ae204131");
    CB_CHECK_EQ(to_hex(md5(ByteView{})), "d41d8cd98f00b204e9800998ecf8427e");   // the empty input
    CB_CHECK_EQ(to_hex(sha256(ByteView{})), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
});

CB_TEST("core.crypto.streaming_equals_one_shot", [] {
    Bytes big(300000);
    for (std::size_t i = 0; i < big.size(); ++i) big[i] = static_cast<std::uint8_t>(i * 31 + 7);
    Hasher h(Hasher::Algorithm::Sha256);
    h.update(ByteView(big).first(1000));
    h.update(ByteView(big).subspan(1000, 99999));
    h.update(ByteView(big).subspan(100999));
    Bytes streamed = h.finish();
    Sha256Digest whole = sha256(big);
    CB_CHECK(equal_bytes(streamed, whole));
    CB_CHECK_THROWS_AS(InternalError, h.finish());   // a finished hasher cannot finish again
    CB_CHECK_THROWS_AS(InternalError, h.update(as_bytes("x")));
});

CB_TEST("core.crypto.hex", [] {
    CB_CHECK_EQ(to_hex(Bytes{0x00, 0xAB, 0xFF}), "00abff");
    CB_CHECK(from_hex("00abFF") == (Bytes{0x00, 0xAB, 0xFF}));
    CB_CHECK_EQ(from_hex("").size(), 0u);
    CB_CHECK_THROWS_AS(FormatError, from_hex("abc"));    // odd length
    CB_CHECK_THROWS_AS(FormatError, from_hex("zz"));     // not hex
});

CB_TEST("core.crypto.crc32_adler32", [] {
    CB_CHECK_EQ(crc32(as_bytes("123456789")), 0xCBF43926u);
    CB_CHECK_EQ(crc32(ByteView{}), 0u);
    // continuing over a second block gives the whole-input value
    const ByteView text = as_bytes("hello, world");
    CB_CHECK_EQ(crc32(text.subspan(5), crc32(text.first(5))), crc32(text));
    CB_CHECK_EQ(adler32(as_bytes("Wikipedia")), 0x11E60398u);
    CB_CHECK_EQ(adler32(ByteView{}), 1u);
    CB_CHECK_EQ(adler32(text.subspan(5), adler32(text.first(5))), adler32(text));
});
