#include "cb/translate/backends/csharp_cpp17.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <functional>
#include <unordered_set>

#include "cb/core/text.hpp"
#include "cb/translate/backends/csharp_cpp17_internal.hpp"

namespace cb::translate::backends::csharp_cpp17 {

namespace detail {

namespace {

// C++ words (and C library names) that a C# identifier must not be.
const std::unordered_set<std::string_view> kCxReserved = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "char", "char16_t", "char32_t", "compl", "concept", "const_cast",
    "constexpr", "decltype", "delete", "double", "dynamic_cast", "explicit", "export", "extern", "float", "friend", "inline", "int", "long",
    "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "register",
    "reinterpret_cast", "requires", "short", "signed", "static_assert", "static_cast", "struct", "template", "this", "thread_local", "typedef",
    "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "xor", "xor_eq", "main", "NULL", "EOF", "errno",
    "stdin", "stdout", "stderr", "min", "max", "near", "far", "small", "cs", "std"};

// The runtime's names for the library's classes (value: the C++ spelling, a class held through cs::Ref).
const std::map<std::string, std::string, std::less<>> kCxSystemClasses = {
    {"System.Exception", "cs::Exception"},
    {"System.SystemException", "cs::SystemException"},
    {"System.ArgumentException", "cs::ArgumentException"},
    {"System.ArgumentNullException", "cs::ArgumentNullException"},
    {"System.ArgumentOutOfRangeException", "cs::ArgumentOutOfRangeException"},
    {"System.InvalidOperationException", "cs::InvalidOperationException"},
    {"System.ArithmeticException", "cs::ArithmeticException"},
    {"System.DivideByZeroException", "cs::DivideByZeroException"},
    {"System.OverflowException", "cs::OverflowException"},
    {"System.IndexOutOfRangeException", "cs::IndexOutOfRangeException"},
    {"System.NullReferenceException", "cs::NullReferenceException"},
    {"System.InvalidCastException", "cs::InvalidCastException"},
    {"System.FormatException", "cs::FormatException"},
    {"System.NotSupportedException", "cs::NotSupportedException"},
    {"System.NotImplementedException", "cs::NotImplementedException"},
    {"System.Collections.Generic.KeyNotFoundException", "cs::KeyNotFoundException"},
    {"System.Collections.Generic.List", "cs::List"},
    {"System.Collections.Generic.Dictionary", "cs::Dictionary"},
    {"System.Collections.Generic.IEnumerable", "cs::IEnumerable"},
    {"System.Linq.IOrderedEnumerable", "cs::IEnumerable"},
    {"System.Linq.IGrouping", "cs::Grouping"},
    {"System.Text.StringBuilder", "cs::StringBuilder"},
    {"System.Type", "cs::Type"},
    {"System.Reflection.MemberInfo", "cs::Type"},
    {"System.Object", "cs::Object"},
};

std::string cx_strip_package(const std::string& qname, const std::string& package) {
    std::string name = qname;
    if (!package.empty() && name.starts_with(package + ".")) name = name.substr(package.size() + 1);
    return text::replace_all(text::replace_all(name, ".", "_"), "+", "_");
}

}  // namespace

std::string ident(std::string_view name) {
    std::string out(name);
    // the compiler's own names (`<>h__TransparentIdentifier0`, a query's range variable) have characters C++ names cannot
    for (char& c : out) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') c = '_';
    }
    if (kCxReserved.contains(out)) out += "_";
    return out;
}

std::string u16_literal_text(const std::string& utf8) {
    std::vector<std::uint32_t> units;
    for (std::size_t i = 0; i < utf8.size();) {
        const auto b = static_cast<unsigned char>(utf8[i]);
        std::uint32_t cp = b;
        std::size_t n = 1;
        if (b >= 0xF0) {
            cp = b & 7;
            n = 4;
        } else if (b >= 0xE0) {
            cp = b & 15;
            n = 3;
        } else if (b >= 0xC0) {
            cp = b & 31;
            n = 2;
        }
        for (std::size_t k = 1; k < n && i + k < utf8.size(); ++k) cp = (cp << 6) | (static_cast<unsigned char>(utf8[i + k]) & 0x3F);
        i += n;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            units.push_back(0xD800 + (cp >> 10));
            units.push_back(0xDC00 + (cp & 0x3FF));
        } else {
            units.push_back(cp);
        }
    }
    std::string out = "u\"";
    bool hex = false;   // the last piece was a \x escape: a following hex digit must not run into it
    for (const std::uint32_t u : units) {
        const bool plain = u >= 0x20 && u < 0x7F && u != '"' && u != '\\' && u != '?';
        if (hex && plain && std::isxdigit(static_cast<int>(u)) != 0) {
            out += "\" u\"";
            hex = false;
        }
        if (plain) {
            out += static_cast<char>(u);
            continue;
        }
        hex = false;
        switch (u) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '?': out += "\\?"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                out += std::format("\\x{:x}", u);
                hex = true;
        }
    }
    return out + "\"";
}

