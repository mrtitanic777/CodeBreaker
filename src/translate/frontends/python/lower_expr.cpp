// cb-unity: alone (file-local helpers named for this module)
// The expressions of a function.
#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

namespace {

// What a resolution is called in a reason ("fn", "classattr", "tuple_slice" ...).
std::string_view resolution_word(infer::ResKind kind) {
    using K = infer::ResKind;
    switch (kind) {
        case K::Local: return "local";
        case K::Fn: return "fn";
        case K::Class: return "class";
        case K::Global: return "global";
        case K::Module: return "module";
        case K::StdModule: return "stdmodule";
        case K::StdObj: return "stdobj";
        case K::Extern: return "extern";
        case K::Missing: return "missing";
        case K::Builtin: return "builtin";
        case K::Exc: return "exc";
        case K::Const: return "const";
        case K::Field: return "field";
        case K::ClassAttr: return "classattr";
        case K::Property: return "property";
        case K::Method: return "method";
        case K::Bound: return "bound";
        case K::Ctor: return "ctor";
        case K::BaseCall: return "basecall";
        case K::Super: return "super";
        case K::StdAttr: return "stdattr";
        case K::BMethod: return "bmethod";
        case K::Slice: return "slice";
        case K::SliceStore: return "slice_store";
        case K::TupleSlice: return "tuple_slice";
        case K::Index: return "index";
        case K::IndexStore: return "index_store";
        case K::TupleIndex: return "tuple_index";
        case K::FieldGet: return "field_get";
        case K::DictGet: return "dict_get";
        case K::DictStore: return "dict_store";
        case K::Unpack: return "unpack";
        case K::MixedBool: return "mixedbool";
        case K::Isinstance: return "isinstance";
        case K::Std: return "std";
    }
    return "?";
}

bool is_none_constant_node(const syntax::Expr* e) { return pyast::is_none_constant(e); }

// The text of an int literal as the source spelled it, when it is a plain number (digits, a base prefix, underscores).
bool is_plain_number_text(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return std::string_view("0123456789abcdefABCDEFxXoObB_").find(c) != std::string_view::npos; });
}

const char* operator_text(syntax::OpKind op) {
    switch (op) {
        case syntax::OpKind::Add: return "+";
        case syntax::OpKind::Sub: return "-";
        case syntax::OpKind::Mult: return "*";
        case syntax::OpKind::Div: return "/";
        case syntax::OpKind::FloorDiv: return "//";
        case syntax::OpKind::Mod: return "%";
        case syntax::OpKind::LShift: return "<<";
        case syntax::OpKind::RShift: return ">>";
        case syntax::OpKind::BitAnd: return "&";
        case syntax::OpKind::BitOr: return "|";
        case syntax::OpKind::BitXor: return "^";
        case syntax::OpKind::Pow: return "**";
        case syntax::OpKind::MatMult: return nullptr;
    }
    return nullptr;
}

// Python's slice bounds over `length` items: a negative bound counts from the end, a bound past either end stops there.
std::vector<std::size_t> slice_indexes(std::size_t length, const std::optional<Wide>& from, const std::optional<Wide>& to) {
    const auto clamp = [&](const std::optional<Wide>& bound, std::int64_t fallback) {
        if (!bound) return fallback;
        std::int64_t b = bound->to_i64().value_or(bound->sign() < 0 ? -(std::int64_t{1} << 62) : (std::int64_t{1} << 62));
        if (b < 0) b += static_cast<std::int64_t>(length);
        return std::clamp<std::int64_t>(b, 0, static_cast<std::int64_t>(length));
    };
    const std::int64_t lo = clamp(from, 0), hi = clamp(to, static_cast<std::int64_t>(length));
    std::vector<std::size_t> out;
    for (std::int64_t i = lo; i < hi; ++i) out.push_back(static_cast<std::size_t>(i));
    return out;
}

}  // namespace

// ---- conditions and conversions ------------------------------------------------------------------------------------------------

// An expression as a C++ condition (Python's truth).
Node* Lowerer::cond(const syntax::Expr* node) {
    in_cond = node;
    return truthy(expr(node));
}

Node* Lowerer::truthy(Node* e) {
    if (e->type == bool_type()) return e;
    return py_call("truthy", {e}, bool_type());
}

