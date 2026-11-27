// dotnet/decompiler/debug_info/i_debug_info_provider.hpp -- what the decompiler asks of debug information (a PDB): the sequence points of a method,
// its local variables' names, and the extra type information (tuple element names, dynamic flags) a PDB keeps.
//
//     class MyProvider : public IDebugInfoProvider { ... };      // reads a portable PDB, or answers from nothing
//     for (const SequencePoint& sp : provider.get_sequence_points(method)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.DebugInfo: IDebugInfoProvider, Variable, PdbExtraTypeInfo, from IDebugInfoProvider.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The methods are const (a provider reads, and caches behind a mutex
// of its own); a method's handle is the metadata reader's. Upstream's `out` parameters are pointers.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/debug_info/sequence_point.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler::debug_info {

// A local variable of a method: its index and its name.
struct Variable {
    int index = 0;
    std::string name;
    Variable() = default;
    Variable(int index_, std::string name_) : index(index_), name(std::move(name_)) {}
};

// What a PDB says about a type beyond its signature: the names of a tuple's elements and which parts of the type are `dynamic`.
struct PdbExtraTypeInfo {
    std::vector<std::string> tuple_element_names;
    std::vector<bool> dynamic_flags;
};

class IDebugInfoProvider {
public:
    virtual ~IDebugInfoProvider() = default;
    virtual std::string description() const = 0;
    virtual std::vector<SequencePoint> get_sequence_points(cb::dotnet::metadata::MethodDefinitionHandle method) const = 0;
    virtual std::vector<Variable> get_variables(cb::dotnet::metadata::MethodDefinitionHandle method) const = 0;
    // The name of the local variable `index` of the method; false when the debug information has none.
    virtual bool try_get_name(cb::dotnet::metadata::MethodDefinitionHandle method, int index, std::string* name) const = 0;
    virtual bool try_get_extra_type_info(cb::dotnet::metadata::MethodDefinitionHandle method, int index, PdbExtraTypeInfo* extra_type_info) const = 0;
    virtual std::string source_file_name() const = 0;
};

}  // namespace cb::dotnet::decompiler::debug_info
