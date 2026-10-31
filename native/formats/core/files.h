// files.h -- reading a file by its UTF-8 path, for the few questions that take a path (they read
// only part of a file, as their Python did).
#pragma once

#include <cstdint>
#include <string>

namespace cbf {

// `size` bytes from `offset` (fewer at the end of the file).  False when the file cannot be read.
bool read_part(const std::string &path_utf8, uint64_t offset, uint64_t size, std::string &out);
// the whole file
bool read_file(const std::string &path_utf8, std::string &out);

}  // namespace cbf
