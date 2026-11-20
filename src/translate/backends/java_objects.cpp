// translate/backends/java_objects.cpp -- the Java back end's idiomatic tier (see the header): the program writer (JoObjects, the
// faithful tier's Faithful with the object model's types, classes and globals) and the function writer (JoWriter, FnWriter with
// places that are Java variables and values that are Java objects).
#include "translate/backends/java_objects.hpp"

#include <algorithm>
#include <array>
#include <set>
#include <unordered_map>

#include "core/text.hpp"

namespace cb::translate::backends::java_objects {

namespace {

using jb::CKind;
using jb::Ex;
using jb::Place;
using jb::Refused;
using jb::Var;
using jb::VarKind;
using ir::Node;
using Lines = std::vector<std::string>;

// ---- small helpers ----------------------------------------------------------------------------------------------------------

bool jo_has(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

const std::vector<std::string>& jo_mods(const Node* member) {
    static const std::vector<std::string> none;
    if (const auto* f = ir::get<ir::FieldDecl>(member)) return f->mods;
    if (const auto* m = ir::get<ir::MethodDecl>(member)) return m->mods;
    return none;
}

ir::TypeP jo_param_type(const Node* param) {
    const ir::Param* p = ir::get<ir::Param>(param);
    if (p == nullptr) return {};
    return p->type ? p->type : (p->sym != nullptr ? p->sym->type : ir::TypeP{});
}

std::string jo_param_name(const Node* param) {
    const ir::Param* p = ir::get<ir::Param>(param);
    return p != nullptr && p->sym != nullptr ? p->sym->name : std::string();
}

std::vector<ir::TypeP> jo_param_types(const ir::MethodDecl& f) {
    std::vector<ir::TypeP> out;
    for (const Node* p : f.params) out.push_back(jo_param_type(p));
    return out;
}

// A C++ reference (T&): a pointer that is never null nor reseated.
bool jo_ref_ptr(const ir::TypeP& t) {
    const auto* p = t ? std::get_if<ir::PtrType>(&t->body) : nullptr;
    return p != nullptr && p->ref;
}

const ir::ArrayType* jo_array(const ir::TypeP& t) { return t ? std::get_if<ir::ArrayType>(&t->body) : nullptr; }

bool jo_is_array(const ir::TypeP& t) { return jb::kind_of(t) == CKind::Array; }

// A scalar or pointer whose address is taken lives boxed in a one-element array; structs and arrays are objects already.
bool jo_boxable(const ir::TypeP& t) { return !jb::is_record(t) && !jo_is_array(t); }

Lines jo_indent(const Lines& lines, std::string_view prefix) {
    Lines out;
    out.reserve(lines.size());
    for (const std::string& l : lines) out.push_back(l.empty() ? l : std::string(prefix) + l);
    return out;
}

void jo_append(Lines& out, const Lines& more) { out.insert(out.end(), more.begin(), more.end()); }

std::string jo_upper_first(std::string s) {
    if (!s.empty() && s[0] >= 'a' && s[0] <= 'z') s[0] = static_cast<char>(s[0] - 'a' + 'A');
    return s;
}

// The statements of a body that is a Block (or any other statement list).
std::vector<Node*> jo_statements(const ir::Suite& body) {
    if (const auto* one = std::get_if<Node*>(&body)) {
        if (const ir::Block* b = ir::get<ir::Block>(*one)) return b->stmts;
    }
    return ir::statements(body);
}

// A literal that is zero, null or false: what a Java field already holds before anything is assigned.
bool jo_zero_literal(const Node* e) {
    const ir::Lit* lit = ir::get<ir::Lit>(e);
    if (lit == nullptr) return false;
    struct Visitor {
        bool operator()(std::monostate) const { return true; }
        bool operator()(bool v) const { return !v; }
        bool operator()(std::int64_t v) const { return v == 0; }
        bool operator()(std::uint64_t v) const { return v == 0; }
        bool operator()(double v) const { return v == 0.0; }
        bool operator()(const std::string&) const { return false; }
    };
    return std::visit(Visitor{}, lit->value);
}

std::optional<std::string> jo_number_text(const Node* e) {
    const ir::Lit* lit = ir::get<ir::Lit>(e);
    if (lit == nullptr) return std::nullopt;
    if (const auto* i = std::get_if<std::int64_t>(&lit->value)) return std::to_string(*i);
    if (const auto* u = std::get_if<std::uint64_t>(&lit->value)) return std::to_string(*u);
    return std::nullopt;
}

// A word of a template argument list that is one of the C types the front end names by kind, as its Java type: `Stack<i32, 8>` is
// `Stack_int_8`.
std::string jo_template_suffix(const std::string& qname) {
    static const std::unordered_map<std::string_view, std::string_view> kinds = {   // lookup only
        {"bool", "boolean"}, {"i8", "byte"}, {"u8", "byte"}, {"char", "byte"}, {"i16", "short"}, {"u16", "char"}, {"i32", "int"},
        {"u32", "int"}, {"i64", "long"}, {"u64", "long"}, {"f32", "float"}, {"f64", "double"}, {"f80", "double"}};
    const std::size_t open = qname.find('<');
    const std::string args = qname.substr(open + 1, qname.size() - open - 2);
    const auto word_char = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; };
    std::string out;
    for (std::size_t i = 0; i < args.size();) {
        std::size_t j = i;
        const bool word = word_char(args[i]);
        while (j < args.size() && word_char(args[j]) == word) ++j;
        const std::string_view run = std::string_view(args).substr(i, j - i);
        if (word) {
            const auto it = kinds.find(run);
            out += it != kinds.end() ? std::string(it->second) : std::string(run);
        } else {
            out += '_';   // a run of anything else is one underscore
        }
        i = j;
    }
    const std::size_t first = out.find_first_not_of('_');
    if (first == std::string::npos) return {};
    return out.substr(first, out.find_last_not_of('_') - first + 1);
}

}  // namespace

// ---- operators --------------------------------------------------------------------------------------------------------------

std::string method_name(std::string_view name) {
    static const std::unordered_map<std::string_view, std::string_view> operators = {   // lookup only
        {"operator+", "plus"}, {"operator-", "minus"}, {"operator*", "times"}, {"operator/", "div"}, {"operator%", "rem"},
        {"operator==", "isEqual"}, {"operator!=", "notEqual"}, {"operator<", "lessThan"}, {"operator>", "greaterThan"},
        {"operator<=", "atMost"}, {"operator>=", "atLeast"}, {"operator[]", "at"}, {"operator()", "call"}, {"operator+=", "plusAssign"},
        {"operator-=", "minusAssign"}, {"operator*=", "timesAssign"}, {"operator/=", "divAssign"}, {"operator=", "assign"},
        {"operator!", "not"}, {"operator<<", "shiftLeft"}, {"operator>>", "shiftRight"}, {"operator&", "and"}, {"operator|", "or"},
        {"operator^", "xor"}, {"operator~", "complement"}, {"operator++", "increment"}, {"operator--", "decrement"},
        {"operator bool", "toBool"}};
    if (const auto it = operators.find(name); it != operators.end()) return std::string(it->second);
    if (name.starts_with("operator")) throw Refused("operator " + std::string(name.substr(8)));
    return jb::jname(name);
}

namespace {

class JoWriter;

// ---- the program ------------------------------------------------------------------------------------------------------------

// The idiomatic tier's program writer: the faithful program with the object model's names, types, classes and globals.
class JoObjects final : public jb::Faithful {
public:
    using jb::Faithful::Faithful;

    std::set<std::string> ctors_used;   // the classes whose aggregate constructor is used
    std::set<std::string> copied;       // ... whose copy constructor is used
    std::set<std::string> assigned;     // ... whose set() is used
    bool arrays_filled = false;         // `filled(new T[n], T::new)` is used
    std::unordered_map<std::string, std::string> cls_names;   // qname -> Java class name (lookup only)

    void check_records() override;
    void make_names() override;

    // ---- types
    // The Java class a struct or class type is.
    std::string cls(const ir::TypeP& type);
    // The class declaration a record type names (null for another: a library type).
    ir::ClassDecl* record_of(const ir::TypeP& type) const;
    // sizeof(type) where the source's layout gives it, else nothing.
    std::optional<std::int64_t> size_hint(const ir::TypeP& type);
    bool is_enum(const ir::TypeP& type) const;
    std::string jt(const ir::TypeP& type) override;
    jb::Sig fn_sig(const ir::Sym& sym) override;
    jb::Sig fn_sig_of_type(const ir::TypeP& fn_or_fnptr) override;
    std::string iface(const jb::Sig& sig) override;
    // A fresh value of the type: what C's zero-initialised storage holds, as Java.
    std::string default_value(const ir::TypeP& type);

    // ---- the file
    std::string write() override;
    std::optional<std::string> gap_signature(const ir::MethodDecl& f, const std::optional<std::string>& owner) override;
    std::vector<std::string> entry() override;
    void declare_global(const ir::FieldDecl& g) override;
    void init_global(const ir::FieldDecl&) override {}   // (declare_global writes the initialiser where the field is)

