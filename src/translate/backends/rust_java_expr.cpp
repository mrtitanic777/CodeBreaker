// translate/backends/rust_java_expr.cpp -- expressions of the Rust back end for Java: literals and names, operators with Java's arithmetic,
// assignments and increments, casts, fields and arrays (calls and `new` are in rust_java_call.cpp; see rust_java.hpp).

#include <format>

#include "translate/backends/rust_java_internal.hpp"

namespace cb::translate::backends::rust_java::detail {

namespace {

Code mk(std::string text, ir::TypeP type, int prec = kAtom) {
    Code c;
    c.text = std::move(text);
    c.type = std::move(type);
    c.prec = prec;
    return c;
}

// The name of a Rust numeric type for a Java kind.
std::string num_type(std::string_view kind) { return std::string(kind); }

// An integer or floating-point literal's text with the type it is given when nothing else says (a receiver of a method call needs one).
std::string suffixed(const Code& c) {
    const std::string kind(kind_of(c.type));
    const bool float_literal = !c.bare && kind == "f64" && !c.text.empty() && c.text.find_first_not_of("0123456789.e+-") == std::string::npos &&
                               c.text.find_first_of("0123456789") != std::string::npos;
    if (!c.bare && !float_literal) return c.text;
    if (c.text.starts_with("-")) return "(" + c.text + kind + ")";
    return c.text + kind;
}

bool is_comparison(const std::string& op) { return op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">="; }

std::string_view class_kind_name(const ir::TypeP& t) { return kind_of(t); }

}  // namespace

// The class declaring field `name`, from `cls` up.
static const ClassInfo* declaring_class(const Program& p, const ClassInfo& cls, std::string_view name) {
    const auto declares = [&](const ClassInfo& c) {
        for (const ir::Node* m : c.members())
            if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym && f->sym->name == name) return true;
        return false;
    };
    if (declares(cls)) return &cls;
    for (const ClassInfo* a : p.ancestors(cls))
        if (declares(*a)) return a;
    return nullptr;
}

std::string receiver_text(const Code& c) {
    const std::string text = suffixed(c);
    return text == c.text ? (c.prec >= kAtom ? text : "(" + text + ")") : text;
}

Code Writer::numeric(const ir::Node* e) {
    Code c = expr(e);
    if (const std::string_view box = boxed_kind(c.type); !box.empty()) return conv(std::move(c), prim_by_kind(box));
    return c;
}

static ir::TypeP eff(const ir::TypeP& t) {
    const std::string_view box = boxed_kind(t);
    return box.empty() ? t : prim_by_kind(box);
}

std::string Writer::wrap(const Code& c, int min_prec) const { return c.prec >= min_prec ? c.text : "(" + c.text + ")"; }

bool Writer::is_subtype(std::string_view a, std::string_view b) const { return p_.model().is_sub(a, b); }

// ---------------------------------------------------------------------------------------------------------------- conversion

Code Writer::conv(Code value, const ir::TypeP& dst) {
    if (!dst || !value.type) return value;
    const std::string_view sk = kind_of(value.type), dk = kind_of(dst);
    if (dk == "void") return value;
    if (sk == "null") {
        if (dk == "class" && class_name(dst) == "java.lang.String") return mk("Str::NULL", dst);
        if (dk == "class" || dk == "array") return mk("None", dst);
        throw Decline("null where a " + std::string(dk) + " is wanted");
    }
    if (is_numeric_kind(sk) && is_numeric_kind(dk)) {
        if (sk == dk) return value;
        if (!value.chr.empty()) return mk(value.chr + " as " + num_type(dk), dst, kCastPrec);
        if (value.bare) {
            if (is_integral_kind(dk)) {
                value.type = dst;
                return value;
            }
            value.text += ".0";
            if (dk == "f32") value.text += "f32";
            value.type = dst;
            value.bare = false;
            value.prec = value.text.starts_with("-") ? kUnary : kAtom;
            return value;
        }
        return mk(wrap(value, kUnary) + " as " + num_type(dk), dst, kCastPrec);
    }
    if (sk == "bool" && dk == "bool") return value;
    if (const std::string_view box = boxed_kind(dst); !box.empty() && is_prim_kind(sk)) {   // boxing: a number is a Some
        const Code prim = conv(std::move(value), prim_by_kind(box));
        return mk("Some(" + prim.text + ")", dst);
    }
    if (const std::string_view box = boxed_kind(value.type); !box.empty() && is_prim_kind(dk)) {   // unboxing: null throws
        return conv(mk(receiver_text(value) + ".unbox()", prim_by_kind(box)), dst);
    }
    if (!boxed_kind(value.type).empty() && boxed_kind(value.type) == boxed_kind(dst)) {
        value.type = dst;
        return value;
    }
    if ((sk == "class" || sk == "array") && (dk == "class" || dk == "array")) return class_conv(std::move(value), dst);
    if (is_prim_kind(sk) && (dk == "class")) throw Decline("boxing a " + std::string(sk) + " as " + std::string(class_name(dst)));
    if (sk == "class" && is_prim_kind(dk)) throw Decline("unboxing " + std::string(class_name(value.type)));
    return value;
}

