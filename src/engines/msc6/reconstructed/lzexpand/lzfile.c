/*
**  lzfile.c  (segment 2)  -  the handle API (LZInit / LZOpenFile / LZSeek /
**  LZRead / LZClose) plus GetExpandedName.  A decompression handle is an index
**  into ahLZ[] biased by LZ_FIRST_HANDLE; anything smaller is a raw DOS handle
**  passed straight through.
*/
#include "lzexp.h"

HFILE far pascal LZInit(HFILE hfSrc)
{
    struct LZHEADER gi;
    struct LZI far *p;
    int  idx, start, found;
    WORD h;
    int  res;

    res = ReadHeader((struct LZHEADER far *)&gi, hfSrc);
    if (res != 1 && lFileSize >= (long)SZDD_HEADER_SIZE)
        return LZERROR_BADINHANDLE;
    if (res == 1 && CheckSig((struct LZHEADER far *)&gi) == 1) {
        if (gi.alg != ALG_SZDD)
            return LZERROR_UNKNOWNALG;
        start = iNextHandle;
        found = 0;
        do {
            if (ahLZ[iNextHandle] == 0) {
                found = 1;
                break;
            }
            if (++iNextHandle >= LZ_MAX_HANDLES)
                iNextHandle = 0;
        } while (iNextHandle != start);
        if (!found)
            return LZERROR_GLOBALLOC;
        idx = iNextHandle;
        h = GlobalAlloc(GA_CONTEXT, LZI_SIZE);
        if (h == 0)
            return LZERROR_GLOBALLOC;
        p = (struct LZI far *)GlobalLock(h);
        if (p == (struct LZI far *)0) {
            GlobalFree(h);
            return LZERROR_GLOBLOCK;
        }
        p->hf  = hfSrc;
        p->alg = gi.alg;
        *(WORD far *)&p->flags = 0;
        p->cbOrig = gi.cbOrig;
        p->d09 = gi.cbFile;
        p->pos = 0L;
        ahLZ[idx] = h;
        Setup((BYTE far *)p);
        gpInPtr = gpInEnd = gpInBuf + cbInSize + 1;
        fFirst = 0;
        gpOutEnd = gpOutBuf + cbOutSize;
        gpOutPtr = gpOutBuf;
        lOutCount = 0L;
        SaveState((BYTE far *)p);
        GlobalUnlock(h);
        if (++iNextHandle >= LZ_MAX_HANDLES)
            iNextHandle = 0;
        return idx + LZ_FIRST_HANDLE;
    }
    if (_llseek(hfSrc, 0L, 0) != 0L)
        return LZERROR_BADINHANDLE;
    return hfSrc;
}

HFILE far pascal LZOpenFile(LPSTR lpszFile, struct OFSTRUCT far *lpOf, WORD style)
{
    HFILE h;

    lstrcpy((BYTE far *)achPath, lpszFile);
    h = OpenFile((BYTE far *)achPath, lpOf, style);
    if (h == -1 && lpOf->nErrCode == 2) {
        MangleName((char far *)achPath);
        h = OpenFile((BYTE far *)achPath, lpOf, style);
    }
    if (h != -1 && !(style & 0xff0f)) {
        HFILE res;
        if ((res = LZInit(h)) < 0)
            _lclose(h);
        return res;
    }
    return h;
}

long far pascal LZSeek(HFILE hf, long lOff, int iOrigin)
{
    struct LZI far *p;
    WORD handle;
    long newpos;

    if (hf < 0 || hf >= LZ_LAST_HANDLE)
        return 0xffffffffL;
    if (hf < LZ_FIRST_HANDLE)
        return _llseek(hf, lOff, iOrigin);
    handle = ahLZ[hf - LZ_FIRST_HANDLE];
    p = (struct LZI far *)GlobalLock(handle);
    if (p == (struct LZI far *)0)
        return (long)LZERROR_GLOBLOCK & 0xffffffffL;
    if (iOrigin == 0)
        newpos = 0L;
    else if (iOrigin == 1)
        newpos = p->pos;
    else if (iOrigin == 2)
        newpos = p->cbOrig;
    else {
        GlobalUnlock(handle);
        return 0xfffffff9L;
    }
    newpos += lOff;
    if (newpos < 0L) {
        GlobalUnlock(handle);
        return 0xfffffff9L;
    }
    p->pos = newpos;
    GlobalUnlock(handle);
    return newpos;
}

