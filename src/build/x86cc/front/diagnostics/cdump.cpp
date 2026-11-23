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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "cvars.h"
#include "frontrun.hpp"


#define STRCHUNK_INCREMENT 512



static void DoDumpType( FrontRun& front, TYPEPTR realtype, SYMPTR sym, STRCHUNK *pch );

/*
 * matches table of type in ctypes.h
 */
static const char   *const CTypeNames[] = {
    #define pick1(type,dtype,cgtype,x86asmtype,name,size) name,
    #include "cdatatyp.h"
    #undef  pick1
};

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CdumpState: cdump_state.h) */

}  // namespace



static void ChunkInit( STRCHUNK *pch )
{
    pch->data = NULL;
    pch->cursize = 0;
    pch->maxsize = 0;
}

static char *ChunkToStr( FrontRun& front, STRCHUNK *pch )
{
    char    *ret;

    /*
     * remove spaces from end of string
     */
    while( pch->cursize > 0 && pch->data[pch->cursize - 1] == ' ' ) {
        pch->data[--pch->cursize] = '\0';
    }
    ret = pch->data;
    if( ret == NULL ) {
        ret = CMemStrdup( front, "" );
    }
    return( ret );
}

static void ChunkSaveData( FrontRun& front, STRCHUNK *pch, const char *data, size_t len )
{
    size_t  requestbytes;
    char    *newdata;

    if( ( pch->cursize + len + 1 ) >= pch->maxsize ) {
        requestbytes = pch->cursize + len + 1 - pch->maxsize;
        if( requestbytes < STRCHUNK_INCREMENT ) {
            requestbytes = STRCHUNK_INCREMENT;
        }
        requestbytes += pch->maxsize;
        newdata = static_cast<char *>(CMemAlloc( front, requestbytes ));
        if( pch->data != NULL ) {
            memcpy( newdata, pch->data, pch->cursize );
            CMemFree( front, pch->data );
        }
        pch->data = newdata;
        pch->maxsize = requestbytes;
    }
    memcpy( pch->data+pch->cursize, data, len );
    pch->cursize += len;
    pch->data[pch->cursize] = '\0';
}

static void ChunkSaveStr( FrontRun& front, STRCHUNK *pch, const char *str )
{
    ChunkSaveData( front, pch, str, strlen( str ) );
}

static void ChunkSaveChar( FrontRun& front, STRCHUNK *pch, char ch )
{
    ChunkSaveData( front, pch, &ch, 1 );
}

static void ChunkSaveStrWord( FrontRun& front, STRCHUNK *pch, const char *str )
{
    ChunkSaveStr( front, pch, str );
    ChunkSaveChar( front, pch, ' ' );
}



static TYPEPTR TrueType( FrontRun& front, TYPEPTR typ )
{
    TYPEPTR     newtyp;

    if ( typ == NULL )
        return typ;

    if( front.cdump.do_message_output ) {
        /*
         * For human: smart typedef expansion. Stop before unnamed struct
         */
        while( typ->decl_type == TYP_TYPEDEF ) {
            newtyp = typ->object;
            if( newtyp->decl_type == TYP_STRUCT || newtyp->decl_type == TYP_UNION
              || newtyp->decl_type == TYP_ENUM ) {
                if( *newtyp->u.tag->name == '\0' ) {
                    break;
                }
            }
            typ = newtyp;
        }
    } else {
        if( !front.cvars.CompFlags.dump_prototypes ) {
            /*
             * -zg, not -v
             */
            SkipTypedefs( typ );
        }
    }
    return( typ );
}

static TYPEPTR Object( FrontRun& front, TYPEPTR typ )
{
    return( TrueType( front, typ->object ) );
}


static void put_keyword( FrontRun& front, int keyword, STRCHUNK *pch )
{
    ChunkSaveStrWord( front, pch, TokenString[keyword] );
}


