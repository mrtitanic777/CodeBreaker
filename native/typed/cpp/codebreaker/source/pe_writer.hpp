// codebreaker/source/pe_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/pe_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct, main, DOC) for the dynamic exe. Conversion and forwarding only: the stub finder is the new DOS writer's.
//
// Not carried over: the internals the module's own port exposed (Writer, Sec, Insn, decode, _STOP, _SEGS, _ARITH, _RR, _PRELUDE, _differing, _stub).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/pe_writer.hpp"
#include "codebreaker/source/dos_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::pe_writer {

inline const py::str DOC(cb::compat::to_py(cb::source::pe_writer::usage()));

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

using StubArg = std::variant<std::monostate, bool, py::str>;   // None, False, or the stub's source

namespace shim_ {
// The DOS stub's source, found by the new DOS writer (its tools as the old settings name them).
inline cb::source::FindStub find_stub() { return cb::source::dos_writer::stub_finder(dos_writer::shim_::options(nullptr, nullptr)); }
}  // namespace shim_

inline Result reconstruct(const py::str& path, const py::str& outdir, const StubArg& stub = std::monostate(),
                          const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        namespace n = cb::source::pe_writer;
        n::Options options;
        options.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm()));
        options.say = [&](std::string_view line) {
            if (log) log(cb::compat::to_py(line));
        };
        if (std::holds_alternative<std::monostate>(stub)) {
            options.stub = cb::source::SearchStub{};
            options.find_stub = shim_::find_stub();
        } else if (const auto* given = std::get_if<py::str>(&stub)) {
            options.stub = cb::fs::from_text(cb::compat::from_py(*given));
        } else {
            options.stub = cb::source::KeepStub{};
        }
        const n::Result r = n::reconstruct(cb::fs::from_text(cb::compat::from_py(path)), cb::fs::from_text(cb::compat::from_py(outdir)), options);
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

// python -m codebreaker pe-source <program> [--out DIR] [--stub FILE] [--keep-stub]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        cb::source::pe_writer::Environment env{.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm())),
                                               .state = cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state")))),
                                               .find_stub = shim_::find_stub()};
        return static_cast<int64_t>(cb::source::pe_writer::command(args, env, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::source::pe_writer
