/* C's arithmetic on Java's types, every width and sign: conversions between all of them, compound assignment on a Java local
   and on emulated memory, comparisons, unary operators, increments, character and string literals. No library calls, so the
   result is the program's exit code. */
typedef signed char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned u32;
typedef long long i64;
typedef unsigned long long u64;

/* (D)x for every pair */
#define CV(D, S) static D cv_##D##_##S(S x) { return (D)x; }
#define CV_ALL(D) CV(D, i8) CV(D, u8) CV(D, i16) CV(D, u16) CV(D, i32) CV(D, u32) CV(D, i64) CV(D, u64) CV(D, float) CV(D, double)
CV_ALL(i8) CV_ALL(u8) CV_ALL(i16) CV_ALL(u16) CV_ALL(i32) CV_ALL(u32) CV_ALL(i64) CV_ALL(u64)
CV(float, i8) CV(float, u8) CV(float, i16) CV(float, u16) CV(float, i32) CV(float, u32) CV(float, i64) CV(float, u64) CV(float, double)
CV(double, i8) CV(double, u8) CV(double, i16) CV(double, u16) CV(double, i32) CV(double, u32) CV(double, i64) CV(double, u64) CV(double, float)

/* compound assignment: on a local (a Java variable) and through a pointer (emulated memory) */
#define CA(T, N, OP) \
    static T cl_##T##_##N(T a, T v) { a OP v; return a; } \
    static T cm_##T##_##N(T *p, T v) { *p OP v; return *p; }
#define CA_INT(T) CA(T, add, +=) CA(T, sub, -=) CA(T, mul, *=) CA(T, div, /=) CA(T, rem, %=) CA(T, shl, <<=) CA(T, shr, >>=) \
    CA(T, and, &=) CA(T, or, |=) CA(T, xor, ^=)
CA_INT(i8) CA_INT(u8) CA_INT(i16) CA_INT(u16) CA_INT(i32) CA_INT(u32) CA_INT(i64) CA_INT(u64)
CA(float, add, +=) CA(float, div, /=)
CA(double, mul, *=) CA(double, sub, -=)

/* the binary operators on two operands of one type, and the comparisons */
#define BIN(T, N, OP) static T bn_##T##_##N(T a, T b) { return a OP b; }
#define CMP(T, N, OP) static int cp_##T##_##N(T a, T b) { return a OP b; }
#define OPS(T) BIN(T, add, +) BIN(T, sub, -) BIN(T, mul, *) BIN(T, div, /) BIN(T, rem, %) BIN(T, and, &) BIN(T, or, |) BIN(T, xor, ^) \
    BIN(T, shl, <<) BIN(T, shr, >>) CMP(T, lt, <) CMP(T, le, <=) CMP(T, gt, >) CMP(T, ge, >=) CMP(T, eq, ==) CMP(T, ne, !=)
OPS(i8) OPS(u8) OPS(i16) OPS(u16) OPS(i32) OPS(u32) OPS(i64) OPS(u64)
static float bn_float_add(float a, float b) { return a + b; }
static double bn_double_mul(double a, double b) { return a * b; }
static int cp_double_lt(double a, double b) { return a < b; }
static int cp_float_ge(float a, float b) { return a >= b; }

/* unary operators and increments, on a local and in memory */
#define UN(T) \
    static T un_neg_##T(T a) { return -a; } \
    static T un_not_##T(T a) { return ~a; } \
    static int un_lnot_##T(T a) { return !a; } \
    static T un_pre_##T(T a) { return ++a; } \
    static T un_post_##T(T a) { T b = a++; return b + a; } \
    static T un_mem_##T(T *p) { T b = (*p)--; --*p; return b + *p; }
UN(i8) UN(u8) UN(i16) UN(u16) UN(i32) UN(u32) UN(i64) UN(u64)

static int chars(int k) {
    unsigned short us = 127;
    int r = 'a' + '\n' + '\0' + '\'' + '\\' + '\a' + '\x41' + '\177';
    us = 'a';
    us += '\t';
    us = 127;
    r += us;
    char buf[8] = "abc";
    r += buf[0] + buf[1] - '0' + k;
    return r;
}

static long long mixed(int a, unsigned b, long long c, unsigned long long d, double e, float f) {
    return a * b + c / 3 - (long long)(d >> 3) + (long long)(e * f) + (a < b) + (c > d) + (a ? c : d) + (e > f ? a : b);
}

static int ptrs(int n) {
    int arr[6] = {1, 2, 3, 4, 5, 6};
    int *p = arr + 1;
    int *q = &arr[4];
    p += 2;
    q -= 1;
    int d = (int)(q - p);
    return d + *p + p[-1] + *(arr + 5) + n;
}

int main(void) {
    long long acc = 0;
    u16 s16 = 1000;
    i64 s64 = -17;
    u32 s32 = 5;
    i8 s8 = -128;
    acc += cv_i8_i32(300) + cv_u8_i32(-1) + cv_i16_u32(70000u) + cv_u16_i64(-2LL) + cv_i32_double(-3.9) + cv_u32_double(4.0e9);
    acc += cv_i64_double(-2.5e10) + (long long)(cv_u64_double(1.0e19) >> 60) + (long long)cv_float_u64(18000000000000000000ull);
    acc += (long long)cv_double_u32(4000000000u) + cv_double_float(1.5f) + cv_u8_u64(1000ull) + cv_i32_u8(250);
    acc += cl_i8_add(100, 100) + cm_u16_shl(&s16, 3) + cl_u32_shr(0x80000000u, 31) + cm_i64_rem(&s64, 5);
    acc += bn_i8_add(100, 3) + bn_u32_div(4000000000u, 7u) + bn_u64_rem(9000000000000000000ull, 11ull) + bn_i16_shr(-300, 2);
    acc += cp_u32_lt(1u, 4000000000u) + cp_i8_ge(-1, 1) + cp_u64_ge(1ull << 63, 5ull) + cp_double_lt(1.0, 2.0) + cp_float_ge(2.0f, 3.0f);
    acc += un_neg_i8(-128) + un_not_u8(5) + un_lnot_i64(0) + un_pre_u16(65535) + un_post_i32(7) + un_mem_u32(&s32) + un_mem_i8(&s8);
    acc += chars(3) + mixed(-3, 7u, 100000000000LL, 80000000000ull, 2.5, 4.0f) + ptrs(2);
    acc += bn_float_add(0.25f, 0.5f) + bn_double_mul(1.5, 4.0);
    return (int)(((acc % 251) + 251) % 251);
}
