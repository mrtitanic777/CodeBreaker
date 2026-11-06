// lib_async.cc -- what asyncio and the front ends need of the runtime beyond the coroutines themselves
// (gen.cc): sys's async generator hooks and coroutine origin tracking, and the registration of the C
// halves of the networking modules (lib_net.cc: _socket, select, _overlapped, _winapi).
#include "rt.hpp"

namespace py {

extern var g_sys;
TypeObj* T(const var& v);                    // object.cc
var set_asyncgen_hooks_(const CallArgs& a);   // gen.cc
var get_asyncgen_hooks_();                    // gen.cc
void init_net_modules();                      // lib_net.cc
void init_mcp();                              // lib_mcp.cc
void init_tui();                              // lib_tui.cc, lib_rich.cc

var annotate_(const var& f, std::initializer_list<const char*> name_text) {
    if (!f.is_ref() || f.ref()->type != T_function) return f;
    var d = new_dict_obj();
    for (auto it = name_text.begin(); it != name_text.end() && it + 1 != name_text.end(); it += 2) d.setitem(new_str(*it), new_str(*(it + 1)));
    static_cast<FuncObj*>(f.ref())->annotations = d;
    return f;
}

var at_line_(const var& f, int line) {
    if (f.is_ref() && f.ref()->type == T_function) static_cast<FuncObj*>(f.ref())->firstlineno = line;
    return f;
}

var doc_(const var& f, const char* doc) {
    if (f.is_ref() && f.ref()->type == T_function) static_cast<FuncObj*>(f.ref())->doc = new_str(doc);
    return f;
}

// f.__code__: what code reads of a code object without running it -- co_name, co_qualname, co_filename (the
// module's file), co_firstlineno (0: not kept), co_flags (CO_GENERATOR / CO_COROUTINE / CO_ASYNC_GENERATOR,
// CO_VARARGS / CO_VARKEYWORDS), co_argcount, co_kwonlyargcount, co_posonlyargcount, co_varnames
var code_of_(const var& fv) {
    static var code_cls;
    if (code_cls.is_unbound()) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "builtins"_s);
        ns.setitem("__repr__"_s, def("__repr__", "self", {}, [](var self) {
            return new_str("<code object " + str_utf8(self.getattr("co_name")) + ", file " + repr_(self.getattr("co_filename")) + ">");
        }));
        code_cls = make_class("code", "code", {}, ns, {}, "builtins");
    }
    auto* f = static_cast<FuncObj*>(fv.ref());
    var c = call0(code_cls);
    c.setattr("co_name", new_str(f->name));
    c.setattr("co_qualname", new_str(f->qualname));
    std::string file;
    var* m = static_cast<DictObj*>(import("sys").getattr("modules").ref())->get(new_str(f->module));
    if (m) {
        var fl = getattr(*m, "__file__"_s, None);
        if (is_str(fl)) file = str_utf8(fl);
    }
    c.setattr("co_filename", new_str(file));
    c.setattr("co_firstlineno", var((int64_t)f->firstlineno));
    int64_t flags = 0x1 | 0x2;   // CO_OPTIMIZED | CO_NEWLOCALS
    if (f->varargs >= 0) flags |= 0x4;
    if (f->varkw >= 0) flags |= 0x8;
    if (f->async_kind == 1) flags |= 0x80;
    else if (f->async_kind == 2) flags |= 0x200;
    else if (f->async_kind == 3) flags |= 0x20 | 0x100;
    else if (f->generator) flags |= 0x20;
    c.setattr("co_flags", var(flags));
    int64_t argc = 0, kwonly = 0, posonly = 0;
    std::vector<var> names;
    for (auto& p : f->params) {
        names.push_back(new_str(p.name));
        if (p.kind == Param::PosOnly) posonly++, argc++;
        else if (p.kind == Param::Normal) argc++;
        else if (p.kind == Param::KwOnly) kwonly++;
    }
    c.setattr("co_argcount", var(argc));
    c.setattr("co_kwonlyargcount", var(kwonly));
    c.setattr("co_posonlyargcount", var(posonly));
    c.setattr("co_varnames", new_tuple(names));
    return c;
}

