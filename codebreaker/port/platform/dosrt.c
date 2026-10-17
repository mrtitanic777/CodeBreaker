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
#include "dosrt.h"
#undef open			/* (here the names are the C library's own) */
#undef creat
#undef read
#undef write
#undef lseek
#undef close
#undef unlink
#undef isatty
#undef fopen

FILE *dos_fopen(const char *name, const char *mode)
{
	char m[8];
	size_t n = 0;
	for (; *mode && n < 5; mode++)
		if (*mode != 't' && *mode != 'b') m[n++] = *mode;
	m[n++] = 'b';
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
	_setmode(_fileno(stdin), O_BINARY);
	_setmode(_fileno(stdout), O_BINARY);
	_setmode(_fileno(stderr), O_BINARY);
#endif
}
#if defined(_MSC_VER) && !defined(__clang__)
#pragma section(".CRT$XCU", read)
__declspec(allocate(".CRT$XCU")) static void (*dos_console_at_start)(void) = dos_console;
#else
__attribute__((constructor)) static void dos_console_at_start(void) { dos_console(); }
#endif

int dos_open(const char *name, int mode)
{
	static const int how[3] = { O_RDONLY, O_WRONLY, O_RDWR };
	return open(name, how[(mode & 3) < 3 ? mode & 3 : 2] | O_FLAGS);
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
