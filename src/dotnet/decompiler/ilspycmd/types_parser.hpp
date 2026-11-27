// dotnet/decompiler/ilspycmd/types_parser.hpp -- what the -l option's values name: the kinds of type to list.
//
//     TypesParser::parse_selection({"c", "i"})            // {Class, Interface}
//     TypesParser::parse_selection({"cid"})               // letters: {Class, Interface, Delegate}
//     TypesParser::parse_selection({"classes", "enum"})   // words (the longest prefix that is one): {Class, Enum}
//
// Ported from ILSpy (ICSharpCode.ILSpyCmd: TypesParser, from TypesParser.cs), Copyright (c) 2017 Christoph Wille, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <set>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/known_type_reference.hpp"

namespace cb::dotnet::decompiler::ilspycmd {

class TypesParser {
public:
    // The kinds the values select. One value that does not start with a kind's word is read as one letter per kind (c, i, s, d, e);
    // otherwise each value is cut back until it is a word (class, struct, interface, enum, delegate; any case) and a value that
    // never is one selects nothing.
    static std::set<typesystem::TypeKind> parse_selection(const std::vector<std::string>& values);
};

}  // namespace cb::dotnet::decompiler::ilspycmd
