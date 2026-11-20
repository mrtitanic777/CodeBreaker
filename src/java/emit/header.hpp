// java/emit/header.hpp -- render a class's .h from the IR and a type resolver.
//
// Decides includes against forward declarations:
//   * parent / interfaces                          -> #include (the full definition is needed to inherit)
//   * value-type members and by-value params/returns -> #include (the full definition is needed)
//   * reference-type (pointer) members/params/returns -> a forward `class X;`
//   * std-mapped types                              -> the right <string>/<vector>/<map>/<set> system header
//
// Every lossy mapping (Object -> void*, NIO buffers) is recorded on the ledger as an ANNOTATED event so the report
// shows it. Three shapes of class come out: a typed enum (constants that carry state or behaviour: a real class with a
// singleton per constant), a constant-only enum (a struct of static const ints), and an ordinary class or interface.
//
//     header::Options options;  options.cpp_std = header::CppStd::of_year(2011);  options.ledger = &ledger;
//     const std::string h = header::render_header(cls, resolver, options);
#pragma once

#include <optional>
#include <set>
#include <string>
#include <string_view>

#include "java/ir.hpp"
#include "java/ledger.hpp"
#include "java/typemap.hpp"
#include "java/types.hpp"

namespace cb::java::emit::header {

// The C++ standard the output targets: its year (2003, 2011, 2014 ...) and its label ("C++03", "C++11").
struct CppStd {
    int year = 2003;
    std::string label = "C++03";

    static CppStd of_year(int year);   // the label is `C++` and the last two digits of the year
};

// What a rendering depends on besides the class and the resolver.
struct Options {
    std::optional<CppStd> cpp_std;   // none: C++03 in the banner and no standard-specific rewrites
    std::string platform_label;      // for the banner; empty reads "unknown"
    // Classes built in two phases (the two-phase pass): the constructor only zeroes the fields and each Java
    // constructor becomes `j2xInit(...)`.
    std::set<std::string, std::less<>> two_phase;
    // Scalar `byte` fields forced back to `unsigned char` ((class, field) pairs; none by default).
    std::set<typemap::ClassField> unsigned_byte_scalar_fields;
    ledger::Ledger* ledger = nullptr;   // where the lossy mappings and risks are recorded; none: not recorded
};

// '// Auto-generated from <class>.java\n// Target: <platform> / <C++ standard>'.
std::string banner(std::string_view class_name, const std::optional<CppStd>& cpp_std, std::string_view platform_label);

// The #include operand (quoted or in angle brackets) for a std-mapped C++ type, or nullopt. Object -> JObject (the
// universal boxed value type) is a name-changing mapping, so it lands here rather than as a user X.h. JObject lives in
// the runtime prelude; a header that takes or returns it BY VALUE (`equals(JObject)`) needs the full definition, so
// JavaRT.h is pulled in -- without it the header has neither an include nor a forward declaration for JObject and
// fails to compile standalone.
std::optional<std::string> std_header_for(std::string_view cpp_base);

// The C++ base-class spelling of a Java class that EXTENDS a container (`extends ArrayList`): ArrayList/List ->
// JList<JObject> (JList.h is always included); HashMap/Map -> JMap<JObject, JObject>; HashSet/Set/TreeSet ->
// JSet<JObject>. A parent that is not a container is returned unchanged.
std::string base_type_spelling(std::string_view parent);

struct Dependencies {
    std::set<std::string> system_includes;   // `<string>`, `"JavaRT.h"`: as written after #include
    std::set<std::string> includes;          // class names whose X.h is included
    std::set<std::string> forward_decls;     // class names declared `class X;`
};

Dependencies collect_dependencies(const ir::JClass& cls, const types::TypeResolver& resolver);

// The declaration of one method of `cls`: `Cls(params);`, `virtual int f(params) = 0;`, `static void g();`.
std::string method_decl(const ir::JClass& cls, const ir::Method& method, const types::TypeResolver& resolver);

std::string render_header(const ir::JClass& cls, const types::TypeResolver& resolver, const Options& options = {});

}  // namespace cb::java::emit::header
