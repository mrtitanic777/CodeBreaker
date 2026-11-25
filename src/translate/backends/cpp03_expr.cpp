// translate/backends/cpp03_expr.cpp -- the C++03 back end's expressions: each writes (C++ text, C++ type) (see cpp03.hpp). Calls are in
// cpp03_call.cpp.

#include <cmath>
#include <format>

#include "core/text.hpp"
#include "java/emit/naming.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace naming = cb::java::emit::naming;
namespace typemap = cb::java::typemap;
using detail::class_type;
using detail::kind_of;
using detail::need_sym;

namespace {
namespace expr_impl {

// int(v) of the value the front end put in a Lit (a Decline for one that is no whole number)
std::int64_t integer_of(const ir::LitValue& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    if (const auto* u = std::get_if<std::uint64_t>(&v)) return static_cast<std::int64_t>(*u);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1 : 0;
    if (const auto* d = std::get_if<double>(&v)) {
        if (std::floor(*d) == *d && std::fabs(*d) < 9.2e18) return static_cast<std::int64_t>(*d);
    }
    throw Decline("literal " + detail::format_value(v));
}

bool truthy(const ir::LitValue& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, std::string>) return !x.empty();
            else return x != 0;
        },
        v);
}

bool is_type_name(const ir::Node* n) { return ir::is<ir::TypeName>(n); }

}  // namespace expr_impl
}  // namespace

// ------------------------------------------------------------------ the dispatch
bool Writer::handles(const ir::Node* e) const {
    return ir::is<ir::Paren>(e) || ir::is<ir::Lit>(e) || ir::is<ir::Name>(e) || ir::is<ir::This>(e) || ir::is<ir::Select>(e) ||
           ir::is<ir::Length>(e) || ir::is<ir::Index>(e) || ir::is<ir::Unary>(e) || ir::is<ir::Binary>(e) || ir::is<ir::Concat>(e) ||
           ir::is<ir::Assign>(e) || ir::is<ir::Cond>(e) || ir::is<ir::Cast>(e) || ir::is<ir::InstanceOf>(e) || ir::is<ir::Call>(e) ||
           ir::is<ir::ClassLit>(e) || ir::is<ir::New>(e) || ir::is<ir::NewArray>(e);
}

Code Writer::dispatch(const ir::Node* e) {
    if (const auto* x = ir::get<ir::Paren>(e)) {
        const Code inner = expr(x->x);
        return {"(" + inner.code + ")", inner.ctype};
    }
    if (const auto* x = ir::get<ir::Lit>(e)) return write_lit(*e, *x);
    if (const auto* x = ir::get<ir::Name>(e)) return write_name(*x);
    if (const auto* x = ir::get<ir::This>(e)) {
        if (x->qualifier) throw Decline("qualified this");
        return {"this", cname_ + "*"};
    }
    if (const auto* x = ir::get<ir::Select>(e)) return write_select(*e, *x);
    if (const auto* x = ir::get<ir::Length>(e)) return write_length(*x);
    if (const auto* x = ir::get<ir::Index>(e)) return write_index(*e, *x);
    if (const auto* x = ir::get<ir::Unary>(e)) return write_unary(*e, *x);
    if (const auto* x = ir::get<ir::Binary>(e)) return write_binary(*e, *x);
    if (const auto* x = ir::get<ir::Concat>(e)) return write_concat(*x);
    if (const auto* x = ir::get<ir::Assign>(e)) return write_assign(*x);
    if (const auto* x = ir::get<ir::Cond>(e)) return write_cond(*e, *x);
    if (const auto* x = ir::get<ir::Cast>(e)) return write_cast(*e, *x);
    if (const auto* x = ir::get<ir::InstanceOf>(e)) return write_instance_of(*x);
    if (const auto* x = ir::get<ir::Call>(e)) return write_call(*e, *x);
    if (const auto* x = ir::get<ir::ClassLit>(e)) return write_class_lit(*x);
    if (const auto* x = ir::get<ir::New>(e)) return write_new(*e, *x);
    if (const auto* x = ir::get<ir::NewArray>(e)) return write_new_array(*e, *x);
    throw Decline("expression " + std::string(ir::kind_name(*e)));
}

