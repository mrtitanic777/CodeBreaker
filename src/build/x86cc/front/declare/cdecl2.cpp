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
* Description:  C declarator processing.
*
****************************************************************************/


#include "cvars.h"
#include "cgswitch.h"
#include "i64.h"
#include "toggles.h"
#include "frontrun.hpp"


static TYPEPTR      DeclPart2( FrontRun& front, TYPEPTR typ, type_modifiers mod );
static TYPEPTR      DeclPart3( FrontRun& front, TYPEPTR typ, type_modifiers mod );
static void         AbsDecl( FrontRun& front, SYMPTR sym, type_modifiers mod, TYPEPTR typ );
static void         FreeParmList( FrontRun& front );
static void         GetFuncParmList( FrontRun& front );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct Cdecl2State: cdecl2_state.h) */

}  // namespace



void Check_Struct_Union_Enum( FrontRun& front, TYPEPTR typ )
{
    SKIP_DUMMY_TYPEDEFS( typ );
    switch( typ->decl_type ) {
    case TYP_STRUCT:
    case TYP_UNION:
        if( typ->u.tag->name[0] == '\0' ) {
            InvDecl(front);
        }
        break;
    case TYP_ENUM:
        break;
    default:
        InvDecl(front);
    }
}


static void FlushBadCode( FrontRun& front )
{
    int         count;

    CErr1( front, ERR_STMT_MUST_BE_INSIDE_FUNCTION );
    count = 0;
    for( ;; ) {
        NextToken(front);
        if( front.cvars.CurToken == T_EOF )
            return;
        if( front.cvars.CurToken == T_LEFT_BRACE ) {
            ++count;
        }
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            if( count == 0 )
                break;
            --count;
        }
    }
    NextToken(front);
}


static stg_classes SCSpecifier( FrontRun& front )
{
    stg_classes     stg_class;

    stg_class = SC_NONE;        /* assume no storage class specified */
    if( TokenClass[front.cvars.CurToken] == TC_STG_CLASS ) {
        switch( front.cvars.CurToken ) {
        case T_EXTERN:
            stg_class = SC_EXTERN;
            break;
        case T_STATIC:
            stg_class = SC_STATIC;
            break;
        case T_TYPEDEF:
            stg_class = SC_TYPEDEF;
            break;
        case T_AUTO:
            stg_class = SC_AUTO;
            break;
        case T_REGISTER:
            stg_class = SC_REGISTER;
            break;
        default:
            break;
        }
        NextToken(front);
    }
    return( stg_class );
}


void InvDecl( FrontRun& front )
{
    CErr1( front, ERR_INVALID_DECLARATOR );
}

static void CmpFuncDecls( FrontRun& front, SYMPTR new_sym, SYMPTR old_sym )
{
    TYPEPTR     type_new, type_old;

    if( (new_sym->mods & ~MASK_FUNC) != (old_sym->mods & ~MASK_FUNC) ) {
        CErr2p( front, ERR_MODIFIERS_DISAGREE, new_sym->name );
    }
    /*
     * check for conflicting information
     * skip over TYPEDEF's
     */
    type_new = new_sym->sym_type;
    SKIP_TYPEDEFS( type_new );
    type_old = old_sym->sym_type;
    SKIP_TYPEDEFS( type_old );
    /*
     * diagnostics for function, target=old and source=new
     */
    SetDiagType2( front, type_old->object, type_new->object );
    if( !CheckIdenticalType( front, type_new->object, type_old->object ) ) {
        TYPEPTR     ret_new, ret_old;

        /*
         * save return types
         */
        ret_new = type_new->object;
        ret_old = type_old->object;
        /*
         * skip over typedef's
         */
        SKIP_TYPEDEFS( ret_new );
        SKIP_TYPEDEFS( ret_old );
        /*
         * don't reorder this expression
         * return value used in forward
         */
        if( old_sym->attribs.stg_class != SC_FORWARD ) {
            CErr2p( front, ERR_INCONSISTENT_TYPE, new_sym->name );
        } else if( ret_new->decl_type != TYP_VOID || (old_sym->flags & SYM_TYPE_GIVEN) ) {
            CErr2p( front, ERR_INCONSISTENT_TYPE, new_sym->name );
        }
    }
    SetDiagPop(front);
    /*
     * check types of parms, including promotion
     */
    CheckCompatibleFunction( front, type_old, type_new, true );
}


static SYM_HANDLE FuncDecl( FrontRun& front, SYMPTR sym, stg_classes stg_class, decl_state *state )
{
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          old_sym_handle;
    SYM_ENTRY           old_sym;
    SYM_ENTRY           sym_typedef;
    TYPEPTR             old_typ;
    SYM_NAMEPTR         sym_name;
    char                *name;
    ENUMPTR             ep;

    front.cvars.PrevProtoType = NULL;
    /*
     * Warn if assuming 'int' return type - should be an error in strict C99 mode
     */
    if( *state & DECL_STATE_NOTYPE ) {
        CWarn2p( front, ERR_NO_RET_TYPE_GIVEN, sym->name );
    }
    sym->attribs.rent = false;      /* Assume not override aka re-entrant */
    if( front.cvars.CompFlags.rent && (sym->attribs.declspec == DECLSPEC_DLLIMPORT) ) {
        sym->attribs.rent = true;
    }
    if( (sym->mods & FLAG_INTERRUPT) == FLAG_INTERRUPT ) {
        sym->mods |= FLAG_FARSS;    /* interrupts always use far stack */
    }
    if( stg_class == SC_REGISTER
      || stg_class == SC_AUTO
      || stg_class == SC_TYPEDEF ) {
        CErr1( front, ERR_INVALID_STG_CLASS_FOR_FUNC );
        stg_class = SC_NONE;
    }
    old_sym_handle = SymLook( front, sym->u1.hash, sym->name );
    if( old_sym_handle == SYM_NULL ) {
        ep = EnumLookup( front, sym->u1.hash, sym->name );
        if( ep != NULL ) {
            SetDiagEnum( front, ep );
            CErr2p( front, ERR_SYM_ALREADY_DEFINED, sym->name );
            SetDiagPop(front);
        }
        if( front.cvars.Check_global_prototype ) {
            if( stg_class != SC_STATIC && stg_class != SC_EXTERN && (sym->mods & FLAG_INLINE) == 0 && front.cvars.CurToken == T_LEFT_BRACE && !CheckFuncMain( sym->name ) ) {
                CWarn2p( front, ERR_ASSUMED_IMPORT, sym->name );
            }
        }
        sym_handle = SymAddL0( front, sym->u1.hash, sym );
    } else {
        SymGet( front, &old_sym, old_sym_handle );
        SetDiagSymbol( front, &old_sym, old_sym_handle );
        if( (old_sym.flags & SYM_FUNCTION) == 0 ) {
            CErr2p( front, ERR_SYM_ALREADY_DEFINED_AS_VAR, sym->name );
//            sym_handle = old_sym_handle;
            sym_handle = SymAddL0( front, sym->u1.hash, sym );
        } else {
            CmpFuncDecls( front, sym, &old_sym );
            front.cvars.PrevProtoType = old_sym.sym_type;
            if( (old_sym.flags & SYM_DEFINED) == 0 ) {
                if( sym->sym_type->u.fn.parms != NULL
                  || ( front.cvars.CurToken != T_COMMA && front.cvars.CurToken != T_SEMI_COLON ) ) {
                    old_typ = old_sym.sym_type;
                    if( old_typ->decl_type == TYP_TYPEDEF
                      && old_typ->object->decl_type == TYP_FUNCTION ) {
                        SymGet( front, &sym_typedef, old_typ->u.typedefn );
                        sym_name = SymName( front, &sym_typedef, old_typ->u.typedefn );
                        name = CMemStrdup( front, sym_name );
                        XferPragInfo( front, name, sym->name );
                        CMemFree( front, name );
                    }
                    old_sym.sym_type = sym->sym_type;
                    old_sym.src_loc = sym->src_loc;
                }
            }
            /*
             * check lang flags to make sure no one saw an incompatible prototype; if
             * previous prototype specified calling convention and later definition does
             * not, propagate the convention from the prototype
             */
            if( (sym->mods & MASK_LANGUAGES) && !CheckCompatibleLanguage( front, sym->mods, old_sym.mods ) ) {
                CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
            }
            if( (sym->mods & FLAG_INLINE) != (old_sym.mods & FLAG_INLINE) ) {
                old_sym.mods |= FLAG_INLINE;    //either is inline
            }
            if( (sym->mods & FLAG_ABORTS) != (old_sym.mods & FLAG_ABORTS) ) {
                if( sym->mods & FLAG_ABORTS ) {
                    CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                }
            }
            if( (sym->mods & FLAG_NORETURN) != (old_sym.mods & FLAG_NORETURN) ) {
                if( sym->mods & FLAG_NORETURN ) {
                    CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                }
            }
            if( (sym->mods & FLAG_FARSS) != (old_sym.mods & FLAG_FARSS) ) {
                if( sym->mods & FLAG_FARSS ) {
                    CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                }
            }
            if( sym->attribs.declspec != old_sym.attribs.declspec ) {
                switch( sym->attribs.declspec ) {
                case DECLSPEC_DLLIMPORT:
                case DECLSPEC_THREAD:
                    CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                    break;
                case DECLSPEC_DLLEXPORT:
                    /*
                     * Allow the following:     void foo( void ); void _Export foo( void );
                     * IBM's compiler allows this, so does our C++ compiler; and it's real useful!
                     */
                    if( old_sym.attribs.declspec == DECLSPEC_DLLIMPORT || old_sym.attribs.declspec == DECLSPEC_NONE ) {
                        old_sym.attribs.declspec = DECLSPEC_DLLEXPORT;
                    } else {
                        CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                    }
                    break;
                }
            }
            old_sym.attribs.naked |= sym->attribs.naked;
            if( stg_class == SC_STATIC && old_sym.attribs.stg_class == SC_EXTERN ) {
                /*
                 * can't redeclare extern function as static
                 * NB: We may want to handle SC_FORWARD functions too!
                 */
                CWarn2p( front, ERR_FUNCTION_STG_CLASS_REDECLARED, sym->name );
            }
            CMemFree( front, sym->name );
            if( stg_class == SC_NONE && old_sym.attribs.stg_class != SC_FORWARD ) {
                stg_class = static_cast<stg_classes>(old_sym.attribs.stg_class);
            }
            if( old_sym.sym_type->decl_type == TYP_FUNCTION ) {
                old_sym.sym_type = FuncNode( front, old_sym.sym_type->object,
                    old_sym.mods, old_sym.sym_type->u.fn.parms );
            }
            memcpy( sym, &old_sym, sizeof( SYM_ENTRY ) );
            sym_handle = old_sym_handle;
        }
        SetDiagPop(front);
    }
    sym->flags |= SYM_FUNCTION;
    if( (sym->flags & SYM_DEFINED) == 0 ) {
        if( sym->mods & FLAG_INLINE ) {
            sym->flags |= SYM_IGNORE_UNREFERENCE;
            stg_class = SC_STATIC;
        } else if( stg_class == SC_NONE ) {
            stg_class = SC_EXTERN;  /* SC_FORWARD; */
        }
        sym->attribs.stg_class = stg_class;
    }
    return( sym_handle );
}

