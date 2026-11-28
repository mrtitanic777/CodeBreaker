// build/resources.hpp -- a module's resources as source: `.rc` text and payload files, compiled back to `.res`.
//
//     codebreaker resources <module> [--out DIR]
//
//     Rebuilt r = rebuild(open_watcom, "comctl32.dll", outdir);   // write, compile, compare; r.res is the .res
//     r.total, r.identical, r.equivalent, r.different, r.missing
//     r.as_text, r.as_files, r.opaque, r.fallback                  // how each resource was written
//
// What is written, per kind of resource:
//
//     text      string tables, dialogs, menus, accelerator tables and version blocks are decoded
//               (formats/resources) and written as the `.rc` statements they came from
//     files     an icon or cursor group with its member images becomes a real .ico / .cur file, a bitmap a .bmp,
//               each named by an ICON / CURSOR / BITMAP statement
//     opaque    kinds with no source form here (fonts, type libraries, raw data, anything user-defined) are carried
//               as a payload file named by a raw-data statement
//
// The `.rc` is compiled with OpenWatcom's resource compiler and the result is compared with the original, resource by
// resource: IDENTICAL bytes, or EQUIVALENT (it decodes to the same thing -- the compiler pads, orders or numbers
// something differently). A resource that comes back different, or whose statement the compiler rejects, is written
// again as a payload of its original bytes ("fallback"), so the `.res` always holds the module's resources; how many
// needed that is reported, because those are not yet source.
//
// EQUIVALENT covers three things the compiler decides for itself: it numbers the member images of an icon or cursor
// group, it pads, and it stamps the build time into a version block's file date (the original's is zero).
//
// Facts about the compiler, each found by a round trip that failed:
//
//   * `-zm` makes it write the Microsoft `.res` layout (its default is a layout of its own). That file is for
//     comparing; the LINKER must be given the default layout (`Rebuilt::link_res`), because from a Microsoft-layout
//     file it binds every resource with language 0.
//   * In the Microsoft layout it does not pad bodies consistently, so entries do not always start on four-byte
//     boundaries (read_res looks for the next entry rather than computing it).
//   * A quoted type or name is taken literally: a resource called MENU is not the keyword.
//   * A CONTROL statement's style has WS_CHILD and WS_VISIBLE unless they are taken out with NOT.
//   * A string ends at a zero character, however it is written: text with one inside has no source form.
//   * Text outside Latin-1 needs a UTF-8 source (`-zku8`), and there the compiler miscounts a string table's strings
//     (UTF-8 bytes instead of characters): in a module that needs UTF-8, string tables with anything beyond ASCII are
//     carried as payloads.
//   * It writes a 16-bit version block with lengths that do not add up; 16-bit version blocks are carried as
//     payloads.
//   * It keeps only 16 bits of a control's or menu item's identifier, also in the extended templates that have room
//     for 32 (where -1 is common): those are carried as payloads.
//   * Without the preprocessor (`-zn`) a comment is a syntax error.
//   * Like every OpenWatcom tool it has to be started by its full path.
//
// A statement that cannot say what a resource holds is a routine failure, not an exception: the writers answer
// cb::Result<...> with a NoSourceForm error and the resource is carried as a payload instead.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "store/converted.hpp"
#include "toolchain/watcom.hpp"
#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "formats/resources.hpp"

namespace cb::build::resources {

namespace fr = cb::formats::resources;

using Key = fr::ResourceKey;   // (type, name, language)

// How text is written, which is also how a resource is decoded.
enum class Text {
    Narrow,    // a 16-bit module: Latin-1 text, the 16-bit templates
    Wide,      // a 32-bit module whose text fits Latin-1: written as octal escapes
    Unicode,   // a 32-bit module with text beyond Latin-1: a UTF-8 source (`-zku8`)
};

// This resource uses something the `.rc` statements written here cannot say.
class NoSourceForm : public UnsupportedError {
public:
    explicit NoSourceForm(const std::string& what) : UnsupportedError("build/resources", 0, what) {}
    CB_ERROR_BOILERPLATE(NoSourceForm)
};

// The module has text outside Latin-1: its `.rc` has to be UTF-8 (write_rc answers this by starting over in
// Text::Unicode; it is never a reason to carry one resource as a payload).
class BeyondLatin1 : public UnsupportedError {
public:
    BeyondLatin1() : UnsupportedError("build/resources", 0, "text beyond Latin-1") {}
    CB_ERROR_BOILERPLATE(BeyondLatin1)
};

// What a rebuild produced and how it compares with the original.
struct Rebuilt {
    std::optional<std::filesystem::path> rc, res;
    std::optional<std::filesystem::path> link_res;   // the .res in the layout the linker binds
    std::size_t total = 0, identical = 0, equivalent = 0;
    std::vector<Key> different, missing, extra;
    std::size_t as_text = 0, as_files = 0, opaque = 0, fallback = 0;
    std::vector<Key> not_source;                     // the resources carried as fallback payloads
    std::string error;