Code Writer::class_conv(Code value, const ir::TypeP& dst) {
    const std::string have = p_.rtype(value.type), want = p_.rtype(dst);
    if (have == want) {
        value.type = dst;
        return value;
    }
    if (kind_of(dst) == "array" || kind_of(value.type) == "array") throw Decline("converting " + have + " to " + want);
    if (class_name(value.type) == "java.lang.String" || class_name(dst) == "java.lang.String") throw Decline("a String as " + want);
    // a reference as one of its bases or interfaces
    std::string trait;
    if (class_name(dst) == "java.lang.Object") trait = "Object";
    else if (const ClassInfo* c = p_.program_class(dst)) trait = p_.trait_name(*c);
    else if (p_.is_lib_throwable(class_name(dst))) trait = "Throwable";
    else throw Decline("converting " + have + " to " + want);
    return mk("up!(" + value.text + ", dyn " + trait + ")", dst);
}

// ---------------------------------------------------------------------------------------------------------------- dispatch

Code Writer::expr(const ir::Node* e) {
    if (e == nullptr) throw Decline("an expression that is missing");
    return ir::visit(*e, [&](const auto& k) -> Code { return on(*e, k); });
}

template <class K> Code Writer::on(const ir::Node& n, const K&) {
    throw Decline("the expression " + std::string(ir::kind_name(n)));
}

Code Writer::on(const ir::Node&, const ir::Paren& k) { return expr(k.x); }

