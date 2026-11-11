// cb/invert/early_c_frame.hpp -- a deterministic model of MC2's stack-frame allocator.
//
// Recovered empirically (30 of 30 ground-truth functions) and now the deterministic inverse of the allocator: given a
// function's temporaries (from the quad temp operands) and its declared locals (in declaration order), compute the
// exact [bp+offset] each gets and the prologue's frame size (`sub sp,N`).
//
// THE RULE (byte-exact against the MC1/MC2 oracle):
//   frame above bp = [ TEMP region ][ LOCAL region ], both growing UP from bp+0.
//     * temps  : one slot per temp NODE, in creation order, sized by type.
//     * locals : declared locals and arrays in DECLARATION ORDER (nested-block locals appended), sized by type. No
//                padding between fields.
//   frame size = temp bytes + local bytes, ROUNDED UP TO EVEN.
//   params     : the i-th param sits at bp + frame size + 4 + (the sizes of the params before it).
//
// Type sizes (small model): char 1, int 2, long 4, float 4, double 8, pointer 2. A type the table does not know is
// two bytes. Arrays and structs are element size times count, contiguous, with no padding beyond the frame rounding.
//
// Frame offsets are non-negative: the compiler allocates upward from bp.
#pragma once

#include <cstddef>
#include <cstdint>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::invert::early_c_frame {

using Offset = std::uint32_t;   // a byte offset from bp

// The size in bytes of `count` objects of a C type name (small model); 2 for a type the table does not know.
std::uint32_t type_size(std::string_view type, std::uint32_t count = 1);

struct Local {
    std::string name;
    std::string type;
    std::uint32_t count = 1;   // more than 1 for an array
};
struct Param {
    std::string name;
    std::string type;
};

struct Layout {
    OrderedMap<std::string, Offset> local_offsets;   // by declaration order (a name declared twice keeps the last offset)
    std::uint32_t frame_size = 0;                    // `sub sp,N`
    OrderedMap<std::string, Offset> param_offsets;
    std::uint32_t temp_bytes = 0;                    // the size of the temp region at the bottom
};
// temps: the type of each temporary, in creation order (e.g. {"int", "long"}).
Layout layout(std::span<const std::string> temps, std::span<const Local> locals, std::span<const Param> params = {});

// A live local-to-local copy `dest = src` (both frame offsets).
struct Copy {
    Offset dest = 0;
    Offset src = 0;
};

// Resolve live copies into per-CHAIN endpoint swaps. A copy is an edge src -> dest; a maximal chain v0 -> v1 -> ... -> vk
// swaps only its ENDPOINTS (v0 <-> vk) and leaves the intermediates fixed (validated: `a=11; b=a; c=b` puts a at 4, b at
// 2, c at 0, i.e. a <-> c). A single copy is a chain of length one (src <-> dest); independent copies are separate chains.
// A copy that closes a cycle is a chain with no origin and swaps nothing. Returns the (offset, offset) swaps.
std::vector<std::pair<Offset, Offset>> chain_swaps(std::span<const Copy> copies);

// The FORWARD model of MC2's copy reversal: locals default to declaration order (ascending offsets) and each live-copy
// chain swaps its endpoints' offsets. Returns declared offset -> final offset, in the order of `offsets`.
OrderedMap<Offset, Offset> apply_copy_swaps(std::span<const Offset> offsets, std::span<const Copy> copies);

// The INVERSE: the declaration order (offsets, in the order to declare them) that makes the compiler's copy-chain
// swaps put each variable at its own offset. Swaps are involutions on disjoint endpoints, so re-applying them to the
// identity yields the pre-swap declaration layout.
std::vector<Offset> infer_decl_order(std::span<const Offset> offsets, std::span<const Copy> copies);

struct Decl {
    std::string type;
    std::string name;
    std::uint32_t count = 1;   // more than 1 for an array
};

// INVERSE use: given the referenced local bp-offsets and the frame size, the C declarations (IN DECLARATION ORDER) that
// reproduce the exact local-region layout on recompile. An `int` (a `long` if the offset is in `longs`) sits at each
// referenced offset, named v<offset>; `char` filler arrays _p<offset> fill gaps (unused locals) and the region up to the
// frame size. The temp region below is reproduced by the matching expressions, so only [lowest offset, frame size) is
// laid out; the first declared local is assumed referenced (the local region starts at the smallest offset), which holds
// for the modeled class. With copies and a contiguous layout the declaration order is the inferred one (see
// infer_decl_order) and there are no fillers.
std::vector<Decl> emit_local_decls(std::span<const Offset> offsets, std::uint32_t frame_size,
                                   const std::set<Offset>& longs = {}, std::span<const Copy> copies = {});

// `int v0;` or `char _p4[3];`
std::string format_decl(const Decl& decl);

}  // namespace cb::invert::early_c_frame
