// cb/formats/xbox/crypto/xell_unpack.hpp -- take apart an XeLL image (the Free60 boot loader, 256 KB): the first stage, and the gzip-compressed
// second stage compressed and decompressed.
//
//     xell_unpack::Parts parts = xell_unpack::split(xell_bytes);
//     parts.stage1;               // the first 16 KB
//     parts.stage2_compressed;    // the gzip stream that follows it
//     parts.stage2;               // that stream inflated (an ELF)
//
// The image is the 16 KB first stage, the second stage as a gzip stream (the slice of 0x3BF5E bytes after the first stage; the stream's own header and
// footer lie in it), and a 16-byte footer. The command `crypto xell_unpack [--dir XeLL] [names...]` reads <dir>/<name>.bin (xell-1f, xell-2f and
// xell-gggggg by default) and writes <name>-stage-1.bin, <name>-stage-2-compressed.elf and <name>-stage-2-decompressed.elf beside it.
//
// Errors: FormatError for an image too short for its stages or a second stage that is not a whole gzip stream.
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::xell_unpack {

inline constexpr std::size_t kImageSize = 256 * 1024;
inline constexpr std::size_t kFooterSize = 16;
inline constexpr std::size_t kStage1Size = 16 * 1024;
inline constexpr std::size_t kGzipOverhead = 10 + 8;     // header and footer of the stream
inline constexpr std::size_t kStage2Size = kImageSize - kStage1Size - kFooterSize - kGzipOverhead;

struct Parts {
    Bytes stage1;
    Bytes stage2_compressed;
    Bytes stage2;
};

Parts split(ByteView image);

// A gzip stream inflated. FormatError when it is damaged or ends early; bytes after the stream (the image's padding) are ignored.
Bytes gunzip(ByteView stream);

// The files of `names` unpacked beside themselves in `folder`; the files written, in order.
std::vector<std::filesystem::path> unpack_files(const std::filesystem::path& folder, const std::vector<std::string>& names);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::xell_unpack
