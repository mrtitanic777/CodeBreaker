// Tests of cb/formats/xbox/god.hpp. The expected hashes of the 300-block package were computed by an independent
// implementation of the layout (Python's hashlib, written from the format description, not from this code) and then
// read against it: the part is 1 master block + 2 subparts (204 and 96 blocks) = 4096 + 2*4096 + 300*4096 bytes; the header
// carries the title ids big-endian, the block count in 24 bits, the part count little-endian, the parts' size in 0x100s.
//
// DIVERGES from the Python tool:
//  - A title cut at the header's 0x7E-byte limit never ends in half a surrogate pair (the Python cut the bytes).
//  - A volume of more than 2^24 blocks (64 GiB) is a UsageError where the Python wrote the low 24 bits; an empty volume is a
//    UsageError where the Python failed on the missing first part.
//  - The part files are written as the volume streams (one subpart at a time), not collected first.
//  - A part shorter than a master block adds nothing to the volume's size (the Python's arithmetic went negative).
//  - The tool's info/list/iso2god/xex2god operations read and build XDVDFS volumes; they stay with the disc-image tool and are
//    usage errors here.
#include "cb/formats/xbox/god.hpp"

#include <algorithm>
#include <string>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::formats::xbox::god;

namespace god_test {

Bytes header_template() { return fs::read_file(testing::data_path("formats/testdata/god/empty_live.bin")); }

// block k, byte j = (k*7 + j) & 255
std::uint8_t volume_byte(std::uint64_t offset) { return static_cast<std::uint8_t>((offset / kBlock) * 7 + offset % kBlock); }

VolumeReader pattern_volume(std::uint64_t size) {
    return [size](std::uint64_t offset, std::size_t length) {
        const std::uint64_t end = std::min<std::uint64_t>(offset + length, size);
        Bytes out(offset < end ? static_cast<std::size_t>(end - offset) : 0);
        for (std::size_t i = 0; i < out.size(); ++i) out[i] = volume_byte(offset + i);
        return out;
    };
}

std::string hex(ByteView v) { return crypto::to_hex(v); }
std::uint32_t be32(ByteView v, std::size_t at) { return v[at] << 24 | v[at + 1] << 16 | v[at + 2] << 8 | v[at + 3]; }

CB_TEST("formats.xbox.god.write_and_read_back", [] {
    fs::ScopedTempDir tmp("god");
    const std::uint64_t size = 300 * kBlock - 100;   // the last block is short and is padded
    const Title title{0x4D530064, 0xCAFEF00D, kDefaultContentType, "Test Game \xC3\xA9\xF0\x9F\x98\x80"};
    const WriteResult made = write_god(pattern_volume(size), size, tmp.path(), header_template(), title);

    CB_CHECK_EQ(made.part_count, std::size_t{1});
    CB_CHECK_EQ(fs::to_text(made.header), fs::to_text(tmp.path() / "4D530064" / "00007000" / "CAFEF00D"));
    CB_CHECK_EQ(fs::to_text(made.data_dir), fs::to_text(tmp.path() / "4D530064" / "00007000" / "CAFEF00D.data"));

    // the part
    const Bytes part = fs::read_file(made.data_dir / "Data0000");
    CB_CHECK_EQ(part.size(), std::size_t{1241088});
    CB_CHECK_EQ(hex(crypto::sha1(part)), "162a868e3748dd47df0b4bb45aaecae4619c2c64");
    // the master block: the hash of each subpart's hash block, then zeros; the header holds the hash of the master block
    CB_CHECK_EQ(hex(crypto::sha1(ByteView(part).first(kBlock))), "bf7c233b8992f992f52250f79f21332b5fbfbd22");
    for (std::size_t i = 40; i < kBlock; ++i) CB_CHECK_EQ(part[i], 0);
    // the first subpart's hash block follows the master block, then 204 data blocks, then the second subpart's hash block
    CB_CHECK(std::equal(part.begin() + kBlock, part.begin() + kBlock + 20, crypto::sha1(ByteView(part).subspan(2 * kBlock, kBlock)).begin()));
    CB_CHECK(std::equal(part.begin(), part.begin() + 20, crypto::sha1(ByteView(part).subspan(kBlock, kBlock)).begin()));
    CB_CHECK_EQ(part[2 * kBlock + 1], volume_byte(1));   // data block 0 starts after the hash block

    // the header
    const Bytes header = fs::read_file(made.header);
    CB_CHECK_EQ(header.size(), std::size_t{45056});
    CB_CHECK_EQ(hex(crypto::sha1(header)), "a676eca8d77278c6843c8e8954971971cf0dda44");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x32C, 20)), "c5e6f0d4daee258d43c82484b6f9df3165a05f3e");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x344, 4)), "00007000");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x354, 4)), "cafef00d");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x360, 4)), "4d530064");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x392, 5)), "00012c0000");        // 300 blocks (24 bits), then 2 zero bytes
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x3A0, 8)), "01000000000012f0");   // 1 part (LE); 0x12f000 / 0x100 (BE)
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x37D, 20)), "bf7c233b8992f992f52250f79f21332b5fbfbd22");
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x411, 24)), "0054006500730074002000470061006d0065002000e9d83d");
    CB_CHECK(std::equal(header.begin() + 0x411, header.begin() + 0x411 + 26, header.begin() + 0x1691));   // the same name in the title place
    // the header's own hash covers 0x344 .. 0x344 + 0xACBC
    CB_CHECK(std::equal(header.begin() + 0x32C, header.begin() + 0x340, crypto::sha1(ByteView(header).subspan(0x344, 0xACBC)).begin()));

    // read it back as a volume
    SvodReader volume = SvodReader::open(made.data_dir);
    CB_CHECK_EQ(volume.parts().size(), std::size_t{1});
    CB_CHECK_EQ(volume.size(), 300 * kBlock);   // (the padded last block is part of the data)
    const Bytes all = volume.read_at(0, 300 * kBlock + 50);
    CB_CHECK_EQ(all.size(), std::size_t{300 * kBlock});
    for (std::uint64_t i = 0; i < size; i += 997) CB_CHECK_EQ(all[i], volume_byte(i));
    for (std::uint64_t i = size; i < all.size(); ++i) CB_CHECK_EQ(all[i], 0);   // the padding
    // a read across the subpart boundary (204 blocks in) and within one block
    const Bytes across = volume.read_at(204 * kBlock - 3, 10);
    for (std::uint64_t i = 0; i < 10; ++i) CB_CHECK_EQ(across[i], volume_byte(204 * kBlock - 3 + i));
    CB_CHECK_EQ(volume.read_at(5, 3).size(), std::size_t{3});
    CB_CHECK(volume.read_at(300 * kBlock, 10).empty());
    CB_CHECK(volume.read_at(1ull << 40, 10).empty());
    CB_CHECK(volume.read_at(0, 0).empty());
});