static SYM_HANDLE VarDecl( FrontRun& front, SYMPTR sym, stg_classes stg_class, decl_state *state )
{
    bool                old_def;
    TYPEPTR             typ;
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          old_sym_handle;
    SYM_ENTRY           old_sym;
    SYM_ENTRY           sym2;
    type_modifiers      old_attrs;
    type_modifiers      new_attrs;

    /*
     * Warn if neither type nor storage class were given; this should probably be
     * an error in strict C89 (and naturally C99) mode
     */
    if( (stg_class == SC_NONE) && (*state & DECL_STATE_NOTYPE) && (*state & DECL_STATE_NOSTWRN) == 0 ) {
        CWarn1( front, ERR_NO_STG_OR_TYPE );
        *state |= DECL_STATE_NOSTWRN;   // Only warn once for each declarator list
    }
    /*
     * Additionally warn if assuming 'int' type - should be an error in strict C99 mode
     */
    if( *state & DECL_STATE_NOTYPE ) {
        CWarn2p( front, ERR_NO_DATA_TYPE_GIVEN, sym->name );
    }
    if( front.cvars.CompFlags.rent ) {
        sym->attribs.rent = true; //Assume instance data
    } else {
        sym->attribs.rent = false;//Assume non instance data
    }
    if( sym->attribs.naked ) {
        CErr1( front, ERR_INVALID_DECLSPEC );
    }

    if( front.cvars.SymLevel == 0 ) {
        /*
         * SymLevel == 0 is global scope (SymLevel is the count of nested {'s)
         */
        if( (stg_class == SC_AUTO) || (stg_class == SC_REGISTER) ) {
            CErr1( front, ERR_INV_STG_CLASS_FOR_GLOBAL );
            stg_class = SC_STATIC;
        } else if( stg_class == SC_NONE ) {
            front.cvars.CompFlags.external_defn_found = true;
        }
        if( sym->attribs.declspec == DECLSPEC_THREAD ) {
            if( !front.cvars.CompFlags.thread_data_present ) {
                front.cdecl2.thread_segid = DefThreadSeg(front);
                front.cvars.CompFlags.thread_data_present = true;
            }
            sym->u.var.segid = front.cdecl2.thread_segid;
        }
    } else {
        /*
         * SymLevel != 0 is function scoped (SymLevel is the count of nested {'s)
         */
        if( stg_class == SC_NONE ) {
            stg_class = SC_AUTO;
        }
        if( stg_class == SC_AUTO || stg_class == SC_REGISTER ) {
            if( sym->mods & MASK_LANGUAGES ) {
                CErr1( front, ERR_INVALID_DECLARATOR );
            }
            if( sym->attribs.declspec != DECLSPEC_NONE ) {
                CErr1( front, ERR_INVALID_DECLSPEC );
            }
            /*
             * Local variables in stack will be far when SS != DS (/zu)
             * (applies only to auto vars, functions params are handled
             * NOT here but in "cexpr.c" [OPR_PUSHADDR])
             */
            if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_SS ) {
                sym->mods |= FLAG_FAR;
            }
        }
        /*
         * static class variables can be thread local also
         */
        if( (stg_class == SC_STATIC) && (sym->attribs.declspec == DECLSPEC_THREAD) ) {
            if( !front.cvars.CompFlags.thread_data_present ) {
                front.cdecl2.thread_segid = DefThreadSeg(front);
                front.cvars.CompFlags.thread_data_present = true;
            }
            sym->u.var.segid = front.cdecl2.thread_segid;
        }
    }
    if( !TOGGLE( unreferenced ) ) {
        sym->flags |= SYM_IGNORE_UNREFERENCE;
    }
    old_sym_handle = SymLook( front, sym->u1.hash, sym->name );
    if( old_sym_handle != SYM_NULL ) {
        SymGet( front, &old_sym, old_sym_handle );
        if( CheckEqSymLevel( &old_sym ) ) {
            SetDiagSymbol( front, &old_sym, old_sym_handle );
            if( old_sym.attribs.stg_class == SC_EXTERN && stg_class == SC_EXTERN ) {
                if( ! CheckIdenticalType( front, old_sym.sym_type, sym->sym_type ) ) {
                    CErr2p( front, ERR_TYPE_DOES_NOT_AGREE, sym->name );
                }
            } else if( old_sym.attribs.stg_class == SC_TYPEDEF ) {
                CErr2p( front, ERR_SYM_ALREADY_DEFINED, sym->name );
            }
            SetDiagPop(front);
        }
    }
    if( stg_class == SC_EXTERN ) {
        old_sym_handle = Sym0Look( front, sym->u1.hash, sym->name );
    }
    if( old_sym_handle != SYM_NULL ) {
        SymGet( front, &old_sym, old_sym_handle );
        SetDiagSymbol( front, &old_sym, old_sym_handle );
        if( CheckEqSymLevel( &old_sym ) || stg_class == SC_EXTERN ) {
            old_attrs = old_sym.mods;
            new_attrs = sym->mods;
            /*
             * add default far/near flags depending on data model
             */
            if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
                old_attrs |= FLAG_FAR;
                new_attrs |= FLAG_FAR;
            } else {
                old_attrs |= FLAG_NEAR;
                new_attrs |= FLAG_NEAR;
            }
            if( (new_attrs & ~MASK_FUNC) != (old_attrs & ~MASK_FUNC) ) {
                 CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
            }
            if( (sym->mods & MASK_LANGUAGES) != (old_sym.mods & MASK_LANGUAGES) ) {
                /*
                 * just inherit old lang flags
                 * if new != 0 then it's possible someone saw a different prototype
                 */
                if( (sym->mods & MASK_LANGUAGES) != 0 ) {
                     CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                }
            }
            if( sym->attribs.declspec != old_sym.attribs.declspec ) {
                switch( sym->attribs.declspec ) {
                case DECLSPEC_DLLIMPORT:
                case DECLSPEC_THREAD:
                    CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                    break;
                case DECLSPEC_DLLEXPORT:
                    if( old_sym.attribs.declspec == DECLSPEC_DLLIMPORT ) {
                        old_sym.attribs.declspec = DECLSPEC_DLLEXPORT;
                    } else {
                        CErr2p( front, ERR_MODIFIERS_DISAGREE, sym->name );
                    }
                    break;
                }
            }
        }
        SetDiagPop(front);
    }
    if( ( old_sym_handle != SYM_NULL )
      && (stg_class == SC_NONE || stg_class == SC_EXTERN
      || (stg_class == SC_STATIC && front.cvars.SymLevel == 0)) ) {
        /*
         * make sure sym->sym_type same type as old_sym->sym_type
         */
        SetDiagSymbol( front, &old_sym, old_sym_handle );
        old_def = VerifyType( front, sym->sym_type, old_sym.sym_type, sym );
        SetDiagPop(front);
        if( !old_def && CheckEqSymLevel( &old_sym ) ) {
            /*
             * new symbol's type supersedes old type
             */
            old_sym.sym_type = sym->sym_type;
            if( (old_sym.flags & SYM_FUNCTION) ) {
                old_sym = *sym;  // ditch old sym
            }
            SymReplace( front, &old_sym, old_sym_handle );
        }
        if( stg_class == SC_EXTERN && front.cvars.SymLevel != 0 )
            goto new_var;
        CMemFree( front, sym->name );
        memcpy( sym, &old_sym, sizeof( SYM_ENTRY ) );
        sym_handle = old_sym_handle;
        SetDiagSymbol( front, &old_sym, old_sym_handle );
        /*
         * verify that newly specified storage class doesn't conflict
         */
        if( (stg_class == SC_NONE) || (stg_class == SC_STATIC) ) {
            if( sym->attribs.stg_class == SC_EXTERN ) {
                /*
                 * was extern, OK to change to none
                 */
                if( stg_class == SC_NONE ) {
                    sym->attribs.stg_class = stg_class;
                } else {
                    /*
                     * was extern, not OK to make static
                     */
                    CErrSymName( front, ERR_STG_CLASS_DISAGREES, sym, sym_handle );
                }
            } else if( sym->attribs.stg_class == SC_STATIC && stg_class == SC_NONE ) {
                /*
                 * was static, not OK to redefine
                 */
                CErrSymName( front, ERR_STG_CLASS_DISAGREES, sym, sym_handle );
            } else if( sym->attribs.stg_class == SC_NONE && stg_class == SC_STATIC ) {
                /*
                 * was extern linkage, not OK to to make static
                 */
                CErrSymName( front, ERR_STG_CLASS_DISAGREES, sym, sym_handle );
            }
        }
        SetDiagPop(front);
    } else {
        if( stg_class == SC_EXTERN && front.cvars.SymLevel != 0 ) {
            ; /* do nothing */
        } else {
            VfyNewSym( front, sym->u1.hash, sym->name );
        }
new_var:
        old_sym_handle = SYM_NULL;
        sym->flags |= SYM_DEFINED;
        typ = sym->sym_type;
        SKIP_DUMMY_TYPEDEFS( typ );
        if( typ->decl_type == TYP_TYPEDEF ) {
            SymGet( front, &sym2, typ->u.typedefn );
            if( sym->u.var.segid == SEG_NULL && sym2.u.var.segid != SEG_NULL ) {
                sym->u.var.segid = sym2.u.var.segid;
            }
            SKIP_TYPEDEFS( typ );
        }
        if( typ->decl_type == TYP_VOID ) {
            CErr2p( front, ERR_VAR_CANT_BE_VOID, sym->name );
            sym->sym_type = TypeDefault(front);
        }
        sym->attribs.stg_class = stg_class;
        sym_handle = SymAdd( front, sym->u1.hash, sym );
    }
    if( sym->u.var.segid == SEG_NULL
      && ( stg_class == SC_STATIC
      || stg_class == SC_NONE
      || stg_class == SC_EXTERN ) ) {
        if( front.cvars.DefDataSegment != SEG_NULL ) {
            sym->u.var.segid = front.cvars.DefDataSegment;
            SymReplace( front, sym, sym_handle );
        }
    }
    if( front.cvars.CurToken == T_EQUAL ) {
        if( stg_class == SC_EXTERN ) {
            stg_class = SC_STATIC;
            if( front.cvars.SymLevel == 0 ) {
                front.cvars.CompFlags.external_defn_found = true;
                stg_class = SC_NONE;
            } else {
                CErr1( front, ERR_CANT_INITIALIZE_EXTERN_VAR );
            }
        }
        sym->attribs.stg_class = stg_class;
        NextToken(front);
        VarDeclEquals( front, sym, sym_handle );
        sym->flags |=  SYM_ASSIGNED;
    }
    SymReplace( front, sym, sym_handle );
    if( old_sym_handle != SYM_NULL ) {
        sym_handle = SYM_NULL;
    }
    return( sym_handle );
}

