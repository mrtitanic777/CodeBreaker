// dotnet/decompiler/il/transforms/tuple_transform.hpp -- matchers for C# tuples in the ILAst: a field access of a tuple element
// (`ldflda Item1(ldflda Rest(target))`, with the nested Rest fields of long tuples flattened) and the construction of a tuple
// (`newobj ValueTuple(...)`, the nested constructions of long tuples flattened into one argument list).
//
//     int position; const IType* tuple_type; ILInstruction* target;
//     if (TupleTransform::match_tuple_field_access(ldflda, &tuple_type, &target, &position)) ...
//     std::vector<ILInstruction*> elements;
//     if (TupleTransform::match_tuple_construction(newobj, &elements)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: TupleTransform, from Transforms/TupleTransform.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's class lives in the IL namespace; it is declared in transforms (where
// the file is) and made visible in il.
#pragma once

#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class TupleTransform {
public:
    // Matches an 'ldflda' instruction accessing a tuple element, e.g. `ldflda Item1(ldflda Rest(target))`. The tuple type is the one
    // declaring the first field, the position is 1-based over the flattened tuple; a false result may have set the outputs already.
    static bool match_tuple_field_access(LdFlda* inst, const IType** tuple_type, ILInstruction** target, int* position);

    // Matches 'newobj TupleType(...)', taking care of flattening long tuples. On a false result `arguments` may be partly filled.
    static bool match_tuple_construction(NewObj* newobj, std::vector<ILInstruction*>* arguments);
};

}  // namespace cb::dotnet::decompiler::il::transforms

namespace cb::dotnet::decompiler::il {
using transforms::TupleTransform;
}
