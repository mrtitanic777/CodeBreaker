/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Callback functions invoked from cg - communicate
*               auxiliary information to the backend.
*
****************************************************************************/


#include "cvars.h"
#include <ctype.h>
#include "cgdefs.h"
#include "cg.h"
#include "cgswitch.h"
#include "pdefn2.h"
#include "compcfg.h"
#include <sys/stat.h>
#include "autodept.h"
#include "langenv.h"
#include "feprotos.h"
#include "cfeinfo.h"
#include "caux.h"
#include "dwarfid.h"
#include "felang.h"
#include "scan.h"
#include "frontrun.hpp"


#define TRUNC_SYMBOL_HASH_LEN        4
#define TRUNC_SYMBOL_LEN_WARN        120

static inline bool HasExeMain( FrontRun& front ) { return front.cvars.CompFlags.has_main || front.cvars.CompFlags.has_winmain; }
static inline bool HasDllMain( FrontRun& front ) { return front.cvars.CompFlags.bd_switch_used || front.cvars.CompFlags.has_libmain; }
static inline bool HasAnyMain( FrontRun& front ) { return HasExeMain( front ) || HasDllMain( front ); }

static unsigned char const VarFuncWeights[] = {
/* a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z */
   0, 0,13, 0, 2, 1, 0, 0, 0, 0, 0,12, 0,14, 4,10, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0
};

static const char *const VarParmFuncs[] = {
    /*
     * functions with var parm lists
     */
    "",             /* 0 */
    "",             /* 1 */
    "",             /* 2 */
    "",             /* 3 */
    "",             /* 4 */
    "",             /* 5 */
    "",             /* 6 */
    "",             /* 7 */
    "fscanf",       /* 8 */
    "fprintf",      /* 9 */
    "execle",       /* 10 */
    "execlpe",      /* 11 */
    "scanf",        /* 12 */
    "sscanf",       /* 13 */
    "sprintf",      /* 14 */
    "spawnle",      /* 15 */
    "spawnlpe",     /* 16 */
    "printf",       /* 17 */
    "execlp",       /* 18 */
    "execl",        /* 19 */
    "cscanf",       /* 20 */
    "cprintf",      /* 21 */
    "open",         /* 22 */
    "spawnlp",      /* 23 */
    "spawnl",       /* 24 */
    "sopen",        /* 25 */
    "",             /* 26 */
    "",             /* 27 */
    "",             /* 28 */
    "",             /* 29 */
    "",             /* 30 */
    "",             /* 31 */
};

/*
 * bitmap for which of the above are Standard
 */
#define VAR_PARM_FUNCS_ANSI \
    ((1<<8) | (1<<9) | (1<<12) | (1<<13) | (1<<14) | (1<<17))

#ifdef __SEH__
    extern hw_reg_set const TryParms[] = {
        HW_D( HW_EAX ),
        HW_D( HW_EMPTY )
    };
#endif

#if _CPU == 386

extern const alt_inline_funcs FlatAlternates[];

static struct STRUCT_BYTE_SEQ( 1 ) const FinallyCode = {
    1, false, { 0xc3 }   /* ret */
};


static struct STRUCT_BYTE_SEQ( 6 ) const TryFiniCode = {
    6, false, { 0x64, 0xA3, 0, 0, 0, 0 }  /* mov fs:[0],eax */
};

#endif


bool VarParm( SYMPTR sym )
/*************************
 * does the specified symbol take variable parameters? manual search.
 */
{
    TYPEPTR     *parm_types;
    TYPEPTR     typ;
    TYPEPTR     fn_typ;

    if( sym == NULL )
        return( false );

    if( sym->flags & SYM_FUNCTION ) {
        fn_typ = sym->sym_type;
        SkipTypedefs( fn_typ );
        if( fn_typ->u.fn.parms != NULL ) {
            for( parm_types = fn_typ->u.fn.parms; (typ = *parm_types) != NULL; parm_types++ ) {
                if( typ->decl_type == TYP_DOT_DOT_DOT ) {
                    return( true );
                }
            }
        }
    }
    return( false );
}

bool VarFunc( FrontRun& front, SYMPTR sym )
/*************************
 * does the specified symbol take variable args? hash calc'ed
 */
{
    unsigned    hash;
    size_t      len;
    char        *p;
    int         c1;
    int         c2;

    if( sym == NULL )
        return( false );

    if( sym->flags & SYM_FUNCTION ) {
        p = sym->name;
        len = strlen( p );
        c1 = IndexLower( (reinterpret_cast<unsigned char *>(p))[0] );
        c2 = IndexLower( (reinterpret_cast<unsigned char *>(p))[len - 1] );
        if( c1 >= 0 && c2 >= 0 ) {
            hash = (len + VarFuncWeights[c1] + VarFuncWeights[c2]) & 0x1f;
            if( strcmp( p, VarParmFuncs[hash] ) == 0
              && ( front.cvars.CompFlags.extensions_enabled
              || (( 1 << hash ) & VAR_PARM_FUNCS_ANSI) ) ) {
                return( true );
            }
        }
        return( VarParm( sym ) );
    }
    return( false );
}