static SYM_HANDLE InitDeclarator( FrontRun& front, SYMPTR sym, decl_info const * const info, decl_state *state )
{
    SYM_HANDLE      sym_handle;
    SYM_HANDLE      old_sym_handle;
    type_modifiers  flags;
    TYPEPTR         typ;
    TYPEPTR         otyp;
    SYM_ENTRY       old_sym;

    if( front.cvars.ParmList != NULL ) {
        FreeParmList(front);
    }
    memset( sym, 0, sizeof( SYM_ENTRY ) );
    sym->name = const_cast<char *>("");
    flags = TypeQualifier(front);
    if( flags & info->mod ) {
       CErr1( front, ERR_INV_TYPE );
    }
    flags |= info->mod;
    if( info->decl == DECLSPEC_DLLEXPORT ) {
        flags |= FLAG_EXPORT; //need to get rid of this
    }
    Declarator( front, sym, flags, info->typ, *state );
    if( sym->name[0] == '\0' ) {
        InvDecl(front);
        return( SYM_NULL );
    }
    typ = sym->sym_type;
    /*
     * skip over typedef's
     */
    SKIP_TYPEDEFS( typ );
    if( info->stg == SC_TYPEDEF ) {
        if( front.cvars.CompFlags.extensions_enabled ) {
            old_sym_handle = SymLook( front, sym->u1.hash, sym->name );
            if( old_sym_handle != SYM_NULL ) {
                SymGet( front, &old_sym, old_sym_handle );
                otyp = old_sym.sym_type;        /* skip typedefs */
                SKIP_TYPEDEFS( otyp );
                if( old_sym.attribs.stg_class == SC_TYPEDEF &&
                    CheckEqSymLevel( &old_sym ) && CheckIdenticalType( front, typ, otyp ) ) {
                    return( SYM_NULL );        /* indicate already in symbol tab */
                }
            }
        }
        VfyNewSym( front, sym->u1.hash, sym->name );
        sym->attribs.stg_class = info->stg;
        AdjModsTypeNode( front, &sym->sym_type, info->decl_mod, sym );
        sym_handle = SymAdd( front, sym->u1.hash, sym );
    } else {
        sym->attribs.declspec = info->decl;
        sym->attribs.naked = info->naked;
        if( sym->attribs.declspec == DECLSPEC_DLLEXPORT ) { //sync up flags
            sym->mods |= FLAG_EXPORT; //need to get rid of this
        } else if( sym->mods & FLAG_EXPORT ) {
            if( sym->attribs.declspec == DECLSPEC_NONE ) {
                sym->attribs.declspec = DECLSPEC_DLLEXPORT;
            } else if( sym->attribs.declspec != DECLSPEC_DLLEXPORT ) {
                 CErr1( front, ERR_INVALID_DECLSPEC );
            }
        }
        AdjModsTypeNode( front, &sym->sym_type, info->decl_mod, sym );
        if( typ->decl_type == TYP_FUNCTION ) {
            sym_handle = FuncDecl( front, sym, info->stg, state );
        } else {
            sym_handle = VarDecl( front, sym, info->stg, state );
        }
    }
    return( sym_handle );
}

