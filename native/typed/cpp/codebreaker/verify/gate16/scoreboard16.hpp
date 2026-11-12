// codebreaker/verify/gate16/scoreboard16.hpp -- COMPAT SHIM over cb/verify/gate16/scoreboard16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/gate16/scoreboard16.{hpp,cpp}: OpenWatcom's half, the rows, the report and the board's JSON. This header
// keeps the old py:: API, header-only: the Microsoft C 8 half is still the old match16::run (its `Last`), turned here into the new module's
// inputs (functions with the original's bytes, match16's result), and the new rows turned back into the old `Row`.
//
// DIVERGES: the rows are in the order of the module's functions, not match16's dict order; the board's JSON is written by nlohmann's
// dump(indent=1), not Python's json.dump (the same keys and values; whitespace may differ); `--show` of a function not on the saved board
// raises std::out_of_range, as the Python raised KeyError.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/verify/gate16/scoreboard16.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/msc6.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "codebreaker/verify/authored/match16.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::gate16::scoreboard16 {

namespace nw = cb::verify::gate16::scoreboard16;
namespace match16 = codebreaker::verify::authored::match16;

using Log = std::function<void(const py::str&)>;
inline void log_print(const py::str& line) { py::print(line); }

namespace detail_shim {

inline nw::Log log_of(const Log& log) {
    if (!log) return {};
    return [log](std::string_view line) { log(cb::compat::to_py(line)); };
}
inline nw::Masked masked_of(const build::msc6::Code& c) { return {cb::compat::from_py(std::get<0>(c)), cb::compat::from_py(std::get<1>(c))}; }
inline build::msc6::Code old_code(const nw::Masked& m) { return {cb::compat::to_py(cb::ByteView(m.code)), cb::compat::to_py(cb::ByteView(m.mask))}; }
inline cb::build::watcom::OpenWatcom open_watcom() { return cb::build::watcom::OpenWatcom(std::filesystem::u8path(build::watcom::openwatcom_root().utf8())); }
}  // namespace detail_shim

// what the decompiler writes that a programmer does not, and that a compiler turns into more code: (name, pattern), in order
inline const py::list<std::tuple<py::str, py::re::Pattern>>& PATTERNS() {
    static const py::list<std::tuple<py::str, py::re::Pattern>> p = {
        {py::str("CONCAT"), py::re::compile(py::str("\\bCONCAT\\d\\d\\("))},
        {py::str("MK_FP"), py::re::compile(py::str("\\bMK_FP\\("))},
        {py::str("segment helper"), py::re::compile(py::str("\\bcodebreaker_(ss|ds|cs|es)\\(\\)"))},
        {py::str("split far (>> 0x10)"), py::re::compile(py::str(">>\\s*0x10\\b"))},
        {py::str("xunknown"), py::re::compile(py::str("\\bxunknown\\d\\b"))},
        {py::str("unaff_/in_/extraout_"), py::re::compile(py::str("\\b(unaff|in|extraout)_[A-Z]{2}\\b"))},
        {py::str("SUB/ZEXT/SEXT"), py::re::compile(py::str("\\b(SUB|ZEXT|SEXT)\\d\\d\\("))},
        {py::str("constant DS address"), py::re::compile(py::str("\\*\\s*\\(\\s*\\w+\\s*\\*\\s*\\)\\s*0x[0-9a-f]{2,4}\\b"))},
    };
    return p;
}

// OpenWatcom's switches for Microsoft's (tests/msc8/suite.py keeps the same mapping)
inline py::list<py::str> watcom_flags(const py::str& ms) {
    py::list<py::str> out;
    for (const std::string& f : nw::watcom_flags(ms.utf8())) out.append(cb::compat::to_py(f));
    return out;
}

// the code alike, a fix-up's bytes (either side's) aside
inline bool _same(const build::msc6::Code& a, const build::msc6::Code& b) { return nw::same_code(detail_shim::masked_of(a), detail_shim::masked_of(b)); }