Code Writer::expr(const ir::Suite& e) { return expr(detail::single(e)); }

Code Writer::expr(const ir::Node* e) {
    if (e != nullptr) {
        if (const auto it = subst_.find(e); it != subst_.end()) return it->second;
    }
    if (e == nullptr || !handles(e)) throw Decline("expression " + (e != nullptr ? std::string(ir::kind_name(*e)) : std::string("NoneType")));
    const std::string pre = order_ ? sequenced(e) : std::string();
    Code written = dispatch(e);
    if (!pre.empty()) return {"(" + pre + ", " + written.code + ")", written.ctype};
    return written;
}

std::string Writer::sequenced(const ir::Node* e) {
    const std::vector<const ir::Node*> nodes = operands(e);
    if (nodes.size() < 2 || !order_matters(nodes)) return "";
    std::vector<std::string> parts;
    for (const ir::Node* n : nodes) {
        if (is_constant(n) || subst_.contains(n)) continue;
        Code c = expr(n);
        if (c.ctype == "JCat") {   // a concatenation: as the String it makes
            c.code = "JavaString((" + c.code + ")._s)";
            c.ctype = "JavaString";
        }
        std::string ct = detail::strip_const_ref(c.ctype);
        if (ct == "char*" || ct == "char *") ct = "JavaString";   // text: held as the String it is
        if (ct == "void" || ct.empty()) return "";
        const std::string nm = std::format("j2xo{}", ++temps_);
        pending_.push_back(std::format("{} {} = {};", ct, nm, zero_of(ct)));
        parts.push_back(nm + " = " + c.code);
        subst_[n] = Code{nm, ct};
    }
    return detail::join_commas(parts);
}

std::string Writer::arg(const ir::Node* a) {
    const Code c = expr(a);
    return c.ctype == "JCat" ? "JavaString((" + c.code + ")._s)" : c.code;
}

std::vector<const ir::Node*> Writer::unmarked(std::span<ir::Node* const> args) const {
    std::vector<const ir::Node*> out;
    for (const ir::Node* a : args) {
        const auto* t = class_type(detail::type_of(a));
        const bool marker = t != nullptr && r_.data().markers.contains(detail::type_of(a)->simple());
        if (!marker) out.push_back(a);
    }
    return out;
}

std::string Writer::checked(const std::string& recv, const std::string& rtype) const {
    if (npe_ != 0 && is_pointer(rtype) && recv != "this" && !detail::reflection_types().contains(rtype))
        return "j2xNN<NullPointerException>(" + recv + ")";
    return recv;
}

// ------------------------------------------------------------------ literals and names
Code Writer::write_lit(const ir::Node& e, const ir::Lit& lit) {
    const ir::LitValue& v = lit.value;
    if (std::holds_alternative<std::monostate>(v)) return {"NULL", "void*"};
    const std::string_view k = kind_of(e);
    if (k == "bool") return {expr_impl::truthy(v) ? "true" : "false", "bool"};
    if (k == "i32") {
        const bool is_min = (std::holds_alternative<std::int64_t>(v) && std::get<std::int64_t>(v) == -2147483648LL) ||
                            (std::holds_alternative<double>(v) && std::get<double>(v) == -2147483648.0);
        if (is_min) return {"(-2147483647 - 1)", "int"};
        if (const auto* s = std::get_if<std::string>(&v)) return {*s, "int"};
        return {std::to_string(expr_impl::integer_of(v)), "int"};
    }
    if (k == "i64") {
        const bool is_min = (std::holds_alternative<std::int64_t>(v) && std::get<std::int64_t>(v) == INT64_MIN) ||
                            (std::holds_alternative<double>(v) && std::get<double>(v) == -9223372036854775808.0);
        if (is_min) return {"(-9223372036854775807LL - 1)", "long long"};
        return {std::to_string(expr_impl::integer_of(v)) + "LL", "long long"};
    }
    if (k == "f32") return {float_literal(v, "F"), "float"};
    if (k == "f64") return {float_literal(v, ""), "double"};
    if (k == "u16") {
        std::int64_t c = 0;
        if (const auto* s = std::get_if<std::string>(&v)) {
            const auto points = detail::code_points(*s);
            if (points.size() != 1) throw Decline("char literal " + detail::format_value(v));
            c = points[0];
        } else {
            c = expr_impl::integer_of(v);
        }
        if (c >= 0 && c <= UINT32_MAX) {
            if (const auto it = ctx_.char_map.find(static_cast<std::uint32_t>(c)); it != ctx_.char_map.end()) c = it->second;   // the program's font, as in its strings
        }
        if (32 <= c && c < 127 && c != '\\' && c != '\'') return {std::string("'") + static_cast<char>(c) + "'", "unsigned char"};
        if (c > 255) return {std::to_string(c), "int"};   // past the runtime's 8-bit char: its value as an int (Java's char is 16 bits)
        return {std::format("(unsigned char){}", c), "unsigned char"};
    }
    const auto* cls = class_type(e.type);
    if (cls != nullptr && cls->name == "java.lang.String") {
        const auto* s = std::get_if<std::string>(&v);
        if (s == nullptr) throw Decline("literal " + detail::format_value(v));
        return {c_string_literal(*s, ctx_.char_map), "const char*"};
    }
    throw Decline("literal " + detail::format_value(v));
}

