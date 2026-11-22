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


#include "cvars.h"
#include "i64.h"
#include "cfeinfo.h"
#include "toggles.h"


typedef struct block_entry {
    struct block_entry  *prev_block;
    struct block_entry  *prev_loop;
    TOKEN       block_type;
    LABEL_INDEX top_label;
    LABEL_INDEX break_label;
    LABEL_INDEX continue_label;
    tryindex_t  try_index;      /* TRY: current index */
    tryindex_t  parent_index;   /* TRY: parent index */
    TREEPTR     inc_var;        /* variable used in FOR statement */
    SYM_HANDLE  sym_list;       /* symbols defined in this block */
    bool        gen_endblock;   /* set if OPR_ENDBLOCK needed */
} BLOCKDEFN, *BLOCKPTR;

/*
 * values for return_type
 */
enum return_with {
    RETURN_WITH_NONE = 0,
    RETURN_WITH_NO_EXPR = 1,
    RETURN_WITH_EXPR = 2,
};

struct return_info {
    enum return_with    with;
    bool                with_expr;
};


namespace {

/* the statics of this file, one set for each run of the compiler */
struct CstmtState {
    TREEPTR LastStmt;
    BLOCKPTR BlockStack;
    BLOCKPTR LoopStack;
    SWITCHPTR SwitchStack;
};

}  // namespace

static inline CstmtState& cstmt_state() { return cbrun_part<CstmtState>(); }


void StmtInit( void )
{
    cvars_state().FirstStmt = NULL;
    cstmt_state().LastStmt = NULL;
    cvars_state().SymListHeads = NULL;
}


static void CheckStringLeaf( TREEPTR leaf )
{
    if( leaf->op.opr == OPR_PUSHSTRING ) {
        leaf->op.u2.string_handle->ref_count++;
    }
}


static void CheckUseful( void )
{
    if( cvars_state().CompFlags.useful_side_effect ) {
        CWarn1( ERR_USEFUL_SIDE_EFFECT );
    } else {
        CWarn1( ERR_MEANINGLESS );
    }
}


static void CheckStmtExpr( void )
{
    TREEPTR     tree;

    tree = Expr();
    if( cvars_state().CompFlags.meaningless_stmt ) {
        CheckUseful();
    }
    AddStmt( tree );
    if( cvars_state().CompFlags.pending_dead_code ) {
        cvars_state().DeadCode = 1;
    }
}


SYM_HANDLE GetBlockSymList( void )
{
    return( cstmt_state().BlockStack->sym_list );
}


void InitStmt( void )
{
    cstmt_state().BlockStack = NULL;
    cstmt_state().LoopStack = NULL;
    cstmt_state().SwitchStack = NULL;
}


void SwitchPurge( void )
{
    SWITCHPTR   sw;
    CASEPTR     c_entry;
    CASEPTR     c_tmp;

    while( (sw = cstmt_state().SwitchStack) != NULL ) {
        cstmt_state().SwitchStack = sw->prev_switch;
        c_entry = sw->case_list;
        while( c_entry != NULL ) {
            c_tmp = c_entry->next_case;
            CMemFree( c_entry );
            c_entry = c_tmp;
        }
        CMemFree( sw );
    }
}


void AddStmt( TREEPTR stmt )
{
    WalkExprTree( stmt, CheckStringLeaf, NoOp, NoOp, DoConstFold );
    stmt = ExprNode( NULL, OPR_STMT, stmt );
    stmt->op.u2.src_loc = cvars_state().SrcLoc;
    stmt->op.u1.unroll_count = cvars_state().UnrollCount;
    if( cvars_state().FirstStmt == NULL )
        cvars_state().FirstStmt = stmt;
    if( cstmt_state().LastStmt != NULL ) {
        cstmt_state().LastStmt->left = stmt;
    }
    cstmt_state().LastStmt = stmt;
}


static void AddSymList( SYM_HANDLE sym_handle )
{
    SYM_LIST_HEADS   *symlisthead;

    symlisthead = (SYM_LIST_HEADS *)CMemAlloc( sizeof( SYM_LIST_HEADS ) );
    symlisthead->next = cvars_state().SymListHeads;
    symlisthead->sym_head = sym_handle;
    cvars_state().SymListHeads = symlisthead;
}


