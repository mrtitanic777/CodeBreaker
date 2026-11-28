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
* Description:  Convert C statements to expression trees.
*
****************************************************************************/


#include "../../l2body.hpp"

#include "cvars.h"
#include "i64.h"
#include "cfeinfo.h"
#include "toggles.h"
#include "frontrun.hpp"




/*
 * values for return_type
 */





namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CstmtState: cstmt_state.h) */

}  // namespace



void StmtInit( FrontRun& front )
{
    front.cvars.FirstStmt = NULL;
    front.cstmt.LastStmt = NULL;
    front.cvars.SymListHeads = NULL;
}


static void CheckStringLeaf( FrontRun& front, TREEPTR leaf )
{
    if( leaf->op.opr == OPR_PUSHSTRING ) {
        leaf->op.u2.string_handle->ref_count++;
    }
}


static void CheckUseful( FrontRun& front )
{
    if( front.cvars.CompFlags.useful_side_effect ) {
        CWarn1( front, ERR_USEFUL_SIDE_EFFECT );
    } else {
        CWarn1( front, ERR_MEANINGLESS );
    }
}


static void CheckStmtExpr( FrontRun& front )
{
    TREEPTR     tree;

    tree = Expr(front);
    if( front.cvars.CompFlags.meaningless_stmt ) {
        CheckUseful(front);
    }
    AddStmt( front, tree );
    if( front.cvars.CompFlags.pending_dead_code ) {
        front.cvars.DeadCode = 1;
    }
}


SYM_HANDLE GetBlockSymList( FrontRun& front )
{
    return( front.cstmt.BlockStack->sym_list );
}


void InitStmt( FrontRun& front )
{
    front.cstmt.BlockStack = NULL;
    front.cstmt.LoopStack = NULL;
    front.cstmt.SwitchStack = NULL;
}


void SwitchPurge( FrontRun& front )
{
    SWITCHPTR   sw;
    CASEPTR     c_entry;
    CASEPTR     c_tmp;

    while( (sw = front.cstmt.SwitchStack) != NULL ) {
        front.cstmt.SwitchStack = sw->prev_switch;
        c_entry = sw->case_list;
        while( c_entry != NULL ) {
            c_tmp = c_entry->next_case;
            CMemFree( front, c_entry );
            c_entry = c_tmp;
        }
        CMemFree( front, sw );
    }
}


void AddStmt( FrontRun& front, TREEPTR stmt )
{
    WalkExprTree( front, stmt, CheckStringLeaf, NoOp, NoOp, DoConstFold );
    stmt = ExprNode( front, NULL, OPR_STMT, stmt );
    stmt->op.u2.src_loc = front.cvars.SrcLoc;
    stmt->op.u1.unroll_count = front.cvars.UnrollCount;
    if( front.cvars.FirstStmt == NULL )
        front.cvars.FirstStmt = stmt;
    if( front.cstmt.LastStmt != NULL ) {
        front.cstmt.LastStmt->left = stmt;
    }
    front.cstmt.LastStmt = stmt;
}


static void AddSymList( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYM_LIST_HEADS   *symlisthead;

    symlisthead = static_cast<SYM_LIST_HEADS *>(CMemAlloc( front, sizeof( SYM_LIST_HEADS ) ));
    symlisthead->next = front.cvars.SymListHeads;
    symlisthead->sym_head = sym_handle;
    front.cvars.SymListHeads = symlisthead;
}


void GenFunctionNode( FrontRun& front, SYM_HANDLE sym_handle )
{
    TREEPTR     tree;
    SYMPTR      sym;

    tree = LeafNode( front, OPR_FUNCTION );
    sym = SymGetPtr( front, sym_handle );
    tree->op.u2.func.sym_handle = sym_handle;
    tree->op.u2.func.flags = FUNC_NONE;
    if( Toggles( front ).tog_inline
      || (sym->mods & FLAG_INLINE) ) {
        if( !sym->attribs.naked ) {
            if( strcmp( sym->name, "main" ) != 0 ) {
                tree->op.u2.func.flags |= FUNC_OK_TO_INLINE;
            }
        }
    }
    tree->op.flags = OpFlags( sym->mods );
    tree->u.expr_type = sym->sym_type->object;    // function return type
    AddStmt( front, tree );
    /*
     * Evil, evil globals! But we need this for later lookups in cgen.c
     */
    sym->u.func.start_of_func = front.cstmt.LastStmt;
    front.cvars.CurFuncNode = tree;
    front.cvars.NodeCount = 0;
}


/*
 * __declspec(noinline) fix the problem with VS2022 MS compiler global optimization
 * by inlining, it was searched by empirical testing that it fix our problem
 * but reason for this behaviour is unknown
 *
 * probably bug, incorrectly inlined code
 * -Ob1 option for this source file solve this issue (always)
 * experimentaly checked that this function if not inlined then fix the issue
 * hard to debug, I didn't found method to say debugger to work with
 * breakpoints in inlined function code
 * of cause it can be some hidden bug in OW code, but happen with VS2022 only
 */
__declspec(noinline)
LABEL_INDEX NextLabel( FrontRun& front )
{
    return( ++front.cvars.LabelIndex );
}


static void DropLabel( FrontRun& front, LABEL_INDEX label )
{
    TREEPTR     tree;

    front.cvars.CompFlags.label_dropped = true;
    front.cvars.DeadCode = 0;
    tree = LeafNode( front, OPR_LABEL );
    tree->op.u2.label_index = label;
    AddStmt( front, tree );
}


static void DropBreakLabel( FrontRun& front )
{
    if( front.cstmt.BlockStack->break_label != 0 ) {
        DropLabel( front, front.cstmt.BlockStack->break_label );
    }
}


static void DropContinueLabel( FrontRun& front )
{
    if( front.cstmt.BlockStack->continue_label != 0 ) {
        DropLabel( front, front.cstmt.BlockStack->continue_label );
    }
}


static TREEPTR BracketExpr( FrontRun& front )
{
    TREEPTR     tree;

    MustRecog( front, T_LEFT_PAREN );
    tree = Expr(front);
    FoldExprTree( front, tree );
    MustRecog( front, T_RIGHT_PAREN );
    return( tree );
}


static void JumpCond( FrontRun& front, TREEPTR expr,
                     LABEL_INDEX label,
                     opr_code jump_opcode,
                     opr_code jump_opposite )
{
    TREEPTR     tree;

    tree = BoolExpr( front, expr );
    if( tree->op.opr == OPR_NOT ) {
        tree->op.opr = jump_opposite;
    } else {
        tree = ExprNode( front, NULL, jump_opcode, tree );
    }
    tree->op.u2.label_index = label;
    AddStmt( front, tree );
}


static void Jump( FrontRun& front, LABEL_INDEX label )
{
    TREEPTR     tree;

    if( front.cvars.DeadCode == 0 ) {
        tree = LeafNode( front, OPR_JUMP );
        tree->op.u2.label_index = label;
        AddStmt( front, tree );
        front.cvars.DeadCode = 1;
    }
}


