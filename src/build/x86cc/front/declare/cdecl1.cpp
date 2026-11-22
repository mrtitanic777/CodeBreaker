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
* Description:  Parse functions, prototypes, handle argument promotion.
*
****************************************************************************/


#include "cvars.h"
#include "cgswitch.h"
#include "langenv.h"
#include "cfeinfo.h"
#include "toggles.h"
#include "frontrun.hpp"


static   void    ParmDeclList( FrontRun& front );
static   void    AddParms( FrontRun& front );
static   void    CheckParms( FrontRun& front );
static   void    FuncDefn( FrontRun& front, SYMPTR );

static   void    BeginFunc( FrontRun& front );
static   void    ReverseParms( FrontRun& front );       /* reverse order of parms */


void ParsePgm( FrontRun& front )
{
    SYM_HANDLE      dummysym;

    front.cvars.CompFlags.external_defn_found = false;
    front.cvars.CompFlags.initializing_data = false;
    dummysym = SYM_NULL;
    front.cvars.GlobalSym = SYM_NULL;

    do {
        if( DeclList( front, &dummysym ) ) {  /* if this is a function defn */
            FuncDefn( front, front.cvars.CurFunc );
            front.cvars.SrcLoc = front.cvars.CurFunc->src_loc;
            GenFunctionNode( front, front.cvars.CurFuncHandle );

            front.cvars.SymLevel = 1;
            ParmDeclList(front);
            front.cvars.SymLevel = 0;
            if( front.cvars.CurToken == T_LEFT_BRACE ) {
                BeginFunc(front);
                Statement(front);
                CMemFree( front, front.cvars.CurFunc->name );
                front.cvars.CurFunc->name = NULL;
                SymReplace( front, front.cvars.CurFunc, front.cvars.CurFuncHandle );
                front.cvars.CurFunc = NULL;
                front.cvars.CurFuncNode = NULL;
                front.cvars.CurFuncHandle = SYM_NULL;
            } else {
                MustRecog( front, T_LEFT_BRACE );
            }
        }
    } while( front.cvars.CurToken != T_EOF );

    if( !front.cvars.CompFlags.external_defn_found ) {
        if( !front.cvars.CompFlags.extensions_enabled ) {
            CErr1( front, ERR_NO_EXTERNAL_DEFNS_FOUND );
        }
    }
}


static void FuncDefn( FrontRun& front, SYMPTR sym )
{
    SYM_NAMEPTR sym_name;
    TYPEPTR     typ;

    /*
     * duplicate name in near space
     */
    sym_name = SymName( front, sym, front.cvars.CurFuncHandle );
    sym->name = CMemStrdup( front, sym_name );
    if( sym->flags & SYM_DEFINED ) {
        CErr2p( front, ERR_SYM_ALREADY_DEFINED, sym->name );
    }
    typ = sym->sym_type->object;                /* get return type */
    SKIP_TYPEDEFS( typ );

    if( typ->decl_type != TYP_VOID ) {
        if( TypeSize( front, typ ) == 0 ) {
            CErr2p( front, ERR_INCOMPLETE_TYPE, sym_name );
        }
    }
    sym->flags |= SYM_DEFINED /* | SYM_REFERENCED */;

    if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
        sym->flags |= SYM_OK_TO_RECURSE;
    }

    if( sym->attribs.stg_class == SC_EXTERN || sym->attribs.stg_class == SC_FORWARD ) {
        sym->attribs.stg_class = SC_NONE;       /* indicate exported function */
    }

    front.cvars.CompFlags.external_defn_found = true;
    if( TOGGLE( check_stack ) )
        sym->flags |= SYM_CHECK_STACK;

#if _INTEL_CPU
    if( !front.cvars.CompFlags.zu_switch_used ) {
        if( sym->mods & FLAG_FARSS ) {          /* function use far stack */
            front.cvars.TargetSwitches |= CGSW_X86_FLOATING_SS;
        } else {
            front.cvars.TargetSwitches &= ~CGSW_X86_FLOATING_SS;
        }
    }
