// translate/frontends/python/infer_expr.cpp -- expressions run on types: constants, f-strings, names, attributes,
// subscripts and slices, and the dispatch to the files that hold the rest (operators, displays, calls).
#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/frontends/python/infer_util.hpp"
#include "translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

using EK = syntax::ExprKind;

// Does `rest` read as [sign][#][0][width][,][.precision][type]?
bool is_format_tail(std::string_view rest) {
    std::size_t i = 0;
    const auto digits = [&] {
        const std::size_t start = i;
        while (i < rest.size() && rest[i] >= '0' && rest[i] <= '9') ++i;
        return i > start;
    };
    if (i < rest.size() && (rest[i] == '+' || rest[i] == '-' || rest[i] == ' ')) ++i;
    if (i < rest.size() && rest[i] == '#') ++i;
    digits();   // the zero flag and the width
    if (i < rest.size() && rest[i] == ',') ++i;
    if (i < rest.size() && rest[i] == '.') {
        ++i;
        if (!digits()) return false;
    }
    if (i < rest.size() && std::string_view("bdoxXcsfeEg%").find(rest[i]) != std::string_view::npos) ++i;
    return i == rest.size();
}

// Is the text a format spec the front end can write: [[fill]align][sign][#][0][width][,][.precision][type]?
bool is_format_spec(std::string_view spec) {
    const auto is_align = [](char c) { return c == '<' || c == '>' || c == '^'; };
    if (is_format_tail(spec)) return true;
    if (!spec.empty() && is_align(spec[0]) && is_format_tail(spec.substr(1))) return true;
    return spec.size() >= 2 && is_align(spec[1]) && is_format_tail(spec.substr(2));
}

const Type* find_attr(const std::unordered_map<std::string, Type>& table, const std::string& name) {
    const auto it = table.find(name);
    return it == table.end() ? nullptr : &it->second;
}

}  // namespace

Type Eval::ev(const syntax::Expr* node, const State& state) {
    TR r = e_dispatch(node, state);
    if (r.res && syntax::as<syntax::Call>(node) != nullptr && (r.res->kind == ResKind::Fn || r.res->kind == ResKind::Method)) {
        res_quiet[node] = *r.res;
    }
    return note(node, r.t, r.res);
}

