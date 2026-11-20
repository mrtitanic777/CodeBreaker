// invert/production.hpp -- the environment `codebreaker invert` runs in for real: every compiler its tools put a model's text to, and the
// describer that finds a module's functions.
//
//     const auto environment = cb::invert::command::production(config, corpus);
//     return cb::invert::command::run(args, environment);
//
// What is bound, and to what (a hook whose tool is not set up on this machine stays empty, and the tool that wants it says so):
//
//     retail           the corpus's copy of a module by name
//     compile_msc6     Microsoft C 6.0a (build/msc6: CL.EXE of settings.json `msc6` in DOSBox-X `dosbox_x`), one text, or many in one session
//     compile_msvc152  Microsoft C/C++ 8 of Visual C++ 1.52c (build/msc6, `msvc152`), which runs on Windows itself where its short paths exist
//     compile_mc203    Microsoft C 2.03 (exact/toolchain's profile "msc203", kit `kits.msc203`) on the DOS executive, in this process
//     compile_msvc2    the 32-bit compiler of 1994 (CL.EXE in settings.json `period_compiler`), one function at a time
//     functions        a 16-bit module's functions as the describer finds them (decompile/describe): where they are, their bytes, the places the linker
//                      fills in, the SDK's signatures of the exported ones and how the module's own calls give the others
//
// What a compile makes is kept in the build cache (the compilers' results are filed under the text, the switches and the compiler's own bytes), so
// the same text put to the same compiler is not compiled again (CB_NO_CACHE turns that off).
#pragma once

#include <memory>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "invert/command.hpp"

namespace cb::invert::command {

// The environment of this checkout. `corpus` may be null: the corpus of the configuration's products is then made. The environment owns what it opens (the cache, the
// DOS executive, the header tables) and shares it between its hooks; copies of it share it too.
Environment production(const config::Config& config, std::shared_ptr<base::Corpus> corpus = nullptr);

}  // namespace cb::invert::command
