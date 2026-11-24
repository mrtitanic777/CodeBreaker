/* backrun.hpp -- the state of one run of the code generator (cg: its optimizer, register allocator, scheduler, x86 encoder and the debug-info, object and float libraries it uses).
 *
 * What OpenWatcom kept in globals and statics is here: every function of this part of the compiler that reads or writes it takes
 * the run as its first parameter (`BackRun& back`). A header's variables are members (their structs are in those headers), and each
 * .cpp file's own variables are a struct of its own that file defines, held by a pointer here and made and freed by the file
 * (`<part>_make`, `<part>_free`). The other parts of the compiler are reached through the pointers at the end; they are set when the
 * run begins (run/cbrun.cpp) and name the same run. */
#ifndef CB_BACKRUN_HPP
#define CB_BACKRUN_HPP

#include "../../run/cbrun.hpp"

#include "_cgstd.h"
#include "coderep.h"
#include "../../dwarf/dw.h"
#include "indvars.h"
#include "confldef.h"
#include "savings.h"
#include "zeropage.h"
#include "blips.h"
#include "_cfloat.h"
#include "../analysis/conflict_state.h"
#include "../analysis/dataflo_state.h"
#include "../analysis/dominate_state.h"
#include "../debuginfo/cvsyms_state.h"
#include "../debuginfo/cvtypes_state.h"
#include "../debuginfo/dbtypes_state.h"
#include "../debuginfo/dfsyms_state.h"
#include "../interface/bldins_state.h"
#include "../interface/bldsel_state.h"
#include "../interface/generate_state.h"
#include "../interface/inline_state.h"
#include "../interface/onexit_state.h"
#include "../interface/rtrtn_state.h"
#include "../interface/types_state.h"
#include "../ir/cachecon_state.h"
#include "../ir/flograph_state.h"
#include "../ir/flowsave_state.h"
#include "../ir/makeblk_state.h"
#include "../ir/makeins_state.h"
#include "../ir/namelist_state.h"
#include "../ir/spawn_state.h"
#include "../ir/temps_state.h"
#include "../object/objio_state.h"
#include "../optimize/breakrtn_state.h"
#include "../optimize/cse_state.h"
#include "../optimize/peepopt_state.h"
#include "../optimize/splitvar_state.h"
#include "../optimize/trecurse_state.h"
#include "../peephole/optrel_state.h"
#include "../regalloc/regtree_state.h"
#include "../schedule/scinfo_state.h"
#include "../support/memlimit_state.h"
#include "../support/memmgt_state.h"
#include "../support/zoiks_state.h"
#include "../tree/makeaddr_state.h"
#include "../tree/tree_state.h"
#include "../x86/shared/wvsyms_state.h"
#include "../x86/shared/wvtypes_state.h"
#include "../x86/shared/x86dbsup_state.h"
#include "../x86/shared/x86enc_state.h"
#include "../x86/shared/x86enc2_state.h"
#include "../x86/shared/x86ldstr_state.h"
#include "../x86/shared/x86obj_state.h"
#include "../x86/shared/x86proc_state.h"
#include "../x86/shared/x86regsv_state.h"
#include "../x86/x87/i87reg_state.h"
#include "../x86/x87/i87sched_state.h"
#include "cvdbg.h"
#include "../object/data.h"
#include "../debuginfo/dbsyms.h"
#include "../debuginfo/dfsyms.h"
#include "../x86/x87/i87data.h"
#include "../x86/x87/i87sched.h"
#include "../optimize/inssched.h"
#include "../optimize/loopopts.h"
#include "model.h"
#include "../peephole/optdata.h"
#include "optmac.h"
#include "../x86/shared/pcencode.h"
#include "../regalloc/regsave.h"
#include "score.h"
#include "../interface/types.h"
#include "../x86/shared/x86opseg.h"
#include "../x86/shared/x86proc.h"

