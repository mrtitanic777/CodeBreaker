// translate/backends/c/tree.cpp -- X: a node of the typed tree, or a part of one, read by the names its document gives it (the
// slots through l2::slot, a type's and a symbol's members as l2::type_to_json and sym_to_json write them), and the helpers the
// writer and the passes share.
#include <string>
#include <type_traits>
#include <variant>

#include "translate/backends/c/ctree.hpp"
#include "translate/l2.hpp"

namespace cb::translate::ctree {

namespace {

[[noreturn]] void type_error(const char* what) { py::raise("TypeError", what); }

X present_str(std::string_view s, bool& present) {
    present = true;
    return X::str(s);
}

std::string_view segment_text(const ir::SegmentRef&) { return "segment"; }

// A slot's value as the document writes it (present: whether the writer writes the key at all).
X slot_value(const l2::SlotPtr& p, bool& present) {
    present = false;
    return std::visit(
        [&](auto v) -> X {
            using A = decltype(v);
            using P = std::remove_cv_t<std::remove_pointer_t<A>>;
            if constexpr (std::is_same_v<A, std::monostate>) {
                return X();
            } else if constexpr (std::is_same_v<P, ir::Node*>) {
                if (*v == nullptr) return X();
                present = true;
                return X::node(*v);
            } else if constexpr (std::is_same_v<P, std::vector<ir::Node*>>) {
                present = true;   // (a list slot the walk follows: written even when empty)
                return X::nodes(v);
            } else if constexpr (std::is_same_v<P, ir::Suite>) {
                if (auto* one = std::get_if<ir::Node*>(v)) {
                    if (*one == nullptr) return X();
                    present = true;
                    return X::node(*one);
                }
                present = true;
                return X::nodes(&std::get<std::vector<ir::Node*>>(*v));
            } else if constexpr (std::is_same_v<P, ir::TypeP>) {
                if (!*v) return X();
                present = true;
                return X::type(*v);
            } else if constexpr (std::is_same_v<P, std::vector<ir::TypeP>>) {
                if (v->empty()) return X();
                present = true;
                return X();   // (a list of types: no slot of the C back end's documents reads one)
            } else if constexpr (std::is_same_v<P, ir::SymP>) {
                if (!*v) return X();
                present = true;
                return X::sym(v->get());
            } else if constexpr (std::is_same_v<P, std::optional<std::string>>) {
                if (!*v) return X();
                present = true;
                return X::str(**v);
            } else if constexpr (std::is_same_v<P, std::vector<std::string>>) {
                if (v->empty()) return X();
                present = true;
                return X();   // (found() answers these: mods, comments ...)
            } else if constexpr (std::is_same_v<P, bool>) {
                if (!*v) return X();
                present = true;
                return X::boolean(true);
            } else if constexpr (std::is_same_v<P, std::optional<bool>>) {
                if (!*v) return X();
                present = true;
                return X::boolean(**v);
            } else if constexpr (std::is_same_v<P, std::optional<std::int64_t>>) {
                if (!*v) return X();
                present = true;
                return X::integer(**v);
            } else if constexpr (std::is_same_v<P, std::optional<std::uint64_t>>) {
                if (!*v) return X();
                present = true;
                return X::unsigned_integer(**v);
            } else if constexpr (std::is_same_v<P, std::uint32_t>) {
                present = true;
                return X::integer(*v);
            } else if constexpr (std::is_same_v<P, std::optional<ir::Overflow>>) {
                if (!*v) return X();
                return present_str(ir::to_text(**v), present);
            } else if constexpr (std::is_same_v<P, std::optional<ir::MemoryModel>>) {
                if (!*v) return X();
                return present_str(ir::to_text(**v), present);
            } else if constexpr (std::is_same_v<P, ir::LitValue>) {
                return std::visit(
                    [&](const auto& x) -> X {
                        using L = std::decay_t<decltype(x)>;
                        if constexpr (std::is_same_v<L, std::monostate>) return X();
                        else {
                            present = true;
                            if constexpr (std::is_same_v<L, bool>) return X::boolean(x);
                            else if constexpr (std::is_same_v<L, std::int64_t>) return X::integer(x);
                            else if constexpr (std::is_same_v<L, std::uint64_t>) return X::unsigned_integer(x);
                            else if constexpr (std::is_same_v<L, double>) return X::real(x);
                            else return X::str(x);
                        }
                    },
                    *v);
            } else if constexpr (std::is_same_v<P, std::string>) {
                present = true;
                return X::str(*v);
            } else {
                return X();   // (type arguments, a compiler target: no C document reads them)
            }
        },
        p);
}

}  // namespace

// (the lists of strings a node or a type holds, and the other parts, are made here: the union's members are private)
X X::found(std::string_view key, bool& present) const {
    present = false;
    X out;
    switch (t_) {
        case T::Node: {
            if (key == "node") {
                present = true;
                return X::str(ir::kind_name(*n_));
            }
            if (key == "line" || key == "col") {
                const std::uint32_t v = key == "line" ? n_->line : n_->col;
                if (v == 0) return X();
                present = true;
                return X::integer(v);
            }
            if (key == "origin") {
                if (!n_->origin) return X();
                present = true;
                out.t_ = T::Origin;
                out.origin_ = n_->origin.get();
                return out;
            }
            const l2::SlotPtr p = l2::slot(*n_, key);
            if (auto* texts = std::get_if<std::vector<std::string>*>(&p)) {   // (mods, comments ...)
                if ((*texts)->empty()) return X();
                present = true;
                out.t_ = T::Texts;
                out.texts_ = *texts;
                return out;
            }
            return slot_value(p, present);
        }
        case T::Type: {
            const ir::Type& t = *ty_;
            if (key == "kind") return present_str(t.kind(), present);
            return std::visit(
                [&](const auto& b) -> X {
                    using B = std::decay_t<decltype(b)>;
                    if constexpr (std::is_same_v<B, ir::PrimType>) {
                        if (key == "signed" && b.is_signed) {
                            present = true;
                            return X::boolean(*b.is_signed);
                        }
                        if (key == "enc" && b.encoding) return present_str("enc", present);
                    } else if constexpr (std::is_same_v<B, ir::ClassType>) {
                        if (key == "name") return present_str(b.name, present);
                        if (key == "nullable" && b.nullable) {
                            present = true;
                            return X::boolean(true);
                        }
                    } else if constexpr (std::is_same_v<B, ir::TypeVarType>) {
                        if (key == "name") return present_str(b.name, present);
                    } else if constexpr (std::is_same_v<B, ir::WildcardType>) {
                        if (key == "type" && b.bound) {
                            present = true;
                            return X::type(b.bound->type);
                        }
                    } else if constexpr (std::is_same_v<B, ir::ArrayType>) {
                        if (key == "elem") {
                            present = true;
                            return X::type(b.elem);
                        }
                        if (key == "count" && b.count) {
                            present = true;
                            return X::unsigned_integer(*b.count);
                        }
                        if (key == "rank" && b.rank != 1) {
                            present = true;
                            return X::integer(b.rank);
                        }
                    } else if constexpr (std::is_same_v<B, ir::PtrType>) {
                        if (key == "space") return present_str(ir::to_text(b.space), present);
                        if (key == "to") {
                            present = true;
                            return X::type(b.to);
                        }
                        if (key == "seg" && b.seg) return present_str(segment_text(*b.seg), present);
                        if (key == "ref" && b.ref) {
                            present = true;
                            return X::boolean(true);
                        }
                    } else if constexpr (std::is_same_v<B, ir::SelectorType>) {
                        if (key == "of" && b.of) return present_str(segment_text(*b.of), present);
                    } else if constexpr (std::is_same_v<B, ir::FnType>) {
                        if (key == "conv") return present_str(ir::to_text(b.conv), present);
                        if (key == "params") {
                            present = true;
                            out.t_ = T::Params;
                            out.params_ = &b.params;
                            return out;
                        }
                        if (key == "result") {
                            present = true;
                            out.t_ = T::Result;
                            out.res_ = &b.result;
                            return out;
                        }
                        if (key == "distance" && b.distance) return present_str(ir::to_text(*b.distance), present);
                        if (key == "pops" && b.pops) {
                            present = true;
                            return X::integer(*b.pops);
                        }
                        if (key == "loadds" && b.loadds) {
                            present = true;
                            return X::boolean(true);
                        }
                        if (key == "preserves" && !b.preserves.empty()) {
                            present = true;
                            out.t_ = T::Texts;
                            out.texts_ = &b.preserves;
                            return out;
                        }
                        if (key == "variadic" && b.variadic) {
                            present = true;
                            return X::boolean(true);
                        }
                    } else if constexpr (std::is_same_v<B, ir::FnPtrType>) {
                        if (key == "space") return present_str(ir::to_text(b.space), present);
                        if (key == "fn") {
                            present = true;
                            return X::type(b.fn);
                        }
                    } else if constexpr (std::is_same_v<B, ir::RecordType>) {
                        if (key == "size") {
                            present = true;
                            return X::unsigned_integer(b.size);
                        }
                        if (key == "fields") {
                            present = true;
                            out.t_ = T::Fields;
                            out.fields_ = &b.fields;
                            return out;
                        }
                        if (key == "name" && b.name && !b.name->empty()) return present_str(*b.name, present);
                        if (key == "packed" && b.packed) {
                            present = true;
                            return X::boolean(*b.packed);
                        }
                        if (key == "role" && b.role) return present_str("role", present);
                    }
                    return X();
                },
                t.body);
        }
        case T::Sym: {
            const ir::Sym& s = *s_;
            if (key == "kind") return present_str(ir::to_text(s.kind), present);
            if (key == "name") return present_str(s.name, present);
            if (key == "owner" && s.owner) return present_str(*s.owner, present);
            if (key == "static" && s.is_static) {
                present = true;
                return X::boolean(true);
            }
            if (key == "type" && s.type) {
                present = true;
                return X::type(s.type);
            }
            if (key == "params" && (!s.params.empty() || s.kind == ir::SymKind::Method || s.kind == ir::SymKind::Ctor || s.kind == ir::SymKind::Function)) {
                present = true;
                out.t_ = T::Types;
                out.tys_ = &s.params;
                return out;
            }
            if (key == "ret" && s.ret) {
                present = true;
                return X::type(s.ret);
            }
            if (key == "varargs" && s.varargs) {
                present = true;
                return X::boolean(true);
            }
            if (key == "flags" && !s.flags.empty()) {
                present = true;
                out.t_ = T::Texts;
                out.texts_ = &s.flags;
                return out;
            }
            return X();
        }
        case T::Param: {
            if (key == "name" && param_->name) return present_str(*param_->name, present);
            if (key == "type") {
                present = true;
                return X::type(param_->type);
            }
            if (key == "at" && !std::holds_alternative<ir::NoLocation>(param_->at)) {
                present = true;
                return X();
            }
            return X();
        }
        case T::Result: {
            if (key == "type" && res_->type) {
                present = true;
                return X::type(res_->type);
            }
            if (key == "at") present = true;
            return X();
        }
        case T::Field: {
            if (key == "name") return present_str(field_->name, present);
            if (key == "offset") {
                present = true;
                return X::unsigned_integer(field_->offset);
            }
            if (key == "type") {
                present = true;
                return X::type(field_->type);
            }
            if (key == "pad" && field_->pad) {
                present = true;
                return X::boolean(true);
            }
            return X();
        }
        case T::Origin: {
            return std::visit(
                [&](const auto& o) -> X {
                    using O = std::decay_t<decltype(o)>;
                    if constexpr (std::is_same_v<O, ir::OriginSegment>) {
                        if (key == "module") return present_str(o.module, present);
                        if (key == "segment") {
                            present = true;
                            return X::integer(o.segment);
                        }
                        if (key == "offset") {
                            present = true;
                            return X::integer(o.offset);
                        }
                    } else if constexpr (std::is_same_v<O, ir::OriginVa>) {
                        if (key == "module") return present_str(o.module, present);
                        if (key == "va") {
                            present = true;
                            return X::unsigned_integer(o.va);
                        }
                    } else {
                        if (key == "at") present = true;
                    }
                    return X();
                },
                *origin_);
        }
        default:
            return X();   // (not a dict: d.get answers None)
    }
}

X X::get(std::string_view key) const {
    bool present = false;
    return found(key, present);
}

X X::at(std::string_view key) const {
    bool present = false;
    X v = found(key, present);
    if (!present) py::raise("KeyError", py::str_repr(std::string(key)));
    return v;
}

bool X::has(std::string_view key) const {
    bool present = false;
    found(key, present);
    return present;
}

std::string X::str() const {
    if (t_ != T::Str) type_error("a str was expected");
    return std::string(sv_);
}

std::int64_t X::as_int() const {
    if (t_ == T::Int) return i_;
    if (t_ == T::UInt) return static_cast<std::int64_t>(u_);
    if (t_ == T::Bool) return b_ ? 1 : 0;
    type_error("an int was expected");
}

std::size_t X::size() const {
    switch (t_) {
        case T::Nodes: {
            std::size_t n = 0;
            for (const ir::Node* x : *nodes_) n += x != nullptr;
            return n;
        }
        case T::Types: return tys_->size();
        case T::Params: return params_->size();
        case T::Fields: return fields_->size();
        case T::Texts: return texts_->size();
        case T::Seq: return seq_->size();
        case T::Str: return cp_len(std::string(sv_));
        default: type_error("object has no len()");
    }
}

std::vector<X> X::seq() const {
    std::vector<X> out;
    switch (t_) {
        case T::Nodes:
            out.reserve(nodes_->size());
            for (ir::Node* x : *nodes_)
                if (x != nullptr) out.push_back(X::node(x));
            break;
        case T::Types:
            for (const ir::TypeP& x : *tys_) out.push_back(X::type(x));
            break;
        case T::Params:
            for (const ir::FnParam& p : *params_) {
                X x;
                x.t_ = T::Param;
                x.param_ = &p;
                out.push_back(x);
            }
            break;
        case T::Fields:
            for (const ir::RecordField& f : *fields_) {
                X x;
                x.t_ = T::Field;
                x.field_ = &f;
                out.push_back(x);
            }
            break;
        case T::Texts:
            for (const std::string& s : *texts_) out.push_back(X::str(s));
            break;
        case T::Seq: out = *seq_; break;
        default: type_error("object is not iterable");
    }
    return out;
}

X X::operator[](std::size_t n) const {
    if (t_ == T::Nodes) {
        std::size_t k = 0;
        for (ir::Node* x : *nodes_) {
            if (x == nullptr) continue;
            if (k++ == n) return X::node(x);
        }
        py::raise("IndexError", "list index out of range");
    }
    std::vector<X> all = seq();
    if (n >= all.size()) py::raise("IndexError", "list index out of range");
    return all[n];
}

Id X::id() const {
    switch (t_) {
        case T::Node: return n_;
        case T::Nodes: return nodes_;
        case T::Type: return ty_;
        case T::Types: return tys_;
        case T::Sym: return s_;
        case T::Params: return params_;
        case T::Param: return param_;
        case T::Result: return res_;
        case T::Fields: return fields_;
        case T::Field: return field_;
        case T::Texts: return texts_;
        case T::Origin: return origin_;
        case T::Seq: return seq_;
        case T::Str: return sv_.data();
        default: return nullptr;
    }
}

V to_v(const X& x) {
    switch (x.kind()) {
        case X::T::None: return V();
        case X::T::Bool: return V(x.b());
        case X::T::Int: return V(static_cast<long long>(x.as_int()));
        case X::T::UInt: return V(static_cast<unsigned long long>(x.as_int()));
        case X::T::Float: return V(x.as_float());
        case X::T::Str: return V(x.str());
        default: py::raise("TypeError", "a part of the tree is not a value");
    }
}

// ---------------------------------------------------------------------------------------------- helpers
V sym_name_v(const X& e) {
    const X sym = e.get("sym");
    if (!py::truthy(sym)) return V();
    return to_v(sym.get("name"));
}
std::string sym_name(const X& e) {
    const X sym = e.get("sym");
    if (!py::truthy(sym)) return std::string();
    const X n = sym.get("name");
    return n.is_str() ? n.str() : std::string();
}
std::string name_of(const X& e) {
    const X sym = e.get("sym");
    if (py::truthy(sym)) {
        const X n = sym.get("name");
        if (py::truthy(n)) return py::str_of(n);
    }
    const X n2 = e.get("name");
    if (py::truthy(n2)) return py::str_of(n2);
    return "?";
}
std::string param_name(const X& p) {
    const X sym = p.get("sym");
    if (py::truthy(sym)) {
        const X n = sym.get("name");
        if (py::truthy(n)) return py::str_of(n);
    }
    if (p.has("name")) {
        const X x = p.get("name");
        return x.is_none() ? std::string() : py::str_of(x);
    }
    return "";
}
std::vector<X> stmts(const X& s) {
    if (s.is_none()) return {};
    if (s.is_list()) return s.seq();
    return {s};
}
bool same(const X& a, const X& b) {
    if (a.is_none() || b.is_none()) return a.is_none() && b.is_none();
    return a.id() == b.id();
}
bool in(const std::set<std::string>& s, const std::string& x) { return s.count(x) != 0; }
bool in(const IdSet& s, Id x) { return s.count(x) != 0; }

std::string fmt(const char* f, std::initializer_list<std::string> args) {
    std::string out;
    auto it = args.begin();
    for (const char* p = f; *p; p++) {
        if (p[0] == '%' && p[1] == 's') {
            out += *it++;
            p++;
        } else if (p[0] == '%' && p[1] == '%') {
            out += '%';
            p++;
        } else
            out += *p;
    }
    return out;
}

bool field_sym(const X& e) {
    const X s = e.get("sym");
    return py::truthy(s) && s.get("kind") == "field";
}
X dict_or_empty(const X& v) { return py::truthy(v) ? v : X(); }   // (None's get answers None, as {}'s did)
std::string node_kind(const X& e) {
    const X n = e.get("node");
    return n.is_str() ? n.str() : std::string();
}
bool is_ident(const std::string& s) {
    static const re::Regex R("[A-Za-z_]\\w*");
    return (bool)R.fullmatch(s);
}

}  // namespace cb::translate::ctree