Code Writer::on(const ir::Node& n, const ir::Lit& k) {
    const ir::LitValue& v = k.value;
    const ir::TypeP type = n.type;
    const std::string_view kind = kind_of(type);
    if (std::holds_alternative<std::monostate>(v)) return mk("None", ir::null_type());
    if (kind == "bool") {
        const bool b = std::holds_alternative<bool>(v) ? std::get<bool>(v) : (std::holds_alternative<std::int64_t>(v) && std::get<std::int64_t>(v) != 0);
        return mk(b ? "true" : "false", type);
    }
    if (kind == "i8" || kind == "i16" || kind == "i32" || kind == "i64") {
        std::string digits;
        std::int64_t as_int = 0;
        bool fits = true;
        if (const auto* i = std::get_if<std::int64_t>(&v)) {
            digits = std::to_string(*i);
            as_int = *i;
        } else if (const auto* u = std::get_if<std::uint64_t>(&v)) {
            digits = std::to_string(*u);
            fits = *u <= 2147483648ull;
        } else {
            throw Decline("an integer literal that is not a number");
        }
        Code c = mk(digits, type, digits.starts_with("-") ? kUnary : kAtom);
        c.bare = true;
        if (!fits || as_int > 2147483647LL || as_int < -2147483648LL) {   // too big to be read as an i32 when nothing says otherwise
            c.text += std::string(kind);
            c.bare = false;
        }
        return c;
    }
    if (kind == "f64" || kind == "f32") {
        double d = 0;
        if (const auto* f = std::get_if<double>(&v)) d = *f;
        else if (const auto* i = std::get_if<std::int64_t>(&v)) d = static_cast<double>(*i);
        else if (const auto* u = std::get_if<std::uint64_t>(&v)) d = static_cast<double>(*u);
        else if (const auto* s = std::get_if<std::string>(&v)) {   // the front end writes what is not finite as text
            if (*s == "NaN") d = std::nan("");
            else if (*s == "Infinity") d = HUGE_VAL;
            else if (*s == "-Infinity") d = -HUGE_VAL;
            else d = std::stod(*s);
        }
        const std::string text = float_text(d, kind == "f32");
        return mk(text, type, text.starts_with("-") ? kUnary : kAtom);
    }
    if (kind == "u16") {
        std::uint32_t code = 0;
        if (const auto* s = std::get_if<std::string>(&v)) {
            if (s->empty()) throw Decline("an empty char literal");
            const unsigned char c0 = static_cast<unsigned char>((*s)[0]);
            if (c0 < 0x80) code = c0;
            else {
                std::size_t len = c0 >= 0xF0 ? 4 : c0 >= 0xE0 ? 3 : 2;
                code = len == 2 ? c0 & 0x1F : len == 3 ? c0 & 0x0F : c0 & 0x07;
                for (std::size_t i = 1; i < len && i < s->size(); ++i) code = (code << 6) | (static_cast<unsigned char>((*s)[i]) & 0x3F);
            }
        } else if (const auto* i = std::get_if<std::int64_t>(&v)) {
            code = static_cast<std::uint32_t>(*i);
        } else {
            throw Decline("a char literal that is not a character");
        }
        if (code > 0xFFFF) throw Decline("a char literal above U+FFFF");
        Code c;
        c.type = type;
        if (code >= 0x20 && code < 0x7F && code != '\'' && code != '\\') c.chr = std::string("'") + static_cast<char>(code) + "'";
        else if (code == '\'') c.chr = "'\\''";
        else if (code == '\\') c.chr = "'\\\\'";
        else if (code == '\n') c.chr = "'\\n'";
        else if (code == '\t') c.chr = "'\\t'";
        else if (code == '\r') c.chr = "'\\r'";
        else if (code == 0) c.chr = "'\\0'";
        else if (code < 0xD800 || code > 0xDFFF) c.chr = std::format("'\\u{{{:x}}}'", code);
        if (c.chr.empty()) {   // a lone surrogate has no char literal
            c.text = std::to_string(code) + "u16";
            return c;
        }
        c.text = c.chr + " as u16";
        c.prec = kCastPrec;
        return c;
    }
    if (const auto* s = std::get_if<std::string>(&v)) return mk("lit(" + rs_string(*s) + ")", ir::class_type("java.lang.String"));
    throw Decline("a literal of type " + std::string(kind));
}

Code Writer::on(const ir::Node& n, const ir::Name& k) {
    const ir::Sym& sym = need_sym(k.sym, "a name");
    ir::TypeP type = n.type ? n.type : sym.type;
    if (sym.kind != ir::SymKind::Local && sym.kind != ir::SymKind::Param) throw Decline("the name " + sym.name + " is not a local or parameter");
    return mk(ident(sym.name), type);
}

Code Writer::this_code() {
    if (static_) throw Decline("this in a static context");
    return mk("Some(this)", ir::class_type(ci_->qname));
}

Code Writer::on(const ir::Node&, const ir::This& k) {
    if (k.qualifier && class_name(k.qualifier) != ci_->qname && !is_subtype(ci_->qname, class_name(k.qualifier))) throw Decline("Outer.this");
    return this_code();
}

// The object a call or field access goes through, as a `&'static` reference (null throws).
Code Writer::receiver(const ir::Node* target) {
    if (target == nullptr || ir::is<ir::This>(strip_paren(target)) || ir::is<ir::Super>(strip_paren(target))) {
        if (static_) throw Decline("this in a static context");
        return mk("this", ir::class_type(ci_->qname));
    }
    Code c = expr(target);
    const std::string_view k = kind_of(c.type);
    if (k == "class") {
        const std::string_view name = class_name(c.type);
        if (name == "java.lang.String" || name == "java.io.PrintStream") return c;
        return mk(wrap(c, kAtom) + ".nn()", c.type);
    }
    return c;
}

