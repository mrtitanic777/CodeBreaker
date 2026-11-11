// cb/source/placement.hpp -- where a module's source goes in the source tree.
//
//     Placement where(tree_root, load_table("placement.json", corpus.current()));
//     where.resolve("comctl32.dll", path, lane)     { "shell/comctl32", How::Known }
//     where.folder("comctl32.dll", path, lane)      the folder itself (created on demand)
//
// There is one source tree per product: the folder that holds the corpus's `design` folder. A module's source lives in
// <tree>/<component>/<module>/. Where a module already has a folder anywhere in the tree, that is where it goes --
// someone put it there. Otherwise the component is worked out, most certain first:
//
//     known      the module is named in the product's table (the system's core modules)
//     kind       what the file is settles it: a virtual device driver, a real-mode program
//     installed  the folder it is installed in settles it (I/O subsystem drivers, COMMAND)
//     suffix     its extension does (.acm, .cpl, .scr, .drv)
//     prefix     its name starts like a family the table knows (mci*, ras*, ole*, ...)
//     unsorted   nothing above applies
//
// The tables are per product, in placement.json. `place_command` is the `codebreaker place` command.
#pragma once

#include <filesystem>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::source::placement {

// How a folder was decided.
enum class How { AlreadyThere, Known, Kind, Installed, Suffix, Prefix, Unsorted };
std::string_view how_name(How how);   // "already there" "known" "kind" "installed" "suffix" "prefix" "unsorted"

struct Decision {
    std::string folder;   // relative to the tree, '/'-separated: "shell/comctl32"
    How how = How::Unsorted;
};

// One product's table of placement.json. Each part names a component folder ("base", "mmedia", "vmm32").
struct ProductTable {
    std::unordered_map<std::string, std::string> known;    // module stem -> component folder (lookup only)
    std::unordered_map<std::string, std::string> kind;     // lane name ("dos", "le") -> component (lookup only)
    OrderedMap<std::string, std::string> installed;        // part of an install path -> component; the file's order
                                                           // is the order tried, so the first match wins
    std::unordered_map<std::string, std::string> suffix;   // ".drv" -> component (lookup only)
    std::vector<std::pair<std::string, std::string>> prefix;   // name prefix -> component; the file's order, first wins
};

// The table of `product` in placement.json. A product the file does not know has the empty table (everything is
// "unsorted" except what is already in the tree). IoError if the file cannot be read; ParseError if it is not JSON;
// FormatError if an entry is not the shape above.
ProductTable load_table(const std::filesystem::path& file, std::string_view product);

class Placement {
public:
    // `tree_root` is the folder that holds the product's `design` folder (corpus.design_dir().parent_path()).
    Placement(std::filesystem::path tree_root, ProductTable table);

    const std::filesystem::path& tree_root() const { return tree_root_; }

    // {module stem (lower case): folder relative to the tree} for every module folder already in the tree: two
    // levels down, skipping `design`, `build`, `tools`, `inc` and anything starting with '.'. When two components
    // hold the same name the one sorted first wins. Looked at once per Placement.
    const std::unordered_map<std::string, std::string>& existing() const;

    // The folder for `module` (a file name; its extension is dropped). `file` is where the module is installed or found
    // (empty when not known: then only its name decides); `lane` is the class of that file, when it is an executable.
    Decision resolve(std::string_view module, const std::filesystem::path& file = {},
                     std::optional<base::Lane> lane = std::nullopt) const;

    // The folder a module's source goes in, absolute; created when `create`.
    std::filesystem::path folder(std::string_view module, const std::filesystem::path& file = {},
                                 std::optional<base::Lane> lane = std::nullopt, bool create = true) const;

private:
    std::filesystem::path tree_root_;
    ProductTable table_;
    mutable std::once_flag existing_once_;
    mutable std::unordered_map<std::string, std::string> existing_;
};

// `codebreaker place <module> ... | --all [--list]`: say where each module's source goes, and how that was decided.
// Writes nothing but the report. `--all` covers the corpus's PE, NE, VxD and real-mode modules; without `--list` it
// prints only the summary. Returns the exit code (cb::kExitUsage with no module).
int place_command(const Placement& placement, const base::Corpus& corpus, std::span<const std::string> args);

}  // namespace cb::source::placement
