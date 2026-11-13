// cb/translate/backends/c_java_call.cpp -- calls, `new` and arrays in the C back end for Java: a call into the program goes through
// a class's table, an interface's tables or straight to the function; a call into the Java library goes to the runtime's
// `jrt_Class_method_Codes` (see c_java.hpp).

#include <algorithm>
#include <format>

#include "cb/core/text.hpp"
#include "cb/translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java::detail {

namespace {

// The C parameter types of a library function, from the erased Java ones: what the runtime declares (a primitive, jstring*, jarray*
// or jobject*).
std::vector<std::string> runtime_parameter_types(const Program& program, std::span<const ir::TypeP> params) {
    std::vector<std::string> out;
    for (const ir::TypeP& t : params) {
        const ir::TypeP erased = jmodel::erase(t);
        if (!prim_ctype(erased->kind()).empty()) out.push_back(program.ct(erased));
        else if (type_code(erased) == "Str") out.emplace_back(kStr);
        else if (erased->kind() == "array") out.emplace_back(kArr);
        else out.emplace_back(kObj);
    }
    return out;
}

std::string runtime_codes(std::span<const ir::TypeP> params) {
    std::string codes;
    for (const ir::TypeP& t : params) codes += type_code(jmodel::erase(t));
    return codes;
}

// The values of a runtime call as `, a, b` (after a count or another argument).
std::string comma_list(const std::vector<std::string>& values) {
    std::string out;
    for (const std::string& v : values) out += ", " + v;
    return out;
}

std::string object_function(std::string_view name) {
    return name == "toString" ? "jrt_toString" : name == "equals" ? "jrt_equals" : "jrt_hashCode";
}

}  // namespace

// The arguments of a call as C: each converted to its parameter's C type, a closure written where an argument is a lambda or a
// method reference, a varargs call's tail packed into an array.
std::string MethodWriter::args(std::span<ir::Node* const> list, const std::vector<std::string>& params, const ir::SymP& sym, bool lib) {
    const auto converted = [&](const ir::Node* a, std::string_view want) {
        const Code c = expr(a);
        return conv(c.code, c.ctype, want);
    };
    bool packed = false;   // a varargs call whose tail is not already an array
    if (sym && sym->varargs && !params.empty()) {
        bool direct = false;
        if (list.size() == params.size()) {
            const std::string_view last = kind_of(type_of(list.back()));
            direct = last == "array" || last == "null";
        }
        packed = !direct;
    }
    if (packed) {
        if (lib) throw Decline("library varargs " + sym->name);
        const std::size_t fixed = params.size() - 1;
        std::vector<std::string> head;
        for (std::size_t i = 0; i < std::min(fixed, list.size()); ++i) head.push_back(converted(list[i], params[i]));
        const ir::TypeP last = sym->params.empty() ? ir::TypeP() : sym->params.back();
        if (!last) throw Decline("a varargs call without parameter types");
        const std::string store = array_store(kind_of(last) == "array" ? p_.ct(jmodel::erase(elem_of(last))) : std::string(kObj));
        std::vector<std::string> rest;
        for (std::size_t i = fixed; i < list.size(); ++i) rest.push_back(converted(list[i], store));
        if (!rest.empty()) {
            head.push_back(std::format("jrt_array_of(sizeof({}), {}, ({}[]){{{}}})", store, rest.size(), store, join_commas(rest)));
        } else {
            head.push_back(std::format("jrt_new_array(sizeof({}), 0)", store));
        }
        return join_commas(head);
    }
    std::vector<std::string> out;
    for (std::size_t i = 0; i < list.size(); ++i) {
        const std::string want = i < params.size() ? params[i] : std::string();
        const ir::Node* a = cpp03::strip_paren(list[i]);
        const Code c = (ir::is<ir::Lambda>(a) || ir::is<ir::MethodRef>(a)) ? closure(a) : expr(list[i]);
        out.push_back(conv(c.code, c.ctype, want));
    }
    return join_commas(out);
}

