// codebreaker/recompile/disassemble.hpp -- SHIM over cb/recompile/disassemble.hpp (docs/CPP_DESIGN.md 6.3): the old functions in
// py:: types, the new Disassembler called. Wired to the old vmx128 decoder and the old powerpc_image reader (the new module takes
// both as data and callbacks); deleted with its last py:: consumer.
//
//     disassemble::listing(code, base_va)              (va, raw word, mnemonic, operands), every word
//     disassemble::classify(code, base_va, entries)    offsets flow reaches from `entries`: code
//     disassemble::code_section(path)                  (bytes, address, entry) of an image's code
//     disassemble::main(argv)                          python -m codebreaker.recompile.disassemble <image or .xex> [--limit N]
//
// The Python generator `listing` is a list here. Each thread has its own Disassembler (an engine is not shared). Capstone is
// loaded by cb::disasm::capstone: set_capstone_dll() has nothing left to do.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/disassemble.hpp"
#include "codebreaker/decompile/vmx128.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"

namespace codebreaker::recompile::disassemble {

inline constexpr int64_t WORD = 4;

namespace shim_ {

namespace cbd = cb::recompile::disassemble;

// The Xenon's vector decoder, the old port's.
inline cbd::VectorDecoder vector_decoder() {
    return [](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
        const auto v = codebreaker::decompile::vmx128::decode(static_cast<int64_t>(word));
        if (!v) return std::nullopt;
        return std::make_pair(cb::compat::from_py(std::get<0>(*v)), cb::compat::from_py(std::get<1>(*v)));
    };
}

// This thread's disassembler.
inline cbd::Disassembler& mine() {
    thread_local cbd::Disassembler dis(vector_decoder());
    return dis;
}

inline std::vector<std::uint32_t> entry_list(const py::list<int64_t>& entries) {
    std::vector<std::uint32_t> out;
    for (const int64_t e : entries) out.push_back(static_cast<std::uint32_t>(e));
    return out;
}

// What the old image reader says, in the new module's terms.
inline cbd::ImageFacts facts_of(const py::str& path) {
    const py::str file = formats::xex::is_xex(path) ? formats::xex::basefile(path) : path;
    const auto img = formats::powerpc_image::read(file);
    cbd::ImageFacts f;
    f.data = cb::compat::from_py(img->data);
    f.base = static_cast<std::uint32_t>(img->base);
    f.entry = static_cast<std::uint32_t>(img->entry);
    f.code_begin = static_cast<std::uint32_t>(std::get<0>(img->code));
    f.code_end = static_cast<std::uint32_t>(std::get<1>(img->code));
    for (const auto& s : img->spans) f.chunks.push_back({static_cast<std::uint32_t>(std::get<0>(s)), static_cast<std::uint32_t>(std::get<1>(s)), static_cast<std::uint32_t>(std::get<2>(s))});
    for (const auto& s : img->sections) f.sections.push_back({cb::compat::from_py(std::get<0>(s)), static_cast<std::uint32_t>(std::get<1>(s)), static_cast<std::uint32_t>(std::get<2>(s)), static_cast<std::uint32_t>(std::get<3>(s))});
    return f;
}

}  // namespace shim_

inline void set_capstone_dll(const py::str&) {}

// A word in VMX128's primary opcodes (4, 5, 6) -- a vector instruction, not data.
inline bool is_vmx128(const py::bytes& raw4) {
    const int64_t op = raw4[0] >> 2;
    return op == 4 || op == 5 || op == 6;
}

// The byte offsets of `code` that flow reaches from `entries` (addresses).
inline py::set<int64_t> classify(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries) {
    return cb::compat::guard([&] {
        const cb::Bytes bytes = cb::compat::from_py(code);
        const std::vector<std::uint32_t> from = shim_::entry_list(entries);
        const cb::recompile::disassemble::Coverage reached = shim_::mine().classify(bytes, static_cast<std::uint32_t>(base_vaddr), from);
        py::set<int64_t> out;
        for (std::size_t i = 0; i < reached.size(); ++i) {
            if (reached[i]) out.add(static_cast<int64_t>(i));
        }
        return out;
    });
}

// [(start, end)] -- the runs of offsets flow never reaches.
inline py::list<std::tuple<int64_t, int64_t>> data_spans(const py::bytes& code, int64_t base_vaddr, const py::list<int64_t>& entries,
                                                         int64_t minrun = WORD) {
    return cb::compat::guard([&] {
        const cb::Bytes bytes = cb::compat::from_py(code);
        const std::vector<std::uint32_t> from = shim_::entry_list(entries);
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& s : shim_::mine().data_spans(bytes, static_cast<std::uint32_t>(base_vaddr), from, static_cast<std::size_t>(minrun))) {
            out.append(std::make_tuple(static_cast<int64_t>(s.begin), static_cast<int64_t>(s.end)));
        }
        return out;
    });
}

// (address, raw word, mnemonic, operands) for every word from `start`, `count` at most. A word neither decoder reads is `.long 0x...`.
inline py::list<std::tuple<int64_t, py::bytes, py::str, py::str>> listing(const py::bytes& code, int64_t base_vaddr, int64_t start = 0,
                                                                         std::optional<int64_t> count = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::Bytes bytes = cb::compat::from_py(code);
        std::optional<std::size_t> n;
        if (count) n = static_cast<std::size_t>(std::max<int64_t>(*count, 1));   // as the Python: a count of 0 or less still gives the first word
        py::list<std::tuple<int64_t, py::bytes, py::str, py::str>> out;
        for (const auto& row : shim_::mine().listing(bytes, static_cast<std::uint32_t>(base_vaddr), static_cast<std::size_t>(std::max<int64_t>(start, 0)), n)) {
            const auto raw = row.raw();
            out.append(std::make_tuple(static_cast<int64_t>(row.address), cb::compat::to_py(cb::ByteView(raw.data(), raw.size())),
                                       cb::compat::to_py(row.mnemonic), cb::compat::to_py(row.operands)));
        }
        return out;
    });
}

// (image bytes, base address, entry, sections [(name, address, size, executable)]) for a PowerPC image or a .xex (its basefile).
inline std::tuple<py::bytes, int64_t, int64_t, py::list<std::tuple<py::str, int64_t, int64_t, bool>>> image(const py::str& path) {
    return cb::compat::guard([&] {
        const auto flat = cb::recompile::disassemble::flatten(shim_::facts_of(path));
        py::list<std::tuple<py::str, int64_t, int64_t, bool>> sections;
        for (const auto& s : flat.sections) sections.append(std::make_tuple(cb::compat::to_py(s.name), static_cast<int64_t>(s.va), static_cast<int64_t>(s.size), s.executable));
        return std::make_tuple(cb::compat::to_py(cb::ByteView(flat.bytes)), static_cast<int64_t>(flat.base), static_cast<int64_t>(flat.entry), sections);
    });
}

// (bytes, address, entry) of the image's code section.
inline std::tuple<py::bytes, int64_t, int64_t> code_section(const py::str& path) {
    return cb::compat::guard([&] {
        const auto cs = cb::recompile::disassemble::code_section(shim_::facts_of(path));
        return std::make_tuple(cb::compat::to_py(cb::ByteView(cs.code)), static_cast<int64_t>(cs.address), static_cast<int64_t>(cs.entry));
    });
}

inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        const cb::recompile::disassemble::ImageReader reader = [](const std::filesystem::path& p) {
            return shim_::facts_of(cb::compat::to_py(cb::fs::to_text(p)));
        };
        return cb::recompile::disassemble::main(reader, shim_::vector_decoder(), args);
    });
}

}  // namespace codebreaker::recompile::disassemble
