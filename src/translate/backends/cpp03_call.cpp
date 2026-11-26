// translate/backends/cpp03_call.cpp -- the C++03 back end's calls: the program's own methods, the runtime's, java.lang.Object's, varargs
// (see cpp03.hpp).

#include <format>

#include "core/text.hpp"
#include "java/emit/ownership.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace ownership = cb::java::emit::ownership;
namespace typemap = cb::java::typemap;
using detail::class_type;

Code Writer::write_call(const ir::Node& e, const ir::Call& c) {
    const ir::SymP& s = c.sym;
    if (!s || s->kind != ir::SymKind::Method)
        throw Decline("call " + (s ? std::string(ir::to_text(s->kind)) : std::string("unresolved")));
    const std::optional<std::string> owner_o = s->owner_simple();
    const std::string owner = owner_o ? *owner_o : "None";
    const std::vector<const ir::Node*> args = unmarked(c.args);   // the decompiler's EmptyN bridge arguments
    const bool target_value = c.target != nullptr && !ir::is<ir::TypeName>(c.target);
    if (typemap::boxed_value_types().contains(owner) && s->name.ends_with("Value") && s->params.empty() && target_value) {
        const Code recv = expr(c.target);   // Integer.intValue(): a boxed type is a primitive here, unless it is still in its JObject box
        if (!recv.ctype.starts_with("JObject")) {
            const std::string ct = ctype(e.type);
            return {"((" + ct + ")(" + recv.code + "))", ct};
        }
    }
    if (typemap::boxed_value_types().contains(owner) && s->name == "equals" && args.size() == 1 && target_value) {
        const Code recv = expr(c.target);   // Integer.equals(Integer): the primitives' values
        if (!recv.ctype.starts_with("JObject")) return {"(" + recv.code + " == " + expr(args[0]).code + ")", "bool"};
    }
    if (s->name == "ordinal" && s->params.empty() && c.target != nullptr) {
        const Code recv = expr(c.target);
        if (recv.ctype == "int") return {"(" + recv.code + ")", "int"};   // a constant-only enum is its ordinal
    }
    if (s->owner && *s->owner == "java.lang.Object") return object_method(c);
    if (s->name == "values" && args.empty() && ir::is<ir::TypeName>(c.target) && r_.data().typed_enums.contains(owner))
        return {owner + "::values()", ctype(e.type)};   // the enum's own table
    std::string arg_text;
    if (s->varargs) {
        arg_text = varargs(c, args, owner);
    } else {
        std::vector<std::string> parts;
        for (const ir::Node* a : args) parts.push_back(arg(a));
        arg_text = detail::join_commas(parts);
    }
    const bool game = ctx_.is_game(s->owner);
    const jir::Method* parsed = game ? ctx_.parsed_method(owner, s->name, s->params) : nullptr;
    std::string ret_t;
    std::string method_name;
    if (game) {
        if (parsed == nullptr) throw Decline("method " + owner + "." + s->name + " not found");
        ret_t = ownership::return_cpp_type(*parsed, r_);
        method_name = r_.renamed_method(owner, s->name);
    } else {
        ret_t = library_return(owner, s->name, s->ret);
        method_name = name(s->name);   // File.delete() -> delete_(), as the runtime has it
    }
    const ir::Node* t = c.target;
    if (t == nullptr) return {"this->" + method_name + "(" + arg_text + ")", ret_t};
    if (ir::is<ir::TypeName>(t)) {
        if (!game) {
            if (s->owner && *s->owner == "java.lang.Math") {
                const auto it = detail::math_functions().find(s->name);
                if (it == detail::math_functions().end()) throw Decline("Math." + s->name);
                return {it->second + "(" + arg_text + ")", ctype(e.type)};
            }
            const ir::TypeP tt = detail::type_of(t);
            const std::string holder = (tt && tt->kind() == "class") ? tt->simple() : owner;
            if (!typemap::runtime_headers().contains(holder) && !typemap::javart_statics().contains(holder))
                throw Decline("library static call " + holder + "." + s->name);
            return {holder + "::" + method_name + "(" + arg_text + ")", ret_t};   // as written: ARBVertexBufferObject::
        }
        return {owner + "::" + method_name + "(" + arg_text + ")", ret_t};
    }
    if (ir::is<ir::Super>(t)) {
        if (!parent_ || parent_->empty()) throw Decline("super call without a parent");
        return {*parent_ + "::" + method_name + "(" + arg_text + ")", ret_t};
    }
    if (ir::is<ir::This>(t)) return {"this->" + method_name + "(" + arg_text + ")", ret_t};
    Code recv = expr(t);
    if (recv.ctype == "const char*") {
        recv.code = "JavaString(" + recv.code + ")";
        recv.ctype = "JavaString";
    }
    if (recv.code.starts_with("new ")) recv.code = "(" + recv.code + ")";   // new File(p).toURI(): `new` binds looser than ->
    return {checked(recv.code, recv.ctype) + (is_pointer(recv.ctype) ? "->" : ".") + method_name + "(" + arg_text + ")", ret_t};
}