static bool JumpFalse( FrontRun& front, TREEPTR expr, LABEL_INDEX label )
{
    bool        jump_generated;

    jump_generated = false;
    if( expr->op.opr == OPR_PUSHINT ) {
        if( CheckZeroConstant( front, expr ) ) {
            Jump( front, label );
            jump_generated = true;
        }
        FreeExprNode( front, expr );
    } else {
        JumpCond( front, expr, label, OPR_JUMPFALSE, OPR_JUMPTRUE );
        jump_generated = true;
    }
    return( jump_generated );
}


static void JumpTrue( FrontRun& front, TREEPTR expr, LABEL_INDEX label )
{
    if( expr->op.opr == OPR_PUSHINT ) {
        if( !CheckZeroConstant( front, expr ) ) {
            Jump( front, label );
        }
        FreeExprNode( front, expr );
    } else {
        JumpCond( front, expr, label, OPR_JUMPTRUE, OPR_JUMPFALSE );
    }
}


void LookAhead( FrontRun& front )
{
    front.cvars.SavedId = CMemStrdup( front, front.cvars.Buffer );     /* save current id */
    front.cvars.SavedTokenLoc = front.cvars.TokenLoc;           /* save linenum and fno */
    NextToken(front);                        /* get next token */
    front.cvars.LAToken = front.cvars.CurToken;                 /* save it in look ahead */
    front.cvars.CurToken = T_SAVED_ID;              /* go back to saved id */
}


static bool GrabLabels( FrontRun& front )
{
    LABELPTR    label;

    label = NULL;
    for( ;; ) {
        if( front.cvars.CurToken == T_SAVED_ID ) {
            front.cvars.SrcLoc = front.cvars.SavedTokenLoc;
        } else {
            front.cvars.SrcLoc = front.cvars.TokenLoc;
            if( front.cvars.CurToken != T_ID )
                break;
            LookAhead(front);
        }
        if( front.cvars.LAToken != T_COLON )
            break; /* quit if look ahead not : */
        label = LkLabel( front, front.cvars.SavedId );
        if( label->defined ) {
            CErr2p( front, ERR_LABEL_ALREADY_DEFINED, label->name );
        } else {
            DropLabel( front, label->ref_list );
            label->defined = true;      /* indicate label defined now */
        }
        CMemFree( front, front.cvars.SavedId );            /* free the saved id */
        front.cvars.SavedId = NULL;
        NextToken(front);                    /* get next token */
    }
    if( label != NULL ) {               /* label was found */
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            CErr1( front, ERR_STMT_REQUIRED_AFTER_LABEL );
        }
        return( true );                    // indicate label found
    }
    return( false );
}


static void UnWindTry( FrontRun& front, tryindex_t try_scope )
{
#ifdef __SEH__
    TREEPTR     tree;

    tree = LeafNode( front, OPR_UNWIND );
    tree->op.u2.st.u.try_index = try_scope;
    AddStmt( front, tree );
#else
    /* unused parameters */ (void)try_scope;
#endif
}


static void ReturnStmt( FrontRun& front, SYM_HANDLE func_result_handle, struct return_info *info )
{
    TREEPTR             tree;
    BLOCKPTR            block;
    enum return_with    with;

    NextToken(front);
    if( front.cvars.CurToken != T_SEMI_COLON ) {
        TYPEPTR     func_type;

        func_type = front.cvars.CurFunc->sym_type->object;
        SkipTypedefs( func_type );
        tree = RValue( front, Expr(front) );
        CheckRetType( front, tree );
        tree = FixupAss( front, tree, func_type );
        tree = ExprNode( front, NULL, OPR_RETURN, tree );
        tree->u.expr_type = func_type;
        tree->op.u2.sym_handle = func_result_handle;
        AddStmt( front, tree );
        with = RETURN_WITH_EXPR;
        info->with_expr = true;
    } else {
        with = RETURN_WITH_NO_EXPR;
    }
    if( info->with == RETURN_WITH_NONE ) {
        info->with = with;
    }
    if( info->with != with ) {
        CErr1( front, ERR_INCONSISTENT_USE_OF_RETURN );
    }
    for( block = front.cstmt.BlockStack; block != NULL; block = block->prev_block ) {
        if( (block->block_type == T__TRY)
          || (block->block_type == T___TRY) ) {
            UnWindTry( front, TRYSCOPE_NONE );
            break;
        }
    }
}


static void SetFuncReturnNode( FrontRun& front, TREEPTR tree )
{
    TYPEPTR     typ;

    typ = front.cvars.CurFunc->sym_type->object;
    tree->u.expr_type = typ;
    SkipTypedefs( typ );
    if( typ->decl_type == TYP_STRUCT
      || typ->decl_type == TYP_UNION ) {
        tree->right = LeafNode( front, OPR_NOP );      // place holder
        tree->right->u.expr_type = NULL;
    }
}


static void CheckRetValue( FrontRun& front )
{
    TYPEPTR     typ;

    typ = front.cvars.CurFunc->sym_type;
    typ = typ->object;
    SkipTypedefs( typ );
    if( typ->decl_type != TYP_VOID ) {
        CWarn2p( front, ERR_MISSING_RETURN_VALUE, front.cvars.CurFunc->name );
    }
}

static SYM_HANDLE GetLocalVarDecls( FrontRun& front )
{
    SYM_HANDLE  symlist;
    SYM_HANDLE  symhandle;
    SYM_ENTRY   sym;

    DeclList( front, &symlist );
    if( symlist != SYM_NULL ) {
        symhandle = front.cvars.CurFunc->u.func.locals;
        /*
         * symhandle will be non-zero if MakeNewDotSym was called while
         * parsing the declaration list.
         */
        if( symhandle != SYM_NULL ) {   // if some temporaries were created
            for( ;; ) {                 // - find end of list
                SymGet( front, &sym, symhandle );
                if( sym.handle == SYM_NULL )
                    break;
                symhandle = sym.handle;
            }
            sym.handle = symlist;
            SymReplace( front, &sym, symhandle );
        } else {
            front.cvars.CurFunc->u.func.locals = symlist;
        }
    }
    return( symlist );
}


static void DeadMsg( FrontRun& front )
/*************************/
{
    CWarn1( front, ERR_DEAD_CODE );
    front.cvars.DeadCode = 2;   /* so we don't get more messages */
}


static void StartNewBlock( FrontRun& front )
{
    BLOCKPTR    block;

    block = static_cast<BLOCKPTR>(CMemAlloc( front, sizeof( BLOCKDEFN ) ));
    block->block_type = front.cvars.CurToken;
    block->top_label = 0;
    block->continue_label = 0;
    block->break_label = 0;
    block->prev_block = front.cstmt.BlockStack;
    block->prev_loop = front.cstmt.LoopStack;
    block->gen_endblock = false;
    front.cstmt.BlockStack = block;
}


