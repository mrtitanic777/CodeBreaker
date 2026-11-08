// codebreaker/translate/backends/java_libc.cpp -- typed C++ port of codebreaker.translate.backends.java_libc
// (see java_libc.hpp).
#include "codebreaker/translate/backends/java_libc.hpp"

namespace codebreaker::translate::backends::java_libc {

namespace ir = codebreaker::translate::ir;
using jb::J;
using jb::NodeP;
using jb::Refused;
using py::str;

namespace {

template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

std::shared_ptr<codebreaker::java::apimap::Registry>& reg_slot() {
    static std::shared_ptr<codebreaker::java::apimap::Registry> r;   // _REG
    return r;
}

py::ref<ir::Lit> _string_literal(const NodeP& a0) {
    NodeP a = jb::strip(a0);
    if (auto c = as_a<ir::Cast>(a); c && c->x) {
        NodeP x = jb::strip(c->x);
        if (auto l = as_a<ir::Lit>(x); l && std::holds_alternative<str>(l->value.v)) return l;
    }
    return nullptr;
}

bool _char_ptr(const ir::TypeP& t) {
    if (!(t && t->kind == "ptr" && t->elem)) return false;
    str k = jb::_kind(t->elem);
    return k == "i8" || k == "u8" || k == "u16";
}

ir::TypeP expr_type(const NodeP& a) {
    if (!a) throw py::AttributeError("'NoneType' object has no attribute 'type'");
    auto x = as_a<ir::Expr>(a);
    if (!x) throw py::AttributeError("'" + a->class_name().utf8() + "' object has no attribute 'type'");
    return x->type;
}

// A function pointer given to the library (qsort's comparator): a Java functional value.
str _callback(jb::FnWriter& w, const NodeP& a, const ir::TypeP& pt) {
    NodeP x = jb::strip(a);
    if (auto c = as_a<ir::Cast>(x)) x = jb::strip(c->x);
    if (auto ao = as_a<ir::AddressOf>(x)) {
        if (!ao->sym) throw py::AttributeError("'NoneType' object has no attribute 'name'");
        if (w.p->functions.contains(ao->sym->name)) return w.p->name + "::" + jb::jname(ao->sym->name);
    }
    jb::Sig sig = w.p->fn_sig_of_type(pt);
    str iface = w.p->iface(sig);
    static const char* all[] = {"a", "b", "c", "d"};
    py::list<str> names;
    for (size_t i = 0; i < sig.params.size() && i < 4; ++i) names.append(str(all[i]));
    str f = w.value(a).code;
    str ns = py::join(str(", "), names);
    return str("(") + ns + ") -> ((" + iface + ") Fn.at(" + f + ")).call(" + ns + ")";
}

}  // namespace

std::shared_ptr<codebreaker::java::apimap::Registry> registry() {
    if (!reg_slot()) reg_slot() = codebreaker::java::apimap::load();
    return reg_slot();
}

J call(jb::FnWriter& w, const py::ref<ir::Call>& e, const str& name, const py::list<NodeP>& args) {
    if (name == "__acrt_iob_func") {                         // MSVC's stdin/stdout/stderr
        return J(str("stream(") + w.value(args[0]).code + ")", jb::P_POST, str("int"));
    }
    auto m = registry()->resolve(name, str("c"), str("java"));
    if (!m) throw Refused(str("library function ") + name + ": not in the C runtime for Java (api_maps/libc.json)");
    if (m->kind == "unsupported") throw Refused(str("library function ") + name + ": " + (m->note.empty() ? str("not carried") : m->note));
    str target = m->target;
    if (target.startswith(str("crt.LibC."))) target = target.slice_from(str("crt.LibC.").size());
    if (!e->sym) throw py::AttributeError("'NoneType' object has no attribute 'params'");
    py::list<ir::TypeP> params = e->sym->params;
    py::list<str> out;
    for (int64_t i = 0; i < args.size(); ++i) {
        const NodeP& a = args[i];
        ir::TypeP pt = i < params.size() ? params[i] : nullptr;
        auto lit = _string_literal(a);
        if (lit && (!pt || _char_ptr(pt))) {
            out.append(jb::string_lit(lit));
            continue;
        }
        if (pt && pt->kind == "fnptr") {
            out.append(_callback(w, a, pt));
            continue;
        }
        if (!pt) {                                           // a variadic argument: as C passed it
            J v = w.as_value(w.value(a), expr_type(a));
            out.append(v.code);
        } else {
            out.append(w.conv(w.value(a), expr_type(a), pt).code);
        }
    }
    str jt = e->type ? w.p->jt(e->type) : str("void");
    return J(target + "(" + py::join(str(", "), out) + ")", jb::P_POST, jt);
}

}  // namespace codebreaker::translate::backends::java_libc
