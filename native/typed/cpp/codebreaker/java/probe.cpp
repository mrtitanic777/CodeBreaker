// codebreaker/java/probe.cpp -- typed C++ port of codebreaker.java.probe (see probe.hpp).

#include "codebreaker/java/probe.hpp"

#include <algorithm>
#include <cstring>
#include <map>
#include <mutex>
#include <thread>

#include "codebreaker/base/parallel.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/textutil.hpp"
#include "codebreaker/java/transpile.hpp"
#include "codebreaker/java/twophase.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::java::probe {

namespace {

using py::str;

str S(const char* s) { return str(s); }

// ---- subprocess.run(..., timeout=) of this module: stdin is the null device, the streams are captured (bytes) or discarded ----
enum class Sink { Capture, Devnull, IntoStdout };

struct Ran {
    int64_t returncode = 0;
    std::string out, err;   // raw bytes
    bool timed_out = false;
};

// Runs `cmd`, waiting at most `timeout_s` seconds (0: forever). On a timeout the child this started is ended
// (subprocess.run's TimeoutExpired path).
Ran run_(const py::list<str>& cmd, Sink out, Sink err, int64_t timeout_s, const std::optional<str>& cwd = std::nullopt) {
    py::subprocess::RunOptions o;
    o.text = false;
    o.stdin_ = py::subprocess::Stream::Devnull;
    o.stdout_ = out == Sink::Capture ? py::subprocess::Stream::Capture : py::subprocess::Stream::Devnull;
    o.stderr_ = err == Sink::Capture ? py::subprocess::Stream::Capture
                : err == Sink::IntoStdout ? py::subprocess::Stream::Stdout
                                          : py::subprocess::Stream::Devnull;
    o.cwd = cwd;
    if (timeout_s > 0) o.timeout = timeout_s;
    Ran r;
    try {
        py::subprocess::CompletedProcess p = py::subprocess::run(cmd, o);
        r.returncode = p.returncode;
        if (p.stdout_bytes) r.out = p.stdout_bytes->raw();
        if (p.stderr_bytes) r.err = p.stderr_bytes->raw();
    } catch (const py::subprocess::TimeoutExpired& e) {
        r.timed_out = true;
        r.returncode = 1;   // (the child was ended with exit code 1)
        if (e.stdout_bytes) r.out = e.stdout_bytes->raw();
        if (e.stderr_bytes) r.err = e.stderr_bytes->raw();
    }
    return r;
}

// pool.threads(fn, items, jobs): fn on every item, `jobs` at a time on threads of this process; the results in the items' order.
// (codebreaker.jobs.pool has a typed port, but its threads() reaches the knowledge / engine cycle for the asker's recording of
// reads, which nothing here uses: the same loop is written out so this module does not link that cycle.)
template <class Fn, class Item>
auto threads_(Fn fn, const py::list<Item>& items, int64_t jobs) {
    using R = decltype(fn(std::declval<const Item&>()));
    std::vector<R> out(static_cast<size_t>(items.size()));
    std::atomic<int64_t> next{0};
    std::mutex failed_lock;
    std::exception_ptr failed;
    auto work = [&]() {
        for (;;) {
            int64_t i = next.fetch_add(1);
            if (i >= items.size()) return;
            try {
                out[static_cast<size_t>(i)] = fn(items[i]);
            } catch (...) {
                std::lock_guard<std::mutex> hold(failed_lock);
                if (!failed) failed = std::current_exception();
            }
        }
    };
    int64_t n = std::max<int64_t>(1, std::min<int64_t>(jobs, items.size()));
    std::vector<std::thread> ts;
    for (int64_t k = 1; k < n; k++) ts.emplace_back(work);
    work();
    for (auto& t : ts) t.join();
    if (failed) std::rethrow_exception(failed);
    return out;
}

str decode_(const std::string& raw) { return py::bytes(raw).decode("utf-8", "replace"); }

std::mutex& caches_() {
    static std::mutex m;
    return m;
}

str read_text_(const str& path) { return textutil::read_text(path, S("replace"), false); }   // (raises OSError)

}  // namespace

str CXX_STD = S("c++03");

const str _CALL_RE = S("\\bstatic\\s+[\\w<>* ]+\\b%s\\s*\\(");

const py::dict<str, std::tuple<str, py::list<str>>> _INPUTS = {
    {S("int"), {S("int"), {S("0"), S("1"), S("-1"), S("2"), S("7"), S("-13"), S("100"), S("255"), S("256"), S("1000"), S("-1000"),
                           S("65535"), S("123456"), S("2147483647"), S("-2147483648")}}},
    {S("long"), {S("long long"), {S("0L"), S("1L"), S("-1L"), S("1000000007L"), S("-123456789012L"), S("9223372036854775807L")}}},
    {S("short"), {S("short"), {S("0"), S("1"), S("-1"), S("255"), S("-32768"), S("32767")}}},
    {S("byte"), {S("signed char"), {S("0"), S("1"), S("-1"), S("100"), S("-128"), S("127")}}},
    {S("char"), {S("unsigned char"), {S("0"), S("32"), S("65"), S("97"), S("122"), S("167"), S("255")}}},
    {S("boolean"), {S("bool"), {S("true"), S("false")}}},
    {S("float"), {S("float"), {S("0.0F"), S("1.0F"), S("-1.0F"), S("0.5F"), S("-0.5F"), S("3.14159F"), S("100.25F"), S("-1000.75F"),
                              S("0.001F"), S("1000000.0F"), S("-0.0F")}}},
    {S("double"), {S("double"), {S("0.0"), S("1.0"), S("-1.0"), S("0.5"), S("-0.5"), S("3.14159"), S("100.25"), S("-1000.75"),
                                 S("0.001"), S("1000000.0"), S("-0.0"), S("12345.678")}}},
    {S("String"), {S("JavaString"), {S(""), S("a"), S("Hello World"), S("123"), S("-45"), S("minecraft"), S(" pad "), S("A.B.C"), S("x=1,y=2"),
                                    S("\u00a7cRed")}}},
};

// ================================================================================================== classes
namespace {

std::map<std::string, py::dict<str, str>>& index_cache_() {
    static std::map<std::string, py::dict<str, str>> m;
    return m;
}

std::map<std::pair<std::string, std::string>, ClassRef>& parsed_cache_() {
    static std::map<std::pair<std::string, std::string>, ClassRef> m;
    return m;
}

py::dict<str, str> class_index_unlocked_(const str& java_dir) {
    auto& cache = index_cache_();
    std::string k = java_dir.utf8();
    auto it = cache.find(k);
    if (it == cache.end()) {
        py::dict<str, str> found;
        for (const auto& w : py::os::walk(java_dir)) {
            for (const str& f : std::get<2>(w)) {
                if (f.endswith(S(".java"))) found.setdefault(f.slice(0, f.size() - 5), py::os::path::join(std::get<0>(w), f));
            }
        }
        it = cache.emplace(k, found).first;
    }
    return it->second;
}

}  // namespace

