/*
**  lzinit.c  (segment 1)  -  working-buffer allocation and the file-open helper.
*/
#include "lzexp.h"

/* AllocBuffers - allocate the window + the largest input/output buffer pair
   that will fit, shrinking by 1K until GlobalAlloc succeeds. */
int far AllocBuffers(void)
{
    if (gpWindow != 0)
        return 1;
    FP_SEG(gpWindow) = GlobalAlloc(GA_FIXBUF, 0x1080L);
    FP_OFF(gpWindow) = 0;
    if (gpWindow == 0)
        return 0;
    cbInSize  = BUF_START;
    cbOutSize = BUF_START;
    do {
        if (cbOutSize == 0)
            break;
        FP_SEG(gpInBuf) = GlobalAlloc(GA_FIXBUF, (DWORD)cbInSize + 1);
        FP_OFF(gpInBuf) = 0;
        if (gpInBuf != 0) {
            FP_SEG(gpOutBuf) = GlobalAlloc(GA_FIXBUF, (DWORD)cbOutSize);
            FP_OFF(gpOutBuf) = 0;
            if (gpOutBuf != 0)
                return 1;
            GlobalFree(FP_SEG(gpInBuf));
        }
        cbOutSize -= BUF_SHRINK;
        cbInSize  -= BUF_SHRINK;
    } while (cbInSize != 0);
    GlobalFree(FP_SEG(gpWindow));
    return 0;
}

void far FreeBuffers(void)
{
    if (gpWindow != 0) { GlobalFree(FP_SEG(gpWindow)); gpWindow = 0; }
    if (gpInBuf  != 0) { GlobalFree(FP_SEG(gpInBuf));  gpInBuf  = 0; }
    if (gpOutBuf != 0) { GlobalFree(FP_SEG(gpOutBuf)); gpOutBuf = 0; }
}

/* OpenOrCreate - open (fCreate==0 -> create) a file, validating it is seekable. */
int far OpenOrCreate(BYTE far *name, int fCreate, WORD far *phFile)
{
    if (name == 0) {
        *phFile = LZERROR_BADOUTHANDLE;
        return 1;
    }
    if (fCreate == 0) {
        *phFile = _lcreat(name, 0);
        if (*phFile != 0xffff)
            return 1;
        return LZERROR_BADOUTHANDLE;
    }
    *phFile = _lopen(name, 0);
    if (*phFile == 0xffff)
        return LZERROR_BADINHANDLE;
    lFileSize = _llseek(*phFile, 0L, 2);
    if (lFileSize < 0 || _llseek(*phFile, 0L, 0) != 0) {
        _lclose(*phFile);
        return LZERROR_BADINHANDLE;
    }
    return 1;
}
