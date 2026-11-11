// codebreaker/invert/early_c_round_trip.hpp -- C++17 port of codebreaker.invert.early_c_round_trip.
//
// early_c_round_trip.py - round-trip proof for the framemodel wiring.
//
// For each probe function: oracle-compile -> extract bytes -> mc2inv.recover -> to_c ->
// oracle-recompile -> masked byte-compare. Exercises unused-local gaps + temp-shifted
// locals, which the old `int v<off>` emission could not reproduce.
//
// Typing notes:
//   * extract_one answers the Python's (bytes, mask_offsets) or (None, None) as an optional pair; the mask is the
//     set of positions the linker fills in (None when the object lists none beyond the function: here an empty set).
//   * masked_eq(a, b, extra_mask) takes the offsets to ignore as an optional set (None: none).
//   * The compilers are invert/compilers (Microsoft C 2.03: settings kits.msc203).
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::invert::early_c_round_trip {

// functions that specifically stress local layout (gaps, temps, order)
extern const py::str BATTERY;

// compile a single-function C string, return (bytes, mask_offsets) for its function.
std::optional<std::tuple<py::bytes, std::optional<py::set<int64_t>>>> extract_one(const py::str& csrc);

std::tuple<bool, py::str> masked_eq(const py::bytes& a, const py::bytes& b, const std::optional<py::set<int64_t>>& extra_mask);

void main();

}  // namespace codebreaker::invert::early_c_round_trip