Code Writer::write_name(const ir::Name& e) {
    const std::string n = name(need_sym(e.sym, "name").name);
    if (const auto it = alias.find(n); it != alias.end()) {
        const auto own = vars.find(n);
        std::string got = own != vars.end() ? own->second : "";
        if (got.empty()) {
            const auto target = vars.find(it->second);
            got = target != vars.end() ? target->second : "";
        }
        return {it->second, got};
    }
    const auto it = vars.find(n);
    if (it == vars.end()) throw Decline("unknown local " + n);
    return {n, it->second};
}

Code Writer::write_select(const ir::Node& e, const ir::Select& sel) {
    const ir::Sym& s = need_sym(sel.sym, "field");
    const std::optional<std::string> owner_o = s.owner_simple();
    const std::string owner = owner_o ? *owner_o : "None";
    if (s.kind == ir::SymKind::EnumConst) return {owner + "::" + naming::enum_member(s.name), ctype(e.type)};
    if (expr_impl::is_type_name(sel.obj)) {
        if (!ctx_.is_game(s.owner)) {
            if (s.owner && *s.owner == "java.lang.Math" && s.name == "PI") return {typemap::math_api().at("PI"), "double"};
            if (typemap::runtime_enum_like().contains(owner))   // Integer.MAX_VALUE, GL11.GL_BLEND, System.out
                return {owner + "::" + s.name, library_field(owner, s.name, e.type)};
            if (const auto field = ctx_.runtime_field(owner, s.name))   // a runtime class's static field, spelled as its header has it
                return {owner + "::" + field->second, field->first};
            throw Decline("library static " + owner + "." + s.name);
        }
        const std::optional<std::string> ct = ctx_.field_ctype(owner, s.name);
        if (!ct) throw Decline("static field " + owner + "." + s.name);
        if (r_.data().enums.contains(owner)) return {owner + "::" + s.name, *ct};   // an enum's statics keep their bare names
        return {owner + "::" + naming::static_field_name(s.name), *ct};
    }
    if (s.is_static) throw Decline("static field through an instance");
    Code receiver;
    if (ir::is<ir::This>(sel.obj) || ir::is<ir::Super>(sel.obj)) receiver = {"this", cname_ + "*"};   // super.field is this object's own
    else receiver = expr(sel.obj);
    const std::optional<std::string> base = pointee_class(receiver.ctype);
    std::optional<std::string> ct;
    if (ctx_.is_game(s.owner)) {
        ct = ctx_.field_ctype(base ? *base : owner, s.name);
        if (!ct || ct->empty()) ct = ctx_.field_ctype(owner, s.name);
    } else {
        std::optional<std::string> spelled;
        if (const auto field = ctx_.runtime_field(owner, s.name)) {   // a library object's field: as the runtime has it
            ct = field->first;
            spelled = field->second;
        }
        if (!spelled && (!ctx_.runtime_header(owner).empty() || typemap::value_stdlib().contains(owner))) {
            ct = ctype(e.type);   // a runtime type that holds it inline
            spelled = s.name;
        }
        if (!spelled || *spelled != s.name) throw Decline("library field " + owner + "." + s.name);
    }
    if (!ct) throw Decline("field " + owner + "." + s.name);
    return {checked(receiver.code, receiver.ctype) + (is_pointer(receiver.ctype) ? "->" : ".") + s.name, *ct};
}

