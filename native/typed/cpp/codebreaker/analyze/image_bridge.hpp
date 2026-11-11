// codebreaker/analyze/image_bridge.hpp -- adapters between analyze::image::Image and the private Image stand-ins of the
// already-ported analyses (call_graph, strings, globals), until those take analyze::image types themselves.
//
// call_graph::Image, strings::Image and globals::Image are abstract classes naming the part of codebreaker.analyze.image
// each reads (and the questions it asks the DLL, answered here by reading cbanalyze.dll's JSON). The analyses of group 1
// (dispatch, structures, classes, conventions) call call_graph::build, strings::at / table / literal and
// globals::pointer_tables through these. The adapter of an image is made once and kept in the image's memo.
//
// What the ported analyses would need to drop their stand-ins: take `const analyze::image::Image&` (call_graph:
// functions, imports, roots, spans, discovered, function(), label(), the call_graph.build question; strings / globals:
// bits, data_segment, span(), label() and their questions) and read a span through a pointer, not by value --
// strings::Image::span() answers std::optional<Span>, a copy of the whole span's bytes on every call.
#pragma once

#include "codebreaker/analyze/call_graph.hpp"
#include "codebreaker/analyze/globals.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/strings.hpp"

namespace codebreaker::analyze::bridge {

// the image as call_graph sees it (made once per image)
call_graph::Image& call_graph_of(image::Image& img);
// the image as strings / globals see it (made once per image)
globals::Image& globals_of(image::Image& img);

// call_graph::build(img)
std::shared_ptr<call_graph::Graph> graph(image::Image& img);
// strings::at(img, va, least), reading the span in place (strings::at through the adapter copies the span)
std::optional<std::tuple<py::str, py::str>> string_at(image::Image& img, int64_t va, int64_t least = 2);
// strings::table(img)
strings::Table string_table(image::Image& img);
// strings::literal(img, args, k) for the operands image::arguments gave
std::optional<std::tuple<int64_t, py::str, py::str>> string_literal(image::Image& img, const py::list<image::Op>& args, int64_t k);
// globals::pointer_tables(img)
globals::Tables pointer_tables(image::Image& img);

}  // namespace codebreaker::analyze::bridge
