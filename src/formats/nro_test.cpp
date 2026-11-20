// cb-unity: alone
// The Switch formats: a NRO (header, segments, MOD0, the dynamic relocations, the asset section), a NSO whose segments are LZ4 blocks, an AArch64 ELF, the encrypted containers
// that are recognised and refused, and the LZ4 decoder.
#include "formats/nro.hpp"

#include <random>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/nro_test_image.hpp"

namespace cb::formats::nro {
namespace {

using namespace test_image;

CB_TEST("formats.nro.reads_the_header_the_segments_and_the_dynamic_section", [] {
    const Program p = parse(nro_bytes());
    CB_CHECK(p.kind == Kind::Nro);
    CB_CHECK_EQ(p.segments.size(), 3u);
    CB_CHECK_EQ(p.segments[0].name, "text");
    CB_CHECK(p.segments[0].code && !p.segments[0].writable);
    CB_CHECK_EQ(p.segments[1].address, 0x1000u);
    CB_CHECK(p.segments[2].writable);
    CB_CHECK_EQ(p.segments[2].memory_size, 0x2000u);   // (the data and the bss)
    CB_CHECK_EQ(p.image_size, 0x4000u);
    CB_CHECK_EQ(p.flat.size(), 0x3000u);
    CB_CHECK_EQ(p.entry, 0u);
    CB_CHECK(p.in_code(0x80) && !p.in_code(0x1000));
    CB_CHECK_EQ(p.build_id.size(), 64u);
    CB_CHECK_EQ(p.build_id.substr(0, 4), "abab");
    CB_CHECK(p.mod_offset != 0);
    CB_CHECK_EQ(p.dynamic, 0x2000u);
    CB_CHECK_EQ(p.relocations.size(), 2u);
    CB_CHECK_EQ(p.relocation_tables.size(), 1u);
    CB_CHECK_EQ(p.relocation_tables[0].second, 48u);
    CB_CHECK(p.relocations[0].type == kRelativeReloc);
    CB_CHECK_EQ(p.relocations[0].offset, 0x2040u);
    CB_CHECK_EQ(p.relocations[0].addend, static_cast<std::int64_t>(0xB0));   // helper
    CB_CHECK_EQ(p.relocations[1].addend, 0x3010);
    CB_CHECK_EQ(p.asset_offset, 0x3000u);
    CB_CHECK(p.note.empty() && p.flat_is_file);
});

CB_TEST("formats.nro.keeps_the_asset_section", [] {
    const Program p = parse(nro_with_assets());
    CB_CHECK_EQ(p.asset_offset, 0x3000u);
    CB_CHECK_EQ(p.icon.offset, 0x3038u);
    CB_CHECK_EQ(p.icon.size, 8u);
    CB_CHECK_EQ(p.nacp.offset, 0x3040u);
    CB_CHECK_EQ(p.romfs.offset, 0x3048u);
    CB_CHECK_EQ(p.flat.size(), 0x3000u + 0x38 + 24);
    CB_CHECK_EQ(p.flat[0x3038], static_cast<std::uint8_t>('a'));
});

CB_TEST("formats.nro.refuses_what_is_cut_short", [] {
    Bytes b = nro_bytes();
    b.resize(0x2000);
    CB_CHECK_THROWS_AS(FormatError, parse(b));
    CB_CHECK_THROWS_AS(FormatError, parse(Bytes(100, 0)));
    Bytes lie = nro_bytes();
    lie[0x24] = 0xFF;   // a text segment larger than the file
    lie[0x25] = 0xFF;
    CB_CHECK_THROWS_AS(FormatError, parse(lie));
});

CB_TEST("formats.nro.a_nso_is_inflated", [] {
    const Bytes nso = nso_bytes();
    const Program p = parse(nso);
    CB_CHECK(p.kind == Kind::Nso);
    CB_CHECK(!p.flat_is_file);
    CB_CHECK_EQ(p.note, "compressed segments inflated");
    CB_CHECK_EQ(p.flat.size(), 0x3000u);
    CB_CHECK(p.flat == nro_bytes());
    CB_CHECK_EQ(p.segments[1].file_offset > 0x100, true);
    CB_CHECK_EQ(p.image_size, 0x4000u);
    CB_CHECK_EQ(p.dynamic, 0x2000u);
    CB_CHECK_EQ(p.relocations.size(), 2u);
    CB_CHECK_EQ(p.build_id.substr(0, 6), "000102");
    Bytes bad = nso;
    bad[0x62] = 0xFF;   // a compressed size that reaches past the end of the file
    CB_CHECK_THROWS_AS(FormatError, parse(bad));
});

CB_TEST("formats.nro.an_aarch64_elf_is_loaded", [] {
    const Program p = parse(elf_bytes());
    CB_CHECK(p.kind == Kind::Elf);
    CB_CHECK_EQ(p.segments.size(), 1u);
    CB_CHECK(p.segments[0].code);
    CB_CHECK_EQ(p.flat.size(), 16u);
    CB_CHECK_EQ(p.flat[0], static_cast<std::uint8_t>(0xE0));
    CB_CHECK_EQ(p.image_size, 16u);
    Bytes arm = elf_bytes();
    arm[18] = 40;   // an ARM ELF is not ours
    arm[19] = 0;
    CB_CHECK_THROWS_AS(FormatError, parse(arm));
});

CB_TEST("formats.nro.encrypted_containers_are_recognised_and_refused", [] {
    Bytes nsp(0x400, 0);
    nsp[0] = 'P';
    nsp[1] = 'F';
    nsp[2] = 'S';
    nsp[3] = '0';
    CB_CHECK_EQ(encrypted_container(nsp).value_or(""), "NSP (PFS0)");
    Bytes xci(0x400, 0);
    for (unsigned i = 0; i < 4; ++i) xci[0x100 + i] = static_cast<std::uint8_t>("HEAD"[i]);
    CB_CHECK_EQ(encrypted_container(xci).value_or(""), "XCI");
    Bytes nca(0x400, 0);
    for (unsigned i = 0; i < 4; ++i) nca[0x200 + i] = static_cast<std::uint8_t>("NCA3"[i]);
    CB_CHECK_EQ(encrypted_container(nca).value_or(""), "NCA");
    CB_CHECK(!encrypted_container(nro_bytes()).has_value());
    try {
        parse(nsp);
        CB_CHECK(false);
    } catch (const FormatError& e) {
        CB_CHECK(std::string(e.what()).find("NSP") != std::string::npos);
    }
});

CB_TEST("formats.nro.probe_reads_the_extension_and_the_magic", [] {
    fs::ScopedTempDir dir("cb_nro_probe");
    fs::write_file(dir.path() / "a.nro", nro_bytes());
    fs::write_file(dir.path() / "b.nso", nso_bytes());
    fs::write_file(dir.path() / "c.elf", elf_bytes());
    fs::write_file(dir.path() / "d.nro", Bytes(0x200, 0));
    fs::write_file(dir.path() / "e.bin", nro_bytes());
    CB_CHECK(probe(dir.path() / "a.nro"));
    CB_CHECK(probe(dir.path() / "b.nso"));
    CB_CHECK(probe(dir.path() / "c.elf"));
    CB_CHECK(!probe(dir.path() / "d.nro"));
    CB_CHECK(!probe(dir.path() / "e.bin"));
    CB_CHECK_EQ(read(dir.path() / "a.nro").segments.size(), 3u);
});

CB_TEST("formats.nro.lz4_round_trips_and_rejects_garbage", [] {
    std::mt19937 rng(5);
    for (const std::size_t n : {0u, 1u, 15u, 16u, 300u, 5000u, 70000u}) {
        Bytes data(n);
        for (std::size_t i = 0; i < n; ++i) data[i] = static_cast<std::uint8_t>((i % 13 < 9) ? (i / 7) % 5 : rng());   // (runs and noise)
        CB_CHECK(lz4_decompress(lz4_compress_simple(data), n) == data);
    }
    const Bytes packed = lz4_compress_simple(Bytes(1000, 7));
    CB_CHECK(packed.size() < 100);
    CB_CHECK_THROWS_AS(FormatError, lz4_decompress(packed, 999));
    CB_CHECK_THROWS_AS(FormatError, lz4_decompress(Bytes{0x11, 0x41, 0x05, 0x00}, 6));   // a match that reaches before the start
});

}  // namespace
}  // namespace cb::formats::nro
