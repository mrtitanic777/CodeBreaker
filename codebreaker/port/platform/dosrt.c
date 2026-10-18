/* dosrt.c -- the calls of a DOS program's own runtime, over the C library of today (see dosrt.h). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#define O_FLAGS	O_BINARY
#else
#include <unistd.h>
#define O_FLAGS	0
#endif
#define DOSRT_IMPL			/* (today's O_ numbers here, not the old ones) */
#include "dosrt.h"
#include "dosrt_config.h"		/* (the old runtime's ways: written by the port from its profile) */
#ifndef DOSRT_TEXT_MODE
#define DOSRT_TEXT_MODE 0		/* (1: the old library had text mode - \n written \r\n, as Microsoft C 3's) */
#endif
#undef open			/* (here the names are the C library's own) */
#undef creat
#undef read
#undef write
#undef lseek
#undef close
#undef unlink
#undef isatty
#undef fopen
#undef exit

void dos_exit(int code)
{
	exit(code & 0377);
}

FILE *dos_fopen(const char *name, const char *mode)
{
	char m[8];
	size_t n = 0;
	int bin = !DOSRT_TEXT_MODE;			/* (no text mode in the old library: every file binary) */
	for (; *mode && n < 5; mode++) {
		if (*mode == 'b') bin = 1;
		else if (*mode == 't') bin = 0;
		else m[n++] = *mode;
	}
	m[n++] = bin ? 'b' : 't';			/* (a text file: \n is \r\n in it, as DOS's library made it) */
	m[n] = 0;
	return fopen(name, m);
}

/* the console as DOS's was: what the program writes goes out as it is written (no buffer, no line ends
   translated - the program writes its own carriage returns), before main */
static void dos_console(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
#ifdef _WIN32
	if (!DOSRT_TEXT_MODE) {			/* (a library with text mode wrote \r\n for \n to the console too) */
		_setmode(_fileno(stdin), O_BINARY);
		_setmode(_fileno(stdout), O_BINARY);
		_setmode(_fileno(stderr), O_BINARY);
	}
#endif
}
#if defined(_MSC_VER) && !defined(__clang__)
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void (*dos_console_at_start)(void) = dos_console;
#else
__attribute__((constructor)) static void dos_console_at_start(void) { dos_console(); }
#endif

/* mode: 0 read, 1 write, 2 both - and Microsoft C's own bits above them, as its fcntl.h numbered them (8 append,
   100h create, 200h truncate, 400h exclusive; 4000h text, 8000h binary: every file binary here); a third
   argument, the new file's permission, when it is created */
int dos_open(const char *name, int mode, ...)
{
	static const int how[3] = { O_RDONLY, O_WRONLY, O_RDWR };
	int f = how[(mode & 3) < 3 ? mode & 3 : 2] | O_FLAGS;
	if (mode & 0x0008) f |= O_APPEND;
	if (mode & 0x0100) f |= O_CREAT;
	if (mode & 0x0200) f |= O_TRUNC;
	if (mode & 0x0400) f |= O_EXCL;
#ifdef _WIN32
	if (DOSRT_TEXT_MODE && !(mode & 0x8000)) f = (f & ~O_BINARY) | O_TEXT;	/* (text, unless 8000h: binary) */
#endif
	return (f & O_CREAT) ? open(name, f, 0666) : open(name, f);
}

int dos_creat(const char *name, int mode)
{
	(void)mode;			/* (DOS made it with no attributes, whatever was asked) */
	return open(name, O_CREAT | O_TRUNC | O_RDWR | O_FLAGS, 0666);
}

int dos_read(int fd, void *buf, unsigned n)	{ return read(fd, buf, n); }
int dos_write(int fd, const void *buf, unsigned n)	{ return write(fd, buf, n); }
int32_t dos_lseek(int fd, int32_t off, int how)	{ return (int32_t)lseek(fd, off, how); }
int dos_close(int fd)			{ return close(fd); }
int dos_unlink(const char *name)		{ return unlink(name); }
int dos_isatty(int fd)			{ return isatty(fd) ? -1 : 0; }

