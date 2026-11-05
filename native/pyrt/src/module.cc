// module.cc -- modules and import: every translated module registers a module_def at static
// initialisation; import runs its init once (sys.modules first, the parents before the child, a
// partially initialised module visible to circular imports as in CPython); the library modules
// (os, re, json ...) are native and made on first import.
#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <windows.h>

#include "rt.hpp"

namespace py {

struct Registry {
    std::map<std::string, module_def*> defs;
    std::map<std::string, native_module_init> natives;
};
static Registry& reg() {
    static Registry* r = new Registry();
    return *r;
}
var g_sys_modules;   // sys.modules (a dict)
const char* module_for_file_(const std::string& path);

static var new_module_obj(const std::string& name) {
    auto* m = new ModuleObj();
    m->type = T_module;
    m->name = name;
    return var(static_cast<Obj*>(m));
}

void ensure_runtime_();
module_def::module_def(const char* name, void (*init)(), bool package, const char* file)
    : init_(init), package_(package), file_(file), name_(name) {
    ensure_runtime_();     // (at static initialisation: the types exist before any module's cells)
    reg().defs[name] = this;
}
var& module_def::global(const char* name) {
    if (mod_.is_unbound()) mod_ = new_module_obj(name_);
    return static_cast<ModuleObj*>(mod_.ref())->cell(name)->v;
}

void register_native_module(const char* name, native_module_init init) { reg().natives[name] = init; }
void mod_set(const var& m, const char* name, const var& v) { static_cast<ModuleObj*>(m.ref())->cell(name)->v = v; }
void mod_def(const var& m, const char* name, native_fn fn) { mod_set(m, name, new_native(name, std::move(fn))); }

static DictObj* sysmods() {
    if (g_sys_modules.is_unbound()) g_sys_modules = new_dict_obj();
    return static_cast<DictObj*>(g_sys_modules.ref());
}

static var load_one(const std::string& name) {
    if (var* m = sysmods()->get(new_str(name))) return *m;
    auto& r = reg();
    auto d = r.defs.find(name);
    if (d != r.defs.end()) {
        module_def* def = d->second;
        if (def->module().is_unbound()) def->global("__name__");
        var mod = def->module();
        auto* mo = static_cast<ModuleObj*>(mod.ref());
        mo->def = def;
        mo->cell("__name__")->v = new_str(name);
        if (def->file_) mo->cell("__file__")->v = new_str(def->file_);
        if (def->package_) mo->cell("__path__")->v = new_list({new_str(def->file_ ? def->file_ : "")});
        std::string pkg = def->package_ ? name : (name.rfind('.') == std::string::npos ? "" : name.substr(0, name.rfind('.')));
        mo->cell("__package__")->v = new_str(pkg);
        if (mo->cells.find("__doc__") == mo->cells.end() || mo->cell("__doc__")->v.is_unbound()) mo->cell("__doc__")->v = None;
        sysmods()->set(new_str(name), mod);
        try {
            def->init_();
        } catch (...) {
            sysmods()->del(new_str(name));
            throw;
        }
        mo->initialized = true;
        return mod;
    }
    auto n = r.natives.find(name);
    if (n != r.natives.end()) {
        var mod = new_module_obj(name);
        static_cast<ModuleObj*>(mod.ref())->cell("__name__")->v = new_str(name);
        sysmods()->set(new_str(name), mod);
        try {
            n->second(mod);
        } catch (...) {
            sysmods()->del(new_str(name));
            throw;
        }
        return mod;
    }
    // by path, as Python's finders: a sys.path entry holding the dotted name as a file the program has under
    // another name (tests.java.x is java.x), or as a folder (a namespace package: tests)
    if (name != "sys" && name.find('/') == std::string::npos) {
        var sp = sysmods()->get(new_str("sys")) ? sysmods()->get(new_str("sys"))->getattr("path") : var(None);
        std::vector<std::string> entries;
        if (!sp.is_none()) for (var p : each(sp)) if (is_str(p)) entries.push_back(str_utf8(p));
        entries.push_back(".");
        std::string rel = name;
        std::replace(rel.begin(), rel.end(), '.', '\\');
        for (auto& en : entries) {
            std::string base = (en.empty() ? std::string(".") : en) + "\\" + rel;
            for (const std::string& f : {base + ".py", base + "\\__init__.py"}) {
                if (GetFileAttributesA(f.c_str()) == INVALID_FILE_ATTRIBUTES) continue;
                const char* mn = module_for_file_(f);
                if (mn && name != mn) {
                    var mod = import(mn);
                    sysmods()->set(new_str(name), mod);
                    return mod;
                }
            }
            DWORD at = GetFileAttributesA(base.c_str());
            if (at != INVALID_FILE_ATTRIBUTES && (at & FILE_ATTRIBUTE_DIRECTORY) && name.find('.') == std::string::npos) {
                var mod = new_module_obj(name);
                auto* mo = static_cast<ModuleObj*>(mod.ref());
                mo->cell("__name__")->v = new_str(name);
                mo->cell("__path__")->v = new_list({new_str(base)});
                mo->cell("__package__")->v = new_str(name);
                mo->cell("__doc__")->v = None;
                sysmods()->set(new_str(name), mod);
                return mod;
            }
        }
    }
    var e = make_exc(E_ModuleNotFoundError, "No module named '" + name + "'");
    e.setattr("name", new_str(name));
    raise(e);
}

var import(const char* dotted) {
    std::string full = dotted;
    if (var* m = sysmods()->get(new_str(full))) return *m;
    // the parents first, each child set as its parent's attribute
    size_t p = 0;
    var parent;
    for (;;) {
        size_t q = full.find('.', p);
        std::string part = full.substr(0, q);
        var m = load_one(part);
        if (!parent.is_unbound()) {
            std::string leaf = part.substr(part.rfind('.') + 1);
            static_cast<ModuleObj*>(parent.ref())->cell(leaf)->v = m;
        }
        parent = m;
        if (q == std::string::npos) return m;
        p = q + 1;
    }
}
var import_top(const char* dotted) {
    import(dotted);
    std::string top = std::string(dotted).substr(0, std::string(dotted).find('.'));
    return *sysmods()->get(new_str(top));
}
var import_from(const var& module, const char* name) {
    auto* m = static_cast<ModuleObj*>(module.ref());
    auto it = m->cells.find(name);
    if (it != m->cells.end() && !it->second->v.is_unbound()) return it->second->v;
    // a submodule
    std::string sub = m->name + "." + name;
    if (reg().defs.count(sub) || reg().natives.count(sub) || sysmods()->get(new_str(sub))) return import(sub.c_str());
    bool found;
    (void)found;
    try {
        return module.getattr(name);
    } catch (error& e) {
        if (!isinstance_(e.value, E_AttributeError)) throw;
    }
    var e = make_exc(E_ImportError, "cannot import name '" + std::string(name) + "' from '" + m->name + "'");
    raise(e);
}
void import_star(const var& module, module_def& into) {
    auto* m = static_cast<ModuleObj*>(module.ref());
    auto all = m->cells.find("__all__");
    if (all != m->cells.end() && !all->second->v.is_unbound()) {
        for (auto& n : collect(all->second->v)) into.global(str_utf8(n).c_str()) = module.getattr(n);
        return;
    }
    for (auto& k : m->order) {
        if (k.empty() || k[0] == '_') continue;
        CellObj* c = m->cells[k];
        if (!c->v.is_unbound()) into.global(k.c_str()) = c->v;
    }
}

// globals() / module.__dict__: a live mapping over the module's cells (what the translated code reads
// and writes), so globals()[name] = v and LANE.export(globals()) reach the module as in Python
struct ModDictObj : DataObj { var mod; };
static TypeObj* T_moddict = nullptr;
static ModuleObj* MD(const var& self) { return static_cast<ModuleObj*>(static_cast<ModDictObj*>(self.ref())->mod.ref()); }
static std::vector<std::string> mod_keys(ModuleObj* m) {
    std::vector<std::string> out;
    for (auto& k : m->order) {
        auto it = m->cells.find(k);
        if (it != m->cells.end() && !it->second->v.is_unbound()) out.push_back(k);
    }
    return out;
}
var module_dict_(const var& mod) {
    if (!T_moddict) {
        T_moddict = new_type("module_dict", {}, Layout::NativeData, false, "builtins");
        auto get = [](const CallArgs& a, bool must) -> var {
            ModuleObj* m = MD(a.pos.at(0));
            auto it = m->cells.find(utf8_of(a.pos.at(1)));
            if (it == m->cells.end() || it->second->v.is_unbound()) {
                if (must) raise(make_exc_args(E_KeyError, {a.pos[1]}));
                return a.pos.size() > 2 ? a.pos[2] : var(None);
            }
            return it->second->v;
        };
        add_method(T_moddict, "__getitem__", [get](const CallArgs& a) { return get(a, true); });
        add_method(T_moddict, "get", [get](const CallArgs& a) { return get(a, false); });
        add_method(T_moddict, "__setitem__", [](const CallArgs& a) { MD(a.pos.at(0))->cell(utf8_of(a.pos.at(1)))->v = a.pos.at(2); return None; });
        add_method(T_moddict, "__delitem__", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            auto it = m->cells.find(utf8_of(a.pos.at(1)));
            if (it == m->cells.end() || it->second->v.is_unbound()) raise(make_exc_args(E_KeyError, {a.pos[1]}));
            it->second->v = var();
            return None;
        });
        add_method(T_moddict, "__contains__", [](const CallArgs& a) {
            if (!is_str(a.pos.at(1))) return var(false);
            ModuleObj* m = MD(a.pos.at(0));
            auto it = m->cells.find(utf8_of(a.pos[1]));
            return var(it != m->cells.end() && !it->second->v.is_unbound());
        });
        add_method(T_moddict, "__len__", [](const CallArgs& a) { return var((int64_t)mod_keys(MD(a.pos.at(0))).size()); });
        add_method(T_moddict, "keys", [](const CallArgs& a) {
            std::vector<var> v;
            for (auto& k : mod_keys(MD(a.pos.at(0)))) v.push_back(new_str(k));
            return new_list(v);
        });
        add_method(T_moddict, "__iter__", [](const CallArgs& a) { return iter(a.pos.at(0).call("keys")); });
        add_method(T_moddict, "values", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            std::vector<var> v;
            for (auto& k : mod_keys(m)) v.push_back(m->cells[k]->v);
            return new_list(v);
        });
        add_method(T_moddict, "items", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            std::vector<var> v;
            for (auto& k : mod_keys(m)) v.push_back(new_tuple({new_str(k), m->cells[k]->v}));
            return new_list(v);
        });
        add_method(T_moddict, "copy", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            var d = new_dict_obj();
            for (auto& k : mod_keys(m)) static_cast<DictObj*>(d.ref())->set(new_str(k), m->cells[k]->v);
            return d;
        });
        add_method(T_moddict, "update", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            if (a.pos.size() > 1) {
                var o = a.pos[1];
                for (var k : each(hasattr(o, "keys"_s).truthy() ? o.call("keys") : o)) m->cell(utf8_of(k))->v = o.getitem(k);
            }
            for (auto& [k, v] : a.kw) m->cell(utf8_of(k))->v = v;
            return None;
        });
        add_method(T_moddict, "setdefault", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            CellObj* c = m->cell(utf8_of(a.pos.at(1)));
            if (c->v.is_unbound()) c->v = a.pos.size() > 2 ? a.pos[2] : var(None);
            return c->v;
        });
        add_method(T_moddict, "pop", [](const CallArgs& a) {
            ModuleObj* m = MD(a.pos.at(0));
            auto it = m->cells.find(utf8_of(a.pos.at(1)));
            if (it == m->cells.end() || it->second->v.is_unbound()) {
                if (a.pos.size() > 2) return a.pos[2];
                raise(make_exc_args(E_KeyError, {a.pos[1]}));
            }
            var v = it->second->v;
            it->second->v = var();
            return v;
        });
        add_method(T_moddict, "__repr__", [](const CallArgs& a) { return new_str(repr_(a.pos.at(0).call("copy"))); });
        add_method(T_moddict, "__eq__", [](const CallArgs& a) { return var(a.pos.at(0).call("copy") == a.pos.at(1)); });
    }
    auto* o = new ModDictObj();
    o->type = T_moddict;
    o->mod = mod;
    return var(static_cast<Obj*>(o));
}

