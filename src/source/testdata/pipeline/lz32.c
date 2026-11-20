/* lz32.dll -- reconstructed source.
 *
 * STATUS, generated 2026-11-12 by CodeBreaker (verifier settings 342fa094a98c).
 * 39 functions:
 *   VERIFIED                        17   43.6%
 *   VERIFIED (weak)                  0    0.0%
 *   IDENTIFIED (runtime library)    19   48.7%
 *   UNVERIFIED                       3    7.7%
 *   NOT JUDGED                       0    0.0%
 *   DOES NOT COMPILE                 0    0.0%
 *   NO SOURCE                        0    0.0%
 *
 * Builds as a whole module: not built here (run `codebreaker build lz32.dll`).
 * Functions are compiled and checked one at a time; the module's data (globals, tables,
 * resources) is referred to by its address in the original unless the whole module is built.
 * Runs in place of the original: NOT TESTED.
 *
 * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original in an x86 emulator:
 * same return value, same calls out of the module with the same arguments, same memory changed,
 * same stack and saved registers, on every input tried. It is evidence, not proof. Names of
 * internal functions (sub_...) and locals are placeholders; no original names are claimed.
 * Written in after judging, changing no code: the SDK's names for constants passed to
 * Windows (with their values in lz32.h), and names for locals, parameters and globals
 * taken from the Windows functions they are passed to or hold the result of.
 * IDENTIFIED = the function's bytes are those of a function in a library that came with the
 * compiler; it is named after that function. It is reconstructed and judged all the same.
 */

#include "lz32.h"

/* [UNVERIFIED] sub_001000  @ bfea1000 -- ends differently (one side faults or runs away) */
int4 sub_001000(int4 a1,int4 a2,int4 a3)

{
  char cVar1;
  int4 iVar2;
  
  cVar1 = cRambfea2016;
  if (a2 == 1) {
    LOCK();
    cRambfea2016 = '\0';
    UNLOCK();
    if (cVar1 != '\0') {
      sub_0013FA(a1);
      iVar2 = sub_001057(-0x4015e000,-0x4015dff3);
      if (iVar2 == 0) {
        return 0;
      }
    }
  }
  return 1;
}

/* [VERIFIED] sub_001057  @ bfea1057 -- 100% of it exercised */
int4 sub_001057(int4 a1,int4 a2)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_41(-0x4015dfe8,-0x4015efc4,0x77c1,a1,a2);
  return iVar1;
}