bool DeclList( FrontRun& front, SYM_HANDLE *sym_head )
{
    decl_state          state;
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          prevsym_handle;
    SYM_ENTRY           sym;
    SYM_ENTRY           prevsym;
    decl_info           info;

    front.cvars.ParmList = NULL;
    prevsym_handle = SYM_NULL;
    *sym_head = SYM_NULL;
    for( ;; ) {
        for( ;; ) {
            for( ;; ) {
                while( front.cvars.CurToken == T_SEMI_COLON ) {
                    if( front.cvars.SymLevel == 0 ) {
                        if( !front.cvars.CompFlags.extensions_enabled ) {
                            CErr2p( front, ERR_EXPECTING_DECL_BUT_FOUND, ";" );
                        }
                    }
                    NextToken(front);
                }
                if( front.cvars.CurToken == T_EOF )
                    return( false );
                FullDeclSpecifier( front, &info );
                if( info.stg != SC_NONE || info.typ != NULL )
                    break;
                if( front.cvars.SymLevel != 0 )
                    return( false );
                break;
            }
            state = DECL_STATE_NONE;
            if( info.typ == NULL ) {
                state |= DECL_STATE_NOTYPE;
                info.typ = TypeDefault(front);
            }
            if( info.stg == SC_NONE && (state & DECL_STATE_NOTYPE) ) {
                if( TokenClass[front.cvars.CurToken] == TC_MODIFIER ) {
                } else {
                    switch( front.cvars.CurToken ) {
                    case T_ID:
                    case T_LEFT_PAREN:
                    case T_TIMES:
                        break;
                    case T_IF:
                    case T_FOR:
                    case T_WHILE:
                    case T_DO:
                    case T_SWITCH:
                    case T_BREAK:
                    case T_CONTINUE:
                    case T_CASE:
                    case T_DEFAULT:
                    case T_ELSE:
                    case T_GOTO:
                    case T_RETURN:
                        FlushBadCode(front);
                        continue;
                    default:
                        CErr2p( front, ERR_EXPECTING_DECL_BUT_FOUND, TokenString[front.cvars.CurToken] );
                        NextToken(front);
                        break;
                    }
                }
            }
            break;
        }
        if( front.cvars.CurToken != T_SEMI_COLON ) {
            if( info.decl == DECLSPEC_DLLIMPORT ) {
                if( !front.cvars.CompFlags.rent ) {
                    if( info.stg == SC_NONE ) {
                        info.stg = SC_EXTERN;
                    }
                }
            }
            for( ;; ) {
                sym_handle = InitDeclarator( front, &sym, &info, &state );
                /*
                 * NULL is returned if sym already exists in symbol table
                 */
                if( sym_handle != SYM_NULL ) {
                    sym.handle = SYM_NULL;
                    if( sym.flags & SYM_FUNCTION ) {
                        if( (state & DECL_STATE_NOTYPE ) == 0 ) {
                            sym.flags |= SYM_TYPE_GIVEN;
                        }
                    } else if( front.cvars.SymLevel > 0 ) { /* variable */
                        if( prevsym_handle != SYM_NULL ) {
                            SymGet( front, &prevsym, prevsym_handle );
                            prevsym.handle = sym_handle;
                            SymReplace( front, &prevsym, prevsym_handle );
                        }
                        if( *sym_head == SYM_NULL ) {
                            *sym_head = sym_handle;
                        }
                        prevsym_handle = sym_handle;
                    }
                    SymReplace( front, &sym, sym_handle );
                }
                /*
                 * case "int x *p" ==> missing ',' msg already given
                 */
                if( front.cvars.CurToken != T_TIMES ) {
                    if( front.cvars.CurToken != T_COMMA )
                        break;
                    NextToken(front);
                }
            }
            /*
             * the following is illegal:
             *         typedef double math(double);
             *         math sin { ; }
             * That's the reason for the check
             *         "typ->decl_type != TYP_FUNCTION"
             */
            if( front.cvars.SymLevel == 0 && front.cvars.CurToken != T_SEMI_COLON && sym_handle != SYM_NULL ) {
                if( sym.sym_type->decl_type == TYP_FUNCTION && sym.sym_type != info.typ ) {
                    front.cvars.CurFuncHandle = sym_handle;
                    front.cvars.CurFunc = &front.cvars.CurFuncSym;
                    memcpy( front.cvars.CurFunc, &sym, sizeof( SYM_ENTRY ) );
                    return( true );     /* indicate this is a function defn */
                }
            }
            MustRecog( front, T_SEMI_COLON );
        } else {
            Check_Struct_Union_Enum( front, info.typ );
            NextToken(front);                /* skip over ';' */
        }
    }
    /*
     * can't get here!      return( false );
     */
}


bool LoopDecl( FrontRun& front, SYM_HANDLE *sym_head )
{
    decl_state          state;
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          prevsym_handle;
    SYM_ENTRY           sym;
    SYM_ENTRY           prevsym;
    decl_info           info;

    front.cvars.ParmList = NULL;
    prevsym_handle = SYM_NULL;
    *sym_head = SYM_NULL;
    if( front.cvars.CurToken == T_EOF )
        return( false );
    FullDeclSpecifier( front, &info );
    if( info.stg == SC_NONE && info.typ == NULL ) {
        return( false );    /* No declaration-specifiers, get outta here */
    }
    if( info.stg != SC_NONE && info.stg != SC_AUTO && info.stg != SC_REGISTER ) {
        CErr1( front, ERR_INVALID_STG_CLASS_FOR_LOOP_DECL );
    }
    state = DECL_STATE_FORLOOP;
    if( info.typ == NULL ) {
        /*
         * C99 requires a type specifier; we could get away with defaulting
         * to int but what would be the point?
         */
        CErr1( front, ERR_NO_TYPE_IN_DECL );
        info.typ = TypeDefault(front);
    }
    if( front.cvars.CurToken != T_SEMI_COLON ) {
        for( ;; ) {
            sym_handle = InitDeclarator( front, &sym, &info, &state );
            /*
             * NULL is returned if sym already exists in symbol table
             */
            if( sym_handle != SYM_NULL ) {
                sym.handle = SYM_NULL;
                if( sym.flags & SYM_FUNCTION ) {
                    if( (state & DECL_STATE_NOTYPE ) == 0 ) {
                        sym.flags |= SYM_TYPE_GIVEN;
                    }
                } else {    /* variable */
                    if( prevsym_handle != SYM_NULL ) {
                        SymGet( front, &prevsym, prevsym_handle );
                        prevsym.handle = sym_handle;
                        SymReplace( front, &prevsym, prevsym_handle );
                    }
                    if( *sym_head == SYM_NULL ) {
                        *sym_head = sym_handle;
                    }
                    prevsym_handle = sym_handle;
                }
                SymReplace( front, &sym, sym_handle );
            }
            /*
             * case "int x *p" ==> missing ',' msg already given
             */
            if( front.cvars.CurToken != T_TIMES ) {
                if( front.cvars.CurToken != T_COMMA )
                    break;
                NextToken(front);
            }
        }
        MustRecog( front, T_SEMI_COLON );
    } else {
//        Check_Struct_Union_Enum( info.typ );
        NextToken(front);                /* skip over ';' */
    }
    return( true );    /* We found a declaration */
}