    Lines class_decl(ir::ClassDecl& c);
    Lines copy_fields(const std::vector<const ir::FieldDecl*>& fields, std::string_view dst, std::string_view src, bool fresh);
};

// ---- a function ---------------------------------------------------------------------------------------------------------------

// A function, method or constructor in the idiomatic tier.
class JoWriter final : public jb::FnWriter {
public:
    JoWriter(JoObjects& program, const ir::MethodDecl* fn, const ir::ClassDecl* cls = nullptr)
        : jb::FnWriter(program, fn), o(program), cls(cls) {
        ret_struct = false;
    }

    JoObjects& o;
    const ir::ClassDecl* cls;   // the class whose member this is (none for a function or an initialiser)

    Lines write() override;
    // A method, constructor, destructor or static initialiser, as the lines of its class's body.
    Lines write_member();

    // An initialiser of the type as a Java expression (a fresh object or array where the type is one).
    Ex init_value(const ir::TypeP& type, Node* init);

    void local(const ir::Local& s, Lines& out) override;
    void ret(const ir::Return& s, Lines& out) override;
    void expr_stmt(Node* e, Lines& out) override;

    Var var(const ir::Sym& s) override;
    Place place(Node* e) override;
    Ex addr(Node* e) override;
    Ex load(const Place& pl) override;
    Ex store(const Place& pl, const Ex& val) override;
    Ex truth(const Ex& j, const ir::TypeP& type) override;
    Ex conv(const Ex& j, const ir::TypeP& from, const ir::TypeP& to) override;
    Ex compare(Node& e, std::string_view op) override;
    Ex incdec(Node& e, bool as_statement) override;

