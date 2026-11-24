/****************************************************************************
*
*                            Open Watcom Project
*
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
* Description:  Program to generate a set of macros to manipulate a
*               fixed-length (but arbitrarily large) bit set.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wio.h"
#include "walloca.h"
#include "supportrun.hpp"

inline int NLongs( int x ) { return ( ( (x) + 31 ) / 32 ); }

static void emitHeaderInit( FILE *fp, int size, char *prefix, char *type_name )
//*****************************************************************************
// emit the typedef of the actual bitset into the generated file, as well as
// other misc cruft that belongs at the beginning of the header
{
    int         i;


    fprintf( fp, "#ifndef A_BIT_SET_DEFINED\n" );
    fprintf( fp, "typedef unsigned a_bit_set;\n" );
    fprintf( fp, "#define A_BIT_SET_DEFINED\n" );
    fprintf( fp, "#endif\n\n" );
    fprintf( fp, "#define %s_SIZE %d\n\n", prefix, size );
    fprintf( fp, "typedef struct %s {\n", type_name );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\ta_bit_set\t_%d;\n", i );
    }
    fprintf( fp, "} %s;\n\n", type_name );
}

static void emitAssign( FILE *fp, int size, char *prefix, char *type_name )
//*************************************************************************
// emit definition of a macro to do assignment of bitsets
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sAssign( dst, src ) { \\\n", prefix );
    // fprintf( fp, "\t\t\t%s *srcp = &(src); \\\n\t\t\t%s *dstp = &(dst); \\\n", type_name, type_name );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(dst)._%d = (src)._%d; \\\n", i, i );
    }
    fprintf( fp, "\t\t}\n\n" );
}

static void emitEmpty( FILE *fp, int size, char *prefix, char *type_name )
//************************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sEmpty( set ) ( \\\n", prefix );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(set)._%d == 0 %s\n", i,
                        ( i == ( NLongs( size ) - 1 ) ) ? " )" : "&& \\" );
    }
    fprintf( fp, "\n" );
}

static void emitSet( FILE *fp, int size, char *prefix, char *type_name )
//**********************************************************************
{
    int         i;
    char        c;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sSet( dst, ", prefix );
    c = 'a';
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "%c%s", c++, ( i == ( NLongs( size ) - 1 ) ) ? " ) { \\\n" : ", " );
    }
    for( c = 'a', i = 0; i < NLongs( size ); i++, c++ ) {
        fprintf( fp, "\t\t\t(dst)._%d = (a_bit_set)%c; \\\n", i, c );
    }
    fprintf( fp, "\t\t}\n\n" );
    fprintf( fp, "#define %sFirst( set ) %sSet( set, 1", prefix, prefix );
    for( i = 1; i < NLongs( size ); i++ ) {
        fprintf( fp, ", 0" );
    }
    fprintf( fp, " )\n\n" );
    fprintf( fp, "#define %sInit( set, val ) %sSet( set, val", prefix, prefix );
    for( i = 1; i < NLongs( size ); i++ ) {
        fprintf( fp, ", val" );
    }
    fprintf( fp, " )\n\n" );
}

static void emitIter( FILE *fp, int size, char *prefix, char *type_name )
//***********************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sIter( routine, set ) { \\\n", prefix );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(routine)( (set)._%d ); \\\n", i );
    }
    fprintf( fp, "\t\t}\n\n" );
}

static void emitOverlap( FILE *fp, int size, char *prefix, char *type_name )
//**************************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sOverlap( a, b ) ( \\\n", prefix );
    for( i = 0; i < NLongs( size ) - 1; i++ ) {
        fprintf( fp, "\t\t\t( (a)._%d & (b)._%d ) || \\\n", i, i );
    }
    fprintf( fp, "\t\t\t( (a)._%d & (b)._%d ) )\n\n", i, i );
}

static void emitSame( FILE *fp, int size, char *prefix, char *type_name )
//***********************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sSame( a, b ) ( \\\n", prefix );
    for( i = 0; i < NLongs( size ) - 1; i++ ) {
        fprintf( fp, "\t\t\t( (a)._%d == (b)._%d ) && \\\n", i, i );
    }
    fprintf( fp, "\t\t\t( (a)._%d == (b)._%d ) )\n\n", i, i );
}

static void emitIntersect( FILE *fp, int size, char *prefix, char *type_name )
//****************************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sIntersect( a, b ) { \\\n", prefix );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(a)._%d &= (b)._%d; \\\n", i, i );
    }
    fprintf( fp, "\t\t}\n\n" );
}

static void emitTurnOff( FILE *fp, int size, char *prefix, char *type_name )
//**************************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sTurnOff( a, b ) { \\\n", prefix );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(a)._%d &= ~(b)._%d; \\\n", i, i );
    }
    fprintf( fp, "\t\t}\n\n" );
}

static void emitTurnOn( FILE *fp, int size, char *prefix, char *type_name )
//*************************************************************************
{
    int         i;

    /* unused parameters */ (void)type_name;

    fprintf( fp, "#define %sTurnOn( a, b ) { \\\n", prefix );
    for( i = 0; i < NLongs( size ); i++ ) {
        fprintf( fp, "\t\t\t(a)._%d |= (b)._%d; \\\n", i, i );
    }
    fprintf( fp, "\t\t}\n\n" );
}