TYPEPTR TypeName( FrontRun& front )
{
    TYPEPTR     typ;
    decl_info   info;
    SYM_ENTRY   abs_sym;

    TypeSpecifier( front, &info );
    typ = info.typ;
    if( typ != NULL ) {
         memset( &abs_sym, 0, sizeof( SYM_ENTRY ) );
         abs_sym.name = const_cast<char *>("");
         AbsDecl( front, &abs_sym, info.mod, info.typ  );
         typ = abs_sym.sym_type;
    }
    return( typ );
}


static type_modifiers GetModifiers( FrontRun& front )
{
    type_modifiers      modifier;

    modifier = static_cast<type_modifiers>(0);
    for( ; TokenClass[front.cvars.CurToken] == TC_MODIFIER; ) {
        switch( front.cvars.CurToken ) {
        case T___NEAR:      modifier |= FLAG_NEAR;      break;
        case T___FAR:       modifier |= FLAG_FAR;       break;
#if _CPU == 8086
        case T__FAR16:
        case T___FAR16:     modifier |= FLAG_FAR;       break;
        case T___HUGE:      modifier |= FLAG_HUGE;      break;
#else
        case T__FAR16:
        case T___FAR16:     modifier |= FLAG_FAR16;     break;
        case T___HUGE:      modifier |= FLAG_FAR;       break;
#endif
        case T___INTERRUPT: modifier |= FLAG_INTERRUPT; break;
        case T__CDECL:
        case T___CDECL:     modifier |= FLAG_CDECL;     break;
        case T___FASTCALL:  modifier |= FLAG_FASTCALL;  break;
        case T___FORTRAN:   modifier |= FLAG_FORTRAN;   break;
        case T__OPTLINK:    modifier |= FLAG_OPTLINK;   break;
        case T__PASCAL:
        case T___PASCAL:    modifier |= FLAG_PASCAL;    break;
        case T___STDCALL:   modifier |= FLAG_STDCALL;   break;
        case T__SYSCALL:
        case T___SYSCALL:
        case T__SYSTEM:     modifier |= FLAG_SYSCALL;   break;
        case T___WATCALL:   modifier |= FLAG_WATCALL;   break;
        case T__EXPORT:
        case T___EXPORT:    modifier |= FLAG_EXPORT;    break;
        case T___LOADDS:    modifier |= FLAG_LOADDS;    break;
        case T___SAVEREGS:  modifier |= FLAG_SAVEREGS;  break;
        default:
            break;
        }
        NextToken(front);
    }
    return( modifier );
}

struct mod_info {
    segment_id       segid;
    type_modifiers   modifier;  // const, vol flags
    BASED_KIND       based_kind;
    SYM_HANDLE       based_sym;
};

static TYPEPTR Pointer( FrontRun& front, TYPEPTR typ, struct mod_info *info )
{
    type_modifiers  flags;
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;

    sym_handle = SYM_NULL;
    if( (typ != NULL) && (typ->decl_type == TYP_TYPEDEF) ) {
        /*
         * get segment from typedef TODO should be done sooner
         */
        TYPEPTR     ptr_typ;
        SYMPTR      symp;

        ptr_typ = typ;
        SKIP_DUMMY_TYPEDEFS( ptr_typ );
        if( ptr_typ->decl_type == TYP_TYPEDEF ) {
            symp = SymGetPtr( front, ptr_typ->u.typedefn );
            if( info->modifier & FLAG_BASED ) {
                info->segid = symp->u.var.segid;
                sym_handle = SegSymHandle( front, info->segid );
            }
        }
    }
    for( ;; ) {
        flags = GetModifiers(front);   // NEAR FAR CDECL stuff
        if( flags & info->modifier ) {
            CWarn1( front, ERR_REPEATED_MODIFIER );
        }
        info->modifier |= flags;
        if( front.cvars.CurToken == T___BASED ) {
            bool        use_seg;
            id_hash_idx hash;

            use_seg = false;
            NextToken(front);
            MustRecog( front, T_LEFT_PAREN );
            info->based_kind = BASED_NONE;
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                NextToken(front);
                MustRecog( front, T___SEGMENT );
                MustRecog( front, T_RIGHT_PAREN );
                use_seg = true;
            }
            switch( front.cvars.CurToken ) {
            case T_ID:                                  /* __based(variable) */
                hash = CalcHashID( front.cvars.Buffer );
                sym_handle = SymLook( front, hash, front.cvars.Buffer );
                if( sym_handle == SYM_NULL ) {
                    SymCreate( front, &sym, front.cvars.Buffer );
                    sym.attribs.stg_class = SC_EXTERN;  /* indicate extern decl */
                    CErr2p( front, ERR_UNDECLARED_SYM, front.cvars.Buffer );
                    sym.sym_type = GetType( front, TYP_INT );
                    sym_handle = SymAdd( front, hash, &sym );
                } else {
                    TYPEPTR     sym_typ;

                    SymGet( front, &sym, sym_handle );
                    sym_typ = sym.sym_type;
                    SKIP_TYPEDEFS( sym_typ );
                    if( use_seg ) {
                        info->based_kind = BASED_VARSEG;
                        if( sym_typ->decl_type != TYP_POINTER ) {
                           CErr1( front, ERR_SYM_MUST_BE_TYPE_SEGMENT );
                           info->based_kind = BASED_NONE;
                        }
                    } else if( sym_typ->decl_type == TYP_POINTER ) {
                        info->based_kind = BASED_VAR;
                    } else if( sym_typ->decl_type < TYP_FLOAT ) {
                        info->based_kind = BASED_SEGVAR;
                    } else {
                        CErr1( front, ERR_SYM_MUST_BE_TYPE_SEGMENT );
                        info->based_kind = BASED_NONE;
                    }
                }
                if( (sym.flags & SYM_REFERENCED) == 0 ) {
                    sym.flags |= SYM_REFERENCED;
                    SymReplace( front, &sym, sym_handle );
                }
                NextToken(front);
                break;
            case T_VOID:            /* __based(void)  */
                if( use_seg ) {
                    CErr1( front, ERR_INVALID_BASED_DECLARATOR );
                }
                info->based_kind = BASED_VOID;
                NextToken(front);
                break;
            case T___SEGNAME:       /* __based(__segname("string")) */
                if( use_seg ) {
                    CErr1( front, ERR_INVALID_BASED_DECLARATOR );
                }
                info->based_kind = BASED_SEGNAME;
                NextToken(front);
                MustRecog( front, T_LEFT_PAREN );
                if( front.cvars.CurToken == T_STRING ) {
                    info->segid = AddSegName( front, front.cvars.Buffer, "", SEGTYPE_BASED);
                    sym_handle = SegSymHandle( front, info->segid );
                    SymGet( front, &sym, sym_handle );
                    sym.u.var.segid = info->segid;
                    SymReplace( front, &sym, sym_handle );
                    NextToken(front);
                } else {
                    CErr1( front, ERR_SEGMENT_NAME_REQUIRED );
                    info->based_kind = BASED_NONE;
                }
                MustRecog( front, T_RIGHT_PAREN );
                break;
            case T___SELF:          /* __based(__self) */
                info->based_kind = BASED_SELFSEG;
                NextToken(front);
                break;
            case T_AND:             /* __based((__segment) &var ) */
                if( !use_seg ) {
                    CErr1( front, ERR_INVALID_BASED_DECLARATOR );
                }
                NextToken(front);
                if( front.cvars.CurToken == T_ID ) {
                    hash = CalcHashID( front.cvars.Buffer );
                    sym_handle = SymLook( front, hash, front.cvars.Buffer );
                    if( sym_handle == SYM_NULL ) {
                        SymCreate( front, &sym, front.cvars.Buffer );
                        sym.attribs.stg_class = SC_EXTERN;  /* indicate extern decl */
                        CErr2p( front, ERR_UNDECLARED_SYM, front.cvars.Buffer );
                        sym.sym_type = GetType( front, TYP_INT );
                        sym_handle = SymAdd( front, hash, &sym );
                    } else {
                        SymGet( front, &sym, sym_handle );
                    }
                    if( (sym.flags & SYM_REFERENCED) == 0 ) {
                        sym.flags |= SYM_REFERENCED;
                        SymReplace( front, &sym, sym_handle );
                    }
                    info->based_kind = BASED_SEGNAME;
                    NextToken(front);
                } else {
                    CErr1( front, ERR_INVALID_BASED_DECLARATOR );
                }
                break;
            default:
                CErr1( front, ERR_INVALID_BASED_DECLARATOR );
                break;
            }
            MustRecog( front, T_RIGHT_PAREN );
            info->modifier &= ~MASK_ALL_MEM_MODELS;
            info->modifier |= FLAG_NEAR | FLAG_BASED;
        }
        if( front.cvars.CurToken == T_TIMES ) {
            NextToken(front);
            /*
             * * seg16 binds with * cause of IBM dorks, and so does far16
             */
            if( (front.cvars.CurToken == T__SEG16) || (front.cvars.CurToken == T__FAR16) || (front.cvars.CurToken == T___FAR16) ) {
    #if _CPU == 8086
                info->modifier |= FLAG_FAR;
    #else /* _CPU == 386 */
                info->modifier |= FLAG_FAR16;
    #endif
                NextToken(front);
            }
            flags = info->modifier & ~FLAG_EXPORT;
            typ = BPtrNode( front, typ, flags, info->segid, sym_handle, info->based_kind );
            sym_handle = SYM_NULL;
            info->segid = SEG_NULL;  // start over
            info->modifier = (flags & (FLAG_INLINE | FLAG_ABORTS | FLAG_NORETURN)) | TypeQualifier(front);  // .. * const
            info->based_kind = BASED_NONE;
        } else {
            break;
        }
    }
    return( typ );
}


