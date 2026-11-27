// dotnet/decompiler/typesystem/extension_info.cpp -- see extension_info.hpp.
#include "dotnet/decompiler/typesystem/extension_info.hpp"

#include <cassert>
#include <string_view>

#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

namespace {

bool starts_with(const std::string& text, std::string_view prefix) { return text.compare(0, prefix.size(), prefix) == 0; }

std::vector<const IType*> as_types(const std::vector<const ITypeParameter*>& type_parameters) {
    return std::vector<const IType*>(type_parameters.begin(), type_parameters.end());
}

}  // namespace

void ExtensionInfo::MemberInfoMap::set(const IMember* key, const ExtensionMemberInfo& info) {
    auto range = index_.equal_range(key->hash_code());
    for (auto it = range.first; it != range.second; ++it) {
        if (member_equals(items_[it->second].first, key)) {
            items_[it->second].second = info;
            return;
        }
    }
    index_.emplace(key->hash_code(), items_.size());
    items_.emplace_back(key, info);
}

const ExtensionMemberInfo* ExtensionInfo::MemberInfoMap::find(const IMember* key) const {
    auto range = index_.equal_range(key->hash_code());
    for (auto it = range.first; it != range.second; ++it)
        if (member_equals(items_[it->second].first, key)) return &items_[it->second].second;
    return nullptr;
}

ExtensionInfo::ExtensionInfo(const MetadataModule& module, const ITypeDefinition* extension_container) : container_(extension_container) {
    for (const ITypeDefinition* nested_type : extension_container->nested_types()) {
        if (try_encoding_v1(module, nested_type)) continue;
        try_encoding_v2(module, nested_type);
    }
}

ExtensionInfo::~ExtensionInfo() = default;

bool ExtensionInfo::try_encoding_v1(const MetadataModule& module, const ITypeDefinition* ext_group) {
    if (!(ext_group->kind() == TypeKind::Class && ext_group->is_sealed() && starts_with(ext_group->name(), "<>E__"))) return false;

    const srm::MetadataReader& metadata = module.metadata();
    srm::TypeDefinition td = metadata.get_type_definition(srm::TypeDefinitionHandle(ext_group->metadata_token()));
    const IMethod* marker = nullptr;
    bool has_multiple_markers = false;
    std::vector<const IMethod*> extension_methods;
    std::vector<const ITypeParameter*> extension_group_type_parameters = ext_group->type_parameters();

    // For easier access to accessors the metadata is read directly.
    for (srm::MethodDefinitionHandle h : td.get_methods()) {
        const IMethod* method = module.get_definition(h);

        if (method->symbol_kind() == SymbolKind::Constructor) continue;
        if (method->name() == "<Extension>$" && method->is_static() && method->parameters().size() == 1) {
            if (marker == nullptr)
                marker = method;
            else
                has_multiple_markers = true;
            continue;
        }

        extension_methods.push_back(method);
    }

    if (marker == nullptr || has_multiple_markers) return false;

    extension_groups_.push_back(ExtensionGroupInfo{marker, extension_group_type_parameters});

    collect_implementation_methods(marker, extension_group_type_parameters, extension_methods);
    return true;
}

