// Tests of cb/formats/xbox/crypto/build_lib.hpp: the header layouts, patch tables, the jump the 4BL's loader is rewritten with, the
// in-place bootloader operations (size, RC4, the SE's hash, the signature), the 4BL signing, the SE's LZX wrappers around a codec of
// the test's own, the first source that has content, and the Free60 tool wrappers' failure paths. Keys are generated here (a 2048-bit
// private key by the operating system's generator); every signature is checked by recomputing what it signs.
//
// DIVERGES from the Python (build_lib.py, the old harness had no cases for it):
//   * The in-place functions take the image and the offset instead of an open stream, and work on bytes: no position is left to
//     restore, and a bootloader that reaches past the end of the image is a FormatError (the Python hashed or signed what was there).
//   * An SD shorter than 0x120 bytes is a FormatError in sign_sd_4bl / verify_sd_4bl (the Python hashed the first 0x10 bytes alone
//     and grew the bytearray to hold the signature).
//   * A patch table that ends before its 0xFFFFFFFF, or inside a record, is a TruncatedError (the stream class raised at the end).
//   * apply_jump_sd_4bl with no 0x4C000024 in the image is a FormatError (the stream raised at its end).
//   * The tools are run without a shell (an argument list), the ELF is made in a work folder of its own (the Python wrote temp.elf in
//     the working folder), and a failure is a ToolError with the tool's name and exit code (the Python asserted).
//   * The ctypes structures are FlashHeader and BootloaderHeader (the nonce is present or not); the BLHeader's reflection
//     (__getitem__ / __setitem__ / __dict__) and the file helpers read_file / write_file (cb::fs) are not carried over.
#include "cb/formats/xbox/crypto/build_lib.hpp"

#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace build_lib_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;
namespace bl = cb::formats::xbox::crypto::build_lib;
namespace xc = cb::formats::xbox::crypto::xecrypt;
namespace stdfs = std::filesystem;

Bytes ramp(std::size_t n, std::uint8_t start = 0) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(start + i * 7);
    return b;
}

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
std::uint32_t get32(const Bytes& b, std::size_t at) {
    return (std::uint32_t{b[at]} << 24) | (std::uint32_t{b[at + 1]} << 16) | (std::uint32_t{b[at + 2]} << 8) | b[at + 3];
}

// A patch table of (address, words) records.
Bytes table(std::initializer_list<std::pair<std::uint32_t, Bytes>> records) {
    Bytes t;
    for (const auto& [address, words] : records) {
        t.resize(t.size() + 8);
        put32(t, t.size() - 8, address);
        put32(t, t.size() - 4, static_cast<std::uint32_t>(words.size() / 4));
        t.insert(t.end(), words.begin(), words.end());
    }
    t.resize(t.size() + 4, 0xFF);
    return t;
}

// A bootloader image of `total` bytes (a magic, size and nonce at the front, a body after).
Bytes bootloader(const char* magic, std::size_t total) {
    Bytes b = ramp(total, 3);
    b[0] = static_cast<std::uint8_t>(magic[0]);
    b[1] = static_cast<std::uint8_t>(magic[1]);
    put32(b, 0xC, static_cast<std::uint32_t>(total));
    return b;
}

const xc::RsaKey& key() {
    static const xc::RsaKey k = xc::RsaKey::generate(2048);
    return k;
}

CB_TEST("formats.xbox.crypto.build_lib.the_flash_header_is_0x80_big_endian_bytes", [] {
    bl::FlashHeader h;
    h.magic = 0xFF4F;
    h.build = 1888;
    h.qfe = 2;
    h.flags = 0x8000;
    h.entry = 0x1234;
    h.size = 0x4000;
    h.copyright[0] = 'M';
    h.copyright[1] = 'S';
    h.padding[3] = 7;
    h.kv_length = 0x4000;
    h.sys_upd_addr = 0x700000;
    h.patch_slots = 2;
    h.kv_version = 3;
    h.kv_offset = 0x4000;
    h.patch_slot_size = 0x10000;
    h.smc_config_offset = 0x3FA0;
    h.smc_length = 0x3000;
    h.smc_offset = 0x1000;
    const Bytes bytes = h.to_bytes();
    CB_CHECK_EQ(bytes.size(), bl::FlashHeader::kSize);
    CB_CHECK_EQ(bytes[0], std::uint8_t{0xFF});   // big-endian
    CB_CHECK_EQ(bytes[1], std::uint8_t{0x4F});
    CB_CHECK_EQ(bytes[2], std::uint8_t{0x07});   // build 1888 = 0x0760
    CB_CHECK_EQ(bytes[3], std::uint8_t{0x60});
    CB_CHECK_EQ(bytes[0x10], std::uint8_t{'M'});
    CB_CHECK_EQ(bytes[0x53], std::uint8_t{7});
    const bl::FlashHeader back = bl::FlashHeader::parse(bytes);
    CB_CHECK(back.to_bytes() == bytes);
    CB_CHECK_EQ(back.build, std::uint16_t{1888});
    CB_CHECK_EQ(back.smc_offset, std::uint32_t{0x1000});
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::FlashHeader::parse(ByteView(bytes).first(0x7F)));
});