void GenFunctionNode( SYM_HANDLE sym_handle )
{
    TREEPTR     tree;
    SYMPTR      sym;

    tree = LeafNode( OPR_FUNCTION );
    sym = SymGetPtr( sym_handle );
    tree->op.u2.func.sym_handle = sym_handle;
    tree->op.u2.func.flags = FUNC_NONE;
    if( TOGGLE( inline )
      || (sym->mods & FLAG_INLINE) ) {
        if( !sym->attribs.naked ) {
            if( strcmp( sym->name, "main" ) != 0 ) {
                tree->op.u2.func.flags |= FUNC_OK_TO_INLINE;
            }
        }
    }
    tree->op.flags = OpFlags( sym->mods );
    tree->u.expr_type = sym->sym_type->object;    // function return type
    AddStmt( tree );
    /*
     * Evil, evil globals! But we need this for later lookups in cgen.c
     */
    sym->u.func.start_of_func = cstmt_state().LastStmt;
    cvars_state().CurFuncNode = tree;
    cvars_state().NodeCount = 0;
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
#ifdef _WIN64
__declspec(noinline)
#endif
LABEL_INDEX NextLabel( void )
{
    return( ++cvars_state().LabelIndex );
}


static void DropLabel( LABEL_INDEX label )
{
    TREEPTR     tree;

    cvars_state().CompFlags.label_dropped = true;
    cvars_state().DeadCode = 0;
    tree = LeafNode( OPR_LABEL );
    tree->op.u2.label_index = label;
    AddStmt( tree );
}


static void DropBreakLabel( void )
{
    if( cstmt_state().BlockStack->break_label != 0 ) {
        DropLabel( cstmt_state().BlockStack->break_label );
    }
}


static void DropContinueLabel( void )
{
    if( cstmt_state().BlockStack->continue_label != 0 ) {
        DropLabel( cstmt_state().BlockStack->continue_label );
    }
}


static TREEPTR BracketExpr( void )
{
    TREEPTR     tree;

    MustRecog( T_LEFT_PAREN );
    tree = Expr();
    FoldExprTree( tree );
    MustRecog( T_RIGHT_PAREN );
    return( tree );
}


static void JumpCond( TREEPTR expr,
                     LABEL_INDEX label,
                     opr_code jump_opcode,
                     opr_code jump_opposite )
{
    TREEPTR     tree;

    tree = BoolExpr( expr );
    if( tree->op.opr == OPR_NOT ) {
        tree->op.opr = jump_opposite;
    } else {
        tree = ExprNode( NULL, jump_opcode, tree );
    }
    tree->op.u2.label_index = label;
    AddStmt( tree );
}


static void Jump( LABEL_INDEX label )
{
    TREEPTR     tree;

    if( cvars_state().DeadCode == 0 ) {
        tree = LeafNode( OPR_JUMP );
        tree->op.u2.label_index = label;
        AddStmt( tree );
        cvars_state().DeadCode = 1;
    }
}


static bool JumpFalse( TREEPTR expr, LABEL_INDEX label )
{
    bool        jump_generated;

    jump_generated = false;
    if( expr->op.opr == OPR_PUSHINT ) {
        if( CheckZeroConstant( expr ) ) {
            Jump( label );
            jump_generated = true;
        }
        FreeExprNode( expr );
    } else {
        JumpCond( expr, label, OPR_JUMPFALSE, OPR_JUMPTRUE );
        jump_generated = true;
    }
    return( jump_generated );
}


static void JumpTrue( TREEPTR expr, LABEL_INDEX label )
{
    if( expr->op.opr == OPR_PUSHINT ) {
        if( !CheckZeroConstant( expr ) ) {
            Jump( label );
        }
        FreeExprNode( expr );
    } else {
        JumpCond( expr, label, OPR_JUMPTRUE, OPR_JUMPFALSE );
    }
}


void LookAhead( void )
{
    cvars_state().SavedId = CMemStrdup( cvars_state().Buffer );     /* save current id */
    cvars_state().SavedTokenLoc = cvars_state().TokenLoc;           /* save linenum and fno */
    NextToken();                        /* get next token */
    cvars_state().LAToken = cvars_state().CurToken;                 /* save it in look ahead */
    cvars_state().CurToken = T_SAVED_ID;              /* go back to saved id */
}


static bool GrabLabels( void )
{
    LABELPTR    label;

    label = NULL;
    for( ;; ) {
        if( cvars_state().CurToken == T_SAVED_ID ) {
            cvars_state().SrcLoc = cvars_state().SavedTokenLoc;
        } else {
            cvars_state().SrcLoc = cvars_state().TokenLoc;
            if( cvars_state().CurToken != T_ID )
                break;
            LookAhead();
        }
        if( cvars_state().LAToken != T_COLON )
            break; /* quit if look ahead not : */
        label = LkLabel( cvars_state().SavedId );
        if( label->defined ) {
            CErr2p( ERR_LABEL_ALREADY_DEFINED, label->name );
        } else {
            DropLabel( label->ref_list );
            label->defined = true;      /* indicate label defined now */
        }
        CMemFree( cvars_state().SavedId );            /* free the saved id */
        cvars_state().SavedId = NULL;
        NextToken();                    /* get next token */
    }
    if( label != NULL ) {               /* label was found */
        if( cvars_state().CurToken == T_RIGHT_BRACE ) {
            CErr1( ERR_STMT_REQUIRED_AFTER_LABEL );
        }
        return( true );                    // indicate label found
    }
    return( false );
}


static void UnWindTry( tryindex_t try_scope )
{
#ifdef __SEH__
    TREEPTR     tree;

    tree = LeafNode( OPR_UNWIND );
    tree->op.u2.st.u.try_index = try_scope;
    AddStmt( tree );
#else
    /* unused parameters */ (void)try_scope;
#endif
}


static void ReturnStmt( SYM_HANDLE func_result_handle, struct return_info *info )
{
    TREEPTR             tree;
    BLOCKPTR            block;
    enum return_with    with;

    NextToken();
    if( cvars_state().CurToken != T_SEMI_COLON ) {
        TYPEPTR     func_type;

        func_type = cvars_state().CurFunc->sym_type->object;
        SKIP_TYPEDEFS( func_type );
        tree = RValue( Expr() );
        CheckRetType( tree );
        tree = FixupAss( tree, func_type );
        tree = ExprNode( NULL, OPR_RETURN, tree );
        tree->u.expr_type = func_type;
        tree->op.u2.sym_handle = func_result_handle;
        AddStmt( tree );
        with = RETURN_WITH_EXPR;
        info->with_expr = true;
    } else {
        with = RETURN_WITH_NO_EXPR;
    }
    if( info->with == RETURN_WITH_NONE ) {
        info->with = with;
    }
    if( info->with != with ) {
        CErr1( ERR_INCONSISTENT_USE_OF_RETURN );
    }
    for( block = cstmt_state().BlockStack; block != NULL; block = block->prev_block ) {
        if( (block->block_type == T__TRY)
          || (block->block_type == T___TRY) ) {
            UnWindTry( TRYSCOPE_NONE );
            break;
        }
    }
}


static void SetFuncReturnNode( TREEPTR tree )
{
    TYPEPTR     typ;

    typ = cvars_state().CurFunc->sym_type->object;
    tree->u.expr_type = typ;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_STRUCT
      || typ->decl_type == TYP_UNION ) {
        tree->right = LeafNode( OPR_NOP );      // place holder
        tree->right->u.expr_type = NULL;
    }
}