static const inline_funcs *Flat( FrontRun& front, const inline_funcs *ifunc )
{
  #if _CPU == 386
    const alt_inline_funcs  *p;

    if( front.cvars.TargetSwitches & CGSW_X86_FLAT_MODEL ) {
        for( p = FlatAlternates; p->byteseq != NULL; p++ ) {
            if( p->byteseq == ifunc->code ) {
                return( &(p->alt_ifunc) );
            }
        }
    }
  #endif
    return( ifunc );
}

static const inline_funcs *IF_Lookup( FrontRun& front, const char *name )
{
    const inline_funcs  *ifunc;

    if( !GetFpuFpc( front.cvars.ProcRevision ) ) {
        for( ifunc = _8087_Functions; ifunc->name != NULL; ifunc++ ) {
            if( strcmp( ifunc->name, name ) == 0 ) {
                return( Flat( front, ifunc ) );
            }
        }
    }
    if( front.cvars.OptSize == 100 ) {              /* if /os specified */
        ifunc = SInline_Functions;
        if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
  #if _CPU == 8086
            if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
                ifunc = ZF_Data_Functions;
            } else {
                ifunc = ZP_Data_Functions;
            }
  #else /* _CPU == 386 */
            if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
                ifunc = SBigData_Functions;
            } else {
                ifunc = SBigDataNoDs_Functions;
            }
  #endif
        }
        for( ; ifunc->name != NULL; ifunc++ ) {
            if( strcmp( ifunc->name, name ) == 0 ) {
                return( Flat( front, ifunc ) );
            }
        }
    }
  #if _CPU == 386
    if( front.cvars.TargetSwitches & CGSW_X86_FLAT_MODEL ) {
        for( ifunc = Flat_Functions; ifunc->name != NULL; ifunc++ ) {
            if( strcmp( ifunc->name, name ) == 0 ) {
                return( ifunc );
            }
        }
    }
  #endif
    ifunc = Inline_Functions;
    if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
  #if _CPU == 8086
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
            ifunc = DF_Data_Functions;
        } else {
            ifunc = DP_Data_Functions;
        }
  #else /* _CPU == 386 */
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
            ifunc = BigData_Functions;
        } else {
            ifunc = BigDataNoDs_Functions;
        }
  #endif
    }
    for( ; ifunc->name != NULL; ifunc++ ) {
        if( strcmp( ifunc->name, name ) == 0 ) {
            return( Flat( front, ifunc ) );
        }
    }
    for( ifunc = Common_Functions; ifunc->name != NULL; ifunc++ ) {
        if( strcmp( ifunc->name, name ) == 0 ) {
            return( Flat( front, ifunc ) );
        }
    }
    return( NULL );
}

aux_info *GetLangInfo( FrontRun& front, type_modifiers flags )
{
    switch( flags & MASK_LANGUAGES ) {
    case FLAG_WATCALL:
        return( &WatcallInfo( front ) );
    case FLAG_CDECL:
        return( &CdeclInfo( front ) );
    case FLAG_PASCAL:
        return( &PascalInfo( front ) );
    case FLAG_FORTRAN:
        return( &FortranInfo( front ) );
    case FLAG_SYSCALL:
        return( &SyscallInfo( front ) );
    case FLAG_STDCALL:
        return( &StdcallInfo( front ) );
    case FLAG_FASTCALL:
        return( &FastcallInfo( front ) );
    case FLAG_OPTLINK:
        return( &OptlinkInfo( front ) );
    default:
        return( &DefaultInfo( front ) );
    }
}

bool ParmsToBeReversed( type_modifiers flags, aux_info *inf )
{
    /* unused parameters */ (void)flags; (void)inf;
    return( false );
}
namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CfeinfoState: cfeinfo_state.h) */

}  // namespace



