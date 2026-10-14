/*
**  lzio.c  (segment 6)  -  buffered input / output over the compressed stream.
*/
#include "lzexp.h"

/* Refill - refill the input buffer from hf, carrying the last byte back to
   window[-1]; returns 1 (data), 0x1f4 (EOF marker), or an LZ error. */
int far Refill(BYTE far *pbOut, HFILE hf)
{
    int n;

    *gpInBuf = *(gpInEnd - 1);
    n = _lread(hf, gpInBuf + 1, cbInSize);
    if (cbInSize != n) {              /* short read: error, EOF, or partial buffer */
        if (n == -1)
            return n;
        if (n == 0)
            return REFILL_EOF;
        gpInEnd = gpInBuf + n + 1;
    }
    gpInPtr = gpInBuf + 1;
    if (fFirst != 0) {
        *pbOut = *gpInBuf;
        fFirst = 0;
    } else {
        *pbOut = *gpInPtr;
        gpInPtr += 1;
    }
    return 1;
}

/* Flush - flush the output buffer to hf (hf==0xfffe means discard), then store
   one more byte; returns 1 or an LZ error. */
int far Flush(int b, HFILE hf)
{
    int nWritten, nBuffered;

    nBuffered = gpOutPtr - gpOutBuf;
    gpOutPtr  = gpOutBuf;
    if (hf != -2) {                  /* -2 == discard-only (no destination file) */
        nWritten = _lwrite(hf, gpOutBuf, nBuffered);
        if (nWritten != nBuffered)
            return nWritten == -1 ? LZERROR_BADOUTHANDLE : LZERROR_WRITE;
    }
    *gpOutPtr = (BYTE)b;
    gpOutPtr += 1;
    return 1;
}
