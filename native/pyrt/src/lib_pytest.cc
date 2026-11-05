// lib_pytest.cc -- pytest for the translated test files: the API they use (raises, mark.skip / skipif /
// parametrize / xfail / usefixtures, fixture, skip, fail, importorskip, approx, param) and the runner
// `program -m pytest [-q] [-x] [-k text] [--basetemp=DIR] [-p plugin] targets...`: test files and folders
// (conftest.py fixtures too), test_* functions and Test* classes' test_* methods in file order, fixtures
// by parameter name (function / class / module / session scope, yield fixtures torn down in reverse;
// tmp_path, tmp_path_factory, monkeypatch, capsys, request built in), CB_TEST_ONLY as run_all's plugin
// reads it, and the summary line pytest -q prints ("3 failed, 120 passed, 4 skipped in 2.01s").
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <cstring>
#include <map>

#include <windows.h>

#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);
const char* module_for_file_(const std::string& path);
void print_exception_(const var& e);

namespace {

var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }
var mkcls(const char* name, std::vector<var> bases, std::vector<std::pair<const char*, var>> members, const char* module) {
    var ns = new_dict();
    for (auto& [k, v] : members) ns.setitem(new_str(k), v);
    return make_class(name, name, bases, ns, {}, module);
}

var g_mod;                     // the pytest module
var E_Skipped, E_Failed, E_XFailed;

[[noreturn]] void skip_(const std::string& why) { raise(make_exc_args(T(E_Skipped), {new_str(why)})); }

// ------------------------------------------------------------------------------------------ marks
// a mark: (name, args, kwargs); a function's marks: its pytestmark list
var mark_of(const std::string& name, const var& args, const var& kwargs) { return new_tuple({new_str(name), args, kwargs}); }
void add_mark(const var& target, const var& mark) {
    var cur = getattr(target, "pytestmark"_s, None);
    var lst = cur.is_none() ? new_list() : new_list(collect(cur));
    lst.call("append", mark);
    target.setattr("pytestmark", lst);
}
var make_decorator(const std::string& name, const var& args, const var& kwargs);
var T_MarkDecorator;
var make_decorator(const std::string& name, const var& args, const var& kwargs) {
    var d = call0(T_MarkDecorator);
    d.setattr("name", new_str(name));
    d.setattr("args", args);
    d.setattr("kwargs", kwargs);
    return d;
}
std::vector<var> marks_of(const var& fn, const var& cls, const var& mod) {
    std::vector<var> out;
    for (const var& o : {mod, cls, fn}) {
        if (o.is_none()) continue;
        var m = getattr(o, "pytestmark"_s, None);
        if (m.is_none()) continue;
        if (is_tuple(m) && len_(m) == 3 && is_str(m[var(0)])) { out.push_back(m); continue; }
        if (m.is_ref() && type_of(m)->name == "MarkDecorator") {   // (pytestmark = pytest.mark.x: one mark, not a list)
            out.push_back(mark_of(str_utf8(m.getattr("name")), m.getattr("args"), m.getattr("kwargs")));
            continue;
        }
        for (var x : each(m)) out.push_back(is_tuple(x) ? x : mark_of(str_utf8(x.getattr("name")), x.getattr("args"), x.getattr("kwargs")));
    }
    return out;
}

// ------------------------------------------------------------------------------------------ fixtures
struct Fixture { var fn; std::string scope = "function"; bool autouse = false; };
struct Run {
    std::map<std::string, Fixture> fixtures;          // conftest + module + class, the nearest last
    std::map<std::string, var> session, module_cache, class_cache;
    std::vector<var> session_fin, module_fin, class_fin;
    std::string basetemp;
    int tmp_n = 0;
};
Run* g_run = nullptr;

std::vector<std::string> param_names(const var& f) {
    var fn = f;
    std::vector<std::string> out;
    if (fn.is_ref() && fn.ref()->type == T_method) fn = static_cast<MethodObj*>(fn.ref())->func;
    if (!fn.is_ref() || fn.ref()->type != T_function) return out;
    auto* fo = static_cast<FuncObj*>(fn.ref());
    for (auto& p : fo->params)
        if (p.kind == Param::PosOnly || p.kind == Param::Normal || p.kind == Param::KwOnly)
            if (!p.has_default) out.push_back(p.name);
    if (f.is_ref() && f.ref()->type == T_method && !out.empty()) out.erase(out.begin());   // (self)
    return out;
}
bool is_generator_fn(const var& f) {
    var fn = f;
    if (fn.is_ref() && fn.ref()->type == T_method) fn = static_cast<MethodObj*>(fn.ref())->func;
    return fn.is_ref() && fn.ref()->type == T_function && static_cast<FuncObj*>(fn.ref())->generator;
}
void collect_fixtures(const var& ns_owner, std::map<std::string, Fixture>& into) {
    var d = ns_owner.getattr("__dict__");
    for (var k : each(d)) {
        var v = d.getitem(k);
        var info = getattr(v, "_pytestfixturefunction"_s, None);
        if (info.is_none()) continue;
        Fixture f;
        f.fn = v;
        f.scope = str_utf8(info[var(0)]);
        f.autouse = info[var(1)].truthy();
        std::string name = info[var(2)].is_none() ? str_utf8(k) : str_utf8(info[var(2)]);
        into[name] = f;
    }
}

struct TestCtx {
    std::string name, nodeid;
    var params;                                       // parametrize values (a dict)
    std::map<std::string, var> fn_cache;
    std::vector<var> fn_fin;                          // teardowns (generators, finalizers)
    var monkey;                                       // the test's monkeypatch (undone after)
    var capsys;
    var instance, module;
    std::map<std::string, Fixture>* fixtures;
};

var builtin_fixture(const std::string& name, TestCtx& t, bool& found);
var fixture_value(const std::string& name, TestCtx& t, std::vector<std::string>& stack) {
    if (t.params.is_ref() && in(new_str(name), t.params)) return t.params.getitem(new_str(name));
    auto it = t.fixtures->find(name);
    if (it == t.fixtures->end()) {
        bool found = false;
        var v = builtin_fixture(name, t, found);
        if (found) return v;
        raise(E_LookupError, "fixture '" + name + "' not found");
    }
    Fixture& f = it->second;
    std::map<std::string, var>& cache = f.scope == "session" || f.scope == "package" ? g_run->session
                                      : f.scope == "module" ? g_run->module_cache
                                      : f.scope == "class" ? g_run->class_cache : t.fn_cache;
    std::vector<var>& fins = f.scope == "session" || f.scope == "package" ? g_run->session_fin
                           : f.scope == "module" ? g_run->module_fin
                           : f.scope == "class" ? g_run->class_fin : t.fn_fin;
    if (auto c = cache.find(name); c != cache.end()) return c->second;
    for (auto& s : stack) if (s == name) raise(E_RecursionError, "recursive dependency involving fixture '" + name + "'");
    stack.push_back(name);
    CallArgs ca;
    var fn = f.fn;
    if (!t.instance.is_none() && fn.is_ref() && fn.ref()->type == T_function && f.scope != "session") {
        // a fixture defined in a test class: bound to the instance
        auto* fo = static_cast<FuncObj*>(fn.ref());
        if (!fo->params.empty() && fo->params[0].name == "self") fn = t.instance.getattr(fo->name.c_str());
    }
    for (auto& p : param_names(fn)) ca.kw.push_back({new_str(p), fixture_value(p, t, stack)});
    stack.pop_back();
    var v;
    if (is_generator_fn(fn)) {
        var gen = call_obj(fn, ca);
        v = next(gen);
        fins.push_back(gen);
    } else v = call_obj(fn, ca);
    cache[name] = v;
    return v;
}

