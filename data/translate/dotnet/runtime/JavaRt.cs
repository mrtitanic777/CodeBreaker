// JavaRt -- the part of Java's library a translation from Java into C# needs where .NET's own does
// something else: how Java turns values into text (1.0 is "1.0", true is "true", null is "null"), its
// String.hashCode, Math.round and Math.abs, the List and Map calls whose results differ (set, remove,
// put, get on a missing key), a stable sort, and java.util.Random's exact sequence.
// Written by CodeBreaker's C# back end (codebreaker/translate/backends/csharp_java.py) beside the
// translated program when the program uses it.
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

public static class JavaRt
{
    // ---- String.valueOf: Java's text for each kind of value
    public static string Str(string s) => s ?? "null";
    public static string Str(bool b) => b ? "true" : "false";
    public static string Str(char c) => c.ToString();
    public static string Str(int i) => i.ToString(CultureInfo.InvariantCulture);
    public static string Str(long l) => l.ToString(CultureInfo.InvariantCulture);
    public static string Str(double d) => Num(d, d.ToString("R", CultureInfo.InvariantCulture));
    public static string Str(float f) => Num(f, f.ToString("R", CultureInfo.InvariantCulture));

    public static string Str(object o)
    {
        switch (o)
        {
            case null: return "null";
            case string s: return s;
            case double d: return Str(d);
            case float f: return Str(f);
            case bool b: return Str(b);
            case System.Collections.IDictionary m: return MapStr(m);
            case System.Collections.IList l: return ListStr(l);
            default: return o.ToString() ?? "null";
        }
    }

    static string ListStr(System.Collections.IList l)
    {
        var sb = new StringBuilder("[");
        for (int i = 0; i < l.Count; i++)
        {
            if (i > 0) sb.Append(", ");
            sb.Append(ReferenceEquals(l[i], l) ? "(this Collection)" : Str(l[i]));
        }
        return sb.Append(']').ToString();
    }

    static string MapStr(System.Collections.IDictionary m)
    {
        var sb = new StringBuilder("{");
        bool first = true;
        foreach (System.Collections.DictionaryEntry e in m)
        {
            if (!first) sb.Append(", ");
            first = false;
            sb.Append(Str(e.Key)).Append('=').Append(Str(e.Value));
        }
        return sb.Append('}').ToString();
    }

    // Double.toString / Float.toString: the shortest digits that read back as the value (as .NET's "R"
    // gives them), laid out as Java lays them out -- plain from 10^-3 to 10^7, else d.dddE[-]n.
    static string Num(double v, string r)
    {
        if (double.IsNaN(v)) return "NaN";
        if (double.IsPositiveInfinity(v)) return "Infinity";
        if (double.IsNegativeInfinity(v)) return "-Infinity";
        if (v == 0) return (1 / v < 0) ? "-0.0" : "0.0";
        bool neg = r.StartsWith("-");
        if (neg) r = r.Substring(1);
        int e = 0;
        int ei = r.IndexOfAny(new[] { 'E', 'e' });
        if (ei >= 0)
        {
            e = int.Parse(r.Substring(ei + 1), CultureInfo.InvariantCulture);
            r = r.Substring(0, ei);
        }
        int dot = r.IndexOf('.');
        string ip = dot >= 0 ? r.Substring(0, dot) : r, fp = dot >= 0 ? r.Substring(dot + 1) : "";
        string digits = (ip + fp).TrimStart('0');
        int lead = (ip + fp).Length - digits.Length;
        int exp = ip.Length - 1 - lead + e;            // the power of ten of the first digit
        digits = digits.TrimEnd('0');
        if (digits.Length == 0) digits = "0";
        var sb = new StringBuilder(neg ? "-" : "");
        if (exp >= -3 && exp < 7)
        {
            if (exp >= 0)
            {
                string whole = digits.Length > exp + 1 ? digits.Substring(0, exp + 1) : digits.PadRight(exp + 1, '0');
                string frac = digits.Length > exp + 1 ? digits.Substring(exp + 1) : "0";
                sb.Append(whole).Append('.').Append(frac);
            }
            else
            {
                sb.Append("0.").Append('0', -exp - 1).Append(digits);
            }
        }
        else
        {
            sb.Append(digits[0]).Append('.').Append(digits.Length > 1 ? digits.Substring(1) : "0").Append('E').Append(exp);
        }
        return sb.ToString();
    }

    // ---- java.lang
    public static int HashCode(string s)
    {
        int h = 0;
        foreach (char c in s) h = unchecked(31 * h + c);
        return h;
    }