static void LeftBrace( FrontRun& front )
{
    TREEPTR     tree;

/*
    <storage> <type> function( <parms> )
    {   <- this is SymLevel == 1
    (weird code is for SymLevel > 1 )
*/
    /*
     * DeclList might generate some quads to do some initializers
     * if that happens, we want them output after the OPR_NEWBLOCK node
     */
    StartNewBlock(front);
    NextToken(front);
    ++front.cvars.SymLevel;
    tree = LeafNode( front, OPR_NEWBLOCK );
    AddStmt( front, tree );
    front.cstmt.BlockStack->sym_list = SYM_NULL;
    front.cstmt.BlockStack->gen_endblock = true;
    DeclList( front, &front.cstmt.BlockStack->sym_list );
    tree->op.u2.sym_handle = front.cstmt.BlockStack->sym_list;
}


static void JumpBreak( FrontRun& front, BLOCKPTR block )
{
    if( front.cvars.DeadCode == 0 ) {
        if( block->break_label == 0 ) {
            block->break_label = NextLabel(front);
        }
        Jump( front, block->break_label );
    }
}


static void BreakStmt( FrontRun& front )
{
    BLOCKPTR    block;
    tryindex_t  try_scope;

    NextToken(front);
    try_scope = TRYSCOPE_UNDEF;
    block = front.cstmt.BlockStack;
    if( block != NULL ) {
        while( block != front.cstmt.LoopStack ) {
            if( block->block_type == T_SWITCH )
                break;
            if( (block->block_type == T__TRY)
              || (block->block_type == T___TRY) ) {
                try_scope = block->parent_index;
            }
            block = block->prev_block;
        }
    }
    if( block != NULL ) {
        if( try_scope != TRYSCOPE_UNDEF ) {
            UnWindTry( front, try_scope );
        }
        JumpBreak( front, block );
    } else {
        CErr1( front, ERR_MISPLACED_BREAK );
    }
    MustRecog( front, T_SEMI_COLON );
}


#ifdef __SEH__
static void LeaveStmt( FrontRun& front )
{
    BLOCKPTR    block;

    NextToken(front);
    block = front.cstmt.BlockStack;
    while( block != NULL ) {
        if( (block->block_type == T__TRY)
          || (block->block_type == T___TRY) )
            break;
        block = block->prev_block;
    }
    if( block != NULL ) {
        JumpBreak( front, block );
    } else {
        CErr1( front, ERR_MISPLACED_LEAVE );
    }
    MustRecog( front, T_SEMI_COLON );
}
#endif


static void ContinueStmt( FrontRun& front )
{
    BLOCKPTR    block;
    tryindex_t  try_scope;

    NextToken(front);
    if( front.cstmt.LoopStack != NULL ) {
        if( front.cvars.DeadCode == 0 ) {
            try_scope = TRYSCOPE_UNDEF;
            block = front.cstmt.BlockStack;
            while( block != front.cstmt.LoopStack ) {
                if( ( block->block_type == T__TRY )
                  || ( block->block_type == T___TRY ) ) {
                    try_scope = block->parent_index;
                }
                block = block->prev_block;
            }
            if( try_scope != TRYSCOPE_UNDEF ) {
                UnWindTry( front, try_scope );
            }
            if( front.cstmt.LoopStack->continue_label == 0 ) {
                front.cstmt.LoopStack->continue_label = NextLabel(front);
            }
            Jump( front, front.cstmt.LoopStack->continue_label );
        }
    } else {
        CErr1( front, ERR_MISPLACED_CONTINUE );
    }
    MustRecog( front, T_SEMI_COLON );
}


static void DefaultStmt( FrontRun& front )
{
    NextToken(front);
    MustRecog( front, T_COLON );
    if( front.cstmt.SwitchStack ) {
        if( front.cstmt.SwitchStack->default_label == 0 ) {
            front.cstmt.SwitchStack->default_label = NextLabel(front);
            DropLabel( front, front.cstmt.SwitchStack->default_label );
        } else {
            CErr1( front, ERR_ONLY_1_DEFAULT );
        }
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            CErr1( front, ERR_STMT_REQUIRED_AFTER_DEFAULT );
        }
    } else {
        CErr1( front, ERR_MISPLACED_DEFAULT );
    }
}


static void ElseStmt( FrontRun& front )
{
    LABEL_INDEX if_label;

    front.cvars.SrcLoc = front.cvars.TokenLoc;
    NextToken(front);
    front.cstmt.BlockStack->block_type = T_ELSE;
    if_label = front.cstmt.BlockStack->break_label;
    if( front.cvars.DeadCode == 0 ) {
        front.cstmt.BlockStack->break_label = NextLabel(front);
        Jump( front, front.cstmt.BlockStack->break_label );
    } else {
        front.cstmt.BlockStack->break_label = 0;
    }
    DropLabel( front, if_label );
}


static void GotoStmt( FrontRun& front )
{
    LABELPTR    label;

    NextToken(front);
    if( front.cvars.CurToken != T_ID ) {
        CErr1( front, ERR_EXPECTING_LABEL );
    } else {
        label = LkLabel( front, front.cvars.Buffer );
        label->referenced = true;
        Jump( front, label->ref_list );
    }
    NextToken(front);
    MustRecog( front, T_SEMI_COLON );
}


static void PushBlock( FrontRun& front )
{
    StartNewBlock(front);
    ++front.cvars.SymLevel;
    front.cstmt.BlockStack->sym_list = SYM_NULL;
}


static void NewLoop( FrontRun& front )
{
    StartNewBlock(front);
    front.cstmt.LoopStack = front.cstmt.BlockStack;
    front.cstmt.LoopStack->top_label = NextLabel(front);
    DropLabel( front, front.cstmt.LoopStack->top_label );
    ++front.cvars.LoopDepth;
}


