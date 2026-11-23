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


/* special "logical" characters that represent special states */

#pragma once
enum {
    LCHR_EOF            = 0x0100,       /* last char in program source */
    LCHR_MAX
};

/*     character classes    */

typedef enum charset_flags {
    C_BC = 0,       /* illegal source input character               */
    C_AL = 0x01,    /* alphabetic characters and '_'                */
    C_DI = 0x02,    /* digit 0-9                                    */
    C_HX = 0x04,    /* upper or lower case hex character (A-F,a-f)  */
    C_EX = 0x08,    /* must be examined by GetNextChar              */
    C_DE = 0x10,    /* any delimiter character                      */
    C_WS = 0x40,    /* white space                                  */
    C_DB = 0x80     /* first byte of double-byte char               */
} charset_flags;

#define C_XW    (C_WS | C_EX)   /* white space character with check */
#define C_XD    (C_DE | C_EX)   /* delimiter character with check */
#define C_AX    (C_AL | C_HX)   /* hexa digit letter */

/*
    After we know a character is a C_HX, we can extract the value
    as follows:

        (( c | HEX_MASK ) - HEX_BASE ) + 10
*/
/* ASCII */
#define HEX_MASK        0x20
#define HEX_BASE        'a'
#define ONE_CASE( c )   ((c)|'\x20')
#define ONE_CASE_EQUAL( a, b )  (ONE_CASE( a ) == ONE_CASE( b ))
#define DEC2BIN( c )            ((c) - '0')
#define HEX2BIN( c )            (((c) | HEX_MASK) - HEX_BASE + 10)


#ifndef CB_STATE_SCAN_H
#define CB_STATE_SCAN_H

/* the variables of scan.h, one set for each run of the compiler (run/cbrun.hpp) */
struct ScanState {
    charset_flags CharSet[LCHR_MAX] = {                         /* ASCII */

    /* 00 NUL 01 SOH 02 STX 03 ETX 04 EOT 05 ENQ 06 ACK 07 BEL  */
        C_EX,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* NUL to BEL */

    /* 08 BS  09 HT  0A LF  0B VT  0C FF  0D CR  0E SO  0F SI   */
        C_BC,  C_WS,  C_BC,  C_WS,  C_WS,  C_WS,  C_BC,  C_BC,  /* BS  to SI */

    /* 10 DLE 11 DC1 12 DC2 13 DC3 14 DC4 15 NAK 16 SYN 17 ETB  */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* DLE to ETB */

    /* 18 CAN 19 EM  1A SUB 1B ESC 1C FS  1D GS  1E RS  1F US   */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* CAN to US */

    /* 20 SP  21 !   22 "   23 #   24 $   25 %   26 &   27 '    */
        C_WS,  C_DE,  C_DE,  C_DE,  C_BC,  C_DE,  C_DE,  C_DE,  /* 20 to 27 */

    /* 28 (   29 )   2A *   2B +   2C ,   2D -   2E .   2F /    */
        C_DE,  C_DE,  C_DE,  C_DE,  C_DE,  C_DE,  C_DE,  C_DE,  /* 28 to 2F */

    /* 30 0   31 1   32 2   33 3   34 4   35 5   36 6   37 7    */
        C_DI,  C_DI,  C_DI,  C_DI,  C_DI,  C_DI,  C_DI,  C_DI,  /* 30 to 37 */

    /* 38 8   39 9   3A :   3B ;   3C <   3D =   3E >   3F ?    */
        C_DI,  C_DI,  C_DE,  C_DE,  C_DE,  C_DE,  C_DE,  C_XD,  /* 38 to 3F */

    /* 40 @   41 A   42 B   43 C   44 D   45 E   46 F   47 G    */
        C_BC,  C_AX,  C_AX,  C_AX,  C_AX,  C_AX,  C_AX,  C_AL,  /* @, A to G */

    /* 48 H   49 I   4A J   4B K   4C L   4D M   4E N   4F O    */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* H to O */

    /* 50 P   51 Q   52 R   53 S   54 T   55 U   56 V   57 W    */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* P to W */

    /* 58 X   59 Y   5A Z   5B [   5C \   5D ]   5E ^   5F _    */
        C_AL,  C_AL,  C_AL,  C_DE,  C_EX,  C_DE,  C_DE,  C_AL,  /* X to _ */

    /* 60 `   61 a   62 b   63 c   64 d   65 e   66 f   67 g    */
        C_BC,  C_AX,  C_AX,  C_AX,  C_AX,  C_AX,  C_AX,  C_AL,  /* `, a to g */

    /* 68 h   69 i   6A j   6B k   6C l   6D m   6E n   6F o    */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* h to o */

    /* 70 p   71 q   72 r   73 s   74 t   75 u   76 v   77 w    */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* p to w */

    /* 78 x   79 y   7A z   7B {   7C |   7D }   7E ~   7F DEL  */
        C_AL,  C_AL,  C_AL,  C_DE,  C_DE,  C_DE,  C_DE,  C_BC,  /* x to DEL */

    /* 80     81     82     83     84     85     86     87      */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* $80 to $87 */

    /* 88     89     8A     8B     8C     8D     8E     8F      */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* $88 to $8F */

    /* 90     91     92     93     94     95     96     97      */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* $90 to $97 */

    /* 98     99     9A     9B     9C     9D     9E     9F      */
        C_AL,  C_AL,  C_AL,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $98 to $9F */

    /* A0     A1     A2     A3     A4     A5     A6     A7      */
        C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  C_AL,  /* $A0 to $A7 */

    /* A8     A9     AA     AB     AC     AD     AE     AF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $A8 to $AF */

    /* B0     B1     B2     B3     B4     B5     B6     B7      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $B0 to $B7 */

    /* B8     B9     BA     BB     BC     BD     BE     BF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $B8 to $BF */

    /* C0     C1     C2     C3     C4     C5     C6     C7      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $C0 to $C7 */

    /* C8     C9     CA     CB     CC     CD     CE     CF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $C8 to $CF */

    /* D0     D1     D2     D3     D4     D5     D6     D7      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $D0 to $D7 */

    /* D8     D9     DA     DB     DC     DD     DE     DF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $D8 to $DF */

    /* E0     E1     E2     E3     E4     E5     E6     E7      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $E0 to $E7 */

    /* E8     E9     EA     EB     EC     ED     EE     EF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $E8 to $EF */

    /* F0     F1     F2     F3     F4     F5     F6     F7      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $F0 to $F7 */

    /* F8     F9     FA     FB     FC     FD     FE     FF      */
        C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  C_BC,  /* $F8 to $FF */

        C_BC,  /* LCHR_EOF */
    };
};

#endif

extern int              EncodeWchar( FrontRun& front, int c );
extern int              IndexLower( int c );
