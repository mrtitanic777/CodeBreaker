// codebreaker/source/ne_asm/fixups.hpp -- SHIM over cb/source/ne_asm/fixups.hpp (docs/CPP_DESIGN.md 6.3): an annotated 16-bit disassembly of an
// NE segment. The old types in (NEFile, py::str, py::dict), the new functions called. Deleted with its last py:: consumer.
//
// Differences from the old port that the shim cannot hide: the two listings go to CodeBreaker's stdout (cb::out), not to the old layer's;
// `build_fixup_map` answers in offset order (the old dict was in the order the chains were walked); a negative start or count is an
// OverflowError (the Python sliced from the end).
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/source/ne_asm/fixups.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::fixups {

namespace cbf = cb::source::ne_asm::fixups;

// Open a normal MZ/NE file, or a raw embedded NE module (starts with 'NE').
inline formats::ne::NEFile open_ne(const py::str& path) {
    py::bytes data;
    {
        py::file f = py::open_rb(path);
        data = f.read();
        f.close();
    }
    if (data.slice(0, 2) == py::bytes("MZ")) return formats::ne::NEFile(path);
    if (data.slice(0, 2) == py::bytes("NE")) return formats::ne::NEFile::from_buffer(data, 0, path);
    throw py::ValueError(path.utf8() + ": not MZ or NE");
}

// offset -> human label, by walking each relocation chain.
inline py::dict<int64_t, py::str> build_fixup_map(const formats::ne::NEFile& ne, const formats::ne::Segment& seg, const py::list<py::str>& mods) {
    return cb::compat::guard([&] {
        cb::formats::ne::Segment s;
        s.index = static_cast<uint16_t>(seg.idx);
        s.file_offset = static_cast<uint64_t>(seg.file_off);
        s.length = static_cast<uint32_t>(seg.length);
        s.flags = static_cast<uint16_t>(seg.flags);
        s.min_alloc = static_cast<uint32_t>(seg.minalloc);
        py::dict<int64_t, py::str> out;
        for (const auto& [offset, target] : cbf::build_fixup_map(ne.module(), s, cb::compat::from_py(mods))) out.set(offset, cb::compat::to_py(target.label));
        return out;
    });
}

inline void disasm_segment(const py::str& path, int64_t seg_index = 1, int64_t start = 0, const std::optional<int64_t>& count = std::nullopt) {
    cb::compat::guard([&] {
        const auto module = cbf::open_module(cb::fs::from_text(cb::compat::from_py(path)));
        cbf::ListingRange range;
        range.segment = cb::compat::int_from_py<std::size_t>(seg_index);
        range.start = cb::compat::int_from_py<uint32_t>(start);
        if (count) range.count = cb::compat::int_from_py<uint32_t>(*count);
        cb::out_text(cbf::annotated_listing(module, cb::compat::from_py(path), range));
    });
}

inline void disasm_entry(const py::str& path, int64_t ordinal) {
    cb::compat::guard([&] {
        const auto module = cbf::open_module(cb::fs::from_text(cb::compat::from_py(path)));
        cb::out_text(cbf::entry_listing(module, cb::compat::from_py(path), cb::compat::int_from_py<uint32_t>(ordinal)));
    });
}

// the script's command line: argv is sys.argv[1:]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t { return cbf::main(cb::compat::from_py(argv)); });
}

}  // namespace codebreaker::source::ne_asm::fixups