// tmp_path: a fresh folder under the base temp
var tmp_dir(const std::string& stem) {
    std::string s;
    for (char c : stem) s += isalnum((unsigned char)c) || c == '_' ? c : '_';
    if (s.size() > 30) s.resize(30);
    var os = import("os");
    std::string p;
    for (;;) {
        p = g_run->basetemp + "\\" + s + std::to_string(g_run->tmp_n++);
        if (!os.attr("path").call("exists", new_str(p)).truthy()) break;
    }
    os.call("makedirs", new_str(p));
    return call1(import("pathlib").getattr("Path"), new_str(p));
}

var T_Monkey, T_Capsys;
var builtin_fixture(const std::string& name, TestCtx& t, bool& found) {
    found = true;
    if (name == "tmp_path") return tmp_dir(t.name);
    if (name == "tmpdir") return tmp_dir(t.name);
    if (name == "tmp_path_factory") {
        var f = call0(import("types").getattr("SimpleNamespace"));
        f.setattr("mktemp", nat("mktemp", [](const CallArgs& a) { return tmp_dir(str_utf8(a.pos.at(0))); }));
        f.setattr("getbasetemp", nat("getbasetemp", [](const CallArgs&) { return call1(import("pathlib").getattr("Path"), new_str(g_run->basetemp)); }));
        return f;
    }
    if (name == "monkeypatch") {
        if (t.monkey.is_unbound()) t.monkey = call0(T_Monkey);
        return t.monkey;
    }
    if (name == "capsys" || name == "capfd") {
        if (t.capsys.is_unbound()) {
            t.capsys = call0(T_Capsys);
            t.capsys.call("_start");
        }
        return t.capsys;
    }
    if (name == "request") {
        var r = call0(import("types").getattr("SimpleNamespace"));
        r.setattr("param", t.params.is_ref() && in("__param__"_s, t.params) ? t.params.getitem("__param__"_s) : var(None));
        var node = call0(import("types").getattr("SimpleNamespace"));
        node.setattr("name", new_str(t.name));
        node.setattr("nodeid", new_str(t.nodeid));
        r.setattr("node", node);
        r.setattr("module", t.module);
        r.setattr("instance", t.instance);
        TestCtx* tp = &t;
        r.setattr("addfinalizer", nat("addfinalizer", [tp](const CallArgs& a) { tp->fn_fin.push_back(a.pos.at(0)); return None; }));
        var cfg = call0(import("types").getattr("SimpleNamespace"));
        cfg.setattr("getoption", nat("getoption", [](const CallArgs& a) { return a.pos.size() > 1 ? a.pos[1] : var(None); }));
        r.setattr("config", cfg);
        return r;
    }
    found = false;
    return None;
}

void finish(std::vector<var>& fins) {
    for (auto it = fins.rbegin(); it != fins.rend(); ++it) {
        var g = *it;
        try {
            if (g.is_ref() && g.ref()->type == T_generator) {
                var dummy;
                next_(g, dummy);
            } else call0(g);
        } catch (error& e) {
            print_exception_(e.value);
        }
    }
    fins.clear();
}

// ------------------------------------------------------------------------------------------ the runner
struct Item {
    std::string file, nodeid, name;
    var fn, cls, module;
    var params;                                      // a dict, or None
    std::vector<var> marks;
    bool unit = false;                               // a unittest.TestCase method
};

bool eval_cond(const var& c, const var& module) {
    if (is_str(c)) raise(E_NotImplementedError, "skipif with a string condition");
    (void)module;
    return c.truthy();
}

std::string short_repr(const var& v) {
    std::string s;
    try { s = is_str(v) ? str_utf8(v) : repr_(v); } catch (...) { s = "?"; }
    return s;
}

// the items of one parametrize mark set: each a dict of names to values, with an id
void expand(const std::vector<var>& pmarks, size_t i, var cur, std::string id, std::vector<std::pair<var, std::string>>& out) {
    if (i == pmarks.size()) { out.push_back({cur, id}); return; }
    var m = pmarks[i];
    var args = m[var(1)], kwargs = m[var(2)];
    var names = args[var(0)];
    var values = len_(args) > 1 ? args[var(1)] : kwargs.getitem("argvalues"_s);
    std::vector<std::string> nl;
    if (is_str(names)) {
        std::string s = str_utf8(names);
        size_t st = 0;
        while (st <= s.size()) {
            size_t e = s.find(',', st);
            std::string n = s.substr(st, e == std::string::npos ? std::string::npos : e - st);
            while (!n.empty() && n.front() == ' ') n.erase(n.begin());
            while (!n.empty() && n.back() == ' ') n.pop_back();
            if (!n.empty()) nl.push_back(n);
            if (e == std::string::npos) break;
            st = e + 1;
        }
    } else for (var n : each(names)) nl.push_back(str_utf8(n));
    var ids = getattr(kwargs, "get"_s, None).is_none() ? var(None) : kwargs.call("get", "ids"_s);
    int k = 0;
    for (var v : each(values)) {
        var vals = v;
        std::string pid;
        if (v.is_ref() && type_of(v)->name == "ParameterSet") {
            vals = v.getattr("values");
            if (!v.getattr("id").is_none()) pid = str_utf8(v.getattr("id"));
            if (nl.size() == 1) vals = vals[var(0)];
        }
        var d = new_dict();
        for (var key : each(cur)) d.setitem(key, cur.getitem(key));
        if (nl.size() == 1) d.setitem(new_str(nl[0]), vals);
        else {
            auto items = collect(vals);
            for (size_t j = 0; j < nl.size() && j < items.size(); j++) d.setitem(new_str(nl[j]), items[j]);
        }
        if (pid.empty()) {
            if (!ids.is_none() && !callable(ids).truthy()) pid = short_repr(collect(ids).at((size_t)k));
            else if (nl.size() == 1 && (is_str(vals) || is_int(vals) || vals.is_float() || vals.is_bool() || vals.is_none())) pid = short_repr(vals);
            else if (nl.size() > 1) {
                auto items = collect(vals);
                for (size_t j = 0; j < items.size(); j++) {
                    var x = items[j];
                    pid += (j ? "-" : "") + ((is_str(x) || is_int(x) || x.is_float() || x.is_bool() || x.is_none()) ? short_repr(x) : nl[j] + std::to_string(k));
                }
            } else pid = nl[0] + std::to_string(k);
        }
        expand(pmarks, i + 1, d, id.empty() ? pid : id + "-" + pid, out);
        k++;
    }
}

