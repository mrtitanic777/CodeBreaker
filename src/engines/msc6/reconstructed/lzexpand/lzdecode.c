/*
**  lzdecode.c  (segment 7)  -  file-level decode: raw copy or SZDD expansion,
**  dispatched on the header, then the trailing flush + timestamp copy.
*/
#include "lzexp.h"

int far CopyRaw(HFILE hfSrc, HFILE hfDst)
{
    unsigned int si, di;

    if (_llseek(hfSrc, 0L, 0) != 0L)
        return LZERROR_BADINHANDLE;
    if (hfDst != HFILE_NODEST && _llseek(hfDst, 0L, 0) != 0L)
        return LZERROR_BADOUTHANDLE;
    gpInPtr = gpInEnd = gpInBuf + cbInSize + 1;
    fFirst = 0;
    gpOutEnd = gpOutBuf + cbOutSize;
    gpOutPtr = gpOutBuf;
    lOutCount = 0L;
    do {
        si = _lread(hfSrc, gpInBuf, cbInSize);
        if (si == 0 || si == 0xffff)
            return si == 0xffff ? LZERROR_BADINHANDLE : 1;
        di = _lwrite(hfDst, gpInBuf, si);
        if (di != si)
            return di == 0xffff ? LZERROR_WRITE : LZERROR_BADOUTHANDLE;
        lOutCount += (WORD)di;
    } while (cbInSize == si);
    return 1;                    /* last block was short (si < cbInSize): copy complete */
}

/* DecodeFile - expand hfSrc into hfDst end-to-end (header, body, timestamp). */
int far DecodeFile(int hfSrc, HFILE hfDst)
{
    struct LZHEADER hdr;
    int fCompressed, di, si;

    if (ReadHeader(&hdr, hfSrc) != 1 && lFileSize >= (long)SZDD_HEADER_SIZE)
        return LZERROR_BADINHANDLE;
    fCompressed = (CheckSig(&hdr) == 1) ? 1 : 0;
    if (fCompressed) {
        switch (hdr.alg) {
        case ALG_SZDD:
            di = LZDecompress(hfSrc, hfDst, hdr.cbOrig - 1L, 1, 1);
            break;
        default:
            di = LZERROR_UNKNOWNALG;
            break;
        }
    } else {
        di = CopyRaw(hfSrc, hfDst);
    }
    if (di != 1)
        return di;
    si = Flush(0x46, hfDst);            /* flush the final output buffer */
    if (si != 1)
        return si;
    di = CopyDateTime(hfSrc, hfDst);
    if (di != 1)
        return di;
    if (fCompressed && lOutCount != hdr.cbOrig)
        return LZERROR_READ;
    return di;
}
