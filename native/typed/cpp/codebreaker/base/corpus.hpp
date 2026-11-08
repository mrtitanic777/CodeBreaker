// codebreaker/base/corpus.hpp -- typed C++ of codebreaker.base.corpus
//
// corpus.py -- the ONE resolver for the retail binary corpus.
//
// Finding "the retail kernel32.dll" used to be re-implemented per tool, and the copies disagreed:
// one took the first file an os.walk happened to reach, another preferred WINDOWS\SYSTEM, and two
// fell back to a hardcoded absolute path. The corpus holds several copies of many modules (the
// installed one, the setup one, per-driver variants), so two tools could build against one binary
// and verify against another. Everything resolves here now:
//
//     namespace corpus = codebreaker::base::corpus;
//     corpus::root()                  // the corpus directory
//     corpus::retail("kernel32")      // THE retail binary for a module name (installed copy first)
//     corpus::candidates("lzexpand.dll")   // every copy, best first
//     corpus::lane(path)              // 'pe' | 'ne' | 'le' | 'dos' | None, from the file header
//     corpus::modules("ne")           // one path per module name in a lane, smallest first
//
// There is more than one product. settings.json lists them under "corpora" ({name: folder}); one
// is selected at a time and everything above answers for that one. Nothing selected means the
// product settings.json names as "target", found exactly as it always was.
//
//     corpus::names()                 // every product there is a folder for
//     corpus::current()               // the selected one's name
//     corpus::use("win98")            // select one (for this process and the workers it starts)
//     { corpus::selected s("win3.1"); corpus::retail("user.exe"); }       // for a moment
//     corpus::root("win98")           // a product's folder without selecting it
//
// The environment variable CB_CORPUS_NAME selects one for a whole run, and so does the global
// option `--corpus NAME` (take_option). Each product's directory walk happens once per process
// and is cached.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `with corpus.selected(name):` is the class `selected`: constructed it selects, destroyed it restores.
//   * settings come from py::settings (target, corpus, state, reference); the settings module's other
//     two uses here, settings.json's "corpora" entry (settings._load()["corpora"]) and the checkout root
//     (settings.repo_root() / ROOT), are read from CB_CHECKOUT_ROOT/settings.json, as py::settings does.
//   * retail()'s _readable() hands a container to its platform: a .xex to formats::xex::basefile (ported),
//     an .xbe to formats.xbox.xbe.to_pe, which has no C++ port yet: it is the std::function
//     `xbe_to_pe` below (unset: an XBE file is a RuntimeError saying so).
//   * Paths and names are py::str; "path or None" is std::optional<py::str>.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::base::corpus {

extern const py::list<py::str> EXTS_;        // _EXTS: the extensions candidates() tries for a bare name
extern const py::str ENV;                    // selects a product for a whole run; use() sets it for workers

// formats.xbox.xbe.to_pe: an original Xbox .xbe as the PE image the stages read (no C++ port of xbe.py yet)
extern std::function<py::str(const py::str&)> xbe_to_pe;

// Every product that has a folder of original binaries, by name.
py::list<py::str> names();

// The selected product's name.
py::str current();

// Select a product: every function here then answers for it, in this process and in the
// worker processes it starts. None goes back to the default. -> the name selected before
py::str use(const std::optional<py::str>& name);

// `with corpus.selected("win98"): ...` -- another product for the length of a block.
class selected {
public:
    explicit selected(const std::optional<py::str>& name);
    selected(const selected&) = delete;
    selected& operator=(const selected&) = delete;
    ~selected();

private:
    std::optional<py::str> was_var_;
    std::optional<py::str> was_env_;
};

// Act on a `--corpus NAME` in a command line and return the line without it.
py::list<py::str> take_option(const py::list<py::str>& argv);

// The folder of original binaries: of the product `name`, else of the selected one. For
// the default product that is CB_CORPUS_ROOT when set; otherwise the sibling Windows95
// repo's design/corpus/files, then WT_DATA_ROOT when it is a directory.
py::str root(const std::optional<py::str>& name = std::nullopt);

// The Win95 'design' arena that owns the corpus (.../Windows95/design). A product other than
// the default has no such arena beside its binaries: what is derived for it is kept in the
// state folder, under products/<name>/design.
py::str design_dir();

// A path inside the period SDK and DDK folders (settings "reference", else the folder of
// that name in the design arena): reference({"Microsoft Windows 95 DDK", "INC32"}).
py::str reference(const py::list<py::str>& parts = py::list<py::str>());

// The name of the product whose folder holds `path` (the deepest one), or None.
std::optional<py::str> owner(const py::str& path);

// Executable class of a file by its header: 'pe', 'ne', 'le' (VxD), 'dos', or None when it
// is not an MZ executable.
std::optional<py::str> lane(const py::str& path);

// Sort key putting the copy the OS actually loads first: WINDOWS\SYSTEM, then WINDOWS, then
// anything else; ties broken by shorter path, then name, so the choice is deterministic.
std::tuple<int64_t, int64_t, py::str> _rank(const py::str& path);

// {basename_lower: [paths, best first]} for the whole corpus (walked once, cached).
py::dict<py::str, py::list<py::str>> index(bool refresh = false);

// Windows 1.x keeps its system modules (KERNEL, GDI, USER, the drivers) inside one boot
// image, a .BIN with an .OVL beside it, not in files of their own. Each is written out once
// as an ordinary executable under the state folder and listed as <module>.exe, so a module
// of such a product is found, read and reconstructed like any other.
void _unpack_boot_images(const py::dict<py::str, py::list<py::str>>& idx);

// Every corpus copy of a module, best first. A bare name tries the usual extensions.
py::list<py::str> candidates(const py::str& name);

// The file a module is named by: a path as it stands, otherwise the corpus's copy of that
// module name.  ValueError when there is none.
py::str resolve(const py::str& name);

// THE retail binary for a module name, or None. An existing path is returned unchanged, so
// callers can pass either a name or a path -- except an Xbox 360 .xex, which is handed over as
// the PowerPC image inside it (formats/xex.py), as Windows 1.x boot images are.
std::optional<py::str> retail(const py::str& name);

// The file the stages read: a container's image (a .xex: its basefile; an .xbe: its PE
// image), else the file.
py::str _readable(const py::str& path);

// One path per module name (its retail() copy), optionally restricted to a lane; smallest
// file first so a sweep gives early feedback.
py::list<py::str> modules(const std::optional<py::str>& want_lane = std::nullopt,
                          const py::list<py::str>& exts = py::list<py::str>{py::str(".dll"), py::str(".exe"), py::str(".drv"),
                                                                            py::str(".ocx"), py::str(".cpl")});

// [(path, kind)] -- every file of the selected product that is an executable, whatever it
// is called (fonts, drivers, overlays): kind is 'pe' | 'ne' | 'le' | 'dos'. Every copy.
py::list<std::tuple<py::str, py::str>> executables();

// sha256 of a module file, memoized by (path, mtime, size) so a stable corpus binary is hashed
// once per process.  None when the file cannot be read.
std::optional<py::str> sha(const py::str& path);

}  // namespace codebreaker::base::corpus
