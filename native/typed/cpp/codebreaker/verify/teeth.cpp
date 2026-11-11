// codebreaker/verify/teeth.cpp -- typed C++ port of codebreaker.verify.teeth (see teeth.hpp).

#include "codebreaker/verify/teeth.hpp"

#include <algorithm>
#include <cstdio>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/ctext.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::teeth {

namespace {

using py::str;
namespace V = codebreaker::verify::verdicts;
namespace R = codebreaker::verify::reconstruct;
namespace eng = codebreaker::engine::native_engine;

str S(const char* s) { return str(s); }

const str& NAME() {
    static const str n = S("[A-Za-z_]\\w*");
    return n;
}

// re.search / re.match with a pattern compiled once per call site
const py::re::Pattern& rx(const char* p) {
    static thread_local std::vector<std::pair<const char*, py::re::Pattern>> cache;
    for (auto& kv : cache)
        if (kv.first == p) return kv.second;
    cache.emplace_back(p, py::re::compile(p));
    return cache.back().second;
}

// the increment of a non-negative integer written as digits (no limit to its size: Python's ints are unbounded)
str decimal_plus_one(str digits) {
    std::string d = digits.utf8();
    size_t i = d.size();
    while (i > 0) {
        --i;
        if (d[i] == '9') {
            d[i] = '0';
        } else {
            ++d[i];
            return str(d);
        }
    }
    return str("1" + d);
}
str hex_plus_one(const str& digits) {
    std::string d = digits.lower().utf8();
    size_t i = d.size();
    while (i > 0) {
        --i;
        if (d[i] == 'f') {
            d[i] = '0';
        } else {
            d[i] = d[i] == '9' ? 'a' : static_cast<char>(d[i] + 1);
            return str(d);
        }
    }
    return str("1" + d);
}
str strip_zeros(const str& digits) {
    size_t i = 0;
    std::string d = digits.utf8();
    while (i + 1 < d.size() && d[i] == '0') ++i;
    return str(d.substr(i));
}

}  // namespace

void log_print(const str& line) { py::print(line); }

const py::list<str> BREAK_KINDS = {S("constant"), S("comparison"), S("operator"), S("store"), S("arguments")};

std::tuple<str, str> _body(const str& text) {
    auto [head, brace, rest] = text.partition(S("{"));
    py::list<str> lines = py::split(rest, S("\n"));
    int64_t i = 0;
    const int64_t n = static_cast<int64_t>(lines.size());
    while (i < n && (lines[i].strip().empty() ||
                     py::re::match(S("^\\s+[\\w ]+[ *]\\w+(\\s*\\[[^\\]]*\\])?;\\s*$"), lines[i]) ||
                     py::re::match(S("^\\s+struct \\{.*\\} \\w+;"), lines[i])))
        ++i;
    py::list<str> top, bottom;
    for (int64_t k = 0; k < n; ++k) (k < i ? top : bottom).append(lines[k]);
    return {head + brace + py::join(S("\n"), top) + (i ? S("\n") : S("")), py::join(S("\n"), bottom)};
}

std::optional<str> _constant(const str& body, const str& shadow) {
    auto m = py::re::search(S("(?<![\\w.\\[])(\\d+|0x[0-9a-fA-F]+)(?![\\w.\\]xX])"), shadow);
    if (!m) return std::nullopt;
    const str text = m->group(1);
    str bumped;
    if (text.startswith(S("0x"))) {
        // value = int(text, 16); "0x%x" % (value + 1)
        bumped = S("0x") + hex_plus_one(strip_zeros(text.slice_from(2)));
    } else {
        // (int(text, 10): the Python's base 0 refused a decimal with leading zeros, like 010, and ended the run: fixed there too)
        bumped = decimal_plus_one(strip_zeros(text));
    }
    return body.slice(0, m->start()) + bumped + body.slice_from(m->end());
}