    // Every resource came back identical or equivalent, and nothing else came with it.
    bool same() const;
    // "12 resources: 10 identical, 2 equivalent, 0 different, 0 missing  (written as text ...)" or "FAILED: <error>"
    std::string line() const;
};

// ---- the pieces of a statement ------------------------------------------------------------------------------------

// A string literal. Control characters and everything above ASCII are written as three-digit octal escapes (a fixed
// width, so a digit after one cannot join it, and no code page is involved: the compiler takes each as one character
// of that value). That reaches Latin-1; text beyond it is BeyondLatin1 in the other modes. A 32-bit module with text
// beyond it is written in UTF-8 instead, where the compiler gets a length-counted string (`counted`: a string
// table's) wrong -- it counts UTF-8 bytes -- so such a string has no source form there. Nor has a string with a zero
// character in it anywhere: the compiler ends the string at it.
cb::Result<std::string> quoted(std::string_view text, Text mode, bool counted = false);

// The statement of a resource that has a source form: a DIALOG, MENU, ACCELERATORS or VERSIONINFO, one line each.
// NoSourceForm when the resource uses something the statement cannot say; FormatError when its bytes are damaged.
cb::Result<std::vector<std::string>> dialog_text(const fr::Resource& resource, Text mode);
cb::Result<std::vector<std::string>> menu_text(const fr::Resource& resource, Text mode);
cb::Result<std::vector<std::string>> accelerators_text(const fr::Resource& resource, Text mode);
cb::Result<std::vector<std::string>> version_text(const fr::Resource& resource, Text mode);

// ---- writing the .rc -------------------------------------------------------------------------------------------------

// A module's resources and how its text is written.
struct Module {
    std::filesystem::path path;
    std::vector<fr::Resource> resources;   // empty: no resources (or not a PE or NE file)
    Text text = Text::Narrow;              // Wide for a PE, Narrow for an NE (write_rc starts over in Unicode if it must)
};
// IoError when the file cannot be read; a damaged resource table is a FormatError.
Module read_module(const std::filesystem::path& path);

// Which lines of the `.rc` a resource (or an icon group with its members) wrote, for matching the compiler's
// "(line): Error!" to the resources to carry as payloads. 1-based, inclusive.
struct Span {
    std::size_t first = 0, last = 0;
    Key key;
};

// How each resource was written.
struct How {
    std::vector<Key> text, files, opaque, fallback;
};

struct RcFiles {
    std::filesystem::path rc;
    std::vector<Span> spans;
    How how;
    Text text = Text::Narrow;   // how the text was finally written
};

// Write `<module>.rc` and its payload files (res/...) under `outdir`. Resources whose key is in `verbatim` are written
// as payloads of their original bytes whatever their kind. A resource that needs a name or text with no source form
// at all (a zero character in a resource name) is a NoSourceForm error.
RcFiles write_rc(const Module& module, const std::filesystem::path& outdir, const std::set<Key>& verbatim = {});

// ---- compiling, and reading what the compiler wrote ---------------------------------------------------------------

struct Compiled {
    std::optional<std::filesystem::path> res;   // nullopt: the compiler failed
    std::string log;                            // its messages
};
// Run the resource compiler on `rc`. `microsoft` chooses the layout of the .res: the Microsoft one, which read_res
// reads, or the compiler's own (`<name>_link.res`), which is the one to hand the linker -- from a Microsoft-layout
// file the linker binds every resource with language 0. ToolError when OpenWatcom has no resource compiler.
Compiled compile_rc(const toolchain::watcom::OpenWatcom& open_watcom, const std::filesystem::path& rc, Text text,
                    bool microsoft = true);

// The resources in a compiled `.res` file (Microsoft layout). FormatError for a file the layout does not fit.
std::vector<fr::Resource> read_res(ByteView file, bool wide);

// Compare two sets of resources. An icon or cursor group is compared with its member images in order, since the
// compiler numbers members itself. Fills the counts and the keys that differ into `into`.
void compare(std::span<const fr::Resource> original, std::span<const fr::Resource> rebuilt, bool wide, Rebuilt& into);

// Write a module's resources as source, compile them, and compare with the original. A resource that does not
// survive is carried as a payload of its original bytes and the build repeated (at most `rounds` times), so the `.res`
// this returns is complete whenever the compiler accepts it at all. A module with no resources, or a compiler that
// rejects the file whatever is done, is `error` in the answer.
Rebuilt rebuild(const toolchain::watcom::OpenWatcom& open_watcom, const std::filesystem::path& module,
                const std::filesystem::path& outdir, int rounds = 12);

// ---- the command ----------------------------------------------------------------------------------------------------

// What the command needs of the rest of the program.
struct Environment {
    toolchain::watcom::OpenWatcom open_watcom{std::filesystem::path()};   // (an empty root: no tools)
    // A corpus name or a file -> the module (corpus.retail); nullopt when there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    // The project folder of a module's binary (Converted/<name>/), where the resources go without --out.
    std::function<store::converted::Project(const std::filesystem::path&)> project;
};
// `resources <module> ... [--out DIR]`: prints a line for each module and what was not reproduced. Without --out the
// resources go into the module's project folder: Converted/<name>/assets/resources/. Exit code 1 when a module's
// resources did not all come back.
cli::Command command(const Environment& environment);

}  // namespace cb::build::resources
