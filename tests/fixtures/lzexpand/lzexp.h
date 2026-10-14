/*
**  lzexp.h  -  internal shared interface for the LZEXPAND modules.
**
**  The library is built the way the retail DLL was: one hand-written .c file per
**  code segment.  Everything those modules share -- primitive types, the on-disk
**  and per-file-context structs, the tunable constants, the KERNEL/USER imports,
**  the DGROUP globals (defined once in lzdata.c) and the cross-module helper
**  prototypes -- lives here.  The public entry points are in lzexpand.h.
*/
#ifndef _INC_LZEXP
#define _INC_LZEXP

#include "lzexpand.h"        /* WORD/BYTE/HFILE/LPSTR/OFSTRUCT + API + LZERROR_* */

typedef WORD HANDLE;         /* DLL instance handle handed to LibMain */

/* ---- far-pointer field access -------------------------------------------- */
#define FP_OFF(p)  (*((WORD *)&(p)))
#define FP_SEG(p)  (*((WORD *)&(p) + 1))

/* ---- codec + format constants -------------------------------------------- */
#define RING_SIZE   0x1000           /* 4096-byte LZSS sliding window            */
#define ALG_SZDD    'A'              /* the one supported compression algorithm  */

/* GlobalAlloc flags */
#define GMEM_MOVEABLE  0x0002
#define GMEM_ZEROINIT  0x0040
#define GMEM_SHARE     0x2000
#define GA_FIXBUF   (GMEM_SHARE | GMEM_ZEROINIT)      /* fixed shared zeroed working buffer   */
#define GA_CONTEXT  (GMEM_MOVEABLE | GMEM_ZEROINIT)   /* moveable, lockable per-file context  */

/* LZ handle namespace: an LZRead/LZSeek/LZClose handle >= LZ_FIRST_HANDLE is one of our
   decompression contexts (index into ahLZ[]); anything below is a raw DOS file handle. */
#define LZ_FIRST_HANDLE  0x400
#define LZ_MAX_HANDLES   16
#define LZ_LAST_HANDLE   (LZ_FIRST_HANDLE + LZ_MAX_HANDLES)   /* 0x410 */

#define SZDD_HEADER_SIZE 14         /* bytes: sig(8)+alg(1)+ext(1)+cbOrig(4)    */
#define LZI_SIZE         0x18a5L    /* sizeof the per-file context (buffers+state) */
#define REFILL_EOF       0x1f4      /* Refill sentinel: the input file is exhausted */
#define HFILE_NODEST     0xfffe     /* LZDecompress dest: decode into the buffer only */
#define BUF_SHRINK       0x400      /* AllocBuffers step-down when memory is tight  */
#define BUF_START        0x8000     /* AllocBuffers initial buffer size to attempt  */
#define CTX_BUFSIZE      0x400      /* fixed per-context in/out buffer size (handle API) */

/* ---- SZDD 14-byte file header -------------------------------------------- */
struct LZHEADER {
    BYTE  sig[8];       /* 0x00  "SZDD\x88\xf0\x27\x33"       */
    BYTE  alg;          /* 0x08  algorithm ('A')             */
    BYTE  ext;          /* 0x09  original last-extension char */
    DWORD cbOrig;       /* 0x0a  uncompressed length          */
    DWORD cbFile;       /* 0x0e  compressed file length       */
};                      /* 14 bytes on disk                   */

/* ---- per-open-file context, GlobalAlloc'd by LZInit ---------------------- */
#pragma pack(1)
struct LZI {
    WORD  hf;                        /* 0x00  underlying file handle          */
    BYTE  alg;                       /* 0x02  algorithm                       */
    BYTE  flags;                     /* 0x03  bit0 = decompression started    */
    BYTE  b04;                       /* 0x04                                  */
    long  cbOrig;                    /* 0x05  uncompressed length             */
    long  d09;                       /* 0x09                                  */
    long  pos;                       /* 0x0d  logical read position           */
    BYTE  gap[0x1493 - 0x11];        /* 0x11  input buffer region             */
    BYTE  win[0x189f - 0x1493];      /* 0x1493 window / output region         */
    WORD  wWinFill;                  /* 0x189f saved window fill count         */
    long  base;                      /* 0x18a1 file offset of window base      */
};                                   /* size 0x18a5                            */