py::dict<str, str> class_index(const str& java_dir) {
    std::lock_guard<std::mutex> hold(caches_());
    return class_index_unlocked_(java_dir);
}

ClassRef lookup(const str& java_dir, const str& name) {
    std::lock_guard<std::mutex> hold(caches_());
    auto key = std::make_pair(java_dir.utf8(), name.utf8());
    auto& cache = parsed_cache_();
    auto it = cache.find(key);
    if (it == cache.end()) {
        py::dict<str, str> idx = class_index_unlocked_(java_dir);
        ClassRef parsed;
        if (idx.contains(name)) {
            try {
                parsed = parse::parse_file(idx.at(name));
            } catch (const py::Exception&) {
                parsed = nullptr;
            }
        }
        it = cache.emplace(key, parsed).first;
    }
    return it->second;
}

bool _simple(const JTypeRef& t) { return !t->is_array() && _INPUTS.contains(t->name); }

std::optional<Maker> maker(const ClassRef& cls) {
    if (!cls || cls->is_abstract || cls->is_interface || cls->is_enum) return std::nullopt;
    py::list<py::ref<ir::Method>> ctors;
    for (const auto& m : cls->methods)
        if (m->is_ctor) ctors.append(m);
    if (!py::truthy(ctors)) return Maker{std::nullopt, py::list<str>()};   // Java's implicit default constructor
    for (const auto& m : ctors) {
        bool all = true;
        for (const auto& p : m->params)
            if (!_simple(p->jtype)) all = false;
        if (m->access != ir::PRIVATE && all) {
            py::list<str> ps;
            for (const auto& p : m->params) ps.append(p->jtype->name);
            return Maker{std::nullopt, ps};
        }
    }
    for (const auto& m : cls->methods) {
        const JTypeRef& rt = m->return_type;
        bool all = true;
        for (const auto& p : m->params)
            if (!_simple(p->jtype)) all = false;
        if (m->is_static && !m->is_ctor && m->access != ir::PRIVATE && rt && !rt->is_array() && rt->name == cls->name && all) {
            py::list<str> ps;
            for (const auto& p : m->params) ps.append(p->jtype->name);
            return Maker{m->name, ps};
        }
    }
    return std::nullopt;
}

Fields fields(const str& java_dir, const ClassRef& cls, const str& cpp_dir) {
    Fields out;
    py::set<str> seen;
    ClassRef c = cls;
    while (c && !seen.contains(c->name)) {
        seen.add(c->name);
        str header;
        try {
            header = read_text_(py::os::path::join(cpp_dir, c->name + S(".h")));
        } catch (const py::OSError&) {
            header = str();
        }
        for (const auto& f_ : c->fields) {
            bool dup = false;
            for (const auto& o : out)
                if (std::get<0>(o) == f_->name) dup = true;
            if (!f_->is_static && f_->access != ir::PRIVATE && _simple(f_->jtype) &&
                py::re::search(S("\\b") + py::re::escape(f_->name) + S("\\s*;"), header) && !dup)
                out.append(std::make_tuple(f_->name, f_->jtype->name));
        }
        c = (c->parent && !c->parent->empty()) ? lookup(java_dir, *c->parent) : nullptr;
    }
    return out;
}

// ================================================================================================== what can be run
std::optional<Slot> _slot(const str& java_dir, const JTypeRef& t) {
    if (_simple(t)) return Slot(t->name);
    if (t->is_array()) return std::nullopt;
    std::optional<Maker> made = maker(lookup(java_dir, t->name));
    if (!made) return std::nullopt;
    return Slot(ObjSlot{t->name, *made});
}

Result _result(const str& java_dir, const str& cpp_dir, const JTypeRef& t) {
    Result r;
    if (!t || t->is_array()) return r;
    if (_INPUTS.contains(t->name)) {
        r.kind = Result::Prim;
        r.type = t->name;
        return r;
    }
    ClassRef cls = lookup(java_dir, t->name);
    Fields fs;
    if (cls) fs = fields(java_dir, cls, cpp_dir);
    if (!py::truthy(fs)) return r;
    r.kind = Result::Obj;
    r.cls = t->name;
    r.fields = fs;
    return r;
}

const str _CLOCK_PATTERN =
    S("\\b(?:System\\s*\\.\\s*(?:currentTimeMillis|nanoTime)|new\\s+Date|[\\w.]*getSystemClipboard)\\s*\\(");

std::tuple<py::set<str>, bool> reads_clock(const ClassRef& cls) {
    static const py::re::Pattern clock = py::re::compile(_CLOCK_PATTERN);
    auto text = [](const py::ref<ir::Method>& m) {
        py::list<str> lines;
        for (const auto& t : m->body) lines.append(std::get<1>(t));
        return py::join(S("\n"), lines);
    };
    auto calls = [](const str& c, const str& t) {
        return py::re::search(S("(?<![\\w.])(?:this\\s*\\.\\s*)?") + py::re::escape(c) + S("\\s*\\("), t).has_value();
    };
    py::dict<str, str> bodies;
    for (const auto& m : cls->methods)
        if (!m->is_ctor) bodies.set(m->name, text(m));
    py::set<str> clocked;
    for (const auto& kv : bodies.items())
        if (clock.search(std::get<1>(kv))) clocked.add(std::get<0>(kv));
    bool grew = true;
    while (grew) {
        grew = false;
        for (const auto& kv : bodies.items()) {
            const str& n = std::get<0>(kv);
            if (!clocked.contains(n)) {
                bool any = false;
                for (const str& c : clocked)
                    if (calls(c, std::get<1>(kv))) {
                        any = true;
                        break;
                    }
                if (any) {
                    clocked.add(n);
                    grew = true;
                }
            }
        }
    }
    py::list<str> ctor_texts;
    for (const auto& m : cls->methods)
        if (m->is_ctor) ctor_texts.append(text(m));
    str ctor = py::join(S("\n"), ctor_texts);
    {
        py::list<str> inits;   // ctor += "\n".join(f.initializer for f in cls.instance_fields() if f.initializer)
        for (const auto& f : cls->instance_fields())
            if (py::truthy(f->initializer)) inits.append(*f->initializer);
        ctor += py::join(S("\n"), inits);
    }
    bool in_ctor = clock.search(ctor).has_value();
    if (!in_ctor) {
        for (const str& c : clocked)
            if (calls(c, ctor)) {
                in_ctor = true;
                break;
            }
    }
    return {clocked, in_ctor};
}

