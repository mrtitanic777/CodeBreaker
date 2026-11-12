// FaithfulArith.java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem).
import crt.*;

import static crt.LibC.*;

public final class FaithfulArith {

    static byte cv_i8_i8(byte x) {
        return x;
    }

    static byte cv_i8_u8(byte x) {
        return x;
    }

    static byte cv_i8_i16(short x) {
        return (byte) x;
    }

    static byte cv_i8_u16(char x) {
        return (byte) x;
    }

    static byte cv_i8_i32(int x) {
        return (byte) x;
    }

    static byte cv_i8_u32(int x) {
        return (byte) x;
    }

    static byte cv_i8_i64(long x) {
        return (byte) x;
    }

    static byte cv_i8_u64(long x) {
        return (byte) x;
    }

    static byte cv_i8_float(float x) {
        return (byte) (int) x;
    }

    static byte cv_i8_double(double x) {
        return (byte) (int) x;
    }

    static byte cv_u8_i8(byte x) {
        return x;
    }

    static byte cv_u8_u8(byte x) {
        return x;
    }

    static byte cv_u8_i16(short x) {
        return (byte) x;
    }

    static byte cv_u8_u16(char x) {
        return (byte) x;
    }

    static byte cv_u8_i32(int x) {
        return (byte) x;
    }

    static byte cv_u8_u32(int x) {
        return (byte) x;
    }

    static byte cv_u8_i64(long x) {
        return (byte) x;
    }

    static byte cv_u8_u64(long x) {
        return (byte) x;
    }

    static byte cv_u8_float(float x) {
        return (byte) (int) x;
    }

    static byte cv_u8_double(double x) {
        return (byte) (int) x;
    }

    static short cv_i16_i8(byte x) {
        return (short) x;
    }

    static short cv_i16_u8(byte x) {
        return (short) (x & 0xFF);
    }

    static short cv_i16_i16(short x) {
        return x;
    }

    static short cv_i16_u16(char x) {
        return (short) x;
    }

    static short cv_i16_i32(int x) {
        return (short) x;
    }

    static short cv_i16_u32(int x) {
        return (short) x;
    }

    static short cv_i16_i64(long x) {
        return (short) x;
    }

    static short cv_i16_u64(long x) {
        return (short) x;
    }

    static short cv_i16_float(float x) {
        return (short) (int) x;
    }

    static short cv_i16_double(double x) {
        return (short) (int) x;
    }

    static char cv_u16_i8(byte x) {
        return (char) x;
    }

    static char cv_u16_u8(byte x) {
        return (char) (x & 0xFF);
    }

    static char cv_u16_i16(short x) {
        return (char) x;
    }

    static char cv_u16_u16(char x) {
        return x;
    }

    static char cv_u16_i32(int x) {
        return (char) x;
    }

    static char cv_u16_u32(int x) {
        return (char) x;
    }

    static char cv_u16_i64(long x) {
        return (char) x;
    }

    static char cv_u16_u64(long x) {
        return (char) x;
    }

    static char cv_u16_float(float x) {
        return (char) (int) x;
    }

    static char cv_u16_double(double x) {
        return (char) (int) x;
    }

    static int cv_i32_i8(byte x) {
        return x;
    }

    static int cv_i32_u8(byte x) {
        return x & 0xFF;
    }

    static int cv_i32_i16(short x) {
        return x;
    }

    static int cv_i32_u16(char x) {
        return x;
    }

    static int cv_i32_i32(int x) {
        return x;
    }

    static int cv_i32_u32(int x) {
        return x;
    }

    static int cv_i32_i64(long x) {
        return (int) x;
    }

    static int cv_i32_u64(long x) {
        return (int) x;
    }

    static int cv_i32_float(float x) {
        return (int) x;
    }

    static int cv_i32_double(double x) {
        return (int) x;
    }

    static int cv_u32_i8(byte x) {
        return x;
    }

    static int cv_u32_u8(byte x) {
        return x & 0xFF;
    }

    static int cv_u32_i16(short x) {
        return x;
    }

    static int cv_u32_u16(char x) {
        return x;
    }

    static int cv_u32_i32(int x) {
        return x;
    }

    static int cv_u32_u32(int x) {
        return x;
    }

    static int cv_u32_i64(long x) {
        return (int) x;
    }

    static int cv_u32_u64(long x) {
        return (int) x;
    }

    static int cv_u32_float(float x) {
        return (int) (long) x;
    }

    static int cv_u32_double(double x) {
        return (int) (long) x;
    }

    static long cv_i64_i8(byte x) {
        return (long) x;
    }

    static long cv_i64_u8(byte x) {
        return (long) (x & 0xFF);
    }

    static long cv_i64_i16(short x) {
        return (long) x;
    }

    static long cv_i64_u16(char x) {
        return (long) x;
    }

    static long cv_i64_i32(int x) {
        return (long) x;
    }

    static long cv_i64_u32(int x) {
        return Integer.toUnsignedLong(x);
    }

    static long cv_i64_i64(long x) {
        return x;
    }

    static long cv_i64_u64(long x) {
        return x;
    }

    static long cv_i64_float(float x) {
        return (long) x;
    }

    static long cv_i64_double(double x) {
        return (long) x;
    }

    static long cv_u64_i8(byte x) {
        return (long) x;
    }

    static long cv_u64_u8(byte x) {
        return (long) (x & 0xFF);
    }

    static long cv_u64_i16(short x) {
        return (long) x;
    }

    static long cv_u64_u16(char x) {
        return (long) x;
    }

    static long cv_u64_i32(int x) {
        return (long) x;
    }