static void ParseDeclPart2( FrontRun& front, TYPEPTR *typep, TYPEPTR typ, type_modifiers mod )
{
    TYPEPTR         decl1;
    TYPEPTR         decl2;

    decl1 = *typep;
    if( decl1 != NULL ) {
        while( decl1->object != NULL ) {
            decl1 = decl1->object;
        }
    }
    /*
     * Pass on pointer flags
     */
    if( (decl1 != NULL) && (decl1->decl_type == TYP_POINTER) )
        mod = decl1->u.p.decl_flags;
    decl2 = DeclPart2( front, typ, mod );
    if( decl1 == NULL ) {
        *typep = decl2;
    } else {
        decl1->object = decl2;
        if( decl1->decl_type == TYP_POINTER && decl2 != NULL ) {
            AddPtrTypeHash( front, decl1 );
        }
    }
}


static void AbsDecl( FrontRun& front, SYMPTR sym, type_modifiers mod, TYPEPTR typ )
{
    struct mod_info     info;

    info.segid = SEG_NULL;
    info.modifier = mod;
    info.based_kind = BASED_NONE;
    info.based_sym = SYM_NULL;
    typ = Pointer( front, typ, &info );
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        NextToken(front);
        if( front.cvars.CurToken == T_RIGHT_PAREN ) {
            typ = FuncNode( front, typ, info.modifier, NULL );
        } else {
            AbsDecl( front, sym, info.modifier, (TYPEPTR)NULL );
            info.modifier = FLAG_NONE;
            MustRecog( front, T_RIGHT_PAREN );
            ParseDeclPart2( front, &sym->sym_type, typ, FLAG_NONE );
        }
    } else {
        sym->mods = info.modifier;
        sym->u.var.segid = info.segid;
        typ = DeclPart2( front, typ, info.modifier );
        sym->sym_type = typ;
    }
}

void Declarator( FrontRun& front, SYMPTR sym, type_modifiers mod, TYPEPTR typ, decl_state state )
{
    TYPEPTR             *type_list;
    TYPEPTR             parm_type;
    struct mod_info     info;

    info.segid = SEG_NULL;
    info.modifier = mod;
    info.based_kind = BASED_NONE;
    info.based_sym = SYM_NULL;
    typ = Pointer( front, typ, &info );
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        NextToken(front);
        if( state & DECL_STATE_ISPARM ) {
            parm_type = TypeName(front);
        } else {
            parm_type = NULL;
        }
        if( parm_type != NULL || front.cvars.CurToken == T_RIGHT_PAREN ) {
            type_list = NULL;
            if( parm_type != NULL ) {
                type_list = MakeParmList( front, NewParm( front, parm_type, NULL ), false );
            }
            typ = FuncNode( front, typ, info.modifier, type_list );
            typ = PtrNode( front, typ, FLAG_NONE, SEG_DATA );
            MustRecog( front, T_RIGHT_PAREN );
        } else {
            if( (state & DECL_STATE_ISPARM) && TokenClass[front.cvars.CurToken] == TC_STG_CLASS ) {
                typ = DeclPart3( front, typ, info.modifier );
            } else {
                Declarator( front, sym, info.modifier, (TYPEPTR)NULL, state );
                info.modifier = FLAG_NONE;
                MustRecog( front, T_RIGHT_PAREN );
            }
        }
        ParseDeclPart2( front, &sym->sym_type, typ, sym->mods );
        typ = sym->sym_type;
    } else {
        if( (front.cvars.CurToken == T_ID) || (front.cvars.CurToken == T_SAVED_ID) ) {
            for( ;; ) {
                if( front.cvars.CurToken == T_ID ) {
                    SymCreate( front, sym, front.cvars.Buffer );
                    sym->u1.hash = CalcHashID( front.cvars.Buffer );
                    NextToken(front);
                } else {
                    SymCreate( front, sym, front.cvars.SavedId );
                    sym->u1.hash = CalcHashID( front.cvars.SavedId );
                    front.cvars.CurToken = front.cvars.LAToken;
                }
                if( front.cvars.CurToken != T_ID && front.cvars.CurToken != T_TIMES )
                    break;
                if( state & DECL_STATE_NOTYPE ) {
                    CErr2p( front, ERR_MISSING_DATA_TYPE, sym->name );
                    if( front.cvars.CurToken == T_TIMES ) { /* "garbage *p" */
                        typ = Pointer( front, typ, &info );
                    }
                } else if( front.cvars.CurToken == T_TIMES ) {
                    Expecting( front, "," );
                } else {
                    Expecting( front, ",' or ';" );
                }
                if( front.cvars.CurToken != T_ID )
                    break;
                CMemFree( front, sym->name );
            }
        } else {
            SymCreate( front, sym, "" );
        }
        sym->mods = info.modifier;
        sym->u.var.segid = info.segid;
        typ = DeclPart2( front, typ, info.modifier );
        sym->sym_type = typ;
    }
    if( typ != NULL ) {
        if( typ->decl_type == TYP_FUNCTION ) {
            if( state & DECL_STATE_FORLOOP ) {
                CErr2p( front, ERR_DECL_IN_LOOP_NOT_OBJECT, sym->name );
            } else if( info.segid != SEG_NULL ) {           // __based( __segname("X"))
                SetFuncSegment( front, sym, info.segid );
            }
        }
    }
}


FIELDPTR FieldCreate( FrontRun& front, const char *name )
{
    FIELDPTR    field;

    field = (FIELDPTR)CPermAlloc( front, sizeof( FIELD_ENTRY ) + strlen( name ) );
    strcpy( field->name, name );
    if( front.cvars.CompFlags.emit_browser_info ) {
        field->xref = NewXref( front, NULL );
    }
    return( field );
}


FIELDPTR FieldDecl( FrontRun& front, TYPEPTR typ, type_modifiers mod, decl_state state )
{
    FIELDPTR            field;
    struct mod_info     info;

    info.segid = SEG_NULL;
    info.modifier = mod;
    info.based_kind = BASED_NONE;
    info.based_sym = SYM_NULL;
    typ = Pointer( front, typ, &info );
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        NextToken(front);
        field = FieldDecl( front, (TYPEPTR)NULL, info.modifier, DECL_STATE_NONE );
        info.modifier = FLAG_NONE;
        MustRecog( front, T_RIGHT_PAREN );
        ParseDeclPart2( front, &field->field_type, typ, FLAG_NONE );
    } else {
        if( front.cvars.CurToken == T_ID ) {
            for( ;; ) {
                field = FieldCreate( front, front.cvars.Buffer );
                NextToken(front);
                if( front.cvars.CurToken != T_ID && front.cvars.CurToken != T_TIMES )
                    break;
                if( state & DECL_STATE_NOTYPE ) {
                    CErr2p( front, ERR_MISSING_DATA_TYPE, field->name );
                    if( front.cvars.CurToken == T_TIMES ) { /* "garbage *p" */
                        typ = Pointer( front, typ, &info );
                    }
                } else if( front.cvars.CurToken == T_TIMES ) {
                    Expecting( front, "," );
                } else {
                    Expecting( front, ",' or ';" );
                }
                if( front.cvars.CurToken != T_ID ) {
                    break;
                }
            }
        } else {
            field = FieldCreate( front, "" );
        }
        field->attrib = info.modifier;
        typ = DeclPart2( front, typ, info.modifier );
        field->field_type = typ;
    }
    return( field );
}


