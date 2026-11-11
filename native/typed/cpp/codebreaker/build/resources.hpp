// codebreaker/build/resources.hpp -- typed C++ of codebreaker.build.resources
//
// resources.py -- a module's resources as source: `.rc` text and payload files, compiled back to `.res`.
//
//     python -m codebreaker resources <module> [--out DIR]
//
//     Result r = resources::rebuild(module_path, outdir);      // write, compile, compare; r.res is the .res
//     r.total, r.identical, r.equivalent, r.different, r.missing
//     r.as_text, r.as_files, r.opaque, r.fallback     // how each resource was written
//
// What is written, per kind of resource:
//
//     text        string tables, dialogs, menus, accelerator tables and version blocks are decoded
//                 (formats/resources) and written as the `.rc` statements they came from
//     files       an icon or cursor group with its member images becomes a real .ico / .cur file,
//                 a bitmap a .bmp, each named by an ICON / CURSOR / BITMAP statement
//     opaque      kinds with no source form here (fonts, type libraries, raw data, anything
//                 user-defined) are carried as a payload file named by a raw-data statement
//
// The `.rc` is compiled with OpenWatcom's resource compiler and the result is compared with the
// original, resource by resource: IDENTICAL bytes, or EQUIVALENT (it decodes to the same thing --
// the compiler pads, orders or numbers something differently). A resource that comes back
// different, or whose statement the compiler rejects, is written again as a payload of its
// original bytes ("fallback"), so the `.res` always holds the module's resources; how many needed
// that is reported, because those are not yet source.
//
// EQUIVALENT covers three things the compiler decides for itself: it numbers the member images of
// an icon or cursor group, it pads, and it stamps the build time into a version block's file date
// (the original's is zero).
//
// Facts about the compiler, each found by a round trip that failed:
//
//   * `-zm` makes it write the Microsoft `.res` layout (its default is a layout of its own). That
//     file is for comparing; the LINKER must be given the default layout (`r.link_res`), because
//     from a Microsoft-layout file it binds every resource with language 0.
//   * In the Microsoft layout it does not pad bodies consistently, so entries do not always start
//     on four-byte boundaries (`read_res` looks for the next entry rather than computing it).
//   * A quoted type or name is taken literally: a resource called MENU is not the keyword.
//   * A CONTROL statement's style has WS_CHILD and WS_VISIBLE unless they are taken out with NOT.
//   * A string ends at a zero character, however it is written: text with one inside has no
//     source form.
//   * Text outside Latin-1 needs a UTF-8 source (`-zku8`), and there the compiler miscounts a
//     string table's strings (UTF-8 bytes instead of characters): in a module that needs UTF-8,
//     string tables with anything beyond ASCII are carried as payloads.
//   * It writes a 16-bit version block with lengths that do not add up; 16-bit version blocks
//     are carried as payloads.
//   * It keeps only 16 bits of a control's or menu item's identifier, also in the extended
//     templates that have room for 32 (where -1 is common): those are carried as payloads.
//   * Without the preprocessor (`-zn`) a comment is a syntax error.
//   * Like every OpenWatcom tool it has to be started by its full path.
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * A resource's key (type, name, language) is `Key`; a type or a name is formats::resources::Ident.
//   * `wide` is the int 0 / WIDE / UNICODE of the Python (a bool where it is only passed to the format decoders).
//   * `_members` answers {group key: [member resource]}: the members are given as indexes into `found`, which is
//     what the Python's `id(member)` identity comes to (the same resource object is the same index).
//   * `write_rc` answers (rc path, spans, how, wide); `how` is {how written: [resource key]}.
//   * The Python's Result is the struct `Result`; `rc`, `res` and `link_res` are None until set.
//   * The module's own exceptions NoSourceForm and BeyondLatin1 are py::Exception subclasses of those names.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/resources.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::resources {

namespace R = codebreaker::formats::resources;

using Ident = R::Ident;
using Key = std::tuple<Ident, Ident, std::optional<int64_t>>;     // Resource.key: (type, name, language)

// How text is written, which is also how a resource is decoded: 0 for a 16-bit module, WIDE for a
// 32-bit one whose text fits Latin-1, UNICODE for a 32-bit one with text beyond it.
inline constexpr int64_t WIDE = 1, UNICODE = 2;

// This resource uses something the `.rc` statements written here cannot say.
struct NoSourceForm : py::Exception {
    NoSourceForm() : py::Exception() {}
    explicit NoSourceForm(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "NoSourceForm"; }
};

// The module has text outside Latin-1: its `.rc` has to be UTF-8.
struct BeyondLatin1 : py::Exception {
    BeyondLatin1() : py::Exception() {}
    const char* type() const override { return "BeyondLatin1"; }
};