std::string Writer::accessor_name(const ir::SymP& prop, const std::string& kind) {
    const std::string base = prop->name == "this[]" || prop->name == "Item" ? "Item" : prop->name;
    return (kind == "get" ? "get_" : "set_") + base;
}

// ------------------------------------------------------------------------------------------------------------------- registry

void Writer::register_class(Node* node, const std::string& package, const std::string& outer) {
    const auto* d = ir::get<ir::ClassDecl>(node);
    if (d == nullptr) return;
    ClassInfo c;
    c.node = node;
    c.decl = d;
    c.package = package;
    const std::string qname = d->qname.value_or(d->name.value_or("?"));
    c.cname = cx_strip_package(qname, package);
    c.typeparams = d->typeparams;
    const std::string kind = d->kind.value_or("class");
    c.is_struct = kind == "struct" || kind == "record struct";
    c.is_interface = kind == "interface";
    c.is_enum = kind == "enum";
    c.is_record = kind == "record" || kind == "record struct";
    c.is_delegate = kind == "delegate";
    (void)outer;
    for (Node* m : d->members) {
        if (const auto* p = ir::get<ir::Property>(m)) {
            PropInfo info;
            info.type = p->type;
            info.is_static = p->sym->is_static;
            const bool abstract_member = c.is_interface || std::ranges::find(p->mods, "abstract") != p->mods.end();
            for (Node* a : p->accessors) {
                const auto* am = ir::get<ir::MethodDecl>(a);
                const std::string k = am->kind.value_or("");
                if (k == "get") info.has_get = true;
                if (k == "set" || k == "init") info.has_set = true;
                if (!abstract_member && !ir::present(am->body)) info.is_auto = true;
            }
            props_[qname + "." + p->sym->name] = info;
        } else if (ir::is<ir::ClassDecl>(m)) {
            register_class(m, package, qname);
        }
    }
    if (c.is_record) {   // a record's primary constructor makes a property of each parameter
        for (Node* m : d->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md == nullptr || md->kind.value_or("") != "ctor" || std::ranges::find(md->mods, "primary") == md->mods.end()) continue;
            for (Node* p : md->params) {
                const auto* pa = ir::get<ir::Param>(p);
                PropInfo info;
                info.type = pa->type;
                info.has_get = info.is_auto = true;
                props_[qname + "." + pa->sym->name] = info;
            }
        }
    }
    order_.push_back(qname);
    classes_[qname] = std::move(c);
}

const ClassInfo* Writer::info(const std::string& qname) const {
    const auto it = classes_.find(qname);
    return it == classes_.end() ? nullptr : &it->second;
}

std::string Writer::cxx_class(const std::string& qname) const {
    const ClassInfo* c = info(qname);
    if (c == nullptr) decline("library", "the class " + qname + " is not one the runtime has");
    if (c->package == package_ && !member_names_.contains(c->cname)) return c->cname;
    return "::" + (c->package.empty() ? "" : text::replace_all(c->package, ".", "::") + "::") + c->cname;
}

std::optional<PropInfo> Writer::prop_of(const ir::SymP& sym) const {
    if (!sym || !sym->owner) return std::nullopt;
    const auto it = props_.find(*sym->owner + "." + sym->name);
    if (it == props_.end()) return std::nullopt;
    return it->second;
}

// ------------------------------------------------------------------------------------------------------------------- types

const ir::ClassType* Writer::class_of(const TypeP& t) const { return t ? std::get_if<ir::ClassType>(&t->body) : nullptr; }