Code Writer::write_length(const ir::Length& e) {
    if (const auto* call = ir::get<ir::Call>(e.arr)) {
        const std::optional<std::string> owner = call->sym ? call->sym->owner_simple() : std::nullopt;
        if (call->sym && call->sym->name == "values" && call->args.empty() && expr_impl::is_type_name(call->target) && owner &&
            r_.data().enums.contains(*owner) && !r_.data().typed_enums.contains(*owner))
            return {*owner + "::_J2X_COUNT", "int"};   // a constant-only enum's size
    }
    return {expr(e.arr).code + ".length", "int"};
}

Code Writer::write_index(const ir::Node& node, const ir::Index& e) {
    const bool target = lvalue_;
    lvalue_ = false;   // (the array and the index inside are values)
    const Code a = expr(e.arr);
    const Code i = expr(e.index);
    const std::string element = array_element(a.ctype);
    // A byte[] is an unsigned-char buffer, but an element READ is a Java byte: signed. Without the cast, `b[i] == (byte)-1` compares 255
    // with -1 (Indev's flood fill never cleared its 255 markers: a world with block 255 in it, and a crash at spawn), and `int v = b[i]`
    // or `b[i] < 0` differ for every byte >= 128. As an assignment's target it stays the plain element.
    if (!target && kind_of(node) == "i8" && element == "unsigned char") return {"((signed char)" + a.code + "[" + i.code + "])", "signed char"};
    return {a.code + "[" + i.code + "]", element};
}

Code Writer::write_unary(const ir::Node& e, const ir::Unary& u) {
    const bool incdec = u.op == "++" || u.op == "--";
    lvalue_ = u.postfix || incdec;   // (++ and -- change their operand)
    const Code x = expr(u.x);
    lvalue_ = false;
    if (u.postfix) return {x.code + u.op, x.ctype};
    return {u.op + x.code, incdec ? x.ctype : ctype(e.type)};
}

Code Writer::write_binary(const ir::Node& e, const ir::Binary& b) {
    const Code l = expr(b.left);
    const Code r = expr(b.right);
    std::string lc = l.code;
    std::string rc = r.code;
    if (detail::bit_operators().contains(b.op)) {   // `a >> 16 & 255` is the same in Java and C++, but MSVC warns (C4554): group it
        const auto* lb = ir::get<ir::Binary>(b.left);
        if (lb != nullptr && lb->op != b.op) lc = "(" + lc + ")";
        const auto* rb = ir::get<ir::Binary>(b.right);
        if (rb != nullptr && rb->op != b.op) rc = "(" + rc + ")";
    }
    if (b.op == ">>>") {
        const std::string_view k = kind_of(e);
        // static_cast, not a C cast: the converter's finalize pass turns `(int)(...)` into the saturating float->int helper, and a
        // shifted bit pattern must wrap, not saturate
        if (k == "i32") return {std::format("static_cast<int>(static_cast<unsigned int>({}) >> ({}))", lc, rc), "int"};
        if (k == "i64") return {std::format("static_cast<long long>(static_cast<unsigned long long>({}) >> ({}))", lc, rc), "long long"};
        throw Decline(">>> on " + std::string(k));
    }
    // (a String == String compares the text, as the text path does: identity has no meaning for the by-value JavaString)
    if ((b.op == "==" || b.op == "!=") && l.ctype != r.ctype && (detail::has(l.ctype, "JObject") || detail::has(r.ctype, "JObject")) &&
        l.ctype != "void*" && r.ctype != "void*") {
        const std::string eq = std::format("j2xIdentEq({}, {})", lc, rc);   // identity across a box, as the text path
        return {b.op == "==" ? eq : "!" + eq, "bool"};
    }
    if (b.op == "%" && (kind_of(e) == "f32" || kind_of(e) == "f64")) {   // Java's floating remainder: C's fmod
        const std::string fmt = kind_of(e) == "f32" ? std::format("(float)fmod((double)({}), (double)({}))", lc, rc) : std::format("fmod({}, {})", lc, rc);
        return {fmt, ctype(e.type)};
    }
    return {lc + " " + b.op + " " + rc, ctype(e.type)};
}