// {function: (bytes, mask) or None}: each source file compiled by OpenWatcom -zmsc as Microsoft C compiled it (the same functions
// together, the same switches); the second: {function: why it was refused}.
inline std::tuple<py::dict<py::str, std::optional<build::msc6::Code>>, py::dict<py::str, py::str>> _watcom(
    const py::list<py::list<py::str>>& files, const py::dict<py::str, py::str>& texts, const py::str& header,
    const py::dict<py::str, match16::Setting>& settings_of, const Log& log) {
    std::vector<std::vector<std::string>> new_files;
    for (const py::list<py::str>& f : files) {
        new_files.emplace_back();
        for (const py::str& n : f) new_files.back().push_back(n.utf8());
    }
    std::map<std::string, std::string> new_texts;
    for (const auto& kv : texts.items()) new_texts[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    const nw::WatcomCode got = nw::compile_watcom(
        detail_shim::open_watcom(), new_files, new_texts, header.utf8(),
        [&](const std::string& n) { return std::get<1>(settings_of.at(cb::compat::to_py(n))).utf8(); }, detail_shim::log_of(log));
    py::dict<py::str, std::optional<build::msc6::Code>> code;
    py::dict<py::str, py::str> refused;
    for (const auto& [n, c] : got.code) code.set(cb::compat::to_py(n), c ? std::optional<build::msc6::Code>(detail_shim::old_code(*c)) : std::nullopt);
    for (const auto& [n, why] : got.refused) refused.set(cb::compat::to_py(n), cb::compat::to_py(why));
    return {code, refused};
}

struct HexBytes {
    py::str original, msc8, watcom;
};

struct Row {
    std::optional<int64_t> segment;
    int64_t original = 0;
    std::optional<int64_t> msc8;
    py::str msc8_vs_original;
    int64_t msc8_bytes_off = 0;
    std::optional<int64_t> watcom;
    py::str watcom_vs_msc8;               // "same", "differs" or "n/a"
    bool fits = false;
    py::list<std::tuple<py::str, int64_t>> patterns;
    bool library = false;
    py::str c;
    HexBytes hex;
};

using Rows = py::dict<py::str, Row>;

namespace detail_shim {
inline Row old_row(const nw::Row& r) {
    Row o;
    if (r.segment) o.segment = static_cast<int64_t>(*r.segment);
    o.original = static_cast<int64_t>(r.original);
    if (r.msc8) o.msc8 = static_cast<int64_t>(*r.msc8);
    o.msc8_vs_original = cb::compat::to_py(cb::verify::word(r.msc8_vs_original));
    o.msc8_bytes_off = static_cast<int64_t>(r.msc8_bytes_off);
    if (r.watcom) o.watcom = static_cast<int64_t>(*r.watcom);
    o.watcom_vs_msc8 = cb::compat::to_py(nw::word(r.watcom_vs_msc8));
    o.fits = r.fits;
    for (const auto& p : r.patterns) o.patterns.append(std::make_tuple(cb::compat::to_py(p.first), static_cast<int64_t>(p.second)));
    o.library = r.library;
    o.c = cb::compat::to_py(r.c);
    o.hex = {cb::compat::to_py(r.hex.original), cb::compat::to_py(r.hex.msc8), cb::compat::to_py(r.hex.watcom)};
    return o;
}
inline nw::Row new_row(const Row& o) {
    nw::Row r;
    if (o.segment) r.segment = static_cast<std::uint32_t>(*o.segment);
    r.original = static_cast<std::size_t>(o.original);
    if (o.msc8) r.msc8 = static_cast<std::size_t>(*o.msc8);
    r.msc8_vs_original = cb::verify::parse_byte_match(o.msc8_vs_original.utf8()).value_or(cb::verify::ByteMatch::NoCompile);
    r.msc8_bytes_off = static_cast<std::size_t>(o.msc8_bytes_off);
    if (o.watcom) r.watcom = static_cast<std::size_t>(*o.watcom);
    const std::string agree = o.watcom_vs_msc8.utf8();
    r.watcom_vs_msc8 = agree == "same" ? nw::Agreement::Same : agree == "differs" ? nw::Agreement::Differs : nw::Agreement::NotApplicable;
    r.fits = o.fits;
    for (const auto& p : o.patterns) r.patterns.emplace_back(std::get<0>(p).utf8(), static_cast<std::size_t>(std::get<1>(p)));
    r.library = o.library;
    r.c = o.c.utf8();
    r.hex = {o.hex.original.utf8(), o.hex.msc8.utf8(), o.hex.watcom.utf8()};
    return r;
}
inline nw::Rows new_rows(const Rows& rows) {
    nw::Rows out;
    for (const auto& kv : rows.items()) out.insert_or_assign(std::get<0>(kv).utf8(), new_row(std::get<1>(kv)));
    return out;
}

// match.distance as the new module's Toolchain
class OldToolchain final : public cb::verify::authored::match16::Toolchain {
  public:
    std::vector<std::string> flag_sets(const std::string&) const override { return {}; }
    std::map<std::string, std::optional<cb::verify::authored::match16::Masked>> compile(const cb::verify::authored::match16::Compilation&) override { return {}; }
    cb::verify::authored::match16::Verdict distance(const cb::verify::authored::match16::Masked& built,
                                                    const cb::verify::authored::match16::Masked& original) const override {
        const auto d = authored::match::distance(authored::match::Masked(cb::compat::to_py(cb::ByteView(built.code)), authored::match::Mask(cb::compat::to_py(cb::ByteView(built.mask)))),
                                                 authored::match::Masked(cb::compat::to_py(cb::ByteView(original.code)), authored::match::Mask(cb::compat::to_py(cb::ByteView(original.mask)))));
        return {cb::verify::parse_byte_match(std::get<0>(d).utf8()).value_or(cb::verify::ByteMatch::NoCompile), static_cast<std::size_t>(std::get<1>(d)),
                static_cast<std::size_t>(std::get<2>(d))};
    }
};
}  // namespace detail_shim

inline Rows run(const py::str& module_path, const std::optional<int64_t>& segment, const Log& log, const match16::Prepare16& win16_prepare,
                const match16::SpansFn& win16_spans) {
    match16::Options options;
    options.log = log ? log : Log();
    options.fold = false;   // (the C as the decompiler wrote it, as the build has it)
    auto [chosen, results] = match16::run(module_path, options, win16_prepare, win16_spans);
    (void)chosen;
    const match16::Last& last = match16::last();
    namespace m16 = cb::verify::authored::match16;
    std::vector<m16::Function> functions;
    m16::Result result;
    std::map<std::string, std::string> texts;
    for (const py::str& n : results.keys()) {
        if (!last.code.contains(n)) continue;
        const std::string name = n.utf8();
        const auto& code = last.code.at(n);
        const auto seg = last.segment.contains(n) ? std::optional<int64_t>(last.segment.at(n)) : std::nullopt;
        functions.push_back({name, static_cast<std::uint32_t>(seg.value_or(0)), 0, detail_shim::masked_of(std::get<1>(code))});
        // (a function with no segment in the old board is never of the segment asked for)
        if (segment && (!seg || *seg != *segment)) functions.pop_back();
        result.code[name] = std::get<0>(code) ? std::optional<m16::Masked>(detail_shim::masked_of(*std::get<0>(code))) : std::nullopt;
        result.verdicts[name] = {cb::verify::parse_byte_match(std::get<0>(results.at(n)).utf8()).value_or(cb::verify::ByteMatch::NoCompile),
                                 static_cast<std::size_t>(std::get<1>(results.at(n))), static_cast<std::size_t>(std::get<2>(results.at(n)))};
        if (last.setting.contains(n)) result.setting[name] = {std::get<0>(last.setting.at(n)).utf8(), std::get<1>(last.setting.at(n)).utf8()};
        if (last.texts.contains(n)) texts[name] = last.texts.at(n).utf8();
    }
    for (const py::list<py::str>& f : last.files) {
        result.files.emplace_back();
        for (const py::str& n : f) result.files.back().push_back(n.utf8());
    }
    const cb::build::watcom::OpenWatcom ow = detail_shim::open_watcom();
    const detail_shim::OldToolchain chain;
    nw::Inputs in;
    in.functions = functions;
    in.result = &result;
    in.texts = &texts;
    in.header = last.header.value_or(py::str("")).utf8();
    in.watcom = &ow;
    in.toolchain = &chain;
    if (segment) in.segment = static_cast<std::uint32_t>(*segment);
    in.log = detail_shim::log_of(log);
    const nw::Rows rows = nw::run(in);
    Rows out;
    for (const auto& [n, r] : rows) out.set(cb::compat::to_py(n), detail_shim::old_row(r));
    return out;
}

inline void report(const Rows& rows, const Log& out = log_print) {
    for (const std::string& line : nw::report_lines(detail_shim::new_rows(rows))) out(cb::compat::to_py(line));
}

// a function's C, and its original's, Microsoft C 8's and OpenWatcom's instructions side by side
inline void show(const Row& row, const Log& out = log_print) {
    for (const std::string& line : nw::show_lines(detail_shim::new_row(row))) out(cb::compat::to_py(line));
}

// json.dump(rows, f, indent=1)
inline py::str rows_json(const Rows& rows) { return cb::compat::to_py(nw::rows_to_json(detail_shim::new_rows(rows))); }

inline const py::str DOC = py::str(std::string(nw::kUsage));

inline int64_t main(const py::list<py::str>& argv, const match16::Prepare16& win16_prepare, const match16::SpansFn& win16_spans) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    const std::optional<nw::Arguments> parsed = nw::parse_arguments(args);
    if (!parsed) {
        py::print(DOC);
        return 2;
    }
    if (!parsed->show.empty()) {
        const nw::Rows rows = nw::rows_from_json(cb::fs::read_text(std::filesystem::u8path(parsed->json)));
        for (const std::string& fn : parsed->show) {
            py::print(py::str("====="), cb::compat::to_py(fn));
            for (const std::string& line : nw::show_lines(rows.at(fn))) py::print(cb::compat::to_py(line));
        }
        return 0;
    }
    const py::str given = cb::compat::to_py(parsed->module);
    std::optional<py::str> path = py::os::path::isfile(given) ? std::optional<py::str>(given) : base::corpus::retail(given);
    if (!path) throw py::SystemExit(py::percent(py::str("%s: not a module of the corpus"), given));
    std::optional<int64_t> seg;
    if (parsed->segment) seg = static_cast<int64_t>(*parsed->segment);
    Rows rows = run(*path, seg, log_print, win16_prepare, win16_spans);
    report(rows);
    if (!parsed->json.empty()) cb::fs::write_text(std::filesystem::u8path(parsed->json), nw::rows_to_json(detail_shim::new_rows(rows)));
    return 0;
}

}  // namespace codebreaker::verify::gate16::scoreboard16
