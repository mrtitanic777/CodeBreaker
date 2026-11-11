#include "cb/java/ir.hpp"

#include <algorithm>
#include <functional>

namespace cb::java::ir {

std::string_view to_string(Access a) {
    switch (a) {
        case Access::Public: return "public";
        case Access::Protected: return "protected";
        case Access::Package: return "package";
        case Access::Private: return "private";
    }
    return "package";
}

std::optional<Access> parse_access(std::string_view text) {
    for (Access a : {Access::Public, Access::Protected, Access::Package, Access::Private}) {
        if (text == to_string(a)) return a;
    }
    return std::nullopt;
}

// ---- JType --------------------------------------------------------------------------------------------

JType JType::element() const {
    JType out = *this;
    if (out.array_dims > 0) --out.array_dims;
    return out;
}

std::string JType::to_string() const {
    std::string s = name;
    if (!args.empty()) {
        s += '<';
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i != 0) s += ", ";
            s += args[i].to_string();
        }
        s += '>';
    }
    for (std::uint32_t i = 0; i < array_dims; ++i) s += "[]";
    return s;
}

std::size_t JType::hash() const {
    std::size_t h = std::hash<std::string>()(name);
    h = h * 1000003u ^ array_dims;
    for (const JType& a : args) h = h * 1000003u ^ a.hash();
    return h;
}

// ---- Method -------------------------------------------------------------------------------------------

std::vector<JType> Method::param_types() const {
    std::vector<JType> out;
    out.reserve(params.size());
    for (const Param& p : params) out.push_back(p.type);
    return out;
}

Signature Method::signature() const {
    Signature s{name, {}};
    s.param_types.reserve(params.size());
    for (const Param& p : params) s.param_types.push_back(p.type.to_string());
    return s;
}

// ---- JClass -------------------------------------------------------------------------------------------

const Field* JClass::field(std::string_view field_name) const {
    auto it = std::find_if(fields.begin(), fields.end(), [&](const Field& f) { return f.name == field_name; });
    return it == fields.end() ? nullptr : &*it;
}

std::vector<const Field*> JClass::instance_fields() const {
    std::vector<const Field*> out;
    for (const Field& f : fields) {
        if (!f.is_static) out.push_back(&f);
    }
    return out;
}

std::vector<const Field*> JClass::static_fields() const {
    std::vector<const Field*> out;
    for (const Field& f : fields) {
        if (f.is_static) out.push_back(&f);
    }
    return out;
}

std::vector<const Method*> JClass::constructors() const {
    std::vector<const Method*> out;
    for (const Method& m : methods) {
        if (m.is_ctor) out.push_back(&m);
    }
    return out;
}

const Method* JClass::method(std::string_view method_name) const {
    auto it = std::find_if(methods.begin(), methods.end(), [&](const Method& m) { return m.name == method_name; });
    return it == methods.end() ? nullptr : &*it;
}

bool JClass::owns_heap_field() const {
    return std::any_of(fields.begin(), fields.end(), [](const Field& f) {
        return f.initializer && f.initializer->find("new ") != std::string::npos;
    });
}

bool JClass::is_typed_enum() const {
    if (!is_enum) return false;
    return std::any_of(enum_consts.begin(), enum_consts.end(), [](const EnumConst& ec) { return !ec.args.empty(); }) ||
           std::any_of(fields.begin(), fields.end(), [](const Field& f) { return !f.is_static; }) ||
           std::any_of(methods.begin(), methods.end(), [](const Method& m) { return !m.is_ctor; });
}

}  // namespace cb::java::ir
