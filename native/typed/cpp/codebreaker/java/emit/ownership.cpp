// codebreaker/java/emit/ownership.cpp -- typed C++ port of codebreaker.java.emit.ownership (see ownership.hpp).

#include "codebreaker/java/emit/ownership.hpp"

#include "codebreaker/java/typemap.hpp"

namespace codebreaker::java::emit::ownership {

namespace {

using py::str;
using ir::JType;

str S(const char* s) { return str(s); }

// '\n'.join(t for _ln, t in body)
str join_body(const py::list<std::tuple<int64_t, str>>& body) {
    py::list<str> parts;
    for (const auto& ln_t : body) {
        parts.append(std::get<1>(ln_t));
    }
    return py::join(S("\n"), parts);
}

}  // namespace

const py::re::Pattern _AGG_ARRAY = py::re::compile(S(R"RX(^new\s+[\w.]+\s*\[\s*\]\s*\{(.*)\}\s*$)RX"), py::re::S);

py::set<py::str> nullable_value_params(const MethodRef& method, const TypeResolver& resolver) {
    str text = join_body(method->body);
    py::set<str> out;
    for (const py::ref<ir::Param>& p : method->params) {
        const py::ref<JType>& jt = p->jtype;
        if (jt->is_array() || resolver.is_pointer(jt) || typemap::PRIMITIVES.contains(jt->name) ||
            typemap::VALUE_STDLIB.contains(jt->name) || typemap::CONTAINERS.contains(jt->name)) {
            continue;
        }
        str nm = py::re::escape(p->name);
        if (py::re::search(S("\\b") + nm + S("\\s*(?:==|!=)\\s*(?:null|NULL)\\b"), text) ||
            py::re::search(S("\\b(?:null|NULL)\\s*(?:==|!=)\\s*") + nm + S("\\b"), text) ||
            py::re::search(S("\\b") + nm + S("\\s*=\\s*(?:null|NULL)\\b"), text)) {
            out.add(p->name);
        }
    }
    return out;
}

py::set<py::str> _reassigned_params(const MethodRef& method) {
    str body = join_body(method->body);
    py::set<str> out;
    for (const py::ref<ir::Param>& p : method->params) {
        if (py::re::search(S("(?<![\\w.])") + py::re::escape(p->name) + S("\\b\\s*(?:[-+*/%|&^]?=(?!=)|\\+\\+|--)"), body)) {
            out.add(p->name);
        }
    }
    return out;
}

py::str params_str(const MethodRef& method, const TypeResolver& resolver) {
    py::set<str> nullable = nullable_value_params(method, resolver);
    py::set<str> reassigned = _reassigned_params(method);
    py::list<str> parts;
    for (const py::ref<ir::Param>& p : method->params) {
        str t = resolver.param_type(p->jtype);
        if (nullable.contains(p->name) && !t.endswith(S("*"))) {
            t += S("*");
        } else if (reassigned.contains(p->name) && t.startswith(S("const ")) && t.endswith(S("&"))) {
            t = t.slice(py::len(S("const ")), -1).strip();  // const X& -> X: a reassigned param is by-value & mutable
        }
        // Java collection out-param (pipeline.compute_collection_out_params): pass the value-struct
        // container by reference so the callee's add()/remove()/put() reach the caller's collection.
        if (p->by_ref && !t.endswith(S("*")) && !t.endswith(S("&"))) {
            t += S("&");
        }
        parts.append(py::percent(S("%s %s"), t, p->name));
    }
    return py::join(S(", "), parts);
}

std::optional<py::str> aggregate_array_body(const py::ref<ir::Field>& field) {
    str init = py::truthy(field->initializer) ? *field->initializer : S("");
    init = init.strip();
    std::optional<py::re::Match> m = _AGG_ARRAY.match(init);
    if (m) {
        return m->group(1);
    }
    return std::nullopt;
}

const py::re::Pattern _ASSIGN_STMT = py::re::compile(S(R"RX((?:this\s*(?:->|\.)\s*)?([A-Za-z_]\w*)\s*=\s*([^;]+);)RX"));
const py::re::Pattern _RHS_NEW = py::re::compile(S(R"RX(^new\s+[A-Za-z_][\w:.]*\s*(\[)?)RX"));
const py::re::Pattern _RHS_FACTORY =
    py::re::compile(S(R"RX(^(?:[A-Za-z_]\w*\s*(?:::|\.)\s*)+(?:create|make|alloc|clone|copyOf)[A-Za-z]\w*\s*\()RX"));
const py::re::Pattern _RHS_NEUTRAL = py::re::compile(S(R"RX(^(?:null|NULL|nullptr|0)$)RX"));
const py::re::Pattern _ADD_CALL =
    py::re::compile(S(R"RX(([A-Za-z_]\w*)\s*(?:->|\.)\s*(?:push_back|add|addElement|append)\s*\(\s*(.*?)\)\s*;)RX"));
const py::re::Pattern _ELEM_SET = py::re::compile(S(R"RX(([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=\s*([^;]+);)RX"));

bool _is_alloc_rhs(const py::str& rhs) {
    return bool(_RHS_NEW.match(rhs)) || bool(_RHS_FACTORY.match(rhs));
}

py::str _body_text(const MethodRef& method) {
    return py::re::sub(S("//[^\\n]*"), S(""), join_body(method->body));
}

py::list<py::str> _method_texts(const ClassRef& cls) {
    py::list<str> out;
    for (const MethodRef& m : cls->methods) {
        out.append(_body_text(m));
    }
    return out;
}

py::dict<py::str, bool> owned_pointers(const ClassRef& cls, const TypeResolver& resolver) {
    py::dict<str, bool> alloc;
    py::set<str> borrowed;

    auto classify = [&](const str& name, const str& rhs) {
        std::optional<py::re::Match> mnew = _RHS_NEW.match(rhs);
        if (mnew) {
            std::optional<str> g1 = mnew->group_opt(1);
            alloc.set(name, py::truthy(g1) || alloc.get(name, false));
        } else if (_RHS_FACTORY.match(rhs)) {
            alloc.setdefault(name, false);
        } else if (_RHS_NEUTRAL.match(rhs)) {
            // = NULL: neutral, delete-of-NULL is safe
        } else {
            borrowed.add(name);  // param / alias / unknown getter -> borrowed
        }
    };

    // field-declaration initializers (`Timer timer = new Timer(60F);`, `= BufferUtils.createIntBuffer(16);`):
    // the emitter moves these into the constructor, but they live on Field.initializer, NOT in any scanned
    // method body -- so without this the destructor silently misses them (the RetroMCP-style leak).
    for (const py::ref<ir::Field>& f : cls->fields) {
        if (!f->is_static && py::truthy(f->initializer) && resolver.is_pointer(f->jtype)) {
            classify(f->name, f->initializer->strip());
        }
    }
    // method-body assignments
    for (const str& text : _method_texts(cls)) {
        for (const py::re::Match& am : _ASSIGN_STMT.finditer(text)) {
            py::ref<ir::Field> fld = cls->field(am.group(1));
            if (fld && !fld->is_static && resolver.is_pointer(fld->jtype)) {
                classify(am.group(1), am.group(2).strip());
            }
        }
    }
    py::dict<str, bool> out;
    for (const auto& kv : alloc.items()) {
        if (!borrowed.contains(std::get<0>(kv))) {
            out.set(std::get<0>(kv), std::get<1>(kv));
        }
    }
    return out;
}

py::dict<py::str, py::str> owned_containers(const ClassRef& cls, const TypeResolver& resolver) {
    py::set<str> add_alloc, add_borrow;
    for (const str& text : _method_texts(cls)) {
        // `c.add(x)` / `c[i] = x` -- own iff x is a fresh alloc
        for (const py::re::Pattern* pat : {&_ADD_CALL, &_ELEM_SET}) {
            for (const py::re::Match& m : pat->finditer(text)) {
                (_is_alloc_rhs(m.group(2).strip()) ? add_alloc : add_borrow).add(m.group(1));
            }
        }
    }
    py::dict<str, str> out;
    for (const str& name : add_alloc) {
        if (add_borrow.contains(name)) {
            continue;
        }
        py::ref<ir::Field> fld = cls->field(name);
        if (!fld || fld->is_static) {
            continue;
        }
        const py::ref<JType>& jt = fld->jtype;
        if (jt->is_array() && resolver.is_pointer(jt->element(jt))) {
            out.set(name, S(".length"));
        } else if (typemap::CONTAINERS.contains(jt->name) && py::truthy(jt->args) && resolver.is_pointer(jt->args[0])) {
            out.set(name, S(".size()"));
        }
    }
    return out;
}

bool owns_heap(const ClassRef& cls, const TypeResolver& resolver) {
    return py::truthy(owned_pointers(cls, resolver)) || py::truthy(owned_containers(cls, resolver));
}

bool _returns_container(const py::ref<ir::JType>& rt) {
    // Pre-generics Minecraft returns RAW containers (`ArrayList`, no <T>), so the element type is NOT on
    // the return signature -- it comes from the `new T` adds and the caller's `JList<T*>` decl. So the gate
    // is just "returns a container type"; ownership is proven by every add being a fresh allocation, and the
    // pointer-ness by the caller-side guard only firing on a `JList<T*>` local.
    if (!rt || rt->name == S("void")) {
        return false;
    }
    return typemap::CONTAINERS.contains(rt->name) || rt->is_array();
}

bool returns_owned_container(const MethodRef& method, const TypeResolver& resolver) {
    (void)resolver;
    if (!_returns_container(method->return_type)) {
        return false;
    }
    str text = _body_text(method);
    py::set<str> returned = py::set<str>::from(py::re::findall(S(R"RX(\breturn\s+([A-Za-z_]\w*)\s*;)RX"), text));
    if (returned.empty()) {
        return false;
    }
    for (const str& var : returned) {
        str v = py::re::escape(var);
        // freshly constructed local container: `var = new ArrayList()` or generic `new ArrayList<T>()`
        if (!py::re::search(S("\\b") + v + S("\\s*=\\s*new\\s+[A-Za-z_][\\w.]*\\s*(?:<[^>]*>)?\\s*\\("), text)) {
            return false;
        }
        py::list<str> adds = py::re::findall(S("\\b") + v + S("\\s*\\.\\s*(?:add|addElement|push_back|append)\\s*\\(\\s*(.*?)\\)\\s*;"), text);
        if (adds.empty()) {
            return false;
        }
        for (const str& a : adds) {
            if (!_is_alloc_rhs(a.strip())) {
                return false;  // no adds, or a borrowed element added
            }
        }
    }
    return true;
}

py::set<py::str> owned_return_container_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver) {
    py::set<str> owned, ambiguous;
    for (const ClassRef& c : classes) {
        for (const MethodRef& m : c->methods) {
            if (m->is_ctor || !_returns_container(m->return_type)) {
                continue;
            }
            (returns_owned_container(m, resolver) ? owned : ambiguous).add(m->name);
        }
    }
    return owned - ambiguous;
}