Code Writer::on(const ir::Node& n, const ir::Select& k) {
    const ir::Sym& sym = need_sym(k.sym, "a field");
    if (sym.kind == ir::SymKind::EnumConst) {
        const ClassInfo* owner = p_.model().info(sym.owner.value_or(""));
        if (owner == nullptr) throw Decline("the library enum constant " + sym.name);
        return mk(p_.static_field_name(*owner, sym.name) + "::get()", n.type ? n.type : sym.type);
    }
    if (sym.kind != ir::SymKind::Field) throw Decline("the member " + sym.name + " is not a field");
    const ir::TypeP type = n.type ? n.type : sym.type;
    const ClassInfo* owner = p_.model().info(sym.owner.value_or(""));
    if (owner == nullptr) return lib_static_field(k, type);
    const ClassInfo* dc = declaring_class(p_, *owner, sym.name);
    if (dc == nullptr) throw Decline("the field " + sym.name + " is not declared by " + owner->qname);
    const ir::FieldDecl* decl = nullptr;
    for (const ir::Node* m : dc->members())
        if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym && f->sym->name == sym.name) decl = f;
    const bool stat = sym.is_static || ir::is<ir::TypeName>(k.obj) || (decl != nullptr && has_mod(decl->mods, "static"));
    if (stat) {
        if (decl != nullptr && p_.is_const_field(*decl)) return mk(p_.holder(*dc) + "::" + member_ident(sym.name), type);
        return mk(p_.static_field_name(*dc, sym.name) + "::get()", type);
    }
    const Code recv = receiver(k.obj);
    return mk(wrap(recv, kAtom) + "." + p_.fields_fn(*dc) + "()." + member_ident(sym.name) + ".get()", type);
}

Code Writer::on(const ir::Node&, const ir::Length& k) {
    const Code a = expr(k.arr);
    return mk(wrap(a, kAtom) + ".length()", ir::i32_type());
}

Code Writer::on(const ir::Node& n, const ir::Index& k) {
    if (k.sym) throw Decline("an indexer");
    const std::vector<ir::Node*> idx = ir::statements(k.index);
    if (idx.size() != 1) throw Decline("an index with several arguments");
    const Code a = expr(k.arr);
    const Code i = conv(expr(idx[0]), ir::i32_type());
    return mk(wrap(a, kAtom) + ".at(" + i.text + ")", n.type ? n.type : elem_of(a.type));
}

// ---------------------------------------------------------------------------------------------------------------- operators

Code Writer::on(const ir::Node&, const ir::Unary& k) {
    const std::string& op = k.op;
    if (op == "++" || op == "--") return incdec(k, true);
    if (op == "!") {
        const Code x = conv(expr(k.x), ir::bool_type());
        return mk("!" + wrap(x, kUnary), ir::bool_type(), kUnary);
    }
    const ir::TypeP pt = unary_promoted(eff(type_of(k.x)));
    if (op == "-") {
        const ir::Node* operand = strip_paren(k.x);
        if (const auto* lit = ir::get<ir::Lit>(operand); lit != nullptr && is_numeric_kind(kind_of(type_of(operand)))) {   // a negative literal
            Code c = conv(expr(operand), pt);
            if (c.chr.empty() && !c.text.starts_with("-")) {
                c.text = "-" + c.text;
                c.prec = kUnary;
                if (c.bare) {
                    std::int64_t v = 0;
                    if (const auto* i = std::get_if<std::int64_t>(&lit->value)) v = *i;
                    if (v == 2147483648LL) c.text = "-2147483648";
                }
                return c;
            }
        }
        const Code x = conv(expr(k.x), pt);
        if (is_float_kind(kind_of(pt))) return mk("-" + wrap(x, kUnary), pt, kUnary);
        return mk(wrap(Code{suffixed(x), x.type, x.bare ? kAtom : x.prec}, kAtom) + ".wrapping_neg()", pt);
    }
    if (op == "+") return conv(expr(k.x), pt);
    if (op == "~") {
        const Code x = conv(expr(k.x), pt);
        return mk("!" + wrap(x, kUnary), pt, kUnary);
    }
    throw Decline("the unary operator " + op);
}

