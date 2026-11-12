// cb/source/authorship.hpp -- whose file is it? A source writer may write its file again only where it wrote it itself.
//
// A file CodeBreaker wrote (this tool or the earlier one) carries a mark in its head -- "reconstructed source", "by CodeBreaker" -- and may
// be written again. A file anyone wrote by hand is never replaced: the writers put what they make in the state folder instead.
#pragma once

#include <filesystem>
#include <string_view>

namespace cb::source {

// Is the file at `path` one CodeBreaker wrote? A file that is not there is ours to write; one that cannot be read is not.
bool made_by_us(const std::filesystem::path& path);

// Does a file head (its first bytes) carry one of the marks?
bool carries_our_mark(std::string_view head);

// How much of a file's head is looked at.
inline constexpr std::size_t kHeadBytes = 600;

}  // namespace cb::source
