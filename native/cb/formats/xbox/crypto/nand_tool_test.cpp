// Tests of cb/formats/xbox/crypto/nand_tool.hpp. The ECC values were computed by the original Python and checked against the
// one property real flash has: an erased page (every data and spare byte 0xFF) carries an all-ones ECC, so its spare stays 0xFF.
//
// DIVERGES from the Python tool:
//  - The spare layouts decode the bit-fields in C order (the first field in the LOW bits: block_id's top nibble is the low half of
//    byte 1, the block type the low six bits of byte 12), which is the order the ECC routines of the same module rely on; the
//    Python declared them as big-endian structures, putting both in the high bits. Its block_id/fs_size also masked the shifted
//    byte away ((x << 8) & 0xF), which always gave 0 for the high part.
//  - Flash reads and writes cover their last partial page (the Python's page loop stopped one page short, so a transfer that
//    ended inside a page lost that page's part); negative offsets are not offsets here.
//  - A flash offset inside a page's spare maps to the start of the next page's data; the Python gave an offset past the page.
//  - `nandtool` with an unknown operation is a usage error (the Python printed the usage and succeeded).
#include "cb/formats/xbox/crypto/nand_tool.hpp"

#include <string>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::formats::xbox::crypto::nand_tool;

namespace nand_tool_test {

Bytes ramp_page() {
    Bytes b(512);
    for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i);
    return b;
}
Bytes typed_page() {
    Bytes b(512);
    for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i * 7 + 3);
    return b;
}
// three pages: byte i of page p is (i*31 + p)
Bytes raw_three() {
    Bytes b;
    for (int p = 0; p < 3; ++p)
        for (int i = 0; i < 512; ++i) b.push_back(static_cast<std::uint8_t>(i * 31 + p));
    return b;
}
std::string hex(ByteView v) { return crypto::to_hex(v); }

CB_TEST("formats.xbox.nand_tool.page_ecc", [] {
    CB_CHECK_EQ(page_ecc(ramp_page(), Bytes(16, 0)), 0x2ef3873u);
    CB_CHECK_EQ(page_ecc(Bytes(512, 0), Bytes(16, 0)), 0x1ddb3e2u);
    // an erased page has the erased ECC
    CB_CHECK_EQ(page_ecc(Bytes(512, 0xff), Bytes(16, 0xff)), 0x3ffffffu);
    // the spare's bytes 0..11 and the low six bits of byte 12 are part of the sum; the stored ECC bits are not
    Bytes spare = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0x2a, 0, 0, 0};
    CB_CHECK_EQ(page_ecc(typed_page(), spare), 0x902797u);
    Bytes with_ecc = spare;
    with_ecc[13] = 0x55;
    with_ecc[15] = 0x99;
    with_ecc[12] |= 0xc0;
    CB_CHECK_EQ(page_ecc(typed_page(), with_ecc), 0x902797u);
    Bytes other = spare;
    other[3] ^= 1;
    CB_CHECK(page_ecc(typed_page(), other) != 0x902797u);

    CB_CHECK_THROWS_AS(UsageError, page_ecc(Bytes(511, 0), Bytes(16, 0)));
    CB_CHECK_THROWS_AS(UsageError, page_ecc(Bytes(512, 0), Bytes(15, 0)));
});

CB_TEST("formats.xbox.nand_tool.fix_page_ecc", [] {
    Bytes spare(16, 0);
    fix_page_ecc(ramp_page(), spare);
    CB_CHECK_EQ(hex(spare), "000000000000000000000000c01ccebb");
    CB_CHECK_EQ(stored_ecc(spare), 0x2ef3873u);

    Bytes typed = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0x2a, 0, 0, 0};
    fix_page_ecc(typed_page(), typed);
    CB_CHECK_EQ(hex(typed), "0102030405060708090a0b0ceae50924");   // the block type bits (0x2a & 0x3f) are kept

    Bytes erased(16, 0xff);
    fix_page_ecc(Bytes(512, 0xff), erased);
    CB_CHECK(erased == Bytes(16, 0xff));
});