py::list<Probeable> probeable(const ClassRef& cls, const str& header_text, const std::optional<str>& java_dir, const std::optional<str>& cpp_dir) {
    py::list<Probeable> out;
    py::set<std::tuple<str, str>> seen;
    std::optional<Maker> made = maker(cls);
    auto clk = reads_clock(cls);
    const py::set<str>& clocked = std::get<0>(clk);
    bool ctor_clocked = std::get<1>(clk);
    std::optional<std::tuple<str, Fields>> me;
    if (java_dir && cpp_dir) {
        Fields fs = fields(*java_dir, cls, *cpp_dir);
        if (py::truthy(fs)) me = std::make_tuple(cls->name, fs);
    }
    for (const auto& m : cls->methods) {
        if (m->is_ctor || m->is_abstract || m->access == ir::PRIVATE || (!m->is_static && !made)) continue;
        if (clocked.contains(m->name) || (ctor_clocked && !m->is_static)) continue;   // depends on the time: not comparable
        const JTypeRef& rt = m->return_type;
        Result result;
        bool self_pending = false;
        if (rt && rt->name == S("void") && !rt->is_array()) {
            if (!m->is_static && me) self_pending = true;
        } else if (java_dir) {
            result = _result(*java_dir, cpp_dir ? *cpp_dir : str(), rt);
        } else if (rt && _simple(rt)) {
            result.kind = Result::Prim;
            result.type = rt->name;
        }
        if (result.kind == Result::None && !self_pending) continue;
        py::list<Slot> slots;
        bool ok = true;
        for (const auto& p : m->params) {
            std::optional<Slot> s;
            if (java_dir) {
                s = _slot(*java_dir, p->jtype);
            } else if (_simple(p->jtype)) {
                s = Slot(p->jtype->name);
            }
            if (!s) {
                ok = false;
                break;
            }
            slots.append(*s);
        }
        if (!ok) continue;
        py::list<str> reprs;
        for (const auto& p : m->params) reprs.append(p->jtype->repr());
        auto key = std::make_tuple(m->name, py::join(S("\x1f"), reprs));
        str pattern = m->is_static ? py::percent(_CALL_RE, py::re::escape(m->name)) : S("\\b") + py::re::escape(m->name) + S("\\s*\\(");
        if (seen.contains(key) || !py::re::search(pattern, header_text)) continue;
        seen.add(key);
        if (self_pending) {   // compared by the object's fields afterwards
            result.kind = Result::Self;
            result.cls = std::get<0>(*me);
            result.fields = std::get<1>(*me);
        }
        Probeable p;
        p.name = m->name;
        p.slots = slots;
        p.result = result;
        if (!m->is_static) p.made = made;
        out.append(p);
    }
    return out;
}

int64_t counted(const ClassRef& cls) {
    int64_t n = 0;
    for (const auto& m : cls->methods)
        if (!m->is_abstract) n++;
    return n;
}

// ================================================================================================== inputs
py::list<str> _flat(const py::list<Slot>& slots, const std::optional<Maker>& made) {
    py::list<str> out;
    if (made) out.extend(made->params);
    for (const Slot& s : slots) {
        if (const auto* o = std::get_if<ObjSlot>(&s)) out.extend(o->made.params);
        else out.append(std::get<str>(s));
    }
    return out;
}

py::list<py::list<str>> cases(const py::list<str>& types) {
    py::list<py::list<str>> result;
    std::vector<py::list<str>> pools;
    for (const str& t : types) pools.push_back(std::get<1>(_INPUTS.at(t)));
    if (pools.empty()) {
        result.append(py::list<str>());
        return result;
    }
    int64_t total = 1;
    for (const auto& p : pools) total *= p.size();
    if (total <= _MAX_CASES) {   // itertools.product: the last pool varies fastest
        std::vector<int64_t> idx(pools.size(), 0);
        for (int64_t n = 0; n < total; n++) {
            py::list<str> c;
            for (size_t k = 0; k < pools.size(); k++) c.append(pools[k][idx[k]]);
            result.append(c);
            for (int64_t k = static_cast<int64_t>(pools.size()) - 1; k >= 0; k--) {
                if (++idx[k] < pools[k].size()) break;
                idx[k] = 0;
            }
        }
        return result;
    }
    // too many to try them all (or even to list them): a fixed spread -- the k-th parameter steps through
    // its inputs at its own stride -- then every input of every parameter at least once
    py::list<py::list<str>> picked;
    for (int64_t i = 0; i < _MAX_CASES; i++) {
        py::list<str> c;
        for (size_t k = 0; k < pools.size(); k++) {
            int64_t kk = static_cast<int64_t>(k);
            c.append(pools[k][(i * (2 * kk + 1) + kk) % pools[k].size()]);
        }
        if (!picked.contains(c)) picked.append(c);
    }
    for (size_t k = 0; k < pools.size(); k++) {
        for (const str& v : pools[k]) {
            bool found = false;
            for (const auto& c : picked)
                if (c[k] == v) found = true;
            if (!found) {
                py::list<str> c;
                for (size_t j = 0; j < pools.size(); j++) c.append(j == k ? v : pools[j][0]);
                picked.append(c);
            }
        }
    }
    return picked;
}

namespace {

str hex4_(char32_t c, const char* fmt) {
    char buf[16];
    std::snprintf(buf, sizeof buf, fmt, static_cast<unsigned>(c));
    return str(std::string(buf));
}

}  // namespace

str _java_literal(const str& t, const str& v) {
    if (t == S("String")) {
        str body;
        for (char32_t c : v.u32()) {
            if (c >= 32 && c < 127 && c != U'"' && c != U'\\') body += str(c);
            else body += S("\\u") + hex4_(c, "%04x");
        }
        return S("\"") + body + S("\"");
    }
    if (t == S("byte") || t == S("short") || t == S("char")) return py::percent(S("(%s)%s"), t, v);
    return v;
}

str _cpp_literal(const str& t, const str& v) {
    if (t == S("String")) {
        str body;
        for (char32_t c : v.u32()) {
            if (c >= 32 && c < 127 && c != U'"' && c != U'\\') body += str(c);
            else body += S("\\x") + hex4_(c, "%02x") + S("\" \"");
        }
        return S("JavaString(\"") + body + S("\")");
    }
    if (t == S("long")) return v.startswith(S("-9223")) ? S("(-9223372036854775807LL - 1)") : v.slice(0, v.size() - 1) + S("LL");
    if ((t == S("float") || t == S("double")) && v.startswith(S("-"))) return S("(") + v + S(")");
    if (t == S("int") && v == S("-2147483648")) return S("(-2147483647 - 1)");
    if (t == S("byte") || t == S("short") || t == S("char")) return py::percent(S("(%s)%s"), std::get<0>(_INPUTS.at(t)), v);
    return v;
}

std::tuple<std::optional<str>, py::list<str>> _args(const py::list<Slot>& slots, const std::optional<Maker>& made, const py::list<str>& values_in,
                                                    const str& lang, const str& cls_name) {
    bool java = lang == S("java");
    py::list<str> values = values_in.copy();

    auto lit = [&](const str& t, const str& v) { return java ? _java_literal(t, v) : _cpp_literal(t, v); };
    auto take = [&](const py::list<str>& types) {
        int64_t n = types.size();
        py::list<str> got = values.slice(0, n);
        values = values.slice(n, values.size());
        py::list<str> parts;
        int64_t i = 0;
        for (const str& v : got) {
            if (i < types.size()) parts.append(lit(types[i], v));
            i++;
        }
        return py::join(S(", "), parts);
    };
    auto make = [&](const str& name, const Maker& how) {
        str a = take(how.params);
        if (!how.factory) return py::percent(S("new %s(%s)"), name, a);
        return py::percent(java ? S("%s.%s(%s)") : S("%s::%s(%s)"), name, *how.factory, a);
    };

    std::optional<str> obj;
    if (made) obj = make(cls_name, *made);
    py::list<str> args;
    for (const Slot& s : slots) {
        if (const auto* o = std::get_if<ObjSlot>(&s)) args.append(make(o->cls, o->made));
        else args.append(take(py::list<str>{std::get<str>(s)}));
    }
    return {obj, args};
}

