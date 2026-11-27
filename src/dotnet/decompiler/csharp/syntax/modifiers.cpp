// dotnet/decompiler/csharp/syntax/modifiers.cpp -- see modifiers.hpp.
#include "dotnet/decompiler/csharp/syntax/modifiers.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

const std::array<Modifiers, 20>& CSharpModifiers::all_modifiers() {
    static const std::array<Modifiers, 20> modifiers = {
        Modifiers::Public,   Modifiers::Private,  Modifiers::Protected, Modifiers::Internal, Modifiers::New,
        Modifiers::Unsafe,   Modifiers::Static,   Modifiers::Abstract,  Modifiers::Virtual,  Modifiers::Sealed,
        Modifiers::Override, Modifiers::Required, Modifiers::Readonly,  Modifiers::Volatile, Modifiers::Ref,
        Modifiers::Extern,   Modifiers::Partial,  Modifiers::Const,     Modifiers::Async,    Modifiers::Any,
    };
    return modifiers;
}

std::string_view CSharpModifiers::get_modifier_name(Modifiers modifier) {
    switch (modifier) {
        case Modifiers::Private: return "private";
        case Modifiers::Internal: return "internal";
        case Modifiers::Protected: return "protected";
        case Modifiers::Public: return "public";
        case Modifiers::Abstract: return "abstract";
        case Modifiers::Virtual: return "virtual";
        case Modifiers::Sealed: return "sealed";
        case Modifiers::Static: return "static";
        case Modifiers::Override: return "override";
        case Modifiers::Readonly: return "readonly";
        case Modifiers::Const: return "const";
        case Modifiers::New: return "new";
        case Modifiers::Partial: return "partial";
        case Modifiers::Extern: return "extern";
        case Modifiers::Volatile: return "volatile";
        case Modifiers::Unsafe: return "unsafe";
        case Modifiers::Async: return "async";
        case Modifiers::Ref: return "ref";
        case Modifiers::Required: return "required";
        // even though it's used for pattern matching only, 'any' needs to be in this list to be usable in the AST
        case Modifiers::Any: return "any";
        default: throw std::invalid_argument("Invalid value for Modifiers");
    }
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