std::optional<str> _comparison(const str& body, const str& shadow) {
    auto m = py::re::search(S("(?<![=!<>])(==|!=)(?!=)"), shadow);
    if (m) return body.slice(0, m->start()) + (m->group(1) == S("==") ? S("!=") : S("==")) + body.slice_from(m->end());
    m = py::re::search(S("(?<![<>=\\-])\\s(<|>)\\s(?![<>=])"), shadow);
    if (m) return body.slice(0, m->start(1)) + (m->group(1) == S("<") ? S(">=") : S("<=")) + body.slice_from(m->end(1));
    return std::nullopt;
}

std::optional<str> _operator(const str& body, const str& shadow) {
    auto m = py::re::search(S("(?<=[\\w)\\]])\\s\\+\\s(?=[\\w(*])"), shadow);
    if (!m) return std::nullopt;
    return body.slice(0, m->start()) + S(" - ") + body.slice_from(m->end());
}

std::optional<str> _store(const str& body, const str& shadow) {
    auto m = py::re::search(S("(?m)^[ \\t]*\\*[^;=\\n]*=[^;=\\n]*;[ \\t]*\\n"), shadow);
    if (!m) return std::nullopt;
    return body.slice(0, m->start()) + body.slice_from(m->end());
}

std::optional<str> _arguments(const str& body, const str& shadow) {
    const str operand = S("(") + NAME() + S("|\\d+|0x[0-9a-fA-F]+)");
    const str pattern = S("\\b(") + NAME() + S(")\\(") + operand + S(",") + operand + S("([,)])");
    auto m = py::re::search(pattern, shadow);
    if (!m) return std::nullopt;
    const str f = m->group(1);
    if (m->group(2) == m->group(3) || f == S("if") || f == S("while") || f == S("for") || f == S("switch") || f == S("return") || f == S("sizeof"))
        return std::nullopt;
    return body.slice(0, m->start(2)) + m->group(3) + S(",") + m->group(2) + body.slice_from(m->end(3));
}

py::list<std::tuple<str, str>> breaks(const str& text) {
    auto [head, body] = _body(text);
    const str shadow = base::ctext::blank(body, true);
    py::list<std::tuple<str, str>> out;
    using Make = std::optional<str> (*)(const str&, const str&);
    const Make makes[] = {_constant, _comparison, _operator, _store, _arguments};
    for (size_t k = 0; k < 5; ++k) {
        std::optional<str> made = makes[k](body, shadow);
        if (made && *made != body) out.append(std::make_tuple(BREAK_KINDS[static_cast<int64_t>(k)], head + *made));
    }
    return out;
}

str _changed(const str& before, const str& after) {
    py::list<str> a = py::splitlines(before), b = py::splitlines(after);
    int64_t i = 0;
    const int64_t na = static_cast<int64_t>(a.size()), nb = static_cast<int64_t>(b.size());
    while (i < std::min(na, nb) && a[i] == b[i]) ++i;
    const str now = (nb == na && i < nb) ? b[i].strip() : S("(removed)");
    return (py::percent(S("%s  ->  %s"), i < na ? a[i].strip() : S(""), now)).slice(0, 200);
}

py::dict<str, py::set<str>> _reach(const py::dict<str, str>& texts) {
    py::set<str> names;
    for (const str& n : texts.keys()) names.add(n);
    const py::re::Pattern word = py::re::compile(NAME());
    py::dict<str, py::set<str>> calls;
    for (const auto& kv : texts.items()) {
        py::set<str> mine;
        for (const str& w : word.findall(std::get<1>(kv)))
            if (names.contains(w) && w != std::get<0>(kv)) mine.add(w);
        calls.set(std::get<0>(kv), mine);
    }
    py::dict<str, py::set<str>> out;
    for (const str& n : texts.keys()) {
        py::set<str> seen;
        seen.add(n);
        std::vector<str> todo{n};
        while (!todo.empty()) {
            const str at = todo.back();
            todo.pop_back();
            for (const str& c : calls.at(at))
                if (!seen.contains(c)) {
                    seen.add(c);
                    todo.push_back(c);
                }
        }
        out.set(n, seen);
    }
    return out;
}

namespace {

// what the judge said of one function: (verdict, reason, limits, detail); the verdict None for a function that was not built
struct Said {
    std::optional<str> verdict;
    str reason;
    py::list<str> limits;
};

}  // namespace