void movbytes(unsigned n, const char *from, char *to)
{
	while (n--) *to++ = *from++;
}

void fillb(int c, void *p, unsigned n)	{ memset(p, c, n); }

int getdrive(void)
{
#ifdef _WIN32
	return _getdrive() - 1;
#else
	return 2;
#endif
}

/* the line as DOS read it: its characters, the carriage return, the line feed after */
static char line[132];
static int left, next;

int kbdchar(void)
{
	if (left == 0) {
		int n = 0, c;
		while ((c = getchar()) != EOF && c != '\n')
			if (n < 127) line[n++] = (char)c;
		if (c == EOF && n == 0) return -1;
		if (n && line[n - 1] == '\r') n--;
		if (!isatty(0)) {		/* (DOS echoed the line it read, and the return; a terminal echoes */
			fwrite(line, 1, n, stdout);	/*  of itself) - then the line feed kbdchar wrote */
			fputs("\r\n", stdout);
		}
		line[n++] = '\r';
		line[n++] = '\n';
		left = n;
		next = 0;
	}
	left--;
	return line[next++] & 0377;
}

void kbdnew(void)	{ left = 0; }

/* the memory above 64K: DOSRT_CORE_PAGES pages (default 896, what a 640K machine left a small
   program), held in the heap */
uint16_t ipnCore;
static char *core;

void coremax(void)
{
	const char *s = getenv("DOSRT_CORE_PAGES");
	ipnCore = s ? (unsigned)atoi(s) : 896;
	core = ipnCore ? calloc(ipnCore, 512) : 0;
	if (core == 0) ipnCore = 0;
}

void corein(unsigned ipn, char *buf)	{ memcpy(buf, core + (size_t)ipn * 512, 512); }
void coreout(unsigned ipn, char *buf)	{ memcpy(core + (size_t)ipn * 512, buf, 512); }

/* ---- DOS's own calls: intdos and INT 21h ---- */
#include <time.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif

/* the date and the time DOS gives: the clock's, or DOSRT_DATE ("1989-01-16 12:00:00") to have the same as
   another run */
static void dos_clock(int *y, int *mo, int *d, int *h, int *mi, int *s)
{
	const char *e = getenv("DOSRT_DATE");
	if (e && sscanf(e, "%d-%d-%d %d:%d:%d", y, mo, d, h, mi, s) == 6) return;
	time_t t = time(0);
	struct tm *tm = localtime(&t);
	*y = tm->tm_year + 1900; *mo = tm->tm_mon + 1; *d = tm->tm_mday;
	*h = tm->tm_hour; *mi = tm->tm_min; *s = tm->tm_sec;
}

