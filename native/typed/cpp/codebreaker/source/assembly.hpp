// codebreaker/source/assembly.hpp -- SHIM over cb/source/assembly.hpp (docs/CPP_DESIGN.md 6.3): what the assembly-source writers share
// (drivers, real-mode programs). The old types in (a candidate is a (text, bytes, mask) tuple; a piece a 5-tuple; sets of them), the
// new functions called. The OpenWatcom tools and their environment come from the old build/driver and build/watcom; the file of
// what is known is state/assembly_probes.json, as before. Deleted with its last py:: consumer (the writers).
//
//     assembly::operands(text)                  a decoder's operand text in the assembler's spelling
//     assembly::data_lines(bytes)               bytes as `db` lines, text as text
//     assembly::probe(candidates, ...)          which instruction texts the assembler turns back into the original's bytes
#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/assembly.hpp"
#include "codebreaker/build/driver.hpp"
#include "codebreaker/build/watcom.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::assembly {

inline const py::list<py::str> CONDITIONS = [] {
    py::list<py::str> out;
    for (const std::string_view c : cb::source::assembly::kConditions) out.append(cb::compat::to_py(c));
    return out;
}();
inline const py::dict<py::str, py::str> SPELLING = {
    {py::str("pushal"), py::str("pushad")}, {py::str("popal"), py::str("popad")}, {py::str("int3"), py::str("int 3")},
    {py::str("retn"), py::str("ret")},      {py::str("fwait"), py::str("wait")},  {py::str("pushaw"), py::str("pusha")},
    {py::str("popaw"), py::str("popa")},    {py::str("retfw"), py::str("retf")},  {py::str("iretw"), py::str("iret")}};
// The patterns, as the Python has them (the callers apply them with py::re).
inline const py::str STRING_OPS = py::str(R"(^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)([bwd])\b)");
inline const py::str HEX_PATTERN = py::str(R"(\b0x([0-9a-f]+)\b)");

namespace shim_ {

// OpenWatcom's tools and environment, the old settings'.
inline cb::source::assembly::Tools tools() {
    cb::source::assembly::Tools t;
    const py::str wasm = build::driver::tool(py::str("wasm")), wlink = build::driver::tool(py::str("wlink"));
    t.wasm = cb::fs::from_text(cb::compat::from_py(wasm));
    t.wlink = cb::fs::from_text(cb::compat::from_py(wlink));
    for (const auto& [k, v] : build::watcom::ow_env(py::str("pe")).items()) t.env[cb::compat::from_py(k)] = cb::compat::from_py(v);
    return t;
}

inline std::filesystem::path known_file() {
    return cb::fs::from_text(cb::compat::from_py(py::os::path::join(py::settings::get_str(py::str("state")), py::str("assembly_probes.json"))));
}

}  // namespace shim_

// Assemble one file with OpenWatcom's assembler. -> (object file or None, its messages)
inline std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work) {
    const auto made = cb::source::assembly::assemble(shim_::tools(), cb::fs::from_text(cb::compat::from_py(source)), cb::fs::from_text(cb::compat::from_py(work)));
    return {made.file ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*made.file))) : std::nullopt, cb::compat::to_py(made.log)};
}

// Link with OpenWatcom's linker from the directives in `lines`. -> (file or None, messages)
inline std::tuple<std::optional<py::str>, py::str> link(const py::list<py::str>& lines, const py::str& out, const py::str& work) {
    std::vector<std::string> directives;
    for (const py::str& l : lines) directives.push_back(cb::compat::from_py(l));
    const auto made = cb::source::assembly::link(shim_::tools(), directives, cb::fs::from_text(cb::compat::from_py(out)), cb::fs::from_text(cb::compat::from_py(work)));
    return {made.file ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*made.file))) : std::nullopt, cb::compat::to_py(made.log)};
}

// Hexadecimal numbers the way the assembler reads them (0x1f -> 01Fh).
inline py::str operands(const py::str& text) { return cb::compat::to_py(cb::source::assembly::operands(cb::compat::from_py(text))); }

inline py::str mnemonic(const py::str& name) { return cb::compat::to_py(cb::source::assembly::mnemonic(cb::compat::from_py(name))); }