static void CheckRetValue( void )
{
    TYPEPTR     typ;

    typ = cvars_state().CurFunc->sym_type;
    typ = typ->object;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type != TYP_VOID ) {
        CWarn2p( ERR_MISSING_RETURN_VALUE, cvars_state().CurFunc->name );
    }
}

static SYM_HANDLE GetLocalVarDecls( void )
{
    SYM_HANDLE  symlist;
    SYM_HANDLE  symhandle;
    SYM_ENTRY   sym;

    DeclList( &symlist );
    if( symlist != SYM_NULL ) {
        symhandle = cvars_state().CurFunc->u.func.locals;
        /*
         * symhandle will be non-zero if MakeNewDotSym was called while
         * parsing the declaration list.
         */
        if( symhandle != SYM_NULL ) {   // if some temporaries were created
            for( ;; ) {                 // - find end of list
                SymGet( &sym, symhandle );
                if( sym.handle == SYM_NULL )
                    break;
                symhandle = sym.handle;
            }
            sym.handle = symlist;
            SymReplace( &sym, symhandle );
        } else {
            cvars_state().CurFunc->u.func.locals = symlist;
        }
    }
    return( symlist );
}


static void DeadMsg( void )
/*************************/
{
    CWarn1( ERR_DEAD_CODE );
    cvars_state().DeadCode = 2;   /* so we don't get more messages */
}


static void StartNewBlock( void )
{
    BLOCKPTR    block;

    block = (BLOCKPTR)CMemAlloc( sizeof( BLOCKDEFN ) );
    block->block_type = cvars_state().CurToken;
    block->top_label = 0;
    block->continue_label = 0;
    block->break_label = 0;
    block->prev_block = cstmt_state().BlockStack;
    block->prev_loop = cstmt_state().LoopStack;
    block->gen_endblock = false;
    cstmt_state().BlockStack = block;
}


static void LeftBrace( void )
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
    StartNewBlock();
    NextToken();
    ++cvars_state().SymLevel;
    tree = LeafNode( OPR_NEWBLOCK );
    AddStmt( tree );
    cstmt_state().BlockStack->sym_list = SYM_NULL;
    cstmt_state().BlockStack->gen_endblock = true;
    DeclList( &cstmt_state().BlockStack->sym_list );
    tree->op.u2.sym_handle = cstmt_state().BlockStack->sym_list;
}


static void JumpBreak( BLOCKPTR block )
{
    if( cvars_state().DeadCode == 0 ) {
        if( block->break_label == 0 ) {
            block->break_label = NextLabel();
        }
        Jump( block->break_label );
    }
}


static void BreakStmt( void )
{
    BLOCKPTR    block;
    tryindex_t  try_scope;

    NextToken();
    try_scope = TRYSCOPE_UNDEF;
    block = cstmt_state().BlockStack;
    if( block != NULL ) {
        while( block != cstmt_state().LoopStack ) {
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
            UnWindTry( try_scope );
        }
        JumpBreak( block );
    } else {
        CErr1( ERR_MISPLACED_BREAK );
    }
    MustRecog( T_SEMI_COLON );
}


#ifdef __SEH__
static void LeaveStmt( void )
{
    BLOCKPTR    block;

    NextToken();
    block = cstmt_state().BlockStack;
    while( block != NULL ) {
        if( (block->block_type == T__TRY)
          || (block->block_type == T___TRY) )
            break;
        block = block->prev_block;
    }
    if( block != NULL ) {
        JumpBreak( block );
    } else {
        CErr1( ERR_MISPLACED_LEAVE );
    }
    MustRecog( T_SEMI_COLON );
}
#endif


static void ContinueStmt( void )
{
    BLOCKPTR    block;
    tryindex_t  try_scope;

    NextToken();
    if( cstmt_state().LoopStack != NULL ) {
        if( cvars_state().DeadCode == 0 ) {
            try_scope = TRYSCOPE_UNDEF;
            block = cstmt_state().BlockStack;
            while( block != cstmt_state().LoopStack ) {
                if( ( block->block_type == T__TRY )
                  || ( block->block_type == T___TRY ) ) {
                    try_scope = block->parent_index;
                }
                block = block->prev_block;
            }
            if( try_scope != TRYSCOPE_UNDEF ) {
                UnWindTry( try_scope );
            }
            if( cstmt_state().LoopStack->continue_label == 0 ) {
                cstmt_state().LoopStack->continue_label = NextLabel();
            }
            Jump( cstmt_state().LoopStack->continue_label );
        }
    } else {
        CErr1( ERR_MISPLACED_CONTINUE );
    }
    MustRecog( T_SEMI_COLON );
}