namespace {

// `m.texts = texts`. In the Python one object is the module the lane builds from and the one teeth reads; here the lane's own module
// (a translate::l2::Made, the first base of the Module16 / Module32 / ModulePPC the lane keeps in Prepared::lane) holds the texts it builds
// from, so they are set there as well as in the Prepared.
void put_texts(R::Prepared& m, const str& kind, const py::dict<str, str>& texts) {
    m.texts = texts;
    if (m.lane && (kind == S("ne") || kind == S("pe") || kind == S("powerpc")))
        std::static_pointer_cast<translate::l2::Made>(m.lane)->set_texts(texts);
}

}  // namespace

Report run(const str& module_path, int64_t rounds, const Log& log) {
    std::shared_ptr<R::Kind> kind = R::kind_of(module_path);
    if (!kind->PER_FUNCTION())
        throw py::SystemExit(py::percent(S("%s: this kind of module is rebuilt whole and compared byte for byte; there is no judge of behaviour to test"),
                                         py::os::path::basename(module_path)));
    auto fs = store::functions::open();
    auto [sha, digest] = R::run(module_path, fs, log);      // make sure it has been judged
    const str name = py::os::path::basename(module_path).lower();
    const str work = build::watcom::_scratch(py::list<str>{S("teeth"), kind->KIND(), name});
    R::Prepared m = kind->prepare(module_path, work);
    const py::dict<str, str> key_of = m.key.copy();
    py::dict<str, str> passed;
    for (const auto& kv : fs->best(sha, digest).items()) {
        const eng::Cell& klass = std::get<1>(kv).klass;
        if (std::holds_alternative<str>(klass) && (std::get<str>(klass) == V::VERIFIED || std::get<str>(klass) == V::WEAK))
            passed.set(std::get<0>(kv), std::get<str>(klass));
    }
    py::list<str> candidates;
    for (const str& n : m.order)
        if (key_of.contains(n) && passed.contains(key_of.at(n)) && !breaks(m.texts.at(n)).empty()) candidates.append(n);
    const py::dict<str, py::set<str>> reach = _reach(m.texts);
    const py::dict<str, str> original_texts = m.texts.copy();

    py::set<str> used;
    int64_t r = 0;
    bool over = false;
    std::function<std::optional<Round>()> broken = [&]() -> std::optional<Round> {
        if (over || r >= rounds) return std::nullopt;
        py::list<str> chosen;
        py::set<str> taken;
        for (const str& n : candidates) {
            if (used.contains(n)) continue;
            bool clash = false;
            for (const str& x : reach.at(n))
                if (taken.contains(x)) {
                    clash = true;
                    break;
                }
            for (const str& c : chosen)
                if (reach.at(c).contains(n)) clash = true;
            if (clash) continue;
            chosen.append(n);
            for (const str& x : reach.at(n)) taken.add(x);
        }
        if (chosen.empty()) {
            over = true;
            return std::nullopt;
        }
        for (const str& n : chosen) used.add(n);
        Round batch;
        int64_t i = 0;
        for (const str& n : chosen) {
            auto options = breaks(original_texts.at(n));
            const auto& pick = options[(i + r) % static_cast<int64_t>(options.size())];   // the kinds of break in turn
            batch.emplace_back(n, std::get<0>(pick), std::get<1>(pick));
            ++i;
        }
        ++r;
        return batch;
    };

    int64_t number = 0;
    std::function<py::dict<str, Said>(const Round&)> judge = [&](const Round& batch) -> py::dict<str, Said> {
        py::dict<str, str> texts = original_texts.copy();
        for (const Break& b : batch) texts.set(std::get<0>(b), std::get<2>(b));
        put_texts(m, kind->KIND(), texts);
        R::Built built = kind->build(m, module_path, py::os::path::join(work, py::percent(S("round%d"), number++)));
        py::list<std::tuple<str, R::Item>> todo;
        for (const Break& b : batch)
            if (built.items.contains(std::get<0>(b))) todo.append(std::make_tuple(key_of.at(std::get<0>(b)), built.items.at(std::get<0>(b))));
        jobs::pool::Options<std::tuple<str, R::Item>, R::Verdict> opts;
        opts.crashed = [](const std::tuple<str, R::Item>&) { return R::NOT_RUN(); };
        opts.jobs = 1;                                            // (the emulators keep per-process state)
        const R::Kind& kref = *kind;
        auto judged = jobs::pool::run_first(
            [&kref](const R::Context& context, const std::tuple<str, R::Item>& item) { return kref.judge(context, std::get<1>(item)); }, todo,
            built.context, opts);
        py::dict<str, Said> out;
        for (const Break& b : batch) {
            const str& n = std::get<0>(b);
            if (built.items.contains(n)) {
                const R::Verdict& v = judged.at(key_of.at(n));
                out.set(n, Said{std::get<0>(v), std::get<1>(v), std::get<2>(v)});
            } else {
                out.set(n, Said{std::nullopt, built.dropped.get(n, S("")), py::list<str>()});
            }
        }
        return out;
    };

    std::function<str(const Said&)> outcome = [](const Said& said) -> str {
        if (said.verdict && R::GOOD().contains(*said.verdict)) return S("missed");
        if (said.verdict && *said.verdict == V::DIVERGENT) return S("caught");
        return said.verdict ? *said.verdict : S("");
    };

    Report report;
    int64_t round_no = 0;
    std::function<void(const std::vector<Planted<Said>>&)> each = [&](const std::vector<Planted<Said>>& batch) {
        for (const auto& p : batch) {
            Result x;
            x.name = p.name;
            x.was = passed.at(key_of.at(p.name));
            x.made = p.what;
            x.reason = p.said.reason;
            if (!p.said.verdict) {
                x.compiled = false;
                x.now = S("does not compile");
            } else {
                x.change = _changed(original_texts.at(p.name), p.made);
                x.now = p.outcome == S("missed") ? S("survived") : p.outcome == S("caught") ? S("caught") : S("not judged");
                x.limits = py::join(S(","), p.said.limits);
            }
            report.results.append(x);
        }
        if (log) {
            int64_t caught = 0, done = 0;
            for (const Result& x : report.results)
                if (x.now == S("caught") || x.now == S("survived")) {
                    ++done;
                    caught += x.now == S("caught");
                }
            log(py::percent(S("  round %d: %d functions broken; so far %d of %d caught"), round_no + 1, static_cast<int64_t>(batch.size()), caught, done));
        }
        ++round_no;
    };
    plant<Said>(broken, judge, outcome, each);
    put_texts(m, kind->KIND(), original_texts);

    std::vector<Caught> tally;
    auto group_of = [&](const str& group) -> Caught& {
        for (Caught& c : tally)
            if (c.group == group) return c;
        Caught c;
        c.group = group;
        tally.push_back(c);
        return tally.back();
    };
    for (const Result& x : report.results) {
        if (x.now == S("caught") || x.now == S("survived")) {
            for (const str& g : {x.was, S("by break: ") + x.made}) {
                Caught& c = group_of(g);
                c.caught += x.now == S("caught");
                c.of += 1;
            }
        }
    }
    std::vector<Caught> sorted_tally = tally;
    std::sort(sorted_tally.begin(), sorted_tally.end(), [](const Caught& a, const Caught& b) { return a.group.u32() < b.group.u32(); });
    report.module = name;
    report.kind = kind->KIND();
    report.rounds = rounds;
    report.caught = py::list<Caught>(sorted_tally);
    return report;
}

