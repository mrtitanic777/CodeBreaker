#define _UPPER 0x1
#define _LOWER 0x2
#define _DIGIT 0x4
#define _SPACE 0x8
extern char _ctype[];
#define isalpha(c)  ( (_ctype+1)[c] & (_UPPER|_LOWER) )
#define isdigit(c)  ( (_ctype+1)[c] & _DIGIT )
#define isspace(c)  ( (_ctype+1)[c] & _SPACE )
