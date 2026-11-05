// census.hpp -- what a Python tree uses (the libraries, their attributes, builtins, methods and the
// language features), as Markdown: the dynamic mode's to-do list (see census.cc).
#pragma once
#include <string>
#include <vector>

namespace pyengine {

// roots: package folders (codebreaker/, tests/ ...); internal_top: the import name that is the tree's own
std::string run_census(const std::vector<std::string>& roots, const std::string& internal_top);

}  // namespace pyengine