CB_TEST("formats.xbox.god.two_parts", [] {
    // A volume of one part and a few blocks more: the second part restarts the block numbering and is read from its own file.
    fs::ScopedTempDir tmp("god2");
    const std::uint64_t blocks = kBlocksPerPart + 3;
    const std::uint64_t size = blocks * kBlock;
    const WriteResult made = write_god(pattern_volume(size), size, tmp.path(), header_template(), Title{1, 2, kDefaultContentType, ""});
    CB_CHECK_EQ(made.part_count, std::size_t{2});
    CB_CHECK_EQ(fs::file_size(made.data_dir / "Data0001"), kBlock + kBlock + 3 * kBlock);   // master, one subpart's hash block, 3 data blocks
    CB_CHECK_EQ(fs::file_size(made.data_dir / "Data0000"), kBlock + kSubpartsPerPart * (kBlock + kSubpartSize));
    const Bytes header = fs::read_file(made.header);
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x3A0, 4)), "02000000");
    const std::uint64_t parts_size = fs::file_size(made.data_dir / "Data0000") + fs::file_size(made.data_dir / "Data0001");
    CB_CHECK_EQ(be32(header, 0x3A4), static_cast<std::uint32_t>(parts_size / 0x100));
    CB_CHECK_EQ(hex(ByteView(header).subspan(0x392, 3)), "00a1c7");   // 0xA1C4 + 3 blocks

    const SvodReader volume = SvodReader::open(made.data_dir);
    CB_CHECK_EQ(volume.parts().size(), std::size_t{2});
    CB_CHECK_EQ(volume.size(), size);
    // the last block of part 0 and the first of part 1, in one read
    const std::uint64_t edge = kBlocksPerPart * kBlock;
    const Bytes across = volume.read_at(edge - 4, 8);
    CB_CHECK_EQ(across.size(), std::size_t{8});
    for (std::uint64_t i = 0; i < 8; ++i) CB_CHECK_EQ(across[i], volume_byte(edge - 4 + i));
    // a read through the whole second part and past the end
    const Bytes tail = volume.read_at(edge, 4 * kBlock);
    CB_CHECK_EQ(tail.size(), std::size_t{3 * kBlock});
    CB_CHECK_EQ(tail[3 * kBlock - 1], volume_byte(size - 1));
});

