// module.cc -- modules and import: every translated module registers a module_def at static
// initialisation; import runs its init once (sys.modules first, the parents before the child, a
// partially initialised module visible to circular imports as in CPython); the library modules
// (os, re, json ...) are native and made on first import.
#include <cstring>
#include <map>

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

var globals_of(module_def& m) {
    var mod = m.module();
    return mod.getattr("__dict__");
}

bool module_registered_(const std::string& name) { return reg().defs.count(name) || reg().natives.count(name); }

int run_main(const char* module, int argc, char** argv) {
    init_runtime(argc, argv);
    auto& r = reg();
    auto d = r.defs.find(module);
    if (d == r.defs.end()) {
        fprintf(stderr, "no module named %s\n", module);
        return 1;
    }
    try {
        // python -m: the module runs as __main__ (its parents imported first)
        std::string name = module;
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
        if (isinstance_(e.value, E_SystemExit)) {
            var code = e.value.getattr("code");
            if (code.is_none()) return 0;
            if (is_int(code)) return (int)code.as_int64();
            fprintf(stderr, "%s\n", str_(code).c_str());
            fflush(stderr);
            return 1;
        }
        extern void print_exception_(const var& e);
        print_exception_(e.value);
        return 1;
    }
    extern void flush_std_();
    flush_std_();
    return 0;
}

}  // namespace py