// A value where a `want` is expected: an optional unwrapped or wrapped, None typed.
Node* Lowerer::coerce(Node* e, const TypeP& want) {
    if (!want || e->type == want) return e;
    if (const auto* literal = ir::get<ir::Lit>(e); literal != nullptr && std::holds_alternative<std::monostate>(literal->value)) return lit_none(want);
    if (is_opt(e->type) && !is_opt(want)) {
        if (class_type_of(e->type)->args.at(0) == want) return py_call("unwrap", {e}, want);
    }
    if (is_opt(want) && e->type == class_type_of(want)->args.at(0)) return e;
    if (is_ref_class(want) && is_ref_class(e->type)) return e;   // a subclass where its base is expected
    if (want == i64_type() && e->type == bool_type()) return mk(ir::Cast{.x = e}, i64_type());
    return e;   // (tuples and bytes of another kind are each other's too)
}

Node* Lowerer::expr(const syntax::Expr* node) {
    using K = syntax::ExprKind;
    switch (node->kind) {
        case K::Constant: return e_constant(syntax::cast<syntax::Constant>(node));
        case K::Name: return e_name(syntax::cast<syntax::Name>(node));
        case K::Attribute: return e_attribute(syntax::cast<syntax::Attribute>(node));
        case K::Subscript: return e_subscript(syntax::cast<syntax::Subscript>(node));
        case K::BinOp: return e_bin_op(syntax::cast<syntax::BinOp>(node));
        case K::UnaryOp: return e_unary_op(syntax::cast<syntax::UnaryOp>(node));
        case K::BoolOp: return e_bool_op(syntax::cast<syntax::BoolOp>(node));
        case K::Compare: return e_compare(syntax::cast<syntax::Compare>(node));
        case K::IfExp: return e_if_exp(syntax::cast<syntax::IfExp>(node));
        case K::JoinedStr: return e_joined_str(syntax::cast<syntax::JoinedStr>(node));
        case K::Tuple: return e_tuple(syntax::cast<syntax::Tuple>(node));
        case K::List: return e_list(syntax::cast<syntax::List>(node));
        case K::Set: return e_set(syntax::cast<syntax::Set>(node));
        case K::Dict: return e_dict(syntax::cast<syntax::Dict>(node));
        case K::ListComp: return hoist_comp(node, "list");
        case K::SetComp: return hoist_comp(node, "set");
        case K::DictComp: return hoist_comp(node, "dict");
        case K::GeneratorExp: refuse(RefusalClass::Syntax, "generator expression as a value", line_of(node));
        case K::Lambda: refuse(RefusalClass::Dynamic, "lambda", line_of(node));
        case K::Call: return e_call(syntax::cast<syntax::Call>(node));
        default: refuse(RefusalClass::Syntax, text::lower(syntax::name_of(node->kind)), line_of(node));
    }
}

// ---- constants, names, attributes, subscripts ----------------------------------------------------------------------------------------

Node* Lowerer::e_constant(const syntax::Constant* n) {
    using K = syntax::ConstValue::Kind;
    const syntax::ConstValue& v = n->value;
    const Type& t = type_at(n, line_of(n));
    switch (v.kind) {
        case K::None: return lit_none(none_type());
        case K::True: return lit(true, bool_type());
        case K::False: return lit(false, bool_type());
        case K::Int: {
            if (!t.fits_i64() || !v.fits_i64) refuse(RefusalClass::IntRange, "literal " + v.text, line_of(n));
            const auto text_of = source_segment(source, n->pos);
            return lit(v.i64, i64_type(), text_of && is_plain_number_text(*text_of) ? text_of : std::nullopt);
        }
        case K::Float: return lit(v.f, f64_type(), source_segment(source, n->pos));
        case K::Str: return lit(v.text, str_type());
        case K::Bytes: return lit(text::from_latin1(as_bytes(v.text)), bytes_type());
        default: refuse(RefusalClass::Syntax, "constant", line_of(n));
    }
}