static void ForStmt( FrontRun& front )
{
    bool    parsed_semi_colon = false;

    NextToken(front);
    MustRecog( front, T_LEFT_PAREN );
    if( front.cvars.CompVars.cstd > STD_C89 ) {
        PushBlock(front);    // 'for' opens new scope
    }
    if( front.cvars.CurToken != T_SEMI_COLON ) {
        if( front.cvars.CompVars.cstd > STD_C89 ) {
            TREEPTR     tree;

            tree = LeafNode( front, OPR_NEWBLOCK );
            AddStmt( front, tree );
            front.cstmt.BlockStack->gen_endblock = true;
            if( !LoopDecl( front, &front.cstmt.BlockStack->sym_list ) ) {
                CheckStmtExpr(front);    // no declarator, try init_expr
            } else {
                parsed_semi_colon = true;   // LoopDecl ate it up
            }
            tree->op.u2.sym_handle = front.cstmt.BlockStack->sym_list;
        } else {
            CheckStmtExpr(front);        // init_expr
        }
    }
    if( !parsed_semi_colon ) {
        MustRecog( front, T_SEMI_COLON );
    }
    NewLoop(front);
    front.cstmt.BlockStack->block_type = T_FOR;
    if( front.cvars.CurToken != T_SEMI_COLON ) {
        front.cstmt.BlockStack->break_label = NextLabel(front);
        if( !JumpFalse( front, Expr(front), front.cstmt.BlockStack->break_label ) ) {
            front.cstmt.BlockStack->break_label = 0;
        }
    }
    MustRecog( front, T_SEMI_COLON );
    front.cstmt.BlockStack->inc_var = NULL;
    if( front.cvars.CurToken != T_RIGHT_PAREN ) {
        front.cstmt.BlockStack->inc_var = Expr(front);                   // save this
        if( front.cvars.CompFlags.meaningless_stmt ) {
            CheckUseful(front);
        }
    }
    MustRecog( front, T_RIGHT_PAREN );
}


static void EndForStmt( FrontRun& front )
{
    DropContinueLabel(front);
    if( front.cstmt.BlockStack->inc_var ) {
        AddStmt( front, front.cstmt.BlockStack->inc_var );
    }
    Jump( front, front.cstmt.BlockStack->top_label );
}


static void StmtExpr( FrontRun& front )
{
    CheckStmtExpr(front);
    if( ExpectingToken( front, T_SEMI_COLON ) ) {
        NextToken(front);
    }
}


static void AddCaseLabel( FrontRun& front, signed_64 value )
{
    CASEPTR         ce;
    CASEPTR         prev_ce;
    CASEPTR         new_ce;
    signed_64       old_value;
    signed_64       converted_value;
    TREEPTR         tree;
    char            buffer[12];

    prev_ce = NULL;
    converted_value = value;
    old_value = converted_value;
    U64Inc( old_value ); /* make old_value different */
    for( ce = front.cstmt.SwitchStack->case_list; ce != NULL; ce = ce->next_case ) {
        old_value = ce->value;
        if( I64Cmp( &old_value, &converted_value ) >= 0 )
            break;
        prev_ce = ce;
    }
    if( U64isEq( converted_value, old_value ) ) {
        /*
         * duplicate case value found
         */
        sprintf( buffer, "%lld", value.u._64[0] );
        CErr2p( front, ERR_DUPLICATE_CASE_VALUE, buffer );
    } else {
        new_ce = static_cast<CASEPTR>(CMemAlloc( front, sizeof( CASEDEFN ) ));
        new_ce->value = converted_value;
        if( prev_ce == NULL ) {
            new_ce->next_case = front.cstmt.SwitchStack->case_list;
            front.cstmt.SwitchStack->case_list = new_ce;
        } else {
            prev_ce->next_case = new_ce;
            new_ce->next_case = ce;
        }
        /* Check if the previous statement was a 'case'. If so, reuse the label, as generating
         * too many labels seriously slows down code generation.
         */
        if( prev_ce
          && front.cstmt.LastStmt->op.opr == OPR_STMT
          && front.cstmt.LastStmt->right->op.opr == OPR_CASE ) {
            new_ce->label = front.cstmt.SwitchStack->last_case_label;
            new_ce->gen_label = false;
        } else {
            new_ce->label = NextLabel(front);
            new_ce->gen_label = true;
        }
        front.cstmt.SwitchStack->number_of_cases++;
        if( I64Cmp( &converted_value, &front.cstmt.SwitchStack->low_value ) < 0 ) {
            front.cstmt.SwitchStack->low_value = converted_value;
        }
        if( I64Cmp( &converted_value, &front.cstmt.SwitchStack->high_value ) > 0 ) {
            front.cstmt.SwitchStack->high_value = converted_value;
        }
        front.cstmt.SwitchStack->last_case_label = new_ce->label;
        tree = LeafNode( front, OPR_CASE );
        tree->op.u2.case_info = new_ce;
        AddStmt( front, tree );
    }
}


static void CaseStmt( FrontRun& front )
{
    const_val   val;

    NextToken(front);
    if( front.cstmt.SwitchStack ) {
        if( ConstExprAndType( front, &val ) ) {
            AddCaseLabel( front, val.value );
        }
        MustRecog( front, T_COLON );
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            CErr1( front, ERR_STMT_REQUIRED_AFTER_CASE );
        }
    } else {
        CErr1( front, ERR_MISPLACED_CASE );
        ConstExprAndType( front, &val );        /* grab constant expression */
        MustRecog( front, T_COLON );
    }
}


#ifdef __SEH__
static void MarkTryVolatile( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYM_ENTRY   sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym.handle ) {
        SymGet( front, &sym, sym_handle );
        sym.flags |= SYM_TRY_VOLATILE; //force into memory
        SymReplace( front, &sym, sym_handle );
    }
}


static void TryStmt( FrontRun& front )
{
    TREEPTR     tree;

    MarkTryVolatile( front, front.cvars.CurFunc->u.func.parms );
    MarkTryVolatile( front, front.cvars.CurFunc->u.func.locals );
    front.cvars.CurFuncNode->op.u2.func.flags |=  FUNC_USES_SEH;
    front.cvars.CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
    front.cvars.CurToken = T__TRY;
    StartNewBlock(front);
    NextToken(front);
    front.cstmt.BlockStack->parent_index = front.cvars.TryScope;
    ++front.cvars.TryCount;
    front.cstmt.BlockStack->try_index = front.cvars.TryCount;
    front.cvars.TryScope = front.cvars.TryCount;
    tree = LeafNode( front, OPR_TRY );
    tree->op.u2.st.u.try_index = 0;
    tree->op.u2.st.parent_scope = front.cvars.TryCount;
    AddStmt( front, tree );
}


static SYM_HANDLE DummyTrySymbol( FrontRun& front )
{
    SYM_ENTRY   sym;
    SYM_HANDLE  sym_handle;

    sym_handle = MakeNewDotSym( front, &sym, 'T', GetType( front, TYP_VOID ), SC_STATIC );
    SymReplace( front, &sym, sym_handle );
    return( sym_handle );
}


