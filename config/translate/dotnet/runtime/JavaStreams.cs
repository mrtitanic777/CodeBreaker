// JavaStreams -- java.util.stream, Optional and Comparator's combinators for a translation from Java into C#, written beside the program
// when it uses them. The methods keep Java's names and Java's behaviour: a stream is lazy and pulls one element at a time, so its stages run
// in Java's order and an endless source with a limit ends; a stage is used once; sorting is stable; strings compare as Java compares them.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public static class JLib
{
    public static JStream<T> stream<T>(IEnumerable<T> items) => new JStream<T>(items);

    // Comparable.compareTo, as Java orders each kind of value.
    public static int Natural<T>(T a, T b)
    {
        if (a is null || b is null) throw new NullReferenceException();
        if (a is string x && b is string y) return JavaRt.CompareTo(x, y);
        if (a is double d && b is double e) return JavaRt.CompareDouble(d, e);
        if (a is float f && b is float g) return JavaRt.CompareDouble(f, g);
        return Comparer<T>.Default.Compare(a, b);
    }

    public static bool Equal<T, U>(T a, U b) => a is null ? b is null : a.Equals(b);

    // hashCode as Java computes it for each kind of value.
    public static int Hash<T>(T v)
    {
        switch (v)
        {
            case null: return 0;
            case string s: return JavaRt.HashCode(s);
            case bool b: return b ? 1231 : 1237;
            case char c: return c;
            case sbyte i8: return i8;
            case short i16: return i16;
            case int i32: return i32;
            case long i64: return HashLong(i64);
            case double d: return HashLong(double.IsNaN(d) ? 0x7ff8000000000000L : BitConverter.DoubleToInt64Bits(d));
            case float f: return float.IsNaN(f) ? 0x7fc00000 : BitConverter.SingleToInt32Bits(f);
            default: return v.GetHashCode();
        }
    }

    static int HashLong(long v) => unchecked((int)(v ^ (long)((ulong)v >> 32)));

    internal static long NotNegative(long n)
    {
        if (n < 0) throw new ArgumentException(n.ToString());
        return n;
    }

    internal static List<T> StableSorted<T>(IEnumerable<T> items, Comparison<T> by)
    {
        var list = new List<T>(items);
        JavaRt.Sort(list, by);
        return list;
    }
}

public sealed class JStream<T>
{
    readonly IEnumerable<T> source;
    bool used;

    public JStream(IEnumerable<T> source) { this.source = source; }

    public static JStream<T> of(params T[] items) => new JStream<T>(items);

    IEnumerable<T> Use()
    {
        if (used) throw new InvalidOperationException("stream has already been operated upon or closed");
        used = true;
        return source;
    }

    public JStream<T> filter(Func<T, bool> p) { var s = Use(); return new JStream<T>(Filtered(s, p)); }
    static IEnumerable<T> Filtered(IEnumerable<T> s, Func<T, bool> p) { foreach (T x in s) if (p(x)) yield return x; }

    public JStream<R> map<R>(Func<T, R> f) { var s = Use(); return new JStream<R>(Mapped(s, f)); }
    static IEnumerable<R> Mapped<R>(IEnumerable<T> s, Func<T, R> f) { foreach (T x in s) yield return f(x); }

    public JIntStream mapToInt(Func<T, int> f) { var s = Use(); return new JIntStream(Mapped(s, f)); }

    public JStream<R> flatMap<R>(Func<T, JStream<R>> f) { var s = Use(); return new JStream<R>(Flat(s, f)); }
    static IEnumerable<R> Flat<R>(IEnumerable<T> s, Func<T, JStream<R>> f)
    {
        foreach (T x in s)
        {
            JStream<R> inner = f(x);
            if (inner == null) continue;
            foreach (R y in inner.Drain()) yield return y;
        }
    }
    internal IEnumerable<T> Drain() => Use();

    public JStream<T> peek(Action<T> f) { var s = Use(); return new JStream<T>(Peeked(s, f)); }
    static IEnumerable<T> Peeked(IEnumerable<T> s, Action<T> f) { foreach (T x in s) { f(x); yield return x; } }

