// cb/base/corpus.hpp -- the ONE resolver for the retail binary corpus.
//
// Finding "the retail kernel32.dll" used to be re-implemented per tool, and the copies disagreed: one took the first
// file a directory walk happened to reach, another preferred WINDOWS\SYSTEM, and two fell back to a hardcoded absolute
// path. The corpus holds several copies of many modules (the installed one, the setup one, per-driver variants), so
// two tools could build against one binary and verify against another. Everything resolves here now:
//
//     Corpus corpus(products, probes);
//     corpus.root()                    the corpus folder
//     corpus.retail("kernel32")        THE retail binary for a module name (installed copy first)
//     corpus.candidates("lzexpand.dll")   every copy, best first
//     corpus.lane(path)                Pe | Ne | Le | Dos, from the file header (nullopt: not an executable)
//     corpus.modules(Lane::Ne)         one path per module name in a lane, smallest first
//
// There is more than one product. The configuration lists them ({name: folder}); one is selected at a time and
// everything above answers for that one. Nothing selected means the product the configuration names as its target.
//
//     corpus.names()                   every product there is a folder for
//     corpus.current()                 the selected one's name
//     corpus.use("win98")              select one (for this process and the workers it starts)
//     { Corpus::Selected s(corpus, "win3.1"); corpus.retail("user.exe"); }    for a moment
//     corpus.root("win98")             a product's folder without selecting it
//
// The environment variable CB_CORPUS_NAME selects one for a whole run, and so does the global option `--corpus NAME`
// (take_option). Each product's directory walk happens once per Corpus and is cached.
//
// What a file IS (its executable class, the image inside a container, the modules inside a Windows 1.x boot image) is
// the formats' knowledge, not the corpus's: the Corpus asks the FormatProbes it was given. Left empty a probe does
// nothing (no file is an executable, no container is unwrapped, no boot image is unpacked).
#pragma once
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::base {

// The executable classes of the corpus: 32-bit PE, 16-bit NE, LE (VxD drivers), plain DOS.
enum class Lane { Pe, Ne, Le, Dos };
std::string_view lane_name(Lane lane);                       // "pe" "ne" "le" "dos"
std::optional<Lane> lane_from_name(std::string_view name);   // nullopt for any other text

// What the configuration says about the products. Folders are absolute (the configuration loader made them so).
struct Products {
    std::string target;                                   // the product used when none is selected ("win95")
    std::filesystem::path target_folder;                  // setting "corpus": the target's folder when `listed` lacks it
    std::map<std::string, std::filesystem::path> listed;  // setting "corpora": {name: folder of original binaries}
    std::filesystem::path checkout_root;                  // the CodeBreaker checkout (the sibling Windows95 repo is beside it)
    std::filesystem::path state;                          // the state folder
    std::optional<std::filesystem::path> reference;       // setting "reference": the period SDK and DDK folders
    friend bool operator==(const Products&, const Products&) = default;
};

// One module of a Windows 1.x boot image, as a standalone executable.
struct BootModule {
    std::string name;   // "kernel", "gdi", ...
    Bytes executable;
};

struct FormatProbes {
    // The class of an executable file by its header; nullopt when it is not an MZ executable.
    std::function<std::optional<Lane>(const std::filesystem::path&)> lane;
    // The file the stages read: a container's image (an .xex: its PowerPC basefile; an .xbe: its PE image), else the
    // file itself.
    std::function<std::filesystem::path(const std::filesystem::path&)> readable;
    // Windows 1.x keeps its system modules inside one boot image (a .BIN with an .OVL beside it). Reads the modules
    // out; throws FormatError or IoError when the pair is not a boot image (it is then left alone).
    std::function<std::vector<BootModule>(const std::filesystem::path& image, const std::filesystem::path& overlay)>
        boot_image_modules;
};

class Corpus {
public:
    explicit Corpus(Products products, FormatProbes probes = {});
    Corpus(const Corpus&) = delete;
    Corpus& operator=(const Corpus&) = delete;

    // ---- the products ----
    // Every product that has a folder of original binaries, sorted by name.
    std::vector<std::string> names() const;
    // The selected product's name (the selection, else CB_CORPUS_NAME, else the target).
    std::string current() const;
    // Select a product (UsageError for a name that is not one), in this process and, through CB_CORPUS_NAME, the workers
    // it starts. nullopt goes back to the default. Returns the name selected before.
    std::string use(const std::optional<std::string>& name);
    // Another product for the length of a scope.
    class Selected {
    public:
        Selected(Corpus& corpus, const std::optional<std::string>& name);
        Selected(const Selected&) = delete;
        Selected& operator=(const Selected&) = delete;
        ~Selected();

    private:
        Corpus& corpus_;
        std::optional<std::string> was_selected_;
        std::optional<std::string> was_env_;
    };
    // Acts on a `--corpus NAME` in a command line and returns the line without it (UsageError when NAME is missing).
    std::vector<std::string> take_option(std::vector<std::string> args);

    // ---- where things are ----
    // The folder of original binaries: of the product `name`, else of the selected one. For the default product that
    // is CB_CORPUS_ROOT when set; otherwise the sibling Windows95 repo's design/corpus/files, then WT_DATA_ROOT when it
    // is a folder. UsageError for a product that is not listed.
    std::filesystem::path root(const std::optional<std::string>& name = std::nullopt) const;
    // The Win95 'design' arena that owns the corpus (.../Windows95/design). A product other than the default has no
    // such arena beside its binaries: what is derived for it is kept in the state folder, under products/<name>/design.
    std::filesystem::path design_dir() const;
    // A path inside the period SDK and DDK folders (the "reference" setting, else the folder of that name in the design
    // arena): reference({"Microsoft Windows 95 DDK", "INC32"}).
    std::filesystem::path reference(const std::vector<std::string>& parts = {}) const;
    // The name of the product whose folder holds `path` (the deepest one), if any.
    std::optional<std::string> owner(const std::filesystem::path& path) const;

    // ---- finding a module ----
    // Executable class of a file by its header, remembered per path.
    std::optional<Lane> lane(const std::filesystem::path& path) const;
    // {basename lowercase: every copy, best first} for the whole corpus: walked once and cached. The best copy is the
    // one the OS actually loads: WINDOWS\SYSTEM, then WINDOWS, then anything else; ties broken by shorter path, then
    // name, so the choice is deterministic.
    using Index = std::unordered_map<std::string, std::vector<std::filesystem::path>>;
    std::shared_ptr<const Index> index(bool refresh = false) const;
    // Every corpus copy of a module, best first. A bare name tries the usual extensions.
    std::vector<std::filesystem::path> candidates(std::string_view name) const;
    // THE retail binary for a module name; nullopt when the corpus has none. An existing path is returned as it is
    // (through readable()), so callers can pass either a name or a path.
    std::optional<std::filesystem::path> retail(std::string_view name) const;
    // retail(), or NotFoundError.
    std::filesystem::path resolve(std::string_view name) const;
    // One path per module name (its best copy), optionally restricted to a lane; smallest file first so a sweep gives
    // early feedback.
    std::vector<std::filesystem::path> modules(std::optional<Lane> only = std::nullopt,
                                               const std::vector<std::string>& extensions = {".dll", ".exe", ".drv", ".ocx", ".cpl"}) const;
    struct Executable {
        std::filesystem::path path;
        Lane lane;
    };
    // Every file of the selected product that is an executable, whatever it is called (fonts, drivers, overlays),
    // every copy, sorted by path.
    std::vector<Executable> executables() const;
    // sha256 (lowercase hex) of a module file, remembered by (path, mtime, size) so a stable corpus binary is hashed
    // once; nullopt when the file cannot be read.
    std::optional<std::string> sha(const std::filesystem::path& path) const;

private:
    std::map<std::string, std::filesystem::path> product_folders() const;
    std::filesystem::path readable(const std::filesystem::path& path) const;
    void select(const std::optional<std::string>& name);
    void unpack_boot_images(Index& index) const;

    Products products_;
    FormatProbes probes_;
    mutable std::mutex index_mutex_;                 // held while an index is built (a walk of the whole corpus)
    mutable std::mutex mutex_;                       // guards the fields below
    std::optional<std::string> selected_;
    mutable std::map<std::string, std::shared_ptr<const Index>> indexes_;   // by root
    mutable std::unordered_map<std::string, std::optional<Lane>> lanes_;    // by path
    mutable std::map<std::string, std::string> shas_;                       // by path, mtime, size
};

}  // namespace cb::base
