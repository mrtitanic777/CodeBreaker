/*
**  lzcopy.c  (segment 4)  -  the classic global-state API: LZStart / LZDone
**  bracket a batch of copies sharing one buffer set; LZCopy / CopyLZFile expand
**  a whole file in one call.
*/
#include "lzexp.h"

/* LZStart / LZDone bracket a batch of classic-API copies and share one buffer set
   via a reference count.  Invariant: the sv* save area always holds the canonical
   buffer pointers between calls; the active gp* globals are a scratch copy.  The
   direction matters - LZStart copies save->active first, and only after a successful
   AllocBuffers copies active->save (never the reverse; inverting these leaves the
   save area pointing at freed/zero memory). LZDone mirrors this on the way out. */
int far pascal LZStart(void)
{
    gpWindow = svWindow;                 /* restore the active buffer set from the save area */
    gpInBuf  = svInBuf;
    gpOutBuf = svOutBuf;
    cbInSize  = svInSize;
    cbOutSize = svOutSize;
    if (cUseCount == 0) {
        if (AllocBuffers() != 1)
            return LZERROR_GLOBALLOC;
        svWindow = gpWindow;             /* first entry: remember the freshly-allocated buffers */
        svInBuf  = gpInBuf;
        svOutBuf = gpOutBuf;
        svInSize  = cbInSize;
        svOutSize = cbOutSize;
    }
    if (++cUseCount == 0)
        cUseCount = 0xffff;
    return 1;
}

void far pascal LZDone(void)
{
    if (cUseCount == 0)
        return;
    if (--cUseCount != 0)
        return;
    gpWindow = svWindow;
    gpInBuf  = svInBuf;
    gpOutBuf = svOutBuf;
    cbInSize  = svInSize;
    cbOutSize = svOutSize;
    FreeBuffers();
    svWindow = gpWindow;
    svInBuf  = gpInBuf;
    svOutBuf = gpOutBuf;
    svInSize  = cbInSize;
    svOutSize = cbOutSize;
}

long far pascal LZCopy(HFILE hfSource, HFILE hfDest)
{
    long result;
    int  si, di;

    si = LZStart();
    if (si != 1)
        return (long)si;

    si = hfSource;
    if (si >= LZ_FIRST_HANDLE) {                          /* an LZ handle: resolve it */
        di = ahLZ[si - LZ_FIRST_HANDLE];
        if (di == 0) {
            result = 0xffffffffL;
            goto done;
        }
        {
            WORD far *lp = (WORD far *)GlobalLock((WORD)di);
            if (lp == (WORD far *)0) {
                result = (long)LZERROR_GLOBLOCK & 0xffffffffL;
                goto done;
            }
            si = *lp;
        }
        GlobalUnlock((WORD)di);
    }
    di = DecodeFile(si, hfDest);
    result = (di != 1) ? (long)di : lOutCount;
done:
    LZDone();
    return result;
}

long far pascal CopyLZFile(HFILE hfSource, HFILE hfDest)
{
    return LZCopy(hfSource, hfDest);
}
