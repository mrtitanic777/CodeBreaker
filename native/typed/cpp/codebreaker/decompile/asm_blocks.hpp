// codebreaker/decompile/asm_blocks.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/asm_blocks.{hpp,cpp}; this header keeps the old py:: surface for decompile/decompiler (the _asm blocks
// pass, CB_ASM_BLOCKS): the L2 tree as asm_blocks::Json, the decoder as a std::function over py::bytes, place and what its
// tests reach. Conversion and forwarding only; deleted with its last py:: consumer.
//
// Not carried over: the private helpers of the Python (_walk, _lists, _at, _calls_helper, _reads, _feeds, _line,
// _HELPER) -- they are asm_blocks::interrupt_called, condition_reads, feeds_interrupt, inline_line in the new module and
// l2tree::walk / for_each_statement_list / origin_offsets.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/asm_blocks.hpp"
#include "codebreaker/decompile/inline_asm.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::decompile::asm_blocks {

namespace inline_asm = ::codebreaker::decompile::inline_asm;
namespace na = ::cb::decompile::asm_blocks;

// A JSON value (what json.load gives, and what the L2 tree is made of).
struct Json {
    enum Kind { None, Bool, Int, Float, Str, List, Dict };
    Kind kind = None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    py::str s;
    py::list<Json> list;
    py::dict<py::str, Json> dict;

    static Json make_none() { return Json(); }
    static Json make_bool(bool v) { Json j; j.kind = Bool; j.b = v; return j; }
    static Json make_int(int64_t v) { Json j; j.kind = Int; j.i = v; return j; }
    static Json make_float(double v) { Json j; j.kind = Float; j.d = v; return j; }
    static Json make_str(const py::str& v) { Json j; j.kind = Str; j.s = v; return j; }
    static Json make_list(const py::list<Json>& v) { Json j; j.kind = List; j.list = v; return j; }
    static Json make_dict(const py::dict<py::str, Json>& v) { Json j; j.kind = Dict; j.dict = v; return j; }
};

// The decoded instruction: inline_asm's, with capstone's access flags of its operands.
struct Instruction : inline_asm::Instruction {
    py::list<int64_t> access;        // op.access of each operand (the same length as `operands`)
};
inline constexpr int64_t CS_AC_WRITE = 2;
// md.disasm(code, address)
using Decoder = std::function<py::list<Instruction>(const py::bytes&, uint64_t)>;

using Frame = py::dict<int64_t, py::str>;      // {bp offset: C name}
using NameAt = inline_asm::NameAt;
using Fixups = inline_asm::Fixups;

namespace detail {

inline ::cb::Json to_new(const Json& j) {
    switch (j.kind) {
        case Json::None: return ::cb::Json(nullptr);
        case Json::Bool: return ::cb::Json(j.b);
        case Json::Int: return ::cb::Json(j.i);
        case Json::Float: return ::cb::Json(j.d);
        case Json::Str: return ::cb::Json(j.s.utf8());
        case Json::List: {
            ::cb::Json out = ::cb::Json::array();
            for (const Json& x : j.list.vec()) out.push_back(to_new(x));
            return out;
        }
        case Json::Dict: {
            ::cb::Json out = ::cb::Json::object();
            for (const auto& kv : j.dict.items()) out[std::get<0>(kv).utf8()] = to_new(std::get<1>(kv));
            return out;
        }
    }
    return ::cb::Json(nullptr);
}

inline Json from_new(const ::cb::Json& j) {
    if (j.is_null()) return Json::make_none();
    if (j.is_boolean()) return Json::make_bool(j.get<bool>());
    if (j.is_number_unsigned()) {
        const auto u = j.get<std::uint64_t>();
        if (u > static_cast<std::uint64_t>(INT64_MAX)) throw py::OverflowError("Python int too large to convert to C long");
        return Json::make_int(static_cast<int64_t>(u));
    }
    if (j.is_number_integer()) return Json::make_int(j.get<std::int64_t>());
    if (j.is_number_float()) return Json::make_float(j.get<double>());
    if (j.is_string()) return Json::make_str(py::str(j.get<std::string>()));
    if (j.is_array()) {
        py::list<Json> out;
        for (const ::cb::Json& x : j) out.append(from_new(x));
        return Json::make_list(out);
    }
    py::dict<py::str, Json> out;
    for (const auto& entry : j.items()) out.set(py::str(entry.key()), from_new(entry.value()));
    return Json::make_dict(out);
}

}  // namespace detail