// What a rebuild produced and how it compares with the original.
struct Result {
    std::optional<py::str> rc, res, link_res;       // link_res: the .res in the layout the linker binds
    int64_t total = 0, identical = 0, equivalent = 0;
    py::list<Key> different, missing, extra;
    int64_t as_text = 0, as_files = 0, opaque = 0, fallback = 0;
    py::list<Key> not_source;                       // keys of the resources carried as fallback payloads
    py::str error;

    bool same() const;
    py::str line() const;
};

// ---------------------------------------------------------------------------------------------
// the pieces of a statement
// ---------------------------------------------------------------------------------------------
// A string literal. Control characters and everything above ASCII are written as
// three-digit octal escapes (a fixed width, so a digit after one cannot join it, and no code
// page is involved: the compiler takes each as one character of that value). That reaches
// Latin-1. A 32-bit module with text beyond it is written in UTF-8 instead, where the compiler
// gets a length-counted string (`counted`: a string table's) wrong -- it counts UTF-8 bytes --
// so such a string has no source form there. Nor has a string with a zero character in it
// anywhere: the compiler ends the string at it.
py::str quoted(const py::str& text, int64_t wide, bool counted = false);

// A resource's name or type as a token: a number, or the string in quotes.
py::str ident(const Ident& value, int64_t wide);

py::str _memory(int64_t flags, int64_t wide);

// An identifier. An extended dialog or menu has room for 32 bits, but the compiler keeps 16
// however the number is written (-1 comes out as 0xFFFF).
py::str _number(int64_t value);

// A control's style: the two bits a CONTROL statement adds by itself are removed when the
// original does not have them.
py::str _style(int64_t value);

py::str _stem(const R::Resource& resource);

// ---------------------------------------------------------------------------------------------
// statements for the kinds that have a source form
// ---------------------------------------------------------------------------------------------
py::list<py::str> dialog_text(const R::Resource& r, int64_t wide);
py::list<py::str> menu_text(const R::Resource& r, int64_t wide);
py::list<py::str> accelerators_text(const R::Resource& r, int64_t wide);
py::list<py::str> version_text(const R::Resource& r, int64_t wide);

// ---------------------------------------------------------------------------------------------
// writing the .rc
// ---------------------------------------------------------------------------------------------
// {(member type, id): [resource]} and, per group that can be put back together as a file,
// the members it takes: {group key: [member resource]} -- as indexes into `found`.
py::dict<Key, py::list<int64_t>> _members(const py::list<R::Resource>& found, int64_t wide);

using Span = std::tuple<int64_t, int64_t, Key>;                      // (first line, last line, resource key)
using How = py::dict<py::str, py::list<Key>>;                        // {how written: [resource key]}

// Write `<module>.rc` and its payload files under `outdir`. Resources whose key is in
// `verbatim` are written as payloads of their original bytes whatever their kind.
// -> (rc path, [(first line, last line, resource key)], {how written: [resource key]}, how text is written)
std::tuple<py::str, py::list<Span>, How, int64_t> write_rc(const py::str& module_path, const py::str& outdir,
                                                           const py::set<Key>& verbatim = py::set<Key>());
std::tuple<py::str, py::list<Span>, How, int64_t> _write_rc(const py::str& module_path, const py::str& outdir,
                                                            py::set<Key> verbatim, int64_t wide);

// ---------------------------------------------------------------------------------------------
// compiling, and reading what the compiler wrote
// ---------------------------------------------------------------------------------------------
// Run the resource compiler. -> (.res path or None, its messages). `microsoft` chooses the
// layout of the .res: the Microsoft one, which read_res reads, or the compiler's own
// (`<name>_link.res`), which is the one to hand the linker -- from a Microsoft-layout file
// the linker binds every resource with language 0.
std::tuple<std::optional<py::str>, py::str> compile_rc(const py::str& rc, int64_t wide, bool microsoft = true);

// The resources in a compiled `.res` file (Microsoft layout).
py::list<R::Resource> read_res(const py::str& path, int64_t wide);

// Compare two sets of resources. An icon or cursor group is compared with its member images
// in order, since the compiler numbers members itself. -> Result (counts and the keys that differ).
Result compare(const py::list<R::Resource>& original, const py::list<R::Resource>& rebuilt, int64_t wide,
               const std::optional<Result>& result = std::nullopt);

// Write a module's resources as source, compile them, and compare with the original. A
// resource that does not survive is carried as a payload of its original bytes and the build
// repeated, so the `.res` this returns is complete whenever the compiler accepts it at all.
Result rebuild(const py::str& module_path, const py::str& outdir, int64_t rounds = 12);

// python -m codebreaker resources <module> [--out DIR]
//
// Without --out the resources go into the module's project folder: Converted/<name>/assets/resources/.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::build::resources
