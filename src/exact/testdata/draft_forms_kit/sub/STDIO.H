int fopen();
int fclose(), fgetc();
#define getc(f) ((--(f)->_cnt >= 0) ? 0 : _filbuf(f))
#define putc(c,f) ((--(f)->_cnt >= 0) ? 0 : _flsbuf(c,f))
extern int printf ( );
