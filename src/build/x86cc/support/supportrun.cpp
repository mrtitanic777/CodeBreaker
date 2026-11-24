/* supportrun.cpp -- see supportrun.hpp: a run of support is made, linked to the other parts and freed here. */
#include "supportrun.hpp"

SupportRun::SupportRun() : clibext(), clibint(), clibext_file(), idemsgfm() {
}

SupportRun::~SupportRun() {
}

SupportRun* support_run_new() { return new SupportRun(); }
void support_run_link(SupportRun& run, FrontRun* front, BackRun* back, AsmRun* assembler, Held* held, AsminlinState* asminlin) {
    run.front = front;
    run.back = back;
    run.assembler = assembler;
    run.held = held;
    (void)asminlin;
}
void support_run_reset(SupportRun* run) { run->~SupportRun(); ::new (run) SupportRun(); }
void support_run_delete(SupportRun* run) { delete run; }
