/* lz32.dll -- types, declarations and globals used by the functions in lz32.c.
 * Globals are bound to their addresses in the original until the module's data is
 * reconstructed. */

#pragma pack(1)
typedef signed char int1; typedef short int2; typedef int int4; typedef long long int8;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned int uint4;
typedef unsigned long long uint8;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned int xunknown4;
typedef unsigned long long xunknown8;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned int undefined4; typedef unsigned long long undefined8;
typedef unsigned char bool; typedef float float4; typedef double float8; typedef long double float10;
typedef int code();
typedef unsigned int unkbyte3; typedef unsigned int unkint3; typedef unsigned int unkuint3;
#define true 1
#define false 0


#define CONCAT11(a,b) ((uint2)(((uint2)(uint1)(a) << 8) | (uint1)(b)))
#define CONCAT12(a,b) ((uint4)(((uint4)(uint1)(a) << 16) | (uint2)(b)))
#define CONCAT13(a,b) ((uint4)(((uint4)(uint1)(a) << 24) | ((uint4)(b) & 0xFFFFFF)))
#define CONCAT21(a,b) ((uint4)(((uint4)(uint2)(a) << 8) | (uint1)(b)))
#define CONCAT22(a,b) ((uint4)(((uint4)(uint2)(a) << 16) | (uint2)(b)))
#define CONCAT31(a,b) ((uint4)(((uint4)(a) << 8) | (uint1)(b)))
#define CONCAT44(a,b) ((uint8)(((uint8)(uint4)(a) << 32) | (uint4)(b)))
#define SUB21(x,n) ((uint1)((uint2)(x) >> (8 * (n))))
#define SUB41(x,n) ((uint1)((uint4)(x) >> (8 * (n))))
#define SUB42(x,n) ((uint2)((uint4)(x) >> (8 * (n))))
#define SUB43(x,n) ((uint4)((uint4)(x) >> (8 * (n))) & 0xFFFFFF)
#define SUB81(x,n) ((uint1)((uint8)(x) >> (8 * (n))))
#define SUB82(x,n) ((uint2)((uint8)(x) >> (8 * (n))))
#define SUB84(x,n) ((uint4)((uint8)(x) >> (8 * (n))))
#define ZEXT12(x) ((uint2)(uint1)(x))
#define ZEXT14(x) ((uint4)(uint1)(x))
#define ZEXT24(x) ((uint4)(uint2)(x))
#define ZEXT48(x) ((uint8)(uint4)(x))
#define SEXT12(x) ((int2)(int1)(x))
#define SEXT14(x) ((int4)(int1)(x))
#define SEXT24(x) ((int4)(int2)(x))
#define SEXT48(x) ((int8)(int4)(x))
#define CARRY1(a,b) ((uint1)(a) > (uint1)(0xFF - (uint1)(b)))
#define CARRY2(a,b) ((uint2)(a) > (uint2)(0xFFFF - (uint2)(b)))
#define CARRY4(a,b) ((uint4)(a) > 0xFFFFFFFFu - (uint4)(b))
#define SCARRY4(a,b) ((((int4)(a) ^ (int4)((uint4)(a) + (uint4)(b))) & ((int4)(b) ^ (int4)((uint4)(a) + (uint4)(b)))) < 0)
#define SBORROW4(a,b) ((((int4)(a) ^ (int4)(b)) & ((int4)(a) ^ (int4)((uint4)(a) - (uint4)(b)))) < 0)
#define SCARRY2(a,b) ((int2)(((int2)(a) ^ (int2)((a) + (b))) & ((int2)(b) ^ (int2)((a) + (b)))) < 0)
#define SBORROW2(a,b) ((int2)(((int2)(a) ^ (int2)(b)) & ((int2)(a) ^ (int2)((a) - (b)))) < 0)
#define SCARRY1(a,b) ((int1)(((int1)(a) ^ (int1)((a) + (b))) & ((int1)(b) ^ (int1)((a) + (b)))) < 0)
#define SBORROW1(a,b) ((int1)(((int1)(a) ^ (int1)(b)) & ((int1)(a) ^ (int1)((a) - (b)))) < 0)
#define CONCAT14(a,b) ((uint8)((((uint8)(a) & 0xFFULL) << 32) | ((uint8)(b) & 0xFFFFFFFFULL)))
#define CONCAT15(a,b) ((uint8)((((uint8)(a) & 0xFFULL) << 40) | ((uint8)(b) & 0xFFFFFFFFFFULL)))
#define CONCAT16(a,b) ((uint8)((((uint8)(a) & 0xFFULL) << 48) | ((uint8)(b) & 0xFFFFFFFFFFFFULL)))
#define CONCAT17(a,b) ((uint8)((((uint8)(a) & 0xFFULL) << 56) | ((uint8)(b) & 0xFFFFFFFFFFFFFFULL)))
#define CONCAT23(a,b) ((uint8)((((uint8)(a) & 0xFFFFULL) << 24) | ((uint8)(b) & 0xFFFFFFULL)))
#define CONCAT24(a,b) ((uint8)((((uint8)(a) & 0xFFFFULL) << 32) | ((uint8)(b) & 0xFFFFFFFFULL)))
#define CONCAT25(a,b) ((uint8)((((uint8)(a) & 0xFFFFULL) << 40) | ((uint8)(b) & 0xFFFFFFFFFFULL)))
#define CONCAT26(a,b) ((uint8)((((uint8)(a) & 0xFFFFULL) << 48) | ((uint8)(b) & 0xFFFFFFFFFFFFULL)))
#define CONCAT32(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFULL) << 16) | ((uint8)(b) & 0xFFFFULL)))
#define CONCAT33(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFULL) << 24) | ((uint8)(b) & 0xFFFFFFULL)))
#define CONCAT34(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFULL) << 32) | ((uint8)(b) & 0xFFFFFFFFULL)))
#define CONCAT35(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFULL) << 40) | ((uint8)(b) & 0xFFFFFFFFFFULL)))
#define CONCAT41(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFULL) << 8) | ((uint8)(b) & 0xFFULL)))
#define CONCAT42(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFULL) << 16) | ((uint8)(b) & 0xFFFFULL)))
#define CONCAT43(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFULL) << 24) | ((uint8)(b) & 0xFFFFFFULL)))
#define CONCAT51(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFULL) << 8) | ((uint8)(b) & 0xFFULL)))
#define CONCAT52(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFULL) << 16) | ((uint8)(b) & 0xFFFFULL)))
#define CONCAT53(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFULL) << 24) | ((uint8)(b) & 0xFFFFFFULL)))
#define CONCAT61(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFFFULL) << 8) | ((uint8)(b) & 0xFFULL)))
#define CONCAT62(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFFFULL) << 16) | ((uint8)(b) & 0xFFFFULL)))
#define CONCAT71(a,b) ((uint8)((((uint8)(a) & 0xFFFFFFFFFFFFFFULL) << 8) | ((uint8)(b) & 0xFFULL)))
#define SUB31(x,n) ((uint1)((((uint8)(x) & 0xFFFFFFULL) >> (8 * (n))) & 0xFFULL))
#define ZEXT13(x) ((uint4)((uint8)(x) & 0xFFULL))
#define SEXT13(x) ((int4)(int1)(x))
#define SUB32(x,n) ((uint2)((((uint8)(x) & 0xFFFFFFULL) >> (8 * (n))) & 0xFFFFULL))
#define ZEXT23(x) ((uint4)((uint8)(x) & 0xFFFFULL))
#define SEXT23(x) ((int4)(int2)(x))
#define ZEXT34(x) ((uint4)((uint8)(x) & 0xFFFFFFULL))
#define SUB51(x,n) ((uint1)((((uint8)(x) & 0xFFFFFFFFFFULL) >> (8 * (n))) & 0xFFULL))
#define ZEXT15(x) ((uint8)((uint8)(x) & 0xFFULL))
#define SEXT15(x) ((int8)(int1)(x))
#define SUB52(x,n) ((uint2)((((uint8)(x) & 0xFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFULL))
#define ZEXT25(x) ((uint8)((uint8)(x) & 0xFFFFULL))
#define SEXT25(x) ((int8)(int2)(x))
#define SUB53(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFULL))
#define ZEXT35(x) ((uint8)((uint8)(x) & 0xFFFFFFULL))
#define SUB54(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFULL))
#define ZEXT45(x) ((uint8)((uint8)(x) & 0xFFFFFFFFULL))
#define SEXT45(x) ((int8)(int4)(x))
#define SUB61(x,n) ((uint1)((((uint8)(x) & 0xFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFULL))
#define ZEXT16(x) ((uint8)((uint8)(x) & 0xFFULL))
#define SEXT16(x) ((int8)(int1)(x))
#define SUB62(x,n) ((uint2)((((uint8)(x) & 0xFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFULL))
#define ZEXT26(x) ((uint8)((uint8)(x) & 0xFFFFULL))
#define SEXT26(x) ((int8)(int2)(x))
#define SUB63(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFULL))
#define ZEXT36(x) ((uint8)((uint8)(x) & 0xFFFFFFULL))
#define SUB64(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFULL))
#define ZEXT46(x) ((uint8)((uint8)(x) & 0xFFFFFFFFULL))
#define SEXT46(x) ((int8)(int4)(x))
#define SUB65(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFULL))
#define ZEXT56(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFULL))
#define SUB71(x,n) ((uint1)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFULL))
#define ZEXT17(x) ((uint8)((uint8)(x) & 0xFFULL))
#define SEXT17(x) ((int8)(int1)(x))
#define SUB72(x,n) ((uint2)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFULL))
#define ZEXT27(x) ((uint8)((uint8)(x) & 0xFFFFULL))
#define SEXT27(x) ((int8)(int2)(x))
#define SUB73(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFULL))
#define ZEXT37(x) ((uint8)((uint8)(x) & 0xFFFFFFULL))
#define SUB74(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFULL))
#define ZEXT47(x) ((uint8)((uint8)(x) & 0xFFFFFFFFULL))
#define SEXT47(x) ((int8)(int4)(x))
#define SUB75(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFULL))
#define ZEXT57(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFULL))
#define SUB76(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFFFULL))
#define ZEXT67(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFFFULL))
#define ZEXT18(x) ((uint8)((uint8)(x) & 0xFFULL))
#define SEXT18(x) ((int8)(int1)(x))
#define ZEXT28(x) ((uint8)((uint8)(x) & 0xFFFFULL))
#define SEXT28(x) ((int8)(int2)(x))
#define SUB83(x,n) ((uint4)((((uint8)(x) & 0xFFFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFULL))
#define ZEXT38(x) ((uint8)((uint8)(x) & 0xFFFFFFULL))
#define SUB85(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFULL))
#define ZEXT58(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFULL))
#define SUB86(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFFFULL))
#define ZEXT68(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFFFULL))
#define SUB87(x,n) ((uint8)((((uint8)(x) & 0xFFFFFFFFFFFFFFFFULL) >> (8 * (n))) & 0xFFFFFFFFFFFFFFULL))
#define ZEXT78(x) ((uint8)((uint8)(x) & 0xFFFFFFFFFFFFFFULL))
double sqrt(double); double sin(double); double cos(double); double tan(double);
double atan2(double, double); double pow(double, double); double ldexp(double, int);
double log(double); double floor(double); double fmod(double, double);
#define ROUND(x) (((double)(x) - floor((double)(x)) > 0.5 || ((double)(x) - floor((double)(x)) == 0.5 && fmod(floor((double)(x)), 2.0) != 0)) ? floor((double)(x)) + 1 : floor((double)(x)))
#define SQRT(x) sqrt((double)(x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define NAN(x) ((x) != (x))
#define fsin(x) sin((double)(x))
#define fcos(x) cos((double)(x))
#define fptan(x) tan((double)(x))
#define fpatan(a,b) atan2((double)(a), (double)(b))
#define f2xm1(x) (pow(2.0, (double)(x)) - 1.0)
#define fscale(a,b) ldexp((double)(a), (int)(b))
#define log2(x) (log((double)(x)) / 0.6931471805599453)
int __stdcall DisableThreadLibraryCalls();
int __stdcall GetShortPathNameA();
int __stdcall GlobalAlloc();
int __stdcall GlobalFree();
int __stdcall KERNEL32_ord_17();
int __stdcall KERNEL32_ord_20();
int __stdcall KERNEL32_ord_21();
int __stdcall KERNEL32_ord_22();
int __stdcall KERNEL32_ord_41();
int __stdcall OpenFile();
int __cdecl SMapLS_IP_EBP_12();
int __cdecl SMapLS_IP_EBP_8();
int __cdecl SUnMapLS_IP_EBP_12();
int __cdecl SUnMapLS_IP_EBP_8();
int __stdcall SetLastError();
int4 sub_001000(int4 a1,int4 a2,int4 a3);
int4 sub_001057(int4 a1,int4 a2);
int4 thunk stub(int4 a1,int4 a2,int4 a3);
int4 sub_001077(int4 a1,int4 a2,int4 a3);
int4 sub_0010A5(int4 a1);
int4 sub_0010A7(int4 a1);
int4 sub_0010BD(int4 a1,int4 a2,int4 a3);
int4 sub_0010BF(int4 a1,int4 a2,int4 a3);
int4 sub_0010E2(int4 a1,int4 a2,int4 a3);
int4 sub_0010E4(int4 a1,int4 a2,int4 a3);
int4 sub_001109(int4 a1);
int4 sub_00110B(int4 a1);
int4 __cdecl LZStart();
int4 __cdecl sub_001125();
int4 sub_001135(int4 a1,int4 a2);
int4 sub_001139(int4 a1,int4 a2);
int4 sub_00113B(int4 a1,int4 a2);
int4 __cdecl LZDone();
int4 __cdecl sub_00115D();
int4 sub_00116C(int4 a1,int4 a2);
int4 sub_00116E(int4 a1,int4 a2);
int4 GetExpandedNameW(int4 a1,int4 a2);
int4 LZOpenFileW(int4 a1,int4 a2,int4 a3);
int4 LZCopy(int4 a1,int4 a2);
int4 LZClose(int4 a1);
int4 LZInit(int4 a1);
int4 LZOpenFileA(int4 a1,int4 a2,int4 a3);
int4 GetExpandedNameA(int4 a1,int4 a2);
int4 CopyLZFile(int4 a1,int4 a2);
int4 LZRead(int4 a1,int4 a2,int4 a3);
int4 LZSeek(int4 a1,int4 a2,int4 a3);
int4 sub_0013FA(int4 a1);
int4 __cdecl sub_001400();
int4 __cdecl sub_00140C();
int4 __cdecl sub_001412();
int4 __cdecl sub_001424();
int4 sub_001430(int4 a1);
int4 sub_001436(int4 a1);
int4 sub_00143C(int4 a1);
#define cRambfea2016 (*(uint1 *)0xbfea2016)
#define pcRambfea106d (*(code * *)0xbfea106d)

/* globals (type recovered from use; still at the original's address):
 *   bfea106d  function pointer                   read by 9, written by 0  initially &bfea201c
 *   bfea2016  unsigned char                      read by 1, written by 1  initially 0x1 */

/* SDK constants written into lz32.c, with the values the SDK headers give them */
#ifndef GMEM_FIXED
#define GMEM_FIXED 0x0
#endif
