// codebreaker/formats/xbox/crypto/format_key.cpp -- C++ port of codebreaker.formats.xbox.crypto.format_key.

#include "codebreaker/formats/xbox/crypto/format_key.hpp"

#include <optional>

#include "codebreaker/formats/xbox/crypto/bin2lang.hpp"

namespace codebreaker::formats::xbox::crypto::format_key {

void main() {
    namespace b2l = codebreaker::formats::xbox::crypto::bin2lang;
    py::print(b2l::lang_format(py::str("Keys/custom_pub.bin"), std::optional<py::str>(py::str("Output/Zero/ExpPubKey.h")),
                               b2l::Language::CPLUSPLUS, py::str("ExpPubKey")));
}

}  // namespace codebreaker::formats::xbox::crypto::format_key