TR Eval::e_dispatch(const syntax::Expr* n, const State& state) {
    switch (n->kind) {
        case EK::Constant: return e_constant(syntax::cast<syntax::Constant>(n));
        case EK::JoinedStr: return e_joined_str(syntax::cast<syntax::JoinedStr>(n), state);
        case EK::FormattedValue: return TR(ev(syntax::cast<syntax::FormattedValue>(n)->value, state));
        case EK::Name: return e_name(syntax::cast<syntax::Name>(n), state);
        case EK::Attribute: return e_attribute(syntax::cast<syntax::Attribute>(n), state);
        case EK::Subscript: return e_subscript(syntax::cast<syntax::Subscript>(n), state);
        case EK::BinOp: {
            const auto* b = syntax::cast<syntax::BinOp>(n);
            const Type l = ev(b->left, state);
            const Type r = ev(b->right, state);
            return TR(binop(n, b->op, l, r, b->left, b->right));
        }
        case EK::UnaryOp: return e_unary_op(syntax::cast<syntax::UnaryOp>(n), state);
        case EK::BoolOp: return e_bool_op(syntax::cast<syntax::BoolOp>(n), state);
        case EK::Compare: return e_compare(syntax::cast<syntax::Compare>(n), state);
        case EK::IfExp: return e_if_exp(syntax::cast<syntax::IfExp>(n), state);
        case EK::NamedExpr:
            ev(syntax::cast<syntax::NamedExpr>(n)->value, state);
            return TR(issue(n, IssueClass::Syntax, "walrus"));
        case EK::Lambda: return TR(issue(n, IssueClass::Dynamic, "first-class lambda"));
        case EK::Starred:
            ev(syntax::cast<syntax::Starred>(n)->value, state);
            return TR(issue(n, IssueClass::Syntax, "starred"));
        case EK::Await: return TR(issue(n, IssueClass::Dynamic, "await"));
        case EK::Yield: {
            const syntax::Expr* v = syntax::cast<syntax::Yield>(n)->value;
            const Type item = v != nullptr ? ev(v, state) : P::none();
            if (live()) yielded = p.join(yielded, item);
            return TR(P::none());
        }
        case EK::YieldFrom:
            if (const syntax::Expr* v = syntax::cast<syntax::YieldFrom>(n)->value) ev(v, state);
            return TR(P::bottom());
        case EK::Tuple: return e_tuple(syntax::cast<syntax::Tuple>(n), state);
        case EK::List: return e_list(syntax::cast<syntax::List>(n), state);
        case EK::Set: return e_set(syntax::cast<syntax::Set>(n), state);
        case EK::Dict: return e_dict(syntax::cast<syntax::Dict>(n), state);
        case EK::ListComp: {
            const auto* c = syntax::cast<syntax::ListComp>(n);
            const State scope = comp_scope(c->generators, state);
            const program::SiteId site = p.new_site(n, program::Container::List, where(n));
            p.grow(program::Table::Elem, site, ev(c->elt, scope));
            return TR(site_type(Kind::List, site));
        }
        case EK::SetComp: {
            const auto* c = syntax::cast<syntax::SetComp>(n);
            const State scope = comp_scope(c->generators, state);
            const program::SiteId site = p.new_site(n, program::Container::Set, where(n));
            p.grow(program::Table::Elem, site, ev(c->elt, scope));
            return TR(site_type(Kind::Set, site));
        }
        case EK::DictComp: {
            const auto* c = syntax::cast<syntax::DictComp>(n);
            const State scope = comp_scope(c->generators, state);
            const program::SiteId site = p.new_site(n, program::Container::Dict, where(n));
            p.make_dynamic(site, "a comprehension");
            p.grow(program::Table::Keys, site, ev(c->key, scope));
            p.grow(program::Table::Elem, site, ev(c->value, scope));
            return TR(site_type(Kind::Dict, site));
        }
        case EK::GeneratorExp: {
            const auto* c = syntax::cast<syntax::GeneratorExp>(n);
            const State scope = comp_scope(c->generators, state);
            return TR(P::iter_of(ev(c->elt, scope), P::mem()));
        }
        case EK::Call: return e_call(syntax::cast<syntax::Call>(n), state);
        default:
            // any other expression (a slice outside a subscript, a t-string ...): the front end does not lower it
            return TR(issue(n, IssueClass::Syntax, text::lower(syntax::name_of(n->kind))));
    }
}

TR Eval::e_constant(const syntax::Constant* n) {
    using CK = syntax::ConstValue::Kind;
    const syntax::ConstValue& v = n->value;
    switch (v.kind) {
        case CK::None: return TR(P::none());
        case CK::True:
        case CK::False: return TR(P::boolean());
        case CK::Int: return TR(P::const_int(v.fits_i64 ? Wide(v.i64) : Wide::from_decimal(v.text).value_or(Wide(0))));
        case CK::Float: return TR(P::floating());
        case CK::Str: return TR(P::str());
        case CK::Bytes: return TR(P::bytes_of(Wide(static_cast<std::int64_t>(v.text.size()))));
        case CK::Complex: return TR(issue(n, IssueClass::Syntax, "constant complex"));
        case CK::Ellipsis: return TR(issue(n, IssueClass::Syntax, "constant ellipsis"));
    }
    return TR(P::bottom());
}