bool Writer::is_string(const TypeP& t) const {
    const auto* c = class_of(t);
    return c != nullptr && c->name == "System.String";
}
bool Writer::is_nullable(const TypeP& t) const {
    const auto* c = class_of(t);
    return c != nullptr && c->name == "System.Nullable";
}
bool Writer::is_numeric(const TypeP& t) const {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    return p != nullptr && p->prim != ir::Prim::Bool;
}
bool Writer::is_integral(const TypeP& t) const {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    return p != nullptr && p->prim != ir::Prim::Bool && p->prim != ir::Prim::F32 && p->prim != ir::Prim::F64 && p->prim != ir::Prim::F80;
}
bool Writer::is_floating(const TypeP& t) const {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    return p != nullptr && (p->prim == ir::Prim::F32 || p->prim == ir::Prim::F64 || p->prim == ir::Prim::F80);
}
bool Writer::is_function(const TypeP& t) const {
    const auto* c = class_of(t);
    if (c == nullptr) return false;
    if (c->name == "System.Action" || c->name == "System.Func" || c->name == "System.Predicate") return true;
    const ClassInfo* k = info(c->name);
    return k != nullptr && k->is_delegate;
}
bool Writer::is_user_value(const TypeP& t) const {
    const auto* c = class_of(t);
    const ClassInfo* k = c != nullptr ? info(c->name) : nullptr;
    return k != nullptr && (k->is_struct || k->is_enum);
}
bool Writer::is_value(const TypeP& t) const {
    if (!t) return false;
    if (t->is_prim()) return true;
    const auto* c = class_of(t);
    if (c == nullptr) return false;
    return is_string(t) || is_nullable(t) || is_function(t) || is_user_value(t) || c->name == "System.Collections.Generic.KeyValuePair";
}
// A value of this type is a reference that boxing a value type would also be: the type is object, or a program interface.
bool Writer::is_object_like(const TypeP& t) const {
    const auto* c = class_of(t);
    if (c == nullptr) return false;
    if (c->name == "System.Object") return true;
    const ClassInfo* k = info(c->name);
    return k != nullptr && k->is_interface;
}

std::string Writer::prim_name(const ir::PrimType& p) {
    switch (p.prim) {
        case ir::Prim::Bool: return "bool";
        case ir::Prim::I8: return "signed char";
        case ir::Prim::I16: return "short";
        case ir::Prim::I32: return "int";
        case ir::Prim::I64: return "long long";
        case ir::Prim::U8: return "unsigned char";
        case ir::Prim::U16: return p.encoding && *p.encoding == ir::CharEncoding::Utf16 ? "char16_t" : "unsigned short";
        case ir::Prim::U32: return "unsigned";
        case ir::Prim::U64: return "unsigned long long";
        case ir::Prim::F32: return "float";
        case ir::Prim::F64: return "double";
        case ir::Prim::F80: return "long double";
        case ir::Prim::Char: return "char";
    }
    return "int";
}

// C#'s binary numeric promotion of two operand types, as a C++ type name; empty when they are not both numbers.
std::string Writer::promoted(const TypeP& a, const TypeP& b) const {
    const auto* pa = a ? std::get_if<ir::PrimType>(&a->body) : nullptr;
    const auto* pb = b ? std::get_if<ir::PrimType>(&b->body) : nullptr;
    if (pa == nullptr || pb == nullptr || pa->prim == ir::Prim::Bool || pb->prim == ir::Prim::Bool) return "";
    const auto has = [&](ir::Prim p) { return pa->prim == p || pb->prim == p; };
    if (has(ir::Prim::F64)) return "double";
    if (has(ir::Prim::F32)) return "float";
    if (has(ir::Prim::U64)) return "unsigned long long";
    if (has(ir::Prim::I64)) return "long long";
    if (has(ir::Prim::U32)) {
        const auto signed_other = [](ir::Prim p) { return p == ir::Prim::I8 || p == ir::Prim::I16 || p == ir::Prim::I32; };
        return signed_other(pa->prim) || signed_other(pb->prim) ? "long long" : "unsigned";
    }
    return "int";
}

TypeP Writer::function_result(const TypeP& t) const {
    const auto* c = class_of(t);
    if (c == nullptr) return TypeP{};
    if (c->name == "System.Action") return ir::void_type();
    if (c->name == "System.Predicate") return ir::bool_type();
    if (c->name == "System.Func" && !c->args.empty()) return c->args.back();
    const ClassInfo* k = info(c->name);
    if (k != nullptr && k->is_delegate) {
        for (Node* m : k->decl->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md != nullptr && md->name.value_or("") == "Invoke") return md->ret;
        }
    }
    return TypeP{};
}

