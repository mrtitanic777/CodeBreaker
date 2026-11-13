#include "cb/formats/xbox/crypto/xekeys_signer.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::crypto::xekeys_signer {

namespace {

constexpr std::size_t kKeyAt = 0x10;
constexpr std::size_t kSignatureAt = 0x20;
constexpr std::size_t kKeySize = 0x10;
constexpr std::uint16_t kMarker = 0xDEAD;

}  // namespace

Bytes sign(const xecrypt::RsaKey& hvx_private_key, ByteView bl1_key, ByteView payload, PayloadMagic magic, xecrypt::RandomSource& random) {
    if (bl1_key.size() != xecrypt::kAesKeySize) throw UsageError("the 1BL key is 16 bytes");
    const Bytes hvx_key = random.bytes(kKeySize);

    io::ByteWriter w;
    w.u16_be(static_cast<std::uint16_t>(magic));
    w.u16_be(kMarker);
    w.u32_be(0);
    w.u32_be(static_cast<std::uint32_t>(kHeaderSize));
    w.u32_be(static_cast<std::uint32_t>(kHeaderSize + payload.size()));
    w.bytes(hvx_key);
    w.zeros(kHeaderSize - w.size());
    w.bytes(payload);
    Bytes image = w.take();

    // the hash covers the first 0x10 bytes and the payload, not the key or the signature place between
    Bytes covered(image.begin(), image.begin() + static_cast<std::ptrdiff_t>(kKeyAt));
    covered.insert(covered.end(), image.begin() + static_cast<std::ptrdiff_t>(kHeaderSize), image.end());
    const Bytes digest = xecrypt::rot_sum_sha(covered);
    const Bytes signature = hvx_private_key.sign(ByteView(digest).first(xecrypt::kShaDigestSize), as_bytes(kSalt));
    std::copy(signature.begin(), signature.end(), image.begin() + static_cast<std::ptrdiff_t>(kSignatureAt));

    const Bytes rc4_key = xecrypt::hmac_sha(bl1_key, hvx_key);
    const Bytes encrypted = xecrypt::rc4(ByteView(rc4_key).first(0x10), ByteView(image).subspan(kSignatureAt));
    std::copy(encrypted.begin(), encrypted.end(), image.begin() + static_cast<std::ptrdiff_t>(kSignatureAt));
    return image;
}

cli::Command command(const keystore::KeyStore& store) {
    return cli::Command{
        "xekeys_signer",
        "sign a payload for XeKeysExecute with the HVX private key",
        {cli::Positional{"input", "the payload executable to sign"}, cli::Positional{"output", "the signed payload file"},
         cli::Option{"--retail", "", "a retail console's payload (the default is a devkit's)", cli::Arg::Flag, "", ""}},
        [store](const cli::Args& args) {
            const Bytes payload = fs::read_file(fs::from_text(args.required("input")));
            const xecrypt::RsaKey key = store.load_and_verify(keystore::KeyFile::HvxPrv);
            const std::optional<Bytes> bl1 = xecrypt::load_store_key(store.folder(), "1BL_key.bin");
            if (!bl1) throw NotFoundError(store.folder() / "1BL_key.bin", 0, "the 1BL key isn't in the key store");
            const Bytes image = sign(key, *bl1, payload, args.flag("retail") ? PayloadMagic::Retail : PayloadMagic::Devkit, xecrypt::os_random());
            fs::write_file(fs::from_text(args.required("output")), image);
            cb::out("Done!\n");
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::xekeys_signer
