// cb/java/signatures.hpp -- signatures.md, a single-file map of the whole game codebase.
//
// Every class, grouped by subsystem (package), with its inheritance, fields and method signatures, showing BOTH the
// Java spelling and the resolved C++03 type. It is made from what the converter itself sees (the parser's IR and the
// type resolver built from the whole corpus), so the map stays faithful to the conversion.
//
//     Codebase code = signatures::load(java_dir);                    // discover and parse, failures kept
//     const types::TypeResolver resolver = ...;                      // built from code.classes by the pipeline
//     std::string md = signatures::render(code, resolver, profile.package_roles);
//
// `render` is pure; `summary` is the text the command prints once the file is written.
#pragma once

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/ir.hpp"
#include "cb/java/types.hpp"

namespace cb::java::signatures {

// A package named without one is listed under this.
inline constexpr std::string_view kDefaultPackage = "(default package)";

// {package: one-line role}: game-specific data of the target profile.
using PackageRoles = std::map<std::string, std::string, std::less<>>;

// A Java file that did not parse.
struct ParseFailure {
    std::string file;      // the file's name (no folder)
    std::string message;
};

struct Codebase {
    std::vector<ir::JClass> classes;     // in the order of the sorted file names
    std::vector<ParseFailure> failures;
};

// Every .java under `java_dir` (one per file name, the first met wins: output is keyed by class name), parsed.
// NotFoundError when `java_dir` is not a folder.
Codebase load(const std::filesystem::path& java_dir);

// The role of a package: the profile's description, else a generic one.
std::string package_role(std::string_view package, const PackageRoles& roles, std::size_t class_count);

// The markdown: subsystem overview, then every class (packages sorted, the default package last; classes by name).
std::string render(const Codebase& code, const types::TypeResolver& resolver, const PackageRoles& roles);

// What the command prints after writing `out_path`: the file, the counts per package, the parse failures.
std::string summary(const Codebase& code, const std::filesystem::path& out_path);

}  // namespace cb::java::signatures