    public JStream<T> limit(long n) { JLib.NotNegative(n); var s = Use(); return new JStream<T>(Limited(s, n)); }
    static IEnumerable<T> Limited(IEnumerable<T> s, long n)
    {
        if (n == 0) yield break;
        long i = 0;
        foreach (T x in s)
        {
            yield return x;
            if (++i >= n) yield break;
        }
    }

    public JStream<T> skip(long n) { JLib.NotNegative(n); var s = Use(); return new JStream<T>(Skipped(s, n)); }
    static IEnumerable<T> Skipped(IEnumerable<T> s, long n) { long i = 0; foreach (T x in s) if (i++ >= n) yield return x; }

    public JStream<T> sorted() => sorted(JLib.Natural);
    public JStream<T> sorted(Comparison<T> by) { var s = Use(); return new JStream<T>(Sorted(s, by)); }
    static IEnumerable<T> Sorted(IEnumerable<T> s, Comparison<T> by) { foreach (T x in JLib.StableSorted(s, by)) yield return x; }

    public JStream<T> distinct() { var s = Use(); return new JStream<T>(Distinct(s)); }
    static IEnumerable<T> Distinct(IEnumerable<T> s) { var seen = new HashSet<T>(); foreach (T x in s) if (seen.Add(x)) yield return x; }

    public long count() { long n = 0; foreach (T _ in Use()) n++; return n; }
    public void forEach(Action<T> f) { foreach (T x in Use()) f(x); }
    public List<T> toList() => new List<T>(Use());
    public R collect<R>(JCollector<T, R> how) => how.Finish(Use());
    public T reduce(T identity, Func<T, T, T> f) { T acc = identity; foreach (T x in Use()) acc = f(acc, x); return acc; }

    public JOptional<T> reduce(Func<T, T, T> f)
    {
        bool any = false;
        T acc = default!;
        foreach (T x in Use())
        {
            acc = any ? f(acc, x) : x;
            any = true;
        }
        return any ? JOptional<T>.of(acc) : JOptional<T>.empty();
    }

    public bool anyMatch(Func<T, bool> p) { foreach (T x in Use()) if (p(x)) return true; return false; }
    public bool allMatch(Func<T, bool> p) { foreach (T x in Use()) if (!p(x)) return false; return true; }
    public bool noneMatch(Func<T, bool> p) => !anyMatch(p);
    public JOptional<T> findFirst() { foreach (T x in Use()) return JOptional<T>.of(x); return JOptional<T>.empty(); }
    public JOptional<T> findAny() => findFirst();
    public JOptional<T> max(Comparison<T> by) => reduce((a, b) => by(a, b) >= 0 ? a : b);   // (a tie keeps the earlier, as BinaryOperator.maxBy)
    public JOptional<T> min(Comparison<T> by) => reduce((a, b) => by(a, b) <= 0 ? a : b);
}

public sealed class JIntStream
{
    readonly JStream<int> s;

    public JIntStream(IEnumerable<int> source) { s = new JStream<int>(source); }
    JIntStream(JStream<int> stream) { s = stream; }

    public static JIntStream of(params int[] items) => new JIntStream(items);
    public static JIntStream range(int a, int b) => new JIntStream(Counted(a, (long)b));
    public static JIntStream rangeClosed(int a, int b) => new JIntStream(Counted(a, (long)b + 1));
    static IEnumerable<int> Counted(int a, long end) { for (long i = a; i < end; i++) yield return (int)i; }
    public static JIntStream iterate(int seed, Func<int, int> f) => new JIntStream(Iterated(seed, f));
    static IEnumerable<int> Iterated(int seed, Func<int, int> f) { for (int v = seed; ; v = f(v)) yield return v; }