TR Eval::e_joined_str(const syntax::JoinedStr* n, const State& state) {
    for (const syntax::Expr* part : n->values) {
        const auto* v = syntax::as<syntax::FormattedValue>(part);
        if (v == nullptr) continue;
        const Type t = ev(v->value, state);
        if (v->format_spec != nullptr) {
            // the spec is a constant string unless it has a replacement field of its own
            std::optional<std::string> spec = std::string();
            if (const auto* spec_text = syntax::as<syntax::JoinedStr>(v->format_spec)) {
                for (const syntax::Expr* x : spec_text->values) {
                    const auto* constant = syntax::as<syntax::Constant>(x);
                    if (constant == nullptr) {
                        spec = std::nullopt;
                        break;
                    }
                    *spec += constant->value.text;
                }
            }
            if (!spec) {
                issue(v, IssueClass::Dynamic, "computed format spec");
            } else {
                check_format(v, t, *spec);
            }
        } else if (v->conversion != -1 && v->conversion != 'r' && v->conversion != 's') {
            issue(v, IssueClass::Syntax, std::format("f-string conversion !{}", static_cast<char>(v->conversion)));
        } else {
            check_str(v, t, v->conversion == 'r');
        }
    }
    return TR(P::str());
}

void Eval::check_str(const syntax::Node* node, const Type& t, bool as_repr) {
    if (is_one_of(t, {Kind::Int, Kind::Str, Kind::Bool, Kind::None, Kind::Bytes, Kind::Bytearray, Kind::Bottom, Kind::Top,
                      Kind::Float})) {
        return;
    }
    if (t.kind == Kind::Opt && is_one_of(t.args[0], {Kind::Int, Kind::Str, Kind::Bool})) return;
    if (is_one_of(t, {Kind::Tuple, Kind::List}) && as_repr) return;
    issue(node, IssueClass::Syntax, "str() of a " + kind_text(t));
}

void Eval::check_format(const syntax::Node* node, const Type& t, const std::string& spec) {
    if (!is_format_spec(spec)) {
        issue(node, IssueClass::Syntax, "format spec '" + spec + "'");
        return;
    }
    // an int formats as a float when the spec ends in a float type (an empty spec ends in none)
    const bool floaty = spec.empty() || std::string_view("fFeEg%").find(spec.back()) != std::string_view::npos;
    if (is_one_of(t, {Kind::Int, Kind::Bool}) && floaty) return;
    if (!is_one_of(t, {Kind::Int, Kind::Str, Kind::Float, Kind::Bool, Kind::Bottom, Kind::Top})) {
        issue(node, IssueClass::Syntax, "format of a " + kind_text(t));
    }
}

TR Eval::e_name(const syntax::Name* n, const State& state) {
    if (const Type* t = state.find(n->id)) return TR(*t, Res::of(ResKind::Local));
    if (locals.contains(n->id)) return TR(P::bottom(), Res::of(ResKind::Local));   // read before any assignment reaches here
    const std::optional<Binding> binding = lookup(n->id);
    if (!binding) return TR(issue(n, IssueClass::Unresolved, n->id));
    return TR(binding_type(n, *binding), binding->resolution());
}

// What a name means here: a local import, a name of the module, or what the language provides.
std::optional<Binding> Eval::lookup(const std::string& name) {
    if (const Ref* imported = local_imports.find(name)) return Binding{Binding::Source::Program, *imported, {}};
    if (m != nullptr) {
        if (std::optional<Ref> r = p.resolve_global(*m, name)) return Binding{Binding::Source::Program, *r, {}};
    }
    if (builtin_functions().contains(name)) return Binding{Binding::Source::Builtin, {}, name};
    if (program::is_builtin_exception(name)) return Binding{Binding::Source::Exception, {}, name};
    if (word_in(name, {"True", "False", "None", "__name__", "__file__", "__doc__"})) {
        return Binding{Binding::Source::Constant, {}, name};
    }
    return std::nullopt;
}

std::optional<std::string> Eval::lookup_class(const syntax::Expr* n) {
    if (const auto* name = syntax::as<syntax::Name>(n)) {
        const std::optional<Binding> b = lookup(name->id);
        if (b && b->source == Binding::Source::Program && b->ref.kind == Ref::Kind::Class) return b->ref.name;
    }
    return std::nullopt;
}