    Ex x_concat(Node& e) override;
    Ex x_length(Node& e) override;
    Ex x_lit(Node& e) override;
    Ex x_name(Node& e) override;
    Ex x_select(Node& e) override;
    Ex x_index(Node& e) override;
    Ex x_this(Node& e) override;
    Ex x_address_of(Node& e) override;
    Ex x_unary(Node& e) override;
    Ex x_assign(Node& e) override;
    Ex x_cast(Node& e) override;
    Ex x_cond(Node& e) override;
    Ex x_call(Node& e) override;
    Ex x_new(Node& e) override;
    Ex x_new_array(Node& e) override;
    Ex lib_call(Node& e, std::string_view name, std::span<Node* const> args) override;

private:
    std::string default_of(const ir::TypeP& type);
    // The parameters as Java declares them, and the lines that box a parameter whose address is taken.
    std::pair<Lines, Lines> params(const ir::MethodDecl& f);
    bool zero_init(Node* s, const ir::ClassDecl& c);
    bool has_destroy(const ir::TypeP& type) const;
    Ex aggregate(const ir::TypeP& type, std::span<Node* const> args);
    Ex copy_of(const Ex& v, const ir::TypeP& type);
    Ex allocation(const ir::Call& call, const ir::TypeP& type);
    bool is_size(Node* e, const ir::TypeP& element);
    std::optional<std::string> count(Node* e, const ir::TypeP* element);
    Lines call_args(std::span<Node* const> args, const std::vector<ir::TypeP>& params);
    Ex method_call(Node& e);
    const ir::MethodDecl* find_method(const ir::Sym& s) const;
    Ex construct(const ir::TypeP& type, Node& e);
    std::string field_text(const ir::Select& e);
    static std::string at(const Place& pl) { return pl.prec >= jb::kPost ? pl.java : "(" + pl.java + ")"; }
};

// ---- the program's checks and names ---------------------------------------------------------------------------------------------

void JoObjects::check_records() {
    for (const auto& [qname, node] : records) {
        const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(node);
        const std::string cname = c.name.value_or("");
        if (c.kind == "union") throw Refused("union " + cname);
        if (!c.implements.empty()) throw Refused(cname + " has more than one base class");
        for (const Node* m : c.members) {
            if (jo_has(jo_mods(m), "bitfield")) throw Refused("bit-fields in " + cname);
            if (const ir::FieldDecl* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym != nullptr && f->sym->name.starts_with("<")) {
                throw Refused("a member the front end did not lower in " + cname);
            }
        }
    }
    ctors_used.clear();
    copied.clear();
    assigned.clear();
    arrays_filled = false;
}

void JoObjects::make_names() {
    std::unordered_set<std::string> taken;
    for (const auto& [fname, fn] : functions) taken.insert(jb::jname(fname));
    for (const ir::FieldDecl* f : global_decls) taken.insert(jb::jname(f->sym->name));
    taken.insert(name);
    cls_names.clear();
    std::vector<std::pair<std::string, Node*>> ordered;   // by the line each is declared at, in the units' order where they tie
    for (const auto& [qname, node] : records) ordered.emplace_back(qname, node);
    std::stable_sort(ordered.begin(), ordered.end(), [](const auto& a, const auto& b) { return a.second->line < b.second->line; });
    for (const auto& [qname, node] : ordered) {
        const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(node);
        std::string base = text::replace_all(c.name.value_or(""), "__unnamed_", "Anon");
        if (qname.find('<') != std::string::npos) base += "_" + jo_template_suffix(qname);   // a template instance: Stack<i32, 8> -> Stack_int_8
        std::string n = jb::jname(base);
        while (taken.contains(n)) n += '_';
        taken.insert(n);
        cls_names[qname] = n;
    }
    layout_names.clear();
    for (const auto& [qname, n] : cls_names) layout_names.insert_or_assign(qname, n);
    used_names = std::move(taken);
}

// ---- types ----------------------------------------------------------------------------------------------------------------------

std::string JoObjects::cls(const ir::TypeP& type) {
    const std::string qn = rec_name(type);
    const auto it = cls_names.find(qn);
    if (it != cls_names.end()) return it->second;
    if (qn.starts_with("java.")) return qn.substr(qn.rfind('.') + 1);   // Java's own library (a program read back from Java)
    if (qn.starts_with("std.")) throw Refused("the C++ standard library (" + text::replace_all(qn, ".", "::") + ") is not carried yet");
    throw Refused("no class for " + jb::spell(type));
}

ir::ClassDecl* JoObjects::record_of(const ir::TypeP& type) const {
    Node* const* node = records.find(rec_name(type));
    return node != nullptr ? ir::get<ir::ClassDecl>(*node) : nullptr;
}

std::optional<std::int64_t> JoObjects::size_hint(const ir::TypeP& type) {
    try {
        return size(type);
    } catch (const Refused&) {
        return std::nullopt;
    }
}

bool JoObjects::is_enum(const ir::TypeP& type) const {
    if (jb::kind_of(type) != CKind::Class) return false;
    const ir::ClassDecl* c = record_of(type);
    return c != nullptr && c->kind == "enum";
}

std::string JoObjects::jt(const ir::TypeP& type) {
    const CKind k = jb::kind_of(type);
    if (const std::string_view prim = jb::java_primitive(k); !prim.empty()) return std::string(prim);
    switch (k) {
        case CKind::Void: return "void";
        case CKind::Class: case CKind::Record: return is_enum(type) ? "int" : cls(type);
        case CKind::Array: return jt(jo_array(type)->elem) + "[]";
        case CKind::Ptr: {
            const ir::TypeP& e = jb::pointee(type);
            if (jb::is_record(e)) return cls(e);
            if (jb::kind_of(e) == CKind::Void) throw Refused("a void pointer");
            if (jb::kind_of(e) == CKind::Fn) return iface(fn_sig_of_type(e));
            return jt(e) + "[]";
        }
        case CKind::FnPtr: return iface(fn_sig_of_type(type));
        case CKind::Null: return "Object";
        default: throw Refused("type " + jb::spell(type));
    }
}

jb::Sig JoObjects::fn_sig(const ir::Sym& sym) {
    jb::Sig sig;
    for (const ir::TypeP& p : sym.params) sig.params.push_back(jt(p));
    sig.ret = sym.ret ? jt(sym.ret) : "void";
    return sig;
}

jb::Sig JoObjects::fn_sig_of_type(const ir::TypeP& fn_or_fnptr) {
    ir::TypeP fn_type = fn_or_fnptr;
    if (const auto* fp = fn_or_fnptr ? std::get_if<ir::FnPtrType>(&fn_or_fnptr->body) : nullptr) fn_type = fp->fn;
    const auto* f = fn_type ? std::get_if<ir::FnType>(&fn_type->body) : nullptr;
    if (f == nullptr) throw Refused("a function type was expected: " + jb::spell(fn_or_fnptr));
    jb::Sig sig;
    for (const ir::FnParam& p : f->params) sig.params.push_back(jt(p.type));
    sig.ret = f->result.type ? jt(f->result.type) : "void";
    return sig;
}

// The functional interface of a signature, named by its parameter and result types (`IntIntToInt`, `ByteArrayToVoid`).
std::string JoObjects::iface(const jb::Sig& sig) {
    for (const auto& [known, iname] : fn_ifaces) {
        if (known == sig) return iname;
    }
    const auto word = [](std::string p) { return jo_upper_first(text::replace_all(p, "[]", "Array")); };
    std::string n;
    for (const std::string& p : sig.params) n += word(p);
    n += "To" + word(sig.ret);
    fn_ifaces.emplace_back(sig, n);
    return n;
}

std::string JoObjects::default_value(const ir::TypeP& type) {
    const CKind k = jb::kind_of(type);
    if (k == CKind::Class || k == CKind::Record) return "new " + cls(type) + "()";
    if (k == CKind::Array) {
        if (!jo_array(type)->count) return "null";   // (an array reference -- Java's own, read back -- not storage)
        std::vector<std::uint64_t> dims;
        ir::TypeP base = type;
        while (const ir::ArrayType* a = jo_array(base)) {
            if (!a->count) throw Refused("an array of unknown size");
            dims.push_back(*a->count);
            base = a->elem;
        }
        if (jb::is_record(base)) {
            if (dims.size() != 1) throw Refused("a multi-dimensional array of structs");
            arrays_filled = true;
            return "filled(new " + cls(base) + "[" + std::to_string(dims[0]) + "], " + cls(base) + "::new)";
        }
        std::string out = "new " + jt(base);
        for (const std::uint64_t d : dims) out += "[" + std::to_string(d) + "]";
        return out;
    }
    if (k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null) return "null";
    const std::string_view prim = jb::java_primitive(k);
    if (prim == "boolean") return "false";
    if (prim == "float") return "0.0f";
    if (prim == "double") return "0.0";
    if (prim == "long") return "0L";
    return "0";
}

// ---- the file ----------------------------------------------------------------------------------------------------------------------

std::string JoObjects::write() {
    fields_out.clear();
    static_init.clear();
    for (const ir::FieldDecl* g : global_decls) declare_global(*g);
    // (written until what the classes must offer -- aggregate, copy and assignment constructors -- is stable: a method written later
    // can ask it of a class written earlier)
    std::optional<std::array<std::size_t, 4>> seen;
    std::vector<Lines> funcs;
    Lines classes;
    for (int round = 0; round < 4; ++round) {
        gaps.clear();
        funcs.clear();
        classes.clear();
        for (ir::MethodDecl* f : all_functions()) {
            funcs.push_back(function_or_gap(*f, [&, f] { return JoWriter(*this, f).write(); }));
        }
        std::vector<std::string> order;
        for (const auto& [qname, node] : records) order.push_back(qname);
        std::sort(order.begin(), order.end(), [&](const std::string& a, const std::string& b) {
            const std::uint32_t la = records.at(a)->line;
            const std::uint32_t lb = records.at(b)->line;
            return la != lb ? la < lb : a < b;
        });
        for (const std::string& qn : order) jo_append(classes, class_decl(*ir::get<ir::ClassDecl>(records.at(qn))));
        const std::array<std::size_t, 4> now = {ctors_used.size(), copied.size(), assigned.size(), arrays_filled ? 1u : 0u};
        if (seen && *seen == now) break;
        seen = now;
    }
    Lines out = {"// " + name + ".java -- translated from " + (analysis.cpp_classes ? "C++" : "C") +
                     " by CodeBreaker (idiomatic tier: structs as classes, pointers as references).",
                 "import crt.*;", "", "import static crt.LibC.*;", "", "public final class " + name + " {"};
    jo_append(out, classes);
    for (const auto& [sig, iname] : fn_ifaces) {
        std::string ps;
        for (std::size_t i = 0; i < sig.params.size(); ++i) ps += (i != 0 ? ", " : "") + sig.params[i] + " a" + std::to_string(i);
        jo_append(out, {"", "    interface " + iname + " {", "        " + sig.ret + " call(" + ps + ");", "    }"});
    }
    if (!fields_out.empty()) {
        out.push_back("");
        for (const std::string& f : fields_out) out.push_back("    " + f);
    }
    if (!static_init.empty()) {
        jo_append(out, {"", "    static {"});
        for (const std::string& s : static_init) out.push_back("        " + s);
        out.push_back("    }");
    }
    for (const Lines& f : funcs) {
        out.push_back("");
        jo_append(out, f);
    }
    jo_append(out, entry());
    out.push_back("}");
    for (const std::string& g : gaps) notes.push_back("gap: " + g);
    return text::join(out, "\n") + "\n";
}

std::optional<std::string> JoObjects::gap_signature(const ir::MethodDecl& f, const std::optional<std::string>& owner) {
    jb::Sig sig;
    try {
        sig = fn_sig(*f.sym);
        if (jo_ref_ptr(f.ret) && !jb::is_record(jb::pointee(f.ret))) sig.ret = jt(jb::pointee(f.ret));
    } catch (const Refused&) {
        return std::nullopt;
    }
    std::string ps;
    for (std::size_t i = 0; i < std::min(sig.params.size(), f.params.size()); ++i) {
        ps += (i != 0 ? ", " : "") + sig.params[i] + " " + jb::jname(jo_param_name(f.params[i]));
    }
    if (!owner) return "static " + sig.ret + " " + jb::jname(f.name.value_or("")) + "(" + ps + ")";
    if (f.kind != "method") return std::nullopt;
    return std::string(jo_has(f.mods, "static") ? "static " : "") + sig.ret + " " + method_name(f.name.value_or("")) + "(" + ps + ")";
}

std::vector<std::string> JoObjects::entry() {
    ir::MethodDecl* const* found = functions.find("main");
    if (found == nullptr) {
        // a program read back from Java: its class with `static void main(String[])`
        for (const auto& [qname, node] : records) {
            for (const Node* x : ir::get<ir::ClassDecl>(node)->members) {
                const ir::MethodDecl* m = ir::get<ir::MethodDecl>(x);
                if (m != nullptr && m->name == "main" && jo_has(m->mods, "static") && m->params.size() == 1 &&
                    jo_is_array(jo_param_type(m->params[0]))) {
                    return {"", "    public static void main(String[] args) {", "        " + cls_names.at(qname) + ".main(args);", "    }"};
                }
            }
        }
        throw Refused("no main function");
    }
    const ir::MethodDecl& m = **found;
    Lines out = {"", "    public static void main(String[] args) {"};
    std::string call = "main()";
    if (m.params.size() >= 2) call = "main(args.length + 1, argv(System.getProperty(\"c.argv0\", \"" + name + "\"), args))";
    if (jb::kind_of(m.ret) == CKind::Void) {
        out.push_back("        " + call + ";");
        out.push_back("        exit(0);");
    } else {
        out.push_back("        exit(" + call + ");");
    }
    out.push_back("    }");
    return out;
}

// ---- globals --------------------------------------------------------------------------------------------------------------------------

void JoObjects::declare_global(const ir::FieldDecl& g) {
    const ir::Sym& s = *g.sym;
    const std::string jn = jb::jname(s.name);
    if (jo_has(g.mods, "const") && std::any_of(g.mods.begin(), g.mods.end(), [](const std::string& m) { return m.starts_with("enum:"); })) {
        const std::optional<std::string> value = jo_number_text(g.init);
        if (!value) throw Refused("an enum constant that is not a number: " + s.name);
        globals[s.name] = Var{jn, false, ir::i32_type(), VarKind::Enum};
        fields_out.push_back("static final int " + jn + " = " + *value + ";");
        return;
    }
    if (const auto earlier = globals.find(s.name); earlier != globals.end() && earlier->second.kind != VarKind::Enum) return;   // (a tentative definition again)
    const ir::TypeP& t = g.type;
    const bool boxed = addr_taken.contains(jb::VarKey{true, "", s.name}) && jo_boxable(t);
    globals[s.name] = Var{jn, boxed, t, VarKind::Global};
    JoWriter w(*this, nullptr);
    const Ex init = g.init != nullptr ? w.init_value(t, g.init) : Ex{default_value(t), jb::kAtom, jt(t), false};
    if (boxed) {
        fields_out.push_back("static " + jt(t) + "[] " + jn + " = {" + init.code + "};");
        return;
    }
    const CKind k = jb::kind_of(t);
    const std::string final_word = k == CKind::Array || k == CKind::Class || k == CKind::Record ? "final " : "";
    fields_out.push_back("static " + final_word + jt(t) + " " + jn + " = " + init.code + ";");
}

// ---- classes -------------------------------------------------------------------------------------------------------------------------

Lines JoObjects::class_decl(ir::ClassDecl& c) {
    const std::string qn = c.qname.value_or("");
    const std::string n = cls_names.at(qn);
    if (c.kind == "enum") {   // a C++ enum class: its constants
        Lines out = {"", "    /** enum class " + c.name.value_or("") + " */", "    static final class " + n + " {"};
        for (Node* m : c.members) {
            const ir::FieldDecl* f = ir::get<ir::FieldDecl>(m);
            const std::optional<std::string> value = f != nullptr ? jo_number_text(f->init) : std::nullopt;
            if (!value) throw Refused("an enum constant that is not a number in " + c.name.value_or(""));
            out.push_back("        static final int " + jb::jname(f->sym->name) + " = " + *value + ";");
        }
        out.push_back("    }");
        return out;
    }
    std::vector<ir::FieldDecl*> fields;
    std::vector<const ir::FieldDecl*> inst;
    std::vector<ir::MethodDecl*> methods;
    for (Node* m : c.members) {
        if (ir::FieldDecl* f = ir::get<ir::FieldDecl>(m)) {
            fields.push_back(f);
            if (!jo_has(f->mods, "static")) inst.push_back(f);
        } else if (ir::MethodDecl* md = ir::get<ir::MethodDecl>(m)) {
            methods.push_back(md);
        }
    }
    const bool abstract = jo_has(c.mods, "abstract") ||
                          std::any_of(methods.begin(), methods.end(), [](const ir::MethodDecl* m) { return jo_has(m->mods, "pure"); });
    std::string ext;
    const ir::ClassDecl* base = nullptr;
    if (c.extends) {
        base = record_of(c.extends);
        ext = " extends " + cls(c.extends);
    }
    const std::string what = c.kind.value_or("");
    const std::string title = text::replace_all(text::replace_all(qn, ".", "::"), "__unnamed_", "(anonymous) ");
    Lines out = {"", "    /** " + what + " " + title + " */", std::string("    static ") + (abstract ? "abstract " : "") + "class " + n + ext + " {"};
    Lines body;
    for (ir::FieldDecl* f : fields) {
        const ir::TypeP& ft = f->type;
        std::string st = jo_has(f->mods, "static") ? "static " : "";
        const std::string fname = jb::jname(f->sym->name);
        std::string init;
        if (f->init != nullptr) {
            JoWriter w(*this, nullptr, &c);
            init = " = " + w.init_value(ft, f->init).code;
        } else if (jb::kind_of(ft) == CKind::Array || jb::is_record(ft)) {
            init = " = " + default_value(ft);
        }
        if (!st.empty() && jo_has(f->mods, "const") && f->init != nullptr) st = "static final ";
        body.push_back(st + jt(ft) + " " + fname + init + ";");
    }
    std::vector<const ir::MethodDecl*> ctors;
    for (const ir::MethodDecl* m : methods) {
        if (m->kind == "ctor") ctors.push_back(m);
    }
    const bool user_copy = std::any_of(ctors.begin(), ctors.end(), [&](const ir::MethodDecl* m) {
        if (m->params.size() != 1) return false;
        const ir::TypeP pt = jo_param_type(m->params[0]);
        return jb::is_ptr(pt) && jb::is_record(jb::pointee(pt)) && rec_name(jb::pointee(pt)) == qn;
    });
    // a no-argument constructor where Java would not make one: beside an aggregate or a copy constructor
    const bool need_noarg = ctors.empty() && ((ctors_used.contains(qn) && !inst.empty()) || copied.contains(qn) || assigned.contains(qn));
    if (need_noarg) jo_append(body, {"", n + "() {", "}"});
    if (ctors_used.contains(qn) && !inst.empty()) {
        std::string ps;
        for (std::size_t i = 0; i < inst.size(); ++i) ps += (i != 0 ? ", " : "") + jt(inst[i]->type) + " " + jb::jname(inst[i]->sym->name);
        jo_append(body, {"", n + "(" + ps + ") {"});
        for (const ir::FieldDecl* f : inst) body.push_back("    this." + jb::jname(f->sym->name) + " = " + jb::jname(f->sym->name) + ";");
        body.push_back("}");
    }
    if ((copied.contains(qn) || assigned.contains(qn)) && !user_copy) {
        jo_append(body, {"", "/** C's copy of a " + what + " value. */", n + "(" + n + " o) {"});
        if (base != nullptr) body.push_back("    super(o);");
        jo_append(body, jo_indent(copy_fields(inst, "this", "o", true), "    "));
        body.push_back("}");
    }
    if (assigned.contains(qn) && std::none_of(methods.begin(), methods.end(), [](const ir::MethodDecl* m) { return m->name == "operator="; })) {
        jo_append(body, {"", "/** C's assignment of a " + what + " value: copies o into this one. */", n + " set(" + n + " o) {"});
        if (base != nullptr) body.push_back("    super.set(o);");
        jo_append(body, jo_indent(copy_fields(inst, "this", "o", false), "    "));
        jo_append(body, {"    return this;", "}"});
    }
    for (ir::MethodDecl* m : methods) {
        if (!ir::present(m->body) && qn.find('<') != std::string::npos && !jo_has(m->mods, "pure")) continue;   // (a template's member this instance never uses: clang made no body)
        if (m->kind == "dtor") {
            if (!ir::present(m->body) || (jo_statements(m->body).empty() && !jo_has(m->mods, "virtual"))) continue;
        }
        body.push_back("");
        jo_append(body, function_or_gap(*m, [&, m] { return JoWriter(*this, m, &c).write_member(); }, "", c.name.value_or("")));
    }
    jo_append(out, jo_indent(body, "        "));
    out.push_back("    }");
    return out;
}

Lines JoObjects::copy_fields(const std::vector<const ir::FieldDecl*>& fields, std::string_view dst, std::string_view src, bool fresh) {
    Lines out;
    const std::string d(dst);
    const std::string s(src);
    for (const ir::FieldDecl* f : fields) {
        const std::string fname = jb::jname(f->sym->name);
        const ir::TypeP& t = f->type;
        if (jb::is_record(t)) {
            out.push_back(fresh ? d + "." + fname + " = new " + cls(t) + "(" + s + "." + fname + ");" : d + "." + fname + ".set(" + s + "." + fname + ");");
            copied.insert(rec_name(t));
            if (!fresh) assigned.insert(rec_name(t));
        } else if (jo_is_array(t)) {
            ir::TypeP base = t;
            int depth = 0;
            while (const ir::ArrayType* a = jo_array(base)) {
                base = a->elem;
                ++depth;
            }
            if (jb::is_record(base)) {
                copied.insert(rec_name(base));
                if (fresh) {
                    out.push_back(d + "." + fname + " = new " + cls(base) + "[" + s + "." + fname + ".length];");
                    out.push_back("for (int i = 0; i < " + s + "." + fname + ".length; i++) " + d + "." + fname + "[i] = new " + cls(base) + "(" + s + "." + fname + "[i]);");
                } else {
                    assigned.insert(rec_name(base));
                    out.push_back("for (int i = 0; i < " + s + "." + fname + ".length; i++) " + d + "." + fname + "[i].set(" + s + "." + fname + "[i]);");
                }
            } else if (depth == 1) {
                out.push_back(fresh ? d + "." + fname + " = " + s + "." + fname + ".clone();"
                                    : "System.arraycopy(" + s + "." + fname + ", 0, " + d + "." + fname + ", 0, " + d + "." + fname + ".length);");
            } else {
                out.push_back(fresh ? d + "." + fname + " = deepCopy(" + s + "." + fname + ");" : "copyInto(" + d + "." + fname + ", " + s + "." + fname + ");");
            }
        } else {
            out.push_back(d + "." + fname + " = " + s + "." + fname + ";");
        }
    }
    return out;
}

// ---- a function: the file's lines ----------------------------------------------------------------------------------------------------

std::string JoWriter::default_of(const ir::TypeP& type) { return jb::is_record(type) ? std::string("null") : o.default_value(type); }

std::pair<Lines, Lines> JoWriter::params(const ir::MethodDecl& f) {
    Lines params;
    Lines pre;
    for (const Node* pn : f.params) {
        const ir::Param& prm = *ir::get<ir::Param>(pn);
        const std::string pname = jb::jname(prm.sym->name);
        declared.insert(pname);
        const ir::TypeP t = jo_param_type(pn);
        if (o.addr_taken.contains(jb::key_of(*prm.sym)) && jo_boxable(t)) {
            const std::string arg = pname + "Arg";
            params.push_back(o.jt(t) + " " + arg);
            pre.push_back(o.jt(t) + "[] " + pname + " = {" + arg + "};");
            vars[prm.sym->name] = Var{pname, true, t, VarKind::Param};
        } else {
            params.push_back(o.jt(t) + " " + pname);
            vars[prm.sym->name] = Var{pname, false, t, VarKind::Param};
        }
    }
    return {params, pre};
}

Lines JoWriter::write() {
    const ir::MethodDecl& f = *fn;
    auto [params_text, pre] = params(f);
    Lines body = pre;
    block_stmts(jo_statements(f.body), body);
    const std::string ret_type = f.ret ? o.jt(f.ret) : "void";
    if (ret_type != "void" && can_complete(f.body)) {
        body.push_back(f.name == "main" ? "return 0;" : "return " + default_of(f.ret) + ";  // (C: the function ends without a value)");
    }
    Lines out = {"    static " + ret_type + " " + jb::jname(f.name.value_or("")) + "(" + text::join(params_text, ", ") + ") {"};
    jo_append(out, jo_indent(body, "        "));
    out.push_back("    }");
    return out;
}

bool JoWriter::zero_init(Node* s, const ir::ClassDecl& c) {
    const ir::ExprStmt* es = ir::get<ir::ExprStmt>(s);
    const ir::Assign* a = es != nullptr ? ir::get<ir::Assign>(es->expr) : nullptr;
    if (a == nullptr || a->op) return false;
    const ir::Select* t = ir::get<ir::Select>(jb::strip(a->target));
    if (t == nullptr) return false;
    const ir::Unary* deref = ir::get<ir::Unary>(jb::strip(t->obj));
    if (deref == nullptr || !ir::is<ir::This>(jb::strip(deref->x))) return false;
    Node* v = jb::strip(a->value);
    while (const ir::Cast* cast = ir::get<ir::Cast>(v)) v = jb::strip(cast->x);
    if (!jo_zero_literal(v) || t->sym == nullptr) return false;
    for (const Node* m : c.members) {
        const ir::FieldDecl* f = ir::get<ir::FieldDecl>(m);
        if (f != nullptr && f->sym != nullptr && f->sym->name == t->sym->name) {
            const CKind k = jb::kind_of(f->type);
            return f->init == nullptr && k != CKind::Class && k != CKind::Record && k != CKind::Array;
        }
    }
    return false;
}

Lines JoWriter::write_member() {
    const ir::MethodDecl& m = *fn;
    const ir::ClassDecl& c = *cls;
    const std::string n = o.cls_names.at(c.qname.value_or(""));
    auto [params_text, pre] = params(m);
    const std::string kind = m.kind.value_or("");
    if (kind == "ctor") {
        Lines body;
        std::vector<Node*> stmts = ir::present(m.body) ? jo_statements(m.body) : std::vector<Node*>{};
        if (!stmts.empty()) {
            if (const ir::CtorCall* cc = ir::get<ir::CtorCall>(stmts.front())) {
                std::vector<ir::TypeP> types = cc->sym != nullptr ? cc->sym->params : std::vector<ir::TypeP>{};
                body.push_back(std::string(cc->kind && cc->kind->starts_with("super") ? "super" : "this") + "(" + text::join(call_args(cc->args, types), ", ") + ");");
                stmts.erase(stmts.begin());
            }
        }
        // a member initialised to zero before anything else: a Java field already is
        while (!stmts.empty() && zero_init(stmts.front(), c)) stmts.erase(stmts.begin());
        jo_append(body, pre);
        block_stmts(stmts, body);
        Lines out = {n + "(" + text::join(params_text, ", ") + ") {"};
        jo_append(out, jo_indent(body, "    "));
        out.push_back("}");
        return out;
    }
    if (kind == "static_init") {
        Lines body;
        block_stmts(jo_statements(m.body), body);
        if (body.empty()) return {};
        Lines out = {"static {"};
        jo_append(out, jo_indent(body, "    "));
        out.push_back("}");
        return out;
    }
    if (kind == "dtor") {
        Lines body = pre;
        block_stmts(jo_statements(m.body), body);
        const ir::ClassDecl* base = c.extends ? o.record_of(c.extends) : nullptr;
        if (base != nullptr) {
            const bool base_destroys = std::any_of(base->members.begin(), base->members.end(), [](const Node* x) {
                const ir::MethodDecl* d = ir::get<ir::MethodDecl>(x);
                return d != nullptr && d->kind == "dtor" && ir::present(d->body) && (!jo_statements(d->body).empty() || jo_has(d->mods, "virtual"));
            });
            if (base_destroys) body.push_back("super.destroy();");
        }
        Lines out = {"/** ~" + c.name.value_or("") + "() */", "void destroy() {"};
        jo_append(out, jo_indent(body, "    "));
        out.push_back("}");
        return out;
    }
    std::string ret_type = m.ret ? o.jt(m.ret) : "void";
    if (jo_ref_ptr(m.ret) && !jb::is_record(jb::pointee(m.ret))) {
        throw Refused(m.name.value_or("") + " returns a reference to a " + jb::spell(jb::pointee(m.ret)));
    }
    if (jo_ref_ptr(m.ret)) ret_type = o.jt(jb::pointee(m.ret));
    const std::string mname = method_name(m.name.value_or(""));
    if (!ir::present(m.body)) {
        if (jo_has(m.mods, "pure")) return {"abstract " + ret_type + " " + mname + "(" + text::join(params_text, ", ") + ");"};
        throw Refused(c.name.value_or("") + "::" + m.name.value_or("") + " is declared but not defined");
    }
    Lines body = pre;
    block_stmts(jo_statements(m.body), body);
    if (ret_type != "void" && can_complete(m.body)) body.push_back("return " + default_of(m.ret) + ";");
    Lines out = {std::string(jo_has(m.mods, "static") ? "static " : "") + ret_type + " " + mname + "(" + text::join(params_text, ", ") + ") {"};
    jo_append(out, jo_indent(body, "    "));
    out.push_back("}");
    return out;
}

// ---- statements ----------------------------------------------------------------------------------------------------------------------

void JoWriter::local(const ir::Local& s, Lines& out) {
    std::string lname = jb::jname(s.sym->name);
    if (declared.contains(lname)) lname = fresh(lname + "_");
    declared.insert(lname);
    const ir::TypeP& t = s.type;
    if (jo_has(s.mods, "extern")) return;
    if (jo_has(s.mods, "static")) {
        static_local(s, lname);
        return;
    }
    if (jb::is_record(t) && has_destroy(t)) {
        throw Refused(o.cls(t) + " " + s.sym->name + " has a destructor that runs at the end of its scope (RAII: try/finally not written yet)");
    }
    const bool boxed = o.addr_taken.contains(jb::key_of(*s.sym)) && jo_boxable(t);
    vars[s.sym->name] = Var{lname, boxed, t, VarKind::Local};
    const std::string jtype = o.jt(t);
    const std::string init = s.init == nullptr ? o.default_value(t) : init_value(t, s.init).code;
    out.push_back(boxed ? jtype + "[] " + lname + " = {" + init + "};" : jtype + " " + lname + " = " + init + ";");
}

Ex JoWriter::init_value(const ir::TypeP& t, Node* init_in) {
    Node* init = jb::strip(init_in);
    const ir::Lit* lit = ir::get<ir::Lit>(init);
    const std::string* text = lit != nullptr ? std::get_if<std::string>(&lit->value) : nullptr;
    const ir::ArrayType* arr = jo_array(t);
    if (text != nullptr && arr != nullptr) {
        const std::uint64_t n = arr->count ? *arr->count : jb::char_count(*text) + 1;
        return {"bytes(" + jb::java_string(*text) + ", " + std::to_string(n) + ")", jb::kPost, "byte[]", false};
    }
    if (const ir::NewArray* na = ir::get<ir::NewArray>(init); na != nullptr && arr != nullptr) {
        if (!na->dims.empty() && na->init.empty()) return x_new_array(*init);   // new T[n]
        std::vector<std::string> items;
        for (Node* x : na->init) items.push_back(init_value(arr->elem, x).code);
        const std::uint64_t n = arr->count ? *arr->count : items.size();
        const std::string jtype = o.jt(t);
        if (items.empty()) return {o.default_value(t), jb::kPost, jtype, false};
        if (items.size() < n) {
            if (jb::is_record(arr->elem)) items.insert(items.end(), n - items.size(), "new " + o.cls(arr->elem) + "()");
            else if (jo_is_array(arr->elem)) items.insert(items.end(), n - items.size(), o.default_value(arr->elem));
            else return {"java.util.Arrays.copyOf(new " + jtype + "{" + text::join(items, ", ") + "}, " + std::to_string(n) + ")", jb::kPost, jtype, false};
        }
        return {"new " + jtype + "{" + text::join(items, ", ") + "}", jb::kPost, jtype, false};
    }
    if (const ir::New* nw = ir::get<ir::New>(init); nw != nullptr && jb::is_record(t) && nw->sym != nullptr && nw->sym->name.starts_with("<aggregate")) {
        return aggregate(t, nw->args);
    }
    if (jb::is_record(t)) {
        const Ex v = value(init);
        return v.fresh ? v : copy_of(v, t);
    }
    return conv(value(init), init->type, t);
}

Ex JoWriter::aggregate(const ir::TypeP& t, std::span<Node* const> args) {
    const ir::ClassDecl* c = o.record_of(t);
    std::vector<const ir::FieldDecl*> fields;
    for (const Node* m : c->members) {
        const ir::FieldDecl* f = ir::get<ir::FieldDecl>(m);
        if (f != nullptr && !jo_has(f->mods, "static")) fields.push_back(f);
    }
    const std::string cname = o.cls(t);
    if (args.empty()) return {"new " + cname + "()", jb::kPost, cname, true};
    o.ctors_used.insert(c->qname.value_or(""));
    std::vector<std::string> vals;
    for (std::size_t i = 0; i < fields.size(); ++i) vals.push_back(i < args.size() ? init_value(fields[i]->type, args[i]).code : o.default_value(fields[i]->type));
    return {"new " + cname + "(" + text::join(vals, ", ") + ")", jb::kPost, cname, true};
}

Ex JoWriter::copy_of(const Ex& v, const ir::TypeP& t) {
    o.copied.insert(o.rec_name(t));
    const std::string cname = o.cls(t);
    return {"new " + cname + "(" + v.code + ")", jb::kPost, cname, true};
}

void JoWriter::ret(const ir::Return& s, Lines& out) {
    const ir::MethodDecl* f = fn;
    const ir::TypeP rt = f != nullptr ? f->ret : ir::TypeP{};
    const bool has_value_type = rt && jb::kind_of(rt) != CKind::Void;
    if (s.expr == nullptr) {
        const std::string fk = f != nullptr ? f->kind.value_or("") : "";
        if (has_value_type && (fk == "method" || fk == "function")) out.push_back("return " + default_of(rt) + ";  // (C: a return without a value)");
        else out.push_back("return;");
        return;
    }
    const std::string fk = f != nullptr ? f->kind.value_or("") : "";
    if (fk == "ctor" || fk == "dtor" || !has_value_type) {
        expr_stmt(s.expr, out);
        out.push_back("return;");
        return;
    }
    if (jo_ref_ptr(rt)) {
        Node* e = jb::strip(s.expr);
        const ir::Unary* u = ir::get<ir::Unary>(e);
        out.push_back("return " + value(u != nullptr && u->op == "&" ? u->x : s.expr).code + ";");
        return;
    }
    if (jb::is_record(rt)) {
        Node* e = jb::strip(s.expr);
        const Ex v = value(e);
        const ir::Name* nm = ir::get<ir::Name>(e);
        const bool is_local = nm != nullptr && nm->sym != nullptr && (nm->sym->kind == ir::SymKind::Local || nm->sym->kind == ir::SymKind::Param) &&
                              !o.addr_taken.contains(jb::key_of(*nm->sym));
        out.push_back("return " + (v.fresh || is_local ? v.code : copy_of(v, rt).code) + ";");
        return;
    }
    out.push_back("return " + conv(value(s.expr), s.expr->type, rt).code + ";");
}

void JoWriter::expr_stmt(Node* e, Lines& out) {
    Node* e0 = jb::strip(e);
    if (const ir::Call* c = ir::get<ir::Call>(e0); c != nullptr && c->target == nullptr && c->sym != nullptr && c->sym->name == "free" &&
                                                    !o.functions.contains(c->sym->name)) {
        out.push_back("// free(" + value(c->args.at(0)).code + "): the collector frees it");
        return;
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(e0); u != nullptr && (u->op == "delete" || u->op == "delete[]")) {
        const Ex x = value(u->x);
        const ir::TypeP& t = u->x->type;
        if (jb::kind_of(t) == CKind::Ptr && jb::is_record(jb::pointee(t)) && has_destroy(jb::pointee(t)) && u->op == "delete") {
            out.push_back(x.at(jb::kPost) + ".destroy();");
        } else {
            out.push_back("// " + u->op + " " + x.code + ": the collector frees it");
        }
        return;
    }
    jb::FnWriter::expr_stmt(e, out);
}

bool JoWriter::has_destroy(const ir::TypeP& t) const {
    for (const ir::ClassDecl* c = o.record_of(t); c != nullptr; c = c->extends ? o.record_of(c->extends) : nullptr) {
        for (const Node* x : c->members) {
            const ir::MethodDecl* m = ir::get<ir::MethodDecl>(x);
            if (m != nullptr && m->kind == "dtor" && ir::present(m->body) && (!jo_statements(m->body).empty() || jo_has(m->mods, "virtual"))) return true;
        }
    }
    return false;
}

// ---- places --------------------------------------------------------------------------------------------------------------------------

Var JoWriter::var(const ir::Sym& s) {
    if ((s.kind == ir::SymKind::Local || s.kind == ir::SymKind::Param)) {
        const auto it = vars.find(s.name);
        if (it != vars.end()) return it->second;
    }
    const auto g = o.globals.find(s.name);
    if (g != o.globals.end()) return g->second;
    throw Refused("unknown variable " + s.name);
}

// An lvalue as a Place whose `java` is a Java lvalue (it is never in the emulated memory here).
Place JoWriter::place(Node* e_in) {
    Node* e = jb::strip(e_in);
    if (const ir::Name* n = ir::get<ir::Name>(e)) {
        const Var v = var(*n->sym);
        return {v.mem ? v.java + "[0]" : v.java, false, v.type, jb::kPost};
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(e); u != nullptr && u->op == "*") {
        const Ex x = value(u->x);
        if (jb::is_record(e->type)) return {x.code, false, e->type, x.prec};
        return {x.at(jb::kPost) + "[0]", false, e->type, jb::kPost};
    }
    if (const ir::Index* ix = ir::get<ir::Index>(e)) {
        const Ex a = value(ix->arr);
        const auto* index_node = std::get_if<Node*>(&ix->index);
        if (index_node == nullptr || *index_node == nullptr) throw Refused("an index with several arguments");
        const Ex i = conv(value(*index_node), (*index_node)->type, ir::i32_type());
        return {a.at(jb::kPost) + "[" + i.code + "]", false, e->type, jb::kPost};
    }
    if (const ir::Select* sel = ir::get<ir::Select>(e)) return {field_text(*sel), false, e->type, jb::kPost};
    if (const ir::Cast* c = ir::get<ir::Cast>(e)) return place(c->x);
    throw Refused("an lvalue " + std::string(ir::kind_name(*e)));
}

std::string JoWriter::field_text(const ir::Select& e) {
    const std::string fname = jb::jname(e.sym->name);
    Node* obj = jb::strip(e.obj);
    if (ir::is<ir::TypeName>(obj)) {
        const std::string owner = o.cls(obj->type);
        if (cls != nullptr) {
            const auto it = o.cls_names.find(cls->qname.value_or(""));
            if (it != o.cls_names.end() && it->second == owner) return fname;
        }
        return owner + "." + fname;
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(obj); u != nullptr && u->op == "*" && ir::is<ir::This>(jb::strip(u->x))) {
        return declared.contains(fname) ? "this." + fname : fname;
    }
    const Ex object = value(obj);
    return object.at(jb::kPost) + "." + fname;
}

Ex JoWriter::load(const Place& pl) { return {pl.java, pl.prec, o.jt(pl.type), false}; }

Ex JoWriter::store(const Place& pl, const Ex& val) {
    const ir::TypeP& t = pl.type;
    if (jb::is_record(t)) {
        o.assigned.insert(o.rec_name(t));
        return {at(pl) + ".set(" + val.code + ")", jb::kPost, o.jt(t), false};
    }
    return {pl.java + " = " + val.at(jb::kAssign), jb::kAssign, o.jt(t), false};
}

Ex JoWriter::addr(Node* e_in) {
    Node* e = jb::strip(e_in);
    if (const ir::Name* n = ir::get<ir::Name>(e)) {
        const Var v = var(*n->sym);
        return {v.java, jb::kAtom, o.jt(e->type), false};
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(e); u != nullptr && u->op == "*") return value(u->x);
    if (jb::is_record(e->type) || jo_is_array(e->type)) return value(e);
    throw Refused("the address of " + std::string(ir::kind_name(*e)));
}

// ---- values ----------------------------------------------------------------------------------------------------------------------------

Ex JoWriter::truth(const Ex& j, const ir::TypeP& t) {
    if (j.jt == "boolean") return j;
    const CKind k = jb::kind_of(t);
    if (t && (k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null)) return {j.at(jb::kEq + 1) + " != null", jb::kEq, "boolean", false};
    return jb::FnWriter::truth(j, t);
}

// Java string concatenation (a program read back from Java): Java's own semantics.
Ex JoWriter::x_concat(Node& e) {
    const ir::Concat& c = *ir::get<ir::Concat>(&e);
    std::vector<Ex> parts;
    for (Node* x : c.parts) parts.push_back(value(x));
    if (parts.empty()) return {"\"\"", jb::kAtom, "String", false};
    if (parts[0].jt != "String" && !(parts.size() > 1 && parts[1].jt == "String")) parts.insert(parts.begin(), Ex{"\"\"", jb::kAtom, "String", false});
    std::string code = parts[0].at(jb::kAdd);
    for (std::size_t i = 1; i < parts.size(); ++i) code += " + " + parts[i].at(jb::kAdd + 1);
    return {code, jb::kAdd, "String", false};
}

Ex JoWriter::x_length(Node& e) { return {value(ir::get<ir::Length>(&e)->arr).at(jb::kPost) + ".length", jb::kPost, "int", false}; }

Ex JoWriter::x_lit(Node& e) {
    const ir::Lit& lit = *ir::get<ir::Lit>(&e);
    const CKind k = jb::kind_of(e.type);
    if (std::holds_alternative<std::monostate>(lit.value) && (!e.type || k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null)) {
        return {"null", jb::kAtom, "Object", false};
    }
    if (const std::string* text = std::get_if<std::string>(&lit.value)) {
        const auto* cls_type = e.type ? std::get_if<ir::ClassType>(&e.type->body) : nullptr;
        if (cls_type != nullptr && cls_type->name == "java.lang.String") return {jb::java_wstring(*text), jb::kAtom, "String", false};
        if (jb::is_wide(e)) throw Refused("a wide string outside a library call");
        return {"bytes(" + jb::java_string(*text) + ")", jb::kPost, "byte[]", false};
    }
    return jb::FnWriter::x_lit(e);
}

Ex JoWriter::x_name(Node& e) {
    const Var v = var(*ir::get<ir::Name>(&e)->sym);
    if (v.mem) return {v.java + "[0]", jb::kPost, o.jt(v.type), false};
    return {v.java, jb::kAtom, o.jt(v.type), false};
}

Ex JoWriter::x_select(Node& e) { return {field_text(*ir::get<ir::Select>(&e)), jb::kPost, o.jt(e.type), false}; }
Ex JoWriter::x_index(Node& e) { return load(place(&e)); }

Ex JoWriter::x_this(Node& e) {
    (void)e;
    if (cls == nullptr) throw Refused("this outside a method");
    return {"this", jb::kAtom, o.cls(ir::class_type(cls->qname.value_or(""))), false};
}

Ex JoWriter::x_address_of(Node& e) {
    const std::string& fname = ir::get<ir::AddressOf>(&e)->sym->name;
    if (!o.functions.contains(fname)) throw Refused("the address of library function " + fname);
    return {o.name + "::" + jb::jname(fname), jb::kPost, o.jt(e.type), false};
}

Ex JoWriter::x_unary(Node& e) {
    const ir::Unary& u = *ir::get<ir::Unary>(&e);
    if (u.op == "*") {
        Node* x = jb::strip(u.x);
        if (ir::is<ir::This>(x)) return {"this", jb::kAtom, o.jt(e.type), false};
        if (jb::is_record(e.type)) return value(u.x);
        return load(place(&e));
    }
    if (u.op == "&") {
        Node* x = jb::strip(u.x);
        if (ir::is<ir::AddressOf>(x)) return value(x);
        return addr(x);
    }
    if ((u.op == "++" || u.op == "--") && jb::is_ptr(e.type)) throw Refused("pointer arithmetic");
    return jb::FnWriter::x_unary(e);
}

Ex JoWriter::incdec(Node& e, bool) {
    const ir::Unary& u = *ir::get<ir::Unary>(&e);
    const Place pl = place(u.x);
    if (jb::kind_of(e.type) == CKind::Bool) throw Refused("++ on a bool");
    const std::string code = u.postfix ? pl.java + u.op : u.op + pl.java;
    return {code, u.postfix ? jb::kPost : jb::kUnary, o.jt(e.type), false};
}

Ex JoWriter::compare(Node& e, std::string_view op) {
    const ir::Binary& b = *ir::get<ir::Binary>(&e);
    const auto refish = [](const ir::TypeP& t) {
        const CKind k = jb::kind_of(t);
        return t && (k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null);
    };
    if (refish(b.left->type) || refish(b.right->type)) {
        const Ex l = value(b.left);
        const Ex r = value(b.right);
        return {l.at(jb::kEq) + " " + std::string(op) + " " + r.at(jb::kEq + 1), jb::kEq, "boolean", false};
    }
    return jb::FnWriter::compare(e, op);
}

Ex JoWriter::x_assign(Node& e) {
    const ir::Assign& a = *ir::get<ir::Assign>(&e);
    if (!a.op) {
        const ir::TypeP& t = a.target->type;
        const Place pl = place(a.target);
        if (jb::is_record(t)) return store(pl, value(a.value));
        const Ex v = value(a.value);
        return store(pl, conv(v, a.value->type, t));
    }
    return jb::FnWriter::x_assign(e);
}

Ex JoWriter::x_cast(Node& e) {
    const ir::Cast& c = *ir::get<ir::Cast>(&e);
    Node* x = jb::strip(c.x);
    const ir::TypeP& t = e.type;
    const ir::TypeP& st = x->type;
    const CKind tk = jb::kind_of(t);
    if (tk == CKind::Void) return value(x);
    if ((tk == CKind::Ptr || tk == CKind::FnPtr) && jb::is_null_constant(x)) return {"null", jb::kAtom, "Object", false};
    if (const ir::Call* call = ir::get<ir::Call>(x); call != nullptr && call->target == nullptr && call->sym != nullptr &&
                                                      (call->sym->name == "malloc" || call->sym->name == "calloc") && !o.functions.contains(call->sym->name)) {
        return allocation(*call, t);
    }
    if (jo_is_array(st) && jb::is_ptr(t)) return value(x);   // an array is its own pointer
    if (const ir::Lit* lit = ir::get<ir::Lit>(x); lit != nullptr && std::holds_alternative<std::monostate>(lit->value)) return {"null", jb::kAtom, "Object", false};
    if (ir::is<ir::AddressOf>(x)) return value(x);
    return conv(value(x), st, t);
}

Ex JoWriter::conv(const Ex& j, const ir::TypeP& s, const ir::TypeP& t) {
    if (!s || !t) return j;
    const auto refish = [](CKind k) {
        return k == CKind::Ptr || k == CKind::FnPtr || k == CKind::Null || k == CKind::Class || k == CKind::Record || k == CKind::Array;
    };
    const CKind sk = jb::kind_of(s);
    const CKind tk = jb::kind_of(t);
    if (refish(sk) || refish(tk)) {
        if (tk == CKind::Bool) return {j.at(jb::kEq + 1) + " != null", jb::kEq, "boolean", false};
        if (refish(sk) != refish(tk) && tk != CKind::Void) throw Refused("a pointer used as an integer, or the reverse");
        return j;
    }
    return jb::FnWriter::conv(j, s, t);
}

// malloc/calloc(...) converted to T*: a new object or array of T.
Ex JoWriter::allocation(const ir::Call& call, const ir::TypeP& t) {
    if (jb::kind_of(t) != CKind::Ptr || jb::kind_of(jb::pointee(t)) == CKind::Void) throw Refused("an allocation of no type");
    const ir::TypeP el = jb::pointee(t);
    std::optional<std::string> n;
    if (call.sym->name == "calloc") {
        if (is_size(call.args.at(1), el)) n = count(call.args.at(0), nullptr);
    } else {
        n = count(call.args.at(0), &el);
    }
    if (!n) throw Refused("an allocation whose size is not a count of " + jb::spell(el));
    if (jb::is_record(el)) {
        const std::string cname = o.cls(el);
        if (*n == "1") return {"new " + cname + "()", jb::kPost, cname, true};
        o.arrays_filled = true;
        return {"filled(new " + cname + "[" + *n + "], " + cname + "::new)", jb::kPost, cname + "[]", false};
    }
    if (!jo_is_array(el)) return {"new " + o.jt(el) + "[" + *n + "]", jb::kPost, o.jt(t), false};
    ir::TypeP base = el;
    std::string dims;
    while (const ir::ArrayType* a = jo_array(base)) {
        dims += "[" + std::to_string(a->count.value_or(0)) + "]";
        base = a->elem;
    }
    return {"new " + o.jt(base) + "[" + *n + "]" + dims, jb::kPost, o.jt(t), false};
}

bool JoWriter::is_size(Node* e_in, const ir::TypeP& element) {
    Node* e = jb::strip(e_in);
    if (const ir::Cast* c = ir::get<ir::Cast>(e)) e = jb::strip(c->x);
    const ir::Lit* lit = ir::get<ir::Lit>(e);
    if (lit == nullptr) return false;
    const std::optional<std::int64_t> size = o.size_hint(element);
    if (!size) return false;
    if (const auto* i = std::get_if<std::int64_t>(&lit->value)) return *i == *size;
    if (const auto* u = std::get_if<std::uint64_t>(&lit->value)) return *u == static_cast<std::uint64_t>(*size);
    return false;
}

// The element count an allocation size expresses (`n * sizeof(T)`, `sizeof(T)`, a byte count of chars) as Java int text.
std::optional<std::string> JoWriter::count(Node* e_in, const ir::TypeP* element) {
    Node* e = jb::strip(e_in);
    while (const ir::Cast* c = ir::get<ir::Cast>(e)) e = jb::strip(c->x);
    if (element == nullptr) return conv(value(e), e->type, ir::i32_type()).code;
    if (is_size(e, *element)) return "1";
    if (const ir::Binary* b = ir::get<ir::Binary>(e); b != nullptr && b->op == "*") {
        for (const auto& [a, other] : {std::pair{b->left, b->right}, std::pair{b->right, b->left}}) {
            if (!is_size(other, *element)) continue;
            Node* a0 = jb::strip(a);
            while (const ir::Cast* c = ir::get<ir::Cast>(a0)) a0 = jb::strip(c->x);
            return conv(value(a0), a0->type, ir::i32_type()).code;
        }
    }
    if (o.size_hint(*element) == 1) return conv(value(e), e->type, ir::i32_type()).code;
    return std::nullopt;
}

// ---- calls ------------------------------------------------------------------------------------------------------------------------------

Ex JoWriter::x_call(Node& e) {
    const ir::Call& c = *ir::get<ir::Call>(&e);
    if (c.sym != nullptr && (c.sym->kind == ir::SymKind::Method || c.sym->kind == ir::SymKind::Ctor) && c.target != nullptr) return method_call(e);
    if (c.sym != nullptr && c.target == nullptr && o.functions.contains(c.sym->name)) {
        const ir::MethodDecl* f = o.function_for(*c.sym);
        const Lines ac = call_args(c.args, jo_param_types(*f));
        const ir::TypeP& rt = f->ret;
        return {jb::jname(c.sym->name) + "(" + text::join(ac, ", ") + ")", jb::kPost, rt ? o.jt(rt) : "void", rt && jb::is_record(rt)};
    }
    if (c.sym != nullptr && c.target == nullptr && (c.sym->name == "malloc" || c.sym->name == "calloc")) throw Refused("an allocation used without a type");
    Node* target = c.target != nullptr ? jb::strip(c.target) : nullptr;
    // (*fp)(...) calls fp
    while (const ir::Unary* u = ir::get<ir::Unary>(target)) {
        if (u->op != "*" || jb::kind_of(u->x->type) != CKind::FnPtr) break;
        target = jb::strip(u->x);
    }
    if (target != nullptr && (jb::kind_of(target->type) == CKind::FnPtr || jb::kind_of(target->type) == CKind::Fn)) {
        const Ex fn_value = value(target);
        const ir::TypeP ft = jb::kind_of(target->type) == CKind::FnPtr ? target->type : ir::make_type(ir::FnPtrType{ir::AddressSpace::Flat, target->type});
        const auto& fn_type = std::get<ir::FnType>(std::get<ir::FnPtrType>(ft->body).fn->body);
        Lines ac;
        for (std::size_t i = 0; i < c.args.size(); ++i) {
            Node* a = c.args[i];
            const ir::TypeP pt = i < fn_type.params.size() ? fn_type.params[i].type : a->type;
            ac.push_back(conv(value(a), a->type, pt).code);
        }
        return {fn_value.at(jb::kPost) + ".call(" + text::join(ac, ", ") + ")", jb::kPost, o.fn_sig_of_type(ft).ret, false};
    }
    if (c.sym != nullptr && c.target == nullptr) return lib_call(e, c.sym->name, c.args);
    throw Refused("a call this back end cannot place");
}

Lines JoWriter::call_args(std::span<Node* const> args, const std::vector<ir::TypeP>& params) {
    Lines out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        Node* a = args[i];
        const ir::TypeP pt = i < params.size() ? params[i] : a->type;
        if (jb::is_record(pt)) {
            const Ex v = value(a);
            out.push_back(v.fresh ? v.code : copy_of(v, pt).code);
        } else if (jo_ref_ptr(pt)) {
            Node* aa = jb::strip(a);
            const ir::Unary* u = ir::get<ir::Unary>(aa);
            out.push_back(u != nullptr && u->op == "&" ? addr(u->x).code : value(a).code);
        } else {
            out.push_back(conv(value(a), a->type, pt).code);
        }
    }
    return out;
}

const ir::MethodDecl* JoWriter::find_method(const ir::Sym& s) const {
    ir::ClassDecl* c = s.owner ? [&]() -> ir::ClassDecl* {
        Node* const* node = o.records.find(*s.owner);
        return node != nullptr ? ir::get<ir::ClassDecl>(*node) : nullptr;
    }() : nullptr;
    for (; c != nullptr; c = c->extends ? o.record_of(c->extends) : nullptr) {
        for (const Node* x : c->members) {
            const ir::MethodDecl* m = ir::get<ir::MethodDecl>(x);
            if (m != nullptr && m->name == s.name && m->params.size() == s.params.size()) return m;
        }
    }
    return nullptr;
}

Ex JoWriter::method_call(Node& e) {
    const ir::Call& c = *ir::get<ir::Call>(&e);
    const ir::Sym& s = *c.sym;
    Node* target = jb::strip(c.target);
    const std::string mname = method_name(s.name);
    const ir::MethodDecl* m = find_method(s);
    const std::vector<ir::TypeP> params = m != nullptr ? jo_param_types(*m) : s.params;
    const Lines ac = call_args(c.args, params);
    const ir::TypeP& rt = e.type;
    const std::string jtype = rt && jb::kind_of(rt) != CKind::Void ? o.jt(rt) : "void";
    const bool fresh = rt && jb::is_record(rt) && !(m != nullptr && m->ret && jb::kind_of(m->ret) == CKind::Ptr);
    const std::string args_text = "(" + text::join(ac, ", ") + ")";
    if (ir::is<ir::TypeName>(target)) {
        const std::string owner = o.cls(target->type);
        const bool here = cls != nullptr && o.cls_names.count(cls->qname.value_or("")) != 0 && o.cls_names.at(cls->qname.value_or("")) == owner;
        return {(here ? mname : owner + "." + mname) + args_text, jb::kPost, jtype, fresh};
    }
    if (const ir::Unary* u = ir::get<ir::Unary>(target); u != nullptr && u->op == "*" && ir::is<ir::This>(jb::strip(u->x))) {
        return {mname + args_text, jb::kPost, jtype, fresh};
    }
    const Ex object = value(target);
    return {object.at(jb::kPost) + "." + mname + args_text, jb::kPost, jtype, fresh};
}

Ex JoWriter::x_new(Node& e) {
    const ir::New& n = *ir::get<ir::New>(&e);
    const ir::TypeP& t = e.type;
    if (n.memory == ir::MemoryModel::Manual && jb::kind_of(t) == CKind::Ptr) {
        const ir::TypeP el = jb::pointee(t);
        if (jb::is_record(el)) return construct(el, e);
        if (!n.args.empty()) {
            return {"new " + o.jt(el) + "[] {" + conv(value(n.args[0]), n.args[0]->type, el).code + "}", jb::kPost, o.jt(t), false};
        }
        return {"new " + o.jt(el) + "[1]", jb::kPost, o.jt(t), false};
    }
    if (jb::is_record(t)) return construct(t, e);
    throw Refused("new (" + (n.memory ? std::string(ir::to_text(*n.memory)) : std::string("None")) + ")");
}

Ex JoWriter::construct(const ir::TypeP& t, Node& e) {
    const ir::New& n = *ir::get<ir::New>(&e);
    if (n.sym != nullptr && n.sym->name.starts_with("<aggregate")) return aggregate(t, n.args);
    const ir::ClassDecl* c = o.record_of(t);
    const ir::MethodDecl* ctor = nullptr;
    if (c != nullptr && n.sym != nullptr) {
        for (const Node* x : c->members) {
            const ir::MethodDecl* m = ir::get<ir::MethodDecl>(x);
            if (m == nullptr || m->kind != "ctor" || m->params.size() != n.args.size()) continue;
            bool same = m->params.size() == n.sym->params.size();
            for (std::size_t i = 0; same && i < m->params.size(); ++i) same = jb::spell(n.sym->params[i]) == jb::spell(jo_param_type(m->params[i]));
            if (same) ctor = m;
        }
    }
    const std::vector<ir::TypeP> params = ctor != nullptr ? jo_param_types(*ctor) : (n.sym != nullptr ? n.sym->params : std::vector<ir::TypeP>{});
    const Lines ac = call_args(n.args, params);
    const std::string cname = o.cls(t);
    return {"new " + cname + "(" + text::join(ac, ", ") + ")", jb::kPost, cname, true};
}

Ex JoWriter::x_new_array(Node& e) {
    const ir::NewArray& na = *ir::get<ir::NewArray>(&e);
    const ir::TypeP& t = e.type;
    const CKind k = jb::kind_of(t);
    if (k == CKind::Ptr) {
        const ir::TypeP el = jb::pointee(t);
        const std::string n = na.dims.empty() ? "0" : conv(value(na.dims[0]), na.dims[0]->type, ir::i32_type()).code;
        if (jb::is_record(el)) {
            o.arrays_filled = true;
            return {"filled(new " + o.cls(el) + "[" + n + "], " + o.cls(el) + "::new)", jb::kPost, o.jt(t), false};
        }
        if (!na.init.empty()) {
            Lines items;
            for (Node* x : na.init) items.push_back(conv(value(x), x->type, el).code);
            return {"java.util.Arrays.copyOf(new " + o.jt(el) + "[] {" + text::join(items, ", ") + "}, " + n + ")", jb::kPost, o.jt(t), false};
        }
        return {"new " + o.jt(el) + "[" + n + "]", jb::kPost, o.jt(t), false};
    }
    if (k == CKind::Array) {
        const ir::ArrayType& arr = *jo_array(t);
        if (!na.dims.empty() && !arr.count) {   // new T[n] (a Java array read back)
            const std::string n = conv(value(na.dims[0]), na.dims[0]->type, ir::i32_type()).code;
            ir::TypeP base = t;
            std::size_t levels = 0;
            while (const ir::ArrayType* a = jo_array(base)) {
                base = a->elem;
                ++levels;
            }
            std::string rest;
            for (std::size_t i = 1; i < levels; ++i) rest += "[]";
            return {"new " + o.jt(base) + "[" + n + "]" + rest, jb::kPost, o.jt(t), false};
        }
        if (!na.dims.empty() && na.init.empty()) throw Refused("an array with both a size and a type that fixes it");
        return init_value(t, &e);
    }
    throw Refused("an array value");
}

Ex JoWriter::x_cond(Node& e) {
    const ir::Cond& c = *ir::get<ir::Cond>(&e);
    if (jb::is_record(e.type)) {
        const Ex test = cond(c.cond);
        const Ex a = value(c.then);
        const Ex b = value(c.orelse);
        return {test.at(jb::kCond + 1) + " ? " + a.at(jb::kCond + 1) + " : " + b.at(jb::kCond), jb::kCond, o.jt(e.type), false};
    }
    return jb::FnWriter::x_cond(e);
}

Ex JoWriter::lib_call(Node& e, std::string_view name, std::span<Node* const> args) {
    if (name.starts_with("__builtin_va")) throw Refused("a variadic function");
    return jb::FnWriter::lib_call(e, name, args);
}

// ---- the tier ------------------------------------------------------------------------------------------------------------------------------

// A TierWriter over the program writer: init (what a constructor would do), then write.
class JoTier final : public jb::TierWriter {
public:
    JoTier(std::span<Node* const> units, std::string_view name, const jb::Analysis& analysis, const jb::LibraryCalls* library)
        : objects_(units, std::string(name), analysis, library) {}

    std::string write() override {
        objects_.init();
        return objects_.write();
    }
    const std::vector<std::string>& gaps() const override { return objects_.gaps; }
    const std::vector<std::string>& notes() const override { return objects_.notes; }

private:
    JoObjects objects_;
};

}  // namespace

jb::TierFactory factory(const jb::LibraryCalls* library) {
    return [library](std::span<Node* const> units, std::string_view name, const jb::Analysis& analysis) -> std::unique_ptr<jb::TierWriter> {
        return std::make_unique<JoTier>(units, name, analysis, library);
    };
}

}  // namespace cb::translate::backends::java_objects