    public static int Abs(int x) => x < 0 ? unchecked(-x) : x;
    public static long Abs(long x) => x < 0 ? unchecked(-x) : x;
    public static double Abs(double x) => Math.Abs(x);
    public static float Abs(float x) => Math.Abs(x);

    public static long Round(double d)
    {
        if (double.IsNaN(d)) return 0;
        double f = Math.Floor(d);
        long r = (long)f;                              // (saturating, as Java's)
        return d - f >= 0.5 && r != long.MaxValue ? r + 1 : r;
    }

    public static int Round(float x)
    {
        if (float.IsNaN(x)) return 0;
        double f = Math.Floor((double)x);
        int r = (int)f;
        return x - f >= 0.5 && r != int.MaxValue ? r + 1 : r;
    }

    public static double Signum(double d) => d > 0 ? 1.0 : d < 0 ? -1.0 : d;
    public static float Signum(float d) => d > 0 ? 1.0f : d < 0 ? -1.0f : d;
    public static int FloorDiv(int a, int b) { int q = a / b; if ((a % b != 0) && ((a ^ b) < 0)) q--; return q; }
    public static int FloorMod(int a, int b) { int m = a % b; if (m != 0 && ((m ^ b) < 0)) m += b; return m; }
    public static long FloorDiv(long a, long b) { long q = a / b; if ((a % b != 0) && ((a ^ b) < 0)) q--; return q; }
    public static long FloorMod(long a, long b) { long m = a % b; if (m != 0 && ((m ^ b) < 0)) m += b; return m; }
    public static double ToRadians(double a) => a * 0.017453292519943295;
    public static double ToDegrees(double r) => r * 57.29577951308232;
    public static int CompareDouble(double a, double b)
    {
        if (a < b) return -1;
        if (a > b) return 1;
        long x = BitConverter.DoubleToInt64Bits(double.IsNaN(a) ? double.NaN : a);
        long y = BitConverter.DoubleToInt64Bits(double.IsNaN(b) ? double.NaN : b);
        return x == y ? 0 : x < y ? -1 : 1;
    }

    public static int ParseInt(string s)
    {
        if (s == null) throw new FormatException("null");
        if (s.Length == 0 || char.IsWhiteSpace(s[0]) || char.IsWhiteSpace(s[s.Length - 1]))
            throw new FormatException("For input string: \"" + s + "\"");
        return int.Parse(s, NumberStyles.AllowLeadingSign, CultureInfo.InvariantCulture);
    }

    public static double ParseDouble(string s) => double.Parse(s.Trim(), NumberStyles.Float, CultureInfo.InvariantCulture);

    public static int CompareTo(string a, string b)
    {
        int n = Math.Min(a.Length, b.Length);
        for (int i = 0; i < n; i++)
            if (a[i] != b[i]) return a[i] - b[i];
        return a.Length - b.Length;
    }

    public static string Trim(string s)
    {
        int i = 0, j = s.Length;
        while (i < j && s[i] <= ' ') i++;
        while (j > i && s[j - 1] <= ' ') j--;
        return s.Substring(i, j - i);
    }

    public static string Substring(string s, int begin, int end)
    {
        if (begin < 0 || end > s.Length || begin > end)
            throw new ArgumentOutOfRangeException("begin " + begin + ", end " + end + ", length " + s.Length);
        return s.Substring(begin, end - begin);
    }

    public static string Repeat(string s, int n)
    {
        var sb = new StringBuilder(s.Length * Math.Max(n, 0));
        for (int i = 0; i < n; i++) sb.Append(s);
        return sb.ToString();
    }

    public static string[] Split(string s, string regex)
    {
        string[] parts = System.Text.RegularExpressions.Regex.Split(s, regex);
        int n = parts.Length;
        while (n > 0 && parts[n - 1].Length == 0) n--;          // Java drops the trailing empty strings
        if (n == parts.Length) return parts;
        var r = new string[n];
        Array.Copy(parts, r, n);
        return r;
    }

    public static StringBuilder Reverse(StringBuilder sb)
    {
        char[] a = sb.ToString().ToCharArray();
        Array.Reverse(a);
        sb.Clear().Append(a);
        return sb;
    }

    public static StringBuilder Delete(StringBuilder sb, int start, int end)
    {
        if (end > sb.Length) end = sb.Length;
        return sb.Remove(start, end - start);
    }

    // ---- java.util
    public static bool Add<T>(ICollection<T> c, T x) { c.Add(x); return true; }
    public static T Set<T>(IList<T> l, int i, T x) { T old = l[i]; l[i] = x; return old; }
    public static T RemoveAt<T>(IList<T> l, int i) { T old = l[i]; l.RemoveAt(i); return old; }

