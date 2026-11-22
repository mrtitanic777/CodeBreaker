/* asmrun.hpp -- the state of one run of the assembler of inline assembly (the wasm engine).
 *
 * What OpenWatcom kept in globals and statics is here: every function of this part of the compiler that reads or writes it takes
 * the run as its first parameter (`AsmRun& assembler`). A header's variables are members (their structs are in those headers), and each
 * .cpp file's own variables are a struct of its own that file defines, held by a pointer here and made and freed by the file
 * (`<part>_make`, `<part>_free`). The other parts of the compiler are reached through the pointers at the end; they are set when the
 * run begins (run/cbrun.cpp) and name the same run. */
#ifndef CB_ASMRUN_HPP
#define CB_ASMRUN_HPP

#include "../run/cbrun.hpp"

#include "asmglob.h"
#include "asmdefs.h"
#include "asmfixup.h"
#include "asminlin.h"

struct AsmdataState;
AsmdataState* asmdata_make();
void asmdata_free(AsmdataState* part);
struct AsmevalState;
AsmevalState* asmeval_make();
void asmeval_free(AsmevalState* part);
struct AsminsState;
AsminsState* asmins_make();
void asmins_free(AsminsState* part);
struct AsmscanState;
AsmscanState* asmscan_make();
void asmscan_free(AsmscanState* part);
struct AsmsymState;
AsmsymState* asmsym_make();
void asmsym_free(AsmsymState* part);
struct TbyteState;
TbyteState* tbyte_make();
void tbyte_free(TbyteState* part);

struct AsmRun {
    AsmdefsState asmdefs;
    AsmfixupState asmfixup;
    AsmglobState asmglob;

    AsmdataState* asmdata = nullptr;
    AsmevalState* asmeval = nullptr;
    AsminsState* asmins = nullptr;
    AsmscanState* asmscan = nullptr;
    AsmsymState* asmsym = nullptr;
    TbyteState* tbyte = nullptr;

    AsminlinState* asminlin = nullptr;   /* (shared with the other part that names it: the run owns it) */
    FrontRun* front = nullptr;   /* (the same run's front) */
    BackRun* back = nullptr;   /* (the same run's back) */
    SupportRun* support = nullptr;   /* (the same run's support) */

    Held* held = nullptr;   /* (what the run holds: its allocations and open files) */

    AsmRun();
    ~AsmRun();
    AsmRun(const AsmRun&) = delete;
    AsmRun& operator=(const AsmRun&) = delete;
};

#endif
