/* supportrun.hpp -- the state of one run of the runtime support routines both use (clibext).
 *
 * What OpenWatcom kept in globals and statics is here: every function of this part of the compiler that reads or writes it takes
 * the run as its first parameter (`SupportRun& support`). A header's variables are members (their structs are in those headers), and each
 * .cpp file's own variables are a struct of its own that file defines, held by a pointer here and made and freed by the file
 * (`<part>_make`, `<part>_free`). The other parts of the compiler are reached through the pointers at the end; they are set when the
 * run begins (run/cbrun.cpp) and name the same run. */
#ifndef CB_SUPPORTRUN_HPP
#define CB_SUPPORTRUN_HPP

#include "../run/cbrun.hpp"

#include "clibext.h"
#include "clibint.h"
#include "clibext_state.h"
#include "idemsgfm_state.h"


struct SupportRun {
    ClibextState clibext;
    ClibintState clibint;
    ClibextFileState clibext_file;
    IdemsgfmState idemsgfm;

    FrontRun* front = nullptr;   /* (the same run's front) */
    BackRun* back = nullptr;   /* (the same run's back) */
    AsmRun* assembler = nullptr;   /* (the same run's asm) */

    Held* held = nullptr;   /* (what the run holds: its allocations and open files) */

    SupportRun();
    ~SupportRun();
    SupportRun(const SupportRun&) = delete;
    SupportRun& operator=(const SupportRun&) = delete;
};

#endif