bool ExtensionInfo::try_encoding_v2(const MetadataModule& module, const ITypeDefinition* extension_groups_container) {
    // there exists one nested type per extension target type
    if (!(extension_groups_container->kind() == TypeKind::Class && extension_groups_container->is_sealed() &&
          starts_with(extension_groups_container->name(), "<G>$")))
        return false;

    const srm::MetadataReader& metadata = module.metadata();
    // if there are multiple extension-blocks with the same target type, but different names for the extension parameter, there is a
    // separate markerType, so there are multiple marker types per target type
    for (const ITypeDefinition* marker_type : extension_groups_container->nested_types()) {
        if (!(starts_with(marker_type->name(), "<M>$") && marker_type->is_static())) continue;
        const IMethod* marker = nullptr;
        int marker_count = 0;
        for (const IMethod* m : marker_type->methods()) {
            if (m->name() == "<Extension>$" && m->is_static() && m->parameters().size() == 1) {
                marker = m;
                marker_count++;
            }
        }
        // SingleOrDefault: more than one candidate throws upstream; treated as no marker here.
        if (marker_count != 1) continue;

        std::vector<const ITypeParameter*> extension_group_type_parameters(static_cast<std::size_t>(marker_type->type_parameter_count()));
        std::vector<srm::GenericParameterHandle> marker_type_type_parameters =
            metadata.get_type_definition(srm::TypeDefinitionHandle(marker_type->metadata_token())).get_generic_parameters();

        for (std::size_t i = 0; i < marker_type_type_parameters.size() && i < extension_group_type_parameters.size(); i++) {
            srm::GenericParameter tp = metadata.get_generic_parameter(marker_type_type_parameters[i]);
            owned_type_parameters_.push_back(std::make_unique<MetadataTypeParameter>(
                module, marker_type, static_cast<int>(i), std::string(metadata.get_string(tp.name())), marker_type_type_parameters[i],
                tp.attributes()));
            extension_group_type_parameters[i] = owned_type_parameters_.back().get();
        }

        extension_groups_.push_back(ExtensionGroupInfo{marker, extension_group_type_parameters});

        srm::TypeDefinition td = metadata.get_type_definition(srm::TypeDefinitionHandle(extension_groups_container->metadata_token()));
        std::vector<const IMethod*> extension_methods;

        // For easier access to accessors the metadata is read directly.
        for (srm::MethodDefinitionHandle h : td.get_methods()) {
            const IMethod* method = module.get_definition(h);

            if (method->symbol_kind() == SymbolKind::Constructor) continue;

            const IAttribute* attribute = method->get_attribute(KnownAttribute::ExtensionMarker);
            if (attribute == nullptr) continue;

            // `FixedArguments[0].Value?.ToString() != markerType.Name`: only a string can be the marker type's name.
            const auto* name = attribute->fixed_arguments().empty() ? nullptr : std::get_if<std::string>(&attribute->fixed_arguments()[0].value);
            if (name == nullptr || *name != marker_type->name()) continue;

            extension_methods.push_back(method);
        }

        collect_implementation_methods(marker, extension_group_type_parameters, extension_methods);
    }

    return true;
}

void ExtensionInfo::collect_implementation_methods(const IMethod* marker, const std::vector<const ITypeParameter*>& type_parameters,
                                                   const std::vector<const IMethod*>& extension_methods) {
    const ITypeDefinition* marker_type = marker->declaring_type_definition();
    std::vector<std::pair<const IMethod*, const IMethod*>> implementations;

    for (const IMethod* extension : extension_methods) {
        std::size_t expected_type_parameter_count = extension->type_parameters().size() + static_cast<std::size_t>(marker_type->type_parameter_count());
        bool has_instance = !extension->is_static();
        std::size_t parameter_offset = has_instance ? 1 : 0;
        std::size_t expected_parameter_count = extension->parameters().size() + parameter_offset;
        std::vector<const IType*> method_type_arguments = marker_type->type_arguments();
        for (const IType* argument : extension->type_arguments()) method_type_arguments.push_back(argument);
        TypeParameterSubstitution subst(std::vector<const IType*>{}, std::move(method_type_arguments));

        auto is_matching_implementation = [&](const IMethod* impl) {
            if (!impl->is_static()) return false;
            if (extension->name() != impl->name()) return false;
            if (expected_type_parameter_count != impl->type_parameters().size()) return false;
            if (expected_parameter_count != impl->parameters().size()) return false;
            if (has_instance) {
                const IType* ti = impl->parameters()[0]->type()->accept_visitor(subst);
                const IType* tm = marker->parameters().front()->type();
                if (!NormalizeTypeVisitor::type_erasure().equivalent_types(ti, tm)) return false;
            }
            for (std::size_t i = 0; i < extension->parameters().size(); i++) {
                const IType* ti = impl->parameters()[i + parameter_offset]->type()->accept_visitor(subst);
                const IType* tm = extension->parameters()[i]->type();
                if (!NormalizeTypeVisitor::type_erasure().equivalent_types(ti, tm)) return false;
            }
            return NormalizeTypeVisitor::type_erasure().equivalent_types(impl->return_type()->accept_visitor(subst), extension->return_type());
        };

        const IMethod* found_impl = nullptr;
        for (const IMethod* impl : container_->methods()) {
            if (!is_matching_implementation(impl)) continue;
            assert(found_impl == nullptr && "Multiple matching implementations found");
            found_impl = impl;
        }

        assert(found_impl != nullptr && "No matching implementation found");
        if (found_impl == nullptr) continue;

        implementations.emplace_back(extension, found_impl);
    }

    for (const auto& [extension, implementation] : implementations) {
        ExtensionMemberInfo info;
        info.extension_marker_method = marker;
        info.extension_member = extension;
        info.implementation_method = implementation;
        info.extension_grouping_type_parameters = type_parameters;
        extension_member_map_.set(extension, info);
        implementation_member_map_.set(implementation, info);
    }
}

