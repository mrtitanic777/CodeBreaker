// translate/frontends/python/infer_ops.cpp -- operators and comparisons on types, and the displays (tuples, lists,
// sets, dicts) and comprehension scopes.
#include <algorithm>
#include <format>
#include <unordered_set>

#include "translate/frontends/python/infer_util.hpp"
#include "translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

using O = syntax::OpKind;

Kind int_kind(Kind k) { return k == Kind::Bool ? Kind::Int : k; }

bool is_text_or_bytes(Kind k) { return k == Kind::Str || k == Kind::Bytes || k == Kind::Bytearray; }

// A conversion of a %-format: `%[flags][width][.precision]conversion`.
struct PercentSpec {
    std::string width;
    std::string precision;
    char conversion = 0;
};

// The conversions of a %-format, left to right (a '%' that does not start one is skipped).
std::vector<PercentSpec> percent_specs(std::string_view fmt) {
    std::vector<PercentSpec> specs;
    for (std::size_t i = 0; i < fmt.size(); ++i) {
        if (fmt[i] != '%') continue;
        std::size_t j = i + 1;
        while (j < fmt.size() && std::string_view("-+ #0").find(fmt[j]) != std::string_view::npos) ++j;
        const auto number = [&](std::size_t& at) {   // '*' or a run of digits, or nothing
            const std::size_t start = at;
            if (at < fmt.size() && fmt[at] == '*') {
                ++at;
            } else {
                while (at < fmt.size() && fmt[at] >= '0' && fmt[at] <= '9') ++at;
            }
            return std::string(fmt.substr(start, at - start));
        };
        PercentSpec spec;
        spec.width = number(j);
        if (j < fmt.size() && fmt[j] == '.') {
            std::size_t after_dot = j + 1;
            std::string precision = number(after_dot);
            if (!precision.empty()) {
                spec.precision = precision;
                j = after_dot;
            }
        }
        const bool letter = j < fmt.size() && ((fmt[j] >= 'a' && fmt[j] <= 'z') || (fmt[j] >= 'A' && fmt[j] <= 'Z') || fmt[j] == '%');
        if (!letter) continue;
        spec.conversion = fmt[j];
        specs.push_back(spec);
        i = j;
    }
    return specs;
}

}  // namespace

