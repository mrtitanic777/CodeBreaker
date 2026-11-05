// call_graph.h -- who calls whom, what refers to what, what the exports reach (call_graph.cc).
#pragma once
#include <string>

#include "image.h"

namespace cba {

// the graph of a discovered image, as JSON (analyze/call_graph.py's Graph reads it)
std::string call_graph(Image &img);

}  // namespace cba