Code Writer::object_method(const ir::Call& c) {
    const ir::Sym& s = *c.sym;
    const ir::Node* t = c.target;
    Code recv;
    if (t == nullptr || ir::is<ir::This>(t)) recv = {"this", cname_ + "*"};
    else recv = expr(t);
    const std::vector<const ir::Node*> args = unmarked(c.args);
    if (s.name == "getClass" && args.empty()) {
        if (recv.ctype.starts_with("JObject")) {   // a box: the class of the object it holds
            const char* access = is_pointer(recv.ctype) ? "->" : ".";
            return {std::format("(({}){}ti ? j2xClassOf(*({}){}ti) : (Class*)NULL)", recv.code, access, recv.code, access), "Class*"};
        }
        if (!is_pointer(recv.ctype)) throw Decline("getClass on " + recv.ctype);
        return {"j2xClassOf(typeid(*" + checked(recv.code, recv.ctype) + "))", "Class*"};
    }
    if (s.name == "equals" && args.size() == 1 && is_pointer(recv.ctype)) {
        const Code other = expr(args[0]);   // Object.equals is identity
        if ((is_pointer(other.ctype) && other.ctype == recv.ctype) || other.code == "NULL")
            return {"(" + recv.code + " == " + other.code + ")", "bool"};
        return {std::format("j2xIdentEq({}, {})", recv.code, other.code), "bool"};
    }
    if (s.name == "equals" && args.size() == 1 && recv.ctype.starts_with("JObject") && !is_pointer(recv.ctype))
        return {"(" + recv.code + ").equals(" + expr(args[0]).code + ")", "bool"};   // the box's own equals
    if (s.name == "toString" && args.empty() && is_pointer(recv.ctype)) return {checked(recv.code, recv.ctype) + "->toString()", "JavaString"};
    throw Decline("Object method " + s.name);
}

namespace {
namespace call_impl {

// The trailing arguments packed into the array a varargs method takes: `fixed` leading ones as they are, the rest as `elem[]`.
std::string pack_varargs(std::vector<std::string> codes, std::size_t fixed, const std::string& elem) {
    std::vector<std::string> rest;
    if (fixed < codes.size()) rest.assign(codes.begin() + static_cast<std::ptrdiff_t>(fixed), codes.end());
    codes.resize(std::min(fixed, codes.size()));
    codes.push_back(!rest.empty() ? std::format("(JArrayInit<{} >(), {})", elem, detail::join_commas(rest)) : std::format("JArray<{} >()", elem));
    return detail::join_commas(codes);
}

}  // namespace call_impl
}  // namespace

