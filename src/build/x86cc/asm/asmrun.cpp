/* asmrun.cpp -- see asmrun.hpp: a run of asm is made, linked to the other parts and freed here. */
#include "asmrun.hpp"

AsmRun::AsmRun() : asmdefs(), asmfixup(), asmglob() {
    asmdata = asmdata_make();
    asmeval = asmeval_make();
    asmins = asmins_make();
    asmscan = asmscan_make();
    asmsym = asmsym_make();
    tbyte = tbyte_make();
}

AsmRun::~AsmRun() {
    tbyte_free(tbyte);
    asmsym_free(asmsym);
    asmscan_free(asmscan);
    asmins_free(asmins);
    asmeval_free(asmeval);
    asmdata_free(asmdata);
}

AsmRun* asm_run_new() { return new AsmRun(); }
void asm_run_link(AsmRun& run, FrontRun* front, BackRun* back, SupportRun* support, Held* held, AsminlinState* asminlin) {
    run.front = front;
    run.back = back;
    run.support = support;
    run.held = held;
    run.asminlin = asminlin;
}
void asm_run_reset(AsmRun* run) { run->~AsmRun(); ::new (run) AsmRun(); }
void asm_run_delete(AsmRun* run) { delete run; }

AsminlinState* asminlin_new() { return new AsminlinState(); }
void asminlin_reset(AsminlinState* part) { part->~AsminlinState(); ::new (part) AsminlinState(); }
void asminlin_delete(AsminlinState* part) { delete part; }
