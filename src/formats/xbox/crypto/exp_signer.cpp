#include "formats/xbox/crypto/exp_signer.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"

namespace cb::formats::xbox::crypto::exp_signer {

namespace {

constexpr std::size_t kHashAt = 0xC;
constexpr std::size_t kIvAt = 0x20;
constexpr std::size_t kSignatureAt = 0x30;
constexpr std::size_t kInfoAt = 0x130;          // the expansion info, which the hash starts at and the encryption does
constexpr std::size_t kSectionInfoAt = 0x160;
constexpr std::size_t kSigned = 0x30;           // what the signature covers

Bytes header_of(std::uint32_t magic, std::uint32_t id, std::size_t padded_payload) {
    io::ByteWriter w;
    w.u32_be(magic);
    w.u32_be(0);   // flags
    w.u32_be(static_cast<std::uint32_t>(kHeaderSize + padded_payload));
    w.zeros(0x14);    // the hash
    w.zeros(0x10);    // the IV
    w.zeros(0x100);   // the signature
    // 0x130: the expansion info
    w.u32_be(id);
    w.zeros(0xC);
    w.u64_be(0);
    w.u64_be(0);
    w.u32_be(0);
    w.u32_be(0);
    w.u32_be(static_cast<std::uint32_t>(kSectionInfoAt));
    w.u32_be(static_cast<std::uint32_t>(padded_payload + 0x10));
    // 0x160: the section info
    w.u32_be(0x10);
    w.u32_be(0x10);
    w.u32_be(static_cast<std::uint32_t>(padded_payload));
    w.zeros(4);
    return w.take();
}

}  // namespace

Bytes sign(const xecrypt::RsaKey& key, ByteView payload, const Options& options, xecrypt::RandomSource& random) {
    Bytes body(payload.begin(), payload.end());
    body.resize(body.size() + ((16 - body.size() % 16) % 16), 0);

    Bytes image = header_of(static_cast<std::uint32_t>(options.magic), options.id, body.size());
    append(image, body);

    const Bytes hash = xecrypt::sha(ByteView(image).subspan(kInfoAt));
    std::copy(hash.begin(), hash.end(), image.begin() + static_cast<std::ptrdiff_t>(kHashAt));

    Bytes signature;
    switch (options.magic) {
        case ExpansionMagic::Hxpr:
            signature = key.sign(xecrypt::rot_sum_sha(ByteView(image).first(kSigned)), as_bytes(kSalt));
            break;
        case ExpansionMagic::Sigm: signature = key.sign_pkcs1(xecrypt::rot_sum_sha(ByteView(image).first(kSigned))); break;
        case ExpansionMagic::Hxpc:
        case ExpansionMagic::Sigc:
            if (!options.cpu_key || !xecrypt::cpu_key_valid(*options.cpu_key)) throw UsageError("a valid CPU key is required for HXPC/SIGC");
            signature = key.sign(xecrypt::hmac_sha(*options.cpu_key, ByteView(image).first(kSigned)), as_bytes(kSalt));
            break;
    }
    std::copy(signature.begin(), signature.end(), image.begin() + static_cast<std::ptrdiff_t>(kSignatureAt));

    if (options.magic == ExpansionMagic::Hxpr && options.encrypt) {
        if (!options.bl1_key) throw UsageError("the 1BL key is needed to encrypt an expansion");
        const Bytes iv = random.bytes(0x10);
        std::copy(iv.begin(), iv.end(), image.begin() + static_cast<std::ptrdiff_t>(kIvAt));
        xecrypt::BlockCipher aes(xecrypt::CipherAlgorithm::Aes, *options.bl1_key, xecrypt::CipherMode::Cbc, iv);
        const Bytes encrypted = aes.encrypt(ByteView(image).subspan(kInfoAt));
        std::copy(encrypted.begin(), encrypted.end(), image.begin() + static_cast<std::ptrdiff_t>(kInfoAt));
    }
    return image;
}

cli::Command command(const keystore::KeyStore& store) {
    return cli::Command{
        "exp_signer",
        "sign an HvxExpansionInstall payload",
        {cli::Positional{"input", "the payload executable to sign"},
         cli::Option{"--ofile", "-o", "the signed payload file (default: the input file)", cli::Arg::Value, "FILE", ""},
         cli::Option{"--expansion-id", "-i", "the expansion id, in hex (default 0x48565050)", cli::Arg::Value, "ID", "0x48565050"},
         cli::Option{"--keyfile", "-k", "the private key to sign with (default: the key store's HVX key)", cli::Arg::Value, "FILE", ""},
         cli::Option{"--encrypt", "", "encrypt the expansion with the 1BL key", cli::Arg::Flag, "", ""}},
        [store](const cli::Args& args) {
            const std::filesystem::path input = fs::from_text(args.required("input"));
            const Bytes payload = fs::read_file(input);
            const std::optional<std::string> key_file = args.value("keyfile");
            const xecrypt::RsaKey key = key_file && !key_file->empty() ? xecrypt::RsaKey::parse(fs::read_file(fs::from_text(*key_file)))
                                                                         : store.load_and_verify(keystore::KeyFile::HvxPrv);
            const std::string id_text = args.required("expansion-id");
            const std::string_view digits = text::istarts_with(id_text, "0x") ? std::string_view(id_text).substr(2) : std::string_view(id_text);
            const std::optional<std::uint32_t> id = text::parse_int<std::uint32_t>(digits, 16);
            if (!id) throw UsageError("the expansion id is hex: " + id_text);
            Options options;
            options.id = *id;
            options.encrypt = args.flag("encrypt");
            if (options.encrypt) {
                options.bl1_key = xecrypt::load_store_key(store.folder(), "1BL_key.bin");
                if (!options.bl1_key) throw NotFoundError(store.folder() / "1BL_key.bin", 0, "the 1BL key isn't in the key store");
            }
            const std::optional<std::string> output = args.value("ofile");
            const Bytes image = sign(key, payload, options);
            fs::write_file(output && !output->empty() ? fs::from_text(*output) : input, image);
            cb::out("Done!\n");
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::exp_signer
