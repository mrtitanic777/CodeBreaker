// codebreaker/verify/gate16/scoreboard16.cpp -- typed C++ port of codebreaker.verify.gate16.scoreboard16 (see scoreboard16.hpp).

#include "codebreaker/verify/gate16/scoreboard16.hpp"

#include <algorithm>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/msc6.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/verify/authored/match.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::verify::gate16::scoreboard16 {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace match = codebreaker::verify::authored::match;
namespace match16 = codebreaker::verify::authored::match16;
using codebreaker::build::msc6::Code;

str S(const char* s) { return str(s); }

match::Masked masked_(const Code& c) { return match::Masked(std::get<0>(c), match::Mask(std::get<1>(c))); }

// match._trim of a Code
Code trim_(const Code& c) { return match::_trim(std::get<0>(c), std::get<1>(c)); }

void put_str_(std::string& o, const str& s) { py::json::put(o, s); }

}  // namespace

void log_print(const str& line) { py::print(line); }

const py::list<std::tuple<str, py::re::Pattern>>& PATTERNS() {
    static const py::list<std::tuple<str, py::re::Pattern>> p = {
        {S("CONCAT"), py::re::compile(S("\\bCONCAT\\d\\d\\("))},
        {S("MK_FP"), py::re::compile(S("\\bMK_FP\\("))},
        {S("segment helper"), py::re::compile(S("\\bcodebreaker_(ss|ds|cs|es)\\(\\)"))},
        {S("split far (>> 0x10)"), py::re::compile(S(">>\\s*0x10\\b"))},
        {S("xunknown"), py::re::compile(S("\\bxunknown\\d\\b"))},
        {S("unaff_/in_/extraout_"), py::re::compile(S("\\b(unaff|in|extraout)_[A-Z]{2}\\b"))},
        {S("SUB/ZEXT/SEXT"), py::re::compile(S("\\b(SUB|ZEXT|SEXT)\\d\\d\\("))},
        {S("constant DS address"), py::re::compile(S("\\*\\s*\\(\\s*\\w+\\s*\\*\\s*\\)\\s*0x[0-9a-f]{2,4}\\b"))},
    };
    return p;
}

py::list<str> watcom_flags(const str& ms) {
    py::list<str> out = {S("-bt=dos"), S("-zq"), S("-w0"), S("-zl"), S("-ecc"), S("-zu"), S("-zmsc"), S("-j"), S("-zastd=c99")};   // (dos: no Windows prolog -- /Gw gives -zW)
    for (str f : py::split(ms)) {
        f = f.lstrip(S("/-"));
        if (f.startswith(S("A"))) {
            str m = S("-mm");
            if (f == S("AS")) m = S("-ms");
            else if (f == S("AM")) m = S("-mm");
            else if (f == S("AC")) m = S("-mc");
            else if (f == S("AL")) m = S("-ml");
            out.append(m);
        } else if (f.startswith(S("G"))) {
            for (const str& c : f.slice_from(1)) {
                if (c == S("2")) out.append(S("-2"));
                else if (c == S("3")) out.append(S("-3"));
                else if (c == S("s")) out.append(S("-s"));
                else if (c == S("w")) out.append(S("-zW"));
            }
        } else if (f.startswith(S("O"))) {
            const str opts = f.slice_from(1);
            out.append(opts == S("d") ? S("-od") : opts.contains(S("x")) ? S("-obmilr") : S("-o") + opts);
            if (opts.contains(S("s")) && opts.contains(S("x"))) out.append(S("-os"));
        }
    }
    return out;
}

bool _same(const Code& a, const Code& b) {
    const py::bytes &ca = std::get<0>(a), &ma = std::get<1>(a), &cb = std::get<0>(b), &mb = std::get<1>(b);
    if (ca.size() != cb.size()) return false;
    for (int64_t i = 0; i < ca.size(); ++i)
        if (!(ca[i] == cb[i] || ma[i] || mb[i])) return false;
    return true;
}

