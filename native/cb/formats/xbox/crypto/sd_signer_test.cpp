// Tests of cb/formats/xbox/crypto/sd_signer.hpp: signing a CD/SD (padding, size, signature that verifies, zeroed nonce), the
// checksum line, refusing an image that is not a CD/SD, and the command on files (output beside the input or to -o).
//
// Behaviour pinned here: the key store and arguments are the command's, not module globals; a bad magic is a
// FormatError and a missing input a NotFoundError rather than an assert / argparse error; the command prints through cb::out.
#include "cb/formats/xbox/crypto/sd_signer.hpp"

#include "cb/core/cli.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"

namespace sd_signer_test {

using namespace cb;
using namespace cb::formats::xbox::crypto;

static Bytes image(char a, char b, std::size_t size) {
    Bytes data(size, 0);
    data[0] = static_cast<std::uint8_t>(a);
    data[1] = static_cast<std::uint8_t>(b);
    for (std::size_t i = 0x10; i < size; ++i) data[i] = static_cast<std::uint8_t>(i * 7 + 1);   // a nonce and a body
    return data;
}

static std::uint32_t be32(const Bytes& b, std::size_t at) {
    return (std::uint32_t{b[at]} << 24) | (std::uint32_t{b[at + 1]} << 16) | (std::uint32_t{b[at + 2]} << 8) | b[at + 3];
}

CB_TEST("formats.xbox.crypto.sd_signer.signs_pads_and_zeroes_the_nonce", [] {
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    const Bytes input = image('S', 'D', 0x305);   // not a multiple of 16
    const Bytes out = sd_signer::sign_sd(key, input);
    CB_CHECK(out.size() == 0x310);
    CB_CHECK(be32(out, 0xC) == 0x310);
    for (std::size_t i = 0x10; i < 0x20; ++i) CB_CHECK(out[i] == 0);
    CB_CHECK(out[0x305] == 0 && out[0x30F] == 0);   // the padding
    // the signature was made over the image with its nonce still there, so verifying the output means putting it back
    Bytes with_nonce = out;
    std::copy(input.begin() + 0x10, input.begin() + 0x20, with_nonce.begin() + 0x10);
    CB_CHECK(build_lib::verify_sd_4bl(key.public_key(), as_bytes(build_lib::kSdSalt), with_nonce));
    // an image that is already a multiple of 16 is not padded
    CB_CHECK(sd_signer::sign_sd(key, image('C', 'D', 0x300)).size() == 0x300);
});

CB_TEST("formats.xbox.crypto.sd_signer.refuses_what_is_not_a_cd_or_sd", [] {
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    bool refused = false;
    try {
        (void)sd_signer::sign_sd(key, image('S', 'E', 0x300));
    } catch (const FormatError&) {
        refused = true;
    }
    CB_CHECK(refused);
    refused = false;
    try {
        (void)sd_signer::sign_sd(key, Bytes{'S'});
    } catch (const FormatError&) {
        refused = true;
    }
    CB_CHECK(refused);
    refused = false;
    try {
        (void)sd_signer::sign_sd(key, image('S', 'D', 0x40));   // too short to be signed (the 4BL signs from 0x120)
    } catch (const FormatError&) {
        refused = true;
    }
    CB_CHECK(refused);
});

CB_TEST("formats.xbox.crypto.sd_signer.the_checksum_line", [] {
    // CRC-32 of "123456789" is cbf43926
    const Bytes check{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    CB_CHECK(sd_signer::checksum_line("SD.bin", check) == "SD.bin,cbf43926");
});

CB_TEST("formats.xbox.crypto.sd_signer.the_command_signs_a_file", [] {
    fs::ScopedTempDir dir("cb-sd-signer");
    const xecrypt::RsaKey key = xecrypt::RsaKey::generate(2048);
    const Bytes key_bytes(key.bytes().begin(), key.bytes().end());
    fs::write_file(dir.path() / "SB_prv.bin", key_bytes);
    keystore::KeyStore store(dir.path());
    store.accept_checksum(keystore::KeyFile::SbPrv, cb::crypto::crc32(key_bytes));
    const cli::Command command = sd_signer::command(store);
    CB_CHECK(command.name == "sd-signer");

    const std::filesystem::path input = dir.path() / "SD.bin";
    const Bytes original = image('S', 'D', 0x305);
    fs::write_file(input, original);
    const std::filesystem::path output = dir.path() / "SD_signed.bin";
    cli::Cli cli("codebreaker", {command});
    CB_CHECK(cli.run({"sd-signer", fs::to_text(input), "-o", fs::to_text(output)}) == 0);
    CB_CHECK(fs::read_file(input) == original);   // the input is left alone with -o
    CB_CHECK(fs::read_file(output) == sd_signer::sign_sd(key, original));
    // without -o the input is replaced
    CB_CHECK(cli.run({"sd-signer", fs::to_text(input)}) == 0);
    CB_CHECK(fs::read_file(input) == fs::read_file(output));
    // a missing input
    bool missing = false;
    try {
        (void)cli.run({"sd-signer", fs::to_text(dir.path() / "nothing.bin")});
    } catch (const NotFoundError&) {
        missing = true;
    }
    CB_CHECK(missing);
});

}  // namespace sd_signer_test