Type Eval::binding_type(const syntax::Node* n, const Binding& b) {
    switch (b.source) {
        case Binding::Source::Builtin: return named_type(Kind::Fn, std::string(kBuiltinPrefix) + b.name);
        case Binding::Source::Exception: return named_type(Kind::Cls, std::string(kExcPrefix) + b.name);
        case Binding::Source::Constant:
            if (b.name == "__doc__") return m->has_docstring() ? P::str() : P::none();
            if (b.name == "__file__" || b.name == "__name__") return issue(n, IssueClass::Dynamic, b.name);
            return P::bottom();
        case Binding::Source::Program: break;
    }
    const Ref& r = b.ref;
    switch (r.kind) {
        case Ref::Kind::Global: return p.global_type(r.name, r.member);
        case Ref::Kind::Function: return named_type(Kind::Fn, r.name);
        case Ref::Kind::Class: return named_type(Kind::Cls, r.name);
        case Ref::Kind::Module: return named_type(Kind::Module, r.name);
        case Ref::Kind::StdModule: return named_type(Kind::Module, std::string(kStdPrefix) + r.name);
        case Ref::Kind::StdObject: return named_type(Kind::Extern, std::string(kStdPrefix) + r.name);
        case Ref::Kind::Extern: return issue(n, IssueClass::Extern, first_part(r.name));
        case Ref::Kind::Missing: return issue(n, IssueClass::Unresolved, r.name);
        case Ref::Kind::Member: break;
    }
    return issue(n, IssueClass::Unresolved, r.name + "." + r.member);
}

TR Eval::e_attribute(const syntax::Attribute* n, const State& state) {
    const Type owner = ev(n->value, state);
    const std::string& attr = n->attr;
    switch (owner.kind) {
        case Kind::Bottom:
        case Kind::Top: return TR(owner);
        case Kind::Inst: {
            const std::string& class_name = *owner.name;
            if (Fn* f = p.find_method(class_name, attr)) {
                if (f->kind == FnKind::Property) return TR(p.ret(f->qname), Res::of(ResKind::Property, f->qname));
                return TR(named_type(Kind::Fn, std::string(kBoundPrefix) + f->qname), Res::of(ResKind::Bound, f->qname));
            }
            const std::optional<Type> t = p.attr_type(class_name, attr);
            if (!t) return TR(issue(n, IssueClass::Unresolved, last_part(class_name) + "." + attr));
            return TR(*t, Res::of(ResKind::Field, p.attr_owner(class_name, attr).value_or(class_name), attr));
        }
        case Kind::Module: {
            if (owner.name->starts_with(kStdPrefix)) {
                const std::string full = owner.name->substr(kStdPrefix.size()) + "." + attr;
                if (const Type* known = find_attr(std_attrs(), full)) return TR(*known, Res::of(ResKind::StdAttr, full));
                return TR(named_type(Kind::Extern, std::string(kStdPrefix) + full), Res::of(ResKind::StdAttr, full));
            }
            Module& module = *p.modules.at(*owner.name);
            const std::optional<Ref> r = p.resolve_global(module, attr);
            if (!r) {
                const std::string full = *owner.name + "." + attr;
                if (p.modules.contains(full)) return TR(named_type(Kind::Module, full), Res::of(ResKind::Module, full));
                return TR(issue(n, IssueClass::Unresolved, full));
            }
            const Binding b{Binding::Source::Program, *r, {}};
            return TR(binding_type(n, b), b.resolution());
        }
        case Kind::Cls: {
            if (owner.name->starts_with(kExcPrefix)) return TR(issue(n, IssueClass::Dynamic, "exception class attribute"));
            if (Fn* f = p.find_method(*owner.name, attr)) {
                if (f->kind == FnKind::Static) return TR(named_type(Kind::Fn, f->qname), Res::of(ResKind::Fn, f->qname));
                return TR(issue(n, IssueClass::Dynamic, "unbound method"));
            }
            for (const std::string& class_name : p.mro(*owner.name)) {
                if (p.classes.at(class_name)->class_attrs.contains(attr)) {
                    return TR(p.class_attr_type(class_name, attr), Res::of(ResKind::ClassAttr, class_name, attr));
                }
            }
            return TR(issue(n, IssueClass::Unresolved, p.classes.at(*owner.name)->name + "." + attr));
        }
        case Kind::Extern: {
            if (owner.name->starts_with(kStdPrefix)) {
                const std::string full = owner.name->substr(kStdPrefix.size()) + "." + attr;
                if (const Type* known = find_attr(std_attrs(), full)) return TR(*known, Res::of(ResKind::StdAttr, full));
                return TR(named_type(Kind::Extern, std::string(kStdPrefix) + full), Res::of(ResKind::StdAttr, full));
            }
            return TR(owner);
        }
        case Kind::Str:
        case Kind::Bytes:
        case Kind::Bytearray:
        case Kind::List:
        case Kind::Dict:
        case Kind::Set:
        case Kind::Int:
        case Kind::Float:
        case Kind::Tuple: {
            const std::string method = kind_text(owner) + "." + attr;
            return TR(named_type(Kind::Fn, std::string(kBMethodPrefix) + method), Res::of(ResKind::BMethod, method));
        }
        case Kind::Opt: return TR(issue(n, IssueClass::Conflict, "attribute ." + attr + " of a value that may be None"));
        default: break;
    }
    return TR(issue(n, IssueClass::Conflict, "attribute ." + attr + " of a " + kind_text(owner)));
}