static void DefaultStmt( void )
{
    NextToken();
    MustRecog( T_COLON );
    if( cstmt_state().SwitchStack ) {
        if( cstmt_state().SwitchStack->default_label == 0 ) {
            cstmt_state().SwitchStack->default_label = NextLabel();
            DropLabel( cstmt_state().SwitchStack->default_label );
        } else {
            CErr1( ERR_ONLY_1_DEFAULT );
        }
        if( cvars_state().CurToken == T_RIGHT_BRACE ) {
            CErr1( ERR_STMT_REQUIRED_AFTER_DEFAULT );
        }
    } else {
        CErr1( ERR_MISPLACED_DEFAULT );
    }
}


static void ElseStmt( void )
{
    LABEL_INDEX if_label;

    cvars_state().SrcLoc = cvars_state().TokenLoc;
    NextToken();
    cstmt_state().BlockStack->block_type = T_ELSE;
    if_label = cstmt_state().BlockStack->break_label;
    if( cvars_state().DeadCode == 0 ) {
        cstmt_state().BlockStack->break_label = NextLabel();
        Jump( cstmt_state().BlockStack->break_label );
    } else {
        cstmt_state().BlockStack->break_label = 0;
    }
    DropLabel( if_label );
}


static void GotoStmt( void )
{
    LABELPTR    label;

    NextToken();
    if( cvars_state().CurToken != T_ID ) {
        CErr1( ERR_EXPECTING_LABEL );
    } else {
        label = LkLabel( cvars_state().Buffer );
        label->referenced = true;
        Jump( label->ref_list );
    }
    NextToken();
    MustRecog( T_SEMI_COLON );
}


static void PushBlock( void )
{
    StartNewBlock();
    ++cvars_state().SymLevel;
    cstmt_state().BlockStack->sym_list = SYM_NULL;
}


static void NewLoop( void )
{
    StartNewBlock();
    cstmt_state().LoopStack = cstmt_state().BlockStack;
    cstmt_state().LoopStack->top_label = NextLabel();
    DropLabel( cstmt_state().LoopStack->top_label );
    ++cvars_state().LoopDepth;
}


static void ForStmt( void )
{
    bool    parsed_semi_colon = false;

    NextToken();
    MustRecog( T_LEFT_PAREN );
    if( cvars_state().CompVars.cstd > STD_C89 ) {
        PushBlock();    // 'for' opens new scope
    }
    if( cvars_state().CurToken != T_SEMI_COLON ) {
        if( cvars_state().CompVars.cstd > STD_C89 ) {
            TREEPTR     tree;

            tree = LeafNode( OPR_NEWBLOCK );
            AddStmt( tree );
            cstmt_state().BlockStack->gen_endblock = true;
            if( !LoopDecl( &cstmt_state().BlockStack->sym_list ) ) {
                CheckStmtExpr();    // no declarator, try init_expr
            } else {
                parsed_semi_colon = true;   // LoopDecl ate it up
            }
            tree->op.u2.sym_handle = cstmt_state().BlockStack->sym_list;
        } else {
            CheckStmtExpr();        // init_expr
        }
    }
    if( !parsed_semi_colon ) {
        MustRecog( T_SEMI_COLON );
    }
    NewLoop();
    cstmt_state().BlockStack->block_type = T_FOR;
    if( cvars_state().CurToken != T_SEMI_COLON ) {
        cstmt_state().BlockStack->break_label = NextLabel();
        if( !JumpFalse( Expr(), cstmt_state().BlockStack->break_label ) ) {
            cstmt_state().BlockStack->break_label = 0;
        }
    }
    MustRecog( T_SEMI_COLON );
    cstmt_state().BlockStack->inc_var = NULL;
    if( cvars_state().CurToken != T_RIGHT_PAREN ) {
        cstmt_state().BlockStack->inc_var = Expr();                   // save this
        if( cvars_state().CompFlags.meaningless_stmt ) {
            CheckUseful();
        }
    }
    MustRecog( T_RIGHT_PAREN );
}


static void EndForStmt( void )
{
    DropContinueLabel();
    if( cstmt_state().BlockStack->inc_var ) {
        AddStmt( cstmt_state().BlockStack->inc_var );
    }
    Jump( cstmt_state().BlockStack->top_label );
}


static void StmtExpr( void )
{
    CheckStmtExpr();
    if( ExpectingToken( T_SEMI_COLON ) ) {
        NextToken();
    }
}