static aux_info *InfoLookup( FrontRun& front, SYMPTR sym )
{
    char            *name;
    aux_info        *inf;
    aux_entry       *ent;

    name = sym->name;
    /*
     * assume default
     */
    inf = &DefaultInfo( front );
    if( name == NULL )
        return( inf );
    ent = AuxLookup( front, name );
    if( ent != NULL )
        inf = ent->info;
    if( ( ent == NULL )
      || (sym->flags & SYM_INTRINSIC) ) {
        if( sym->flags & SYM_DEFINED )
            return( inf );
        if( (sym->flags & SYM_INTRINSIC) == 0 ) {
            if( strncmp( name, "_inline_", 8 ) != 0 )
                return( inf );
            name += 8;
        }
        {
            const inline_funcs  *ifunc;

            ifunc = IF_Lookup( front, name );
            if( ifunc == NULL )
                return( inf );
  #if ( _CPU == 8086 )
            if( HW_CEqual( ifunc->returns, HW_DX_AX )
              || HW_CEqual( ifunc->returns, HW_DS_SI )
              || HW_CEqual( ifunc->returns, HW_ES_DI )
              || HW_CEqual( ifunc->returns, HW_CX_DI ) ) {
                if( SizeOfArg( front, sym->sym_type->object ) != 4 ) {
  #else /* _CPU == 386 */
            if( HW_CEqual( ifunc->returns, HW_DX_AX )
              || HW_CEqual( ifunc->returns, HW_DS_ESI )
              || HW_CEqual( ifunc->returns, HW_ES_EDI )
              || HW_CEqual( ifunc->returns, HW_CX_DI ) ) {
                if( SizeOfArg( front, sym->sym_type->object ) != 6 ) {
  #endif
                    return( inf );
                }
            }
            inf = &front.pdefn2.InlineInfo;
            inf->cclass         = FECALL_GEN_NONE;
            inf->cclass_target  = (WatcallInfo( front ).cclass_target & FECALL_X86_FAR_CALL) | FECALL_X86_MODIFY_EXACT;

            if( (sym->flags & SYM_INTRINSIC)
              && ( ent != NULL ) ) {
                inf->cclass         |= ent->info->cclass;
                inf->cclass_target  |= ent->info->cclass_target;
            }
            inf->code = ifunc->code;
            inf->parms = const_cast<hw_reg_set *>(ifunc->parms);   // (never written through)
            inf->returns = ifunc->returns;
            if( !HW_CEqual( inf->returns, HW_xAX )
              && !HW_CEqual( inf->returns, HW_EMPTY ) ) {
                inf->cclass_target |= FECALL_X86_SPECIAL_RETURN;
            }
            HW_CAsgn( inf->streturn, HW_EMPTY );
            inf->save = ifunc->save;
            inf->objname = WatcallInfo( front ).objname;
            inf->use = 1;
        }
    }
    return( inf );
}

aux_info *FindInfo( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    SYM_ENTRY       sym_typedef;
    aux_entry       *ent;
    TYPEPTR         typ;
    aux_info        *inf;

    /*
     * assume default
     */
    inf = &DefaultInfo( front );
    if( sym_handle == SYM_NULL )
        return( inf );

    SymGet( front, sym, sym_handle );
#if _CPU == 386
    if( (sym_handle == front.cvars.SymSTOSB)
      || (sym_handle == front.cvars.SymSTOSD) ) {
        return( &front.pragdefn.STOSBInfo );
    } else if( sym_handle == front.cvars.SymFinally ) {
        front.pdefn2.InlineInfo = WatcallInfo( front );
        front.pdefn2.InlineInfo.code = const_cast<byte_seq *>(reinterpret_cast<const byte_seq *>(&FinallyCode));
        return( &front.pdefn2.InlineInfo );
    } else if( sym_handle == front.cvars.SymTryFini ) {
        front.pdefn2.InlineInfo = WatcallInfo( front );
        front.pdefn2.InlineInfo.parms = front.cfeinfo.TryFiniParms;
        front.pdefn2.InlineInfo.code = const_cast<byte_seq *>(reinterpret_cast<const byte_seq *>(&TryFiniCode));
        return( &front.pdefn2.InlineInfo );
    }
#endif
    if( (sym->flags & SYM_TEMP) == 0 ) {
        /*
         * not an indirect func call
         */
        inf = InfoLookup( front, sym );
    }
    if( inf == &DefaultInfo( front ) ) {
        typ = sym->sym_type;
        SkipDummyTypedefs( typ );
        if( typ->decl_type == TYP_TYPEDEF ) {
            SymGet( front, &sym_typedef, typ->u.typedefn );
            if( sym_typedef.name != NULL ) {
                ent = AuxLookup( front, sym_typedef.name );
                if( ent != NULL ) {
                    inf = ent->info;
                }
            }
        }
    }
#if _CPU == 386
    if( (inf->flags & AUX_FLAG_FAR16)
      || (sym->mods & FLAG_FAR16) ) {
        if( (sym->mods & MASK_LANGUAGES) == FLAG_PASCAL
          || (inf->cclass & FECALL_GEN_REVERSE_PARMS) ) {
            return( &Far16PascalInfo( front ) );
        } else {
            return( &Far16CdeclInfo( front ) );
        }
    }
#endif
    if( inf == &DefaultInfo( front ) ) {
        inf = GetLangInfo( front, sym->mods );
    }
    return( inf );
}

bool FunctionAborts( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    aux_entry    *ent;

    if( sym_handle != SYM_NULL ) {
        SymGet( front, sym, sym_handle );
        if( sym->mods & FLAG_ABORTS )
            return( true );
        if( sym->mods & FLAG_NORETURN )
            return( true );
        ent = AuxLookup( front, SymName( front, sym, sym_handle ) );
        if( ent != NULL ) {
            if( ent->info->cclass & FECALL_GEN_ABORTS ) {
                return( true );
            }
        }
    }
    return( false );
}

static call_class getCallClass( FrontRun& front, SYM_HANDLE sym_handle )
/******************************************************
 * handle only generic attributes for call class
 */
{
    aux_info            *inf;
    SYM_ENTRY           sym;
    call_class          cclass;

    sym.mods = static_cast<type_modifiers>(0);
    inf = FindInfo( front, &sym, sym_handle );
    cclass = inf->cclass;
    if( sym_handle != SYM_NULL ) {
        if( sym.flags & SYM_FUNCTION ) {
            if( sym.mods & FLAG_ABORTS ) {
                cclass |= FECALL_GEN_ABORTS;
            }
            if( sym.mods & FLAG_NORETURN ) {
                cclass |= FECALL_GEN_NORETURN;
            }
            if( sym.mods & FLAG_EXPORT ) {
                cclass |= FECALL_GEN_DLL_EXPORT;
            }
            if( IsInLineFunc( front, sym_handle ) ) {
                cclass |= FECALL_GEN_MAKE_CALL_INLINE;
            }
            if( VarFunc( front, &sym ) ) {
                cclass |= FECALL_GEN_CALLER_POPS | FECALL_GEN_HAS_VARARGS;
            }
        }
    }
    return( cclass );
}

static call_class_target getCallClassTarget( FrontRun& front, SYM_HANDLE sym_handle )
/*******************************************************************
 * handle only target specific attributes for call class
 */
{
    call_class_target   cclass_target;
    aux_info            *inf;
    SYM_ENTRY           sym;

    sym.mods = static_cast<type_modifiers>(0);
    inf = FindInfo( front, &sym, sym_handle );
    cclass_target = inf->cclass_target;
    if( sym_handle != SYM_NULL ) {
        if( sym.flags & SYM_FUNCTION ) {
  #if _CPU == 8086
            if( front.cvars.TargetSystem == TS_WINDOWS ) {
                if( inf == &PascalInfo( front )
                  || inf == &CdeclInfo( front ) ) {
                    cclass_target |= FECALL_X86_PROLOG_FAT_WINDOWS;
                }
            }
  #endif
            if( sym.mods & FLAG_FARSS ) {
                cclass_target |= FECALL_X86_FARSS;
            }
            if( front.cvars.CompFlags.emit_names ) {
                cclass_target |= FECALL_X86_EMIT_FUNCTION_NAME;
            }
            if( sym.mods & FLAG_FAR ) {
                cclass_target |= FECALL_X86_FAR_CALL;
                if( sym.mods & FLAG_NEAR ) {
                    cclass_target |= FECALL_X86_INTERRUPT;
                }
            } else if( sym.mods & FLAG_NEAR ) {
                cclass_target &= ~ FECALL_X86_FAR_CALL;
            }
            if( sym.mods & FLAG_LOADDS ) {
                cclass_target |= FECALL_X86_LOAD_DS_ON_ENTRY;
            }
        }
        if( sym.flags & SYM_FUNC_NEEDS_THUNK ) {
            cclass_target |= FECALL_X86_THUNK_PROLOG;
        }
    }
    if( front.cvars.CompFlags.ep_switch_used ) {
        cclass_target |= FECALL_X86_PROLOG_HOOKS;
    }
    if( front.cvars.CompFlags.ee_switch_used ) {
        cclass_target |= FECALL_X86_EPILOG_HOOKS;
    }
    if( front.cvars.CompFlags.sg_switch_used ) {
        cclass_target |= FECALL_X86_GROW_STACK;
    }
    if( front.cvars.CompFlags.st_switch_used ) {
        cclass_target |= FECALL_X86_TOUCH_STACK;
    }
    return( cclass_target );
}

static void addDefaultLibs( FrontRun& front )
/*********************************
 * NextLibrary
 *  Called (indirectly) from the code generator to inject automagically defined symbols.
 * Inputs:
 *  index   (n-1)
 *      Usually called from a loop until we return 0/NULL to show no more libraries
 *  request
 *      FEINF_NEXT_LIBRARY
 *          examines the current flags to see if any libraries should be
 *          automagically referenced and returns the relevant index if so.
 *      FEINF_LIBRARY_NAME
 *          returns the requested name.
 */
{
    if( front.cvars.CompFlags.emit_library_names ) {
        if( HasAnyMain( front )
          || front.cvars.CompFlags.pragma_library
          || front.cvars.CompFlags.emit_all_default_libs ) {
            AddLibraryName( front, front.cvars.CLIB_Name + 1, front.cvars.CLIB_Name[0] );
        }
        if( front.cvars.CompFlags.pgm_used_8087
          || front.cvars.CompFlags.float_used ) {
            AddLibraryName( front, front.cvars.MATHLIB_Name + 1, front.cvars.MATHLIB_Name[0] );
            if( front.cvars.EmuLib_Name != NULL ) {
                AddLibraryName( front, front.cvars.EmuLib_Name + 1, front.cvars.EmuLib_Name[0] );
            }
        }
    }
}

static CGPOINTER NextLibrary( FrontRun& front, int index, aux_class request )
{
    library_list    *lib;
    char            *name;
    int             i;

    name = NULL;
    if( index == 0 ) {
        addDefaultLibs(front);
    }
    if( request == FEINF_NEXT_LIBRARY )
        index++;
    if( index > 0 ) {
        for( i = 1, lib = front.cvars.HeadLibs; lib != NULL; i++, lib = lib->next ) {
            if( i == index ) {
                name = lib->libname;
                break;
            }
        }
    }
    /*
     * return library name, or
     */
    if( request == FEINF_LIBRARY_NAME
      || name == NULL )
        return( reinterpret_cast<CGPOINTER>(name) );
    /*
     * library index
     */
    return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(index)) );
}