#endif
    if( strcmp( front.cvars.CurFunc->name, "main" ) == 0
      || strcmp( front.cvars.CurFunc->name, "wmain" ) == 0 ) {
        sym->mods &= ~MASK_LANGUAGES;  // Turn off any language flags
        sym->mods |= FLAG_WATCALL;     // Turn on __watcall calling convention for main
    }
    SymReplace( front, sym, front.cvars.CurFuncHandle );
}

#define NAMELIT(c) c, sizeof( c )

#define MAIN_NAMES \
    pick( MAIN_WMAIN,    "wmain" ) \
    pick( MAIN_MAIN,     "main" ) \
    pick( MAIN_WWINMAIN, "wWinMain" ) \
    pick( MAIN_WINMAIN,  "WinMain" ) \
    pick( MAIN_WLIBMAIN, "wLibMain" ) \
    pick( MAIN_LIBMAIN,  "LibMain" ) \
    pick( MAIN_WDLLMAIN, "wDllMain" ) \
    pick( MAIN_DLLMAIN,  "DllMain" )

enum main_names {
    #define pick(e,n) e,
    MAIN_NAMES
    #undef pick
    MAIN_NUM,
};

static struct {
    char const      *name;
    unsigned char   len;
} const MainNames[] = {
    #define pick(e,n) { NAMELIT( n ) },
    MAIN_NAMES
    #undef pick
};

static enum main_names checkMain( const char *name )
{
    enum main_names     main_entry;

    for( main_entry = MAIN_WMAIN; main_entry < MAIN_NUM; ++main_entry ) {
       if( strcmp( name, MainNames[main_entry].name ) == 0 ) {
           break;
       }
    }
    return( main_entry );
}

static void BeginFunc( FrontRun& front )
{
    char                *name;
    char                *segname;

    if( front.cvars.CurFunc->seginfo == NULL ) {
        front.cvars.CurFunc->seginfo = front.cvars.DefCodeSegment;
        if( front.cvars.CurFunc->seginfo == NULL ) {
            if( front.cvars.CompFlags.zm_switch_used ) {
                name = const_cast<char *>("");
#if _INTEL_CPU
                if( front.cvars.TargetSwitches & CGSW_X86_BIG_CODE )
                    name = front.cvars.CurFunc->name;
#endif
                segname = const_cast<char *>(TS_SEG_CODE); /* "_TEXT" */
                if( front.cvars.TextSegName[0] != '\0' ) {
                    segname = front.cvars.TextSegName;
                }
                front.cvars.CurFunc->seginfo = NewTextSeg( front, name, segname, "" );
            }
        }
    }
    switch( checkMain( front.cvars.CurFunc->name ) ) {
    case MAIN_WMAIN:
        front.cvars.CompFlags.has_wchar_entry = true;
        /* fall through */
    case MAIN_MAIN:
        if( front.cvars.CurFunc->u.func.parms != SYM_NULL ) {
            front.cvars.CompFlags.main_has_parms = true;
        } else {
            front.cvars.CompFlags.main_has_parms = false;
        }
        front.cvars.CompFlags.has_main = true;
        break;

    case MAIN_WWINMAIN:
        front.cvars.CompFlags.has_wchar_entry = true;
        /* fall through */
    case MAIN_WINMAIN:
        if( front.cvars.TargetSystem == TS_WINDOWS || front.cvars.TargetSystem == TS_CHEAP_WINDOWS || front.cvars.TargetSystem == TS_NT ) {
            front.cvars.CompFlags.has_winmain = true;
        } else {
            front.cvars.CompFlags.has_wchar_entry = false;
        }
        break;

    case MAIN_WLIBMAIN:
    case MAIN_WDLLMAIN:
        front.cvars.CompFlags.has_wchar_entry = true;
        /* fall through */
    case MAIN_LIBMAIN:
    case MAIN_DLLMAIN:
        front.cvars.CompFlags.has_libmain = true;
        break;

    case MAIN_NUM:
        break;
    }
}

