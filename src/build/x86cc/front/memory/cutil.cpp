/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Miscellaneous C front end utilities.
*
****************************************************************************/


#include "cvars.h"
#include "frontrun.hpp"


void MustRecog( FrontRun& front, TOKEN this_token )
{
    if( ExpectingToken( front, this_token ) ) {
        NextToken(front);
    } else {
/*      if( CurToken != T_LEFT_BRACE && CurToken != T_RIGHT_BRACE */
        if( front.cvars.CurToken != T_EOF ) {
            if( front.cvars.CurToken == T_SEMI_COLON ) {
                switch( this_token ) {
                case T_RIGHT_BRACKET:
                case T_RIGHT_PAREN:
                    break;
                default:
                    NextToken(front);
                    break;
                }
            } else if( this_token != T_SEMI_COLON ) {
                NextToken(front);
            }
        }
    }
}

static const char *NameOfCurToken( FrontRun& front )
{
    const char  *token;

    switch( front.cvars.CurToken ) {
    case T_BAD_CHAR:
        front.cvars.Buffer[1] = '\0';
        /* fall through */
    case T_ID:
    case T_STRING:
    case T_CONSTANT:
    case T_BAD_TOKEN:
        token = front.cvars.Buffer;
        break;
    default:
        token = TokenString[front.cvars.CurToken];
    }
    return( token );
}

void Expecting( FrontRun& front, const char *a_token )
{
    CErr3p( front, ERR_EXPECTING_BUT_FOUND, a_token, NameOfCurToken(front) );
}

bool ExpectingToken( FrontRun& front, TOKEN token )
{
    if( front.cvars.CurToken == token ) {
        return( true );
    }
    CErr3p( front, ERR_EXPECTING_BUT_FOUND, TokenString[token], NameOfCurToken(front) );
    return( false );
}

void ExpectingAfter( FrontRun& front, TOKEN token, TOKEN after_token )
{
    CErr4p( front, ERR_EXPECTING_AFTER_BUT_FOUND, TokenString[token], TokenString[after_token], NameOfCurToken(front) );
}


void ExpectEndOfLine( FrontRun& front )
{
    CErr2p( front, ERR_EXPECTING_END_OF_LINE_BUT_FOUND, NameOfCurToken(front) );
}

void ExpectIdentifier( FrontRun& front )
{
    CErr2p( front, ERR_EXPECTING_IDENTIFIER_BUT_FOUND, NameOfCurToken(front) );
}

bool ExpectingConstant( FrontRun& front )
{
    if( front.cvars.CurToken == T_CONSTANT ) {
        return( true );
    }
    CErr2p( front, ERR_EXPECTING_CONSTANT_BUT_FOUND, NameOfCurToken(front) );
    return( false );
}

void ExpectString( FrontRun& front )
{
    CErr2p( front, ERR_EXPECTING_STRING_BUT_FOUND, NameOfCurToken(front) );
}

void ExpectStructUnionTag( FrontRun& front )
{
    CErr2p( front, ERR_EXPECTING_STRUCT_UNION_TAG_BUT_FOUND, NameOfCurToken(front) );
}


SYM_NAMEPTR SymName( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    SYM_HASHPTR hsym;

    if( sym_handle == front.cvars.CharSymHandle )
        return( "char" );
    if( sym->name != NULL )
        return( sym->name );
    hsym = front.cvars.HashTab[sym->u1.hash];
    while( hsym->handle != sym_handle )
        hsym = hsym->next_sym;
    return( hsym->name );
}


void CErrSymName( FrontRun& front, int err, SYMPTR sym, SYM_HANDLE sym_handle )
{
    CErr2p( front, static_cast<msg_codes>(err), SymName( front, sym, sym_handle ) );
}