static void DumpFlags( FrontRun& front, type_modifiers flags, TYPEPTR typ, STRCHUNK *fp )
{
    SYM_NAMEPTR     p;
    SYM_ENTRY       sym;

    if( flags & FLAG_VOLATILE )
        put_keyword( front, T_VOLATILE, fp );
    if( flags & FLAG_CONST )
        put_keyword( front, T_CONST, fp );
    if( flags & FLAG_UNALIGNED )
        put_keyword( front, T___UNALIGNED, fp );
    if( flags & FLAG_RESTRICT )
        put_keyword( front, T_RESTRICT, fp );
    if( flags & FLAG_LOADDS )
        put_keyword( front, T___LOADDS, fp );
    if( flags & FLAG_EXPORT )
        put_keyword( front, T___EXPORT, fp );
    if( flags & FLAG_SAVEREGS )
        put_keyword( front, T___SAVEREGS, fp );
    if( ( flags & FLAG_INTERRUPT ) == FLAG_INTERRUPT ) {
        put_keyword( front, T___INTERRUPT, fp );
    } else if( flags & FLAG_NEAR ) {
        if( flags & FLAG_BASED ) {
            ChunkSaveStr( front, fp, "__based(" );
            if( typ->u.p.based_sym == SYM_NULL ) {
                ChunkSaveStr( front, fp, "void" );
            } else {
                SymGet( front, &sym, typ->u.p.based_sym );
                p = SymName( front, &sym, typ->u.p.based_sym );
                ChunkSaveStr( front, fp, p );
            }
            ChunkSaveStr( front, fp, ") " );
        } else {
            put_keyword( front, T___NEAR, fp );
        }
    } else if( flags & FLAG_FAR ) {
        put_keyword( front, T___FAR, fp );
    } else if( flags & FLAG_HUGE ) {
        put_keyword( front, T___HUGE, fp );
    } else if( flags & FLAG_FAR16 ) {
        put_keyword( front, T___FAR16, fp );
    }
    switch( flags & MASK_LANGUAGES ) {
    case FLAG_WATCALL:
        put_keyword( front, T___WATCALL, fp );
        break;
    case FLAG_CDECL:
        put_keyword( front, T___CDECL, fp );
        break;
    case FLAG_PASCAL:
        put_keyword( front, T___PASCAL, fp );
        break;
    case FLAG_FORTRAN:
        put_keyword( front, T___FORTRAN, fp );
        break;
    case FLAG_SYSCALL:
        put_keyword( front, T__SYSCALL, fp );
        break;
    case FLAG_STDCALL:
        put_keyword( front, T___STDCALL, fp );
        break;
    case FLAG_OPTLINK:
        put_keyword( front, T__OPTLINK, fp );
        break;
    case FLAG_FASTCALL:
        put_keyword( front, T___FASTCALL, fp );
        break;
    }
}


static void DumpArray( FrontRun& front, TYPEPTR typ, STRCHUNK *pch )
{
    target_size     size;
    char            tempbuf[20];

    while( typ->decl_type == TYP_ARRAY ) {
        size = typ->u.array->dimension;
        if( size != 0 ) {
            sprintf( tempbuf, "[%u]", (unsigned)size );
            ChunkSaveStr( front, pch, tempbuf );
        } else {
            ChunkSaveStr( front, pch, "[]" );
        }
        typ = Object( front, typ );
    }
}


static void DumpTagName( FrontRun& front, const char *tag_name, STRCHUNK *pch )
{
    if( *tag_name == '\0' ) {
        if( front.cdump.do_message_output ) {
            tag_name = "{...}";
        } else {
            tag_name = "_no_name_";
        }
    }
    ChunkSaveStrWord( front, pch, tag_name );
}


static TYPEPTR DefArgPromotion( FrontRun& front, TYPEPTR arg_typ )
{
    TYPEPTR     typ;

    /*
     * perform default argument promotions
     */
    typ = arg_typ;
    while( typ->decl_type == TYP_TYPEDEF ) typ = Object( front, typ );
    switch( typ->decl_type ) {
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_SHORT:
    case TYP_ENUM:
        arg_typ = GetType( front, TYP_INT );
        break;
    case TYP_USHORT:
        arg_typ = GetType( front, TYP_UINT );
        break;
    case TYP_FLOAT:
        arg_typ = GetType( front, TYP_DOUBLE );
        break;
    default:
        break;
    }
    return( arg_typ );
}

static void DumpBaseType( FrontRun& front, TYPEPTR typ, STRCHUNK *pch )
{
    SYM_ENTRY           sym;
    TYPEPTR             obj;
    char                tempbuf[20];

    for( ; typ->decl_type != TYP_TYPEDEF && typ->decl_type != TYP_ENUM; ) {
        obj = Object( front, typ );
        if( obj == NULL )
            break;
        typ = obj;
    }
    SkipDummyTypedefs( typ );
    if( typ->decl_type == TYP_TYPEDEF ) {
        SymGet( front, &sym, typ->u.typedefn );
        ChunkSaveStrWord( front, pch, SymName( front, &sym, typ->u.typedefn ) );
    } else {
        if( typ->type_flags & TF2_TYP_PLAIN_CHAR ) {
            ChunkSaveStrWord( front, pch, "char" );
        } else if( typ->decl_type == TYP_FIELD || typ->decl_type == TYP_UFIELD ) {
            ChunkSaveStr( front, pch, CTypeNames[typ->u.f.field_type] );
            sprintf( tempbuf, ":%u", (unsigned)typ->u.f.field_width );
            ChunkSaveStr( front, pch, tempbuf );
        } else {
            ChunkSaveStrWord( front, pch, CTypeNames[typ->decl_type] );
        }
        if( typ->decl_type == TYP_STRUCT || typ->decl_type == TYP_UNION
          || typ->decl_type == TYP_ENUM ) {
            /*
             * if there is no tag name, then should print out the
             * entire structure or union definition or enum list
             */
            DumpTagName( front, typ->u.tag->name, pch );
        }
    }
}