Node* Lowerer::e_name(const syntax::Name* n) {
    const infer::Res* r = res_at(n);
    const Type& t = type_at(n, line_of(n));
    const std::uint32_t line = line_of(n);
    if (r == nullptr || r->kind == infer::ResKind::Local) {
        if (fn != nullptr && (kind == program::FnKind::Method || kind == program::FnKind::Property) && !params.empty() && n->id == params[0])
            return mk(ir::This{}, ir::make_type(ir::ClassType{.name = cls->qname, .ref = true}), line);   // `self`
        if (!local_types.contains(n->id)) refuse(RefusalClass::Untyped, "local " + n->id, line);
        Node* e = name_expr(n->id);
        return narrowed(e, ty(t, n));
    }
    if (r->kind == infer::ResKind::Global) {
        globals_used.emplace(r->name.value_or(""), r->member);
        const TypeP global_type = ctx.type_of(p.global_type(r->name.value_or(""), r->member), line, "global " + r->member);
        return mk(ir::Select{.obj = class_name_node(r->name.value_or("")), .sym = ir::make_sym(ir::SymKind::Global, r->member, r->name, true, global_type)}, global_type, line);
    }
    refuse(RefusalClass::Dynamic, std::format("{} {} as a value", resolution_word(r->kind), n->id), line);
}

// A value that may be None where the flow shows it is not: the value inside.
Node* Lowerer::narrowed(Node* e, const TypeP& flow) {
    if (is_opt(e->type) && !is_opt(flow)) {
        if (flow == none_type()) return e;
        return py_call("unwrap", {e}, flow);
    }
    return e;
}

Node* Lowerer::e_attribute(const syntax::Attribute* n) {
    const infer::Res* r = res_at(n);
    const std::uint32_t line = line_of(n);
    if (r == nullptr) refuse(RefusalClass::Dynamic, "attribute ." + n->attr, line);
    const std::string owner = r->name.value_or("");
    switch (r->kind) {
        case infer::ResKind::Field: {
            Node* obj = expr(n->value);
            const auto stored = p.attr_type(owner, r->member);
            if (!stored) refuse(RefusalClass::Untyped, "attribute " + r->member, line);
            const TypeP field_type = ctx.type_of(*stored, line, "attribute " + r->member);
            return mk(ir::Select{.obj = obj, .sym = ir::make_sym(ir::SymKind::Field, r->member, owner, false, field_type)}, field_type, line);
        }
        case infer::ResKind::Property: {
            Node* obj = expr(n->value);
            calls.insert(owner);
            const TypeP t = ty(type_at(n, line), n);
            const Fn& f = *p.fns.at(owner);
            const ir::SymP sym = ir::make_sym(ir::SymKind::Method, f.name, f.cls->qname);
            sym->ret = t;
            return mk(ir::Call{.target = obj, .sym = sym}, t, line);
        }
        case infer::ResKind::Global: {
            globals_used.emplace(owner, r->member);
            const TypeP global_type = ctx.type_of(p.global_type(owner, r->member), line, "global " + r->member);
            return mk(ir::Select{.obj = class_name_node(owner), .sym = ir::make_sym(ir::SymKind::Global, r->member, owner, true, global_type)}, global_type, line);
        }
        case infer::ResKind::ClassAttr: {
            const TypeP t = ctx.type_of(p.class_attr_type(owner, r->member), line, "class attribute " + r->member);
            return mk(ir::Select{.obj = class_name_node(owner), .sym = ir::make_sym(ir::SymKind::Field, r->member, owner, true, t)}, t, line);
        }
        case infer::ResKind::StdAttr: return py_call(owner, {}, ty(type_at(n, line), n), nullptr, line);
        default: refuse(RefusalClass::Dynamic, "attribute " + n->attr + " as a value", line);
    }
}

