// globals.h -- the module's global variables and its tables of pointers (globals.cc; was the
// core of codebreaker/analyze/globals.py).
#pragma once
#include <string>

#include "image.h"

namespace cba {

// what: "variables" or "tables"; the answer as JSON in out -> false: unknown
bool globals_answer(Image &img, const char *what, std::string &out);

}  // namespace cba
