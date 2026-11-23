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
* Description:  data directive DB,DW,DD,... and structure processing
*
****************************************************************************/


#include "asmglob.h"
#include "asmexpnd.h"
#include "tbyte.h"
#include "asmfixup.h"

#include "asmrun.hpp"



static token_idx dup_array( AsmRun& assembler, asm_sym_handle sym, asm_sym_handle struct_sym, token_buffer *tokbuf, token_idx start_pos, unsigned no_of_bytes );

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct AsmdataState {
    bool More_Array_Element = false;
    unsigned Last_Element_Size;
};
namespace {

}  // namespace



/* data initialization stuff */

static void output_float( AsmRun& assembler, token_buffer *tokbuf, token_idx index, unsigned no_of_bytes, bool negative )
/****************************************************************************************************/
{
    double              double_value;
    float               float_value;
    const char          *char_ptr = NULL;
    unsigned            count;
    TB_LD               tbyte;

    if( no_of_bytes == BYTE_10 ) {
        char_ptr = (char *)strtotb( assembler, tokbuf->tokens[index].string_ptr, &tbyte, negative );
    } else {
        double_value = strtod( tokbuf->tokens[index].string_ptr, NULL );
        if( negative )
            double_value = -double_value;
        switch( no_of_bytes ) {
        case BYTE_1:
        case BYTE_2:
        default:
            char_ptr = (char *)&tokbuf->tokens[index].u.value;
            break;
        case BYTE_4:
            float_value = (float)double_value;
            char_ptr = (char *)&float_value;
            break;
        case BYTE_8:
            char_ptr = (char *)&double_value;
            break;
        }
    }
    for( count = 0; count < no_of_bytes; ++count ) {
        AsmDataByte( *char_ptr++ );
    }
}