std::tuple<py::dict<str, std::optional<Code>>, py::dict<str, str>> _watcom(const py::list<py::list<str>>& files, const py::dict<str, str>& texts,
                                                                          const str& header, const py::dict<str, match16::Setting>& settings_of,
                                                                          const Log& log) {
    const str wcc = build::watcom::ow_tool(S("wcc"));
    const py::dict<str, str> env = build::watcom::ow_env(S("ne"));
    const str work = py::tempfile::mkdtemp(std::nullopt, std::optional<str>(S("score16_")));
    {
        py::file f = py::open(py::os::path::join(work, S("m.h")), S("w"), std::optional<str>(S("utf-8")), std::nullopt, std::optional<str>(S("\n")));
        f.write(header);
    }
    py::dict<str, std::optional<Code>> out;
    py::dict<str, str> why;

    auto one = [&](const str& stem, const py::list<str>& names) -> std::tuple<std::optional<py::bytes>, str> {
        {
            py::list<str> parts;
            for (const str& n : names) parts.append(texts.at(n));
            py::file f = py::open(py::os::path::join(work, stem + S(".c")), S("w"), std::optional<str>(S("utf-8")), std::nullopt, std::optional<str>(S("\n")));
            f.write(S("#include \"m.h\"\n\n") + py::join(S("\n\n"), parts) + S("\n"));
        }
        py::list<str> args = {wcc};
        for (const str& a : watcom_flags(std::get<1>(settings_of.at(names[0])))) args.append(a);
        args.append(S("-fo=") + stem + S(".obj"));
        args.append(stem + S(".c"));
        py::subprocess::RunOptions o;
        o.cwd = work;
        o.env = env;
        o.encoding = "cp1252";
        o.errors = "replace";
        py::subprocess::CompletedProcess r = py::subprocess::run(args, o);
        const str obj = py::os::path::join(work, stem + S(".obj"));
        if (r.returncode == 0 && py::os::path::isfile(obj)) {
            py::file f = py::open(obj, S("rb"));
            py::bytes data = f.read();
            f.close();
            return {data, S("")};
        }
        const str all = r.stdout_.value_or(S("")) + r.stderr_.value_or(S(""));
        for (const str& ln : py::splitlines(all))
            if (ln.contains(S("Error!"))) return {std::nullopt, ln};
        return {std::nullopt, all.strip().slice(-200, all.strip().size())};
    };

    int64_t k = 0;
    for (const py::list<str>& file : files) {
        py::list<str> names;
        for (const str& n : file)
            if (texts.contains(n)) names.append(n);
        if (names.empty()) {
            ++k;
            continue;
        }
        auto [data, err] = one(py::percent(S("f%04d"), k), names);
        if (!data && names.size() > 1) {   // (one function's error does not sink its file)
            int64_t j = 0;
            for (const str& n : names) {
                auto [d, e] = one(py::percent(S("f%04d_%02d"), k, j), py::list<str>{n});
                out.set(n, d ? build::msc6::function_code(*d, n) : std::nullopt);
                if (!d) why.set(n, e);
                ++j;
            }
            ++k;
            continue;
        }
        for (const str& n : names) {
            out.set(n, data ? build::msc6::function_code(*data, n) : std::nullopt);
            if (!data) why.set(n, err);
        }
        ++k;
    }
    if (log && !why.empty()) {
        const str first = why.keys()[0];
        log(py::percent(S("  OpenWatcom refused %d functions, e.g. %s: %s"), static_cast<int64_t>(why.size()), first, why.at(first)));
    }
    return {out, why};
}

Rows run(const str& module_path, const std::optional<int64_t>& segment, const Log& log, const match16::Prepare16& win16_prepare,
         const match16::SpansFn& win16_spans) {
    match16::Options options;
    if (log) options.log = log;
    else options.log = nullptr;
    options.fold = false;   // (the C as the decompiler wrote it, as the build has it)
    auto [chosen, results] = match16::run(module_path, options, win16_prepare, win16_spans);
    (void)chosen;
    const match16::Last& last = match16::last();
    py::list<str> names;
    for (const str& n : results.keys())
        if (last.code.contains(n)) names.append(n);
    const py::dict<str, str>& texts = last.texts;
    const py::dict<str, int64_t>& seg_of = last.segment;
    if (segment) {
        py::list<str> kept;
        for (const str& n : names)
            if (seg_of.contains(n) && seg_of.at(n) == *segment) kept.append(n);
        names = kept;
    }
    py::list<py::list<str>> files;
    for (const py::list<str>& f : last.files) {
        py::list<str> g;
        for (const str& n : f)
            if (names.contains(n)) g.append(n);
        files.append(g);
    }
    py::list<py::list<str>> nonempty;
    for (const auto& f : files)
        if (!f.empty()) nonempty.append(f);
    auto [ow, refused] = _watcom(nonempty, texts, last.header.value_or(S("")), last.setting, log);
    (void)refused;
    Rows rows;
    for (const str& n : names) {
        const auto& code = last.code.at(n);
        const std::optional<Code>& ms = std::get<0>(code);
        const Code& orig = std::get<1>(code);
        std::optional<Code> o = ow.contains(n) ? ow.at(n) : std::nullopt;
        std::tuple<str, int64_t, int64_t> msv = ms ? [&] {
            auto d = match::distance(masked_(*ms), masked_(orig));
            return std::make_tuple(std::get<0>(d), std::get<1>(d), std::get<2>(d));
        }() : std::make_tuple(V::NO_COMPILE, int64_t(0), int64_t(0));
        const int64_t orig_len = std::get<0>(trim_(orig)).size();
        Row r;
        if (seg_of.contains(n)) r.segment = seg_of.at(n);
        r.original = orig_len;
        if (ms) r.msc8 = std::get<0>(trim_(*ms)).size();
        r.msc8_vs_original = std::get<0>(msv);
        r.msc8_bytes_off = std::get<1>(msv);
        if (o) r.watcom = std::get<0>(trim_(*o)).size();
        r.watcom_vs_msc8 = (ms && o && _same(trim_(*o), trim_(*ms))) ? S("same") : (ms && o) ? S("differs") : S("n/a");
        r.fits = o && static_cast<int64_t>(std::get<0>(trim_(*o)).size()) <= orig_len;
        const str text = texts.get(n, S(""));
        for (const auto& pat : PATTERNS()) {
            const int64_t k = static_cast<int64_t>(std::get<1>(pat).findall(text).size());
            if (std::get<1>(pat).search(text)) r.patterns.append(std::make_tuple(std::get<0>(pat), k));
        }
        r.library = V::match(std::get<0>(results.at(n))) == V::IDENTIFIED;
        r.c = text;
        r.hex.original = std::get<0>(orig).hex();
        r.hex.msc8 = ms ? std::get<0>(*ms).hex() : S("");
        r.hex.watcom = o ? std::get<0>(*o).hex() : S("");
        rows.set(n, r);
    }
    return rows;
}

