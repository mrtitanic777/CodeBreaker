// cb/formats/xbox/crypto/shadowboot.hpp -- Xbox 360 shadowboot images: reading one apart (flash header, SMC, keyvault, the SB / SC / SD / SE
// bootloaders, the hypervisor and kernel inside the SE, the patch table of the SD), building one from its parts, and the `shadowboot` command.
//
//     Keys keys = Keys::load(store);                                       the 1BL key and public key, the private SB key
//     ShadowbootImage image = ShadowbootImage::parse(bytes, keys, codec);  decrypt, decompress and check it
//     std::string report = image.info();                                   console type, versions, retail or XShell, kit
//     Bytes built = ShadowbootImage::create(parts, keys, codec);           sign, compress and encrypt a new one
//
// A shadowboot is the console's boot image without its NAND wrapper: a 0x80-byte flash header, the SMC (encrypted), the keyvault, and four
// bootloaders each with a 0x20-byte header (magic, build, QFE, flags, entry, size, a 16-byte nonce). The body of each is RC4-encrypted
// with a key derived from the chain: SB's from the 1BL key and its nonce, SC's from zeros, SD's from SC's key, SE's from SD's, each by the
// HMAC-SHA1 of the nonce (first 16 bytes). SB (2BL) is signed by the 1BL's key, SC and SD by the key inside SB, and SE is vouched for by
// a rot-sum hash in SD. The SE is the hypervisor (0x40000 bytes) and the kernel, LZX-compressed behind a 0x30-byte header.
//
// The Xbox LZX library is not part of this program: every call that reads or builds an SE takes the `build_lib::LzxCodec` made from it.
// The keys come from the key store (none is in the repository), so a caller makes them: `Keys::load(store)`.
//
// Errors: FormatError for a flash header or bootloader that does not hold together, a body outside the image, a check that fails (the
// message names it: "Invalid SB signature"), UsageError for a missing key, UnsupportedError for no LZX codec.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"
#include "cb/formats/xbox/crypto/keystore.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::shadowboot {

inline constexpr std::uint32_t kBuildVersion = 17559;
inline constexpr std::size_t kHypervisorBytes = 0x40000;   // the HV at the start of the SE's decompressed body
inline constexpr std::size_t kSmcConfigSize = 280;

// The keys a parse or a build needs. `one_bl_key` is the 16-byte HMAC key SB's RC4 key derives from (the 1BL's own: key-store file
// 1BL_key.bin), `one_bl_pub` verifies SB's signature, `sb_prv` signs the SD when an image is built. A parse without checks needs only
// the first; a build the first and the last.
struct Keys {
    Bytes one_bl_key;
    std::optional<xecrypt::RsaKey> one_bl_pub;
    std::optional<xecrypt::RsaKey> sb_prv;

    // From the key store: 1BL_key.bin (NotFoundError when it is not there) and the public 1BL and private SB keys (checked by checksum).
    static Keys load(const keystore::KeyStore& store);
};

// One bootloader as the flash header lays it out.
struct Bootloader {
    std::string name;                         // "SB" "SC" "SD" "SE"
    std::uint32_t offset = 0;                 // of its header in the image
    std::uint32_t size = 0, padded_size = 0;  // the header's size and that rounded to 16
    build_lib::BootloaderHeader header;       // as stored, its nonce zeroed once the key is derived from it
    Bytes key;                                // the RC4 key of its body
};

// A patch of the SD's patch table: the HV or kernel words to write at `address`.
struct PatchRecord {
    std::uint32_t offset = 0;                 // of the record in the SD
    std::uint32_t address = 0;
    std::uint32_t size_words = 0;
    Bytes code;
    Bytes whole() const;                      // the record as the table stores it: address, count, words
};

struct ParseOptions {
    bool checks = true;       // verify SB, SC and SD signatures and the SD and SE digests (needs `one_bl_pub`)
    bool patches = true;      // read the patch table of the SD
};

class ShadowbootImage {
public:
    static ShadowbootImage parse(ByteView data, const Keys& keys, build_lib::LzxCodec& codec, ParseOptions options = {});

    // What `create` builds an image from. SB, SC and SD are whole bootloaders (header and body, 0x20-byte header with magic SB / SC /
    // SD). `se` is a whole SE (magic SE: decompressed) or the raw hypervisor and kernel (a header is made for it from `build`). The
    // SMC is decrypted, the keyvault decrypted with its CPU key.
    struct Parts {
        Bytes sb, sc, sd, se;
        std::optional<Bytes> smc;
        struct Keyvault {
            Bytes cpu_key;
            Bytes data;
        };
        std::optional<Keyvault> keyvault;
        std::optional<Bytes> patches;      // a patch table written over the hypervisor and kernel
        bool test_kit = false;             // SB's branch is rewritten for a test kit, and the image is not verified afterwards
        std::uint32_t build = kBuildVersion;
    };
    static Bytes create(const Parts& parts, const Keys& keys, build_lib::LzxCodec& codec,
                        xecrypt::RandomSource& rng = xecrypt::os_random());

    // ---- what the image holds (all decrypted) ----
    build_lib::FlashHeader flash_header;
    Bootloader sb, sc, sd, se;
    std::optional<Bytes> smc, smc_config, keyvault;   // the keyvault as stored (encrypted)
    Bytes sb_data, sc_data, sd_data, se_data;         // header (nonce zeroed) and body
    Bytes hypervisor, kernel;
    std::optional<Bytes> patch_loader;                // the 0x40 bytes of code before the patch records
    std::uint32_t patch_loader_offset = 0;
    std::vector<PatchRecord> patches;

    // ---- the metadata read out of the bootloaders ----
    Bytes sb_signature, sc_signature, sd_signature;
    std::optional<xecrypt::RsaKey> sb_pub_key, sd_pub_key;
    Bytes sc_nonce, sc_salt, sd_salt, sd_digest;      // from SB
    Bytes sf_nonce, sf_salt, se_digest;               // from SD
    std::string console_type, smc_version;            // empty without an SMC
    std::uint16_t sb_build = 0, sc_build = 0, sd_build = 0, se_build = 0;
    std::uint16_t hypervisor_version = 0;
    std::uint16_t kernel_version = 0;                 // the SE's build
    bool is_retail = false;                           // the HV starts with 0x4E: the dashboard is the main menu, not XShell
    bool is_testkit = false;                          // the kernel names \testkit\

    // The report `shadowboot info` prints.
    std::string info() const;

    // The checks, one by one (true when the part checks out). `one_bl_pub` is needed for the SB signature.
    bool check_signature_sb(const xecrypt::RsaKey& one_bl_pub) const;
    bool check_signature_sc() const;
    bool check_signature_sd() const;
    bool check_digest_sd() const;
    bool check_digest_se() const;
};

// A flash dump with its spare bytes (16 after every 512) as the plain image: the spare bytes dropped.
Bytes unecc(ByteView dump, std::size_t block_size = 512, std::size_t spare_size = 16);

// The shadowboots inside a bigger file (the kernel images of an XDK cabinet, one after another): where each starts, found by the
// copyright notice in the flash header.
std::vector<std::size_t> find_images(ByteView data);

// `shadowboot build|extract|info|split|test`. `codec` makes the SE's LZX; a command that needs it with none given is an
// UnsupportedError. Keys are read from `store` when a command needs them.
cli::Command command(const keystore::KeyStore& store, std::shared_ptr<build_lib::LzxCodec> codec = nullptr);

}  // namespace cb::formats::xbox::crypto::shadowboot
