/*
**  lzdata.c  -  the DLL's shared state (DGROUP).
**
**  Every global the modules touch is defined once, here.  This module has no
**  code, so it contributes only to the single DATA segment -- not an extra code
**  segment.  The declarations the other modules see are the externs in lzexp.h;
**  the two SZDD signature strings and the path-mangling literals are initialized.
*/
#include "lzexp.h"

WORD  ahLZ[16];                 /* open-context handle table                */
int   iNextHandle;              /* next handle-table slot to probe          */
int   cUseCount;                /* LZStart / LZDone nesting count            */

/* save area for the classic API (mirrors the active globals below) */
BYTE far *svWindow;
BYTE far *svInBuf;
BYTE far *svOutBuf;
WORD      svInSize;
WORD      svOutSize;

/* active buffer set (carved from one GlobalAlloc block by Setup / AllocBuffers) */
BYTE far *gpWindow;             /* 4096-byte LZSS ring window                */
BYTE far *gpInBuf;              /* compressed-input buffer                   */
BYTE far *gpInEnd;              /* end of valid input                        */
BYTE far *gpInPtr;              /* next input byte                          */
BYTE far *gpOutBuf;             /* decompressed-output buffer                */
BYTE far *gpOutEnd;             /* end of output buffer                      */
BYTE far *gpOutPtr;             /* next output byte                         */

BYTE  szSigIn[9]  = { 'S','Z','D','D',0x88,0xF0,0x27,0x33,0 };
BYTE  szSigOut[9] = { 'S','Z','D','D',0x88,0xF0,0x27,0x33,0 };
char  szSep[2]      = "\\";     /* path separator                           */
char  szUnder[2]    = "_";      /* mangled-extension pad                    */
char  szDotUnder[3] = "._";     /* no-extension mangle                      */

BYTE  achPath[128];             /* OpenFile path work buffer                 */
WORD  wDate;                    /* file date                                */
WORD  wTime;                    /* file time                                */
int   cbThreshold;              /* LZSS match threshold (0x10)               */
long  lFileSize;                /* size of the file being read              */
WORD  cbOutSize;                /* output buffer size                       */
WORD  wRingPos;                 /* ring insertion position                  */
WORD  wControl;                 /* LZSS control-bit accumulator             */
WORD  cbInSize;                 /* input buffer size                        */
WORD  fFirst;                   /* first-byte-of-run flag                   */
long  lOutCount;                /* running count of bytes emitted           */
