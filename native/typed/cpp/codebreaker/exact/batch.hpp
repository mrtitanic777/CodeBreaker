// codebreaker/exact/batch.hpp -- compat shim for cb/exact/batch.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer.
//
// batch.py -- many candidates of a function compiled in one run.
//
// Typing notes:
//   * compile_many's `prj` and `tc` are objects of other modules (a Project, a toolchain); what it asks of them is the
//     two small interfaces below: prj.headers() / prj.include / prj.switches, and tc.compile(...).
//   * `split` and `build` return None as std::nullopt.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/batch.hpp"
#include "codebreaker/formats/omf.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::batch {

inline const py::set<py::str> KEYWORDS = {
    py::str("extern"), py::str("int"), py::str("char"), py::str("long"), py::str("short"), py::str("unsigned"),
    py::str("signed"), py::str("struct"), py::str("union"), py::str("double"), py::str("float"), py::str("register"),
    py::str("static"), py::str("void"), py::str("auto"), py::str("typedef"), py::str("enum"), py::str("sizeof"),
    py::str("return"), py::str("if"), py::str("else"), py::str("while"), py::str("for"), py::str("do"), py::str("switch"),
    py::str("case"), py::str("default"), py::str("break"), py::str("continue"), py::str("goto")};

// What compile_many asks of a project: prj.headers(), prj.include, prj.switches.
struct Project {
    std::function<py::dict<py::str, py::bytes>()> headers;
    py::list<py::str> include;
    std::optional<py::str> switches;
};

// What compile_many asks of a toolchain: tc.compile(src, headers=, include=, switches=) ->
// (object bytes or None, what the tool printed).
class Toolchain {
  public:
    virtual ~Toolchain() = default;
    virtual std::tuple<std::optional<py::bytes>, py::str> compile(
        const py::bytes& src, const py::dict<py::str, py::bytes>& headers, const py::list<py::str>& include,
        const std::optional<py::str>& switches) = 0;
};

// (file comment and #include lines, the rest) - or None when the candidate is not one to batch
inline std::optional<std::tuple<py::str, py::str>> split(const py::str& src) {
    const auto s = cb::exact::batch::split(src.utf8());
    if (!s) return std::nullopt;
    return std::make_tuple(py::str(s->preamble), py::str(s->body));
}

// the names a candidate gives the file: its function, what it declares extern, the structs it defines
inline py::set<py::str> own_names(const py::str& body) {
    return cb::compat::to_py(cb::exact::batch::own_names(body.utf8()));
}

inline py::str renamed(const py::str& body, int64_t k) {
    return py::str(cb::exact::batch::renamed(body.utf8(), cb::compat::int_from_py<std::size_t>(k)));
}

// -> (text, [(first line, last line) of each candidate], [index of each candidate in srcs]) or None
inline std::optional<std::tuple<py::str, py::list<std::tuple<int64_t, int64_t>>, py::list<int64_t>>> build(const py::list<py::str>& srcs) {
    const auto b = cb::exact::batch::build(cb::compat::from_py(srcs));
    if (!b) return std::nullopt;
    py::list<std::tuple<int64_t, int64_t>> spans;
    for (const auto& s : b->spans) spans.append(std::make_tuple(static_cast<int64_t>(s.first), static_cast<int64_t>(s.last)));
    return std::make_tuple(py::str(b->text), spans, cb::compat::to_py(b->candidates));
}

inline py::set<int64_t> error_lines(const py::str& con) {
    py::set<int64_t> out;
    for (std::size_t l : cb::exact::batch::error_lines(con.utf8())) out.add(static_cast<int64_t>(l));
    return out;
}

// -> {index in srcs: (object, the function's index among the object's)} for the candidates compiled together;
// those missing are the caller's to prove alone
inline py::dict<int64_t, std::tuple<py::bytes, int64_t>> compile_many(const Project& prj, Toolchain& tc, const py::list<py::str>& srcs,
                                                                      int64_t rounds = 3) {
    return cb::compat::guard([&] {
        class Adapter final : public cb::exact::batch::Toolchain {
        public:
            explicit Adapter(codebreaker::exact::batch::Toolchain& t) : tc_(t) {}
            cb::exact::batch::CompileOutcome compile(cb::ByteView source, const cb::exact::batch::CompileOptions& options) override {
                py::dict<py::str, py::bytes> headers;
                for (const auto& [name, bytes] : options.headers) headers.set(cb::compat::to_py(name), cb::compat::to_py(cb::ByteView(bytes)));
                py::list<py::str> include = cb::compat::to_py(options.include);
                std::optional<py::str> switches;
                if (options.switches) switches = py::str(*options.switches);
                auto [object, console] = tc_.compile(cb::compat::to_py(source), headers, include, switches);
                cb::exact::batch::CompileOutcome out;
                if (object) out.object = cb::compat::from_py(*object);
                out.console = console.utf8();
                return out;
            }

        private:
            codebreaker::exact::batch::Toolchain& tc_;
        };
        Adapter adapter(tc);
        cb::exact::batch::CompileOptions options;
        for (const auto& kv : prj.headers().items()) options.headers[std::get<0>(kv).utf8()] = cb::compat::from_py(std::get<1>(kv));
        options.include = cb::compat::from_py(prj.include);
        if (prj.switches) options.switches = prj.switches->utf8();
        const cb::exact::batch::SymbolRenamer rename = [](cb::ByteView object, const std::function<std::string(std::string_view)>& fn) {
            const py::bytes renamed_object = formats::omf::rename_symbols(
                cb::compat::to_py(object), [&](const py::str& n) { return py::str(fn(n.utf8())); });   // COMPAT(py): remove when formats/omf is refactored
            return cb::compat::from_py(renamed_object);
        };
        py::dict<int64_t, std::tuple<py::bytes, int64_t>> out;
        const auto compiled = cb::exact::batch::compile_many(adapter, options, rename, cb::compat::from_py(srcs), static_cast<int>(rounds));
        if (!compiled) return out;
        const py::bytes object = cb::compat::to_py(cb::ByteView(compiled->object));
        for (const auto& p : compiled->placed) out.set(static_cast<int64_t>(p.source), std::make_tuple(object, static_cast<int64_t>(p.function)));
        return out;
    });
}

}  // namespace codebreaker::exact::batch
