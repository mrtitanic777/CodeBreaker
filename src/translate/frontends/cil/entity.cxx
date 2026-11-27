// translate/frontends/cil/entity.cpp -- ClrEntity and the lane's literal texts (see cil.hpp).
#include <charconv>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "translate/frontends/cil/cil.hpp"
#include "translate/frontends/cil/cil_names.hpp"

namespace cb::translate::frontends::cil {

std::string ClrEntity::key() const {
    namespace ts = ts_ns;
    switch (kind_) {
        case ir::EntityKind::Type: return "T:" + static_cast<const ts::IType*>(object_)->reflection_name();
        case ir::EntityKind::Method: return "M:" + static_cast<const ts::IMethod*>(object_)->to_string();
        case ir::EntityKind::Field: return "F:" + static_cast<const ts::IField*>(object_)->to_string();
        case ir::EntityKind::Property: return "P:" + static_cast<const ts::IMember*>(object_)->to_string();
        case ir::EntityKind::Event: return "E:" + static_cast<const ts::IMember*>(object_)->to_string();
        case ir::EntityKind::Parameter: return "A:" + static_cast<const ts::IParameter*>(object_)->to_string();
        case ir::EntityKind::Module: break;
    }
    return "?";
}

namespace cil_entity {

const ClrEntity* clr_entity_of(const ir::EntityP& e) {
    return e != nullptr && e->family() == "clr" ? static_cast<const ClrEntity*>(e.get()) : nullptr;
}

}  // namespace cil_entity

const ts_ns::IType* clr_type(const ir::EntityP& entity) {
    const ClrEntity* c = cil_entity::clr_entity_of(entity);
    return c != nullptr && c->kind() == ir::EntityKind::Type ? static_cast<const ts_ns::IType*>(c->object()) : nullptr;
}
const ts_ns::IType* clr_type(const ir::TypeP& type) { return type ? clr_type(type->entity) : nullptr; }
const ts_ns::IMethod* clr_method(const ir::EntityP& entity) {
    const ClrEntity* c = cil_entity::clr_entity_of(entity);
    return c != nullptr && c->kind() == ir::EntityKind::Method ? static_cast<const ts_ns::IMethod*>(c->object()) : nullptr;
}
const ts_ns::IMethod* clr_method(const ir::SymP& sym) { return sym ? clr_method(sym->entity) : nullptr; }
const ts_ns::IField* clr_field(const ir::EntityP& entity) {
    const ClrEntity* c = cil_entity::clr_entity_of(entity);
    return c != nullptr && c->kind() == ir::EntityKind::Field ? static_cast<const ts_ns::IField*>(c->object()) : nullptr;
}
const ts_ns::IField* clr_field(const ir::SymP& sym) { return sym ? clr_field(sym->entity) : nullptr; }
const ts_ns::IMember* clr_member(const ir::SymP& sym) {
    const ClrEntity* c = sym ? cil_entity::clr_entity_of(sym->entity) : nullptr;
    if (c == nullptr) return nullptr;
    switch (c->kind()) {
        case ir::EntityKind::Method: return static_cast<const ts_ns::IMethod*>(c->object());
        case ir::EntityKind::Field: return static_cast<const ts_ns::IField*>(c->object());
        case ir::EntityKind::Property:
        case ir::EntityKind::Event: return static_cast<const ts_ns::IMember*>(c->object());
        default: return nullptr;
    }
}
const ts_ns::IParameter* clr_parameter(const ir::SymP& sym) {
    const ClrEntity* c = sym ? cil_entity::clr_entity_of(sym->entity) : nullptr;
    return c != nullptr && c->kind() == ir::EntityKind::Parameter ? static_cast<const ts_ns::IParameter*>(c->object()) : nullptr;
}

std::string decimal_text(const dotnet::decompiler::util::Decimal& d) {
    std::string text = d.to_string();
    if (d.negative && d.is_zero()) text.insert(0, 1, '-');
    return text;
}

std::optional<dotnet::decompiler::util::Decimal> decimal_from(std::string_view text) {
    dotnet::decompiler::util::Decimal d;
    if (!text.empty() && text.front() == '-') {
        d.negative = true;
        text.remove_prefix(1);
    }
    if (text.empty()) return std::nullopt;
    std::uint32_t words[3] = {0, 0, 0};   // lo, mid, hi
    bool point = false;
    int scale = 0;
    for (char ch : text) {
        if (ch == '.') {
            if (point) return std::nullopt;
            point = true;
            continue;
        }
        if (ch < '0' || ch > '9') return std::nullopt;
        if (point) ++scale;
        std::uint64_t carry = static_cast<std::uint64_t>(ch - '0');
        for (std::uint32_t& w : words) {
            const std::uint64_t cur = static_cast<std::uint64_t>(w) * 10 + carry;
            w = static_cast<std::uint32_t>(cur);
            carry = cur >> 32;
        }
        if (carry != 0) return std::nullopt;
    }
    if (scale > 28) return std::nullopt;
    d.lo = words[0];
    d.mid = words[1];
    d.hi = words[2];
    d.scale = static_cast<std::uint8_t>(scale);
    return d;
}

}  // namespace cb::translate::frontends::cil
