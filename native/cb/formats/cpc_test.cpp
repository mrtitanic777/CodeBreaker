#include "cb/formats/cpc.hpp"

#include <algorithm>
#include <cstring>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::cpc {
namespace {

// An AMSDOS file: the 128-byte header (checksum included) and the body.
Bytes amsdos_file(const char* name, std::uint8_t type, std::uint16_t load, std::uint16_t exec, const Bytes& body) {
    Bytes f(128, 0);
    std::memset(&f[1], ' ', 11);
    std::memcpy(&f[1], name, std::min<std::size_t>(std::strlen(name), 8));
    std::memcpy(&f[9], "BIN", 3);
    f[18] = type;
    f[21] = static_cast<std::uint8_t>(load);
    f[22] = static_cast<std::uint8_t>(load >> 8);
    f[24] = static_cast<std::uint8_t>(body.size());
    f[25] = static_cast<std::uint8_t>(body.size() >> 8);
    f[26] = static_cast<std::uint8_t>(exec);
    f[27] = static_cast<std::uint8_t>(exec >> 8);
    f[64] = f[24];
    f[65] = f[25];
    unsigned sum = 0;
    for (std::size_t i = 0; i < 67; ++i) sum += f[i];
    f[67] = static_cast<std::uint8_t>(sum);
    f[68] = static_cast<std::uint8_t>(sum >> 8);
    f.insert(f.end(), body.begin(), body.end());
    return f;
}

// A one-track data-format disk (sectors $C1-$C9 of 512 bytes) holding `file` as TEST.BIN (extended: the EXTENDED CPC DSK layout).
Bytes disk(const Bytes& file, bool extended) {
    constexpr std::size_t kTrack = 0x100 + 9 * 512;
    Bytes d(0x100 + kTrack, 0);
    const char* magic = extended ? "EXTENDED CPC DSK File\r\nDisk-Info\r\n" : "MV - CPCEMU Disk-File\r\nDisk-Info\r\n";
    std::memcpy(d.data(), magic, std::strlen(magic));
    d[0x30] = 1;
    d[0x31] = 1;
    if (extended) {
        d[0x34] = static_cast<std::uint8_t>(kTrack / 256);
    } else {
        d[0x32] = static_cast<std::uint8_t>(kTrack);
        d[0x33] = static_cast<std::uint8_t>(kTrack >> 8);
    }
    std::uint8_t* t = d.data() + 0x100;
    std::memcpy(t, "Track-Info\r\n", 12);
    t[0x14] = 2;
    t[0x15] = 9;
    for (std::size_t s = 0; s < 9; ++s) {
        t[0x18 + s * 8 + 2] = static_cast<std::uint8_t>(0xC1 + s);
        t[0x18 + s * 8 + 3] = 2;
        if (extended) {
            t[0x18 + s * 8 + 6] = 0x00;
            t[0x18 + s * 8 + 7] = 0x02;
        }
    }
    std::uint8_t* sectors = t + 0x100;
    // directory entry in logical sector 0: user 0, "TEST    BIN", one extent, the records, block 2
    std::uint8_t* e = sectors;
    e[0] = 0;
    std::memcpy(e + 1, "TEST    BIN", 11);
    e[15] = static_cast<std::uint8_t>((file.size() + 127) / 128);
    e[16] = 2;
    for (std::size_t i = 32; i < 4 * 512; i += 32) sectors[i] = 0xE5;   // the other entries: deleted
    // block 2: logical sectors 4 and 5
    std::copy(file.begin(), file.end(), sectors + 4 * 512);
    return d;
}

Bytes body() {
    Bytes b(100, 0);
    b[0] = 0xCD;   // CALL $BB06
    b[1] = 0x06;
    b[2] = 0xBB;
    b[3] = 0xC9;
    return b;
}

CB_TEST("formats.cpc.an_amsdos_header_is_found_by_its_checksum", [] {
    const Bytes f = amsdos_file("GAME", 2, 0x4000, 0x4000, body());
    const std::optional<AmsdosHeader> h = amsdos_header(f);
    CB_CHECK(h.has_value());
    CB_CHECK_EQ(h->name, "GAME.BIN");
    CB_CHECK_EQ(h->load, 0x4000);
    CB_CHECK_EQ(h->length, 100);
    Bytes bad = f;
    bad[30] ^= 1;
    CB_CHECK(!amsdos_header(bad).has_value());
});

CB_TEST("formats.cpc.a_binary_is_loaded_at_its_load_address_and_started_at_its_exec", [] {
    const Program p = parse(amsdos_file("GAME", 2, 0x4000, 0x4010, body()), ".bin");
    CB_CHECK(p.kind == Kind::Binary);
    CB_CHECK_EQ(p.origin, 0x4000u);
    CB_CHECK_EQ(p.image.size(), 100u);
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 0x4010);
    CB_CHECK_EQ(p.memory.size(), kMemorySize);
    // a BASIC program (type 0) is not a program of this lane
    CB_CHECK_THROWS_AS(FormatError, parse(amsdos_file("GAME", 0, 0x170, 0, body()), ".bin"));
});