    static long cv_u64_u32(int x) {
        return Integer.toUnsignedLong(x);
    }

    static long cv_u64_i64(long x) {
        return x;
    }

    static long cv_u64_u64(long x) {
        return x;
    }

    static long cv_u64_float(float x) {
        return doubleToUnsigned(x);
    }

    static long cv_u64_double(double x) {
        return doubleToUnsigned(x);
    }

    static float cv_float_i8(byte x) {
        return (float) x;
    }

    static float cv_float_u8(byte x) {
        return (float) (x & 0xFF);
    }

    static float cv_float_i16(short x) {
        return (float) x;
    }

    static float cv_float_u16(char x) {
        return (float) x;
    }

    static float cv_float_i32(int x) {
        return (float) x;
    }

    static float cv_float_u32(int x) {
        return (float) Integer.toUnsignedLong(x);
    }

    static float cv_float_i64(long x) {
        return (float) x;
    }

    static float cv_float_u64(long x) {
        return (float) unsignedToDouble(x);
    }

    static float cv_float_double(double x) {
        return (float) x;
    }

    static double cv_double_i8(byte x) {
        return (double) x;
    }

    static double cv_double_u8(byte x) {
        return (double) (x & 0xFF);
    }

    static double cv_double_i16(short x) {
        return (double) x;
    }

    static double cv_double_u16(char x) {
        return (double) x;
    }

    static double cv_double_i32(int x) {
        return (double) x;
    }

    static double cv_double_u32(int x) {
        return (double) Integer.toUnsignedLong(x);
    }

    static double cv_double_i64(long x) {
        return (double) x;
    }

    static double cv_double_u64(long x) {
        return unsignedToDouble(x);
    }

    static double cv_double_float(float x) {
        return (double) x;
    }

    static byte cl_i8_add(byte a, byte v) {
        a += v;
        return a;
    }

