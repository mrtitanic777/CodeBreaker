// Tests of cb/formats/xbox/crypto/shadowboot.hpp on a synthetic image: SB, SC and SD made by state/work/xbox-deferred/shadow_golden.py (in
// the work that wrote this) and kept in formats/testdata/shadowboot, signed with the xecrypt test key (formats/testdata/xecrypt/rsa2048.*,
// standing for both the 1BL's and the SB's); the hypervisor and kernel from a formula, an identity stand-in for the Xbox LZX library, and
// a counter stream for the random bytes. The Python's ShadowbootImage.create makes from the same parts, keys and stream the image whose
// SHA-1 is checked here, and its parse gives the facts compared below (keys, offsets, digests, the metadata, the patch table). (The
// Python's keyvault encryption draws 16 random bytes for a field it overwrites at once; the harness gives it zeros for those, so that the
// 8 bytes of obfuscation key that follow are the ones the C++ draws.)
//
// DIVERGES from the Python: the checks and the build take their keys from a `Keys` rather than module globals; a body or a bootloader that
// lies outside the image is FormatError (the Python read short and failed later); an unknown SMC console number reads "unknown" (the
// Python's table had 10 entries for the 16 numbers a nibble can be, an IndexError past them); the patch table is looked for only where the
// SD has its loader (the Python's `find` returned -1 for none and went on from offset 15); a patch record's size is its real size in
// bytes (the Python's field was four times it); `shadowboot build` with a keyvault is refused with the key's own error (the Python
// passed an all-zero CPU key, which XeCryptKeyVaultEncrypt rejects as invalid, so it never worked).
#include "cb/formats/xbox/crypto/shadowboot.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"

namespace shadowboot_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;
namespace sbt = cb::formats::xbox::crypto::shadowboot;

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

static std::filesystem::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/") + std::string(name)); }

class IdentityLzx final : public build_lib::LzxCodec {
public:
    Bytes compress_continuous(ByteView data) override { return Bytes(data.begin(), data.end()); }
    Bytes decompress_continuous(ByteView data) override { return Bytes(data.begin(), data.end()); }
};

// out[i] = i*37 + 11 over the whole stream, however it is asked for
class CounterRandom final : public xecrypt::RandomSource {
public:
    void fill(ByteSpan out) override {
        for (auto& b : out) b = static_cast<std::uint8_t>(next_++ * 37 + 11);
    }

private:
    std::uint32_t next_ = 0;
};

static Bytes pattern(std::size_t n, unsigned a, unsigned b) {
    Bytes out(n);
    for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<std::uint8_t>(i * a + (i >> 5) * b);
    return out;
}

static Bytes hypervisor_and_kernel() {
    Bytes hv(0x40000);
    for (std::size_t i = 0; i < hv.size(); ++i) hv[i] = static_cast<std::uint8_t>(i * 7 + (i >> 8) * 13);
    hv[0] = 0x5E;   // "^N": what the SE's check looks for
    hv[1] = 0x4E;
    hv[2] = 0x45;
    hv[3] = 0x2B;
    Bytes kernel = pattern(0x3000, 9, 1);
    const std::string_view mark = "\\testkit\\";
    std::copy(mark.begin(), mark.end(), kernel.begin() + 0x100);
    append(hv, kernel);
    return hv;
}

static sbt::Keys test_keys() {
    sbt::Keys keys;
    keys.one_bl_key = Bytes(16);
    for (std::size_t i = 0; i < 16; ++i) keys.one_bl_key[i] = static_cast<std::uint8_t>(0xA0 + i);
    keys.one_bl_pub = xecrypt::RsaKey::parse(fs::read_file(fixture("xecrypt/rsa2048.pub")));
    keys.sb_prv = xecrypt::RsaKey::parse(fs::read_file(fixture("xecrypt/rsa2048.prv")));
    return keys;
}

