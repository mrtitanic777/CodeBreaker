#include "cb/formats/xbox/crypto/sd_signer.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/build_lib.hpp"

namespace cb::formats::xbox::crypto::sd_signer {

namespace {

constexpr std::string_view kReader = "sd_signer";
constexpr std::size_t kSizeAt = 0xC;
constexpr std::size_t kNonceAt = 0x10;
constexpr std::size_t kNonceSize = 0x10;

}  // namespace

Bytes sign_sd(const xecrypt::RsaKey& key, ByteView bootloader) {
    const bool is_cd = bootloader.size() >= 2 && bootloader[0] == 'C' && bootloader[1] == 'D';
    const bool is_sd = bootloader.size() >= 2 && bootloader[0] == 'S' && bootloader[1] == 'D';
    if (!is_cd && !is_sd) throw FormatError(kReader, 0, "the input isn't a valid CD or SD bootloader image");
    Bytes data(bootloader.begin(), bootloader.end());
    data.resize(data.size() + build_lib::calc_pad_size(data.size()), 0);
    // the padded length goes at 0xC, big-endian; the 4BL is at least a header long or the signing below refuses it
    if (data.size() < kSizeAt + 4) throw FormatError(kReader, data.size(), "a bootloader shorter than its header");
    const std::uint32_t size = static_cast<std::uint32_t>(data.size());
    for (int i = 0; i < 4; ++i) data[kSizeAt + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(size >> (24 - 8 * i));
    Bytes signed_image = build_lib::sign_sd_4bl(key, as_bytes(build_lib::kSdSalt), data);
    std::fill_n(signed_image.begin() + static_cast<std::ptrdiff_t>(kNonceAt), kNonceSize, std::uint8_t{0});
    return signed_image;
}

std::string checksum_line(std::string_view name, ByteView data) {
    return std::format("{},{:08x}", name, cb::crypto::crc32(data));
}

cli::Command command(const keystore::KeyStore& store) {
    return cli::Command{
        "sd-signer",
        "sign a retail CD or devkit SD bootloader for the Xbox 360 and print its XeBuild checksum",
        {cli::Positional{"input", "the CD/SD to sign"},
         cli::Option{"--output", "-o", "the file to write (default: the input file)", cli::Arg::Value, "FILE"}},
        [store](const cli::Args& args) {
            const std::filesystem::path input = fs::from_text(args.required("input"));
            if (!std::filesystem::is_regular_file(input)) throw NotFoundError(input, 0, "the file doesn't exist");
            const std::optional<std::string> out = args.value("output");
            const std::filesystem::path out_path = out ? fs::from_text(*out) : input;
            const xecrypt::RsaKey key = store.load_and_verify(keystore::KeyFile::SbPrv);
            const Bytes signed_image = sign_sd(key, fs::read_file(input));
            fs::write_file(out_path, signed_image);
            cb::out("{}\n", checksum_line(out_path.filename().string(), signed_image));
            return 0;
        },
        {},
        "Xbox 360",
        ""};
}

}  // namespace cb::formats::xbox::crypto::sd_signer
