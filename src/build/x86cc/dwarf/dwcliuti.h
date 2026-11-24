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
* Description:  Client independent utility functions for DWARF writer lib.
*
****************************************************************************/


#ifndef DWCLIUTI_H_INCLUDED
#define DWCLIUTI_H_INCLUDED

inline dw_out_offset CLISectionOffset( BackRun& back, dw_client cli, dw_sectnum sect ) { return CLITell( back, cli, sect ) - cli->section_base[sect]; }
inline dw_out_offset CLISectionAbs( BackRun& back, dw_client cli, dw_sectnum sect ) { return CLITell( back, cli, sect ); }
inline void CLISectionSkip( BackRun& back, dw_client cli, dw_sectnum sect, dw_out_offset len ) { CLISeek( back, cli, sect, len, DW_SEEK_CUR ); }
inline void CLISectionSeekOffset( BackRun& back, dw_client cli, dw_sectnum sect, dw_out_offset offset ) { CLISeek( back, cli, sect, cli->section_base[sect] + offset, DW_SEEK_SET ); }
inline void CLISectionSeekAbs( BackRun& back, dw_client cli, dw_sectnum sect, dw_out_offset offset ) { CLISeek( back, cli, sect, offset, DW_SEEK_SET ); }
inline void CLISectionSeekEnd( BackRun& back, dw_client cli, dw_sectnum sect ) { CLISeek( back, cli, sect, 0, DW_SEEK_END ); }

inline void CLISectionReserveSize( BackRun& back, dw_client cli, dw_sectnum sect ) { CLISectionSkip( back, cli, sect, sizeof( uint_32 ) ); }
inline void InfoSkip( BackRun& back, dw_client cli, dw_out_offset len ) { CLISectionSkip( back, cli, DW_DEBUG_INFO, len ); }
inline dw_out_offset InfoSectionOffset( BackRun& back, dw_client cli ) { return CLISectionOffset( back, cli, DW_DEBUG_INFO ); }

extern void     CLIWriteSLEB128( BackRun& back, dw_client cli, dw_sectnum sect, dw_sconst value );
extern void     CLIWriteULEB128( BackRun& back, dw_client cli, dw_sectnum sect, dw_uconst value );
extern void     CLIWriteU8( BackRun& back, dw_client cli, dw_sectnum sect, uint_8 data );
extern void     CLIWriteU16( BackRun& back, dw_client cli, dw_sectnum sect, uint_16 data );
extern void     CLIWriteU32( BackRun& back, dw_client cli, dw_sectnum sect, uint_32 data );
extern void     CLIWriteString( BackRun& back, dw_client cli, dw_sectnum sect, const char *str );

extern void     BufWriteU16( dw_client cli, void *buf, uint_16 data );
extern void     BufWriteU32( dw_client cli, void *buf, uint_32 data );

extern void     CLISectionSetSize( BackRun& back, dw_client cli, dw_sectnum sect );
extern void     CLISectionWriteZeros( BackRun& back, dw_client cli, dw_sectnum sect, size_t len );

#endif
