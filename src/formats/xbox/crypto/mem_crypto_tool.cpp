#include "formats/xbox/crypto/mem_crypto_tool.hpp"

#include <array>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "formats/xbox/crypto/mem_crypto.hpp"
#include "formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::mem_crypto_tool {

namespace {

constexpr std::uint8_t kTestKeyByte = 0x55;
constexpr std::string_view kSaltHex = "892BB9F952C7759392A12A3184E0358E";
constexpr std::uint32_t kReportHvexAddress = 0xB00B;
constexpr std::string_view kNonce = "testtest";

std::string keys_text(ByteView white, ByteView aes, ByteView hash) {
    return std::format("W: {}\nA: {}\nH: {}\n\n", support::hex_upper(white), support::hex_upper(aes), support::hex_upper(hash));
}

}  // namespace

std::string calculate_report(ByteView hypervisor) {
    const Bytes key(kMemKeySize, kTestKeyByte);
    const Bytes salt = cb::crypto::from_hex(kSaltHex);
    Bytes nonce = to_bytes(kNonce);
    nonce.resize(kKeyBlobNonceSize, 0);
    const MemoryCrypto mem(key, key, key);
    std::string out = keys_text(key, key, key) + keys_text(key, key, key);
    out += "Hash 1: " + support::hex_upper(mem.hash_1_digest(hypervisor, salt)) + "\n\n";
    out += "Hash 2: " + support::hex_upper(mem.hash_2_digest(hypervisor, salt, kReportHvexAddress)) + "\n\n";
    out += "Key blob: " + support::hex_upper(mem.key_blob(nonce)) + "\n\n";
    return out;
}

cli::Command command(const std::filesystem::path& bin_dir) {
    cli::Command calculate{"calculate",
                           "the challenge-response hashes and key blob of the cleaned 17559 hypervisor",
                           {},
                           [bin_dir](const cli::Args&) {
                               cb::out_text(calculate_report(fs::read_file(bin_dir / "HV_17559_Cleaned.bin")));
                               cb::out("Done!\n");
                               return 0;
                           },
                           {},
                           "",
                           ""};
    cli::Command test{"test",
                      "decrypt HV.enc.bin with the keys of keys.bin into HV.dec.bin",
                      {},
                      [bin_dir](const cli::Args&) {
                          const Bytes keys = fs::read_file(bin_dir / "keys.bin");
                          if (keys.size() < 3 * kMemKeySize) throw FormatError("MemCrypto", keys.size(), "keys.bin holds the white, AES and hash keys (48 bytes)");
                          const MemoryCrypto mem(ByteView(keys).first(kMemKeySize), ByteView(keys).subspan(kMemKeySize, kMemKeySize), ByteView(keys).last(kMemKeySize));
                          fs::write_file("HV.dec.bin", mem.decrypt(fs::read_file(bin_dir / "HV.enc.bin"), 0));
                          cb::out("Done!\n");
                          return 0;
                      },
                      {},
                      "",
                      ""};
    return cli::Command{"MemCrypto", "the Xbox 360 memory-encryption crypto: calculate the challenge responses, or decrypt a hypervisor", {}, {},
                        {std::move(calculate), std::move(test)}, "", ""};
}

}  // namespace cb::formats::xbox::crypto::mem_crypto_tool