bool _returns_scalar_pointer(const MethodRef& method, const TypeResolver& resolver) {
    const py::ref<JType>& rt = method->return_type;
    if (!rt || rt->name == S("void") || rt->is_array() || typemap::CONTAINERS.contains(rt->name)) {
        return false;
    }
    return resolver.is_pointer(rt) || returns_pointer(method, resolver);
}

bool returns_owned_scalar(const MethodRef& method, const TypeResolver& resolver) {
    if (!_returns_scalar_pointer(method, resolver)) {
        return false;
    }
    py::list<str> rets = py::re::findall(S(R"RX(\breturn\b([^;]*);)RX"), _body_text(method));
    if (rets.empty()) {
        return false;
    }
    bool saw_new = false;
    for (const str& r : rets) {
        str e = r.strip();
        if (e == S("") || _RHS_NEUTRAL.match(e)) {
            continue;  // bare / null / 0 return -> temp may be null; delete NULL is safe
        }
        if (_RHS_NEW.match(e)) {
            saw_new = true;
        } else {
            return false;  // returns a field/param/other -> not caller-owned
        }
    }
    return saw_new;
}

py::dict<py::str, py::str> owned_scalar_return_methods(const py::list<ClassRef>& classes, const TypeResolver& resolver) {
    py::dict<str, str> owned;
    py::set<str> ambiguous;
    for (const ClassRef& c : classes) {
        for (const MethodRef& m : c->methods) {
            if (m->is_ctor || !_returns_scalar_pointer(m, resolver)) {
                continue;
            }
            if (returns_owned_scalar(m, resolver)) {
                str elem = return_cpp_type(m, resolver).rstrip(S("*")).strip();
                if (owned.get(m->name, elem) != elem) {
                    ambiguous.add(m->name);
                }
                owned.set(m->name, elem);
            } else {
                ambiguous.add(m->name);
            }
        }
    }
    py::dict<str, str> out;
    for (const auto& kv : owned.items()) {
        if (!ambiguous.contains(std::get<0>(kv))) {
            out.set(std::get<0>(kv), std::get<1>(kv));
        }
    }
    return out;
}