struct IntrfaceState;
IntrfaceState* intrface_make();
void intrface_free(IntrfaceState* part);
struct InsutilState;
InsutilState* insutil_make();
void insutil_free(InsutilState* part);
struct InsschedFileState;
InsschedFileState* inssched_file_make();
void inssched_file_free(InsschedFileState* part);
struct LoopoptsFileState;
LoopoptsFileState* loopopts_file_make();
void loopopts_file_free(LoopoptsFileState* part);
struct MultiplyState;
MultiplyState* multiply_make();
void multiply_free(MultiplyState* part);
struct CgmemmgrState;
CgmemmgrState* cgmemmgr_make();
void cgmemmgr_free(CgmemmgrState* part);
#if _TARGET & _TARG_80386
struct X386rgtblState;
X386rgtblState* x386rgtbl_make();
void x386rgtbl_free(X386rgtblState* part);
#endif
#if _TARGET & _TARG_80386
struct X386rtrtnState;
X386rtrtnState* x386rtrtn_make();
void x386rtrtn_free(X386rtrtnState* part);
#endif
#if _TARGET & _TARG_8086
struct I86rgtblState;
I86rgtblState* i86rgtbl_make();
void i86rgtbl_free(I86rgtblState* part);
#endif
#if _TARGET & _TARG_8086
struct I86rtrtnState;
I86rtrtnState* i86rtrtn_make();
void i86rtrtn_free(I86rtrtnState* part);
#endif
#if _TARGET & _TARG_8086
struct ScoreFileState;
ScoreFileState* score_file_make();
void score_file_free(ScoreFileState* part);
#endif
struct X86escState;
X86escState* x86esc_make();
void x86esc_free(X86escState* part);
struct X86omfState;
X86omfState* x86omf_make(BackRun&);
void x86omf_free(X86omfState* part);
#if _TARGET & _TARG_80386
struct X86owlState;
X86owlState* x86owl_make();
void x86owl_free(X86owlState* part);
#endif
struct I87expState;
I87expState* i87exp_make();
void i87exp_free(I87expState* part);
struct DwabbrevState;
DwabbrevState* dwabbrev_make();
void dwabbrev_free(DwabbrevState* part);
struct OwcoffState;
OwcoffState* owcoff_make();
void owcoff_free(OwcoffState* part);
struct OwelfState;
OwelfState* owelf_make();
void owelf_free(OwelfState* part);
struct OwrelocState;
OwrelocState* owreloc_make();
void owreloc_free(OwrelocState* part);

struct BackRun {
    BlipsState blips;
    CfloatState cfloat;
    ConflictVarsState conflict_vars;
    DatafloState dataflo;
    DominateState dominate;
    CvsymsState cvsyms;
    CvtypesState cvtypes;
    DbtypesState dbtypes;
    DfsymsFileState dfsyms_file;
    BldinsState bldins;
    BldselState bldsel;
    GenerateState generate;
    InlineState inline_part;
    OnexitState onexit;
    RtrtnState rtrtn;
    TypesFileState types_file;
    CacheconState cachecon;
    FlographState flograph;
    FlowsaveState flowsave;
    MakeblkState makeblk;
    MakeinsState makeins;
    NamelistState namelist;
    SpawnState spawn;
    TempsState temps;
    ObjioState objio;
    BreakrtnState breakrtn;
    CseState cse;
    PeepoptState peepopt;
    SplitvarState splitvar;
    TrecurseState trecurse;
    OptrelState optrel;
    RegtreeState regtree;
    ScinfoState scinfo;
    MemlimitState memlimit;
    MemmgtState memmgt;
    ZoiksState zoiks;
    MakeaddrState makeaddr;
    TreeState tree;
    WvsymsState wvsyms;
    WvtypesState wvtypes;
    X86dbsupState x86dbsup;
    X86encState x86enc;
    X86enc2State x86enc2;
    X86ldstrState x86ldstr;
    X86objState x86obj;
    X86procFileState x86proc_file;
    X86regsvState x86regsv;
    I87regState i87reg;
    I87schedFileState i87sched_file;
    CvdbgState cvdbg;
    DataState data;
    DbsymsState dbsyms;
    DfsymsState dfsyms;
    I87dataState i87data;
    I87schedState i87sched;
    InsschedState inssched;
    LoopoptsState loopopts;
    ModelState model;
    OptdataState optdata;
    OptmacState optmac;
    PcencodeState pcencode;
    RegsaveState regsave;
    ScoreState score;
    TypesState types;
    X86opsegState x86opseg;
    X86procState x86proc;