// ================================================================================================== harness
const str _JAVA_HELPERS = S(R"JX(
  static void out(String k, String v) { System.out.println(k + "|" + v); }
  static String fi(long v) { return Long.toString(v); }
  static String fc(char v) { return Integer.toString((int)v); }
  static String fb(boolean v) { return v ? "1" : "0"; }
  static String ff(float v) { return Integer.toHexString(Float.floatToRawIntBits(v)); }
  static String fd(double v) { return Long.toHexString(Double.doubleToRawLongBits(v)); }
  static String fs(String v) {
    if (v == null) return "";
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < v.length(); ++i) { int c = v.charAt(i); b.append(c < 256 ? String.format("%02x", c) : "u" + Integer.toHexString(c)); }
    return b.toString();
  }
)JX");

const str _CPP_HELPERS = S(R"JX(#include <cstdio>
#include <cstring>
#include <string>
static void out(const char* k, const std::string& v) { std::printf("%s|%s\n", k, v.c_str()); }
static std::string fi(long long v) { char b[32]; std::sprintf(b, "%lld", v); return b; }
static std::string fc(unsigned char v) { char b[8]; std::sprintf(b, "%d", (int)v); return b; }
static std::string fb(bool v) { return v ? "1" : "0"; }
static std::string ff(float v) { unsigned int u; std::memcpy(&u, &v, 4); char b[16]; std::sprintf(b, "%x", u); return b; }
static std::string fd(double v) { unsigned long long u; std::memcpy(&u, &v, 8); char b[24]; std::sprintf(b, "%llx", u); return b; }
static std::string fs(const JavaString& v) {
	std::string r; char b[4];
	for (std::string::size_type i = 0; i < v.size(); ++i) { std::sprintf(b, "%02x", (unsigned char)v[i]); r += b; }
	return r;
}
)JX");

const py::dict<str, str> _FMT = {{S("int"), S("fi")},   {S("long"), S("fi")},  {S("short"), S("fi")}, {S("byte"), S("fi")}, {S("char"), S("fc")},
                                 {S("boolean"), S("fb")}, {S("float"), S("ff")}, {S("double"), S("fd")}, {S("String"), S("fs")}};

py::list<str> _dumpers(const py::list<Probeable>& methods, const str& lang) {
    py::set<str> done;
    py::list<str> out;
    for (const Probeable& pm : methods) {
        const Result& result = pm.result;
        if ((result.kind == Result::Obj || result.kind == Result::Self) && !done.contains(result.cls)) {
            done.add(result.cls);
            const str& name = result.cls;
            py::list<str> parts;
            for (const auto& f : result.fields) {
                parts.append(lang == S("java") ? py::percent(S(" + \"%s:\" + %s(o.%s)"), std::get<0>(f), _FMT.at(std::get<1>(f)), std::get<0>(f))
                                               : py::percent(S(" + \"%s:\" + %s(o->%s)"), std::get<0>(f), _FMT.at(std::get<1>(f)), std::get<0>(f)));
            }
            str joined = py::join(S(" + \",\""), parts);
            if (lang == S("java")) {
                out.append(py::percent(S("  static String dump_%s(%s o) { if (o == null) return \"null\"; return \"\"%s; }"), name, name, joined));
            } else {
                out.append(py::percent(S("static std::string dump_%s(%s* o) { if (o == NULL) return \"null\"; return std::string()%s; }"), name, name, joined));
            }
        }
    }
    return out;
}

str _statement(const str& name, const py::list<Slot>&, const Result& result, const std::optional<Maker>& made, const std::optional<str>& obj,
               const py::list<str>& args, const str& lang, const str& cls_name) {
    str a = py::join(S(", "), args);
    bool java = lang == S("java");
    str sep = java ? S(".") : S("->");
    str target;
    if (!made) {
        target = py::percent(S("%s%s%s(%s)"), cls_name, java ? S(".") : S("::"), name, a);
    } else {
        target = py::percent(S("(%s)%s%s(%s)"), obj ? *obj : S("None"), sep, name, a);
    }
    if (result.kind == Result::Self) {
        str var = py::percent(java ? S("%s o = %s;") : S("%s* o = %s;"), cls_name, obj ? *obj : S("None"));
        return py::percent(S("%s o%s%s(%s); out(\"{key}\", dump_%s(o));"), var, sep, name, a, cls_name);
    }
    if (result.kind == Result::Obj) return py::percent(S("out(\"{key}\", dump_%s(%s));"), result.cls, target);
    return py::percent(S("out(\"{key}\", %s(%s));"), _FMT.at(result.type), target);
}

str java_harness(const str& cls_name, const py::list<Probeable>& methods, const std::optional<str>& package, const str& name,
                 const std::optional<py::set<str>>& only) {
    py::list<str> lines;
    if (package && !package->empty()) lines.append(py::percent(S("package %s;"), *package));
    lines.append(py::percent(S("public class %s {"), name));
    lines.extend(py::split(_JAVA_HELPERS.strip(S("\n")), S("\n")));
    lines.extend(_dumpers(methods, S("java")));
    lines.append(S("  public static void main(String[] a) {"));
    int64_t mi = 0;
    for (const Probeable& pm : methods) {
        int64_t ci = 0;
        for (const py::list<str>& values : cases(_flat(pm.slots, pm.made))) {
            str key = py::percent(S("%d.%d"), mi, ci);
            ci++;
            if (only && !only->contains(key)) continue;
            auto oa = _args(pm.slots, pm.made, values, S("java"), cls_name);
            str stmt = _statement(pm.name, pm.slots, pm.result, pm.made, std::get<0>(oa), std::get<1>(oa), S("java"), cls_name).replace(S("{key}"), key);
            lines.append(py::percent(S("    try { %s } catch (Throwable t) { out(\"%s\", \"throw\"); }"), stmt, key));
        }
        mi++;
    }
    lines.extend(py::list<str>{S("  }"), S("}"), S("")});
    return py::join(S("\n"), lines);
}

