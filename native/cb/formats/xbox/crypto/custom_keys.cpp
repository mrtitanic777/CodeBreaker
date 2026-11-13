#include "cb/formats/xbox/crypto/custom_keys.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/bin2lang.hpp"
#include "cb/formats/xbox/crypto/format_key.hpp"

namespace cb::formats::xbox::crypto::custom_keys {

void create(const std::filesystem::path& folder, unsigned bits, xecrypt::RandomSource& random) {
    const xecrypt::KeyPair pair = xecrypt::generate_key_pair(bits, 0x10001, random);
    fs::make_dirs(folder);
    fs::write_file(folder / std::string(kPublicKeyFile), pair.public_key);
    fs::write_file(folder / std::string(kPrivateKeyFile), pair.private_key);
}

std::string export_source(const std::filesystem::path& folder, xecrypt::RandomSource& random) {
    const Bytes private_bytes = fs::read_file(folder / std::string(kPrivateKeyFile));
    const Bytes public_bytes = fs::read_file(folder / std::string(kPublicKeyFile));
    const xecrypt::RsaKey key = xecrypt::RsaKey::parse(private_bytes);
    const Bytes data = random.bytes(0x100);
    const Bytes signature = key.sign(xecrypt::rot_sum_sha(data), as_bytes(kSalt));
    const auto cpp = [](ByteView bytes, std::string_view name) { return bin2lang::format(bytes, bin2lang::Language::Cpp, name); };
    // each array is followed by a blank line, as the tool prints them
    return cpp(private_bytes, "TestPrvKey") + "\n" + cpp(public_bytes, "TestPubKey") + "\n" + cpp(data, "TestData") + "\n" + cpp(signature, "TestSignature") + "\n";
}

cli::Command create_command(const keystore::KeyStore& store) {
    return cli::Command{"create_key",
                        "make a 2048-bit RSA key pair: custom_pub.bin and custom_prv.bin in the key store",
                        {},
                        [store](const cli::Args&) {
                            create(store.folder());
                            cb::out("Done!\n");
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

cli::Command export_command(const keystore::KeyStore& store) {
    return cli::Command{"export_keys",
                        "print the custom key pair, a block of test data and its signature as C++ arrays",
                        {},
                        [store](const cli::Args&) {
                            cb::out_text(export_source(store.folder()));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

cli::Command format_command(const keystore::KeyStore& store) {
    return cli::Command{
        "format_key",
        "write the custom public key as the C++ array ExpPubKey (Output/Zero/ExpPubKey.h)",
        {},
        [store](const cli::Args&) {
            const std::string source = format_key::byte_array_source(fs::read_file(store.folder() / std::string(kPublicKeyFile)));
            const std::filesystem::path header = fs::from_text(format_key::kHeaderFile);
            fs::make_dirs(header.parent_path());
            fs::write_text(header, source);
            cb::out_text(source);
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::custom_keys