Node* Lowerer::e_subscript(const syntax::Subscript* n) {
    const infer::Res* r = res_at(n);
    const std::uint32_t line = line_of(n);
    const TypeP t = ty(type_at(n, line), n);
    Node* obj = expr(n->value);
    const infer::ResKind kind_of_res = r != nullptr ? r->kind : infer::ResKind::Missing;
    if (kind_of_res == infer::ResKind::Slice) {
        const auto* slice = syntax::as<syntax::Slice>(n->slice);
        if (slice == nullptr) refuse(RefusalClass::Syntax, "slice", line);
        std::vector<Node*> args = {slice->lower != nullptr ? expr(slice->lower) : lit_none(none_type()), slice->upper != nullptr ? expr(slice->upper) : lit_none(none_type())};
        if (slice->step != nullptr) args.push_back(expr(slice->step));
        return py_call("slice", std::move(args), t, obj, line);
    }
    if (kind_of_res == infer::ResKind::TupleSlice) {
        const ir::ClassType* tuple = class_type_of(obj->type);
        if (tuple == nullptr) refuse(RefusalClass::Conflict, "slice of a non-tuple", line);
        if (effects(obj)) refuse(RefusalClass::Syntax, "slice of a computed tuple", line);
        std::vector<Node*> items;
        for (const std::size_t i : slice_indexes(tuple->args.size(), r->lo, r->hi))
            items.push_back(py_call("get", {obj}, tuple->args[i], nullptr, 0, {static_cast<std::int64_t>(i)}));
        return py_call("tuple", std::move(items), t);
    }
    if (kind_of_res == infer::ResKind::TupleIndex) {
        const ir::ClassType* tuple = class_type_of(obj->type);
        if (!r->lo) refuse(RefusalClass::Syntax, "computed tuple index", line);
        if (tuple == nullptr || tuple->args.empty()) refuse(RefusalClass::Conflict, "index of an empty tuple", line);
        const std::int64_t count = static_cast<std::int64_t>(tuple->args.size());
        const std::int64_t index = ((r->lo->to_i64().value_or(0) % count) + count) % count;   // Python's modulo: a negative index counts from the end
        return py_call("get", {obj}, t, nullptr, line, {index});
    }
    if (kind_of_res == infer::ResKind::FieldGet) return field_ref(obj, r->name.value_or(""), n->value, line);
    Node* index = expr(n->slice);
    if (kind_of_res == infer::ResKind::DictGet) return py_call("dict.at", {index}, t, obj, line);
    return mk(ir::Index{.arr = obj, .index = index}, t, line);
}

// ---- operators -------------------------------------------------------------------------------------------------------------------------

Node* Lowerer::e_bin_op(const syntax::BinOp* n) {
    Node* a = expr(n->left);
    Node* b = expr(n->right);
    return binop(n, n->op, a, b, type_at(n->left, line_of(n)), type_at(n->right, line_of(n)), type_at(n, line_of(n)));
}

Node* Lowerer::binop(const syntax::Node* n, syntax::OpKind op, Node* a, Node* b, const Type& ta_in, const Type& tb_in, const Type& tr) {
    const std::uint32_t line = line_of(n);
    const char* op_text = operator_text(op);
    if (op_text == nullptr) refuse(RefusalClass::Syntax, "matrix multiplication", line);
    const std::string o = op_text;
    const TypeP rt = ty(tr, n);
    Type ta = ta_in, tb = tb_in;
    if (is_opt(a->type) || is_opt(b->type)) refuse(RefusalClass::Conflict, "arithmetic on a value that may be None", line);
    pt::Kind ka = ta.kind, kb = tb.kind;
    if (ka == pt::Kind::Bool) {   // a bool is the int 0 or 1
        ka = pt::Kind::Int;
        ta = pt::int_range(Wide(0), Wide(1));
        a = mk(ir::Cast{.x = a}, i64_type());
    }
    if (kb == pt::Kind::Bool) {
        kb = pt::Kind::Int;
        tb = pt::int_range(Wide(0), Wide(1));
        b = mk(ir::Cast{.x = b}, i64_type());
    }
    if (ka == pt::Kind::Int && kb == pt::Kind::Int) return int_op(n, o, a, b, ta, tb, tr, rt);
    if (rt == f64_type()) {
        if (o == "/") return py_call("truediv", {a, b}, f64_type(), nullptr, line);
        if (o == "//") return py_call("floordiv", {a, b}, f64_type(), nullptr, line);
        if (o == "%") return py_call("mod", {a, b}, f64_type(), nullptr, line);
        if (o == "**") return py_call("pow", {a, b}, f64_type(), nullptr, line);
        return mk(ir::Binary{.op = o, .left = as_double(a), .right = b}, f64_type(), line);
    }
    if (ka == pt::Kind::Str && o == "+") {
        std::vector<Node*> parts;
        for (Node* side : {a, b}) {
            if (const auto* c = ir::get<ir::Concat>(side)) parts.insert(parts.end(), c->parts.begin(), c->parts.end());
            else parts.push_back(side);
        }
        return mk(ir::Concat{.parts = std::move(parts)}, str_type(), line);
    }
    if (ka == pt::Kind::Str && o == "%") {
        const auto* tuple = ir::get<ir::Call>(b);
        std::vector<Node*> values;
        if (tuple != nullptr && tuple->sym->name == "tuple" && tuple->sym->owner == std::optional<std::string>(std::string(kPy))) values = tuple->args;
        else values = {b};
        values.insert(values.begin(), a);
        return py_call("percent", std::move(values), str_type(), nullptr, line);
    }
    if (o == "*" && (ka == pt::Kind::Str || ka == pt::Kind::Bytes || ka == pt::Kind::List)) return py_call("repeat", {a, b}, rt, nullptr, line);
    if (o == "+") return py_call("concat", {a, b}, rt, nullptr, line);
    if (ka == pt::Kind::Set && kb == pt::Kind::Set) return py_call("set" + o, {a, b}, rt, nullptr, line);
    refuse(RefusalClass::Conflict, std::format("{} {} {}", pt::kind_word(ka), o, pt::kind_word(kb)), line);
}