str cpp_harness(const str& cls_name, const py::list<Probeable>& methods, const py::set<str>& keep, const py::list<str>& init_order_,
                const py::set<str>& extra_headers, const py::list<std::tuple<str, str>>& ancestry_) {
    py::list<str> bases;
    for (const auto& pair : ancestry_) {
        bases.append(std::get<0>(pair));
        bases.append(std::get<1>(pair));
    }
    py::list<str> heads{cls_name};
    for (const str& c : init_order_)
        if (c != cls_name) heads.append(c);
    for (const str& c : py::sorted(extra_headers))
        if (c != cls_name) heads.append(c);
    for (const str& c : bases)
        if (c != cls_name) heads.append(c);
    py::set<str> seen;
    py::list<str> inc;
    for (const str& h : heads) {
        if (!seen.contains(h)) {
            seen.add(h);
            inc.append(py::percent(S("#include \"%s.h\""), h));
        }
    }
    py::list<str> lines{S("#include \"stdafx.h\""), S("#include \"JavaRT.h\"")};
    lines.extend(inc);
    lines.extend(py::split(_CPP_HELPERS.strip(S("\n")), S("\n")));
    lines.extend(_dumpers(methods, S("cpp")));
    lines.append(S("int main() {"));
    // FIXED BUG probe-no-ancestry: the game registers each class's bases at boot (j2x_rtti.cpp) so that
    // instanceof sees subclasses; the harness did not, and Beta's recipe setup crashed in every harness
    // that linked it (x instanceof Block was false for BlockOreStorage)
    for (const auto& pair : ancestry_)
        lines.append(py::percent(S("\tj2xRegisterBase(typeid(%s).name(), typeid(%s).name());"), std::get<0>(pair), std::get<1>(pair)));
    for (const str& c : init_order_) lines.append(py::percent(S("\t%s::initStatics();"), c));
    int64_t mi = 0;
    for (const Probeable& pm : methods) {
        int64_t ci = 0;
        for (const py::list<str>& values : cases(_flat(pm.slots, pm.made))) {
            str key = py::percent(S("%d.%d"), mi, ci);
            ci++;
            if (keep.contains(key)) {
                auto oa = _args(pm.slots, pm.made, values, S("cpp"), cls_name);
                lines.append(py::percent(S("\t{ %s }"),
                                         _statement(pm.name, pm.slots, pm.result, pm.made, std::get<0>(oa), std::get<1>(oa), S("cpp"), cls_name)
                                             .replace(S("{key}"), key)));
            }
        }
        mi++;
    }
    lines.extend(py::list<str>{S("\treturn 0;"), S("}"), S("")});
    return py::join(S("\n"), lines);
}

py::set<str> _classes_named(const py::list<Probeable>& methods, const str& cls_name) {
    py::set<str> out;
    for (const Probeable& pm : methods) {
        for (const Slot& s : pm.slots)
            if (const auto* o = std::get_if<ObjSlot>(&s)) out.add(o->cls);
        if (pm.result.kind == Result::Obj) out.add(pm.result.cls);
    }
    out.discard(cls_name);
    return out;
}

// ================================================================================================== compare
py::dict<str, str> _results(const str& text) {
    py::dict<str, str> out;
    for (const str& line : py::splitlines(text)) {
        auto p = line.strip().partition(S("|"));
        if (!std::get<0>(p).empty()) out.set(std::get<0>(p), std::get<2>(p));
    }
    return out;
}

namespace {

// int(x, 16) as a struct.pack('<I' / '<Q') takes it: the value, or nothing when int() or pack refuses it.
std::optional<uint64_t> hex_bits_(const str& x, bool wide) {
    bool plain = !x.empty() && x.size() <= 16;
    for (char32_t c : x.u32())
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) plain = false;
    uint64_t v = 0;
    if (plain) {
        for (char32_t c : x.u32()) v = v * 16 + static_cast<uint64_t>(c <= '9' ? c - '0' : (c | 0x20) - 'a' + 10);
    } else {
        try {
            int64_t n = py::int_parse(x, 16);
            if (n < 0) return std::nullopt;   // (struct.error: argument out of range)
            v = static_cast<uint64_t>(n);
        } catch (const py::ValueError&) {
            return std::nullopt;
        } catch (const py::OverflowError&) {
            return std::nullopt;
        }
    }
    if (!wide && v > 0xFFFFFFFFull) return std::nullopt;
    return v;
}

}  // namespace

bool same(const str& rt, const str& a, const str& b) {
    if (a == b) return true;
    if (rt == S("float") || rt == S("double")) {
        bool wide = rt == S("double");
        auto fa = hex_bits_(a, wide);
        auto fb = hex_bits_(b, wide);
        if (!fa || !fb) return false;
        auto nan = [&](uint64_t v) {
            if (wide) return ((v >> 52) & 0x7FF) == 0x7FF && (v & 0xFFFFFFFFFFFFFull) != 0;
            return ((v >> 23) & 0xFF) == 0xFF && (v & 0x7FFFFFull) != 0;
        };
        return nan(*fa) && nan(*fb);
    }
    return false;
}

// ================================================================================================== building
str stage_sources(const str& java_dir, const str& dest) {
    static const py::re::Pattern package = py::re::compile(S("^\\s*package\\s+([\\w.]+)\\s*;"), 8);
    for (const str& path : transpile::discover(java_dir)) {
        str text = read_text_(path);
        auto m = package.search(text);
        str folder = dest;
        if (m) {
            for (const str& part : py::split(m->group(1), S("."))) folder = py::os::path::join(folder, part);
        }
        py::os::makedirs(folder, 0777, true);
        textutil::write_text(py::os::path::join(folder, py::os::path::basename(path)), text);
    }
    {
        std::lock_guard<std::mutex> hold(caches_());
        index_cache_().erase(dest.utf8());
    }
    return dest;
}

const str _BASE_PATTERN = S("j2xRegisterBase\\(typeid\\((\\w+)\\)\\.name\\(\\), typeid\\((\\w+)\\)\\.name\\(\\)\\);");

py::list<std::tuple<str, str>> ancestry(const str& cpp_dir) {
    py::list<std::tuple<str, str>> out;
    try {
        str text = read_text_(py::os::path::join(cpp_dir, S("j2x_rtti.cpp")));
        static const py::re::Pattern base = py::re::compile(_BASE_PATTERN);
        for (const auto& m : base.finditer(text)) out.append(std::make_tuple(m.group(1), m.group(2)));
    } catch (const py::OSError&) {
        return py::list<std::tuple<str, str>>();
    }
    return out;
}

py::list<str> init_order(const str& cpp_dir) {
    try {
        str text = read_text_(py::os::path::join(cpp_dir, S("j2x_init.cpp")));
        static const py::re::Pattern init = py::re::compile(S("^\\s*(\\w+)::initStatics\\(\\);"), 8);
        return init.findall(text);
    } catch (const py::OSError&) {
        return py::list<str>();
    }
}