// A call into the Java library: the runtime's function for the class, the method and the parameters' codes.
Code MethodWriter::lib_call(const ir::Node& n, const ir::Call& k) {
    const ir::Sym& s = *k.sym;
    if (!s.owner) throw Decline("a library call with no owner");
    const std::string& s_owner = *s.owner;
    std::string owner = std::string(jmodel::runtime_name(s_owner).value_or(after_dot(s_owner)));
    if (s_owner == "java.util.Map.Entry") owner = "MapEntry";
    if ((s_owner == "java.util.List" || s_owner == "java.util.Set" || s_owner == "java.util.Map") && s.name == "of") {
        std::vector<std::string> values;
        for (const ir::Node* a : k.args) {
            const Code c = expr(a);
            values.push_back(conv(c.code, c.ctype, kObj));
        }
        return {std::format("jrt_{}_of({}{})", owner, values.size(), comma_list(values)), std::string(kObj)};
    }
    if (s_owner == "java.lang.String" && s.name == "format") {
        if (k.args.empty()) throw Decline("String.format without a format");
        std::vector<std::string> values;
        for (std::size_t i = 1; i < k.args.size(); ++i) {
            const Code c = expr(k.args[i]);
            values.push_back(conv(c.code, c.ctype, kObj));
        }
        const Code format = expr(k.args[0]);
        return {std::format("jrt_String_format({}, {}{})", conv(format.code, format.ctype, kStr), values.size(), comma_list(values)),
                std::string(kStr)};
    }
    // toString, equals and hashCode of any object go through the runtime's dispatch
    if ((s.name == "toString" || s.name == "equals" || s.name == "hashCode") && s.params.size() == (s.name == "equals" ? 1u : 0u) &&
        !s.is_static && k.target != nullptr && !ir::is<ir::TypeName>(k.target) && s_owner != "java.lang.String") {
        const Code recv = receiver(k.target, k.sym);
        std::vector<std::string> as{conv(recv.code, recv.ctype, kObj)};
        for (const ir::Node* a : k.args) {
            const Code c = expr(a);
            as.push_back(conv(c.code, c.ctype, kObj));
        }
        return {std::format("{}({})", object_function(s.name), join_commas(as)),
                s.name == "toString" ? std::string(kStr) : s.name == "equals" ? std::string("bool") : std::string("int32_t")};
    }
    if (s_owner == "java.lang.Comparable" && s.name == "compareTo") {
        if (k.args.empty()) throw Decline("compareTo without an argument");
        const Code a = expr(k.target);
        const Code b = expr(k.args[0]);
        return {std::format("jrt_natural((jobject*){}, {})", a.code, conv(b.code, b.ctype, kObj)), "int32_t"};
    }
    const std::vector<std::string> params = runtime_parameter_types(p_, s.params);
    const std::string name = std::format("jrt_{}_{}_{}", owner, s.name, runtime_codes(s.params));
    const std::string as = args(k.args, params, k.sym, true);
    std::string ret = p_.ct(jmodel::erase(s.ret));
    if (is_pointer_ctype(ret) && ret != kStr && ret != kArr && ret != kSb) ret = std::string(kObj);
    if (s_owner == "java.lang.StringBuilder" && ret == kObj) ret = std::string(kSb);
    std::string code;
    if (s.is_static || ir::is<ir::TypeName>(k.target)) {
        code = std::format("{}({})", name, as);
    } else {
        const Code recv = receiver(k.target, k.sym);
        if (kind_of(type_of(k.target)) == "array" && s.name == "clone") return {std::format("jrt_array_clone({})", recv.code), std::string(kArr)};
        const std::string_view want = s_owner == "java.lang.String" ? kStr : owner == "StringBuilder" ? kSb : owner == "PrintStream" ? std::string_view("void*") : kObj;
        std::vector<std::string> all{conv(recv.code, recv.ctype, want)};
        if (!as.empty()) all.push_back(as);
        code = std::format("{}({})", name, join_commas(all));
    }
    const std::string want = p_.ct(n.type);
    if (ret != want && is_pointer_ctype(want) && ret == kObj) return {conv(code, ret, want), want};   // erased: cast to what javac said
    if (ret != want && is_prim_c(want) && is_pointer_ctype(ret)) return {conv(code, ret, want), want};
    return {code, ret};
}

