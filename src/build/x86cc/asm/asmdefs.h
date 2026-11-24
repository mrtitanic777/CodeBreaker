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

#ifndef _ASMDEFS_H_INCLUDED
#define _ASMDEFS_H_INCLUDED

inline bool IsCall( asm_token inst ) { return ( inst == T_CALL || inst == T_CALLF ); }
inline bool IsJmp( asm_token inst ) { return ( inst >= T_JA && inst <= T_JZ ); }
inline bool IsBranch( asm_token inst ) { return ( IsJmp( inst ) || IsCall( inst ) ); }
inline bool IsJmpCallF( asm_token inst ) { return ( inst == T_CALLF || inst == T_JMPF ); }
inline bool IsAnyBranch( asm_token inst ) { return ( IsBranch( inst ) || ( ( inst >= T_LOOP ) && ( inst <= T_LOOPZW ) ) ); }

#define MOD_00          0x00
#define MOD_01          0x40
#define MOD_10          0x80
#define MOD_11          0xC0

#define W_BIT           0x01
#define NOT_W_BIT       0xFE

#define ADRSIZ          0x67
#define OPSIZ           0x66
#define OP_WAIT         0x9B
#define EXTENDED_OPCODE 0x0F
#define OP_NOP          0x90

#define S_I_B           0x04
#define D32             0x05
#define D16             0x06

#define ESP             0x04
#define EBP             0x05
#define BP              0x06

#define MEM_BX_SI       0x00
#define MEM_BX_DI       0x01
#define MEM_BP_SI       0x02
#define MEM_BP_DI       0x03

#define SCALE_FACTOR_1  0x00
#define SCALE_FACTOR_2  0x40
#define SCALE_FACTOR_4  0x80
#define SCALE_FACTOR_8  0xC0

#define FPE_MIN         0xD8

inline void SetAdrsiz32( asm_code *s ) { s->prefix.adrsiz = ( s->use32 == 0 ); }
inline void SetAdrsiz16( asm_code *s ) { s->prefix.adrsiz = ( s->use32 != 0 ); }
inline void SetAdrsizOn( asm_code *s ) { s->prefix.adrsiz = 1; }
inline void SetAdrsizOff( asm_code *s ) { s->prefix.adrsiz = 0; }
inline void SetOpsiz32( asm_code *s ) { s->prefix.opsiz = ( s->use32 == 0 ); }
inline void SetOpsiz16( asm_code *s ) { s->prefix.opsiz = ( s->use32 != 0 ); }
inline void SetOpsizOn( asm_code *s ) { s->prefix.opsiz = 1; }
inline void SetOpsizOff( asm_code *s ) { s->prefix.opsiz = 0; }

inline bool IsAdrsiz16( asm_code *s ) { return ( ( s->use32 ^ s->prefix.adrsiz ) == 0 ); }
inline bool IsAdrsiz32( asm_code *s ) { return ( ( s->use32 ^ s->prefix.adrsiz ) != 0 ); }
inline bool IsOpsiz32( asm_code *s ) { return ( ( s->use32 ^ s->prefix.opsiz ) != 0 ); }


#define MEM_TYPE( op, typ ) ( (op) == MT_##typ )


/* global variables */
extern asm_sym_handle   Frame;
extern char             Parse_Pass;     // phase of parsing

/* the variables of asmdefs.h, one set for each run of the compiler (run/cbrun.hpp) */
struct AsmdefsState {
    asm_code Code_Info;
    asm_code *Code = &Code_Info;
    operand_idx Opnd_Count;
};
extern bool             Modend;         // end of module is reached
extern bool             Use32;          // if 32-bit code is use

extern void             AsmByte( AsmRun& assembler, unsigned char );
extern void             SetFinalToken( token_buffer *tokbuf, token_idx i );
extern bool             AsmScan( AsmRun& assembler, token_buffer *tokbuf, const char * );
extern void             RadixSet( AsmRun& assembler, unsigned );




#endif
