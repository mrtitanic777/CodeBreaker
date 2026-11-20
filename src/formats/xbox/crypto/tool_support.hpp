// formats/xbox/crypto/tool_support.hpp -- the few helpers the small tools of the `crypto` command share.
#pragma once

#include <string>

#include "core/bytes.hpp"
#include "core/crypto.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::crypto::support {

// Upper-case hex, the way the console tools print keys and hashes.
inline std::string hex_upper(ByteView data) { return text::upper(cb::crypto::to_hex(data)); }

}  // namespace cb::formats::xbox::crypto::support