// The Java arithmetic of `l op r` on the promoted type.
Code Writer::arith(const std::string& op, const Code& l0, const Code& r0, const ir::TypeP&, const ir::Node*) {
    const std::string_view lk = kind_of(l0.type), rk = kind_of(r0.type);
    if (lk == "bool" && rk == "bool") {
        if (op == "&") return mk(wrap(l0, kBitAnd) + " & " + wrap(r0, kBitAnd + 1), ir::bool_type(), kBitAnd);
        if (op == "|") return mk(wrap(l0, kBitOr) + " | " + wrap(r0, kBitOr + 1), ir::bool_type(), kBitOr);
        if (op == "^") return mk(wrap(l0, kBitXor) + " ^ " + wrap(r0, kBitXor + 1), ir::bool_type(), kBitXor);
        throw Decline("the operator " + op + " on booleans");
    }
    if (!is_numeric_kind(lk) || !is_numeric_kind(rk)) throw Decline("the operator " + op + " on " + std::string(lk) + " and " + std::string(rk));
    if (op == "<<" || op == ">>" || op == ">>>") {
        if (!is_integral_kind(lk) || !is_integral_kind(rk)) throw Decline("a shift of a non-integer");
        const ir::TypeP pt = unary_promoted(l0.type);
        const Code a = conv(l0, pt);
        std::string count;
        if (r0.bare) count = r0.text;
        else count = wrap(conv(r0, kind_of(r0.type) == "i64" || kind_of(r0.type) == "i32" ? r0.type : ir::i32_type()), kUnary) + " as u32";
        const std::string recv = wrap(Code{suffixed(a), pt, a.bare ? kAtom : a.prec}, kAtom);
        if (op == "<<") return mk(recv + ".wrapping_shl(" + count + ")", pt);
        if (op == ">>") return mk(recv + ".wrapping_shr(" + count + ")", pt);
        const std::string unsigned_type = kind_of(pt) == "i64" ? "u64" : "u32";
        return mk("(" + recv + " as " + unsigned_type + ").wrapping_shr(" + count + ") as " + std::string(kind_of(pt)), pt, kCastPrec);
    }
    const ir::TypeP pt = promoted(l0.type, r0.type);
    const Code a = conv(l0, pt), b = conv(r0, pt);
    if (is_float_kind(kind_of(pt))) {
        if (op == "+" || op == "-") return mk(wrap(a, kAdd) + " " + op + " " + wrap(b, kAdd + 1), pt, kAdd);
        if (op == "*" || op == "/" || op == "%") return mk(wrap(a, kMul) + " " + op + " " + wrap(b, kMul + 1), pt, kMul);
        throw Decline("the operator " + op + " on floating point");
    }
    const std::string recv = wrap(Code{suffixed(a), pt, a.bare ? kAtom : a.prec}, kAtom);
    if (op == "+") return mk(recv + ".wrapping_add(" + b.text + ")", pt);
    if (op == "-") return mk(recv + ".wrapping_sub(" + b.text + ")", pt);
    if (op == "*") return mk(recv + ".wrapping_mul(" + b.text + ")", pt);
    if (op == "/" || op == "%") {
        // a divisor that is a nonzero literal other than -1 cannot fail; any other goes through the check that throws
        if (b.bare && b.text != "0" && b.text != "-1" && !b.text.starts_with("-")) return mk(wrap(a, kMul) + " " + op + " " + wrap(b, kMul + 1), pt, kMul);
        return mk(recv + (op == "/" ? ".jdiv(" : ".jrem(") + b.text + ")", pt);
    }
    if (op == "&") return mk(wrap(a, kBitAnd) + " & " + wrap(b, kBitAnd + 1), pt, kBitAnd);
    if (op == "|") return mk(wrap(a, kBitOr) + " | " + wrap(b, kBitOr + 1), pt, kBitOr);
    if (op == "^") return mk(wrap(a, kBitXor) + " ^ " + wrap(b, kBitXor + 1), pt, kBitXor);
    throw Decline("the operator " + op);
}