static token_idx array_element( AsmRun& assembler, asm_sym_handle sym, asm_sym_handle struct_sym, token_buffer *tokbuf, token_idx start_pos, unsigned no_of_bytes )
/***********************************************************************************************************************************
 * - parse an array and initialize the number;
 * - call by dup_array() only;
 */
{
    token_idx           cur_pos = start_pos;
    unsigned            count;
    bool                negative = false;


    for( cur_pos = start_pos;
        ( cur_pos < tokbuf->count ) && ( tokbuf->tokens[cur_pos].class_ != TC_FINAL );
        cur_pos++ ) {
        if(( cur_pos == tokbuf->count - 1 )
            && ( tokbuf->tokens[cur_pos].class_ == TC_CL_BRACKET ))
            break;
        switch( tokbuf->tokens[cur_pos].class_ ) {
        case TC_QUESTION_MARK:
            if( cur_pos != start_pos ) {
                if( tokbuf->tokens[cur_pos - 1].class_ != TC_COMMA ) {
                    AsmError( *assembler.front, EXPECTING_COMMA );
                    return( INVALID_IDX );
                }
            }
            for( count = 0; count < no_of_bytes; ++count ) {
                AsmDataByte( 0 );
            }
            break;
        case TC_MINUS:
            switch( tokbuf->tokens[cur_pos+1].class_ ) {
            case TC_NUM:
                tokbuf->tokens[cur_pos+1].u.value = -tokbuf->tokens[cur_pos+1].u.value;
                tokbuf->tokens[cur_pos].class_ = TC_PLUS;
                break;
            case TC_FLOAT:
                negative = true;
                break;
            default:
                AsmError( *assembler.front, EXPECTING_NUMBER );
                return( INVALID_IDX );
            }
            break;
        case TC_PLUS:
        case TC_DOT:
            break; // go around again
        case TC_NUM:
        case TC_FLOAT:
          {
            const char  *char_ptr;

            if( tokbuf->tokens[cur_pos+1].class_ == TC_RES_ID
              && tokbuf->tokens[cur_pos+1].u.token == T_DUP ) {
                cur_pos = dup_array( assembler, sym, struct_sym, tokbuf, cur_pos, no_of_bytes );
                if( ISINVALID_IDX( cur_pos ) )
                    return( INVALID_IDX );
                break;
            }

            if( cur_pos != start_pos ) {
                switch( tokbuf->tokens[cur_pos - 1].class_ ) {
                case TC_COMMA:
                    break;
                case TC_PLUS:
                case TC_MINUS:
                    if( tokbuf->tokens[cur_pos - 2].class_ == TC_COMMA )
                        break;
                    if( cur_pos - 1 == start_pos )
                        break;
                    /* fall through */
                default:
                    AsmError( *assembler.front, EXPECTING_COMMA );
                    return( INVALID_IDX );
                }
            }
            if( tokbuf->tokens[cur_pos].class_ == TC_FLOAT ) {
                output_float( assembler, tokbuf, cur_pos, no_of_bytes, negative );
                negative = false;
                break;
            }
                char_ptr = (char *)tokbuf->tokens[cur_pos].u.bytes;
                for( count = 0; count < no_of_bytes; ++count ) {
                    AsmDataByte( *char_ptr++ );
                }
            break;
          }
        case TC_COMMA:
            if( cur_pos != start_pos ) {
                if( tokbuf->tokens[cur_pos - 1].class_ == TC_COMMA ) {
                    AsmError( *assembler.front, EXPECTING_NUMBER );
                    return( INVALID_IDX );
                    /********SHOULD WE DO IT THIS WAY?*********
                    for( count = 0; count < no_of_bytes; count++ ) {
                        AsmDataByte( 0x00 );
                    }
                    ******************************************/
                }
            } else {
                AsmError( *assembler.front, EXPECTING_NUMBER );
                return( INVALID_IDX );
            }
            if( cur_pos == ( tokbuf->count - 1 ) ) {
                assembler.asmdata->More_Array_Element = true;
                assembler.asmdata->Last_Element_Size = no_of_bytes;
            }
            break;
        case TC_STRING:
        case TC_STRING_SQUOTE:
        case TC_STRING_DQUOTE:
        case TC_STRING_ANGLE:
        case TC_STRING_BRACE:
          {
            const char  *char_ptr;
            unsigned    i;
            int         direction;

            if( no_of_bytes != 1 ) {
                if( (unsigned)tokbuf->tokens[cur_pos].u.value > no_of_bytes ) {
                    AsmError( *assembler.front, INITIALIZER_OUT_OF_RANGE );
                    return( INVALID_IDX );
                }
            }
            /*
             * anything bigger than a byte must be stored in little-endian
             * format -- LSB first
             */
            direction = ( no_of_bytes > 1 ) ? -1 : 1;
                count = tokbuf->tokens[cur_pos].u.value;
                char_ptr = tokbuf->tokens[cur_pos].string_ptr;
                if( direction == -1 )
                    char_ptr += strlen( char_ptr ) - 1;
                for( i = 0; i < count; ++i ) {
                    AsmDataByte( *char_ptr );
                    char_ptr += direction;
                }
                /*
                 * no_of_bytes is size of array element
                 * if the number of initialization bytes is less
                 * than this number, then it is extended by zeros
                 * up to no_of_bytes
                 */
                for( ; i < no_of_bytes; ++i ) {
                    AsmDataByte( 0 );
                }
            break;
          }
        case TC_ID:
          {
            token_idx           i;
            fixup_types         fixup_type = static_cast<fixup_types>(0);
            asm_sym_handle      init_sym;
            const char          *ptr;
            long                data = 0;
            asmfixup            *fixup;

            /* temporary test .. if this works, combine code for id & resid */

            init_sym = AsmLookup( assembler, tokbuf->tokens[cur_pos].string_ptr );

            if( init_sym == NULL )
                return( INVALID_IDX );

                switch( no_of_bytes ) {
                case 1:
                    AsmError( *assembler.front, OFFSET_TOO_SMALL ); // fixme
                    return( INVALID_IDX );
                case 2:
                    fixup_type = FIX_OFF16;
                    break;
                case 4:
                    if( assembler.asmdefs.Code->use32 ) {
                        fixup_type = FIX_OFF32;
                    } else {
                        fixup_type = FIX_PTR16;
                    }
                    break;
                case 6:
                    // fixme -- this needs work .... check USE32, etc
                    fixup_type = FIX_PTR32;
                    assembler.asmdefs.Code->info.opnd_type[OPND1] = OP_J48;
                    break;
                default:
                    AsmError( *assembler.front, NOT_IMPLEMENTED );
                    return( INVALID_IDX );
                }
            fixup = AddFixup( assembler, init_sym, fixup_type, OPTJ_NONE );
            //          if( fixup == NULL ) return( INVALID_IDX );
            // fixme
            assembler.asmfixup.InsFixups[OPND1] = fixup;
            data += fixup->u_offset;

            for( cur_pos++;
                ( cur_pos < tokbuf->count ) && ( tokbuf->tokens[cur_pos].class_ != TC_FINAL )
                    && ( tokbuf->tokens[cur_pos].class_ != TC_COMMA )
                    && ( tokbuf->tokens[cur_pos].class_ != TC_CL_BRACKET );
                cur_pos++ ) {
                switch( tokbuf->tokens[cur_pos].class_ ) {
                case TC_PLUS:
                case TC_DOT:
                case TC_OP_SQ_BRACKET:
                    break;
                case TC_MINUS:
                    if( tokbuf->tokens[cur_pos+1].class_ != TC_NUM ) {
                        AsmError( *assembler.front, EXPECTING_NUMBER );
                        return( INVALID_IDX );
                    }
                    tokbuf->tokens[cur_pos+1].u.value = -tokbuf->tokens[cur_pos+1].u.value;
                    break;
                case TC_NUM:
                    data += tokbuf->tokens[cur_pos].u.value;
                }
            }

            /* now actually output the data */
            ptr = (char *)&data;
                /*
                 * only output up to 4 bytes of offset (segment is on fixup)
                 */
                for( i = 0; i < no_of_bytes; i++ ) {
                    if( i < 4 ) {
                        /* copy offset */
                        AsmDataByte( *ptr++ );
                    } else {
                        /* leave space for segment */
                        AsmDataByte( 0 );
                    }
                }
            // set position back to main loop worked correctly
            cur_pos--;
            break;
          }
        case TC_UNARY_OPERATOR:
          {
            token_idx           i;
            fixup_types         fixup_type = static_cast<fixup_types>(0);
            token_idx           seg_off_operator_loc = 0;
            asm_sym_handle      init_sym;
            const char          *ptr;
            long                data = 0;
            asmfixup            *fixup;

            if( tokbuf->tokens[cur_pos].u.token == T_OFFSET ||
                tokbuf->tokens[cur_pos].u.token == T_OFFSET32 ||
                tokbuf->tokens[cur_pos].u.token == T_SEG ) {
                // see asmins.h about T_SEG
                if( no_of_bytes < 2 ) {
                    AsmError( *assembler.front, OFFSET_TOO_SMALL );
                    return( INVALID_IDX );
                }
            }
            seg_off_operator_loc = cur_pos;
            if( cur_pos + 1 < tokbuf->count ) {
                if( tokbuf->tokens[++cur_pos].class_ == TC_ID ) {
                    init_sym = AsmLookup( assembler, tokbuf->tokens[cur_pos].string_ptr );
                    if( init_sym == NULL )
                        return( INVALID_IDX );

                    if( tokbuf->tokens[seg_off_operator_loc].u.token == T_OFFSET
                      || tokbuf->tokens[seg_off_operator_loc].u.token == T_OFFSET32 ) {
                        if( init_sym->state == SYM_STACK ) {
                            AsmError( *assembler.front, CANNOT_OFFSET_AUTO );
                            return( INVALID_IDX );
                        }
                        switch( no_of_bytes ) {
                        case 1:
                            AsmError( *assembler.front, OFFSET_TOO_SMALL ); // fixme
                            return( INVALID_IDX );
                        case 2:
                            fixup_type = FIX_OFF16;
                            break;
                        case 4:
                            fixup_type = FIX_OFF32;
                            break;
                        default:
                            AsmError( *assembler.front, NOT_IMPLEMENTED );
                            return( INVALID_IDX );
                        }
                    } else if( tokbuf->tokens[seg_off_operator_loc].u.token == T_SEG ) {
                        if( init_sym->state == SYM_STACK ) {
                            AsmError( *assembler.front, CANNOT_SEG_AUTO );
                        }
                        fixup_type = FIX_SEG;
                    }

                    switch( tokbuf->tokens[seg_off_operator_loc].u.token ) {
                    case T_OFFSET32:
                    case T_OFFSET:
                    case T_SEG:
                        fixup = AddFixup( assembler, init_sym, fixup_type, OPTJ_NONE );
                        assembler.asmfixup.InsFixups[OPND1] = fixup;
                        if( tokbuf->tokens[seg_off_operator_loc].u.token == T_OFFSET
                          || tokbuf->tokens[seg_off_operator_loc].u.token == T_OFFSET32 ) {
                            data += fixup->u_offset;
                        }
                        break;
                    default:
                        AsmError( *assembler.front, SYNTAX_ERROR );
                        return( INVALID_IDX );
                    }
                    for( cur_pos++;
                        ( cur_pos < tokbuf->count ) && ( tokbuf->tokens[cur_pos].class_ != TC_FINAL)
                        && ( tokbuf->tokens[cur_pos].class_ != TC_COMMA );
                        cur_pos++ ) {
                        switch( tokbuf->tokens[cur_pos].class_ ) {
                        case TC_PLUS:
                        case TC_DOT:
                        case TC_OP_SQ_BRACKET:
                            break;
                        case TC_MINUS:
                            if( tokbuf->tokens[cur_pos+1].class_ != TC_NUM ) {
                                AsmError( *assembler.front, EXPECTING_NUMBER );
                                return( INVALID_IDX );
                            }
                            tokbuf->tokens[cur_pos+1].u.value = -tokbuf->tokens[cur_pos+1].u.value;
                            break;
                        case TC_NUM:
                            data += tokbuf->tokens[cur_pos].u.value;
                        }
                    }

                    /* now actually output the data */
                    ptr = (char *)&data;
                        for( i = 0; i < no_of_bytes; i++ ) {
                            AsmDataByte( *ptr );
                            ptr++;
                        }
                } else {
                    AsmError( *assembler.front, SYNTAX_ERROR );
                    return( INVALID_IDX );
                }
            }
            // set position back to main loop worked correctly
            cur_pos--;
            // AsmError( NOT_IMPLEMENTED );
            break;
          }
        case TC_CL_BRACKET:
            return( cur_pos );
        default:
            AsmError( *assembler.front, EXPECTING_NUMBER );
            return( INVALID_IDX );
        }
    }
    return( cur_pos );
}

