/*
**  lzhdr.c  (segment 8)  -  the SZDD 14-byte file header: pack, parse, verify.
*/
#include "lzexp.h"

int far WriteHeader(struct LZHEADER far *p, HFILE h)
{
    BYTE  buf[14];
    int   i;
    WORD  n, wr;
    char  shift, t;

    for (i = 0; i < 8; i++)
        buf[i] = p->sig[i];
    buf[i++] = p->alg;
    buf[i++] = p->ext;
    shift = 0;
    n = 4;
    do {
        buf[i++] = (BYTE)(p->cbOrig >> shift);
        t = shift; t += 8; shift = t;
    } while (--n);
    wr = _lwrite(h, (BYTE far *)buf, SZDD_HEADER_SIZE);
    if (wr != SZDD_HEADER_SIZE) {
        if (wr == 0xffff)
            return LZERROR_BADOUTHANDLE;
        return LZERROR_WRITE;
    }
    lOutCount += wr;
    return 1;
}

int far ReadHeader(struct LZHEADER far *p, HFILE h)
{
    BYTE  buf[14];
    int   i;
    WORD  n, r;
    char  shift, t;

    lFileSize = _llseek(h, 0L, 2);
    if (lFileSize < 0L || _llseek(h, 0L, 0) != 0L)
        return LZERROR_BADINHANDLE;
    r = _lread(h, (BYTE far *)buf, SZDD_HEADER_SIZE);
    if (r != SZDD_HEADER_SIZE) {
        if (r == 0xffff)
            return LZERROR_BADINHANDLE;
        return LZERROR_READ;
    }
    for (i = 0; i < 8; i++)
        p->sig[i] = buf[i];
    p->alg = buf[i++];
    p->ext = buf[i++];
    p->cbOrig = 0L;
    shift = 0;
    n = 4;
    do {
        p->cbOrig |= (DWORD)(BYTE)buf[i] << shift;
        i++;
        t = shift; t += 8; shift = t;
    } while (--n);
    p->cbFile = lFileSize;
    return 1;
}

int far CheckSig(struct LZHEADER far *p)
{
    BYTE buf[10];
    int  i;

    for (i = 0; i < 8; i++)
        buf[i] = p->sig[i];
    buf[i] = 0;
    return lstrcmp((BYTE far *)buf, (BYTE far *)szSigIn) == 0;
}

void far InitHeader(struct LZHEADER far *p, int alg, int ext)
{
    int i;

    for (i = 0; i < 8; i++)
        p->sig[i] = szSigOut[i];
    p->alg = (BYTE)alg;
    p->ext = (BYTE)ext;
    p->cbOrig = lFileSize;
    p->cbFile = 0L;
}