    public JIntStream filter(Func<int, bool> p) => new JIntStream(s.filter(p));
    public JIntStream map(Func<int, int> f) => new JIntStream(s.map<int>(f));
    public JStream<R> mapToObj<R>(Func<int, R> f) => s.map(f);
    public JStream<int> boxed() => s;
    public JIntStream peek(Action<int> f) => new JIntStream(s.peek(f));
    public JIntStream limit(long n) => new JIntStream(s.limit(n));
    public JIntStream skip(long n) => new JIntStream(s.skip(n));
    public JIntStream sorted() => new JIntStream(s.sorted());
    public JIntStream distinct() => new JIntStream(s.distinct());
    public int sum() { int t = 0; foreach (int x in s.Drain()) t = unchecked(t + x); return t; }
    public long count() => s.count();
    public void forEach(Action<int> f) => s.forEach(f);
    public int reduce(int identity, Func<int, int, int> f) => s.reduce(identity, f);
    public JOptionalInt reduce(Func<int, int, int> f) => JOptionalInt.From(s.reduce(f));
    public bool anyMatch(Func<int, bool> p) => s.anyMatch(p);
    public bool allMatch(Func<int, bool> p) => s.allMatch(p);
    public bool noneMatch(Func<int, bool> p) => s.noneMatch(p);
    public int[] toArray() => s.Drain().ToArray();
    public JOptionalInt findFirst() => JOptionalInt.From(s.findFirst());
    public JOptionalInt findAny() => findFirst();
    public JOptionalInt min() => JOptionalInt.From(s.reduce((a, b) => b < a ? b : a));
    public JOptionalInt max() => JOptionalInt.From(s.reduce((a, b) => b > a ? b : a));

    public JOptionalDouble average()
    {
        long n = 0, total = 0;
        foreach (int x in s.Drain()) { n++; total += x; }
        return n == 0 ? JOptionalDouble.empty() : JOptionalDouble.of((double)total / n);
    }
}

public sealed class JOptional<T>
{
    readonly bool has;
    readonly T value;

    JOptional(bool has, T value) { this.has = has; this.value = value; }

    public static JOptional<T> of(T v)
    {
        if (v is null) throw new NullReferenceException();
        return new JOptional<T>(true, v);
    }
    public static JOptional<T> ofNullable(T v) => v is null ? empty() : of(v);
    public static JOptional<T> empty() => new JOptional<T>(false, default!);

    public bool isPresent() => has;
    public bool isEmpty() => !has;
    public T get() => has ? value : throw new InvalidOperationException("No value present");
    public T orElseThrow() => get();
    public T orElse(T other) => has ? value : other;
    public T orElseGet(Func<T> f) => has ? value : f();
    public JOptional<R> map<R>(Func<T, R> f) => has ? JOptional<R>.ofNullable(f(value)) : JOptional<R>.empty();
    public JOptional<T> filter(Func<T, bool> p) => !has || p(value) ? this : empty();
    public void ifPresent(Action<T> f) { if (has) f(value); }

    public override string ToString() => has ? "Optional[" + JavaRt.Str((object)value!) + "]" : "Optional.empty";
    public override bool Equals(object o) => o is JOptional<T> x && has == x.has && (!has || JLib.Equal(value, x.value));
    public override int GetHashCode() => has ? JLib.Hash(value) : 0;
}

public sealed class JOptionalInt
{
    readonly bool has;
    readonly int value;

    JOptionalInt(bool has, int value) { this.has = has; this.value = value; }

    internal static JOptionalInt From(JOptional<int> o) => o.isPresent() ? of(o.get()) : empty();
    public static JOptionalInt of(int v) => new JOptionalInt(true, v);
    public static JOptionalInt empty() => new JOptionalInt(false, 0);

    public bool isPresent() => has;
    public bool isEmpty() => !has;
    public int getAsInt() => has ? value : throw new InvalidOperationException("No value present");
    public int orElseThrow() => getAsInt();
    public int orElse(int other) => has ? value : other;

    public override string ToString() => has ? "OptionalInt[" + JavaRt.Str(value) + "]" : "OptionalInt.empty";
    public override bool Equals(object o) => o is JOptionalInt x && has == x.has && value == x.value;
    public override int GetHashCode() => has ? value : 0;
}

public sealed class JOptionalDouble
{
    readonly bool has;
    readonly double value;

    JOptionalDouble(bool has, double value) { this.has = has; this.value = value; }

    public static JOptionalDouble of(double v) => new JOptionalDouble(true, v);
    public static JOptionalDouble empty() => new JOptionalDouble(false, 0);

    public bool isPresent() => has;
    public bool isEmpty() => !has;
    public double getAsDouble() => has ? value : throw new InvalidOperationException("No value present");
    public double orElseThrow() => getAsDouble();
    public double orElse(double other) => has ? value : other;