Code Writer::write_concat(const ir::Concat& e) {
    std::vector<std::string> parts;
    for (const ir::Node* p : e.parts) parts.push_back(expr(p).code);
    return {"(JCat() + " + text::join(parts, " + ") + ")", "JCat"};
}

Code Writer::write_assign(const ir::Assign& e) {
    lvalue_ = true;
    const Code t = expr(e.target);
    lvalue_ = false;
    const Code v = expr(e.value);
    if (!e.op) return {t.code + " = " + unbox(coerce(v.code, t.ctype), v.ctype, t.ctype), t.ctype};
    const ir::TypeP target_type = detail::type_of(e.target);
    if (*e.op == ">>>") {
        const std::string_view k = kind_of(target_type);
        bool plain = ir::is<ir::Name>(e.target);
        if (const auto* sel = ir::get<ir::Select>(e.target)) plain = plain || ir::is<ir::This>(sel->obj) || ir::is<ir::TypeName>(sel->obj);
        if (!(k == "i32" || k == "i64") || !plain)   // evaluated twice below: no side effects allowed
            throw Decline(">>>=");
        const char* wide = k == "i32" ? "unsigned int" : "unsigned long long";
        const char* narrow = k == "i32" ? "int" : "long long";
        return {std::format("{} = static_cast<{}>(static_cast<{}>({}) >> ({}))", t.code, narrow, wide, t.code, v.code), t.ctype};
    }
    if (*e.op == "%" && (kind_of(target_type) == "f32" || kind_of(target_type) == "f64")) throw Decline("floating %=");
    return {t.code + " " + *e.op + "= " + v.code, t.ctype};
}

Code Writer::write_cond(const ir::Node& e, const ir::Cond& c) {
    const std::string test = cond(c.cond);
    const Code a = expr(c.then);
    const Code b = expr(c.orelse);
    if (kind_of(e) == "null") return {std::format("({} ? {} : {})", test, a.code, b.code), a.ctype};
    const std::string t = ctype(e.type);
    if (a.ctype != b.ctype) {   // Java converts both arms to the result type
        return {std::format("({} ? {} : {})", test, arm(a.code, a.ctype, t), arm(b.code, b.ctype, t)), t};
    }
    return {std::format("({} ? {} : {})", test, a.code, b.code), t};
}

std::string Writer::arm(const std::string& code, const std::string& ctype_of_arm, const std::string& target) {
    if (code == "NULL") return null_of(target);
    if (ctype_of_arm == target) return code;
    if (target == "JavaString") return ctype_of_arm == "JCat" ? "JavaString((" + code + ")._s)" : "JavaString(" + code + ")";
    return "(" + target + ")(" + code + ")";
}

Code Writer::write_cast(const ir::Node& e, const ir::Cast& c) {
    const Code x = expr(c.x);
    if (x.ctype.starts_with("JObject") && class_type(e.type) != nullptr && typemap::boxed_value_types().contains(e.type->simple()))
        return {x.code, "JObject"};   // (Integer)o stays boxed: o.intValue() is JObject's
    const std::string ct = ctype(e.type);
    if (x.ctype.starts_with("JObject") && detail::starts_with_any_collection(ct))   // (List)o: the boxed list itself, not a copy
        return {std::format("j2xUnboxRef<{} >({})", ct, x.code), ct};
    if (const auto it = detail::saturating_casts().find(ct); it != detail::saturating_casts().end())   // Java's (int)/(long) of a float
        return {std::format("{}({})", it->second, x.code), ct};                                          // saturates; C++'s is undefined out of range
    if (e.type && e.type->is_prim() && !detail::is_simple_chain(x.code)) return {std::format("({})({})", ct, x.code), ct};   // grouped
    return {"(" + ct + ")" + x.code, ct};   // as the text path: the reflection pass reads `(T*)recv->newInstance()`
}

