/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Intel instruction encoding, part II. Processes labels,
*               jumps and the like.
*
****************************************************************************/


#pragma once
extern void     DoCall( BackRun& back, label_handle lbl, bool imported, bool big, bool pop );
extern void     GenCall( BackRun& back, instruction *ins );
extern void     GenCallIndirect( BackRun& back, instruction *ins );
extern void     GenCallRegister( BackRun& back, instruction *ins );
extern void     GenSelEntry( BackRun& back, bool starts );
extern void     Gen1ByteValue( BackRun& back, byte value );
extern void     Gen2ByteValue( BackRun& back, uint_16 value );
extern void     Gen4ByteValue( BackRun& back, uint_32 value );
extern void     Gen8ByteValue( BackRun& back, unsigned_64 *value );
extern void     GenCodePtr( BackRun& back, pointer label );
extern void     GenJmpMemory( BackRun& back, instruction *ins );
extern void     GenJmpRegister( BackRun& back, instruction *ins );
extern void     EyeCatchBytes( BackRun& back, const void *src, byte_seq_len len );
extern void     GenReturn( BackRun& back, int pop, bool is_long );
extern void     GenIRET( BackRun& back );
extern void     CodeBytes( BackRun& back, const void *src, byte_seq_len len );