    static byte cm_i8_add(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) + v));
        return Mem.getByte(p);
    }

    static byte cl_i8_sub(byte a, byte v) {
        a -= v;
        return a;
    }

    static byte cm_i8_sub(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) - v));
        return Mem.getByte(p);
    }

    static byte cl_i8_mul(byte a, byte v) {
        a *= v;
        return a;
    }

    static byte cm_i8_mul(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) * v));
        return Mem.getByte(p);
    }

    static byte cl_i8_div(byte a, byte v) {
        a = (byte) (a / v);
        return a;
    }

    static byte cm_i8_div(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) / v));
        return Mem.getByte(p);
    }

    static byte cl_i8_rem(byte a, byte v) {
        a = (byte) (a % v);
        return a;
    }

    static byte cm_i8_rem(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) % v));
        return Mem.getByte(p);
    }

    static byte cl_i8_shl(byte a, byte v) {
        a <<= v;
        return a;
    }

    static byte cm_i8_shl(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) << v));
        return Mem.getByte(p);
    }

    static byte cl_i8_shr(byte a, byte v) {
        a = (byte) (a >> v);
        return a;
    }

    static byte cm_i8_shr(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) >> v));
        return Mem.getByte(p);
    }

    static byte cl_i8_and(byte a, byte v) {
        a &= v;
        return a;
    }

    static byte cm_i8_and(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) & v));
        return Mem.getByte(p);
    }

    static byte cl_i8_or(byte a, byte v) {
        a |= v;
        return a;
    }

    static byte cm_i8_or(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) | v));
        return Mem.getByte(p);
    }

    static byte cl_i8_xor(byte a, byte v) {
        a ^= v;
        return a;
    }

    static byte cm_i8_xor(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) ^ v));
        return Mem.getByte(p);
    }

    static byte cl_u8_add(byte a, byte v) {
        a += v & 0xFF;
        return a;
    }

    static byte cm_u8_add(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) + (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_sub(byte a, byte v) {
        a -= v & 0xFF;
        return a;
    }

    static byte cm_u8_sub(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) - (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_mul(byte a, byte v) {
        a *= v & 0xFF;
        return a;
    }

    static byte cm_u8_mul(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) * (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_div(byte a, byte v) {
        a = (byte) ((a & 0xFF) / (v & 0xFF));
        return a;
    }

    static byte cm_u8_div(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) / (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_rem(byte a, byte v) {
        a = (byte) ((a & 0xFF) % (v & 0xFF));
        return a;
    }

    static byte cm_u8_rem(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) % (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_shl(byte a, byte v) {
        a <<= v & 0xFF;
        return a;
    }

    static byte cm_u8_shl(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) << (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_shr(byte a, byte v) {
        a = (byte) ((a & 0xFF) >> (v & 0xFF));
        return a;
    }

    static byte cm_u8_shr(int p, byte v) {
        Mem.setByte(p, (byte) ((Mem.getByte(p) & 0xFF) >> (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_and(byte a, byte v) {
        a &= v & 0xFF;
        return a;
    }

    static byte cm_u8_and(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) & 0xFF & (v & 0xFF)));
        return Mem.getByte(p);
    }

    static byte cl_u8_or(byte a, byte v) {
        a |= v & 0xFF;
        return a;
    }

    static byte cm_u8_or(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) & 0xFF | v & 0xFF));
        return Mem.getByte(p);
    }

    static byte cl_u8_xor(byte a, byte v) {
        a ^= v & 0xFF;
        return a;
    }

    static byte cm_u8_xor(int p, byte v) {
        Mem.setByte(p, (byte) (Mem.getByte(p) & 0xFF ^ v & 0xFF));
        return Mem.getByte(p);
    }

    static short cl_i16_add(short a, short v) {
        a += v;
        return a;
    }

    static short cm_i16_add(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) + v));
        return Mem.getShort(p);
    }

    static short cl_i16_sub(short a, short v) {
        a -= v;
        return a;
    }

    static short cm_i16_sub(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) - v));
        return Mem.getShort(p);
    }

    static short cl_i16_mul(short a, short v) {
        a *= v;
        return a;
    }

    static short cm_i16_mul(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) * v));
        return Mem.getShort(p);
    }

    static short cl_i16_div(short a, short v) {
        a = (short) (a / v);
        return a;
    }

    static short cm_i16_div(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) / v));
        return Mem.getShort(p);
    }

    static short cl_i16_rem(short a, short v) {
        a = (short) (a % v);
        return a;
    }

    static short cm_i16_rem(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) % v));
        return Mem.getShort(p);
    }

    static short cl_i16_shl(short a, short v) {
        a <<= v;
        return a;
    }

    static short cm_i16_shl(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) << v));
        return Mem.getShort(p);
    }

    static short cl_i16_shr(short a, short v) {
        a = (short) (a >> v);
        return a;
    }

    static short cm_i16_shr(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) >> v));
        return Mem.getShort(p);
    }

    static short cl_i16_and(short a, short v) {
        a &= v;
        return a;
    }

    static short cm_i16_and(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) & v));
        return Mem.getShort(p);
    }

    static short cl_i16_or(short a, short v) {
        a |= v;
        return a;
    }

    static short cm_i16_or(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) | v));
        return Mem.getShort(p);
    }

    static short cl_i16_xor(short a, short v) {
        a ^= v;
        return a;
    }

    static short cm_i16_xor(int p, short v) {
        Mem.setShort(p, (short) (Mem.getShort(p) ^ v));
        return Mem.getShort(p);
    }

    static char cl_u16_add(char a, char v) {
        a += v;
        return a;
    }

    static char cm_u16_add(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) + v));
        return Mem.getChar(p);
    }

    static char cl_u16_sub(char a, char v) {
        a -= v;
        return a;
    }

    static char cm_u16_sub(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) - v));
        return Mem.getChar(p);
    }

    static char cl_u16_mul(char a, char v) {
        a *= v;
        return a;
    }

    static char cm_u16_mul(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) * v));
        return Mem.getChar(p);
    }

    static char cl_u16_div(char a, char v) {
        a = (char) (a / v);
        return a;
    }

    static char cm_u16_div(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) / v));
        return Mem.getChar(p);
    }

    static char cl_u16_rem(char a, char v) {
        a = (char) (a % v);
        return a;
    }

    static char cm_u16_rem(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) % v));
        return Mem.getChar(p);
    }

    static char cl_u16_shl(char a, char v) {
        a <<= v;
        return a;
    }

    static char cm_u16_shl(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) << v));
        return Mem.getChar(p);
    }

    static char cl_u16_shr(char a, char v) {
        a = (char) (a >> v);
        return a;
    }

    static char cm_u16_shr(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) >> v));
        return Mem.getChar(p);
    }

    static char cl_u16_and(char a, char v) {
        a &= v;
        return a;
    }

    static char cm_u16_and(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) & v));
        return Mem.getChar(p);
    }

    static char cl_u16_or(char a, char v) {
        a |= v;
        return a;
    }

    static char cm_u16_or(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) | v));
        return Mem.getChar(p);
    }

    static char cl_u16_xor(char a, char v) {
        a ^= v;
        return a;
    }

    static char cm_u16_xor(int p, char v) {
        Mem.setChar(p, (char) (Mem.getChar(p) ^ v));
        return Mem.getChar(p);
    }

    static int cl_i32_add(int a, int v) {
        a += v;
        return a;
    }

    static int cm_i32_add(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) + v);
        return Mem.getInt(p);
    }

    static int cl_i32_sub(int a, int v) {
        a -= v;
        return a;
    }

    static int cm_i32_sub(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) - v);
        return Mem.getInt(p);
    }

    static int cl_i32_mul(int a, int v) {
        a *= v;
        return a;
    }

    static int cm_i32_mul(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) * v);
        return Mem.getInt(p);
    }

    static int cl_i32_div(int a, int v) {
        a /= v;
        return a;
    }

    static int cm_i32_div(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) / v);
        return Mem.getInt(p);
    }

    static int cl_i32_rem(int a, int v) {
        a %= v;
        return a;
    }

    static int cm_i32_rem(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) % v);
        return Mem.getInt(p);
    }

    static int cl_i32_shl(int a, int v) {
        a <<= v;
        return a;
    }

    static int cm_i32_shl(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) << v);
        return Mem.getInt(p);
    }

    static int cl_i32_shr(int a, int v) {
        a >>= v;
        return a;
    }

    static int cm_i32_shr(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) >> v);
        return Mem.getInt(p);
    }

    static int cl_i32_and(int a, int v) {
        a &= v;
        return a;
    }

    static int cm_i32_and(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) & v);
        return Mem.getInt(p);
    }

    static int cl_i32_or(int a, int v) {
        a |= v;
        return a;
    }

    static int cm_i32_or(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) | v);
        return Mem.getInt(p);
    }

    static int cl_i32_xor(int a, int v) {
        a ^= v;
        return a;
    }

    static int cm_i32_xor(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) ^ v);
        return Mem.getInt(p);
    }

    static int cl_u32_add(int a, int v) {
        a += v;
        return a;
    }

    static int cm_u32_add(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) + v);
        return Mem.getInt(p);
    }

    static int cl_u32_sub(int a, int v) {
        a -= v;
        return a;
    }

    static int cm_u32_sub(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) - v);
        return Mem.getInt(p);
    }

    static int cl_u32_mul(int a, int v) {
        a *= v;
        return a;
    }

    static int cm_u32_mul(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) * v);
        return Mem.getInt(p);
    }

    static int cl_u32_div(int a, int v) {
        a = Integer.divideUnsigned(a, v);
        return a;
    }

    static int cm_u32_div(int p, int v) {
        Mem.setInt(p, Integer.divideUnsigned(Mem.getInt(p), v));
        return Mem.getInt(p);
    }

    static int cl_u32_rem(int a, int v) {
        a = Integer.remainderUnsigned(a, v);
        return a;
    }

    static int cm_u32_rem(int p, int v) {
        Mem.setInt(p, Integer.remainderUnsigned(Mem.getInt(p), v));
        return Mem.getInt(p);
    }

    static int cl_u32_shl(int a, int v) {
        a <<= v;
        return a;
    }

    static int cm_u32_shl(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) << v);
        return Mem.getInt(p);
    }

    static int cl_u32_shr(int a, int v) {
        a >>>= v;
        return a;
    }

    static int cm_u32_shr(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) >>> v);
        return Mem.getInt(p);
    }

    static int cl_u32_and(int a, int v) {
        a &= v;
        return a;
    }

    static int cm_u32_and(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) & v);
        return Mem.getInt(p);
    }

    static int cl_u32_or(int a, int v) {
        a |= v;
        return a;
    }

    static int cm_u32_or(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) | v);
        return Mem.getInt(p);
    }

    static int cl_u32_xor(int a, int v) {
        a ^= v;
        return a;
    }

    static int cm_u32_xor(int p, int v) {
        Mem.setInt(p, Mem.getInt(p) ^ v);
        return Mem.getInt(p);
    }

    static long cl_i64_add(long a, long v) {
        a += v;
        return a;
    }

    static long cm_i64_add(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) + v);
        return Mem.getLong(p);
    }

    static long cl_i64_sub(long a, long v) {
        a -= v;
        return a;
    }

    static long cm_i64_sub(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) - v);
        return Mem.getLong(p);
    }

    static long cl_i64_mul(long a, long v) {
        a *= v;
        return a;
    }

    static long cm_i64_mul(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) * v);
        return Mem.getLong(p);
    }

    static long cl_i64_div(long a, long v) {
        a /= v;
        return a;
    }

    static long cm_i64_div(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) / v);
        return Mem.getLong(p);
    }

    static long cl_i64_rem(long a, long v) {
        a %= v;
        return a;
    }

    static long cm_i64_rem(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) % v);
        return Mem.getLong(p);
    }

    static long cl_i64_shl(long a, long v) {
        a <<= (int) v;
        return a;
    }

    static long cm_i64_shl(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) << (int) v);
        return Mem.getLong(p);
    }

    static long cl_i64_shr(long a, long v) {
        a >>= v;
        return a;
    }

    static long cm_i64_shr(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) >> (int) v);
        return Mem.getLong(p);
    }

    static long cl_i64_and(long a, long v) {
        a &= v;
        return a;
    }

    static long cm_i64_and(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) & v);
        return Mem.getLong(p);
    }

    static long cl_i64_or(long a, long v) {
        a |= v;
        return a;
    }

    static long cm_i64_or(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) | v);
        return Mem.getLong(p);
    }

    static long cl_i64_xor(long a, long v) {
        a ^= v;
        return a;
    }

    static long cm_i64_xor(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) ^ v);
        return Mem.getLong(p);
    }

    static long cl_u64_add(long a, long v) {
        a += v;
        return a;
    }

    static long cm_u64_add(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) + v);
        return Mem.getLong(p);
    }

    static long cl_u64_sub(long a, long v) {
        a -= v;
        return a;
    }

    static long cm_u64_sub(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) - v);
        return Mem.getLong(p);
    }

    static long cl_u64_mul(long a, long v) {
        a *= v;
        return a;
    }

    static long cm_u64_mul(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) * v);
        return Mem.getLong(p);
    }

    static long cl_u64_div(long a, long v) {
        a = Long.divideUnsigned(a, v);
        return a;
    }

    static long cm_u64_div(int p, long v) {
        Mem.setLong(p, Long.divideUnsigned(Mem.getLong(p), v));
        return Mem.getLong(p);
    }

    static long cl_u64_rem(long a, long v) {
        a = Long.remainderUnsigned(a, v);
        return a;
    }

    static long cm_u64_rem(int p, long v) {
        Mem.setLong(p, Long.remainderUnsigned(Mem.getLong(p), v));
        return Mem.getLong(p);
    }

    static long cl_u64_shl(long a, long v) {
        a <<= (int) v;
        return a;
    }

    static long cm_u64_shl(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) << (int) v);
        return Mem.getLong(p);
    }

    static long cl_u64_shr(long a, long v) {
        a >>>= v;
        return a;
    }

    static long cm_u64_shr(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) >>> (int) v);
        return Mem.getLong(p);
    }

    static long cl_u64_and(long a, long v) {
        a &= v;
        return a;
    }

    static long cm_u64_and(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) & v);
        return Mem.getLong(p);
    }

    static long cl_u64_or(long a, long v) {
        a |= v;
        return a;
    }

    static long cm_u64_or(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) | v);
        return Mem.getLong(p);
    }

    static long cl_u64_xor(long a, long v) {
        a ^= v;
        return a;
    }

    static long cm_u64_xor(int p, long v) {
        Mem.setLong(p, Mem.getLong(p) ^ v);
        return Mem.getLong(p);
    }

    static float cl_float_add(float a, float v) {
        a += v;
        return a;
    }

    static float cm_float_add(int p, float v) {
        Mem.setFloat(p, Mem.getFloat(p) + v);
        return Mem.getFloat(p);
    }

    static float cl_float_div(float a, float v) {
        a /= v;
        return a;
    }

    static float cm_float_div(int p, float v) {
        Mem.setFloat(p, Mem.getFloat(p) / v);
        return Mem.getFloat(p);
    }

    static double cl_double_mul(double a, double v) {
        a *= v;
        return a;
    }

    static double cm_double_mul(int p, double v) {
        Mem.setDouble(p, Mem.getDouble(p) * v);
        return Mem.getDouble(p);
    }

    static double cl_double_sub(double a, double v) {
        a -= v;
        return a;
    }

    static double cm_double_sub(int p, double v) {
        Mem.setDouble(p, Mem.getDouble(p) - v);
        return Mem.getDouble(p);
    }

    static byte bn_i8_add(byte a, byte b) {
        return (byte) (a + b);
    }

    static byte bn_i8_sub(byte a, byte b) {
        return (byte) (a - b);
    }

    static byte bn_i8_mul(byte a, byte b) {
        return (byte) (a * b);
    }

    static byte bn_i8_div(byte a, byte b) {
        return (byte) (a / b);
    }

    static byte bn_i8_rem(byte a, byte b) {
        return (byte) (a % b);
    }

    static byte bn_i8_and(byte a, byte b) {
        return (byte) (a & b);
    }

    static byte bn_i8_or(byte a, byte b) {
        return (byte) (a | b);
    }

    static byte bn_i8_xor(byte a, byte b) {
        return (byte) (a ^ b);
    }

    static byte bn_i8_shl(byte a, byte b) {
        return (byte) (a << b);
    }

    static byte bn_i8_shr(byte a, byte b) {
        return (byte) (a >> b);
    }

    static int cp_i8_lt(byte a, byte b) {
        return a < b ? 1 : 0;
    }

    static int cp_i8_le(byte a, byte b) {
        return a <= b ? 1 : 0;
    }

    static int cp_i8_gt(byte a, byte b) {
        return a > b ? 1 : 0;
    }

    static int cp_i8_ge(byte a, byte b) {
        return a >= b ? 1 : 0;
    }

    static int cp_i8_eq(byte a, byte b) {
        return a == b ? 1 : 0;
    }

    static int cp_i8_ne(byte a, byte b) {
        return a != b ? 1 : 0;
    }

    static byte bn_u8_add(byte a, byte b) {
        return (byte) ((a & 0xFF) + (b & 0xFF));
    }

    static byte bn_u8_sub(byte a, byte b) {
        return (byte) ((a & 0xFF) - (b & 0xFF));
    }

    static byte bn_u8_mul(byte a, byte b) {
        return (byte) ((a & 0xFF) * (b & 0xFF));
    }

    static byte bn_u8_div(byte a, byte b) {
        return (byte) ((a & 0xFF) / (b & 0xFF));
    }

    static byte bn_u8_rem(byte a, byte b) {
        return (byte) ((a & 0xFF) % (b & 0xFF));
    }

    static byte bn_u8_and(byte a, byte b) {
        return (byte) (a & 0xFF & (b & 0xFF));
    }

    static byte bn_u8_or(byte a, byte b) {
        return (byte) (a & 0xFF | b & 0xFF);
    }

    static byte bn_u8_xor(byte a, byte b) {
        return (byte) (a & 0xFF ^ b & 0xFF);
    }

    static byte bn_u8_shl(byte a, byte b) {
        return (byte) ((a & 0xFF) << (b & 0xFF));
    }

    static byte bn_u8_shr(byte a, byte b) {
        return (byte) ((a & 0xFF) >> (b & 0xFF));
    }

    static int cp_u8_lt(byte a, byte b) {
        return (a & 0xFF) < (b & 0xFF) ? 1 : 0;
    }

    static int cp_u8_le(byte a, byte b) {
        return (a & 0xFF) <= (b & 0xFF) ? 1 : 0;
    }

    static int cp_u8_gt(byte a, byte b) {
        return (a & 0xFF) > (b & 0xFF) ? 1 : 0;
    }

    static int cp_u8_ge(byte a, byte b) {
        return (a & 0xFF) >= (b & 0xFF) ? 1 : 0;
    }

    static int cp_u8_eq(byte a, byte b) {
        return (a & 0xFF) == (b & 0xFF) ? 1 : 0;
    }

    static int cp_u8_ne(byte a, byte b) {
        return (a & 0xFF) != (b & 0xFF) ? 1 : 0;
    }

    static short bn_i16_add(short a, short b) {
        return (short) (a + b);
    }

    static short bn_i16_sub(short a, short b) {
        return (short) (a - b);
    }

    static short bn_i16_mul(short a, short b) {
        return (short) (a * b);
    }

    static short bn_i16_div(short a, short b) {
        return (short) (a / b);
    }

    static short bn_i16_rem(short a, short b) {
        return (short) (a % b);
    }

    static short bn_i16_and(short a, short b) {
        return (short) (a & b);
    }

    static short bn_i16_or(short a, short b) {
        return (short) (a | b);
    }

    static short bn_i16_xor(short a, short b) {
        return (short) (a ^ b);
    }

    static short bn_i16_shl(short a, short b) {
        return (short) (a << b);
    }

    static short bn_i16_shr(short a, short b) {
        return (short) (a >> b);
    }

    static int cp_i16_lt(short a, short b) {
        return a < b ? 1 : 0;
    }

    static int cp_i16_le(short a, short b) {
        return a <= b ? 1 : 0;
    }

    static int cp_i16_gt(short a, short b) {
        return a > b ? 1 : 0;
    }

    static int cp_i16_ge(short a, short b) {
        return a >= b ? 1 : 0;
    }

    static int cp_i16_eq(short a, short b) {
        return a == b ? 1 : 0;
    }

    static int cp_i16_ne(short a, short b) {
        return a != b ? 1 : 0;
    }

    static char bn_u16_add(char a, char b) {
        return (char) (a + b);
    }

    static char bn_u16_sub(char a, char b) {
        return (char) (a - b);
    }

    static char bn_u16_mul(char a, char b) {
        return (char) (a * b);
    }

    static char bn_u16_div(char a, char b) {
        return (char) (a / b);
    }

    static char bn_u16_rem(char a, char b) {
        return (char) (a % b);
    }

    static char bn_u16_and(char a, char b) {
        return (char) (a & b);
    }

    static char bn_u16_or(char a, char b) {
        return (char) (a | b);
    }

    static char bn_u16_xor(char a, char b) {
        return (char) (a ^ b);
    }

    static char bn_u16_shl(char a, char b) {
        return (char) (a << b);
    }

    static char bn_u16_shr(char a, char b) {
        return (char) (a >> b);
    }

    static int cp_u16_lt(char a, char b) {
        return a < b ? 1 : 0;
    }

    static int cp_u16_le(char a, char b) {
        return a <= b ? 1 : 0;
    }

    static int cp_u16_gt(char a, char b) {
        return a > b ? 1 : 0;
    }

    static int cp_u16_ge(char a, char b) {
        return a >= b ? 1 : 0;
    }

    static int cp_u16_eq(char a, char b) {
        return a == b ? 1 : 0;
    }

    static int cp_u16_ne(char a, char b) {
        return a != b ? 1 : 0;
    }

    static int bn_i32_add(int a, int b) {
        return a + b;
    }

    static int bn_i32_sub(int a, int b) {
        return a - b;
    }

    static int bn_i32_mul(int a, int b) {
        return a * b;
    }

    static int bn_i32_div(int a, int b) {
        return a / b;
    }

    static int bn_i32_rem(int a, int b) {
        return a % b;
    }

    static int bn_i32_and(int a, int b) {
        return a & b;
    }

    static int bn_i32_or(int a, int b) {
        return a | b;
    }

    static int bn_i32_xor(int a, int b) {
        return a ^ b;
    }

    static int bn_i32_shl(int a, int b) {
        return a << b;
    }

    static int bn_i32_shr(int a, int b) {
        return a >> b;
    }

    static int cp_i32_lt(int a, int b) {
        return a < b ? 1 : 0;
    }

    static int cp_i32_le(int a, int b) {
        return a <= b ? 1 : 0;
    }

    static int cp_i32_gt(int a, int b) {
        return a > b ? 1 : 0;
    }

    static int cp_i32_ge(int a, int b) {
        return a >= b ? 1 : 0;
    }

    static int cp_i32_eq(int a, int b) {
        return a == b ? 1 : 0;
    }

    static int cp_i32_ne(int a, int b) {
        return a != b ? 1 : 0;
    }

    static int bn_u32_add(int a, int b) {
        return a + b;
    }

    static int bn_u32_sub(int a, int b) {
        return a - b;
    }

    static int bn_u32_mul(int a, int b) {
        return a * b;
    }

    static int bn_u32_div(int a, int b) {
        return Integer.divideUnsigned(a, b);
    }

    static int bn_u32_rem(int a, int b) {
        return Integer.remainderUnsigned(a, b);
    }

    static int bn_u32_and(int a, int b) {
        return a & b;
    }

    static int bn_u32_or(int a, int b) {
        return a | b;
    }

    static int bn_u32_xor(int a, int b) {
        return a ^ b;
    }

    static int bn_u32_shl(int a, int b) {
        return a << b;
    }

    static int bn_u32_shr(int a, int b) {
        return a >>> b;
    }

    static int cp_u32_lt(int a, int b) {
        return Integer.compareUnsigned(a, b) < 0 ? 1 : 0;
    }

    static int cp_u32_le(int a, int b) {
        return Integer.compareUnsigned(a, b) <= 0 ? 1 : 0;
    }

    static int cp_u32_gt(int a, int b) {
        return Integer.compareUnsigned(a, b) > 0 ? 1 : 0;
    }

    static int cp_u32_ge(int a, int b) {
        return Integer.compareUnsigned(a, b) >= 0 ? 1 : 0;
    }

    static int cp_u32_eq(int a, int b) {
        return a == b ? 1 : 0;
    }

    static int cp_u32_ne(int a, int b) {
        return a != b ? 1 : 0;
    }

    static long bn_i64_add(long a, long b) {
        return a + b;
    }

    static long bn_i64_sub(long a, long b) {
        return a - b;
    }

    static long bn_i64_mul(long a, long b) {
        return a * b;
    }

    static long bn_i64_div(long a, long b) {
        return a / b;
    }

    static long bn_i64_rem(long a, long b) {
        return a % b;
    }

    static long bn_i64_and(long a, long b) {
        return a & b;
    }

    static long bn_i64_or(long a, long b) {
        return a | b;
    }

    static long bn_i64_xor(long a, long b) {
        return a ^ b;
    }

    static long bn_i64_shl(long a, long b) {
        return a << (int) b;
    }

    static long bn_i64_shr(long a, long b) {
        return a >> (int) b;
    }

    static int cp_i64_lt(long a, long b) {
        return a < b ? 1 : 0;
    }

    static int cp_i64_le(long a, long b) {
        return a <= b ? 1 : 0;
    }

    static int cp_i64_gt(long a, long b) {
        return a > b ? 1 : 0;
    }

    static int cp_i64_ge(long a, long b) {
        return a >= b ? 1 : 0;
    }

    static int cp_i64_eq(long a, long b) {
        return a == b ? 1 : 0;
    }

    static int cp_i64_ne(long a, long b) {
        return a != b ? 1 : 0;
    }

    static long bn_u64_add(long a, long b) {
        return a + b;
    }

    static long bn_u64_sub(long a, long b) {
        return a - b;
    }

    static long bn_u64_mul(long a, long b) {
        return a * b;
    }

    static long bn_u64_div(long a, long b) {
        return Long.divideUnsigned(a, b);
    }

    static long bn_u64_rem(long a, long b) {
        return Long.remainderUnsigned(a, b);
    }

    static long bn_u64_and(long a, long b) {
        return a & b;
    }

    static long bn_u64_or(long a, long b) {
        return a | b;
    }

    static long bn_u64_xor(long a, long b) {
        return a ^ b;
    }

    static long bn_u64_shl(long a, long b) {
        return a << (int) b;
    }

    static long bn_u64_shr(long a, long b) {
        return a >>> (int) b;
    }

    static int cp_u64_lt(long a, long b) {
        return Long.compareUnsigned(a, b) < 0 ? 1 : 0;
    }

    static int cp_u64_le(long a, long b) {
        return Long.compareUnsigned(a, b) <= 0 ? 1 : 0;
    }

    static int cp_u64_gt(long a, long b) {
        return Long.compareUnsigned(a, b) > 0 ? 1 : 0;
    }

    static int cp_u64_ge(long a, long b) {
        return Long.compareUnsigned(a, b) >= 0 ? 1 : 0;
    }

    static int cp_u64_eq(long a, long b) {
        return a == b ? 1 : 0;
    }

    static int cp_u64_ne(long a, long b) {
        return a != b ? 1 : 0;
    }

    static float bn_float_add(float a, float b) {
        return a + b;
    }

    static double bn_double_mul(double a, double b) {
        return a * b;
    }

    static int cp_double_lt(double a, double b) {
        return a < b ? 1 : 0;
    }

    static int cp_float_ge(float a, float b) {
        return a >= b ? 1 : 0;
    }

    static byte un_neg_i8(byte a) {
        return (byte) -a;
    }

    static byte un_not_i8(byte a) {
        return (byte) ~a;
    }

    static int un_lnot_i8(byte a) {
        return a == 0 ? 1 : 0;
    }

    static byte un_pre_i8(byte a) {
        return ++a;
    }

    static byte un_post_i8(byte a) {
        byte b = a++;
        return (byte) (b + a);
    }

    static byte un_mem_i8(int p) {
        byte b = Mem.postAddByte(p, -1);
        Mem.addByte(p, -1);
        return (byte) (b + Mem.getByte(p));
    }

    static byte un_neg_u8(byte a) {
        return (byte) -(a & 0xFF);
    }

    static byte un_not_u8(byte a) {
        return (byte) ~(a & 0xFF);
    }

    static int un_lnot_u8(byte a) {
        return a == 0 ? 1 : 0;
    }

    static byte un_pre_u8(byte a) {
        return ++a;
    }

    static byte un_post_u8(byte a) {
        byte b = a++;
        return (byte) ((b & 0xFF) + (a & 0xFF));
    }

    static byte un_mem_u8(int p) {
        byte b = Mem.postAddByte(p, -1);
        Mem.addByte(p, -1);
        return (byte) ((b & 0xFF) + (Mem.getByte(p) & 0xFF));
    }

    static short un_neg_i16(short a) {
        return (short) -a;
    }

    static short un_not_i16(short a) {
        return (short) ~a;
    }

    static int un_lnot_i16(short a) {
        return a == 0 ? 1 : 0;
    }

    static short un_pre_i16(short a) {
        return ++a;
    }

    static short un_post_i16(short a) {
        short b = a++;
        return (short) (b + a);
    }

    static short un_mem_i16(int p) {
        short b = Mem.postAddShort(p, -1);
        Mem.addShort(p, -1);
        return (short) (b + Mem.getShort(p));
    }

    static char un_neg_u16(char a) {
        return (char) -a;
    }

    static char un_not_u16(char a) {
        return (char) ~a;
    }

    static int un_lnot_u16(char a) {
        return a == 0 ? 1 : 0;
    }

    static char un_pre_u16(char a) {
        return ++a;
    }

    static char un_post_u16(char a) {
        char b = a++;
        return (char) (b + a);
    }

    static char un_mem_u16(int p) {
        char b = Mem.postAddChar(p, -1);
        Mem.addChar(p, -1);
        return (char) (b + Mem.getChar(p));
    }

    static int un_neg_i32(int a) {
        return -a;
    }

    static int un_not_i32(int a) {
        return ~a;
    }

    static int un_lnot_i32(int a) {
        return a == 0 ? 1 : 0;
    }

    static int un_pre_i32(int a) {
        return ++a;
    }

    static int un_post_i32(int a) {
        int b = a++;
        return b + a;
    }

    static int un_mem_i32(int p) {
        int b = Mem.postAddInt(p, -1);
        Mem.addInt(p, -1);
        return b + Mem.getInt(p);
    }

    static int un_neg_u32(int a) {
        return -a;
    }

    static int un_not_u32(int a) {
        return ~a;
    }

    static int un_lnot_u32(int a) {
        return a == 0 ? 1 : 0;
    }

    static int un_pre_u32(int a) {
        return ++a;
    }

    static int un_post_u32(int a) {
        int b = a++;
        return b + a;
    }

    static int un_mem_u32(int p) {
        int b = Mem.postAddInt(p, -1);
        Mem.addInt(p, -1);
        return b + Mem.getInt(p);
    }

    static long un_neg_i64(long a) {
        return -a;
    }

    static long un_not_i64(long a) {
        return ~a;
    }

    static int un_lnot_i64(long a) {
        return a == 0 ? 1 : 0;
    }

    static long un_pre_i64(long a) {
        return ++a;
    }

    static long un_post_i64(long a) {
        long b = a++;
        return b + a;
    }

    static long un_mem_i64(int p) {
        long b = Mem.postAddLong(p, -1L);
        Mem.addLong(p, -1L);
        return b + Mem.getLong(p);
    }

    static long un_neg_u64(long a) {
        return -a;
    }

    static long un_not_u64(long a) {
        return ~a;
    }

    static int un_lnot_u64(long a) {
        return a == 0 ? 1 : 0;
    }

    static long un_pre_u64(long a) {
        return ++a;
    }

    static long un_post_u64(long a) {
        long b = a++;
        return b + a;
    }

    static long un_mem_u64(int p) {
        long b = Mem.postAddLong(p, -1L);
        Mem.addLong(p, -1L);
        return b + Mem.getLong(p);
    }

    static int chars(int k) {
        int frame = Mem.enter(16);
        try {
            char us = (char) 127;
            int r = 'a' + '\n' + '\0' + '\'' + '\\' + 7 + 'A' + 127;
            us = 'a';
            us += '\t';
            us = (char) 127;
            r += us;
            int buf = frame;  // char buf[8]
            Mem.setString(buf, "abc", 8);
            r += Mem.getByte(buf) + Mem.getByte(buf + 1) - '0' + k;
            return r;
        } finally {
            Mem.leave(frame);
        }
    }

    static long mixed(int a, int b, long c, long d, double e, float f) {
        return Integer.toUnsignedLong(a * b) + c / 3L - (d >>> 3) + (long) (e * (double) f) + (long) (Integer.compareUnsigned(a, b) < 0 ? 1 : 0) + (long) (Long.compareUnsigned(c, d) > 0 ? 1 : 0) + (a != 0 ? c : d) + Integer.toUnsignedLong(e > (double) f ? a : b);
    }

    static int ptrs(int n) {
        int frame = Mem.enter(32);
        try {
            int arr = frame;  // int arr[6]
            Mem.setInts(arr, 1, 2, 3, 4, 5, 6);
            int p = arr + 4;
            int q = arr + 16;
            p += 2 * 4;
            q -= 1 * 4;
            int d = (int) (long) ((q - p) / 4);
            return d + Mem.getInt(p) + Mem.getInt(p + -4) + Mem.getInt((arr + 20)) + n;
        } finally {
            Mem.leave(frame);
        }
    }

    static int main() {
        int frame = Mem.enter(32);
        try {
            long acc = 0L;
            int s16 = frame;  // unsigned short s16
            Mem.setChar(s16, (char) 1000);
            int s64 = frame + 8;  // long long s64
            Mem.setLong(s64, -17L);
            int s32 = frame + 16;  // unsigned s32
            Mem.setInt(s32, 5);
            int s8 = frame + 20;  // signed char s8
            Mem.setByte(s8, (byte) -128);
            acc += Integer.toUnsignedLong(cv_i8_i32(300) + (cv_u8_i32(-1) & 0xFF) + cv_i16_u32(70000) + cv_u16_i64(-2L) + cv_i32_double(-3.9) + cv_u32_double(4000000000.0));
            acc += cv_i64_double(-25000000000.0) + (cv_u64_double(1.0e+19) >>> 60) + (long) cv_float_u64(0xF9CCD8A1C5080000L);
            acc = (long) ((double) acc + ((double) (long) cv_double_u32(0xEE6B2800) + cv_double_float(1.5f) + (double) (cv_u8_u64(1000L) & 0xFF) + (double) cv_i32_u8((byte) 250)));
            acc += Integer.toUnsignedLong(cl_i8_add((byte) 100, (byte) 100) + cm_u16_shl(s16, (char) 3) + cl_u32_shr(-2147483648, 31)) + cm_i64_rem(s64, 5L);
            acc += Integer.toUnsignedLong(bn_i8_add((byte) 100, (byte) 3) + bn_u32_div(0xEE6B2800, 7)) + bn_u64_rem(0x7CE66C50E2840000L, 11L) + (long) bn_i16_shr((short) -300, (short) 2);
            acc += (long) (cp_u32_lt(1, 0xEE6B2800) + cp_i8_ge((byte) -1, (byte) 1) + cp_u64_ge(1L << 63, 5L) + cp_double_lt(1.0, 2.0) + cp_float_ge(2.0f, 3.0f));
            acc += Integer.toUnsignedLong(un_neg_i8((byte) -128) + (un_not_u8((byte) 5) & 0xFF) + un_lnot_i64(0L) + un_pre_u16((char) 65535) + un_post_i32(7) + un_mem_u32(s32) + un_mem_i8(s8));
            acc += (long) chars(3) + mixed(-3, 7, 100000000000L, 80000000000L, 2.5, 4.0f) + (long) ptrs(2);
            acc = (long) ((double) acc + ((double) bn_float_add(0.25f, 0.5f) + bn_double_mul(1.5, 4.0)));
            return (int) (((acc % 251L) + 251L) % 251L);
        } finally {
            Mem.leave(frame);
        }
    }

    public static void main(String[] args) {
        exit(main());
    }
}