Type Eval::binop(const syntax::Node* n, syntax::OpKind op, Type a, Type b, const syntax::Expr* left, const syntax::Expr* right) {
    if (a.kind == Kind::Bottom || b.kind == Kind::Bottom) return P::bottom();
    if (a.kind == Kind::Top) return a;
    if (b.kind == Kind::Top) return b;
    Kind ka = a.kind;
    Kind kb = b.kind;
    if (ka == Kind::Bool) {
        ka = Kind::Int;
        a = P::int_range(Wide(0), Wide(1));
    }
    if (kb == Kind::Bool) {
        kb = Kind::Int;
        b = P::int_range(Wide(0), Wide(1));
    }
    if (ka == Kind::Int && kb == Kind::Int) {
        switch (op) {
            case O::Add: return P::add(a, b);
            case O::Sub: return P::subtract(a, b);
            case O::Mult: return P::multiply(a, b);
            case O::FloorDiv: return P::floor_divide(a, b);
            case O::Mod: return P::modulo(a, b);
            case O::LShift: return P::shift_left(a, b);
            case O::RShift: return P::shift_right(a, b);
            case O::BitAnd: return P::bit_and(a, b);
            case O::BitOr: return P::bit_or(a, b);
            case O::BitXor: return P::bit_xor(a, b);
            case O::Pow:
                if (b.lo && *b.lo >= Wide(0)) return P::power(a, b);
                return issue(n, IssueClass::Conflict, "int ** a possibly negative int (a float)");
            case O::Div:
                exact_float(n, a);
                exact_float(n, b);
                return P::floating();
            default: break;
        }
    }
    const bool a_number = ka == Kind::Int || ka == Kind::Float;
    const bool b_number = kb == Kind::Int || kb == Kind::Float;
    if (a_number && b_number) {
        if (ka == Kind::Int) exact_float(n, a);
        if (kb == Kind::Int) exact_float(n, b);
        if (op == O::Add || op == O::Sub || op == O::Mult || op == O::Div || op == O::FloorDiv || op == O::Mod || op == O::Pow) {
            return P::floating();
        }
    }
    if (ka == kb && is_text_or_bytes(ka) && op == O::Add) {
        if (ka == Kind::Str) return P::str();
        return P::bytes_of((!a.hi || !b.hi) ? Bound() : Bound(*a.hi + *b.hi), ka);
    }
    if (ka == Kind::Bytearray && kb == Kind::Bytes && op == O::Add) return P::bytes_of(std::nullopt, Kind::Bytearray);
    if (ka == Kind::Bytes && kb == Kind::Bytearray && op == O::Add) return P::bytes_of();
    if ((ka == Kind::Str || ka == Kind::Bytes) && kb == Kind::Int && op == O::Mult) {
        return ka == Kind::Bytes ? P::bytes_of() : P::str();
    }
    if (ka == Kind::Str && op == O::Mod) {
        check_percent(n, left, right, b);
        return P::str();
    }
    if (ka == Kind::Bytes && op == O::Mod) return issue(n, IssueClass::Syntax, "bytes %-formatting");
    if (ka == Kind::List && kb == Kind::List && op == O::Add) {
        const program::SiteId site = p.new_site(n, program::Container::List, where(n));
        p.grow(program::Table::Elem, site, p.root(a.site.value()).elem);
        p.grow(program::Table::Elem, site, p.root(b.site.value()).elem);
        return site_type(Kind::List, site);
    }
    if (ka == Kind::List && kb == Kind::Int && op == O::Mult) {
        const program::SiteId site = p.new_site(n, program::Container::List, where(n));
        p.grow(program::Table::Elem, site, p.root(a.site.value()).elem);
        return site_type(Kind::List, site);
    }
    if (ka == Kind::Tuple && kb == Kind::Tuple && op == O::Add) {
        std::vector<Type> all = a.args;
        all.insert(all.end(), b.args.begin(), b.args.end());
        return P::tuple_of(std::move(all));
    }
    if (ka == Kind::Set && kb == Kind::Set && (op == O::BitOr || op == O::BitAnd || op == O::Sub || op == O::BitXor)) {
        const program::SiteId site = p.new_site(n, program::Container::Set, where(n));
        p.grow(program::Table::Elem, site, p.root(a.site.value()).elem);
        p.grow(program::Table::Elem, site, p.root(b.site.value()).elem);
        return site_type(Kind::Set, site);
    }
    return issue(n, IssueClass::Conflict, std::string(P::kind_word(ka)) + " " + std::string(syntax::name_of(op)) + " " + std::string(P::kind_word(kb)));
}

// An int meeting a float is exact in a double only within 2**53.
void Eval::exact_float(const syntax::Node* n, const Type& t) {
    if (t.kind != Kind::Int) return;
    static const Wide limit = Wide::pow2(53);
    if (!(t.lo && t.hi && -limit <= *t.lo && *t.hi <= limit)) issue(n, IssueClass::IntRange, "an int beyond 2**53 meets a float");
}

