// cb/formats/xbox/crypto/kv_tool.hpp -- make the files a console image needs from a keyvault: its eFuse dump and the keyvault encrypted with
// the console's CPU key.
//
//     kv_tool::Output out = kv_tool::make(vault, cpu_key, dvd_key, kv_tool::ConsoleType::Devkit);
//     out.fuses;   // 96 bytes, twelve fuse lines
//     out.keyvault;  // the vault with its DVD key set, sealed under the CPU key
//
// The eFuse dump has line 0 = C0FFFFFFFFFFFFFF, the console-type bytes at 0xE (0F0F test kit F00F; devkit 0F0F; retail phat 0FF0; retail slim
// F0F0, after 0F0F0F0F0F0F at 8), the CPU key in lines 3..6 (each half twice) and 0xF0 at 0x38. The keyvault's DVD key is the 16 bytes at 0x100.
//
// The command `crypto kv_tool` takes the vault (default KV/banned.bin), a CPU key (a new one by default), a DVD key (zeros), a console type
// (devkit) and writes `fuses.bin` and `kv_enc.bin` into an output folder (default Output/Zero).
#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::kv_tool {

enum class ConsoleType { RetailPhat, RetailSlim, TestKit, Devkit };

struct Output {
    Bytes fuses;
    Bytes keyvault;
};

// UsageError for a CPU key that is not valid, a DVD key that is not 16 bytes or a vault that is not 0x4000 bytes.
Output make(ByteView vault, ByteView cpu_key, ByteView dvd_key, ConsoleType type, xecrypt::RandomSource& random = xecrypt::os_random());

cli::Command command();

}  // namespace cb::formats::xbox::crypto::kv_tool