static CGPOINTER NextAlias( FrontRun& front, int index, aux_class request )
/*********************************************************
 * NextAlias
 *  Called (indirectly) from the code generator to go through the list of
 *  linker aliases.
 * Inputs:
 *  index    (n-1)
 *      Called from a loop until we return 0/NULL to show no more aliases
 *  request
 *      FEINF_NEXT_ALIAS
 *          returns the index of next alias in the list, or zero if none.
 *      FEINF_ALIAS_NAME
 *          returns the alias name, or NULL if alias refers to a symbol.
 *      FEINF_ALIAS_SYMBOL
 *          returns the alias symbol, or NULL if alias refers to a name.
 *      FEINF_ALIAS_SUBST_NAME
 *          returns the name to be substituted for the alias, or NULL.
 *      FEINF_ALIAS_SUBST_SYMBOL
 *          returns the symbol to be substituted for the alias, or NULL.
 *
 * Note: One of FEINF_ALIAS..._NAME and FEINF_ALIAS..._SYMBOL will always be 0/NULL
 * and the other will be valid, depending on which form of the pragma was used.
 */
{
    alias_list          *aliaslist;
    int                 i;

    if( request == FEINF_NEXT_ALIAS )
        ++index;

    for( i = 1, aliaslist = front.cvars.AliasHead; aliaslist != NULL; i++, aliaslist = aliaslist->next ) {
        if( i == index ) {
            switch( request ) {
            case FEINF_ALIAS_NAME:
                return( const_cast<CGPOINTER>(static_cast<const void *>(aliaslist->name)) );
            case FEINF_ALIAS_SYMBOL:
                return( aliaslist->a_sym );
            case FEINF_ALIAS_SUBST_NAME:
                return( const_cast<CGPOINTER>(static_cast<const void *>(aliaslist->subst)) );
            case FEINF_ALIAS_SUBST_SYMBOL:
                return( aliaslist->s_sym );
            }
            return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(index)) );
        }
    }
    /* no (more) aliases */
    return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(0)) );
}