namespace {

void put_str(std::string& o, const str& s) { py::json::put(o, s); }

}  // namespace

py::str report_json(const Report& report) {
    std::string o = "{\n \"module\": ";
    put_str(o, report.module);
    o += ",\n \"kind\": ";
    put_str(o, report.kind);
    o += ",\n \"rounds\": " + std::to_string(report.rounds) + ",\n \"results\": ";
    if (report.results.empty()) {
        o += "[]";
    } else {
        o += "[\n";
        bool first = true;
        for (const Result& x : report.results) {
            o += first ? "  {\n" : ",\n  {\n";
            first = false;
            auto field = [&](const char* k, const str& v, bool last = false) {
                o += std::string("   \"") + k + "\": ";
                put_str(o, v);
                o += last ? "\n" : ",\n";
            };
            if (!x.compiled) {
                field("name", x.name);
                field("was", x.was);
                field("now", x.now);
                field("reason", x.reason);
                field("made", x.made, true);
            } else {
                field("name", x.name);
                field("was", x.was);
                field("made", x.made);
                field("reason", x.reason);
                field("change", x.change);
                field("now", x.now);
                field("limits", x.limits, true);
            }
            o += "  }";
        }
        o += "\n ]";
    }
    o += ",\n \"caught\": ";
    if (report.caught.empty()) {
        o += "{}";
    } else {
        o += "{\n";
        bool first = true;
        for (const Caught& c : report.caught) {
            o += first ? "  " : ",\n  ";
            first = false;
            put_str(o, c.group);
            o += ": {\n   \"caught\": " + std::to_string(c.caught) + ",\n   \"of\": " + std::to_string(c.of) + "\n  }";
        }
        o += "\n }";
    }
    o += "\n}";
    return str(o);
}

