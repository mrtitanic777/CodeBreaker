// codebreaker/formats/xbox/crypto/format_key.hpp -- C++ port of codebreaker.formats.xbox.crypto.format_key.
//
// format_key.py -- writes Keys/custom_pub.bin as the C++ source of the byte array ExpPubKey
// (Output/Zero/ExpPubKey.h) and prints that source. It uses bin2lang.lang_format; the paths are relative to
// the current directory, as in the Python.
//
// Typing notes: none needed -- main() takes and returns nothing. It raises what bin2lang.lang_format raises
// (FileNotFoundError for a missing input or output folder, IndexError for an empty input).
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::format_key {

void main();

}  // namespace codebreaker::formats::xbox::crypto::format_key