/* [IDENTIFIED (runtime library)] thunk stub  @ bfea1075 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 thunk stub(int4 a1,int4 a2,int4 a3)

{
  int4 iVar1;
  
  iVar1 = sub_00140C();
  sub_001400(iVar1);
  (*pcRambfea106d)((xunknown2)a3);
  sub_001424();
  iVar1 = sub_001412();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_001077  @ bfea1077 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_001077(int4 a1,int4 a2,int4 a3)

{
  int4 iVar1;
  
  iVar1 = sub_00140C();
  sub_001400(iVar1);
  (*pcRambfea106d)((xunknown2)a3);
  sub_001424();
  iVar1 = sub_001412();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_0010A5  @ bfea10a5 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010A5(int4 a1)

{
  int2 iVar1;
  
  iVar1 = (*pcRambfea106d)((xunknown2)a1);
  return (int4)iVar1;
}

/* [IDENTIFIED (runtime library)] sub_0010A7  @ bfea10a7 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010A7(int4 a1)

{
  int2 iVar1;
  
  iVar1 = (*pcRambfea106d)((xunknown2)a1);
  return (int4)iVar1;
}

/* [IDENTIFIED (runtime library)] sub_0010BD  @ bfea10bd -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010BD(int4 a1,int4 a2,int4 a3)

{
  xunknown8 xVar1;
  
  xVar1 = (*pcRambfea106d)((xunknown2)a3);
  return (uint4)xVar1 & 0xffff | (int4)((uint8)xVar1 >> 0x20) << 0x10;
}

/* [IDENTIFIED (runtime library)] sub_0010BF  @ bfea10bf -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010BF(int4 a1,int4 a2,int4 a3)

{
  xunknown8 xVar1;
  
  xVar1 = (*pcRambfea106d)((xunknown2)a3);
  return (uint4)xVar1 & 0xffff | (int4)((uint8)xVar1 >> 0x20) << 0x10;
}

/* [IDENTIFIED (runtime library)] sub_0010E2  @ bfea10e2 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010E2(int4 a1,int4 a2,int4 a3)

{
  int4 iVar1;
  
  sub_001400((xunknown2)a1);
  (*pcRambfea106d)((xunknown2)a3);
  iVar1 = sub_001412();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_0010E4  @ bfea10e4 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_0010E4(int4 a1,int4 a2,int4 a3)

{
  int4 iVar1;
  
  sub_001400((xunknown2)a1);
  (*pcRambfea106d)((xunknown2)a3);
  iVar1 = sub_001412();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_001109  @ bfea1109 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_001109(int4 a1)

{
  (*pcRambfea106d)((xunknown2)a1);
  return 1;
}

/* [IDENTIFIED (runtime library)] sub_00110B  @ bfea110b -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_00110B(int4 a1)

{
  (*pcRambfea106d)((xunknown2)a1);
  return 1;
}

/* [IDENTIFIED (runtime library)] LZStart  @ bfea1123 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 __cdecl LZStart(void)

{
  int2 iVar1;
  
  iVar1 = (*pcRambfea106d)();
  return (int4)iVar1;
}

/* [IDENTIFIED (runtime library)] sub_001125  @ bfea1125 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 __cdecl sub_001125(void)

{
  int2 iVar1;
  
  iVar1 = (*pcRambfea106d)();
  return (int4)iVar1;
}

/* [IDENTIFIED (runtime library)] sub_001135  @ bfea1135 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_001135(int4 a1,int4 a2)

{
  int4 iVar1;
  
  iVar1 = sub_00113B(a1,a2);
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_001139  @ bfea1139 -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_001139(int4 a1,int4 a2)

{
  xunknown8 xVar1;
  
  xVar1 = (*pcRambfea106d)((xunknown2)a2);
  return (uint4)xVar1 & 0xffff | (int4)((uint8)xVar1 >> 0x20) << 0x10;
}

/* [IDENTIFIED (runtime library)] sub_00113B  @ bfea113b -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_00113B(int4 a1,int4 a2)

{
  xunknown8 xVar1;
  
  xVar1 = (*pcRambfea106d)((xunknown2)a2);
  return (uint4)xVar1 & 0xffff | (int4)((uint8)xVar1 >> 0x20) << 0x10;
}

/* [IDENTIFIED (runtime library)] LZDone  @ bfea115b -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 __cdecl LZDone(void)

{
  int4 iVar1;
  
  iVar1 = (*pcRambfea106d)();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_00115D  @ bfea115d -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 __cdecl sub_00115D(void)

{
  int4 iVar1;
  
  iVar1 = (*pcRambfea106d)();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_00116C  @ bfea116c -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_00116C(int4 a1,int4 a2)

{
  int4 iVar1;
  
  iVar1 = sub_00140C();
  iVar1 = sub_001400(iVar1);
  (*pcRambfea106d)(iVar1);
  sub_001424();
  iVar1 = sub_001412();
  return iVar1;
}

/* [IDENTIFIED (runtime library)] sub_00116E  @ bfea116e -- thunk stub of Microsoft thunk compiler: a 32-to-16-bit flat thunk; reconstructed below: DOES NOT COMPILE, not in the build */
int4 sub_00116E(int4 a1,int4 a2)

{
  int4 iVar1;
  
  iVar1 = sub_00140C();
  iVar1 = sub_001400(iVar1);
  (*pcRambfea106d)(iVar1);
  sub_001424();
  iVar1 = sub_001412();
  return iVar1;
}

