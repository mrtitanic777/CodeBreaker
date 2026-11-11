#include "cb/formats/sms.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::sms {
namespace {

// `pages` 16 KB pages, page n filled with 0x40+n; an optional header at `header_at`: TMR SEGA, checksum, product
// bytes (low, mid), top-digit/version byte, region/size byte.
Bytes image(unsigned pages, std::uint32_t header_at = 0, std::uint8_t product_lo = 0, std::uint8_t product_mid = 0,
            std::uint8_t digit_version = 0, std::uint8_t region_size = 0) {
    Bytes out;
    for (unsigned p = 0; p < pages; ++p) out.insert(out.end(), kPage, static_cast<std::uint8_t>(0x40 + p));
    if (header_at != 0) {
        const Bytes h = {'T', 'M', 'R', ' ', 'S', 'E', 'G', 'A', 0, 0, 0x34, 0x12, product_lo, product_mid, digit_version, region_size};
        std::copy(h.begin(), h.end(), out.begin() + header_at);
    }
    return out;
}

CB_TEST("formats.sms.header_fields", [] {
    // product code 5 12 34 (BCD digits 5,1,2,3,4), version 3, region 4 (export), size code C (32 KB)
    const Rom rom = parse(image(2, 0x3FF0, 0x34, 0x12, 0x53, 0x4C));
    CB_CHECK(rom.header.has_value());
    CB_CHECK_EQ(rom.header->at, 0x3FF0u);
    CB_CHECK_EQ(rom.header->checksum, 0x1234);
    CB_CHECK_EQ(rom.header->product, "51234");
    CB_CHECK_EQ(rom.header->version, 3);
    CB_CHECK_EQ(rom.header->region, "export");
    CB_CHECK_EQ(*rom.header->size, 0x8000u);
    CB_CHECK_EQ(rom.system, System::MasterSystem);
    CB_CHECK_EQ(to_string(rom.system), "Master System");
});

CB_TEST("formats.sms.header_locations", [] {
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0, 0, 0, 0x4C)).header->at, 0x7FF0u);
    CB_CHECK_EQ(parse(image(1, 0x1FF0, 0, 0, 0, 0x4C)).header->at, 0x1FF0u);
    CB_CHECK(!parse(image(2)).header.has_value());
    CB_CHECK(!parse(image(1, 0x2FF0, 0, 0, 0, 0x4C)).header.has_value());   // not one of the three places
});

CB_TEST("formats.sms.product_drops_leading_zeros", [] {
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0x07, 0x00, 0x01, 0x4C)).header->product, "7");   // 0 00 07 -> "00007"
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0x00, 0x00, 0x00, 0x4C)).header->product, "0");
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0x10, 0x00, 0x00, 0x4C)).header->product, "10");
});

CB_TEST("formats.sms.regions_decide_the_system", [] {
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0, 0, 0, 0x6C)).system, System::GameGear);
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0, 0, 0, 0x6C)).header->region, "export");
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0, 0, 0, 0x7C)).header->region, "international");
    CB_CHECK_EQ(parse(image(2, 0x7FF0, 0, 0, 0, 0x3C)).header->region, "Japan");
    // an unknown region code: "?", and the Master System by default
    const Rom odd = parse(image(2, 0x7FF0, 0, 0, 0, 0x9C));
    CB_CHECK_EQ(odd.header->region, "?");
    CB_CHECK_EQ(odd.system, System::MasterSystem);
    CB_CHECK(parse(image(2, 0x7FF0, 0, 0, 0, 0x4C)).header->size.has_value());
    CB_CHECK(!parse(image(2, 0x7FF0, 0, 0, 0, 0x43)).header->size.has_value());   // size code 3 is not listed
});

CB_TEST("formats.sms.without_a_header_the_extension_names_the_system", [] {
    CB_CHECK_EQ(parse(image(2), ".gg").system, System::GameGear);
    CB_CHECK_EQ(parse(image(2), ".GG").system, System::GameGear);
    CB_CHECK_EQ(parse(image(2), ".sg").system, System::Sg1000);
    CB_CHECK_EQ(parse(image(2), ".sms").system, System::MasterSystem);
    CB_CHECK_EQ(parse(image(2)).system, System::MasterSystem);
});