    public static V Get<K, V>(IDictionary<K, V> m, K k) => m.TryGetValue(k, out V v) ? v : default!;
    public static V GetOrDefault<K, V>(IDictionary<K, V> m, K k, V d) => m.TryGetValue(k, out V v) ? v : d;

    public static V Put<K, V>(IDictionary<K, V> m, K k, V v)
    {
        m.TryGetValue(k, out V old);
        m[k] = v;
        return old!;
    }

    public static V Remove<K, V>(IDictionary<K, V> m, K k)
    {
        if (!m.TryGetValue(k, out V old)) return default!;
        m.Remove(k);
        return old;
    }

    // Collections.sort and List.sort are stable (a merge sort); .NET's List.Sort is not.
    public static void Sort<T>(List<T> l, Comparison<T> cmp)
    {
        var tmp = new T[l.Count];
        T[] a = l.ToArray();
        MergeSort(a, tmp, 0, a.Length, cmp);
        for (int i = 0; i < a.Length; i++) l[i] = a[i];
    }

    public static void Sort<T>(List<T> l) where T : IComparable<T> => Sort(l, (x, y) => x.CompareTo(y));

    static void MergeSort<T>(T[] a, T[] tmp, int lo, int hi, Comparison<T> cmp)
    {
        if (hi - lo < 2) return;
        int mid = (lo + hi) >> 1;
        MergeSort(a, tmp, lo, mid, cmp);
        MergeSort(a, tmp, mid, hi, cmp);
        int i = lo, j = mid, k = lo;
        while (i < mid && j < hi) tmp[k++] = cmp(a[j], a[i]) < 0 ? a[j++] : a[i++];
        while (i < mid) tmp[k++] = a[i++];
        while (j < hi) tmp[k++] = a[j++];
        Array.Copy(tmp, lo, a, lo, hi - lo);
    }

    public static string ArrayStr<T>(T[] a)
    {
        if (a == null) return "null";
        var sb = new StringBuilder("[");
        for (int i = 0; i < a.Length; i++)
        {
            if (i > 0) sb.Append(", ");
            sb.Append(Str((object)a[i]!));
        }
        return sb.Append(']').ToString();
    }

    public static long CurrentTimeMillis() => DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
    public static long NanoTime() => System.Diagnostics.Stopwatch.GetTimestamp() * (1000000000L / System.Diagnostics.Stopwatch.Frequency);
}

// java.util.Random: the same 48-bit linear congruential generator, so a seed gives Java's sequence.
public class JavaRandom
{
    long seed;
    double nextNextGaussian;
    bool haveNextNextGaussian;
    static long seedUniquifier = 8682522807148012L;

    public JavaRandom() : this(unchecked((seedUniquifier *= 1181783497276652981L) ^ System.Diagnostics.Stopwatch.GetTimestamp())) { }

    public JavaRandom(long seed) { SetSeed(seed); }

    public void SetSeed(long s)
    {
        seed = (s ^ 0x5DEECE66DL) & ((1L << 48) - 1);
        haveNextNextGaussian = false;
    }

    protected int Next(int bits)
    {
        seed = unchecked(seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
        return (int)((ulong)seed >> (48 - bits));
    }

    public int NextInt() => Next(32);

    public int NextInt(int bound)
    {
        if (bound <= 0) throw new ArgumentException("bound must be positive");
        if ((bound & -bound) == bound) return (int)((bound * (long)Next(31)) >> 31);
        int bits, val;
        do
        {
            bits = Next(31);
            val = bits % bound;
        } while (bits - val + (bound - 1) < 0);
        return val;
    }

    public long NextLong() => unchecked(((long)Next(32) << 32) + Next(32));
    public bool NextBoolean() => Next(1) != 0;
    public float NextFloat() => Next(24) / ((float)(1 << 24));
    public double NextDouble() => (((long)Next(26) << 27) + Next(27)) * (1.0 / (1L << 53));

    public double NextGaussian()
    {
        if (haveNextNextGaussian)
        {
            haveNextNextGaussian = false;
            return nextNextGaussian;
        }
        double v1, v2, s;
        do
        {
            v1 = 2 * NextDouble() - 1;
            v2 = 2 * NextDouble() - 1;
            s = v1 * v1 + v2 * v2;
        } while (s >= 1 || s == 0);
        double multiplier = Math.Sqrt(-2 * Math.Log(s) / s);   // (Java's StrictMath.sqrt and log)
        nextNextGaussian = v2 * multiplier;
        haveNextNextGaussian = true;
        return v1 * multiplier;
    }
}