Code MethodWriter::on(const ir::Node& n, const ir::Call& k) {
    if (!k.sym || k.sym->kind != ir::SymKind::Method) throw Decline("call " + (k.sym ? std::string(ir::to_text(k.sym->kind)) : std::string("unresolved")));
    const ir::Sym& s = *k.sym;
    const auto target = p_.model().method_decl(s);
    const ClassInfo* owner = p_.model().info(s.owner.value_or(""));
    if (!target && owner != nullptr && owner->kind == "enum" && s.name == "values" && k.args.empty())
        return {std::format("{}_values()", owner->cname), std::string(kArr)};
    if (!target) return lib_call(n, k);
    const ClassInfo* mci = target->cls;
    const ir::MethodDecl& m2 = method_of(target->method);
    const Sig sg = p_.sig(*mci, target->method);
    const std::string as = args(k.args, sg.ps, k.sym);
    const std::string name = p_.mname(m2.name.value_or(""), sg.eps);
    const ir::Node* t = k.target;
    const auto with_recv = [&](const std::string& recv) {
        std::vector<std::string> all{std::format("(jobject*){}", recv)};
        if (!as.empty()) all.push_back(as);
        return join_commas(all);
    };
    std::string code;
    if (is_static(m2) || ir::is<ir::TypeName>(t)) {
        code = std::format("{}_{}({})", mci->cname, name, as);
    } else if (ir::is<ir::Super>(t)) {   // super.m(): the superclass's function, not the table's
        code = std::format("{}_{}({})", mci->cname, name, with_recv(this_ref().code));
    } else {
        const Code recv = receiver(t, k.sym);
        if (is_object_method(m2)) {
            code = std::format("{}({})", object_function(*m2.name), with_recv(recv.code));
        } else if (mci->kind == "interface") {
            code = std::format("{}_{}_i({})", mci->cname, name, with_recv(recv.code));   // through the interface table
        } else if (is_private(m2)) {
            code = std::format("{}_{}({})", mci->cname, name, with_recv(recv.code));   // not virtual
        } else {
            code = std::format("{}_{}_v({})", slot_owner(mci, name)->cname, name, with_recv(recv.code));   // through the class's table
        }
    }
    const std::string want = p_.ct(n.type);
    const std::string& r = sg.r;
    if (r != want && is_pointer_ctype(want) && is_pointer_ctype(r) && !is_sub(r, want)) return {conv(code, r, want), want};
    if (r != want && is_prim_c(want) && is_pointer_ctype(r)) return {conv(code, r, want), want};
    return {code, r};
}

const ClassInfo* MethodWriter::slot_owner(const ClassInfo* cls, std::string_view name) {
    const ClassInfo* best = cls;
    std::optional<std::string> q = cls->base;
    while (q && !q->empty()) {
        const ClassInfo* b = p_.model().info(*q);
        if (b == nullptr || b->kind == "interface") break;
        const std::vector<Slot>& slots = p_.slots(*b);
        if (std::any_of(slots.begin(), slots.end(), [&](const Slot& sl) { return sl.name == name; })) best = b;
        q = b->base;
    }
    return best;
}

Code MethodWriter::on(const ir::Node& n, const ir::New& k) {
    if (k.body != nullptr) {   // an anonymous class
        const ClassInfo* anon = p_.model().of_decl(k.body);
        if (anon == nullptr) throw Decline("an anonymous class the model does not know");
        return new_class(anon, n, k);
    }
    if (kind_of(n.type) != "class") throw Decline("new of " + (n.type ? ir::to_text(*n.type) : std::string("None")));
    const ClassInfo* cls = p_.model().info(class_name(n.type));
    if (cls == nullptr) return lib_new(n, k);
    if (cls->local || cls->anon) return new_class(cls, n, k);
    const auto target = k.sym ? p_.model().method_decl(*k.sym) : std::nullopt;
    if (!target && p_.has_ctor(*cls)) throw Decline("constructor of " + cls->cname);
    std::vector<std::string> parameter_types;
    if (target) {
        for (const ir::Node* pn : method_of(target->method).params) parameter_types.push_back(p_.ct(param_of(pn).type));
    }
    const std::string as = args(k.args, parameter_types, k.sym);
    std::vector<std::string> pre;
    if (cls->needs_outer) pre.push_back(k.outer != nullptr ? expr(k.outer).code : outer_of(cls->outer->qname).code);
    const std::string name = target ? text::replace_first(p_.ctor_name(*cls, target->method), "_init", "_new") : cls->cname + "_new";
    if (!as.empty()) pre.push_back(as);
    return {std::format("{}({})", name, join_commas(pre)), cls->cname + "*"};
}