str _compile(const str& cxx, const str& cpp_dir, const str& unit, const str& obj, const std::optional<str>& prefix) {
    if (py::os::path::isfile(obj)) return str();
    str tmp = py::percent(S("%s.%d.tmp"), obj, static_cast<int64_t>(GetCurrentProcessId()));
    py::list<str> cmd{cxx, S("-std=") + CXX_STD, S("-w"), S("-O0"), S("-iquote"), cpp_dir};
    if (prefix && !prefix->empty()) {
        cmd.append(S("-include"));
        cmd.append(*prefix);
    }
    cmd.extend(py::list<str>{S("-c"), unit, S("-o"), tmp});
    Ran r = run_(cmd, Sink::Devnull, Sink::Capture, _STEP);
    if (r.timed_out) throw py::TimeoutError(S("timed out"));
    if (r.returncode != 0) {
        if (py::os::path::exists(tmp)) py::os::remove(tmp);
        py::list<str> lines = py::splitlines(decode_(r.err).strip());
        return py::truthy(lines) ? lines[lines.size() - 1] : S("compile failed");
    }
    py::os::replace(tmp, obj);
    return str();
}

str library(const str& cxx, const str& cpp_dir, const str& work, const std::optional<str>& prefix, const std::optional<int64_t>& jobs_in) {
    str folder = py::os::path::join(work, S("probe"), S("_objects"));
    str lib = py::os::path::join(work, S("probe"), S("libj2x.a"));
    if (py::os::path::isfile(lib)) return lib;
    py::os::makedirs(folder, 0777, true);
    py::list<str> units;
    for (const str& f : py::sorted(py::os::listdir(cpp_dir)))
        if (f.endswith(S(".cpp"))) units.append(f);
    int64_t jobs = jobs_in ? *jobs_in : base::parallel::workers();

    auto one = [&](const str& f) -> std::optional<str> {
        str obj = py::os::path::join(folder, f.slice(0, f.size() - 4) + S(".o"));
        try {
            return _compile(cxx, cpp_dir, py::os::path::join(cpp_dir, f), obj, prefix).empty() ? std::optional<str>(obj) : std::nullopt;
        } catch (const py::TimeoutError&) {
            return std::nullopt;
        }
    };
    auto results = threads_(one, units, jobs);
    py::list<str> objects;
    for (const auto& o : results)
        if (o) objects.append(*o);
    str tmp = py::percent(S("%s.%d.tmp"), lib, static_cast<int64_t>(GetCurrentProcessId()));
    str ar = py::os::path::join(py::os::path::dirname(cxx), S("ar.exe"));
    ar = py::os::path::isfile(ar) ? ar : S("ar");
    str listing = tmp + S(".rsp");   // the member list in a file: the command line would be too long for Windows
    {
        py::list<str> names;
        for (const str& o : objects) names.append(py::os::path::basename(o));
        py::file f = py::open(listing, S("w"), S("utf-8"));
        f.write(py::join(S("\n"), names));
    }
    Ran r = run_(py::list<str>{ar, S("rcs"), tmp, S("@") + listing}, Sink::Devnull, Sink::Devnull, 0, folder);
    py::os::remove(listing);
    if (r.returncode != 0) return str();
    py::os::replace(tmp, lib);
    return lib;
}

std::tuple<bool, str, py::list<str>> build_native(const str& cxx, const str& cpp_dir, const str& harness, const str& exe, const str& lib,
                                                  const std::optional<str>& prefix) {
    static const py::re::Pattern loaded = py::re::compile(S("libj2x\\.a\\(([A-Za-z_]\\w*)\\.o\\)"));
    Ran r;
    str mapfile = exe + S(".map");   // the link map names every archive member taken
    try {
        str main_obj = std::get<0>(py::os::path::splitext(harness)) + S(".o");
        if (py::os::path::exists(main_obj)) py::os::remove(main_obj);
        str err = _compile(cxx, cpp_dir, harness, main_obj, prefix);
        if (!err.empty()) return {false, S("the harness did not compile: ") + err.slice(0, 160), py::list<str>()};
        r = run_(py::list<str>{cxx, main_obj, S("-Wl,--start-group"), lib, S("-Wl,--end-group"), S("-Wl,-Map,") + mapfile, S("-o"), exe},
                 Sink::Capture, Sink::Capture, _STEP);
        if (r.timed_out) throw py::TimeoutError(S("timed out"));
    } catch (const py::TimeoutError&) {
        return {false, py::percent(S("a compile or link took over %d s"), _STEP), py::list<str>()};
    }
    if (r.returncode != 0) {
        str e = decode_(r.err);
        static const py::re::Pattern undef = py::re::compile(S("undefined reference to `([^']+)'"));
        py::set<str> missing = py::set<str>::from(undef.findall(e));
        if (py::truthy(missing)) {   // the symbols, not ld's long path in front of them
            py::list<str> m = py::list<str>::from(py::sorted(missing));
            str j = py::join(S(", "), m.slice(0, 4));
            return {false, S("undefined: ") + j.slice(0, 200), py::list<str>()};
        }
        str pick = e.slice(0, 200);
        for (const str& l : py::splitlines(e)) {
            if (l.contains(S("undefined")) || l.contains(S("error"))) {
                pick = l;
                break;
            }
        }
        pick = pick.strip();
        return {false, pick.size() > 200 ? pick.slice(pick.size() - 200, pick.size()) : pick, py::list<str>()};
    }
    py::list<str> linked;
    try {
        str text = read_text_(mapfile);
        linked = py::list<str>::from(py::sorted(py::set<str>::from(loaded.findall(text))));
    } catch (const py::OSError&) {
        linked = py::list<str>();
    }
    return {true, str(), linked};
}

// ================================================================================================== run
bool _two_phase(const str& cpp_dir, const str& name) {
    try {
        return read_text_(py::os::path::join(cpp_dir, name + S(".h"))).contains(S(" j2xInit("));
    } catch (const py::OSError&) {
        return false;
    }
}

str harness_name(const str& cls_name) { return S("J2XProbe_") + cls_name; }

py::list<str> libraries() {
    str folder = py::settings::get_str(S("java_libs"));
    py::list<str> out;
    if (folder.empty() || !py::os::path::isdir(folder)) return out;
    for (const str& f : py::sorted(py::os::listdir(folder)))
        if (f.lower().endswith(S(".jar"))) out.append(py::os::path::join(folder, f));
    return out;
}

namespace {
std::mutex& shim_lock_() {
    static std::mutex m;
    return m;
}
}  // namespace

str shims(const str& work, const str& javac) {
    str out = py::os::path::join(work, S("probe"), S("_shims"));
    str done = py::os::path::join(out, S(".compiled"));
    py::list<str> libs = libraries();
    if (!py::truthy(libs)) return str();
    std::lock_guard<std::mutex> hold(shim_lock_());
    if (!py::os::path::isfile(done)) {
        str root = _paths::data_path(py::list<str>{S("probe_shims")});
        py::list<str> files;
        for (const auto& w : py::os::walk(root))
            for (const str& f : std::get<2>(w))
                if (f.endswith(S(".java"))) files.append(py::os::path::join(std::get<0>(w), f));
        if (!py::truthy(files)) return str();
        py::os::makedirs(out, 0777, true);
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("-cp"), py::join(py::os::pathsep, libs), S("-d"), out};
        cmd.extend(files);
        Ran r = run_(cmd, Sink::Devnull, Sink::Devnull, TIMEOUT * 3);
        if (r.timed_out) return str();
        if (r.returncode != 0) return str();
        py::file f = py::open(done, S("w"));
        f.write(str());
    }
    return out;
}

