// dotnet/decompiler/typesystem/tuple_type.cpp -- TupleType and FunctionPointerType, and the visitor's defaults for them.
#include "dotnet/decompiler/typesystem/tuple_type.hpp"

#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

constexpr int kRestIndex = TupleType::kRestPosition - 1;

const IType* find_value_tuple_type(const ICompilation& compilation, const IModule* value_tuple_assembly, int tpc) {
    const TopLevelTypeName name("System", "ValueTuple", tpc);
    if (value_tuple_assembly != nullptr)
        if (const ITypeDefinition* def = value_tuple_assembly->get_type_definition(name)) return def;
    return find_type(compilation, name);
}

const ParameterizedType* create_underlying_type(const ICompilation& compilation, const std::vector<const IType*>& element_types, const IModule* assembly) {
    const int count = static_cast<int>(element_types.size());
    const int remainder = (count - 1) % (TupleType::kRestPosition - 1) + 1;
    int pos = count - remainder;
    const IType* type = make_parameterized_type(find_value_tuple_type(compilation, assembly, remainder),
                                                std::vector<const IType*>(element_types.begin() + pos, element_types.end()));
    while (pos > 0) {
        pos -= TupleType::kRestPosition - 1;
        std::vector<const IType*> args(element_types.begin() + pos, element_types.begin() + pos + TupleType::kRestPosition - 1);
        args.push_back(type);
        type = make_parameterized_type(find_value_tuple_type(compilation, assembly, TupleType::kRestPosition), std::move(args));
    }
    return dynamic_cast<const ParameterizedType*>(type);
}

bool is_value_tuple_name(const IType* type) { return type->namespace_() == "System" && type->name() == "ValueTuple"; }

}  // namespace

const IType* TypeVisitor::visit_tuple_type(const TupleType& type) { return type.visit_children(*this); }
const IType* TypeVisitor::visit_function_pointer_type(const FunctionPointerType& type) { return type.visit_children(*this); }

const IType* NormalizeTypeVisitor::visit_tuple_type(const TupleType& type) {
    if (tuple_to_underlying_type) return type.underlying_type()->accept_visitor(*this);
    return TypeVisitor::visit_tuple_type(type);
}

TupleType::TupleType(const ICompilation& compilation, std::vector<const IType*> element_types, std::vector<std::optional<std::string>> element_names,
                     const IModule* value_tuple_assembly)
    : compilation_(&compilation), underlying_(create_underlying_type(compilation, element_types, value_tuple_assembly)), element_types_(std::move(element_types)) {
    if (element_names.empty()) element_names_.assign(element_types_.size(), std::nullopt);
    else element_names_ = std::move(element_names);
}

bool TupleType::is_tuple_compatible(const IType* type, int& tuple_cardinality) {
    switch (type->kind()) {
    case TypeKind::Tuple:
        tuple_cardinality = dynamic_cast<const TupleType*>(type)->cardinality();
        return true;
    case TypeKind::Struct:
        // C# requires System.ValueTuple to be a struct: a class of that name is some other type
        if (is_value_tuple_name(type)) {
            const int tpc = type->type_parameter_count();
            if (tpc > 0 && tpc < kRestPosition) {
                tuple_cardinality = tpc;
                return true;
            }
            if (tpc == kRestPosition)
                if (const auto* pt = dynamic_cast<const ParameterizedType*>(type))
                    if (is_tuple_compatible(pt->type_arguments()[kRestIndex], tuple_cardinality)) {
                        tuple_cardinality += kRestPosition - 1;
                        return true;
                    }
        }
        break;
    default: break;
    }
    tuple_cardinality = 0;
    return false;
}

const TupleType* TupleType::from_underlying_type(const ICompilation& compilation, const IType* type) {
    const auto elements = get_tuple_element_types(type);
    if (!elements || elements->empty()) return nullptr;
    const ITypeDefinition* def = type->get_definition();
    return make_tuple_type(compilation, *elements, {}, def != nullptr ? def->parent_module() : nullptr);
}

