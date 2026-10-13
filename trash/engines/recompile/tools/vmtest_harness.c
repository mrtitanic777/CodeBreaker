/* vmtest_harness.c -- Lifted C differential test, runs in a 32-bit Windows (win95+ / WOW64).
 * Freestanding (-nostdlib): custom entry, only win95-safe kernel32 APIs, no CRT.
 *
 * For each tested win95 function, on fixed inputs, it compares:
 *   SELF = LIFT.DLL (our recompiled Lifted C) vs an embedded reference.
 * The reference is that function's output computed by EMULATING win95's real shipped bytes
 * (the Unicorn oracle). So SELF passing = the recompiled Lifted C, running natively on this OS,
 * computes exactly what Microsoft's machine code computes. That IS the sound comparison against
 * the real win95 code -- no need to call the live internal functions (they are not entry points
 * and are unsafe to invoke standalone; the emulated reference already carries their behaviour). */
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

typedef u32 (__cdecl *C0)(void); typedef u32 (__cdecl *C1)(u32);
typedef u32 (__cdecl *C2)(u32,u32); typedef u32 (__cdecl *C3)(u32,u32,u32);
typedef u32 (__cdecl *C4)(u32,u32,u32,u32);
static u32 callc(void *f,int n,const u32 *a){ switch(n){case 0:return((C0)f)();case 1:return((C1)f)(a[0]);
  case 2:return((C2)f)(a[0],a[1]);case 3:return((C3)f)(a[0],a[1],a[2]);default:return((C4)f)(a[0],a[1],a[2],a[3]);} }

static char buf[1<<16]; static int blen;
static void put(const char*s){ while(*s) buf[blen++]=*s++; }
static void putd(int v){ if(v<0){put("-");v=-v;} char t[12]; int i=0;
  if(!v){put("0");return;} while(v){t[i++]="0123456789"[v%10];v/=10;}
  while(i--){ char c[2]; c[0]=t[i]; c[1]=0; put(c); } }

void __cdecl run(void){
  void *hl = LoadLibraryA("LIFT.DLL");
  int ok=0, tot=0, fnok=0, fns=0;
  put("== Lifted C differential harness (SELF: recompiled Lifted C vs the real-win95-bytes"
      " reference) ==\r\n");
  if(!hl){ put("FATAL: LIFT.DLL failed to load\r\n"); goto out; }
  C0 zero = (C0)GetProcAddress(hl, "__zero");        /* reset the register-file globals per call */
  for(int i=0;i<NTEST;i++){
    const TEST *t=&TESTS[i]; void *lf=GetProcAddress(hl,t->expname); int fok=1;
    for(int k=0;k<t->ncase;k++){
      const u32 *c=&t->cases[k*(t->arity+1)]; u32 exp=c[t->arity];
      if(zero) zero();
      u32 v = lf ? callc(lf,t->arity,c) : ~exp;
      tot++; if(v==exp) ok++; else fok=0;
    }
    fns++; if(fok) fnok++;
    put(fok?"  ok  ":"  XX  "); put(t->mod); put(":"); put(t->expname); put("\r\n");
  }
  put("\r\nSELF: "); putd(ok); put("/"); putd(tot); put(" cases identical; ");
  putd(fnok); put("/"); putd(fns); put(" functions fully identical to Windows 95's own code.\r\n");
out:;
  u32 wr; void *h=CreateFileA("RESULTS.TXT",GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);
  if(h){ WriteFile(h,buf,blen,&wr,0); CloseHandle(h); }
  void *so=GetStdHandle(STD_OUT); if(so) WriteFile(so,buf,blen,&wr,0);
  ExitProcess(0);
}