static void AddCaseLabel( signed_64 value )
{
    CASEPTR         ce;
    CASEPTR         prev_ce;
    CASEPTR         new_ce;
    signed_64       old_value;
    signed_64       converted_value;
    TREEPTR         tree;
    char            buffer[12];

    prev_ce = NULL;
#if 0
    leaf.u.ulong_konst = value;
    leaf.data_type = SwitchStack->case_type;
    SetLeafType( &leaf, 1 );
//    converted_value = value & SwitchStack->case_mask;
    converted_value = leaf.u.ulong_konst;
#else
    converted_value = value;
#endif
    old_value = converted_value;
    U64Inc( old_value ); /* make old_value different */
    for( ce = cstmt_state().SwitchStack->case_list; ce != NULL; ce = ce->next_case ) {
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
        CErr2p( ERR_DUPLICATE_CASE_VALUE, buffer );
    } else {
        new_ce = (CASEPTR)CMemAlloc( sizeof( CASEDEFN ) );
        new_ce->value = converted_value;
        if( prev_ce == NULL ) {
            new_ce->next_case = cstmt_state().SwitchStack->case_list;
            cstmt_state().SwitchStack->case_list = new_ce;
        } else {
            prev_ce->next_case = new_ce;
            new_ce->next_case = ce;
        }
        /* Check if the previous statement was a 'case'. If so, reuse the label, as generating
         * too many labels seriously slows down code generation.
         */
        if( prev_ce
          && cstmt_state().LastStmt->op.opr == OPR_STMT
          && cstmt_state().LastStmt->right->op.opr == OPR_CASE ) {
            new_ce->label = cstmt_state().SwitchStack->last_case_label;
            new_ce->gen_label = false;
        } else {
            new_ce->label = NextLabel();
            new_ce->gen_label = true;
        }
        cstmt_state().SwitchStack->number_of_cases++;
        if( I64Cmp( &converted_value, &cstmt_state().SwitchStack->low_value ) < 0 ) {
            cstmt_state().SwitchStack->low_value = converted_value;
        }
        if( I64Cmp( &converted_value, &cstmt_state().SwitchStack->high_value ) > 0 ) {
            cstmt_state().SwitchStack->high_value = converted_value;
        }
        cstmt_state().SwitchStack->last_case_label = new_ce->label;
        tree = LeafNode( OPR_CASE );
        tree->op.u2.case_info = new_ce;
        AddStmt( tree );
    }
}


static void CaseStmt( void )
{
    const_val   val;

    NextToken();
    if( cstmt_state().SwitchStack ) {
        if( ConstExprAndType( &val ) ) {
            AddCaseLabel( val.value );
        }
        MustRecog( T_COLON );
        if( cvars_state().CurToken == T_RIGHT_BRACE ) {
            CErr1( ERR_STMT_REQUIRED_AFTER_CASE );
        }
    } else {
        CErr1( ERR_MISPLACED_CASE );
        ConstExprAndType( &val );        /* grab constant expression */
        MustRecog( T_COLON );
    }
}


#ifdef __SEH__
static void MarkTryVolatile( SYM_HANDLE sym_handle )
{
    SYM_ENTRY   sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym.handle ) {
        SymGet( &sym, sym_handle );
        sym.flags |= SYM_TRY_VOLATILE; //force into memory
        SymReplace( &sym, sym_handle );
    }
}


static void TryStmt( void )
{
    TREEPTR     tree;

    MarkTryVolatile( cvars_state().CurFunc->u.func.parms );
    MarkTryVolatile( cvars_state().CurFunc->u.func.locals );
    cvars_state().CurFuncNode->op.u2.func.flags |=  FUNC_USES_SEH;
    cvars_state().CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
    cvars_state().CurToken = T__TRY;
    StartNewBlock();
    NextToken();
    cstmt_state().BlockStack->parent_index = cvars_state().TryScope;
    ++cvars_state().TryCount;
    cstmt_state().BlockStack->try_index = cvars_state().TryCount;
    cvars_state().TryScope = cvars_state().TryCount;
    tree = LeafNode( OPR_TRY );
    tree->op.u2.st.u.try_index = 0;
    tree->op.u2.st.parent_scope = cvars_state().TryCount;
    AddStmt( tree );
}


static SYM_HANDLE DummyTrySymbol( void )
{
    SYM_ENTRY   sym;
    SYM_HANDLE  sym_handle;

    sym_handle = MakeNewDotSym( &sym, 'T', GetType( TYP_VOID ), SC_STATIC );
    SymReplace( &sym, sym_handle );
    return( sym_handle );
}


static bool EndTry( void )
{
    tryindex_t  parent_scope;
    TREEPTR     expr;
    TREEPTR     func;
    TREEPTR     tree;
    TYPEPTR     typ;
    DATA_TYPE   expr_type;

    DropBreakLabel();           /* _leave jumps to this label */
    parent_scope = cstmt_state().BlockStack->parent_index;
    tree = LeafNode( OPR_TRY );
    tree->op.u2.st.u.try_index = cstmt_state().BlockStack->try_index;
    tree->op.u2.st.parent_scope = parent_scope;
    AddStmt( tree );
    if( (cvars_state().CurToken == T__EXCEPT)
      || (cvars_state().CurToken == T___EXCEPT) ) {
        NextToken();
        cstmt_state().BlockStack->block_type = T__EXCEPT;
        cstmt_state().BlockStack->break_label = NextLabel();
        Jump( cstmt_state().BlockStack->break_label );
        cvars_state().DeadCode = 0;
        tree = LeafNode( OPR_EXCEPT );
        tree->op.u2.st.u.try_sym_handle = DummyTrySymbol();
        tree->op.u2.st.parent_scope = parent_scope;
        AddStmt( tree );
        cvars_state().CompFlags.exception_filter_expr = true;
        expr = RValue( BracketExpr() );
        cvars_state().CompFlags.exception_filter_expr = false;
        cvars_state().CompFlags.exception_handler = true;
        typ = TypeOf( expr );
        expr_type = DataTypeOf( typ );
        if( expr_type != TYP_VOID ) {
            if( expr_type > TYP_ULONG ) {
                CErr1( ERR_EXPR_MUST_BE_INTEGRAL );
            }
        }
        func = VarLeaf( SymGetPtr( cvars_state().SymExcept ), cvars_state().SymExcept );
        func->op.opr = OPR_FUNCNAME;
        expr = ExprNode( NULL, OPR_PARM, expr );
        expr->u.expr_type = typ;
        expr->op.u2.result_type = typ;
        tree = ExprNode( func, OPR_CALL, expr );
        tree->u.expr_type = GetType( TYP_VOID );
        AddStmt( tree );
        return( true );
    } else if( (cvars_state().CurToken == T__FINALLY)
      || (cvars_state().CurToken == T___FINALLY) ) {
        cvars_state().CompFlags.in_finally_block = true;
        NextToken();
        cstmt_state().BlockStack->block_type = T__FINALLY;
        cvars_state().DeadCode = 0;
        tree = LeafNode( OPR_FINALLY );
        tree->op.u2.st.u.try_sym_handle = DummyTrySymbol();
        tree->op.u2.st.parent_scope = parent_scope;
        AddStmt( tree );
        return( true );
    }
    return( false );
}
#endif