bool CheckFuncMain( const char *name )
{
    switch( checkMain( name ) ) {
    case MAIN_MAIN:
        return( true );
    case MAIN_WMAIN:
    case MAIN_WWINMAIN:
    case MAIN_WINMAIN:
    case MAIN_WLIBMAIN:
    case MAIN_WDLLMAIN:
    case MAIN_LIBMAIN:
    case MAIN_DLLMAIN:
    case MAIN_NUM:
    default:
        return( false );
    }
}

static void  ArgPromotion( FrontRun& front, SYMPTR sym )
{
    TYPEPTR     typ;
    TYPEPTR     arg_typ;

    AdjParmType( front, sym );
    arg_typ = sym->sym_type;
    /*
     * perform default argument promotions
     */
    typ = arg_typ;
    SKIP_TYPEDEFS( typ );

    switch( typ->decl_type ) {
#if 0
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_SHORT:
#endif
    case TYP_ENUM:
        arg_typ = GetType( front, TYP_INT );
        break;

#if 0
    case TYP_USHORT:
        arg_typ = GetType( TYP_UINT );
        break;

    case TYP_FLOAT:
        arg_typ = GetType( TYP_DOUBLE );
        break;
#endif
    default:
        break;
    }
}


static void ParmDeclList( FrontRun& front )     /* process old style function definitions */
{
    TYPEPTR             typ;
    PARMPTR             parm;
    decl_state          state;
    SYM_ENTRY           sym;
    decl_info           info;

    while( front.cvars.CurToken != T_LEFT_BRACE ) {
        FullDeclSpecifier( front, &info );
        if( info.stg == SC_NONE && info.typ == NULL ) {
            if( front.cvars.CurToken == T_ID ) {
                CErr2p( front, ERR_MISSING_DATA_TYPE, front.cvars.Buffer );
            }
        }
        if( info.stg != SC_NONE && info.stg != SC_REGISTER ) {
            CErr1( front, ERR_INVALID_STG_CLASS_FOR_PARM );
            info.stg = SC_NONE;
        }
        state = DECL_STATE_NONE;
        typ = info.typ;
        if( typ == NULL ) {
            state |= DECL_STATE_NOTYPE;
            typ = TypeDefault(front);
        }
        if( info.stg == SC_NONE )
            info.stg = SC_AUTO;

        for( ;; ) {
            if( front.cvars.CurToken == T_SEMI_COLON ) {
                Check_Struct_Union_Enum( front, typ );
            } else {
                sym.name = NULL;
                Declarator( front, &sym, info.mod, typ, state );
                if( sym.name == NULL || sym.name[0] == '\0' ) {
                    InvDecl(front);
                } else {
                    for( parm = front.cvars.ParmList; parm != NULL; parm = parm->next_parm ) {
                        if( parm->sym.name != NULL ) {
                            if( strcmp( parm->sym.name, sym.name ) == 0 ) {
                                break;
                            }
                        }
                    }
                    if( parm == NULL ) {
                        CErr2p( front, ERR_SYM_NOT_IN_PARM_LIST, sym.name );
                    } else if( parm->sym.sym_type != NULL ) {
                        CErr2p( front, ERR_PARM_ALREADY_DECLARED, sym.name );
                    } else {
                        ArgPromotion( front, &sym );
                        parm->sym.sym_type = sym.sym_type;
                        parm->sym.attribs.stg_class = info.stg;
                    }
                }
                CMemFree( front, sym.name );
            }

            if( front.cvars.CurToken == T_SEMI_COLON ) {
                NextToken(front);
                break;
            }
            if( front.cvars.CurToken == T_LEFT_BRACE ) {
                CErr1( front, ERR_MISSING_SEMICOLON );
                break;
            }
            if( front.cvars.CurToken == T_EOF )
                return;
            MustRecog( front, T_COMMA );
        }
    }
    ReverseParms(front);
    if( front.cvars.CurFunc->sym_type->u.fn.parms == NULL ) {
        front.cvars.CurFunc->flags |= SYM_OLD_STYLE_FUNC;
        AddParms(front);
    } else {
        CheckParms(front);
    }
    front.cvars.ParmList = NULL;
    if( VarParm( front.cvars.CurFunc ) ) {
        front.cvars.CurFunc->flags &= ~ SYM_OK_TO_RECURSE;
    }
}