static unsigned GetParmsSize( FrontRun& front, SYM_HANDLE sym_handle )
/****************************************************
 * Return the size of function parameters or -1 if size could
 * not be determined (symbol isn't a function or is variadic)
 */
{
    unsigned    total_parm_size = 0;
    unsigned    parm_size;
    TYPEPTR     fn_typ;
    TYPEPTR     *parm_types;
    TYPEPTR     typ;
    SYM_ENTRY   sym;

    SymGet( front, &sym, sym_handle );
    fn_typ = sym.sym_type;
    SkipTypedefs( fn_typ );
    if( fn_typ->decl_type == TYP_FUNCTION ) {
        if( fn_typ->u.fn.parms != NULL ) {
            for( parm_types = fn_typ->u.fn.parms; (typ = *parm_types) != NULL; parm_types++ ) {
                if( typ->decl_type == TYP_DOT_DOT_DOT ) {
                    total_parm_size = static_cast<unsigned>(-1);
                    break;
                }

                SkipTypedefs( typ );
                if( typ->decl_type == TYP_VOID )
                    break;

                parm_size = RoundUp( TypeSize( front, typ ), TARGET_INT );
                total_parm_size += parm_size;
            }
        }
    } else {
        total_parm_size = static_cast<unsigned>(-1);
    }
    return( total_parm_size );
}

static const char *GetNamePattern( FrontRun& front, SYM_HANDLE sym_handle )
/*********************************************************
 * Return name pattern manipulator string
 */
{
    char                 *pattern;
    SYM_ENTRY            sym;
    aux_info             *inf;

    inf = FindInfo( front, &sym, sym_handle );
#ifdef __SEH__
    if(( sym_handle == front.cvars.SymTryInit )
      || ( sym_handle == front.cvars.SymTryFini )
      || ( sym_handle == front.cvars.SymTryUnwind )
      || ( sym_handle == front.cvars.SymExcept )) {
        pattern = const_cast<char *>("*");
    } else {
#endif
        if( sym.flags & SYM_FUNCTION ) {
            pattern = inf->objname;
            if( VarFunc( front, &sym ) ) {
                if( inf == &DefaultInfo( front ) )
                    inf = front.callinfo.DftCallConv;
                if( inf == &StdcallInfo( front ) ) {
                    pattern = CdeclInfo( front ).objname;
                } else if( inf == &FastcallInfo( front ) ) {
                    pattern = CdeclInfo( front ).objname;
                }
            }
            if( pattern == NULL ) {
                pattern =  const_cast<char *>(TS_CODE_MANGLE);
            }
        } else {
            pattern = VarNamePattern( front, inf );
            if( pattern == NULL ) {
                pattern =  const_cast<char *>(TS_DATA_MANGLE);
            }
        }
#ifdef __SEH__
    }   /* close that else */
#endif
    return( pattern );
}

static const char *GetBaseName( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYM_ENTRY            sym;

    SymGet( front, &sym, sym_handle );
    return( sym.name );
}