static void PopBlock( void )
{
    BLOCKPTR    block;
    TREEPTR     tree;

    if( cstmt_state().BlockStack->gen_endblock ) {
        tree = LeafNode( OPR_ENDBLOCK );
        tree->op.u2.sym_handle = cstmt_state().BlockStack->sym_list;
        AddStmt( tree );
    }
    if( cstmt_state().BlockStack->sym_list != SYM_NULL ) {
        AddSymList( cstmt_state().BlockStack->sym_list );
    }
    block = cstmt_state().BlockStack;
    cstmt_state().LoopStack = block->prev_loop;
    cstmt_state().BlockStack = block->prev_block;
    CMemFree( block );
}


static void SwitchStmt( void )
{
    SWITCHPTR   sw;
    TREEPTR     tree;
    TYPEPTR     typ;

    StartNewBlock();
    NextToken();
    sw = (SWITCHPTR)CMemAlloc( sizeof( SWITCHDEFN ) );
    sw->prev_switch = cstmt_state().SwitchStack;
    Set64Val1m( sw->low_value );
    Set64ValZero( sw->high_value );
    cstmt_state().SwitchStack = sw;
    tree = RValue( BracketExpr() );
    typ = TypeOf( tree );
    SKIP_ENUM( typ );
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
        CErr1( ERR_INVALID_TYPE_FOR_SWITCH );
    }
    tree = ExprNode( NULL, OPR_SWITCH, tree );
    tree->op.u2.switch_info = sw;
    AddStmt( tree );
}


static void EndSwitch( void )
{
    SWITCHPTR   sw;
//    CASEPTR   ce;

    sw = cstmt_state().SwitchStack;
    cstmt_state().SwitchStack = sw->prev_switch;
    if( sw->default_label == 0 ) {
        if( cstmt_state().BlockStack->break_label == 0 ) {
            sw->default_label = NextLabel();
            DropLabel( sw->default_label );
        } else {
            sw->default_label = cstmt_state().BlockStack->break_label;
        }
    }
//    if( sw->case_list == NULL ) {
//        CWarn1( ERR_EMPTY_SWITCH );
//    }
#if 0
    for( ; (ce = sw->case_list) != NULL; ) {
        sw->case_list = ce->next_case;
        CMemFree( ce );
    }
    CMemFree( sw );
#endif
}


static void EndOfStmt( void )
{
    do {
        switch( cstmt_state().BlockStack->block_type ) {
        case T_LEFT_BRACE:
            EndBlock();
            break;
        case T_IF:
            if( cvars_state().CurToken == T_ELSE ) {
                ElseStmt();
                return;
            }
            DropBreakLabel();
            break;
        case T_ELSE:
            DropBreakLabel();
            break;
        case T_WHILE:
            DropContinueLabel();
            Jump( cstmt_state().BlockStack->top_label );
            --cvars_state().LoopDepth;
            DropBreakLabel();
            break;
        case T_FOR:
            EndForStmt();
            --cvars_state().LoopDepth;
            DropBreakLabel();
            if( cvars_state().CompVars.cstd > STD_C89 ) {
                EndBlock();     /* Terminate the scope introduced by 'for' */
                PopBlock();
            }
            break;
        case T_DO:
            DropContinueLabel();
            MustRecog( T_WHILE );
            cvars_state().SrcLoc = cvars_state().TokenLoc;
            JumpTrue( BracketExpr(), cstmt_state().BlockStack->top_label );
            MustRecog( T_SEMI_COLON );
            cvars_state().SrcLoc = cvars_state().TokenLoc;
            --cvars_state().LoopDepth;
            DropBreakLabel();
            break;
        case T_SWITCH:
            EndSwitch();
            DropBreakLabel();
            break;
#ifdef __SEH__
        case T__TRY:
        case T___TRY:
            if( EndTry() )
                return;
            break;
        case T__EXCEPT:
        case T___EXCEPT:
            DropBreakLabel();
            cvars_state().TryScope = cstmt_state().BlockStack->parent_index;
            cvars_state().CompFlags.exception_handler = false;
            break;
        case T__FINALLY:
        case T___FINALLY:
            AddStmt( LeafNode( OPR_END_FINALLY ) );
            cvars_state().CompFlags.in_finally_block = false;
            cvars_state().TryScope = cstmt_state().BlockStack->parent_index;
            break;
#endif
        }
        PopBlock();
        if( cstmt_state().BlockStack == NULL ) {
            break;
        }
    } while( cstmt_state().BlockStack->block_type != T_LEFT_BRACE );
}


