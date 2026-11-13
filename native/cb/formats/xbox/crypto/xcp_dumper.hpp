// cb/formats/xbox/crypto/xcp_dumper.hpp -- decrypt an Xbox 360 XCP package and turn it into a LIVE package.
//
//     std::filesystem::path live = xcp_dumper::unpack(xcp_file, key, {});     // XCP_FILE.XCP -> XCP_FILE (a LIVE package beside it)
//
// An XCP is a Microsoft cabinet (MSCF) whose headers, folder records and folder data are RC4-encrypted: each encrypted piece is introduced by
// a 28-byte record (a 20-byte checksum and an 8-byte confounder) from whose HMAC-SHA1 under the key the RC4 key is made; the confounder is
// decrypted first and thrown away. The pieces:
//     the cabinet header   0x60 bytes at 0, its record at 0x60
//     the folder records   cabinets' folders at 0x180, 0x24 bytes each (8 and the 0x1C of "xenon data"), their record at 0x28
//     the file entries     from the file table on: the 16-byte entry and its NUL-ended name, one RC4 stream after the record at 0x44
//     each folder's data   its record follows its 8-byte folder record
// Everything is decrypted in place, and each file's name is then overwritten with its index ("000", "001" ...) in its last three characters so
// the extracted files sort in order. The cabinet is then extracted, its files joined in name order, and "LIVE" written over the first four bytes.
// A cabinet that is not encrypted is renamed and extracted all the same.
//
// The input file is CONSUMED: it is changed in place and removed at the end (a backup is kept beside it as <name>.bak unless told not to).
//
// Errors: UsageError for an encrypted package and no key, FormatError for a key that does not give a cabinet, ToolError... for no cabinet extractor.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/crypto/stream_io.hpp"

namespace cb::formats::xbox::crypto::xcp_dumper {

inline constexpr std::uint32_t kCabMagic = 0x4643534D;           // "MSCF" read little-endian
inline constexpr std::uint32_t kCabMagicSwapped = 0x4D534346;    // and the other order, which the tool accepts too

struct Options {
    bool backup = true;      // keep <name>.bak beside the input
};

// Whether the package's cabinet header is in the clear (it starts with MSCF).
bool is_encrypted(ByteView head);

// Decrypt (when it is encrypted) and rename the file entries of an XCP held in `stream`, in place. FormatError when the key does not give a
// cabinet; UsageError for an encrypted package and no key. Returns whether it was encrypted.
bool decrypt_in_place(StreamIO& stream, const std::optional<Bytes>& key);

// The whole job on a file: the LIVE package is written beside it (named after it, in capitals) and its path answered.
std::filesystem::path unpack(const std::filesystem::path& xcp, const std::optional<Bytes>& key, const Options& options);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::xcp_dumper