// Python's truth value of a JSON value (None, False, 0, 0.0, "", [] and {} are false)
inline bool truthy(const Json& v) {
    switch (v.kind) {
        case Json::None: return false;
        case Json::Bool: return v.b;
        case Json::Int: return v.i != 0;
        case Json::Float: return v.d != 0.0;
        case Json::Str: return !v.s.empty();
        case Json::List: return !v.list.empty();
        case Json::Dict: return !v.dict.empty();
    }
    return false;
}
// v == "text"
inline bool is_str(const Json& v, const char* text) { return v.kind == Json::Str && v.s == py::str(text); }
// copy.deepcopy(v)
inline Json deepcopy(const Json& v) { return detail::from_new(detail::to_new(v)); }

// The names of the frame's places as asm_blocks::frame_names finds them (the Python's `far` argument was unused).
inline Frame _frame_names(const Json& doc, bool /*far*/) {
    const ::cb::Json tree = detail::to_new(doc);
    Frame out;
    for (const auto& [off, name] : na::frame_names(tree)) out.set(off, ::cb::compat::to_py(name));
    return out;
}

// [bp + k] / [bp - k] in an operand by the C name of that place.
inline py::str _named(const py::str& text, const Frame& frame) {
    na::Frame f;
    for (const auto& kv : frame.items()) f[std::get<0>(kv)] = std::get<1>(kv).utf8();
    return ::cb::compat::to_py(na::name_frame_places(text.utf8(), f));
}

namespace detail {

inline na::Decoder new_decoder(const Decoder& md) {
    return [md](::cb::ByteView code, std::uint32_t address) {
        std::vector<na::Instruction> out;
        for (const Instruction& old : md(::cb::compat::to_py(code), address).vec()) {
            na::Instruction i = inline_asm::detail::to_new(old);
            for (std::size_t k = 0; k < i.operands.size() && static_cast<int64_t>(k) < old.access.size(); ++k) {
                i.operands[k].access = static_cast<std::uint8_t>(old.access[static_cast<int64_t>(k)]);
            }
            out.push_back(std::move(i));
        }
        return out;
    };
}

template <class Data>
std::tuple<Json, py::list<py::str>> place_(const Decoder& md, const Json& doc, const Data& code, int64_t start, int64_t end,
                                           const Fixups& fixups, const std::tuple<int64_t, int64_t>& window) {
    return ::cb::compat::guard([&] {
        ::cb::Json tree = to_new(doc);
        na::FixupSites sites;
        for (const auto& kv : fixups.names.items()) sites.insert(::cb::compat::int_from_py<std::uint32_t>(std::get<0>(kv)));
        const ::cb::Bytes bytes = ::cb::compat::from_py(inline_asm::detail::bytes_of(code));
        const na::Window w{static_cast<std::int32_t>(std::get<0>(window)), static_cast<std::int32_t>(std::get<1>(window))};
        const std::vector<std::string> said = na::place(tree, bytes, ::cb::compat::int_from_py<std::uint32_t>(start),
                                                        ::cb::compat::int_from_py<std::uint32_t>(end), sites, w, new_decoder(md));
        return std::make_tuple(from_new(tree), ::cb::compat::to_py(said));
    });
}

}  // namespace detail

// See the module's doc. code: the segment's bytes; start, end: the function's place in it. NameAt and seg were never read
// by the Python (inline_asm's operands do not use them). -> (the tree, what was done)
inline std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc, const py::bytes& code, int64_t start, int64_t end,
                                                 const Fixups& fixups = Fixups(), const NameAt& = NameAt(),
                                                 std::optional<int64_t> = std::nullopt,
                                                 const std::tuple<int64_t, int64_t>& window = std::make_tuple(0, 0)) {
    return detail::place_(md, doc, code, start, end, fixups, window);
}
inline std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc, const py::bytearray& code, int64_t start, int64_t end,
                                                 const Fixups& fixups = Fixups(), const NameAt& = NameAt(),
                                                 std::optional<int64_t> = std::nullopt,
                                                 const std::tuple<int64_t, int64_t>& window = std::make_tuple(0, 0)) {
    return detail::place_(md, doc, code, start, end, fixups, window);
}

// The locals the blocks took the last use of left out (the tree is edited in place, as the Python edits it).
inline void _unused(Json& doc) {
    ::cb::Json tree = detail::to_new(doc);
    na::drop_unused_locals(tree);
    doc = detail::from_new(tree);
}

}  // namespace codebreaker::decompile::asm_blocks
