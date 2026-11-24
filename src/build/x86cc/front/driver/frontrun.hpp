/* frontrun.hpp -- the state of one run of the front end of the compiler (declarations, expressions, statements, scanner, preprocessor, pragmas, messages, output).
 *
 * What OpenWatcom kept in globals and statics is here: every function of this part of the compiler that reads or writes it takes
 * the run as its first parameter (`FrontRun& front`). A header's variables are members (their structs are in those headers), and each
 * .cpp file's own variables are a struct of its own that file defines, held by a pointer here and made and freed by the file
 * (`<part>_make`, `<part>_free`). The other parts of the compiler are reached through the pointers at the end; they are set when the
 * run begins (run/cbrun.cpp) and name the same run. */
#ifndef CB_FRONTRUN_HPP
#define CB_FRONTRUN_HPP

#include "../../run/cbrun.hpp"

#include "cvars.h"
#include "../shared/callinfo.h"
#include "../diagnostics/cerrs.h"
#include "../declare/cdecl2_state.h"
#include "../declare/csym_state.h"
#include "../declare/ctype_state.h"
#include "../diagnostics/cdump_state.h"
#include "../diagnostics/cerror_state.h"
#include "../diagnostics/cmsg_state.h"
#include "ccmain_state.h"
#include "cfeinfo_state.h"
#include "ideentry_state.h"
#include "../expr/cexpr_state.h"
#include "../expr/exprtree_state.h"
#include "../expr/treewalk_state.h"
#include "../memory/cmemmgr_state.h"
#include "../output/cgen_state.h"
#include "../output/cgendata_state.h"
#include "../output/dwarf_state.h"
#include "../pragma/cpragx86_state.h"
#include "../precompiled/pchdr_state.h"
#include "../preprocess/cmac1_state.h"
#include "../preprocess/cmac2_state.h"
#include "../preprocess/cmacadd_state.h"
#include "../preprocess/ppexpn_state.h"
#include "../scan/cgetch_state.h"
#include "../scan/cscan_state.h"
#include "../scan/cscanbuf_state.h"
#include "../scan/cstring_state.h"
#include "../scan/ftoa_state.h"
#include "../shared/callinfo_state.h"
#include "../shared/cmdscan_state.h"
#include "../stmt/cstmt_state.h"
#include "../pragma/pdefn2.h"
#include "../pragma/pragdefn.h"
#include "../scan/scan.h"
#include "../pragma/toggles.h"
#include "../shared/unicode.h"
#include "../../asm/asminlin.h"

struct CdinitState;
CdinitState* cdinit_make();
void cdinit_free(CdinitState* part);
struct CinfoState;
CinfoState* cinfo_make();
void cinfo_free(CinfoState* part);
struct Cmdlnx86State;
Cmdlnx86State* cmdlnx86_make();
void cmdlnx86_free(Cmdlnx86State* part);
struct CoptionsState;
CoptionsState* coptions_make();
void coptions_free(CoptionsState* part);
struct BrowsioState;
BrowsioState* browsio_make();
void browsio_free(BrowsioState* part);
struct CpragmaState;
CpragmaState* cpragma_make(FrontRun&);
void cpragma_free(CpragmaState* part);

void cmac2_start(FrontRun& run);
void cscan_start(FrontRun& run);
struct FrontRun {
    CallinfoState callinfo;
    CerrsState cerrs;
    CvarsState cvars;
    Cdecl2State cdecl2;
    CsymState csym;
    CtypeState ctype;
    CdumpState cdump;
    CerrorState cerror;
    CmsgState cmsg;
    CcmainState ccmain;
    CfeinfoState cfeinfo;
    IdeentryState ideentry;
    CexprState cexpr;
    ExprtreeState exprtree;
    TreewalkState treewalk;
    CmemmgrState cmemmgr;
    CgenState cgen;
    CgendataState cgendata;
    DwarfState dwarf;
    Cpragx86State cpragx86;
    PchdrState pchdr;
    Cmac1State cmac1;
    Cmac2State cmac2;
    CmacaddState cmacadd;
    PpexpnState ppexpn;
    CgetchState cgetch;
    CscanState cscan;
    CscanbufState cscanbuf;
    CstringState cstring;
    FtoaState ftoa;
    CallinfoFileState callinfo_file;
    CmdscanState cmdscan;
    CstmtState cstmt;
    Pdefn2State pdefn2;
    PragdefnState pragdefn;
    ScanState scan;
    TogglesState toggles;
    UnicodeState unicode;