static sbt::ShadowbootImage::Parts the_parts() {
    sbt::ShadowbootImage::Parts parts;
    parts.sb = fs::read_file(fixture("shadowboot/sb.bin"));
    parts.sc = fs::read_file(fixture("shadowboot/sc.bin"));
    parts.sd = fs::read_file(fixture("shadowboot/sd.bin"));
    parts.se = hypervisor_and_kernel();
    parts.smc = fs::read_file(fixture("shadowboot/smc.bin"));
    parts.keyvault = sbt::ShadowbootImage::Parts::Keyvault{fs::read_file(fixture("shadowboot/cpu_key.bin")), pattern(0x4000, 3, 17)};
    parts.patches = fs::read_file(fixture("shadowboot/hvk_patches.bin"));
    parts.build = 17559;
    return parts;
}

static Bytes the_image(const sbt::Keys& keys) {
    IdentityLzx lzx;
    CounterRandom rng;
    return sbt::ShadowbootImage::create(the_parts(), keys, lzx, rng);
}

CB_TEST("formats.xbox.crypto.shadowboot.create_makes_the_pythons_image", [] {
    const sbt::Keys keys = test_keys();
    const Bytes parts_sb = the_parts().sb;
    CB_CHECK_EQ(sha1_hex(parts_sb), "572dc50e606bb45785448b0bdebb8ff853106c2c");   // the fixtures are the ones the Python used
    CB_CHECK_EQ(sha1_hex(the_parts().sc), "d15158ea7ea86226165484c0a968f906d9fa5b6a");
    CB_CHECK_EQ(sha1_hex(the_parts().sd), "6fc892ac4000d3f1870fdd6a6ee1b0ed52d76576");
    CB_CHECK_EQ(sha1_hex(the_parts().se), "b69869660017dbb6dfaea4e4856aa2f0dfc8d4e5");
    const Bytes image = the_image(keys);
    CB_CHECK_EQ(image.size(), 299008u);
    CB_CHECK_EQ(sha1_hex(image), "b4908074dfadd6085166e7aeea41bdbf608c63d8");
});