TR Eval::e_subscript(const syntax::Subscript* n, const State& state) {
    const Type owner = ev(n->value, state);
    const Kind k = owner.kind;
    if (const auto* slice = syntax::as<syntax::Slice>(n->slice)) {
        const std::vector<Type> parts = ev_slice_parts(slice, state);
        const syntax::Expr* exprs[3] = {slice->lower, slice->upper, slice->step};
        for (std::size_t i = 0; i < 3; ++i) {
            if (exprs[i] != nullptr && !is_one_of(parts[i], {Kind::Int, Kind::Bottom, Kind::Top, Kind::None})) {
                issue(exprs[i], IssueClass::Conflict, "slice bound of " + kind_text(parts[i]));
            }
        }
        if (slice->step != nullptr) {
            const std::optional<Wide> step = int_constant(slice->step);
            if (!step || *step == Wide(0)) issue(n, IssueClass::Syntax, "slice with a computed step");
        }
        switch (k) {
            case Kind::Bytes:
            case Kind::Bytearray: return TR(P::bytes_of(slice_cap(slice, owner.hi), k), Res::of(ResKind::Slice, kind_text(owner)));
            case Kind::Str: return TR(P::str(), Res::of(ResKind::Slice, kind_text(owner)));
            case Kind::List: {
                const program::SiteId site = p.new_site(n, program::Container::List, where(n));
                p.grow(program::Table::Elem, site, p.root(owner.site.value()).elem);
                return TR(site_type(Kind::List, site), Res::of(ResKind::Slice, "list"));
            }
            case Kind::Tuple: {
                const std::optional<Wide> from = int_constant(slice->lower);
                const std::optional<Wide> to = int_constant(slice->upper);
                if (slice->step == nullptr && (slice->lower == nullptr || from) && (slice->upper == nullptr || to)) {
                    Res r = Res::of(ResKind::TupleSlice);
                    r.lo = from;
                    r.hi = to;
                    return TR(P::tuple_of(tuple_slice(owner.args, from, to)), r);
                }
                return TR(issue(n, IssueClass::Conflict, "computed slice of a tuple"));
            }
            case Kind::Bottom:
            case Kind::Top: return TR(owner);
            default: return TR(issue(n, IssueClass::Conflict, "slice of a " + kind_text(owner)));
        }
    }
    const Type index = ev(n->slice, state);
    switch (k) {
        case Kind::Bytes:
        case Kind::Bytearray:
            need_int_index(n->slice, index);
            return TR(P::unsigned_int(8), Res::of(ResKind::Index, kind_text(owner)));
        case Kind::Str:
            need_int_index(n->slice, index);
            return TR(P::str(), Res::of(ResKind::Index, kind_text(owner)));
        case Kind::List:
            need_int_index(n->slice, index);
            return TR(p.root(owner.site.value()).elem, Res::of(ResKind::Index, kind_text(owner)));
        case Kind::Tuple: {
            const std::optional<Wide> at = int_constant(n->slice);
            if (!at) {
                Type element = P::bottom();
                for (const Type& a : owner.args) element = p.join(element, a);
                if (element.kind == Kind::Top) {
                    return TR(issue(n, IssueClass::Conflict, "computed index into a tuple of mixed items"));
                }
                need_int_index(n->slice, index);
                return TR(element, Res::of(ResKind::TupleIndex));
            }
            const Wide length(static_cast<std::int64_t>(owner.args.size()));
            if (!(-length <= *at && *at < length)) {
                return TR(issue(n, IssueClass::Conflict, "tuple index " + at->to_string() + " out of range"));
            }
            const std::int64_t ix = at->to_i64().value();
            Res r = Res::of(ResKind::TupleIndex);
            r.lo = at;
            return TR(owner.args[static_cast<std::size_t>(ix < 0 ? ix + static_cast<std::int64_t>(owner.args.size()) : ix)], r);
        }
        case Kind::Dict: {
            const program::SiteId root = p.find(owner.site.value());
            const std::optional<std::string> key = pyast::string_constant(n->slice);
            if (key && !p.site(root).dynamic) {
                const Type* field = p.site(root).fields.find(*key);
                return TR(field != nullptr ? *field : P::bottom(), Res::of(ResKind::FieldGet, *key));
            }
            if (!key) p.make_dynamic(root, "a key that is not a constant string");
            p.grow(program::Table::Keys, root, index);
            return TR(p.site(root).elem, Res::of(ResKind::DictGet));
        }
        case Kind::Bottom:
        case Kind::Top: return TR(owner);
        case Kind::Extern:
            if (owner.name && *owner.name == "std:os.environ") return TR(issue(n, IssueClass::Stdlib, "os.environ"));
            break;
        case Kind::Opt: return TR(issue(n, IssueClass::Conflict, "subscript of a value that may be None"));
        default: break;
    }
    return TR(issue(n, IssueClass::Conflict, "subscript of a " + kind_text(owner)));
}

