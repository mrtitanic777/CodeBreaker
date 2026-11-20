// translate/backends/c.hpp -- what the C back end decides, as data: the hub's back end that writes an L2 function document
// as C in the decompiler's own dialect (docs/IR_UNIFICATION.md, step 2).
//
// The decompiler's PrintL2 writes each function as L2. The writer that turns it back into C is the L2 core library (translate/l2,
// linked in, asked through translate/native_l2: `Library::function`, `module`, `type_text` ...); it writes the decompiler's type
// names (int2, uint4, xunknown2, code), its calling-convention words (__cdecl16far, __stdcall) and hexadecimal past 9, so what it
// writes goes through the decompiler's c_text passes as the decompiler's own C does, and is compiled and judged the same way.
//
// This module is what is left to say here: which of c_text's passes the writer does on the tree (`moved_passes` and the three
// sets of them a path has), how many bytes each of the decompiler's scalar type names is, and the name a parameter has in a
// document. A node the writer does not write is `native_l2::Declined`.
#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "translate/ir.hpp"
#include "translate/native_l2.hpp"

namespace cb::translate::backends::c {

// A node this back end does not write (the C back end is the core library's: its decline is the one native_l2 raises).
using Declined = native_l2::Declined;

// c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order), in the order the writer runs them.
std::span<const std::string_view> moved_passes();
// ... those of them that need what the module's facts say of the function (its model in the description, what its instructions
// read and start with): done only when the function is written as part of its module (c_text makes the module with them); the C
// the decompiler is first run for is written without them.
std::span<const std::string_view> module_passes();
// ... those of them the 32-bit path has (the flat compilable form).
std::span<const std::string_view> flat_passes();
// ... and those of the PowerPC path (the facts' "lane": "powerpc").
std::span<const std::string_view> powerpc_passes();

// The size in bytes of one of the decompiler's scalar type names (`int2` is 2, `float10` is 10); nothing for any other name.
std::optional<std::size_t> scalar_size(std::string_view type_name);

// Every scalar type name the decompiler writes and its size, in the order the decompiler lists them (for a caller that builds a
// table of its own).
struct ScalarSize {
    std::string_view name;
    std::size_t bytes;
};
std::span<const ScalarSize> scalar_sizes();

// The name a parameter has: its symbol's, empty if the document gives it none.
std::string param_name(const ir::Param& param);

}  // namespace cb::translate::backends::c