Code Writer::write_instance_of(const ir::InstanceOf& e) {
    const Code x = expr(e.x);
    const jir::JType target = jtype(e.test);
    if (target.is_array() && target.array_dims == 1 && target.name == "String" && x.ctype.starts_with("JObject"))
        return {"(" + x.code + ").isStringArray()", "bool"};
    if (!target.is_array()) {
        if (const auto it = detail::boxed_tests().find(target.name); it != detail::boxed_tests().end()) {
            if (x.ctype.starts_with("JObject")) return {"(" + x.code + ")." + it->second + "()", "bool"};
            throw Decline("instanceof " + target.name);
        }
    }
    if (target.is_array() || !r_.is_pointer(target)) throw Decline("instanceof " + target.name);
    if (is_pointer(x.ctype)) return {std::format("(dynamic_cast<{}*>({}) != NULL)", target.name, x.code), "bool"};
    if (x.ctype.starts_with("JObject")) return {std::format("({}).is<{}>()", x.code, target.name), "bool"};
    throw Decline("instanceof on " + x.ctype);
}

Code Writer::write_class_lit(const ir::ClassLit& e) {
    if (!e.of || e.of->kind() != "class") throw Decline("class literal of " + (e.of ? ir::to_text(*e.of) : std::string("None")));
    const std::string simple = e.of->simple();
    const auto it = detail::class_literal_types().find(simple);
    return {std::format("j2xClassOf(typeid({}))", it != detail::class_literal_types().end() ? it->second : simple), "Class*"};
}

Code Writer::write_new(const ir::Node& e, const ir::New& n) {
    if (n.body != nullptr || n.outer != nullptr) throw Decline("anonymous or inner class");
    const jir::JType jt = jtype(e.type);
    if (r_.data().values.contains(jt.name)) throw Decline("value type " + jt.name);
    std::vector<std::string> parts;
    for (const ir::Node* a : unmarked(n.args)) parts.push_back(arg(a));
    const std::string args = detail::join_commas(parts);
    const auto* cls = class_type(e.type);
    if (cls != nullptr && ctx_.is_game(cls->name)) {
        if (r_.extends_value_container(jt.name))   // a container subclass IS the container value
            return {std::format("(*(new {}({})))", jt.name, args), jt.name};
        return {std::format("new {}({})", jt.name, args), jt.name + "*"};
    }
    const std::string ct = r_.cpp_type(jt, cb::java::types::Use::Local);
    if (typemap::boxed_value_types().contains(jt.name)) return {std::format("(({})({}))", ct, args), ct};   // new Integer(5): a boxed type is its primitive
    if (is_pointer(ct)) return {std::format("new {}({})", text::trim(std::string_view(ct).substr(0, ct.size() - 1)), args), ct};
    return {std::format("{}({})", ct, args), ct};
}

Code Writer::write_new_array(const ir::Node& e, const ir::NewArray& n) {
    const std::string ct = ctype(e.type);
    if (!n.init.empty()) {
        if (!n.dims.empty()) throw Decline("sized initialised array");
        std::vector<std::string> parts;
        for (const ir::Node* x : n.init) parts.push_back(expr(x).code);
        return {std::format("(JArrayInit<{} >(), {})", array_element(ct), detail::join_commas(parts)), ct};
    }
    if (n.dims.empty()) return {ct + "()", ct};   // `new T[]{}`: an empty array (`(JArrayInit<T >(), )` would not be C++)
    const std::size_t total = ir::dims(e.type).first;
    std::vector<std::string> dims;
    for (const ir::Node* d : n.dims) dims.push_back(expr(d).code);
    if (dims.size() == 1) return {std::format("{}({})", ct, dims[0]), ct};   // new T[n], or jagged new T[n][]: n empty rows
    if (dims.size() == total && (total == 2 || total == 3)) {                // new T[a][b]([c]): the runtime's nested builders
        std::string inner = ct;
        for (std::size_t i = 0; i < total; ++i) inner = array_element(inner);
        return {std::format("j2x_newarr{}<{} >({})", total, inner, detail::join_commas(dims)), ct};
    }
    throw Decline("multi-dimensional array");
}

}  // namespace cb::translate::backends::cpp03