// Bytes as `db` lines; runs of printable characters as quoted text.
inline py::list<py::str> data_lines(const py::bytes& data, int64_t per_line = 16) {
    const cb::Bytes bytes = cb::compat::from_py(data);
    return cb::compat::to_py(cb::source::assembly::data_lines(bytes, static_cast<std::size_t>(per_line)));
}

using Candidate = std::tuple<py::str, py::bytes, py::bytes>;

// Which candidates assemble to their original bytes (see the new module). `mode` is "flat32" or "real16".
inline py::set<Candidate> probe(const py::list<Candidate>& candidates, const py::set<py::str>& labels, const py::str& outdir,
                                const py::str& mode = py::str("flat32"), const py::set<py::str>& near = py::set<py::str>()) {
    return cb::compat::guard([&] {
        std::vector<cb::source::assembly::Candidate> wanted;
        for (const Candidate& c : candidates) wanted.push_back({cb::compat::from_py(std::get<0>(c)), cb::compat::from_py(std::get<1>(c)), cb::compat::from_py(std::get<2>(c))});
        std::set<std::string> names, close;
        for (const py::str& l : labels) names.insert(cb::compat::from_py(l));
        for (const py::str& l : near) close.insert(cb::compat::from_py(l));
        // one Prober for the file of what is known, as the one lock of the old module
        static std::mutex lock;
        const std::lock_guard<std::mutex> guard(lock);
        cb::source::assembly::Prober prober(shim_::tools(), shim_::known_file());
        const auto good = prober.probe(wanted, names, close, cb::fs::from_text(cb::compat::from_py(outdir)),
                                       mode == py::str("real16") ? cb::source::assembly::Mode::Real16 : cb::source::assembly::Mode::Flat32);
        py::set<Candidate> out;
        for (const auto& c : good) out.add(Candidate(cb::compat::to_py(c.text), cb::compat::to_py(cb::ByteView(c.original)), cb::compat::to_py(cb::ByteView(c.mask))));
        return out;
    });
}

// {offset: bytes} of the data records of the last segment an object file defines, joined where one record continues the one before.
inline py::dict<int64_t, py::bytes> _last_segment_data(const py::bytes& obj) {
    return cb::compat::guard([&] {
        py::dict<int64_t, py::bytes> out;
        for (const auto& [off, data] : cb::source::assembly::last_segment_data(cb::compat::from_py(obj))) out.set(static_cast<int64_t>(off), cb::compat::to_py(cb::ByteView(data)));
        return out;
    });
}

// One piece of the original: (offset, length, mask or None, key or None, is a jump inside the segment)
template <class Key>
using Piece = std::tuple<int64_t, int64_t, std::optional<py::bytes>, std::optional<Key>, bool>;

// Which instructions did not come back as they were (see the new module). -> the set of keys to write as bytes instead.
template <class Key>
py::set<Key> culprits(const py::bytes& original, const py::bytes& built, const py::list<Piece<Key>>& pieces) {
    // The new function orders its keys; the old callers' keys (a Place) need not be ordered, so each piece is keyed by its position
    // and the positions found are turned back into the callers' keys.
    std::vector<cb::source::assembly::Piece<std::size_t>> mine;
    std::vector<const Key*> keys;
    for (const Piece<Key>& p : pieces) {
        cb::source::assembly::Piece<std::size_t> q;
        q.offset = static_cast<std::size_t>(std::get<0>(p));
        q.length = static_cast<std::size_t>(std::get<1>(p));
        if (std::get<2>(p)) q.mask = cb::compat::from_py(*std::get<2>(p));
        if (std::get<3>(p)) {
            q.key = keys.size();
            keys.push_back(&*std::get<3>(p));
        }
        q.inner_jump = std::get<4>(p);
        mine.push_back(std::move(q));
    }
    const cb::Bytes was = cb::compat::from_py(original), now = cb::compat::from_py(built);
    py::set<Key> out;
    for (const std::size_t at : cb::source::assembly::culprits<std::size_t>(was, now, mine)) out.add(*keys[at]);
    return out;
}

}  // namespace codebreaker::source::assembly