CB_TEST("formats.xbox.nand_tool.spare_layouts", [] {
    // byte numbers 0..15 with the type/ECC byte set so the fields are distinguishable
    Bytes spare = {0x10, 0x23, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0xc5, 0x0f, 0x10, 0x11};

    BlockSpare small = parse_spare(SpareLayout::SmallBlock, spare);
    CB_CHECK_EQ(small.block_id, 0x310);       // low nibble of byte 1 (3) is the id's top, byte 0 its low byte
    CB_CHECK_EQ(small.fs_unused_0, 2);        // the high nibble of byte 1
    CB_CHECK(small.fs_sequence == (std::array<std::uint8_t, 4>{0x04, 0x05, 0x06, 0x08}));
    CB_CHECK_EQ(small.bad_block, 0x07);
    CB_CHECK_EQ(small.fs_size, 0x0a09);       // byte 8 high, byte 7 low
    CB_CHECK_EQ(small.fs_page_count, 0x0b);
    CB_CHECK_EQ(small.fs_block_type, 5);      // the low six bits of 0xc5
    CB_CHECK_EQ(small.ecc, 0x3u | (0x0fu << 2) | (0x10u << 10) | (0x11u << 18));

    BlockSpare on_small = parse_spare(SpareLayout::BigOnSmall, spare);
    CB_CHECK_EQ(on_small.block_id, 0x423);    // byte 1 is the low byte, byte 2's low nibble the top
    CB_CHECK_EQ(on_small.fs_unused_0, 0);
    CB_CHECK_EQ(on_small.fs_sequence[0], 0x10);

    BlockSpare big = parse_spare(SpareLayout::BigBlock, spare);
    CB_CHECK_EQ(big.bad_block, 0x10);
    CB_CHECK_EQ(big.fs_sequence[0], 0x07);

    // every layout round-trips
    for (const SpareLayout layout : {SpareLayout::SmallBlock, SpareLayout::BigOnSmall, SpareLayout::BigBlock}) {
        const BlockSpare fields = parse_spare(layout, spare);
        CB_CHECK(encode_spare(layout, fields) == spare);
        CB_CHECK(parse_spare(layout, encode_spare(layout, fields)) == fields);
    }
    // the ECC bits agree with the ECC routines: a spare that fix_page_ecc finished parses to that ECC
    Bytes fixed(16, 0);
    fix_page_ecc(ramp_page(), fixed);
    CB_CHECK_EQ(parse_spare(SpareLayout::SmallBlock, fixed).ecc, page_ecc(ramp_page(), fixed));

    CB_CHECK_THROWS_AS(FormatError, parse_spare(SpareLayout::SmallBlock, Bytes(15, 0)));
    BlockSpare too_wide;
    too_wide.block_id = 0x1000;
    CB_CHECK_THROWS_AS(UsageError, encode_spare(SpareLayout::SmallBlock, too_wide));
    too_wide = {};
    too_wide.ecc = 1u << 26;
    CB_CHECK_THROWS_AS(UsageError, encode_spare(SpareLayout::SmallBlock, too_wide));
    too_wide = {};
    too_wide.fs_block_type = 0x40;
    CB_CHECK_THROWS_AS(UsageError, encode_spare(SpareLayout::BigBlock, too_wide));
});