    IntrfaceState* intrface = nullptr;
    InsutilState* insutil = nullptr;
    InsschedFileState* inssched_file = nullptr;
    LoopoptsFileState* loopopts_file = nullptr;
    MultiplyState* multiply = nullptr;
    CgmemmgrState* cgmemmgr = nullptr;
#if _TARGET & _TARG_80386
    X386rgtblState* x386rgtbl = nullptr;
#endif
#if _TARGET & _TARG_80386
    X386rtrtnState* x386rtrtn = nullptr;
#endif
#if _TARGET & _TARG_8086
    I86rgtblState* i86rgtbl = nullptr;
#endif
#if _TARGET & _TARG_8086
    I86rtrtnState* i86rtrtn = nullptr;
#endif
#if _TARGET & _TARG_8086
    ScoreFileState* score_file = nullptr;
#endif
    X86escState* x86esc = nullptr;
    X86omfState* x86omf = nullptr;
#if _TARGET & _TARG_80386
    X86owlState* x86owl = nullptr;
#endif
    I87expState* i87exp = nullptr;
    DwabbrevState* dwabbrev = nullptr;
    OwcoffState* owcoff = nullptr;
    OwelfState* owelf = nullptr;
    OwrelocState* owreloc = nullptr;

    FrontRun* front = nullptr;   /* (the same run's front) */
    AsmRun* assembler = nullptr;   /* (the same run's asm) */
    SupportRun* support = nullptr;   /* (the same run's support) */

    Held* held = nullptr;   /* (what the run holds: its allocations and open files) */

    BackRun();
    ~BackRun();
    BackRun(const BackRun&) = delete;
    BackRun& operator=(const BackRun&) = delete;
};

/* The bodies of the functions that were macros reading the run by name and are declared in model.h, optmac.h, cgauxinf.h and
 * i87sched.h: they read members, so they are here, where BackRun is complete. */
#include "../interface/feprotos.h"

inline bool IsModel( const BackRun& back, cg_switches mdl ) { return ( back.model.Model & mdl ) != 0; }
inline bool IsntModel( const BackRun& back, cg_switches mdl ) { return ( back.model.Model & mdl ) == 0; }
inline bool IsTargetModel( const BackRun& back, cg_target_switches mdl ) { return ( back.model.TargetModel & mdl ) != 0; }
inline bool IsntTargetModel( const BackRun& back, cg_target_switches mdl ) { return ( back.model.TargetModel & mdl ) == 0; }
inline bool CPULevel( const BackRun& back, proc_revision level ) { return GetCpu( back.model.CGProcessorVersion ) >= level; }
inline bool FPULevel( const BackRun& back, proc_revision level ) { return ( GetFpu( back.model.CGProcessorVersion ) & ~FPU_EMU ) >= level; }
inline bool IsEmulation( const BackRun& back ) { return ( back.model.CGProcessorVersion & FPU_EMU ) != 0; }

inline void OptBegin( BackRun& back ) { ++back.optmac.InOptimizer; }
inline void OptEnd( BackRun& back ) { --back.optmac.InOptimizer; }

CB_ALWAYS_INLINE pointer FindAuxInfoSym( BackRun& back, pointer sym, aux_class request )
{
    return FEAuxInfo( *back.front, FEAuxInfo( *back.front, sym, FEINF_AUX_LOOKUP ), request );
}

inline actual_st_locn& RegSTLoc( BackRun& back, int seq, int vir ) { return ( back.i87sched.STLocations + seq )->actual_locn[vir]; }

#endif