const str DOC = S(
    "python -m codebreaker teeth <module> [--rounds N] [--json FILE]\n"
    "\n"
    "Break functions the judge has passed, one small way each, and see whether it notices.");

int64_t main(const py::list<str>& argv) {
    py::list<str> args;
    for (const str& a : argv)
        if (!a.startswith(S("--"))) args.append(a);
    auto after = [&](const char* flag) -> str {
        for (int64_t i = 0; i < static_cast<int64_t>(argv.size()); ++i)
            if (argv[i] == S(flag)) {
                if (i + 1 >= static_cast<int64_t>(argv.size())) throw py::IndexError("list index out of range");
                return argv[i + 1];
            }
        return S("");
    };
    const int64_t rounds = argv.contains(S("--rounds")) ? py::int_parse(after("--rounds")) : 3;
    std::optional<str> out;
    if (argv.contains(S("--json"))) out = after("--json");
    py::list<str> rest;
    for (const str& a : args)
        if (a != py::to_str(rounds) && !(out && a == *out)) rest.append(a);
    args = rest;
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    std::optional<str> path = py::os::path::isfile(args[0]) ? std::optional<str>(args[0]) : base::corpus::retail(args[0]);
    if (!path || path->empty()) throw py::SystemExit(py::percent(S("no module %s"), py::repr(args[0])));
    path = base::corpus::_readable(*path);   // (a .xex: its basefile, as the lanes read it)
    const Report report = run(*path, rounds);
    str file = out ? *out : py::os::path::join(py::settings::get_str(S("state")), S("teeth"), report.module + S(".json"));
    // (the Python makes the folder of the file even when `file` has none, which fails for a bare file name: not repeated)
    if (!py::os::path::dirname(file).empty()) py::os::makedirs(py::os::path::dirname(file), 0777, true);
    {
        py::file f = py::open(file, S("w"), std::optional<str>(S("utf-8")));
        f.write(report_json(report));
    }
    const py::dict<str, str> titles = {{S("verified"), S("it had called VERIFIED")}, {S("weak"), S("it had called VERIFIED (weak)")}};
    for (const Caught& c : report.caught)
        py::print(py::percent(S("  %-34s caught %d of %d  (%d%%)"), titles.get(c.group, c.group), c.caught, c.of, 100 * c.caught / std::max<int64_t>(c.of, 1)));
    int64_t other = 0;
    for (const Result& x : report.results)
        if (x.now != S("caught") && x.now != S("survived")) ++other;
    if (other) py::print(py::percent(S("  not counted: %d (the break did not compile, or could not be judged)"), other));
    int64_t shown = 0;
    for (const Result& x : report.results) {
        if (x.now == S("survived") && x.was == S("verified")) {
            if (shown++ >= 12) break;
            py::print(py::percent(S("    survived: %-24s %-10s %s"), x.name, x.made, x.limits));
        }
    }
    py::print(py::percent(S("written to %s"), file));
    return 0;
}

}  // namespace codebreaker::verify::teeth