static TYPEPTR ArrayDecl( FrontRun& front, TYPEPTR typ )
{
    target_size dimension;
    TYPEPTR     first_node, next_node, prev_node;

    if( typ != NULL ) {
        if( typ->decl_type == TYP_FUNCTION ) {
            CErr1( front, ERR_CANT_HAVE_AN_ARRAY_OF_FUNCTIONS );
        }
    }
    first_node = NULL;
    prev_node = NULL;
    while( front.cvars.CurToken == T_LEFT_BRACKET ) {
        NextToken(front);
        if( front.cvars.CurToken != T_RIGHT_BRACKET ) {
            const_val   val;

            if( ConstExprAndType( front, &val ) ) {
                dimension = U32FetchTrunc( val.value );
                switch( val.type ) {
                case TYP_ULONG64:
                    if( !U64IsU32( val.value ) )
                        CErr1( front, ERR_CONSTANT_TOO_BIG );
                    break;
                case TYP_LONG64:
                    if( !U64IsI32( val.value ) )
                        CErr1( front, ERR_CONSTANT_TOO_BIG );
                    break;
                case TYP_ULONG:
                case TYP_UINT:
                    break;
                default:
                    if( I32FetchTrunc( val.value ) <= 0 ) {
                        CErr1( front, ERR_INVALID_DIMENSION );
                        dimension = 1;
                    }
                    break;
                }
            } else {
                dimension = 1;
            }
        } else {
            if( first_node == NULL ) {
                dimension = 0;
            } else {
                CErr1( front, ERR_DIMENSION_REQUIRED );
                dimension = 1;
            }
        }
        MustRecog( front, T_RIGHT_BRACKET );
        next_node = ArrayNode( front, typ );
        next_node->u.array->dimension = dimension;
        next_node->u.array->unspecified_dim = ( dimension == 0 );
        if( first_node == NULL ) {
            first_node = next_node;
        } else {
            prev_node->object = next_node;
        }
        prev_node = next_node;
    }
    return( first_node );
}


static TYPEPTR  *FuncProtoType( FrontRun& front );

static TYPEPTR DeclPart3( FrontRun& front, TYPEPTR typ, type_modifiers mod )
{
    PARMPTR     parms_list;
    TYPEPTR     *parms;

    parms = NULL;
    if( front.cvars.CurToken != T_RIGHT_PAREN ) {
        parms_list = front.cvars.ParmList;
        front.cvars.ParmList = NULL;
        parms = FuncProtoType(front);
        if( (parms == NULL) && (front.cvars.ParmList != NULL) ) {
            if( front.cvars.CurToken == T_SEMI_COLON || front.cvars.CurToken == T_COMMA ) {
                /*
                 * int f16(i,j);
                 */
                CErr1( front, ERR_ID_LIST_SHOULD_BE_EMPTY );
            }
            /*
             * Old-style declarations are obsolescent (ever since ANSI C89!)
             */
            CWarn1( front, ERR_OBSOLETE_FUNC_DECL );
        }
        if( parms_list != NULL ) {
            FreeParmList(front);
            front.cvars.ParmList = parms_list;
        }
    } else {
        NextToken(front);    /* skip over ')' */
        /*
         * Non-prototype declarators are obsolescent too; however, __interrupt
         * functions have a special exemption due to messy historical usage,
         * with variants both with and without arguments in use. Note that
         * __interrupt functions are unlikely to be called directly.
         */
        if( (mod & FLAG_INTERRUPT) == 0 ) {
            CWarn1( front, ERR_OBSOLETE_FUNC_DECL );
        }
    }
    if( typ != NULL ) {
        TYPEPTR     typ2;

        typ2 = typ;
        SKIP_TYPEDEFS( typ2 );
        if( typ2->decl_type == TYP_ARRAY ) {
            CErr1( front, ERR_FUNCTION_CANT_RETURN_AN_ARRAY );
        } else if( typ2->decl_type == TYP_FUNCTION ) {
            CErr1( front, ERR_FUNCTION_CANT_RETURN_A_FUNCTION );
        }
    }
    typ = FuncNode( front, typ, mod, parms );
    return( typ );
}


static TYPEPTR DeclPart2( FrontRun& front, TYPEPTR typ, type_modifiers mod )
{
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        NextToken(front);
        typ = DeclPart3( front, typ, mod );
    }
    for( ;; ) {
        if( front.cvars.CurToken == T_LEFT_BRACKET ) {
            typ = ArrayDecl( front, typ );
        }
        if( front.cvars.CurToken != T_LEFT_PAREN )
            break;
        NextToken(front);
        typ = DeclPart3( front, typ, mod );
    }
    return( typ );
}


static void CheckUniqueName( FrontRun& front, PARMPTR parm, const char *name )
{
    if( name != NULL && *name != '\0' ) {
        for( ; parm != NULL; parm = parm->next_parm ) {
            if( parm->sym.name != NULL ) {
                if( strcmp( parm->sym.name, name ) == 0 ) {
                    CErr2p( front, ERR_SYM_ALREADY_DEFINED, name );
                    parm->sym.flags |= SYM_REFERENCED;
                }
            }
        }
    }
}


parm_list *NewParm( FrontRun& front, TYPEPTR typ, parm_list *prev_parm )
{
    parm_list    *parm;

    parm = (parm_list *)CMemAlloc( front, sizeof( parm_list ) );
    parm->parm_type = typ;
    parm->next_parm = prev_parm;
    return( parm );
}


void AdjParmType( FrontRun& front, SYMPTR sym )
{
    TYPEPTR     typ;

    typ = sym->sym_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_FUNCTION ) {
        sym->sym_type = PtrNode( front, sym->sym_type, sym->mods, SEG_CODE );
        sym->mods = FLAG_NONE;
    } else if( typ->decl_type == TYP_ARRAY ) {
        sym->sym_type = PtrNode( front, typ->object, FLAG_WAS_ARRAY | sym->mods, SEG_DATA );
        sym->mods = FLAG_NONE;
    }
}