CB_TEST("formats.xbox.god.header_fields", [] {
    const Bytes tpl = header_template();
    const Bytes hash(20, 0xab);
    Title t{0x11223344, 0x55667788, 0x7000, ""};
    const Bytes h = make_header(tpl, t, 0x123456, 3, 0x40000, hash);
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x344, 4)), "00007000");
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x354, 4)), "55667788");
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x360, 4)), "11223344");
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x392, 3)), "123456");
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x3A0, 4)), "03000000");
    CB_CHECK_EQ(hex(ByteView(h).subspan(0x3A4, 4)), "00000400");
    CB_CHECK(Bytes(h.begin() + 0x37D, h.begin() + 0x37D + 20) == hash);
    CB_CHECK_EQ(h[0x35B], 0);
    CB_CHECK_EQ(h[0x35F], 0);
    CB_CHECK_EQ(h[0x391], 0);
    // no title: the name places are left as the template has them
    CB_CHECK(Bytes(h.begin() + 0x411, h.begin() + 0x411 + 8) == Bytes(tpl.begin() + 0x411, tpl.begin() + 0x411 + 8));

    // a title longer than the place keeps whole UTF-16 units (63 of them), and a surrogate pair is not split
    t.title = std::string(62, 'a') + "\xF0\x9F\x98\x80";                    // 62 units + a pair: the pair would be units 62 and 63
    const Bytes cut = make_header(tpl, t, 1, 1, 0x1000, hash);
    CB_CHECK_EQ(cut[0x411 + 123], 0x61);                                    // the 62nd 'a' (its low byte)
    CB_CHECK_EQ(cut[0x411 + 124], tpl[0x411 + 124]);                        // nothing of the emoji: the template's bytes
    CB_CHECK_EQ(cut[0x411 + 125], tpl[0x411 + 125]);
    t.title = std::string(70, 'b');
    const Bytes plain = make_header(tpl, t, 1, 1, 0x1000, hash);
    CB_CHECK_EQ(plain[0x411 + 125], 0x62);                                   // 63 units fit
    CB_CHECK_EQ(plain[0x411 + 126], tpl[0x411 + 126]);                       // and no more
    CB_CHECK(std::equal(plain.begin() + 0x411, plain.begin() + 0x411 + 126, plain.begin() + 0x1691));

    CB_CHECK_THROWS_AS(FormatError, make_header(ByteView(tpl).first(0x1000), t, 1, 1, 0x1000, hash));
    CB_CHECK_THROWS_AS(UsageError, make_header(tpl, t, 1, 1, 0x1000, Bytes(19, 0)));
    CB_CHECK_THROWS_AS(UsageError, make_header(tpl, t, 0x1000000, 1, 0x1000, hash));
    CB_CHECK_THROWS_AS(UsageError, make_header(tpl, t, 1, 1, 0x100ull * 0x100000000ull, hash));
});

CB_TEST("formats.xbox.god.write_errors", [] {
    fs::ScopedTempDir tmp("goderr");
    const Bytes tpl = header_template();
    CB_CHECK_THROWS_AS(UsageError, write_god(pattern_volume(0), 0, tmp.path(), tpl, Title{}));
    CB_CHECK_THROWS_AS(FormatError, write_god(pattern_volume(100), 100, tmp.path(), ByteView(tpl).first(100), Title{}));
    // a reader that answers short: the rest of the volume reads as zeros
    const WriteResult made = write_god([](std::uint64_t, std::size_t) { return Bytes{1, 2, 3}; }, 2 * kBlock, tmp.path(), tpl, Title{5, 6, 0x7000, ""});
    const SvodReader volume = SvodReader::open(made.data_dir);
    const Bytes back = volume.read_at(0, 2 * kBlock);
    CB_CHECK_EQ(back.size(), std::size_t{2 * kBlock});
    CB_CHECK_EQ(back[0], 1);
    CB_CHECK_EQ(back[2], 3);
    CB_CHECK_EQ(back[3], 0);
});

CB_TEST("formats.xbox.god.reader_errors", [] {
    fs::ScopedTempDir tmp("godread");
    CB_CHECK_THROWS_AS(NotFoundError, (void)SvodReader::open(tmp.path() / "missing"));
    CB_CHECK_THROWS_AS(FormatError, (void)SvodReader::open(tmp.path()));   // no part files
    fs::write_file(tmp.path() / "readme.txt", Bytes{1});
    CB_CHECK_THROWS_AS(FormatError, (void)SvodReader::open(tmp.path()));

    // parts are found by a case-insensitive "data" prefix and read in name order; other files are ignored
    fs::write_file(tmp.path() / "DATA0001", Bytes(10, 0));
    fs::write_file(tmp.path() / "Data0000", Bytes(10, 0));
    const SvodReader reader = SvodReader::open(tmp.path());
    CB_CHECK_EQ(reader.parts().size(), std::size_t{2});
    CB_CHECK_EQ(fs::to_text(reader.parts()[0].filename()), "DATA0001");   // (the byte order of names: upper case first)
    // parts that are shorter than a master block hold no data
    CB_CHECK_EQ(reader.size(), std::uint64_t{0});
    CB_CHECK(reader.read_at(0, 10).empty());

    // a truncated part: the data it holds is returned and the read stops
    fs::ScopedTempDir tmp2("godshort");
    Bytes shortpart(kBlock + kBlock + 100, 0x77);   // master, hash block, 100 bytes of the first data block
    fs::write_file(tmp2.path() / "Data0000", shortpart);
    const SvodReader cut = SvodReader::open(tmp2.path());
    CB_CHECK_EQ(cut.size(), std::uint64_t{100});
    CB_CHECK_EQ(cut.read_at(0, 500).size(), std::size_t{100});
    CB_CHECK_EQ(cut.read_at(90, 500).size(), std::size_t{10});
});