bool _param_retained(const py::str& name, const py::str& text) {
    str v = py::re::escape(name);
    for (const py::re::Match& mo : py::re::finditer(S("\\b") + v + S("\\b"), text)) {
        if (!py::re::match(S("\\s*[.\\[]"), text.slice_from(mo.end()))) {
            return true;
        }
    }
    return false;
}

py::dict<py::str, py::set<int64_t>> retained_param_positions(const py::list<ClassRef>& classes) {
    py::dict<str, py::set<int64_t>> retained;
    for (const ClassRef& c : classes) {
        for (const MethodRef& m : c->methods) {
            if (m->is_ctor) {
                continue;
            }
            str text = _body_text(m);
            int64_t brace = text.find(S("{"));  // drop the signature line -- the param name in
            if (brace >= 0) {                    // `f(T p)` is a decl, not a retention use
                text = text.slice_from(brace + 1);
            }
            int64_t i = 0;
            for (const py::ref<ir::Param>& p : m->params) {
                if (_param_retained(p->name, text)) {
                    retained.setdefault(m->name, py::set<int64_t>()).add(i);
                }
                i += 1;
            }
        }
    }
    return retained;
}

bool returns_pointer(const MethodRef& method, const TypeResolver& resolver) {
    const py::ref<JType>& rt = method->return_type;
    if (!rt || rt->name == S("void")) {
        return false;
    }
    // arrays are by-value JArray<T> (their own null handling: `return null` -> empty JArray).
    // Never promote to pointer, or `return theField;` (a value JArray) can't convert.
    if (rt->is_array()) {
        return false;
    }
    // collections are by-value (JList/JMap/JSet), with their own null handling (`= null` / `return
    // null` -> empty via the long-long ctor) exactly like arrays. NEVER promote a container return to
    // a pointer: a sibling method / local / field of the SAME interface type is a value, and a promoted
    // pointer return can't interconvert with it (the StatsSyncher/StatFileWriter JMap*<->JMap collisions).
    if (typemap::CONTAINERS.contains(rt->name)) {
        return false;
    }
    if (resolver.is_pointer(rt)) {
        return true;
    }
    // std::string (and other value-stdlib types) is NEVER a pointer: it has dedicated
    // by-value handling (return-by-value, `return null` -> empty string). Promoting it
    // both reintroduces the dangling-pointer risk and breaks virtual-override return
    // unification (base `return null` -> string*, derived `return "x"` -> string).
    if (typemap::VALUE_STDLIB.contains(rt->name)) {
        return false;
    }
    str text = join_body(method->body);
    // value-type return promoted to pointer if the body new's it in a return (incl. a
    // ternary) OR returns null -- both impossible for a by-value return
    if (py::re::search(S("\\breturn\\b[^;]*\\bnew\\s+") + py::re::escape(rt->name) + S("\\b"), text)) {
        return true;
    }
    if (py::re::search(S("\\breturn\\s+(?:null|NULL)\\b"), text)) {
        return true;
    }
    return false;
}

py::str return_cpp_type(const MethodRef& method, const TypeResolver& resolver) {
    if (!method->return_type || method->return_type->name == S("void")) {
        return S("void");
    }
    str rt = resolver.return_type(method->return_type);
    if (returns_pointer(method, resolver) && !rt.endswith(S("*")) && !rt.endswith(S("&"))) {
        rt += S("*");
    }
    return rt;
}

}  // namespace codebreaker::java::emit::ownership
