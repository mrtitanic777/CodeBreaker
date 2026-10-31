package crt;

import java.lang.invoke.MethodHandles;
import java.lang.invoke.VarHandle;
import java.nio.ByteOrder;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.Map;

/**
 * C's memory, emulated: one little-endian byte array that addresses index. The faithful tier of
 * CodeBreaker's Java back end keeps here everything a C program can reach through a pointer -- its
 * arrays, its structs, every variable whose address it takes, the stack frames that hold them, its
 * heap -- so pointer arithmetic, unions and type punning mean what they meant in C.
 *
 * <p>Address 0 is NULL and nothing lives below 0x1000. The stack is [0x1000, 0x1000 + STACK); the
 * globals, string literals and the heap come after it, growing upward. A pointer is stored in 8 bytes
 * (the source's LLP64 layout), its value an int.
 */
public final class Mem {
    private Mem() {
    }

    public static final int NULL = 0;
    static final int STACK_BASE = 0x1000;
    static final int STACK = 4 << 20;

    static byte[] m = new byte[STACK_BASE + STACK + (1 << 20)];
    static int sp = STACK_BASE;
    static int brk = STACK_BASE + STACK;

    private static final VarHandle SHORT = MethodHandles.byteArrayViewVarHandle(short[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle CHAR = MethodHandles.byteArrayViewVarHandle(char[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle INT = MethodHandles.byteArrayViewVarHandle(int[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle LONG = MethodHandles.byteArrayViewVarHandle(long[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle FLOAT = MethodHandles.byteArrayViewVarHandle(float[].class, ByteOrder.LITTLE_ENDIAN);
    private static final VarHandle DOUBLE = MethodHandles.byteArrayViewVarHandle(double[].class, ByteOrder.LITTLE_ENDIAN);

    // ------------------------------------------------------------------------------- the stack
    /** A function's frame: size bytes, zeroed, aligned to 16. Returns its address. */
    public static int enter(int size) {
        int base = (sp + 15) & ~15;
        if (base + size > STACK_BASE + STACK) {
            throw new StackOverflowError("the emulated C stack is full");
        }
        java.util.Arrays.fill(m, base, base + size, (byte) 0);
        sp = base + size;
        return base;
    }

    /** Leaves the frame that enter returned. */
    public static void leave(int frame) {
        sp = frame;
    }

    // ------------------------------------------------------------------------------- static data and heap
    private static int bump(int size, int align) {
        int a = (brk + align - 1) & -align;
        long end = (long) a + size;
        if (end > Integer.MAX_VALUE - 16) {
            throw new OutOfMemoryError("the emulated C memory is full");
        }
        if (end > m.length) {
            long n = m.length;
            while (n < end) {
                n *= 2;
            }
            m = java.util.Arrays.copyOf(m, (int) Math.min(n, Integer.MAX_VALUE - 16));
        }
        brk = (int) end;
        return a;
    }

    /** A global variable's memory (zeroed): its address. */
    public static int global(int size, int align) {
        return bump(Math.max(size, 1), Math.max(align, 1));
    }

    private static final Map<String, Integer> STRINGS = new HashMap<>();

    /** A string literal's address: its bytes and a NUL, one copy per literal text. */
    public static int str(String s) {
        Integer a = STRINGS.get(s);
        if (a == null) {
            a = global(s.length() + 1, 1);
            for (int i = 0; i < s.length(); i++) {
                m[a + i] = (byte) s.charAt(i);
            }
            STRINGS.put(s, a);
        }
        return a;
    }

    private static final Map<String, Integer> WSTRINGS = new HashMap<>();

    /** A wide (wchar_t, UTF-16) string literal's address. */
    public static int wstr(String s) {
        Integer a = WSTRINGS.get(s);
        if (a == null) {
            a = global(2 * (s.length() + 1), 2);
            for (int i = 0; i < s.length(); i++) {
                setChar(a + 2 * i, s.charAt(i));
            }
            WSTRINGS.put(s, a);
        }
        return a;
    }

    /** The wide C string at a. */
    public static String wcstr(int a) {
        StringBuilder sb = new StringBuilder();
        for (int i = a; getChar(i) != 0; i += 2) {
            sb.append(getChar(i));
        }
        return sb.toString();
    }

    private static final Map<Integer, ArrayDeque<Integer>> FREE = new HashMap<>();
    private static final Map<Integer, Integer> SIZES = new HashMap<>();

    public static int malloc(long n) {
        if (n < 0 || n > Integer.MAX_VALUE / 2) {
            return NULL;
        }
        int size = (int) ((Math.max(n, 1) + 15) & ~15);
        ArrayDeque<Integer> q = FREE.get(size);
        int a;
        if (q != null && !q.isEmpty()) {
            a = q.pop();
            java.util.Arrays.fill(m, a, a + size, (byte) 0xCD);
        } else {
            a = bump(size, 16);
        }
        SIZES.put(a, size);
        return a;
    }

    public static int calloc(long n, long size) {
        int a = malloc(n * size);
        if (a != NULL) {
            java.util.Arrays.fill(m, a, a + SIZES.get(a), (byte) 0);
        }
        return a;
    }

    public static int realloc(int p, long n) {
        if (p == NULL) {
            return malloc(n);
        }
        Integer old = SIZES.get(p);
        int b = malloc(n);
        if (b != NULL && old != null) {
            System.arraycopy(m, p, m, b, (int) Math.min(old, n));
            free(p);
        }
        return b;
    }

    public static void free(int p) {
        if (p == NULL) {
            return;
        }
        Integer size = SIZES.remove(p);
        if (size != null) {
            FREE.computeIfAbsent(size, k -> new ArrayDeque<>()).push(p);
        }
    }

    // ------------------------------------------------------------------------------- loads and stores
    public static byte getByte(int a) { return m[a]; }
    public static short getShort(int a) { return (short) SHORT.get(m, a); }
    public static char getChar(int a) { return (char) CHAR.get(m, a); }
    public static int getInt(int a) { return (int) INT.get(m, a); }
    public static long getLong(int a) { return (long) LONG.get(m, a); }
    public static float getFloat(int a) { return (float) FLOAT.get(m, a); }
    public static double getDouble(int a) { return (double) DOUBLE.get(m, a); }
    public static boolean getBool(int a) { return m[a] != 0; }
    /** A pointer: 8 bytes in memory, an int address in Java. */
    public static int getPtr(int a) { return (int) INT.get(m, a); }

    public static byte setByte(int a, byte v) { m[a] = v; return v; }
    public static short setShort(int a, short v) { SHORT.set(m, a, v); return v; }
    public static char setChar(int a, char v) { CHAR.set(m, a, v); return v; }
    public static int setInt(int a, int v) { INT.set(m, a, v); return v; }
    public static long setLong(int a, long v) { LONG.set(m, a, v); return v; }
    public static float setFloat(int a, float v) { FLOAT.set(m, a, v); return v; }
    public static double setDouble(int a, double v) { DOUBLE.set(m, a, v); return v; }
    public static boolean setBool(int a, boolean v) { m[a] = (byte) (v ? 1 : 0); return v; }
    public static int setPtr(int a, int v) { LONG.set(m, a, (long) v & 0xFFFFFFFFL); return v; }

    // ++ and -- on memory: add* gives the new value (prefix), postAdd* the old one (postfix)
    public static byte addByte(int a, int d) { return setByte(a, (byte) (m[a] + d)); }
    public static short addShort(int a, int d) { return setShort(a, (short) (getShort(a) + d)); }
    public static char addChar(int a, int d) { return setChar(a, (char) (getChar(a) + d)); }
    public static int addInt(int a, int d) { return setInt(a, getInt(a) + d); }
    public static long addLong(int a, long d) { return setLong(a, getLong(a) + d); }
    public static float addFloat(int a, float d) { return setFloat(a, getFloat(a) + d); }
    public static double addDouble(int a, double d) { return setDouble(a, getDouble(a) + d); }
    public static int addPtr(int a, int d) { return setPtr(a, getPtr(a) + d); }
    public static byte postAddByte(int a, int d) { byte v = m[a]; m[a] = (byte) (v + d); return v; }
    public static short postAddShort(int a, int d) { short v = getShort(a); setShort(a, (short) (v + d)); return v; }
    public static char postAddChar(int a, int d) { char v = getChar(a); setChar(a, (char) (v + d)); return v; }
    public static int postAddInt(int a, int d) { int v = getInt(a); setInt(a, v + d); return v; }
    public static long postAddLong(int a, long d) { long v = getLong(a); setLong(a, v + d); return v; }
    public static float postAddFloat(int a, float d) { float v = getFloat(a); setFloat(a, v + d); return v; }
    public static double postAddDouble(int a, double d) { double v = getDouble(a); setDouble(a, v + d); return v; }
    public static int postAddPtr(int a, int d) { int v = getPtr(a); setPtr(a, v + d); return v; }

    // array initialisers: consecutive elements from a
    public static int setBytes(int a, int... v) { for (int i = 0; i < v.length; i++) m[a + i] = (byte) v[i]; return a; }
    public static int setShorts(int a, int... v) { for (int i = 0; i < v.length; i++) setShort(a + 2 * i, (short) v[i]); return a; }
    public static int setChars(int a, int... v) { for (int i = 0; i < v.length; i++) setChar(a + 2 * i, (char) v[i]); return a; }
    public static int setInts(int a, int... v) { for (int i = 0; i < v.length; i++) setInt(a + 4 * i, v[i]); return a; }
    public static int setLongs(int a, long... v) { for (int i = 0; i < v.length; i++) setLong(a + 8 * i, v[i]); return a; }
    public static int setFloats(int a, float... v) { for (int i = 0; i < v.length; i++) setFloat(a + 4 * i, v[i]); return a; }
    public static int setDoubles(int a, double... v) { for (int i = 0; i < v.length; i++) setDouble(a + 8 * i, v[i]); return a; }
    public static int setPtrs(int a, int... v) { for (int i = 0; i < v.length; i++) setPtr(a + 8 * i, v[i]); return a; }
    public static int setBools(int a, boolean... v) { for (int i = 0; i < v.length; i++) setBool(a + i, v[i]); return a; }

    /**
     * A variadic function's extra arguments as MSVC's va_list sees them: one 8-byte slot each, on the
     * stack above the caller's frame (the callee's Mem.leave releases them with its own frame).
     */
    public static int varargs(Object[] va) {
        int a = (sp + 15) & ~15;
        if (a + 8 * va.length + 8 > STACK_BASE + STACK) {
            throw new StackOverflowError("the emulated C stack is full");
        }
        sp = a + 8 * va.length + 8;
        for (int i = 0; i < va.length; i++) {
            Object o = va[i];
            long v;
            if (o instanceof Double || o instanceof Float) {
                v = Double.doubleToRawLongBits(((Number) o).doubleValue());
            } else if (o instanceof Number) {
                v = ((Number) o).longValue();
            } else if (o instanceof Character) {
                v = (Character) o;
            } else if (o instanceof Boolean) {
                v = ((Boolean) o) ? 1 : 0;
            } else if (o instanceof String) {
                v = str((String) o);
            } else {
                v = 0;
            }
            setLong(a + 8 * i, v);
        }
        return a;
    }

    /** A char array initialised from a string literal: its bytes, then zeros to n. */
    public static int setString(int a, String s, int n) {
        for (int i = 0; i < n; i++) {
            m[a + i] = i < s.length() ? (byte) s.charAt(i) : 0;
        }
        return a;
    }

    /** memset. */
    public static int fill(int a, int v, long n) {
        java.util.Arrays.fill(m, a, (int) (a + n), (byte) v);
        return a;
    }

    /** memmove: n bytes from src to dst. */
    public static int copy(int dst, int src, long n) {
        System.arraycopy(m, src, m, dst, (int) n);
        return dst;
    }

    /** The C string at a, one character per byte. */
    public static String cstr(int a) {
        if (a == NULL) {
            return "(null)";
        }
        int e = a;
        while (m[e] != 0) {
            e++;
        }
        StringBuilder sb = new StringBuilder(e - a);
        for (int i = a; i < e; i++) {
            sb.append((char) (m[i] & 0xFF));
        }
        return sb.toString();
    }

    /** main's argv: the program name and the arguments as C strings, then NULL. */
    public static int argv(String program, String[] args) {
        int v = global(8 * (args.length + 2), 8);
        setPtr(v, str(program));
        for (int i = 0; i < args.length; i++) {
            setPtr(v + 8 * (i + 1), str(args[i]));
        }
        return v;
    }
}
