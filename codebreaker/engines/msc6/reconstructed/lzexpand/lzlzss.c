/*
**  lzlzss.c  (segment 9)  -  the LZSS decompression core.
**
**  Classic Microsoft SZDD codec: a 4096-byte sliding window pre-filled with
**  spaces, an LSB-first control byte (1 = literal, 0 = window match), 12-bit
**  offsets and (length+3) run lengths.  Bytes move through the shared buffers
**  via Refill/Flush (see lzio.c) so the decode can be suspended and resumed.
*/
#include "lzexp.h"

/* GETB/PUTB - fetch one input byte / store one output byte through the buffers.
   Each sets its status (si/s) to 1 on success, or Refill/Flush's return otherwise;
   the buffer pointers (gpInPtr/gpOutPtr) advance in lockstep with the counts so the
   codec state can be snapshotted mid-stream (see SaveState/RestoreState). */
#define GETB(d)    if (gpInEnd <= gpInPtr) si = Refill(&(d), hfSource); \
                   else { (d) = *gpInPtr; ++gpInPtr; si = 1; }
#define PUTB(v, s) if (gpOutPtr >= gpOutEnd) { lOutCount++; s = Flush((v), hfDest); } \
                   else { *gpOutPtr = (v); ++gpOutPtr; lOutCount++; s = 1; }

int far LZDecompress(WORD hfSource, WORD hfDest, long lEnd, int fStart, int mode)
{
    int  si, st, i, bb, count;
    BYTE cur, b1;
    WORD off;

    cbThreshold = 0x10;
    if (fStart == 1) {
        if (_llseek(hfSource, (long)SZDD_HEADER_SIZE, 0) != (long)SZDD_HEADER_SIZE)
            return LZERROR_BADINHANDLE;
        if (hfDest != HFILE_NODEST && _llseek(hfDest, 0L, 0) != 0L)
            return LZERROR_BADOUTHANDLE;
        gpInEnd = gpInBuf + cbInSize + 1;
        gpInPtr = gpInEnd;
        gpOutEnd = gpOutBuf + cbOutSize;
        gpOutPtr = gpOutBuf;
        fFirst = i = 0;
        lOutCount = 0;
        for (; i < RING_SIZE - cbThreshold; i++)
            gpWindow[i] = 0x20;
        wControl = 0;
        wRingPos = RING_SIZE - cbThreshold;
    }
    GETB(cur);
    if (si != 1 && si != REFILL_EOF)
        return si;
    do {
        if (si == REFILL_EOF)
            return 1;
        if (lOutCount > lEnd) {
            /* produced the whole file: push the lookahead byte back for a resume */
            if (gpInBuf + 1 == gpInPtr)
                fFirst = 1;
            else
                --gpInPtr;
            return 1;
        }
        wControl >>= 1;
        if ((wControl & 0x100) == 0) {
            wControl = 0xff00 | cur;
            GETB(cur);
            if (--si != 0)
                return LZERROR_READ;
        }
        if (wControl & 1) {                 /* literal byte */
            PUTB(cur, si);
            if (si != 1)
                return si;
            gpWindow[wRingPos] = cur;
            wRingPos = (wRingPos + 1) & (RING_SIZE - 1);
        } else {                            /* window match */
            GETB(b1);
            if (--si != 0)
                return LZERROR_READ;
            bb = b1;
            off   = ((bb & 0xf0) << 4) | cur;
            count = (bb & 0x0f) + 2;
            for (si = 0; si <= count; si++) {
                cur = gpWindow[(off + si) & (RING_SIZE - 1)];
                PUTB(cur, st);
                if (st != 1)
                    return st;
                gpWindow[wRingPos] = cur;
                wRingPos = (wRingPos + 1) & (RING_SIZE - 1);
            }
        }
        GETB(cur);
    } while (si == 1 || si == REFILL_EOF);
    return si;
}
