// Tests of the Amstrad CPC writer over a small AMSDOS binary and a snapshot written byte by byte: each must come back as source that builds the same
// bytes, with the firmware's names for what the code calls, and a file that is not a CPC program must be refused.

#include "cb/source/cpc_writer.hpp"

#include <cstring>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::source::cpc_writer {
namespace {

// $4000 start: CALL $4010; RET        $4010 sub: CALL $BB5A (TXT_OUTPUT); LD A,(HL); RET
Bytes code() {
    Bytes b(0x20, 0);
    const Bytes start = {0xCD, 0x10, 0x40, 0xC9};
    std::copy(start.begin(), start.end(), b.begin());
    const Bytes sub = {0xCD, 0x5A, 0xBB, 0x7E, 0xC9};
    std::copy(sub.begin(), sub.end(), b.begin() + 0x10);
    return b;
}

Bytes amsdos(const Bytes& body) {
    Bytes f(128, 0);
    std::memset(&f[1], ' ', 11);
    std::memcpy(&f[1], "DEMO", 4);
    std::memcpy(&f[9], "BIN", 3);
    f[18] = 2;
    f[22] = 0x40;   // load $4000
    f[24] = static_cast<std::uint8_t>(body.size());
    f[27] = 0x40;   // exec $4000
    unsigned sum = 0;
    for (std::size_t i = 0; i < 67; ++i) sum += f[i];
    f[67] = static_cast<std::uint8_t>(sum);
    f[68] = static_cast<std::uint8_t>(sum >> 8);
    f.insert(f.end(), body.begin(), body.end());
    return f;
}

CB_TEST("source.cpc_writer.a_binary_comes_back_as_source_that_builds_the_same_bytes", [] {
    fs::ScopedTempDir dir("cb_cpc");
    const formats::cpc::Program p = formats::cpc::parse(amsdos(code()), ".bin");
    const image_writer::Written r = write(p, "demo.bin", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.at(0x4000), "start");
    CB_CHECK_EQ(r.functions.at(0x4010), "sub_4010");
    CB_CHECK_EQ(r.code_bytes, 9u);
    CB_CHECK(r.built_image && *r.built_image == p.image);
    const std::string text = fs::read_text(r.source);
    for (const char* line : {"; Amstrad CPC binary demo.bin; 32 bytes at $4000", "TXT_OUTPUT", "        CALL TXT_OUTPUT", "        CALL sub_4010", "start:"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
});

CB_TEST("source.cpc_writer.a_snapshot_is_written_from_its_program_counter", [] {
    fs::ScopedTempDir dir("cb_cpc");
    Bytes sna(0x100 + 0x10000, 0);
    std::memcpy(sna.data(), "MV - SNA", 8);
    sna[0x24] = 0x40;   // PC = $4000
    sna[0x6B] = 64;
    const Bytes c = code();
    std::copy(c.begin(), c.end(), sna.begin() + 0x100 + 0x4000);
    fs::write_file(dir.path() / "demo.sna", sna);
    const image_writer::Written r = reconstruct(dir.path() / "demo.sna", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.at(0x4000), "start");
    CB_CHECK(fs::read_text(r.source).find("registers at the snapshot:") != std::string::npos);
});

CB_TEST("source.cpc_writer.what_is_not_a_cpc_program_is_refused", [] {
    fs::ScopedTempDir dir("cb_cpc");
    fs::write_file(dir.path() / "junk.bin", Bytes(300, 'x'));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "junk.bin", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "absent.bin", dir.path() / "out2"));
});

}  // namespace
}  // namespace cb::source::cpc_writer
