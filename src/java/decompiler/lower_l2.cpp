// java/decompiler/lower_l2.cpp -- Fernflower's final tree lowered into L2 (see the header).
//
// The lowering walks what ClassWriter walks, in its order, and makes for each piece what javac's tree of the text ClassWriter writes for it
// lowers to (java/frontend.cpp): every decision that changes what the text says is made here the same way -- a member hidden, a cast written, a
// parameter left out, a break written after a statement, a label put before one. What only shapes the text (spacing, parentheses, imports) has
// no L2 of its own: javac's Paren nodes are the one difference, and nothing downstream reads them.
#include "java/decompiler/lower_l2.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <string_view>

#include "java/decompiler/class_writer.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/var_type.hpp"
#include "java/decompiler/vars.hpp"
#include "java/frontend.hpp"

namespace cb::java::decompiler::lower_l2 {

namespace {

namespace ir = cb::translate::ir;
using ir::Node;
using ir::SymP;
using ir::TypeP;

// ---- names and types --------------------------------------------------------------------------------------------------------------

// The Java name of a class: "a/b/C" is a.b.C, an own member class its outer class's name and its own ("a.b.Outer.Inner"), as javac names it.
std::string l2_class_name(const std::string& internal) {
    ClassesProcessor* processor = DecompilerContext::current().class_processor;
    ClassNode* node = processor != nullptr ? processor->get_class_node(internal) : nullptr;
    if (node != nullptr && node->type == ClassNode::CLASS_MEMBER && node->parent != nullptr && node->simple_name)
        return l2_class_name(node->parent->class_struct->qualified_name) + "." + *node->simple_name;
    std::string out = internal;
    std::replace(out.begin(), out.end(), '/', '.');
    std::replace(out.begin(), out.end(), '$', '.');
    return out;
}

// A VarType as javac's type text would be ("int", "java.lang.String[]"), then as the javac front end reads that text.
std::string l2_type_text(const VarType& t) {
    std::string s;
    switch (t.type) {
        case cc::TYPE_BYTE: s = "byte"; break;
        case cc::TYPE_CHAR: s = "char"; break;
        case cc::TYPE_DOUBLE: s = "double"; break;
        case cc::TYPE_FLOAT: s = "float"; break;
        case cc::TYPE_INT: s = "int"; break;
        case cc::TYPE_LONG: s = "long"; break;
        case cc::TYPE_SHORT: s = "short"; break;
        case cc::TYPE_BOOLEAN: s = "boolean"; break;
        case cc::TYPE_BYTECHAR: s = "byte"; break;
        case cc::TYPE_SHORTCHAR: s = "short"; break;
        case cc::TYPE_VOID: s = "void"; break;
        case cc::TYPE_NULL: return "<nulltype>";
        case cc::TYPE_OBJECT: s = t.value ? l2_class_name(*t.value) : "java.lang.Object"; break;
        default: return "<any>";
    }
    for (int d = 0; d < t.array_dim; ++d) s += "[]";
    return s;
}

TypeP l2_type(const VarType& t) { return frontend::parse_type(l2_type_text(t)); }

// A type of a generic signature (DECOMPILE_GENERIC_SIGNATURES), as javac's type of the same text: a type variable, a class with its
// arguments (a wildcard, bounded or not), a primitive, each with its array dimensions.
TypeP l2_generic_type(const GenericType& g) {
    TypeP t;
    if (g.type == cc::TYPE_GENVAR) {
        t = ir::typevar_type(g.value.value_or("T"));
    } else if (g.type == cc::TYPE_OBJECT) {
        std::vector<TypeP> args;
        for (std::size_t i = 0; i < g.arguments.size(); ++i) {
            const int wildcard = i < g.wildcards.size() ? g.wildcards[i] : GenericType::WILDCARD_NO;
            if (g.arguments[i] == nullptr || wildcard == GenericType::WILDCARD_UNBOUND) {
                args.push_back(ir::make_type(ir::WildcardType{}));
            } else if (wildcard == GenericType::WILDCARD_EXTENDS || wildcard == GenericType::WILDCARD_SUPER) {
                const ir::BoundMode mode = wildcard == GenericType::WILDCARD_EXTENDS ? ir::BoundMode::Extends : ir::BoundMode::Super;
                args.push_back(ir::make_type(ir::WildcardType{ir::WildcardBound{mode, l2_generic_type(*g.arguments[i])}}));
            } else {
                args.push_back(l2_generic_type(*g.arguments[i]));
            }
        }
        t = ir::class_type(g.value ? l2_class_name(*g.value) : "java.lang.Object", std::move(args));
    } else {
        t = l2_type(VarType(g.type, 0, g.value));
    }
    for (int d = 0; d < g.array_dim; ++d) t = ir::array_of(t);
    return t;
}

bool l2_has_typevar(const TypeP& t) {
    if (!t) return false;
    if (t->kind() == "typevar") return true;
    if (const auto* c = std::get_if<ir::ClassType>(&t->body)) return std::any_of(c->args.begin(), c->args.end(), l2_has_typevar);
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) return l2_has_typevar(a->elem);
    if (const auto* w = std::get_if<ir::WildcardType>(&t->body)) return w->bound && l2_has_typevar(w->bound->type);
    return false;
}

template <class Member>
const GenericSignatureAttribute* l2_signature(const Member& m) {
    if (!DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES)) return nullptr;
    return m.template attribute<GenericSignatureAttribute>("Signature");
}

const TypeP& l2_string_type() {
    static const TypeP t = ir::class_type("java.lang.String");
    return t;
}

// The modifiers ClassWriter writes for `flags` (append_modifiers), in javac's order of them.
std::vector<std::string> l2_modifiers(int flags, int allowed, bool is_interface, int excluded) {
    flags &= allowed;
    if (!is_interface) excluded = 0;
    static const std::pair<int, const char*> order[] = {{cc::ACC_PUBLIC, "public"},       {cc::ACC_PROTECTED, "protected"}, {cc::ACC_PRIVATE, "private"},
                                                        {cc::ACC_ABSTRACT, "abstract"},   {cc::ACC_STATIC, "static"},       {cc::ACC_FINAL, "final"},
                                                        {cc::ACC_TRANSIENT, "transient"}, {cc::ACC_VOLATILE, "volatile"},   {cc::ACC_SYNCHRONIZED, "synchronized"},
                                                        {cc::ACC_NATIVE, "native"},       {cc::ACC_STRICT, "strictfp"}};
    std::vector<std::string> out;
    for (const auto& [modifier, text] : order) {
        if ((flags & modifier) == modifier && (modifier & excluded) == 0) out.emplace_back(text);
    }
    return out;
}

constexpr int kClassAllowed = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_ABSTRACT | cc::ACC_STATIC | cc::ACC_FINAL | cc::ACC_STRICT;
constexpr int kFieldAllowed = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_STATIC | cc::ACC_FINAL | cc::ACC_TRANSIENT | cc::ACC_VOLATILE;
constexpr int kMethodAllowed = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_ABSTRACT | cc::ACC_STATIC | cc::ACC_FINAL | cc::ACC_SYNCHRONIZED |
                               cc::ACC_NATIVE | cc::ACC_STRICT;
constexpr int kClassExcluded = cc::ACC_ABSTRACT | cc::ACC_STATIC;
constexpr int kFieldExcluded = cc::ACC_PUBLIC | cc::ACC_STATIC | cc::ACC_FINAL;
constexpr int kMethodExcluded = cc::ACC_PUBLIC | cc::ACC_ABSTRACT;

// A UTF-16 code unit as the UTF-8 text javac's dump gives a char literal (a lone surrogate as its three bytes).
std::string l2_char_text(std::uint32_t c) {
    std::string s;
    if (c < 0x80) {
        s += static_cast<char>(c);
    } else if (c < 0x800) {
        s += static_cast<char>(0xC0 | (c >> 6));
        s += static_cast<char>(0x80 | (c & 0x3F));
    } else {
        s += static_cast<char>(0xE0 | (c >> 12));
        s += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        s += static_cast<char>(0x80 | (c & 0x3F));
    }
    return s;
}

// What the text cannot carry: an Unsupported node with its reason.
struct L2Declined {
    std::string kind;
    std::string why;
};

// ---- the lowering of one root class -------------------------------------------------------------------------------------------------

class ClassLowering {
public:
    explicit ClassLowering(cb::Arena& arena) : arena_(arena) {}