var globals_of(module_def& m) {
    var mod = m.module();
    return module_dict_(mod);
}

bool module_registered_(const std::string& name) { return reg().defs.count(name) || reg().natives.count(name); }
// runpy.run_module: the module's statements run again with __name__ = run_name (a package: its
// __main__), its globals returned
var run_module_(const std::string& name_in, const std::string& run_name) {
    std::string name = name_in;
    auto d = reg().defs.find(name);
    if (d != reg().defs.end() && d->second->package_) d = reg().defs.find(name += ".__main__");
    if (d == reg().defs.end()) {
        var e = make_exc(E_ImportError, "No module named " + name);
        raise(e);
    }
    if (name.find('.') != std::string::npos) import(name.substr(0, name.rfind('.')).c_str());
    module_def* def = d->second;
    var saved = def->global("__name__");
    def->global("__name__") = new_str(run_name);
    var mod = def->module();
    auto* mo = static_cast<ModuleObj*>(mod.ref());
    mo->def = def;
    if (def->file_) mo->cell("__file__")->v = new_str(def->file_);
    try {
        def->init_();
    } catch (...) {
        def->global("__name__") = saved.is_unbound() ? new_str(name) : saved;
        throw;
    }
    var g = mod.getattr("__dict__");
    def->global("__name__") = saved.is_unbound() ? new_str(name) : saved;
    return g;
}

