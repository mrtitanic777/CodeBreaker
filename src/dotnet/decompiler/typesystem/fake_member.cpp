// dotnet/decompiler/typesystem/fake_member.cpp -- the fake members.
#include "dotnet/decompiler/typesystem/fake_member.hpp"

#include "dotnet/decompiler/typesystem/specialized_member.hpp"

namespace cb::dotnet::decompiler::typesystem {

const IModule* FakeMember::parent_module() const {
    const ITypeDefinition* def = declaring_type_value != nullptr ? declaring_type_value->get_definition() : nullptr;
    return def != nullptr ? def->parent_module() : nullptr;
}

std::string FakeMember::full_name() const { return declaring_type_value != nullptr ? declaring_type_value->full_name() + "." + name_value : name_value; }

std::string FakeMember::reflection_name() const {
    return declaring_type_value != nullptr ? declaring_type_value->reflection_name() + "." + name_value : name_value;
}

int FakeMember::hash_code() const {
    const auto v = reinterpret_cast<std::uintptr_t>(this);
    return static_cast<int>(static_cast<std::uint32_t>(v ^ (v >> 32)));
}

const IMember* FakeField::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedField::create(this, substitution); }

const std::vector<const IType*>& FakeMethod::type_arguments() const {
    std::call_once(type_arguments_once_, [&] { type_arguments_.assign(type_parameters_value.begin(), type_parameters_value.end()); });
    return type_arguments_;
}

const IMember* FakeMethod::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedMethod::create(this, substitution); }
const IMethod* FakeMethod::specialize_method(const TypeParameterSubstitution& substitution) const { return SpecializedMethod::create(this, substitution); }

const IMember* FakeProperty::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedProperty::create(this, substitution); }

std::string FakeProperty::to_string() const {
    std::string s = "FakeProperty " + return_type_value->to_string() + " " + declaring_type_value->name() + "." + name_value;
    if (!parameters_value.empty()) {
        s += "[";
        for (std::size_t i = 0; i < parameters_value.size(); ++i) {
            if (i > 0) s += ", ";
            s += parameters_value[i]->to_string();
        }
        s += "]";
    }
    return s + " { " + (can_get() ? "get; " : "") + (can_set() ? "set; " : "") + "}";
}

const IMember* FakeEvent::specialize(const TypeParameterSubstitution& substitution) const { return SpecializedEvent::create(this, substitution); }

const IMethod* create_dummy_constructor(const ICompilation& compilation, const IType* declaring_type, Accessibility accessibility) {
    auto ctor = std::make_unique<FakeMethod>(compilation, SymbolKind::Constructor);
    ctor->declaring_type_value = declaring_type;
    ctor->name_value = ".ctor";
    ctor->return_type_value = compilation.find_type(KnownTypeCode::Void);
    ctor->accessibility_value = accessibility;
    return compilation.arena().keep(std::move(ctor));
}

}  // namespace cb::dotnet::decompiler::typesystem