Code Writer::on(const ir::Node& n, const ir::Binary& k) {
    const std::string& op = k.op;
    if (op == "&&" || op == "||") {
        const Code l = conv(expr(k.left), ir::bool_type()), r = conv(expr(k.right), ir::bool_type());
        const int prec = op == "&&" ? kAnd : kOr;
        return mk(wrap(l, prec) + " " + op + " " + wrap(r, prec + 1), ir::bool_type(), prec);
    }
    if (op == "==" || op == "!=") {
        const ir::TypeP lt0 = type_of(k.left), rt0 = type_of(k.right);
        if (!boxed_kind(lt0).empty() && !boxed_kind(rt0).empty()) {   // two boxes: the same object, which for the cached small ones is the same value
            if (boxed_kind(lt0) != boxed_kind(rt0)) throw Decline("== on boxes of two kinds");
            const std::string test = "box_same(" + expr(k.left).text + ", " + expr(k.right).text + ")";
            return op == "==" ? mk(test, ir::bool_type()) : mk("!" + test, ir::bool_type(), kUnary);
        }
        const ir::TypeP lt = kind_of(rt0) == "null" ? lt0 : eff(lt0), rt = kind_of(lt0) == "null" ? rt0 : eff(rt0);
        const bool l_null = kind_of(lt) == "null", r_null = kind_of(rt) == "null";
        const std::string_view lk = kind_of(lt), rk = kind_of(rt);
        const bool eq = op == "==";
        if (l_null || r_null) {
            const Code other = expr(l_null ? k.right : k.left);
            const bool is_str = class_name(other.type) == "java.lang.String";
            if (is_prim_kind(kind_of(other.type))) throw Decline("comparing a primitive with null");
            const std::string test = wrap(other, kAtom) + (is_str ? ".is_null()" : eq ? ".is_none()" : ".is_some()");
            if (is_str && !eq) return mk("!" + test, ir::bool_type(), kUnary);
            return mk(test, ir::bool_type());
        }
        if (is_numeric_kind(lk) && is_numeric_kind(rk)) {
            const ir::TypeP pt = promoted(lt, rt);
            const Code l = conv(numeric(k.left), pt), r = conv(numeric(k.right), pt);
            return mk(wrap(l, kBitOr) + " " + op + " " + wrap(r, kBitOr), ir::bool_type(), kCmp);
        }
        if (lk == "bool" && rk == "bool") {
            const Code l = numeric(k.left), r = numeric(k.right);
            return mk(wrap(l, kBitOr) + " " + op + " " + wrap(r, kBitOr), ir::bool_type(), kCmp);
        }
        if ((lk == "class" || lk == "array") && (rk == "class" || rk == "array")) {
            const Code l = expr(k.left), r = expr(k.right);
            const bool str = class_name(lt) == "java.lang.String" && class_name(rt) == "java.lang.String";
            if ((class_name(lt) == "java.lang.String") != (class_name(rt) == "java.lang.String")) throw Decline("comparing a String with another type by ==");
            const std::string test = std::string(str ? "same_str(" : "same(") + l.text + ", " + r.text + ")";
            return eq ? mk(test, ir::bool_type()) : mk("!" + test, ir::bool_type(), kUnary);
        }
        throw Decline("comparing " + std::string(lk) + " with " + std::string(rk));
    }
    if (is_comparison(op)) {
        const ir::TypeP pt = promoted(eff(type_of(k.left)), eff(type_of(k.right)));
        const Code l = conv(numeric(k.left), pt), r = conv(numeric(k.right), pt);
        return mk(wrap(l, kBitOr) + " " + op + " " + wrap(r, kBitOr), ir::bool_type(), kCmp);
    }
    if (op == "+" && class_name(n.type) == "java.lang.String") {   // (the front end normally makes this a Concat)
        const Code l = expr(k.left), r = expr(k.right);
        return mk("cat!(" + l.text + ", " + r.text + ")", n.type);
    }
    return arith(op, numeric(k.left), numeric(k.right), n.type, k.right);
}

Code Writer::on(const ir::Node&, const ir::Concat& k) {
    std::vector<std::string> parts;
    for (const ir::Node* p : k.parts) {
        const Code c = expr(p);
        if (kind_of(c.type) == "array") throw Decline("an array in a string concatenation");
        parts.push_back(kind_of(c.type) == "null" ? "lit(\"null\")" : c.text);
    }
    return mk("cat!(" + join(parts) + ")", ir::class_type("java.lang.String"));
}

Code Writer::on(const ir::Node& n, const ir::Cond& k) {
    const ir::TypeP type = n.type ? n.type : type_of(k.then);
    const Code c = conv(expr(k.cond), ir::bool_type());
    const Code a = conv(expr(k.then), type), b = conv(expr(k.orelse), type);
    return mk("if " + c.text + " { " + a.text + " } else { " + b.text + " }", type, kLow);
}

