// dotnet/decompiler/typesystem/type_system_dump.cpp -- the type system as the oracle's text (testdata/oracle/TypeSystemDump).
#include "dotnet/decompiler/typesystem/type_system_dump.hpp"

#include <format>
#include <functional>

#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/specialized_member.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/number_format.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// ---- .NET's enum names -------------------------------------------------------------------------------------------------

const char* name_of(TypeKind k) {
    static constexpr const char* kNames[] = {"Other", "Class", "Interface", "Struct", "Delegate", "Enum", "Void", "Unknown", "Null", "None", "Dynamic",
                                             "UnboundTypeArgument", "TypeParameter", "Array", "Pointer", "ByReference", "Intersection", "ArgList", "Tuple",
                                             "ModOpt", "ModReq", "NInt", "NUInt", "FunctionPointer"};
    return kNames[static_cast<int>(k)];
}
const char* name_of(Accessibility a) {
    static constexpr const char* kNames[] = {"None", "Private", "ProtectedAndInternal", "Protected", "Internal", "ProtectedOrInternal", "Public"};
    return kNames[static_cast<int>(a)];
}
const char* name_of(SymbolKind k) {
    static constexpr const char* kNames[] = {"None", "Module", "TypeDefinition", "Field", "Property", "Indexer", "Event", "Method", "Operator", "Constructor",
                                             "Destructor", "Accessor", "Namespace", "Variable", "Parameter", "TypeParameter", "Constraint", "ReturnType"};
    return kNames[static_cast<int>(k)];
}
const char* name_of(VarianceModifier v) {
    static constexpr const char* kNames[] = {"Invariant", "Covariant", "Contravariant"};
    return kNames[static_cast<int>(v)];
}
const char* name_of(Nullability n) {
    static constexpr const char* kNames[] = {"Oblivious", "NotNullable", "Nullable"};
    return kNames[static_cast<int>(n)];
}
const char* name_of(ReferenceKind r) {
    static constexpr const char* kNames[] = {"None", "Out", "Ref", "In", "RefReadOnly"};
    return kNames[static_cast<int>(r)];
}
const char* name_of(KnownTypeCode code) {
    static constexpr const char* kNames[kKnownTypeCodeCount] = {
        "None", "Object", "DBNull", "Boolean", "Char", "SByte", "Byte", "Int16", "UInt16", "Int32", "UInt32", "Int64", "UInt64", "Single",
        "Double", "Decimal", "DateTime", "17", "String", "Void", "Type", "Array", "Attribute", "ValueType", "Enum", "Delegate",
        "MulticastDelegate", "Exception", "IntPtr", "UIntPtr", "IEnumerable", "IEnumerator", "IEnumerableOfT", "IEnumeratorOfT",
        "ICollection", "ICollectionOfT", "IList", "IListOfT", "IReadOnlyCollectionOfT", "IReadOnlyListOfT", "Task", "TaskOfT", "ValueTask",
        "ValueTaskOfT", "NullableOfT", "IDisposable", "IAsyncDisposable", "INotifyCompletion", "ICriticalNotifyCompletion", "TypedReference",
        "IFormattable", "FormattableString", "DefaultInterpolatedStringHandler", "SpanOfT", "ReadOnlySpanOfT", "MemoryOfT", "Unsafe",
        "IAsyncEnumerableOfT", "IAsyncEnumeratorOfT", "Index", "Range",
    };
    return kNames[static_cast<int>(code)];
}
std::string name_of(srm::SignatureCallingConvention c) {
    switch (c) {
    case srm::SignatureCallingConvention::Default: return "Default";
    case srm::SignatureCallingConvention::CDecl: return "CDecl";
    case srm::SignatureCallingConvention::StdCall: return "StdCall";
    case srm::SignatureCallingConvention::ThisCall: return "ThisCall";
    case srm::SignatureCallingConvention::FastCall: return "FastCall";
    case srm::SignatureCallingConvention::VarArgs: return "VarArgs";
    case srm::SignatureCallingConvention::Unmanaged: return "Unmanaged";
    }
    return std::to_string(static_cast<int>(c));
}
std::string name_of(MethodSemanticsAttributes a) {
    switch (a) {
    case MethodSemanticsAttributes::None: return "0";
    case MethodSemanticsAttributes::Setter: return "Setter";
    case MethodSemanticsAttributes::Getter: return "Getter";
    case MethodSemanticsAttributes::Other: return "Other";
    case MethodSemanticsAttributes::Adder: return "Adder";
    case MethodSemanticsAttributes::Remover: return "Remover";
    case MethodSemanticsAttributes::Raiser: return "Raiser";
    }
    return std::to_string(static_cast<int>(a));
}