CB_TEST("formats.xbox.crypto.shadowboot.parse_reads_what_the_python_reads", [] {
    const sbt::Keys keys = test_keys();
    const Bytes image = the_image(keys);
    IdentityLzx lzx;
    const sbt::ShadowbootImage img = sbt::ShadowbootImage::parse(image, keys, lzx);

    const auto& f = img.flash_header;
    CB_CHECK_EQ(f.magic, 0xFF4F);
    CB_CHECK_EQ(f.build, 17559);
    CB_CHECK_EQ(f.qfe, 0x8000);
    CB_CHECK_EQ(f.entry, 20992u);
    CB_CHECK_EQ(f.patch_slots, 2);
    CB_CHECK_EQ(f.kv_version, 0x712);
    CB_CHECK_EQ(f.kv_offset, 0x4000u);
    CB_CHECK_EQ(f.kv_length, 0x4000u);
    CB_CHECK_EQ(f.smc_offset, 0x1000u);
    CB_CHECK_EQ(f.smc_length, 512u);
    CB_CHECK_EQ(f.sys_upd_addr, 299008u);
    CB_CHECK_EQ(f.patch_slot_size, 0x10000u);

    CB_CHECK_EQ(img.sb.offset, 20992u);
    CB_CHECK_EQ(img.sb.size, 1280u);
    CB_CHECK_EQ(img.sb.padded_size, 1280u);
    CB_CHECK_EQ(crypto::to_hex(img.sb.key), "6e1e0adb6388029484fb4a465464e228");
    CB_CHECK_EQ(img.sc.offset, 22272u);
    CB_CHECK_EQ(img.sc.size, 1024u);
    CB_CHECK_EQ(crypto::to_hex(img.sc.key), "aae5c7dcfa5a9eb13fc411ebf59eae07");
    CB_CHECK_EQ(img.sd.offset, 23296u);
    CB_CHECK_EQ(img.sd.size, 880u);
    CB_CHECK_EQ(crypto::to_hex(img.sd.key), "78b9b1b7ac9f47271132bc173c29a0e3");
    CB_CHECK_EQ(img.se.offset, 24176u);
    CB_CHECK_EQ(img.se.size, 274480u);
    CB_CHECK_EQ(crypto::to_hex(img.se.key), "a71cfc66ed37f809a447ba22c61269cf");
    CB_CHECK_EQ(sha1_hex(img.sb_data), "572dc50e606bb45785448b0bdebb8ff853106c2c");   // as given: the nonce is zero in what is returned
    CB_CHECK_EQ(sha1_hex(img.sc_data), "d15158ea7ea86226165484c0a968f906d9fa5b6a");
    CB_CHECK_EQ(sha1_hex(img.sd_data), "2e95f2c8ff52abd33d214fa2c413b413f72b0e53");   // (the SD now carries the SE's digest and its signature)
    CB_CHECK_EQ(sha1_hex(img.se_data), "cad5efcfb08fbee595a7618c0f15b6d2def46ee7");

    CB_CHECK(img.smc.has_value());
    CB_CHECK_EQ(sha1_hex(*img.smc), "b124d72f5b896f4ec707fc6a7bb2870b22bbf475");
    CB_CHECK(img.keyvault.has_value());
    CB_CHECK_EQ(sha1_hex(*img.keyvault), "39bd2cb9a958ca4579a146144be3b61364e1a89f");
    CB_CHECK_EQ(sha1_hex(img.hypervisor), "50ec00ab9576eaca07bd89edec3a6619b14ca2ef");
    CB_CHECK_EQ(sha1_hex(img.kernel), "8fd1f7567a1324b33456428506595c0d2764615b");

    CB_CHECK_EQ(img.console_type, "Falcon");
    CB_CHECK_EQ(img.smc_version, "3.1 (4.2)");
    CB_CHECK_EQ(img.sb_build, 1888);
    CB_CHECK_EQ(img.sc_build, 1889);
    CB_CHECK_EQ(img.sd_build, 1890);
    CB_CHECK_EQ(img.se_build, 17559);
    CB_CHECK_EQ(img.hypervisor_version, 17707);
    CB_CHECK_EQ(img.kernel_version, 17559);
    CB_CHECK(!img.is_retail);
    CB_CHECK(img.is_testkit);

    CB_CHECK_EQ(sha1_hex(img.sb_signature), "d2d99c27ac9cbab22fecaea2f180b9f36a215493");
    CB_CHECK_EQ(sha1_hex(img.sc_signature), "8a6b66e6e34198b6c34d2dc201981cb32ed72108");
    CB_CHECK_EQ(sha1_hex(img.sd_signature), "823685eeb4f6199ddb1de26c46e67de73748a41f");
    CB_CHECK_EQ(crypto::to_hex(img.sc_nonce), "8a8d909396999c9fa7aaadb0b3b6b9bc");
    CB_CHECK_EQ(as_text(img.sc_salt), "XBOX_ROM_3");
    CB_CHECK_EQ(as_text(img.sd_salt), "XBOX_ROM_4");
    CB_CHECK_EQ(crypto::to_hex(img.sd_digest), "0000000000000000000000000000000000000000");
    CB_CHECK_EQ(crypto::to_hex(img.sf_nonce), "00070e151c232a31383f464d545b6269");
    CB_CHECK_EQ(as_text(img.sf_salt), "XBOX_ROM_3");
    CB_CHECK_EQ(crypto::to_hex(img.se_digest), "dd8fe9961df50d6693bd6301617eeb171389a248");

    // the patch table of the SD: its loader and two records
    CB_CHECK(img.patch_loader.has_value());
    CB_CHECK_EQ(img.patch_loader_offset, 784u);
    CB_CHECK_EQ(sha1_hex(*img.patch_loader), "0063a17ce646d36c3bebb6cf798f455e1f142bb1");
    CB_CHECK_EQ(img.patches.size(), 2u);
    CB_CHECK_EQ(img.patches[0].offset, 848u);
    CB_CHECK_EQ(img.patches[0].address, 4096u);
    CB_CHECK_EQ(img.patches[0].size_words, 2u);
    CB_CHECK_EQ(crypto::to_hex(img.patches[0].code), "386000014e800020");
    CB_CHECK_EQ(crypto::to_hex(img.patches[0].whole()), "0000100000000002386000014e800020");
    CB_CHECK_EQ(img.patches[1].offset, 864u);
    CB_CHECK_EQ(img.patches[1].address, 266240u);
    CB_CHECK_EQ(crypto::to_hex(img.patches[1].whole()), "000410000000000160000000");
    // the hypervisor patch table was applied before the SE was compressed
    CB_CHECK_EQ(crypto::to_hex(ByteView(img.hypervisor).subspan(0x2000, 4)), "deadbeef");

    CB_CHECK_EQ(img.info(),
                "Console Type:   Falcon\nSMC Version:    3.1 (4.2)\nSB Version:     1888\nSC Version:     1889\nSD Version:     1890\n"
                "SE Version:     17559\nHV Version:     17707\nKernel Version: 17559\nMain Menu:      XShell\nHardware:       Test Kit\n");
});