void Eval::check_percent(const syntax::Node* n, const syntax::Expr* left, const syntax::Expr* right, const Type& b) {
    (void)right;
    const std::optional<std::string> fmt = pyast::string_constant(left);
    if (!fmt) {
        issue(n, IssueClass::Dynamic, "% with a computed format");
        return;
    }
    const std::vector<PercentSpec> specs = percent_specs(*fmt);
    for (const PercentSpec& spec : specs) {
        const bool known = std::string_view("dixXosrcfeEgG%").find(spec.conversion) != std::string_view::npos;
        if (!known || spec.width == "*" || spec.precision == "*") {
            issue(n, IssueClass::Syntax, std::string("percent conversion %") + spec.conversion);
        }
    }
    if (fmt->find("%(") != std::string::npos) issue(n, IssueClass::Syntax, "percent with a mapping");
    const std::size_t values = static_cast<std::size_t>(std::ranges::count_if(specs, [](const PercentSpec& s) { return s.conversion != '%'; }));
    const std::vector<Type> items = b.kind == Kind::Tuple ? b.args : std::vector<Type>{b};
    if (b.kind == Kind::Tuple && items.size() != values) {
        issue(n, IssueClass::Conflict, std::format("% with {} values for {} conversions", items.size(), values));
    }
    for (const Type& t : items) {
        const bool plain_optional = t.kind == Kind::Opt && is_one_of(t.args[0], {Kind::Int, Kind::Str});
        if (is_one_of(t, {Kind::List, Kind::Dict, Kind::Set, Kind::Inst, Kind::Opt}) && !plain_optional) {
            issue(n, IssueClass::Syntax, "% of a " + kind_text(t));
        }
    }
}

TR Eval::e_unary_op(const syntax::UnaryOp* n, const State& state) {
    Type t = ev(n->operand, state);
    if (n->op == syntax::UnaryOpKind::Not) return TR(P::boolean());
    if (is_one_of(t, {Kind::Bottom, Kind::Top})) return TR(t);
    if (t.kind == Kind::Bool) t = P::int_range(Wide(0), Wide(1));
    if (t.kind == Kind::Int) {
        if (n->op == syntax::UnaryOpKind::USub) return TR(P::negate(t));
        if (n->op == syntax::UnaryOpKind::UAdd) return TR(t);
        if (n->op == syntax::UnaryOpKind::Invert) return TR(P::bit_invert(t));
    }
    if (t.kind == Kind::Float && (n->op == syntax::UnaryOpKind::USub || n->op == syntax::UnaryOpKind::UAdd)) return TR(P::floating());
    return TR(issue(n, IssueClass::Conflict, std::string(syntax::name_of(n->op)) + " of " + kind_text(t)));
}

TR Eval::e_bool_op(const syntax::BoolOp* n, const State& state) {
    std::vector<Type> operands;
    Flow s = state;
    for (const syntax::Expr* v : n->values) {
        operands.push_back(ev(v, *s));
        s = refine(v, s, n->op == syntax::BoolOpKind::And);
    }
    // the value is one of the operands (Python): in a test only truth matters, so bool; as a value, all operands must be
    // of one type
    Type joined = P::bottom();
    for (const Type& t : operands) joined = p.join(joined, t);
    if (std::ranges::all_of(operands, [](const Type& t) { return is_one_of(t, {Kind::Bool, Kind::Bottom}); })) return TR(P::boolean());
    if (n->op == syntax::BoolOpKind::Or && operands.size() == 2 && operands[0].kind == Kind::Opt) {
        const Type with_default = p.join(P::strip_optional(operands[0]), operands[1]);   // `x or default`: None never comes out
        if (with_default.kind != Kind::Top) return TR(with_default);
    }
    if (joined.kind == Kind::Top) return TR(P::boolean(), Res::of(ResKind::MixedBool, joined.why));
    return TR(joined);
}

TR Eval::e_compare(const syntax::Compare* n, const State& state) {
    Type left = ev(n->left, state);
    for (std::size_t i = 0; i < n->ops.size(); ++i) {
        const Type right = ev(n->comparators[i], state);
        check_compare(n, n->ops[i], left, right);
        left = right;
    }
    return TR(P::boolean());
}