std::string Writer::type_list(const std::vector<TypeP>& args) {
    std::string out;
    for (std::size_t i = 0; i < args.size(); ++i) out += (i ? ", " : "") + type(args[i]);
    return out;
}

// A class's C++ name with its type arguments, without the reference wrapper.
std::string Writer::class_text(const ir::ClassType& c) {
    if (const auto it = kCxSystemClasses.find(c.name); it != kCxSystemClasses.end()) {
        return c.args.empty() ? it->second : it->second + "<" + type_list(c.args) + ">";
    }
    std::string name = cxx_class(c.name);
    if (!c.args.empty()) name += "<" + type_list(c.args) + ">";
    return name;
}

std::string Writer::type(const TypeP& t) {
    if (!t) decline("type", "a type the front end did not give");
    return std::visit(
        [&](const auto& b) -> std::string {
            using B = std::decay_t<decltype(b)>;
            if constexpr (std::is_same_v<B, ir::PrimType>) {
                return prim_name(b);
            } else if constexpr (std::is_same_v<B, ir::VoidType>) {
                return "void";
            } else if constexpr (std::is_same_v<B, ir::NullType>) {
                return "std::nullptr_t";
            } else if constexpr (std::is_same_v<B, ir::ClassType>) {
                if (b.name == "System.String") return "cs::String";
                if (b.name == "System.Nullable" && b.args.size() == 1) return "std::optional<" + type(b.args[0]) + ">";
                if (b.name == "System.Collections.Generic.KeyValuePair") return "cs::KeyValuePair<" + type_list(b.args) + ">";
                // a task (and the awaiter of one: cs::TaskOf is its own awaiter) with a result is a TaskOf<T>, one without is a Task
                if (b.name == "System.Threading.Tasks.Task" || b.name == "System.Runtime.CompilerServices.TaskAwaiter") {
                    return b.args.size() == 1 ? "cs::Ref<cs::TaskOf<" + type(b.args[0]) + ">>" : "cs::Ref<cs::Task>";
                }
                if (b.name == "System.IComparable" || b.name == "System.IDisposable") return "cs::Ref<cs::Object>";
                if (b.name == "System.Action" || b.name == "System.Func" || b.name == "System.Predicate") {
                    std::string params;
                    std::string result = "void";
                    const std::size_t n = b.args.size();
                    if (b.name == "System.Action") {
                        params = type_list(b.args);
                    } else if (b.name == "System.Predicate") {
                        params = type_list(b.args);
                        result = "bool";
                    } else if (n > 0) {
                        params = type_list(std::vector<TypeP>(b.args.begin(), b.args.end() - 1));
                        result = type(b.args.back());
                    }
                    return "std::function<" + result + "(" + params + ")>";
                }
                if (const ClassInfo* k = info(b.name)) {
                    if (k->is_delegate) {
                        std::string params;
                        std::string result = "void";
                        for (Node* m : k->decl->members) {
                            const auto* md = ir::get<ir::MethodDecl>(m);
                            if (md == nullptr || md->name.value_or("") != "Invoke") continue;
                            result = type(md->ret);
                            for (std::size_t i = 0; i < md->params.size(); ++i) params += (i ? ", " : "") + type(ir::get<ir::Param>(md->params[i])->type);
                        }
                        return "std::function<" + result + "(" + params + ")>";
                    }
                    if (k->is_struct || k->is_enum) return class_text(b);
                    return "cs::Ref<" + class_text(b) + ">";
                }
                if (kCxSystemClasses.contains(b.name)) return "cs::Ref<" + class_text(b) + ">";
                decline("library", "the library type " + b.name + " has no C++ counterpart yet");
            } else if constexpr (std::is_same_v<B, ir::TypeVarType>) {
                return b.name;
            } else if constexpr (std::is_same_v<B, ir::ArrayType>) {
                if (b.rank == 1) return "cs::Ref<cs::Array<" + type(b.elem) + ">>";
                if (b.rank == 2) return "cs::Ref<cs::Array2<" + type(b.elem) + ">>";
                decline("array", "an array of more than two dimensions");
            } else {
                decline("type", "the type " + ir::to_text(*t) + " has no C# meaning");
            }
        },
        t->body);
}

