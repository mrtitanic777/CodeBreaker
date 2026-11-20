// formats/xbox/crypto/lzx_dll.hpp -- the LZX codec of the Xbox 360 bootloaders (the SE's compressed hypervisor and kernel), served by upstream's
// LZXCompression.dll and LZXDecompression.dll.
//
//     std::shared_ptr<build_lib::LzxCodec> codec = lzx_dll::load(bin_dir);      // null when the DLLs are not there
//     Bytes packed = codec->compress_continuous(data);
//
// The compression is the Windows LCI/LDI interface of the LZX library (window 128 KB, 32 KB chunks, 32 KB second partition) which hands each
// compressed chunk to a callback; the codec joins the chunks as the console's loader reads them: for each chunk a header of the compressed
// and the uncompressed size, both big-endian 16-bit, and the compressed bytes. The DLLs are not part of this program; they are the LZX/Windows
// folder of upstream's bin/ (the setting `xbox360_crypto_bin`).
//
// The command `crypto lzx [--input FILE]` is the library's self-test: compress a file, decompress it again and check it is what it started as.
//
// Errors: ToolError when a call of the library fails (its code is in the message); FormatError for a chunk list that is cut short.
#pragma once

#include <filesystem>
#include <memory>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xbox/crypto/build_lib.hpp"

namespace cb::formats::xbox::crypto::lzx_dll {

inline constexpr std::size_t kWindowSize = 128 * 1024;
inline constexpr std::size_t kChunkSize = 32 * 1024;
inline constexpr std::size_t kMaxGrowth = 0x1800;      // what a chunk may grow by when it does not compress

// The codec over `<bin_dir>/LZX/Windows/LZXCompression.dll` and `LZXDecompression.dll`; nullptr when either is not there. IoError when they will not load.
std::shared_ptr<build_lib::LzxCodec> load(const std::filesystem::path& bin_dir);

// The same codec, loaded when it is first used (a command that never compresses never loads the DLLs); UnsupportedError at the use when they are not there.
std::shared_ptr<build_lib::LzxCodec> lazy(const std::filesystem::path& bin_dir);

cli::Command command(const std::filesystem::path& bin_dir);

}  // namespace cb::formats::xbox::crypto::lzx_dll