std::string b(bool v) { return v ? "1" : "0"; }

// ---- text ----------------------------------------------------------------------------------------------------------------

std::string esc16(std::u16string_view s) {
    std::string r = "\"";
    for (const char16_t c : s) {
        if (c == u'\\' || c == u'"') {
            r += '\\';
            r += static_cast<char>(c);
        } else if (c < 0x20 || c > 0x7e) {
            r += std::format("\\u{:04x}", static_cast<int>(c));
        } else {
            r += static_cast<char>(c);
        }
    }
    return r + "\"";
}
std::string esc(std::string_view s) { return esc16(util::utf16(s)); }

std::string ref_text(std::optional<bool> v) { return !v ? "null" : *v ? "ref" : "value"; }

std::string T(const IType* t);

std::string T(const IType* t) {
    if (t == nullptr) return "null";
    if (const auto* n = dynamic_cast<const NullabilityAnnotatedType*>(t))
        return T(n->type_without_annotation()) + (n->nullability() == Nullability::Nullable ? "?" : "!");
    if (const auto* tt = dynamic_cast<const TupleType*>(t)) {
        std::string s = "(";
        for (std::size_t i = 0; i < tt->element_types().size(); ++i) {
            if (i > 0) s += ", ";
            s += T(tt->element_types()[i]);
            if (i < tt->element_names().size() && tt->element_names()[i]) s += " " + *tt->element_names()[i];
        }
        return s + ")";
    }
    if (const auto* p = dynamic_cast<const ParameterizedType*>(t)) {
        std::string s = T(p->generic_type()) + "<";
        for (std::size_t i = 0; i < p->type_arguments().size(); ++i) s += (i > 0 ? "," : "") + T(p->type_arguments()[i]);
        return s + ">";
    }
    if (const auto* a = dynamic_cast<const ArrayType*>(t))
        return T(a->element_type()) + "[" + std::string(static_cast<std::size_t>(a->dimensions() - 1), ',') + "]" +
               (a->nullability() == Nullability::Nullable ? "?" : a->nullability() == Nullability::NotNullable ? "!" : "");
    if (const auto* pt = dynamic_cast<const PointerType*>(t)) return T(pt->element_type()) + "*";
    if (const auto* r = dynamic_cast<const ByReferenceType*>(t)) return T(r->element_type()) + "&";
    if (const auto* m = dynamic_cast<const ModifiedType*>(t))
        return T(m->element_type()) + (m->kind() == TypeKind::ModReq ? " modreq(" : " modopt(") + T(m->modifier()) + ")";
    if (const auto* pn = dynamic_cast<const PinnedType*>(t)) return T(pn->element_type()) + " pinned";
    if (const auto* f = dynamic_cast<const FunctionPointerType*>(t)) {
        std::string s = "delegate*<" + name_of(f->calling_convention);
        if (!f->custom_calling_conventions.empty()) {
            s += "[";
            for (std::size_t i = 0; i < f->custom_calling_conventions.size(); ++i) s += (i > 0 ? "," : "") + T(f->custom_calling_conventions[i]);
            s += "]";
        }
        s += ">(";
        for (std::size_t i = 0; i < f->parameter_types.size(); ++i)
            s += std::string(i > 0 ? ", " : "") + name_of(f->parameter_reference_kinds[i]) + " " + T(f->parameter_types[i]);
        s += "):" + std::string(f->return_is_ref_read_only ? "ref readonly " : "") + T(f->return_type);
        return s + (f->kind() == TypeKind::FunctionPointer ? "" : "/struct");
    }
    if (const auto* tp = dynamic_cast<const ITypeParameter*>(t))
        return std::string(tp->owner_type() == SymbolKind::Method ? "!!" : "!") + std::to_string(tp->index()) + ":" + tp->name();
    if (const auto* u = dynamic_cast<const UnknownType*>(t)) return "unknown:" + u->full_type_name().reflection_name() + ":" + ref_text(u->is_reference_type());
    if (const auto* td = dynamic_cast<const ITypeDefinition*>(t)) return td->full_type_name().reflection_name() + "@" + td->parent_module()->assembly_name();
    switch (t->kind()) {
    case TypeKind::Unknown: case TypeKind::Null: case TypeKind::None: case TypeKind::Dynamic: case TypeKind::UnboundTypeArgument: case TypeKind::ArgList:
    case TypeKind::NInt: case TypeKind::NUInt: return std::string("special:") + name_of(t->kind());
    default: return "other";
    }
}