    Node* unit(ClassNode* root) {
        const std::string& internal = root->class_struct->qualified_name;
        ir::Unit u;
        u.path = internal;
        const auto slash = internal.rfind('/');
        u.package = slash == std::string::npos ? std::string() : l2_class_name(internal.substr(0, slash));
        u.lang = "java";
        if (ImportCollector* imports = DecompilerContext::current().import_collector) u.imports = imports->imports();
        u.types.push_back(class_decl(root, std::nullopt));
        return mk(std::move(u));
    }

private:
    cb::Arena& arena_;
    ClassNode* node_ = nullptr;          // the class being lowered
    std::string qname_;                  // its Java name
    MethodWrapper* method_ = nullptr;    // the method being lowered
    std::string owner_;                  // its locals' owner ("C.m")
    std::pair<int, int> captured_{0, 0}; // in a lambda's body: the slots of the values it captured (the method around's locals)

    template <class K>
    Node* mk(K kind, TypeP type = {}) {
        return ir::make(arena_, std::move(kind), std::move(type));
    }
    template <class K>
    Node* mk_at(K kind, TypeP type, const Exprent* from) {
        Node* n = ir::make(arena_, std::move(kind), std::move(type));
        if (from != nullptr && from->bytecode && !from->bytecode->empty())   // (the offsets of its instructions: from the first to the last one's)
            n->origin = std::make_shared<const ir::Origin>(ir::OriginSpan{static_cast<std::uint32_t>(*from->bytecode->begin()), static_cast<std::uint32_t>(*from->bytecode->rbegin() + 1)});
        return n;
    }
    Node* unsupported(const std::string& kind, const std::string& why) { return mk(ir::Unsupported{.kind = kind, .raw = why}, ir::any_type()); }

    // ---- declarations