CB_TEST("formats.xbox.crypto.shadowboot.a_tampered_image_fails_its_check_and_reads_without_it", [] {
    const sbt::Keys keys = test_keys();
    Bytes image = the_image(keys);
    IdentityLzx lzx;
    const sbt::ShadowbootImage good = sbt::ShadowbootImage::parse(image, keys, lzx);
    image[good.sc.offset + 0x200] ^= 1;
    try {
        (void)sbt::ShadowbootImage::parse(image, keys, lzx);
        CB_CHECK(false);
    } catch (const FormatError& e) {
        CB_CHECK(std::string_view(e.what()).find("Invalid SC signature") != std::string_view::npos);
    }
    const sbt::ShadowbootImage unchecked = sbt::ShadowbootImage::parse(image, keys, lzx, {.checks = false});
    CB_CHECK_EQ(unchecked.sc_build, 1889);
    CB_CHECK(!unchecked.check_signature_sc());
    CB_CHECK(unchecked.check_signature_sd());
    CB_CHECK(unchecked.check_signature_sb(*keys.one_bl_pub));
    // the SE digest check: damage the SE's body and the image still reads, but its digest does not hold
    Bytes second = the_image(keys);
    second[good.se.offset + 0x1000] ^= 1;
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::parse(second, keys, lzx));
    const sbt::ShadowbootImage no_checks = sbt::ShadowbootImage::parse(second, keys, lzx, {.checks = false});
    CB_CHECK(!no_checks.check_digest_se());
});

CB_TEST("formats.xbox.crypto.shadowboot.errors_name_what_is_wrong", [] {
    const sbt::Keys keys = test_keys();
    IdentityLzx lzx;
    CounterRandom rng;
    // building needs the private key, checking needs the public one
    sbt::Keys no_private = keys;
    no_private.sb_prv.reset();
    CB_CHECK_THROWS_AS(UsageError, (void)sbt::ShadowbootImage::create(the_parts(), no_private, lzx, rng));
    const Bytes image = the_image(keys);
    sbt::Keys no_public = keys;
    no_public.one_bl_pub.reset();
    CB_CHECK_THROWS_AS(UsageError, (void)sbt::ShadowbootImage::parse(image, no_public, lzx));
    (void)sbt::ShadowbootImage::parse(image, no_public, lzx, {.checks = false});

    // parts of the wrong kind
    auto parts = the_parts();
    std::swap(parts.sb, parts.sc);
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::create(parts, keys, lzx, rng));
    parts = the_parts();
    parts.se = Bytes(0x41000, 1);   // a hypervisor that does not start with "^N"
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::create(parts, keys, lzx, rng));
    parts = the_parts();
    parts.test_kit = true;          // the synthetic SB has no branch to rewrite at 0x1348
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::create(parts, keys, lzx, rng));

    // images that are cut short or are not images
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::parse(ByteView(image).subspan(0, 0x40), keys, lzx));
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::parse(ByteView(image).subspan(0, 30000), keys, lzx));
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::parse(Bytes(0x2000, 0x41), keys, lzx));
    Bytes wrong_magic = image;
    wrong_magic[20992] = 'C';   // SB's magic becomes "CB": not a shadowboot chain
    CB_CHECK_THROWS_AS(FormatError, (void)sbt::ShadowbootImage::parse(wrong_magic, keys, lzx, {.checks = false}));
});

