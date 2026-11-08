// codebreaker/invert/early_c_frame.hpp -- C++17 port of codebreaker.invert.early_c_frame.
//
// early_c_frame.py - deterministic model of MC2's stack-frame allocator.
//
// Recovered empirically (allocprobe.py, 30/30 ground-truth functions) and now the
// deterministic inverse-of-the-allocator: given a function's temporaries (from the quad
// temp operands) and its declared locals (in declaration order), compute the exact
// [bp+offset] each gets and the prologue framesize (`sub sp,N`).
//
// THE RULE (byte-exact vs MC1/MC2 oracle):
//   frame above bp = [ TEMP region ][ LOCAL region ], both growing UP from bp+0.
//     * temps  : one slot per temp NODE, in creation order, sized by type.
//     * locals : declared locals/arrays in DECLARATION ORDER (incl. nested-block
//                locals, appended), sized by type. NO inter-field alignment padding.
//   framesize = temp_bytes + local_bytes, ROUNDED UP TO EVEN.
//   params    : i-th param sits at bp + framesize + 4 + (sum of prior param sizes).
//
// Type sizes: char=1, int=2, long=4, float=4, double=8, pointer=2 (small model).
// Arrays/structs: element_size * count (contiguous, no trailing pad beyond frame rounding).
//
// The copy-swap functions (_chain_swaps, apply_copy_swaps, infer_decl_order) work on any hashable,
// orderable offset key; the Python calls them with int offsets (the compiler's frame offsets) and the
// tests also call apply_copy_swaps with str names. Both key types are provided (K = int64_t, py::str).
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::early_c_frame {

// type name -> size in bytes
extern const py::dict<py::str, int64_t> SIZE;

int64_t tsize(const py::str& t, int64_t count = 1);

// temps  : list of type-strings, in creation order (e.g. ['int','long']).
// locals_: list of (name, type, count) in declaration order (count>=1; arrays >1).
// params : list of (name, type) in order, or None.
// returns (offsets: dict name->off, framesize, param_offsets: dict, temp_bytes).
std::tuple<py::dict<py::str, int64_t>, int64_t, py::dict<py::str, int64_t>, int64_t>
layout(const py::list<py::str>& temps, const py::list<std::tuple<py::str, py::str, int64_t>>& locals_,
       const std::optional<py::list<std::tuple<py::str, py::str>>>& params = std::nullopt);

// Resolve live copies into per-CHAIN endpoint swaps. A copy `dest=src` is an edge
// src->dest; a maximal chain v0->v1->...->vk swaps only its ENDPOINTS (v0<->vk), leaving
// intermediates fixed (validated: `a=11;b=a;c=b` -> a@4,b@2,c@0, i.e. a<->c). A single
// copy is a length-1 chain (src<->dest). Independent copies are separate chains.
// Returns a list of (offA, offB) endpoint swaps.
template <class K> py::list<std::tuple<K, K>> _chain_swaps(const py::list<std::tuple<K, K>>& copies);

// FORWARD model of MC2's copy-reversal: locals default to declaration order / ascending
// offsets; each live-copy chain swaps its endpoints' offsets. Returns {declared_off:
// final_off}. `offsets` is iterated for its keys (the declared offsets).
template <class K> py::dict<K, K> apply_copy_swaps(const py::dict<K, int64_t>& offsets, const py::list<std::tuple<K, K>>& copies);

// INVERSE: declaration order (list of offsets) to emit so the compiler's copy-chain
// endpoint swaps reproduce each var at its own offset. Swaps are involutions on disjoint
// endpoints, so re-applying them to the identity yields the pre-swap declaration layout.
template <class K> py::list<K> infer_decl_order(const py::list<K>& offsets, const py::list<std::tuple<K, K>>& copies);

// INVERSE use: given the referenced local bp-offsets and the framesize, produce C
// declarations (IN DECLARATION ORDER) that reproduce the exact local-region layout on
// recompile. An `int` (or `long`, if in longset) sits at each referenced offset; `char`
// filler arrays fill gaps (unused locals) and the trailing region up to framesize.
// The temp region below is reproduced automatically by the matching expressions, so we
// only lay out [min_offset .. framesize). Assumes the first declared local is referenced
// (local-region base == smallest offset), which holds for the modeled class.
// Returns (ordered_decls, names): ordered_decls = [(ctype, name, count), ...].
std::tuple<py::list<std::tuple<py::str, py::str, int64_t>>, py::list<py::str>>
emit_local_decls(const py::list<int64_t>& offsets, int64_t framesize,
                 const std::optional<py::set<int64_t>>& longset = std::nullopt,
                 const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies = std::nullopt);

py::str fmt_decl(const py::str& t, const py::str& name, int64_t count);

// ---- self-validation against the two allocprobe batteries (observed ground truth) ----
// each: (temps, locals[(name,type,count)], params, expected_fs, expected_referenced_offsets)
// (the first element is the case's name)
using Case = std::tuple<py::str, py::list<py::str>, py::list<std::tuple<py::str, py::str, int64_t>>,
                        py::list<std::tuple<py::str, py::str>>, int64_t, py::list<int64_t>>;
extern const py::list<Case> CASES;

// Prints the failures and the "framemodel self-test: N/M byte-exact frame layouts" line to stdout.
bool _self_test();

// The module's `if __name__=="__main__":` block.
void cli();

}  // namespace codebreaker::invert::early_c_frame