static token_idx dup_array( AsmRun& assembler, asm_sym_handle sym, asm_sym_handle struct_sym, token_buffer *tokbuf, token_idx start_pos, unsigned no_of_bytes )
/*******************************************************************************************************************************
 * parse array with DUP operator;
 */
{
    token_idx           cur_pos;
    token_idx           returned_pos = 0;
    unsigned            count;

    ExpandTheWorld( assembler, tokbuf, start_pos, false, true );
    for( cur_pos = start_pos; cur_pos + 2 < tokbuf->count; ) {
        if(( tokbuf->tokens[cur_pos + 1].class_ == TC_RES_ID )
          && ( tokbuf->tokens[cur_pos + 1].u.token == T_DUP )) {
            if( tokbuf->tokens[cur_pos].class_ != TC_NUM ) {
                AsmError( *assembler.front, SYNTAX_ERROR );
                return( INVALID_IDX );
            }
            count = tokbuf->tokens[cur_pos].u.value;
            cur_pos += 2;
            if( tokbuf->tokens[cur_pos].class_ != TC_OP_BRACKET ) {
                AsmError( *assembler.front, SYNTAX_ERROR );
                return( INVALID_IDX );
            }

            cur_pos++;
            if( count == 0 ) {
                int     level;
                /* zero count is valid, needs special processing */
                for( level = 0; tokbuf->tokens[cur_pos].class_ != TC_FINAL; cur_pos++ ) {
                    if( tokbuf->tokens[cur_pos].class_ == TC_OP_BRACKET ) {
                        level++;
                    } else if( tokbuf->tokens[cur_pos].class_ == TC_CL_BRACKET ) {
                        level--;
                    }
                    if( level < 0 ) {
                        break;
                    }
                }
                returned_pos = cur_pos;
            }
            for( ; count > 0; --count ) {
                /* in case there was a "," inside the dup */
                returned_pos = array_element( assembler, sym, struct_sym, tokbuf, cur_pos, no_of_bytes );
                if( ISINVALID_IDX( returned_pos ) ) {
                    return( INVALID_IDX );
                }
            }
            if( tokbuf->tokens[returned_pos].class_ != TC_CL_BRACKET ) {
                AsmError( *assembler.front, BRACKET_EXPECTED );
                return( INVALID_IDX );
            }
        } else {
            returned_pos = array_element( assembler, sym, struct_sym, tokbuf, cur_pos, no_of_bytes );
            if( ISINVALID_IDX( returned_pos ) )
                return( INVALID_IDX );
            if( tokbuf->tokens[returned_pos].class_ != TC_CL_BRACKET ) {
                /* array_element hit TC_FINAL so stop */
                return( returned_pos );
            }
        }
        if( tokbuf->tokens[returned_pos + 1].class_ != TC_COMMA ) {
            return( returned_pos );
        }
        cur_pos = returned_pos + 2;
    }
    return( array_element( assembler, sym, struct_sym, tokbuf, cur_pos, no_of_bytes ) );
}