static void DumpParmList( FrontRun& front, TYPEPTR *parm_types, SYMPTR funcsym, STRCHUNK *pch )
{
    TYPEPTR         typ;
    int             parm_num;
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;
    SYMPTR          parm_sym;
    char            temp_name[20];

    if( parm_types == NULL ) {
        ChunkSaveStr( front, pch, "void" );
    } else {
        parm_num = 1;
        if( funcsym != NULL ) {
            sym_handle = funcsym->u.func.parms;
        } else {
            sym_handle = SYM_NULL;
        }
        for( ; (typ = *parm_types) != NULL; ++parm_types ) {
            typ = TrueType( front, typ );
            if( funcsym != NULL ) {
                if( funcsym->flags & SYM_OLD_STYLE_FUNC ) {
                    typ = DefArgPromotion( front, typ );
                }
            }
            parm_sym = &sym;
            if( sym_handle != SYM_NULL ) {
                SymGet( front, parm_sym, sym_handle );
            } else if( typ->decl_type == TYP_VOID || typ->decl_type == TYP_DOT_DOT_DOT ) {
                parm_sym = NULL;
            } else {
                sym.handle = SYM_INVALID;
                sym.name = temp_name;
                sprintf( temp_name, "__p%d", parm_num );
            }
            DoDumpType( front, typ, parm_sym, pch );
            if( *(parm_types + 1) != NULL ) {
                ChunkSaveChar( front, pch, ',' );
            }
            ++parm_num;
        }
    }
}


static void DumpTail( FrontRun& front, TYPEPTR typ, SYMPTR funcsym, type_modifiers pointer_flags, STRCHUNK *pch )
{
    TYPEPTR     top_typ;
    TYPEPTR     obj;

    top_typ = typ;
    for( ;; ) {
        if( typ->decl_type == TYP_FUNCTION ) {
            ChunkSaveChar( front, pch, '(' );
            if( typ == top_typ || typ->u.fn.parms != NULL ) {
                DumpParmList( front, typ->u.fn.parms, funcsym, pch);
                funcsym = NULL;
            }
            ChunkSaveChar( front, pch, ')' );
        }
        typ = Object( front, typ );
        while( typ->decl_type == TYP_POINTER ) {
            typ = Object( front, typ );
        }
        if( typ->decl_type == TYP_ARRAY ) {
            ChunkSaveChar( front, pch, ')' );
            if( pointer_flags & FLAG_WAS_ARRAY ) {
                /*
                 * we don't know the dimension anymore. just put out [1]
                 */
                ChunkSaveStr( front, pch, "[1]" );
                pointer_flags = static_cast<type_modifiers>(0);
            }
            DumpArray( front, typ, pch) ;
            for( ;; ) {
                obj = Object( front, typ );
                if( obj->decl_type != TYP_ARRAY )
                    break;
                typ = obj;
            }
        } else {
            if( typ->decl_type != TYP_FUNCTION )
                break;
            ChunkSaveChar( front, pch, ')' );
        }
    }
}

static void DumpPointer( FrontRun& front, TYPEPTR typ, STRCHUNK *pch )
{
    if( typ->decl_type == TYP_POINTER ) {
        DumpPointer( front, Object( front, typ ), pch );
        if( ( typ->u.p.decl_flags & FLAG_WAS_ARRAY) == 0 ) {
            DumpFlags( front, typ->u.p.decl_flags & ~MASK_QUALIFIERS, typ, pch );
            DumpFlags( front, typ->u.p.decl_flags & MASK_QUALIFIERS, typ, pch );
            ChunkSaveChar( front, pch, '*' );
        }
    }
}

static void DumpDecl( FrontRun& front, TYPEPTR typ, SYMPTR funcsym, STRCHUNK *pch )
{
    TYPEPTR         obj;

    switch( typ->decl_type ) {
    case TYP_FUNCTION:
        DumpDecl( front, Object( front, typ ), NULL, pch );
        if ( funcsym ) {
            DumpFlags( front, funcsym->mods & (FLAG_LOADDS | FLAG_EXPORT | FLAG_SAVEREGS), typ, pch );
            DumpFlags( front, funcsym->mods & ~(FLAG_LOADDS | FLAG_EXPORT | FLAG_SAVEREGS), typ, pch );
            ChunkSaveStr( front, pch, funcsym->name );
        }
        /* fall through */
    case TYP_ARRAY:
        DumpTail( front, typ, funcsym, FLAG_NONE, pch );
        break;
    case TYP_POINTER:
        obj = Object( front, typ );
        while( obj->decl_type == TYP_POINTER )
            obj = Object( front, obj );
        switch( obj->decl_type ) {
        case TYP_FUNCTION:
            DumpDecl( front, Object( front, obj ), NULL, pch );
            ChunkSaveChar( front, pch, '(' );
            break;
        case TYP_ARRAY:
            while( obj->decl_type == TYP_ARRAY )
                obj = Object( front, obj );
            DumpDecl( front, obj, NULL, pch );
            ChunkSaveChar( front, pch, '(' );
            break;
        default:
            break;
        }
        DumpPointer( front, typ, pch );
        break;
    default:
        break;
    }
}

