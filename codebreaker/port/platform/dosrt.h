/* dosrt.h -- the calls of a DOS program's own runtime, over the C library of today.

   The old programs called DOS through small assembler routines of their C library: handles opened,
   read, written and moved by number; a line typed at the keyboard; the memory above the program's 64K
   as pages. Here each is the same call, done the portable way. The names are the old ones, so the
   program's source keeps reading as it did; on a system whose library has an open() of its own the old
   name is a macro for ours. */
#ifndef DOSRT_H
#define DOSRT_H

#include <stdint.h>

/* handles: open(name, 0 read / 1 write / 2 both), creat(name, mode) a new empty file; every file binary,
   as DOS's were. -1 when it cannot be done */
#define open	dos_open
#define creat	dos_creat
#define read	dos_read
#define write	dos_write
#define lseek	dos_lseek
#define close	dos_close
#define unlink	dos_unlink
#define isatty	dos_isatty
int dos_open(const char *name, int mode, ...);
int dos_creat(const char *name, int mode);
int dos_read(int fd, void *buf, unsigned n);
int dos_write(int fd, const void *buf, unsigned n);
int32_t dos_lseek(int fd, int32_t off, int how);
int dos_close(int fd);
int dos_unlink(const char *name);
int dos_isatty(int fd);

/* the code a program ends with: DOS kept its low byte (exit(272) left 16 for ERRORLEVEL) */
#include <stdlib.h>
#define exit	dos_exit
void dos_exit();			/* (Lattice's programs call exit() with none: an old declaration takes either) */

/* the old C library had no text mode: a file is read and written as its bytes are, whatever the mode said */
#include <stdio.h>
#define fopen	dos_fopen
FILE *dos_fopen(const char *name, const char *mode);

/* movbytes(count, from, to): the bytes moved one at a time from the first up (REP MOVSB: an overlap
   repeats, as memmove would not) */
void movbytes(unsigned n, const char *from, char *to);

/* fillb(c, p, n): n bytes at p all made c (REP STOSB) */
void fillb(int c, void *p, unsigned n);

/* the current drive, 0 for A: */
int getdrive(void);

/* the next character of a line typed at the keyboard (DOS function 0Ah): the line's characters, a
   carriage return, a line feed; -1 at the end of the input. kbdnew: the rest of the line forgotten */
int kbdchar(void);
void kbdnew(void);

/* the memory above the program's 64K, as pages of 512 bytes: coremax says how many there are
   (ipnCore), corein and coreout copy one */
extern uint16_t ipnCore;
void coremax(void);
void corein(unsigned ipn, char *buf);
void coreout(unsigned ipn, char *buf);

/* DOS's own calls, as Microsoft C's dos.h gave them: the registers in a union, INT 21h by intdos() (the
   date and the time, and whatever dos_int21 knows); the same block for assembly language (dos_int21: rax rbx
   rcx rdx rsi rdi, then the carry) */
union REGS {
	struct { uint16_t ax, bx, cx, dx, si, di, cflag; } x;
	struct { uint8_t al, ah, bl, bh, cl, ch, dl, dh; } h;
};
int intdos(union REGS *in, union REGS *out);
void dos_int21(uint64_t *r);

/* a far pointer built of a segment and an offset: the program's PSP (its segment _psp, the command line at
   80h, as DOS laid it out) is the one kind there is now */
extern uint16_t _psp;

/* what a NULL pointer read on DOS: the start of the data segment (Microsoft C's NULL segment, zeros where a
   string is looked for) - the assembly routines are given this for a NULL address */
extern unsigned char dos_nullseg[256];
void *dos_far(unsigned seg, unsigned off);

/* the old library's others: the far heap (one heap now), what is left of it (DOSRT_HEAP_AVAIL paragraphs,
   what the original found), the end through the library's exit, a line of the standard input, numbers
   and names compared without case */
/* the far heap is as big as a DOS machine's was (DOSRT_FAR_HEAP bytes, 512K unless said): a program that
   allocates until it is refused - to count what memory there is - is refused here too, not given the host's */