str _cp(const str& work, const py::list<str>& dirs) {
    str shim = py::os::path::join(work, S("probe"), S("_shims"));
    py::list<str> parts = dirs.copy();
    if (py::os::path::isfile(py::os::path::join(shim, S(".compiled")))) parts.append(shim);
    parts.extend(libraries());
    return py::join(py::os::pathsep, parts);
}

Compiled compile_java(const str& java_dir, const Todo& todo, const str& work, const str& javac, int64_t size, const std::optional<int64_t>& jobs_in,
                      const str& tag) {
    Compiled out;
    std::mutex out_lock;
    str root = py::os::path::join(work, S("probe"), S("_java"));
    shims(work, javac);

    auto attempt = [&](const Todo& group, const str& label) -> std::tuple<bool, str> {
        str jdir = py::os::path::join(root, label);
        py::list<str> files;
        for (const auto& cm : group) {
            const ClassRef& cls = std::get<0>(cm);
            str folder = jdir;
            if (cls->package && !cls->package->empty()) {
                for (const str& part : py::split(*cls->package, S("."))) folder = py::os::path::join(folder, part);
            }
            py::os::makedirs(folder, 0777, true);
            str path = py::os::path::join(folder, harness_name(cls->name) + S(".java"));
            textutil::write_text(path, java_harness(cls->name, std::get<1>(cm), cls->package, harness_name(cls->name)));
            files.append(path);
        }
        str listing = py::os::path::join(jdir, S("sources.txt"));   // an @file: the command line stays short
        {
            py::list<str> quoted;
            for (const str& x : files) quoted.append(S("\"") + x.replace(S("\\"), S("/")) + S("\""));
            py::file f = py::open(listing, S("w"), S("utf-8"));
            f.write(py::join(S("\n"), quoted));
        }
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8")};
        if (py::truthy(libraries())) {
            cmd.append(S("-cp"));
            cmd.append(_cp(work));
        }
        cmd.extend(py::list<str>{S("-sourcepath"), java_dir, S("-d"), jdir, S("@") + listing});
        Ran r = run_(cmd, Sink::Capture, Sink::IntoStdout, TIMEOUT * 3 + 10 * group.size());
        if (r.timed_out) return {false, S("javac did not finish")};
        if (r.returncode != 0) {
            str pick = S("failed");
            for (const str& l : py::splitlines(decode_(r.out))) {
                if (l.contains(S("error"))) {
                    pick = l;
                    break;
                }
            }
            return {false, S("javac: ") + pick.slice(0, 180)};
        }
        for (const auto& cm : group) {
            const ClassRef& cls = std::get<0>(cm);
            std::lock_guard<std::mutex> hold(out_lock);
            out.set(cls->name, std::make_tuple(jdir, ((cls->package && !cls->package->empty()) ? *cls->package + S(".") : str()) + harness_name(cls->name)));
        }
        return {true, str()};
    };

    std::function<void(const Todo&, const str&)> settle = [&](const Todo& group, const str& label) {
        auto a = attempt(group, label);
        if (std::get<0>(a)) return;
        if (group.size() == 1) {
            std::lock_guard<std::mutex> hold(out_lock);
            out.set(std::get<0>(group[0])->name, std::make_tuple(str(), std::get<1>(a)));
            return;
        }
        int64_t half = group.size() / 2;
        settle(group.slice(0, half), label + S("a"));
        settle(group.slice(half, group.size()), label + S("b"));
    };

    py::list<std::tuple<int64_t, Todo>> batches;
    for (int64_t i = 0, b = 0; i < todo.size(); i += size, b++) batches.append(std::make_tuple(b, todo.slice(i, std::min(i + size, todo.size()))));
    int64_t jobs = jobs_in ? *jobs_in : base::parallel::workers();
    jobs = std::min<int64_t>(jobs, batches.size() ? batches.size() : 1);
    auto run_batch = [&](const std::tuple<int64_t, Todo>& nb) {
        settle(std::get<1>(nb), tag + py::to_str(std::get<0>(nb)));
        return 0;
    };
    threads_(run_batch, batches, jobs);
    return out;
}

std::optional<py::dict<str, str>> _java_again(const str& java_dir, const ClassRef& cls, const py::list<Probeable>& methods, const str& work,
                                              const str& javac, const str& java, const str& jdir, const py::set<str>& only) {
    str kdir = py::os::path::join(work, S("probe"), cls->name, S("_java_only"));
    str folder = kdir;
    if (cls->package && !cls->package->empty()) {
        for (const str& part : py::split(*cls->package, S("."))) folder = py::os::path::join(folder, part);
    }
    py::os::makedirs(folder, 0777, true);
    str name = harness_name(cls->name) + S("Only");
    str path = py::os::path::join(folder, name + S(".java"));
    textutil::write_text(path, java_harness(cls->name, methods, cls->package, name, only));
    str cp = _cp(work, py::list<str>{kdir, jdir});
    Ran r = run_(py::list<str>{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("-cp"), _cp(work, py::list<str>{jdir}), S("-sourcepath"), java_dir,
                               S("-d"), kdir, path},
                 Sink::Devnull, Sink::Devnull, TIMEOUT * 3);
    if (r.timed_out || r.returncode != 0) return std::nullopt;
    r = run_(py::list<str>{java, S("-cp"), cp, ((cls->package && !cls->package->empty()) ? *cls->package + S(".") : str()) + name}, Sink::Capture,
             Sink::Devnull, TIMEOUT);
    if (r.timed_out) return std::nullopt;
    return _results(decode_(r.out));
}

Todo plan(const str& java_dir, const str& cpp_dir, const py::list<str>& names) {
    Todo out;
    for (const str& name : names) {
        ClassRef cls = lookup(java_dir, name);
        if (!cls) continue;
        py::list<Probeable> methods;
        try {
            methods = probeable(cls, read_text_(py::os::path::join(cpp_dir, name + S(".h"))), java_dir, cpp_dir);
        } catch (const py::OSError&) {
            continue;
        }
        if (py::truthy(methods)) out.append(std::make_tuple(cls, methods));
    }
    return out;
}

