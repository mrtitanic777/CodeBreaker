// symbols.h -- inside cbsymbols.dll: the questions codebreaker/symbols/ asked, answered in C++.
#pragma once
#include <string>

namespace cbs {
// symbols/demangle: a decorated name's parse as a JSON object (demangle.parse's dict), or false
bool demangle_parse(const std::u32string& name, std::string& json);
// symbols/demangle: an RTTI type name (`.?AVCFoo@@`) as a JSON string, or false
bool demangle_type_name(const std::u32string& decorated, std::string& json);
// symbols/prototypes: the prototype table read from the headers of `folders` (lines of
// "<bits> TAB <folder>", in rank order) as JSON {"32": {name: prototype}, "16": {...}}
bool prototypes_build(const std::u32string& folders, std::string& json);
}