/* [UNVERIFIED] GetExpandedNameW  @ bfea1198 -- removes a different number of bytes from the stack (convention or argument count) */
int4 GetExpandedNameW(int4 a1,int4 a2)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_17();
  return iVar1;
}

/* [UNVERIFIED] LZOpenFileW  @ bfea11a1 -- removes a different number of bytes from the stack (convention or argument count) */
int4 LZOpenFileW(int4 a1,int4 a2,int4 a3)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_17();
  return iVar1;
}

/* [VERIFIED] LZCopy  @ bfea11aa -- 100% of it exercised */
int4 LZCopy(int4 a1,int4 a2)

{
  uint4 uVar1;
  uint4 uVar2;
  int4 iVar3;
  
  uVar1 = sub_001430(a1);
  if ((uVar1 & 0xffff) != 0) {
    uVar2 = sub_001430(a2);
    if ((uVar2 & 0xffff) != 0) {
      iVar3 = sub_001139(uVar1 & 0xffff,uVar2 & 0xffff);
      return iVar3;
    }
  }
  return -1;
}

/* [VERIFIED] LZClose  @ bfea11dd -- 100% of it exercised */
int4 LZClose(int4 a1)

{
  uint4 uVar1;
  
  uVar1 = sub_001430(a1);
  if ((uVar1 & 0xffff) != 0) {
    sub_001436(a1);
    uVar1 = sub_001109(uVar1 & 0xffff);
  }
  return uVar1;
}

/* [VERIFIED] LZInit  @ bfea1201 -- 100% of it exercised */
int4 LZInit(int4 a1)

{
  uint4 uVar1;
  int4 a1_00;
  int4 iVar2;
  
  uVar1 = sub_001430(a1);
  if ((uVar1 & 0xffff) == 0) {
    a1_00 = -1;
  }
  else {
    a1_00 = sub_0010A5(uVar1 & 0xffff);
    if (-1 < a1_00) {
      iVar2 = sub_00143C(a1_00);
      a1_00 = -5;
      if (iVar2 != 0) {
        a1_00 = iVar2;
      }
    }
  }
  return a1_00;
}

/* [VERIFIED] LZOpenFileA  @ bfea1238 -- 100% of it exercised */
int4 LZOpenFileA(int4 a1,int4 lpReOpenBuff,int4 a3)

{
  uint4 uVar1;
  int4 iVar2;
  
  iVar2 = 0;
  if (a1 != 0) {
    iVar2 = GlobalAlloc(GMEM_FIXED,0x104);
    if (iVar2 == 0) {
      SetLastError(8);
      return -1;
    }
    uVar1 = GetShortPathNameA(a1,iVar2,0x104);
    if ((uVar1 == 0) || (0x104 < uVar1)) {
      GlobalFree(iVar2);
      iVar2 = OpenFile(a1,lpReOpenBuff,a3 & 0xffff);
      return iVar2;
    }
  }
  uVar1 = thunk stub(iVar2,lpReOpenBuff,a3);
  if (iVar2 != 0) {
    GlobalFree(iVar2);
  }
  if ((uVar1 & 0xffff) != 0xffff) {
    iVar2 = sub_00143C(uVar1);
    if (iVar2 != 0) {
      return iVar2;
    }
    sub_001109(uVar1);
  }
  return -1;
}

/* [VERIFIED] GetExpandedNameA  @ bfea12d8 -- 100% of it exercised */
int4 GetExpandedNameA(int4 lpszLongPath,int4 a2)

{
  int4 hMem;
  uint4 uVar1;
  int4 iVar2;
  
  hMem = GlobalAlloc(GMEM_FIXED,0x104);
  if (hMem == 0) {
    iVar2 = -7;
  }
  else {
    uVar1 = GetShortPathNameA(lpszLongPath,hMem,0x104);
    if ((uVar1 == 0) || (0x103 < uVar1)) {
      iVar2 = -7;
    }
    else {
      iVar2 = sub_00116C(hMem,a2);
    }
    GlobalFree(hMem);
  }
  return iVar2;
}

/* [VERIFIED] CopyLZFile  @ bfea1330 -- 100% of it exercised */
int4 CopyLZFile(int4 a1,int4 a2)