    Node* class_decl(ClassNode* node, const std::optional<std::string>& outer) {
        ClassNode* const saved_node = std::exchange(node_, node);
        const std::string saved_qname = std::exchange(qname_, l2_class_name(node->class_struct->qualified_name));
        ClassWrapper* wrapper = node->get_wrapper();
        const StructClass* cl = wrapper->get_class_struct();
        DecompilerContext& ctx = DecompilerContext::current();
        ClassNode* const saved_current = std::exchange(ctx.current_class_node, node);

        int flags = node->type == ClassNode::CLASS_ROOT ? cl->access_flags() : node->access;
        const bool is_enum = DecompilerContext::option(pref::DECOMPILE_ENUM) && (flags & cc::ACC_ENUM) != 0;
        const bool is_interface = (flags & cc::ACC_INTERFACE) != 0;
        const bool is_annotation = (flags & cc::ACC_ANNOTATION) != 0;
        if (is_enum) flags &= ~(cc::ACC_ABSTRACT | cc::ACC_FINAL);

        ir::ClassDecl c;
        c.name = node->simple_name;
        c.qname = qname_;
        c.kind = is_enum ? "enum" : is_annotation ? "annotation" : is_interface ? "interface" : "class";
        c.mods = l2_modifiers(flags, kClassAllowed, is_interface, kClassExcluded);
        if (!is_enum && !is_interface && cl->super_class != nullptr) {
            const VarType supertype = VarType::parse(cl->super_class->get_string(), true);
            if (!(VarType::OBJECT == supertype)) c.extends = l2_type(supertype);
        }
        if (!is_annotation) {
            for (std::size_t i = 0; i < cl->interfaces().size(); ++i) c.implements.push_back(l2_type(VarType::parse(cl->get_interface(i), true)));
        }
        // The class's generic signature: with DECOMPILE_GENERIC_SIGNATURES all of it, as ClassWriter writes it. Without, the bases of a class
        // that has no type parameters of its own still take theirs (Comparable<Item>: its compareTo(Item) is what implements it; the text's raw
        // `Comparable` leaves that to a bridge method the decompiler hides).
        const GenericSignatureAttribute* class_sig = DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES) ? l2_signature(*cl)
                                                                                                                    : cl->attribute<GenericSignatureAttribute>("Signature");
        if (class_sig != nullptr) {
            if (const std::optional<GenericClassDescriptor> g = generics::parse_class_signature(class_sig->signature())) {
                const bool all = DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES);
                bool bound_free = true;   // (no type variable of an outer class in its bases)
                for (const GenericType& base : g->superinterfaces) bound_free = bound_free && !l2_has_typevar(l2_generic_type(base));
                bound_free = bound_free && !l2_has_typevar(l2_generic_type(g->superclass));
                if (all || (g->fparameters.empty() && bound_free)) {
                    if (all) c.typeparams = g->fparameters;
                    if (c.extends) c.extends = l2_generic_type(g->superclass);
                    if (!is_annotation && g->superinterfaces.size() == c.implements.size()) {
                        for (std::size_t i = 0; i < c.implements.size(); ++i) c.implements[i] = l2_generic_type(g->superinterfaces[i]);
                    }
                }
            }
        }
        c.outer = outer;
        Node* decl = mk(std::move(c));
        auto& members = ir::get<ir::ClassDecl>(decl)->members;

        for (const auto& fd_ptr : cl->fields()) {
            const StructField* fd = fd_ptr.get();
            const bool hide = (fd->is_synthetic() && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) ||
                              wrapper->get_hidden_members().contains(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
            if (!hide) members.push_back(field(wrapper, cl, fd, is_interface));
        }
        bool any_ctor = false;
        for (const auto& mt_ptr : cl->methods()) {
            const StructMethod* mt = mt_ptr.get();
            const bool hide = (mt->is_synthetic() && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) ||
                              (mt->has_modifier(cc::ACC_BRIDGE) && DecompilerContext::option(pref::REMOVE_BRIDGE)) ||
                              wrapper->get_hidden_members().contains(InterpreterUtil::make_unique_key(mt->name(), mt->descriptor()));
            if (hide) continue;
            if (Node* m = method(node, mt, is_interface, is_enum)) {
                const auto* d = ir::get<ir::MethodDecl>(m);
                if (d != nullptr && d->kind == std::optional<std::string>("ctor")) any_ctor = true;
                if (d != nullptr && has_mod(d->mods, "IMPLICIT")) members.insert(members.begin(), m);   // (a default constructor the text leaves out)
                else members.push_back(m);
            }
        }
        if (!any_ctor && !is_interface) members.insert(members.begin(), implicit_ctor(flags, is_enum, cl));   // javac's default constructor, first
        for (ClassNode* inner : node->nested) {
            if (inner->type != ClassNode::CLASS_MEMBER) continue;
            const StructClass* inner_cl = inner->class_struct;
            const bool is_synthetic = (inner->access & cc::ACC_SYNTHETIC) != 0 || inner_cl->is_synthetic() || inner->nameless_constructor_stub;
            const bool hide = (is_synthetic && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) || wrapper->get_hidden_members().contains(inner_cl->qualified_name);
            if (!hide) members.push_back(class_decl(inner, qname_));
        }

        ctx.current_class_node = saved_current;
        node_ = saved_node;
        qname_ = saved_qname;
        return decl;
    }

    // The constructor javac gives a class that declares none: the class's access, a call of the base class's no-argument constructor.
    Node* implicit_ctor(int class_flags, bool is_enum, const StructClass* cl) {
        ir::MethodDecl d;
        d.sym = ir::make_sym(ir::SymKind::Ctor, "<init>", qname_, false);
        d.sym->ret = ir::void_type();
        d.name = "<init>";
        d.kind = "ctor";
        d.ret = ir::void_type();
        if (is_enum) d.mods.push_back("private");
        else
            for (const std::string& m : l2_modifiers(class_flags, cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE, false, 0)) d.mods.push_back(m);
        d.mods.push_back("IMPLICIT");
        d.body = mk(ir::Block{.stmts = {implicit_super(is_enum, cl)}});
        return mk(std::move(d));
    }

    Node* implicit_super(bool is_enum, const StructClass* cl) {
        std::string base = "java.lang.Object";
        if (is_enum) base = "java.lang.Enum";
        else if (cl->super_class != nullptr) base = l2_class_name(cl->super_class->get_string());
        SymP s = ir::make_sym(ir::SymKind::Ctor, "<init>", base, false);
        if (is_enum) s->params = {l2_string_type(), ir::i32_type()};
        s->ret = ir::void_type();
        return mk(ir::CtorCall{.kind = "super:implicit", .sym = s});
    }

    Node* field(ClassWrapper* wrapper, const StructClass* cl, const StructField* fd, bool is_interface) {
        const bool is_enum = fd->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
        const VarType field_type = VarType::parse(fd->descriptor(), false);
        TypeP type = l2_type(field_type);
        if (const GenericSignatureAttribute* sig = l2_signature(*fd)) {
            if (const std::optional<GenericFieldDescriptor> g = generics::parse_field_signature(sig->signature())) type = l2_generic_type(g->type);
        }
        const bool is_static = fd->has_modifier(cc::ACC_STATIC);
        Exprent* initializer = is_static ? wrapper->get_static_field_initializers().get_with_key(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()))
                                         : wrapper->get_dynamic_field_initializers().get_with_key(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
        try {
            if (is_enum) {
                ir::EnumConst e;
                e.sym = ir::make_sym(ir::SymKind::EnumConst, fd->name(), qname_, true, type);
                if (initializer != nullptr && initializer->type == Exprent::EXPRENT_NEW) {
                    auto* made = static_cast<NewExprent*>(initializer);
                    if (made->is_anonymous()) throw L2Declined{"enum-body", "an enum constant with a body"};
                    if (InvocationExprent* ctor = made->get_constructor()) {
                        const auto& params = ctor->get_lst_parameters();
                        for (std::size_t i = 2; i < params.size(); ++i) e.args.push_back(casted(params[i], ctor->get_descriptor().params.at(i), true));
                        e.ctor = ctor_sym(*ctor);
                    }
                }
                return mk(std::move(e));
            }
            ir::FieldDecl f;
            f.sym = ir::make_sym(ir::SymKind::Field, fd->name(), qname_, is_static, type);
            f.type = type;
            f.mods = l2_modifiers(fd->access_flags(), kFieldAllowed, is_interface, kFieldExcluded);
            if (initializer != nullptr) {
                f.init = expr(initializer);
            } else if (fd->has_modifier(cc::ACC_FINAL) && is_static) {
                if (const auto* cv = fd->attribute<ConstantValueAttribute>(attr::CONSTANT_VALUE)) {
                    const PrimitiveConstant* constant = cl->pool().primitive(static_cast<std::size_t>(cv->index()));
                    f.init = expr(make<ConstExprent>(field_type, constant->value, std::nullopt));
                }
            }
            return mk(std::move(f));
        } catch (const L2Declined& d) {
            return unsupported(d.kind, d.why);
        }
    }

    Node* method(ClassNode* node, const StructMethod* mt, bool is_interface, bool is_enum) {
        ClassWrapper* wrapper = node->get_wrapper();
        MethodWrapper* mw = wrapper->get_method_wrapper(mt->name(), mt->descriptor());
        DecompilerContext& ctx = DecompilerContext::current();
        MethodWrapper* const saved_wrapper = std::exchange(ctx.current_method_wrapper, mw);
        MethodWrapper* const saved_method = std::exchange(method_, mw);
        const std::string saved_owner = std::exchange(owner_, qname_ + "." + (cc::CLINIT_NAME == mt->name() ? std::string() : mt->name()));   // (javac's static block has no name)
        struct Restore {
            std::function<void()> f;
            ~Restore() { f(); }
        } restore{[&] {
            ctx.current_method_wrapper = saved_wrapper;
            method_ = saved_method;
            owner_ = saved_owner;
        }};

        const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
        int flags = mt->access_flags();
        if ((flags & cc::ACC_NATIVE) != 0) flags &= ~cc::ACC_STRICT;
        const bool clinit = cc::CLINIT_NAME == mt->name();
        const bool init = cc::INIT_NAME == mt->name();
        if (clinit) flags &= cc::ACC_STATIC;
        if (init && node->type == ClassNode::CLASS_ANONYMOUS) return unsupported("anonymous-class", "the constructor of an anonymous class");

        std::optional<GenericMethodDescriptor> generic;
        if (const GenericSignatureAttribute* sig = l2_signature(*mt)) {
            generic = generics::parse_method_signature(sig->signature());
            if (generic) {
                int actual = static_cast<int>(md.params.size());
                if (mw->signature_fields) {
                    actual = 0;
                    for (const std::optional<VarVersionPair>& f : *mw->signature_fields) actual += f ? 0 : 1;
                } else if (is_enum && init) {
                    actual -= 2;
                }
                if (actual != static_cast<int>(generic->params.size())) generic.reset();
            }
        }

        ir::MethodDecl d;
        d.name = mt->name();
        d.kind = clinit ? "static_init" : init ? "ctor" : "method";
        d.ret = init || clinit ? ir::void_type() : generic ? l2_generic_type(generic->ret) : l2_type(md.ret);
        if (generic) d.typeparams = generic->fparameters;
        if (!clinit) {
            d.mods = l2_modifiers(flags, kMethodAllowed, is_interface, kMethodExcluded);
            if (is_interface && mt->contains_code()) d.mods.insert(d.mods.begin() + static_cast<std::ptrdiff_t>(std::count_if(d.mods.begin(), d.mods.end(), [](const std::string& m) {
                                                                      return m == "public" || m == "protected" || m == "private" || m == "abstract";
                                                                  })),
                                                                  "default");
            d.sym = ir::make_sym(init ? ir::SymKind::Ctor : ir::SymKind::Method, mt->name(), qname_, (flags & cc::ACC_STATIC) != 0);
            d.sym->ret = d.ret;
        }

        // parameters: those the text writes (a local or anonymous class's captured values and an enum's name and ordinal are not)
        int param_count = 0;
        if (!clinit) {
            const bool this_var = !mt->has_modifier(cc::ACC_STATIC);
            const SignatureFields& sign_fields = mw->signature_fields;
            int index = is_enum && init ? 3 : this_var ? 1 : 0;
            const int start = is_enum && init && !generic ? 2 : 0;
            const int count = generic ? static_cast<int>(generic->params.size()) : static_cast<int>(md.params.size());
            for (int i = start; i < count; ++i) {
                if (generic || !sign_fields || !sign_fields->at(static_cast<std::size_t>(i))) {
                    const std::optional<std::string> pname = mw->varproc->get_var_name(VarVersionPair(index, 0));
                    const TypeP ptype = generic ? l2_generic_type(generic->params[static_cast<std::size_t>(i)]) : l2_type(md.params[static_cast<std::size_t>(i)]);
                    ir::Param p;
                    p.sym = ir::make_sym(ir::SymKind::Param, pname.value_or("param" + std::to_string(index)), owner_, false, ptype);
                    p.type = ptype;
                    if (mw->varproc->get_var_final(VarVersionPair(index, 0)) == VarTypeProcessor::VAR_EXPLICIT_FINAL) p.mods.push_back("final");
                    d.params.push_back(mk(std::move(p)));
                    d.sym->params.push_back(ptype);
                    ++param_count;
                }
                index += md.params[static_cast<std::size_t>(i)].stack_size;
            }
            if (d.sym && mt->has_modifier(cc::ACC_VARARGS)) d.sym->varargs = true;
            if (const auto* exc = mt->attribute<ExceptionsAttribute>("Exceptions")) {
                for (std::size_t i = 0; i < exc->throws_exceptions().size(); ++i)
                    d.throws.push_back(generic && i < generic->exceptions.size() ? l2_generic_type(generic->exceptions[i])
                                                                                 : l2_type(VarType::parse(exc->exception_class_name(i, node->class_struct->pool()), true)));
            }
        }

        if ((flags & (cc::ACC_ABSTRACT | cc::ACC_NATIVE)) != 0) return mk(std::move(d));   // no body

        std::vector<Node*> stmts;
        if (mw->root != nullptr && !mw->decompiled_with_errors) {
            try {
                stmts = root_stmts(mw->root);
            } catch (const L2Declined& decl) {
                stmts = {mk(ir::ExprStmt{.expr = unsupported(decl.kind, decl.why)})};
            }
        } else {
            stmts = {mk(ir::ExprStmt{.expr = unsupported("method", "a method Fernflower could not decompile")})};
        }
        const bool throws_exceptions = !d.throws.empty();
        if (stmts.empty() && (clinit || hide_constructor(wrapper, init, throws_exceptions, param_count))) {
            return clinit ? nullptr : implicit_ctor_of(node, wrapper, is_enum);
        }
        if (init && (stmts.empty() || !ir::is<ir::CtorCall>(stmts.front()))) stmts.insert(stmts.begin(), implicit_super(is_enum, wrapper->get_class_struct()));
        d.body = mk(ir::Block{.stmts = std::move(stmts)});
        return mk(std::move(d));
    }

    Node* implicit_ctor_of(ClassNode* node, ClassWrapper* wrapper, bool is_enum) {
        const int flags = node->type == ClassNode::CLASS_ROOT ? wrapper->get_class_struct()->access_flags() : node->access;
        return implicit_ctor(flags, is_enum, wrapper->get_class_struct());
    }

    static bool has_mod(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

    static bool hide_constructor(ClassWrapper* wrapper, bool init, bool throws_exceptions, int param_count) {
        if (!init || throws_exceptions || param_count > 0 || !DecompilerContext::option(pref::HIDE_DEFAULT_CONSTRUCTOR)) return false;
        int count = 0;
        for (const auto& mt : wrapper->get_class_struct()->methods()) {
            if (cc::INIT_NAME == mt->name() && ++count > 1) return false;
        }
        return true;
    }

    // ---- statements

    std::vector<Node*> root_stmts(RootStatement* root) {
        std::vector<Node*> out;
        exprent_list(root->get_var_definitions(), out);
        stmt(root->get_first(), out);
        return out;
    }

    void exprent_list(const std::vector<Exprent*>& list, std::vector<Node*>& out) {
        for (Exprent* e : list) {
            if (Node* s = stmt_of(e)) out.push_back(s);
        }
    }
    void exprent_list(const std::optional<std::vector<Exprent*>>& list, std::vector<Node*>& out) {
        if (list) exprent_list(*list, out);
    }

    // ExprProcessor::jmp_wrapper: the statement, then the break or continue its one edge out is written as.
    void jmp(Statement* stat, std::vector<Node*>& out) {
        stmt(stat, out);
        const std::vector<StatEdge*> succs = stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
        if (succs.size() == 1) {
            StatEdge* edge = succs[0];
            if (edge->get_type() != StatEdge::TYPE_REGULAR && edge->explicit_ && edge->get_destination()->type != Statement::TYPE_DUMMYEXIT) {
                if (Node* j = jump(edge)) out.push_back(j);
            }
        }
    }

    Node* jump(StatEdge* edge) {
        std::optional<std::string> label;
        if (edge->labeled) label = "label" + std::to_string(edge->closure->id);
        if (edge->get_type() == StatEdge::TYPE_BREAK) return mk(ir::Break{.label = label});
        if (edge->get_type() == StatEdge::TYPE_CONTINUE) return mk(ir::Continue{.label = label});
        return nullptr;
    }

    Node* block_of(Statement* stat) {
        std::vector<Node*> body;
        jmp(stat, body);
        return mk(ir::Block{.stmts = std::move(body)});
    }

    Node* labeled(Statement* stat, Node* s) {
        if (!stat->is_labeled()) return s;
        return mk(ir::Labeled{.label = "label" + std::to_string(stat->id), .body = s});
    }

    void stmt(Statement* stat, std::vector<Node*>& out) {
        exprent_list(stat->get_var_definitions(), out);
        switch (stat->type) {
            case Statement::TYPE_BASICBLOCK:
                exprent_list(stat->get_exprents(), out);
                return;
            case Statement::TYPE_SEQUENCE: {
                std::vector<Node*> body;
                for (Statement* st : stat->get_stats()) jmp(st, body);
                if (stat->is_labeled()) out.push_back(labeled(stat, mk(ir::Block{.stmts = std::move(body)})));
                else out.insert(out.end(), body.begin(), body.end());
                return;
            }
            case Statement::TYPE_IF: {
                auto* s = static_cast<IfStatement*>(stat);
                stmt(s->get_first(), out);
                out.push_back(labeled(stat, if_node(s)));
                return;
            }
            case Statement::TYPE_DO: {
                out.push_back(labeled(stat, do_node(static_cast<DoStatement*>(stat))));
                return;
            }
            case Statement::TYPE_SWITCH: {
                auto* s = static_cast<SwitchStatement*>(stat);
                stmt(s->get_first(), out);
                out.push_back(labeled(stat, switch_node(s)));
                return;
            }
            case Statement::TYPE_SYNCRONIZED: {
                auto* s = static_cast<SynchronizedStatement*>(stat);
                stmt(s->get_first(), out);
                auto* monitor = static_cast<MonitorExprent*>(s->get_headexprent());
                out.push_back(labeled(stat, mk(ir::Sync{.lock = expr(monitor->get_value()), .body = block_of(s->get_body())})));
                return;
            }
            case Statement::TYPE_TRYCATCH:
                out.push_back(labeled(stat, try_node(static_cast<CatchStatement*>(stat))));
                return;
            case Statement::TYPE_CATCHALL:
                out.push_back(labeled(stat, catch_all_node(static_cast<CatchAllStatement*>(stat))));
                return;
            case Statement::TYPE_ROOT: {
                exprent_list(stat->get_var_definitions(), out);
                stmt(stat->get_first(), out);
                return;
            }
            default:
                throw L2Declined{"statement", "a statement of kind " + std::to_string(stat->type)};
        }
    }

    Node* if_node(IfStatement* s) {
        ir::If i;
        i.cond = expr(static_cast<IfExprent*>(s->get_headexprent())->get_condition());
        std::vector<Node*> then;
        if (s->get_ifstat() == nullptr) {
            if (s->get_if_edge()->explicit_) {
                if (Node* j = jump(s->get_if_edge())) then.push_back(j);
            }
        } else {
            jmp(s->get_ifstat(), then);
        }
        i.then = mk(ir::Block{.stmts = std::move(then)});
        i.orelse = static_cast<Node*>(nullptr);
        if (Statement* el = s->get_elsestat()) {
            const std::vector<StatEdge*> succ = el->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
            if (el->type == Statement::TYPE_IF && el->get_var_definitions().empty() && el->get_first()->get_exprents().value().empty() && !el->is_labeled() &&
                (succ.empty() || !succ.at(0)->explicit_)) {
                std::vector<Node*> chain;   // else if
                jmp(el, chain);
                i.orelse = chain.size() == 1 ? ir::Suite(chain[0]) : ir::Suite(mk(ir::Block{.stmts = std::move(chain)}));
            } else {
                std::vector<Node*> body;
                jmp(el, body);
                if (!body.empty()) i.orelse = mk(ir::Block{.stmts = std::move(body)});
            }
        }
        return mk(std::move(i));
    }

    Node* do_node(DoStatement* s) {
        Node* body = block_of(s->get_first());
        switch (s->get_looptype()) {
            case DoStatement::LOOP_DO:
                return mk(ir::While{.cond = mk(ir::Lit{.value = true}, ir::bool_type()), .body = body});
            case DoStatement::LOOP_DOWHILE:
                return mk(ir::DoWhile{.body = body, .cond = expr(s->get_condition_exprent())});
            case DoStatement::LOOP_WHILE:
                return mk(ir::While{.cond = expr(s->get_condition_exprent()), .body = body});
            case DoStatement::LOOP_FOR: {
                ir::For f;
                if (Exprent* init = s->get_init_exprent()) {
                    if (Node* st = stmt_of(init)) f.init.push_back(st);
                }
                f.cond = expr(s->get_condition_exprent());
                f.update.push_back(expr(s->get_inc_exprent()));
                f.body = body;
                return mk(std::move(f));
            }
            default:
                throw L2Declined{"loop", "a loop of kind " + std::to_string(s->get_looptype())};
        }
    }

    Node* switch_node(SwitchStatement* s) {
        ir::Switch sw;
        Exprent* head = s->get_headexprent();
        Exprent* value = static_cast<SwitchExprent*>(head)->get_value();
        sw.expr = expr(value);
        const VarType switch_type = head->get_expr_type();
        for (std::size_t i = 0; i < s->get_case_statements().size(); ++i) {
            std::vector<StatEdge*>& edges = s->get_case_edges()[i];
            std::vector<ConstExprent*>& values = s->get_case_values()[i];
            for (std::size_t j = 0; j < edges.size(); ++j) {
                ir::Case c;
                if (edges[j] == s->get_default_edge()) {
                    c.is_default = true;
                } else {
                    auto* label = static_cast<ConstExprent*>(values[j]->copy());
                    label->set_const_type(switch_type);
                    c.labels.push_back(expr(label));
                }
                if (j + 1 == edges.size()) jmp(s->get_case_statements()[i], c.body);
                sw.cases.push_back(mk(std::move(c)));
            }
        }
        return mk(std::move(sw));
    }

    Node* catch_node(VarExprent* var, std::vector<TypeP> types, Statement* handler) {
        ir::Catch c;
        const TypeP t = l2_type(var->get_var_type());
        c.sym = ir::make_sym(ir::SymKind::Local, var_name(var), owner_, false, t);
        c.types = std::move(types);
        if (c.types.empty()) c.types.push_back(t);
        c.body = block_of(handler);
        return mk(std::move(c));
    }

    Node* try_node(CatchStatement* s) {
        ir::Try t;
        t.body = block_of(s->get_first());
        const auto& stats = s->get_stats();
        for (std::size_t i = 1; i < stats.size(); ++i) {
            const std::vector<std::string>& names = s->get_exception_types().at(i - 1);
            std::vector<TypeP> types;
            for (std::size_t k = 1; k < names.size(); ++k) types.push_back(l2_type(VarType(cc::TYPE_OBJECT, 0, names[k])));   // (A | B | the variable's)
            VarExprent* var = s->get_vars().at(i - 1);
            if (!types.empty()) types.push_back(l2_type(var->get_var_type()));
            t.catches.push_back(catch_node(var, std::move(types), stats[i]));
        }
        return mk(std::move(t));
    }

    Node* catch_all_node(CatchAllStatement* s) {
        Statement* first = s->get_first();
        const std::vector<StatEdge*> succs = first->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
        Node* handler = block_of(s->get_handler());
        if (VarExprent* monitor = s->get_monitor()) {
            ir::If guard{.cond = expr(monitor), .then = handler, .orelse = static_cast<Node*>(nullptr)};
            handler = mk(ir::Block{.stmts = {mk(std::move(guard))}});
        }
        Node* t = nullptr;
        if (first->type == Statement::TYPE_TRYCATCH && first->get_var_definitions().empty() && s->is_finally() && !s->is_labeled() && !first->is_labeled() &&
            (succs.empty() || !succs[0]->explicit_)) {
            std::vector<Node*> inner;   // try ... catch ... finally: one Try
            jmp(first, inner);
            if (inner.size() == 1 && ir::is<ir::Try>(inner[0])) t = inner[0];
        }
        if (t == nullptr) t = mk(ir::Try{.body = block_of(first)});
        auto& tr = *ir::get<ir::Try>(t);
        if (s->is_finally()) tr.finally_body = handler;
        else tr.catches.push_back(catch_node(s->get_vars().at(0), {}, s->get_handler()));
        return t;
    }

    // One expression of a statement list as the statement its text is (null: it writes nothing).
    Node* stmt_of(Exprent* e) {
        try {
            switch (e->type) {
                case Exprent::EXPRENT_VAR: {
                    auto* v = static_cast<VarExprent*>(e);
                    if (v->is_class_def()) throw L2Declined{"local-class", "a class declared in a method"};
                    if (v->is_definition()) return local(v, nullptr);
                    break;
                }
                case Exprent::EXPRENT_ASSIGNMENT: {
                    auto* a = static_cast<AssignmentExprent*>(e);
                    if (hidden_field_assignment(a)) return nullptr;
                    if (a->get_left()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(a->get_left())->is_definition() &&
                        a->get_cond_type() == AssignmentExprent::CONDITION_NONE) {
                        auto* v = static_cast<VarExprent*>(a->get_left());
                        return local(v, assigned_value(a));
                    }
                    break;
                }
                case Exprent::EXPRENT_EXIT: {
                    auto* x = static_cast<ExitExprent*>(e);
                    if (x->get_exit_type() == ExitExprent::EXIT_RETURN) {
                        ir::Return r;
                        if (x->get_ret_type().type != cc::TYPE_VOID) r.expr = casted(x->get_value(), x->get_ret_type(), false);
                        return mk_at(std::move(r), {}, e);
                    }
                    return mk_at(ir::Throw{.expr = expr(x->get_value())}, {}, e);
                }
                case Exprent::EXPRENT_INVOCATION: {
                    auto* inv = static_cast<InvocationExprent*>(e);
                    if (inv->get_functype() == InvocationExprent::TYP_INIT) return ctor_call(inv);
                    break;
                }
                case Exprent::EXPRENT_MONITOR: {
                    auto* m = static_cast<MonitorExprent*>(e);
                    if (m->get_mon_type() == MonitorExprent::MONITOR_ENTER)
                        return mk_at(ir::Sync{.lock = expr(m->get_value()), .body = mk(ir::Block{})}, {}, e);   // an empty synchronized block
                    return nullptr;
                }
                case Exprent::EXPRENT_ASSERT:
                    throw L2Declined{"assert", "an assert"};
                default:
                    break;
            }
            return mk_at(ir::ExprStmt{.expr = expr(e)}, {}, e);
        } catch (const L2Declined& d) {
            return mk(ir::ExprStmt{.expr = unsupported(d.kind, d.why)});
        }
    }

    Node* local(VarExprent* v, Node* init) {
        ir::Local l;
        const TypeP t = l2_type(v->get_var_type());
        l.sym = ir::make_sym(ir::SymKind::Local, var_name(v), owner_, false, t);
        l.type = t;
        l.init = init;
        if (v->get_processor() != nullptr && v->get_processor()->get_var_final(VarVersionPair(v->get_index(), v->get_version())) == VarTypeProcessor::VAR_EXPLICIT_FINAL)
            l.mods.push_back("final");
        return mk_at(std::move(l), {}, v);
    }

    // `this(...)` or `super(...)`: the parameters the text writes.
    Node* ctor_call(InvocationExprent* inv) {
        Exprent* inst = inv->get_instance();
        const std::string* this_class = inst != nullptr && inst->type == Exprent::EXPRENT_VAR ? this_class_of(static_cast<VarExprent*>(inst)) : nullptr;
        if (this_class == nullptr) throw L2Declined{"ctor-call", "a constructor called on what is not this"};
        ir::CtorCall c;
        c.kind = inv->get_classname() != *this_class ? "super" : "this";
        c.sym = ctor_sym(*inv);
        c.args = call_args(*inv, inv->get_classname(), true);
        return mk_at(std::move(c), {}, inv);
    }

    // ---- expressions

    std::string var_name(VarExprent* v) {
        std::optional<std::string> name;
        if (v->get_processor() != nullptr) name = v->get_processor()->get_var_name(VarVersionPair(v->get_index(), v->get_version()));
        return name ? *name : "var" + std::to_string(v->get_index()) + (v->get_version() == 0 ? "" : "_" + std::to_string(v->get_version()));
    }

    // The class a variable is `this` of, or null.
    const std::string* this_class_of(VarExprent* v) {
        VarProcessor* proc = v->get_processor();
        if (proc == nullptr && method_ != nullptr) proc = method_->varproc;
        return proc != nullptr ? proc->get_this_vars().get(VarVersionPair(v)) : nullptr;
    }

    // The class that declares a member a reference names through `classname` (javac's symbol's owner), among the classes this run read: that
    // class, else its base classes, else its interfaces (as Java looks a member up); none when no read class has it.
    static const StructClass* declaring_class(const std::string& classname, const std::function<bool(const StructClass&)>& has, int depth = 0) {
        const StructContext* sc = DecompilerContext::current().struct_context;
        if (sc == nullptr || depth > 64) return nullptr;
        const StructClass* cl = sc->get_class(classname);
        if (cl == nullptr) return nullptr;
        if (has(*cl)) return cl;
        if (cl->super_class != nullptr) {
            if (const StructClass* found = declaring_class(cl->super_class->get_string(), has, depth + 1)) return found;
        }
        for (std::size_t i = 0; i < cl->interfaces().size(); ++i) {
            if (const StructClass* found = declaring_class(cl->get_interface(i), has, depth + 1)) return found;
        }
        return nullptr;
    }
    // The name of that class; when no read class has the member, the first base class this run did not read (a library's, which has it:
    // `size()` of a class extending ArrayList is ArrayList's), else `classname`.
    static std::string declaring(const std::string& classname, const std::function<bool(const StructClass&)>& has) {
        if (const StructClass* cl = declaring_class(classname, has)) return cl->qualified_name;
        const StructContext* sc = DecompilerContext::current().struct_context;
        std::string at = classname;
        for (int depth = 0; sc != nullptr && depth < 64; ++depth) {
            const StructClass* cl = sc->get_class(at);
            if (cl == nullptr) return at;   // (not read: the library's class)
            if (cl->super_class == nullptr) break;
            at = cl->super_class->get_string();
        }
        return classname;
    }

    SymP method_sym(const InvocationExprent& inv) {
        const auto has = [&](const StructClass& cl) { return cl.get_method(inv.get_name(), inv.get_string_descriptor()) != nullptr; };
        const StructClass* decl = declaring_class(inv.get_classname(), has);
        const bool on_array = inv.get_classname().starts_with("[");
        SymP s = ir::make_sym(ir::SymKind::Method, inv.get_name(), on_array ? std::string("Array") : l2_class_name(declaring(inv.get_classname(), has)), inv.is_static());
        for (const VarType& p : inv.get_descriptor().params) s->params.push_back(l2_type(p));
        s->ret = l2_type(inv.get_descriptor().ret);
        if (on_array && inv.get_name() == "clone") s->ret = l2_type(VarType::parse(inv.get_classname(), false));   // (an array's clone() is of its type)
        if (decl != nullptr) {
            if (const StructMethod* m = decl->get_method(inv.get_name(), inv.get_string_descriptor()); m != nullptr && m->has_modifier(cc::ACC_VARARGS)) s->varargs = true;
        }
        return s;
    }

    SymP ctor_sym(const InvocationExprent& inv) {
        SymP s = ir::make_sym(ir::SymKind::Ctor, "<init>", l2_class_name(inv.get_classname()), false);
        const SignatureFields sig = signature_fields(inv.get_classname(), inv.get_string_descriptor(), inv.get_lst_parameters().size());
        ClassNode* node = DecompilerContext::current().class_processor->get_class_node(inv.get_classname());
        const bool is_enum = node != nullptr && node->class_struct->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
        const auto& params = inv.get_descriptor().params;
        for (std::size_t i = is_enum ? 2 : 0; i < params.size(); ++i) {
            if (!sig || !sig->at(i)) s->params.push_back(l2_type(params[i]));
        }
        s->ret = ir::void_type();
        return s;
    }

    // Which parameters of an own class's constructor the text leaves out (a captured value, the outer instance).
    SignatureFields signature_fields(const std::string& classname, const std::string& descriptor, std::size_t count) {
        ClassNode* node = DecompilerContext::current().class_processor->get_class_node(classname);
        if (node == nullptr) return std::nullopt;
        if (node->get_wrapper() != nullptr) {
            if (MethodWrapper* mw = node->get_wrapper()->get_method_wrapper(std::string(cc::INIT_NAME), descriptor)) return mw->signature_fields;
            return std::nullopt;
        }
        if (node->type == ClassNode::CLASS_MEMBER && (node->access & cc::ACC_STATIC) == 0 && count > 0) {
            std::vector<std::optional<VarVersionPair>> sig(count);
            sig[0] = VarVersionPair(-1, 0);
            return sig;
        }
        return std::nullopt;
    }

    std::vector<Node*> call_args(const InvocationExprent& inv, const std::string& classname, bool init) {
        std::vector<Node*> out;
        SignatureFields sig;
        bool is_enum = false;
        if (init) {
            sig = signature_fields(classname, inv.get_string_descriptor(), inv.get_lst_parameters().size());
            ClassNode* node = DecompilerContext::current().class_processor->get_class_node(classname);
            is_enum = node != nullptr && node->class_struct->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
        }
        const auto& params = inv.get_lst_parameters();
        for (std::size_t i = is_enum ? 2 : 0; i < params.size(); ++i) {
            if (!sig || !sig->at(i)) out.push_back(casted(params[i], inv.get_descriptor().params.at(i), true));
        }
        return out;
    }

    // ExprProcessor::is_int_constant: a constant of int's family.
    static bool int_constant(Exprent* e) {
        if (e->type != Exprent::EXPRENT_CONST) return false;
        const int t = static_cast<ConstExprent*>(e)->get_const_type().type;
        return t == cc::TYPE_BYTE || t == cc::TYPE_BYTECHAR || t == cc::TYPE_SHORT || t == cc::TYPE_SHORTCHAR || t == cc::TYPE_INT;
    }

    // ExprProcessor::get_casted_exprent: the expression, with the cast the text writes before it.
    Node* casted(Exprent* e, const VarType& left_type, bool cast_null) {
        const VarType right_type = e->get_expr_type();
        Node* x = expr(e);
        const bool cast = (!left_type.is_superset(right_type) && (right_type == VarType::OBJECT || left_type.type != cc::TYPE_OBJECT)) ||
                          (cast_null && right_type.type == cc::TYPE_NULL && ExprProcessor::UNDEFINED_TYPE_STRING != ExprProcessor::get_type_name(left_type)) ||
                          (int_constant(e) && VarType::INT.is_strict_superset(left_type));
        if (!cast) return x;
        return mk_at(ir::Cast{.x = x}, l2_type(left_type), e);
    }

    Node* assigned_value(AssignmentExprent* a) {
        const VarType left_type = a->get_left()->get_expr_type();
        const VarType right_type = a->get_right()->get_expr_type();
        Node* r = expr(a->get_right());
        if (a->get_cond_type() == AssignmentExprent::CONDITION_NONE && !left_type.is_superset(right_type) &&
            (right_type == VarType::OBJECT || left_type.type != cc::TYPE_OBJECT))
            r = mk_at(ir::Cast{.x = r}, l2_type(left_type), a->get_right());
        return r;
    }

    bool hidden_field_assignment(AssignmentExprent* a) {
        if (a->get_left()->type != Exprent::EXPRENT_FIELD || node_ == nullptr) return false;
        const auto* field = static_cast<const FieldExprent*>(a->get_left());
        const StructField* fd = node_->class_struct->get_field(field->get_name(), field->get_descriptor().descriptor_string);
        return fd != nullptr && node_->get_wrapper() != nullptr &&
               node_->get_wrapper()->get_hidden_members().contains(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
    }

    Node* static_field(const std::string& owner, const std::string& name, const TypeP& type, const Exprent* at) {
        const TypeP owner_type = ir::class_type(owner);
        return mk_at(ir::Select{.obj = mk(ir::TypeName{}, owner_type), .sym = ir::make_sym(ir::SymKind::Field, name, owner, true, type)}, type, at);
    }

    Node* this_node(const std::string& internal) {
        const std::string name = l2_class_name(internal);
        if (name == qname_) return mk(ir::This{}, ir::class_type(name));
        return mk(ir::This{.qualifier = ir::class_type(name)}, ir::class_type(name));   // Outer.this
    }

    Node* expr(Exprent* e) {
        if (e == nullptr) throw L2Declined{"expression", "an expression that is not there"};
        const TypeP t = l2_type(e->get_expr_type());
        switch (e->type) {
            case Exprent::EXPRENT_CONST: return constant(static_cast<ConstExprent*>(e));
            case Exprent::EXPRENT_VAR: {
                auto* v = static_cast<VarExprent*>(e);
                if (v->is_class_def()) throw L2Declined{"local-class", "a class declared in a method"};
                if (const std::string* this_class = this_class_of(v)) return this_node(*this_class);
                const std::string name = var_name(v);
                if (name.ends_with(".this")) return this_node(name.substr(0, name.size() - 5));   // (an outer instance the nested-class pass named)
                const bool captured = v->get_index() >= captured_.first && v->get_index() < captured_.second && v->get_version() == 0;
                const bool param = method_ != nullptr && !captured && is_param(v);
                // the variable's declared type (a use's own type can be the stack's: an int where the variable is a boolean)
                TypeP declared = t;
                if (v->get_processor() != nullptr) {
                    if (const std::optional<VarType> vt = v->get_processor()->get_var_type(VarVersionPair(v)); vt && vt->type != cc::TYPE_UNKNOWN) declared = l2_type(*vt);
                }
                return mk_at(ir::Name{.sym = ir::make_sym(param ? ir::SymKind::Param : ir::SymKind::Local, name, owner_, false, declared)}, declared, e);
            }
            case Exprent::EXPRENT_FIELD: {
                auto* f = static_cast<FieldExprent*>(e);
                const auto has = [&](const StructClass& cl) { return cl.has_field(f->get_name(), f->get_descriptor().descriptor_string); };
                const StructClass* decl = declaring_class(f->get_classname(), has);
                const std::string owner = l2_class_name(declaring(f->get_classname(), has));
                if (f->is_static()) {
                    const StructField* fd = decl != nullptr ? decl->get_field(f->get_name(), f->get_descriptor().descriptor_string) : nullptr;
                    Node* sel = static_field(owner, f->get_name(), t, e);
                    if (fd != nullptr && fd->has_modifier(cc::ACC_ENUM)) ir::get<ir::Select>(sel)->sym->kind = ir::SymKind::EnumConst;   // Color.RED
                    return sel;
                }
                Exprent* inst = f->get_instance();
                Node* obj = nullptr;
                if (inst != nullptr && inst->type == Exprent::EXPRENT_VAR) {
                    if (const std::string* this_class = this_class_of(static_cast<VarExprent*>(inst)); this_class != nullptr && f->get_classname() != *this_class)
                        obj = mk(ir::Super{}, ir::class_type(l2_class_name(*this_class)));   // super.x
                }
                if (obj == nullptr) obj = casted(inst, VarType(cc::TYPE_OBJECT, 0, f->get_classname()), true);
                return mk_at(ir::Select{.obj = obj, .sym = ir::make_sym(ir::SymKind::Field, f->get_name(), owner, false, t)}, t, e);
            }
            case Exprent::EXPRENT_ASSIGNMENT: {
                auto* a = static_cast<AssignmentExprent*>(e);
                std::optional<std::string> op;
                if (a->get_cond_type() != AssignmentExprent::CONDITION_NONE) op = binary_op(a->get_cond_type());
                Node* target = expr(a->get_left());
                return mk_at(ir::Assign{.op = op, .target = target, .value = assigned_value(a)}, target->type, e);
            }
            case Exprent::EXPRENT_FUNCTION: return function(static_cast<FunctionExprent*>(e), t);
            case Exprent::EXPRENT_INVOCATION: return invocation(static_cast<InvocationExprent*>(e), t);
            case Exprent::EXPRENT_NEW: return new_node(static_cast<NewExprent*>(e), t);
            case Exprent::EXPRENT_ARRAY: {
                auto* a = static_cast<ArrayExprent*>(e);
                return mk_at(ir::Index{.arr = expr(a->get_array()), .index = expr(a->get_index())}, t, e);
            }
            case Exprent::EXPRENT_IF: return expr(static_cast<IfExprent*>(e)->get_condition());
            default: throw L2Declined{"expression", "an expression of kind " + std::to_string(e->type)};
        }
    }

    bool is_param(VarExprent* v) {
        // a parameter: an index below the method's parameters' slots, version 0 (Fernflower's parameters keep version 0)
        const StructMethod* mt = method_->method_struct;
        const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
        int slots = mt->has_modifier(cc::ACC_STATIC) ? 0 : 1;
        for (const VarType& p : md.params) slots += p.stack_size;
        return v->get_index() < slots && v->get_version() == 0;
    }

    static std::string binary_op(int func) {
        static const char* const ops[] = {"+", "-", "*", "/", "&", "|", "^", "%", "<<", ">>", ">>>"};
        if (func >= 0 && func <= FunctionExprent::FUNCTION_USHR) return ops[func];
        throw L2Declined{"operator", "the operator " + std::to_string(func)};
    }

    Node* constant(ConstExprent* c) {
        const VarType& ct = c->get_const_type();
        const auto& v = c->get_value();
        if (ct.type != cc::TYPE_NULL && std::holds_alternative<std::monostate>(v)) throw L2Declined{"constant", "a type written as a constant"};
        const auto lit = [&](ir::LitValue value, TypeP type) {
            // javac reads "-0.5D" as a minus before the number (a negative integer it reads as one literal)
            bool negative = false;
            if (auto* d = std::get_if<double>(&value); d != nullptr && std::signbit(*d)) {
                negative = true;
                *d = -*d;
            }
            Node* n = mk_at(ir::Lit{.value = std::move(value)}, type, c);
            return negative ? mk_at(ir::Unary{.op = "-", .x = n}, std::move(type), c) : n;
        };
        const bool literal = DecompilerContext::option(pref::LITERALS_AS_IS);
        switch (ct.type) {
            case cc::TYPE_BOOLEAN: return lit(std::get<std::int32_t>(v) != 0, ir::bool_type());
            case cc::TYPE_CHAR: return lit(l2_char_text(static_cast<std::uint32_t>(std::get<std::int32_t>(v)) & 0xFFFFu), frontend::parse_type("char"));
            case cc::TYPE_BYTE:
            case cc::TYPE_BYTECHAR:
            case cc::TYPE_SHORT:
            case cc::TYPE_SHORTCHAR:
            case cc::TYPE_INT: {
                const std::int32_t i = std::get<std::int32_t>(v);
                const TypeP int_t = ir::i32_type();
                if (!literal && i == std::numeric_limits<std::int32_t>::max()) return static_field("java.lang.Integer", "MAX_VALUE", int_t, c);
                if (!literal && i == std::numeric_limits<std::int32_t>::min()) return static_field("java.lang.Integer", "MIN_VALUE", int_t, c);
                return lit(std::int64_t{i}, int_t);
            }
            case cc::TYPE_LONG: {
                const std::int64_t l = std::get<std::int64_t>(v);
                const TypeP long_t = frontend::parse_type("long");
                if (!literal && l == std::numeric_limits<std::int64_t>::max()) return static_field("java.lang.Long", "MAX_VALUE", long_t, c);
                if (!literal && l == std::numeric_limits<std::int64_t>::min()) return static_field("java.lang.Long", "MIN_VALUE", long_t, c);
                return lit(l, long_t);
            }
            case cc::TYPE_DOUBLE:
            case cc::TYPE_FLOAT: {
                const bool is_float = ct.type == cc::TYPE_FLOAT;
                const double d = is_float ? static_cast<double>(std::get<float>(v)) : std::get<double>(v);
                const TypeP ft = frontend::parse_type(is_float ? "float" : "double");
                const std::string owner = is_float ? "java.lang.Float" : "java.lang.Double";
                if (!literal) {
                    if (std::isnan(d)) return static_field(owner, "NaN", ft, c);
                    if (d == std::numeric_limits<double>::infinity()) return static_field(owner, "POSITIVE_INFINITY", ft, c);
                    if (d == -std::numeric_limits<double>::infinity()) return static_field(owner, "NEGATIVE_INFINITY", ft, c);
                    if (is_float ? std::get<float>(v) == std::numeric_limits<float>::max() : d == std::numeric_limits<double>::max())
                        return static_field(owner, "MAX_VALUE", ft, c);
                    if (is_float ? std::get<float>(v) == std::numeric_limits<float>::denorm_min() : d == std::numeric_limits<double>::denorm_min())
                        return static_field(owner, "MIN_VALUE", ft, c);
                } else if (!std::isfinite(d)) {
                    throw L2Declined{"constant", "a literal that is not a finite number"};
                }
                return lit(d, ft);
            }
            case cc::TYPE_NULL: return lit(std::monostate{}, ir::null_type());
            case cc::TYPE_OBJECT:
                if (ct == VarType::STRING) return lit(std::get<std::string>(v), l2_string_type());
                if (ct == VarType::CLASS) {
                    const std::string& s = std::get<std::string>(v);
                    const VarType of = s.starts_with("[") ? VarType::parse(s, false) : VarType::parse(s, true);
                    return mk_at(ir::ClassLit{.of = l2_type(of)}, frontend::parse_type("java.lang.Class"), c);
                }
                break;
            default: break;
        }
        throw L2Declined{"constant", "a constant of type " + ct.to_string()};
    }

    Node* function(FunctionExprent* f, const TypeP& t) {
        const int op = f->get_func_type();
        auto& xs = f->get_lst_operands();
        const auto bin = [&](const std::string& o) { return mk_at(ir::Binary{.op = o, .left = expr(xs.at(0)), .right = expr(xs.at(1))}, t, f); };
        const auto un = [&](const std::string& o, bool postfix) { return mk_at(ir::Unary{.op = o, .x = expr(xs.at(0)), .postfix = postfix}, t, f); };
        if (op == FunctionExprent::FUNCTION_STR_CONCAT) {
            ir::Concat c;
            for (Exprent* part : {xs.at(0), xs.at(1)}) {
                Node* x = expr(part);
                if (auto* inner = ir::get<ir::Concat>(x); inner != nullptr && part == xs.at(0)) c.parts.insert(c.parts.end(), inner->parts.begin(), inner->parts.end());
                else c.parts.push_back(x);
            }
            return mk_at(std::move(c), l2_string_type(), f);
        }
        if (op <= FunctionExprent::FUNCTION_USHR) return bin(binary_op(op));
        switch (op) {
            case FunctionExprent::FUNCTION_EQ: return bin("==");
            case FunctionExprent::FUNCTION_NE: return bin("!=");
            case FunctionExprent::FUNCTION_LT: return bin("<");
            case FunctionExprent::FUNCTION_GE: return bin(">=");
            case FunctionExprent::FUNCTION_GT: return bin(">");
            case FunctionExprent::FUNCTION_LE: return bin("<=");
            case FunctionExprent::FUNCTION_CADD: return bin("&&");
            case FunctionExprent::FUNCTION_COR: return bin("||");
            case FunctionExprent::FUNCTION_BIT_NOT: return un("~", false);
            case FunctionExprent::FUNCTION_BOOL_NOT: return un("!", false);
            case FunctionExprent::FUNCTION_NEG: return un("-", false);
            case FunctionExprent::FUNCTION_IPP: return un("++", true);
            case FunctionExprent::FUNCTION_PPI: return un("++", false);
            case FunctionExprent::FUNCTION_IMM: return un("--", true);
            case FunctionExprent::FUNCTION_MMI: return un("--", false);
            case FunctionExprent::FUNCTION_CAST: return mk_at(ir::Cast{.x = expr(xs.at(0))}, l2_type(xs.at(1)->get_expr_type()), f);
            case FunctionExprent::FUNCTION_INSTANCEOF:
                return mk_at(ir::InstanceOf{.x = expr(xs.at(0)), .test = l2_type(xs.at(1)->get_expr_type())}, ir::bool_type(), f);
            case FunctionExprent::FUNCTION_ARRAY_LENGTH: {
                Node* arr = expr(xs.at(0));
                if (xs.at(0)->get_expr_type().array_dim == 0) arr = mk(ir::Cast{.x = arr}, l2_type(VarType::OBJECT.resize_array_dim(1)));
                return mk_at(ir::Length{.arr = arr}, ir::i32_type(), f);
            }
            case FunctionExprent::FUNCTION_IIF:
                return mk_at(ir::Cond{.cond = expr(xs.at(0)), .then = expr(xs.at(1)), .orelse = expr(xs.at(2))}, t, f);
            default: break;
        }
        if (op >= FunctionExprent::FUNCTION_I2L && op <= FunctionExprent::FUNCTION_I2S) return mk_at(ir::Cast{.x = expr(xs.at(0))}, t, f);   // (int)x ...
        throw L2Declined{"operator", "the function " + std::to_string(op)};
    }

    Node* invocation(InvocationExprent* inv, const TypeP& t) {
        if (inv->get_invocation_typ() == InvocationExprent::INVOKE_DYNAMIC) throw L2Declined{"invokedynamic", "an invokedynamic call"};
        if (inv->get_functype() != InvocationExprent::TYP_GENERAL) throw L2Declined{"ctor-call", "a constructor call inside an expression"};
        ir::Call c;
        c.sym = method_sym(*inv);
        if (inv->is_static()) {
            c.target = mk(ir::TypeName{}, ir::class_type(l2_class_name(inv->get_classname())));
        } else {
            Exprent* inst = inv->get_instance();
            const std::string* this_class = inst != nullptr && inst->type == Exprent::EXPRENT_VAR ? this_class_of(static_cast<VarExprent*>(inst)) : nullptr;
            if (this_class != nullptr && inv->get_invocation_typ() == InvocationExprent::INVOKE_SPECIAL && inv->get_classname() != *this_class) {
                c.target = mk(ir::Super{}, ir::class_type(l2_class_name(*this_class)));   // super.m()
            } else if (inst != nullptr) {
                const VarType right_type = inst->get_expr_type();
                const VarType left_type(cc::TYPE_OBJECT, 0, inv->get_classname());
                c.target = expr(inst);
                if (right_type == VarType::OBJECT && !(left_type == right_type)) c.target = mk(ir::Cast{.x = c.target}, l2_type(left_type));
            }
        }
        c.args = call_args(*inv, inv->get_classname(), false);
        const TypeP type = c.sym->ret;
        return mk_at(std::move(c), inv->get_classname().starts_with("[") && inv->get_name() == "clone" ? type : t, inv);
    }

    // A lambda or a method reference (ClassWriter::class_lambda_to_java): its parameters are the content method's after the values it captures,
    // its body that method's statements, lowered as part of the method around it (its locals are that method's, as javac's are).
    Node* lambda(NewExprent* n, const TypeP& t) {
        ClassNode* child = n->get_new_type().value ? DecompilerContext::current().class_processor->get_class_node(*n->get_new_type().value) : nullptr;
        if (child == nullptr || !child->lambda_information || child->get_wrapper() == nullptr) throw L2Declined{"lambda", "a lambda whose class is not known"};
        const ClassNode::LambdaInformation& li = *child->lambda_information;
        InvocationExprent* ctor = n->get_constructor();
        Exprent* method_object = ctor != nullptr ? ctor->get_instance() : nullptr;
        if (li.is_method_reference) {
            ir::MethodRef r;
            const bool is_new = li.content_method_name == cc::INIT_NAME;
            if (!li.is_content_method_static && method_object != nullptr && !is_new) r.target = expr(method_object);
            else r.target = mk(ir::TypeName{}, l2_type(VarType(cc::TYPE_OBJECT, 0, li.content_class_name)));
            const MethodDescriptor md = MethodDescriptor::parse(li.content_method_descriptor);
            r.sym = ir::make_sym(is_new ? ir::SymKind::Ctor : ir::SymKind::Method, li.content_method_name, l2_class_name(li.content_class_name), li.is_content_method_static);
            for (const VarType& p : md.params) r.sym->params.push_back(l2_type(p));
            r.sym->ret = is_new ? ir::void_type() : l2_type(md.ret);
            r.mode = is_new ? "new" : "invoke";
            return mk_at(std::move(r), t, n);
        }
        ClassWrapper* wrapper = child->get_wrapper();
        const StructMethod* mt = wrapper->get_class_struct()->get_method(li.content_method_key);
        if (mt == nullptr) throw L2Declined{"lambda", "a lambda whose method is not known"};
        MethodWrapper* mw = wrapper->get_method_wrapper(mt->name(), mt->descriptor());
        if (mw == nullptr || mw->root == nullptr || mw->decompiled_with_errors) throw L2Declined{"lambda", "a lambda Fernflower could not decompile"};
        const MethodDescriptor md_content = MethodDescriptor::parse(li.content_method_descriptor);
        const MethodDescriptor md_lambda = MethodDescriptor::parse(li.method_descriptor);

        DecompilerContext& ctx = DecompilerContext::current();
        MethodWrapper* const saved_wrapper = std::exchange(ctx.current_method_wrapper, mw);
        MethodWrapper* const saved_method = std::exchange(method_, mw);
        ClassNode* const saved_current = std::exchange(ctx.current_class_node, child);
        const auto saved_captured = captured_;
        struct Restore {
            std::function<void()> f;
            ~Restore() { f(); }
        } restore{[&] {
            ctx.current_method_wrapper = saved_wrapper;
            method_ = saved_method;
            ctx.current_class_node = saved_current;
            captured_ = saved_captured;
        }};
        ir::Lambda l;
        int index = li.is_content_method_static ? 0 : 1;
        const int start = static_cast<int>(md_content.params.size()) - static_cast<int>(md_lambda.params.size());
        captured_ = {index, index};
        for (int i = 0; i < static_cast<int>(md_content.params.size()); ++i) {
            const VarType& pt = md_content.params[static_cast<std::size_t>(i)];
            if (i >= start) {
                const std::optional<std::string> pname = mw->varproc->get_var_name(VarVersionPair(index, 0));
                const TypeP ptype = l2_type(pt);   // (the content method's: the types javac inferred)
                l.params.push_back(mk(ir::Param{.sym = ir::make_sym(ir::SymKind::Param, pname.value_or("param" + std::to_string(index)), owner_, false, ptype), .type = ptype}));
            } else {
                captured_.second = index + pt.stack_size;   // (a value the lambda captured: the method around's local)
            }
            index += pt.stack_size;
        }
        l.body = mk(ir::Block{.stmts = root_stmts(mw->root)});
        return mk_at(std::move(l), t, n);
    }


    Node* new_node(NewExprent* n, const TypeP& t) {
        if (n->is_lambda()) return lambda(n, t);
        if (n->is_anonymous()) throw L2Declined{"anonymous-class", "an anonymous class"};
        const VarType& new_type = n->get_new_type();
        if (new_type.array_dim > 0) {
            ir::NewArray a;
            a.elem = ir::dims(t).second;
            if (n->get_lst_array_elements().empty())   // (new int[]{1, 2} writes no size)
                for (Exprent* d : n->get_lst_dims()) a.dims.push_back(expr(d));
            const VarType item_type = new_type.decrease_array_dim();
            for (Exprent* x : n->get_lst_array_elements()) a.init.push_back(casted(x, item_type, false));
            return mk_at(std::move(a), t, n);
        }
        InvocationExprent* ctor = n->get_constructor();
        if (ctor == nullptr) throw L2Declined{"new", "a new object with no constructor"};
        ir::New made;
        made.sym = ctor_sym(*ctor);
        const SignatureFields sig = signature_fields(ctor->get_classname(), ctor->get_string_descriptor(), ctor->get_lst_parameters().size());
        ClassNode* node = DecompilerContext::current().class_processor->get_class_node(ctor->get_classname());
        if (sig && node != nullptr && node->type == ClassNode::CLASS_MEMBER && (node->access & cc::ACC_STATIC) == 0 && !ctor->get_lst_parameters().empty()) {
            Exprent* outer = ctor->get_lst_parameters()[0];   // outer.new Inner(): the enclosing instance, unless it is this
            const bool is_this = outer->type == Exprent::EXPRENT_VAR && this_class_of(static_cast<VarExprent*>(outer)) != nullptr;
            if (!is_this) made.outer = expr(outer);
        }
        const auto& params = ctor->get_lst_parameters();
        for (std::size_t i = 0; i < params.size(); ++i) {
            if (sig && sig->at(i)) continue;
            if (i + 1 == params.size() && params[i]->get_expr_type().is_constant(VarType::NULL_TYPE)) {
                const VarType left_type = ctor->get_descriptor().params.at(i);
                ClassNode* stub = left_type.value ? DecompilerContext::current().class_processor->get_class_node(*left_type.value) : nullptr;
                if (stub != nullptr && stub->nameless_constructor_stub) break;   // (the synthetic constructor's last parameter)
            }
            made.args.push_back(casted(params[i], ctor->get_descriptor().params.at(i), true));
        }
        return mk_at(std::move(made), t, n);
    }
};

}  // namespace

Node* lower_class(cb::Arena& arena, ClassNode* root) { return ClassLowering(arena).unit(root); }

Lowered decompile(const DecompileRequest& request, cb::Arena& arena) {
    Lowered out;
    DecompileRequest with = request;
    with.on_class_written = [&out, &arena, then = request.on_class_written](ClassNode* root) {
        if (then) then(root);
        out.units.push_back(lower_class(arena, root));
    };
    out.text = cb::java::decompiler::decompile(with);
    return out;
}

}  // namespace cb::java::decompiler::lower_l2