std::string src(const IMember* m) {
    if (dynamic_cast<const VarArgInstanceMethod*>(m) != nullptr) return "vararg";
    if (dynamic_cast<const SpecializedMember*>(m) != nullptr) return "specialized";
    if (dynamic_cast<const FakeMember*>(m) != nullptr) return "fake";
    if (dynamic_cast<const MetadataMemberBase*>(m) != nullptr) return "metadata";
    return "other";
}

std::string M(const IMember* m) {
    if (m == nullptr) return "null";
    std::string s = src(m) + " " + name_of(m->symbol_kind()) + " " + T(m->declaring_type()) + "." + m->name();
    if (const auto* method = dynamic_cast<const IMethod*>(m); method != nullptr && !method->type_arguments().empty()) {
        s += "<";
        for (std::size_t i = 0; i < method->type_arguments().size(); ++i) s += (i > 0 ? "," : "") + T(method->type_arguments()[i]);
        s += ">";
    }
    if (const auto* pm = dynamic_cast<const IParameterizedMember*>(m)) {
        s += "(";
        for (std::size_t i = 0; i < pm->parameters().size(); ++i) s += (i > 0 ? ", " : "") + T(pm->parameters()[i]->type());
        s += ")";
    }
    return s + ":" + T(m->return_type());
}

std::string V(const AttributeArgumentValue& v);

std::string V(const AttributeArgumentValue& v) {
    return std::visit(
        [](const auto& x) -> std::string {
            using X = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<X, std::monostate>) return "null";
            else if constexpr (std::is_same_v<X, std::string>) return esc(x);
            else if constexpr (std::is_same_v<X, bool>) return x ? "True" : "False";
            else if constexpr (std::is_same_v<X, char16_t>) return "char:" + std::to_string(static_cast<int>(x));
            else if constexpr (std::is_same_v<X, float>) return "float:" + util::format_r(x);
            else if constexpr (std::is_same_v<X, double>) return "double:" + util::format_r(x);
            else if constexpr (std::is_same_v<X, const IType*>) return "type:" + T(x);
            else if constexpr (std::is_same_v<X, std::vector<AttributeTypedArgument>>) {
                std::string s = "[";
                for (std::size_t i = 0; i < x.size(); ++i) s += (i > 0 ? ", " : "") + T(x[i].type) + "=" + V(x[i].value);
                return s + "]";
            } else if constexpr (std::is_same_v<X, std::shared_ptr<const AttributeTypedArgument>>) return x ? T(x->type) + "=" + V(x->value) : "null";
            else if constexpr (std::is_same_v<X, std::uint8_t>) return "Byte:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::int8_t>) return "SByte:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::int16_t>) return "Int16:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::uint16_t>) return "UInt16:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::int32_t>) return "Int32:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::uint32_t>) return "UInt32:" + std::to_string(x);
            else if constexpr (std::is_same_v<X, std::int64_t>) return "Int64:" + std::to_string(x);
            else return "UInt64:" + std::to_string(x);
        },
        v);
}

std::string V(const ConstantValue& v) {
    return std::visit(
        [](const auto& x) -> std::string {
            using X = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<X, std::monostate>) return "null";
            else if constexpr (std::is_same_v<X, std::u16string>) return esc16(x);
            else if constexpr (std::is_same_v<X, util::Decimal>) return "decimal:" + x.to_string();
            else return V(AttributeArgumentValue(x));
        },
        v);
}