{
  uint4 uVar1;
  uint4 uVar2;
  int4 iVar3;
  
  uVar1 = sub_001430(a1);
  if ((uVar1 & 0xffff) != 0) {
    uVar2 = sub_001430(a2);
    if ((uVar2 & 0xffff) != 0) {
      iVar3 = sub_001135(uVar1 & 0xffff,uVar2 & 0xffff);
      return iVar3;
    }
  }
  return -1;
}

/* [VERIFIED] LZRead  @ bfea1363 -- 100% of it exercised */
int4 LZRead(int4 a1,int4 a2,int4 a3)

{
  uint4 uVar1;
  int4 iVar2;
  int4 iVar3;
  
  iVar3 = 0;
  uVar1 = sub_001430(a1);
  uVar1 = uVar1 & 0xffff;
  if (uVar1 == 0) {
    iVar2 = -1;
  }
  else {
    for (; 0x7000 < a3; a3 = a3 - iVar2) {
      iVar2 = sub_0010E2(uVar1,a2,0x7000);
      if (iVar2 < 0) {
        return iVar2;
      }
      iVar3 = iVar3 + iVar2;
      if (iVar2 < 0x7000) {
        return iVar3;
      }
      a2 = a2 + 0x7000;
    }
    iVar2 = sub_0010E2(uVar1,a2,a3);
    if (-1 < iVar2) {
      iVar2 = iVar2 + iVar3;
    }
  }
  return iVar2;
}

/* [VERIFIED] LZSeek  @ bfea13d4 -- 100% of it exercised */
int4 LZSeek(int4 a1,int4 a2,int4 a3)

{
  uint4 uVar1;
  int4 iVar2;
  
  uVar1 = sub_001430(a1);
  iVar2 = -1;
  if ((uVar1 & 0xffff) != 0) {
    iVar2 = sub_0010BD(uVar1 & 0xffff,a2,a3);
  }
  return iVar2;
}

/* [VERIFIED] sub_0013FA  @ bfea13fa -- 100% of it exercised; calls DisableThreadLibraryCalls; no caller here uses what it returns */
int4 sub_0013FA(int4 hLibModule)

{
  int4 iVar1;
  
  iVar1 = DisableThreadLibraryCalls(hLibModule);
  return iVar1;
}

/* [VERIFIED] sub_001400  @ bfea1400 -- 100% of it exercised */
int4 __cdecl sub_001400(void)

{
  int4 iVar1;
  
  iVar1 = SMapLS_IP_EBP_12();
  return iVar1;
}

/* [VERIFIED] sub_00140C  @ bfea140c -- 100% of it exercised */
int4 __cdecl sub_00140C(void)

{
  int4 iVar1;
  
  iVar1 = SMapLS_IP_EBP_8();
  return iVar1;
}

/* [VERIFIED] sub_001412  @ bfea1412 -- 100% of it exercised */
int4 __cdecl sub_001412(void)

{
  int4 iVar1;
  
  iVar1 = SUnMapLS_IP_EBP_12();
  return iVar1;
}

/* [VERIFIED] sub_001424  @ bfea1424 -- 100% of it exercised; no caller here uses what it returns */
int4 __cdecl sub_001424(void)

{
  int4 iVar1;
  
  iVar1 = SUnMapLS_IP_EBP_8();
  return iVar1;
}

/* [VERIFIED] sub_001430  @ bfea1430 -- 100% of it exercised */
int4 sub_001430(int4 a1)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_21(a1);
  return iVar1;
}

/* [VERIFIED] sub_001436  @ bfea1436 -- 100% of it exercised; no caller here uses what it returns */
int4 sub_001436(int4 a1)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_22(a1);
  return iVar1;
}

/* [VERIFIED] sub_00143C  @ bfea143c -- 100% of it exercised */
int4 sub_00143C(int4 a1)

{
  int4 iVar1;
  
  iVar1 = KERNEL32_ord_20(a1);
  return iVar1;
}