CB_TEST("formats.xbox.crypto.build_lib.a_bootloader_header_with_and_without_its_nonce", [] {
    Bytes data = bootloader("SD", 0x40);
    const bl::BootloaderHeader with = bl::BootloaderHeader::parse(data);
    CB_CHECK_EQ(std::string(reinterpret_cast<const char*>(with.magic.data()), 2), std::string("SD"));
    CB_CHECK_EQ(with.size, std::uint32_t{0x40});
    CB_CHECK(with.nonce.has_value());
    CB_CHECK_EQ(with.header_size(), std::size_t{0x20});
    CB_CHECK(with.to_bytes() == Bytes(data.begin(), data.begin() + 0x20));
    const bl::BootloaderHeader without = bl::BootloaderHeader::parse(data, false);
    CB_CHECK(!without.nonce.has_value());
    CB_CHECK_EQ(without.header_size(), std::size_t{0x10});
    CB_CHECK(without.to_bytes() == Bytes(data.begin(), data.begin() + 0x10));
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::BootloaderHeader::parse(ByteView(data).first(0x18)));
    // the size the console loads is rounded up to 16
    bl::BootloaderHeader odd;
    odd.size = 0x1F5;
    CB_CHECK_EQ(odd.padded_size(), std::uint32_t{0x200});
    odd.size = 0x200;
    CB_CHECK_EQ(odd.padded_size(), std::uint32_t{0x200});
});

CB_TEST("formats.xbox.crypto.build_lib.the_magics_are_the_loaders", [] {
    CB_CHECK_EQ(static_cast<int>(bl::BlMagic::Sd4bl), 0x5344);
    CB_CHECK_EQ(static_cast<int>(bl::BlMagic::Ca1bl), 0x0342);
    CB_CHECK_EQ(static_cast<int>(bl::BlMagic::Cg7bl), 0x4347);
});

CB_TEST("formats.xbox.crypto.build_lib.a_patch_table_is_records_up_to_the_end_marker", [] {
    const Bytes words = ramp(8);
    const Bytes t = table({{0x100, words}, {0x2000, ramp(12, 1)}});
    const auto records = bl::patch_records(t);
    CB_CHECK_EQ(records.size(), std::size_t{2});
    CB_CHECK_EQ(records[0].address, std::uint32_t{0x100});
    CB_CHECK(Bytes(records[0].words.begin(), records[0].words.end()) == words);
    CB_CHECK_EQ(records[1].words.size(), std::size_t{12});
    CB_CHECK(bl::patch_records(table({})).empty());
    // a table that ends without its marker, or in the middle of a record
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::patch_records(ByteView(t).first(t.size() - 4)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::patch_records(ByteView(t).first(12)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::patch_records(Bytes{}));
    // a count of words past the end of the table is refused, not read
    Bytes huge = table({});
    huge.insert(huge.begin(), {0, 0, 1, 0, 0xFF, 0xFF, 0xFF, 0x00});
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::patch_records(huge));
});