std::string A(const IAttribute* a) {
    std::string s = T(a->attribute_type()) + " ctor=" + M(a->constructor()) + " err=" + b(a->has_decode_errors()) + " (";
    const auto& fixed = a->fixed_arguments();
    for (std::size_t i = 0; i < fixed.size(); ++i) s += (i > 0 ? ", " : "") + T(fixed[i].type) + "=" + V(fixed[i].value);
    s += ") {";
    const auto& named = a->named_arguments();
    for (std::size_t i = 0; i < named.size(); ++i)
        s += std::string(i > 0 ? ", " : "") + (named[i].kind == srm::CustomAttributeNamedArgumentKind::Field ? "Field" : "Property") + " " +
             named[i].name.value_or(std::string()) + ":" + T(named[i].type) + "=" + V(named[i].value);
    return s + "}";
}

// ---- the dump ------------------------------------------------------------------------------------------------------------

class Dumper {
public:
    explicit Dumper(const DecompilerTypeSystem& ts) : ts_(ts), module_(ts.main_metadata_module()) {}

    std::string run() {
        const srm::MetadataReader& md = module_.metadata();
        line(std::format("options {:x}", static_cast<std::uint32_t>(ts_.type_system_options())));
        for (const IModule* m : ts_.modules()) {
            const bool non_assembly = m->metadata_file() != nullptr && !m->metadata_file()->is_assembly();
            line("module " + esc(m->assembly_name()) + " full=" + esc(m->full_assembly_name()) + " version=" + (non_assembly ? "" : m->assembly_version().to_string()) +
                 " main=" + b(m->is_main_module()) + " file=" + b(m->metadata_file() != nullptr));
        }
        for (int i = 1; i <= static_cast<int>(KnownTypeCode::Range); ++i) {
            if (KnownTypeReference::get(static_cast<KnownTypeCode>(i)) == nullptr) continue;
            line(std::string("known ") + name_of(static_cast<KnownTypeCode>(i)) + " " + T(ts_.find_type(static_cast<KnownTypeCode>(i))));
        }
        guard("", [&] {
            for (const IAttribute* a : module_.get_assembly_attributes()) line("asmattr " + A(a));
        });
        guard("", [&] {
            for (const IAttribute* a : module_.get_module_attributes()) line("modattr " + A(a));
        });
        name_space(module_.root_namespace(), "");
        for (const ITypeDefinition* td : module_.type_definitions()) guard("", [&] { type(td); });
        for (const srm::MemberReferenceHandle h : md.member_references())
            guard("", [&] { line(std::format("ref {:08x} ", srm::EntityHandle(h).token()) + M(dynamic_cast<const IMember*>(module_.resolve_entity(h)))); });
        for (std::uint32_t row = 1; row <= static_cast<std::uint32_t>(md.get_table_row_count(srm::TableIndex::MethodSpec)); ++row) {
            const srm::MethodSpecificationHandle h(row);
            guard("", [&] { line(std::format("spec {:08x} ", srm::EntityHandle(h).token()) + M(dynamic_cast<const IMember*>(module_.resolve_entity(h)))); });
        }
        for (const srm::TypeReferenceHandle h : md.type_references())
            guard("", [&] { line(std::format("tref {:08x} ", srm::EntityHandle(h).token()) + T(module_.resolve_type(h, GenericContext()))); });
        for (std::uint32_t row = 1; row <= static_cast<std::uint32_t>(md.get_table_row_count(srm::TableIndex::TypeSpec)); ++row) {
            const srm::TypeSpecificationHandle h(row);
            guard("", [&] { line(std::format("tspec {:08x} ", srm::EntityHandle(h).token()) + T(module_.resolve_type(h, GenericContext()))); });
        }
        return std::move(out_);
    }

private:
    void line(const std::string& s) {
        out_ += s;
        out_ += '\n';
    }

    void guard(const std::string& indent, const std::function<void()>& f) {
        try {
            f();
        } catch (const std::exception&) {
            line(indent + "error");
        }
    }

