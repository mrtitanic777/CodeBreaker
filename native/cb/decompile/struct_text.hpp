// cb/decompile/struct_text.hpp -- which field accesses are fields: the module's decision, for the C back end.
//
//     Plan plan = struct_text::plan(module_path, docs, by_va, declared, analysis);
//     header += struct_text::declarations(plan, used);
//
// The decompiler writes what the machine does: `*(int4 *)(a1 + 0x48)`. A person wrote `p->width`. Which pointers
// are which structure is worked out from the whole module's code (analyze/structures: an argument handed on is
// the same structure in the function that receives it); here that is decided for the module from its functions'
// trees, and the C back end writes it. For every argument known to point at a structure, an access through it at
// a constant offset becomes a field of that structure:
//
//     *(int4 *)(a1 + 0x48)        ->   ((struct_7 *)a1)->f_48
//
// and the structure is declared in the header with each field at its offset, gaps as padding. The field's type
// is the type most of the module's accesses to that offset use; an access with another type (one byte of a word,
// a signed read of an unsigned field) is left as it was, so every rewritten access means exactly what it meant:
// the same bytes, read as the same type. Nothing else changes -- the argument stays the number the decompiler
// made it.
//
// Fields have no names in a binary; `f_48` says where the field is. Names are for a person (or a header that
// documents the structure) to give.
//
// The module reaches three collaborators that are not part of it: the image loader and structure recovery, and
// the C back end's parameter_names / struct_accesses over a function's L2 document. They are handed in as
// `Analysis`; a function's document is named by the function (`docs` is the set of functions that have one).
#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace cb::decompile::struct_text {

// A field's type as written in the C ("int4", "uint1", "int4 *") and its size in bytes.
struct FieldType {
    std::string type;
    std::uint32_t size = 0;
    friend auto operator<=>(const FieldType&, const FieldType&) = default;
};

// The field type of an access `*(base stars)...` where `stars` is the number of `*` in the access's pointer type:
// the base type itself for a scalar (stars <= 1, a name whose size is known), "base **" (four bytes) for a
// pointer to a pointer; nullopt when the access is not one a field can be made of.
std::optional<FieldType> kind(std::string_view base, std::size_t stars);

enum class RootKind { Arg, This, Other };

// One place that is known to point at a structure: argument `index` of the function at `va`, or the `this`
// object of the member function at `va` (no index). `Other` kinds are ignored.
struct Root {
    RootKind kind = RootKind::Other;
    std::uint64_t va = 0;
    std::optional<std::int64_t> index;
};

// A structure analyze/structures recovered: its name and the places known to point at it.
struct Structure {
    std::string name;
    std::vector<Root> roots;
};

// One access `*(base stars)(param + offset)` through a parameter.
struct Access {
    std::string base;
    std::size_t stars = 0;
    std::int64_t offset = 0;
};

struct Analysis {
    // Recover the module's structures; nullopt when the analysis cannot (the plan is then empty).
    std::function<std::optional<std::vector<Structure>>(std::string_view module_path)> recover;
    // The parameter names of a function's L2 document, in order.
    std::function<std::vector<std::string>(const std::string& function)> parameter_names;
    // The accesses a function's tree makes through one parameter.
    std::function<std::vector<Access>(const std::string& function, const std::string& param)> struct_accesses;
};

// {offset: field}, offsets ascending. Signed: a negative offset is an access before the pointer.
using Fields = std::map<std::int64_t, FieldType>;

struct PlannedParam {
    std::string structure;
    Fields fields;
};

// function -> parameter -> the structure it points at and that structure's fields.
using Plan = std::map<std::string, std::map<std::string, PlannedParam>>;

// The accesses through each function's parameters that are a structure's fields, decided over the module: `docs`
// the functions that have an L2 document, `by_va` function names by address, `declared` each function's heading
// as written. Nothing when the analysis fails: this is for reading, never for meaning.
Plan plan(std::string_view module_path, const std::set<std::string>& docs,
          const std::map<std::uint64_t, std::string>& by_va, const std::map<std::string, std::string>& declared,
          const Analysis& analysis);

// The header's declarations of the structures `used` (the ones a function was written with a field of), each
// field at its offset, gaps as padding, in the order of the number a name ends in (a name without one first, by
// name). A name in `used` that the plan has no fields for is a caller's bug (UsageError).
std::string declarations(const Plan& plan, const std::set<std::string>& used);

}  // namespace cb::decompile::struct_text
