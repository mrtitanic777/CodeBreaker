// codebreaker/source/le_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/le_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct, main, DOC) for the dynamic exe. Conversion and forwarding only.
//
// Not carried over: the internals the module's own port exposed (Writer, Driver, Obj, Site, Place, Key, _STOP, _SEGS, _WIDTH, _PRELUDE,
// _target, _nasm, _differing).
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/le_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::le_writer {

inline const py::str DOC(cb::compat::to_py(cb::source::le_writer::usage()));

struct Result {
    py::str source;
    std::optional<py::str> image;
    bool same = false;
    int64_t rounds = 0;
    int64_t raw = 0;
    py::str log;
    int64_t instructions = 0;

    image_writer::FlatReport report() const {
        image_writer::FlatReport r;
        r.instructions = instructions;
        r.raw = raw;
        r.rounds = rounds;
        r.same = same;
        r.image = image;
        r.source = source;
        return r;
    }
};

inline Result reconstruct(const py::str& path, const py::str& outdir, const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        namespace l = cb::source::le_writer;
        l::Options options;
        options.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm()));
        options.say = [&](std::string_view line) {
            if (log) log(cb::compat::to_py(line));
        };
        const l::Result r = l::reconstruct(cb::fs::from_text(cb::compat::from_py(path)), cb::fs::from_text(cb::compat::from_py(outdir)), options);
        Result out;
        out.source = cb::compat::to_py(cb::fs::to_text(r.source));
        if (r.image) out.image = cb::compat::to_py(cb::fs::to_text(*r.image));
        out.same = r.same;
        out.rounds = static_cast<int64_t>(r.rounds);
        out.raw = static_cast<int64_t>(r.raw);
        out.log = cb::compat::to_py(r.log);
        out.instructions = static_cast<int64_t>(r.instructions);
        return out;
    });
}

// python -m codebreaker le-source <driver> [--out DIR]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        cb::source::le_writer::Environment env{.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm())),
                                               .state = cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state"))))};
        return static_cast<int64_t>(cb::source::le_writer::command(args, env, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::source::le_writer