/* r: rax rbx rcx rdx rsi rdi, then the flags (bit 0 the carry) - the registers as INT 21h had them */
void dos_int21(uint64_t *r)
{
	unsigned ah = (r[0] >> 8) & 0377, bx = r[1] & 0xFFFF, cx = r[2] & 0xFFFF;
	int y, mo, d, h, mi, s, n;
	r[6] &= ~(uint64_t)1;
	switch (ah) {
	case 0x3F:					/* read: bx the handle, cx bytes, ds:dx the buffer */
		n = dos_read(bx, (void *)(uintptr_t)r[3], cx);
		goto result;
	case 0x40:					/* write */
		n = dos_write(bx, (const void *)(uintptr_t)r[3], cx);
	result:
		if (n < 0) { r[0] = (r[0] & ~(uint64_t)0xFFFF) | 5; r[6] |= 1; }
		else r[0] = (r[0] & ~(uint64_t)0xFFFF) | (unsigned)n;
		return;
	case 0x42: {					/* seek: bx the handle, cx:dx where, al from where; dx:ax */
		int32_t at = dos_lseek(bx, (int32_t)((uint32_t)cx << 16 | (r[3] & 0xFFFF)), (int)(r[0] & 0377));
		if (at < 0) { r[0] = (r[0] & ~(uint64_t)0xFFFF) | 6; r[6] |= 1; return; }
		r[0] = (r[0] & ~(uint64_t)0xFFFF) | ((uint32_t)at & 0xFFFF);
		r[3] = (r[3] & ~(uint64_t)0xFFFF) | ((uint32_t)at >> 16);
		return;
	}
	case 0x48:					/* memory: bx paragraphs - the largest there is said when it is
							   more than there is (DOSRT_HEAP_AVAIL); a block is "given" (none is
							   needed: what the program reads of it is the number) */
		if (bx > heap_avail(0)) {
			r[1] = (r[1] & ~(uint64_t)0xFFFF) | heap_avail(0);
			r[0] = (r[0] & ~(uint64_t)0xFFFF) | 8;
			r[6] |= 1;
		} else
			r[0] = (r[0] & ~(uint64_t)0xFFFF) | 0x8000;
		return;
	case 0x2A:					/* the date: cx the year, dh the month, dl the day */
		dos_clock(&y, &mo, &d, &h, &mi, &s);
		r[2] = (r[2] & ~(uint64_t)0xFFFF) | (unsigned)y;
		r[3] = (r[3] & ~(uint64_t)0xFFFF) | (unsigned)(mo << 8 | d);
		return;
	case 0x2C:					/* the time: ch the hour, cl the minute, dh the second */
		dos_clock(&y, &mo, &d, &h, &mi, &s);
		r[2] = (r[2] & ~(uint64_t)0xFFFF) | (unsigned)(h << 8 | mi);
		r[3] = (r[3] & ~(uint64_t)0xFFFF) | (unsigned)(s << 8);
		return;
	}
	fprintf(stderr, "dosrt: INT 21h function %02Xh is not in the platform layer\n", ah);
	r[6] |= 1;
}

int intdos(union REGS *in, union REGS *out)
{
	uint64_t r[7] = { in->x.ax, in->x.bx, in->x.cx, in->x.dx, in->x.si, in->x.di, 0 };
	dos_int21(r);
	out->x.ax = (uint16_t)r[0]; out->x.bx = (uint16_t)r[1]; out->x.cx = (uint16_t)r[2];
	out->x.dx = (uint16_t)r[3]; out->x.si = (uint16_t)r[4]; out->x.di = (uint16_t)r[5];
	out->x.cflag = (uint16_t)(r[6] & 1);
	return out->x.ax;
}

unsigned char dos_nullseg[256];

unsigned char dos_ctype[257] = {
	0,							/* EOF */
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x28,0x28,0x28,0x28,0x28,0x20,0x20,	/* 00 */
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,	/* 10 */
	0x48,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,	/* 20 */
	0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x10,0x10,0x10,0x10,0x10,0x10,	/* 30 */
	0x10,0x81,0x81,0x81,0x81,0x81,0x81,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,	/* 40 */
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x10,0x10,0x10,0x10,0x10,	/* 50 */
	0x10,0x82,0x82,0x82,0x82,0x82,0x82,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,	/* 60 */
	0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x10,0x10,0x10,0x10,0x20,	/* 70 */
};

/* the PSP: 256 bytes as DOS laid them out, the command line at 80h (its length, the text, a return) */
uint16_t _psp = 0x0100;
static unsigned char psp[256];
static int psp_made;

static void make_psp(void)
{
	char tail[200] = "";
	size_t n = 0;
#ifdef _WIN32
	const char *c = GetCommandLineA();		/* (what follows the program's own name) */
	if (*c == '"') { c++; while (*c && *c != '"') c++; if (*c) c++; }
	else while (*c && *c != ' ' && *c != '\t') c++;
	n = strlen(c) < 126 ? strlen(c) : 126;
	memcpy(tail, c, n);
#else
	FILE *f = fopen("/proc/self/cmdline", "rb");
	if (f) {
		int ch, first = 1;
		while ((ch = fgetc(f)) != EOF && n < 126) {
			if (first) { if (ch == 0) first = 0; continue; }
			tail[n++] = ch ? ch : ' ';
		}
		fclose(f);
		if (n && tail[n - 1] == ' ') n--;
		if (n) { memmove(tail + 1, tail, n); tail[0] = ' '; n++; }
	}
#endif
	psp[0] = 0xCD; psp[1] = 0x20;
	psp[0x80] = (unsigned char)n;
	memcpy(psp + 0x81, tail, n);
	psp[0x81 + n] = '\r';
	psp_made = 1;
}