Node* Lowerer::as_double(Node* e) { return e->type == i64_type() ? mk(ir::Cast{.x = e}, f64_type()) : e; }

// An integer operation: C++'s own operator where the intervals show it agrees with Python's, else the support header's
// Python-exact one (and with IntPolicy::Checked, an overflow-checked one where the result is not shown to fit).
Node* Lowerer::int_op(const syntax::Node* n, std::string_view o, Node* a, Node* b, const Type& ta, const Type& tb, const Type& tr, const TypeP&) {
    const std::uint32_t line = line_of(n);
    const bool checked = ctx.int_policy() == program::IntPolicy::Checked && !tr.fits_i64();
    const auto native = [&](std::string_view op_text) { return mk(ir::Binary{.op = std::string(op_text), .left = a, .right = b}, i64_type(), line); };
    const auto helper = [&](std::string_view name) { return py_call(name, {a, b}, i64_type(), nullptr, line); };
    const bool small_shift = tb.nonneg() && tb.hi && *tb.hi < Wide(63);
    if (o == "+" || o == "-" || o == "*") {
        if (checked) return helper(o == "+" ? "checked_add" : o == "-" ? "checked_sub" : "checked_mul");
        return native(o);
    }
    if (o == "&" || o == "|" || o == "^") return native(o);
    if (o == "//" || o == "%") {
        if (ta.nonneg() && tb.positive()) return native(o == "//" ? "/" : "%");
        return helper(o == "//" ? "floordiv" : "mod");
    }
    if (o == "<<") {
        if (!checked && ta.nonneg() && small_shift) return native("<<");
        return helper(checked ? "checked_shl" : "shl");
    }
    if (o == ">>") {
        if (ta.nonneg() && small_shift) return native(">>");
        return helper("shr");
    }
    if (o == "**") return helper(checked ? "checked_pow" : "ipow");
    if (o == "/") return py_call("truediv", {a, b}, f64_type(), nullptr, line);
    refuse(RefusalClass::Conflict, "int " + std::string(o) + " int", line);
}

Node* Lowerer::e_unary_op(const syntax::UnaryOp* n) {
    const std::uint32_t line = line_of(n);
    Node* x = expr(n->operand);
    if (n->op == syntax::UnaryOpKind::Not) return mk(ir::Unary{.op = "!", .x = paren(truthy(x)), .postfix = false}, bool_type(), line);
    const Type& source_type = type_at(n, line);
    const TypeP t = ty(source_type, n);
    if (x->type == bool_type()) x = mk(ir::Cast{.x = x}, i64_type());
    switch (n->op) {
        case syntax::UnaryOpKind::USub:
            if (ctx.int_policy() == program::IntPolicy::Checked && t == i64_type() && !source_type.fits_i64()) return py_call("checked_neg", {x}, i64_type());
            return mk(ir::Unary{.op = "-", .x = paren(x), .postfix = false}, t, line);
        case syntax::UnaryOpKind::UAdd: return x;
        case syntax::UnaryOpKind::Invert: return mk(ir::Unary{.op = "~", .x = paren(x), .postfix = false}, t, line);
        default: refuse(RefusalClass::Syntax, "unary", line);
    }
}

Node* Lowerer::paren(Node* x) {
    if (ir::is<ir::Binary>(x) || ir::is<ir::Cond>(x) || ir::is<ir::Assign>(x)) return mk(ir::Paren{.x = x}, x->type);
    return x;
}