static TYPEPTR *GetProtoType( FrontRun& front, decl_info *first )
{
    PARMPTR             parm;
    PARMPTR             prev_parm = NULL;
    PARMPTR             parm_namelist;
    int                 parm_count;
    parm_list           *parmlist;
    decl_state          state;
//    declspec_class      declspec;
    stg_classes         stg_class;
    type_modifiers      mod;
    TYPEPTR             typ;
    decl_info           info;

    parm_count = 0;
    parmlist = NULL;
    parm_namelist = NULL;
    info = *first;
    for( ;; ) {
        SYMPTR           sym; // parm sym

        stg_class = info.stg;
//        declspec = info.decl;
        typ = info.typ;
        mod = info.mod;
        if( stg_class != SC_NONE && stg_class != SC_REGISTER ) {
            CErr1( front, ERR_INVALID_STG_CLASS_FOR_PARM_PROTO );
        }
        state = DECL_STATE_ISPARM;
        if( typ == NULL ) {
            state |= DECL_STATE_NOTYPE;
            if( stg_class == SC_NONE ) {
                CErr1( front, ERR_TYPE_REQUIRED_IN_PARM_LIST );
            }
            typ = TypeDefault(front);
        }
        parm = (PARMPTR) CMemAlloc( front, sizeof( PARM_ENTRY ) );
        parm->next_parm = NULL;
        sym = &parm->sym;
        memset( sym, 0, sizeof( SYM_ENTRY ) );
        sym->name = const_cast<char *>("");
        Declarator( front, sym, mod, typ, state );
        typ = sym->sym_type;
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_VOID ) {
            char    buffer[20];
            char    *name;
            if( sym->name[0] == '\0' ) {
                sprintf( buffer, "Parm %d", parm_count );
                name = buffer;
            } else {
                name = sym->name;
            }
            CErr2p( front, ERR_VAR_CANT_BE_VOID, name );
            sym->sym_type = TypeDefault(front);
        }
        if( stg_class == SC_NONE ) {
            stg_class = SCSpecifier(front);
            if( stg_class == SC_NONE ) {
                stg_class = SC_AUTO;
            }
        }
        sym->attribs.stg_class = stg_class;
        AdjModsTypeNode( front, &sym->sym_type, info.decl_mod, sym );
        AdjParmType( front, sym );
        parmlist = NewParm( front, sym->sym_type, parmlist );
        if( parm_count == 0 ) {
            parm_namelist = parm;
        } else {
            CheckUniqueName( front, parm_namelist, sym->name );
            prev_parm->next_parm = parm;
        }
        if( !TOGGLE( unreferenced ) ) {
            sym->flags |= SYM_REFERENCED;
        }
        ++parm_count;
        if( front.cvars.CurToken == T_RIGHT_PAREN )
            break;
        if( (front.cvars.CurToken == T_EOF) || (front.cvars.CurToken == T_LEFT_BRACE) )
            break;
        MustRecog( front, T_COMMA );
        if( front.cvars.CurToken == T_DOT_DOT_DOT ) {
            typ = GetType( front, TYP_DOT_DOT_DOT );
            parmlist = NewParm( front, typ, parmlist );
            NextToken(front);
            break;
        }
        prev_parm = parm;
        FullDeclSpecifier( front, &info );
    }
    front.cvars.ParmList = parm_namelist;
    /*
     * if void is specified as a parm, it is the only parm allowed
     */
    return( MakeParmList( front, parmlist, false ) );
}


TYPEPTR *MakeParmList( FrontRun& front, parm_list *parm, bool reversed )
{
    TYPEPTR             *parm_types;
    parm_list           *next_parm;
    parm_list           *prev_parm;
    TYPEPTR             typ;
    parm_hash_idx       hash;
    int                 parm_count;

    parm_types = NULL;
    if( parm != NULL ) {
        if( !reversed ) {
            prev_parm = NULL;
            for( ;; ) {
                next_parm = parm->next_parm;
                parm->next_parm = prev_parm;
                if( next_parm == NULL )
                    break;
                prev_parm = parm;
                parm = next_parm;
            }
        }
        parm_count = 0;
        for( next_parm = parm; next_parm != NULL; next_parm = next_parm->next_parm ) {
            ++parm_count;
        }
        /*
         * try to find an existing parm list that matches
         */
        hash = MAX_PARM_LIST_HASH_SIZE;
        if( parm_count < MAX_PARM_LIST_HASH_SIZE ) {
            hash = (parm_hash_idx)parm_count;
        }
        for( typ = front.cvars.FuncTypeHead[hash]; typ != NULL; typ = typ->next_type ) {
            next_parm = parm;
            for( parm_types = typ->u.fn.parms; ; ++parm_types ) {
                if( next_parm == NULL ) {
                    if( *parm_types != NULL )
                        break;
                    while( parm != NULL ) {
                        next_parm = parm->next_parm;
                        CMemFree( front, parm );
                        parm = next_parm;
                    }
                    return( typ->u.fn.parms );
                }
                if( next_parm->parm_type != *parm_types )
                    break;
                next_parm = next_parm->next_parm;
            }
        }

        parm_types = (TYPEPTR *)CPermAlloc( front, ( parm_count + 1 ) * sizeof( TYPEPTR ) );
        if( parm_types != NULL ) {
            parm_types[parm_count] = NULL;
            parm_count = 0;
            for( ; parm != NULL; parm = next_parm ) {
                next_parm = parm->next_parm;
                parm_types[parm_count++] = parm->parm_type;
                CMemFree( front, parm );
            }
        }
    }
    return( parm_types );
}

static bool VoidType( TYPEPTR typ )
{
    if( typ != NULL ) {
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_VOID ) {
            return( true );
        }
    }
    return( false );
}

static TYPEPTR *FuncProtoType( FrontRun& front )
{
    TYPEPTR             *parm_types;
    TAGPTR              old_taghead;
    decl_info           info;

    if( !front.cvars.CompFlags.extensions_enabled ) {
        ++front.cvars.SymLevel;
    }
    old_taghead = front.cvars.TagHead;
    FullDeclSpecifier( front, &info );
    if( (info.stg == SC_NONE) && (info.typ == NULL) ) {
        GetFuncParmList(front);
        if( ExpectingToken( front, T_RIGHT_PAREN ) ) {
            NextToken(front);
        }
        parm_types = NULL;
    } else {    /* function prototype present */
        if( VoidType( info.typ ) && info.stg == SC_NONE && front.cvars.CurToken == T_RIGHT_PAREN ) {
            parm_types = front.cvars.VoidParmList;
        } else {
            parm_types = GetProtoType( front, &info );
        }
        if( ExpectingToken( front, T_RIGHT_PAREN ) ) {
            NextToken(front);
        }
        if( front.cvars.CurToken != T_LEFT_BRACE ) {
            if( front.cvars.SymLevel > 1 || !front.cvars.CompFlags.extensions_enabled ) {
                /*
                 * get rid of any new tags regardless of SymLevel
                 * get rid of new tags from proto
                 */
                front.cvars.TagHead = old_taghead;
                FreeEnums(front);
            }
        }
    }
    if( !front.cvars.CompFlags.extensions_enabled ) {
        --front.cvars.SymLevel;
    }
    return( parm_types );
}


static void GetFuncParmList( FrontRun& front )
{
    PARMPTR     parm = NULL;
    PARMPTR     newparm;
    PARMPTR     parm_namelist;

    parm_namelist = NULL;
    /*
     * scan off func parm list
     */
    while( front.cvars.CurToken == T_ID ) {
        if( parm_namelist == NULL ) {
            parm = (PARMPTR)CMemAlloc( front, sizeof( PARM_ENTRY ) );
            SymCreate( front, &parm->sym, front.cvars.Buffer );
            parm_namelist = parm;
        } else {
            newparm = (PARMPTR)CMemAlloc( front, sizeof( PARM_ENTRY ) );
            SymCreate( front, &newparm->sym, front.cvars.Buffer );
            CheckUniqueName( front, parm_namelist, front.cvars.Buffer );
            parm->next_parm = newparm;
            parm = newparm;
        }
        parm->sym.u1.hash = CalcHashID( front.cvars.Buffer );
        if( !TOGGLE( unreferenced ) ) {
            parm->sym.flags |= SYM_REFERENCED;
        }
        NextToken(front);
        if( front.cvars.CurToken == T_RIGHT_PAREN )
            break;
        if( front.cvars.CurToken == T_EOF )
            break;
        if( front.cvars.CurToken != T_COMMA ) {
            MustRecog( front, T_COMMA );               /* forces error msg */
            /*
             * skip until ')' to avoid cascading errors
             */
            while( front.cvars.CurToken != T_RIGHT_PAREN && front.cvars.CurToken != T_EOF ) {
                NextToken(front);
            }
            break;
        }
        MustRecog( front, T_COMMA );
        if( front.cvars.CurToken == T_DOT_DOT_DOT ) {
            parm->next_parm = (PARMPTR)CMemAlloc( front, sizeof( PARM_ENTRY ) );
            parm = parm->next_parm;
            SymCreate( front, &parm->sym, "" );
            /*
             * set flags so we don't give funny error messages
             */
            parm->sym.flags |= SYM_TEMP | SYM_ASSIGNED | SYM_REFERENCED;
            NextToken(front);
            break;
        }
    }
    front.cvars.ParmList = parm_namelist;
}


static void FreeParmList( FrontRun& front )
{
    PARMPTR     parm;

    for( ; (parm = front.cvars.ParmList) != NULL; ) {
        front.cvars.ParmList = parm->next_parm;
        CMemFree( front, parm->sym.name );
        CMemFree( front, parm );
    }
}

