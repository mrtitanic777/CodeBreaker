/*
 * ctype.h as the sources of Microsoft C 3.00 saw it - written here, not Microsoft's text.
 *
 * The compiler's own programs were compiled with a <ctype.h> whose table is a plain char (as in the 3.00.04 beta
 * of May 1984), not the unsigned char of the 3.00 diskettes: the bytes show it where a class bit is the sign bit,
 * (_ctype_+1)[c] & 0x80 being "mov al,..; cwde; test ax,80h" and not a TEST of the byte.
 */
#define _UPPER      0x1
#define _LOWER      0x2
#define _DIGIT      0x4
#define _SPACE      0x8
#define _PUNCT      0x10
#define _CONTROL    0x20
#define _BLANK      0x40
#define _HEX        0x80

extern char _ctype_[];
extern char _ctype[];

#define isalpha(c)  ((_ctype_+1)[c] & (_UPPER|_LOWER))
#define isupper(c)  ((_ctype_+1)[c] & _UPPER)
#define islower(c)  ((_ctype_+1)[c] & _LOWER)
#define isdigit(c)  ((_ctype_+1)[c] & _DIGIT)
#define isxdigit(c) ((_ctype_+1)[c] & _HEX)
#define isspace(c)  ((_ctype_+1)[c] & _SPACE)
#define ispunct(c)  ((_ctype_+1)[c] & _PUNCT)
#define isalnum(c)  ((_ctype_+1)[c] & (_UPPER|_LOWER|_DIGIT))
#define isprint(c)  ((_ctype_+1)[c] & (_BLANK|_PUNCT|_UPPER|_LOWER|_DIGIT))
#define isgraph(c)  ((_ctype_+1)[c] & (_PUNCT|_UPPER|_LOWER|_DIGIT))
#define iscntrl(c)  ((_ctype_+1)[c] & _CONTROL)
#define isascii(c)  ((unsigned)(c) < 0x80)
#define toascii(c)  ((c) & 0x7f)
#define toupper(c)  (islower(c) ? (c) - ('a' - 'A') : (c))
#define tolower(c)  (isupper(c) ? (c) + ('a' - 'A') : (c))
