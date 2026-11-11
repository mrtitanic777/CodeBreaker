/* geometry */
typedef struct tagPOINT { LONG x; LONG y; } POINT, *PPOINT, FAR *LPPOINT;
typedef struct tagRECT { LONG left; LONG top; LONG right; LONG bottom; } RECT, *PRECT;   // sixteen bytes
typedef struct _SIZEPAIR { POINT a; RECT r; BYTE flag; } SIZEPAIR;
#include <pshpack1.h>
typedef struct tagPACKED { BYTE b; DWORD d; WORD w; } PACKED;
#include <poppack.h>
typedef struct tagNATURAL { BYTE b; DWORD d; WORD w; } NATURAL;
typedef struct tagWITHARRAY { char name[MAX_NAME]; WORD w[4]; int count; LPSTR p; } WITHARRAY;
typedef struct tagWITHDOUBLE { BYTE b; double d; } WITHDOUBLE;
typedef struct tagUNKNOWNFIELD { SOMETYPE x; DWORD y; } UNKNOWNFIELD;
typedef struct tagWITHUNION { DWORD a; union { WORD w; DWORD d; } u; } WITHUNION;
typedef struct tagDIRECTIVE { DWORD a;
#ifdef FOO
  DWORD b;
#endif
} DIRECTIVE;
typedef struct tagMACRO { SOME_MACRO_FIELDS } MACRO;
typedef const RECT FAR *LPCRECT;
typedef struct tagTWOPTRS { RECT FAR *pr; const POINT *pp, **ppp; HWND h, h2; } TWOPTRS;
typedef struct tagNESTED2 { SIZEPAIR s; WORD tail; } NESTED2;
typedef struct tagBADARRAY { BYTE b[UNKNOWN_CONSTANT]; } BADARRAY;
typedef struct { DWORD anon; } ANONYMOUS;