static bool IsDeclarator( TOKEN token )
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
    if( (cvars_state().CurToken == T_ID)
      || (cvars_state().CurToken == T_SAVED_ID) ) {
        if( cvars_state().CurToken == T_ID ) {
            sym_handle = SymLookTypedef( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer, &sym );
        } else {    /* T_SAVED_ID */
            sym_handle = SymLookTypedef( CalcHashID( cvars_state().SavedId ), cvars_state().SavedId, &sym );
        }
        if( sym_handle != SYM_NULL
          && sym.attribs.stg_class == SC_TYPEDEF ) {
            return( true );
        }
    }
    return( false );
}


static void FixupC99MainReturn( SYM_HANDLE func_result_handle, struct return_info *info )
{
    TREEPTR             tree;
    TYPEPTR             main_type;

    /* In C99 mode, return statement need not be explicit for main()... */
    main_type = cvars_state().CurFunc->sym_type->object;
    SKIP_TYPEDEFS( main_type );
    /* ... as long as return type is compatible with int */
    if( main_type->decl_type == TYP_INT ) {
        tree = IntLeaf( 0 );    /* zero is the default return value */
        tree = ExprNode( NULL, OPR_RETURN, tree );
        tree->u.expr_type = main_type;
        tree->op.u2.sym_handle = func_result_handle;
        AddStmt( tree );
        info->with_expr = true;
    }
}