// ------------------------------------------------------------------------------------------------------------------- order

std::vector<std::string> Writer::sorted_classes() const {
    std::vector<std::string> out;
    std::set<std::string> done, busy;
    std::function<void(const std::string&)> visit = [&](const std::string& q) {
        const ClassInfo* c = info(q);
        if (c == nullptr || done.contains(q) || busy.contains(q)) return;
        busy.insert(q);
        const auto dep = [&](const TypeP& t) {
            const auto* ct = class_of(t);
            if (ct != nullptr) visit(ct->name);
        };
        dep(c->decl->extends);
        for (const TypeP& i : c->decl->implements) dep(i);
        for (Node* m : c->decl->members) {   // a struct held by value must be complete first
            if (const auto* f = ir::get<ir::FieldDecl>(m)) {
                if (is_user_value(f->type)) dep(f->type);
            } else if (const auto* p = ir::get<ir::Property>(m)) {
                if (is_user_value(p->type)) dep(p->type);
            }
        }
        busy.erase(q);
        done.insert(q);
        out.push_back(q);
    };
    for (const std::string& q : order_) visit(q);
    return out;
}

std::string Writer::template_head(const ClassInfo& c, const std::vector<std::string>& method_tparams) const {
    std::string out;
    if (!c.typeparams.empty()) {
        out += "template <";
        for (std::size_t i = 0; i < c.typeparams.size(); ++i) out += (i ? ", " : "") + std::string("class ") + c.typeparams[i];
        out += "> ";
    }
    if (!method_tparams.empty()) {
        out += "template <";
        for (std::size_t i = 0; i < method_tparams.size(); ++i) out += (i ? ", " : "") + std::string("class ") + method_tparams[i];
        out += "> ";
    }
    return out;
}

std::string Writer::self_type(const ClassInfo& c) const {
    std::string name = c.cname;
    if (!c.typeparams.empty()) {
        name += "<";
        for (std::size_t i = 0; i < c.typeparams.size(); ++i) name += (i ? ", " : "") + c.typeparams[i];
        name += ">";
    }
    return name;
}

std::string Writer::params_text(const std::vector<Node*>& params, const ir::SymP& sym) {
    std::string out;
    for (std::size_t i = 0; i < params.size(); ++i) {
        const auto* p = ir::get<ir::Param>(params[i]);
        std::string t = type(p->type);
        const ir::PassMode mode = sym && i < sym->refs.size() ? sym->refs[i] : ir::PassMode::ByValue;
        if (mode == ir::PassMode::Ref || mode == ir::PassMode::Out) t += "&";
        else if (mode == ir::PassMode::In) t = "const " + t + "&";
        const std::string name = boxed_.contains(sym_key(*p->sym)) ? ident(p->sym->name) + "__in" : ident(p->sym->name);   // (a captured one is boxed on entry)
        out += (i ? ", " : "") + t + " " + name;
    }
    return out;
}

// ------------------------------------------------------------------------------------------------------------------- program

Output Writer::run() {
    Output result;
    for (ir::Node* u : units_) {
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        for (Node* t : unit->types) register_class(t, unit->package.value_or(""), "");
    }
    const std::vector<std::string> order = sorted_classes();
    out_ += "// Translated from C# by CodeBreaker (the csharp_cpp17 back end); built against config/translate/runtime/cpp17/csrt.hpp.\n";
    out_ += "#include \"csrt.hpp\"\n\n";
    // one namespace block for a run of classes of one package
    const auto each = [&](const std::function<void(const ClassInfo&)>& f) {
        std::optional<std::string> open_package;
        const auto leave = [&] {
            if (open_package && !open_package->empty()) line("}  // namespace " + text::replace_all(*open_package, ".", "::"));
            open_package.reset();
        };
        for (const std::string& q : order) {
            const ClassInfo& c = classes_.at(q);
            if (!open_package || *open_package != c.package) {
                leave();
                open_package = c.package;
                if (!c.package.empty()) line("namespace " + text::replace_all(c.package, ".", "::") + " {");
            }
            package_ = c.package;
            f(c);
        }
        leave();
    };
    each([&](const ClassInfo& c) { write_forward(c); });
    line("");
    each([&](const ClassInfo& c) {
        if (c.is_delegate) {
            where_ = c.cname;
            try {
                line("using " + c.cname + " = " + type(ir::class_type(c.decl->qname.value_or(c.cname))) + ";");
            } catch (const Declined& d) {
                gaps_.push_back({c.cname, d.kind, d.why});
            }
        }
    });
    line("");
    each([&](const ClassInfo& c) {
        if (!c.is_delegate && !c.is_enum) write_class(c);
    });
    each([&](const ClassInfo& c) {
        if (!c.is_delegate && !c.is_enum) write_bodies(c);
    });
    line("");
    each([&](const ClassInfo& c) {
        if (!c.is_delegate && !c.is_enum) write_statics(c);
    });
    package_.clear();
    write_main();
    result.declined = std::move(gaps_);
    if (result.declined.empty()) result.source = std::move(out_);
    return result;
}