// a translated module's source file and whether it is a package (false: not translated, or native)
bool module_source_(const std::string& name, std::string& file, bool& package) {
    auto d = reg().defs.find(name);
    if (d == reg().defs.end() || !d->second->file_) return false;
    file = d->second->file_;
    package = d->second->package_;
    return true;
}

// a script path given as `python path.py`: the module registered for that file (compared as absolute,
// case-insensitive paths with either slash)
static std::string norm_path_(const std::string& p) {
    std::string a = p;
    char buf[4096];
    if (DWORD n = GetFullPathNameA(p.c_str(), sizeof buf, buf, nullptr); n > 0 && n < sizeof buf) a = buf;
    for (auto& c : a) c = c == '/' ? '\\' : (char)tolower((unsigned char)c);
    return a;
}
const char* module_for_file_(const std::string& path) {
    std::string want = norm_path_(path);
    for (auto& [name, def] : reg().defs)
        if (def->file_ && norm_path_(def->file_) == want) return name.c_str();
    return nullptr;
}

extern std::vector<std::string> g_argv;
int run_main(const char* module, int argc, char** argv) {
    init_runtime(argc, argv);
    auto& r = reg();
    // the command line as python's: interpreter flags skipped (-u -B -I -E -s -S -O, -X opt, -W opt),
    // then `-m module args`, `script.py args`, or (none) the program's own main module
    std::string name = module;
    size_t i = 1;
    static const std::set<std::string> flags = {"-u", "-B", "-I", "-E", "-s", "-S", "-O", "-OO", "-q", "-P"};
    while (i < g_argv.size()) {
        const std::string& f = g_argv[i];
        if (flags.count(f)) i++;
        else if (f == "-X" || f == "-W") i += 2;
        else if (f.size() > 2 && (f.compare(0, 2, "-X") == 0 || f.compare(0, 2, "-W") == 0)) i++;
        else break;
    }
    std::vector<std::string> rest;
    if (i + 1 < g_argv.size() && g_argv[i] == "-m") {
        name = g_argv[i + 1];
        rest.assign(g_argv.begin() + (long long)i + 2, g_argv.end());
    } else if (i < g_argv.size() && g_argv[i] == "-c") {
        fprintf(stderr, "-c: a translated program runs no source text\n");
        return 2;
    } else if (i < g_argv.size() && g_argv[i].size() > 3 && g_argv[i].compare(g_argv[i].size() - 3, 3, ".py") == 0) {
        const char* m = module_for_file_(g_argv[i]);
        if (!m) {
            fprintf(stderr, "%s: not a module of this program\n", g_argv[i].c_str());
            return 2;
        }
        name = m;
        rest.assign(g_argv.begin() + (long long)i + 1, g_argv.end());
    } else
        rest.assign(g_argv.begin() + (long long)(i < g_argv.size() ? i : g_argv.size()), g_argv.end());
    auto d = r.defs.find(name);
    if (d != r.defs.end() && d->second->package_) {       // python -m package: its __main__
        name += ".__main__";
        d = r.defs.find(name);
    }
    if (d == r.defs.end() && r.natives.count(name)) {
        // -m of a module the runtime has in C++ (pytest): its _main(argv)
        g_argv.clear();
        g_argv.push_back(name);
        g_argv.insert(g_argv.end(), rest.begin(), rest.end());
        extern void set_sys_argv_();
        set_sys_argv_();
        extern void flush_std_();
        try {
            var mod = import(name.c_str());
            var rc = mod.call("_main", import("sys").getattr("argv"));
            flush_std_();
            return rc.is_none() ? 0 : (int)rc.as_int64();
        } catch (error& e) {
            flush_std_();
            if (isinstance_(e.value, E_SystemExit)) {
                var code = e.value.getattr("code");
                return code.is_none() ? 0 : is_int(code) ? (int)code.as_int64() : 1;
            }
            extern void print_exception_(const var& e);
            print_exception_(e.value);
            return 1;
        }
    }
    if (d == r.defs.end()) {
        fprintf(stderr, "No module named %s\n", name.c_str());
        return 1;
    }
    g_argv.clear();
    g_argv.push_back(d->second->file_ ? d->second->file_ : name);
    g_argv.insert(g_argv.end(), rest.begin(), rest.end());
    extern void set_sys_argv_();
    set_sys_argv_();
    try {
        // python -m: the module runs as __main__ (its parents imported first)
        if (name.find('.') != std::string::npos) import(name.substr(0, name.rfind('.')).c_str());
        module_def* def = d->second;
        def->global("__name__") = new_str("__main__");
        var mod = def->module();
        auto* mo = static_cast<ModuleObj*>(mod.ref());
        mo->def = def;
        if (def->file_) mo->cell("__file__")->v = new_str(def->file_);
        std::string pkg = name.rfind('.') == std::string::npos ? "" : name.substr(0, name.rfind('.'));
        mo->cell("__package__")->v = new_str(pkg);
        if (mo->cell("__doc__")->v.is_unbound()) mo->cell("__doc__")->v = None;
        sysmods()->set(new_str("__main__"), mod);
        def->init_();
    } catch (error& e) {
        extern void flush_std_();
        if (isinstance_(e.value, E_SystemExit)) {
            flush_std_();
            var code = e.value.getattr("code");
            if (code.is_none()) return 0;
            if (is_int(code)) return (int)code.as_int64();
            fprintf(stderr, "%s\n", str_(code).c_str());
            fflush(stderr);
            return 1;
        }
        extern void print_exception_(const var& e);
        flush_std_();
        print_exception_(e.value);
        return 1;
    }
    extern void flush_std_();
    flush_std_();
    return 0;
}

}  // namespace py