    void attrs(const std::string& indent, const std::vector<const IAttribute*>& list) {
        for (const IAttribute* a : list) line(indent + "attr " + A(a));
    }

    void name_space(const INamespace& ns, const std::string& indent) {
        line(indent + "ns " + esc(ns.full_name()) + " name=" + esc(ns.name()) + " types=" + std::to_string(ns.types().size()));
        for (const INamespace* child : ns.child_namespaces()) name_space(*child, indent + "  ");
    }

    void type_parameters(const std::string& indent, const std::vector<const ITypeParameter*>& tps, std::size_t from) {
        for (std::size_t i = from; i < tps.size(); ++i) {
            const ITypeParameter* tp = tps[i];
            line(indent + "tp " + std::to_string(tp->index()) + " " + tp->name() + " owner=" + name_of(tp->owner_type()) + " var=" + name_of(tp->variance()) +
                 " class=" + b(tp->has_reference_type_constraint()) + " struct=" + b(tp->has_value_type_constraint()) +
                 " new=" + b(tp->has_default_constructor_constraint()) + " unmanaged=" + b(tp->has_unmanaged_constraint()) +
                 " allowsref=" + b(tp->allows_ref_like_type()) + " nullc=" + name_of(tp->nullability_constraint()));
            attrs(indent + "  ", tp->get_attributes());
            for (const TypeConstraint& c : tp->type_constraints()) {
                line(indent + "  constraint " + T(c.type));
                attrs(indent + "    ", c.attributes);
            }
            line(indent + "  ebc " + T(tp->effective_base_class()));
            for (const IType* i2 : tp->effective_interface_set()) line(indent + "  eis " + T(i2));
        }
    }

    void parameters(const std::string& indent, const std::vector<const IParameter*>& ps) {
        for (std::size_t i = 0; i < ps.size(); ++i) {
            const IParameter* p = ps[i];
            std::string s = indent + "param " + std::to_string(i) + " " + p->name() + " type=" + T(p->type()) + " ref=" + name_of(p->reference_kind()) +
                            " params=" + b(p->is_params()) + " opt=" + b(p->is_optional()) + " hasconst=" + b(p->has_constant_value_in_signature()) +
                            " scoped=" + b(p->lifetime().scoped_ref);
            if (p->has_constant_value_in_signature()) s += " value=" + V(p->get_constant_value());
            line(s);
            attrs(indent + "  ", p->get_attributes());
        }
    }

    static std::string member_flags(const IMember* m) {
        return std::string("acc=") + name_of(m->accessibility()) + " static=" + b(m->is_static()) + " abstract=" + b(m->is_abstract()) +
               " sealed=" + b(m->is_sealed()) + " virtual=" + b(m->is_virtual()) + " override=" + b(m->is_override()) +
               " overridable=" + b(m->is_overridable()) + " explicit=" + b(m->is_explicit_interface_implementation());
    }

