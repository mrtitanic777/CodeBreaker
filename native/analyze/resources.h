// resources.h -- the resources the code loads and the dialog controls it touches (resources.cc;
// was the core of codebreaker/analyze/resources.py).
#pragma once
#include <string>

#include "image.h"

namespace cba {

// what: "uses"; the answer as JSON in out -> false: unknown
bool resources_answer(Image &img, const char *what, std::string &out);

}  // namespace cba
