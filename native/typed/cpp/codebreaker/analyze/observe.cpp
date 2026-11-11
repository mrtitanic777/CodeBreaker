// codebreaker/analyze/observe.cpp -- typed C++ port of codebreaker.analyze.observe (see observe.hpp).

#include "codebreaker/analyze/observe.hpp"

#include <algorithm>
#include <cstdio>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <vector>

#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/symbols/api_arguments.hpp"

namespace codebreaker::analyze::observe {

namespace emu = codebreaker::emulate::emulator;
namespace native_trace = codebreaker::emulate::native_trace;
using py::str;

namespace {

str hx(int64_t v) {                                   // "%x"
    char buf[32];
    std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(v));
    return str(buf);
}
str num(int64_t v) { return str(std::to_string(v)); }

}  // namespace

py::bytes _fill(int64_t k) {
    std::string out;
    out.reserve(static_cast<size_t>(BUFFER_SIZE));
    for (int64_t o = 0; o < BUFFER_SIZE; o += 4) {
        const uint32_t v = static_cast<uint32_t>(MARK + (k << 24) + (o << 12));
        out.push_back(static_cast<char>(v & 0xFF));
        out.push_back(static_cast<char>((v >> 8) & 0xFF));
        out.push_back(static_cast<char>((v >> 16) & 0xFF));
        out.push_back(static_cast<char>((v >> 24) & 0xFF));
    }
    return py::bytes(out);
}

std::optional<int64_t> _argc(const str& dll, const str& name) { return symbols::api_arguments::argc(dll, name); }