/* saved decompression state, stored in the tail of an LZI context (LZI+0x1893) */
struct LZSAVE {
    int  wControl;      /* 0x1893  control-bit accumulator          */
    int  wRingPos;      /* 0x1895  ring insertion position          */
    int  cbInAvail;     /* 0x1897  gpInPtr  - gpInBuf               */
    int  cbInFill;      /* 0x1899  gpInEnd  - gpInBuf               */
    int  fFirst;        /* 0x189b  first-byte flag                  */
    int  cbOutAvail;    /* 0x189d  gpOutPtr - gpOutBuf              */
    int  cbOutFill;     /* 0x189f  gpOutEnd - gpOutBuf  (= wWinFill)*/
    long lBase;         /* 0x18a1  window base offset   (= base)    */
};
#pragma pack()

/* ---- KERNEL / USER imports ----------------------------------------------- */
extern WORD  far pascal GlobalAlloc(WORD, DWORD);
extern void far * far pascal GlobalLock(WORD);
extern WORD  far pascal GlobalUnlock(WORD);
extern WORD  far pascal GlobalFree(WORD);
extern HFILE far pascal OpenFile(BYTE far *, struct OFSTRUCT far *, WORD);
extern WORD  far pascal _lopen(BYTE far *, int);
extern WORD  far pascal _lcreat(BYTE far *, int);
extern void  far pascal _lclose(HFILE);
extern WORD  far pascal _lread(HFILE, void far *, WORD);
extern WORD  far pascal _lwrite(HFILE, void far *, WORD);
extern long  far pascal _llseek(HFILE, long, int);
extern BYTE far * far pascal lstrcpy(BYTE far *, BYTE far *);
extern char far * far pascal lstrcat(char far *, char far *);
extern int  far pascal lstrlen(char far *);
extern int  far pascal lstrcmp(BYTE far *, BYTE far *);
extern int  far pascal IsCharUpper(char);
extern int  far pascal IsCharLower(char);

/* ---- DGROUP globals (defined in lzdata.c) -------------------------------- */
extern WORD  ahLZ[16];
extern int   iNextHandle;
extern int   cUseCount;

extern BYTE far *svWindow;
extern BYTE far *svInBuf;
extern BYTE far *svOutBuf;
extern WORD      svInSize;
extern WORD      svOutSize;

extern BYTE far *gpWindow;
extern BYTE far *gpInBuf;
extern BYTE far *gpInEnd;
extern BYTE far *gpInPtr;
extern BYTE far *gpOutBuf;
extern BYTE far *gpOutEnd;
extern BYTE far *gpOutPtr;

extern BYTE  szSigIn[9];
extern BYTE  szSigOut[9];
extern char  szSep[2];
extern char  szUnder[2];
extern char  szDotUnder[3];

extern BYTE  achPath[128];
extern WORD  wDate;
extern WORD  wTime;
extern int   cbThreshold;
extern long  lFileSize;
extern WORD  cbOutSize;
extern WORD  wRingPos;
extern WORD  wControl;
extern WORD  cbInSize;
extern WORD  fFirst;
extern long  lOutCount;

/* ---- cross-module internal helpers --------------------------------------- */
int  far AllocBuffers(void);                              /* lzinit.c  */
void far FreeBuffers(void);
int  far OpenOrCreate(BYTE far *name, int fCreate, WORD far *phFile);
int  far Refill(BYTE far *pbOut, HFILE hf);               /* lzio.c    */
int  far Flush(int b, HFILE hf);
int  far WriteHeader(struct LZHEADER far *p, HFILE h);    /* lzhdr.c   */
int  far ReadHeader(struct LZHEADER far *p, HFILE h);
int  far CheckSig(struct LZHEADER far *p);
void far InitHeader(struct LZHEADER far *p, int alg, int ext);
int  far LZDecompress(WORD hfSource, WORD hfDest, long lEnd, int fStart, int mode); /* lzlzss.c */
int  far CopyRaw(HFILE hfSrc, HFILE hfDst);               /* lzdecode.c */
int  far DecodeFile(int hfSrc, HFILE hfDst);
void far Setup(BYTE far *base);                           /* lzctx.c   */
void far SaveState(BYTE far *ctx);
void far RestoreState(BYTE far *ctx);
char far MangleName(char far *name);                      /* lzname.c  */
void far RestoreExt(char far *name, unsigned char c);
int  far CopyDateTime(int hfSrc, int hfDest);

#endif  /* _INC_LZEXP */