void Eval::check_compare(const syntax::Compare* n, syntax::CmpOpKind op, const Type& a, const Type& b) {
    using C = syntax::CmpOpKind;
    if (is_one_of(a, {Kind::Bottom, Kind::Top}) || is_one_of(b, {Kind::Bottom, Kind::Top})) return;
    if (op == C::Is || op == C::IsNot) {
        if (b.kind != Kind::None && a.kind != Kind::None) issue(n, IssueClass::Dynamic, "identity comparison of " + kind_text(a));
        return;
    }
    if (op == C::In || op == C::NotIn) {
        if (is_one_of(b, {Kind::List, Kind::Set})) {
            comparable(n, a, p.root(b.site.value()).elem);
        } else if (b.kind == Kind::Dict) {
            const program::SiteId root = p.find(b.site.value());
            if (!pyast::string_constant(n->left)) p.make_dynamic(root, "membership of a computed key");
        } else if (b.kind == Kind::Tuple) {
            for (const Type& x : b.args) comparable(n, a, x);
        } else if (is_text_or_bytes(b.kind)) {
            const bool both_bytes = is_one_of(a, {Kind::Bytes, Kind::Bytearray}) && is_one_of(b, {Kind::Bytes, Kind::Bytearray});
            if (!(a.kind == b.kind || (a.kind == Kind::Int && b.kind != Kind::Str) || both_bytes)) {
                issue(n, IssueClass::Conflict, kind_text(a) + " in " + kind_text(b));
            }
        } else {
            issue(n, IssueClass::Conflict, "membership in a " + kind_text(b));
        }
        return;
    }
    if (op == C::Eq || op == C::NotEq) {
        comparable(n, a, b, true);
        return;
    }
    comparable(n, a, b, false, true);
}

void Eval::comparable(const syntax::Node* n, const Type& a, const Type& b, bool eq, bool order) {
    if (is_one_of(a, {Kind::Bottom, Kind::Top}) || is_one_of(b, {Kind::Bottom, Kind::Top})) return;
    const Kind ka = int_kind(a.kind);
    const Kind kb = int_kind(b.kind);
    if (ka == kb && (ka == Kind::Int || ka == Kind::Str || ka == Kind::Float || ka == Kind::Bytes || ka == Kind::Bytearray || ka == Kind::None)) return;
    const auto pair_is = [&](Kind x, Kind y) { return (ka == x && kb == y) || (ka == y && kb == x); };
    if (pair_is(Kind::Bytes, Kind::Bytearray)) return;
    if (pair_is(Kind::Int, Kind::Float)) {
        exact_float(n, ka == Kind::Int ? a : b);
        return;
    }
    if (ka == Kind::Tuple && kb == Kind::Tuple && a.args.size() == b.args.size()) {
        for (std::size_t i = 0; i < a.args.size(); ++i) comparable(n, a.args[i], b.args[i], eq, order);
        return;
    }
    if (eq && (ka == Kind::Opt || kb == Kind::Opt)) {
        const Type inner_a = P::strip_optional(a);
        const Type inner_b = P::strip_optional(b);
        if (inner_b.kind == Kind::None || inner_a.kind == Kind::None) return;
        comparable(n, inner_a, inner_b, eq, order);
        return;
    }
    if (eq && ka == Kind::List && kb == Kind::List) {
        comparable(n, p.root(a.site.value()).elem, p.root(b.site.value()).elem, eq);
        return;
    }
    if (eq && ka == Kind::Inst && kb == Kind::Inst) {
        issue(n, IssueClass::Dynamic, "equality of instances");
        return;
    }
    if (eq) {
        issue(n, IssueClass::Conflict, std::format("comparing {} with {} (always unequal in Python)", P::kind_word(ka), P::kind_word(kb)));
        return;
    }
    issue(n, IssueClass::Conflict, std::format("ordering {} against {}", P::kind_word(ka), P::kind_word(kb)));
}

TR Eval::e_if_exp(const syntax::IfExp* n, const State& state) {
    ev(n->test, state);
    const Type a = ev(n->body, *refine(n->test, state, true));
    const Type b = ev(n->orelse, *refine(n->test, state, false));
    const Type joined = p.join(a, b);
    if (joined.kind == Kind::Top) return TR(issue(n, IssueClass::Conflict, joined.why.value_or("")));
    return TR(joined);
}

// ---- displays ------------------------------------------------------------------------------------------------

