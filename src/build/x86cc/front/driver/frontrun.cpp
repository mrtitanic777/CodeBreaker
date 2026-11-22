/* frontrun.cpp -- see frontrun.hpp: a run of front is made, linked to the other parts and freed here. */
#include "frontrun.hpp"

FrontRun::FrontRun() : callinfo(), cerrs(), cvars(), cdecl2(), csym(), ctype(), cdump(), cerror(), cmsg(), ccmain(), cfeinfo(), ideentry(), cexpr(), exprtree(), treewalk(), cmemmgr(), cgen(), cgendata(), dwarf(), cpragx86(), pchdr(), cmac1(), cmac2(), cmacadd(), ppexpn(), cgetch(), cscan(), cscanbuf(), cstring(), ftoa(), callinfo_file(), cmdscan(), cstmt(), pdefn2(), pragdefn(), scan(), toggles(), unicode() {
    cdinit = cdinit_make();
    cinfo = cinfo_make();
    cmdlnx86 = cmdlnx86_make();
    coptions = coptions_make();
    browsio = browsio_make();
    cpragma = cpragma_make(*this);
    cmac2_start(*this);
    cscan_start(*this);
}

FrontRun::~FrontRun() {
    cpragma_free(cpragma);
    browsio_free(browsio);
    coptions_free(coptions);
    cmdlnx86_free(cmdlnx86);
    cinfo_free(cinfo);
    cdinit_free(cdinit);
}

FrontRun* front_run_new() { return new FrontRun(); }
void front_run_link(FrontRun& run, BackRun* back, AsmRun* assembler, SupportRun* support, Held* held, AsminlinState* asminlin) {
    run.back = back;
    run.assembler = assembler;
    run.support = support;
    run.held = held;
    run.asminlin = asminlin;
}
void front_run_reset(FrontRun* run) { run->~FrontRun(); ::new (run) FrontRun(); }
void front_run_delete(FrontRun* run) { delete run; }
