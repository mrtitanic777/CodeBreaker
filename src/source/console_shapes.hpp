// source/console_shapes.hpp -- inside the console writer (console_writer.hpp): the shapes a description names, each in its own file.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "source/console_writer.hpp"

namespace cb::source::console_writer {

// The `banked` shape (console_banked.cpp). `head` is the head comment's lines, said already.
image_writer::Written write_banked(const Description& d, const Loaded& program, const std::vector<std::string>& head, const Values& values,
                                   std::string_view file_name, const std::filesystem::path& folder);

}  // namespace cb::source::console_writer
