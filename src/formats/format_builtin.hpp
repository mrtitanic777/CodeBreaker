// formats/format_builtin.hpp -- the descriptions of config/formats/*.fmt built into the program (CMake writes them into the
// executable at configure time: src/formats/embed_formats.cmake), so the readers that run on the format engine need no
// file beside the program.
#pragma once

#include "formats/format_engine.hpp"

namespace cb::formats::engine {

// Every description, parsed once.
const Registry& builtin();

// Read `data` as `format` through the built-in descriptions. FormatError / TruncatedError when it is not one.
Tree read_builtin(std::string_view format, ByteView data);

}  // namespace cb::formats::engine