const ExtensionMemberInfo* ExtensionInfo::info_of_extension_member(const IMethod* method) const { return extension_member_map_.find(method); }

const ExtensionMemberInfo* ExtensionInfo::info_of_implementation_member(const IMethod* method) const {
    return implementation_member_map_.find(method);
}

bool ExtensionInfo::is_extension_group_type(const ITypeDefinition* td) const {
    for (const ExtensionGroupInfo& group : extension_groups_) {
        const ITypeDefinition* marker_type = group.marker_method->declaring_type_definition();
        const ITypeDefinition* group_type = marker_type != nullptr ? marker_type->declaring_type_definition() : nullptr;
        if (group_type != nullptr && type_equals(group_type, td)) return true;
    }
    return false;
}

bool ExtensionInfo::is_extension_marker_type(const ITypeDefinition* td, ExtensionGroupInfo* extension_group) const {
    if (extension_group != nullptr) *extension_group = ExtensionGroupInfo();
    for (const ExtensionGroupInfo& group : extension_groups_) {
        const ITypeDefinition* marker_type = group.marker_method->declaring_type_definition();
        if (marker_type != nullptr && type_equals(marker_type, td)) {
            if (extension_group != nullptr) *extension_group = group;
            return true;
        }
    }
    return false;
}

bool ExtensionInfo::is_extension_marker_type(const ITypeDefinition* td, const ITypeDefinition** marker_type) const {
    if (marker_type != nullptr) *marker_type = nullptr;
    ExtensionGroupInfo group;
    if (!is_extension_marker_type(td, &group)) return false;
    if (marker_type != nullptr) *marker_type = group.marker_method->declaring_type_definition();
    return true;
}

std::vector<const IMember*> ExtensionInfo::get_members_of_group(const IMethod* marker) const {
    const ITypeDefinition* marker_type = marker->declaring_type_definition();
    assert(marker_type != nullptr && "Marker should always be contained in a type");

    std::vector<const IMember*> members;
    for (const auto& item : extension_member_map_.items()) {
        const ExtensionMemberInfo& info = item.second;
        if (!type_equals(marker_type, info.extension_marker_type())) continue;
        TypeParameterSubstitution subst(as_types(info.extension_grouping_type_parameters), std::nullopt);
        const IMember* member = info.extension_member->is_accessor() ? info.extension_member->accessor_owner()->specialize(subst)
                                                                      : info.extension_member->specialize(subst);
        // Distinct: a property is reached once per accessor.
        bool seen = false;
        for (const IMember* existing : members) {
            if (member_equals(existing, member)) {
                seen = true;
                break;
            }
        }
        if (!seen) members.push_back(member);
    }
    return members;
}

namespace {

// The maker MetadataTypeDefinition::extension_info() asks: a class whose extension methods all use the classic "this" parameter has no
// extension groups and so no extension info.
std::shared_ptr<const ExtensionInfo> make_extension_info(const MetadataModule& module, const ITypeDefinition* container) {
    auto info = std::make_shared<const ExtensionInfo>(module, container);
    if (info->extension_groups().empty()) return nullptr;
    return info;
}

const bool extension_info_maker_registered = (set_extension_info_maker(&make_extension_info), true);

}  // namespace

}  // namespace cb::dotnet::decompiler::typesystem