std::string Writer::varargs(const ir::Call& c, std::span<const ir::Node* const> args, std::string_view owner) {
    const ir::Sym& s = *c.sym;
    if (!ctx_.is_game(s.owner)) return library_varargs(c, args);
    const jir::Method* parsed = ctx_.parsed_method(owner, s.name, s.params);
    if (parsed == nullptr) throw Decline("method " + std::string(owner) + "." + s.name + " not found");
    const auto declared = split_params(ownership::params_str(*parsed, r_));
    if (declared.empty()) throw Decline("varargs method without parameters");
    std::vector<std::string> codes;
    for (const ir::Node* a : args) codes.push_back(arg(a));
    if (args.size() == s.params.size()) {
        const ir::TypeP lt = detail::type_of(args.back());
        if (lt && (lt->kind() == "array" || lt->kind() == "null")) return detail::join_commas(codes);   // the array itself was passed
    }
    std::string last = text::replace_all(declared.back().first, "const ", "");
    while (last.ends_with('&')) last.pop_back();
    return call_impl::pack_varargs(std::move(codes), s.params.size() - 1, array_element(text::trim(last)));
}

std::string Writer::library_varargs(const ir::Call& c, std::span<const ir::Node* const> args) {
    const ir::Sym& s = *c.sym;
    const ir::TypeP last = s.params.empty() ? ir::TypeP() : s.params.back();
    if (!last || last->kind() != "array")
        throw Decline("library varargs " + s.owner_simple().value_or("None") + "." + s.name);
    const std::string elem = array_element(ctype(last));
    std::vector<std::string> codes;
    for (const ir::Node* a : args) codes.push_back(arg(a));
    if (args.size() == s.params.size()) {
        const ir::TypeP lt = detail::type_of(args.back());
        if (lt && (lt->kind() == "array" || lt->kind() == "null")) return detail::join_commas(codes);
    }
    return call_impl::pack_varargs(std::move(codes), s.params.size() - 1, elem);
}

std::string Writer::library_field(std::string_view owner, std::string_view name, const ir::TypeP& type) const {
    if (owner == "System" && (name == "out" || name == "err")) return "System::_PrintStream";   // a value in the runtime (JavaRT.h)
    return ctype(type);
}

std::string Writer::library_return(std::string_view owner, std::string_view name, const ir::TypeP& ret_type) const {
    std::string base = r_.base_cpp(owner);
    base.erase(std::min(base.size(), base.find('<')));
    // an NIO buffer's position(int) / limit(int) answers the buffer (Java's return type says which overload): the runtime header's
    // first declaration is the getter's int, and `b.position(0).limit(n)` came out as a call on an int
    if (class_type(ret_type) != nullptr && ret_type->simple().ends_with("Buffer") && owner.ends_with("Buffer")) return std::string(owner) + "&";
    if (const auto declared = ctx_.runtime_return(owner, name); declared && !declared->empty()) return *declared;   // what the runtime's own header declares, first
    const auto& stubs = ctx_.runtime.stub_members();
    const std::map<std::string, cb::java::runtime::StubMembers, std::less<>>::const_iterator end = stubs.end();
    auto record = stubs.find(base);
    if (record == end) record = stubs.find(owner);
    std::set<std::string, std::less<>> seen;
    while (record != end && seen.insert(record->first).second) {
        const auto method = record->second.methods.find(name);
        if (method != record->second.methods.end() && !method->second.empty()) return method->second == "std::string" ? "JavaString" : method->second;
        record = record->second.parent ? stubs.find(*record->second.parent) : end;
    }
    if (name == "iterator") return "Iterator*";
    if (class_type(ret_type) != nullptr && ret_type->simple() == owner && owner.ends_with("Buffer"))
        return std::string(owner) + "&";   // NIO buffers chain by reference (put().put())
    if (!ret_type || ret_type->kind() == "void") return "void";
    return ctype(ret_type, cb::java::types::Use::Return);
}

}  // namespace cb::translate::backends::cpp03
