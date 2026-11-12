// codebreaker/source/ne_asm/datacode.hpp -- SHIM over cb/source/ne_asm/datacode.hpp (docs/CPP_DESIGN.md 6.3): separate DATA from CODE in a
// segment (strings, and the clusters of nonsense instructions objdump makes of data). The old types in (py::bytes, the old Seg), the new
// functions called, the old types out. Deleted with its last py:: consumer.
//
//     python datacode.py MOUSE --seg 1        # list the data regions found
//
// Differences from the old port that the shim cannot hide:
//   * `main` parses its command line the way CodeBreaker's commands do (no argparse wording or abbreviations) and writes to
//     CodeBreaker's stdout/stderr; the streams of `set_streams` are no longer used.
//   * `_texty` is not offered (an internal of the detector).
#pragma once

#include <cstdint>
#include <iosfwd>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/ne_asm/datacode.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::datacode {

namespace cbs = cb::source::ne_asm::datacode;

inline constexpr int64_t MINSTR = 5;   // min printable run to call it a string

// (start, end, kind)
using Region = std::tuple<int64_t, int64_t, py::str>;

namespace shim_ {

inline std::vector<cb::source::ne_asm::segment::Row> rows_of(const py::dict<int64_t, py::str>& rows) {
    std::vector<cb::source::ne_asm::segment::Row> out;
    for (const auto& kv : rows.items()) out.push_back({cb::compat::int_from_py<uint32_t>(std::get<0>(kv)), cb::compat::from_py(std::get<1>(kv))});
    return out;
}

inline Region region_of(const cbs::Region& r) { return Region(int64_t{r.start}, int64_t{r.end}, py::str(std::string(cbs::kind_name(r.kind)))); }

}  // namespace shim_

// [(start, end, text)] for printable-ASCII runs that look like text.
inline py::list<std::tuple<int64_t, int64_t, py::str>> find_strings(const py::bytes& code, int64_t minlen = MINSTR) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t, py::str>> out;
        const cb::Bytes bytes = cb::compat::from_py(code);
        for (const cbs::TextRun& s : cbs::find_strings(bytes, cb::compat::int_from_py<std::size_t>(minlen))) {
            out.append(std::make_tuple(int64_t{s.start}, int64_t{s.end}, cb::compat::to_py(s.text)));
        }
        return out;
    });
}

// [(start, end)] over runs of >=3 consecutive rows whose mnemonic is the kind objdump only produces over data.
inline py::list<std::tuple<int64_t, int64_t>> garbage_spans(const py::dict<int64_t, py::str>& rows, const py::bytes&) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const cbs::Region& r : cbs::garbage_spans(shim_::rows_of(rows))) out.append(std::make_tuple(int64_t{r.start}, int64_t{r.end}));
        return out;
    });
}

// Merge string + garbage detectors into sorted, non-overlapping data spans.
inline py::list<Region> data_regions(const segment::Seg& sg) {
    return cb::compat::guard([&] {
        py::dict<int64_t, py::str> rows;
        for (const auto& oa : sg.rows) rows.set(std::get<0>(oa), std::get<1>(oa));
        py::list<Region> out;
        const cb::Bytes code = cb::compat::from_py(sg.code);
        for (const cbs::Region& r : cbs::data_regions(code, shim_::rows_of(rows))) out.append(shim_::region_of(r));
        return out;
    });
}

inline void set_streams(std::ostream&, std::ostream&) {}

// the script's command line: argv is sys.argv (argv[0] the program's name)
inline void main(const py::list<py::str>& argv) {
    cb::compat::guard([&] {
        std::vector<std::string> args;
        for (int64_t i = 1; i < argv.size(); ++i) args.push_back(cb::compat::from_py(argv[i]));
        const int code = cbs::main(::codebreaker::base::corpus::detail::instance(), args);
        if (code != 0) throw py::SystemExit(code);
    });
}

}  // namespace codebreaker::source::ne_asm::datacode