CB_TEST("formats.sms.copier_header_is_dropped", [] {
    Bytes img(512, 0xEE);
    const Bytes body = image(2);
    img.insert(img.end(), body.begin(), body.end());
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.data.size(), 2 * kPage);
    CB_CHECK_EQ(rom.data.front(), 0x40);
});

CB_TEST("formats.sms.pages_and_power_on_memory", [] {
    const Rom rom = parse(image(4));
    CB_CHECK_EQ(rom.page_count(), 4u);
    CB_CHECK_EQ(rom.page(3).front(), 0x43);
    CB_CHECK(rom.has_mapper);                                 // 64 KB: more than the three slots
    const Bytes mem = rom.memory();
    CB_CHECK_EQ(mem.size(), 0x10000u);
    CB_CHECK_EQ(mem[0x0000], 0x40);
    CB_CHECK_EQ(mem[0x4000], 0x41);
    CB_CHECK_EQ(mem[0x8000], 0x42);
    CB_CHECK_EQ(mem[0xC000], 0);                              // RAM, and page 3 is not mapped
    CB_CHECK_EQ(*rom.mapped(0x8000), 0x8000u);
    CB_CHECK(!rom.mapped(0xC000));
    const Rom small = parse(image(1));
    CB_CHECK(!small.has_mapper);
    CB_CHECK(!small.mapped(0x4000));                          // past the end of a 16 KB ROM
    CB_CHECK_EQ(small.page(0).size(), kPage);
    CB_CHECK(!parse(image(4), ".sg").has_mapper);             // the SG-1000 has none
});

CB_TEST("formats.sms.a_partial_last_page", [] {
    Bytes img = image(1);
    img.resize(kPage + 100, 0x55);
    const Rom rom = parse(img);
    CB_CHECK_EQ(rom.page_count(), 2u);
    CB_CHECK_EQ(rom.page(1).size(), 100u);
});

CB_TEST("formats.sms.functions", [] {
    // A toy Z80: C9 ret, CD lo hi call, 00 nop.
    const Decoder decode = [](ByteView mem, std::uint32_t pc) -> std::optional<DecodedInsn> {
        switch (mem[pc]) {
            case 0x00: return DecodedInsn{1, Flow::Next, std::nullopt};
            case 0xC9: return DecodedInsn{1, Flow::Return, std::nullopt};
            case 0xCD: return DecodedInsn{3, Flow::Call, static_cast<std::uint32_t>(mem[pc + 1] | (mem[pc + 2] << 8))};
            default: return std::nullopt;
        }
    };
    Bytes img(kPage, 0xFF);                      // every vector filled with $FF: unused
    img[0] = 0xCD; img[1] = 0x00; img[2] = 0x02; // reset: call $0200
    img[3] = 0xC9;
    img[0x200] = 0xC9;
    img[0x66] = 0xC9;                            // the NMI vector holds code
    const Rom rom = parse(img);
    // $0038 is $FF: not a start; $0066 holds a ret; $0000 always is one
    CB_CHECK((rom.functions(decode)) == ((std::vector<std::uint32_t>{0x0000, 0x0066, 0x0200})));
    // a call past the ROM is not followed
    img[0x200] = 0xCD; img[0x201] = 0x00; img[0x202] = 0x80; img[0x203] = 0xC9;
    CB_CHECK((parse(img).functions(decode)) == ((std::vector<std::uint32_t>{0x0000, 0x0066, 0x0200})));
});

CB_TEST("formats.sms.read_and_probe_a_file", [] {
    fs::ScopedTempDir dir("sms-test");
    fs::write_file(dir.path() / "game.gg", image(2));
    fs::write_file(dir.path() / "tiny.sms", Bytes(100, 0));
    fs::write_file(dir.path() / "game.bin", image(2));
    CB_CHECK(probe(dir.path() / "game.gg"));
    CB_CHECK(!probe(dir.path() / "tiny.sms"));      // under 8 KB
    CB_CHECK(!probe(dir.path() / "game.bin"));      // not a cartridge extension
    CB_CHECK(!probe(dir.path() / "missing.sms"));
    CB_CHECK_EQ(read(dir.path() / "game.gg").system, System::GameGear);
});

}  // namespace
}  // namespace cb::formats::sms