int run_main_(const var& argv) {
    auto t0 = std::chrono::steady_clock::now();
    std::vector<std::string> targets;
    std::string kexpr, basetemp;
    bool exitfirst = false, verbose = false;
    auto args = collect(argv);
    for (size_t i = 1; i < args.size(); i++) {
        std::string a = str_utf8(args[i]);
        if (a == "-q" || a == "-s" || a == "-rA" || a == "--no-header" || a == "-qq") continue;
        if (a == "-v") { verbose = true; continue; }
        if (a == "-x" || a == "--exitfirst") { exitfirst = true; continue; }
        if (a == "-p" || a == "-W" || a == "-o" || a == "--tb" || a == "-m") { i++; continue; }
        if (a == "-k" && i + 1 < args.size()) { kexpr = str_utf8(args[++i]); continue; }
        if (a.compare(0, 11, "--basetemp=") == 0) { basetemp = a.substr(11); continue; }
        if (a.compare(0, 5, "--tb=") == 0 || a.compare(0, 2, "-p") == 0 || a.compare(0, 2, "-W") == 0) continue;
        targets.push_back(a);
    }
    var os = import("os");
    Run run;
    g_run = &run;
    if (basetemp.empty()) {
        const char* st = getenv("CB_STATE");
        basetemp = str_utf8(os.attr("path").call("abspath", new_str(std::string(st ? st : "state") + "\\test-tmp\\pytest-cxx")));
    }
    basetemp = str_utf8(os.attr("path").call("abspath", new_str(basetemp)));
    if (os.attr("path").call("isdir", new_str(basetemp)).truthy()) import("shutil").call("rmtree", new_str(basetemp), py::kw("ignore_errors", var(true)));
    os.call("makedirs", new_str(basetemp), py::kw("exist_ok", var(true)));
    run.basetemp = basetemp;
    // the files: test_*.py / *_test.py under each folder, in name order; conftest.py beside them
    std::vector<std::string> files;
    std::vector<std::string> conftests;
    auto add_dir = [&](const std::string& d) {
        var walk = os.call("walk", new_str(d));
        for (var w : each(walk)) {
            auto parts = collect(w);
            std::string root = str_utf8(parts[0]);
            if (root.find("__pycache__") != std::string::npos) continue;
            auto names = collect(parts[2]);
            std::vector<std::string> ns;
            for (auto& n : names) ns.push_back(str_utf8(n));
            std::sort(ns.begin(), ns.end());
            for (auto& n : ns) {
                std::string full = root + "\\" + n;
                if (n == "conftest.py") conftests.push_back(full);
                else if (n.size() > 3 && n.compare(n.size() - 3, 3, ".py") == 0 &&
                         (n.compare(0, 5, "test_") == 0 || (n.size() > 8 && n.compare(n.size() - 8, 8, "_test.py") == 0)))
                    files.push_back(full);
            }
        }
    };
    for (auto& t : targets) {
        std::string p = t.substr(0, t.find("::"));
        if (os.attr("path").call("isdir", new_str(p)).truthy()) add_dir(p);
        else files.push_back(p);
    }
    std::string only;
    if (const char* o = getenv("CB_TEST_ONLY")) only = o;
    auto selected_file = [&](const std::string& f) {
        if (only.empty()) return true;
        std::string base = f.substr(f.find_last_of("/\\") + 1);
        size_t st = 0;
        while (st <= only.size()) {
            size_t e = only.find(',', st);
            if (only.substr(st, e == std::string::npos ? std::string::npos : e - st) == base) return true;
            if (e == std::string::npos) break;
            st = e + 1;
        }
        return false;
    };
    std::map<std::string, Fixture> conf_fixtures;
    for (auto& c : conftests) {
        const char* mn = module_for_file_(c);
        if (!mn) continue;
        collect_fixtures(import(mn), conf_fixtures);
    }
    int passed = 0, failed = 0, skipped = 0, errors = 0, xfailed = 0, xpassed = 0, deselected = 0;
    std::vector<std::string> fail_lines;
    bool stop = false;
    for (auto& f : files) {
        if (stop) break;
        std::string rel = str_utf8(os.attr("path").call("relpath", new_str(f)));
        for (auto& ch : rel) if (ch == '\\') ch = '/';
        const char* mn = module_for_file_(f);
        if (!mn) {
            errors++;
            fail_lines.push_back("ERROR " + rel + " - not a module of this program");
            continue;
        }
        var module;
        try {
            module = import(mn);
        } catch (error& e) {
            if (isinstance_(e.value, T(E_Skipped))) { skipped++; continue; }
            errors++;
            fprintf(stdout, "_ ERROR collecting %s _\n", rel.c_str());
            fflush(stdout);
            print_exception_(e.value);
            fail_lines.push_back("ERROR " + rel + " - " + tname(e.value) + ": " + short_repr(e.value));
            continue;
        }
        if (!selected_file(f)) continue;
        std::map<std::string, Fixture> fixtures = conf_fixtures;
        collect_fixtures(module, fixtures);
        // the items, in the module's order
        std::vector<Item> items;
        var d = module.getattr("__dict__");
        for (var k : each(d)) {
            std::string n = str_utf8(k);
            var v = d.getitem(k);
            if (n.compare(0, 4, "test") == 0 && v.is_ref() && v.ref()->type == T_function &&
                getattr(v, "_pytestfixturefunction"_s, None).is_none()) {
                Item it;
                it.file = rel;
                it.name = n;
                it.nodeid = rel + "::" + n;
                it.fn = v;
                it.cls = None;
                it.module = module;
                items.push_back(it);
            } else if (v.is_ref() && v.ref()->type->layout == Layout::Type && (n.compare(0, 4, "Test") == 0 || is_subtype(T(v), T(import("unittest").getattr("TestCase"))))) {
                bool unit = is_subtype(T(v), T(import("unittest").getattr("TestCase")));
                TypeObj* ct = T(v);
                if (!unit && ct->dict->get("__init__")) continue;   // (pytest does not collect a class with __init__)
                std::vector<std::string> meths;
                for (TypeObj* b : ct->mro) {
                    if (b == T_object) continue;
                    for (auto& e : b->dict->entries)
                        if (!e.key.is_unbound() && is_str(e.key) && str_utf8(e.key).compare(0, 4, "test") == 0 &&
                            std::find(meths.begin(), meths.end(), str_utf8(e.key)) == meths.end())
                            meths.push_back(str_utf8(e.key));
                }
                for (auto& mname : meths) {
                    Item it;
                    it.file = rel;
                    it.name = mname;
                    it.nodeid = rel + "::" + n + "::" + mname;
                    it.fn = *ct->lookup(new_str(mname));
                    it.cls = v;
                    it.unit = unit;
                    it.module = module;
                    items.push_back(it);
                }
            }
        }
        var last_cls = None;
        for (auto& base_item : items) {
            if (stop) break;
            if (!kexpr.empty() && base_item.nodeid.find(kexpr) == std::string::npos) { deselected++; continue; }
            if (!is(base_item.cls, last_cls)) {
                finish(run.class_fin);
                run.class_cache.clear();
                last_cls = base_item.cls;
            }
            auto marks = marks_of(base_item.fn, base_item.cls, module);
            std::vector<var> pmarks;
            std::string skip_reason;
            bool skip = false, xfail = false;
            std::vector<std::string> use;
            for (auto& m : marks) {
                std::string mn2 = str_utf8(m[var(0)]);
                var margs = m[var(1)], mkw = m[var(2)];
                if (mn2 == "parametrize") pmarks.insert(pmarks.begin(), m);   // (the decorator nearest the def applies first)
                else if (mn2 == "skip") { skip = true; skip_reason = len_(margs) ? str_utf8(margs[var(0)]) : in("reason"_s, mkw) ? str_utf8(mkw.getitem("reason"_s)) : "unconditional skip"; }
                else if (mn2 == "skipif") {
                    bool c = false;
                    for (var x : each(margs)) if (eval_cond(x, module)) c = true;
                    if (in("condition"_s, mkw) && mkw.getitem("condition"_s).truthy()) c = true;
                    if (c) { skip = true; skip_reason = in("reason"_s, mkw) ? str_utf8(mkw.getitem("reason"_s)) : "condition"; }
                } else if (mn2 == "xfail") {
                    bool c = len_(margs) == 0 || margs[var(0)].truthy();
                    if (c) xfail = true;
                } else if (mn2 == "usefixtures") for (var x : each(margs)) use.push_back(str_utf8(x));
            }
            std::vector<std::pair<var, std::string>> variants;
            if (pmarks.empty()) variants.push_back({None, ""});
            else expand(pmarks, 0, new_dict(), "", variants);
            for (auto& [params, pid] : variants) {
                if (stop) break;
                std::string nodeid = base_item.nodeid + (pid.empty() ? "" : "[" + pid + "]");
                if (skip) {
                    skipped++;
                    if (verbose) printf("%s SKIPPED (%s)\n", nodeid.c_str(), skip_reason.c_str());
                    continue;
                }
                TestCtx t;
                t.name = base_item.name + (pid.empty() ? "" : "[" + pid + "]");
                t.nodeid = nodeid;
                t.params = params;
                t.fixtures = &fixtures;
                t.module = module;
                t.instance = None;
                var fn = base_item.fn;
                std::string outcome = "passed", detail;
                var err;
                try {
                    if (base_item.unit) {
                        // a unittest.TestCase: setUpClass once, then setUp / the method / tearDown, its skips
                        for (const var& o : {base_item.cls, base_item.fn})
                            if (getattr(o, "__unittest_skip__"_s, var(false)).truthy())
                                skip_(str_utf8(call1(t::str, getattr(o, "__unittest_skip_why__"_s, ""_s))));
                        if (!getattr(base_item.cls, "_pyrt_class_set_up"_s, var(false)).truthy()) {
                            base_item.cls.setattr("_pyrt_class_set_up", var(true));
                            base_item.cls.call("setUpClass");
                            var cls = base_item.cls;
                            run.class_fin.push_back(nat("tearDownClass", [cls](const CallArgs&) { cls.call("tearDownClass"); return None; }));
                        }
                        t.instance = call1(base_item.cls, new_str(base_item.name));
                        t.instance.call("setUp");
                        try {
                            t.instance.call(base_item.name.c_str());
                        } catch (...) {
                            try { t.instance.call("tearDown"); t.instance.call("doCleanups"); } catch (...) {}
                            throw;
                        }
                        t.instance.call("tearDown");
                        t.instance.call("doCleanups");
                    } else {
                        if (!base_item.cls.is_none()) {
                            t.instance = call0(base_item.cls);
                            fn = t.instance.getattr(base_item.name.c_str());
                            if (hasattr(t.instance, "setup_method"_s).truthy()) t.instance.call("setup_method", fn);
                        }
                        std::vector<std::string> stack;
                        for (auto& [fname, fx] : fixtures) if (fx.autouse) fixture_value(fname, t, stack);
                        for (auto& u : use) fixture_value(u, t, stack);
                        CallArgs ca;
                        for (auto& p : param_names(fn)) ca.kw.push_back({new_str(p), fixture_value(p, t, stack)});
                        call_obj(fn, ca);
                        if (!t.instance.is_none() && hasattr(t.instance, "teardown_method"_s).truthy()) t.instance.call("teardown_method", fn);
                    }
                } catch (error& e) {
                    if (isinstance_(e.value, T(E_Skipped)) || isinstance_(e.value, T(import("unittest").getattr("SkipTest")))) { outcome = "skipped"; detail = short_repr(e.value); }
                    else { outcome = "failed"; err = e.value; }
                }
                try { finish(t.fn_fin); } catch (...) {}
                if (!t.monkey.is_unbound()) { try { t.monkey.call("undo"); } catch (error&) {} }
                if (!t.capsys.is_unbound()) { try { t.capsys.call("_stop"); } catch (error&) {} }
                if (xfail) outcome = outcome == "failed" ? "xfailed" : outcome == "passed" ? "xpassed" : outcome;
                if (outcome == "passed") passed++;
                else if (outcome == "skipped") skipped++;
                else if (outcome == "xfailed") xfailed++;
                else if (outcome == "xpassed") xpassed++;
                else {
                    failed++;
                    fprintf(stdout, "_______ %s _______\n", nodeid.c_str());
                    fflush(stdout);
                    print_exception_(err);
                    std::string msg = short_repr(err);
                    if (msg.size() > 160) msg.resize(160);
                    for (auto& ch : msg) if (ch == '\n') ch = ' ';
                    fail_lines.push_back("FAILED " + nodeid + " - " + type_of(err)->name + (msg.empty() ? "" : ": " + msg));
                    if (exitfirst) stop = true;
                }
                if (verbose) printf("%s %s\n", nodeid.c_str(), outcome.c_str());
            }
        }
        finish(run.class_fin);
        run.class_cache.clear();
        finish(run.module_fin);
        run.module_cache.clear();
    }
    finish(run.session_fin);
    for (auto& l : fail_lines) printf("%s\n", l.c_str());
    double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    std::string sum;
    auto part = [&](int n, const char* w) { if (n) sum += (sum.empty() ? "" : ", ") + std::to_string(n) + " " + w; };
    part(failed, "failed");
    part(passed, "passed");
    part(skipped, "skipped");
    part(deselected, "deselected");
    part(xfailed, "xfailed");
    part(xpassed, "xpassed");
    part(errors, errors == 1 ? "error" : "errors");
    char tail[64];
    snprintf(tail, sizeof tail, " in %.2fs", secs);
    if (sum.empty()) sum = "no tests ran";
    printf("%s%s\n", sum.c_str(), tail);
    fflush(stdout);
    g_run = nullptr;
    if (failed || errors) return 1;
    if (!passed && !skipped && !xfailed && !xpassed) return 5;
    return 0;
}

}  // namespace