int far pascal LZRead(HFILE hf, void far *buf, int count)
{
    struct LZI far *p;
    BYTE far *pOut;
    WORD handle;
    int  total, firstFlag, navail, si;
    long n, end;

    total = 0;
    if (hf < 0 || hf >= LZ_LAST_HANDLE)
        return LZERROR_BADINHANDLE;
    if (count < 0)
        return LZERROR_BADVALUE;
    if (hf < LZ_FIRST_HANDLE)
        return _lread(hf, buf, count);
    handle = ahLZ[hf - LZ_FIRST_HANDLE];
    p = (struct LZI far *)GlobalLock(handle);
    if (p == (struct LZI far *)0)
        return LZERROR_GLOBLOCK;
    if (!(p->flags & 1)) {
        p->flags |= 1;
        firstFlag = 1;
    } else {
        firstFlag = (p->pos < p->base) ? 1 : 0;
    }
    RestoreState((BYTE far *)p);
    pOut = (BYTE far *)buf;
    while (count > 0) {
        if (p->cbOrig <= p->pos)
            break;
        navail = gpOutPtr - gpOutBuf;
        n = p->pos - p->base;
        si = (n >= (long)p->wWinFill) ? -1 : (int)n;
        if (si >= 0) {
            int nCopied = 0;
            if (navail > si) {
                for (;;) {
                    if (count <= 0)
                        break;
                    *pOut++ = p->win[si];
                    nCopied++;
                    count--;
                    si++;
                    if (navail <= si)
                        break;
                }
            }
            p->pos += (long)nCopied;
            total += nCopied;
        }
        if (count <= 0 || p->cbOrig <= p->pos)
            break;
        if (firstFlag) {
            n = p->pos - (long)cbThreshold;
            if (n < 0L)
                n = 0L;
        } else {
            n = (long)p->wWinFill + p->base - (long)cbThreshold + (long)navail;
        }
        end = p->pos + (long)count;
        if (end > p->cbOrig - 1L)
            end = p->cbOrig - 1L;
        if (end > n)
            end = n;
        gpOutPtr = gpOutBuf;
        if (p->alg != ALG_SZDD)
            si = LZERROR_UNKNOWNALG;
        else
            si = LZDecompress(p->hf, HFILE_NODEST, end, firstFlag, 1);
        if (si != 1) {
            GlobalUnlock(handle);
            return si;
        }
        if ((gpOutPtr - gpOutBuf) > 0) {
            p->base = lOutCount - (long)(gpOutPtr - gpOutBuf);
            if (lOutCount < end) {
                GlobalUnlock(handle);
                return LZERROR_READ;
            }
            firstFlag = 0;
        }
    }
    SaveState((BYTE far *)p);
    GlobalUnlock(handle);
    return total;
}

void far pascal LZClose(HFILE hf)
{
    struct LZI far *p;
    WORD handle;

    if (hf < 0 || hf >= LZ_LAST_HANDLE)
        return;
    if (hf < LZ_FIRST_HANDLE) {
        _lclose(hf);
        return;
    }
    handle = ahLZ[hf - LZ_FIRST_HANDLE];
    ahLZ[hf - LZ_FIRST_HANDLE] = 0;
    p = (struct LZI far *)GlobalLock(handle);
    if (p == (struct LZI far *)0)
        return;
    _lclose(p->hf);
    GlobalUnlock(handle);
    GlobalFree(handle);
}

int far pascal GetExpandedName(LPSTR lpszSource, LPSTR lpszBuffer)
{
    struct OFSTRUCT of;
    struct LZHEADER gi;
    HFILE h;
    int   res;

    h = OpenFile(lpszSource, (struct OFSTRUCT far *)&of, 0);
    if (h == -1)
        return LZERROR_BADINHANDLE;
    res = ReadHeader((struct LZHEADER far *)&gi, h);
    if (res != 1 && lFileSize >= (long)SZDD_HEADER_SIZE)
        return LZERROR_BADINHANDLE;
    _lclose(h);
    lstrcpy(lpszBuffer, lpszSource);
    if (res == 1 && CheckSig((struct LZHEADER far *)&gi) == 1)
        RestoreExt((char far *)lpszBuffer, gi.ext);
    return 1;
}
