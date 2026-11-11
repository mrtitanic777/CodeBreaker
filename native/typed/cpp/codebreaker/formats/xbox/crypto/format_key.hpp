// codebreaker/formats/xbox/crypto/format_key.hpp -- SHIM over cb/formats/xbox/crypto/format_key.hpp (docs/CPP_DESIGN.md 6.3);
// deleted with its last py:: consumer. main() writes Keys/custom_pub.bin as Output/Zero/ExpPubKey.h (relative to the current
// folder) and prints the source.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/xbox/crypto/format_key.hpp"

namespace codebreaker::formats::xbox::crypto::format_key {

inline void main() {
    cb::compat::guard([&] { py::print(cb::compat::to_py(cb::formats::xbox::crypto::format_key::write_exp_pub_key("."))); });
}

}  // namespace codebreaker::formats::xbox::crypto::format_key