#define _fmalloc(n)	dos_fmalloc(n)
#define _ffree(p)	dos_ffree(p)
void *dos_fmalloc(size_t n);
void dos_ffree(void *p);
#define _fstrcpy	strcpy
#define _fstrlen	strlen
#define _fmemcpy	memcpy
unsigned heap_avail(int which);
void terminate(int code);
/* the old library's table of the characters' kinds (_ctype; indexed from -1, EOF): 1 upper, 2 lower, 4 digit,
   8 space, 10h punctuation, 20h control, 40h blank, 80h hex digit - as Microsoft C numbered them */
#define _ctype	dos_ctype
extern unsigned char dos_ctype[257];
#define gets	dos_gets
char *dos_gets(char *buf);
#define itoa	dos_itoa
char *dos_itoa(int v, char *buf, int radix);
#define strcmpi	dos_strcmpi
#define stricmp	dos_strcmpi
int dos_strcmpi(const char *a, const char *b);
#define memicmp	dos_memicmp
int dos_memicmp(const void *a, const void *b, size_t n);

/* Microsoft C's fcntl.h numbers, which open() and sopen() are given (the platform layer makes them today's) */
#ifndef DOSRT_IMPL
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR
#undef O_APPEND
#undef O_CREAT
#undef O_TRUNC
#undef O_EXCL
#undef O_TEXT
#undef O_BINARY
#define O_RDONLY	0x0000
#define O_WRONLY	0x0001
#define O_RDWR	0x0002
#define O_APPEND	0x0008
#define O_CREAT	0x0100
#define O_TRUNC	0x0200
#define O_EXCL	0x0400
#define O_TEXT	0x4000
#define O_BINARY	0x8000
#undef SH_COMPAT
#undef SH_DENYRW
#undef SH_DENYWR
#undef SH_DENYRD
#undef SH_DENYNO
#define SH_COMPAT	0x00
#define SH_DENYRW	0x10
#define SH_DENYWR	0x20
#define SH_DENYRD	0x30
#define SH_DENYNO	0x40
#endif
/* sopen(name, mode, share[, permission]): open() - the sharing is DOS's business; setmode(handle, O_TEXT or
   O_BINARY): the mode it had; _freect(size): how many blocks of size bytes the near heap had room for */
/* a pointer kept in two bytes, in a record the program keeps as bytes (port/oldlayout.py): its handle, and back.
   Below 8000h a value stays itself (a number the program put where a pointer was) */
uint16_t dos_ph(const void *p);
void *dos_hp(unsigned h);

/* Lattice C's memory calls (MS C 1.0x's library): allmem() takes the memory there is for the heap (0: done),
   getmem(n) a block, rlsmem(p, n) gives it back (0: done), sizmem() what is left */
#define allmem	dos_allmem
#define getmem	dos_getmem
#define rlsmem	dos_rlsmem
#define sizmem	dos_sizmem
#define getml	dos_getml
#define rlsml	dos_rlsml
int dos_allmem(void);
char *dos_getmem(unsigned n);
int dos_rlsmem(void *p, unsigned n);
long dos_sizmem(void);
char *dos_getml(long n);
int dos_rlsml(void *p, long n);
#define flushall	dos_flushall
int dos_flushall(void);
#define sopen	dos_sopen
#define setmode	dos_setmode
#define _freect	dos_freect
int dos_sopen(const char *name, int mode, int share, ...);
int dos_setmode(int fd, int mode);
unsigned dos_freect(unsigned size);

/* setjmp(buf) and longjmp(buf[, value]) on the program's own buffer (its words were the registers): a buffer
   of today's for each of the old ones; a longjmp without a value gave what was in AX - not 0 */
#include <setjmp.h>
jmp_buf *dos_jmpbuf(void *old);
#define dos_setjmp(b)	setjmp(*dos_jmpbuf(b))
#define dos_longjmp(...)	dos_longjmp_(__VA_ARGS__, 1, 0)
#define dos_longjmp_(b, v, ...)	longjmp(*dos_jmpbuf(b), (v))

#endif