// unittest: TestCase and its assertions (the messages unittest prints), SkipTest, the skip decorators,
// and main() over the __main__ module's TestCase classes (unittest's summary: "Ran N tests ... OK")
void init_unittest(const var& m) {
    var skip_exc = make_class("SkipTest", "SkipTest", {t::Exception}, new_dict(), {}, "unittest");
    mod_set(m, "SkipTest", skip_exc);
    var ns = new_dict();
    auto fail_with = [](const CallArgs& a, size_t msg_at, const std::string& std_msg) -> var {
        var msg = arg(a, msg_at, "msg", None);
        raise(make_exc_args(E_AssertionError, {new_str(msg.is_none() ? std_msg : std_msg + " : " + str_(msg))}));
    };
    auto def2 = [&](const char* name, std::function<bool(const var&, const var&)> ok, const char* fmt) {
        std::string f = fmt;
        ns.setitem(new_str(name), new_native(name, [ok, f, fail_with](const CallArgs& a) -> var {
            var x = a.pos.at(1), y = a.pos.at(2);
            if (ok(x, y)) return None;
            std::string s = f;
            size_t p = s.find("%a");
            if (p != std::string::npos) s.replace(p, 2, repr_(x));
            p = s.find("%b");
            if (p != std::string::npos) s.replace(p, 2, repr_(y));
            return fail_with(a, 3, s);
        }));
    };
    auto def1 = [&](const char* name, std::function<bool(const var&)> ok, const char* fmt) {
        std::string f = fmt;
        ns.setitem(new_str(name), new_native(name, [ok, f, fail_with](const CallArgs& a) -> var {
            var x = a.pos.at(1);
            if (ok(x)) return None;
            std::string s = f;
            size_t p = s.find("%a");
            if (p != std::string::npos) s.replace(p, 2, repr_(x));
            return fail_with(a, 2, s);
        }));
    };
    def2("assertEqual", [](const var& x, const var& y) { return x == y; }, "%a != %b");
    for (const char* n : {"assertListEqual", "assertTupleEqual", "assertDictEqual", "assertSetEqual", "assertSequenceEqual", "assertMultiLineEqual"})
        def2(n, [](const var& x, const var& y) { return x == y; }, "%a != %b");
    def2("assertNotEqual", [](const var& x, const var& y) { return x != y; }, "%a == %b");
    def2("assertIn", [](const var& x, const var& y) { return in(x, y); }, "%a not found in %b");
    def2("assertNotIn", [](const var& x, const var& y) { return !in(x, y); }, "%a unexpectedly found in %b");
    def2("assertIs", [](const var& x, const var& y) { return is(x, y); }, "%a is not %b");
    def2("assertIsNot", [](const var& x, const var& y) { return !is(x, y); }, "unexpectedly identical: %a");
    def2("assertLess", [](const var& x, const var& y) { return x < y; }, "%a not less than %b");
    def2("assertLessEqual", [](const var& x, const var& y) { return x <= y; }, "%a not less than or equal to %b");
    def2("assertGreater", [](const var& x, const var& y) { return x > y; }, "%a not greater than %b");
    def2("assertGreaterEqual", [](const var& x, const var& y) { return x >= y; }, "%a not greater than or equal to %b");
    def2("assertIsInstance", [](const var& x, const var& y) { return isinstance(x, y).truthy(); }, "%a is not an instance of %b");
    def2("assertNotIsInstance", [](const var& x, const var& y) { return !isinstance(x, y).truthy(); }, "%a is an instance of %b");
    def2("assertCountEqual", [](const var& x, const var& y) {
        var cx = call1(import("collections").getattr("Counter"), new_list(collect(x)));
        var cy = call1(import("collections").getattr("Counter"), new_list(collect(y)));
        return cx == cy;
    }, "Element counts were not equal: %a, %b");
    def2("assertRegex", [](const var& x, const var& y) { return !import("re").call("search", y, x).is_none(); }, "Regex didn't match: %b not found in %a");
    def2("assertNotRegex", [](const var& x, const var& y) { return import("re").call("search", y, x).is_none(); }, "Regex matched: %b in %a");
    def1("assertTrue", [](const var& x) { return x.truthy(); }, "%a is not true");
    def1("assertFalse", [](const var& x) { return !x.truthy(); }, "%a is not false");
    def1("assertIsNone", [](const var& x) { return x.is_none(); }, "%a is not None");
    def1("assertIsNotNone", [](const var& x) { return !x.is_none(); }, "unexpectedly None");
    ns.setitem("assertAlmostEqual"_s, new_native("assertAlmostEqual", [fail_with](const CallArgs& a) -> var {
        var x = a.pos.at(1), y = a.pos.at(2);
        if (x == y) return None;
        int64_t places = kwarg(a, "places", a.pos.size() > 3 && !a.pos[3].is_none() ? a.pos[3] : var(7)).as_int64();
        var delta = kwarg(a, "delta", None);
        double d = std::fabs(x.as_double() - y.as_double());
        if (!delta.is_none() ? d <= delta.as_double() : std::round(d * std::pow(10.0, (double)places)) == 0) return None;
        return fail_with(a, 99, repr_(x) + " != " + repr_(y) + " within " + std::to_string(places) + " places");
    }));
    ns.setitem("fail"_s, new_native("fail", [](const CallArgs& a) -> var {
        raise(make_exc_args(E_AssertionError, {a.pos.size() > 1 ? a.pos[1] : var(None)}));
    }));
    ns.setitem("skipTest"_s, new_native("skipTest", [skip_exc](const CallArgs& a) -> var { raise(make_exc_args(T(skip_exc), {a.pos.at(1)})); }));
    ns.setitem("assertRaises"_s, new_native("assertRaises", [](const CallArgs& a) -> var {
        CallArgs b;
        b.pos.assign(a.pos.begin() + 1, a.pos.end());
        b.kw = a.kw;
        return call_obj(import("pytest").getattr("raises"), b);
    }));
    ns.setitem("assertRaisesRegex"_s, new_native("assertRaisesRegex", [](const CallArgs& a) -> var {
        CallArgs b;
        b.pos = {a.pos.at(1)};
        b.kw.push_back({"match"_s, a.pos.at(2)});
        for (size_t i = 3; i < a.pos.size(); i++) b.pos.push_back(a.pos[i]);
        return call_obj(import("pytest").getattr("raises"), b);
    }));
    ns.setitem("__init__"_s, new_native("__init__", [](const CallArgs& a) {
        a.pos.at(0).setattr("_testMethodName", a.pos.size() > 1 ? a.pos[1] : var("runTest"_s));
        a.pos[0].setattr("_cleanups", new_list());
        a.pos[0].setattr("maxDiff", var(640));
        return None;
    }));
    for (const char* n : {"setUp", "tearDown"}) ns.setitem(new_str(n), new_native(n, [](const CallArgs&) { return None; }));
    ns.setitem("addCleanup"_s, new_native("addCleanup", [](const CallArgs& a) {
        CallArgs rest;
        rest.pos.assign(a.pos.begin() + 2, a.pos.end());
        rest.kw = a.kw;
        var f = a.pos.at(1);
        a.pos[0].getattr("_cleanups").call("append", new_native("cleanup", [f, rest](const CallArgs&) { return call_obj(f, rest); }));
        return None;
    }));
    ns.setitem("doCleanups"_s, new_native("doCleanups", [](const CallArgs& a) {
        var cl = a.pos.at(0).getattr("_cleanups");
        while (len_(cl)) call0(cl.call("pop"));
        return None;
    }));
    ns.setitem("subTest"_s, new_native("subTest", [](const CallArgs&) { return call0(import("contextlib").getattr("nullcontext")); }));
    ns.setitem("id"_s, new_native("id", [](const CallArgs& a) { return new_str(type_of(a.pos.at(0))->qualname + "." + str_utf8(a.pos[0].getattr("_testMethodName"))); }));
    var tc = make_class("TestCase", "TestCase", {}, ns, {}, "unittest");
    add_classmethod(T(tc), "setUpClass", [](const CallArgs&) { return None; });
    add_classmethod(T(tc), "tearDownClass", [](const CallArgs&) { return None; });
    mod_set(m, "TestCase", tc);
    auto skipper = [](const var& why) {
        return new_native("skip", [why](const CallArgs& a) {
            a.pos.at(0).setattr("__unittest_skip__", var(true));
            a.pos[0].setattr("__unittest_skip_why__", why);
            return a.pos[0];
        });
    };
    var identity = new_native("noskip", [](const CallArgs& a) { return a.pos.at(0); });
    mod_def(m, "skip", [skipper](const CallArgs& a) { return skipper(a.pos.at(0)); });
    mod_def(m, "skipIf", [skipper, identity](const CallArgs& a) { return a.pos.at(0).truthy() ? skipper(a.pos.at(1)) : identity; });
    mod_def(m, "skipUnless", [skipper, identity](const CallArgs& a) { return !a.pos.at(0).truthy() ? skipper(a.pos.at(1)) : identity; });
    mod_def(m, "expectedFailure", [](const CallArgs& a) { a.pos.at(0).setattr("__unittest_expecting_failure__", var(true)); return a.pos[0]; });
    // main(): the TestCase classes of __main__, in order; unittest's report on stderr, the exit status
    mod_def(m, "main", [tc, skip_exc](const CallArgs& a) -> var {
        auto t0 = std::chrono::steady_clock::now();
        var mod = import("sys").getattr("modules").getitem("__main__"_s);
        var d = mod.getattr("__dict__");
        int ran = 0, failures = 0, errors = 0, skipped = 0;
        var err = import("sys").getattr("stderr");
        for (var k : each(d)) {
            var c = d.getitem(k);
            if (!c.is_ref() || c.ref()->type->layout != Layout::Type || !is_subtype(T(c), T(tc)) || is(c, tc)) continue;
            std::vector<std::string> meths;
            for (TypeObj* b : T(c)->mro)
                for (auto& e : b->dict->entries)
                    if (!e.key.is_unbound() && is_str(e.key) && str_utf8(e.key).compare(0, 4, "test") == 0 &&
                        std::find(meths.begin(), meths.end(), str_utf8(e.key)) == meths.end())
                        meths.push_back(str_utf8(e.key));
            std::sort(meths.begin(), meths.end());
            c.call("setUpClass");
            for (auto& mn : meths) {
                ran++;
                var inst = call1(c, new_str(mn));
                try {
                    if (getattr(c, "__unittest_skip__"_s, var(false)).truthy() || getattr(inst.getattr(mn.c_str()), "__unittest_skip__"_s, var(false)).truthy()) {
                        skipped++;
                        continue;
                    }
                    inst.call("setUp");
                    inst.call(mn.c_str());
                    inst.call("tearDown");
                    inst.call("doCleanups");
                } catch (error& e) {
                    if (isinstance_(e.value, T(skip_exc))) { skipped++; continue; }
                    bool f = isinstance_(e.value, E_AssertionError);
                    (f ? failures : errors)++;
                    err.call("write", new_str(std::string(70, '=') + "\n" + (f ? "FAIL" : "ERROR") + ": " + mn + " (" + T(c)->qualname + ")\n" +
                                              std::string(70, '-') + "\n" + tname(e.value) + ": " + str_(e.value) + "\n\n"));
                }
            }
            c.call("tearDownClass");
        }
        double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        char buf[64];
        snprintf(buf, sizeof buf, "%.3fs", secs);
        std::string tail = failures || errors ? "FAILED (" + std::string(failures ? "failures=" + std::to_string(failures) : "") +
                                                   (failures && errors ? ", " : "") + (errors ? "errors=" + std::to_string(errors) : "") +
                                                   (skipped ? ", skipped=" + std::to_string(skipped) : "") + ")"
                                             : "OK" + std::string(skipped ? " (skipped=" + std::to_string(skipped) + ")" : "");
        err.call("write", new_str(std::string(70, '-') + "\nRan " + std::to_string(ran) + " test" + (ran == 1 ? "" : "s") + " in " + buf + "\n\n" + tail + "\n"));
        bool ok = !failures && !errors;
        if (kwarg(a, "exit", var(true)).truthy()) raise(make_exc_args(E_SystemExit, {var(ok ? 0 : 1)}));
        return None;
    });
}

