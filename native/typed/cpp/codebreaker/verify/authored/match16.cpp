// codebreaker/verify/authored/match16.cpp -- typed C++ of codebreaker.verify.authored.match16 (see match16.hpp).

#include "codebreaker/verify/authored/match16.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/wine16.hpp"
#include "codebreaker/verify/authored/search16.hpp"
#include "codebreaker/verify/gate16/runtime16.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::authored::match16 {

namespace V = verify::verdicts;
namespace msc6 = build::msc6;
using Json = knowledge::sources::Json;
using Key = msc6::Key;

namespace {

py::str state_(const std::vector<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

Json J(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}

void dump_(std::string& o, const Json& j, int depth) {
    const std::string in(size_t(depth + 1), ' ');
    switch (j.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += j.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(j.i); break;
        case Json::Float: o += py::to_str(j.d).utf8(); break;
        case Json::Str: py::json::put(o, j.s); break;
        case Json::List:
            if (j.list.empty()) { o += "[]"; break; }
            o += "[\n";
            for (size_t i = 0; i < j.list.size(); ++i) {
                o += in;
                dump_(o, j.list[i], depth + 1);
                o += i + 1 < j.list.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "]";
            break;
        default: {
            auto items = j.dict.items();
            if (items.empty()) { o += "{}"; break; }
            o += "{\n";
            for (size_t i = 0; i < items.size(); ++i) {
                o += in;
                py::json::put(o, std::get<0>(items[i]));
                o += ": ";
                dump_(o, std::get<1>(items[i]), depth + 1);
                o += i + 1 < items.size() ? ",\n" : "\n";
            }
            o += std::string(size_t(depth), ' ') + "}";
        }
    }
}

Key K(const py::str& s) { return Key(s); }

}  // namespace

void log_print(const py::str& line) { py::print(line); }

Last& last() {
    static Last l;
    return l;
}

py::list<py::list<py::str>> source_files(const py::list<py::str>& names, const Where16& where, const py::dict<int64_t, match::ByteMasked>& originals,
                                         const py::dict<py::str, int64_t>& address) {
    const match::Decoder& md = match::decoder(16);
    py::dict<std::tuple<int64_t, int64_t>, py::str> at;
    for (const py::str& fn : names) at[where.at(address.at(fn))] = fn;
    py::dict<int64_t, py::list<py::str>> by_seg;
    for (const py::str& fn : names) by_seg.setdefault(std::get<0>(where.at(address.at(fn))), py::list<py::str>()).append(fn);
    py::list<py::list<py::str>> files;
    for (const auto& sf : by_seg.items()) {
        int64_t seg = std::get<0>(sf);
        py::list<py::str> fns = std::get<1>(sf);
        std::stable_sort(fns.vec().begin(), fns.vec().end(), [&](const py::str& a, const py::str& b) {
            return std::get<1>(where.at(address.at(a))) < std::get<1>(where.at(address.at(b)));
        });
        py::dict<py::str, int64_t> index;
        for (int64_t i = 0; i < py::len(fns); i++) index[fns[i]] = i;
        std::vector<bool> joined(static_cast<size_t>(py::len(fns)), false);   // joined[i]: fns[i] and fns[i + 1] in one file
        for (const py::str& fn : fns) {
            py::bytes code = std::get<0>(originals.at(address.at(fn)));
            int64_t start = std::get<1>(where.at(address.at(fn)));
            py::list<match::Insn> ins = md(code, 0);
            for (const match::Insn& i : ins) {
                if (i.mnemonic != py::str("call") || !i.op_str.startswith(py::str("0x")) || i.op_str.contains(py::str(","))) continue;   // (a direct near call: `push cs; call` or a near callee)
                std::tuple<int64_t, int64_t> target(seg, (start + py::int_parse(i.op_str, 16)) & 0xFFFF);
                if (!at.contains(target)) continue;
                const py::str& other = at.at(target);
                if (other == fn) continue;
                int64_t a = std::min(index.at(fn), index.at(other)), b = std::max(index.at(fn), index.at(other));
                for (int64_t j = a; j < b; j++) joined[static_cast<size_t>(j)] = true;
            }
        }
        py::list<py::str> cur{fns[0]};
        for (int64_t j = 1; j < py::len(fns); j++) {
            if (joined[static_cast<size_t>(j - 1)]) {
                cur.append(fns[j]);
            } else {
                files.append(cur);
                cur = py::list<py::str>{fns[j]};
            }
        }
        files.append(cur);
    }
    return files;
}

double _score1(const Verdict& v) {
    if (std::get<0>(v) == V::NO_COMPILE) return 0.0;
    return std::get<0>(v) == V::IDENTICAL ? 2.0 : 1.0 - static_cast<double>(std::get<1>(v)) / static_cast<double>(std::max<int64_t>(std::get<2>(v), 1));
}

double _score(const py::dict<int64_t, Verdict>& results) {
    double total = 0.0;
    for (const auto& kv : results.items()) total += _score1(std::get<1>(kv));
    return total;
}

std::tuple<py::dict<int64_t, Setting>, py::dict<py::str, Verdict>> run(const py::str& module_path, const Options& options, const Prepare16& win16_prepare,
                                                                        const SpansFn& win16_spans) {
    const Log& log = options.log;
    // Microsoft C 8.00c built Windows 95's Setup modules and runs on Windows itself; Microsoft C 6.0a (DOS, the emulator) is
    // tried only when asked for (settings: match16_msc6 = on) -- choosing SUWIN's switches among both gave nothing more than
    // among C 8's alone (35 either way).
    py::str sw = py::settings::get_str(py::str("match16_msc6")).lower();
    bool msc6_on = sw == py::str("on") || sw == py::str("1") || sw == py::str("true") || sw == py::str("yes");
    py::list<py::str> wanted = msc6_on ? py::list<py::str>{py::str("msc6"), py::str("msvc152")} : py::list<py::str>{py::str("msvc152")};
    py::list<py::str> compilers;
    if (options.compiler) {
        compilers.append(*options.compiler);
    } else {
        for (const py::str& c : wanted)
            if (msc6::available(c)) compilers.append(c);
    }
    bool all_ok = true;
    for (const py::str& c : compilers)
        if (!msc6::available(c)) all_ok = false;
    if (compilers.empty() || !all_ok) throw py::SystemExit("Microsoft C is not set up (settings.json: dosbox_x, msc6, msvc152)");
    py::str name = py::os::path::basename(module_path).lower();
    Prepared16 m = win16_prepare(module_path, state_({py::str("match16"), name, py::str("source")}));
    Where16 where_of_by_va;
    for (const auto& kv : m.by_va.items()) where_of_by_va[std::get<0>(kv)] = m.where.at(std::get<0>(kv));
    Spans16 spans = win16_spans(formats::ne::NEFile(module_path), where_of_by_va);
    py::dict<int64_t, match::ByteMasked> originals =
        std::get<match::Originals16>(match::ARCH().at(py::str("x86-16")).originals)(module_path, where_of_by_va, spans);
    py::dict<py::str, int64_t> address;
    for (const auto& kv : m.by_va.items()) address[std::get<1>(kv)] = std::get<0>(kv);
    py::list<py::str> todo;
    for (const py::str& fn : m.order) {
        if (address.contains(fn) && originals.contains(address.at(fn))) todo.append(fn);
    }
    if (options.limit) todo = todo.slice(0, options.limit);
    py::str source_header = msc6::cdecl_imports(m.header);          // (a C function imported by number: not pascal)
    py::str header = std::get<0>(msc6::dialect(source_header, py::str("")));
    py::list<py::str> pascal_in{source_header};
    for (const auto& kv : m.texts.items()) pascal_in.append(std::get<1>(kv));
    py::set<py::str> pascal = msc6::pascal_names(pascal_in);
    header = msc6::pascal_order(header, pascal);
    auto [decls, header2] = msc6::ds_globals(header);
    header = header2 + decls;                                       // (after the typedefs; the header's macros use them)
    Last& L = last();
    L = Last();
    L.header = header;
    py::str work = state_({py::str("match16"), name, py::str("work")});
    py::dict<py::str, int64_t> segment;
    for (const py::str& fn : todo) segment[fn] = std::get<0>(m.where.at(address.at(fn)));
    py::list<py::list<py::str>> files = source_files(todo, m.where, originals, address);   // (compiled together, one setting each)
    py::dict<py::str, int64_t> file_of;
    for (int64_t k = 0; k < py::len(files); k++)
        for (const py::str& fn : files[k]) file_of[fn] = k;
    // Each function is said several ways -- as the decompiler wrote it, with its temporaries folded back into the
    // expressions they came from, through the SDK's prototypes -- and judged by the one nearest the original: which a
    // function's source was is the compiler's to say (a temporary may be the programmer's variable, at /Od a stack slot of its own).
    std::vector<py::str> stages;
    if (options.fold) stages = {py::str("fold"), py::str("far"), py::str("frame")};   // (each subset of these is a way of saying it)
    if (options.sdk) stages.push_back(py::str("sdk"));
    if (options.wine) stages.push_back(py::str("wine"));                              // (imports typed by Wine's .spec: symbols/wine16.py)
    py::dict<py::str, py::set<py::str>> variants;
    for (int64_t k = 0; k < (int64_t(1) << stages.size()); k++) {
        py::set<py::str> on;
        for (size_t b = 0; b < stages.size(); b++)
            if ((k >> b) & 1) on.add(stages[b]);
        std::vector<py::str> parts;
        for (const py::str& st : stages)
            if (on.contains(st)) parts.push_back(st);
        py::str vname = py::join(py::str("+"), parts);
        variants[vname.empty() ? py::str("plain") : vname] = on;
    }
    msc6::Prototypes protos;
    if (options.sdk) protos = msc6::sdk_prototypes();
    py::dict<py::str, symbols::wine16::Export> own;
    if (options.wine) {
        py::str mod = py::split(name, py::str("."))[0];
        for (const auto& kv : symbols::wine16::exports(mod).items()) own[std::get<1>(kv).name.upper()] = std::get<1>(kv);
    }
    auto said = [&](const py::str& fn, const py::set<py::str>& on) -> py::str {
        py::str t = m.texts.at(fn);
        if (on.contains(py::str("wine")) && own.contains(fn.upper())) t = msc6::wine_definition(t, own.at(fn.upper()));   // (the module's own export, typed by Wine's .spec)
        if (on.contains(py::str("fold"))) t = msc6::fold_temporaries(t);
        if (on.contains(py::str("far"))) t = msc6::join_far_parameters(t);
        if (on.contains(py::str("frame"))) t = msc6::split_frame(t);
        t = msc6::pascal_order(t, pascal);
        auto [d, tt] = msc6::ds_globals(on.contains(py::str("sdk")) ? msc6::sdk_calls(t, protos) : t);
        return d + tt;
    };
    py::dict<py::str, py::dict<py::str, py::str>> texts;
    for (const auto& vv : variants.items()) {
        py::dict<py::str, py::str> t;
        for (const py::str& fn : todo) t[fn] = said(fn, std::get<1>(vv));
        texts[std::get<0>(vv)] = t;
    }
    py::dict<py::str, py::str> heads;
    for (const auto& vv : variants.items()) {
        const py::str& v = std::get<0>(vv);
        const py::set<py::str>& on = std::get<1>(vv);
        py::str h = header;
        if (on.contains(py::str("wine"))) {
            auto [h2, t2] = msc6::wine_typing(header, texts.at(v));
            h = h2;
            texts[v] = t2;
        }
        heads[v] = on.contains(py::str("sdk")) ? msc6::sdk_header(h) : h;
    }
    py::dict<py::str, py::str>& winner = L.winner;
    py::dict<py::str, std::optional<msc6::Code>> code;

    auto together = [&] {
        py::list<py::list<Key>> out;
        for (const py::list<py::str>& f : files) {
            py::list<Key> g;
            for (const py::str& fn : f) g.append(K(fn));
            out.append(g);
        }
        return out;
    };
    auto distance_of = [&](const std::optional<msc6::Code>& built, const py::str& fn) -> Verdict {
        if (built) {
            auto d = match::distance(match::Masked(std::get<0>(*built), std::get<1>(*built)), match::Masked(std::get<0>(originals.at(address.at(fn))), std::get<1>(originals.at(address.at(fn)))));
            return Verdict(std::get<0>(d), std::get<1>(d), std::get<2>(d));
        }
        return Verdict(V::NO_COMPILE, 0, 0);
    };
    auto judged = [&](const py::str& cc, const py::str& fl, const py::list<py::str>& names, const std::vector<py::str>& only) {
        py::dict<py::str, Verdict> best;
        for (const py::str& v : only) {
            py::dict<Key, py::str> sources, nm;
            for (const py::str& fn : names) {
                sources[K(fn)] = texts.at(v).at(fn);
                nm[K(fn)] = fn;
            }
            py::dict<Key, std::optional<msc6::Code>> built =
                msc6::compile_many(sources, heads.at(v), fl, work, 900, cc, std::optional<py::dict<Key, py::str>>(nm), msc6::BATCH, together());
            for (const py::str& fn : names) {
                std::optional<msc6::Code> b = built.contains(K(fn)) ? built.at(K(fn)) : std::nullopt;
                Verdict got = distance_of(b, fn);
                if (!best.contains(fn) || _score1(got) > _score1(best.at(fn))) {
                    best[fn] = got;
                    winner[fn] = v;
                    code[fn] = b;
                }
            }
        }
        return best;
    };
    py::list<int64_t> segs;
    {
        py::set<int64_t> seen;
        std::vector<int64_t> v;
        for (const auto& kv : segment.items())
            if (!seen.contains(std::get<1>(kv))) {
                seen.add(std::get<1>(kv));
                v.push_back(std::get<1>(kv));
            }
        std::sort(v.begin(), v.end());
        segs = py::list<int64_t>(v);
    }
    py::str remembered = state_({py::str("match16"), name, py::str("choices.json")});
    py::dict<int64_t, Setting> known;
    py::dict<py::str, Setting> each;
    if (py::os::path::isfile(remembered) && !options.choose && !options.compiler) {
        Json saved = knowledge::sources::_json(remembered);
        if (!saved.dict.contains(py::str("segments"))) {
            Json wrapped;
            wrapped.kind = Json::Dict;
            wrapped.dict.set(py::str("segments"), saved);
            saved = wrapped;
        }
        for (const auto& kv : saved.dict.at(py::str("segments")).dict.items())
            known[py::int_parse(std::get<0>(kv), 10)] = Setting(std::get<1>(kv).list[0].s, std::get<1>(kv).list[1].s);
        if (saved.dict.contains(py::str("functions")))
            for (const auto& kv : saved.dict.at(py::str("functions")).dict.items()) each[std::get<0>(kv)] = Setting(std::get<1>(kv).list[0].s, std::get<1>(kv).list[1].s);
    }
    py::dict<int64_t, Setting> chosen;
    bool all_known = true;
    for (int64_t s : segs)
        if (!known.contains(s)) all_known = false;
    if (options.flags && !options.flags->empty()) {
        for (int64_t s : segs) chosen[s] = Setting(compilers[0], *options.flags);
    } else if (all_known) {
        chosen = known;   // (found by an earlier run: --choose finds them again)
        if (log) log(py::percent(py::str("  each segment's and function's compiler and switches as found before (%s)"), remembered));
    } else {
        py::dict<int64_t, py::list<py::str>> sample;
        for (int64_t s : segs) {
            py::list<py::str> v;
            for (const py::str& fn : todo)
                if (segment.at(fn) == s) v.append(fn);
            int64_t step = std::max<int64_t>(1, py::len(v) / 6);
            py::list<py::str> picked;
            for (int64_t i = 0; i < py::len(v) && py::len(picked) < 6; i += step) picked.append(v[i]);
            sample[s] = picked;
        }
        py::list<py::str> every;
        for (const auto& kv : sample.items())
            for (const py::str& fn : std::get<1>(kv)) every.append(fn);
        std::vector<std::tuple<int64_t, py::str, py::str, double>> scores;   // (segment, compiler, switches, score) in the order made
        std::vector<py::str> trial;
        for (const char* v : {"plain", "fold", "frame", "fold+frame"})
            if (variants.contains(py::str(v))) trial.push_back(py::str(v));
        for (const py::str& cc : compilers) {
            for (const py::str& fl : (cc == py::str("msc6") ? msc6::FLAG_SETS : match::ARCH().at(py::str("x86-16")).flag_sets)) {
                py::dict<py::str, Verdict> r = judged(cc, fl, every, trial);
                for (int64_t s : segs) {
                    py::dict<int64_t, Verdict> part;
                    int64_t k = 0;
                    for (const py::str& fn : sample.at(s)) part[k++] = r.at(fn);
                    // (the Python's {fn: r[fn]} dict: one entry per function; a name in two segments cannot occur)
                    scores.push_back({s, cc, fl, _score(part)});
                }
            }
        }
        py::dict<int64_t, py::list<Setting>> top;
        for (int64_t s : segs) {
            std::vector<std::tuple<double, py::str, py::str>> ranked;
            for (const auto& sc : scores)
                if (std::get<0>(sc) == s) ranked.push_back({std::get<3>(sc), std::get<1>(sc), std::get<2>(sc)});
            std::stable_sort(ranked.begin(), ranked.end(), [](const auto& a, const auto& b) { return -std::get<0>(a) < -std::get<0>(b); });
            chosen[s] = Setting(std::get<1>(ranked[0]), std::get<2>(ranked[0]));
            py::list<Setting> t;
            for (size_t i = 0; i < std::min<size_t>(3, ranked.size()); i++) t.append(Setting(std::get<1>(ranked[i]), std::get<2>(ranked[i])));
            top[s] = t;
            if (log) log(py::percent(py::str("  segment %-3s %-8s %-16s (%.1f of a sample of %d)"), py::to_str(s), std::get<1>(ranked[0]), std::get<2>(ranked[0]), std::get<0>(ranked[0]), py::len(sample.at(s))));
        }
        // A segment of the file can be several source files' code (the linker packs them), each built with its own
        // switches: every function is judged under its segment's three best, and keeps the one it comes nearest under
        // (its segment's first where none is nearer).
        each = py::dict<py::str, Setting>();
        py::dict<py::str, std::vector<std::tuple<double, bool, py::str, py::str>>> tried;
        std::vector<Setting> all_top;
        for (const auto& kv : top.items())
            for (const Setting& c : std::get<1>(kv))
                if (std::find(all_top.begin(), all_top.end(), c) == all_top.end()) all_top.push_back(c);
        std::sort(all_top.begin(), all_top.end());
        for (const Setting& c : all_top) {
            py::list<py::str> names;
            for (const py::str& fn : todo) {
                const py::list<Setting>& t = top.at(segment.at(fn));
                if (std::find(t.vec().begin(), t.vec().end(), c) != t.vec().end()) names.append(fn);
            }
            for (const auto& kv : judged(std::get<0>(c), std::get<1>(c), names, {py::str("plain")}).items()) {
                const py::str& fn = std::get<0>(kv);
                py::dict<int64_t, Verdict> one;
                one[0] = std::get<1>(kv);
                tried[fn].push_back({_score(one), c == chosen.at(segment.at(fn)), std::get<0>(c), std::get<1>(c)});
            }
        }
        for (const py::list<py::str>& f : files) {   // one file, one command line: the setting its functions come nearest under together
            std::vector<std::pair<Setting, double>> total;
            for (const py::str& fn : f) {
                if (!tried.contains(fn)) continue;
                for (const auto& t : tried.at(fn)) {
                    Setting key(std::get<2>(t), std::get<3>(t));
                    auto it = std::find_if(total.begin(), total.end(), [&](const auto& p) { return p.first == key; });
                    if (it == total.end()) {
                        total.push_back({key, 0.0});
                        it = total.end() - 1;
                    }
                    it->second = it->second + std::get<0>(t) + (std::get<1>(t) ? 1e-6 : 0);
                }
            }
            if (total.empty()) continue;
            size_t bi = 0;
            for (size_t i = 1; i < total.size(); i++)
                if (total[i].second > total[bi].second) bi = i;   // max(total, key=total.get): the first of the greatest
            for (const py::str& fn : f)
                if (total[bi].first != chosen.at(segment.at(fn))) each[fn] = total[bi].first;
        }
        if (log) log(py::percent(py::str("  %d functions nearer under another of their segment's switches than its first"), each.size()));
        if (!options.compiler) {
            Json doc;
            doc.kind = Json::Dict;
            Json segs_j, funcs_j;
            segs_j.kind = Json::Dict;
            funcs_j.kind = Json::Dict;
            for (const auto& kv : chosen.items()) {
                Json l;
                l.kind = Json::List;
                l.list = {J(std::get<0>(std::get<1>(kv))), J(std::get<1>(std::get<1>(kv)))};
                segs_j.dict.set(py::to_str(std::get<0>(kv)), l);
            }
            std::vector<std::tuple<py::str, Setting>> items;
            for (const auto& kv : each.items()) items.push_back(kv);
            std::sort(items.begin(), items.end());
            for (const auto& kv : items) {
                Json l;
                l.kind = Json::List;
                l.list = {J(std::get<0>(std::get<1>(kv))), J(std::get<1>(std::get<1>(kv)))};
                funcs_j.dict.set(std::get<0>(kv), l);
            }
            doc.dict.set(py::str("segments"), segs_j);
            doc.dict.set(py::str("functions"), funcs_j);
            std::string o;
            dump_(o, doc, 0);
            py::file f = py::open(remembered, py::str("w"));
            f.write(py::str(o));
            f.close();
        }
    }
    py::dict<py::str, Setting>& setting = L.setting;
    for (const py::str& fn : todo) setting[fn] = each.contains(fn) ? each.at(fn) : chosen.at(segment.at(fn));
    for (const py::list<py::str>& f : files) {   // (one setting a file: its first function's, where they differ)
        std::vector<Setting> votes;
        for (const py::str& fn : f) votes.push_back(setting.at(fn));
        Setting one = votes[0];
        int64_t best_n = -1;
        for (const Setting& v : votes) {   // max(votes, key=votes.count): the first with the most votes
            int64_t n = std::count(votes.begin(), votes.end(), v);
            if (n > best_n) {
                best_n = n;
                one = v;
            }
        }
        for (const py::str& fn : f) setting[fn] = one;
    }
    py::dict<py::str, Verdict> out;
    {
        std::vector<Setting> uniq;
        for (const auto& kv : setting.items())
            if (std::find(uniq.begin(), uniq.end(), std::get<1>(kv)) == uniq.end()) uniq.push_back(std::get<1>(kv));
        std::sort(uniq.begin(), uniq.end());
        std::vector<py::str> all_variants;
        for (const auto& kv : variants.items()) all_variants.push_back(std::get<0>(kv));
        for (const Setting& c : uniq) {
            py::list<py::str> names;
            for (const py::str& fn : todo)
                if (setting.at(fn) == c) names.append(fn);
            for (const auto& kv : judged(std::get<0>(c), std::get<1>(c), names, all_variants).items()) out[std::get<0>(kv)] = std::get<1>(kv);
        }
    }
    {   // (the compiler's run-time library: linked, not written)
        py::dict<py::str, match::ByteMasked> orig_by_fn;
        for (const py::str& fn : todo) orig_by_fn[fn] = originals.at(address.at(fn));
        L.library = gate16::runtime16::identify(orig_by_fn);
        for (const auto& kv : L.library.items()) {
            const py::str& fn = std::get<0>(kv);
            if (!(out.contains(fn) && std::get<0>(out.at(fn)) == V::IDENTICAL)) out[fn] = Verdict(V::IDENTIFIED, 0, py::len(std::get<0>(originals.at(address.at(fn)))));
        }
    }
    if (options.search) {   // the last bytes of the near misses (verify/authored/search16.py)
        py::dict<py::str, search16::Near> near;
        for (const auto& kv : out.items()) {
            const py::str& fn = std::get<0>(kv);
            const Verdict& v = std::get<1>(kv);
            if ((std::get<0>(v) == V::SAME_SIZE || std::get<0>(v) == V::DIFFERENT) && static_cast<double>(std::get<1>(v)) <= 0.4 * static_cast<double>(std::get<2>(v))) {
                const Setting& s = setting.at(fn);
                near[fn] = search16::Near(texts.at(winner.at(fn)).at(fn), v, search16::Group(std::get<0>(s), std::get<1>(s), winner.at(fn)));
            }
        }
        auto compile_group = [&](const search16::Group& g, const py::dict<py::str, py::str>& sources) {
            py::dict<Key, py::str> all, nm;
            std::vector<int64_t> ks;
            for (const auto& kv : sources.items()) {
                int64_t k = file_of.at(std::get<0>(kv));
                if (std::find(ks.begin(), ks.end(), k) == ks.end()) ks.push_back(k);
            }
            std::sort(ks.begin(), ks.end());
            for (int64_t k : ks)   // (a rewrite is judged in its file)
                for (const py::str& fn : files[k])
                    if (!sources.contains(fn) && winner.contains(fn)) {
                        all[K(fn)] = texts.at(winner.at(fn)).at(fn);
                        nm[K(fn)] = fn;
                    }
            for (const auto& kv : sources.items()) {
                all[K(std::get<0>(kv))] = std::get<1>(kv);
                nm[K(std::get<0>(kv))] = std::get<0>(kv);
            }
            py::str head = heads.contains(std::get<2>(g)) ? heads.at(std::get<2>(g)) : header;
            py::dict<Key, std::optional<msc6::Code>> built =
                msc6::compile_many(all, head, std::get<1>(g), work, 900, std::get<0>(g), std::optional<py::dict<Key, py::str>>(nm), msc6::BATCH, together());
            py::dict<py::str, search16::Verdict> res;
            for (const auto& kv : sources.items()) {
                const py::str& fn = std::get<0>(kv);
                std::optional<msc6::Code> b = built.contains(K(fn)) ? built.at(K(fn)) : std::nullopt;
                res[fn] = distance_of(b, fn);
            }
            return res;
        };
        auto found = search16::search(near, compile_group, [](const Verdict& v) { return _score1(v); }, options.search, 1995, log);
        for (const auto& kv : found.items()) {
            const py::str& fn = std::get<0>(kv);
            const py::str& t = std::get<0>(std::get<1>(kv));
            const Verdict& v = std::get<1>(std::get<1>(kv));
            if (_score1(v) > _score1(out.at(fn))) {
                out[fn] = v;
                winner[fn] = py::str("search");
                texts[py::str("search")][fn] = t;
            }
        }
    }
    for (const py::str& fn : todo)
        if (winner.contains(fn)) L.texts[fn] = texts.at(winner.at(fn)).at(fn);
    {
        py::set<Setting> settings_set;
        for (const auto& kv : setting.items()) settings_set.add(std::get<1>(kv));
        L.settings = settings_set;
    }
    L.files = files;
    L.segment = segment;                                            // (for scoreboard16)
    for (const py::str& fn : todo) L.code[fn] = std::make_tuple(code.contains(fn) ? code.at(fn) : std::optional<msc6::Code>(), originals.at(address.at(fn)));   // (its bytes, and the original's)
    L.ran = true;
    return {chosen, out};
}

const py::str DOC(R"DOC(match16.py -- does Microsoft C 6 turn a 16-bit module's reconstruction back into its bytes?

    python -m codebreaker match <16-bit module> [--flags "/AM /Gcsw /Os"] [--first N]

The 16-bit counterpart of match.py (see there for what the comparison is and is not). Each
reconstructed function is compiled by Microsoft C 6.0a (build/msc6.py: the real compiler, in an
emulator, a whole module's functions in one session) and its bytes are compared with the
original function's, outside the places a loader or linker fills in: in the original, every
word a relocation record of its segment names and the distance of every near call and jump to
another function; in the compiled function, every place the object file has a fix-up for. (What
is 16-bit about that is match.py's ARCH["x86-16"]; this is the driver.)

Without --flags the switches are found first: the memory model and options under which most of
a sample of the functions come out the original's length say how the module was built.
)DOC");

int64_t main(const py::list<py::str>& argv, const Prepare16& win16_prepare, const SpansFn& win16_spans) {
    std::optional<py::str> flags, compiler, rounds;
    if (argv.contains(py::str("--flags"))) flags = argv[argv.index(py::str("--flags")) + 1];
    if (argv.contains(py::str("--compiler"))) compiler = argv[argv.index(py::str("--compiler")) + 1];
    int64_t limit = argv.contains(py::str("--first")) ? py::int_parse(argv[argv.index(py::str("--first")) + 1], 10) : 0;
    if (argv.contains(py::str("--search"))) rounds = argv[argv.index(py::str("--search")) + 1];
    py::list<py::str> args;
    for (const py::str& a : argv) {
        if (a.startswith(py::str("--"))) continue;
        if ((flags && a == *flags) || (compiler && a == *compiler) || a == py::to_str(limit) || (rounds && a == *rounds)) continue;
        args.append(a);
    }
    py::str path = py::os::path::isfile(args[0]) ? args[0] : base::corpus::retail(args[0]).value_or(py::str(""));
    Options o;
    o.flags = flags;
    o.limit = limit;
    o.compiler = compiler;
    o.choose = argv.contains(py::str("--choose"));
    o.sdk = argv.contains(py::str("--sdk"));
    o.fold = !argv.contains(py::str("--no-fold"));
    o.search = argv.contains(py::str("--search")) ? py::int_parse(argv[argv.index(py::str("--search")) + 1], 10) : 0;
    o.wine = argv.contains(py::str("--wine"));
    auto [chosen, results] = run(path, o, win16_prepare, win16_spans);
    py::dict<py::str, int64_t> counts;
    for (const auto& kv : results.items()) counts[std::get<0>(std::get<1>(kv))] = counts.get(std::get<0>(std::get<1>(kv)), 0) + 1;
    std::vector<py::str> settings_text;
    {
        std::vector<py::str> v;
        if (last().ran && last().settings) {
            for (const Setting& c : *last().settings) v.push_back(py::percent(py::str("%s %s"), std::get<0>(c), std::get<1>(c)));
        } else {
            for (const auto& kv : chosen.items()) v.push_back(py::percent(py::str("%s %s"), std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv))));
        }
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());   // ({...} is a set)
        settings_text = v;
    }
    std::vector<py::str> by_verdict;
    for (const py::str& k : {V::IDENTICAL, V::IDENTIFIED, V::SAME_SIZE, V::DIFFERENT, V::NO_COMPILE})
        if (counts.get(k, 0)) by_verdict.push_back(py::percent(py::str("%s %d"), V::MATCH_LABEL().at(k), counts.at(k)));
    py::print(py::percent(py::str("%s: %d functions compiled with Microsoft C (%s): %s"), py::os::path::basename(path), results.size(),
                          py::join(py::str("; "), settings_text), py::join(py::str(", "), by_verdict)));
    py::dict<py::str, int64_t> won;
    for (const auto& kv : last().winner.items()) {
        const py::str& fn = std::get<0>(kv);
        if (results.contains(fn) && std::get<0>(results.at(fn)) == V::IDENTICAL) won[std::get<1>(kv)] = won.get(std::get<1>(kv), 0) + 1;
    }
    if (!won.empty()) {
        std::vector<std::tuple<py::str, int64_t>> items;
        for (const auto& kv : won.items()) items.push_back(kv);
        std::sort(items.begin(), items.end());
        std::vector<py::str> parts;
        for (const auto& kv : items) parts.push_back(py::percent(py::str("%s %d"), std::get<0>(kv), std::get<1>(kv)));
        py::print(py::percent(py::str("  identical by way of saying: %s"), py::join(py::str(", "), parts)));
    }
    struct Near {
        double ratio;
        py::str name;
        Verdict v;
    };
    std::vector<Near> near;
    for (const auto& kv : results.items()) {
        const Verdict& v = std::get<1>(kv);
        if (std::get<0>(v) == V::SAME_SIZE || std::get<0>(v) == V::DIFFERENT)
            near.push_back({static_cast<double>(std::get<1>(v)) / static_cast<double>(std::max<int64_t>(std::get<2>(v), 1)), std::get<0>(kv), v});
    }
    std::sort(near.begin(), near.end(), [](const Near& a, const Near& b) {
        return std::tie(a.ratio, a.name, std::get<0>(a.v), std::get<1>(a.v), std::get<2>(a.v)) < std::tie(b.ratio, b.name, std::get<0>(b.v), std::get<1>(b.v), std::get<2>(b.v));
    });
    for (size_t i = 0; i < std::min<size_t>(8, near.size()); i++)
        py::print(py::percent(py::str("  %-28s %-10s %d of %d bytes differ"), near[i].name, V::MATCH_LABEL().at(std::get<0>(near[i].v)), std::get<1>(near[i].v), std::get<2>(near[i].v)));
    return 0;
}

}  // namespace codebreaker::verify::authored::match16
