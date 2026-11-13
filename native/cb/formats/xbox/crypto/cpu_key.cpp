#include "cb/formats/xbox/crypto/cpu_key.hpp"

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::cpu_key {

std::string report(ByteView key) {
    if (key.size() != xecrypt::kCpuKeySize) throw UsageError("a CPU key is 16 bytes");
    std::string bits;
    for (const std::uint8_t byte : key)
        for (int bit = 7; bit >= 0; --bit) bits += ((byte >> bit) & 1) ? '1' : '0';
    const std::size_t uid_bits = bits.size() - kEccBits;
    return "ECC Bits: " + bits.substr(uid_bits) + "\nUID Bits: " + bits.substr(0, uid_bits) + "\n" + support::hex_upper(key) + "\n";
}

cli::Command command() {
    return cli::Command{"cpu_key",
                        "make a console CPU key and show its unique-id and ECC bits",
                        {},
                        [](const cli::Args&) {
                            cb::out_text(report(xecrypt::cpu_key_generate()));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::cpu_key
