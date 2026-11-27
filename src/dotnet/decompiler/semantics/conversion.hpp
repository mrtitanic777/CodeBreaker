// dotnet/decompiler/semantics/conversion.hpp -- a C# conversion between two types, as the resolver classifies it: identity, numeric,
// reference, boxing, user-defined, method group, tuple ... with the properties that tell them apart.
//
//     const Conversion* c = Conversion::implicit_reference_conversion();
//     c->is_implicit() && c->is_reference_conversion();     // true
//     c->to_string();                                        // "implicit reference conversion"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: Conversion and its inner conversion classes, from Conversion.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// The conversions without parameters are singletons (upstream's static readonly fields, here functions); the others are arena objects
// made by the factory functions.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::semantics {

class Conversion : public il::ArenaObject {
public:
    // ---- the conversions (upstream's factory methods and static fields) ------------------------------------------------------

    static const Conversion* none();
    static const Conversion* identity_conversion();
    static const Conversion* implicit_numeric_conversion();
    static const Conversion* explicit_numeric_conversion();
    static const Conversion* implicit_lifted_numeric_conversion();
    static const Conversion* explicit_lifted_numeric_conversion();
    static const Conversion* enumeration_conversion(bool is_implicit, bool is_lifted);
    static const Conversion* null_literal_conversion();
    static const Conversion* implicit_constant_expression_conversion();
    static const Conversion* implicit_reference_conversion();
    static const Conversion* explicit_reference_conversion();
    static const Conversion* implicit_dynamic_conversion();
    static const Conversion* explicit_dynamic_conversion();
    static const Conversion* implicit_nullable_conversion();
    static const Conversion* explicit_nullable_conversion();
    static const Conversion* implicit_pointer_conversion();
    static const Conversion* explicit_pointer_conversion();
    static const Conversion* boxing_conversion();
    static const Conversion* unboxing_conversion();
    static const Conversion* try_cast();
    static const Conversion* implicit_interpolated_string_conversion();
    static const Conversion* throw_expression_conversion();
    static const Conversion* inline_array_conversion();
    static const Conversion* implicit_span_conversion();

    // A user-defined conversion through `operator_method`, with the standard conversions before and after it.
    static const Conversion* user_defined_conversion(const typesystem::IMethod* operator_method, bool is_implicit,
                                                     const Conversion* conversion_before_user_defined_operator,
                                                     const Conversion* conversion_after_user_defined_operator, bool is_lifted = false,
                                                     bool is_ambiguous = false);
    static const Conversion* method_group_conversion(const typesystem::IMethod* chosen_method, bool is_virtual_method_lookup,
                                                     bool delegate_captures_first_argument);
    static const Conversion* invalid_method_group_conversion(const typesystem::IMethod* chosen_method, bool is_virtual_method_lookup,
                                                             bool delegate_captures_first_argument);
    static const Conversion* tuple_conversion(std::vector<const Conversion*> conversions);

    // ---- what a conversion is -------------------------------------------------------------------------------------------------

    // Whether the conversion exists (false: none, or an ambiguous user-defined one).
    virtual bool is_valid() const { return true; }
    virtual bool is_implicit() const { return false; }
    virtual bool is_explicit() const { return false; }
    virtual bool is_try_cast() const { return false; }
    virtual bool is_throw_expression_conversion() const { return false; }
    virtual bool is_identity_conversion() const { return false; }
    virtual bool is_null_literal_conversion() const { return false; }
    virtual bool is_constant_expression_conversion() const { return false; }
    virtual bool is_numeric_conversion() const { return false; }
    // A conversion between the nullable versions of its types.
    virtual bool is_lifted() const { return false; }
    virtual bool is_dynamic_conversion() const { return false; }
    virtual bool is_reference_conversion() const { return false; }
    virtual bool is_enumeration_conversion() const { return false; }
    virtual bool is_nullable_conversion() const { return false; }
    virtual bool is_user_defined() const { return false; }
    virtual const Conversion* conversion_before_user_defined_operator() const { return nullptr; }
    virtual const Conversion* conversion_after_user_defined_operator() const { return nullptr; }
    virtual bool is_boxing_conversion() const { return false; }
    virtual bool is_unboxing_conversion() const { return false; }
    virtual bool is_pointer_conversion() const { return false; }
    virtual bool is_method_group_conversion() const { return false; }
    virtual bool is_virtual_method_lookup() const { return false; }
    virtual bool delegate_captures_first_argument() const { return false; }
    virtual bool is_anonymous_function_conversion() const { return false; }
    // The operator method of a user-defined conversion, the chosen method of a method group conversion; else null.
    virtual const typesystem::IMethod* method() const { return nullptr; }
    virtual bool is_tuple_conversion() const { return false; }
    virtual bool is_interpolated_string_conversion() const { return false; }
    virtual bool is_inline_array_conversion() const { return false; }
    virtual bool is_implicit_span_conversion() const { return false; }
    // The conversions of a tuple conversion's elements (none for other conversions).
    virtual const std::vector<const Conversion*>& element_conversions() const;

    // Whether two conversions are the same one (upstream's Equals(Conversion): by identity unless a class says otherwise).
    virtual bool equals(const Conversion* other) const { return this == other; }
    virtual int hash_code() const;
    // (upstream does not override ToString here: it prints the runtime class's name)
    virtual std::string to_string() const { return "Conversion"; }

protected:
    Conversion() = default;
};

// Two conversions are the same conversion (upstream's a.Equals(b), with null equal to null).
inline bool conversion_equals(const Conversion* a, const Conversion* b) { return a == b || (a != nullptr && a->equals(b)); }

}  // namespace cb::dotnet::decompiler::semantics