static void emitScalar( FILE *fp, int size, char *prefix, char *type_name )
//*************************************************************************
{
    /* unused parameters */ (void)type_name; (void)size;

    fprintf( fp, "#define %sScalar( a ) ( (a)._0 )\n\n", prefix );
}

static void emitStmt( FILE *fp, int indent, char *stmt )
//******************************************************
{
    while( indent >= 2 ) {
        fprintf( fp, "\t" );
        indent -= 2;
    }
    if( indent ) {
        fprintf( fp, "    " );
    }
    fprintf( fp, "%s\n", stmt );
}

static void doSet( FILE *fp, int indent, int size, char *prefix, char *var, int which_long )
//******************************************************************************************
// set the given long in the bitset (_0 == 0) to 1, and all other bits
// to 0
{
    char        *buffer;
    int         i;
    char        *ptr;

    buffer = static_cast<char *>(alloca( strlen( var ) + strlen( prefix ) + NLongs( size ) * 3 + 100 ));
    sprintf( buffer, "%sSet( *%s, ", prefix, var );
    ptr = buffer + strlen( buffer );
    for( i = 0; i < NLongs( size ) - 1; i++ ) {
        sprintf( ptr, "%d, ", ( i == which_long ) ? 1 : 0 );
        ptr += 3;
    }
    sprintf( ptr, "%d );", ( i == which_long ) ? 1 : 0 );
    emitStmt( fp, indent, buffer );
}

static void doShift( FILE *fp, int indent, int size, char *var )
//**************************************************************
{
    int         i;
    char        buffer[ 80 ];

    for( i = 0; i < NLongs( size ); i++ ) {
        sprintf( buffer, "(*%s)._%d <<= 1;", var, i );
        emitStmt( fp, indent, buffer );
    }
}

static void checkOtherBits( FILE *fp, int indent, int size, char *prefix, char *var, int which_long ) {
//*****************************************************************************************************

    if( which_long >= NLongs( size ) ) {
        doShift( fp, indent, size, var );
    } else {
        char    buffer[ 80 ];

        sprintf( buffer, "if( %s->_%d & 0x80000000 ) {", var, which_long );
        emitStmt( fp, indent, buffer );
        doSet( fp, indent + 1, size, prefix, var, which_long + 1 );
        emitStmt( fp, indent, const_cast<char *>("} else {") );
        checkOtherBits( fp, indent + 1, size, prefix, var, which_long + 1 );
        emitStmt( fp, indent, const_cast<char *>("}") );
    }
}

static void emitBitNext( FILE *fp, int size, char *prefix, char *type_name )
//**************************************************************************
{
    int         indent;

    fprintf( fp, "#ifdef %s_DEFINE_BITNEXT\n", prefix );
    fprintf( fp, "static void %sNext( %s *set ) {\n\n", prefix, type_name );
    indent = 1;
    if( size > 32 ) {
        emitStmt( fp, indent, const_cast<char *>("if( set->_0 & 0x80000000 ) {") );
        doSet( fp, indent + 1, size, prefix, const_cast<char *>("set"), 1 );
        emitStmt( fp, indent, const_cast<char *>("} else {") );
        if( size > 64 ) {
            checkOtherBits( fp, indent + 1, size, prefix, const_cast<char *>("set"), 1 );
        } else {
            doShift( fp, indent + 1, size, const_cast<char *>("set") );
        }
        emitStmt( fp, indent, const_cast<char *>("}") );
    } else {
        doShift( fp, indent, size, const_cast<char *>("set") );
    }
    fprintf( fp, "}\n" );
    fprintf( fp, "#endif\n\n" );
}

static void genBitsetHeader( SupportRun& support, char *file, int size, char *prefix, char *type_name )
//*************************************************************************
// create a header file named 'file' which contains type and macro definitions
// for dealing with a bitset of length 'size'.
{
    FILE        *fp;

    fp = stdout;
    if( file != NULL ) {
        fp = support.held->fopen( file, "wt" );
    }
    if( fp != NULL ) {
        emitHeaderInit( fp, size, prefix, type_name );
        emitAssign( fp, size, prefix, type_name );
        emitEmpty( fp, size, prefix, type_name );
        emitSet( fp, size, prefix, type_name );
        emitIter( fp, size, prefix, type_name );
        emitOverlap( fp, size, prefix, type_name );
        emitSame( fp, size, prefix, type_name );
        emitTurnOff( fp, size, prefix, type_name );
        emitIntersect( fp, size, prefix, type_name );
        emitTurnOn( fp, size, prefix, type_name );
        emitScalar( fp, size, prefix, type_name );
        emitBitNext( fp, size, prefix, type_name );
        support.held->fclose( fp );
    } else {
        fprintf( stderr, "Error: unable to create file '%s': %s\n", file, strerror( errno ) );
    }
}

int main( int argc, char *argv[] ) {
    SupportRun support;   // (this tool's own run of the support routines: it is no part of a compiler's)

    if( argc != 5 ) {
        fprintf( stderr, "usage: %s <size> <prefix> <type_name> <file>\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }
    genBitsetHeader( support, argv[ 4 ], atoi( argv[ 1 ] ), argv[ 2 ], argv[ 3 ] );
    return 0;
}