void Writer::write_forward(const ClassInfo& c) {
    if (c.is_delegate) return;
    if (c.is_enum) {
        write_enum(c);
        return;
    }
    line(template_head(c) + "struct " + c.cname + ";");
}

void Writer::write_enum(const ClassInfo& c) {
    open("enum class " + c.cname + " : int");
    for (Node* m : c.decl->members) {
        const auto* e = ir::get<ir::EnumConst>(m);
        if (e == nullptr) continue;
        line(ident(e->sym->name) + " = " + std::to_string(e->value.value_or(0)) + ",");
    }
    close(";");
    open("inline cs::String cs_enum_name(" + c.cname + " v)");
    open("switch (v)");
    for (Node* m : c.decl->members) {
        const auto* e = ir::get<ir::EnumConst>(m);
        if (e == nullptr) continue;
        line("case " + c.cname + "::" + ident(e->sym->name) + ": return cs::String(" + u16_literal_text(e->sym->name) + ");");
    }
    close();
    line("return cs::str(static_cast<int>(v));");
    close();
}

void Writer::write_main() {
    const ClassInfo* entry = nullptr;
    const ir::MethodDecl* main_method = nullptr;
    for (const std::string& q : order_) {
        const ClassInfo& c = classes_.at(q);
        if (!main_.empty() && q != main_ && c.cname != main_) continue;
        for (Node* m : c.decl->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md != nullptr && md->name.value_or("") == "Main" && md->kind.value_or("") == "method" && md->sym->is_static && md->params.size() <= 1) {
                entry = &c;
                main_method = md;
                break;
            }
        }
        if (entry != nullptr) break;
    }
    if (entry == nullptr) {
        gaps_.push_back({"", "program", "no static Main to run"});
        return;
    }
    package_ = entry->package;
    open("int main()");
    open("try");
    for (const std::string& q : order_) {
        const ClassInfo& c = classes_.at(q);
        for (Node* m : c.decl->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (md != nullptr && md->kind.value_or("") == "static_init") line("::" + (c.package.empty() ? "" : text::replace_all(c.package, ".", "::") + "::") + c.cname + "::cs_static_init();");
        }
    }
    const std::string qualified = "::" + (entry->package.empty() ? "" : text::replace_all(entry->package, ".", "::") + "::") + entry->cname;
    const std::string args = main_method->params.empty() ? "" : "cs::make_array<cs::String>(0)";
    const bool returns_int = main_method->ret && main_method->ret->is_prim();
    const auto* async_main = class_of(main_method->ret);
    if (async_main != nullptr && async_main->name == "System.Threading.Tasks.Task") {   // `static async Task Main`: run until the task is done
        if (async_main->args.size() == 1) line("return " + qualified + "::Main(" + args + ")->Result();");
        else {
            line(qualified + "::Main(" + args + ")->Wait();");
            line("return 0;");
        }
    } else if (returns_int) line("return " + qualified + "::Main(" + args + ");");
    else {
        line(qualified + "::Main(" + args + ");");
        line("return 0;");
    }
    close();
    open("catch (const cs::Thrown& t)");
    line("std::fflush(stdout);");
    line("std::fputs((\"Unhandled exception. \" + cs::utf8(t.ex->ToString().chars()) + \"\\n\").c_str(), stderr);");
    line("return static_cast<int>(0xE0434352u);");
    close();
    close();
}

}  // namespace detail

Output write(std::span<ir::Node* const> units, const std::string& main) {
    detail::Writer writer(units, main);
    return writer.run();
}

}  // namespace cb::translate::backends::csharp_cpp17
