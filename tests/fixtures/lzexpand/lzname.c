/*
**  lzname.c  (segment 10)  -  path / filename helpers behind GetExpandedName and
**  the LZOpenFile name-mangler, plus the file-timestamp copy.
**
**  The timestamp copy uses INT 21h AH=57h (get/set a file's date and time by
**  handle) -- the same DOS service the retail DLL reached through KERNEL's
**  DOS3Call.  Keeping it here, inside the module that owns CopyDateTime, means the
**  DLL needs no extra import for it.
*/
#include "lzexp.h"

char far *far FileName(char far *arg)
{
    register char far *p;
    char far *q;

    p = q = arg;
    while (*p != 0) {
        if (*p == '/' || *p == '\\' || *p == ':')
            q = p + 1;
        p++;
    }
    return q;
}

char far *far FileExt(char far *name)
{
    char far *s;

    s = FileName(name);
    if (*s != 0) {
        do {
            if (*s == '.')
                break;
            s++;
        } while (*s != 0);
    }
    return (*s == '.') ? s + 1 : (char far *)0;
}

/* AppendName - append the filename component of src onto the path dst. */
void far AppendName(char far *dst, char far *src)
{
    char c;

    src = FileName(src);
    if (*src == 0)
        return;
    if (src[0] == '.' && src[1] == 0)
        return;
    c = dst[lstrlen(dst) - 1];
    if (c != '/' && c != '\\' && c != ':')
        lstrcat(dst, szSep);
    lstrcat(dst, src);
}

/* MangleName - turn the last extension char into '_' (or append "_" / "._"),
   returning the character that was replaced so it can be restored. */
char far MangleName(char far *name)
{
    char far *ext;
    char last;
    int  n;

    last = 0;
    ext = FileExt(name);
    if (ext != 0) {
        if (lstrlen(ext) >= 3) {
            n = lstrlen(ext);
            last = ext[n - 1];
            ext[n - 1] = '_';
        } else {
            lstrcat(ext, szUnder);
        }
    } else {
        lstrcat(name, szDotUnder);
    }
    return last;
}

/* RestoreExt - GetExpandedName: restore the mangled extension character,
   matching the case of the file's first alphabetic character. */
void far RestoreExt(char far *name, unsigned char c)
{
    char far *ext;
    int len;

    ext = FileExt(name);
    if (ext == 0)
        return;
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z') {
        if (*name != 0) {
            do {
                if (*name >= 'A' && *name <= 'Z' || *name >= 'a' && *name <= 'z')
                    break;
                name++;
            } while (*name != 0);
        }
        /* Match the restored last character's case to the file name's own
           case (first alphabetic char): lower name -> lower char, and vice
           versa.  (USER IsCharLower is ordinal 436, IsCharUpper 435.) */
        if (IsCharLower(*name)) {
            if (c >= 'A' && c <= 'Z')
                c += 0x20;
        } else if (IsCharUpper(*name)) {
            if (c >= 'a' && c <= 'z')
                c -= 0x20;
        }
    }
    len = lstrlen(ext);
    if (len > 0 && (ext - 1)[len] == '_')
        ext[len - 1] = (char)c;
    if (ext[0] == 0 && ext[-1] == '.')
        ext[-1] = 0;
}

/* ---- INT 21h AH=57h: get (AL=0) / set (AL=1) a file's date & time by handle -
   Both return 0 on success, non-zero on a DOS error (CF set). */
static int far GetFileDateTime(int hf, WORD *pDate, WORD *pTime)
{
    WORD d = 0, t = 0, err = 0;

    _asm {
        mov  bx, hf
        mov  ax, 5700h
        int  21h
        adc  err, 0        /* err += carry: 1 on DOS error, 0 otherwise */
        mov  d, dx         /* DX = date, CX = time (valid only when CF clear) */
        mov  t, cx
    }
    *pDate = d;
    *pTime = t;
    return err;
}

static int far SetFileDateTime(int hf, WORD wD, WORD wT)
{
    WORD err = 0;

    _asm {
        mov  bx, hf
        mov  cx, wT
        mov  dx, wD
        mov  ax, 5701h
        int  21h
        adc  err, 0
    }
    return err;
}

int far CopyDateTime(int hfSrc, int hfDest)
{
    if (GetFileDateTime(hfSrc, &wDate, &wTime) != 0)
        return -1;
    if (SetFileDateTime(hfDest, wDate, wTime) != 0)
        return -2;
    return 1;
}
