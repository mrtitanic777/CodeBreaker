/* harness.c -- Lifted C differential test, runs in a 32-bit Windows (win95+ / WOW64).
 * Freestanding (-nostdlib): custom entry, only win95-safe kernel32 APIs, no CRT.
 * For each tested win95 function it compares THREE things on fixed inputs:
 *   SELF  = LIFT.DLL (our recompiled Lifted C)   vs   the embedded reference (Unicorn oracle)
 *   LIVE  = the REAL win95 DLL (loaded natively)  vs   that same reference
 * SELF proves the recompiled 32-bit Lifted C runs correctly; LIVE proves it against the actual
 * shipped DLL in a real 32-bit process (covers memory-touching code the host emulator can't). */
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef void *HMODULE_;
#define WINAPI __stdcall
#define IMP __declspec(dllimport)
IMP HMODULE_ WINAPI LoadLibraryA(const char *);
IMP void * WINAPI GetProcAddress(HMODULE_, const char *);
IMP void * WINAPI CreateFileA(const char *, u32, u32, void *, u32, u32, void *);
IMP int    WINAPI WriteFile(void *, const void *, u32, u32 *, void *);
IMP int    WINAPI CloseHandle(void *);
IMP void * WINAPI GetStdHandle(u32);
IMP void   WINAPI ExitProcess(u32);
#define GENERIC_WRITE 0x40000000u
#define CREATE_ALWAYS 2
#define STD_OUT ((u32)-11)

#include "tests.h"

/* call helpers: real win95 fns are __stdcall, our Lifted C is __cdecl. arities 0..4. */
typedef u32 (__cdecl  *C0)(void); typedef u32 (__cdecl  *C1)(u32);
typedef u32 (__cdecl  *C2)(u32,u32); typedef u32 (__cdecl *C3)(u32,u32,u32);
typedef u32 (__cdecl  *C4)(u32,u32,u32,u32);
typedef u32 (__stdcall *S0)(void); typedef u32 (__stdcall *S1)(u32);
typedef u32 (__stdcall *S2)(u32,u32); typedef u32 (__stdcall *S3)(u32,u32,u32);
typedef u32 (__stdcall *S4)(u32,u32,u32,u32);
static u32 callc(void *f, int n, const u32 *a){
  switch(n){case 0:return ((C0)f)();case 1:return ((C1)f)(a[0]);
  case 2:return ((C2)f)(a[0],a[1]);case 3:return ((C3)f)(a[0],a[1],a[2]);
  default:return ((C4)f)(a[0],a[1],a[2],a[3]);}}
static u32 calls(void *f, int n, const u32 *a){
  switch(n){case 0:return ((S0)f)();case 1:return ((S1)f)(a[0]);
  case 2:return ((S2)f)(a[0],a[1]);case 3:return ((S3)f)(a[0],a[1],a[2]);
  default:return ((S4)f)(a[0],a[1],a[2],a[3]);}}

static char buf[1<<16]; static int blen;
static void put(const char*s){ while(*s) buf[blen++]=*s++; }
static void putu(u32 v){ char t[12]; int i=0; put("0x");
  for(int s=28;s>=0;s-=4){int d=(v>>s)&0xf; t[i++]="0123456789abcdef"[d];} t[i]=0; put(t);}
static void putd(int v){ if(v<0){put("-");v=-v;} char t[12]; int i=0;
  if(!v){put("0");return;} while(v){t[i++]="0123456789"[v%10];v/=10;}
  while(i--)  {char c[2]; c[0]=t[i]; c[1]=0; put(c);} }

static void flush(void){                          /* (re)write the whole buffer to RESULTS.TXT + console */
  u32 wr; void *h=CreateFileA("RESULTS.TXT",GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
  if(h){ WriteFile(h,buf,blen,&wr,0); CloseHandle(h); }
  void *so=GetStdHandle(STD_OUT); if(so) WriteFile(so,buf,blen,&wr,0);
}
void __cdecl run(void){
  void *hl = LoadLibraryA("LIFT.DLL");
  int self_ok=0, self_tot=0, live_ok=0, live_tot=0, live_dll=0;
  put("== Lifted C differential harness ==\r\n");
  if(!hl){ put("FATAL: LIFT.DLL not found\r\n"); flush(); ExitProcess(1); }
  C0 zero = (C0)GetProcAddress(hl, "__zero");     /* reset register-file globals per call */

  /* PASS 1 -- SELF: recompiled Lifted C vs the embedded reference. Written out immediately so
   * it survives even if PASS 2 (loading a real DLL) faults on a non-win95 host. */
  for(int i=0;i<NTEST;i++){
    const TEST *t=&TESTS[i]; void *lf = GetProcAddress(hl, t->expname); int fok=1;
    for(int k=0;k<t->ncase;k++){
      const u32 *c=&t->cases[k*(t->arity+1)]; u32 exp=c[t->arity];
      if(zero) zero(); u32 lv = lf ? callc(lf, t->arity, c) : ~exp;
      self_tot++; if(lv==exp) self_ok++; else fok=0;
    }
    put(fok?"  SELF ok  ":"  SELF XX  "); put(t->mod); put(":"); put(t->expname); put("\r\n");
  }
  put("\r\nSELF (recompiled Lifted C vs reference): "); putd(self_ok); put("/"); putd(self_tot);
  put(" cases identical\r\n"); flush();

  /* PASS 2 -- LIVE: the real win95 DLL (loaded from the ISO copy) vs the reference. Only runs
   * meaningfully inside a win95 VM, where these DLLs load and relocate correctly. */
  put("\r\n-- LIVE (real win95 DLL) --\r\n");
  for(int i=0;i<NTEST;i++){
    const TEST *t=&TESTS[i]; void *lf = GetProcAddress(hl, t->expname);
    void *hr = LoadLibraryA(t->realdll); if(!hr){ continue; }
    void *rf = (void*)((char*)hr + t->rva); int fok=1; live_dll++;
    for(int k=0;k<t->ncase;k++){
      const u32 *c=&t->cases[k*(t->arity+1)]; u32 exp=c[t->arity];
      if(zero) zero(); u32 lv = lf ? callc(lf, t->arity, c) : ~exp;
      u32 rv = calls(rf, t->arity, c); live_tot++;
      if(rv==exp && rv==lv) live_ok++; else fok=0;
    }
    put(fok?"  LIVE ok  ":"  LIVE XX  "); put(t->mod); put(":"); put(t->expname); put("\r\n");
  }
  put("\r\nLIVE (real win95 DLL vs reference): "); putd(live_ok); put("/"); putd(live_tot);
  put(" cases identical  ["); putd(live_dll); put(" real DLLs loaded]\r\n"); flush();
  ExitProcess(0);
}
