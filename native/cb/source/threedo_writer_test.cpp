// cb-unity: alone
// The 3DO writer over a small program written word by word (big-endian, as the disc holds it): the source it writes, built again by CodeBreaker's
// assembler and its words reversed, is the AIF (and the disc it came in).

#include "cb/source/threedo_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::source::threedo_writer {
namespace {

void put_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// entry $40: push {r4, lr} ; mov r4, #1 ; bl sub ; ldr r1, [pc, #4] ; pop {r4, pc} ; data ; the literal $03400014 ; sub $5C: ldr r0, [r1] ; mov pc, lr.
Bytes aif() {
    Bytes f(0x64, 0);
    put_be(f, 0, 0xE1A00000);
    put_be(f, 4, 0xE1A00000);
    put_be(f, 8, 0xE1A00000);
    put_be(f, 12, 0xEB00000B);
    put_be(f, 16, 0xEF000011);
    put_be(f, 20, 0x64);
    put_be(f, 48, 0x20);
    put_be(f, 0x40, 0xE92D4010);
    put_be(f, 0x44, 0xE3A04001);
    put_be(f, 0x48, 0xEB000003);   // bl 0x5C
    put_be(f, 0x4C, 0xE59F1004);   // ldr r1, [pc, #4] -> 0x58
    put_be(f, 0x50, 0xE8BD8010);
    put_be(f, 0x54, 0x12345678);
    put_be(f, 0x58, 0x03400014);
    put_be(f, 0x5C, 0xE5910000);
    put_be(f, 0x60, 0xE1A0F00E);
    return f;
}

// An Opera disc: the volume header at block 0, the root directory at block 1 (one file, LaunchMe), the file at block 2.
Bytes disc(const Bytes& launch) {
    Bytes d(2048 * 2, 0);
    d[0] = 1;
    for (std::size_t i = 1; i <= 5; ++i) d[i] = 0x5A;
    d[6] = 1;
    put_be(d, 75, 2048);
    put_be(d, 79, 3);
    put_be(d, 87, 1);
    put_be(d, 91, 2048);
    put_be(d, 99, 1);
    put_be(d, 2048, 0xFFFFFFFF);
    put_be(d, 2048 + 4, 0xFFFFFFFF);
    put_be(d, 2048 + 12, 0x14 + 72);
    put_be(d, 2048 + 16, 0x14);
    const std::size_t e = 2048 + 0x14;
    put_be(d, e, 0xC0000002);
    put_be(d, e + 12, 2048);
    put_be(d, e + 16, static_cast<std::uint32_t>(launch.size()));
    put_be(d, e + 20, 1);
    const std::string name = "LaunchMe";
    std::copy(name.begin(), name.end(), d.begin() + static_cast<std::ptrdiff_t>(e) + 32);
    put_be(d, e + 68, 2);
    d.insert(d.end(), launch.begin(), launch.end());
    d.resize(2048 * 3, 0);
    return d;
}

}  // namespace

CB_TEST("source.threedo_writer.a_disc_comes_back_with_its_launchme_put_back", [] {
    fs::ScopedTempDir dir("cb_3do_disc");
    const image_writer::Written r = write(formats::threedo::parse(disc(aif())), "game.iso", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.size(), 2u);
    CB_CHECK(r.built.has_value() && fs::read_file(*r.built) == disc(aif()));
});

CB_TEST("source.threedo_writer.a_bare_aif_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_3do_aif");
    const image_writer::Written r = write(formats::threedo::parse(aif()), "LaunchMe", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x40, "entry"}, {0x5C, "sub_00005C"}}));
    CB_CHECK_EQ(r.image_bytes, 0x64u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {".arm", "entry:", "sub_00005C:", "push {r4, lr}", "bl sub_00005C", "CLIO_VCOUNT", "0x12345678"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

}  // namespace cb::source::threedo_writer
