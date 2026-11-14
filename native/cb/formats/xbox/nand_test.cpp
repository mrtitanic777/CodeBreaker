// Tests of cb/formats/xbox/nand.hpp on a small flash made by a recipe (a newer root block, an older one, a two-block file, a file whose
// logical block is carried by another physical block, a directory name met twice). A flash reads the same recipe's bytes
// as: root block 5, files one.bin (start 1, size 16584, time 333: the later of its two
// entries) and two.bin (start 3, size 50, time 222), and the SHA-1s below.
//
// Behaviour pinned here: a flash whose file chain or pages lie outside the dump is FormatError, and a
// directory name's non-ASCII byte becomes U+FFFD in UTF-8.
#include "cb/formats/xbox/nand.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace nand_test {

using namespace cb;
using namespace cb::formats::xbox;

constexpr std::size_t kPageWithSpare = nand::kPage + nand::kSpare;

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

// The recipe of make_flash.
static Bytes make_flash() {
    constexpr std::size_t blocks = 6;
    Bytes raw(blocks * nand::kPagesPerBlock * kPageWithSpare, 0xFF);
    const auto page_at = [](std::size_t p) { return p * kPageWithSpare; };
    const auto spare_at = [](std::size_t b) { return b * nand::kPagesPerBlock * kPageWithSpare + nand::kPage; };
    for (const auto [block, sequence] : {std::pair<std::size_t, std::uint32_t>{0, 1}, {5, 2}}) {
        Bytes s(nand::kSpare, 0xFF);
        s[0] = static_cast<std::uint8_t>(sequence >> 24);
        s[3] = static_cast<std::uint8_t>(sequence >> 16);
        s[4] = static_cast<std::uint8_t>(sequence >> 8);
        s[6] = static_cast<std::uint8_t>(sequence);
        s[12] = 0x30;
        std::copy(s.begin(), s.end(), raw.begin() + static_cast<std::ptrdiff_t>(spare_at(block)));
    }
    constexpr std::size_t root = 5;
    std::vector<std::uint16_t> alloc(4096, 0);
    alloc[1] = 2;
    alloc[2] = 0x1FFF;
    alloc[3] = 0x1FFF;
    for (std::size_t i = 0; i < nand::kPagesPerBlock; i += 2) {
        std::size_t at = page_at(root * nand::kPagesPerBlock + i);
        for (std::size_t k = 0; k < 256; ++k) {
            const std::uint16_t a = alloc[(i / 2) * 256 + k];
            raw[at++] = static_cast<std::uint8_t>(a >> 8);
            raw[at++] = static_cast<std::uint8_t>(a);
        }
    }
    const auto dirent = [](const std::string& name, std::uint16_t start, std::uint32_t size, std::uint32_t time) {
        Bytes e(22, 0);
        std::copy(name.begin(), name.end(), e.begin());
        e.push_back(static_cast<std::uint8_t>(start >> 8));
        e.push_back(static_cast<std::uint8_t>(start));
        for (const std::uint32_t v : {size, time}) {
            for (int s = 24; s >= 0; s -= 8) e.push_back(static_cast<std::uint8_t>(v >> s));
        }
        return e;
    };
    Bytes directory;
    append(directory, dirent("one.bin", 1, 0x4000 + 100, 111));
    append(directory, dirent("two.bin", 3, 50, 222));
    append(directory, dirent("one.bin", 1, 0x4000 + 200, 333));
    std::copy(directory.begin(), directory.end(), raw.begin() + static_cast<std::ptrdiff_t>(page_at(root * nand::kPagesPerBlock + 1)));
    for (const auto [block, seed] : {std::pair<std::size_t, std::size_t>{1, 1}, {2, 2}, {4, 3}}) {
        for (std::size_t i = 0; i < nand::kPagesPerBlock; ++i) {
            std::size_t at = page_at(block * nand::kPagesPerBlock + i);
            for (std::size_t k = 0; k < nand::kPage; ++k) raw[at++] = static_cast<std::uint8_t>(seed * 31 + i * 7 + k);
        }
    }
    Bytes s(nand::kSpare, 0xFF);   // block 4 carries logical block 3
    s[1] = 3;
    s[2] = 0;
    s[12] = 0x01;
    std::copy(s.begin(), s.end(), raw.begin() + static_cast<std::ptrdiff_t>(spare_at(4)));
    return raw;
}

