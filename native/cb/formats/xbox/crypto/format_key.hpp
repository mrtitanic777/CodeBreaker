// cb/formats/xbox/crypto/format_key.hpp -- the custom public key as C++ source: Keys/custom_pub.bin written as the byte
// array `ExpPubKey` of Output/Zero/ExpPubKey.h, which the hypervisor-patch build includes.
//
// The array is the C/C++ language of the bin2lang embedding tool: a guarded `typedef` of BYTE, an include guard named
// after the variable, and the bytes in hex, 16 to a line. The paths are relative to a base folder (the current folder
// for the tool).
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::crypto::format_key {

inline constexpr std::string_view kKeyFile = "Keys/custom_pub.bin";
inline constexpr std::string_view kHeaderFile = "Output/Zero/ExpPubKey.h";
inline constexpr std::string_view kVariable = "ExpPubKey";

// `data` as the source of `BYTE <variable>[] = { ... };`, `per_line` bytes to a line (at least 1), no text after the
// closing `#endif` but its newline. FormatError for no data (there is no empty array to write).
std::string byte_array_source(ByteView data, std::string_view variable = kVariable, std::size_t per_line = 16);

// Read <base>/Keys/custom_pub.bin, write <base>/Output/Zero/ExpPubKey.h (folders are created) and return the source.
// NotFoundError if the key file is missing, FormatError if it is empty.
std::string write_exp_pub_key(const std::filesystem::path& base = ".");

}  // namespace cb::formats::xbox::crypto::format_key
