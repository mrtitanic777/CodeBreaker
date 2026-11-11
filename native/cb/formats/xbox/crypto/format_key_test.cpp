// Tests of cb/formats/xbox/crypto/format_key.hpp: the array source for keys of every size around the line width, and the
// tool's files.
//
// DIVERGES from the Python: the guarded `typedef unsigned char BYTE` ends with a semicolon (the Python's text lacked it,
// so the header did not compile where BYTE was not already a macro); a missing Output/Zero folder is created (the
// Python raised FileNotFoundError); an empty key is a FormatError (IndexError there).
#include "cb/formats/xbox/crypto/format_key.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace fk = cb::formats::xbox::crypto::format_key;

namespace {

const std::string kHead =
    "#ifndef BYTE\ntypedef unsigned char BYTE;\n#endif\n\n#ifndef __ExpPubKey__\n#define __ExpPubKey__\nBYTE ExpPubKey[] = {\n";
const std::string kTail = "};\n#endif\n";

Bytes counting(std::size_t n) {
    Bytes b;
    for (std::size_t i = 0; i < n; ++i) b.push_back(static_cast<std::uint8_t>(0xC2 + i * 7));
    return b;
}

}  // namespace

CB_TEST("formats.xbox.crypto.format_key.one_byte", [] {
    CB_CHECK_EQ(fk::byte_array_source(Bytes{0xC2}), kHead + "\t0xC2\n" + kTail);
});

CB_TEST("formats.xbox.crypto.format_key.line_widths", [] {
    // exactly one line: no trailing comma
    const Bytes b16 = counting(16);
    std::string line16 = "\t";
    for (std::size_t i = 0; i < 16; ++i) line16 += std::format("{}0x{:02X}", i ? ", " : "", b16[i]);
    CB_CHECK_EQ(fk::byte_array_source(b16), kHead + line16 + "\n" + kTail);
    // 17: the first line ends with a comma, the second holds one byte and has none
    const Bytes b17 = counting(17);
    std::string first = "\t";
    for (std::size_t i = 0; i < 16; ++i) first += std::format("{}0x{:02X}", i ? ", " : "", b17[i]);
    CB_CHECK_EQ(fk::byte_array_source(b17), kHead + first + ",\n" + std::format("\t0x{:02X}\n", b17[16]) + kTail);
    // 32: two full lines
    const std::string s32 = fk::byte_array_source(counting(32));
    CB_CHECK_EQ(std::count(s32.begin(), s32.end(), '\n'), 7 + 2 + 2);   // 7 head lines, 2 data lines, `};` and `#endif`
    CB_CHECK(s32.find(",\n\t0x") != std::string::npos);
    CB_CHECK(s32.ends_with("\n};\n#endif\n"));
    CB_CHECK(!s32.ends_with(",\n};\n#endif\n"));
});

CB_TEST("formats.xbox.crypto.format_key.options", [] {
    CB_CHECK_EQ(fk::byte_array_source(Bytes{1, 2, 3}, "Key", 2),
                "#ifndef BYTE\ntypedef unsigned char BYTE;\n#endif\n\n#ifndef __Key__\n#define __Key__\nBYTE Key[] = {\n"
                "\t0x01, 0x02,\n\t0x03\n};\n#endif\n");
    CB_CHECK_EQ(fk::byte_array_source(Bytes{1, 2}, "K", 0), fk::byte_array_source(Bytes{1, 2}, "K", 1));   // at least one a line
    CB_CHECK_THROWS_AS(FormatError, fk::byte_array_source(Bytes{}));
});

CB_TEST("formats.xbox.crypto.format_key.tool", [] {
    fs::ScopedTempDir tmp("fk");
    const Bytes key = counting(270);   // a real key is 0x110 bytes
    fs::write_file(tmp.path() / "Keys" / "custom_pub.bin", key);
    const std::string source = fk::write_exp_pub_key(tmp.path());
    CB_CHECK_EQ(source, fk::byte_array_source(key));
    CB_CHECK_EQ(fs::read_text(tmp.path() / "Output" / "Zero" / "ExpPubKey.h"), source);   // the folders were made
    // nothing but the source is in the file, and every byte is in it
    for (const std::uint8_t b : key) CB_CHECK(source.find(std::format("0x{:02X}", b)) != std::string::npos);

    fs::ScopedTempDir empty("fk");
    CB_CHECK_THROWS_AS(NotFoundError, fk::write_exp_pub_key(empty.path()));
    fs::write_file(empty.path() / "Keys" / "custom_pub.bin", Bytes{});
    CB_CHECK_THROWS_AS(FormatError, fk::write_exp_pub_key(empty.path()));
    CB_CHECK(!std::filesystem::exists(empty.path() / "Output"));   // nothing is written for a key that cannot be embedded
});