const char *FEExtName( FrontRun& front, CGSYM_HANDLE sym_handle, int request )
/***********************************************************/
{
    switch( request ) {
    case EXTN_BASENAME:
        return( GetBaseName( front, reinterpret_cast<SYM_HANDLE>(sym_handle) ) );
    case EXTN_PATTERN:
        return( GetNamePattern( front, reinterpret_cast<SYM_HANDLE>(sym_handle) ) );
    case EXTN_PRMSIZE:
        return( reinterpret_cast<const char *>(static_cast<pointer_uint>(GetParmsSize( front, reinterpret_cast<SYM_HANDLE>(sym_handle) ))) );
    case EXTN_IMPPREFIX:
        return( ( front.cvars.TargetSystem == TS_NT ) ? "__imp_" : NULL );
    case EXTN_CALLBACKNAME:
    default:
        return( NULL );
    }
}

static void addDefaultImports( FrontRun& front )
{
    typedef enum {
        CM_DLLMAIN      = 0x01,
        CM_WINMAIN      = 0x02,
        CM_MAIN         = 0x04,
        CM_NULL         = 0x00
    } check_mask;

    if( HasAnyMain( front ) ) {
        check_mask control;

        if( front.cvars.CompFlags.bd_switch_used ) {
            control = CM_DLLMAIN;
        } else if( front.cvars.CompFlags.bw_switch_used ) {
            control = CM_WINMAIN | CM_MAIN;
        } else if( front.cvars.CompFlags.bg_switch_used ) {
            control = CM_WINMAIN;
        } else if( front.cvars.CompFlags.bc_switch_used ) {
            control = CM_MAIN;
        } else {
            control = CM_DLLMAIN | CM_WINMAIN | CM_MAIN;
        }
        if( (control & CM_DLLMAIN) && HasDllMain( front ) ) {
            if( front.cvars.CompFlags.has_wchar_entry ) {
                AddExtRefN( front, "__DLLstartw_" );
            } else {
                AddExtRefN( front, "__DLLstart_" );
            }
            control = CM_NULL;
        }
#if _CPU == 8086
        if( (control & CM_WINMAIN) && front.cvars.CompFlags.has_winmain
          || (front.cvars.TargetSwitches & CGSW_X86_WINDOWS) && front.cvars.CompFlags.has_main ) {
#else
        if( (control & CM_WINMAIN) && front.cvars.CompFlags.has_winmain ) {
#endif
            if( front.cvars.CompFlags.has_wchar_entry ) {
                AddExtRefN( front, "_wstartw_" );
            } else {
                AddExtRefN( front, "_wstart_" );
            }
            control = CM_NULL;
        }
        if( control & CM_MAIN ) {
            assert( CompFlags.has_main );
            if( front.cvars.CompFlags.has_wchar_entry ) {
                AddExtRefN( front, "_cstartw_" );
            } else {
                AddExtRefN( front, "_cstart_" );
            }
            control = CM_NULL;
        }
    }
    if( front.cvars.CompFlags.emit_library_names ) {
        if( front.cvars.CompFlags.float_used ) {
            if( front.cvars.CompFlags.use_long_double ) {
                AddExtRefN( front, "_fltused_80bit_" );
            }
            AddExtRefN( front, "_fltused_" );
        }
  #if _CPU == 8086
        if( front.cvars.FirstStmt != 0 ) {
            if( front.cvars.TargetSwitches & CGSW_X86_BIG_CODE ) {
                AddExtRefN( front, "_big_code_" );
            } else {
                AddExtRefN( front, "_small_code_" );
            }
        }
  #endif
        if( front.cvars.CompFlags.pgm_used_8087
          || front.cvars.CompFlags.float_used ) {
            if( GetFpuEmu( front.cvars.ProcRevision ) ) {
  #if _CPU == 8086
                AddExtRefN( front, "__init_87_emulator" );
  #else /* _CPU == 386 */
                AddExtRefN( front, "__init_387_emulator" );
  #endif
            }
            if( !GetFpuFpc( front.cvars.ProcRevision ) ) {
                if( front.cvars.Stack87 == 4 ) {
                    AddExtRefN( front, "__old_8087" );
                } else {
                    AddExtRefN( front, "__8087" );
                }
            }
        }
    }
    if( front.cvars.CompFlags.main_has_parms ) {
  #if _CPU == 8086
        if( front.cvars.CompFlags.has_wchar_entry ) {
            AddExtRefN( front, "__wargc" );
        } else {
            AddExtRefN( front, "__argc" );
        }
  #else /* _CPU == 386 */
        if( front.cvars.CompFlags.register_conventions ) {
            if( front.cvars.CompFlags.has_wchar_entry ) {
                AddExtRefN( front, "__wargc" );
            } else {
                AddExtRefN( front, "__argc" );
            }
        } else {
            if( front.cvars.CompFlags.has_wchar_entry ) {
                AddExtRefN( front, "_wargc" );
            } else {
                AddExtRefN( front, "_argc" );
            }
        }
  #endif
    }
    /*
     * handle default windowing app
     */
    if( front.cvars.CompFlags.bw_switch_used ) {
        AddExtRefN( front, "__init_default_win" );
    }
    /*
     * handle NetWare
     */
    if( front.cvars.TargetSystem == TS_NETWARE
      || front.cvars.TargetSystem == TS_NETWARE5 ) {
        /*
         * is target NETWARE or NETWARE5?
         */
        AddExtRefN( front, "__WATCOM_Prelude" );
    }
    /*
     * handle 'old' profiling
     */
    if( front.cvars.TargetSwitches & CGSW_X86_P5_PROFILING ) {
        /*
         * is profiling enabled (-et)?
         */
        AddExtRefN( front, "__p5_profile" );
    }
    /*
     * handle 'new' profiling
     */
    if( front.cvars.TargetSwitches & CGSW_X86_NEW_P5_PROFILING ) {
        /*
         * is profiling enabled (-etp)?
         */
        AddExtRefN( front, "__new_p5_profile" );
    }
}


static CGPOINTER NextImport( FrontRun& front, int index, aux_class request )
/**********************************************************
 * NextImport
 *  Called (indirectly) from the code generator to inject automagically defined symbols.
 * Inputs:
 *  index    (n-1)
 *      Usually called from a loop until we return 0/NULL to show no more symbols
 *      are required.
 *  request
 *      FEINF_NEXT_IMPORT
 *          examines the current flags to see if any symbols should be
 *          automagically inserted and returns the relevant index if so.
 *      FEINF_IMPORT_NAME
 *          returns the requested name. if we have returned an index for
 *          the current compiler settings we should be called with a valid
 *          index but we still perform exactly the same checks as this is
 *          good practise.
 */
{
    char        *name;
    int         i;
    extref_info *e;

    if( !front.cvars.CompFlags.emit_targimp_symbols )
        return( NULL );

    name = NULL;
    if( index == 0 ) {
        addDefaultImports(front);
    }
    if( request == FEINF_NEXT_IMPORT )
        index++;
    if( index > 0 ) {
        for( i = 1, e = front.cvars.ExtrefInfo; e != NULL; i++, e = e->next ) {
            if( e->symbol != NULL )
                continue;
            if( i == index ) {
                name = e->name;
                break;
            }
        }
    }
    /*
     * return the import name, or
     */
    if( request == FEINF_IMPORT_NAME
      || name == NULL )
        return( reinterpret_cast<CGPOINTER>(name) );
    /*
     * return the index
     */
    return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(index)) );
}