emu::Trace _trace(const str& path, int64_t va, const py::list<int64_t>& args, const py::list<std::tuple<int64_t, py::bytes>>& mem) {
    emu::TraceOptions o;
    o.args = args;
    o.mem_init = mem;
    o.stub_externals = true;
    o.sig_resolver = _argc;
    o.insn_cap = INSNS;
    // (the emulator keeps its machines and its hooks' state in the process: the Python's workers were processes, the pool's here are
    // threads, so one run at a time)
    static std::mutex one_run;
    std::lock_guard<std::mutex> guard(one_run);
    try {
        return emu::trace_function(path, emu::FuncSelector(va), o);
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {                  // the emulator could not run it at all
        emu::Trace t;
        t.exit = str("error: ") + str(std::string(e.type()));
        return t;
    }
}

std::optional<Where> _where(int64_t addr) {
    if (BUFFERS <= addr && addr < BUFFERS + MAX_ARGS * 0x10000 && (addr & 0xFFFF) < BUFFER_SIZE) return Where{str("arg"), (addr - BUFFERS) >> 16, addr & 0xFFFF, 0};
    if (MARK <= addr && addr < MARK + (MAX_ARGS << 24)) return Where{str("field"), (addr - MARK) >> 24, (addr >> 12) & 0xFFF, addr & 0xFFF};
    return std::nullopt;
}

void _digest(const emu::Trace& rec, Observation& out) {
    auto touch = [&out](int64_t addr, int64_t size, int col) {
        const std::optional<Where> w = _where(addr);
        if (!w) return;
        Cell* cell;
        if (w->kind == str("arg")) cell = &out.fields[w->k][w->o];
        else cell = &out.nested[std::make_tuple(w->k, w->o)][w->offset];
        (*cell)[0] = std::max((*cell)[0], size);
        (*cell)[static_cast<size_t>(col)] += 1;
    };
    for (const auto& m : rec.mem_reads) touch(static_cast<int64_t>(m.addr), static_cast<int64_t>(m.size), 1);
    if (std::holds_alternative<py::list<native_trace::MemWrite>>(rec.mem_writes))
        for (const auto& m : std::get<py::list<native_trace::MemWrite>>(rec.mem_writes)) touch(static_cast<int64_t>(m.addr), static_cast<int64_t>(m.size), 2);
    else
        for (const auto& m : std::get<py::list<native_trace::LeanWrite>>(rec.mem_writes)) touch(static_cast<int64_t>(std::get<0>(m)), static_cast<int64_t>(std::get<1>(m)), 2);
    for (const auto& c : rec.calls) {
        const std::optional<int64_t> t = c.resolved_target;
        if (c.indirect && t && *t != 0 && !(c.external && *c.external)) {
            bool seen = false;
            for (const auto& x : out.indirect)
                if (std::get<0>(x) == c.pc && std::get<1>(x) == *t) seen = true;
            if (!seen) out.indirect.append({c.pc, *t});
        }
    }
    if (rec.api_calls) {
        for (const auto& call : *rec.api_calls) {
            const str& api = std::get<0>(call);
            const auto& args = std::get<1>(call);
            if (!args) continue;
            for (int64_t j = 0; j < args->size(); j++) {
                const std::optional<Where> w = _where((*args)[j]);
                std::tuple<str, int64_t, int64_t, int64_t, str> row;
                if (w && w->kind == str("arg")) row = {api, j, w->k, w->o, str("address")};
                else if (w && w->offset == 0) row = {api, j, w->k, w->o, str("value")};
                else continue;
                bool dup = false;
                for (const auto& p : out.passes)
                    if (p == row) dup = true;
                if (!dup) out.passes.append(row);
            }
        }
    }
    if (rec.exit && *rec.exit == str("ret")) {
        out.exit = str("ret");
        if (rec.esp_delta && *rec.esp_delta < 0x100) out.removed = *rec.esp_delta - 4;
        const std::optional<Where> w = _where(rec.ret ? *rec.ret : 0);
        if (w && w->kind == str("arg") && w->o == 0) out.returns = Returns{str("argument"), w->k, std::nullopt};
        else if (w && w->kind == str("field") && w->offset == 0) out.returns = Returns{str("field"), w->k, w->o};
    } else if (!(out.exit && *out.exit == str("ret"))) {
        out.exit = rec.truncated ? str("cut") : str("fault");
    }
}

// ---- behaving like a library function --------------------------------------------------------
py::bytes _after(const emu::Trace& rec, int64_t base, const py::bytes& initial) {
    std::string buf = initial.raw();
    auto apply = [&](int64_t addr, int64_t size, uint64_t value) {
        const int64_t off = addr - base;
        if (0 <= off && off + size <= static_cast<int64_t>(buf.size())) {
            const uint64_t v = size >= 8 ? value : (value & ((uint64_t(1) << (8 * size)) - 1));
            for (int64_t i = 0; i < size; i++) buf[static_cast<size_t>(off + i)] = static_cast<char>((i < 8 ? v >> (8 * i) : 0) & 0xFF);
        }
    };
    if (std::holds_alternative<py::list<native_trace::MemWrite>>(rec.mem_writes))
        for (const auto& m : std::get<py::list<native_trace::MemWrite>>(rec.mem_writes)) apply(static_cast<int64_t>(m.addr), static_cast<int64_t>(m.size), m.value);
    else
        for (const auto& m : std::get<py::list<native_trace::LeanWrite>>(rec.mem_writes))
            apply(static_cast<int64_t>(std::get<0>(m)), static_cast<int64_t>(std::get<1>(m)), std::get<2>(m));
    return py::bytes(buf);
}

int64_t _s32(int64_t v) {
    v &= 0xFFFFFFFFLL;
    return (v & 0x80000000LL) ? v - (int64_t(1) << 32) : v;
}

int64_t _sign(int64_t x) { return (x > 0) - (x < 0); }

std::tuple<std::optional<int64_t>, py::bytes, py::bytes> _case(const str& path, int64_t va, const py::list<int64_t>& args, const py::bytes& a_in,
                                                               const py::bytes& b_in) {
    std::string a = a_in.raw(), b = b_in.raw();
    a += std::string(static_cast<size_t>(std::max<int64_t>(0, 64 - static_cast<int64_t>(a.size()))), '\xEE');
    b += std::string(static_cast<size_t>(std::max<int64_t>(0, 64 - static_cast<int64_t>(b.size()))), '\xDD');
    py::list<std::tuple<int64_t, py::bytes>> mem;
    mem.append({TEXT_A, py::bytes(a)});
    mem.append({TEXT_B, py::bytes(b)});
    const emu::Trace rec = _trace(path, va, args, mem);
    if (!(rec.exit && *rec.exit == str("ret"))) return {std::nullopt, py::bytes(a), py::bytes(b)};
    return {rec.ret ? *rec.ret : 0, _after(rec, TEXT_A, py::bytes(a)), _after(rec, TEXT_B, py::bytes(b))};
}

namespace {

struct RunResult {
    std::optional<int64_t> ret;
    std::string A, B;
};

}  // namespace

std::optional<str> _idiom(const str& path, int64_t va, const std::optional<int64_t>& nargs) {
    const int64_t A = TEXT_A, B = TEXT_B;
    auto run = [&](std::initializer_list<int64_t> args, const std::string& a = "", const std::string& b = "") {
        py::list<int64_t> l;
        for (int64_t x : args) l.append(x);
        const auto [r, ra, rb] = _case(path, va, l, py::bytes(a), py::bytes(b));
        return RunResult{r, ra.raw(), rb.raw()};
    };
    auto unchanged = [](const RunResult& r, const std::string& a, const std::string& b) {
        return r.A.substr(0, a.size()) == a && r.B.substr(0, b.size()) == b;
    };
    auto S = [](const char* p, size_t n) { return std::string(p, n); };
    auto has = [&nargs](int64_t n) { return !nargs || *nargs == n; };

    std::vector<std::pair<const char*, std::function<bool()>>> tests;
    if (has(1)) {
        tests.push_back({"strlen", [&] {
                             for (const auto& [s, n] : std::vector<std::pair<std::string, int64_t>>{{S("hello\0", 6), 5}, {S("\0", 1), 0}, {S("abcdefghijk\0", 12), 11}}) {
                                 const RunResult r = run({A}, s);
                                 if (!(r.ret && *r.ret == n && unchanged(r, s, ""))) return false;
                             }
                             return true;
                         }});
        tests.push_back({"wcslen", [&] {
                             for (const auto& [t, n] : std::vector<std::pair<std::string, int64_t>>{{"hey", 3}, {"", 0}, {"abcdefg", 7}}) {
                                 std::string s;
                                 for (char c : t) {
                                     s.push_back(c);
                                     s.push_back('\0');
                                 }
                                 s += S("\0\0", 2);
                                 const RunResult r = run({A}, s);
                                 if (!(r.ret && *r.ret == n)) return false;
                             }
                             return true;
                         }});
    }
    if (has(2)) {
        tests.push_back({"strcpy", [&] {
                             for (const std::string& s : {S("hello\0", 6), S("\0", 1), S("a longer string\0", 16)}) {
                                 const RunResult r = run({B, A}, s);
                                 if (!(r.ret && r.B.substr(0, s.size() + 1) == s + "\xDD")) return false;
                             }
                             return true;
                         }});
        tests.push_back({"strcat", [&] {
                             for (const auto& [s, d] : std::vector<std::pair<std::string, std::string>>{{S("cd\0", 3), S("ab\0", 3)}, {S("xyz\0", 4), S("\0", 1)}, {S("\0", 1), S("q\0", 2)}}) {
                                 const RunResult r = run({B, A}, s, d);
                                 if (!(r.ret && r.B.substr(0, d.size() + s.size()) == d.substr(0, d.size() - 1) + s + "\xDD")) return false;
                             }
                             return true;
                         }});
        using Cmp = std::vector<std::tuple<std::string, std::string, int64_t>>;
        auto compares = [&](const Cmp& cases) {
            for (const auto& [a, b, want] : cases) {
                const RunResult r = run({A, B}, a, b);
                if (!(r.ret && _sign(_s32(*r.ret)) == want && unchanged(r, a, b))) return false;
            }
            return true;
        };
        const Cmp ordered = {{S("abc\0", 4), S("abc\0", 4), 0}, {S("abc\0", 4), S("abd\0", 4), -1}, {S("abd\0", 4), S("abc\0", 4), 1},
                             {S("ab\0", 3), S("abc\0", 4), -1}, {S("\0", 1), S("\0", 1), 0}};
        tests.push_back({"strcmp", [&, compares, ordered] {
                             Cmp c = ordered;
                             c.push_back({S("ABC\0", 4), S("abc\0", 4), -1});
                             return compares(c);
                         }});
        tests.push_back({"stricmp", [&, compares, ordered] {
                             Cmp c = ordered;
                             c.push_back({S("ABC\0", 4), S("abc\0", 4), 0});
                             c.push_back({S("aBd\0", 4), S("Abc\0", 4), 1});
                             return compares(c);
                         }});
        tests.push_back({"strchr", [&] {
                             for (const auto& [c, want] : std::vector<std::pair<int64_t, int64_t>>{{'l', A + 2}, {'h', A}, {'z', 0}}) {
                                 const RunResult r = run({A, c}, S("hello\0", 6));
                                 if (!(r.ret && *r.ret == want)) return false;
                             }
                             return true;
                         }});
        tests.push_back({"strrchr", [&] {
                             for (const auto& [c, want] : std::vector<std::pair<int64_t, int64_t>>{{'l', A + 3}, {'h', A}, {'z', 0}}) {
                                 const RunResult r = run({A, c}, S("hello\0", 6));
                                 if (!(r.ret && *r.ret == want)) return false;
                             }
                             return true;
                         }});
    }
    if (has(3)) {
        std::string src;
        for (int i = 1; i < 33; i++) src.push_back(static_cast<char>(i));
        tests.push_back({"strncpy", [&] {
                             for (const auto& [s, n] : std::vector<std::pair<std::string, int64_t>>{{S("hello\0", 6), 3}, {S("hi\0", 3), 6}, {S("abc\0", 4), 4}}) {
                                 const RunResult r = run({B, A, n}, s);
                                 const std::string padded = (s.substr(0, static_cast<size_t>(n)) + std::string(static_cast<size_t>(n), '\0')).substr(0, static_cast<size_t>(n));
                                 if (!(r.ret && r.B.substr(0, static_cast<size_t>(n) + 1) == padded + "\xDD")) return false;
                             }
                             return true;
                         }});
        tests.push_back({"memcpy", [&, src] {
                             for (const auto& [s, n] : std::vector<std::pair<std::string, int64_t>>{{src, 7}, {src, 1}, {src, 16}, {src, 0}, {S("ab\0cdef", 7), 6}}) {
                                 const RunResult r = run({B, A, n}, s);
                                 if (!(r.ret && r.B.substr(0, static_cast<size_t>(n) + 1) == s.substr(0, static_cast<size_t>(n)) + "\xDD")) return false;
                             }
                             return true;
                         }});
        tests.push_back({"memset", [&] {
                             for (const auto& [c, n] : std::vector<std::pair<int64_t, int64_t>>{{0x5A, 9}, {0, 4}, {0x11, 1}, {0x7F, 0}}) {
                                 const RunResult r = run({B, c, n});
                                 if (!(r.ret && r.B.substr(0, static_cast<size_t>(n) + 1) == std::string(static_cast<size_t>(n), static_cast<char>(c)) + "\xDD")) return false;
                             }
                             return true;
                         }});
        using Cnt = std::vector<std::tuple<std::string, std::string, int64_t, int64_t>>;
        auto counted = [&](const Cnt& cases) {
            for (const auto& [a, b, n, want] : cases) {
                const RunResult r = run({A, B, n}, a, b);
                if (!(r.ret && _sign(_s32(*r.ret)) == want)) return false;
            }
            return true;
        };
        const Cnt sized = {{"abcd", "abcd", 4, 0}, {"abcd", "abce", 4, -1}, {"abce", "abcd", 4, 1}, {"abcd", "abce", 3, 0}};
        tests.push_back({"memcmp", [&, counted, sized] {
                             Cnt c = sized;
                             c.push_back({S("\0b", 2), S("\0c", 2), 2, -1});
                             c.push_back({"ABCD", "abcd", 4, -1});
                             return counted(c);
                         }});
        tests.push_back({"memicmp", [&, counted, sized] {
                             Cnt c = sized;
                             c.push_back({S("\0b", 2), S("\0c", 2), 2, -1});
                             c.push_back({"ABCD", "abcd", 4, 0});
                             return counted(c);
                         }});
        tests.push_back({"strncmp", [&, counted, sized] {
                             Cnt c = sized;
                             c.push_back({S("\0b", 2), S("\0c", 2), 2, 0});
                             c.push_back({"ABCD", "abcd", 4, -1});
                             return counted(c);
                         }});
    }
    // (the order of the Python's list: strlen wcslen | strcpy strcat strcmp stricmp strchr strrchr | strncpy memcpy memset memcmp memicmp strncmp)
    for (const auto& t : tests) {
        try {
            if (t.second()) return str(t.first);
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception&) {
        }
    }
    return std::nullopt;
}

Observation _observe(const str& path, const Item& item) {
    const int64_t va = std::get<0>(item);
    const std::optional<int64_t>& nargs = std::get<1>(item);
    const py::list<int64_t>& pointers = std::get<2>(item);
    const bool battery = std::get<3>(item);
    const int64_t n = std::min<int64_t>(nargs ? *nargs : 4, MAX_ARGS);
    Observation out;
    py::list<std::tuple<int64_t, py::bytes>> mem;
    for (int64_t k = 0; k < n; k++) mem.append({BUFFERS + k * 0x10000, _fill(k)});
    py::list<int64_t> all;
    for (int64_t k = 0; k < n; k++) all.append(BUFFERS + k * 0x10000);
    _digest(_trace(path, va, all, mem), out);
    if (n && 0 < pointers.size() && pointers.size() < n) {
        py::list<int64_t> some;
        for (int64_t k = 0; k < n; k++) {
            bool p = false;
            for (int64_t x : pointers)
                if (x == k) p = true;
            some.append(p ? BUFFERS + k * 0x10000 : 0);
        }
        _digest(_trace(path, va, some, mem), out);
    }
    if (!n || !(out.exit && *out.exit == str("ret"))) {
        py::list<int64_t> zeros;
        for (int64_t k = 0; k < n; k++) zeros.append(0);
        _digest(_trace(path, va, zeros, py::list<std::tuple<int64_t, py::bytes>>()), out);
    }
    if (battery) out.idiom = _idiom(path, va, nargs);
    return out;
}

Seen _run(image::Image& img, const std::optional<int64_t>& limit, int64_t jobs) {
    if (!img.flat()) return Seen();
    const auto facts = conventions::functions(img);
    py::dict<int64_t, py::set<int64_t>> pointer_args;
    for (const auto& s : *structures::recover(img))
        for (const auto& r : s.roots)
            if (r.kind == str("arg")) pointer_args[r.addr].add(r.index);
    py::list<Item> items_l;
    std::vector<Item> items;
    for (const int64_t f : img.functions) {
        const conventions::Fact& fact = facts->at(f);
        if (fact.shape == str("import thunk") || fact.shape == str("jump thunk") || fact.shape == str("empty") || fact.shape == str("not code")) continue;
        bool reg = false;
        for (const str& r : fact.reg_args)
            if (r != str("ecx")) reg = true;
        if (reg) continue;
        // worth the battery: small, calls nothing, loops (or is a few instructions in front of the function after it, as
        // hand-written entry points are), and takes one to three arguments
        bool battery = fact.shape == str("leaf") && fact.reg_args.empty();
        if (battery) {
            const bool loops = (fact.loops && *fact.loops != 0) || (fact.string_ops && !fact.string_ops->empty());
            battery = loops || img.function(f)->insns.size() <= 3;
        }
        if (battery) battery = !fact.args || (1 <= *fact.args && *fact.args <= 3);
        py::list<int64_t> ptrs;
        if (pointer_args.contains(f)) {
            std::vector<int64_t> v;
            for (int64_t x : pointer_args.at(f)) v.push_back(x);
            std::sort(v.begin(), v.end());
            for (int64_t x : v) ptrs.append(x);
        }
        items.push_back({f, fact.args, ptrs, battery});
    }
    std::stable_sort(items.begin(), items.end(), [](const Item& a, const Item& b) {      // the informative ones first
        const auto ka = std::make_tuple(!std::get<3>(a), std::get<2>(a).empty(), std::get<0>(a));
        const auto kb = std::make_tuple(!std::get<3>(b), std::get<2>(b).empty(), std::get<0>(b));
        return ka < kb;
    });
    if (limit && static_cast<int64_t>(items.size()) > std::max<int64_t>(*limit, 0)) items.resize(static_cast<size_t>(std::max<int64_t>(*limit, 0)));
    for (const Item& it : items) items_l.append(it);
    jobs::pool::Options<Item, std::optional<Observation>> o;
    o.jobs = jobs;
    o.small = 12;
    return jobs::pool::run(
        [](const str& ctx, const Item& item) -> std::optional<Observation> { return _observe(ctx, item); }, items_l, img.path,
        [](const Item& item) { return std::get<0>(item); }, o);
}

std::shared_ptr<Seen> run(image::Image& img, const std::optional<int64_t>& limit, int64_t jobs) {
    const str key = str("observe:") + (limit ? num(*limit) : str("None"));
    return img.memo<Seen>(key, [limit, jobs](image::Image& i) { return std::make_shared<Seen>(_run(i, limit, jobs)); });
}

// ---- the analysis
namespace {

// {"+%x": cell} of a dict of cells, by offset
py::dict<str, Cell> cells_text(const Cells& c) {
    std::vector<std::pair<int64_t, Cell>> v;
    for (const auto& kv : c.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    py::dict<str, Cell> out;
    for (const auto& x : v) out.set(str("+") + hx(x.first), x.second);
    return out;
}

}  // namespace

Analysis analyse(image::Image& img, const std::optional<int64_t>& limit, int64_t jobs) {
    std::map<int64_t, Observation> seen;
    for (const auto& kv : run(img, limit, jobs)->items())
        if (std::get<1>(kv)) seen[std::get<0>(kv)] = *std::get<1>(kv);
    const auto facts_c = conventions::functions(img);
    std::set<int64_t> funcs;
    for (const int64_t f : img.functions) funcs.insert(f);
    py::list<Fact> facts;
    std::set<int64_t> new_targets;
    int64_t edges = 0, agree = 0, differ = 0, confirmed = 0, added = 0;
    // (argument's function, argument number) -> the structure; insertion order as the Python's dict
    std::vector<std::pair<std::tuple<int64_t, int64_t>, std::shared_ptr<const structures::Structure>>> static_fields;
    const auto recovered = structures::recover(img);
    std::vector<std::shared_ptr<const structures::Structure>> keep;
    for (const auto& s : *recovered) keep.push_back(std::make_shared<structures::Structure>(s));
    for (const auto& s : keep) {
        for (const auto& r : s->roots) {
            if (r.kind != str("arg")) continue;
            const auto key = std::make_tuple(r.addr, r.index);
            bool found = false;
            for (auto& e : static_fields)
                if (e.first == key) {
                    e.second = s;
                    found = true;
                }
            if (!found) static_fields.emplace_back(key, s);
        }
    }
    auto static_of = [&static_fields](int64_t f, int64_t k) -> std::shared_ptr<const structures::Structure> {
        for (const auto& e : static_fields)
            if (e.first == std::make_tuple(f, k)) return e.second;
        return nullptr;
    };
    for (const auto& [f, o] : seen) {
        ObservedFact fact;
        fact.addr = f;
        fact.exit = o.exit;
        if (!o.fields.empty()) {
            std::vector<std::pair<int64_t, Cells>> v;
            for (const auto& kv : o.fields.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
            std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            py::dict<str, py::dict<str, Cell>> d;
            for (const auto& x : v) d.set(str("argument ") + num(x.first), cells_text(x.second));
            fact.fields = d;
        }
        if (!o.nested.empty()) {
            std::vector<std::pair<std::tuple<int64_t, int64_t>, Cells>> v;
            for (const auto& kv : o.nested.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
            std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            py::dict<str, py::dict<str, Cell>> d;
            for (const auto& x : v) d.set(str("argument ") + num(std::get<0>(x.first)) + str(" +") + hx(std::get<1>(x.first)), cells_text(x.second));
            fact.nested = d;
        }
        if (!o.passes.empty()) {
            py::list<str> l;
            for (const auto& [a, j, k, off, how] : o.passes)
                l.append(a + str(" parameter ") + num(j) + str(" gets ") + (how == str("address") ? str("the address of") : str("the value at")) +
                         str(" argument ") + num(k) + str(" +") + hx(off));
            fact.passes = l;
        }
        py::list<str> ind;
        for (const auto& [pc, t] : o.indirect) {
            const std::optional<int64_t> owner = img.owner(pc);
            if (!owner || !img.is_code(t)) continue;
            ind.append(img.label(*owner) + str(" at ") + hx(pc) + str(" -> ") + img.label(t));
            edges += 1;
            if (!funcs.count(t)) new_targets.insert(t);
        }
        if (!ind.empty()) fact.indirect_calls = ind;
        if (o.removed) {
            fact.removed = o.removed;
            const std::optional<int64_t> want = facts_c->at(f).removes;
            if (want) {
                agree += *want == *o.removed;
                differ += *want != *o.removed;
            }
        }
        if (o.returns) fact.returns = o.returns;
        if (o.idiom) fact.behaves_like = o.idiom;
        for (const auto& kv : o.fields.items()) {
            const auto s = static_of(f, std::get<0>(kv));
            if (s) {
                for (const auto& off : std::get<1>(kv).items()) {
                    if (s->fields.contains(std::get<0>(off))) confirmed += 1;
                    else added += 1;
                }
            }
        }
        facts.append(Fact(fact));
    }
    // what the runs say about each recovered structure
    std::vector<std::pair<str, std::pair<std::shared_ptr<const structures::Structure>, std::set<int64_t>>>> by_structure;
    for (const auto& e : static_fields) {
        const int64_t f = std::get<0>(e.first), k = std::get<1>(e.first);
        auto it = seen.find(f);
        if (it == seen.end()) continue;
        if (!it->second.fields.contains(k)) continue;
        const Cells& offs = it->second.fields.at(k);
        if (offs.empty()) continue;
        auto slot = std::find_if(by_structure.begin(), by_structure.end(), [&](const auto& x) { return x.first == e.second->name; });
        if (slot == by_structure.end()) {
            by_structure.push_back({e.second->name, {e.second, {}}});
            slot = by_structure.end() - 1;
        }
        for (const auto& kv : offs.items()) slot->second.second.insert(std::get<0>(kv));
    }
    std::sort(by_structure.begin(), by_structure.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    for (const auto& [name, se] : by_structure) {
        const auto& s = se.first;
        const auto& offs = se.second;
        std::vector<str> conf, add;
        for (const int64_t o : offs) (s->fields.contains(o) ? conf : add).push_back(str("+") + hx(o));
        auto by_text = [](const str& a, const str& b) { return a.u32() < b.u32(); };
        std::sort(conf.begin(), conf.end(), by_text);
        std::sort(add.begin(), add.end(), by_text);
        StructureObservedFact sf;
        sf.name = name;
        for (const str& x : conf) sf.fields_confirmed.append(x);
        for (const str& x : add) sf.fields_added.append(x);
        for (const auto& kv : s->fields.items())
            if (!offs.count(std::get<0>(kv))) sf.fields_not_reached += 1;
        facts.append(Fact(sf));
    }
    std::vector<std::pair<str, int64_t>> idioms;
    for (const auto& [f, o] : seen)
        if (o.idiom) idioms.emplace_back(*o.idiom, f);
    std::sort(idioms.begin(), idioms.end(), [](const auto& a, const auto& b) {
        if (a.first.u32() != b.first.u32()) return a.first.u32() < b.first.u32();
        return a.second < b.second;
    });
    int64_t returned = 0, faulted = 0, cut = 0, with_ptr = 0, fields_observed = 0, nested_followed = 0, passed = 0;
    for (const auto& [f, o] : seen) {
        (void)f;
        if (o.exit && *o.exit == str("ret")) returned++;
        if (o.exit && *o.exit == str("fault")) faulted++;
        if (o.exit && *o.exit == str("cut")) cut++;
        if (!o.fields.empty()) with_ptr++;
        for (const auto& kv : o.fields.items()) fields_observed += std::get<1>(kv).size();
        nested_followed += o.nested.size();
        passed += o.passes.size();
    }
    int64_t structures_confirmed = 0;
    for (const auto& [name, se] : by_structure) {
        (void)name;
        for (const int64_t o : se.second)
            if (se.first->fields.contains(o)) {
                structures_confirmed++;
                break;
            }
    }
    Analysis a;
    a.counts.set(str("functions_run"), static_cast<int64_t>(seen.size()));
    a.counts.set(str("returned"), returned);
    a.counts.set(str("faulted"), faulted);
    a.counts.set(str("cut_off"), cut);
    a.counts.set(str("with_pointer_arguments"), with_ptr);
    a.counts.set(str("fields_observed"), fields_observed);
    a.counts.set(str("nested_pointers_followed"), nested_followed);
    a.counts.set(str("static_fields_confirmed"), confirmed);
    a.counts.set(str("fields_only_seen_running"), added);
    a.counts.set(str("structures_with_a_field_confirmed"), structures_confirmed);
    a.counts.set(str("indirect_calls_resolved"), edges);
    a.counts.set(str("function_starts_found"), static_cast<int64_t>(new_targets.size()));
    a.counts.set(str("fields_passed_to_system_calls"), passed);
    a.counts.set(str("removed_bytes_agree_with_static"), agree);
    a.counts.set(str("removed_bytes_differ"), differ);
    a.counts.set(str("library_idioms"), static_cast<int64_t>(idioms.size()));
    for (size_t i = 0; i < idioms.size() && i < 16; i++) a.text.append(str("behaves like ") + idioms[i].first + str(": ") + img.label(idioms[i].second));
    int64_t taken = 0;
    for (const Fact& fa : facts) {
        if (!std::holds_alternative<ObservedFact>(fa) || !std::get<ObservedFact>(fa).indirect_calls) continue;
        for (const str& x : *std::get<ObservedFact>(fa).indirect_calls) {
            if (taken >= 8) break;
            a.text.append(x);
            taken++;
        }
    }
    if (!new_targets.empty()) {
        std::vector<str> hs;
        for (const int64_t t : new_targets) {
            if (hs.size() >= 8) break;
            hs.push_back(hx(t));
        }
        py::list<str> l;
        for (const str& h : hs) l.append(h);
        a.text.append(str("function starts reached only by running: ") + py::join(str(", "), l));
    }
    a.facts = facts;
    return a;
}

}  // namespace codebreaker::analyze::observe