CB_TEST("formats.xbox.nand_tool.build_and_check_images", [] {
    const Bytes raw = raw_three();
    const Bytes image = add_spare(raw);
    CB_CHECK_EQ(image.size(), std::size_t{3 * 528});
    CB_CHECK_EQ(hex(ByteView(image).subspan(512, 16)), "00000000000000000000000000cd8045");
    CB_CHECK_EQ(hex(ByteView(image).subspan(528 + 512, 16)), "00000000000000000000000000e2716f");
    CB_CHECK_EQ(hex(ByteView(image).subspan(2 * 528 + 512, 16)), "000000000000000000000000802130cf");
    CB_CHECK(verify_ecc(image).empty());
    CB_CHECK(strip_spare(image) == raw);

    // damage page 1's data, find it, repair it
    Bytes bad = image;
    bad[528 + 5] ^= 1;
    CB_CHECK(verify_ecc(bad) == std::vector<std::size_t>{1});
    CB_CHECK(verify_ecc(fix_all_ecc(bad)).empty());
    CB_CHECK(fix_all_ecc(image) == image);

    // a meta template supplies the spares' other bytes; the ECC is still recomputed
    Bytes meta(48);
    for (int i = 0; i < 48; ++i) meta[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(i);
    const Bytes with_meta = add_spare(raw, ByteView(meta));
    CB_CHECK_EQ(hex(ByteView(with_meta).subspan(512, 16)), "000102030405060708090a0bccbd8c63");
    CB_CHECK_EQ(hex(ByteView(with_meta).subspan(528 + 512, 16)), "101112131415161718191a1bdcddd45c");
    CB_CHECK(verify_ecc(with_meta).empty());
    CB_CHECK_THROWS_AS(UsageError, add_spare(raw, ByteView(meta).first(47)));

    // a partial trailing page belongs to nothing
    CB_CHECK(strip_spare(ByteView(image).first(528 * 2 + 100)).size() == 1024);
    CB_CHECK(verify_ecc(ByteView(image).first(528 * 2 + 100)).empty());
    CB_CHECK(add_spare(ByteView(raw).first(512 + 10)).size() == 528);
    CB_CHECK(add_spare(Bytes{}).empty());
    CB_CHECK(strip_spare(Bytes{}).empty());
});

CB_TEST("formats.xbox.nand_tool.image_pages", [] {
    NandImage image = NandImage::from_bytes(add_spare(raw_three()));
    CB_CHECK_EQ(image.page_count(), std::size_t{3});
    CB_CHECK_EQ(image.flash_size(), std::size_t{1536});
    CB_CHECK_EQ(image.file_size(), std::size_t{1584});
    CB_CHECK_EQ(image.page(1)[0], 1);
    CB_CHECK_EQ(image.page(1)[1], 32);
    CB_CHECK_EQ(image.spare(0).size(), std::size_t{16});

    // replacing a page recomputes its ECC from its data and the spare it has
    Bytes replaced(512, 0xab);
    image.set_page(2, replaced);
    CB_CHECK(Bytes(image.page(2).begin(), image.page(2).end()) == replaced);
    CB_CHECK(verify_ecc(image.file_bytes()).empty());
    // a spare is stored as given
    Bytes spare(16, 0x5a);
    image.set_spare(0, spare);
    CB_CHECK(Bytes(image.spare(0).begin(), image.spare(0).end()) == spare);
    CB_CHECK(verify_ecc(image.file_bytes()) == std::vector<std::size_t>{0});

    CB_CHECK_THROWS_AS(UsageError, (void)image.page(3));
    CB_CHECK_THROWS_AS(UsageError, (void)image.spare(3));
    CB_CHECK_THROWS_AS(UsageError, image.set_page(0, Bytes(511, 0)));
    CB_CHECK_THROWS_AS(UsageError, image.set_page(3, Bytes(512, 0)));
    CB_CHECK_THROWS_AS(UsageError, image.set_spare(0, Bytes(17, 0)));
});

CB_TEST("formats.xbox.nand_tool.image_flash_io", [] {
    const Bytes raw = raw_three();
    NandImage image = NandImage::from_bytes(add_spare(raw));

    // a read across both page boundaries skips the spares; so does one that ends inside a page
    CB_CHECK(image.read_flash(500, 30) == Bytes(raw.begin() + 500, raw.begin() + 530));
    CB_CHECK(image.read_flash(0, 1536) == raw);
    CB_CHECK(image.read_flash(1536, 0).empty());
    CB_CHECK(image.read_flash(700, 600) == Bytes(raw.begin() + 700, raw.begin() + 1300));
    CB_CHECK_THROWS_AS(UsageError, (void)image.read_flash(1530, 7));
    CB_CHECK_THROWS_AS(UsageError, (void)image.read_flash(1537, 0));

    // a write across the boundaries patches the data and keeps every touched page's ECC right
    Bytes patch(600, 0xee);
    image.write_flash(300, patch);
    Bytes expect = raw;
    for (std::size_t i = 0; i < patch.size(); ++i) expect[300 + i] = 0xee;
    CB_CHECK(image.read_flash(0, 1536) == expect);
    CB_CHECK(verify_ecc(image.file_bytes()).empty());
    CB_CHECK_THROWS_AS(UsageError, image.write_flash(1500, Bytes(100, 0)));
    // the spares of the written pages kept their other bytes
    NandImage tagged = NandImage::from_bytes(add_spare(raw, ByteView(Bytes(48, 0x33))));
    tagged.write_flash(0, Bytes(1, 7));
    CB_CHECK_EQ(tagged.spare(0)[0], 0x33);
    CB_CHECK_EQ(tagged.page(0)[0], 7);
});

CB_TEST("formats.xbox.nand_tool.image_files", [] {
    fs::ScopedTempDir tmp("nandimg");
    const std::filesystem::path path = tmp.path() / "flash.bin";
    fs::write_file(path, add_spare(raw_three()));
    NandImage image = NandImage::open(path);
    image.write_flash(10, Bytes(4, 1));
    image.save();
    CB_CHECK(NandImage::open(path).read_flash(10, 4) == Bytes(4, 1));
    image.save_as(tmp.path() / "copy.bin");
    CB_CHECK(fs::read_file(tmp.path() / "copy.bin") == fs::read_file(path));
    CB_CHECK_THROWS_AS(UsageError, NandImage::from_bytes(Bytes{}).save());
    CB_CHECK_THROWS_AS(NotFoundError, (void)NandImage::open(tmp.path() / "none.bin"));
    CB_CHECK_EQ(NandImage::from_bytes(Bytes{}).page_count(), std::size_t{0});
});

CB_TEST("formats.xbox.nand_tool.offsets", [] {
    CB_CHECK_EQ(file_to_flash_offset(0), 0u);
    CB_CHECK_EQ(file_to_flash_offset(511), 511u);
    CB_CHECK_EQ(file_to_flash_offset(512), 512u);      // inside the first spare: the next page's start
    CB_CHECK_EQ(file_to_flash_offset(527), 512u);
    CB_CHECK_EQ(file_to_flash_offset(528), 512u);
    CB_CHECK_EQ(file_to_flash_offset(528 + 7), 512u + 7);
    CB_CHECK_EQ(flash_to_file_offset(0), 0u);
    CB_CHECK_EQ(flash_to_file_offset(511), 511u);
    CB_CHECK_EQ(flash_to_file_offset(512), 528u);
    CB_CHECK_EQ(flash_to_file_offset(1030), 2u * 528 + 6);
    for (std::uint64_t flash : {0ull, 1ull, 511ull, 512ull, 100000ull}) CB_CHECK_EQ(file_to_flash_offset(flash_to_file_offset(flash)), flash);
});

CB_TEST("formats.xbox.nand_tool.command", [] {
    fs::ScopedTempDir tmp("nandcmd");
    const std::string raw_path = fs::to_text(tmp.path() / "raw.bin");
    const std::string img_path = fs::to_text(tmp.path() / "flash.bin");
    const std::string back_path = fs::to_text(tmp.path() / "back.bin");
    const std::string fixed_path = fs::to_text(tmp.path() / "fixed.bin");
    fs::write_file(raw_path, raw_three());

    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{}), 0);
        CB_CHECK_EQ(c.out(), std::string(usage()));
        CB_CHECK(c.out().starts_with("usage:\n  nandtool info"));
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"ecc", raw_path, img_path}), 0);
        CB_CHECK_EQ(c.out(), "built 3 pages with ECC -> " + img_path + " (1584 bytes)\n");
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"info", img_path}), 0);
        CB_CHECK_EQ(c.out(),
                    "file size   : 0x630 (1584 bytes)\n"
                    "layout      : 3 pages x 528 (512 data + 16 spare)\n"
                    "flash size  : 0x600 (0 MB)\n"
                    "ECC         : all 3 pages OK\n");
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"info", raw_path}), 0);
        CB_CHECK_EQ(c.out(),
                    "file size   : 0x600 (1536 bytes)\n"
                    "layout      : 3 raw pages x 512 (no spare) \xE2\x80\x94 use `ecc` to build\n");
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"verify", img_path}), 0);
        CB_CHECK_EQ(c.out(), "pages       : 3\nECC result  : all pages valid\n");
    }
    // damage a page: verify says which and exits 1; fix repairs
    Bytes damaged = fs::read_file(img_path);
    damaged[528 * 2 + 9] ^= 0x80;
    fs::write_file(img_path, damaged);
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"verify", img_path}), 1);
        CB_CHECK_EQ(c.out(), "pages       : 3\nECC result  : 1 bad: [2]\n");
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"info", img_path}), 0);
        CB_CHECK(c.out().ends_with("ECC         : 1 bad pages (e.g. [2])\n"));
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"fix", img_path, fixed_path}), 0);
        CB_CHECK_EQ(c.out(), "recomputed ECC for 3 pages -> " + fixed_path + "\n");
        CB_CHECK(verify_ecc(fs::read_file(fixed_path)).empty());
    }
    {
        testing::CapturedStd c;
        CB_CHECK_EQ(command(std::vector<std::string>{"unecc", fixed_path, back_path}), 0);
        CB_CHECK_EQ(c.out(), "stripped 3 pages -> " + back_path + " (1536 bytes raw)\n");
        CB_CHECK(fs::read_file(back_path).size() == 1536);
    }

    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"bogus"}));
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"info"}));
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"ecc", raw_path}));
    CB_CHECK_THROWS_AS(NotFoundError, command(std::vector<std::string>{"verify", fs::to_text(tmp.path() / "none.bin")}));

    // a file that is neither whole raw nor whole flash pages
    fs::write_file(raw_path, Bytes(700, 1));
    testing::CapturedStd c;
    CB_CHECK_EQ(command(std::vector<std::string>{"info", raw_path}), 0);
    CB_CHECK(c.out().ends_with("layout      : not a whole number of 512/528 pages\n"));
});

}  // namespace nand_tool_test
