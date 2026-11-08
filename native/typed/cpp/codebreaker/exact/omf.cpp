// codebreaker/exact/omf.cpp -- typed C++ of codebreaker.exact.omf (see omf.hpp).
//
// The module has no code of its own: it is the same module as codebreaker.formats.omf.  What this file
// holds is the check that the alias is that module, at compile time.

#include "codebreaker/exact/omf.hpp"

#include <type_traits>

namespace codebreaker::exact {

static_assert(std::is_same_v<omf::Obj, codebreaker::formats::omf::Obj>, "exact.omf is formats.omf");
static_assert(std::is_same_v<omf::Walk, codebreaker::formats::omf::Walk>, "exact.omf is formats.omf");

}  // namespace codebreaker::exact
