/*
**  LZEXPAND.H  -  public interface to the LZSS (SZDD) file-expansion library.
**
**  This is the header an application includes to use LZEXPAND.DLL: the LZ error
**  codes and the exported entry points.  (In the shipping SDK the primitive
**  types and OFSTRUCT come from WINDOWS.H; they are spelled out here so the
**  reconstruction is self-contained.)
*/
#ifndef _INC_LZEXPAND
#define _INC_LZEXPAND

/* ---- primitive types (normally supplied by WINDOWS.H) --------------------- */
#ifndef _INC_WINDOWS
typedef unsigned int   WORD;
typedef unsigned long  DWORD;
typedef unsigned char  BYTE;
typedef int            HFILE;
typedef BYTE far      *LPSTR;

struct OFSTRUCT {
    BYTE  cBytes;
    BYTE  fFixedDisk;
    WORD  nErrCode;
    WORD  Reserved1;
    WORD  Reserved2;
    BYTE  szPathName[128];
};
#endif  /* _INC_WINDOWS */

/* ---- LZ error codes ------------------------------------------------------- */
#define LZERROR_BADINHANDLE   (-1)   /* the source handle was invalid            */
#define LZERROR_BADOUTHANDLE  (-2)   /* the destination handle was invalid       */
#define LZERROR_READ          (-3)   /* the source was corrupt                   */
#define LZERROR_WRITE         (-4)   /* out of space on the destination          */
#define LZERROR_GLOBALLOC     (-5)   /* insufficient memory for the buffers      */
#define LZERROR_GLOBLOCK      (-6)   /* bad global handle                        */
#define LZERROR_BADVALUE      (-7)   /* an input parameter was out of range      */
#define LZERROR_UNKNOWNALG    (-8)   /* unrecognized compression algorithm       */

/* ---- exported entry points ------------------------------------------------ */
int   far pascal LZStart(void);
void  far pascal LZDone(void);
long  far pascal LZCopy(HFILE hfSource, HFILE hfDest);
long  far pascal CopyLZFile(HFILE hfSource, HFILE hfDest);
int   far pascal GetExpandedName(LPSTR lpszSource, LPSTR lpszBuffer);
HFILE far pascal LZOpenFile(LPSTR lpszFile, struct OFSTRUCT far *lpOf, WORD style);
HFILE far pascal LZInit(HFILE hfSrc);
long  far pascal LZSeek(HFILE hf, long lOffset, int iOrigin);
int   far pascal LZRead(HFILE hf, void far *lpBuffer, int cbRead);
void  far pascal LZClose(HFILE hf);

#endif  /* _INC_LZEXPAND */