    void member(const IMember* member) {
        if (const auto* f = dynamic_cast<const IField*>(member)) {
            std::string s = "  field " + f->name() + " type=" + T(f->return_type()) + " " + member_flags(f) + " readonly=" + b(f->is_read_only()) +
                            " volatile=" + b(f->is_volatile()) + " const=" + b(f->is_const()) + " rro=" + b(f->return_type_is_ref_read_only());
            if (f->is_const()) s += " value=" + V(f->get_constant_value());
            line(s);
            attrs("    ", f->get_attributes());
        } else if (const auto* m = dynamic_cast<const IMethod*>(member)) {
            line("  method " + m->name() + " kind=" + name_of(m->symbol_kind()) + " " + member_flags(m) + " ext=" + b(m->is_extension_method()) +
                 " body=" + b(m->has_body()) + " init=" + b(m->is_init_only()) + " rro=" + b(m->return_type_is_ref_read_only()) +
                 " thisro=" + b(m->this_is_ref_read_only()) + " accessor=" + name_of(m->accessor_kind()) +
                 " owner=" + (m->accessor_owner() != nullptr ? m->accessor_owner()->name() : std::string("-")) + " ret=" + T(m->return_type()));
            type_parameters("    ", m->type_parameters(), 0);
            parameters("    ", m->parameters());
            attrs("    ", m->get_attributes());
            for (const IAttribute* a : m->get_return_type_attributes()) line("    retattr " + A(a));
            for (const IMember* i : m->explicitly_implemented_interface_members()) line("    impl " + M(i));
            if (m->has_body() && !m->metadata_token().is_nil()) {
                const srm::MethodDefinition def = module_.metadata().get_method_definition(srm::MethodDefinitionHandle(m->metadata_token()));
                const srm::MethodBodyBlock body = module_.metadata_file()->get_method_body(def.relative_virtual_address());
                if (!body.local_signature().is_nil()) {
                    const IType* declaring = m->declaring_type();
                    const std::vector<const IType*> locals = module_.decode_local_signature(
                        body.local_signature(), GenericContext(declaring != nullptr ? &declaring->type_parameters() : nullptr, &m->type_parameters()));
                    for (std::size_t i = 0; i < locals.size(); ++i) line("    local " + std::to_string(i) + " " + T(locals[i]));
                }
            }
        } else if (const auto* p = dynamic_cast<const IProperty*>(member)) {
            line("  property " + p->name() + " kind=" + name_of(p->symbol_kind()) + " " + member_flags(p) + " get=" + b(p->can_get()) + " set=" + b(p->can_set()) +
                 " indexer=" + b(p->is_indexer()) + " rro=" + b(p->return_type_is_ref_read_only()) + " ret=" + T(p->return_type()) +
                 " getter=" + (p->getter() != nullptr ? p->getter()->name() : std::string("-")) +
                 " setter=" + (p->setter() != nullptr ? p->setter()->name() : std::string("-")));
            parameters("    ", p->parameters());
            attrs("    ", p->get_attributes());
            for (const IMember* i : p->explicitly_implemented_interface_members()) line("    impl " + M(i));
        } else if (const auto* e = dynamic_cast<const IEvent*>(member)) {
            line("  event " + e->name() + " " + member_flags(e) + " add=" + (e->add_accessor() != nullptr ? e->add_accessor()->name() : std::string("-")) +
                 " remove=" + (e->remove_accessor() != nullptr ? e->remove_accessor()->name() : std::string("-")) +
                 " invoke=" + (e->invoke_accessor() != nullptr ? e->invoke_accessor()->name() : std::string("-")) + " ret=" + T(e->return_type()));
            attrs("    ", e->get_attributes());
            for (const IMember* i : e->explicitly_implemented_interface_members()) line("    impl " + M(i));
        }
    }

    void type(const ITypeDefinition* td) {
        const ITypeDefinition* declaring = td->declaring_type_definition();
        std::string s = "type " + td->full_type_name().reflection_name() + " kind=" + name_of(td->kind()) + " acc=" + name_of(td->accessibility()) +
                        " static=" + b(td->is_static()) + " abstract=" + b(td->is_abstract()) + " sealed=" + b(td->is_sealed()) +
                        " reftype=" + ref_text(td->is_reference_type()) + " byreflike=" + b(td->is_by_ref_like()) + " readonly=" + b(td->is_read_only()) +
                        " record=" + b(td->is_record()) + " ktc=" + name_of(td->known_type_code()) + " nullctx=" + name_of(td->nullable_context()) +
                        " ext=" + b(td->has_extensions()) + " meta=" + esc(td->metadata_name()) +
                        " decl=" + (declaring != nullptr ? declaring->full_type_name().reflection_name() : std::string("-"));
        if (td->kind() == TypeKind::Enum) s += " enumtype=" + T(td->enum_underlying_type());
        line(s);
        type_parameters("  ", td->type_parameters(), declaring != nullptr ? static_cast<std::size_t>(declaring->type_parameter_count()) : 0);
        for (const IType* base : td->direct_base_types()) line("  base " + T(base));
        attrs("  ", td->get_attributes());
        for (const IMember* m : td->members()) guard("  ", [&] { member(m); });
    }

    const DecompilerTypeSystem& ts_;
    const MetadataModule& module_;
    std::string out_;
};

}  // namespace

std::string dump_type_system(const DecompilerTypeSystem& type_system) { return Dumper(type_system).run(); }

}  // namespace cb::dotnet::decompiler::typesystem
