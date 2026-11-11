// cb/port/old_sizes.hpp -- how many bytes a declaration had in the original program (the 8086 compilers').
//
// An int was 2 bytes, a long 4, a near pointer 2 and a far one 4; a structure is the sum of its members with a word
// on an even offset. The port's passes ask these sizes to find which numbers in a program are the sizes of its
// structures (and so must change when the new structures are bigger).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/port/pre_ansi_types.hpp"

namespace cb::port::pre_ansi {

// The size of a scalar type in the table size_of reads (the fixed-width names and the old compilers' words:
// `int` 2, `long` 4, `double` 8, `int16_t` 2); empty for a name that is not in it.
std::optional<std::uint32_t> scalar_size(std::string_view type);
// The size of a scalar of the original program (`unsigned long int` 4, `double` 8, `signed` 2); empty for a name
// that is not a scalar.
std::optional<std::uint32_t> old_scalar_size(std::string_view type);

// The two tables, in order (for a caller that wants to list them).
std::vector<std::pair<std::string_view, std::uint32_t>> scalar_sizes();
std::vector<std::pair<std::string_view, std::uint32_t>> old_scalar_sizes();

// The product of the numbers in the declarator's `[n]` suffixes (1 when it has none).
std::optional<std::uint64_t> array_count(std::string_view declarator);

// The size a declaration had in the original: `base` the type, `decl` the declarator, `name` its name, `ptr` the
// bytes of a pointer there. Empty when the type is not known (not a scalar of the table, not a structure in `structs`).
std::optional<std::uint64_t> size_of(std::string_view base, std::string_view decl, std::string_view name, unsigned ptr,
                                     const StructLayouts& structs);

// The same with the structures' sizes given by a function (the size of `struct s`, if it has one).
using StructSizeFn = std::function<std::optional<std::uint64_t>(std::string_view tag)>;
std::optional<std::uint64_t> size_of(std::string_view base, std::string_view decl, std::string_view name, unsigned ptr,
                                     const StructSizeFn& struct_size);

}  // namespace cb::port::pre_ansi