void Statement( void )
{
    LABEL_INDEX         end_of_func_label;
    SYM_HANDLE          func_result_handle;
    TREEPTR             tree;
    bool                return_at_outer_level;
    bool                skip_to_next_token;
    bool                declaration_allowed;
    struct return_info  return_info;
    SYM_ENTRY           sym;

#ifdef DEVBUILD
    if( DebugFlag >= 1 ) {
        printf( "***** line %u, func=%s\n", TokenLoc.line, CurFunc->name );
        PrintStats();
    }
#endif
    ++cvars_state().FuncCount;
    return_info.with = RETURN_WITH_NONE; /* indicate no return statements */
    return_info.with_expr = false;
    cvars_state().CompFlags.label_dropped = false;
    cvars_state().CompFlags.addr_of_auto_taken = false;
    end_of_func_label = 0;
    return_at_outer_level = false;
    declaration_allowed = false;
    cvars_state().DeadCode = 0;
    cvars_state().LoopDepth = 0;
    cvars_state().LabelIndex = 0;
    cstmt_state().BlockStack = NULL;
    cstmt_state().LoopStack = NULL;
    cstmt_state().SwitchStack = NULL;
#ifdef __SEH__
    cvars_state().TryCount = TRYSCOPE_NONE;
    cvars_state().TryScope = TRYSCOPE_NONE;
#endif
    StartNewBlock();
    NextToken();                        // skip over {
    ++cvars_state().SymLevel;
    tree = LeafNode( OPR_LABELCOUNT );
    AddStmt( tree );
    if( GrabLabels() == 0 ) {
        GetLocalVarDecls();
    }
    func_result_handle = MakeNewDotSym( &sym, 'R', cvars_state().CurFunc->sym_type->object, SC_AUTO );
    sym.flags |= SYM_FUNC_RETURN_VAR;
    SymReplace( &sym, func_result_handle );
    for( ;; ) {
        cvars_state().CompFlags.pending_dead_code = false;
        if( GrabLabels() == 0
          && declaration_allowed
          && IsDeclarator( cvars_state().CurToken ) ) {
            GetLocalVarDecls();
        }
        if( cvars_state().CompVars.cstd > STD_C89 ) {
            declaration_allowed = true;
        }
        skip_to_next_token = false;
        switch( cvars_state().CurToken ) {
        case T_IF:
            StartNewBlock();
            NextToken();
            cstmt_state().BlockStack->break_label = NextLabel();
            JumpFalse( BracketExpr(), cstmt_state().BlockStack->break_label );
            /* only issue msg if ';' is on same line as 'if' */
            if( cvars_state().CurToken == T_SEMI_COLON
              && cvars_state().SrcLoc.line == cvars_state().TokenLoc.line
              && cvars_state().SrcLoc.fno == cvars_state().TokenLoc.fno ) {
                SetErrLoc( &cvars_state().TokenLoc );
                NextToken();    /* look ahead for else keyword */
                if( cvars_state().CurToken != T_ELSE ) {
                    CheckUseful();
                }
                InitErrLoc();
                break;
            }
            declaration_allowed = false;
            continue;
        case T_ELSE:
            /* misplaced else, no previous if */
            CErr1( ERR_ELSE_WITHOUT_IF );
            NextToken();
            continue;
        case T_WHILE:
            NewLoop();
            NextToken();
            cstmt_state().BlockStack->break_label = NextLabel();
            if( !JumpFalse( BracketExpr(), cstmt_state().BlockStack->break_label ) ) {
                cstmt_state().BlockStack->break_label = 0;
            }
            if( cvars_state().CurToken == T_SEMI_COLON ) {
                if( !cvars_state().CompFlags.useful_side_effect ) {
                    CWarn1( ERR_MEANINGLESS );
                }
            }
            declaration_allowed = false;
            continue;
        case T_DO:
            NewLoop();
            NextToken();
            if( cvars_state().CurToken == T_RIGHT_BRACE ) {
                CErr1( ERR_STMT_REQUIRED_AFTER_DO );
                break;
            }
            declaration_allowed = false;
            continue;
        case T_FOR:
            ForStmt();
            declaration_allowed = false;
            continue;
        case T_SWITCH:
            SwitchStmt();
            cvars_state().DeadCode = 1;
            declaration_allowed = false;
            continue;
        case T_CASE:
            cvars_state().DeadCode = 0;
            CaseStmt();
            declaration_allowed = false;
            continue;
        case T_DEFAULT:
            DefaultStmt();
            declaration_allowed = false;
            continue;
        case T_BREAK:
            BreakStmt();
            cvars_state().DeadCode = 1;
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_CONTINUE:
            ContinueStmt();
            cvars_state().DeadCode = 1;
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
#ifdef __SEH__
        case T__LEAVE:
        case T___LEAVE:
            LeaveStmt();
            cvars_state().DeadCode = 1;
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
#endif
        case T_RETURN:
            ReturnStmt( func_result_handle, &return_info );
            if( cstmt_state().BlockStack->prev_block == NULL ) {
                return_at_outer_level = true;
            }
            MustRecog( T_SEMI_COLON );
            if( cvars_state().SymLevel != 1
              || cvars_state().CurToken != T_RIGHT_BRACE
              || cstmt_state().BlockStack->block_type != T_LEFT_BRACE ) {
                if( end_of_func_label == 0 ) {
                    end_of_func_label = NextLabel();
                }
                Jump( end_of_func_label );
            }
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_GOTO:
            GotoStmt();
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        case T_SEMI_COLON:
            NextToken();
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE ) {
                if( cvars_state().CurToken == T_ELSE ) {
                    declaration_allowed = false;
                }
                break;
            }
            continue;
        case T_LEFT_BRACE:
            LeftBrace();
            continue;
        case T_RIGHT_BRACE:
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE ) {
                CErr1( ERR_MISPLACED_RIGHT_BRACE );
            }
            if( cstmt_state().BlockStack->prev_block == NULL ) {
                skip_to_next_token = true;
            } else {
                NextToken();
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
            if( cvars_state().CompVars.cstd < STD_C99 ) {
                CErr1( ERR_MISSING_RIGHT_BRACE );
            } else {
                CErr1( ERR_UNEXPECTED_DECLARATION );
            }
            break;
        case T_EOF:
            CErr1( ERR_MISSING_RIGHT_BRACE );
            break;
#ifdef __SEH__
        case T__TRY:
        case T___TRY:
            TryStmt();
            continue;
#endif
        case T___ASM:
            AsmStmt();
            continue;
        default:
            if( cvars_state().DeadCode == 1 ) {
                DeadMsg();
            }
            StmtExpr();
            if( cstmt_state().BlockStack->block_type != T_LEFT_BRACE )
                break;
            continue;
        }
        EndOfStmt();
        if( cstmt_state().BlockStack == NULL )
            break;
        if( skip_to_next_token ) {
            NextToken();
        }
    }
    /* C99 has special semantics for return value of main() */
    if( ( cvars_state().CompVars.cstd > STD_C89 )
      && strcmp( cvars_state().CurFunc->name, "main" ) == 0 ) {
        if( !return_at_outer_level ) {
            FixupC99MainReturn( func_result_handle, &return_info );
            return_at_outer_level = true;
        }
    }
    if( !return_info.with_expr ) {   /* no return values present */
        if( cvars_state().DeadCode == 0
          && !cvars_state().CurFunc->attribs.naked ) {
            CheckRetValue();
        }
    } else if( !return_at_outer_level ) {
        if( cvars_state().DeadCode == 0
          && !cvars_state().CurFunc->attribs.naked ) {
            CWarn2p( ERR_MISSING_RETURN_VALUE, cvars_state().CurFunc->name );
        }
    }
    if( end_of_func_label != 0 ) {
        DropLabel( end_of_func_label );
    }
    cvars_state().DeadCode = 0;
    tree->op.u2.label_count = cvars_state().LabelIndex;
    tree = LeafNode( OPR_FUNCEND );
    if( !return_info.with_expr ) {
        tree->u.expr_type = NULL;
        tree->op.u2.sym_handle = SYM_NULL;
    } else {
        tree->op.u2.sym_handle = func_result_handle;
        SetFuncReturnNode( tree );
    }
    AddStmt( tree );
    AddSymList( cvars_state().CurFunc->u.func.locals );
    cvars_state().SrcLoc = cvars_state().TokenLoc;
    FreeLabels();
    if( skip_to_next_token ) {
        NextToken();
    }
    if( cvars_state().CompFlags.generate_prototypes ) {
        if( cvars_state().DefFile == NULL ) {
            OpenDefFile();
        }
        if( cvars_state().DefFile != NULL
          && cvars_state().CurFunc->attribs.stg_class == SC_NONE ) {
            /* function exported */
            DumpFuncDefn();
        }
    }

    if( TOGGLE( inline ) ) {
        if( cvars_state().Inline_Threshold < cvars_state().NodeCount ) {
            cvars_state().CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
        }
    }
    if( VarParm( cvars_state().CurFunc ) ) {
        cvars_state().CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
    }
    cvars_state().NodeCount = 0;
    if( cvars_state().CompFlags.addr_of_auto_taken ) {
        cvars_state().CurFunc->flags &= ~ SYM_OK_TO_RECURSE;
    }
}