CB_TEST("formats.xbox.nand.the_flash_reads", [] {
    const Bytes raw = make_flash();
    CB_CHECK_EQ(sha1_hex(raw), "daae5e1c90915c9a5c4ca826a9ba7f6c9ca801da");   // the recipe is deterministic
    const nand::Flash flash = nand::Flash::parse(raw);
    CB_CHECK_EQ(flash.root_block(), 5u);
    CB_CHECK_EQ(flash.files().size(), 2u);
    CB_CHECK_EQ(flash.files()[0].name, "one.bin");   // (the name met twice keeps its place and takes the later entry)
    CB_CHECK_EQ(flash.files()[0].start_block, 1u);
    CB_CHECK_EQ(flash.files()[0].size, 16584u);
    CB_CHECK_EQ(flash.files()[0].time, 333u);
    CB_CHECK_EQ(flash.files()[1].name, "two.bin");
    CB_CHECK_EQ(flash.files()[1].time, 222u);
    CB_CHECK_EQ(sha1_hex(flash.read("one.bin")), "9a2e7d724a974523f5b6fd01351e032a612c91f7");
    CB_CHECK_EQ(sha1_hex(flash.read("two.bin")), "5b0b57178786d9bf848e56fb4d9f738870bf5fd7");
    CB_CHECK_EQ(flash.chain(*flash.find("one.bin")).size(), 2u);
    CB_CHECK(flash.find("nothing") == nullptr);
});

CB_TEST("formats.xbox.nand.damaged_flash_is_a_format_error", [] {
    Bytes raw = make_flash();
    CB_CHECK_THROWS_AS(FormatError, (void)nand::Flash::parse(Bytes(raw.begin(), raw.end() - 1)));   // not whole pages
    CB_CHECK_THROWS_AS(FormatError, (void)nand::Flash::parse(Bytes(100 * kPageWithSpare, 0xFF)));    // no root at all
    const nand::Flash flash = nand::Flash::parse(raw);
    CB_CHECK_THROWS_AS(FormatError, (void)flash.read("missing.bin"));

    // a chain that runs off the allocation table: one.bin's size claims 3 blocks but its chain ends after two
    Bytes shortened = raw;
    const std::size_t dir_page = (5 * nand::kPagesPerBlock + 1) * kPageWithSpare;
    shortened[dir_page + 3 * 32 - 32 + 24 + 1] = 0x01;   // (the third entry's size: 0x0001xxxx bytes)
    const nand::Flash big = nand::Flash::parse(shortened);
    CB_CHECK_THROWS_AS(FormatError, (void)big.read("one.bin"));
});

CB_TEST("formats.xbox.nand.is_nand_tells_a_dump_by_its_size_and_magic", [] {
    fs::ScopedTempDir dir("cb-nand");
    // too small
    const auto small = dir.path() / "small.bin";
    fs::write_file(small, make_flash());
    CB_CHECK(!nand::is_nand(small));
    // 16 MB of whole pages with the flash magic and the copyright
    Bytes dump((0x1000000 / kPageWithSpare + 1) * kPageWithSpare, 0xFF);
    dump[0] = 0xFF;
    dump[1] = 0x4F;
    const std::string_view notice = "(c) Microsoft Corporation";
    std::copy(notice.begin(), notice.end(), dump.begin() + 0x10);
    const auto big = dir.path() / "dump.bin";
    fs::write_file(big, dump);
    CB_CHECK(nand::is_nand(big));
    dump[1] = 0x00;   // the wrong magic
    fs::write_file(big, dump);
    CB_CHECK(!nand::is_nand(big));
    CB_CHECK(!nand::is_nand(dir.path() / "missing.bin"));
});

}  // namespace nand_test