CB_TEST("formats.xbox.god.install_to_god", [] {
    Bytes installed = header_template();
    for (std::size_t i = 0; i < 4; ++i) installed[i] = static_cast<std::uint8_t>("CON "[i]);
    installed[0x344] = 0;
    installed[0x345] = 0;
    installed[0x346] = 0x40;
    installed[0x347] = 0;
    installed[0x3FD] = 0x99;
    installed[0x411] = 0x99;
    CB_CHECK(is_nxe_install(installed));
    CB_CHECK(!is_nxe_install(header_template()));
    CB_CHECK(!is_nxe_install(ByteView(installed).first(0x347)));

    const Bytes god = install_to_god(installed);
    CB_CHECK_EQ(god.size(), installed.size());
    CB_CHECK_EQ(hex(crypto::sha1(god)), "ca911fd3cff2104ece3bba1c098c167aebe43d25");   // from the Python-side model of the transform
    CB_CHECK_EQ(std::string(as_text(ByteView(god).first(4))), "LIVE");
    CB_CHECK_EQ(hex(ByteView(god).subspan(0x22C, 8)), "ffffffffffffffff");
    CB_CHECK_EQ(hex(ByteView(god).subspan(0x340, 8)), "0000ad0e00007000");
    CB_CHECK_EQ(hex(ByteView(god).subspan(0x379, 4)), "24050511");
    CB_CHECK_EQ(god[0x3FD], 0);
    CB_CHECK_EQ(god[0x411], 0);
    CB_CHECK(!is_nxe_install(god));
    CB_CHECK(std::equal(god.begin() + 0x32C, god.begin() + 0x340, crypto::sha1(ByteView(god).subspan(0x344, 0xACBC)).begin()));
    CB_CHECK_THROWS_AS(FormatError, install_to_god(ByteView(installed).first(0x412)));
    CB_CHECK_EQ(install_to_god(ByteView(installed).first(0x413)).size(), std::size_t{0x413});
});

CB_TEST("formats.xbox.god.command", [] {
    fs::ScopedTempDir tmp("godcmd");
    const std::string header_path = fs::to_text(tmp.path() / "in.hdr"), out_path = fs::to_text(tmp.path() / "out.hdr");

    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{}), 0);
        CB_CHECK_EQ(c.out(), std::string(usage()));
    }
    // install2god: a header that is not an NXE install gets the warning and is converted anyway
    fs::write_file(header_path, header_template());
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"install2god", header_path, out_path}), 0);
        CB_CHECK_EQ(c.out(),
                    "warning: header content-type is not 0x00004000 (NXE-installed); converting anyway\n"
                    "GoD/LIVE header written -> " + out_path + " (copy the sibling .data folder alongside)\n");
        CB_CHECK(fs::read_file(out_path).size() == 45056);
    }

    // god2iso: the de-interleaved volume as a file
    const std::uint64_t size = 10 * kBlock;
    const WriteResult made = write_god(pattern_volume(size), size, tmp.path() / "pkg", header_template(), Title{7, 8, 0x7000, ""});
    const std::string iso = fs::to_text(tmp.path() / "disc.iso");
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"god2iso", fs::to_text(made.data_dir), iso}), 0);
        CB_CHECK_EQ(c.out(), "wrote 40960 bytes -> " + iso + "\n");
    }
    const Bytes disc = fs::read_file(iso);
    CB_CHECK_EQ(disc.size(), std::size_t{40960});
    for (std::uint64_t i = 0; i < disc.size(); i += 331) CB_CHECK_EQ(disc[i], volume_byte(i));

    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"install2god", header_path}));
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"god2iso", fs::to_text(made.data_dir)}));
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"info", fs::to_text(made.data_dir)}));
    CB_CHECK_THROWS_AS(NotFoundError, command(std::vector<std::string>{"god2iso", fs::to_text(tmp.path() / "none"), iso}));
});

}  // namespace god_test