static void ReverseParms( FrontRun& front )       /* reverse order of parms */
{
    PARMPTR     prev_parm, parm, next_parm;

    if( ParmsToBeReversed( front.cvars.CurFunc->mods, NULL ) ) {
        prev_parm = NULL;
        for( parm = front.cvars.ParmList; parm != NULL; parm = next_parm ) {
            next_parm = parm->next_parm;
            parm->next_parm = prev_parm;
            prev_parm = parm;
        }
        front.cvars.ParmList = prev_parm;
    }
}


static void CheckProtoType( FrontRun& front )
{
    TYPEPTR     ret1;
    TYPEPTR     ret2;
    TYPEPTR     typ1;
    TYPEPTR     typ2;

    typ1 = front.cvars.CurFunc->sym_type;
    SKIP_TYPEDEFS( typ1 );
    ret1 = typ1->object;
    typ2 = front.cvars.PrevProtoType;
    SKIP_TYPEDEFS( typ2 );
    ret2 = typ2->object;
    typ1->object = NULL;
    typ2->object = NULL;
    VerifyType( front, front.cvars.CurFunc->sym_type, front.cvars.PrevProtoType, front.cvars.CurFunc );
    typ1->object = ret1;
    typ2->object = ret2;
}


static void AddParms( FrontRun& front )
{
    PARMPTR             parm;
    PARMPTR             prev_parm;
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          prev_sym_handle;
    SYM_HANDLE          new_sym_handle;
    TYPEPTR             typ = NULL;
    int                 parm_count;
    id_hash_idx         hash;
    parm_list           *parmlist;
    SYM_ENTRY           new_sym;

    front.cvars.CurFunc->u.func.locals = SYM_NULL;
    front.cvars.CurFunc->u.func.parms = SYM_NULL;
    parmlist = NULL;
    prev_sym_handle = SYM_NULL;
    parm_count = 0;
    prev_parm = NULL;
    for( parm = front.cvars.ParmList; parm != NULL; parm = parm->next_parm ) {
        new_sym_handle = SYM_NULL;
        parm->sym.flags |= SYM_DEFINED | SYM_ASSIGNED;
        parm->sym.attribs.is_parm = true;
        hash = parm->sym.u1.hash;
        if( parm->sym.name[0] == '\0' ) {
            /*
             * no name ==> ...
             */
            parm->sym.sym_type = GetType( front, TYP_DOT_DOT_DOT );
            parm->sym.attribs.stg_class = SC_AUTO;
        } else if( parm->sym.sym_type == NULL ) {
            parm->sym.sym_type = TypeDefault(front);
            parm->sym.attribs.stg_class = SC_AUTO;
        } else {
            /*
             * go through ParmList again, looking for FLOAT parms
             * change the name to ".P" and duplicate the symbol with type
             * float and generate an assignment statement.
             */
            typ = parm->sym.sym_type;
            SKIP_TYPEDEFS( typ );

            switch( typ->decl_type ) {
            case TYP_CHAR:
            case TYP_UCHAR:
            case TYP_SHORT:
                if( front.cvars.CompFlags.strict_ANSI ) {
                    parm->sym.sym_type = GetType( front, TYP_INT );
                }
                break;

            case TYP_USHORT:
                if( front.cvars.CompFlags.strict_ANSI ) {
#if TARGET_SHORT == TARGET_INT
                    parm->sym.sym_type = GetType( front, TYP_UINT );
#else
                    parm->sym.sym_type = GetType( front, TYP_INT );
#endif
                }
                break;

            case TYP_FLOAT:
                memcpy( &new_sym, &parm->sym, sizeof( SYM_ENTRY ) );
                new_sym.handle = front.cvars.CurFunc->u.func.locals;
                new_sym_handle = SymAdd( front, hash, &new_sym );
                front.cvars.CurFunc->u.func.locals = new_sym_handle;
                SymReplace( front, &new_sym, new_sym_handle );
                parm->sym.name = const_cast<char *>(".P");
                parm->sym.flags |= SYM_REFERENCED;
                parm->sym.sym_type = GetType( front, TYP_DOUBLE );
                break;

            default:
                break;
            }
        }
        sym_handle = SymAdd( front, hash, &parm->sym );
        if( new_sym_handle != SYM_NULL ) {
            TREEPTR         tree;

            tree = ExprNode( front, VarLeaf( front, &new_sym, new_sym_handle ),
                 OPR_EQUALS, RValue( front, VarLeaf( front, &parm->sym, sym_handle ) ) );
            tree->op.u2.result_type = typ;
            tree->u.expr_type = typ;
            AddStmt( front, tree );
        }

        if( prev_parm == NULL ) {
            front.cvars.CurFunc->u.func.parms = sym_handle;
        } else {
            prev_parm->sym.handle = sym_handle;
            SymReplace( front, &prev_parm->sym, prev_sym_handle );
            CMemFree( front, prev_parm );
        }
        prev_parm = parm;
        prev_sym_handle = sym_handle;
        ++parm_count;
        parmlist = NewParm( front, parm->sym.sym_type, parmlist );
    }
    if( prev_parm != NULL ) {
        prev_parm->sym.handle = SYM_NULL;
        SymReplace( front, &prev_parm->sym, prev_sym_handle );
        CMemFree( front, prev_parm );
    }
    typ = front.cvars.CurFunc->sym_type;
    /*
     * TODO not following my scheme
     */
    front.cvars.CurFunc->sym_type = FuncNode( front, typ->object, FLAG_NONE,
        MakeParmList( front, parmlist, ParmsToBeReversed( front.cvars.CurFunc->mods, NULL ) ) );

    if( front.cvars.PrevProtoType != NULL ) {
        CheckProtoType(front);
    }
}