bool data_init( AsmRun& assembler, token_buffer *tokbuf, token_idx sym_loc, token_idx initializer_loc )
/***********************************************************************************
 * parse data initialization assembly line;
 */
{
    unsigned        no_of_bytes;
    memtype         mem_type;
    asm_sym_handle  sym = NULL;
    asm_sym_handle  struct_sym = NULL;

    if( ISVALID_IDX( sym_loc ) ) {
        sym = AsmLookup( assembler, tokbuf->tokens[sym_loc].string_ptr );
        if( sym == NULL ) {
            return( RC_ERROR );
        }
    }
    switch( tokbuf->tokens[initializer_loc].u.token ) {
    case T_DB:
    case T_BYTE:
        mem_type = MT_BYTE;
        no_of_bytes = BYTE_1;
        break;
    case T_DW:
    case T_WORD:
        mem_type = MT_WORD;
        no_of_bytes = BYTE_2;
        break;
    case T_DD:
    case T_DWORD:
        mem_type = MT_DWORD;
        no_of_bytes = BYTE_4;
        break;
    case T_DF:                          // 20-Aug-92
    case T_FWORD:
    case T_DP:
    case T_PWORD:
        mem_type = MT_FWORD;
        no_of_bytes = BYTE_6;
        break;
    default:
        AsmError( *assembler.front, INVALID_LABEL_DEFINITION );
        return( RC_ERROR );
    }
    if( tokbuf->tokens[initializer_loc + 1].class_ == TC_FINAL ) {
        AsmError( *assembler.front, SYNTAX_ERROR );
        return( RC_ERROR );
    }


    if( assembler.asmdata->More_Array_Element ) {
        assembler.asmdata->More_Array_Element = false;
    } else if( ISVALID_IDX( sym_loc ) ) {
        if( sym->state != SYM_UNDEFINED ) {
            // redefine label
            AsmError( *assembler.front, SYMBOL_ALREADY_DEFINED );
            return( RC_ERROR );
        }
        sym->state = SYM_INTERNAL;
        sym->mem_type = mem_type;
        BackPatch( assembler, sym );
    }
    if( ISINVALID_IDX( dup_array( assembler, sym, struct_sym, tokbuf, initializer_loc + 1, no_of_bytes ) ) ) {
        return( RC_ERROR );
    }
    return( RC_OK );
}

bool NextArrayElement( AsmRun& assembler, token_buffer *tokbuf, bool *next )
{
    token_idx   rc;

    *next = false;
    if( assembler.asmdata->More_Array_Element ) {
        assembler.asmdata->More_Array_Element = false;
        rc = dup_array( assembler, NULL, NULL, tokbuf, 0, assembler.asmdata->Last_Element_Size );
        if( ISINVALID_IDX( rc ) )
            return( RC_ERROR );
        *next = true;
    }
    return( RC_OK );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
AsmdataState* asmdata_make() { return new AsmdataState(); }
void asmdata_free(AsmdataState* part) { delete part; }