static bool EndTry( FrontRun& front )
{
    tryindex_t  parent_scope;
    TREEPTR     expr;
    TREEPTR     func;
    TREEPTR     tree;
    TYPEPTR     typ;
    DATA_TYPE   expr_type;

    DropBreakLabel(front);           /* _leave jumps to this label */
    parent_scope = front.cstmt.BlockStack->parent_index;
    tree = LeafNode( front, OPR_TRY );
    tree->op.u2.st.u.try_index = front.cstmt.BlockStack->try_index;
    tree->op.u2.st.parent_scope = parent_scope;
    AddStmt( front, tree );
    if( (front.cvars.CurToken == T__EXCEPT)
      || (front.cvars.CurToken == T___EXCEPT) ) {
        NextToken(front);
        front.cstmt.BlockStack->block_type = T__EXCEPT;
        front.cstmt.BlockStack->break_label = NextLabel(front);
        Jump( front, front.cstmt.BlockStack->break_label );
        front.cvars.DeadCode = 0;
        tree = LeafNode( front, OPR_EXCEPT );
        tree->op.u2.st.u.try_sym_handle = DummyTrySymbol(front);
        tree->op.u2.st.parent_scope = parent_scope;
        AddStmt( front, tree );
        front.cvars.CompFlags.exception_filter_expr = true;
        expr = RValue( front, BracketExpr(front) );
        front.cvars.CompFlags.exception_filter_expr = false;
        front.cvars.CompFlags.exception_handler = true;
        typ = TypeOf( expr );
        expr_type = DataTypeOf( typ );
        if( expr_type != TYP_VOID ) {
            if( expr_type > TYP_ULONG ) {
                CErr1( front, ERR_EXPR_MUST_BE_INTEGRAL );
            }
        }
        func = VarLeaf( front, SymGetPtr( front, front.cvars.SymExcept ), front.cvars.SymExcept );
        func->op.opr = OPR_FUNCNAME;
        expr = ExprNode( front, NULL, OPR_PARM, expr );
        expr->u.expr_type = typ;
        expr->op.u2.result_type = typ;
        tree = ExprNode( front, func, OPR_CALL, expr );
        tree->u.expr_type = GetType( front, TYP_VOID );
        AddStmt( front, tree );
        return( true );
    } else if( (front.cvars.CurToken == T__FINALLY)
      || (front.cvars.CurToken == T___FINALLY) ) {
        front.cvars.CompFlags.in_finally_block = true;
        NextToken(front);
        front.cstmt.BlockStack->block_type = T__FINALLY;
        front.cvars.DeadCode = 0;
        tree = LeafNode( front, OPR_FINALLY );
        tree->op.u2.st.u.try_sym_handle = DummyTrySymbol(front);
        tree->op.u2.st.parent_scope = parent_scope;
        AddStmt( front, tree );
        return( true );
    }
    return( false );
}
#endif


static void PopBlock( FrontRun& front )
{
    BLOCKPTR    block;
    TREEPTR     tree;

    if( front.cstmt.BlockStack->gen_endblock ) {
        tree = LeafNode( front, OPR_ENDBLOCK );
        tree->op.u2.sym_handle = front.cstmt.BlockStack->sym_list;
        AddStmt( front, tree );
    }
    if( front.cstmt.BlockStack->sym_list != SYM_NULL ) {
        AddSymList( front, front.cstmt.BlockStack->sym_list );
    }
    block = front.cstmt.BlockStack;
    front.cstmt.LoopStack = block->prev_loop;
    front.cstmt.BlockStack = block->prev_block;
    CMemFree( front, block );
}


static void SwitchStmt( FrontRun& front )
{
    SWITCHPTR   sw;
    TREEPTR     tree;
    TYPEPTR     typ;

    StartNewBlock(front);
    NextToken(front);
    sw = static_cast<SWITCHPTR>(CMemAlloc( front, sizeof( SWITCHDEFN ) ));
    sw->prev_switch = front.cstmt.SwitchStack;
    Set64Val1m( sw->low_value );
    Set64ValZero( sw->high_value );
    front.cstmt.SwitchStack = sw;
    tree = RValue( front, BracketExpr(front) );
    typ = TypeOf( tree );
    SkipEnum( typ );
    switch( typ->decl_type ) {
    case TYP_BOOL:
    case TYP_USHORT:
    case TYP_UINT:
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_SHORT:
    case TYP_INT:
    case TYP_FIELD:
    case TYP_UFIELD:
    case TYP_ULONG:
    case TYP_LONG:
    case TYP_ULONG64:
    case TYP_LONG64:
        break;
    default:
        CErr1( front, ERR_INVALID_TYPE_FOR_SWITCH );
    }
    tree = ExprNode( front, NULL, OPR_SWITCH, tree );
    tree->op.u2.switch_info = sw;
    AddStmt( front, tree );
}


static void EndSwitch( FrontRun& front )
{
    SWITCHPTR   sw;
//    CASEPTR   ce;

    sw = front.cstmt.SwitchStack;
    front.cstmt.SwitchStack = sw->prev_switch;
    if( sw->default_label == 0 ) {
        if( front.cstmt.BlockStack->break_label == 0 ) {
            sw->default_label = NextLabel(front);
            DropLabel( front, sw->default_label );
        } else {
            sw->default_label = front.cstmt.BlockStack->break_label;
        }
    }
//    if( sw->case_list == NULL ) {
//        CWarn1( ERR_EMPTY_SWITCH );
//    }
}


static void EndOfStmt( FrontRun& front )
{
    do {
        switch( front.cstmt.BlockStack->block_type ) {
        case T_LEFT_BRACE:
            EndBlock(front);
            break;
        case T_IF:
            if( front.cvars.CurToken == T_ELSE ) {
                ElseStmt(front);
                return;
            }
            DropBreakLabel(front);
            break;
        case T_ELSE:
            DropBreakLabel(front);
            break;
        case T_WHILE:
            DropContinueLabel(front);
            Jump( front, front.cstmt.BlockStack->top_label );
            --front.cvars.LoopDepth;
            DropBreakLabel(front);
            break;
        case T_FOR:
            EndForStmt(front);
            --front.cvars.LoopDepth;
            DropBreakLabel(front);
            if( front.cvars.CompVars.cstd > STD_C89 ) {
                EndBlock(front);     /* Terminate the scope introduced by 'for' */
                PopBlock(front);
            }
            break;
        case T_DO:
            DropContinueLabel(front);
            MustRecog( front, T_WHILE );
            front.cvars.SrcLoc = front.cvars.TokenLoc;
            JumpTrue( front, BracketExpr(front), front.cstmt.BlockStack->top_label );
            MustRecog( front, T_SEMI_COLON );
            front.cvars.SrcLoc = front.cvars.TokenLoc;
            --front.cvars.LoopDepth;
            DropBreakLabel(front);
            break;
        case T_SWITCH:
            EndSwitch(front);
            DropBreakLabel(front);
            break;
#ifdef __SEH__
        case T__TRY:
        case T___TRY:
            if( EndTry(front) )
                return;
            break;
        case T__EXCEPT:
        case T___EXCEPT:
            DropBreakLabel(front);
            front.cvars.TryScope = front.cstmt.BlockStack->parent_index;
            front.cvars.CompFlags.exception_handler = false;
            break;
        case T__FINALLY:
        case T___FINALLY:
            AddStmt( front, LeafNode( front, OPR_END_FINALLY ) );
            front.cvars.CompFlags.in_finally_block = false;
            front.cvars.TryScope = front.cstmt.BlockStack->parent_index;
            break;
#endif
        }
        PopBlock(front);
        if( front.cstmt.BlockStack == NULL ) {
            break;
        }
    } while( front.cstmt.BlockStack->block_type != T_LEFT_BRACE );
}