CB_TEST("formats.xbox.crypto.build_lib.patches_are_written_at_their_addresses_and_may_grow_the_image", [] {
    const Bytes image = ramp(0x40);
    const Bytes patch = {0xDE, 0xAD, 0xBE, 0xEF};
    const Bytes patched = bl::apply_patches(image, table({{0x10, patch}}));
    CB_CHECK_EQ(patched.size(), std::size_t{0x40});
    CB_CHECK(Bytes(patched.begin() + 0x10, patched.begin() + 0x14) == patch);
    CB_CHECK(patched[0xF] == image[0xF] && patched[0x14] == image[0x14]);
    // a record past the end grows the image (zeros between)
    const Bytes grown = bl::apply_patches(image, table({{0x48, patch}}));
    CB_CHECK_EQ(grown.size(), std::size_t{0x4C});
    CB_CHECK(grown[0x40] == 0 && grown[0x47] == 0);
    CB_CHECK(Bytes(grown.begin() + 0x48, grown.end()) == patch);
    // in place: how many records changed anything
    Bytes work = image;
    const Bytes both = table({{0x10, patch}, {0x20, Bytes(image.begin() + 0x20, image.begin() + 0x24)}});   // the second is already there
    CB_CHECK_EQ(bl::patch_in_place(work, both), std::size_t{1});
    CB_CHECK_EQ(bl::patch_in_place(work, both), std::size_t{0});   // the first is there now
    // the size an image must grow by to hold the records at or past `size`
    CB_CHECK_EQ(bl::calc_patch_size(table({{0x10, patch}, {0x40, ramp(8)}, {0x80, ramp(12)}}), 0x40), std::size_t{20});
    CB_CHECK_EQ(bl::calc_patch_size(table({{0x10, patch}}), 0x40), std::size_t{0});
});

CB_TEST("formats.xbox.crypto.build_lib.padding_to_sixteen", [] {
    CB_CHECK_EQ(bl::calc_pad_size(0x20), std::size_t{0});
    CB_CHECK_EQ(bl::calc_pad_size(0x21), std::size_t{15});
    CB_CHECK_EQ(bl::calc_pad_size(0x2F), std::size_t{1});
    CB_CHECK_EQ(bl::calc_pad_size(5, 8), std::size_t{3});
    CB_CHECK_EQ(bl::calc_pad_size(0, 16), std::size_t{0});
    CB_CHECK_EQ(bl::calc_bldr_pad_size(0x1E5), std::size_t{11});
    CB_CHECK_EQ(bl::calc_bldr_pad_size(0x1E0), std::size_t{0});
});

CB_TEST("formats.xbox.crypto.build_lib.the_4bl_jump_branches_to_the_end_of_the_image", [] {
    Bytes image = ramp(0x80, 9);
    for (std::size_t at = 0; at + 4 <= image.size(); at += 4)   // make sure no word of the ramp is the anchor
        if (get32(image, at) == 0x4C000024) image[at] ^= 1;
    put32(image, 0x40, 0x4C000024);
    const Bytes jumped = bl::apply_jump_sd_4bl(image, 0x1000);
    // the distance from 0x40 to 0x1000: 0xFC0, as a `b`
    CB_CHECK_EQ(get32(jumped, 0x40), std::uint32_t{0x48000FC0});
    CB_CHECK(Bytes(jumped.begin(), jumped.begin() + 0x40) == Bytes(image.begin(), image.begin() + 0x40));
    CB_CHECK(Bytes(jumped.begin() + 0x44, jumped.end()) == Bytes(image.begin() + 0x44, image.end()));
    // the high bit of an address (uncached) does not count
    CB_CHECK_EQ(get32(bl::apply_jump_sd_4bl(image, 0x80001000), 0x40), std::uint32_t{0x48000FC0});
    // only the first anchor is rewritten
    put32(image, 0x60, 0x4C000024);
    CB_CHECK_EQ(get32(bl::apply_jump_sd_4bl(image, 0x1000), 0x60), std::uint32_t{0x4C000024});
    // none: nothing to branch through; an anchor off a word boundary is not one
    Bytes plain = ramp(0x20, 9);
    for (std::size_t at = 0; at + 4 <= plain.size(); at += 4)
        if (get32(plain, at) == 0x4C000024) plain[at] ^= 1;
    CB_CHECK_THROWS_AS(FormatError, (void)bl::apply_jump_sd_4bl(plain, 0x100));
    Bytes off(0x20, 0);
    put32(off, 0x11, 0x4C000024);
    CB_CHECK_THROWS_AS(FormatError, (void)bl::apply_jump_sd_4bl(off, 0x100));
});

