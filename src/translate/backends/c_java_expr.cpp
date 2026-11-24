// translate/backends/c_java_expr.cpp -- expressions in the C back end for Java: each one is its C text and its C type. Java's
// rules are made explicit where C's differ: operand order, shift counts, integer division by zero, double-to-int saturation, boxing,
// string concatenation (see c_java.hpp). Calls and `new` are in c_java_call.cpp, closures in c_java_closure.cpp.

#include <format>

#include "core/text.hpp"
#include "translate/backends/c_java_internal.hpp"
#include "translate/backends/jmodel.hpp"

namespace cb::translate::backends::c_java::detail {

namespace {

// An integer (or a boolean, which is one) a literal holds.
std::optional<std::int64_t> literal_int(const ir::LitValue& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1 : 0;
    return std::nullopt;
}

// An integer literal's decimal text.
std::string literal_digits(const ir::LitValue& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return std::to_string(*i);
    if (const auto* u = std::get_if<std::uint64_t>(&v)) return std::to_string(*u);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? "1" : "0";
    throw Decline("an integer literal that is not a number");
}

bool literal_true(const ir::LitValue& v) {
    if (const auto* b = std::get_if<bool>(&v)) return *b;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i != 0;
    if (const auto* u = std::get_if<std::uint64_t>(&v)) return *u != 0;
    if (const auto* d = std::get_if<double>(&v)) return *d != 0.0;
    if (const auto* s = std::get_if<std::string>(&v)) return !s->empty();
    return false;
}

// The literal an operand is, once its parentheses are off: its integer value.
std::optional<std::int64_t> operand_int(const ir::Node* operand) {
    const auto* lit = ir::get<ir::Lit>(cpp03::strip_paren(operand));
    return lit != nullptr ? literal_int(lit->value) : std::nullopt;
}

bool is_bits_operator(std::string_view op) { return op == "<<" || op == ">>" || op == "&" || op == "|" || op == "^"; }

bool is_ident_start(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
bool is_ident_char(char c) { return is_ident_start(c) || (c >= '0' && c <= '9'); }

// A name or member chain with no grouping (`a`, `self->x.y`): a cast of it needs no parentheses.
bool is_plain_chain(std::string_view code) {
    std::size_t i = 0;
    while (true) {
        if (i >= code.size() || !is_ident_start(code[i])) return false;
        while (i < code.size() && is_ident_char(code[i])) ++i;
        if (i == code.size()) return true;
        if (code.substr(i).starts_with("->")) i += 2;
        else if (code[i] == '.') i += 1;
        else return false;
    }
}

const ir::FieldDecl* find_field(const ClassInfo* cls, std::string_view name) {
    if (cls == nullptr) return nullptr;
    for (const ir::Node* fn : cls->fields()) {
        const ir::FieldDecl& f = field_of(fn);
        if (f.sym && f.sym->name == name) return &f;
    }
    return nullptr;
}

}  // namespace

// ---- expressions -> (code, C type)
Code MethodWriter::expr(const ir::Node* e) {
    if (e == nullptr) throw Decline("expression NoneType");
    if (const auto it = subst_.find(e); it != subst_.end()) return it->second;   // already in a temporary
    return ir::visit(*e, [&](const auto& k) -> Code {
        if constexpr (requires { this->on(*e, k); }) {
            // Java evaluates operands left to right; C does not say: the operands whose order shows are taken into temporaries
            const std::string pre = order_ ? sequenced(e) : std::string();
            Code r = on(*e, k);
            p_.touch(r.ctype);   // (a chain `a->b->c` needs the struct of each link, which the text may never name)
            if (!pre.empty()) r.code = std::format("({}, {})", pre, r.code);
            return r;
        } else {
            throw Decline("expression " + std::string(ir::kind_name(*e)));
        }
    });
}

std::string MethodWriter::sequenced(const ir::Node* e) {
    if (ir::is<ir::Lambda>(e) || ir::is<ir::MethodRef>(e)) return "";
    if (const auto* concat = ir::get<ir::Concat>(e); concat != nullptr && concat_in_turn(*concat)) return "";   // (it orders its own parts)
    const std::vector<const ir::Node*> nodes = cpp03::operands(e);
    if (nodes.size() < 2 || !cpp03::order_matters(nodes)) return "";
    std::vector<std::string> parts;
    for (const ir::Node* n : nodes) {
        if (cpp03::is_constant(n) || subst_.contains(n) || ir::is<ir::Lambda>(n) || ir::is<ir::MethodRef>(n)) continue;
        const Code c = expr(n);
        if (c.ctype == "void" || c.ctype == "nullptr_t") return "";
        const std::string name = std::format("jrt_o{}", ++temps_);
        pending_.push_back(decl(c.ctype, name));
        parts.push_back(std::format("{} = {}", name, c.code));
        subst_[n] = Code{name, c.ctype};
    }
    return join_commas(parts);
}

bool MethodWriter::is_sub(std::string_view src, std::string_view dst) const {
    if (src == dst || dst == kObj || dst == "void*" || src == "nullptr_t") return true;
    const ClassInfo* a = p_.class_of_ctype(src);
    const ClassInfo* b = p_.class_of_ctype(dst);
    return a != nullptr && b != nullptr && p_.model().is_sub(a->qname, b->qname);
}

// `code` of C type `src` where a `dst` is wanted: numeric conversions, boxing and unboxing, checked casts.
// Whether text has an assignment outside every bracket: a cast in front of it would bind to the place alone.
static bool assigns_at_top(const std::string& code) {
    int depth = 0;
    for (std::size_t i = 0; i < code.size(); ++i) {
        const char c = code[i];
        if (c == '(' || c == '[' || c == '{') ++depth;
        else if (c == ')' || c == ']' || c == '}') --depth;
        else if (c == '"' || c == '\'') {   // (a literal: skipped)
            for (++i; i < code.size() && code[i] != c; ++i) {
                if (code[i] == '\\') ++i;
            }
        } else if (c == '=' && depth == 0 && i > 0 && i + 1 < code.size() && code[i + 1] != '=' && std::string_view("=!<>").find(code[i - 1]) == std::string_view::npos) {
            return true;
        }
    }
    return false;
}

std::string MethodWriter::conv(const std::string& given, std::string_view src, std::string_view dst) const {
    if (src == dst || dst == "void" || dst.empty() || src.empty()) return given;
    if (src == "nullptr_t") return "NULL";
    const std::string code = assigns_at_top(given) ? "(" + given + ")" : given;
    if (is_prim_c(dst) && is_prim_c(src)) {
        if ((src == "double" || src == "float") && is_integral(dst)) {   // Java saturates
            if (dst == "int64_t") return std::format("jrt_d2l({})", code);
            return dst == "int32_t" ? std::format("jrt_d2i({})", code) : std::format("({})jrt_d2i({})", dst, code);
        }
        return std::format("({})({})", dst, code);
    }
    if (is_prim_c(dst) && is_pointer_ctype(src)) return std::format("jrt_unbox_{}((jobject*){})", ctype_letter(dst), code);
    if (is_pointer_ctype(dst) && is_prim_c(src))
        return std::format("{}jrt_box_{}({})", dst == kObj ? std::string() : std::format("({})", dst), ctype_letter(src), code);
    if (is_pointer_ctype(dst) && is_pointer_ctype(src)) {
        if (is_sub(src, dst)) return src == dst ? code : std::format("({}){}", dst, code);
        if (dst == kStr) return std::format("(jstring*)jrt_cast((jobject*){}, &jrt_String_class)", code);
        if (const ClassInfo* c = p_.class_of_ctype(dst)) return std::format("({})jrt_cast((jobject*){}, &{}_class)", dst, code, c->cname);
        return std::format("({}){}", dst, code);
    }
    return code;
}

Code MethodWriter::on(const ir::Node&, const ir::Paren& k) {
    const Code x = expr(k.x);
    return {std::format("({})", x.code), x.ctype};
}

Code MethodWriter::on(const ir::Node& n, const ir::Lit& k) {
    const ir::LitValue& v = k.value;
    const std::string_view kind = n.type ? n.type->kind() : std::string_view("null");
    if (std::holds_alternative<std::monostate>(v)) return {"NULL", "nullptr_t"};
    if (kind == "bool") return {literal_true(v) ? "true" : "false", "bool"};
    if (kind == "i32") return {literal_int(v) == -2147483648LL ? "JRT_INT_MIN" : literal_digits(v), "int32_t"};
    if (kind == "i64") {
        if (literal_int(v) == INT64_MIN) return {"JRT_LONG_MIN", "int64_t"};
        return {literal_digits(v) + "LL", "int64_t"};
    }
    if (kind == "f32") return {text::replace_all(cpp03::float_literal(v, "F"), "F", "f"), "float"};
    if (kind == "f64") return {cpp03::float_literal(v, ""), "double"};
    if (kind == "u16") {
        std::optional<std::uint32_t> code;
        if (const auto* s = std::get_if<std::string>(&v)) code = first_code_point(*s);
        else if (const auto i = literal_int(v)) code = static_cast<std::uint32_t>(*i);
        if (!code) throw Decline("a char literal that is not a character");
        return {jmodel::char_literal(*code), "char16_t"};
    }
    if (const auto* s = std::get_if<std::string>(&v)) return {p_.lit(*s), std::string(kStr)};
    throw Decline("literal of type " + std::string(kind));
}

Code MethodWriter::on(const ir::Node& n, const ir::Name& k) {
    const ir::Sym& sym = need_sym(k.sym, "a name");
    const std::string name = ident(sym.name);
    if (const auto it = vars_.find(name); it != vars_.end()) return {name, it->second};
    if (lam_) {
        if (const auto it = lam_->find(name); it != lam_->end()) return it->second;
    }
    if (const auto it = captures_.find(sym.name); it != captures_.end()) return {it->second, p_.ct(n.type)};
    throw Decline("unknown local " + name);
}

Code MethodWriter::this_ref() {
    if (lam_) {
        const auto it = lam_->find("#this");
        if (it == lam_->end()) throw Decline("this in a closure that did not capture it");
        return it->second;
    }
    if (static_) throw Decline("this in a static context");
    return {"self", ci_->kind == "interface" ? std::string(kObj) : ci_->cname + "*"};
}

Code MethodWriter::on(const ir::Node& n, const ir::This& k) {
    const ir::TypeP q = k.qualifier ? k.qualifier : n.type;
    if (kind_of(q) == "class" && class_name(q) != ci_->qname && ci_->outer != nullptr &&
        !p_.model().is_sub(ci_->qname, class_name(q)))
        return outer_of(class_name(q));   // Outer.this
    return this_ref();
}

// The enclosing instance of class `qname`: self's outer_, and its outer_ in turn.
Code MethodWriter::outer_of(std::string_view qname) {
    std::string code = this_ref().code;
    const ClassInfo* c = ci_;
    while (c != nullptr && !(c->qname == qname || p_.model().is_sub(c->qname, qname))) {
        if (!c->needs_outer) throw Decline("no enclosing instance of " + std::string(qname));
        code += "->outer_";
        c = c->outer;
    }
    if (c == nullptr) throw Decline("no enclosing instance of " + std::string(qname));
    return {code, c->cname + "*"};
}

// What a member access is made on: this, an enclosing instance (the member is an outer class's), or an expression.
Code MethodWriter::receiver(const ir::Node* obj, const ir::SymP& sym) {
    bool self_like = obj == nullptr;
    if (!self_like && (ir::is<ir::This>(obj) || ir::is<ir::Super>(obj))) {
        const auto* t = ir::get<ir::This>(obj);
        const ir::TypeP& qualifier = t != nullptr ? t->qualifier : ir::get<ir::Super>(obj)->qualifier;
        const ir::TypeP ot = type_of(obj);
        if (!qualifier && (!ot || class_name(ot) == ci_->qname || p_.model().is_sub(ci_->qname, class_name(ot)) || ir::is<ir::Super>(obj)))
            self_like = true;
    }
    if (self_like) {
        if (sym && sym->owner && !sym->owner->empty() && *sym->owner != ci_->qname && ci_->outer != nullptr &&
            !p_.model().is_sub(ci_->qname, *sym->owner) && (obj == nullptr || ir::is<ir::This>(obj)))
            return outer_of(*sym->owner);
        return this_ref();
    }
    return expr(obj);
}

Code MethodWriter::on(const ir::Node& n, const ir::Select& k) {
    const ir::Sym& s = need_sym(k.sym, "a field");
    if (ir::is<ir::TypeName>(k.obj) || s.is_static) {
        const ClassInfo* owner = p_.model().info(s.owner.value_or(""));
        if (owner == nullptr) {   // a library constant
            const Code* c = library_static(s.owner.value_or(""), s.name);
            if (c == nullptr) throw Decline("library static " + s.owner.value_or("None") + "." + s.name);
            return *c;
        }
        std::string code = std::format("{}_{}", owner->cname, ident(s.name));
        if (p_.needs_clinit(*owner) && owner != ci_) code = std::format("({}_clinit(), {})", owner->cname, code);   // first use runs the initialiser
        if (s.kind == ir::SymKind::EnumConst) return {code, owner->cname + "*"};
        const ir::FieldDecl* f = find_field(owner, s.name);
        return {code, p_.ct(f != nullptr ? f->type : n.type)};
    }
    const Code recv = receiver(k.obj, k.sym);
    const ClassInfo* owner = p_.model().info(s.owner.value_or(""));
    if (owner == nullptr) throw Decline("library field " + s.owner.value_or("None") + "." + s.name);
    const ir::FieldDecl* f = find_field(owner, s.name);
    const std::string declared = p_.ct(jmodel::erase(f != nullptr ? f->type : n.type));
    const bool exact = recv.ctype == owner->cname + "*";
    std::string access = std::format("{}->{}", exact ? recv.code : std::format("(({}*)jrt_nn({}))", owner->cname, recv.code), ident(s.name));
    if (exact && recv.code != "self" && !recv.code.starts_with("self->"))
        access = std::format("(({}*)jrt_nn({}))->{}", owner->cname, recv.code, ident(s.name));
    const std::string want = p_.ct(n.type);
    if (declared != want && is_pointer_ctype(want) && declared == kObj) return {conv(access, declared, want), want};
    return {access, declared};
}

Code MethodWriter::on(const ir::Node&, const ir::Length& k) { return {std::format("JLEN({})", expr(k.arr).code), "int32_t"}; }

Code MethodWriter::on(const ir::Node&, const ir::Index& k) {
    const std::string array = expr(k.arr).code;
    const Code index = expr(single(k.index));
    const ir::TypeP elem = elem_of(type_of(k.arr));
    const std::string store = array_store(elem ? p_.ct(elem) : std::string(kObj));
    return {std::format("JA({}, {}, {})", store, array, conv(index.code, index.ctype, "int32_t")), store};
}

std::string MethodWriter::conv_to(const Code& value, const ir::TypeP& type, std::string_view dst) const {
    if (is_pointer_ctype(dst) && is_prim_c(value.ctype) && kind_of(type) == "class") {
        if (const std::string_view letter = box_letter(class_name(type)); !letter.empty() && letter_ctype(letter) != value.ctype) {
            return std::format("({})jrt_box_{}(({}){})", dst, letter, letter_ctype(letter), value.code);
        }
    }
    return conv(value.code, value.ctype, dst);
}

// A static field of a class still to be initialised reads `(X_clinit(), X_f)`, which C cannot assign to: as a place it is `(*(X_clinit(), &X_f))`.
static std::string as_place(std::string code) {
    static constexpr std::string_view kCall = "_clinit(), ";
    const std::size_t at = code.find(kCall);
    if (!code.starts_with('(') || !code.ends_with(')') || at == std::string::npos || code.find(',') != at + kCall.size() - 2) return code;
    const std::string head = code.substr(1, at + kCall.size() - 3);
    const std::string field = code.substr(at + kCall.size(), code.size() - at - kCall.size() - 1);
    return "(*(" + head + ", &" + field + "))";
}

// A boxed value read where a number is wanted: its accessor.
Code MethodWriter::unboxed(const std::string& code, std::string_view ct, const ir::TypeP& type) const {
    if (is_pointer_ctype(ct) && kind_of(type) == "class") {
        if (const std::string_view letter = box_letter(class_name(type)); !letter.empty())
            return {std::format("jrt_unbox_{}((jobject*){})", letter, code), std::string(letter_ctype(letter))};
    }
    return {code, std::string(ct)};
}

Code MethodWriter::on(const ir::Node& n, const ir::Unary& k) {
    Code x = expr(k.x);
    if (k.op == "++" || k.op == "--") {
        x.code = as_place(std::move(x.code));
        if (is_pointer_ctype(x.ctype)) {   // a box is replaced by a new one; the place is named once, so what it is made of runs once
            const ir::TypeP held = type_of(k.x);
            const std::string_view letter = kind_of(held) == "class" ? box_letter(class_name(held)) : std::string_view();
            if (letter.empty()) throw Decline("++ on a box");
            return {std::format("(({})jrt_box_step_{}((jobject* volatile*)&{}, {}, {}))", x.ctype, letter, x.code, k.op == "++" ? 1 : -1, k.postfix ? "true" : "false"), x.ctype};
        }
        return {k.postfix ? x.code + k.op : k.op + x.code, x.ctype};
    }
    x = unboxed(x.code, x.ctype, type_of(k.x));
    if (k.op == "!") return {"!" + x.code, "bool"};
    return {k.op + x.code, p_.ct(n.type)};
}

Code MethodWriter::on(const ir::Node& n, const ir::Binary& k) {
    Code l = expr(k.left);
    Code r = expr(k.right);
    const std::string& op = k.op;
    if ((op == "==" || op == "!=") && (is_pointer_ctype(l.ctype) || l.ctype == "nullptr_t") && (is_pointer_ctype(r.ctype) || r.ctype == "nullptr_t"))
        return {std::format("(jobject*){} {} (jobject*){}", l.code, op, r.code), "bool"};   // identity
    l = unboxed(l.code, l.ctype, type_of(k.left));
    r = unboxed(r.code, r.ctype, type_of(k.right));
    if (op == "&&" || op == "||") return {std::format("{} {} {}", l.code, op, r.code), "bool"};
    const std::string res = p_.ct(n.type);
    if (is_bits_operator(op)) {   // C's precedence of the bit operators differs from what a reader expects: group a mixed operand
        if (const auto* lb = ir::get<ir::Binary>(k.left); lb != nullptr && lb->op != op) l.code = std::format("({})", l.code);
        if (const auto* rb = ir::get<ir::Binary>(k.right); rb != nullptr && rb->op != op) r.code = std::format("({})", r.code);
    }
    if (op == "<<" || op == ">>" || op == ">>>") {   // Java masks the count
        const int width = l.ctype == "int64_t" ? 64 : 32;
        const std::optional<std::int64_t> count = operand_int(k.right);
        if (!(count && 0 <= *count && *count < width)) r.code = std::format("({} & {})", r.code, width - 1);
        if (op == ">>>") {
            return {std::format("jrt_ushr_{}({}, {})", l.ctype == "int64_t" ? "J" : "I",
                                (l.ctype == "int32_t" || l.ctype == "int64_t") ? l.code : std::format("(int32_t){}", l.code), r.code),
                    res};
        }
        return {std::format("{} {} {}", l.code, op, r.code), res};
    }
    if ((op == "/" || op == "%") && is_integral(res)) {   // a zero divisor throws; so does MIN / -1 in C
        const std::optional<std::int64_t> divisor = operand_int(k.right);
        if (!(divisor && *divisor != 0 && *divisor != -1))
            return {std::format("jrt_{}_{}({}, {})", op == "/" ? "div" : "rem", res == "int64_t" ? "J" : "I", l.code, r.code), res};
    }
    if (op == "%" && (res == "float" || res == "double")) return {std::format("({})fmod({}, {})", res, l.code, r.code), res};
    const bool comparison = op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=";
    return {std::format("{} {} {}", l.code, op, r.code), comparison ? "bool" : res};
}

// An operand of a concatenation or of `+=` on a String: as a jstring*.
std::string MethodWriter::as_string(const ir::Node* x) {
    const Code c = expr(x);
    if (ir::is<ir::Lit>(cpp03::strip_paren(x)) && c.ctype == kStr) return c.code;
    if (const std::string_view letter = ctype_letter(c.ctype); !letter.empty()) {
        if (letter == "B" || letter == "S") return std::format("jrt_str_I((int32_t){})", c.code);
        return std::format("jrt_str_{}({})", letter, c.code);
    }
    if (c.ctype == "nullptr_t") return "jrt_str_O(NULL)";
    return std::format("jrt_str_O((jobject*){})", c.code);
}

// Java turns each part to text in turn, left to right, and C does not order a call's arguments. That shows when an object is written
// beside something that can change it: another part with an effect, or a second object, whose toString may have one.
bool MethodWriter::concat_in_turn(const ir::Concat& k) const {
    if (!order_) return false;
    std::size_t open = 0, objects = 0, boxes = 0;
    bool effects = false;
    for (const ir::Node* x : k.parts) {
        if (cpp03::is_constant(x)) continue;
        ++open;
        effects = effects || cpp03::has_effects(x);
        const ir::TypeP t = type_of(x);
        if (kind_of(t) == "class" && class_name(t) != "java.lang.String") ++(box_letter(class_name(t)).empty() ? objects : boxes);
    }
    return open >= 2 && ((objects >= 1 && (effects || objects >= 2)) || (boxes >= 1 && effects));
}

Code MethodWriter::on(const ir::Node&, const ir::Concat& k) {
    std::vector<std::string> parts, steps;
    const bool in_turn = concat_in_turn(k);
    for (const ir::Node* x : k.parts) {
        std::string text = as_string(x);
        if (in_turn && !cpp03::is_constant(x)) {
            const std::string name = std::format("jrt_o{}", ++temps_);
            pending_.push_back(decl(kStr, name));
            steps.push_back(std::format("{} = {}", name, text));
            text = name;
        }
        parts.push_back(std::move(text));
    }
    const std::string joined = std::format("jrt_concat({}, {})", parts.size(), join_commas(parts));
    return {steps.empty() ? joined : std::format("({}, {})", join_commas(steps), joined), std::string(kStr)};
}

Code MethodWriter::on(const ir::Node&, const ir::Assign& k) {
    // a[i] = f(): Java works out a and i before f, and C does not order the two sides
    if (const auto* place = ir::get<ir::Index>(cpp03::strip_paren(k.target)); !k.op && place != nullptr && cpp03::has_effects(k.value)) {
        std::unordered_set<std::string> written;
        for_each_node(k.value, [&](const ir::Node* n) {
            const ir::Node* changed = nullptr;
            if (const auto* a = ir::get<ir::Assign>(n)) changed = cpp03::strip_paren(a->target);
            if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) changed = cpp03::strip_paren(u->x);
            if (const auto* name = ir::get<ir::Name>(changed); name != nullptr && name->sym) written.insert(name->sym->name);
        });
        const auto stays = [&](const ir::Node* x) {
            x = cpp03::strip_paren(x);
            if (ir::is<ir::Lit>(x) || ir::is<ir::This>(x)) return true;
            const auto* name = ir::get<ir::Name>(x);
            return name != nullptr && name->sym && (name->sym->kind == ir::SymKind::Local || name->sym->kind == ir::SymKind::Param) && !written.contains(name->sym->name);
        };
        const ir::Node* at = single(place->index);
        if (!stays(place->arr) || !stays(at)) {
            const ir::TypeP elem = elem_of(type_of(place->arr));
            const std::string store = array_store(elem ? p_.ct(elem) : std::string(kObj));
            const std::size_t id = ++temps_;
            const std::string array = std::format("jrt_arr{}", id), index = std::format("jrt_idx{}", id), held = std::format("jrt_val{}", id);
            pending_.push_back(decl("jarray*", array));
            pending_.push_back(decl("int32_t", index));
            pending_.push_back(decl(store, held));
            const std::string array_code = expr(place->arr).code;
            const Code index_code = expr(at);
            const Code value_code = expr(k.value);
            return {std::format("({} = (jarray*){}, {} = {}, {} = {}, JA({}, {}, {}) = {})", array, array_code, index, conv(index_code.code, index_code.ctype, "int32_t"), held,
                                conv_to(value_code, elem, store), store, array, index, held),
                    store};
        }
    }
    Code target = expr(k.target);
    target.code = as_place(std::move(target.code));
    Code value = expr(k.value);
    const std::string& place_code = target.code;
    const std::string& place_type = target.ctype;
    if (!k.op) return {std::format("{} = {}", place_code, conv_to(value, type_of(k.target), place_type)), place_type};
    const std::string& op = *k.op;
    if (place_type == kStr && op == "+") return {std::format("{} = jrt_concat(2, jrt_str_O((jobject*){}), {})", place_code, place_code, as_string(k.value)), place_type};
    // the target is read twice by the forms below: only a name or a `this`/static field may be
    const ir::Node* tgt = cpp03::strip_paren(k.target);
    bool simple = ir::is<ir::Name>(tgt);
    if (const auto* sel = ir::get<ir::Select>(tgt); !simple && sel != nullptr) simple = ir::is<ir::This>(sel->obj) || ir::is<ir::TypeName>(sel->obj);
    value = unboxed(value.code, value.ctype, type_of(k.value));
    const std::string& v = value.code;
    const std::string& vt = value.ctype;
    const auto update = [&](const std::string& t, const std::string& tt, bool plain) -> Code {
    if ((op == "/" || op == "%") && is_integral(tt) && is_integral(vt)) {
        if (!plain) throw Decline("integer /= on a complex target");
        const char* wide = (tt == "int64_t" || vt == "int64_t") ? "J" : "I";
        return {std::format("{} = ({})jrt_{}_{}({}, {})", t, tt, op == "/" ? "div" : "rem", wide, t, v), tt};
    }
    if (op == "<<" || op == ">>" || op == ">>>") {
        const int width = tt == "int64_t" ? 64 : 32;
        if (op == ">>>") {
            if (!plain) throw Decline(">>>= on a complex target");
            return {std::format("{} = ({})jrt_ushr_{}({}, ({} & {}))", t, tt, tt == "int64_t" ? "J" : "I",
                                (tt == "int32_t" || tt == "int64_t") ? t : "(int32_t)" + t, v, width - 1),
                    tt};
        }
        return {std::format("{} {}= ({} & {})", t, op, v, width - 1), tt};
    }
    if (is_integral(tt) && (vt == "float" || vt == "double")) {   // the double result is truncated back, as Java does
        if (!plain) throw Decline("integer op= double on a complex target");
        return {std::format("{} = ({}){}({} {} {})", t, tt, tt == "int64_t" ? "jrt_d2l" : "jrt_d2i", t, op, v), tt};
    }
    if (op == "%" && (tt == "float" || tt == "double")) {
        if (!plain) throw Decline("%= on a complex target");
        return {std::format("{} = ({})fmod({}, {})", t, tt, t, v), tt};
    }
    return {std::format("{} {}= {}", t, op, v), tt};
    };
    if (!is_pointer_ctype(place_type)) return update(place_code, place_type, simple);
    // a box: the place is found once, its number read (a null throws), the right side worked out, and a new box put there
    const ir::TypeP held = type_of(k.target);
    const std::string_view letter = kind_of(held) == "class" ? box_letter(class_name(held)) : std::string_view();
    if (letter.empty()) throw Decline("compound assignment to a reference");
    const std::string number(letter_ctype(letter));
    const std::size_t id = ++temps_;
    const std::string place = std::format("jrt_place{}", id), old = std::format("jrt_old{}", id);
    pending_.push_back(decl("jobject* volatile*", place));
    pending_.push_back(decl(number, old));
    return {std::format("({} = (jobject* volatile*)&{}, {} = jrt_unbox_{}(*{}), {}, *{} = jrt_box_{}({}), ({})*{})", place, place_code, old, letter, place,
                        update(old, number, true).code, place, letter, old, place_type, place),
            place_type};
}

Code MethodWriter::on(const ir::Node& n, const ir::Cond& k) {
    const std::string test = cond(k.cond);
    const Code a = expr(k.then);
    const Code b = expr(k.orelse);
    const std::string t = (n.type && n.type->kind() != "null") ? p_.ct(n.type) : (a.ctype != "nullptr_t" ? a.ctype : b.ctype);
    return {std::format("({} ? {} : {})", test, conv(a.code, a.ctype, t), conv(b.code, b.ctype, t)), t};
}

Code MethodWriter::on(const ir::Node& n, const ir::Cast& k) {
    const Code x = expr(k.x);
    const std::string ct = p_.ct(n.type);
    if (ct == x.ctype) return {x.code, ct};
    if (is_prim_c(ct) && is_prim_c(x.ctype)) {
        if ((x.ctype == "double" || x.ctype == "float") && !(ct == "double" || ct == "float")) return {conv(x.code, x.ctype, ct), ct};
        return {std::format("({}){}", ct, is_plain_chain(x.code) ? x.code : std::format("({})", x.code)), ct};
    }
    if ((is_pointer_ctype(ct) && is_pointer_ctype(x.ctype)) || x.ctype == "nullptr_t") {
        if (is_sub(x.ctype, ct)) return {x.ctype == ct ? x.code : std::format("({}){}", ct, x.code), ct};   // an upcast needs no check
        if (kind_of(n.type) == "class")
            return {std::format("({})jrt_cast((jobject*){}, {})", ct, x.code, p_.class_ref(class_name(n.type))), ct};   // a downcast is checked
        return {std::format("({}){}", ct, x.code), ct};
    }
    return {conv(x.code, x.ctype, ct), ct};
}

Code MethodWriter::on(const ir::Node&, const ir::InstanceOf& k) {
    if (k.binding != nullptr) throw Decline("instanceof pattern not lowered");
    const std::string x = expr(k.x).code;
    if (kind_of(k.test) == "array") return {std::format("jrt_instanceof((jobject*){}, &jrt_Array_class)", x), "bool"};
    return {std::format("jrt_instanceof((jobject*){}, {})", x, p_.class_ref(class_name(k.test))), "bool"};
}

Code MethodWriter::on(const ir::Node&, const ir::SwitchExpr&) { throw Decline("a switch expression inside an expression"); }
Code MethodWriter::on(const ir::Node&, const ir::ClassLit&) { throw Decline("class literal"); }
Code MethodWriter::on(const ir::Node&, const ir::TypeName&) { throw Decline("a type as a value"); }

}  // namespace cb::translate::backends::c_java::detail