CB_TEST("formats.cpc.a_disk_gives_its_files_and_its_best_binary", [] {
    const Bytes file = amsdos_file("TEST", 2, 0x8000, 0x8000, body());
    for (const bool extended : {false, true}) {
        const Bytes d = disk(file, extended);
        const std::vector<DiskFile> files = disk_files(d);
        CB_CHECK_EQ(files.size(), 1u);
        CB_CHECK_EQ(files[0].name, "TEST.BIN");
        CB_CHECK_EQ(files[0].data.size(), 256u);   // two 128-byte records
        CB_CHECK(files[0].header.has_value());
        const Program p = parse(d, ".dsk");
        CB_CHECK(p.kind == Kind::Disk);
        CB_CHECK_EQ(p.name, "TEST.BIN");
        CB_CHECK_EQ(p.origin, 0x8000u);
        CB_CHECK_EQ(p.image.size(), 100u);
        CB_CHECK(p.image == body());
        CB_CHECK_EQ(p.files.size(), 1u);
    }
});

CB_TEST("formats.cpc.a_snapshot_is_the_registers_and_the_ram", [] {
    Bytes sna(0x100 + 0x10000, 0);
    std::memcpy(sna.data(), "MV - SNA", 8);
    sna[0x10] = 1;
    sna[0x12] = 0x42;   // A
    sna[0x23] = 0x00;   // PC = $4000
    sna[0x24] = 0x40;
    sna[0x21] = 0xF0;   // SP = $BFF0
    sna[0x22] = 0xBF;
    sna[0x6B] = 64;
    sna[0x100 + 0x4000] = 0xC9;
    const Program p = parse(sna, ".sna");
    CB_CHECK(p.kind == Kind::Snapshot);
    CB_CHECK(p.registers.has_value());
    CB_CHECK_EQ(p.registers->a, 0x42);
    CB_CHECK_EQ(p.registers->pc, 0x4000);
    CB_CHECK_EQ(p.origin, kSnapshotStart);
    CB_CHECK_EQ(p.image.size(), kSnapshotEnd - kSnapshotStart);
    CB_CHECK_EQ(p.image[0x4000 - kSnapshotStart], 0xC9);
    CB_CHECK_EQ(p.entries.size(), 1u);
    sna[0x6B] = 0;   // a chunked snapshot has no RAM block field
    sna.resize(0x200);
    CB_CHECK_THROWS_AS(UnsupportedError, parse(sna, ".sna"));
});

CB_TEST("formats.cpc.probe_takes_a_binary_a_disk_with_one_and_a_snapshot", [] {
    fs::ScopedTempDir dir("cb_cpc");
    fs::write_file(dir.path() / "game.bin", amsdos_file("GAME", 2, 0x4000, 0x4000, body()));
    fs::write_file(dir.path() / "text.bin", Bytes(300, 'x'));
    fs::write_file(dir.path() / "game.dsk", disk(amsdos_file("TEST", 2, 0x8000, 0x8000, body()), false));
    fs::write_file(dir.path() / "basic.dsk", disk(amsdos_file("TEST", 0, 0x170, 0, body()), false));
    Bytes sna(0x100 + 0x10000, 0);
    std::memcpy(sna.data(), "MV - SNA", 8);
    sna[0x6B] = 64;
    fs::write_file(dir.path() / "game.sna", sna);
    CB_CHECK(probe(dir.path() / "game.bin"));
    CB_CHECK(!probe(dir.path() / "text.bin"));
    CB_CHECK(probe(dir.path() / "game.dsk"));
    CB_CHECK(!probe(dir.path() / "basic.dsk"));
    CB_CHECK(probe(dir.path() / "game.sna"));
    CB_CHECK(!probe(dir.path() / "absent.dsk"));
});

CB_TEST("formats.cpc.the_firmware_jumpblock_has_the_names_of_the_firmware_guide", [] {
    CB_CHECK_EQ(firmware_names().at(0xBB06), "KM_WAIT_CHAR");
    CB_CHECK_EQ(firmware_names().at(0xBB5A), "TXT_OUTPUT");
    CB_CHECK_EQ(firmware_names().at(0xBD19), "MC_WAIT_FLYBACK");
});

}  // namespace
}  // namespace cb::formats::cpc