CB_TEST("formats.xbox.crypto.build_lib.a_bootloaders_size_is_its_field_less_the_header_padded", [] {
    CB_CHECK_EQ(bl::bootloader_size(bootloader("SD", 0x200), 0), std::size_t{0x1E0});
    CB_CHECK_EQ(bl::bootloader_size(bootloader("SE", 0x205), 0), std::size_t{0x1F0});   // 0x1E5 padded to 0x1F0
    CB_CHECK_EQ(bl::bootloader_size(bootloader("SB", 0x20), 0), std::size_t{0});
    // at an offset in a bigger image
    Bytes image(0x100, 0);
    const Bytes inner = bootloader("SC", 0x60);
    std::copy(inner.begin(), inner.end(), image.begin() + 0x30);
    CB_CHECK_EQ(bl::bootloader_size(image, 0x30), std::size_t{0x40});
    // not a signed bootloader, or one that is not there
    CB_CHECK_THROWS_AS(FormatError, (void)bl::bootloader_size(bootloader("CD", 0x40), 0));
    CB_CHECK_THROWS_AS(FormatError, (void)bl::bootloader_size(bootloader("SD", 0x10), 0));   // smaller than its header
    CB_CHECK_THROWS_AS(FormatError, (void)bl::bootloader_size(image, 0xF8));                   // the header does not fit
});

CB_TEST("formats.xbox.crypto.build_lib.the_body_is_rc4_and_the_same_call_undoes_it", [] {
    const Bytes k = ramp(0x10, 77);
    Bytes image = bootloader("SD", 0x100);
    const Bytes original = image;
    bl::encrypt_bootloader_in_place(k, image, 0);
    CB_CHECK(Bytes(image.begin(), image.begin() + 0x20) == Bytes(original.begin(), original.begin() + 0x20));   // header and nonce stay
    CB_CHECK(image != original);
    CB_CHECK(Bytes(image.begin() + 0x20, image.end()) == xc::rc4(k, ByteView(original).subspan(0x20)));
    bl::encrypt_bootloader_in_place(k, image, 0);
    CB_CHECK(image == original);
    // encrypt_bl: the whole image, or all but the first 0x20 bytes
    const Bytes all = bl::encrypt_bl(k, original, false);
    CB_CHECK(all == xc::rc4(k, original));
    const Bytes skipped = bl::encrypt_bl(k, original);
    CB_CHECK(Bytes(skipped.begin(), skipped.begin() + 0x20) == Bytes(original.begin(), original.begin() + 0x20));
    CB_CHECK(Bytes(skipped.begin() + 0x20, skipped.end()) == xc::rc4(k, ByteView(original).subspan(0x20)));
    CB_CHECK(bl::encrypt_bl(k, Bytes(0x10, 1)).size() == 0x10);   // shorter than the header: nothing to encrypt
    CB_CHECK(bl::encrypt_bl(k, Bytes(0x10, 1)) == Bytes(0x10, 1));
});

CB_TEST("formats.xbox.crypto.build_lib.the_se_hash_covers_the_header_and_the_body_but_not_the_nonce", [] {
    Bytes image = bootloader("SE", 0x80);
    const Bytes hash = bl::calc_se_hash(image, 0);
    Bytes covered(image.begin(), image.begin() + 0x10);
    covered.insert(covered.end(), image.begin() + 0x20, image.begin() + 0x20 + 0x60);
    CB_CHECK(hash == xc::rot_sum_sha(covered));
    CB_CHECK_EQ(hash.size(), std::size_t{20});
    image[0x15] ^= 0xFF;   // the nonce: not covered
    CB_CHECK(bl::calc_se_hash(image, 0) == hash);
    image[0x25] ^= 0xFF;   // the body
    CB_CHECK(bl::calc_se_hash(image, 0) != hash);
});

