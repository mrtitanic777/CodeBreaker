// conventions.h -- how each function is called and what kind of function it is (conventions.cc).
#pragma once
#include <string>

#include "image.h"

namespace cba {

// one fact per function of a discovered, named image, as a JSON list (analyze/conventions.py)
std::string conventions(Image &img);

}  // namespace cba
