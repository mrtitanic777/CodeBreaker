// codebreaker/verify/lanes/dos.hpp -- COMPAT SHIM over cb/verify/lanes/dos.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/dos.{hpp,cpp} now (reconstructing a real-mode program: its source is assembly
// written from its image, assembled, linked and compared with the original as a whole; nothing is run). This header
// keeps the old py:: API: the kind's four functions (profile, prepare, build, judge) over the old records, and
// `set_packager`, which installs the platform's packager (the writer and the tools) the new lane is given.
// No logic of the lane's own.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/core/text.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/lanes/dos.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::dos {

inline const py::str KIND = py::str("dos");
inline constexpr int64_t REV = cb::verify::lanes::DosLane::kRev;
inline const py::str SAME = py::str(std::string(cb::verify::lanes::kSameProgram));

// a function the writer wrote: its name (the writer's dict holds more, the lane asks only this)
struct Function {
    py::str name;
};

// what the packager's writer returns (a dict)
struct Written {
    std::optional<py::str> source;                          // the assembly source file written
    std::optional<py::dict<int64_t, Function>> functions;   // address -> function
    std::optional<bool> linked;                             // the program was assembled and linked
    std::optional<py::list<py::str>> notes;                 // what the build said, the last line last
    std::optional<bool> same;                               // the built program is the original, byte for byte
    std::optional<py::str> built;                           // the built file
};

struct Packager {
    std::function<py::dict<py::str, py::str>()> tools;                                // {name: exe path}
    std::function<Written(const py::str& module_path, const py::str& work)> writer;   // writes and rebuilds the program
};

namespace detail {
namespace nv = cb::verify::lanes;

inline Packager& packager() {
    static Packager p;
    return p;
}
inline Written old_written(const nv::Written& w) {
    Written out;
    if (!w.source.empty()) out.source = cb::compat::to_py(w.source.generic_string());
    py::dict<int64_t, Function> fns;
    for (const auto& [address, name] : w.functions) fns.set(static_cast<int64_t>(address), Function{cb::compat::to_py(name)});
    out.functions = fns;
    out.linked = w.linked;
    out.notes = cb::compat::to_py(w.notes);
    out.same = w.same;
    if (!w.built.empty()) out.built = cb::compat::to_py(w.built.generic_string());
    return out;
}
inline nv::Written new_written(const Written& w) {
    nv::Written out;
    if (w.source) out.source = w.source->utf8();
    if (w.functions)
        for (const auto& kv : w.functions->items()) out.functions[static_cast<std::uint32_t>(std::get<0>(kv))] = std::get<1>(kv).name.utf8();
    out.linked = w.linked.value_or(false);
    if (w.notes) out.notes = cb::compat::from_py(*w.notes);
    out.same = w.same.value_or(false);
    if (w.built) out.built = w.built->utf8();
    return out;
}
// the new lane over the installed packager (asking before one is set is a RuntimeError, as before)
inline nv::DosLane lane() {
    const Packager p = packager();
    if (!p.writer || !p.tools) throw py::RuntimeError("the DOS platform's packager has not been installed");
    return nv::DosLane(
        [p](const std::filesystem::path& module, const std::filesystem::path& out_dir) {
            return new_written(p.writer(cb::compat::to_py(module.generic_string()), cb::compat::to_py(out_dir.generic_string())));
        },
        [p] {
            std::map<std::string, std::filesystem::path> tools;
            for (const auto& kv : p.tools().items()) tools[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
            return tools;
        });
}
}  // namespace detail

inline void set_packager(const Packager& packager) { detail::packager() = packager; }

// (a digest of the settings a verdict depends on, the text it is made from)
inline std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt) {
    const auto p = detail::lane().profile(module_path ? std::filesystem::path(module_path->utf8()) : std::filesystem::path());
    return {cb::compat::to_py(p.digest), cb::compat::to_py(p.settings)};
}

struct Prepared {
    Written written;
    py::str header;                              // the source text (read as latin-1)
    py::list<py::str> order;                     // function names, by address
    py::dict<py::str, py::str> key;              // name -> "%05x" of its address
    py::list<py::str> all_keys;
    py::dict<py::str, py::str> texts;
    py::str sig;
};

inline Prepared prepare(const py::str& module_path, const py::str& work) {
    return cb::compat::guard([&] {
        const auto native = detail::lane().prepare(module_path.utf8(), std::filesystem::path(work.utf8()));
        const auto& m = static_cast<const cb::verify::lanes::DosLane::PreparedProgram&>(*native);
        Prepared out;
        out.written = detail::old_written(m.written);
        out.header = cb::compat::to_py(cb::text::from_latin1(cb::as_bytes(m.header)));   // (the Python read it as latin-1)
        out.order = cb::compat::to_py(m.order);
        for (const std::string& n : m.order) out.key.set(cb::compat::to_py(n), cb::compat::to_py(m.key.at(n)));
        for (const std::string& n : m.order)
            if (m.texts.contains(n)) out.texts.set(cb::compat::to_py(n), cb::compat::to_py(m.texts.at(n)));
        out.all_keys = cb::compat::to_py(m.all_keys);
        out.sig = cb::compat::to_py(m.sig);
        return out;
    });
}

// (did the program come back identical, why not)
using Context = std::tuple<bool, py::str>;
// a function's item for judge: (its key,)
using Item = std::tuple<py::str>;

struct Built {
    Context context;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> missing;   // {name: why it is not in the build}
};

inline Built build(const Prepared& m, const py::str&, const py::str&) {
    namespace nv = cb::verify::lanes;
    Built out;
    const nv::Written w = detail::new_written(m.written);
    if (!w.linked) {
        const std::string why = nv::DosLane::why_not_built(w);
        out.context = Context{false, cb::compat::to_py(why)};
        for (const py::str& n : m.order) out.missing.set(n, cb::compat::to_py(why));
        return out;
    }
    out.context = Context{w.same, cb::compat::to_py(nv::DosLane::why_differs(w))};
    for (const py::str& n : m.order) out.items.set(n, Item(m.key.at(n)));
    return out;
}

// (verdict, what differs, [], text)
using Verdict = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

inline Verdict judge(const Context& context, const Item&) {
    const cb::verify::Judgement j = cb::verify::lanes::DosLane::judge_function(std::get<0>(context), std::get<1>(context).utf8());
    return Verdict{cb::compat::to_py(cb::verify::word(j.verdict)), cb::compat::to_py(j.reason), cb::compat::to_py(j.limits),
                   cb::compat::to_py(j.detail)};
}

}  // namespace codebreaker::verify::lanes::dos