Node* Lowerer::e_bool_op(const syntax::BoolOp* n) {
    const Type& t = type_at(n, line_of(n));
    const std::uint32_t line = line_of(n);
    const std::string op = n->op == syntax::BoolOpKind::And ? "&&" : "||";
    const infer::Res* r = res_at(n);
    if (r != nullptr && r->kind == infer::ResKind::MixedBool && in_cond != n)
        refuse(RefusalClass::Conflict, "value of and/or over " + r->name.value_or(""), line);
    if (t.kind == pt::Kind::Bool) {
        std::vector<Node*> saved = std::move(pre);
        std::vector<Node*> parts;
        for (std::size_t i = 0; i < n->values.size(); ++i) {
            pre.clear();
            Node* c = cond(n->values[i]);
            if (!pre.empty() && i > 0) refuse(RefusalClass::Syntax, "a lazy operand that needs statements", line);
            saved.insert(saved.end(), pre.begin(), pre.end());
            parts.push_back(c);
        }
        pre = std::move(saved);
        Node* e = parts[0];
        for (std::size_t i = 1; i < parts.size(); ++i) e = mk(ir::Binary{.op = op, .left = paren_bool(e, op), .right = paren_bool(parts[i], op)}, bool_type(), line);
        return e;
    }
    // a value: `a or b` is a if a is true, else b (b only evaluated then)
    const TypeP rt = ty(t, n);
    if (n->values.size() != 2) refuse(RefusalClass::Syntax, "value of a chained and/or", line);
    Node* a = expr(n->values[0]);
    std::vector<Node*> saved = std::move(pre);
    pre.clear();
    Node* b = expr(n->values[1]);
    if (!pre.empty() || effects(b)) refuse(RefusalClass::Syntax, "value of and/or with a right side that has effects", line);
    pre = std::move(saved);
    if (is_opt(a->type) && !is_opt(rt)) return py_call(op == "||" ? "or_none" : "and_none", {a, coerce(b, rt)}, rt, nullptr, line);
    return py_call(op == "||" ? "or" : "and", {coerce(a, rt), coerce(b, rt)}, rt, nullptr, line);
}

Node* Lowerer::paren_bool(Node* e, std::string_view op) {
    const auto* b = ir::get<ir::Binary>(e);
    if (b != nullptr && (b->op == "&&" || b->op == "||") && b->op != op) return mk(ir::Paren{.x = e}, bool_type());
    return e;
}

Node* Lowerer::e_compare(const syntax::Compare* n) {
    const std::uint32_t line = line_of(n);
    const syntax::Expr* left_node = n->left;
    Node* a = expr(left_node);
    std::vector<Node*> parts;
    for (std::size_t i = 0; i < n->ops.size(); ++i) {
        Node* b = expr(n->comparators[i]);
        if (i + 1 < n->ops.size() && effects(b)) refuse(RefusalClass::Syntax, "chained comparison with a computed middle", line);
        parts.push_back(compare(n, n->ops[i], a, b, left_node, n->comparators[i]));
        a = b;
        left_node = n->comparators[i];
    }
    Node* e = parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i) e = mk(ir::Binary{.op = "&&", .left = e, .right = parts[i]}, bool_type(), line);
    return e;
}