static void CheckParms( FrontRun& front )
{
    PARMPTR             parm;
    PARMPTR             prev_parm;
    SYM_HANDLE          sym_handle;
    SYM_HANDLE          prev_sym_handle;
    TYPEPTR             typ;

    front.cvars.CurFunc->u.func.locals = SYM_NULL;
    front.cvars.CurFunc->u.func.parms = SYM_NULL;
    typ = *(front.cvars.CurFunc->sym_type->u.fn.parms);
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type != TYP_VOID ) {
        prev_sym_handle = SYM_NULL;
        prev_parm = NULL;
        for( parm = front.cvars.ParmList; parm != NULL; parm = parm->next_parm ) {
            if( parm->sym.name == NULL ) {
                parm->sym.name = const_cast<char *>(".J");
                parm->sym.flags |= SYM_REFERENCED;
            }
            if( parm->sym.name[0] == '\0' ) {
                parm->sym.name = const_cast<char *>(".I");
                InvDecl(front);
            }
            if( parm->sym.sym_type == NULL ) {
                parm->sym.sym_type = TypeDefault(front);
            }
            /*
             * make sure name not already defined in this SymLevel
             */
            sym_handle = SymAdd( front, parm->sym.u1.hash, &parm->sym );
            if( prev_parm == NULL ) {
                front.cvars.CurFunc->u.func.parms = sym_handle;
            } else {
                prev_parm->sym.handle = sym_handle;
                SymReplace( front, &prev_parm->sym, prev_sym_handle );
                CMemFree( front, prev_parm );
            }
            prev_parm = parm;
            prev_sym_handle = sym_handle;
            parm->sym.flags |= SYM_DEFINED | SYM_ASSIGNED;
            parm->sym.attribs.is_parm = true;
        }
        if( prev_parm != NULL ) {
#if _CPU == 370
            {
                SYM_ENTRY   var_parm;

                if( VarParm( CurFunc ) ) {
                    typ = ArrayNode( GetType( TYP_CHAR ) );
                    typ->u.array->dimension = 160;
                    sym_handle = GetNewDotSym( &var_parm, 'V', typ, SC_AUTO );
                    SymReplace( &var_parm, sym_handle );
                    prev_parm->sym.handle = sym_handle;
                }
            }
#endif
            SymReplace( front, &prev_parm->sym, prev_sym_handle );
            CMemFree( front, prev_parm );
        }
    }
}