namespace py {

using cb::translate::ctree::X;

bool truthy(const X& v) {
    switch (v.kind()) {
        case X::T::None: return false;
        case X::T::Bool: return v.b();
        case X::T::Int: return v.as_int() != 0;
        case X::T::UInt: return true;
        case X::T::Float: return v.as_float() != 0.0;
        case X::T::Str: return !v.view().empty();
        default:
            if (v.is_list()) return v.size() != 0;
            return true;   // (a node, a type, a symbol: a dict with keys)
    }
}

std::string str_of(const X& v) {
    switch (v.kind()) {
        case X::T::None: return "None";
        case X::T::Bool: return v.b() ? "True" : "False";
        case X::T::Int: return std::to_string(v.as_int());
        case X::T::UInt: return std::to_string(static_cast<std::uint64_t>(v.as_int()));
        case X::T::Float: return float_repr(v.as_float());
        case X::T::Str: return v.str();
        default: raise("TypeError", "str() of a part of the tree");
    }
}

std::string repr(const X& v) { return v.is_str() ? str_repr(v.str()) : str_of(v); }

bool eq(const X& a, const V& b) {
    if (a.is_none() || a.is_intlike() || a.is_float() || a.is_str()) return eq(cb::translate::ctree::to_v(a), b);
    return false;
}

}  // namespace py