TR Eval::e_tuple(const syntax::Tuple* n, const State& state) {
    std::vector<Type> items;
    for (const syntax::Expr* e : n->elts) items.push_back(ev(e, state));
    for (const syntax::Expr* e : n->elts) {
        if (syntax::as<syntax::Starred>(e) != nullptr) return TR(issue(n, IssueClass::Syntax, "starred in a tuple"));
    }
    return TR(P::tuple_of(std::move(items)));
}

TR Eval::e_list(const syntax::List* n, const State& state) {
    const program::SiteId site = p.new_site(n, program::Container::List, where(n));
    for (const syntax::Expr* e : n->elts) p.grow(program::Table::Elem, site, ev(e, state));
    return TR(site_type(Kind::List, site));
}

TR Eval::e_set(const syntax::Set* n, const State& state) {
    const program::SiteId site = p.new_site(n, program::Container::Set, where(n));
    for (const syntax::Expr* e : n->elts) p.grow(program::Table::Elem, site, ev(e, state));
    return TR(site_type(Kind::Set, site));
}

TR Eval::e_dict(const syntax::Dict* n, const State& state) {
    const program::SiteId site = p.new_site(n, program::Container::Dict, where(n));
    std::vector<std::string> keys;
    for (std::size_t i = 0; i < n->keys.size(); ++i) {
        const syntax::Expr* k = n->keys[i];
        const syntax::Expr* v = n->values[i];
        const Type value = ev(v, state);
        if (k == nullptr) {   // {**other}
            spread(site, value, n);
            continue;
        }
        const Type key_type = ev(k, state);
        if (const std::optional<std::string> key = pyast::string_constant(k)) {
            p.grow_field(site, *key, fit(v, value, "field " + *key));
            keys.push_back(*key);
        } else {
            p.make_dynamic(site, "a key that is not a constant string");
            p.grow(program::Table::Keys, site, key_type);
            p.grow(program::Table::Elem, site, value);
        }
    }
    if (live()) {
        std::vector<std::string> all = keys;
        if (const auto& spread_keys = p.site(site).spread_keys) all.insert(all.end(), spread_keys->begin(), spread_keys->end());
        p.literal_keys[n] = all;
        if (builder != n) p.created(site, program::StringSet(all.begin(), all.end()));
    }
    return TR(site_type(Kind::Dict, site));
}

// `{**other, ...}`: what the other dict has, this one has.
void Eval::spread(program::SiteId into, const Type& value_type, const syntax::Node* n) {
    if (value_type.kind != Kind::Dict) {
        if (!is_one_of(value_type, {Kind::Bottom, Kind::Top})) issue(n, IssueClass::Conflict, "** of a " + kind_text(value_type));
        return;
    }
    const program::SiteId source = p.find(value_type.site.value());
    if (p.site(source).dynamic) {
        p.make_dynamic(into, "filled from a dict");
        const Type keys = p.site(source).keys;
        const Type elems = p.site(source).elem;
        p.grow(program::Table::Keys, into, keys);
        p.grow(program::Table::Elem, into, elems);
        return;
    }
    const auto fields = p.site(source).fields;   // (a copy: growing a field of `into` may merge the two sites)
    for (const auto& [name, type] : fields) p.grow_field(into, name, type);
    const std::optional<program::StringSet> required = p.site(source).required;
    p.site(into).spread_keys = required ? *required : program::StringSet();
    p.site(into).spread_from.insert(source);
}

State Eval::comp_scope(const std::vector<syntax::comprehension*>& gens, const State& state) {
    State scope = state;
    for (const syntax::comprehension* g : gens) {
        if (g->is_async) issue(g, IssueClass::Dynamic, "async comprehension");
        const Type iterated = ev(g->iter, scope);
        scope = assign(g->target, iteration(g->iter, iterated).first, scope);
        for (const syntax::Expr* condition : g->ifs) {
            ev(condition, scope);
            scope = *refine(condition, scope, true);
        }
    }
    return scope;
}

}  // namespace cb::translate::frontends::python::infer