CB_TEST("formats.xbox.crypto.shadowboot.helpers_unecc_and_finding_images", [] {
    Bytes dump;
    for (int block = 0; block < 3; ++block) {
        for (int i = 0; i < 512; ++i) dump.push_back(static_cast<std::uint8_t>(block));
        for (int i = 0; i < 16; ++i) dump.push_back(0xEE);   // the spare bytes
    }
    const Bytes plain = sbt::unecc(dump);
    CB_CHECK_EQ(plain.size(), 3u * 512);
    CB_CHECK_EQ(plain[0], 0);
    CB_CHECK_EQ(plain[512], 1);
    CB_CHECK_EQ(plain[1535], 2);
    CB_CHECK(std::ranges::none_of(plain, [](std::uint8_t b) { return b == 0xEE; }));
    CB_CHECK(sbt::unecc({}).empty());

    // the notice is 28 bytes into an image's flash header
    const std::string notice = "Microsoft Corporation. All rights reserved";
    Bytes file(100, 0);
    for (const std::size_t at : {std::size_t{200}, std::size_t{900}}) {
        file.resize(at + 28 + notice.size() + 10, 0);
        std::copy(notice.begin(), notice.end(), file.begin() + static_cast<std::ptrdiff_t>(at + 28));
    }
    const std::vector<std::size_t> found = sbt::find_images(file);
    CB_CHECK_EQ(found.size(), 2u);
    CB_CHECK_EQ(found[0], 200u);
    CB_CHECK_EQ(found[1], 900u);
});

// A folder with a key store of the test keys and the synthetic image in it.
struct Workbench {
    fs::ScopedTempDir dir{"cb-shadowboot"};
    keystore::KeyStore store{"Keys"};
    std::filesystem::path image_path;
    Workbench() {
        const auto keys_dir = dir.path() / "Keys";
        std::filesystem::create_directories(keys_dir);
        const Bytes prv = fs::read_file(fixture("xecrypt/rsa2048.prv")), pub = fs::read_file(fixture("xecrypt/rsa2048.pub"));
        fs::write_file(keys_dir / "1BL_key.bin", test_keys().one_bl_key);
        fs::write_file(keys_dir / "1BL_pub.bin", pub);
        fs::write_file(keys_dir / "SB_prv.bin", prv);
        store = keystore::KeyStore(keys_dir);
        store.accept_checksum(keystore::KeyFile::Bl1Pub, crypto::crc32(pub));
        store.accept_checksum(keystore::KeyFile::SbPrv, crypto::crc32(prv));
        image_path = dir.path() / "xboxrom.bin";
        fs::write_file(image_path, the_image(test_keys()));
    }
};

CB_TEST("formats.xbox.crypto.shadowboot.the_command_reads_info", [] {
    Workbench bench;
    const cli::Cli program("codebreaker", {sbt::command(bench.store, std::make_shared<IdentityLzx>())});
    testing::CapturedStd out;
    CB_CHECK_EQ(program.run({"shadowboot", "info", fs::to_text(bench.image_path)}), 0);
    CB_CHECK(out.out().starts_with("Console Type:   Falcon\n"));
    CB_CHECK(out.out().find("Hardware:       Test Kit\n") != std::string::npos);
});

