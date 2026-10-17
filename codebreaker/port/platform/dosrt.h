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
int dos_open(const char *name, int mode);
int dos_creat(const char *name, int mode);
int dos_read(int fd, void *buf, unsigned n);
int dos_write(int fd, const void *buf, unsigned n);
int32_t dos_lseek(int fd, int32_t off, int how);
int dos_close(int fd);
int dos_unlink(const char *name);
int dos_isatty(int fd);

/* the old C library had no text mode: a file is read and written as its bytes are, whatever the mode said */
#include <stdio.h>
#define fopen	dos_fopen
FILE *dos_fopen(const char *name, const char *mode);

/* movbytes(count, from, to): the bytes moved one at a time from the first up (REP MOVSB: an overlap
   repeats, as memmove would not) */
void movbytes(unsigned n, const char *from, char *to);

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

#endif
