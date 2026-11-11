// cb/port/frames.hpp -- a local array the program reads past the end of: the locals it reaches, side by side.
//
// The 8086 compilers put a function's locals on the stack one after another, and some programs counted on it: a
// name buffer handed to a function that takes the pointer for a structure's, whose next member is the frame's next
// variable. Compilers of today place locals as they like. `frames` finds such a call and gathers the locals it
// reaches into one structure of their own, in the order the original's frame had them.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

// A local variable of a function's head: `base declarator;`
struct LocalDecl {
    std::string base;                    // `register struct s`
    std::string declarator;              // `buf[64]`, `*p`
    std::optional<std::uint64_t> size;   // its bytes in the original, when known
    std::size_t line = 0;                // the line it is declared on
    bool is_register = false;
};

// The declarations heading the body that opens on line `open` and ends on `last`, in their order; the head ends at
// the first line that is not a declaration of type words. `ptr` is the original's pointer size in bytes.
OrderedMap<std::string, LocalDecl> local_decls(const std::vector<std::string>& code, std::size_t open, std::size_t last,
                                               unsigned ptr = 2);

// Rewrites, in each .c file of `out`, the locals a call reaches past an array into one `struct { ... } frame;`.
// `fns` are the program's functions (their bodies and parameters say what a callee reads past its pointer),
// `pointer_bytes` the profile's old pointer size (0: the default, 2) and `layout` lays out the structures of the files (lay_out_structs when empty).
void frames(Files& out, const OrderedMap<std::string, Fn>& fns, unsigned pointer_bytes, const LayoutFn& layout, Notes& notes);

}  // namespace cb::port::pre_ansi
