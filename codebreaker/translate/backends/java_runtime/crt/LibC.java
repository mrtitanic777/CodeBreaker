package crt;

import java.io.BufferedOutputStream;
import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;
import java.util.function.IntBinaryOperator;

/**
 * C's standard library for translated C programs (CodeBreaker's Java back end), statically imported so
 * calls read as they did in C. Strings are bytes: a Java String here holds one character per byte.
 * A {@code char *} parameter is an Object -- a Java String (a literal the translation passed as is) or
 * an Integer address in {@link Mem}. Output is written byte for byte; printf formats as C does
 * (exactly rounded %f/%e/%g, two-digit exponents, the LLP64 length modifiers).
 */
public final class LibC {
    private LibC() {
    }

    public static final int EOF = -1;
    private static final OutputStream OUT = new BufferedOutputStream(new FileOutputStream(FileDescriptor.out), 1 << 16);
    private static final OutputStream ERR = new FileOutputStream(FileDescriptor.err);
    private static final InputStream IN = System.in;

    // ------------------------------------------------------------------------------- helpers
    /** A char * argument as a String (its bytes). */
    public static String text(Object s) {
        if (s instanceof String) {
            return (String) s;
        }
        if (s == null) {
            return "(null)";
        }
        if (s instanceof byte[]) {
            byte[] b = (byte[]) s;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < b.length && b[i] != 0; i++) {
                sb.append((char) (b[i] & 0xFF));
            }
            return sb.toString();
        }
        return Mem.cstr(((Number) s).intValue());
    }

    /** Writes t and zeros up to n bytes at char * d, from byte off: an array (the idiomatic tier) or an address. */
    private static int put(Object d, int off, String t, int n) {
        if (d instanceof byte[]) {
            byte[] b = (byte[]) d;
            for (int i = 0; i < n; i++) {
                b[off + i] = i < t.length() ? (byte) t.charAt(i) : 0;
            }
            return 0;
        }
        int a = addr(d);
        Mem.setString(a + off, t, n);
        return a;
    }

    /** A string as a NUL-terminated byte array (the idiomatic tier's char array / string literal). */
    public static byte[] bytes(String s) {
        return bytes(s, s.length() + 1);
    }

    public static byte[] bytes(String s, int n) {
        byte[] b = new byte[n];
        for (int i = 0; i < Math.min(n, s.length()); i++) {
            b[i] = (byte) s.charAt(i);
        }
        return b;
    }

    /** An array of structs, each element a fresh zero struct (C's array of struct values). */
    public static <T> T[] filled(T[] a, java.util.function.Supplier<T> make) {
        for (int i = 0; i < a.length; i++) {
            a[i] = make.get();
        }
        return a;
    }

    /** main's argv in the idiomatic tier: the program name and the arguments as byte arrays, then null. */
    public static byte[][] argv(String program, String[] args) {
        byte[][] v = new byte[args.length + 2][];
        v[0] = bytes(program);
        for (int i = 0; i < args.length; i++) {
            v[i + 1] = bytes(args[i]);
        }
        return v;
    }

    /** A copy of a (multi-dimensional) array of scalars. */
    @SuppressWarnings("unchecked")
    public static <T> T deepCopy(T a) {
        if (a instanceof Object[]) {
            Object[] src = (Object[]) a;
            Object[] dst = src.clone();
            for (int i = 0; i < src.length; i++) {
                dst[i] = deepCopy(src[i]);
            }
            return (T) dst;
        }
        if (a instanceof int[]) return (T) ((int[]) a).clone();
        if (a instanceof byte[]) return (T) ((byte[]) a).clone();
        if (a instanceof long[]) return (T) ((long[]) a).clone();
        if (a instanceof double[]) return (T) ((double[]) a).clone();
        if (a instanceof float[]) return (T) ((float[]) a).clone();
        if (a instanceof short[]) return (T) ((short[]) a).clone();
        if (a instanceof char[]) return (T) ((char[]) a).clone();
        if (a instanceof boolean[]) return (T) ((boolean[]) a).clone();
        return a;
    }

    /** Copies a (multi-dimensional) array of scalars into one of the same shape. */
    public static void copyInto(Object dst, Object src) {
        if (src instanceof Object[]) {
            Object[] d = (Object[]) dst, s = (Object[]) src;
            for (int i = 0; i < s.length; i++) {
                copyInto(d[i], s[i]);
            }
        } else {
            System.arraycopy(src, 0, dst, 0, java.lang.reflect.Array.getLength(src));
        }
    }

    /** A char * argument as an address (a String literal is placed in memory first). */
    public static int addr(Object s) {
        if (s instanceof String) {
            return Mem.str((String) s);
        }
        return ((Number) s).intValue();
    }

    private static long num(Object o) {
        if (o instanceof Number) {
            return ((Number) o).longValue();
        }
        if (o instanceof Character) {
            return (Character) o;
        }
        if (o instanceof Boolean) {
            return ((Boolean) o) ? 1 : 0;
        }
        if (o instanceof String) {
            return Mem.str((String) o);
        }
        return 0;
    }

    private static double dbl(Object o) {
        if (o instanceof Number) {
            return ((Number) o).doubleValue();
        }
        return num(o);
    }

    /** C's comma operator inside an expression: evaluates a, gives b. */
    public static int seq(Object a, int b) { return b; }
    public static long seq(Object a, long b) { return b; }
    public static double seq(Object a, double b) { return b; }
    public static float seq(Object a, float b) { return b; }
    public static byte seq(Object a, byte b) { return b; }
    public static short seq(Object a, short b) { return b; }
    public static char seq(Object a, char b) { return b; }
    public static boolean seq(Object a, boolean b) { return b; }

    /** An expression statement whose value C discards and Java would not accept as a statement. */
    public static void discard(Object x) {
    }

    public static double unsignedToDouble(long v) {
        double d = (double) (v >>> 1) * 2.0 + (v & 1);
        return v >= 0 ? (double) v : d;
    }

    public static long doubleToUnsigned(double d) {
        if (d >= 9223372036854775808.0) {
            return (long) (d - 9223372036854775808.0) + Long.MIN_VALUE;
        }
        return (long) d;
    }

    // ------------------------------------------------------------------------------- streams
    /** MSVC's __acrt_iob_func: stdin, stdout, stderr as FILE * values 1, 2, 3. */
    public static int stream(int which) {
        return which + 1;
    }

    private static void write(int f, String s) {
        try {
            OutputStream o = f == 3 ? ERR : OUT;
            byte[] b = new byte[s.length()];
            for (int i = 0; i < b.length; i++) {
                b[i] = (byte) s.charAt(i);
            }
            if (f == 3) {
                OUT.flush();
            }
            o.write(b);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void exit(int status) {
        try {
            OUT.flush();
            ERR.flush();
        } catch (IOException e) {
            // nothing more can be said
        }
        System.exit(status);
    }

    public static void abort() {
        write(3, "abnormal program termination\n");
        exit(3);
    }

    /** MSVC's assert failure (_wassert): its message on stderr, then abort's exit code. */
    public static void wassert(Object message, Object file, int line) {
        String m = message instanceof String ? (String) message : Mem.wcstr(((Number) message).intValue());
        String f = file instanceof String ? (String) file : Mem.wcstr(((Number) file).intValue());
        write(3, "Assertion failed: " + m + ", file " + f + ", line " + line + "\n");
        exit(3);
    }

    public static int fflush(int f) {
        try {
            OUT.flush();
        } catch (IOException e) {
            return EOF;
        }
        return 0;
    }

    public static int printf(Object fmt, Object... args) {
        String s = format(text(fmt), args);
        write(2, s);
        return s.length();
    }

    public static int vprintf(Object fmt, int ap) {
        String s = format(text(fmt), new SlotArgs(ap));
        write(2, s);
        return s.length();
    }

    public static int vfprintf(int f, Object fmt, int ap) {
        String s = format(text(fmt), new SlotArgs(ap));
        write(f, s);
        return s.length();
    }

    public static int vsprintf(int dst, Object fmt, int ap) {
        String s = format(text(fmt), new SlotArgs(ap));
        Mem.setString(dst, s, s.length() + 1);
        return s.length();
    }

    public static int vsnprintf(int dst, long n, Object fmt, int ap) {
        String s = format(text(fmt), new SlotArgs(ap));
        if (n > 0) {
            int k = (int) Math.min(s.length(), n - 1);
            Mem.setString(dst, s.substring(0, k), k + 1);
        }
        return s.length();
    }

    public static int fprintf(int f, Object fmt, Object... args) {
        String s = format(text(fmt), args);
        write(f, s);
        return s.length();
    }

    public static int sprintf(Object dst, Object fmt, Object... args) {
        String s = format(text(fmt), args);
        put(dst, 0, s, s.length() + 1);
        return s.length();
    }

    public static int snprintf(Object dst, long n, Object fmt, Object... args) {
        String s = format(text(fmt), args);
        if (n > 0) {
            int k = (int) Math.min(s.length(), n - 1);
            put(dst, 0, s.substring(0, k), k + 1);
        }
        return s.length();
    }

    public static int puts(Object s) {
        write(2, text(s) + "\n");
        return 0;
    }

    public static int fputs(Object s, int f) {
        write(f, text(s));
        return 0;
    }

    public static int putchar(int c) {
        write(2, String.valueOf((char) (c & 0xFF)));
        return c & 0xFF;
    }

    public static int fputc(int c, int f) {
        write(f, String.valueOf((char) (c & 0xFF)));
        return c & 0xFF;
    }

    public static int putc(int c, int f) {
        return fputc(c, f);
    }

    public static int getchar() {
        try {
            OUT.flush();
            return IN.read();
        } catch (IOException e) {
            return EOF;
        }
    }

    public static int fgets(int buf, int n, int f) {
        int i = 0;
        while (i < n - 1) {
            int c = getchar();
            if (c < 0) {
                break;
            }
            Mem.setByte(buf + i++, (byte) c);
            if (c == '\n') {
                break;
            }
        }
        if (i == 0) {
            return Mem.NULL;
        }
        Mem.setByte(buf + i, (byte) 0);
        return buf;
    }

    // ------------------------------------------------------------------------------- printf
    /** C's printf formatting (the LLP64 length modifiers of the source's target). */
    public static String format(String f, Object[] args) {
        return format(f, new ArrayArgs(args));
    }

    /** Where printf's arguments come from: Java values (a call), or 8-byte slots in memory (a va_list). */
    public interface Args {
        long nextLong();

        double nextDouble();

        String nextText();
    }

    static final class ArrayArgs implements Args {
        private final Object[] a;
        private int i;

        ArrayArgs(Object[] a) {
            this.a = a;
        }

        public long nextLong() {
            return num(a[i++]);
        }

        public double nextDouble() {
            return dbl(a[i++]);
        }

        public String nextText() {
            Object s = a[i++];
            return s == null || (s instanceof Number && ((Number) s).intValue() == 0) ? "(null)" : text(s);
        }
    }

    /** A va_list: the variadic arguments as 8-byte slots at an address (crt.Mem.varargs). */
    static final class SlotArgs implements Args {
        private int ap;

        SlotArgs(int ap) {
            this.ap = ap;
        }

        public long nextLong() {
            long v = Mem.getLong(ap);
            ap += 8;
            return v;
        }

        public double nextDouble() {
            double v = Mem.getDouble(ap);
            ap += 8;
            return v;
        }

        public String nextText() {
            int p = Mem.getPtr(ap);
            ap += 8;
            return p == 0 ? "(null)" : Mem.cstr(p);
        }
    }

    public static String format(String f, Args a) {
        StringBuilder o = new StringBuilder();
        int n = f.length();
        for (int i = 0; i < n; i++) {
            char c = f.charAt(i);
            if (c != '%') {
                o.append(c);
                continue;
            }
            if (++i >= n) {
                break;
            }
            if (f.charAt(i) == '%') {
                o.append('%');
                continue;
            }
            boolean left = false, plus = false, space = false, alt = false, zero = false;
            for (; i < n; i++) {
                char ch = f.charAt(i);
                if (ch == '-') left = true;
                else if (ch == '+') plus = true;
                else if (ch == ' ') space = true;
                else if (ch == '#') alt = true;
                else if (ch == '0') zero = true;
                else break;
            }
            int width = 0;
            if (i < n && f.charAt(i) == '*') {
                width = (int) a.nextLong();
                if (width < 0) {
                    left = true;
                    width = -width;
                }
                i++;
            } else {
                while (i < n && Character.isDigit(f.charAt(i))) {
                    width = width * 10 + (f.charAt(i++) - '0');
                }
            }
            int prec = -1;
            if (i < n && f.charAt(i) == '.') {
                i++;
                prec = 0;
                if (i < n && f.charAt(i) == '*') {
                    prec = (int) a.nextLong();
                    if (prec < 0) prec = -1;
                    i++;
                } else {
                    while (i < n && Character.isDigit(f.charAt(i))) {
                        prec = prec * 10 + (f.charAt(i++) - '0');
                    }
                }
            }
            int bits = 32;
            while (i < n) {
                char ch = f.charAt(i);
                if (ch == 'h') {
                    bits = bits == 16 ? 8 : 16;
                    i++;
                } else if (ch == 'l') {
                    bits = bits == 32 && i + 1 < n && f.charAt(i + 1) == 'l' ? 64 : 32;
                    i += bits == 64 ? 2 : 1;
                } else if (ch == 'z' || ch == 'j' || ch == 't') {
                    bits = 64;
                    i++;
                } else if (ch == 'L' || ch == 'w') {
                    i++;
                } else if (ch == 'I') {
                    if (f.startsWith("I64", i)) { bits = 64; i += 3; }
                    else if (f.startsWith("I32", i)) { bits = 32; i += 3; }
                    else { bits = 64; i++; }
                } else {
                    break;
                }
            }
            if (i >= n) {
                break;
            }
            char conv = f.charAt(i);
            String prefix = "";
            String body;
            boolean numeric = true;
            switch (conv) {
                case 'd':
                case 'i': {
                    long v = a.nextLong();
                    v = bits == 8 ? (byte) v : bits == 16 ? (short) v : bits == 32 ? (int) v : v;
                    String digits = v < 0 ? (v == Long.MIN_VALUE ? "9223372036854775808" : Long.toString(-v)) : Long.toString(v);
                    body = precise(digits, prec, v == 0);
                    prefix = v < 0 ? "-" : plus ? "+" : space ? " " : "";
                    break;
                }
                case 'u':
                case 'x':
                case 'X':
                case 'o': {
                    long v = a.nextLong();
                    v = bits == 8 ? v & 0xFF : bits == 16 ? v & 0xFFFF : bits == 32 ? v & 0xFFFFFFFFL : v;
                    int radix = conv == 'u' ? 10 : conv == 'o' ? 8 : 16;
                    String digits = Long.toUnsignedString(v, radix);
                    if (conv == 'X') digits = digits.toUpperCase();
                    body = precise(digits, prec, v == 0);
                    if (alt && conv == 'o' && !body.startsWith("0")) body = "0" + body;
                    if (alt && v != 0 && conv == 'x') prefix = "0x";
                    if (alt && v != 0 && conv == 'X') prefix = "0X";
                    break;
                }
                case 'c':
                    body = String.valueOf((char) (a.nextLong() & 0xFF));
                    numeric = false;
                    break;
                case 's': {
                    body = a.nextText();
                    if (prec >= 0 && body.length() > prec) body = body.substring(0, prec);
                    numeric = false;
                    break;
                }
                case 'p': {
                    long v = a.nextLong() & 0xFFFFFFFFL;
                    body = String.format("%016X", v);
                    numeric = false;
                    break;
                }
                case 'f':
                case 'F':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                case 'a':
                case 'A': {
                    double d = a.nextDouble();
                    boolean neg = d < 0 || (d == 0 && 1 / d < 0) || (Double.isNaN(d) && Double.doubleToRawLongBits(d) < 0);
                    body = floating(Math.abs(d), conv, prec, alt);
                    prefix = neg ? "-" : plus ? "+" : space ? " " : "";
                    if (Double.isNaN(d) || Double.isInfinite(d)) zero = false;
                    break;
                }
                case 'n':
                    a.nextLong();
                    continue;
                default:
                    body = String.valueOf(conv);
                    numeric = false;
            }
            int len = prefix.length() + body.length();
            if (len >= width) {
                o.append(prefix).append(body);
            } else if (left) {
                o.append(prefix).append(body);
                pad(o, ' ', width - len);
            } else if (zero && numeric && (prec < 0 || "fFeEgGaA".indexOf(conv) >= 0)) {
                o.append(prefix);
                pad(o, '0', width - len);
                o.append(body);
            } else {
                pad(o, ' ', width - len);
                o.append(prefix).append(body);
            }
        }
        return o.toString();
    }

    private static void pad(StringBuilder o, char c, int k) {
        for (int i = 0; i < k; i++) {
            o.append(c);
        }
    }

    private static String precise(String digits, int prec, boolean isZero) {
        if (prec == 0 && isZero) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (int k = digits.length(); k < prec; k++) {
            sb.append('0');
        }
        return sb.append(digits).toString();
    }

    private static String floating(double d, char conv, int prec, boolean alt) {
        boolean upper = Character.isUpperCase(conv);
        if (Double.isNaN(d)) {
            return upper ? "NAN" : "nan";
        }
        if (Double.isInfinite(d)) {
            return upper ? "INF" : "inf";
        }
        char lc = Character.toLowerCase(conv);
        String s;
        if (lc == 'f') {
            s = fixed(d, prec < 0 ? 6 : prec, alt);
        } else if (lc == 'e') {
            s = expo(d, prec < 0 ? 6 : prec, alt);
        } else if (lc == 'g') {
            int p = prec < 0 ? 6 : prec == 0 ? 1 : prec;
            int x = d == 0 ? 0 : exponent(d, p);
            if (p > x && x >= -4) {
                s = fixed(d, p - 1 - x, alt);
            } else {
                s = expo(d, p - 1, alt);
            }
            if (!alt) {
                s = trimZeros(s);
            }
        } else {
            s = Double.toHexString(d).replace("0x1.0p", "0x1p");
        }
        return upper ? s.toUpperCase() : s;
    }

    private static String fixed(double d, int prec, boolean alt) {
        String s = new BigDecimal(d).setScale(prec, RoundingMode.HALF_EVEN).toPlainString();
        if (prec == 0 && alt) {
            s += ".";
        }
        return s;
    }

    /** The decimal exponent of d rounded to p significant digits. */
    private static int exponent(double d, int p) {
        BigDecimal b = new BigDecimal(d).round(new MathContext(p, RoundingMode.HALF_EVEN));
        return b.precision() - b.scale() - 1;
    }

    private static String expo(double d, int prec, boolean alt) {
        String digits;
        int e;
        if (d == 0) {
            StringBuilder z = new StringBuilder("0");
            for (int i = 0; i < prec; i++) z.append('0');
            digits = z.toString();
            e = 0;
        } else {
            BigDecimal b = new BigDecimal(d).round(new MathContext(prec + 1, RoundingMode.HALF_EVEN));
            e = b.precision() - b.scale() - 1;
            digits = b.unscaledValue().abs().toString();
            StringBuilder z = new StringBuilder(digits);
            while (z.length() < prec + 1) z.append('0');
            digits = z.substring(0, prec + 1);
        }
        StringBuilder s = new StringBuilder();
        s.append(digits.charAt(0));
        if (prec > 0 || alt) s.append('.');
        s.append(digits, 1, digits.length());
        s.append('e').append(e < 0 ? '-' : '+');
        int ae = Math.abs(e);
        if (ae < 10) s.append('0');
        s.append(ae);
        return s.toString();
    }

    private static String trimZeros(String s) {
        int e = s.indexOf('e');
        String mant = e >= 0 ? s.substring(0, e) : s;
        String rest = e >= 0 ? s.substring(e) : "";
        if (mant.indexOf('.') >= 0) {
            int k = mant.length();
            while (k > 0 && mant.charAt(k - 1) == '0') k--;
            if (k > 0 && mant.charAt(k - 1) == '.') k--;
            mant = mant.substring(0, k);
        }
        return mant + rest;
    }

    // ------------------------------------------------------------------------------- stdlib
    public static int malloc(long n) { return Mem.malloc(n); }
    public static int calloc(long n, long size) { return Mem.calloc(n, size); }
    public static int realloc(int p, long n) { return Mem.realloc(p, n); }
    public static void free(int p) { Mem.free(p); }

    public static int abs(int v) { return v < 0 ? -v : v; }
    public static int labs(int v) { return v < 0 ? -v : v; }
    public static long llabs(long v) { return v < 0 ? -v : v; }

    private static long parseLong(String s, int[] end, int base, boolean unsigned) {
        int i = 0, n = s.length();
        while (i < n && isspace(s.charAt(i)) != 0) i++;
        boolean neg = false;
        if (i < n && (s.charAt(i) == '+' || s.charAt(i) == '-')) neg = s.charAt(i++) == '-';
        if ((base == 0 || base == 16) && i + 1 < n && s.charAt(i) == '0' && (s.charAt(i + 1) | 32) == 'x') {
            i += 2;
            base = 16;
        } else if (base == 0 && i < n && s.charAt(i) == '0') {
            base = 8;
        } else if (base == 0) {
            base = 10;
        }
        long v = 0;
        int start = i;
        while (i < n) {
            int dgt = Character.digit(s.charAt(i), base);
            if (dgt < 0) break;
            v = v * base + dgt;
            i++;
        }
        end[0] = i == start ? 0 : i;
        return neg ? -v : v;
    }

    public static int atoi(Object s) { return (int) parseLong(text(s), new int[1], 10, false); }
    public static int atol(Object s) { return atoi(s); }
    public static long atoll(Object s) { return parseLong(text(s), new int[1], 10, false); }

    public static int strtol(Object s, int endp, int base) {
        int[] end = new int[1];
        long v = parseLong(text(s), end, base, false);
        if (endp != 0) Mem.setPtr(endp, addr(s) + end[0]);
        return v > Integer.MAX_VALUE ? Integer.MAX_VALUE : v < Integer.MIN_VALUE ? Integer.MIN_VALUE : (int) v;
    }

    public static int strtoul(Object s, int endp, int base) {
        int[] end = new int[1];
        long v = parseLong(text(s), end, base, true);
        if (endp != 0) Mem.setPtr(endp, addr(s) + end[0]);
        return (int) v;
    }

    public static long strtoll(Object s, int endp, int base) {
        int[] end = new int[1];
        long v = parseLong(text(s), end, base, false);
        if (endp != 0) Mem.setPtr(endp, addr(s) + end[0]);
        return v;
    }

    public static long strtoull(Object s, int endp, int base) {
        return strtoll(s, endp, base);
    }

    public static double atof(Object s) { return strtod(s, 0); }

    public static double strtod(Object s, int endp) {
        String t = text(s);
        int i = 0, n = t.length();
        while (i < n && isspace(t.charAt(i)) != 0) i++;
        int start = i;
        if (i < n && (t.charAt(i) == '+' || t.charAt(i) == '-')) i++;
        while (i < n && Character.isDigit(t.charAt(i))) i++;
        if (i < n && t.charAt(i) == '.') {
            i++;
            while (i < n && Character.isDigit(t.charAt(i))) i++;
        }
        if (i < n && (t.charAt(i) | 32) == 'e') {
            int j = i + 1;
            if (j < n && (t.charAt(j) == '+' || t.charAt(j) == '-')) j++;
            if (j < n && Character.isDigit(t.charAt(j))) {
                i = j;
                while (i < n && Character.isDigit(t.charAt(i))) i++;
            }
        }
        double v = 0;
        try {
            v = i > start ? Double.parseDouble(t.substring(start, i)) : 0;
        } catch (NumberFormatException e) {
            i = start;
        }
        if (endp != 0) Mem.setPtr(endp, addr(s) + (i > start ? i : 0));
        return v;
    }

    private static int holdrand = 1;

    /** MSVC's rand: the same sequence as the C runtime the original links. */
    public static int rand() {
        holdrand = holdrand * 214013 + 2531011;
        return (holdrand >>> 16) & 0x7fff;
    }

    public static void srand(int seed) {
        holdrand = seed;
    }

    public static int system(Object cmd) {
        return -1;
    }

    /** qsort: a stable merge sort of n elements of size bytes at base. */
    public static void qsort(int base, long n, long size, IntBinaryOperator cmp) {
        int k = (int) n, sz = (int) size;
        if (k < 2) return;
        int tmp = Mem.malloc((long) k * sz);
        sortRange(base, tmp, 0, k, sz, cmp);
        Mem.free(tmp);
    }

    private static void sortRange(int base, int tmp, int lo, int hi, int sz, IntBinaryOperator cmp) {
        if (hi - lo < 2) return;
        int mid = (lo + hi) >>> 1;
        sortRange(base, tmp, lo, mid, sz, cmp);
        sortRange(base, tmp, mid, hi, sz, cmp);
        int i = lo, j = mid, t = 0;
        while (i < mid || j < hi) {
            if (j >= hi || (i < mid && cmp.applyAsInt(base + i * sz, base + j * sz) <= 0)) {
                Mem.copy(tmp + t * sz, base + i++ * sz, sz);
            } else {
                Mem.copy(tmp + t * sz, base + j++ * sz, sz);
            }
            t++;
        }
        Mem.copy(base + lo * sz, tmp, (long) (hi - lo) * sz);
    }

    public static int bsearch(int key, int base, long n, long size, IntBinaryOperator cmp) {
        long lo = 0, hi = n - 1;
        while (lo <= hi) {
            long mid = (lo + hi) >>> 1;
            int p = (int) (base + mid * size);
            int c = cmp.applyAsInt(key, p);
            if (c == 0) return p;
            if (c < 0) hi = mid - 1;
            else lo = mid + 1;
        }
        return Mem.NULL;
    }

    public static int strdup(Object s) {
        String t = text(s);
        int a = Mem.malloc(t.length() + 1);
        Mem.setString(a, t, t.length() + 1);
        return a;
    }

    // ------------------------------------------------------------------------------- string.h
    public static long strlen(Object s) {
        if (s instanceof String || s instanceof byte[]) return text(s).length();
        int a = addr(s), e = a;
        while (Mem.getByte(e) != 0) e++;
        return e - a;
    }

    public static int strcpy(Object d, Object s) {
        String t = text(s);
        return put(d, 0, t, t.length() + 1);
    }

    public static int strncpy(Object d, Object s, long n) {
        String t = text(s);
        return put(d, 0, t.length() > n ? t.substring(0, (int) n) : t, (int) n);
    }

    public static int strcat(Object d, Object s) {
        String t = text(s);
        return put(d, (int) strlen(d), t, t.length() + 1);
    }

    public static int strncat(Object d, Object s, long n) {
        String t = text(s);
        if (t.length() > n) t = t.substring(0, (int) n);
        return put(d, (int) strlen(d), t, t.length() + 1);
    }

    public static int strcmp(Object a, Object b) {
        String x = text(a), y = text(b);
        int n = Math.min(x.length(), y.length());
        for (int i = 0; i < n; i++) {
            if (x.charAt(i) != y.charAt(i)) return x.charAt(i) < y.charAt(i) ? -1 : 1;
        }
        return Integer.compare(x.length(), y.length());
    }

    public static int strncmp(Object a, Object b, long k) {
        String x = text(a), y = text(b);
        if (x.length() > k) x = x.substring(0, (int) k);
        if (y.length() > k) y = y.substring(0, (int) k);
        return strcmp(x, y);
    }

    public static int stricmp(Object a, Object b) {
        return strcmp(text(a).toLowerCase(), text(b).toLowerCase());
    }

    public static int strchr(Object s, int c) {
        int a = addr(s);
        for (int i = a; ; i++) {
            int ch = Mem.getByte(i) & 0xFF;
            if (ch == (c & 0xFF)) return i;
            if (ch == 0) return Mem.NULL;
        }
    }

    public static int strrchr(Object s, int c) {
        int a = addr(s), found = Mem.NULL;
        for (int i = a; ; i++) {
            int ch = Mem.getByte(i) & 0xFF;
            if (ch == (c & 0xFF)) found = i;
            if (ch == 0) return found;
        }
    }

    public static int strstr(Object s, Object w) {
        int a = addr(s);
        int k = text(s).indexOf(text(w));
        return k < 0 ? Mem.NULL : a + k;
    }

    public static long strspn(Object s, Object set) {
        String t = text(s), z = text(set);
        int i = 0;
        while (i < t.length() && z.indexOf(t.charAt(i)) >= 0) i++;
        return i;
    }

    public static long strcspn(Object s, Object set) {
        String t = text(s), z = text(set);
        int i = 0;
        while (i < t.length() && z.indexOf(t.charAt(i)) < 0) i++;
        return i;
    }

    public static int strpbrk(Object s, Object set) {
        long k = strcspn(s, set);
        return k == strlen(s) ? Mem.NULL : addr(s) + (int) k;
    }

    public static int strerror(int e) {
        return Mem.str("Unknown error");
    }

    public static int memset(int p, int c, long n) { return Mem.fill(p, c, n); }
    public static int memcpy(int d, int s, long n) { return Mem.copy(d, s, n); }
    public static int memmove(int d, int s, long n) { return Mem.copy(d, s, n); }

    public static int memcmp(int a, int b, long n) {
        for (int i = 0; i < n; i++) {
            int x = Mem.getByte(a + i) & 0xFF, y = Mem.getByte(b + i) & 0xFF;
            if (x != y) return x < y ? -1 : 1;
        }
        return 0;
    }

    public static int memchr(int p, int c, long n) {
        for (int i = 0; i < n; i++) {
            if ((Mem.getByte(p + i) & 0xFF) == (c & 0xFF)) return p + i;
        }
        return Mem.NULL;
    }

    // ------------------------------------------------------------------------------- ctype.h (MSVC's "C" locale masks)
    private static int ctype(int c) {
        if (c < 0 || c > 127) return 0;
        if (c == ' ') return 0x48;
        if (c >= 9 && c <= 13) return 0x28;
        if (c < 32 || c == 127) return 0x20;
        if (c >= '0' && c <= '9') return 0x84;
        if (c >= 'A' && c <= 'F') return 0x181;
        if (c >= 'G' && c <= 'Z') return 0x101;
        if (c >= 'a' && c <= 'f') return 0x182;
        if (c >= 'g' && c <= 'z') return 0x102;
        return 0x10;
    }

    public static int isupper(int c) { return ctype(c) & 0x1; }
    public static int islower(int c) { return ctype(c) & 0x2; }
    public static int isdigit(int c) { return ctype(c) & 0x4; }
    public static int isspace(int c) { return ctype(c) & 0x8; }
    public static int ispunct(int c) { return ctype(c) & 0x10; }
    public static int iscntrl(int c) { return ctype(c) & 0x20; }
    public static int isxdigit(int c) { return ctype(c) & 0x80; }
    public static int isalpha(int c) { return ctype(c) & 0x103; }
    public static int isalnum(int c) { return ctype(c) & 0x107; }
    public static int isprint(int c) { return ctype(c) & 0x157; }
    public static int isgraph(int c) { return ctype(c) & 0x117; }
    public static int toupper(int c) { return c >= 'a' && c <= 'z' ? c - 32 : c; }
    public static int tolower(int c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }

    // ------------------------------------------------------------------------------- time.h
    public static long time(int p) {
        long t = System.currentTimeMillis() / 1000;
        if (p != 0) Mem.setLong(p, t);
        return t;
    }

    public static int clock() {
        return (int) (System.nanoTime() / 1000000);
    }

    // ------------------------------------------------------------------------------- math.h
    public static double fmod(double a, double b) { return a % b; }
    public static double round(double x) { return x < 0 ? -Math.floor(-x + 0.5) : Math.floor(x + 0.5); }
    public static double trunc(double x) { return x < 0 ? Math.ceil(x) : Math.floor(x); }
    public static double fmin(double a, double b) { return Double.isNaN(a) ? b : Double.isNaN(b) ? a : Math.min(a, b); }
    public static double fmax(double a, double b) { return Double.isNaN(a) ? b : Double.isNaN(b) ? a : Math.max(a, b); }
    public static double ldexp(double x, int e) { return x * Math.pow(2, e); }
    public static double log2(double x) { return Math.log(x) / Math.log(2); }
    public static double exp2(double x) { return Math.pow(2, x); }
    public static float sqrtf(float x) { return (float) Math.sqrt(x); }
    public static float sinf(float x) { return (float) Math.sin(x); }
    public static float cosf(float x) { return (float) Math.cos(x); }
    public static float powf(float a, float b) { return (float) Math.pow(a, b); }
    public static float fabsf(float x) { return Math.abs(x); }
    public static float floorf(float x) { return (float) Math.floor(x); }
    public static float ceilf(float x) { return (float) Math.ceil(x); }
    public static float fmodf(float a, float b) { return a % b; }
    public static float expf(float x) { return (float) Math.exp(x); }
    public static float logf(float x) { return (float) Math.log(x); }
    public static float atan2f(float a, float b) { return (float) Math.atan2(a, b); }
    public static float roundf(float x) { return (float) round(x); }

    public static double modf(double x, int ip) {
        double t = trunc(x);
        Mem.setDouble(ip, t);
        return x - t;
    }

    public static double frexp(double x, int ep) {
        if (x == 0 || Double.isNaN(x) || Double.isInfinite(x)) {
            Mem.setInt(ep, 0);
            return x;
        }
        int e = Math.getExponent(x) + 1;
        double m = x / Math.pow(2, e);
        if (Math.abs(m) < 0.5) {
            m *= 2;
            e--;
        }
        Mem.setInt(ep, e);
        return m;
    }
}