static CGPOINTER NextImportS( FrontRun& front, int index, aux_class request )
/**********************************************************/
{
    void        *symbol;
    int         i;
    extref_info *e;

    if(!front.cvars.CompFlags.emit_targimp_symbols)
        return( NULL );

    symbol = NULL;
    if( request == FEINF_NEXT_IMPORT_S )
        index++;
    if( index > 0 ) {
        for( i = 1, e = front.cvars.ExtrefInfo; e != NULL; i++, e = e->next ) {
            if( e->symbol == NULL )
                continue;
            if( i == index ) {
                symbol = e->symbol;
                break;
            }
        }
    }
    /*
     * return the import symbol, or
     */
    if( request == FEINF_IMPORT_NAME_S
      || symbol == NULL )
        return( symbol );
    /*
     * return the index
     */
    return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(index)) );
}

CGPOINTER FEAuxInfo( FrontRun& front, CGPOINTER req_handle, aux_class request )
/*************************************************************
 * pass auxiliary information to back end
 */
{
    aux_info            *inf;
    SYM_ENTRY           sym;

    switch( request ) {
    case FEINF_SOURCE_LANGUAGE:
        return( const_cast<CGPOINTER>(static_cast<const void *>(FE_LANG_C)) );
    case FEINF_STACK_SIZE_8087:
        return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(front.cvars.Stack87)) );
    case FEINF_CODE_GROUP:
        return( reinterpret_cast<CGPOINTER>(front.cvars.GenCodeGroup) );
    case FEINF_DATA_GROUP:
        return( reinterpret_cast<CGPOINTER>(front.cvars.DataSegName) );
    case FEINF_OBJECT_FILE_NAME:
        return( reinterpret_cast<CGPOINTER>(ObjFileName(front)) );
    case FEINF_REVISION_NUMBER:
        return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(II_REVISION)) );
    case FEINF_AUX_LOOKUP:
        return( req_handle );
    case FEINF_PROEPI_DATA_SIZE:
        return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(front.cvars.ProEpiDataSize)) );
    case FEINF_DBG_PREDEF_SYM:
        return( front.cvars.SymDFAbbr );
    case FEINF_P5_CHIP_BUG_SYM:
        return( front.cvars.SymChipBug );
    case FEINF_CODE_LABEL_ALIGNMENT:
        {

            if( front.cvars.OptSize == 0 )
                front.cfeinfo.FEAuxInfo_Alignment[1] = TARGET_INT;

            return( static_cast<CGPOINTER>(front.cfeinfo.FEAuxInfo_Alignment) );
        }
    case FEINF_CLASS_NAME:
        return( reinterpret_cast<CGPOINTER>(SegClassName( front, static_cast<segment_id>(reinterpret_cast<pointer_uint>(req_handle)) )) );
    case FEINF_USED_8087:
        front.cvars.CompFlags.pgm_used_8087 = true;
        return( NULL );
  #if _CPU == 386
    case FEINF_P5_PROF_DATA:
        return( reinterpret_cast<CGPOINTER>(front.cvars.FunctionProfileBlock) );
    case FEINF_P5_PROF_SEG:
        return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(front.cvars.FunctionProfileSegId)) );
  #endif
    case FEINF_SOURCE_NAME:
        return( reinterpret_cast<CGPOINTER>(FNameFullPath( front, front.cvars.FNames )) );
    case FEINF_CALL_CLASS:
        return( reinterpret_cast<CGPOINTER>(getCallClass( front, req_handle )) );
    case FEINF_CALL_CLASS_TARGET:
        return( reinterpret_cast<CGPOINTER>(getCallClassTarget( front, req_handle )) );
    case FEINF_FREE_SEGMENT:
        return( NULL );
    case FEINF_NEXT_LIBRARY:
    case FEINF_LIBRARY_NAME:
        return( NextLibrary( front, static_cast<int>(reinterpret_cast<pointer_uint>(req_handle)), request ) );
    case FEINF_NEXT_IMPORT:
    case FEINF_IMPORT_NAME:
        return( NextImport( front, static_cast<int>(reinterpret_cast<pointer_uint>(req_handle)), request ) );
    case FEINF_NEXT_IMPORT_S:
    case FEINF_IMPORT_NAME_S:
        return( NextImportS( front, static_cast<int>(reinterpret_cast<pointer_uint>(req_handle)), request ) );
    case FEINF_NEXT_ALIAS:
    case FEINF_ALIAS_NAME:
    case FEINF_ALIAS_SYMBOL:
    case FEINF_ALIAS_SUBST_NAME:
    case FEINF_ALIAS_SUBST_SYMBOL:
        return( NextAlias( front, static_cast<int>(reinterpret_cast<pointer_uint>(req_handle)), request ) );
    case FEINF_TEMP_LOC_NAME:
        return( reinterpret_cast<CGPOINTER>(static_cast<pointer_uint>(TEMP_LOC_QUIT)) );
    case FEINF_TEMP_LOC_TELL:
        return( NULL );
    case FEINF_NEXT_DEPENDENCY:
        if( front.cvars.CompFlags.emit_dependencies )
            return( reinterpret_cast<CGPOINTER>(NextDependency( front, static_cast<FNAMEPTR>(req_handle) )) );
        return( NULL );
    case FEINF_DEPENDENCY_TIMESTAMP:
        return( reinterpret_cast<CGPOINTER>(&((static_cast<FNAMEPTR>(req_handle))->mtime)) );
    case FEINF_DEPENDENCY_NAME:
        return( reinterpret_cast<CGPOINTER>(FNameFullPath( front, static_cast<FNAMEPTR>(req_handle) )) );
    case FEINF_PEGGED_REGISTER:
        return( reinterpret_cast<CGPOINTER>(SegPeggedReg( front, static_cast<segment_id>(reinterpret_cast<pointer_uint>(req_handle)) )) );
    case FEINF_DBG_DWARF_PRODUCER:
        return( const_cast<CGPOINTER>(static_cast<const void *>(DWARF_PRODUCER_ID)) );
    case FEINF_SAVE_REGS:
        sym.mods = static_cast<type_modifiers>(0);
        inf = FindInfo( front, &sym, req_handle );
        front.cfeinfo.FEAuxInfo_save_set = inf->save;
        if( sym.mods & FLAG_SAVEREGS ) {
            HW_CTurnOn( front.cfeinfo.FEAuxInfo_save_set, HW_SEGS );
        }
  #ifdef __SEH__
        if( req_handle == front.cvars.SymTryInit ) {
            HW_CTurnOff( front.cfeinfo.FEAuxInfo_save_set, HW_xSP );
        }
  #endif
        return( reinterpret_cast<CGPOINTER>(&front.cfeinfo.FEAuxInfo_save_set) );
    case FEINF_RETURN_REG:
        inf = FindInfo( front, &sym, req_handle );
        return( reinterpret_cast<CGPOINTER>(&inf->returns) );
    case FEINF_CALL_BYTES:
        inf = FindInfo( front, &sym, req_handle );
        return( reinterpret_cast<CGPOINTER>(inf->code) );
    case FEINF_PARM_REGS:
  #ifdef __SEH__
        if(( req_handle == front.cvars.SymTryInit )
          || ( req_handle == front.cvars.SymTryFini )
          || ( req_handle == front.cvars.SymTryUnwind )
          || ( req_handle == front.cvars.SymExcept )) {
            return( const_cast<CGPOINTER>(static_cast<const void *>(TryParms)) );
        }
  #endif
        inf = FindInfo( front, &sym, req_handle );
        if( req_handle != NULL ) {
            if( inf->code == NULL
              && VarFunc( front, &sym ) ) {
                return( const_cast<CGPOINTER>(static_cast<const void *>(DefaultVarParms)) );
            }
        }
        return( reinterpret_cast<CGPOINTER>(inf->parms) );
    case FEINF_STRETURN_REG:
        inf = FindInfo( front, &sym, req_handle );
        return( reinterpret_cast<CGPOINTER>(&inf->streturn) );
    default:
        break;
    }
    return( NULL );
}

char *SrcFullPath( char const *name, char *buff, size_t max )
{
    return( _getFilenameFullPath( buff, name, max ) );
}