std::optional<std::vector<const IType*>> TupleType::get_tuple_element_types(const IType* tuple_type) {
    std::optional<std::vector<const IType*>> output;
    std::function<bool(const IType*)> collect = [&](const IType* type) -> bool {
        switch (type->kind()) {
        case TypeKind::Tuple: {
            if (!output) output.emplace();
            const auto& e = dynamic_cast<const TupleType*>(type)->element_types();
            output->insert(output->end(), e.begin(), e.end());
            return true;
        }
        case TypeKind::Class:
        case TypeKind::Struct:
            if (is_value_tuple_name(type)) {
                if (!output) output.emplace();
                const int tpc = type->type_parameter_count();
                if (tpc > 0 && tpc < kRestPosition) {
                    output->insert(output->end(), type->type_arguments().begin(), type->type_arguments().end());
                    return true;
                }
                if (tpc == kRestPosition) {
                    output->insert(output->end(), type->type_arguments().begin(), type->type_arguments().begin() + (kRestPosition - 1));
                    return collect(type->type_arguments()[kRestIndex]);
                }
            }
            break;
        default: break;
        }
        return false;
    };
    if (collect(tuple_type)) return output;
    return std::nullopt;
}

bool TupleType::equals(const IType* other) const {
    const auto* o = dynamic_cast<const TupleType*>(other);
    return o != nullptr && underlying_->equals(o->underlying_) && element_names_ == o->element_names_;
}

int TupleType::hash_code() const {
    int h = underlying_->hash_code();
    for (const auto& n : element_names_)
        h = static_cast<int>(static_cast<std::uint32_t>(h) * 31u + (n ? static_cast<std::uint32_t>(std::hash<std::string>()(*n)) : 0u));
    return h;
}

std::string TupleType::to_string() const {
    std::string b = "(";
    for (std::size_t i = 0; i < element_types_.size(); ++i) {
        if (i > 0) b += ", ";
        b += element_types_[i]->to_string();
        if (element_names_[i]) b += " " + *element_names_[i];
    }
    return b + ")";
}

const IType* TupleType::visit_children(TypeVisitor& visitor) const {
    std::optional<std::vector<const IType*>> changed;
    for (std::size_t i = 0; i < element_types_.size(); ++i) {
        const IType* t = element_types_[i]->accept_visitor(visitor);
        if (t != element_types_[i]) {
            if (!changed) changed = element_types_;
            (*changed)[i] = t;
        }
    }
    if (!changed) return this;
    const ITypeDefinition* def = get_definition();
    return make_tuple_type(*compilation_, std::move(*changed), element_names_, def != nullptr ? def->parent_module() : nullptr);
}

const TupleType* make_tuple_type(const ICompilation& compilation, std::vector<const IType*> element_types, std::vector<std::optional<std::string>> element_names,
                                 const IModule* value_tuple_assembly) {
    TypeKey key{30, {&compilation, value_tuple_assembly}, {}, {}};
    for (const IType* t : element_types) key.parts.push_back(t);
    for (const auto& n : element_names) {
        key.numbers.push_back(n ? 1 : 0);
        key.names.push_back(n.value_or(""));
    }
    return compilation.arena().intern<TupleType>(std::move(key), [&] {
        return std::make_unique<TupleType>(compilation, element_types, element_names, value_tuple_assembly);
    });
}

const IType* tuple_underlying_type_or_self(const IType* type) {
    const auto* t = dynamic_cast<const TupleType*>(type);
    return without_nullability(t != nullptr ? t->underlying_type() : type);
}

// ---- FunctionPointerType -------------------------------------------------------------------------------------------------

FunctionPointerType::FunctionPointerType(const ICompilation& compilation, bool function_pointers, srm::SignatureCallingConvention calling_convention_,
                                         std::vector<const IType*> custom_calling_conventions_, const IType* return_type_, bool return_is_ref_read_only_,
                                         std::vector<const IType*> parameter_types_, std::vector<ReferenceKind> parameter_reference_kinds_)
    : calling_convention(calling_convention_), custom_calling_conventions(std::move(custom_calling_conventions_)), return_type(return_type_),
      return_is_ref_read_only(return_is_ref_read_only_), parameter_types(std::move(parameter_types_)),
      parameter_reference_kinds(std::move(parameter_reference_kinds_)), compilation_(&compilation), function_pointers_(function_pointers) {}

