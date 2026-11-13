// cb/formats/xbox/crypto/cpu_key.hpp -- the `crypto cpu_key` tool: make a console CPU key and show how it divides.
//
// A CPU key is 128 bits: the first 102 are the unique id, the last 26 the ECC that protects it. `crypto cpu_key` makes a valid one (the
// rules of XeCrypt's CpuKeyGen) and prints the two parts as bit strings, then the key in hex.
#pragma once

#include <string>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::cpu_key {

inline constexpr std::size_t kEccBits = 26;

// The report for a 16-byte key: "ECC Bits: <26 bits>", "UID Bits: <102 bits>" and the key in upper-case hex, a line each. The bits are
// each byte's, most significant first, in the key's order. UsageError for a key that is not 16 bytes.
std::string report(ByteView key);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::cpu_key
