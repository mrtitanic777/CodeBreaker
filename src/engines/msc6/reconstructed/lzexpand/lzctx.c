/*
**  lzctx.c  (segment 5)  -  per-open-file context: carve the buffers out of the
**  GlobalAlloc'd LZI block and snapshot / restore the codec state across reads.
*/
#include "lzexp.h"

/* Setup - point the active buffer globals at the sub-regions carved out of one
   LZI context block.  These offsets must match struct LZI exactly:
     +0x11   input buffer   (CTX_BUFSIZE + 1 guard byte, ends before the window)
     +0x413  4K ring window
     +0x1493 output buffer  (CTX_BUFSIZE)
   The tail from +0x1893 holds the saved codec state (struct LZSAVE). */
void far Setup(BYTE far *base)
{
    gpInBuf  = base + 0x11;
    gpOutBuf = base + 0x1493;
    gpWindow = base + 0x413;
    cbInSize  = CTX_BUFSIZE;
    cbOutSize = CTX_BUFSIZE;
}

void far SaveState(BYTE far *ctx)
{
    struct LZSAVE far *s = (struct LZSAVE far *)(ctx + 0x1893);

    s->cbInAvail  = gpInPtr  - gpInBuf;
    s->cbInFill   = gpInEnd  - gpInBuf;
    s->fFirst     = fFirst;
    s->cbOutAvail = gpOutPtr - gpOutBuf;
    s->cbOutFill  = gpOutEnd - gpOutBuf;
    s->lBase      = lOutCount - (long)(gpOutPtr - gpOutBuf);
    s->wControl   = wControl;
    s->wRingPos   = wRingPos;
}

void far RestoreState(BYTE far *ctx)
{
    struct LZSAVE far *s;

    Setup(ctx);
    s = (struct LZSAVE far *)(ctx + 0x1893);
    gpInPtr  = gpInBuf + s->cbInAvail;
    gpInEnd  = gpInBuf + s->cbInFill;
    fFirst   = s->fFirst;
    gpOutPtr = gpOutBuf + s->cbOutAvail;
    gpOutEnd = gpOutBuf + s->cbOutFill;
    lFileSize = *(long far *)(ctx + 9);
    lOutCount = *(long far *)(ctx + 0x18a1) + (long)s->cbOutAvail;
    wControl  = s->wControl;
    wRingPos  = s->wRingPos;
    cbThreshold = 0x10;
}