const ITypeDefinition* FunctionPointerType::get_definition() const {
    if (function_pointers_) return nullptr;
    // without function pointers in the type system, they act as aliases of UIntPtr
    return compilation_->find_type(KnownTypeCode::UIntPtr)->get_definition();
}

const ITypeDefinitionOrUnknown* FunctionPointerType::get_definition_or_unknown() const { return get_definition(); }

const IType* FunctionPointerType::visit_children(TypeVisitor& visitor) const {
    const IType* r = return_type->accept_visitor(visitor);
    std::optional<std::vector<const IType*>> pt;
    if (r != return_type) pt.emplace(parameter_types.size());
    for (std::size_t i = 0; i < parameter_types.size(); ++i) {
        const IType* p = parameter_types[i]->accept_visitor(visitor);
        if (p == nullptr) throw InternalError("TypeVisitor.Visit-method returned null");
        if (!pt && p != parameter_types[i]) {
            pt.emplace(parameter_types.size());
            for (std::size_t j = 0; j < i; ++j) (*pt)[j] = parameter_types[j];
        }
        if (pt) (*pt)[i] = p;
    }
    if (!pt) return this;
    return make_function_pointer_type(*compilation_, function_pointers_, calling_convention, custom_calling_conventions, r, return_is_ref_read_only,
                                      std::move(*pt), parameter_reference_kinds);
}

bool FunctionPointerType::equals(const IType* other) const {
    const auto* f = dynamic_cast<const FunctionPointerType*>(other);
    if (f == nullptr || calling_convention != f->calling_convention || custom_calling_conventions.size() != f->custom_calling_conventions.size() ||
        !return_type->equals(f->return_type) || return_is_ref_read_only != f->return_is_ref_read_only || parameter_types.size() != f->parameter_types.size() ||
        parameter_reference_kinds != f->parameter_reference_kinds)
        return false;
    for (std::size_t i = 0; i < custom_calling_conventions.size(); ++i)
        if (!custom_calling_conventions[i]->equals(f->custom_calling_conventions[i])) return false;
    for (std::size_t i = 0; i < parameter_types.size(); ++i)
        if (!parameter_types[i]->equals(f->parameter_types[i])) return false;
    return true;
}

int FunctionPointerType::hash_code() const {
    std::uint32_t h = static_cast<std::uint32_t>(return_type->hash_code()) ^ static_cast<std::uint32_t>(calling_convention);
    for (std::size_t i = 0; i < parameter_types.size(); ++i) {
        h ^= static_cast<std::uint32_t>(parameter_types[i]->hash_code()) ^ static_cast<std::uint32_t>(parameter_reference_kinds[i]);
        h *= 8310859u;
    }
    return static_cast<int>(h);
}

const IType* FunctionPointerType::with_signature(const IType* return_type_, std::vector<const IType*> parameter_types_) const {
    return make_function_pointer_type(*compilation_, function_pointers_, calling_convention, custom_calling_conventions, return_type_, return_is_ref_read_only,
                                      std::move(parameter_types_), parameter_reference_kinds);
}

const FunctionPointerType* make_function_pointer_type(const ICompilation& compilation, bool function_pointers, srm::SignatureCallingConvention calling_convention,
                                                      std::vector<const IType*> custom_calling_conventions, const IType* return_type, bool return_is_ref_read_only,
                                                      std::vector<const IType*> parameter_types, std::vector<ReferenceKind> parameter_reference_kinds) {
    TypeKey key{31, {&compilation, return_type}, {function_pointers ? 1 : 0, static_cast<int>(calling_convention), return_is_ref_read_only ? 1 : 0}, {}};
    key.numbers.push_back(static_cast<std::int64_t>(custom_calling_conventions.size()));
    for (const IType* t : custom_calling_conventions) key.parts.push_back(t);
    for (const IType* t : parameter_types) key.parts.push_back(t);
    for (const ReferenceKind k : parameter_reference_kinds) key.numbers.push_back(static_cast<int>(k));
    return compilation.arena().intern<FunctionPointerType>(std::move(key), [&] {
        return std::make_unique<FunctionPointerType>(compilation, function_pointers, calling_convention, custom_calling_conventions, return_type,
                                                     return_is_ref_read_only, parameter_types, parameter_reference_kinds);
    });
}

}  // namespace cb::dotnet::decompiler::typesystem
