// cb/java/console/tree.hpp -- a top-down walk of a folder tree in a fixed order.
//
// The console lane scans decompiled source trees, SDK include trees and converted output folders for files of
// a kind. They all want the same walk: folders top-down, each folder's files before its subfolders, names in
// a fixed (case-insensitive) order so what is found first does not depend on the file system, some folder
// names skipped, and sometimes a depth limit.
#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::java::console::tree {

// One folder of the walk: its path and the names directly inside it.
struct Listing {
    std::filesystem::path dir;
    std::vector<std::string> subdirs;   // in walk order; already without the skipped ones
    std::vector<std::string> files;
};

struct WalkOptions {
    // Folders deeper than this are not entered (the root is depth 0; a folder at the limit is listed, with no
    // subfolders). Unlimited when empty.
    std::optional<std::size_t> max_depth;
    // A subfolder, by its name, that is left out of the walk (and of Listing::subdirs).
    std::function<bool(std::string_view)> skip_dir;
};

// Call `visit` for the root and every folder below it. The walk ends early when `visit` returns false. A root
// that is not a folder, or a folder that cannot be listed, contributes nothing (the callers ask "is there a
// jar here?", for which a missing folder is just "no"). Symbolic links to folders are listed but not entered.
void walk(const std::filesystem::path& root, const WalkOptions& options,
          const std::function<bool(const Listing&)>& visit);

// Every file below `root` (see walk) as full paths, in walk order.
std::vector<std::filesystem::path> files_below(const std::filesystem::path& root, const WalkOptions& options = {});

}  // namespace cb::java::console::tree
