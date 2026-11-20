// formats/xbox/crypto/custom_keys.hpp -- the tools that make and publish a custom RSA key pair: `crypto create_key`, `crypto export_keys` and
// `crypto format_key`.
//
//     custom_keys::create(store.folder(), 2048);                       // custom_pub.bin and custom_prv.bin in the key store folder
//     std::string source = custom_keys::export_source(folder, random); // the pair, and a test signature, as C++ arrays
//
// The key pair is a 2048-bit RSA pair in the console's key format, written as `custom_pub.bin` (the public part) and `custom_prv.bin` in the
// key store folder. `export_keys` prints both as C++ byte arrays (TestPubKey, TestPrvKey) with a block of random test data (TestData) and its
// signature by the private key (TestSignature; the rot-sum-SHA of the data, salt "SALTTEST"), so a console-side build can check what it verifies.
// `format_key` writes the public key as the array ExpPubKey into Output/Zero/ExpPubKey.h (format_key.hpp).
#pragma once

#include <filesystem>
#include <string>

#include "core/cli.hpp"
#include "formats/xbox/crypto/keystore.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::custom_keys {

inline constexpr std::string_view kPublicKeyFile = "custom_pub.bin";
inline constexpr std::string_view kPrivateKeyFile = "custom_prv.bin";
inline constexpr std::string_view kSalt = "SALTTEST";

// Generate a pair and write it into `folder` (created if need be).
void create(const std::filesystem::path& folder, unsigned bits = 2048, xecrypt::RandomSource& random = xecrypt::os_random());

// The C++ source of the pair in `folder` and of a signed block of random data: TestPrvKey, TestPubKey, TestData, TestSignature, in that order.
// NotFoundError when the pair is not there.
std::string export_source(const std::filesystem::path& folder, xecrypt::RandomSource& random = xecrypt::os_random());

// The three tools' commands; they work in the key store's folder.
cli::Command create_command(const keystore::KeyStore& store);
cli::Command export_command(const keystore::KeyStore& store);
cli::Command format_command(const keystore::KeyStore& store);

}  // namespace cb::formats::xbox::crypto::custom_keys