void init_async_modules() {
    static int origin_depth = 0;
    mod_def(g_sys, "set_asyncgen_hooks", [](const CallArgs& a) { return set_asyncgen_hooks_(a); });
    mod_def(g_sys, "get_asyncgen_hooks", [](const CallArgs&) { return get_asyncgen_hooks_(); });
    mod_def(g_sys, "get_coroutine_origin_tracking_depth", [](const CallArgs&) { return var(origin_depth); });
    mod_def(g_sys, "set_coroutine_origin_tracking_depth", [](const CallArgs& a) {
        origin_depth = (int)arg(a, 0, "depth").as_int64();
        return None;
    });
    // builtins the async and networking code names: the other warning categories, exception groups, aiter / anext
    extern var g_builtins;
    auto sub = [](const char* name, const var& base, var ns = var()) {
        if (ns.is_unbound()) ns = new_dict();
        var c = make_class(name, name, {base}, ns, {}, "builtins");
        mod_set(g_builtins, name, c);
        return c;
    };
    for (const char* w : {"FutureWarning", "PendingDeprecationWarning", "ResourceWarning", "SyntaxWarning", "ImportWarning", "EncodingWarning",
                          "UnicodeWarning", "BytesWarning"})
        sub(w, t::Warning);
    {
        // BaseExceptionGroup(message, exceptions): .message, .exceptions, derive / subgroup / split
        var ns = new_dict();
        ns.setitem("__init__"_s, def("__init__", "self, message, exceptions", {}, [](var self, var message, var excs) {
            if (!is_str(message)) raise(E_TypeError, "argument 1 must be str, not " + tname(message));
            std::vector<var> xs = collect(excs);
            if (xs.empty()) raise(E_ValueError, "second argument (exceptions) must be a non-empty sequence");
            self.setattr("args", new_tuple({message, new_tuple(xs)}));
            self.setattr("message", message);
            self.setattr("exceptions", new_tuple(xs));
            return var(None);
        }));
        ns.setitem("__str__"_s, def("__str__", "self", {}, [](var self) {
            int64_t n = index_(len(self.getattr("exceptions")));
            return new_str(str_utf8(self.getattr("message")) + " (" + std::to_string(n) + " sub-exception" + (n == 1 ? "" : "s") + ")");
        }));
        ns.setitem("derive"_s, def("derive", "self, excs", {}, [](var self, var excs) { return call_obj(tv(type_of(self)), pack_args_(self.getattr("message"), excs)); }));
        auto pick = [](var self, var cond, bool keep) -> var {
            std::vector<var> got;
            for (var e : each(self.getattr("exceptions"))) {
                bool hit = (cond.is_ref() && cond.ref()->type->layout == Layout::Type) || is_tuple(cond) ? isinstance(e, cond).truthy() : call1(cond, e).truthy();
                if (hit == keep) got.push_back(e);
            }
            if (got.empty()) return None;
            return self.call("derive", new_list(got));
        };
        ns.setitem("subgroup"_s, def("subgroup", "self, condition", {}, [pick](var self, var cond) { return pick(self, cond, true); }));
        ns.setitem("split"_s, def("split", "self, condition", {}, [pick](var self, var cond) { return new_tuple({pick(self, cond, true), pick(self, cond, false)}); }));
        var beg = sub("BaseExceptionGroup", t::BaseException, ns);
        var ns2 = new_dict();
        var eg = make_class("ExceptionGroup", "ExceptionGroup", {beg, t::Exception}, ns2, {}, "builtins");
        mod_set(g_builtins, "ExceptionGroup", eg);
    }
    mod_def(g_builtins, "aiter", [](const CallArgs& a) { return aiter_(a.pos.at(0)); });
    // anext(it[, default]): it.__anext__(), or an awaitable giving default at StopAsyncIteration
    mod_def(g_builtins, "anext", [](const CallArgs& a) -> var {
        var it = a.pos.at(0);
        var aw = it.call("__anext__");
        if (a.pos.size() < 2) return aw;
        var dflt = a.pos[1];
        return def_coroutine("anext", "", {}, [aw, dflt](yielder& y) -> var {
            try {
                return y.await_(aw);
            } catch (error& e) {
                if (isinstance_(e.value, T(t::StopAsyncIteration))) return dflt;
                throw;
            }
        })();
    });
    // importlib.metadata.version(name): a distribution's version from a <name>-<version>.dist-info folder on
    // sys.path, else a pyproject.toml on sys.path whose [project] is that name (what an editable install's
    // metadata says); PackageNotFoundError otherwise
    register_native_module("importlib.metadata", [](const var& m) {
        var ns = new_dict();
        var pnf = make_class("PackageNotFoundError", "PackageNotFoundError", {t::ModuleNotFoundError}, ns, {}, "importlib.metadata");
        mod_set(m, "PackageNotFoundError", pnf);
        mod_def(m, "version", [pnf](const CallArgs& a) -> var {
            std::string want = str_utf8(a.pos.at(0));
            auto norm = [](std::string s) {
                for (auto& c : s) c = c == '-' || c == '.' ? '_' : (char)tolower((unsigned char)c);
                return s;
            };
            std::string w = norm(want);
            var os = import("os");
            // (sys.path, then the current folder: what `python -m` puts first on sys.path, which this program's is not)
            var dirs = new_list(collect(import("sys").getattr("path")));
            dirs.call("append", os.call("getcwd"));
            for (var d : each(dirs)) {
                if (!is_str(d)) continue;
                std::string dir = str_utf8(d).empty() ? "." : str_utf8(d);
                if (!os.attr("path").call("isdir", new_str(dir)).truthy()) continue;
                for (var e : each(os.call("listdir", new_str(dir)))) {
                    std::string n = str_utf8(e);
                    if (n.size() > 10 && n.compare(n.size() - 10, 10, ".dist-info") == 0) {
                        std::string stem = n.substr(0, n.size() - 10);
                        size_t dash = stem.find('-');
                        if (dash != std::string::npos && norm(stem.substr(0, dash)) == w) return new_str(stem.substr(dash + 1));
                    }
                }
                var pp = os.attr("path").call("join", new_str(dir), "pyproject.toml"_s);
                if (os.attr("path").call("isfile", pp).truthy()) {
                    try {
                        var f = import("builtins").getattr("open")(pp, "rb"_s);
                        var doc = import("tomllib").call("load", f);
                        f.call("close");
                        var proj = doc.call("get", "project"_s, new_dict_obj());
                        var nm = proj.call("get", "name"_s), ver = proj.call("get", "version"_s);
                        if (is_str(nm) && norm(str_utf8(nm)) == w && is_str(ver)) return ver;
                    } catch (error&) {}
                }
            }
            throw error(make_exc_args(T(pnf), {new_str(want)}));
        });
    });
    init_net_modules();
    init_mcp();
    init_tui();
}

}  // namespace py