RunResult run(const str& java_dir, const str& cpp_dir, const ClassRef& cls, const str& work, const str& cxx, const str& javac, const str& java,
              const std::optional<str>& prefix, const std::optional<Compiled>& compiled_in) {
    str header = py::os::path::join(cpp_dir, cls->name + S(".h"));
    py::list<Probeable> methods;
    try {
        methods = probeable(cls, read_text_(header), java_dir, cpp_dir);
    } catch (const py::OSError&) {
        methods = py::list<Probeable>();
    }
    RunResult res;
    for (const Probeable& m : methods) res.methods.append(m.name);
    res.total = counted(cls);
    if (!py::truthy(methods)) {
        auto clk = reads_clock(cls);
        res.why = (py::truthy(std::get<0>(clk)) || std::get<1>(clk)) ? S("it reads the clock or the clipboard (they differ between the two runs)")
                                                                      : S("no method it can call and compare (its arguments made, its result or object read back)");
        return res;
    }
    str folder = py::os::path::join(work, S("probe"), cls->name);
    py::os::makedirs(folder, 0777, true);
    std::optional<std::tuple<str, str>> got;
    if (compiled_in && compiled_in->contains(cls->name)) got = compiled_in->at(cls->name);
    if (!got) {   // not compiled with the module's batch: alone
        Todo one;
        one.append(std::make_tuple(cls, methods));
        Compiled c = compile_java(java_dir, one, work, javac, 40, std::nullopt, cls->name);
        if (c.contains(cls->name)) got = c.at(cls->name);
    }
    if (!got) throw py::TypeError(S("cannot unpack non-iterable NoneType object"));
    str jdir = std::get<0>(*got), main = std::get<1>(*got);
    if (jdir.empty()) {
        res.why = main;
        return res;
    }
    // the original runs twice: a method any of whose results changes between two runs of Java itself
    // (a Random seeded from the clock, the time, an identity hash) is left out whole
    std::vector<py::dict<str, str>> runs;
    for (int twice = 0; twice < 2; twice++) {
        Ran r = run_(py::list<str>{java, S("-cp"), _cp(work, py::list<str>{jdir}), main}, Sink::Capture, Sink::Devnull, TIMEOUT);
        if (r.timed_out) {
            res.why = S("the Java side did not finish");
            return res;
        }
        runs.push_back(_results(decode_(r.out)));
    }
    py::dict<str, str> jres = runs[0];
    py::set<str> shaky;
    for (const str& k : jres.keys()) {
        auto other = runs[1].get(k);
        if (!other || *other != jres.at(k)) shaky.add(py::split(k, S("."))[0]);
    }
    py::set<str> unsteady;
    for (const str& k : jres.keys())
        if (shaky.contains(py::split(k, S("."))[0])) unsteady.add(k);
    res.unsteady = unsteady.size();
    py::set<str> keep;
    for (const auto& kv : jres.items())
        if (std::get<1>(kv) != S("throw") && !unsteady.contains(std::get<0>(kv))) keep.add(std::get<0>(kv));
    if (!py::truthy(keep)) {
        res.why = py::truthy(unsteady) ? S("Java gives a different result on every run") : S("the Java methods threw on every input");
        return res;
    }
    // The cases run one after another in one program, and a case can leave state behind for the next ones
    // (an Item's constructor registers it in Item.itemsList, even when the rest of that call throws). The C++
    // side runs only the cases kept, so when some were left out the original is run again on exactly
    // those, in the same order, and that is what C++ is compared with; repeated until no kept case throws.
    bool settled = keep.size() == jres.size();
    if (settled) {
        for (const str& k : keep)
            if (!jres.contains(k)) settled = false;
    }
    for (int again = 0; again < 4; again++) {
        if (settled) break;
        auto sub = _java_again(java_dir, cls, methods, work, javac, java, jdir, keep);
        if (!sub) {
            res.why = S("the original could not be run again on the cases kept");
            return res;
        }
        py::set<str> now;
        for (const str& k : keep) {
            auto v = sub->get(k);
            if (v && *v != S("throw")) now.add(k);
        }
        for (const str& k : now) jres.set(k, sub->at(k));
        {
            bool same_set = now.size() == keep.size();
            if (same_set)
                for (const str& k : now)
                    if (!keep.contains(k)) same_set = false;
            settled = same_set;
        }
        keep = now;
        if (!py::truthy(keep)) break;
    }
    if (!py::truthy(keep) || !settled) {
        res.why = S("the cases depend on what the ones left out did (no steady set to compare)");
        return res;
    }
    str lib = library(cxx, cpp_dir, work, prefix);
    if (lib.empty()) {
        res.why = S("the program could not be archived for linking");
        return res;
    }
    py::list<str> order = init_order(cpp_dir);
    str harness = py::os::path::join(folder, S("probe.cpp"));
    str exe = py::os::path::join(folder, S("probe.exe"));
    py::set<str> named = _classes_named(methods, cls->name);
    py::list<str> need;
    for (const str& c : order)
        if (c == cls->name || named.contains(c)) need.append(c);
    py::set<str> made_in_two;
    {
        py::set<str> cand = py::set<str>::from(named);
        cand.add(cls->name);
        for (const str& c : cand)
            if (_two_phase(cpp_dir, c)) made_in_two.add(c);
    }
    auto edges = ancestry(cpp_dir);
    py::set<str> present = py::set<str>::from(named);
    present.add(cls->name);
    bool ok = false;
    str why;
    py::list<str> linked;
    for (int pass = 0; pass < 4; pass++) {
        py::list<std::tuple<str, str>> pairs;
        for (const auto& e : edges)
            if (present.contains(std::get<0>(e)) && present.contains(std::get<1>(e))) pairs.append(e);
        textutil::write_text(harness, std::get<0>(twophase::rewrite_news(cpp_harness(cls->name, methods, keep, need, named, pairs), made_in_two)));
        auto bn = build_native(cxx, cpp_dir, harness, exe, lib, prefix);
        ok = std::get<0>(bn);
        why = std::get<1>(bn);
        linked = std::get<2>(bn);
        // the classes linked in may have static initialisers of their own, and bases to register: all of
        // theirs, in order
        py::set<str> now = py::set<str>::from(linked);
        now.add(cls->name);
        now.update(named);
        py::list<str> wanted;
        for (const str& c : order)
            if (now.contains(c)) wanted.append(c);
        if (!ok || (wanted == need && now == present)) break;
        need = wanted;
        present = now;
    }
    if (!ok) {
        res.why = S("did not link: ") + why;
        return res;
    }
    Ran r = run_(py::list<str>{exe}, Sink::Capture, Sink::Devnull, TIMEOUT);
    if (r.timed_out) {
        res.why = S("the C++ side did not finish");
        return res;
    }
    py::dict<str, str> cres = _results(decode_(r.out));
    py::set<str> judged;
    int64_t mi = 0;
    for (const Probeable& pm : methods) {
        int64_t ci = 0;
        for (const py::list<str>& values : cases(_flat(pm.slots, pm.made))) {
            str key = py::percent(S("%d.%d"), mi, ci);
            ci++;
            if (!keep.contains(key)) continue;
            res.cases += 1;
            judged.add(py::to_str(mi));
            str kind = pm.result.kind == Result::Prim ? pm.result.type : S("obj");
            if (!cres.contains(key)) {
                res.differs.append(std::make_tuple(pm.name, values, jres.at(key), S("no result (crashed?)")));
            } else if (!same(kind, jres.at(key), cres.at(key))) {
                res.differs.append(std::make_tuple(pm.name, values, jres.at(key), cres.at(key)));
            }
        }
        mi++;
    }
    res.judged = judged.size();
    return res;
}

}  // namespace codebreaker::java::probe