void report(const Rows& rows, const Log& out) {
    std::vector<Row> c;
    for (const Row& r : rows.values())
        if (!r.library) c.push_back(r);
    const int64_t n = static_cast<int64_t>(c.size());
    int64_t ident = 0, same_ow = 0, fits = 0, off = 0;
    for (const Row& r : c) {
        ident += V::match(r.msc8_vs_original) == V::IDENTICAL;
        same_ow += r.watcom_vs_msc8 == S("same");
        fits += r.fits;
        off += std::max<int64_t>(0, r.watcom.value_or(0) - r.original);
    }
    out(py::percent(S("%d functions (runtime library aside)"), n));
    out(py::percent(S("  C        Microsoft C 8 gives the original's bytes: %4d   (decompiler's work: %d)"), ident, n - ident));
    out(py::percent(S("  compiler OpenWatcom -zmsc gives Microsoft C 8's:   %4d   (compiler's work: %d)"), same_ow, n - same_ow));
    out(py::percent(S("  fits     OpenWatcom's no longer than the original:  %4d   (would move: %d)"), fits, n - fits));
    out(py::percent(S("  bytes over the original's length, all together: %d"), off));
    std::vector<std::tuple<str, int64_t, int64_t>> tally;   // pattern, functions, bytes off
    for (const Row& r : c) {
        if (V::match(r.msc8_vs_original) == V::IDENTICAL) continue;
        for (const auto& p : r.patterns) {
            auto it = std::find_if(tally.begin(), tally.end(), [&](const auto& t) { return std::get<0>(t) == std::get<0>(p); });
            if (it == tally.end()) {
                tally.emplace_back(std::get<0>(p), 0, 0);
                it = tally.end() - 1;
            }
            std::get<1>(*it) += 1;
            std::get<2>(*it) += r.msc8_bytes_off;
        }
    }
    out(S("  the decompiler's patterns in functions Microsoft C 8 does not match (functions, bytes off):"));
    std::stable_sort(tally.begin(), tally.end(), [](const auto& a, const auto& b) { return std::get<2>(a) > std::get<2>(b); });
    for (const auto& t : tally) out(py::percent(S("    %-24s %4d  %6d"), std::get<0>(t), std::get<1>(t), std::get<2>(t)));
}

void show(const Row& row, const Log& out) {
    const match::Decoder& md = match::decoder(16);
    out(row.c);
    std::vector<std::vector<str>> cols;
    for (const py::str* hex : {&row.hex.original, &row.hex.msc8, &row.hex.watcom}) {
        std::vector<str> col;
        for (const match::Insn& i : md(py::bytes::fromhex(*hex), 0)) col.push_back(py::percent(S("%s %s"), i.mnemonic, i.op_str));
        cols.push_back(col);
    }
    out(py::percent(S("  %-32s| %-32s| %s"), S("original"), S("Microsoft C 8"), S("OpenWatcom -zmsc")));
    size_t most = 0;
    for (const auto& c : cols) most = std::max(most, c.size());
    for (size_t i = 0; i < most; ++i)
        out(py::percent(S("  %-32s| %-32s| %s"), i < cols[0].size() ? cols[0][i] : S(""), i < cols[1].size() ? cols[1][i] : S(""),
                        i < cols[2].size() ? cols[2][i] : S("")));
}