// The most items a slice can hold: d[a:a+4] four, d[:8] eight, d[2:6] four.
Bound Eval::slice_cap(const syntax::Slice* sl, const Bound& cap) {
    const syntax::Expr* lower = sl->lower;
    const syntax::Expr* upper = sl->upper;
    if (sl->step != nullptr) return cap;
    const std::optional<Wide> hi = int_constant(upper);
    const std::optional<Wide> lo = int_constant(lower);
    if (upper != nullptr && hi && *hi >= Wide(0)) {
        if (lower == nullptr) return cap ? Bound(std::min(*cap, *hi)) : Bound(*hi);
        if (lo && *lo >= Wide(0)) return std::max(*hi - *lo, Wide(0));
    }
    if (const auto* sum = syntax::as<syntax::BinOp>(upper);
        sum != nullptr && sum->op == syntax::OpKind::Add && lower != nullptr &&
        syntax::dump(sum->left, false) == syntax::dump(lower, false)) {
        const std::optional<Wide> extent = int_constant(sum->right);
        if (extent && *extent >= Wide(0)) return *extent;
    }
    return cap;
}

std::vector<Type> Eval::ev_slice_parts(const syntax::Slice* sl, const State& state) {
    std::vector<Type> out;
    for (const syntax::Expr* part : {sl->lower, sl->upper, sl->step}) out.push_back(part != nullptr ? ev(part, state) : P::none());
    return out;
}

void Eval::need_int_index(const syntax::Node* node, const Type& t) {
    if (!is_one_of(t, {Kind::Int, Kind::Bottom, Kind::Top, Kind::Bool})) issue(node, IssueClass::Conflict, "index of " + kind_text(t));
}

program::Origin Eval::where(const syntax::Node* n) {
    return program::Origin{m != nullptr ? m->name : "?", static_cast<std::uint32_t>(n->pos.lineno)};
}

}  // namespace cb::translate::frontends::python::infer
