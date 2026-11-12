// The port of the assembly against what the Python gave over real programs. Runs only when CB_ASM16_ORACLE names a folder
// with expected.txt and inputs.txt of the old harness (state/work/typed-port/port.asm16: `label<TAB>json` lines made by
// codebreaker.port.asm16 over the assembly of five old Microsoft tools, and the files they were made from). For each assembly
// module of each tree it ports the module twice as the harness did (plain; then with argument types, far routines, a data
// map, a listing and aliases) and compares what it means: the text emitted, the kinds before every line, each routine's exit
// and writes, the arguments found.
// cb-unity: alone
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <set>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/base/ctext.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"

namespace {

using namespace cb;
using namespace cb::port::asm16;
using J = nlohmann::json;

struct Expected {
    std::string text;
    std::unordered_map<std::string, std::string_view> by_label;
    explicit Expected(const std::filesystem::path& file) : text(fs::read_text(file)) {
        std::size_t pos = 0;
        while (pos < text.size()) {
            std::size_t eol = text.find('\n', pos);
            if (eol == std::string::npos) eol = text.size();
            const std::string_view line = std::string_view(text).substr(pos, eol - pos);
            if (const std::size_t tab = line.find('\t'); tab != std::string_view::npos) by_label.emplace(line.substr(0, tab), line.substr(tab + 1));
            pos = eol + 1;
        }
    }
    std::optional<J> get(const std::string& label) const {
        const auto it = by_label.find(label);
        if (it == by_label.end()) return std::nullopt;
        return J::parse(it->second);
    }
};

J origins_json(const OriginSet& o) {
    std::vector<J> v;
    for (const Origin& x : o.get()) {   // (the set is in the order the Python sorted them: by kind, then the names)
        if (x.source == Origin::Source::In) v.push_back(J::array({"in", x.a, x.b}));
        else if (x.source == Origin::Source::Param) v.push_back(J::array({"param", x.a, x.n}));
        else v.push_back(J::array({"segof", x.a}));
    }
    return J(v);
}

J k_json(const std::optional<K>& k) {
    if (!k) return nullptr;
    static const char* const kinds[] = {"val", "ptr", "opq", "frame", "seg", "ret"};
    J to = nullptr;
    if (k->owner) {
        J stack = J::array();
        for (const auto& [a, b] : k->owner->stack) stack.push_back(J::array({a, b}));
        to = J::array({k->owner->routine ? J(*k->owner->routine) : J(nullptr), stack});
    } else if (k->region == Region::Code) {
        to = "code";
    } else if (k->region == Region::DataSegment) {
        char buf[32];
        std::snprintf(buf, sizeof buf, "@ds:%x", k->ds_address);
        to = buf;
    } else if (k->to) {
        to = *k->to;
    }
    return J::array({kinds[static_cast<int>(k->kind)], to, k->old, k->new_, origins_json(k->origin)});
}

J state_json(const State& s) {
    J regs = J::array();
    if (s.ds) regs.push_back(J::array({"@ds", k_json(s.ds)}));
    if (s.es) regs.push_back(J::array({"@es", k_json(s.es)}));
    std::vector<std::string> names(kReg16.begin(), kReg16.end());
    std::sort(names.begin(), names.end());
    for (const std::string& n : names)
        if (s.reg(n)) regs.push_back(J::array({n, k_json(*s.reg(n))}));
    J stack = J::array();
    for (const StackItem& it : s.stack) stack.push_back(J::array({it.old_size, it.new_size, k_json(it.k)}));
    return J::array({regs, stack, s.sp_old, s.sp_new, J(std::vector<std::string>(s.routines.begin(), s.routines.end()))});
}

std::string basename_of(const std::string& p) { return std::filesystem::path(p).filename().string(); }

std::vector<std::string> split_tabs(const std::string& line) {
    std::vector<std::string> out;
    for (const std::string_view v : text::split(line, '\t')) out.emplace_back(v);
    return out;
}

// Seconds spent in the module's real work (what the old harness timed as CTypes, c_functions, Port(), run and emit).
struct Clock {
    double ctypes = 0, c_functions = 0, construct = 0, run = 0, emit = 0;
    template <class F>
    static auto timed(double& into, F&& f) {
        const auto t0 = std::chrono::steady_clock::now();
        struct Add {
            double& into;
            std::chrono::steady_clock::time_point t0;
            ~Add() { into += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count(); }
        } add{into, t0};
        return f();
    }
};

struct Counter {
    int checked = 0, failed = 0;
    void check(bool ok, const std::string& what) {
        ++checked;
        if (!ok) {
            ++failed;
            if (failed <= 40) cb::testing::record_failure(__FILE__, __LINE__, what);
        }
    }
};

std::string first_diff(const std::string& a, const std::string& b) {
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    const auto cut = [&](const std::string& s) { return s.substr(i > 80 ? i - 80 : 0, 200); };
    return "first difference at byte " + std::to_string(i) + "\n--- got\n" + cut(a) + "\n--- want\n" + cut(b);
}

}  // namespace