Node* Lowerer::compare(const syntax::Node* n, syntax::CmpOpKind op, Node* a, Node* b, const syntax::Expr* an, const syntax::Expr* bn) {
    using C = syntax::CmpOpKind;
    const std::uint32_t line = line_of(n);
    if (op == C::Is || op == C::IsNot || ((op == C::Eq || op == C::NotEq) && is_none_constant_node(bn))) {
        const bool positive = op == C::Is || op == C::Eq;
        Node* x = !is_none_constant_node(an) ? a : b;
        return py_call(positive ? "is_none" : "is_not_none", {x}, bool_type(), nullptr, line);
    }
    if (op == C::In || op == C::NotIn) {
        const bool negated = op == C::NotIn;
        const auto negate = [&](Node* e) { return negated ? mk(ir::Unary{.op = "!", .x = e, .postfix = false}, bool_type()) : e; };
        if (syntax::as<syntax::Tuple>(bn) != nullptr && !effects(a)) {   // `x in (1, 2)`: each compared
            const auto* literal = ir::get<ir::Call>(b);
            if (literal != nullptr && literal->sym->name == "tuple" && literal->sym->owner == std::optional<std::string>(std::string(kPy)) && !literal->args.empty()) {
                std::vector<Node*> tests;
                for (Node* x : literal->args) tests.push_back(mk(ir::Binary{.op = negated ? "!=" : "==", .left = a, .right = coerce(x, a->type)}, bool_type()));
                Node* e = tests[0];
                for (std::size_t i = 1; i < tests.size(); ++i) e = mk(ir::Binary{.op = negated ? "&&" : "||", .left = e, .right = tests[i]}, bool_type());
                return tests.size() > 1 ? mk(ir::Paren{.x = e}, bool_type()) : e;
            }
        }
        const Type& container = type_at(bn, line);
        if (container.kind == pt::Kind::Tuple) return negate(py_call("in", {a, b}, bool_type(), nullptr, line));   // `x in t` of a tuple: each item compared
        if (container.kind == pt::Kind::Dict && container.site && p.is_record(*container.site)) {
            const auto key = pyast::string_constant(an);
            const std::vector<RecordField> fields = ctx.records().fields(*container.site);
            const auto field = key ? std::find_if(fields.begin(), fields.end(), [&](const RecordField& f) { return f.key == *key; }) : fields.end();
            if (field == fields.end()) return lit(negated, bool_type());
            if (field->required) {
                if (effects(b)) refuse(RefusalClass::Syntax, "membership in a computed record", line);
                return lit(!negated, bool_type());
            }
            const ir::ClassType* owner = class_type_of(b->type);
            Node* selected = mk(ir::Select{.obj = b, .sym = ir::make_sym(ir::SymKind::Field, *key, owner != nullptr ? std::optional<std::string>(owner->name) : std::nullopt)}, py_class("py.optional"));
            return py_call(negated ? "is_none" : "is_not_none", {selected}, bool_type());
        }
        return negate(py_call("contains", {a}, bool_type(), b, line));
    }
    const char* o = op == C::Eq ? "==" : op == C::NotEq ? "!=" : op == C::Lt ? "<" : op == C::LtE ? "<=" : op == C::Gt ? ">" : ">=";
    if (is_opt(a->type) || is_opt(b->type)) {
        if (op == C::Eq || op == C::NotEq) return mk(ir::Binary{.op = o, .left = a, .right = b}, bool_type(), line);
        refuse(RefusalClass::Conflict, "ordering a value that may be None", line);
    }
    if (a->type == bool_type() && b->type == i64_type()) a = mk(ir::Cast{.x = a}, i64_type());
    if (b->type == bool_type() && a->type == i64_type()) b = mk(ir::Cast{.x = b}, i64_type());
    const std::string_view ka = kind_of(a->type), kb = kind_of(b->type);
    if ((ka == "i64" && kb == "f64") || (ka == "f64" && kb == "i64")) {
        a = as_double(a);
        b = as_double(b);
    }
    return mk(ir::Binary{.op = o, .left = a, .right = b}, bool_type(), line);
}

// ---- conditional expressions, strings, displays --------------------------------------------------------------------------------------

Node* Lowerer::e_if_exp(const syntax::IfExp* n) {
    const TypeP t = ty(type_at(n, line_of(n)), n);
    Node* test = cond(n->test);
    std::vector<Node*> saved = std::move(pre);
    pre.clear();
    Node* a = coerce(expr(n->body), t);
    Node* b = coerce(expr(n->orelse), t);
    if (!pre.empty()) refuse(RefusalClass::Syntax, "a conditional branch that needs statements", line_of(n));
    pre = std::move(saved);
    return mk(ir::Cond{.cond = test, .then = a, .orelse = b}, t, line_of(n));
}

