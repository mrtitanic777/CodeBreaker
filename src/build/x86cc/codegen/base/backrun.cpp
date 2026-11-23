/* backrun.cpp -- see backrun.hpp: a run of back is made, linked to the other parts and freed here. */
#include "backrun.hpp"

BackRun::BackRun() : blips(), cfloat(), conflict_vars(), dataflo(), dominate(), cvsyms(), cvtypes(), dbtypes(), dfsyms_file(), bldins(), bldsel(), generate(), inline_part(), onexit(), rtrtn(), types_file(), cachecon(), flograph(), flowsave(), makeblk(), makeins(), namelist(), spawn(), temps(), objio(), breakrtn(), cse(), peepopt(), splitvar(), trecurse(), optrel(), regtree(), scinfo(), memlimit(), memmgt(), zoiks(), makeaddr(), tree(), wvsyms(), wvtypes(), x86dbsup(), x86enc(), x86enc2(), x86ldstr(), x86obj(), x86proc_file(), x86regsv(), i87reg(), i87sched_file(), cvdbg(), data(), dbsyms(), dfsyms(), i87data(), i87sched(), inssched(), loopopts(), model(), optdata(), optmac(), pcencode(), regsave(), score(), types(), x86opseg(), x86proc() {
    intrface = intrface_make();
    insutil = insutil_make();
    inssched_file = inssched_file_make();
    loopopts_file = loopopts_file_make();
    multiply = multiply_make();
    cgmemmgr = cgmemmgr_make();
#if _TARGET & _TARG_80386
    x386rgtbl = x386rgtbl_make();
#endif
#if _TARGET & _TARG_80386
    x386rtrtn = x386rtrtn_make();
#endif
#if _TARGET & _TARG_8086
    i86rgtbl = i86rgtbl_make();
#endif
#if _TARGET & _TARG_8086
    i86rtrtn = i86rtrtn_make();
#endif
#if _TARGET & _TARG_8086
    score_file = score_file_make();
#endif
    x86esc = x86esc_make();
    x86omf = x86omf_make(*this);
#if _TARGET & _TARG_80386
    x86owl = x86owl_make();
#endif
    i87exp = i87exp_make();
    dwabbrev = dwabbrev_make();
    owcoff = owcoff_make();
    owelf = owelf_make();
    owreloc = owreloc_make();
}

BackRun::~BackRun() {
    owreloc_free(owreloc);
    owelf_free(owelf);
    owcoff_free(owcoff);
    dwabbrev_free(dwabbrev);
    i87exp_free(i87exp);
#if _TARGET & _TARG_80386
    x86owl_free(x86owl);
#endif
    x86omf_free(x86omf);
    x86esc_free(x86esc);
#if _TARGET & _TARG_8086
    score_file_free(score_file);
#endif
#if _TARGET & _TARG_8086
    i86rtrtn_free(i86rtrtn);
#endif
#if _TARGET & _TARG_8086
    i86rgtbl_free(i86rgtbl);
#endif
#if _TARGET & _TARG_80386
    x386rtrtn_free(x386rtrtn);
#endif
#if _TARGET & _TARG_80386
    x386rgtbl_free(x386rgtbl);
#endif
    cgmemmgr_free(cgmemmgr);
    multiply_free(multiply);
    loopopts_file_free(loopopts_file);
    inssched_file_free(inssched_file);
    insutil_free(insutil);
    intrface_free(intrface);
}

BackRun* back_run_new() { return new BackRun(); }
void back_run_link(BackRun& run, FrontRun* front, AsmRun* assembler, SupportRun* support, Held* held, AsminlinState* asminlin) {
    run.front = front;
    run.assembler = assembler;
    run.support = support;
    run.held = held;
    (void)asminlin;
}
void back_run_reset(BackRun* run) { run->~BackRun(); ::new (run) BackRun(); }
void back_run_delete(BackRun* run) { delete run; }

FrontRun& front_of(BackRun& back) { return *back.front; }
