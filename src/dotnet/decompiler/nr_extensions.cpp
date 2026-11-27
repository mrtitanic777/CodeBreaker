// dotnet/decompiler/nr_extensions.cpp -- see nr_extensions.hpp.
#include "dotnet/decompiler/nr_extensions.hpp"

#include <string>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler {

namespace {

bool name_has_anonymous_type_marker(const std::string& name) {
    return name.find("AnonType") != std::string::npos || name.find("AnonymousType") != std::string::npos;
}

// A C# anonymous type is immutable and compares all of its members, so only an anonymous type with no settable property can be written
// as one. The C# compiler emits nothing else, but VB's properties are settable unless declared 'Key', and only 'Key' members take part
// in Equals/GetHashCode: those types keep their own declaration.
bool has_only_read_only_properties(const typesystem::ITypeDefinition* type) {
    for (const typesystem::IProperty* property : type->properties()) {
        if (property->can_set()) return false;
    }
    return true;
}

class ContainsAnonTypeVisitor final : public typesystem::TypeVisitor {
public:
    bool contains_anon_type = false;

    const typesystem::IType* visit_other_type(const typesystem::IType& type) override {
        if (is_anonymous_type(&type)) contains_anon_type = true;
        return TypeVisitor::visit_other_type(type);
    }
    const typesystem::IType* visit_type_definition(const typesystem::ITypeDefinition& type) override {
        if (is_anonymous_type(&type)) contains_anon_type = true;
        return TypeVisitor::visit_type_definition(type);
    }
};

}  // namespace

bool is_compiler_generated(const typesystem::IEntity* entity) {
    return entity != nullptr && entity->has_attribute(typesystem::KnownAttribute::CompilerGenerated);
}

bool is_compiler_generated_or_is_in_compiler_generated_class(const typesystem::IEntity* entity) {
    for (; entity != nullptr; entity = entity->declaring_type_definition())
        if (is_compiler_generated(entity)) return true;
    return false;
}

bool contains_anonymous_type(const typesystem::IType* type) {
    ContainsAnonTypeVisitor visitor;
    type->accept_visitor(visitor);
    return visitor.contains_anon_type;
}

bool has_generated_name(const typesystem::IType* type) { return is_generated_name(type->name()); }

bool is_anonymous_type_declared_as_named_type(const typesystem::ITypeDefinition* type) {
    return type != nullptr && type->namespace_().empty() && has_generated_name(type) && name_has_anonymous_type_marker(type->name()) &&
           is_compiler_generated(type) && !has_only_read_only_properties(type);
}

bool is_anonymous_type(const typesystem::IType* type) {
    if (type == nullptr) return false;
    if (type->namespace_().empty() && has_generated_name(type) && name_has_anonymous_type_marker(type->name())) {
        const typesystem::ITypeDefinition* td = type->get_definition();
        return td != nullptr && is_compiler_generated(td) && has_only_read_only_properties(td);
    }
    return false;
}

bool has_generated_name(const typesystem::IMember* member) { return member->name().rfind('<', 0) == 0; }

std::uint32_t get_metadata_attributes(const typesystem::ITypeDefinition* type) {
    const metadata::MetadataFile* file = type->parent_module()->metadata_file();
    if (file == nullptr || type->metadata_token().is_nil()) return 0;
    return file->metadata().get_type_definition(srm::TypeDefinitionHandle(type->metadata_token())).attributes();
}

}  // namespace cb::dotnet::decompiler