Code Writer::on(const ir::Node& n, const ir::Cast& k) {
    const ir::TypeP dst = n.type;
    Code x = expr(k.x);
    const std::string_view sk = kind_of(x.type), dk = kind_of(dst);
    if (is_prim_kind(sk) && is_prim_kind(dk)) {
        if (x.bare && sk != dk) {   // a literal cast is a real cast (`200 as i8`), not a literal of the other type
            x.text = suffixed(x);
            x.bare = false;
            x.prec = kAtom;
        }
        return conv(std::move(x), dst);
    }
    if (dk == "class" || dk == "array") {
        if (sk == "null") return conv(std::move(x), dst);
        if (p_.rtype(x.type) == p_.rtype(dst)) {
            x.type = dst;
            return x;
        }
        const ClassInfo* target = p_.program_class(dst);
        if (target != nullptr && sk == "class") {
            if (is_subtype(class_name(x.type), class_name(dst))) return conv(std::move(x), dst);   // an upcast
            p_.need_caster(*target);
            const Code as_object = class_name(x.type) == "java.lang.Object" ? x : conv(std::move(x), ir::class_type("java.lang.Object"));
            return mk("cast_" + target->cname + "(" + as_object.text + ")", dst);
        }
        if (sk == "class" && is_subtype(class_name(x.type), class_name(dst))) return conv(std::move(x), dst);
        throw Decline("a cast of " + p_.rtype(x.type) + " to " + p_.rtype(dst));
    }
    throw Decline("a cast to " + std::string(dk));
}

Code Writer::on(const ir::Node&, const ir::InstanceOf& k) {
    if (k.binding != nullptr) throw Decline("an instanceof pattern the lowering left");
    const ir::TypeP test = k.test;
    if (kind_of(test) != "class") throw Decline("an instanceof of an array");
    if (class_name(test) == "java.lang.String") throw Decline("an instanceof String");
    Code x = expr(k.x);
    if (class_name(x.type) == "java.lang.String") throw Decline("an instanceof on a String");
    const Code as_object = class_name(x.type) == "java.lang.Object" ? x : conv(std::move(x), ir::class_type("java.lang.Object"));
    return mk("instance_of(" + as_object.text + ", " + rs_string(std::string(class_name(test))) + ")", ir::bool_type());
}

// ---------------------------------------------------------------------------------------------------------------- places

struct Writer::Place {
    std::string read;
    std::function<std::string(const std::string&)> write;
    ir::TypeP type;
    bool simple = false;   // reading it twice costs nothing and changes nothing
};

Writer::Place Writer::place_of(const ir::Node* target, std::vector<std::string>& pre) {
    target = strip_paren(target);
    Place p;
    p.type = type_of(target);
    const auto temp = [&](const std::string& value, const char* stem) {
        const std::string name = std::string(stem) + std::to_string(++temps_);
        pre.push_back("let " + name + " = " + value + ";");
        return name;
    };
    if (const auto* name = ir::get<ir::Name>(target)) {
        const std::string n = ident(need_sym(name->sym, "a name").name);
        p.read = n;
        p.write = [n](const std::string& v) { return n + " = " + v; };
        p.simple = true;
        return p;
    }
    if (const auto* sel = ir::get<ir::Select>(target)) {
        const Code read = on(*target, *sel);
        const ir::Sym& sym = need_sym(sel->sym, "a field");
        const ClassInfo* owner = p_.model().info(sym.owner.value_or(""));
        if (owner == nullptr) throw Decline("assigning the library field " + sym.name);
        const ClassInfo* dc = declaring_class(p_, *owner, sym.name);
        const ir::FieldDecl* decl = nullptr;
        for (const ir::Node* m : dc->members())
            if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym && f->sym->name == sym.name) decl = f;
        const bool stat = sym.is_static || ir::is<ir::TypeName>(sel->obj) || (decl != nullptr && has_mod(decl->mods, "static"));
        if (stat) {
            if (decl != nullptr && p_.is_const_field(*decl)) throw Decline("assigning the constant " + sym.name);
            const std::string holder = p_.static_field_name(*dc, sym.name);
            p.read = holder + "::get()";
            p.write = [holder](const std::string& v) { return holder + "::set(" + v + ")"; };
            p.simple = true;
            return p;
        }
        Code recv = receiver(sel->obj);
        std::string r = wrap(recv, kAtom);
        if (!is_pure(sel->obj)) r = temp(r, "o");
        const std::string field = r + "." + p_.fields_fn(*dc) + "()." + member_ident(sym.name);
        p.read = field + ".get()";
        p.write = [field](const std::string& v) { return field + ".set(" + v + ")"; };
        p.simple = pre.empty() && is_pure(sel->obj);
        return p;
    }
    if (const auto* idx = ir::get<ir::Index>(target)) {
        const std::vector<ir::Node*> list = ir::statements(idx->index);
        if (list.size() != 1 || idx->sym) throw Decline("an index with several arguments");
        std::string a = wrap(expr(idx->arr), kAtom);
        std::string i = conv(expr(list[0]), ir::i32_type()).text;
        if (!is_pure(idx->arr)) a = temp(a, "a");
        if (!is_pure(list[0])) i = temp(i, "i");
        p.read = a + ".at(" + i + ")";
        p.write = [a, i](const std::string& v) { return a + ".put(" + i + ", " + v + ")"; };
        p.simple = pre.empty();
        return p;
    }
    throw Decline("an assignment to " + std::string(ir::kind_name(*target)));
}