CB_TEST("formats.xbox.crypto.shadowboot.the_command_extracts_the_parts", [] {
    fs::ScopedTempDir dir("cb-shadowboot");
    // a key store of the test keys
    const auto keys_dir = dir.path() / "Keys";
    std::filesystem::create_directories(keys_dir);
    const Bytes prv = fs::read_file(fixture("xecrypt/rsa2048.prv")), pub = fs::read_file(fixture("xecrypt/rsa2048.pub"));
    fs::write_file(keys_dir / "1BL_key.bin", test_keys().one_bl_key);
    fs::write_file(keys_dir / "1BL_pub.bin", pub);
    fs::write_file(keys_dir / "SB_prv.bin", prv);
    keystore::KeyStore store(keys_dir);
    store.accept_checksum(keystore::KeyFile::Bl1Pub, crypto::crc32(pub));
    store.accept_checksum(keystore::KeyFile::SbPrv, crypto::crc32(prv));

    const auto image_path = dir.path() / "xboxrom.bin";
    fs::write_file(image_path, the_image(test_keys()));
    const cli::Cli program("codebreaker", {sbt::command(store, std::make_shared<IdentityLzx>())});

    {
        testing::CapturedStd out;
        CB_CHECK_EQ(program.run({"shadowboot", "info", fs::to_text(image_path)}), 0);
        CB_CHECK(out.out().starts_with("Console Type:   Falcon\n"));
        CB_CHECK(out.out().find("Hardware:       Test Kit\n") != std::string::npos);
    }
    const auto folder = dir.path() / "parts";
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(program.run({"shadowboot", "extract", fs::to_text(image_path), fs::to_text(folder), "--all"}), 0);
    }
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "hypervisor.bin")), "50ec00ab9576eaca07bd89edec3a6619b14ca2ef");
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "kernel.exe")), "8fd1f7567a1324b33456428506595c0d2764615b");
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "SMC_dec.bin")), "b124d72f5b896f4ec707fc6a7bb2870b22bbf475");
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "KV_enc.bin")), "39bd2cb9a958ca4579a146144be3b61364e1a89f");
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "sb_1888.bin")), "572dc50e606bb45785448b0bdebb8ff853106c2c");
    CB_CHECK_EQ(sha1_hex(fs::read_file(folder / "sd_1890.bin")), "2e95f2c8ff52abd33d214fa2c413b413f72b0e53");
    CB_CHECK_EQ(fs::read_file(folder / "patch_loader.bin").size(), 0x40u);
    // the patches, named by the address the console maps them at (kernel ones above 0x80000000)
    CB_CHECK_EQ(crypto::to_hex(fs::read_file(folder / "Patches" / "00001000.bin")), "386000014e800020");
    CB_CHECK_EQ(crypto::to_hex(fs::read_file(folder / "Patches" / "80041000.bin")), "60000000");
    CB_CHECK_EQ(crypto::to_hex(fs::read_file(folder / "patches_raw.bin")), "0000100000000002386000014e8000200004100000000001600000" "00ffffffff");

});

CB_TEST("formats.xbox.crypto.shadowboot.the_command_builds_from_a_build_folder", [] {
    Workbench bench;
    const auto& dir = bench.dir;
    const auto& image_path = bench.image_path;
    const cli::Cli program("codebreaker", {sbt::command(bench.store, std::make_shared<IdentityLzx>())});
    // a build from a build folder with a base image
    const auto build_dir = dir.path() / "build";
    std::filesystem::create_directories(build_dir);
    fs::write_file(build_dir / "xboxrom.bin", fs::read_file(image_path));
    fs::write_file(build_dir / "sb.bin", the_parts().sb);
    fs::write_file(build_dir / "sc.bin", the_parts().sc);
    fs::write_file(build_dir / "sd.bin", the_parts().sd);
    const Bytes hv_and_kernel = the_parts().se;
    fs::write_file(build_dir / "hypervisor.bin", Bytes(hv_and_kernel.begin(), hv_and_kernel.begin() + 0x40000));
    fs::write_file(build_dir / "kernel.bin", Bytes(hv_and_kernel.begin() + 0x40000, hv_and_kernel.end()));
    const auto built = dir.path() / "built.bin";
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(program.run({"shadowboot", "build", fs::to_text(built), "--build-dir", fs::to_text(build_dir)}), 0);
        CB_CHECK(out.out().find("Reading raw HV/kernel...\n") != std::string::npos);
        CB_CHECK(out.out().find("Final image verified!\n") != std::string::npos);
    }
    // the new image (new nonces, so other bytes) holds the same hypervisor, kernel and SMC, and checks out
    IdentityLzx lzx;
    const sbt::ShadowbootImage rebuilt = sbt::ShadowbootImage::parse(fs::read_file(built), test_keys(), lzx);
    const Bytes original = the_parts().se;
    CB_CHECK(rebuilt.hypervisor == Bytes(original.begin(), original.begin() + 0x40000));
    CB_CHECK(rebuilt.kernel == Bytes(original.begin() + 0x40000, original.end()));
    CB_CHECK(rebuilt.smc == the_parts().smc);   // (taken from the base image: no SMC file in the folder)
    CB_CHECK(!rebuilt.keyvault.has_value() || rebuilt.flash_header.kv_length == 0);
    CB_CHECK_EQ(rebuilt.sc_build, 1889);

    // no manifest and no folder: the command says what it needs
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(program.run({"shadowboot", "build", fs::to_text(built)}), 1);
        CB_CHECK_EQ(out.out(), "Building requires -m or -b arguments!\n");
    }
});

}  // namespace shadowboot_test
