#include "cb/formats/xbox/crypto/shadowboot.hpp"

#include <algorithm>
#include <format>
#include <memory>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::crypto::shadowboot {

namespace {

namespace stdfs = std::filesystem;
using build_lib::BootloaderHeader;
using build_lib::FlashHeader;

constexpr std::string_view kReader = "shadowboot";
constexpr std::size_t kBootloaderHeader = 0x20;   // the header and its nonce
constexpr std::size_t kSignatureSize = 0x100;
constexpr std::size_t kRsaPublicSize = 0x110;
constexpr std::size_t kSbSignatureAt = 64, kSbPublicKeyAt = 616, kSbScNonceAt = 888, kSbScSaltAt = 904, kSbSdSaltAt = 914, kSbSdDigestAt = 924;
constexpr std::size_t kScSignatureAt = 32;
constexpr std::size_t kSdSignatureAt = 32, kSdPublicKeyAt = 288, kSdSfNonceAt = 560, kSdSfSaltAt = 576, kSdSeDigestAt = 588;
constexpr std::size_t kSbHashedFrom = 0x140, kScSdHashedFrom = 0x120;   // after the signature block
constexpr std::size_t kSdSeDigestWriteAt = 0x24C;                       // where the SE's digest goes in the SD
constexpr std::size_t kSbTestKitBranchAt = 0x1348;                      // from SB's header
constexpr std::size_t kFlashHeaderEnd = 0x80;
constexpr std::size_t kSmcAt = 0x1000, kKeyvaultAt = 0x4000;
constexpr std::string_view kCopyright = "\xA9 2005-2023 Microsoft Corporation. All rights reserved";
constexpr std::string_view kImageNotice = "Microsoft Corporation. All rights reserved";
constexpr std::size_t kImageNoticeAt = 28;                              // from the start of an image
constexpr std::array<std::uint8_t, 16> kPatchLoaderEnd = {0x4B, 0xFF, 0xFF, 0xB0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
constexpr std::size_t kPatchLoaderSize = 0x40;
constexpr std::uint32_t kPatchEnd = 0xFFFFFFFF;
constexpr std::array<std::uint8_t, 4> kSbBranchBefore = {0x41, 0x9A, 0x00, 0x14}, kSbBranchAfter = {0x48, 0x00, 0x01, 0x94};
constexpr std::array<std::uint8_t, 9> kTestKitMark = {0x5C, 't', 'e', 's', 't', 'k', 'i', 't', 0x5C};
const char* const kConsoleTypes[] = {"Error", "Xenon", "Zephyr", "Falcon", "Jasper", "Trinity", "Corona", "Winchester", "unknown", "unknown"};

[[noreturn]] void damaged(std::string_view what) { throw FormatError(kReader, 0, std::string(what)); }

Bytes cat(ByteView a, ByteView b, ByteView c = {}) {
    Bytes out(a.begin(), a.end());
    append(out, b);
    append(out, c);
    return out;
}

Bytes first16(const Bytes& digest) { return Bytes(digest.begin(), digest.begin() + 0x10); }

}  // namespace

Keys Keys::load(const keystore::KeyStore& store) {
    Keys k;
    std::optional<Bytes> one_bl = xecrypt::load_store_key(store.folder(), "1BL_key.bin");
    if (!one_bl) throw NotFoundError(store.folder() / "1BL_key.bin", 0, "the 1BL key is not in the key store");
    k.one_bl_key = std::move(*one_bl);
    k.one_bl_pub = store.load_and_verify(keystore::KeyFile::Bl1Pub);
    k.sb_prv = store.load_and_verify(keystore::KeyFile::SbPrv);
    return k;
}

Bytes PatchRecord::whole() const {
    io::ByteWriter w;
    w.u32_be(address);
    w.u32_be(size_words);
    w.bytes(code);
    return w.take();
}

// ---- parsing ---------------------------------------------------------------------------------------------------------------------

namespace {

// The four bootloader headers in the order the image lays them out, each at its place; the keys are derived once all are read.
void map_bootloaders(ShadowbootImage& img, ByteView data, const Keys& keys) {
    img.flash_header = FlashHeader::parse(data);
    if (img.flash_header.smc_offset > 0 && img.flash_header.smc_length > 0) {
        img.smc = xecrypt::smc_decrypt(io::checked_range(data, img.flash_header.smc_offset, img.flash_header.smc_length));
    }
    if (img.flash_header.smc_config_offset > 0) {
        const ByteView config = io::checked_range(data, img.flash_header.smc_config_offset, kSmcConfigSize);
        img.smc_config = Bytes(config.begin(), config.end());
    }
    if (img.flash_header.kv_offset > 0) {
        const ByteView kv = io::checked_range(data, img.flash_header.kv_offset, img.flash_header.kv_length);
        img.keyvault = Bytes(kv.begin(), kv.end());
    }

    io::ByteReader r(data, kReader);
    r.seek(img.flash_header.entry);
    Bootloader* by_name[4] = {&img.sb, &img.sc, &img.sd, &img.se};
    const char* names[4] = {"SB", "SC", "SD", "SE"};
    bool seen[4] = {};
    Bytes nonces[4];
    for (int i = 0; i < 4; ++i) {
        const std::size_t offset = r.pos();
        BootloaderHeader h = BootloaderHeader::parse(r.bytes(kBootloaderHeader));
        const std::string name(reinterpret_cast<const char*>(h.magic.data()), h.magic.size());
        const auto it = std::find(std::begin(names), std::end(names), name);
        if (it == std::end(names)) damaged(std::format("the bootloader at {:#x} is '{}', not one of SB, SC, SD or SE", offset, name));
        const int slot = static_cast<int>(it - std::begin(names));
        if (seen[slot]) damaged(std::format("two {} bootloaders", name));
        seen[slot] = true;
        Bootloader& b = *by_name[slot];
        b.name = name;
        b.offset = static_cast<std::uint32_t>(offset);
        b.size = h.size;
        b.padded_size = h.padded_size();
        nonces[slot] = Bytes(h.nonce->begin(), h.nonce->end());
        h.nonce = std::array<std::uint8_t, 0x10>{};   // the stored header's nonce is zeroed once the key is derived from it
        b.header = h;
        if (i != 3) {
            if (h.size < kBootloaderHeader) damaged(std::format("{} is smaller than its own header", name));
            r.skip(h.size - kBootloaderHeader);
        }
    }
    // the keys chain: SB from the 1BL key, SC from zeros, SD from SC's, SE from SD's
    const Bytes zero_key(0x10, 0);
    img.sb.key = first16(xecrypt::hmac_sha(keys.one_bl_key, nonces[0]));
    img.sc.key = first16(xecrypt::hmac_sha(zero_key, nonces[1]));
    img.sd.key = first16(xecrypt::hmac_sha(img.sc.key, nonces[2]));
    img.se.key = first16(xecrypt::hmac_sha(img.sd.key, nonces[3]));
}

// The bootloader's header (nonce zeroed) and its body decrypted.
Bytes decrypt_body(ByteView data, const Bootloader& b, std::uint32_t size) {
    if (size < kBootloaderHeader) damaged(std::format("{} is smaller than its own header", b.name));
    const ByteView body = io::checked_range(data, std::uint64_t{b.offset} + kBootloaderHeader, size - kBootloaderHeader);
    return cat(b.header.to_bytes(), xecrypt::rc4(b.key, body));
}

ByteView part(const Bytes& data, std::size_t at, std::size_t size) { return io::checked_range(data, at, size); }

void read_metadata(ShadowbootImage& img) {
    const auto copy = [](ByteView v) { return Bytes(v.begin(), v.end()); };
    img.sb_signature = copy(part(img.sb_data, kSbSignatureAt, kSignatureSize));
    img.sb_pub_key = xecrypt::RsaKey::parse(part(img.sb_data, kSbPublicKeyAt, kRsaPublicSize));   // verifies SC and SD
    img.sc_nonce = copy(part(img.sb_data, kSbScNonceAt, 0x10));
    img.sc_salt = copy(part(img.sb_data, kSbScSaltAt, 0xA));
    img.sd_salt = copy(part(img.sb_data, kSbSdSaltAt, 0xA));
    img.sd_digest = copy(part(img.sb_data, kSbSdDigestAt, 0x14));
    img.sc_signature = copy(part(img.sc_data, kScSignatureAt, kSignatureSize));
    img.sd_signature = copy(part(img.sd_data, kSdSignatureAt, kSignatureSize));
    img.sd_pub_key = xecrypt::RsaKey::parse(part(img.sd_data, kSdPublicKeyAt, kRsaPublicSize));   // verifies SE
    img.sf_nonce = copy(part(img.sd_data, kSdSfNonceAt, 0x10));
    img.sf_salt = copy(part(img.sd_data, kSdSfSaltAt, 0xA));
    img.se_digest = copy(part(img.sd_data, kSdSeDigestAt, 0x14));
    if (img.smc && !img.smc->empty()) {
        const Bytes& smc = *img.smc;
        if (smc.size() < 259) damaged("the SMC is too short to name its console");
        const unsigned num = smc[256];
        img.console_type = kConsoleTypes[std::min<std::size_t>((num >> 4) & 15, std::size(kConsoleTypes) - 1)];
        img.smc_version = std::format("{}.{} ({}.{})", (num >> 4) & 15, num & 15, smc[257], smc[258]);
    }
    img.sb_build = img.sb.header.build;
    img.sc_build = img.sc.header.build;
    img.sd_build = img.sd.header.build;
    img.se_build = img.se.header.build;
    img.hypervisor_version = static_cast<std::uint16_t>((img.hypervisor.at(2) << 8) | img.hypervisor.at(3));
    img.kernel_version = img.se_build;
    img.is_retail = img.hypervisor[0] == 0x4E;
    img.is_testkit = std::search(img.kernel.begin(), img.kernel.end(), kTestKitMark.begin(), kTestKitMark.end()) != img.kernel.end();
}

// The patch records after the loader the SD ends with, if it has them.
void read_patches(ShadowbootImage& img) {
    const auto found = std::search(img.sd_data.begin(), img.sd_data.end(), kPatchLoaderEnd.begin(), kPatchLoaderEnd.end());
    if (found == img.sd_data.end()) return;   // no patch loader
    const std::size_t end = static_cast<std::size_t>(found - img.sd_data.begin()) + kPatchLoaderEnd.size();
    if (end == img.sd_data.size()) return;    // nothing after it
    io::ByteReader r(img.sd_data, "shadowboot SD");
    r.seek(end);
    img.patch_loader_offset = static_cast<std::uint32_t>(end);
    img.patch_loader = r.bytes_copy(kPatchLoaderSize);   // the loader code for the patches
    for (;;) {
        const auto offset = static_cast<std::uint32_t>(r.pos());
        const std::uint32_t address = r.u32_be();
        if (address == kPatchEnd) break;
        PatchRecord p;
        p.offset = offset;
        p.address = address;
        p.size_words = r.u32_be();
        p.code = r.bytes_copy(std::uint64_t{p.size_words} * 4);
        img.patches.push_back(std::move(p));
    }
}

}  // namespace

ShadowbootImage ShadowbootImage::parse(ByteView data, const Keys& keys, build_lib::LzxCodec& codec, ParseOptions options) {
    ShadowbootImage img;
    map_bootloaders(img, data, keys);
    img.sb_data = decrypt_body(data, img.sb, img.sb.padded_size);
    img.sc_data = decrypt_body(data, img.sc, img.sc.padded_size);
    img.sd_data = decrypt_body(data, img.sd, img.sd.padded_size);
    img.se_data = decrypt_body(data, img.se, img.se.size);   // (the SE is read to its size, not its padded one)
    const Bytes stream = build_lib::decompress_se(codec, img.se_data);
    if (stream.size() < kHypervisorBytes) damaged(std::format("the SE decompresses to {:#x} bytes, less than the hypervisor", stream.size()));
    img.hypervisor.assign(stream.begin(), stream.begin() + kHypervisorBytes);
    img.kernel.assign(stream.begin() + kHypervisorBytes, stream.end());
    read_metadata(img);
    if (options.patches) read_patches(img);
    if (options.checks) {
        if (!keys.one_bl_pub) throw UsageError("checking a shadowboot needs the 1BL public key");
        if (!img.check_signature_sb(*keys.one_bl_pub)) damaged("Invalid SB signature");
        if (!img.check_signature_sc()) damaged("Invalid SC signature");
        if (!img.check_signature_sd()) damaged("Invalid SD signature");
        if (!img.check_digest_sd()) damaged("Invalid SD digest");
        if (!img.check_digest_se()) damaged("Invalid SE digest");
    }
    return img;
}

// ---- the checks ------------------------------------------------------------------------------------------------------------------

namespace {

// What a bootloader's hash covers: its first 0x10 bytes, then everything after the signature block.
Bytes hashed_part(const Bytes& data, std::size_t from) {
    if (data.size() < from) damaged("a bootloader is shorter than its signature block");
    return cat(ByteView(data).subspan(0, 0x10), ByteView(data).subspan(from));
}

bool is_zero(const Bytes& b) { return std::ranges::all_of(b, [](std::uint8_t x) { return x == 0; }); }

}  // namespace

bool ShadowbootImage::check_signature_sb(const xecrypt::RsaKey& one_bl_pub) const {
    return one_bl_pub.verify(sb_signature, xecrypt::rot_sum_sha(hashed_part(sb_data, kSbHashedFrom)), as_bytes(xecrypt::kSalt1bl));
}

bool ShadowbootImage::check_signature_sc() const {
    return sb_pub_key->verify(sc_signature, xecrypt::rot_sum_sha(hashed_part(sc_data, kScSdHashedFrom)), sc_salt);
}

bool ShadowbootImage::check_signature_sd() const {
    return sb_pub_key->verify(sd_signature, xecrypt::rot_sum_sha(hashed_part(sd_data, kScSdHashedFrom)), sd_salt);
}

bool ShadowbootImage::check_digest_sd() const {
    if (is_zero(sd_digest)) return true;   // no digest to check
    return xecrypt::rot_sum_sha(cat(ByteView(sd_data).subspan(0, 0x10), ByteView(sd_data).subspan(0x20))) == sd_digest;
}

bool ShadowbootImage::check_digest_se() const {
    if (is_zero(se_digest)) return true;
    const Bytes padding(build_lib::calc_bldr_pad_size(se_data.size()), 0);
    return xecrypt::rot_sum_sha(cat(ByteView(se_data).subspan(0, 0x10), ByteView(se_data).subspan(0x20), padding)) == se_digest;
}

std::string ShadowbootImage::info() const {
    std::string out;
    out += std::format("Console Type:   {}\n", console_type);
    out += std::format("SMC Version:    {}\n", smc_version);
    out += std::format("SB Version:     {}\n", sb_build);
    out += std::format("SC Version:     {}\n", sc_build);
    out += std::format("SD Version:     {}\n", sd_build);
    out += std::format("SE Version:     {}\n", se_build);
    out += std::format("HV Version:     {}\n", hypervisor_version);
    out += std::format("Kernel Version: {}\n", kernel_version);
    out += is_retail ? "Main Menu:      Dashboard\n" : "Main Menu:      XShell\n";
    out += is_testkit ? "Hardware:       Test Kit\n" : "Hardware:       Development Kit\n";
    return out;
}

// ---- building --------------------------------------------------------------------------------------------------------------------

Bytes ShadowbootImage::create(const Parts& parts, const Keys& keys, build_lib::LzxCodec& codec, xecrypt::RandomSource& rng) {
    if (!keys.sb_prv) throw UsageError("building a shadowboot needs the private SB key");
    const Bytes zero_key(0x10, 0);
    const Bytes sb_nonce = rng.bytes(0x10), sc_nonce = rng.bytes(0x10), sd_nonce = rng.bytes(0x10), se_nonce = rng.bytes(0x10);
    const Bytes sb_key = first16(xecrypt::hmac_sha(keys.one_bl_key, sb_nonce));
    const Bytes sc_key = first16(xecrypt::hmac_sha(zero_key, sc_nonce));
    const Bytes sd_key = first16(xecrypt::hmac_sha(sc_key, sd_nonce));
    const Bytes se_key = first16(xecrypt::hmac_sha(sd_key, se_nonce));

    // The three signed bootloaders: header (checked by its magic), then the body.
    const auto split_bootloader = [](const Bytes& whole, std::string_view magic, std::array<std::uint8_t, 0x10> nonce, BootloaderHeader& header, Bytes& body) {
        header = BootloaderHeader::parse(ByteView(whole).subspan(0, std::min<std::size_t>(whole.size(), kBootloaderHeader)));
        if (as_text(header.magic) != magic) damaged(std::format("Invalid {} bootloader!", magic));
        header.nonce = nonce;
        body.assign(whole.begin() + kBootloaderHeader, whole.end());
    };
    const auto nonce_of = [](const Bytes& b) {
        std::array<std::uint8_t, 0x10> a{};
        std::copy(b.begin(), b.end(), a.begin());
        return a;
    };
    BootloaderHeader sb_hdr, sc_hdr, sd_hdr, se_hdr;
    Bytes sb_body, sc_body, sd_body, se_data;
    split_bootloader(parts.sb, "SB", nonce_of(sb_nonce), sb_hdr, sb_body);
    split_bootloader(parts.sc, "SC", nonce_of(sc_nonce), sc_hdr, sc_body);
    split_bootloader(parts.sd, "SD", nonce_of(sd_nonce), sd_hdr, sd_body);
    const BootloaderHeader maybe_se = BootloaderHeader::parse(ByteView(parts.se).subspan(0, std::min<std::size_t>(parts.se.size(), kBootloaderHeader)));
    if (as_text(maybe_se.magic) == "SE") {   // an SE as the loader keeps it (compressed, with its header)
        se_hdr = maybe_se;
        se_data = build_lib::decompress_se(codec, parts.se);
    } else {   // the hypervisor and kernel: a header is made for them
        se_hdr.magic = {'S', 'E'};
        se_hdr.build = static_cast<std::uint16_t>(parts.build);
        se_hdr.qfe = 0x8000;
        se_hdr.nonce = std::array<std::uint8_t, 0x10>{};
        se_data = parts.se;
    }
    se_hdr.nonce = nonce_of(se_nonce);
    if (se_data.size() < 2 || se_data[0] != '^' || se_data[1] != 'N') damaged("Invalid SE bootloader!");

    FlashHeader flash;
    flash.magic = 0xFF4F;
    flash.build = static_cast<std::uint16_t>(parts.build);
    flash.qfe = 0x8000;
    flash.flags = 0;
    std::copy(kCopyright.begin(), kCopyright.end(), flash.copyright.begin());   // (the rest of the 0x40 bytes zero)
    flash.patch_slots = 2;
    flash.kv_version = 0x712;
    flash.patch_slot_size = 0x10000;

    Bytes img(kFlashHeaderEnd, 0);   // the flash header is written last
    if (parts.smc) {
        const Bytes smc = xecrypt::smc_encrypt(*parts.smc);
        img.resize(kSmcAt, 0);
        append(img, smc);
        flash.smc_offset = kSmcAt;
        flash.smc_length = static_cast<std::uint32_t>(smc.size());
        flash.entry = kSmcAt + flash.smc_length;
    } else {
        flash.smc_offset = kSmcAt;
        flash.smc_length = 0;
        flash.entry = kFlashHeaderEnd;
    }
    if (parts.keyvault) {
        Bytes vault = parts.keyvault->data;
        append(img, xecrypt::keyvault_encrypt(parts.keyvault->cpu_key, vault, rng));
        flash.kv_offset = kKeyvaultAt;
        flash.kv_length = 0x4000;
        flash.entry += 0x4000;
    } else {
        flash.kv_offset = kKeyvaultAt;
        flash.kv_length = 0;
    }

    const std::size_t sb_at = img.size();
    append(img, sb_hdr.to_bytes());
    append(img, sb_body);
    if (parts.test_kit) {   // SB's branch is rewritten
        const std::size_t at = sb_at + kSbTestKitBranchAt;
        if (at + 4 > img.size() || !std::equal(kSbBranchBefore.begin(), kSbBranchBefore.end(), img.begin() + static_cast<std::ptrdiff_t>(at))) {
            damaged("Original bytes mismatch!");
        }
        std::copy(kSbBranchAfter.begin(), kSbBranchAfter.end(), img.begin() + static_cast<std::ptrdiff_t>(at));
    }
    const std::size_t sc_at = img.size();
    append(img, sc_hdr.to_bytes());
    append(img, sc_body);
    const std::size_t sd_at = img.size();
    append(img, sd_hdr.to_bytes());
    append(img, sd_body);

    if (parts.patches) build_lib::patch_in_place(se_data, *parts.patches);   // the HV and kernel patched before they are compressed
    const Bytes compressed = build_lib::compress_se(codec, se_data, false);
    se_hdr.size = static_cast<std::uint32_t>(compressed.size() + 0x30);
    const std::size_t se_at = img.size();
    append(img, se_hdr.to_bytes());
    io::ByteWriter lead;   // the 8 zero bytes, the 0x280000 the loader reads and 4 more
    lead.zeros(8);
    lead.u32_be(0x280000);
    lead.zeros(4);
    append(img, lead.data());
    append(img, compressed);
    img.resize(img.size() + (se_hdr.padded_size() - se_hdr.size), 0);

    const Bytes se_hash = build_lib::calc_se_hash(img, se_at);
    std::copy(se_hash.begin(), se_hash.end(), img.begin() + static_cast<std::ptrdiff_t>(sd_at + kSdSeDigestWriteAt));
    build_lib::sign_bootloader_in_place(img, sd_at, *keys.sb_prv);
    build_lib::encrypt_bootloader_in_place(sb_key, img, sb_at);
    build_lib::encrypt_bootloader_in_place(sc_key, img, sc_at);
    build_lib::encrypt_bootloader_in_place(sd_key, img, sd_at);
    build_lib::encrypt_bootloader_in_place(se_key, img, se_at);

    img.resize(img.size() + build_lib::calc_pad_size(img.size(), 0x1000), 0);
    flash.sys_upd_addr = static_cast<std::uint32_t>(img.size());   // right after the image
    const Bytes header = flash.to_bytes();
    std::copy(header.begin(), header.end(), img.begin());

    if (!parts.test_kit) (void)parse(img, keys, codec, {.checks = true, .patches = true});   // an image that does not read back is a bug
    return img;
}

// ---- helpers and the command -------------------------------------------------------------------------------------------------------

Bytes unecc(ByteView dump, std::size_t block_size, std::size_t spare_size) {
    Bytes out;
    out.reserve(dump.size() / (block_size + spare_size) * block_size);
    for (std::size_t at = 0; at < dump.size(); at += block_size + spare_size) {
        const std::size_t n = std::min(block_size, dump.size() - at);
        out.insert(out.end(), dump.begin() + static_cast<std::ptrdiff_t>(at), dump.begin() + static_cast<std::ptrdiff_t>(at + n));
    }
    return out;
}

std::vector<std::size_t> find_images(ByteView data) {
    std::vector<std::size_t> out;
    for (auto at = data.begin();;) {
        const auto hit = std::search(at, data.end(), kImageNotice.begin(), kImageNotice.end());
        if (hit == data.end()) break;
        const std::size_t found = static_cast<std::size_t>(hit - data.begin());
        if (found >= kImageNoticeAt) out.push_back(found - kImageNoticeAt);
        at = hit + 1;
    }
    return out;
}

namespace {

using Json = nlohmann::json;

Bytes read_image_file(const stdfs::path& path, bool flash) {
    Bytes data = fs::read_file(path);
    return flash ? unecc(data) : data;
}

std::optional<Bytes> read_if_file(const stdfs::path& path) {
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return std::nullopt;
    return fs::read_file(path);
}

// The first of a file and a fallback that has bytes (what the build scripts call "try reading the sources").
std::optional<Bytes> first_of(const stdfs::path& file, const std::optional<Bytes>& fallback) {
    std::vector<build_lib::Source> sources = {file};
    if (fallback) sources.emplace_back(*fallback);
    return build_lib::first_source(sources);
}

Bytes need(const std::optional<Bytes>& bytes, std::string_view what) {
    if (!bytes) damaged(std::format("no {} was found in the build folder or the base image", what));
    return *bytes;
}

int build_command(const cli::Args& args, const keystore::KeyStore& store, build_lib::LzxCodec& codec) {
    const Keys keys = Keys::load(store);
    const stdfs::path output = fs::from_text(args.required("output"));
    stdfs::path smc_file, kv_file, sb_file, sc_file, sd_file, se_file, kernel_file, hypervisor_file, base_file, patches_file;
    bool use_smc = true, use_kv = true, test_kit = false, base_checks_disabled = false;
    std::uint32_t version = kBuildVersion;
    if (const auto manifest = args.value("manifest"); manifest && stdfs::is_regular_file(fs::from_text(*manifest))) {
        cb::out("Loading build manifest...\n");
        try {
            const Json m = Json::parse(fs::read_text(fs::from_text(*manifest)));
            cb::out("Setting up paths...\n");
            const Json& files = m.at("files");
            const stdfs::path dir = fs::from_text(files.at("base_directory").get<std::string>());
            const auto in_dir = [&](const char* key) { return dir / fs::from_text(files.at(key).get<std::string>()); };
            base_file = fs::from_text(files.at("base_image").get<std::string>());
            smc_file = in_dir("SMC");
            kv_file = in_dir("KV");
            sb_file = in_dir("SB");
            sc_file = in_dir("SC");
            sd_file = in_dir("SD");
            se_file = in_dir("SE");
            kernel_file = in_dir("kernel");
            hypervisor_file = in_dir("HV");
            patches_file = in_dir("HVK_patches");
            const Json& options = m.at("options");
            use_smc = options.at("use_smc").get<bool>();
            use_kv = options.at("use_kv").get<bool>();
            test_kit = options.at("test_kit").get<bool>();
            base_checks_disabled = options.at("base_image_checks_disabled").get<bool>();
            version = m.at("build").at("version").get<std::uint32_t>();
        } catch (const Json::exception& e) {   // a manifest of another shape
            damaged(std::format("the build manifest is not usable: {}", e.what()));
        }
    } else if (const auto folder = args.value("build-dir")) {   // a build directory instead of a manifest
        const stdfs::path dir = fs::from_text(*folder);
        smc_file = dir / "SMC_dec.bin";
        kv_file = dir / "KV_dec.bin";
        sb_file = dir / "sb.bin";
        sc_file = dir / "sc.bin";
        sd_file = dir / "sd.bin";
        se_file = dir / "se.bin";
        kernel_file = dir / "kernel.bin";
        hypervisor_file = dir / "hypervisor.bin";
        base_file = dir / "xboxrom.bin";
        patches_file = dir / "hvk.bin";
        use_kv = false;   // (the keyvault is built only from a manifest that asks for it)
    } else {
        cb::out("Building requires -m or -b arguments!\n");
        return 1;
    }

    std::optional<ShadowbootImage> base;   // the base image, when there is one
    if (const std::optional<Bytes> base_bytes = read_if_file(base_file)) {
        base = ShadowbootImage::parse(*base_bytes, keys, codec, {.checks = !base_checks_disabled, .patches = true});
    }
    Bytes se;
    const std::optional<Bytes> kernel = read_if_file(kernel_file), hypervisor = read_if_file(hypervisor_file);
    if (kernel && hypervisor) {
        cb::out("Reading raw HV/kernel...\n");
        se = cat(*hypervisor, *kernel);
    } else if (const std::optional<Bytes> se_bytes = read_if_file(se_file)) {
        cb::out("Decompressing SE...\n");
        se = build_lib::decompress_se(codec, *se_bytes);
    } else if (base) {
        cb::out("Reading HV/kernel from base image...\n");
        se = cat(base->hypervisor, base->kernel);
    } else {
        damaged("No HV/kernel pair, SE, or fallback image was provided!");
    }
    const auto from_base = [&](Bytes ShadowbootImage::*member) -> std::optional<Bytes> {
        return base ? std::optional<Bytes>(base.value().*member) : std::nullopt;
    };
    const auto optional_from_base = [&](std::optional<Bytes> ShadowbootImage::*member) -> std::optional<Bytes> { return base ? base.value().*member : std::nullopt; };
    ShadowbootImage::Parts parts;
    parts.sb = need(first_of(sb_file, from_base(&ShadowbootImage::sb_data)), "SB");
    parts.sc = need(first_of(sc_file, from_base(&ShadowbootImage::sc_data)), "SC");
    parts.sd = need(first_of(sd_file, from_base(&ShadowbootImage::sd_data)), "SD");
    parts.se = std::move(se);
    if (use_smc) parts.smc = first_of(smc_file, optional_from_base(&ShadowbootImage::smc));
    if (use_kv) {
        if (const std::optional<Bytes> kv = first_of(kv_file, optional_from_base(&ShadowbootImage::keyvault))) parts.keyvault = ShadowbootImage::Parts::Keyvault{Bytes(0x10, 0), *kv};
    }
    parts.patches = first_of(patches_file, std::nullopt);
    parts.test_kit = test_kit;
    parts.build = version;
    const Bytes built = ShadowbootImage::create(parts, keys, codec);
    cb::out_text(test_kit ? "Final image created!\n" : "Final image verified!\n");
    fs::write_file(output, built);
    cb::out("Final image location: \"{}\"\n", fs::to_text(stdfs::absolute(output)));
    return 0;
}

int extract_command(const cli::Args& args, const keystore::KeyStore& store, build_lib::LzxCodec& codec) {
    const Keys keys = Keys::load(store);
    const ShadowbootImage img = ShadowbootImage::parse(read_image_file(fs::from_text(args.required("input")), args.flag("flash")), keys, codec,
                                                       {.checks = !args.flag("nochecks"), .patches = true});
    cb::out("{}", img.info());
    const stdfs::path out = fs::from_text(args.required("output"));
    const bool all = args.flag("all");
    const auto wanted = [&](const char* a, const char* b = nullptr) { return all || args.flag(a) || (b != nullptr && args.flag(b)); };
    const auto write = [&](const std::string& name, ByteView data) {
        stdfs::create_directories(out);
        fs::write_file(out / fs::from_text(name), data);
    };
    if (wanted("smc") && img.smc && !img.smc->empty()) write("SMC_dec.bin", *img.smc);
    if (wanted("keyvault", "kv") && img.keyvault && !img.keyvault->empty()) write("KV_enc.bin", *img.keyvault);
    if (wanted("sb")) write(std::format("sb_{}.bin", img.sb_build), img.sb_data);
    if (wanted("sc")) write(std::format("sc_{}.bin", img.sc_build), img.sc_data);
    if (wanted("sd")) write(std::format("sd_{}.bin", img.sd_build), img.sd_data);
    if (wanted("se")) write(std::format("se_{}.bin", img.se_build), img.se_data);
    if (wanted("kernel")) write("kernel.exe", img.kernel);
    if (wanted("hypervisor", "hv")) write("hypervisor.bin", img.hypervisor);
    if (wanted("loader")) {
        if (img.patch_loader) {
            write("patch_loader.bin", *img.patch_loader);
        } else {
            cb::out("No patch loader found!\n");
        }
    }
    if (wanted("patches")) {
        if (!img.patches.empty()) {
            stdfs::create_directories(out / "Patches");
            Bytes combined;
            for (const PatchRecord& p : img.patches) {
                const std::uint32_t shown = p.address > kHypervisorBytes ? p.address + 0x80000000u : p.address;   // kernel addresses are shown as the console maps them
                fs::write_file(out / "Patches" / fs::from_text(std::format("{:08X}.bin", shown)), p.code);
                append(combined, p.whole());
            }
            io::ByteWriter end;
            end.u32_be(kPatchEnd);
            append(combined, end.data());
            write("patches_raw.bin", combined);
        } else {
            cb::out("No patches found!\n");
        }
    }
    return 0;
}

int split_command(const cli::Args& args, const keystore::KeyStore& store, build_lib::LzxCodec& codec) {
    const Keys keys = Keys::load(store);
    const Bytes data = fs::read_file(fs::from_text(args.required("input")));
    const std::vector<std::size_t> starts = find_images(data);
    const std::string kind = args.value_or("type", ""), revision = text::lower(args.value_or("revision", ""));
    const std::optional<std::uint32_t> kernel = args.integer<std::uint32_t>("kernel");
    const stdfs::path out = fs::from_text(args.required("output"));
    for (std::size_t i = 0; i < starts.size(); ++i) {
        const ByteView image = ByteView(data).subspan(starts[i], (i + 1 < starts.size() ? starts[i + 1] : data.size()) - starts[i]);
        const ShadowbootImage img = ShadowbootImage::parse(image, keys, codec, {.checks = !args.flag("nochecks"), .patches = false});
        const bool match = kernel && img.kernel_version == *kernel && *kernel == img.hypervisor_version && text::lower(img.console_type) == revision;
        const char* name = nullptr;   // the file this image would be written to, for the kind asked
        if (img.is_retail && kind == "retail") {
            name = "xboxromw2d.bin";
        } else if (img.is_testkit && kind == "testkit") {
            name = "xboxromtw2d.bin";
        } else if (!img.is_testkit && !img.is_retail && kind == "devkit") {
            name = "xboxromw2d.bin";
        }
        if (name != nullptr && match) {
            fs::write_file(out / name, image);
            cb::out("Found!\n");
            return 0;
        }
    }
    return 0;
}

}  // namespace

cli::Command command(const keystore::KeyStore& store, std::shared_ptr<build_lib::LzxCodec> codec) {
    const auto lzx = [codec]() -> build_lib::LzxCodec& {
        if (!codec) throw UnsupportedError("shadowboot", 0, "the Xbox LZX library is not available (LZXCompression.dll / LZXDecompression.dll)");
        return *codec;
    };
    const cli::Option flash{"--flash", "", "parse a flash image instead of a shadowboot"};
    const cli::Option nochecks{"--nochecks", "", "perform shadowboot parsing without integrity checks"};
    cli::Command build{"build", "build a shadowboot from a manifest or a build folder",
                       {cli::Positional{"output", "the output path"}, nochecks,
                        cli::Option{"--manifest", "-m", "the build manifest file", cli::Arg::Value, "FILE"},
                        cli::Option{"--build-dir", "-b", "the build directory path", cli::Arg::Value, "DIR"}},
                       [store, lzx](const cli::Args& a) { return build_command(a, store, lzx()); }};
    cli::Command extract{"extract", "extract the parts of a shadowboot",
                         {cli::Positional{"input", "the input path"}, cli::Positional{"output", "the output folder"}, flash, nochecks,
                          cli::Option{"--all", "", "extract all sections"}, cli::Option{"--smc", "", "extract the SMC"},
                          cli::Option{"--keyvault", "", "extract the keyvault"}, cli::Option{"--kv", "", "extract the keyvault"},
                          cli::Option{"--sb", "", "extract the SB"}, cli::Option{"--sc", "", "extract the SC"},
                          cli::Option{"--sd", "", "extract the SD"}, cli::Option{"--se", "", "extract the SE"},
                          cli::Option{"--kernel", "", "extract the kernel"}, cli::Option{"--hypervisor", "", "extract the hypervisor"},
                          cli::Option{"--hv", "", "extract the hypervisor"}, cli::Option{"--loader", "", "extract the HV/kernel patch loader"},
                          cli::Option{"--patches", "", "extract the HV/kernel patches"}},
                         [store, lzx](const cli::Args& a) { return extract_command(a, store, lzx()); }};
    cli::Command info{"info", "show what a shadowboot is", {cli::Positional{"input", "the input path"}, flash, nochecks},
                      [store, lzx](const cli::Args& a) {
                          const Keys keys = Keys::load(store);
                          const ShadowbootImage img = ShadowbootImage::parse(read_image_file(fs::from_text(a.required("input")), a.flag("flash")), keys, lzx(),
                                                                             {.checks = !a.flag("nochecks"), .patches = true});
                          cb::out("{}", img.info());
                          return 0;
                      }};
    cli::Command split{"split", "find one console's shadowboot in a file of several",
                       {cli::Positional{"input", "the input path"}, cli::Positional{"output", "the output folder"}, nochecks,
                        cli::Option{"--kernel", "-k", "the kernel version", cli::Arg::Value, "N"},
                        cli::Option{"--revision", "-r", "the console revision (xenon zephyr falcon jasper trinity corona winchester)", cli::Arg::Value, "NAME"},
                        cli::Option{"--type", "-t", "the console type (retail testkit devkit)", cli::Arg::Value, "TYPE"}},
                       [store, lzx](const cli::Args& a) { return split_command(a, store, lzx()); }};
    cli::Command test{"test", "(nothing yet)", {cli::Positional{"input", "the input path"}, cli::Positional{"output", "the output path"}, nochecks},
                      [](const cli::Args&) { return 0; }};
    return cli::Command{"shadowboot", "extract and build Xbox 360 shadowboots", {}, {}, {build, extract, info, split, test}, "Xbox 360", ""};
}

}  // namespace cb::formats::xbox::crypto::shadowboot