CB_TEST("formats.xbox.crypto.build_lib.a_bootloader_is_signed_over_the_header_and_the_body_after_the_signature", [] {
    Bytes image = bootloader("SD", 0x240);
    const std::size_t size = bl::bootloader_size(image, 0);   // 0x220
    bl::sign_bootloader_in_place(image, 0, key());
    const Bytes signature(image.begin() + 0x20, image.begin() + 0x120);
    Bytes covered(image.begin(), image.begin() + 0x10);
    covered.insert(covered.end(), image.begin() + 0x120, image.begin() + 0x120 + (size - 0x100));
    CB_CHECK(key().public_key().verify(signature, xc::rot_sum_sha(covered), as_bytes(bl::kSdSalt)));
    // another body is not what was signed
    image[0x130] ^= 1;
    Bytes other(image.begin(), image.begin() + 0x10);
    other.insert(other.end(), image.begin() + 0x120, image.begin() + 0x120 + (size - 0x100));
    CB_CHECK(!key().public_key().verify(signature, xc::rot_sum_sha(other), as_bytes(bl::kSdSalt)));
    // a bootloader past the end of the image, or too small to hold a signature, is refused
    Bytes cut = bootloader("SD", 0x240);
    cut.resize(0x100);
    CB_CHECK_THROWS_AS(FormatError, bl::sign_bootloader_in_place(cut, 0, key()));
    Bytes tiny = bootloader("SD", 0x60);
    CB_CHECK_THROWS_AS(FormatError, bl::sign_bootloader_in_place(tiny, 0, key()));
});