CB_TEST("port.asm16.oracle", [] {
    const auto dir_env = fs::get_env("CB_ASM16_ORACLE");
    if (!dir_env) return;   // (the old harness's output is not part of the tree)
    const std::filesystem::path dir = fs::from_text(*dir_env);
    const Expected exp(dir / "expected.txt");
    std::map<std::string, std::vector<std::string>> trees, asms;
    const std::string inputs = fs::read_text(dir / "inputs.txt");
    for (const std::string_view line : text::lines(inputs)) {
        const auto f = split_tabs(std::string(line));
        if (f.size() >= 2 && f[0] == "tree") trees[f[1]] = {f.begin() + 2, f.end()};
        if (f.size() >= 2 && f[0] == "asm") asms[f[1]] = {f.begin() + 2, f.end()};
    }
    Counter c;
    Clock clock;
    const std::vector<std::string> kTrees = {"lib-2.20", "link-2.44", "masm-4.00", "msc-1.04-mc1", "masm-5.10"};
    for (const std::string& t : kTrees) {
        std::vector<std::string> texts;
        for (const std::string& p : trees[t]) texts.push_back(fs::read_text(p));
        const CTypes ct = Clock::timed(clock.ctypes, [&] { return CTypes(texts, texts); });
        // CTypes: variables and layouts of the tree
        if (const auto want = exp.get("[" + t + "] ct.vars")) {
            J got = J::array();
            for (const auto& [name, v] : ct.vars) got.push_back(J::array({name, J::array({v.base, v.declarator})}));
            c.check(got == *want, "[" + t + "] ct.vars");
        }
        if (const auto want = exp.get("[" + t + "] ct.far_ptrs")) c.check(J(std::vector<std::string>(ct.far_ptrs.begin(), ct.far_ptrs.end())) == *want, "[" + t + "] far_ptrs");
        for (const auto& [name, v] : ct.vars) {
            const auto want = exp.get("[" + t + "] ct.var " + name);
            if (!want) continue;
            const auto es = ct.elem_sizes(name);
            const auto pe = ct.pointee(name);
            J got = J::array({J::array({v.base, v.declarator}), pe ? J(*pe) : J(nullptr), ct.is_pointer(name),
                              es ? J::array({es->old_size, es->new_size}) : J(nullptr)});
            c.check(got == *want, "[" + t + "] ct.var " + name);
        }
        for (const auto& [st, lay] : ct.old_layouts) {
            for (int off = 0; off <= std::min<int>(static_cast<int>(lay.size), 40) + 1; ++off) {
                const auto want = exp.get("[" + t + "] ct.field " + st + " " + std::to_string(off));
                if (!want) continue;
                const auto f = ct.field(st, off);
                const auto mt = ct.member_type(st, off);
                J got = J::array({f ? J::array({f->name, f->old_offset, f->old_size, f->new_offset, f->new_size, f->is_pointer, f->type}) : J(nullptr),
                                  mt ? J(*mt) : J(nullptr)});
                c.check(got == *want, "[" + t + "] ct.field " + st + " " + std::to_string(off));
            }
        }
        const OrderedMap<std::string, CSig> cf = Clock::timed(clock.c_functions, [&] { return c_functions(texts); });
        if (const auto want = exp.get("[" + t + "] c_functions")) {
            J got = J::array();
            for (const auto& [name, s] : cf) {
                J args = J::array();
                static const char* const kind[] = {"val", "long", "ptr", "double"};
                for (const CArg& a : s.args) args.push_back(J::array({a.size, kind[static_cast<int>(a.kind)]}));
                static const char* const ret[] = {"val", "long", "ptr", "void"};
                got.push_back(J::array({name, J::array({s.pascal, args, ret[static_cast<int>(s.ret)], s.to ? J(*s.to) : J(nullptr)})}));
            }
            c.check(got == *want, "[" + t + "] c_functions");
        }
        // the calls of the C program
        std::set<std::string> call_set;
        static const Regex call_re(R"(\b([A-Za-z_]\w*)\s*\()");
        for (const std::string& x : texts) {
            const std::string blanked = base::ctext::blank(x);
            call_re.for_each_match(blanked, [&](const RegexMatch& m) {
                call_set.insert(std::string(m.group(1)));
                return true;
            });
        }
        const std::vector<std::string> calls(call_set.begin(), call_set.end());
        std::map<std::string, std::string> asm_texts;
        for (const std::string& p : asms[t]) asm_texts[basename_of(p)] = fs::read_text(p);
        std::map<std::string, std::set<std::string>> routines, externs;
        for (const auto& [n, tx] : asm_texts) {
            PortSettings s;
            s.c_names = calls;
            Port m0(module(n, tx), ct, s);
            for (const std::string& p : m0.public_names())
                if (m0.labels().contains(p)) routines[n].insert(p);
            for (const auto& [e, kind] : m0.externs()) externs[n].insert(e);
        }
        for (const auto& [n, tx] : asm_texts) {
            for (const bool second : {false, true}) {
                const std::string tag = "[" + t + ":" + n + (second ? "+]" : "]");
                std::set<std::string> others;
                if (routines.size() > 1)
                    for (const auto& [k, r] : routines)
                        if (k != n) others.insert(r.begin(), r.end());
                PortSettings s;
                s.c_names = calls;
                s.c_funcs = cf;
                s.asm_routines = others;
                s.underscore = !second;
                Port m = Clock::timed(clock.construct, [&] { return Port(module(n, tx), ct, s); });
                std::set<std::string> longret;
                if (!second) {
                    if (externs.size() > 1)
                        for (const auto& [k, e] : externs)
                            if (k != n) m.used_outside.insert(e.begin(), e.end());
                    for (const auto& [p, cn] : m.cname)
                        if (text::iends_with(p, "mul")) longret.insert(p);
                } else {
                    std::vector<std::string> names;
                    for (const auto& [p, cn] : m.cname) names.push_back(p);
                    for (const std::string& p : names) m.param_types[p] = {{2, "char"}, {4, std::nullopt}, {2, "struct a"}, {4, "char"}, {2, std::nullopt}};
                    for (std::size_t i = 0; i < names.size(); i += 2) m.far.insert(names[i]);
                    m.data_map = {{0x100, "_x"}, {0x200, "_y"}, {0x400, "_z"}};
                    m.aliases["x"] = {"y", 2};
                    for (const auto& [label, idx] : m.labels()) m.offsets.labels.insert_or_assign(label, Place{*m.code()[idx].seg, static_cast<std::uint32_t>(16 * idx)});
                    for (std::size_t i = 0; i < m.code().size(); ++i) {
                        const Line& l = m.code()[i];
                        if (!l.directive || (l.directive->kind != "DB" && l.directive->kind != "DW" && l.directive->kind != "DD")) continue;
                        std::string collapsed;
                        bool sp = false;
                        for (const char ch : strip_comment(l.text)) {
                            if (text::is_space(ch)) sp = true;
                            else {
                                if (sp && !collapsed.empty()) collapsed.push_back(' ');
                                sp = false;
                                collapsed.push_back(ch);
                            }
                        }
                        static const std::uint8_t kBytes[] = {0x8B, 0x1E, 0x34, 0x12};
                        ListedLine ll{*l.seg, static_cast<std::uint32_t>(16 * i + 1), 2, collapsed.substr(0, 24), {}};
                        ll.bytes.assign(kBytes, kBytes + (i % 4) + 1);
                        m.offsets.lines.push_back(std::move(ll));
                    }
                }
                const auto want_run = exp.get(tag + " run");
                if (!want_run) continue;
                bool ran = true;
                std::string error;
                try {
                    Clock::timed(clock.run, [&] { m.run(); return 0; });
                } catch (const Asm16Error& e) {
                    ran = false;
                    error = e.what();
                }
                const bool want_ok = want_run->is_string();
                c.check(ran == want_ok, tag + " run: " + (ran ? "ran, Python failed" : "failed (" + error + "), Python ran"));
                if (!ran || !want_ok) continue;
                if (const auto w = exp.get(tag + " called")) c.check(J(std::vector<std::string>(m.called().begin(), m.called().end())) == *w, tag + " called");
                if (const auto w = exp.get(tag + " own_ptr")) {
                    J g = J::array();
                    for (const auto& [sy, d] : m.own_pointers()) g.push_back(J::array({sy, d}));
                    c.check(g == *w, tag + " own_ptr");
                }
                if (const auto w = exp.get(tag + " params")) {
                    J g = J::array();
                    for (const auto& [r, ps] : m.params()) {   // (None first, then by name: the harness's order)
                        J lst = J::array();
                        for (const Param& p : ps) lst.push_back(J::array({p.old_offset, p.size}));
                        g.push_back(J::array({r ? *r : std::string(), lst}));
                    }
                    c.check(g == *w, tag + " params");
                }
                if (const auto w = exp.get(tag + " notes-after")) c.check(J(m.notes()) == *w, tag + " notes");
                std::size_t before_bad = 0;
                for (std::size_t i = 0; i < m.before().size(); ++i) {
                    if (!m.before()[i]) continue;
                    const auto w = exp.get(tag + " before " + std::to_string(i));
                    if (!w) continue;
                    if (state_json(*m.before()[i]) != *w) {
                        if (before_bad++ < 3) c.check(false, tag + " before " + std::to_string(i) + "\n got  " + state_json(*m.before()[i]).dump() + "\n want " + w->dump());
                        else ++c.failed;
                    } else {
                        ++c.checked;
                    }
                }
                for (const auto& [p, cn] : m.cname) {
                    const auto w = exp.get(tag + " exit " + p);
                    if (!w) continue;
                    const auto ex = m.exit_of(p);
                    c.check((ex ? state_json(*ex) : J(nullptr)) == *w, tag + " exit " + p);
                }
                for (const auto& [label, idx] : m.labels()) {
                    const auto w = exp.get(tag + " writes " + label);
                    if (!w) continue;
                    const auto wr = m.writes(label);
                    c.check(J(std::vector<std::string>(wr.begin(), wr.end())) == *w, tag + " writes " + label);
                }
                for (std::size_t i = 0; i < m.code().size(); ++i) {
                    const auto w = exp.get(tag + " routine " + std::to_string(i));
                    if (!w) continue;
                    const Rt r = m.routine(i);
                    c.check((r ? J(*r) : J(nullptr)) == *w, tag + " routine " + std::to_string(i));
                }
                const auto want_emit = exp.get(tag + " emit");
                if (!want_emit) continue;
                std::string text_out;
                bool emitted = true;
                try {
                    text_out = Clock::timed(clock.emit, [&] { return m.emit(longret); });
                } catch (const Asm16Error& e) {
                    emitted = false;
                    error = e.what();
                }
                if (want_emit->is_string()) {
                    c.check(emitted && text_out == want_emit->get<std::string>(), tag + " emit: " + (emitted ? first_diff(text_out, want_emit->get<std::string>()) : error));
                } else {
                    c.check(!emitted, tag + " emit: Python failed (" + want_emit->dump().substr(0, 160) + "), ours emitted");
                    if (!emitted) {   // (the same line, the same complaint)
                        const std::string py_msg = (*want_emit)[2].get<std::string>();
                        c.check(py_msg == error, tag + " emit message: got " + error + " want " + py_msg);
                    }
                }
                if (const auto w = exp.get(tag + " unreached")) c.check(J(m.unreached()) == *w, tag + " unreached");
                if (const auto w = exp.get(tag + " asm_calls")) c.check(J(std::vector<std::string>(m.asm_calls().begin(), m.asm_calls().end())) == *w, tag + " asm_calls");
            }
        }
    }
    std::fprintf(stderr, "asm16 oracle timings: CTypes %.2fs, c_functions %.2fs, Port() %.2fs, run %.2fs, emit %.2fs\n", clock.ctypes, clock.c_functions, clock.construct, clock.run, clock.emit);
    cb::testing::count_check();
    if (c.failed) cb::testing::record_failure(__FILE__, __LINE__, std::to_string(c.failed) + " of " + std::to_string(c.checked) + " oracle comparisons differ");
});