static bool IsDeclarator( FrontRun& front, TOKEN token )
{
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;

    /* If token class is storage class or qualifier, it's a declaration */
    if( TokenClass[token] == TC_STG_CLASS
      || TokenClass[token] == TC_TYPE_QUALIFIER
      || TokenClass[token] == TC_TYPE_SPECIFIER
      || TokenClass[token] == TC_DECLSPEC ) {
        return( true );
    }

    /* If token is an ID, it might be a typedef */
    if( (front.cvars.CurToken == T_ID)
      || (front.cvars.CurToken == T_SAVED_ID) ) {
        if( front.cvars.CurToken == T_ID ) {
            sym_handle = SymLookTypedef( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer, &sym );
        } else {    /* T_SAVED_ID */
            sym_handle = SymLookTypedef( front, CalcHashID( front.cvars.SavedId ), front.cvars.SavedId, &sym );
        }
        if( sym_handle != SYM_NULL
          && sym.attribs.stg_class == SC_TYPEDEF ) {
            return( true );
        }
    }
    return( false );
}


static void FixupC99MainReturn( FrontRun& front, SYM_HANDLE func_result_handle, struct return_info *info )
{
    TREEPTR             tree;
    TYPEPTR             main_type;

    /* In C99 mode, return statement need not be explicit for main()... */
    main_type = front.cvars.CurFunc->sym_type->object;
    SkipTypedefs( main_type );
    /* ... as long as return type is compatible with int */
    if( main_type->decl_type == TYP_INT ) {
        tree = IntLeaf( front, 0 );    /* zero is the default return value */
        tree = ExprNode( front, NULL, OPR_RETURN, tree );
        tree->u.expr_type = main_type;
        tree->op.u2.sym_handle = func_result_handle;
        AddStmt( front, tree );
        info->with_expr = true;
    }
}


/*
 * What a function's body keeps from its `{` to its `}`: Statement reads the statements from the text between them,
 * L2Statement takes them from its caller's builder (l2body.hpp); both begin and end a body as these two do.
 */
struct FuncBody {
    LABEL_INDEX         end_of_func_label = 0;
    SYM_HANDLE          func_result_handle = SYM_NULL;
    TREEPTR             tree = NULL;            /* the OPR_LABELCOUNT node */
    bool                return_at_outer_level = false;
    struct return_info  return_info;
};

static void BeginFuncBody( FrontRun& front, FuncBody& body )
{
    SYM_ENTRY           sym;

    ++front.cvars.FuncCount;
    body.return_info.with = RETURN_WITH_NONE; /* indicate no return statements */
    body.return_info.with_expr = false;
    front.cvars.CompFlags.label_dropped = false;
    front.cvars.CompFlags.addr_of_auto_taken = false;
    body.end_of_func_label = 0;
    body.return_at_outer_level = false;
    front.cvars.DeadCode = 0;
    front.cvars.LoopDepth = 0;
    front.cvars.LabelIndex = 0;
    front.cstmt.BlockStack = NULL;
    front.cstmt.LoopStack = NULL;
    front.cstmt.SwitchStack = NULL;
#ifdef __SEH__
    front.cvars.TryCount = TRYSCOPE_NONE;
    front.cvars.TryScope = TRYSCOPE_NONE;
#endif
    StartNewBlock(front);
    NextToken(front);                        // skip over {
    ++front.cvars.SymLevel;
    body.tree = LeafNode( front, OPR_LABELCOUNT );
    AddStmt( front, body.tree );
    if( GrabLabels(front) == 0 ) {
        GetLocalVarDecls(front);
    }
    body.func_result_handle = MakeNewDotSym( front, &sym, 'R', front.cvars.CurFunc->sym_type->object, SC_AUTO );
    sym.flags |= SYM_FUNC_RETURN_VAR;
    SymReplace( front, &sym, body.func_result_handle );
}

static void EndFuncBody( FrontRun& front, FuncBody& body, bool skip_to_next_token );

