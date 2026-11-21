// build/x86link/directives/common.hpp -- what the directives common to every format (common.cpp, wlink's cmdall.c) and the
// driver (driver.cpp, wlink's cmdline.c) give the format files (formats.hpp).
#pragma once

#include <string_view>

#include "build/x86link/directives/language.hpp"
#include "build/x86link/link/linkstd.h"

namespace cb::x86link::directives {

// ---- common.cpp (cmdall.c)
bool DoParseDirective();          // the next directive, any of them; false when the token is none
bool DoParseDirectiveSubset();    // ... only those a system block run after the command line may hold
bool DoParseDirectiveSuicide();   // ... any, and a word that is none stops the link (the command line)
bool DoMatchDirective(std::string_view word);   // the word is a directive (Novell's import files)
bool ProcLibrary();               // `library` (DOS's `fixedlib` runs it too)
void ChkBase(offset align);       // `option offset`: the base given is aligned to `align`, or warned of
outfilelist* NewOutFile(char* file_name);   // the output file of that name (one per name; the name is the list's after)

// ---- driver.cpp (cmdline.c)
void Syntax();                    // a directive not understood: said, and the link stopped
void Ignite();                    // stop parsing (help was given)

// The executable's name (`name`): wlink's memory, null when none was given.
char*& exe_name();

}  // namespace cb::x86link::directives

// The format, once decided, is one of these (also called from objpass1.cpp and wlink.cpp).
bool HintFormat(exe_format hint);
