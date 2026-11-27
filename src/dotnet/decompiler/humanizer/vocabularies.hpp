// dotnet/decompiler/humanizer/vocabularies.hpp -- the one vocabulary the decompiler uses: the English rules, irregular words and
// uncountable words of Humanizer.
//
//     const Vocabulary& vocabulary = vocabularies::get_default();
//     vocabulary.singularize("people", false);      // "person"
//
// Ported from Humanizer (Humanizer.Inflections: Vocabularies, vendored in ILSpy's ICSharpCode.Decompiler/Humanizer, from
// Humanizer/Vocabularies.cs), Copyright (c) .NET Foundation and Contributors, MIT (see the LICENSE of Humanizer in ILSpy).
#pragma once

#include "dotnet/decompiler/humanizer/vocabulary.hpp"

namespace cb::dotnet::decompiler::humanizer::vocabularies {

// The default vocabulary used for singular/plural irregularities (built on first use).
const Vocabulary& get_default();

}  // namespace cb::dotnet::decompiler::humanizer::vocabularies