void Statement( FrontRun& front )
{
    FuncBody            body;
    bool                skip_to_next_token;
    bool                declaration_allowed;

    declaration_allowed = false;
    BeginFuncBody( front, body );
    for( ;; ) {
        front.cvars.CompFlags.pending_dead_code = false;
        if( GrabLabels(front) == 0
          && declaration_allowed
          && IsDeclarator( front, front.cvars.CurToken ) ) {
            GetLocalVarDecls(front);
        }
        if( front.cvars.CompVars.cstd > STD_C89 ) {
            declaration_allowed = true;
        }
        skip_to_next_token = false;
        switch( front.cvars.CurToken ) {
        case T_IF:
            StartNewBlock(front);
            NextToken(front);
            front.cstmt.BlockStack->break_label = NextLabel(front);
            JumpFalse( front, BracketExpr(front), front.cstmt.BlockStack->break_label );
            /* only issue msg if ';' is on same line as 'if' */
            if( front.cvars.CurToken == T_SEMI_COLON
              && front.cvars.SrcLoc.line == front.cvars.TokenLoc.line
              && front.cvars.SrcLoc.fno == front.cvars.TokenLoc.fno ) {
                SetErrLoc( front, &front.cvars.TokenLoc );
                NextToken(front);    /* look ahead for else keyword */
                if( front.cvars.CurToken != T_ELSE ) {
                    CheckUseful(front);
                }
                InitErrLoc(front);
                break;
            }
            declaration_allowed = false;
            continue;
        case T_ELSE:
            /* misplaced else, no previous if */
            CErr1( front, ERR_ELSE_WITHOUT_IF );
            NextToken(front);
            continue;
        case T_WHILE:
            NewLoop(front);
            NextToken(front);
            front.cstmt.BlockStack->break_label = NextLabel(front);
            if( !JumpFalse( front, BracketExpr(front), front.cstmt.BlockStack->break_label ) ) {
                front.cstmt.BlockStack->break_label = 0;
            }
            if( front.cvars.CurToken == T_SEMI_COLON ) {
                if( !front.cvars.CompFlags.useful_side_effect ) {
                    CWarn1( front, ERR_MEANINGLESS );
                }
            }
            declaration_allowed = false;
            continue;
        case T_DO:
            NewLoop(front);
            NextToken(front);
            if( front.cvars.CurToken == T_RIGHT_BRACE ) {
                CErr1( front, ERR_STMT_REQUIRED_AFTER_DO );
                break;
            }
            declaration_allowed = false;
            continue;
        case T_FOR:
            ForStmt(front);
            declaration_allowed = false;
            continue;
        case T_SWITCH:
            SwitchStmt(front);
            front.cvars.DeadCode = 1;
            declaration_allowed = false;
            continue;
        case T_CASE:
            front.cvars.DeadCode = 0;
            CaseStmt(front);
            declaration_allowed = false;
            continue;
        case T_DEFAULT:
            DefaultStmt(front);
            declaration_allowed = false;
            continue;
        case T_BREAK:
            BreakStmt(front);
            front.cvars.DeadCode = 1;
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_CONTINUE:
            ContinueStmt(front);
            front.cvars.DeadCode = 1;
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
#ifdef __SEH__
        case T__LEAVE:
        case T___LEAVE:
            LeaveStmt(front);
            front.cvars.DeadCode = 1;
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
#endif
        case T_RETURN:
            ReturnStmt( front, body.func_result_handle, &body.return_info );
            if( front.cstmt.BlockStack->prev_block == NULL ) {
                body.return_at_outer_level = true;
            }
            MustRecog( front, T_SEMI_COLON );
            if( front.cvars.SymLevel != 1
              || front.cvars.CurToken != T_RIGHT_BRACE
              || front.cstmt.BlockStack->block_type != T_LEFT_BRACE ) {
                if( body.end_of_func_label == 0 ) {
                    body.end_of_func_label = NextLabel(front);
                }
                Jump( front, body.end_of_func_label );
            }
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_GOTO:
            GotoStmt(front);
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_SEMI_COLON:
            NextToken(front);
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE ) {
                if( front.cvars.CurToken == T_ELSE ) {
                    declaration_allowed = false;
                }
                break;
            }
            continue;
        case T_LEFT_BRACE:
            LeftBrace(front);
            continue;
        case T_RIGHT_BRACE:
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE ) {
                CErr1( front, ERR_MISPLACED_RIGHT_BRACE );
            }
            if( front.cstmt.BlockStack->prev_block == NULL ) {
                skip_to_next_token = true;
            } else {
                NextToken(front);
            }
            break;
        case T_EXTERN:
        case T_STATIC:
        case T_AUTO:
        case T_REGISTER:
        case T_VOID:
        case T_CHAR:
        case T_SHORT:
        case T_INT:
        case T_LONG:
        case T_FLOAT:
        case T_DOUBLE:
        case T_SIGNED:
        case T_UNSIGNED:
            if( front.cvars.CompVars.cstd < STD_C99 ) {
                CErr1( front, ERR_MISSING_RIGHT_BRACE );
            } else {
                CErr1( front, ERR_UNEXPECTED_DECLARATION );
            }
            break;
        case T_EOF:
            CErr1( front, ERR_MISSING_RIGHT_BRACE );
            break;
#ifdef __SEH__
        case T__TRY:
        case T___TRY:
            TryStmt(front);
            continue;
#endif
        case T___ASM:
            AsmStmt(front);
            continue;
        default:
            if( front.cvars.DeadCode == 1 ) {
                DeadMsg(front);
            }
            StmtExpr(front);
            if( front.cstmt.BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        }
        EndOfStmt(front);
        if( front.cstmt.BlockStack == NULL )
            break;
        if( skip_to_next_token ) {
            NextToken(front);
        }
    }
    EndFuncBody( front, body, skip_to_next_token );
}

static void EndFuncBody( FrontRun& front, FuncBody& body, bool skip_to_next_token )
{
    TREEPTR             tree;

    /* C99 has special semantics for return value of main() */
    if( ( front.cvars.CompVars.cstd > STD_C89 )
      && strcmp( front.cvars.CurFunc->name, "main" ) == 0 ) {
        if( !body.return_at_outer_level ) {
            FixupC99MainReturn( front, body.func_result_handle, &body.return_info );
            body.return_at_outer_level = true;
        }
    }
    if( !body.return_info.with_expr ) {   /* no return values present */
        if( front.cvars.DeadCode == 0
          && !front.cvars.CurFunc->attribs.naked ) {
            CheckRetValue(front);
        }
    } else if( !body.return_at_outer_level ) {
        if( front.cvars.DeadCode == 0
          && !front.cvars.CurFunc->attribs.naked ) {
            CWarn2p( front, ERR_MISSING_RETURN_VALUE, front.cvars.CurFunc->name );
        }
    }
    if( body.end_of_func_label != 0 ) {
        DropLabel( front, body.end_of_func_label );
    }
    front.cvars.DeadCode = 0;
    body.tree->op.u2.label_count = front.cvars.LabelIndex;
    tree = LeafNode( front, OPR_FUNCEND );
    if( !body.return_info.with_expr ) {
        tree->u.expr_type = NULL;
        tree->op.u2.sym_handle = SYM_NULL;
    } else {
        tree->op.u2.sym_handle = body.func_result_handle;
        SetFuncReturnNode( front, tree );
    }
    AddStmt( front, tree );
    AddSymList( front, front.cvars.CurFunc->u.func.locals );
    front.cvars.SrcLoc = front.cvars.TokenLoc;
    FreeLabels(front);
    if( skip_to_next_token ) {
        NextToken(front);
    }
    if( front.cvars.CompFlags.generate_prototypes ) {
        if( front.cvars.DefFile == NULL ) {
            OpenDefFile(front);
        }
        if( front.cvars.DefFile != NULL
          && front.cvars.CurFunc->attribs.stg_class == SC_NONE ) {
            /* function exported */
            DumpFuncDefn(front);
        }
    }

    if( Toggles( front ).tog_inline ) {
        if( front.cvars.Inline_Threshold < front.cvars.NodeCount ) {
            front.cvars.CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
        }
    }
    if( VarParm( front.cvars.CurFunc ) ) {
        front.cvars.CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
    }
    front.cvars.NodeCount = 0;
    if( front.cvars.CompFlags.addr_of_auto_taken ) {
        front.cvars.CurFunc->flags &= ~ SYM_OK_TO_RECURSE;
    }
}


/*
 * A function body given by the host (build/x86cc/l2body.hpp) rather than read from the text: the text's body is `{}`, and
 * the statements between the braces are the builder's calls. Each call does what the parser does for the same C
 * (ReturnStmt, CheckStmtExpr, SymLeaf, the reductions of GetExpr) without the tokens, so the trees are the ones the text
 * would give.
 */
namespace {

namespace x86cc = cb::build::x86cc;

class FrontBodyBuilder final : public x86cc::BodyBuilder {
public:
    FrontBodyBuilder( FrontRun& front, FuncBody& body ) : front_( front ), body_( body ) {}

    Value name( std::string_view id ) override
    {
        std::string text( id );
        TREEPTR     t;

        t = NameLeaf( front_, text.c_str() );
        if( t == NULL ) {
            fail( "no symbol named " + text );
            t = IntLeaf( front_, 0 );
        }
        return( wrap( t ) );
    }

