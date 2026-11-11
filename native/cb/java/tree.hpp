// cb/java/tree.hpp -- listing the files of a source tree, in an order that does not depend on the file system.
//
// Several passes of the Java lane scan a converted or decompiled tree (the `.java` corpus, the emitted `.cpp`/`.h`
// folder) and either read facts from it or rewrite files in place. What they find, and what a report lists, must not
// change with the directory's on-disk order, so this is the one place that walks a tree: every directory's files
// first, in name order, then its sub-directories, in name order, each descended in turn (the order of a top-down
// os.walk over sorted names). A directory that cannot be read is skipped, and so is a symbolic link to a
// directory (it is not descended into).
#pragma once

#include <filesystem>
#include <initializer_list>
#include <string_view>
#include <vector>

namespace cb::java::tree {

// Every regular file under `root`, recursively. A missing or non-directory root gives an empty list.
std::vector<std::filesystem::path> files(const std::filesystem::path& root);

// The same, keeping only files whose name ends with one of `extensions` (each with its dot: ".cpp"). The match is
// case-sensitive, as the Java toolchain writes these names.
std::vector<std::filesystem::path> files_with_extension(const std::filesystem::path& root,
                                                        std::initializer_list<std::string_view> extensions);

}  // namespace cb::java::tree
