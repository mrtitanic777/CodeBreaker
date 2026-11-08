// codebreaker/exact/omf.hpp -- typed C++ of codebreaker.exact.omf
//
// omf.py -- now codebreaker/formats/omf.py (one OMF-86 reader for exact, invert and the builders);
// this name is kept for exact's imports and is the same module.
//
// The Python replaces itself in sys.modules with codebreaker.formats.omf, so `exact.omf` IS that
// module: every function, class and constant is the formats one.  Here that is a namespace alias:
// codebreaker::exact::omf::read is codebreaker::formats::omf::read (one reader, one set of types).
#pragma once

#include "codebreaker/formats/omf.hpp"

namespace codebreaker::exact {

namespace omf = codebreaker::formats::omf;

}  // namespace codebreaker::exact