    Value integer( std::uint64_t bits, x86cc::IntType type ) override
    {
        DATA_TYPE   dt;

        switch( type ) {
        case x86cc::IntType::UInt:      dt = TYP_UINT; break;
        case x86cc::IntType::Long:      dt = TYP_LONG; break;
        case x86cc::IntType::ULong:     dt = TYP_ULONG; break;
        case x86cc::IntType::LongLong:  dt = TYP_LONG64; break;
        case x86cc::IntType::ULongLong: dt = TYP_ULONG64; break;
        default:                        dt = TYP_INT; break;
        }
        return( wrap( IntConstLeaf( front_, dt, bits ) ) );
    }

    Value binary( x86cc::BinaryOp op, Value left, Value right ) override
    {
        TREEPTR     l = tree( left );
        TREEPTR     r = tree( right );

        /* as GetExpr reduces each class of operator */
        switch( op ) {
        case x86cc::BinaryOp::Add: return( wrap( AddOp( front_, l, T_PLUS, r ) ) );
        case x86cc::BinaryOp::Sub: return( wrap( AddOp( front_, l, T_MINUS, r ) ) );
        case x86cc::BinaryOp::Mul: return( wrap( BinOp( front_, RValue( front_, l ), T_TIMES, r ) ) );
        case x86cc::BinaryOp::Div: return( wrap( BinOp( front_, RValue( front_, l ), T_DIV, r ) ) );
        case x86cc::BinaryOp::Rem: return( wrap( BinOp( front_, RValue( front_, l ), T_PERCENT, r ) ) );
        case x86cc::BinaryOp::Shl: return( wrap( ShiftOp( front_, l, T_LSHIFT, r ) ) );
        case x86cc::BinaryOp::Shr: return( wrap( ShiftOp( front_, l, T_RSHIFT, r ) ) );
        case x86cc::BinaryOp::And: return( wrap( IntOp( front_, l, T_AND, r ) ) );
        case x86cc::BinaryOp::Or:  return( wrap( IntOp( front_, l, T_OR, r ) ) );
        case x86cc::BinaryOp::Xor: return( wrap( IntOp( front_, l, T_XOR, r ) ) );
        case x86cc::BinaryOp::Eq:  return( wrap( RelOp( front_, l, T_EQ, r ) ) );
        case x86cc::BinaryOp::Ne:  return( wrap( RelOp( front_, l, T_NE, r ) ) );
        case x86cc::BinaryOp::Lt:  return( wrap( RelOp( front_, l, T_LT, r ) ) );
        case x86cc::BinaryOp::Le:  return( wrap( RelOp( front_, l, T_LE, r ) ) );
        case x86cc::BinaryOp::Gt:  return( wrap( RelOp( front_, l, T_GT, r ) ) );
        case x86cc::BinaryOp::Ge:  return( wrap( RelOp( front_, l, T_GE, r ) ) );
        }
        fail( "an operator the front end has no action for" );
        return( left );
    }

    void expression( Value value ) override
    {
        /* as CheckStmtExpr, with the expression made */
        if( front_.cvars.DeadCode == 1 ) {
            DeadMsg( front_ );
        }
        AddStmt( front_, tree( value ) );
        if( front_.cvars.CompFlags.pending_dead_code ) {
            front_.cvars.DeadCode = 1;
        }
        next();
    }

    void return_value( Value value, bool last ) override
    {
        TYPEPTR     func_type;
        TREEPTR     t;

        /* as ReturnStmt */
        func_type = front_.cvars.CurFunc->sym_type->object;
        SkipTypedefs( func_type );
        t = RValue( front_, tree( value ) );
        CheckRetType( front_, t );
        t = FixupAss( front_, t, func_type );
        t = ExprNode( front_, NULL, OPR_RETURN, t );
        t->u.expr_type = func_type;
        t->op.u2.sym_handle = body_.func_result_handle;
        AddStmt( front_, t );
        body_.return_info.with_expr = true;
        returned( RETURN_WITH_EXPR, last );
    }

    void return_void( bool last ) override
    {
        returned( RETURN_WITH_NO_EXPR, last );
    }

    bool failed() const override { return( !why_.empty() ); }
    const std::string& why() const override { return( why_ ); }

private:
    static Value wrap( TREEPTR t ) { return( Value{ t } ); }
    static TREEPTR tree( Value value ) { return( static_cast<TREEPTR>( value.tree ) ); }

    void fail( std::string why )
    {
        if( why_.empty() ) {
            why_ = std::move( why );
        }
    }

    /* what Statement's loop does before the next statement */
    void next()
    {
        front_.cvars.CompFlags.pending_dead_code = false;
    }

    /* the rest of ReturnStmt and what Statement does after it; `last` stands for the text's `; }` at the outer block */
    void returned( enum return_with with, bool last )
    {
        BLOCKPTR    block;

        if( body_.return_info.with == RETURN_WITH_NONE ) {
            body_.return_info.with = with;
        }
        if( body_.return_info.with != with ) {
            CErr1( front_, ERR_INCONSISTENT_USE_OF_RETURN );
        }
        for( block = front_.cstmt.BlockStack; block != NULL; block = block->prev_block ) {
            if( (block->block_type == T__TRY)
              || (block->block_type == T___TRY) ) {
                UnWindTry( front_, TRYSCOPE_NONE );
                break;
            }
        }
        if( front_.cstmt.BlockStack->prev_block == NULL ) {
            body_.return_at_outer_level = true;
        }
        if( front_.cvars.SymLevel != 1
          || !last
          || front_.cstmt.BlockStack->block_type != T_LEFT_BRACE ) {
            if( body_.end_of_func_label == 0 ) {
                body_.end_of_func_label = NextLabel( front_ );
            }
            Jump( front_, body_.end_of_func_label );
        }
        next();
    }

    FrontRun&   front_;
    FuncBody&   body_;
    std::string why_;
};

x86cc::BodySource *HostBodies = NULL;   /* the host's, for the compile running now (compiles run one at a time: x86cc.cpp) */

}  // namespace

/* the host's bodies for the next compiles (null: none), each target's entry named for it as its IDE entries are */
#if _CPU == 386
extern "C" void wcc386_L2SetBodies( x86cc::BodySource *bodies )
#else
extern "C" void wcci86_L2SetBodies( x86cc::BodySource *bodies )
#endif
{
    HostBodies = bodies;
}

bool L2Statement( FrontRun& front )
{
    if( HostBodies == NULL
      || !HostBodies->gives( front.cvars.CurFunc->name ) ) {
        return( false );
    }
    FuncBody body;
    BeginFuncBody( front, body );
    {
        FrontBodyBuilder builder( front, body );
        HostBodies->build( front.cvars.CurFunc->name, builder );
    }
    /* the text's `}`, as Statement's loop takes it */
    front.cvars.CompFlags.pending_dead_code = false;
    GrabLabels(front);
    if( front.cvars.CurToken != T_RIGHT_BRACE ) {
        CErr1( front, ERR_MISSING_RIGHT_BRACE );
    }
    EndOfStmt(front);
    EndFuncBody( front, body, true );
    return( true );
}