// new of a library class: an exception is made with its message and cause, the others by the runtime's constructor.
Code MethodWriter::lib_new(const ir::Node& n, const ir::New& k) {
    const std::string tname(class_name(n.type));
    const std::string owner(jmodel::runtime_name(tname).value_or(after_dot(tname)));
    if (is_library_throwable(tname)) {
        std::string msg = "NULL", cause = "NULL";
        const ir::TypeP first = k.args.empty() ? ir::TypeP() : type_of(k.args[0]);
        if (k.args.size() == 1 && kind_of(first) == "class" && class_name(first) != "java.lang.String") {   // new E(cause)
            const Code c = expr(k.args[0]);
            cause = conv(c.code, c.ctype, kObj);
        } else if (!k.args.empty()) {
            const Code c = expr(k.args[0]);
            msg = conv(c.code, c.ctype, kStr);
            if (k.args.size() > 1) {
                const Code c2 = expr(k.args[1]);
                cause = conv(c2.code, c2.ctype, kObj);
            }
        }
        return {std::format("jrt_new_throwable(&jrt_{}_class, {}, {})", owner, msg, cause), std::string(kObj)};
    }
    const std::vector<ir::TypeP> params = k.sym ? k.sym->params : std::vector<ir::TypeP>();
    const std::vector<std::string> parameter_types = runtime_parameter_types(p_, params);
    const std::string as = args(k.args, parameter_types, k.sym, true);
    const std::string ret = owner == "StringBuilder" ? std::string(kSb) : owner == "String" ? std::string(kStr) : std::string(kObj);
    return {std::format("jrt_{}_new_{}({})", owner, runtime_codes(params), as), ret};
}

// new of a local or anonymous class: the enclosing instance and the captured locals come first.
Code MethodWriter::new_class(const ClassInfo* cls, const ir::Node&, const ir::New& k) {
    std::vector<std::string> pre;
    if (cls->needs_outer) pre.push_back(this_ref().code);
    for (const jmodel::Capture& c : cls->captures) {
        ir::Node* name = ir::make(scratch(), ir::Name{.sym = ir::make_sym(ir::SymKind::Local, c.name)}, c.type);
        pre.push_back(on(*name, *ir::get<ir::Name>(name)).code);
    }
    std::vector<std::string> parameter_types;
    std::string name;
    if (p_.has_ctor(*cls)) {
        const auto target = k.sym ? p_.model().method_decl(*k.sym) : std::nullopt;
        if (target) {
            for (const ir::Node* pn : method_of(target->method).params) parameter_types.push_back(p_.ct(param_of(pn).type));
        }
        name = text::replace_first(p_.ctor_name(*cls, target ? target->method : nullptr), "_init", "_new");
    } else {   // an anonymous class passes its arguments to the super's constructor
        const ClassInfo* base = cls->base ? p_.model().info(*cls->base) : nullptr;
        if (base != nullptr && base->kind != "interface" && k.sym) {
            if (const auto target = p_.model().method_decl(*k.sym)) {
                for (const ir::Node* pn : method_of(target->method).params) parameter_types.push_back(p_.ct(param_of(pn).type));
            }
        }
        name = cls->cname + "_new";
    }
    const std::string as = args(k.args, parameter_types, k.sym);
    if (!as.empty()) pre.push_back(as);
    return {std::format("{}({})", name, join_commas(pre)), cls->cname + "*"};
}

Code MethodWriter::on(const ir::Node& n, const ir::NewArray& k) {
    ir::TypeP type = n.type;
    if (!type) {
        if (const auto hint = array_types_.find(&n); hint != array_types_.end()) type = hint->second;   // a nested initialiser's own type
    }
    if (kind_of(type) != "array") throw Decline("an array creation with no array type");
    const ir::TypeP elem = elem_of(type);
    const std::string et = p_.ct(elem);
    const std::string store = array_store(et);
    if (!k.init.empty() || k.dims.empty()) {   // `new T[] { ... }` (also an empty one)
        std::vector<std::string> items;
        for (const ir::Node* x : k.init) {
            if (ir::is<ir::NewArray>(x) && !x->type) array_types_[x] = elem;
            const Code c = expr(x);
            items.push_back(conv(c.code, c.ctype, store));
        }
        if (items.empty()) return {std::format("jrt_new_array(sizeof({}), 0)", store), std::string(kArr)};
        return {std::format("jrt_array_of(sizeof({}), {}, ({}[]){{{}}})", store, items.size(), store, join_commas(items)), std::string(kArr)};
    }
    std::vector<std::string> dims;
    for (const ir::Node* d : k.dims) {
        const Code c = expr(d);
        dims.push_back(conv(c.code, c.ctype, "int32_t"));
    }
    if (dims.size() == 1) return {std::format("jrt_new_array(sizeof({}), {})", store, dims[0]), std::string(kArr)};
    if (dims.size() == 2) {
        const std::string inner = array_store(p_.ct(elem_of(elem)));
        return {std::format("jrt_new_array2(sizeof({}), {}, {})", inner, dims[0], dims[1]), std::string(kArr)};
    }
    throw Decline(std::format("array of {} dimensions", dims.size()));
}

}  // namespace cb::translate::backends::c_java::detail
