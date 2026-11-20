// translate/backends/java_libc.cpp -- a C library call as Java (see the header).
#include "translate/backends/java_libc.hpp"

#include "core/text.hpp"

namespace cb::translate::backends::java_libc {

namespace {

using jb::CKind;
using jb::Ex;

// The string literal a char-pointer argument is, where the front end made it decay (a Cast of the Lit); null for any other.
const ir::Node* jlc_string_literal(ir::Node* a) {
    const ir::Cast* cast = ir::get<ir::Cast>(jb::strip(a));
    if (cast == nullptr || cast->x == nullptr) return nullptr;
    ir::Node* x = jb::strip(cast->x);
    const ir::Lit* lit = ir::get<ir::Lit>(x);
    return lit != nullptr && std::holds_alternative<std::string>(lit->value) ? x : nullptr;
}

bool jlc_char_ptr(const ir::TypeP& type) {
    if (jb::kind_of(type) != CKind::Ptr) return false;
    const CKind k = jb::kind_of(jb::pointee(type));
    return jb::pointee(type) && (k == CKind::I8 || k == CKind::U8 || k == CKind::U16);
}

// A function pointer given to the library (qsort's comparator): a Java functional value.
std::string jlc_callback(jb::FnWriter& w, ir::Node* a, const ir::TypeP& pointer_type) {
    ir::Node* x = jb::strip(a);
    if (const ir::Cast* cast = ir::get<ir::Cast>(x)) x = jb::strip(cast->x);
    if (const ir::AddressOf* address = ir::get<ir::AddressOf>(x); address != nullptr && address->sym != nullptr &&
                                                                  w.p.functions.contains(address->sym->name)) {
        return w.p.name + "::" + jb::jname(address->sym->name);
    }
    const jb::Sig sig = w.p.fn_sig_of_type(pointer_type);
    const std::string iface = w.p.iface(sig);
    static constexpr std::string_view kNames[] = {"a", "b", "c", "d"};
    std::string names;   // the lambda's parameters: as many as the function takes, up to four
    for (std::size_t i = 0; i < sig.params.size() && i < std::size(kNames); ++i) names += (i != 0 ? ", " : "") + std::string(kNames[i]);
    const std::string f = w.value(a).code;
    return "(" + names + ") -> ((" + iface + ") Fn.at(" + f + ")).call(" + names + ")";
}

}  // namespace

Ex LibC::call(jb::FnWriter& w, ir::Node& call, std::string_view name, std::span<ir::Node* const> args) const {
    if (name == "__acrt_iob_func") {   // MSVC's stdin/stdout/stderr
        return {"stream(" + w.value(args[0]).code + ")", jb::kPost, "int", false};
    }
    const std::optional<cb::java::apimap::Mapping> mapping = registry_.resolve(name, "c", "java");
    if (!mapping) throw jb::Refused("library function " + std::string(name) + ": not in the C runtime for Java (api_maps/libc.json)");
    if (mapping->kind == cb::java::apimap::Kind::Unsupported) {
        throw jb::Refused("library function " + std::string(name) + ": " + (mapping->note.empty() ? std::string("not carried") : mapping->note));
    }
    std::string target = mapping->target;
    constexpr std::string_view kClass = "crt.LibC.";
    if (target.starts_with(kClass)) target.erase(0, kClass.size());   // (LibC is statically imported: calls read as they did in C)
    const ir::Call* c = ir::get<ir::Call>(&call);
    const std::vector<ir::TypeP> no_params;
    const std::vector<ir::TypeP>& params = c != nullptr && c->sym != nullptr ? c->sym->params : no_params;
    std::vector<std::string> out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        ir::Node* a = args[i];
        const ir::TypeP* pt = i < params.size() ? &params[i] : nullptr;   // none: past the prototype's last parameter (variadic)
        if (const ir::Node* lit = jlc_string_literal(a); lit != nullptr && (pt == nullptr || jlc_char_ptr(*pt))) {
            out.push_back(jb::string_lit(*lit));
        } else if (pt != nullptr && jb::kind_of(*pt) == CKind::FnPtr) {
            out.push_back(jlc_callback(w, a, *pt));
        } else if (pt == nullptr) {   // a variadic argument: as C passed it
            out.push_back(w.as_value(w.value(a), a->type).code);
        } else {
            out.push_back(w.conv(w.value(a), a->type, *pt).code);
        }
    }
    const std::string jt = call.type ? w.p.jt(call.type) : "void";
    return {target + "(" + text::join(out, ", ") + ")", jb::kPost, jt, false};
}

}  // namespace cb::translate::backends::java_libc
