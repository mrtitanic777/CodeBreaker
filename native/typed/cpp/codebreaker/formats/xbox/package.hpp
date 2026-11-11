// codebreaker/formats/xbox/package.hpp -- typed C++ port of codebreaker/formats/xbox/__init__.py.
//
// formats/xbox -- the Xbox and Xbox 360 file formats: containers, discs, filesystems, assets.
//
//     summary(path, match)            {field: value} for `codebreaker info` (match: detect.identify's)
//
// Containers are read through formats/media.py (volumes.py adapts each one), so `codebreaker media list|extract`
// takes all of them (stfs, gdf, god, ciso, chd, fatx, zar, xzp, pck); assets are summarised by `codebreaker info`:
// xbe, xdbf, xpr, texture, xwb, xma, xur, bink.
//
// Typing notes:
//   * summary() answers {field name: value} with values of several kinds (a count, a hex text, a size, a float): a
//     `Field` is the variant of what the readers put there (None, bool, int, float, str). `NonScalar` stands for the
//     bytes / bytearray / list / dict values of xpr.parse's dict, which `_xpr` leaves out.
//   * `match` is the first of detect.identify(path): a dict of text (`command`, `format` ...): py::dict<str, str>.
//   * The readers this module calls that have no typed port yet -- formats/media.volumes (and the volume objects
//     volumes.py makes), formats/xbox/xbe.XBE, xdbf.XDBF and xpr.parse -- are Hooks: each answers a plain record of
//     what `summary` reads of the Python object (the attributes it names: the `getattr(x, "title_name", "")` defaults
//     are the record's defaults). Install them with set_hooks() before calling summary for such a file; without a hook
//     the call raises ImportError. xwb, xur, bink and chd are the typed ports (formats/xbox/xwb.hpp ...) and are called.
//   * The Python's `except Exception: pass` around xdbf's achievements() is the hook answering nullopt for them.
//   * `_BY_FORMAT` is a list of (key, function) pairs; the keys are listed here, the functions are `_xbe` ... `_chd`.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::xbox {

struct NonScalar {};   // a bytes / bytearray / list / dict value (a field `_xpr` drops)
using Field = std::variant<std::monostate, bool, int64_t, double, py::str, NonScalar>;
using Summary = py::dict<py::str, Field>;

// ---- what the readers without a port answer ------------------------------------------------------------------
// One file of a volume: the two things `_container` reads of volumes.py's Entry.
struct FileEntry {
    bool is_dir = false;
    int64_t size = 0;
};
// One volume of a container (formats/volumes.py: kind, label, where, boot, files()).
struct Volume {
    py::str kind;
    py::str label;
    std::optional<py::str> where;                // the text after the counts ("" / None: none)
    py::dict<py::str, Field> boot;               // `(v.boot or {})`
    py::list<FileEntry> files;                   // v.files()
};
// xbe.XBE(data): the attributes `_xbe` reads (`title_name`, `title_id`, `region`, `allowed_media` have getattr defaults).
struct XbeInfo {
    py::str title_name;
    int64_t title_id = 0;
    int64_t base = 0;
    int64_t size_of_image = 0;
    int64_t section_count = 0;
    int64_t region = 0;
    int64_t allowed_media = 0;
};
// xdbf.XDBF(data): version / entry_count / free_count, counts() as (namespace label, count) in namespace order (the label
// is `xdbf.NS.get(ns, ns)`), and the achievements as (unlocked, score) -- nullopt when achievements() raises.
struct XdbfInfo {
    int64_t version = 0;
    int64_t entry_count = 0;
    int64_t free_count = 0;
    py::list<std::tuple<py::str, int64_t>> namespaces;
    std::optional<py::list<std::tuple<bool, int64_t>>> achievements;
};

struct Hooks {
    std::function<py::list<Volume>(const py::str& path)> volumes;          // media.volumes(path)
    std::function<XbeInfo(const py::bytes& data)> xbe;                     // xbe.XBE(data)
    std::function<XdbfInfo(const py::bytes& data)> xdbf;                   // xdbf.XDBF(data)
    std::function<py::dict<py::str, Field>(const py::bytes& data)> xpr;    // xpr.parse(data)
};
void set_hooks(const Hooks& hooks);

// ---- the module ----------------------------------------------------------------------------------------------
Summary _container(const py::str& path);
Summary _xbe(const py::str& path);
Summary _xdbf(const py::str& path);
Summary _xpr(const py::str& path);
Summary _xwb(const py::str& path);
Summary _xur(const py::str& path);
Summary _bink(const py::str& path);
Summary _chd(const py::str& path);

// (key, summarising function): the formats `summary` knows, in the order it asks
const py::list<std::tuple<py::str, std::function<Summary(const py::str&)>>>& _BY_FORMAT();

// What `codebreaker info` shows for an Xbox file: {field: value}, or {} when there is nothing to add to the format
// line. `match` is the first of detect.identify(path).
Summary summary(const py::str& path, const py::dict<py::str, py::str>& match);

}  // namespace codebreaker::formats::xbox