CB_TEST("formats.xbox.crypto.build_lib.an_sd_is_signed_and_verified_by_its_hash", [] {
    const Bytes image = bootloader("SD", 0x200);
    const Bytes signed_image = bl::sign_sd_4bl(key(), as_bytes(bl::kSdSalt), image);
    CB_CHECK_EQ(signed_image.size(), image.size());
    CB_CHECK(Bytes(signed_image.begin(), signed_image.begin() + 0x20) == Bytes(image.begin(), image.begin() + 0x20));
    CB_CHECK(Bytes(signed_image.begin() + 0x120, signed_image.end()) == Bytes(image.begin() + 0x120, image.end()));
    CB_CHECK(Bytes(signed_image.begin() + 0x20, signed_image.begin() + 0x120) != Bytes(image.begin() + 0x20, image.begin() + 0x120));
    CB_CHECK(bl::verify_sd_4bl(key().public_key(), as_bytes(bl::kSdSalt), signed_image));
    CB_CHECK(bl::verify_sd_4bl(key(), as_bytes(bl::kSdSalt), signed_image));   // a private key verifies too
    // the nonce (0x10..0x20) is not covered, the header and the body are
    Bytes renonced = signed_image;
    renonced[0x18] ^= 1;
    CB_CHECK(bl::verify_sd_4bl(key().public_key(), as_bytes(bl::kSdSalt), renonced));
    Bytes changed = signed_image;
    changed[0x5] ^= 1;
    CB_CHECK(!bl::verify_sd_4bl(key().public_key(), as_bytes(bl::kSdSalt), changed));
    changed = signed_image;
    changed[0x150] ^= 1;
    CB_CHECK(!bl::verify_sd_4bl(key().public_key(), as_bytes(bl::kSdSalt), changed));
    // another salt, another signature
    CB_CHECK(!bl::verify_sd_4bl(key().public_key(), as_bytes(std::string_view("XBOX_ROM_3")), signed_image));
    // an image too short to have a body
    CB_CHECK_THROWS_AS(FormatError, (void)bl::sign_sd_4bl(key(), as_bytes(bl::kSdSalt), Bytes(0x100, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)bl::verify_sd_4bl(key(), as_bytes(bl::kSdSalt), Bytes(0x100, 0)));
});

// A codec that is no compression at all: every byte exclusive-or'd, so a round trip and the framing can be told apart.
class XorCodec : public bl::LzxCodec {
public:
    Bytes compress_continuous(ByteView data) override { return xor_all(data); }
    Bytes decompress_continuous(ByteView data) override { return xor_all(data); }
    static Bytes xor_all(ByteView data) {
        Bytes out(data.begin(), data.end());
        for (std::uint8_t& b : out) b ^= 0x5A;
        return out;
    }
};

CB_TEST("formats.xbox.crypto.build_lib.the_se_is_framed_by_a_0x30_byte_header", [] {
    XorCodec codec;
    const Bytes body = ramp(0x41, 5);
    const Bytes packed = bl::compress_se(codec, body);
    CB_CHECK_EQ(packed.size(), std::size_t{0x30 + 0x41});
    CB_CHECK_EQ(get32(packed, 0xC), std::uint32_t{0x30 + 0x41});   // its own size, header included
    CB_CHECK_EQ(get32(packed, 0x28), std::uint32_t{0x280000});
    CB_CHECK(packed[0] == 0 && packed[0x10] == 0 && packed[0x2F] == 0);
    CB_CHECK(Bytes(packed.begin() + 0x30, packed.end()) == XorCodec::xor_all(body));
    CB_CHECK(bl::decompress_se(codec, packed) == body);
    // no header
    CB_CHECK(bl::compress_se(codec, body, false) == XorCodec::xor_all(body));
    CB_CHECK(bl::decompress_se(codec, XorCodec::xor_all(body), false) == body);
    // data shorter than the header
    CB_CHECK_THROWS_AS(TruncatedError, (void)bl::decompress_se(codec, Bytes(0x2F, 1)));
});

CB_TEST("formats.xbox.crypto.build_lib.the_first_source_with_content_wins", [] {
    fs::ScopedTempDir dir("cb-build-lib");
    fs::write_file(dir.path() / "one.bin", Bytes{1, 2, 3});
    fs::write_file(dir.path() / "empty.bin", Bytes{});
    std::vector<bl::Source> sources = {stdfs::path(), stdfs::path(dir.path() / "missing.bin"), Bytes{}, stdfs::path(dir.path()),
                                       stdfs::path(dir.path() / "one.bin"), Bytes{9}};
    CB_CHECK(bl::first_source(sources) == std::optional<Bytes>(Bytes{1, 2, 3}));   // (an empty path, a missing file, no bytes, a folder: passed over)
    sources = {stdfs::path(dir.path() / "empty.bin"), Bytes{9}};
    CB_CHECK(bl::first_source(sources) == std::optional<Bytes>(Bytes{}));   // (an empty file is a file: it is read, and it is empty)
    CB_CHECK(!bl::first_source(std::vector<bl::Source>{Bytes{}, stdfs::path()}).has_value());
    CB_CHECK(!bl::first_source({}).has_value());
});

CB_TEST("formats.xbox.crypto.build_lib.a_patch_is_built_for_a_target_by_its_symbols", [] {
    CB_CHECK(bl::target_defines(bl::PatchTarget::Plain).empty());
    CB_CHECK((bl::target_defines(bl::PatchTarget::Devkit) == std::vector<std::string>{"DEVKIT"}));
    CB_CHECK((bl::target_defines(bl::PatchTarget::Retail) == std::vector<std::string>{"RETAIL"}));
    CB_CHECK((bl::target_defines(bl::PatchTarget::Flash) == std::vector<std::string>{"FLASH"}));
    CB_CHECK((bl::target_defines(bl::PatchTarget::VfusesFlash) == std::vector<std::string>{"VFUSES", "FLASH"}));
    CB_CHECK((bl::target_defines(bl::PatchTarget::Hdd) == std::vector<std::string>{"HDD"}));
    CB_CHECK((bl::target_defines(bl::PatchTarget::VfusesHdd) == std::vector<std::string>{"VFUSES", "HDD"}));
});

CB_TEST("formats.xbox.crypto.build_lib.assembling_without_the_tools_is_a_tool_error", [] {
    fs::ScopedTempDir dir("cb-build-lib");
    fs::write_text(dir.path() / "p.s", "nop\n");
    bl::PatchTools tools;
    tools.bin_dir = dir.path() / "no-free60";
    CB_CHECK_THROWS_AS(ToolError, bl::assemble_patch(tools, dir.path() / "p.s", dir.path() / "p.bin"));
    CB_CHECK(!stdfs::exists(dir.path() / "p.bin"));
});

CB_TEST("formats.xbox.crypto.build_lib.a_command_gives_its_exit_code_and_its_output", [] {
    const std::string where = fs::to_text(stdfs::path(fs::get_env("SystemRoot").value_or("C:/Windows")) / "System32" / "where.exe");
    const std::vector<std::string> found = {"cmd.exe"};
    const bl::CommandResult ok = bl::run_command(where, found);
    CB_CHECK_EQ(ok.exit_code, std::uint32_t{0});
    CB_CHECK(ok.output.find("cmd.exe") != std::string::npos);
    const std::vector<std::string> absent = {"no-such-program-cb-build-lib.exe"};
    CB_CHECK_EQ(bl::run_command(where, absent).exit_code, std::uint32_t{1});
    CB_CHECK_THROWS_AS(ToolError, (void)bl::run_command("no-such-tool.exe", absent));
});

}  // namespace build_lib_test
