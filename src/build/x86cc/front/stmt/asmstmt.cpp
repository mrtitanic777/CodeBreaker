/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Inline assembler statement processing.
*
****************************************************************************/


#include "cvars.h"
#include "asmstmt.h"
#include "frontrun.hpp"


#define MAX_ASM_LINE_LEN 511

static bool EndOfAsmStmt( FrontRun& front )
/******************************/
{
    if( front.cvars.CurToken == T_SEMI_COLON ) {
        /*
         * ; .ASM comment
         */
        for( ;; ) {
            NextToken(front);
            if( ( front.cvars.CurToken == T_EOF ) || ( front.cvars.CurToken == T_NULL ) ) {
                break;
            }
        }
        return( true );
    }
    if( front.cvars.CurToken == T_EOF )
        return( true );
    if( front.cvars.CurToken == T_NULL )
        return( true );
    if( front.cvars.CurToken == T___ASM )
        return( true );
    if( front.cvars.CurToken == T_RIGHT_BRACE )
        return( true );
    return( false );
}

static void GetAsmLine( FrontRun& front )
/****************************/
{
    unsigned    AsmErrLine;
    TOKEN       LastToken;
    char        buf[MAX_ASM_LINE_LEN + 1];

    PPCTL_ENABLE_EOL();
    AsmErrLine = front.cvars.TokenLoc.line;
    *buf = '\0';
    if( strcmp( front.cvars.Buffer, "_emit" ) == 0 ) {
        strcpy( buf, AsmSysDefineByte() );
        NextToken(front);
    }
    LastToken = T_DOT;
    for( ;; ) {
        if( EndOfAsmStmt(front) )
            break;
        if(( LastToken != T_DOT ) && ( LastToken != T_BAD_CHAR ) && ( front.cvars.CurToken != T_XOR ))
            strncat( buf, " ", MAX_ASM_LINE_LEN );
        strncat( buf, front.cvars.Buffer, MAX_ASM_LINE_LEN );
        LastToken = front.cvars.CurToken;
        NextToken(front);
    }
    buf[MAX_ASM_LINE_LEN] = '\0';
    if( *buf != '\0' ) {
        front.cvars.TokenLoc.line = AsmErrLine;
        AsmSysLine( front, buf );
    }
    PPCTL_DISABLE_EOL();
}

void AsmStmt( FrontRun& front )
/******************/
{
    bool            too_many_bytes;
    unsigned char   buff[MAXIMUM_BYTESEQ + ASM_BLOCK];
    TOKEN           skip_token;
    ppctl_t         old_ppctl;
    TREEPTR         tree;

    old_ppctl = front.cvars.PPControl;
    /*
     * indicate that we are inside an __asm statement so scanner will
     * allow tokens unique to the assembler. e.g. 21h
     */
    PPCTL_ENABLE_ASM();

    NextToken(front);
    AsmSysInit( front, buff );
    too_many_bytes = false;
    if( front.cvars.CurToken == T_LEFT_BRACE ) {
        NextToken(front);
        for( ;; ) {             // grab assembler lines
            GetAsmLine(front);
            if( front.asminlin->AsmCodeAddress > MAXIMUM_BYTESEQ ) {
                if( !too_many_bytes ) {
                    CErr1( front, ERR_TOO_MANY_BYTES_IN_PRAGMA );
                    too_many_bytes = true;
                }
                /*
                 * reset index to we don't overrun buffer
                 */
                front.asminlin->AsmCodeAddress = 0;
            }
            if( front.cvars.CurToken == T_RIGHT_BRACE )
                break;
            if( front.cvars.CurToken == T_EOF )
                break;
            NextToken(front);
        }
        skip_token = T_RIGHT_BRACE;
    } else {
        GetAsmLine(front);           // grab single assembler instruction
        skip_token = T_NULL;
    }
    front.cvars.PPControl = old_ppctl;
    if( front.asminlin->AsmCodeAddress > 0 ) {
        CreateAuxInlineFunc( front, too_many_bytes );
        front.pragdefn.CurrEntry = NULL;
        tree = LeafNode( front, OPR_FUNCNAME );
        tree->op.u2.sym_handle = MakeFunction( front, front.pragdefn.AuxList->name, FuncNode( front, GetType( front, TYP_VOID ), FLAG_NONE, NULL ) );
        tree = ExprNode( front, tree, OPR_CALL, NULL );
        tree->u.expr_type = GetType( front, TYP_VOID );
        AddStmt( front, tree );
    } else {
//        expr = NULL;
    }
    AsmSysFini(front);
    if( front.cvars.CurToken == skip_token ) {
        NextToken(front);
    }
}