static void DumpSymbol( FrontRun& front, TYPEPTR typ, SYMPTR sym, STRCHUNK *pch )
{
    bool        was_array;

    was_array = ( typ->decl_type == TYP_POINTER && (typ->u.p.decl_flags & FLAG_WAS_ARRAY) != 0 );
    if( was_array ) {
        DumpFlags( front, typ->u.p.decl_flags & ~MASK_QUALIFIERS, typ, pch );
        DumpFlags( front, typ->u.p.decl_flags & MASK_QUALIFIERS, typ, pch );
    }
    if( sym )
        ChunkSaveStr( front, pch, sym->name );
    if( was_array ) {
        typ = Object( front, typ );
        if( typ->decl_type != TYP_ARRAY ) {
            ChunkSaveStr( front, pch, "[]" );
        }
    }
}

static void DoDumpType( FrontRun& front, TYPEPTR realtype, SYMPTR sym, STRCHUNK *pch )
{
    type_modifiers  pointer_flags;
    TYPEPTR         typ;

    realtype = TrueType( front, realtype );
    DumpBaseType( front, realtype, pch );
    DumpDecl( front, realtype, NULL, pch );
    DumpSymbol( front, realtype, sym, pch );
    for( typ = realtype; typ != NULL; typ = Object( front, typ ) ) {
        if( typ->decl_type == TYP_TYPEDEF )
            break;
        pointer_flags = static_cast<type_modifiers>(0);
        while( typ->decl_type == TYP_POINTER ) {
            pointer_flags = typ->u.p.decl_flags;
            typ = Object( front, typ );
        }
        if( typ->decl_type == TYP_ARRAY || typ->decl_type == TYP_FUNCTION ) {
            DumpTail( front, realtype, NULL, pointer_flags, pch );
            break;
        }
    }
}


static void DumpParmTags( FrontRun& front, TYPEPTR *parm, FILE *fp )
{
    TYPEPTR     typ;
    STRCHUNK    chunk;
    char        *result;

    if( parm != NULL ) {
        for( ; (typ = *parm) != NULL; ) {
            typ = TrueType( front, typ );
            if( typ->decl_type == TYP_STRUCT || typ->decl_type == TYP_UNION ) {
                ChunkInit(&chunk);
                ChunkSaveStrWord( front, &chunk, CTypeNames[typ->decl_type] );
                DumpTagName( front, typ->u.tag->name, &chunk );
                result = ChunkToStr( front, &chunk );
                fprintf( fp, "%s;\n", result );
                CMemFree( front, result );
            }
            ++parm;
        }
    }
}


void DumpFuncDefn( FrontRun& front )
{
    TYPEPTR     typ;
    FNAMEPTR    flist;
    STRCHUNK    chunk;
    char        *result;

    typ = front.cvars.CurFunc->sym_type;
    DumpParmTags( front, typ->u.fn.parms, front.cvars.DefFile );
    flist = FileIndexToFName( front, front.cvars.CurFunc->src_loc.fno );
    fprintf( front.cvars.DefFile, "//#line \"%s\" %u\n",
                flist->name,
                front.cvars.CurFunc->src_loc.line );

    ChunkInit( &chunk );

    if( front.cvars.CurFunc->attribs.stg_class == SC_STATIC ) {
        put_keyword( front, T_STATIC, &chunk );
    } else {
        put_keyword( front, T_EXTERN, &chunk );
    }
    if( front.cvars.CurFunc->flags & SYM_TYPE_GIVEN ) {
        DumpBaseType( front, Object( front, typ ), &chunk );
    }
    DumpDecl( front, typ, front.cvars.CurFunc, &chunk );

    result = ChunkToStr( front, &chunk );
    fprintf( front.cvars.DefFile, "%s;\n", result );
    CMemFree( front, result );
}

char *DiagGetTypeName( FrontRun& front, TYPEPTR typ )
{
    STRCHUNK    chunk;

    ChunkInit( &chunk );
    front.cdump.do_message_output = true;
    DoDumpType( front, typ, NULL, &chunk );
    front.cdump.do_message_output = false;
    return( ChunkToStr( front, &chunk ) );
}