    CdinitState* cdinit = nullptr;
    CinfoState* cinfo = nullptr;
    Cmdlnx86State* cmdlnx86 = nullptr;
    CoptionsState* coptions = nullptr;
    BrowsioState* browsio = nullptr;
    CpragmaState* cpragma = nullptr;

    AsminlinState* asminlin = nullptr;   /* (shared with the other part that names it: the run owns it) */
    BackRun* back = nullptr;   /* (the same run's back) */
    AsmRun* assembler = nullptr;   /* (the same run's asm) */
    SupportRun* support = nullptr;   /* (the same run's support) */

    Held* held = nullptr;   /* (what the run holds: its allocations and open files) */

    FrontRun();
    ~FrontRun();
    FrontRun(const FrontRun&) = delete;
    FrontRun& operator=(const FrontRun&) = delete;
};

/* The bodies of the functions that were macros reading the run by name and are declared in ctypes.h, cvars.h, cconst.h, toggles.h and
 * callinfo.h: they read members, so they are here, where FrontRun is complete. */
inline void PPCtlEnableAsm( FrontRun& front ) { front.cvars.PPControl |= PPCTL_ASM; }
inline void PPCtlEnableEol( FrontRun& front ) { front.cvars.PPControl |= PPCTL_EOL; }
inline void PPCtlDisableEol( FrontRun& front ) { front.cvars.PPControl &= ~PPCTL_EOL; }
inline void PPCtlEnableMacros( FrontRun& front ) { front.cvars.PPControl &= ~PPCTL_NO_EXPAND; }
inline void PPCtlDisableMacros( FrontRun& front ) { front.cvars.PPControl |= PPCTL_NO_EXPAND; }
inline void PPCtlEnableLexErrors( FrontRun& front ) { front.cvars.PPControl &= ~PPCTL_NO_LEX_ERRORS; }
inline void PPCtlDisableLexErrors( FrontRun& front ) { front.cvars.PPControl |= PPCTL_NO_LEX_ERRORS; }
inline bool IsPPCtlNormal( const FrontRun& front ) { return ( front.cvars.PPControl & ( PPCTL_EOL | PPCTL_NO_EXPAND ) ) == 0; }

template< class Entry >
inline bool CheckEqSymLevel( const FrontRun& front, const Entry *entry ) { return entry->level == static_cast<id_level_type>(front.cvars.SymLevel); }
template< class Entry >
inline bool CheckLtSymLevel( const FrontRun& front, const Entry *entry ) { return entry->level < static_cast<id_level_type>(front.cvars.SymLevel); }

CB_ALWAYS_INLINE bool IsPPOperatorPragma( const FrontRun& front, const char *s, size_t length )
{
    return ( front.cvars.CompFlags.extensions_enabled || front.cvars.CompVars.cstd > STD_C89 )
        && length == LiteralLength( PPOPERATOR_PRAGMA )
        && strcmp( s, PPOPERATOR_PRAGMA ) == 0;
}
inline bool IsHugeData( const FrontRun& front )
{
    return ( front.cvars.TargetSwitches & ( CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER ) ) == CGSW_X86_BIG_DATA;
}

inline pragma_toggles& Toggles( FrontRun& front ) { return front.toggles.PragmaToggles; }

inline aux_info& DefaultInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[0]; }
inline aux_info& WatcallInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[1]; }
inline aux_info& CdeclInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[2]; }
inline aux_info& PascalInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[3]; }
inline aux_info& FortranInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[4]; }
inline aux_info& SyscallInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[5]; }
inline aux_info& OptlinkInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[6]; }
inline aux_info& StdcallInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[7]; }
inline aux_info& FastcallInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[8]; }
#if _CPU == 386
inline aux_info& Far16CdeclInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[9]; }
inline aux_info& Far16PascalInfo( FrontRun& front ) { return front.callinfo.BuiltinAuxInfo[10]; }
#endif

#endif