void *dos_far(unsigned seg, unsigned off)
{
	if (seg == _psp) {
		if (!psp_made) make_psp();
		return psp + (off & 0xFF);
	}
	fprintf(stderr, "dosrt: a far pointer into segment %04Xh, which is not in the platform layer\n", seg);
	return 0;
}

/* what is left of the heap, in paragraphs: DOSRT_HEAP_AVAIL, or as much as a small program had under DOS */
unsigned heap_avail(int which)
{
	const char *e = getenv("DOSRT_HEAP_AVAIL");
	(void)which;
	return e ? (unsigned)strtoul(e, 0, 0) : 0x6000;	/* (24K paragraphs' worth: a small machine's) */
}

void terminate(int code)
{
	fflush(0);
	dos_exit(code);
}

char *dos_gets(char *buf)
{
	if (!fgets(buf, 256, stdin)) return 0;	/* (the line without its end, as gets gave it) */
	buf[strcspn(buf, "\r\n")] = 0;
	return buf;
}

char *dos_itoa(int v, char *buf, int radix)
{
	char tmp[34], *p = tmp;
	unsigned u = (radix == 10 && v < 0) ? (unsigned)-v : (unsigned)v;
	do { int d = u % radix; *p++ = d < 10 ? '0' + d : 'a' + d - 10; u /= radix; } while (u);
	char *o = buf;
	if (radix == 10 && v < 0) *o++ = '-';
	while (p > tmp) *o++ = *--p;
	*o = 0;
	return buf;
}

int dos_strcmpi(const char *a, const char *b)
{
	for (;; a++, b++) {
		int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);
		if (x != y || !x) return x - y;
	}
}

int dos_memicmp(const void *a, const void *b, size_t n)
{
	const unsigned char *p = a, *q = b;
	for (; n; n--, p++, q++) {
		int x = tolower(*p), y = tolower(*q);
		if (x != y) return x - y;
	}
	return 0;
}

static size_t far_used, far_limit;

void *dos_fmalloc(size_t n)
{
	size_t *p;
	if (!far_limit) {
		const char *e = getenv("DOSRT_FAR_HEAP");
		far_limit = e ? (size_t)strtoul(e, 0, 0) : 0x80000;	/* (512K: what a 640K machine left) */
	}
	if (n > far_limit - far_used)
		return 0;
	if (!(p = malloc(n + 16)))
		return 0;
	p[0] = n;
	far_used += n;
	return (char *)p + 16;
}

void dos_ffree(void *q)
{
	size_t *p;
	if (!q) return;
	p = (size_t *)((char *)q - 16);
	far_used -= p[0];
	free(p);
}

int dos_flushall(void)
{
	fflush(NULL);				/* (every stream written out; the old one said how many were open) */
	return 0;
}

int dos_sopen(const char *name, int mode, int share, ...)
{
	(void)share;				/* (no other program shares the file here) */
	return dos_open(name, mode, 0666);
}

int dos_setmode(int fd, int mode)
{
#ifdef _WIN32
	int was = _setmode(fd, (mode & 0x4000) ? O_TEXT : O_BINARY);
	return was == O_TEXT ? 0x4000 : 0x8000;
#else
	(void)fd; (void)mode;
	return 0x8000;
#endif
}

unsigned dos_freect(unsigned size)
{
	return heap_avail(0) / (size ? size : 2);
}

jmp_buf *dos_jmpbuf(void *old)
{
	static struct { void *old; jmp_buf b; } bufs[16];
	int i;
	for (i = 0; i < 16 && bufs[i].old; i++)
		if (bufs[i].old == old) return &bufs[i].b;
	if (i == 16) abort();			/* (more buffers than any old program had) */
	bufs[i].old = old;
	return &bufs[i].b;
}