py::str rows_json(const Rows& rows) {
    std::string o;
    if (rows.empty()) return S("{}");
    o += "{\n";
    bool first = true;
    auto opt = [](const std::optional<int64_t>& v) { return v ? std::to_string(*v) : std::string("null"); };
    for (const auto& kv : rows.items()) {
        const Row& r = std::get<1>(kv);
        o += first ? " " : ",\n ";
        first = false;
        put_str_(o, std::get<0>(kv));
        o += ": {\n";
        o += "  \"segment\": " + opt(r.segment) + ",\n";
        o += "  \"original\": " + std::to_string(r.original) + ",\n";
        o += "  \"msc8\": " + opt(r.msc8) + ",\n";
        o += "  \"msc8_vs_original\": ";
        put_str_(o, r.msc8_vs_original);
        o += ",\n  \"msc8_bytes_off\": " + std::to_string(r.msc8_bytes_off) + ",\n";
        o += "  \"watcom\": " + opt(r.watcom) + ",\n";
        o += "  \"watcom_vs_msc8\": ";
        put_str_(o, r.watcom_vs_msc8);
        o += std::string(",\n  \"fits\": ") + (r.fits ? "true" : "false") + ",\n";
        o += "  \"patterns\": ";
        if (r.patterns.empty()) {
            o += "{}";
        } else {
            o += "{\n";
            bool f2 = true;
            for (const auto& p : r.patterns) {
                o += f2 ? "   " : ",\n   ";
                f2 = false;
                put_str_(o, std::get<0>(p));
                o += ": " + std::to_string(std::get<1>(p));
            }
            o += "\n  }";
        }
        o += std::string(",\n  \"library\": ") + (r.library ? "true" : "false") + ",\n";
        o += "  \"c\": ";
        put_str_(o, r.c);
        o += ",\n  \"hex\": {\n   \"original\": ";
        put_str_(o, r.hex.original);
        o += ",\n   \"msc8\": ";
        put_str_(o, r.hex.msc8);
        o += ",\n   \"watcom\": ";
        put_str_(o, r.hex.watcom);
        o += "\n  }\n }";
    }
    o += "\n}";
    return str(o);
}

const str DOC = S(
    "python -m codebreaker score16 <16-bit module> [--segment N] [--json FILE]\n"
    "python -m codebreaker score16 --show FUNCTION --json FILE   (one function from a saved board)");

namespace {

// a saved board's row: what show() reads of it
Row row_of_json_(const knowledge::sources::Json& j) {
    Row r;
    r.c = j.dict.at(S("c")).s;
    const auto& h = j.dict.at(S("hex"));
    r.hex.original = h.dict.at(S("original")).s;
    r.hex.msc8 = h.dict.at(S("msc8")).s;
    r.hex.watcom = h.dict.at(S("watcom")).s;
    return r;
}

}  // namespace

int64_t main(const py::list<str>& argv, const match16::Prepare16& win16_prepare, const match16::SpansFn& win16_spans) {
    auto after = [&](const char* flag) -> str {
        for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
            if (argv[i] == S(flag)) {
                if (i + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
                return argv[i + 1];
            }
        return S("");
    };
    if (argv.contains(S("--show"))) {
        if (!argv.contains(S("--json"))) throw py::ValueError("list.index(x): x not in list");   // (argv.index("--json"))
        const knowledge::sources::Json rows = knowledge::sources::_json(after("--json"));
        for (const str& fn : py::split(after("--show"), S(","))) {
            py::print(S("====="), fn);
            if (!rows.dict.contains(fn)) throw py::KeyError(py::repr(fn).utf8());
            show(row_of_json_(rows.dict.at(fn)));
        }
        return 0;
    }
    std::optional<int64_t> seg;
    if (argv.contains(S("--segment"))) seg = py::int_parse(after("--segment"));
    std::optional<str> js;
    if (argv.contains(S("--json"))) js = after("--json");
    py::list<str> args;
    for (const str& a : argv)
        if (!a.startswith(S("--")) && !(a == (seg ? py::to_str(*seg) : S("None"))) && !(js && a == *js)) args.append(a);
    if (args.empty()) throw py::IndexError("list index out of range");
    std::optional<str> path = py::os::path::isfile(args[0]) ? std::optional<str>(args[0]) : base::corpus::retail(args[0]);
    if (!path) throw py::TypeError("no module");
    Rows rows = run(*path, seg, log_print, win16_prepare, win16_spans);
    report(rows);
    if (js) {
        py::file f = py::open(*js, S("w"));
        f.write(rows_json(rows));
    }
    return 0;
}

}  // namespace codebreaker::verify::gate16::scoreboard16