    public override string ToString() => has ? "OptionalDouble[" + JavaRt.Str(value) + "]" : "OptionalDouble.empty";
    public override bool Equals(object o) => o is JOptionalDouble x && has == x.has && (!has || JavaRt.CompareDouble(value, x.value) == 0);
    public override int GetHashCode() => has ? JLib.Hash(value) : 0;
}

// What collect does with the elements.
public sealed class JCollector<T, R>
{
    internal readonly Func<IEnumerable<T>, R> Finish;
    public JCollector(Func<IEnumerable<T>, R> finish) { Finish = finish; }
}

public static class JCollectors<T>
{
    public static JCollector<T, List<T>> toList() => new JCollector<T, List<T>>(s => new List<T>(s));
    public static JCollector<T, ISet<T>> toSet() => new JCollector<T, ISet<T>>(s => new HashSet<T>(s));
    public static JCollector<T, string> joining() => joining("", "", "");
    public static JCollector<T, string> joining(string separator) => joining(separator, "", "");

    public static JCollector<T, string> joining(string separator, string prefix, string suffix) => new JCollector<T, string>(s =>
    {
        var b = new StringBuilder(prefix);
        bool first = true;
        foreach (T x in s)
        {
            if (!first) b.Append(separator);
            first = false;
            b.Append(JavaRt.Str((object)x!));
        }
        return b.Append(suffix).ToString();
    });

    public static JCollector<T, long> counting() => new JCollector<T, long>(s => { long n = 0; foreach (T _ in s) n++; return n; });
    public static JCollector<T, int> summingInt(Func<T, int> f) => new JCollector<T, int>(s => { int t = 0; foreach (T x in s) t = unchecked(t + f(x)); return t; });

    static Dictionary<K, List<T>> Groups<K>(IEnumerable<T> s, Func<T, K> key)
    {
        var groups = new Dictionary<K, List<T>>();
        foreach (T x in s)
        {
            K k = key(x);
            if (k is null) throw new NullReferenceException("element cannot be mapped to a null key");
            if (!groups.TryGetValue(k, out List<T> l)) groups[k] = l = new List<T>();
            l.Add(x);
        }
        return groups;
    }

    public static JCollector<T, IDictionary<K, List<T>>> groupingBy<K>(Func<T, K> key) =>
        new JCollector<T, IDictionary<K, List<T>>>(s => Groups(s, key));

    public static JCollector<T, IDictionary<K, D>> groupingBy<K, D>(Func<T, K> key, JCollector<T, D> down) => new JCollector<T, IDictionary<K, D>>(s =>
    {
        var result = new Dictionary<K, D>();
        foreach (KeyValuePair<K, List<T>> g in Groups(s, key)) result[g.Key] = down.Finish(g.Value);
        return result;
    });
}

// Comparator's factories and default methods, on the delegate a Comparator is in C#.
public static class JCompare<T>
{
    public static Comparison<T> naturalOrder() => JLib.Natural;
    public static Comparison<T> reverseOrder() => (a, b) => JLib.Natural(b, a);
    public static Comparison<T> comparing<U>(Func<T, U> key) => (a, b) => { U x = key(a); return JLib.Natural(x, key(b)); };
    public static Comparison<T> comparing<U>(Func<T, U> key, Comparison<U> by) => (a, b) => { U x = key(a); return by(x, key(b)); };
    public static Comparison<T> comparingInt(Func<T, int> key) => (a, b) => { int x = key(a); return x.CompareTo(key(b)); };
    public static Comparison<T> reversed(Comparison<T> c) => (a, b) => c(b, a);
    public static Comparison<T> thenComparing(Comparison<T> c, Comparison<T> next) => (a, b) => { int r = c(a, b); return r != 0 ? r : next(a, b); };
    public static Comparison<T> thenComparing<U>(Comparison<T> c, Func<T, U> key) => thenComparing(c, comparing(key));
    public static Comparison<T> thenComparing<U>(Comparison<T> c, Func<T, U> key, Comparison<U> by) => thenComparing(c, comparing(key, by));
    public static Comparison<T> thenComparingInt(Comparison<T> c, Func<T, int> key) => thenComparing(c, comparingInt(key));
}