Node* Lowerer::e_joined_str(const syntax::JoinedStr* n) {
    std::vector<Node*> parts;
    for (const syntax::Expr* v : n->values) {
        if (const auto* c = syntax::as<syntax::Constant>(v)) {
            if (!c->value.text.empty()) parts.push_back(lit(c->value.text, str_type()));
            continue;
        }
        const auto* formatted = syntax::as<syntax::FormattedValue>(v);
        if (formatted == nullptr) refuse(RefusalClass::Syntax, "f-string part", line_of(v));
        Node* x = expr(formatted->value);
        if (formatted->format_spec != nullptr) {
            std::string spec;
            if (const auto* spec_parts = syntax::as<syntax::JoinedStr>(formatted->format_spec))
                for (const syntax::Expr* piece : spec_parts->values)
                    if (const auto* c = syntax::as<syntax::Constant>(piece)) spec += c->value.text;
            parts.push_back(py_call("format", {x, lit(spec, str_type())}, str_type()));
        } else if (formatted->conversion == 114) {   // !r
            parts.push_back(py_call("repr", {x}, str_type()));
        } else if (x->type == str_type()) {
            parts.push_back(x);
        } else {
            parts.push_back(py_call("str", {x}, str_type()));
        }
    }
    if (parts.empty()) return lit(std::string(), str_type());
    if (parts.size() == 1) return parts[0]->type == str_type() ? parts[0] : py_call("str", {parts[0]}, str_type());
    return mk(ir::Concat{.parts = std::move(parts)}, str_type(), line_of(n));
}

Node* Lowerer::e_tuple(const syntax::Tuple* n) {
    const TypeP t = ty(type_at(n, line_of(n)), n);
    std::vector<Node*> items;
    for (const syntax::Expr* e : n->elts) items.push_back(expr(e));
    return py_call("tuple", std::move(items), t, nullptr, line_of(n));
}

Node* Lowerer::e_list(const syntax::List* n) {
    const TypeP t = ty(type_at(n, line_of(n)), n);
    std::vector<Node*> items;
    for (const syntax::Expr* e : n->elts) items.push_back(coerce(expr(e), class_type_of(t)->args.at(0)));
    return py_call("list", std::move(items), t, nullptr, line_of(n));
}

Node* Lowerer::e_set(const syntax::Set* n) {
    const TypeP t = ty(type_at(n, line_of(n)), n);
    std::vector<Node*> items;
    for (const syntax::Expr* e : n->elts) items.push_back(coerce(expr(e), class_type_of(t)->args.at(0)));
    return py_call("set", std::move(items), t, nullptr, line_of(n));
}

Node* Lowerer::e_dict(const syntax::Dict* n) {
    const Type& t = type_at(n, line_of(n));
    if (t.site && p.is_record(*t.site)) return record_value(n);
    const TypeP rt = ty(t, n);
    const std::vector<TypeP>& kv = class_type_of(rt)->args;
    std::vector<Node*> args;
    for (std::size_t i = 0; i < n->keys.size(); ++i) {
        if (n->keys[i] == nullptr) refuse(RefusalClass::Syntax, "** in a dict", line_of(n));
        args.push_back(py_call("tuple", {coerce(expr(n->keys[i]), kv.at(0)), coerce(expr(n->values[i]), kv.at(1))}, py_class("py.tuple", kv)));
    }
    return py_call("dict", std::move(args), rt, nullptr, line_of(n));
}

Node* Lowerer::lambda_expr(const syntax::Lambda* lam, const std::vector<TypeP>& ptypes) {
    std::vector<Node*> parameters;
    for (std::size_t i = 0; i < lam->args->args.size() && i < ptypes.size(); ++i) {
        const std::string& name = lam->args->args[i]->arg;
        local_types[name] = ptypes[i];
        parameters.push_back(mk(ir::Param{.sym = ir::make_sym(ir::SymKind::Param, name, qname, false, ptypes[i]), .type = ptypes[i]}));
    }
    std::vector<Node*> saved = std::move(pre);
    pre.clear();
    Node* body = expr(lam->body);
    if (!pre.empty()) refuse(RefusalClass::Syntax, "a lambda that needs statements", line_of(lam));
    pre = std::move(saved);
    return mk(ir::Lambda{.params = std::move(parameters), .body = body}, body->type);
}

// Whether evaluating `e` can do anything but read (a call, an assignment).
bool Lowerer::effects(Node* e) const {
    static const std::set<std::string, std::less<>> reads = {"tuple", "get", "unwrap", "truthy", "is_none", "is_not_none", "opt_value", "list", "str", "empty", "record", "len"};
    for (Node* x : ir::walk(e)) {
        if (ir::is<ir::Assign>(x) || ir::is<ir::New>(x)) return true;
        if (const auto* call = ir::get<ir::Call>(x)) {
            if (call->sym->owner == std::optional<std::string>(std::string(kPy)) && reads.contains(call->sym->name)) continue;
            return true;
        }
        if (ir::is<ir::Index>(x)) return true;   // can raise
    }
    return false;
}

}  // namespace cb::translate::frontends::python::lower
