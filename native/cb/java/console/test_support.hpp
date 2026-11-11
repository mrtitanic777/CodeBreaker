// cb/java/console/test_support.hpp -- what the console lane's tests share (included by *_test.cpp only).
#pragma once

#include <filesystem>
#include <algorithm>
#include <optional>
#include <vector>
#include <string>
#include <string_view>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// CB_CHECK_EQ for two paths (the test harness cannot print a std::filesystem::path): they are compared as the
// text the program would show.
#define CB_CHECK_PATH(actual, expected) CB_CHECK_EQ(::cb::fs::to_text(actual), ::cb::fs::to_text(expected))

namespace cb::java::console::test {

// A file of testdata/ next to the console lane's sources.
inline std::filesystem::path data(std::string_view relative) {
    return testing::data_path(std::string("java/console/testdata/") + std::string(relative));
}

// The text with every line end a plain "\n" (goldens are written with the Windows ones).
inline std::string lf(std::string_view s) { return text::replace_all(text::replace_all(s, "\r\n", "\n"), "\r", "\n"); }

inline std::string golden(std::string_view name) { return lf(fs::read_text(data(name))); }

// The files below a folder as relative '/'-paths in the order the goldens list them: a folder's files by name, then its
// folders (by name), each the same way.
inline void files_in_walk_order(const std::filesystem::path& root, const std::filesystem::path& dir, std::vector<std::string>& out) {
    std::vector<std::string> files, dirs;
    for (const auto& e : std::filesystem::directory_iterator(dir))
        (e.is_directory() ? dirs : files).push_back(fs::to_text(e.path().filename()));
    std::sort(files.begin(), files.end());
    std::sort(dirs.begin(), dirs.end());
    for (const std::string& f : files) out.push_back(fs::to_text((dir / f).lexically_relative(root)));
    for (const std::string& d : dirs) files_in_walk_order(root, dir / d, out);
}

// A path as the goldens write it: relative to `base`, forward slashes, "-" for none.
inline std::string rel(const std::optional<std::filesystem::path>& p, const std::filesystem::path& base) {
    return p ? fs::to_text(p->lexically_relative(base)) : std::string("-");
}

}  // namespace cb::java::console::test