void init_pytest(const var& m) {
    g_mod = m;
    E_Skipped = make_class("Skipped", "Skipped", {t::BaseException}, new_dict(), {}, "_pytest.outcomes");
    E_Failed = make_class("Failed", "Failed", {t::BaseException}, new_dict(), {}, "_pytest.outcomes");
    E_XFailed = make_class("XFailed", "XFailed", {E_Failed}, new_dict(), {}, "_pytest.outcomes");
    // skip and fail: callables carrying their exception class (pytest.skip.Exception), as pytest's
    auto with_exc = [](const char* name, native_fn f, const var& exc) {
        var ns = new_dict();
        ns.setitem("__call__"_s, new_native(name, [f](const CallArgs& a) { CallArgs b; b.pos.assign(a.pos.begin() + 1, a.pos.end()); b.kw = a.kw; return f(b); }));
        var o = call0(make_class(name, name, {}, ns, {}, "_pytest.outcomes"));
        o.setattr("Exception", exc);
        return o;
    };
    var skip = with_exc("skip", [](const CallArgs& a) -> var { skip_(a.pos.empty() ? str_utf8(kwarg(a, "reason", ""_s)) : str_utf8(a.pos[0])); }, E_Skipped);
    mod_set(m, "skip", skip);
    var fail = with_exc("fail", [](const CallArgs& a) -> var { raise(make_exc_args(T(E_Failed), {a.pos.empty() ? kwarg(a, "reason", ""_s) : a.pos[0]})); }, E_Failed);
    mod_set(m, "fail", fail);
    mod_def(m, "xfail", [](const CallArgs& a) -> var { raise(make_exc_args(T(E_XFailed), {a.pos.empty() ? ""_s : a.pos[0]})); });
    mod_def(m, "importorskip", [](const CallArgs& a) -> var {
        try {
            return import(str_utf8(a.pos.at(0)).c_str());
        } catch (error& e) {
            if (!isinstance_(e.value, E_ImportError)) throw;
            skip_("could not import '" + str_utf8(a.pos[0]) + "': " + short_repr(e.value));
        }
    });
    // marks: pytest.mark.<name>(...) and @pytest.mark.<name>
    var mdns = new_dict();
    mdns.setitem("__call__"_s, nat("__call__", [](const CallArgs& a) -> var {
        var self = a.pos.at(0);
        std::string name = str_utf8(self.getattr("name"));
        if (a.pos.size() == 2 && a.kw.empty() && callable(a.pos[1]).truthy() &&   // (pytest's rule: one callable: decorate it)
            (a.pos[1].ref()->type == T_function || a.pos[1].ref()->type->layout == Layout::Type)) {
            add_mark(a.pos[1], mark_of(name, self.getattr("args"), self.getattr("kwargs")));
            return a.pos[1];
        }
        var kw = new_dict();
        for (auto& [k, v] : a.kw) kw.setitem(k, v);
        return make_decorator(name, new_tuple(std::vector<var>(a.pos.begin() + 1, a.pos.end())), kw);
    }));
    T_MarkDecorator = make_class("MarkDecorator", "MarkDecorator", {}, mdns, {}, "_pytest.mark");
    var mgns = new_dict();
    mgns.setitem("__getattr__"_s, nat("__getattr__", [](const CallArgs& a) -> var {
        std::string n = str_utf8(a.pos.at(1));
        if (n.size() > 1 && n[0] == '_') raise(E_AttributeError, n);
        return make_decorator(n, new_tuple(), new_dict());
    }));
    mod_set(m, "mark", call0(make_class("MarkGenerator", "MarkGenerator", {}, mgns, {}, "_pytest.mark")));
    var psns = new_dict();
    var param_set = make_class("ParameterSet", "ParameterSet", {}, psns, {}, "_pytest.mark");
    mod_def(m, "param", [param_set](const CallArgs& a) {
        var p = call0(param_set);
        p.setattr("values", new_tuple(a.pos));
        p.setattr("id", kwarg(a, "id", None));
        p.setattr("marks", kwarg(a, "marks", new_tuple()));
        return p;
    });
    // fixture(fn) / fixture(scope=..., autouse=..., name=...)(fn)
    mod_def(m, "fixture", [](const CallArgs& a) -> var {
        var scope = kwarg(a, "scope", "function"_s), autouse = kwarg(a, "autouse", var(false)), name = kwarg(a, "name", None);
        var info = new_tuple({scope, autouse, name});
        if (!a.pos.empty() && callable(a.pos[0]).truthy()) {
            a.pos[0].setattr("_pytestfixturefunction", info);
            return a.pos[0];
        }
        return nat("fixture", [info](const CallArgs& b) {
            b.pos.at(0).setattr("_pytestfixturefunction", info);
            return b.pos[0];
        });
    });
    // raises
    var info_ns = new_dict();
    info_ns.setitem("match"_s, nat("match", [](const CallArgs& a) -> var {
        var self = a.pos.at(0);
        var s = call1(t::str, self.getattr("value"));
        if (import("re").call("search", a.pos.at(1), s).is_none())
            raise(E_AssertionError, "Regex pattern did not match.\n Regex: " + repr_(a.pos[1]) + "\n Input: " + repr_(s));
        return var(true);
    }));
    info_ns.setitem("errisinstance"_s, nat("errisinstance", [](const CallArgs& a) { return var(isinstance(a.pos.at(0).getattr("value"), a.pos.at(1)).truthy()); }));
    var exc_info = make_class("ExceptionInfo", "ExceptionInfo", {}, info_ns, {}, "_pytest._code");
    var rc_ns = new_dict();
    rc_ns.setitem("__enter__"_s, nat("__enter__", [](const CallArgs& a) { return a.pos.at(0).getattr("_info"); }));
    rc_ns.setitem("__exit__"_s, nat("__exit__", [](const CallArgs& a) -> var {
        var self = a.pos.at(0), et = a.pos.at(1), ev = a.pos.at(2);
        if (et.is_none()) raise(make_exc_args(T(E_Failed), {new_str("DID NOT RAISE " + repr_(self.getattr("_expected")))}));
        if (!issubclass(et, self.getattr("_expected")).truthy()) return var(false);
        var info = self.getattr("_info");
        info.setattr("value", ev);
        info.setattr("type", et);
        info.setattr("tb", None);
        var match = self.getattr("_match");
        if (!match.is_none()) info.call("match", match);
        return var(true);
    }));
    var raises_ctx = make_class("RaisesContext", "RaisesContext", {}, rc_ns, {}, "_pytest.python_api");
    mod_def(m, "raises", [exc_info, raises_ctx](const CallArgs& a) -> var {
        var expected = a.pos.at(0);
        var match = kwarg(a, "match", None);
        var info = call0(exc_info);
        if (a.pos.size() > 1) {
            CallArgs c;
            c.pos.assign(a.pos.begin() + 2, a.pos.end());
            for (auto& [k, v] : a.kw) if (str_utf8(k) != "match") c.kw.push_back({k, v});
            try {
                call_obj(a.pos[1], c);
            } catch (error& e) {
                if (!isinstance(e.value, expected).truthy()) throw;
                info.setattr("value", e.value);
                info.setattr("type", tv(type_of(e.value)));
                return info;
            }
            raise(make_exc_args(T(E_Failed), {new_str("DID NOT RAISE " + repr_(expected))}));
        }
        var ctx = call0(raises_ctx);
        ctx.setattr("_expected", expected);
        ctx.setattr("_match", match);
        ctx.setattr("_info", info);
        return ctx;
    });
    // approx: numbers (and sequences / dicts of them) within rel 1e-6 or abs 1e-12
    var ap_ns = new_dict();
    ap_ns.setitem("__eq__"_s, nat("__eq__", [](const CallArgs& a) -> var {
        var self = a.pos.at(0), other = a.pos.at(1);
        var exp = self.getattr("expected");
        double rel = self.getattr("rel").is_none() ? 1e-6 : self.getattr("rel").as_double();
        double ab = self.getattr("abs").is_none() ? 1e-12 : self.getattr("abs").as_double();
        std::function<bool(const var&, const var&)> close = [&](const var& x, const var& y) -> bool {
            if ((is_int(x) || x.is_float()) && (is_int(y) || y.is_float())) {
                double e = x.as_double(), g = y.as_double();
                return std::fabs(e - g) <= std::max(rel * std::fabs(e), ab);
            }
            if (is_dict(x) && is_dict(y)) {
                if (len_(x) != len_(y)) return false;
                for (var k : each(x)) if (!in(k, y) || !close(x.getitem(k), y.getitem(k))) return false;
                return true;
            }
            if ((is_list(x) || is_tuple(x)) && (is_list(y) || is_tuple(y))) {
                auto xs = collect(x), ys = collect(y);
                if (xs.size() != ys.size()) return false;
                for (size_t i = 0; i < xs.size(); i++) if (!close(xs[i], ys[i])) return false;
                return true;
            }
            return x == y;
        };
        return var(close(exp, other));
    }));
    ap_ns.setitem("__repr__"_s, nat("__repr__", [](const CallArgs& a) { return new_str(repr_(a.pos.at(0).getattr("expected")) + " +- tolerance"); }));
    var approx_cls = make_class("ApproxBase", "ApproxBase", {}, ap_ns, {}, "_pytest.python_api");
    mod_def(m, "approx", [approx_cls](const CallArgs& a) {
        var o = call0(approx_cls);
        o.setattr("expected", a.pos.at(0));
        o.setattr("rel", kwarg(a, "rel", None));
        o.setattr("abs", kwarg(a, "abs", None));
        return o;
    });
    // monkeypatch
    var mp = new_dict();
    mp.setitem("__init__"_s, nat("__init__", [](const CallArgs& a) { a.pos.at(0).setattr("_undo", new_list()); return None; }));
    mp.setitem("setattr"_s, nat("setattr", [](const CallArgs& a) {
        var self = a.pos.at(0);
        var target = a.pos.at(1), name, value;
        bool raising = kwarg(a, "raising", var(true)).truthy();
        if (is_str(target) && a.pos.size() == 3) {
            // "pkg.mod.attr"
            std::string s = str_utf8(target);
            size_t dot = s.rfind('.');
            name = new_str(s.substr(dot + 1));
            target = import(s.substr(0, dot).c_str());
            value = a.pos[2];
        } else {
            name = a.pos.at(2);
            value = a.pos.at(3);
        }
        var old = getattr(target, name, var());
        if (old.is_unbound() && raising) raise(E_AttributeError, repr_(target) + " has no attribute " + repr_(name));
        self.getattr("_undo").call("append", new_tuple({"attr"_s, target, name, old.is_unbound() ? var(None) : old, var(!old.is_unbound())}));
        setattr(target, name, value);
        return None;
    }));
    mp.setitem("delattr"_s, nat("delattr", [](const CallArgs& a) {
        var self = a.pos.at(0), target = a.pos.at(1), name = a.pos.at(2);
        var old = getattr(target, name, var());
        if (old.is_unbound()) { if (kwarg(a, "raising", var(true)).truthy()) raise(E_AttributeError, str_utf8(name)); return None; }
        self.getattr("_undo").call("append", new_tuple({"attr"_s, target, name, old, var(true)}));
        delattr(target, name);
        return None;
    }));
    mp.setitem("setitem"_s, nat("setitem", [](const CallArgs& a) {
        var self = a.pos.at(0), d = a.pos.at(1), k = a.pos.at(2);
        bool had = in(k, d);
        self.getattr("_undo").call("append", new_tuple({"item"_s, d, k, had ? d.getitem(k) : var(None), var(had)}));
        d.setitem(k, a.pos.at(3));
        return None;
    }));
    mp.setitem("delitem"_s, nat("delitem", [](const CallArgs& a) {
        var self = a.pos.at(0), d = a.pos.at(1), k = a.pos.at(2);
        if (!in(k, d)) { if (kwarg(a, "raising", var(true)).truthy()) raise(make_exc_args(E_KeyError, {k})); return None; }
        self.getattr("_undo").call("append", new_tuple({"item"_s, d, k, d.getitem(k), var(true)}));
        d.delitem(k);
        return None;
    }));
    mp.setitem("setenv"_s, nat("setenv", [](const CallArgs& a) {
        var self = a.pos.at(0);
        var env = import("os").getattr("environ");
        var v = call1(t::str, a.pos.at(2));
        var pre = kwarg(a, "prepend", None);
        if (!pre.is_none() && in(a.pos[1], env)) v = v + pre + env.getitem(a.pos[1]);
        self.call("setitem", env, a.pos[1], v);
        return None;
    }));
    mp.setitem("delenv"_s, nat("delenv", [](const CallArgs& a) {
        var env = import("os").getattr("environ");
        if (!in(a.pos.at(1), env) && !kwarg(a, "raising", var(true)).truthy()) return None;
        a.pos[0].call("delitem", env, a.pos[1], kw("raising", kwarg(a, "raising", var(true))));
        return None;
    }));
    mp.setitem("syspath_prepend"_s, nat("syspath_prepend", [](const CallArgs& a) {
        var p = import("sys").getattr("path");
        a.pos.at(0).getattr("_undo").call("append", new_tuple({"syspath"_s, new_list(collect(p)), None, None, var(true)}));
        p.call("insert", var(0), call1(t::str, a.pos.at(1)));
        return None;
    }));
    mp.setitem("chdir"_s, nat("chdir", [](const CallArgs& a) {
        var os = import("os");
        a.pos.at(0).getattr("_undo").call("append", new_tuple({"cwd"_s, os.call("getcwd"), None, None, var(true)}));
        os.call("chdir", a.pos.at(1));
        return None;
    }));
    mp.setitem("undo"_s, nat("undo", [](const CallArgs& a) {
        var u = a.pos.at(0).getattr("_undo");
        auto items = collect(u);
        for (auto it = items.rbegin(); it != items.rend(); ++it) {
            auto p = collect(*it);
            std::string kind = str_utf8(p[0]);
            if (kind == "attr") { if (p[4].truthy()) setattr(p[1], p[2], p[3]); else delattr(p[1], p[2]); }
            else if (kind == "item") { if (p[4].truthy()) p[1].setitem(p[2], p[3]); else if (in(p[2], p[1])) p[1].delitem(p[2]); }
            else if (kind == "syspath") { var sp = import("sys").getattr("path"); sp.call("clear"); sp.call("extend", p[1]); }
            else if (kind == "cwd") import("os").call("chdir", p[1]);
        }
        u.call("clear");
        return None;
    }));
    T_Monkey = make_class("MonkeyPatch", "MonkeyPatch", {}, mp, {}, "_pytest.monkeypatch");
    mod_set(m, "MonkeyPatch", T_Monkey);
    // capsys: sys.stdout / sys.stderr into StringIO while the test runs
    var cs = new_dict();
    cs.setitem("_start"_s, nat("_start", [](const CallArgs& a) {
        var self = a.pos.at(0), sys = import("sys"), io = import("io");
        self.setattr("_old", new_tuple({sys.getattr("stdout"), sys.getattr("stderr")}));
        self.setattr("_out", call0(io.getattr("StringIO")));
        self.setattr("_err", call0(io.getattr("StringIO")));
        sys.setattr("stdout", self.getattr("_out"));
        sys.setattr("stderr", self.getattr("_err"));
        return None;
    }));
    cs.setitem("_stop"_s, nat("_stop", [](const CallArgs& a) {
        var self = a.pos.at(0), sys = import("sys");
        var old = self.getattr("_old");
        sys.setattr("stdout", old[var(0)]);
        sys.setattr("stderr", old[var(1)]);
        return None;
    }));
    cs.setitem("readouterr"_s, nat("readouterr", [](const CallArgs& a) {
        var self = a.pos.at(0), io = import("io");
        var out = self.getattr("_out").call("getvalue"), err = self.getattr("_err").call("getvalue");
        self.getattr("_out").call("seek", var(0));
        self.getattr("_out").call("truncate");
        self.getattr("_err").call("seek", var(0));
        self.getattr("_err").call("truncate");
        var r = call0(import("types").getattr("SimpleNamespace"));
        r.setattr("out", out);
        r.setattr("err", err);
        return r;
    }));
    T_Capsys = make_class("CaptureFixture", "CaptureFixture", {}, cs, {}, "_pytest.capture");
    mod_def(m, "_main", [](const CallArgs& a) { return var((int64_t)run_main_(a.pos.at(0))); });
    mod_def(m, "main", [](const CallArgs& a) {
        var argv = new_list({"pytest"_s});
        if (!a.pos.empty()) for (var x : each(a.pos[0])) argv.call("append", x);
        return var((int64_t)run_main_(argv));
    });
    mod_set(m, "__version__", "8.0.0 (pyrt)"_s);
}

}  // namespace py