// `target = value` or `target op= value`. As a statement (want_value false) the text is the assignment itself; as a value it is a block
// that gives the assigned value.
Code Writer::assign_code(const ir::Node* target, const ir::Node* value, const std::optional<std::string>& op, bool want_value, const ir::TypeP&) {
    std::vector<std::string> pre;
    Place pl = place_of(target, pre);
    const ir::TypeP tt = pl.type;
    Code fresh;
    if (!op) {
        if (const auto* na = ir::get<ir::NewArray>(strip_paren(value)); na != nullptr && !value->type) array_hint_[value] = tt;
        fresh = conv(expr(value), tt);
    } else if (class_name(tt) == "java.lang.String" && *op == "+") {
        const Code v = expr(value);
        fresh = mk("cat!(" + pl.read + ", " + v.text + ")", tt);
    } else {
        Code cur = mk(pl.read, tt);
        const Code v = numeric(value);
        Code res = arith(*op, cur, v, tt, value);
        fresh = conv(res, tt);
    }
    std::string text;
    if (!want_value) {
        text = pl.write(fresh.text);
        if (!pre.empty()) text = "{ " + join(pre, " ") + " " + text + "; }";
        return mk(text, tt, kLow);
    }
    if (pl.simple && pre.empty() && ir::is<ir::Name>(strip_paren(target))) return mk("{ " + pl.write(fresh.text) + "; " + pl.read + " }", tt, kAtom);
    const std::string v = "v" + std::to_string(++temps_);
    return mk("{ " + (pre.empty() ? std::string() : join(pre, " ") + " ") + "let " + v + " = " + fresh.text + "; " + pl.write(v) + "; " + v + " }", tt, kAtom);
}

Code Writer::on(const ir::Node& n, const ir::Assign& k) { return assign_code(k.target, k.value, k.op, true, n.type); }

Code Writer::incdec(const ir::Unary& k, bool want_value) {
    std::vector<std::string> pre;
    Place pl = place_of(k.x, pre);
    const ir::TypeP tt = pl.type;
    const std::string_view kind = kind_of(tt);
    if (!is_numeric_kind(kind)) throw Decline("++ or -- on " + std::string(kind));
    const bool up = k.op == "++";
    const auto stepped = [&](const std::string& from) {
        if (is_float_kind(kind)) return from + (up ? " + 1.0" : " - 1.0");
        return from + (up ? ".wrapping_add(1)" : ".wrapping_sub(1)");
    };
    if (!want_value) {
        std::string text = pl.write(stepped(pl.read));
        if (!pre.empty()) text = "{ " + join(pre, " ") + " " + text + "; }";
        return mk(text, tt, kLow);
    }
    const std::string v = "v" + std::to_string(++temps_);
    const std::string head = "{ " + (pre.empty() ? std::string() : join(pre, " ") + " ");
    if (k.postfix) return mk(head + "let " + v + " = " + pl.read + "; " + pl.write(stepped(v)) + "; " + v + " }", tt);
    return mk(head + "let " + v + " = " + stepped(pl.read) + "; " + pl.write(v) + "; " + v + " }", tt);
}

}  // namespace cb::translate::backends::rust_java::detail
