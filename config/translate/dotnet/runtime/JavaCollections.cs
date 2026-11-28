// JavaCollections -- java.util.HashMap, LinkedHashMap, HashSet and LinkedHashSet for a translation from Java into C#, written beside the
// program when it uses them. The table is Java's: the same hash of each key, the same spreading of it, the same growth and the same order
// inside a bin, so a loop over one visits what Java visits in the order Java visits it. The one exception: Java turns a bin of more than eight
// keys in a table of 64 or more into a tree, whose order can depend on object addresses. Such a map still finds, adds and removes, but walking
// it throws NotSupportedException rather than show an order that may not be Java's.
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

public static partial class JLib
{
    // Comparable.compareTo, as Java orders each kind of value.
    public static int Natural<T>(T a, T b)
    {
        if (a is null || b is null) throw new NullReferenceException();
        if (a is string x && b is string y) return JavaRt.CompareTo(x, y);
        if (a is double d && b is double e) return JavaRt.CompareDouble(d, e);
        if (a is float f && b is float g) return JavaRt.CompareDouble(f, g);
        return Comparer<T>.Default.Compare(a, b);
    }

    public static IComparer<T> Order<T>() => Comparer<T>.Create(Natural);

    public static bool Equal<T, U>(T a, U b)
    {
        if (a is null) return b is null;
        if (a is IList x && b is IList y && !(a is Array) && !(b is Array))
        {
            if (x.Count != y.Count) return false;
            for (int i = 0; i < x.Count; i++)
                if (!Equal(x[i], y[i])) return false;
            return true;
        }
        return a.Equals(b);
    }

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
            case Array _: return v.GetHashCode();
            case IList l:
                int h = 1;
                foreach (object x in l) h = unchecked(31 * h + Hash(x));
                return h;
            default: return v.GetHashCode();
        }
    }

    static int HashLong(long v) => unchecked((int)(v ^ (long)((ulong)v >> 32)));

    public static T NonNull<T>(T v) where T : class => v ?? throw new NullReferenceException();

    public static string CauseText(Exception cause) => cause == null ? null : JavaRt.Str(cause);

    // Java's Collection.removeAll: every element the other holds goes, and whether anything did.
    public static bool RemoveAll<T>(ICollection<T> from, IEnumerable other)
    {
        var gone = new HashSet<object>();
        bool hasNull = false;
        foreach (object x in other)
        {
            if (x == null) hasNull = true;
            else gone.Add(x);
        }
        var kept = new List<T>();
        bool changed = false;
        foreach (T x in from)
        {
            if (x == null ? hasNull : gone.Contains(x)) changed = true;
            else kept.Add(x);
        }
        if (!changed) return false;
        from.Clear();
        foreach (T x in kept) from.Add(x);
        return true;
    }

    // Java's Set.addAll: whether the set grew.
    public static bool AddAll<T>(ICollection<T> into, IEnumerable<T> from)
    {
        int before = into.Count;
        foreach (T x in new List<T>(from)) into.Add(x);
        return into.Count != before;
    }

    public static T First<T>(SortedSet<T> set) => set.Count != 0 ? set.Min : throw new InvalidOperationException();
    public static T Last<T>(SortedSet<T> set) => set.Count != 0 ? set.Max : throw new InvalidOperationException();

    // Arrays.fill(a, from, to, value)
    public static void FillRange<T, V>(T[] a, int from, int to, V value)
    {
        if (from > to) throw new ArgumentException("fromIndex(" + from + ") > toIndex(" + to + ")");
        if (from < 0 || to > a.Length) throw new IndexOutOfRangeException();
        T item = (T)Convert.ChangeType(value, typeof(T));
        for (int i = from; i < to; i++) a[i] = item;
    }

    public static float ParseFloat(string text) => (float)double.Parse(text.Trim(), System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture);

    // Throwable.toString: the class's name, then the message if it has one
    public static string ThrowableText(Exception e)
    {
        string message = Message(e);
        string name = e.GetType().FullName;
        return message == null ? name : name + ": " + message;
    }

    // Arrays.sort(a, comparator): equal elements keep their order
    public static void SortWith<T>(T[] a, Comparison<T> by)
    {
        var order = new int[a.Length];
        for (int i = 0; i < order.Length; i++) order[i] = i;
        var held = (T[])a.Clone();
        Array.Sort(order, (x, y) =>
        {
            int c = by(held[x], held[y]);
            return c != 0 ? c : x.CompareTo(y);
        });
        for (int i = 0; i < a.Length; i++) a[i] = held[order[i]];
    }

    // Integer.toString(v, radix): lower-case digits, a minus sign
    public static string ToRadix(int value, int radix)
    {
        if (radix < 2 || radix > 36) radix = 10;
        if (value == 0) return "0";
        var digits = new StringBuilder();
        long left = Math.Abs((long)value);
        while (left > 0)
        {
            digits.Insert(0, "0123456789abcdefghijklmnopqrstuvwxyz"[(int)(left % radix)]);
            left /= radix;
        }
        if (value < 0) digits.Insert(0, '-');
        return digits.ToString();
    }

    // A number's bits, with every NaN the one NaN, as Java's has it.
    public static long DoubleBits(double v) => double.IsNaN(v) ? 0x7ff8000000000000L : BitConverter.DoubleToInt64Bits(v);
    public static int FloatBits(float v) => float.IsNaN(v) ? 0x7fc00000 : BitConverter.SingleToInt32Bits(v);

    // The end of a Java try-with-resources for one resource: a null is not closed; a close that throws while `thrown` is on its way is added to it.
    public static void CloseResource(IDisposable resource, Exception thrown)
    {
        if (resource == null) return;
        if (thrown == null) { resource.Dispose(); return; }
        try { resource.Dispose(); }
        catch (Exception closing) { AddSuppressed(thrown, closing); }
    }

    public static void AddSuppressed(Exception e, Exception more)
    {
        if (e.Data["java.suppressed"] is not List<Exception> held) e.Data["java.suppressed"] = held = new List<Exception>();
        held.Add(more);
    }

    // Java's getMessage: null for an exception made with no message, where .NET's Message makes one up.
    // It is recognised as the text the nearest .NET exception type gives when made with no arguments.
    static readonly Dictionary<Type, string> MadeUpMessages = new Dictionary<Type, string>();
    public static string Message(Exception e)
    {
        Type type = e.GetType();
        while (type.Assembly != typeof(Exception).Assembly) type = type.BaseType;
        if (!MadeUpMessages.TryGetValue(type, out string madeUp))
        {
            try { madeUp = ((Exception)Activator.CreateInstance(type)).Message; }
            catch (Exception) { madeUp = null; }
            MadeUpMessages[type] = madeUp;
        }
        return e.Message == madeUp ? null : e.Message;
    }

    public static Exception[] Suppressed(Exception e) => e.Data["java.suppressed"] is List<Exception> held ? held.ToArray() : new Exception[0];

    // A Java box that may be null: reading its number throws as Java's unboxing does.
    public static T Unbox<T>(T? v) where T : struct => v ?? throw new NullReferenceException();

    // map.get of boxed values: a key the map does not hold is a null.
    public static V? GetBox<K, V>(IDictionary<K, V> m, K key) where V : struct => m.TryGetValue(key, out V v) ? v : (V?)null;
    public static V? GetBox<K, V>(IDictionary<K, V?> m, K key) where V : struct => m.TryGetValue(key, out V? v) ? v : null;

    // `x++` of a box: the value it had, after the update.
    public static T Before<T, U>(T before, U after) => before;

    // == of two boxes compares references. Java shares the small ones, so those are the same box; two equal values past that range are
    // the same box only if one was copied from the other, which a value cannot say, so the program stops rather than guess.
    public static bool SameBox<T>(T? a, T? b) where T : struct
    {
        if (!a.HasValue || !b.HasValue) return !a.HasValue && !b.HasValue;
        if (!a.Value.Equals(b.Value)) return false;
        switch (a.Value)
        {
            case bool: return true;
            case sbyte: return true;
            case char c when c <= 127: return true;
            case short s when s >= -128 && s <= 127: return true;
            case int i when i >= -128 && i <= 127: return true;
            case long l when l >= -128 && l <= 127: return true;
        }
        throw new NotSupportedException("== of two equal boxes outside Java's shared range compares references");
    }

    // `o instanceof Box` of a generic class written raw: whether o is a Box of any type arguments, or of a class or interface derived from it
    // (`closed` is one Box: its generic definition is compared).
    public static bool isRaw(object o, Type closed)
    {
        if (o is null) return false;
        Type definition = closed.GetGenericTypeDefinition();
        for (Type t = o.GetType(); t != null; t = t.BaseType)
            if (t.IsGenericType && t.GetGenericTypeDefinition() == definition) return true;
        foreach (Type i in o.GetType().GetInterfaces())
            if (i.IsGenericType && i.GetGenericTypeDefinition() == definition) return true;
        return false;
    }

    // Number's intValue(), longValue() ... on a number known only as a Number (object here): the box's own value, converted as Java's
    // casts convert it (an integer wraps; a double truncates, NaN is 0 and a value out of range saturates, as .NET's casts do since 9).
    public static long numberLong(object n) => n switch
    {
        int v => v, long v => v, short v => v, sbyte v => v, byte v => v, char v => v,
        double v => (long)v, float v => (long)v,
        null => throw new NullReferenceException(),
        _ => throw new InvalidCastException(n.GetType().FullName + " is not a number Java's Number has"),
    };
    public static double numberDouble(object n) => n switch { double v => v, float v => v, _ => numberLong(n) };
    public static int numberInt(object n) => n is double d ? (int)d : n is float f ? (int)f : unchecked((int)numberLong(n));
    public static float numberFloat(object n) => n is float f ? f : n is double d ? (float)d : numberLong(n);
    public static short numberShort(object n) => unchecked((short)numberInt(n));
    public static sbyte numberByte(object n) => unchecked((sbyte)numberInt(n));

    // List.of: no nulls.
    public static List<T> listOf<T>(params T[] items)
    {
        foreach (T x in items)
            if (x is null) throw new NullReferenceException();
        return new List<T>(items);
    }

    // `new int[a][b]`: an array of arrays, every size checked before anything is made.
    public static T Jagged<T>(params int[] sizes)
    {
        foreach (int n in sizes)
            if (n < 0) throw new OverflowException(n.ToString());
        return (T)(object)MakeArray(typeof(T), sizes, 0);
    }

    // A size of `new int[n]` in a program that catches a negative one: the exception's message is the size, as Java's is.
    public static int ArraySize(int n) => n >= 0 ? n : throw new OverflowException(n.ToString());

    static Array MakeArray(Type type, int[] sizes, int at)
    {
        Type item = type.GetElementType();
        Array made = Array.CreateInstance(item, sizes[at]);
        if (at + 1 < sizes.Length)
            for (int i = 0; i < sizes[at]; i++) made.SetValue(MakeArray(item, sizes, at + 1), i);
        return made;
    }

    // `o instanceof Box`, where Box is generic.
    public static bool IsGeneric(object o, Type open)
    {
        if (o is null) return false;
        for (Type t = o.GetType(); t != null; t = t.BaseType)
            if (t.IsGenericType && t.GetGenericTypeDefinition() == open) return true;
        foreach (Type i in o.GetType().GetInterfaces())
            if (i.IsGenericType && i.GetGenericTypeDefinition() == open) return true;
        return false;
    }

    public static T EnumMade<T>(T constant, string name, int ordinal) where T : JEnum
    {
        constant.JName = name;
        constant.JOrdinal = ordinal;
        return constant;
    }

    public static T EnumOf<T>(T[] all, string name) where T : JEnum
    {
        if (name is null) throw new NullReferenceException("Name is null");
        foreach (T x in all)
            if (x.JName == name) return x;
        throw new ArgumentException("No enum constant " + typeof(T).Name + "." + name);
    }

    // ---- java.util.Iterator
    public static IEnumerator<T> Walk<T>(JIterator<T> it)
    {
        while (it.hasNext()) yield return it.next();
    }

    public static JIterator<T> iteratorOf<T>(IEnumerable<T> items) => new JEnumeratorIterator<T>(items.GetEnumerator());

    // ---- Map's default methods
    public static V merge<K, V>(IDictionary<K, V> m, K key, V value, Func<V, V, V> f)
    {
        if (value is null) throw new NullReferenceException();
        bool had = m.TryGetValue(key, out V old);
        if (!had || old is null)
        {
            Store(m, key, value, had);
            return value;
        }
        V made = f(old, value);
        if (made is null) m.Remove(key); else m[key] = made;
        return made;
    }

    public static V putIfAbsent<K, V>(IDictionary<K, V> m, K key, V value)
    {
        if (m.TryGetValue(key, out V old) && old is not null) return old;
        m[key] = value;
        return default!;
    }

    public static V computeIfAbsent<K, V>(IDictionary<K, V> m, K key, Func<K, V> f)
    {
        if (m.TryGetValue(key, out V old) && old is not null) return old;
        bool had = m.ContainsKey(key);
        V made = f(key);
        if (made is not null) Store(m, key, made, had);
        return made;
    }

    public static V computeIfPresent<K, V>(IDictionary<K, V> m, K key, Func<K, V, V> f)
    {
        if (!m.TryGetValue(key, out V old) || old is null) return default!;
        V made = f(key, old);
        if (made is null) m.Remove(key); else m[key] = made;
        return made;
    }

    public static V compute<K, V>(IDictionary<K, V> m, K key, Func<K, V, V> f)
    {
        bool had = m.TryGetValue(key, out V old);
        if (!had && typeof(V).IsValueType) throw new NotSupportedException("compute on a missing key hands the function a null this value type cannot hold");
        V made = f(key, old);
        if (made is null) { if (had) m.Remove(key); } else Store(m, key, made, had);
        return made;
    }

    // merge and the compute methods put a new key at the head of its bin, where put puts it at the tail: the order a walk shows depends on it.
    static void Store<K, V>(IDictionary<K, V> m, K key, V value, bool had)
    {
        if (!had && m is JHashMap<K, V> table) table.InsertAtHead(key, value);
        else m[key] = value;
    }

    // ---- java.util.Formatter: the conversions programs use, with Java's rounding (half up, on the shortest decimal of the double)
    public static string format(string f, params object[] args)
    {
        var b = new StringBuilder();
        int next = 0, last = -1;
        for (int i = 0; i < f.Length;)
        {
            if (f[i] != '%') { b.Append(f[i++]); continue; }
            i++;
            int index = -1, j = i;
            while (j < f.Length && char.IsDigit(f[j])) j++;
            if (j > i && j < f.Length && f[j] == '$') { index = int.Parse(f.Substring(i, j - i)) - 1; i = j + 1; }
            else if (i < f.Length && f[i] == '<') { index = last; i++; }
            int flagsAt = i;
            while (i < f.Length && "-#+ 0,(".IndexOf(f[i]) >= 0) i++;
            string flags = f.Substring(flagsAt, i - flagsAt);
            int width = -1, precision = -1;
            j = i;
            while (j < f.Length && char.IsDigit(f[j])) j++;
            if (j > i) { width = int.Parse(f.Substring(i, j - i)); i = j; }
            if (i < f.Length && f[i] == '.')
            {
                j = ++i;
                while (j < f.Length && char.IsDigit(f[j])) j++;
                precision = int.Parse(f.Substring(i, j - i));
                i = j;
            }
            if (i >= f.Length) throw new FormatException("a format that ends in the middle of a conversion");
            char conv = f[i++];
            if (conv == 'n') { b.Append(Environment.NewLine); continue; }
            if (conv == '%') { b.Append(Pad("%", width, flags)); continue; }
            if (index < 0) index = next++;
            if (args == null || index >= args.Length) throw new FormatException("Format specifier '%" + conv + "'");
            last = index;
            b.Append(Convert1(conv, args[index], flags, width, precision));
        }
        return b.ToString();
    }

    static string Pad(string s, int width, string flags) =>
        width <= s.Length ? s : flags.Contains('-') ? s.PadRight(width) : s.PadLeft(width);

    static string Convert1(char conv, object a, string flags, int width, int precision)
    {
        switch (conv)
        {
            case 's':
            case 'S':
            case 'b':
            case 'B':
            {
                string s = conv == 's' || conv == 'S' ? JavaRt.Str(a) : a is bool t ? (t ? "true" : "false") : a is null ? "false" : "true";
                if (precision >= 0 && precision < s.Length) s = s.Substring(0, precision);
                return Pad(char.IsUpper(conv) ? s.ToUpperInvariant() : s, width, flags);
            }
            case 'c':
            case 'C':
            {
                string s = a switch { null => "null", char c => c.ToString(), int p => char.ConvertFromUtf32(p), _ => throw new FormatException("%c of this value") };
                return Pad(conv == 'C' ? s.ToUpperInvariant() : s, width, flags);
            }
            case 'd':
                if (a is null) return Pad("null", width, flags);
                if (!(a is sbyte || a is short || a is int || a is long)) throw new FormatException("%d of this value");
                long v = Convert.ToInt64(a);
                string digits = v < 0 ? v.ToString().Substring(1) : v.ToString();
                return Signed(v < 0, Grouped(digits, flags), flags, width);
            case 'x':
            case 'X':
            case 'o':
            {
                if (a is null) return Pad("null", width, flags);
                ulong bits = a switch
                {
                    sbyte x8 => (byte)x8, short x16 => (ushort)x16, int x32 => (uint)x32, long x64 => unchecked((ulong)x64),
                    _ => throw new FormatException("%x of this value"),
                };
                string s = conv == 'o' ? Convert.ToString(unchecked((long)bits), 8) : bits.ToString("x");
                if (flags.Contains('#')) s = (conv == 'o' ? "0" : "0x") + s;
                if (flags.Contains('0') && width > s.Length) s = s.PadLeft(width, '0');
                return Pad(conv == 'X' ? s.ToUpperInvariant() : s, width, flags);
            }
            case 'f':
            case 'e':
            case 'E':
            {
                if (a is null) return Pad("null", width, flags);
                if (!(a is double || a is float)) throw new FormatException("%f of this value");
                double d = Convert.ToDouble(a);
                bool negative = double.IsNegative(d) && !double.IsNaN(d);
                if (double.IsNaN(d)) return Pad("NaN", width, flags);
                if (double.IsInfinity(d)) return Signed(negative, "Infinity", flags.Replace("0", ""), width);
                int places = precision < 0 ? 6 : precision;
                string body = conv == 'f' ? FixedText(Math.Abs(d), places, flags) : ExpText(Math.Abs(d), places);
                return Signed(negative, conv == 'E' ? body.ToUpperInvariant() : body, flags, width);
            }
            default:
                throw new NotSupportedException("the format conversion %" + conv);
        }
    }

    static string Signed(bool negative, string body, string flags, int width)
    {
        string before = negative ? (flags.Contains('(') ? "(" : "-") : flags.Contains('+') ? "+" : flags.Contains(' ') ? " " : "";
        string after = negative && flags.Contains('(') ? ")" : "";
        int room = width - before.Length - body.Length - after.Length;
        if (room > 0 && flags.Contains('0')) body = new string('0', room) + body;
        return Pad(before + body + after, width, flags);
    }

    static string Grouped(string digits, string flags)
    {
        if (!flags.Contains(',')) return digits;
        var b = new StringBuilder();
        for (int i = 0; i < digits.Length; i++)
        {
            if (i > 0 && (digits.Length - i) % 3 == 0) b.Append(',');
            b.Append(digits[i]);
        }
        return b.ToString();
    }

    // The shortest decimal of a non-negative double: its digits, with the value 0.digits * 10^point.
    static (string digits, int point) Decimal(double v)
    {
        if (v == 0) return ("", 0);
        string r = v.ToString("R", System.Globalization.CultureInfo.InvariantCulture);
        int e = r.IndexOfAny(new[] { 'E', 'e' });
        int exp = e >= 0 ? int.Parse(r.Substring(e + 1), System.Globalization.CultureInfo.InvariantCulture) : 0;
        string mantissa = e >= 0 ? r.Substring(0, e) : r;
        int dot = mantissa.IndexOf('.');
        int point = (dot >= 0 ? dot : mantissa.Length) + exp;
        string digits = mantissa.Replace(".", "");
        int lead = 0;
        while (lead < digits.Length && digits[lead] == '0') lead++;
        return (digits.Substring(lead).TrimEnd('0'), point - lead);
    }

    // `keep` digits of `digits`, rounded half up; a carry out of the first digit moves the point.
    static (string digits, int point) RoundUp(string digits, int point, int keep)
    {
        if (keep < 0) return ("", point);
        if (keep >= digits.Length) return (digits, point);
        bool up = digits[keep] >= '5';
        char[] kept = digits.Substring(0, keep).ToCharArray();
        if (!up) return (new string(kept), point);
        for (int i = keep - 1; i >= 0; i--)
        {
            if (kept[i] != '9') { kept[i]++; return (new string(kept), point); }
            kept[i] = '0';
        }
        return ("1" + new string(kept), point + 1);
    }

    static string FixedText(double v, int places, string flags)
    {
        (string digits, int point) = Decimal(v);
        (digits, point) = RoundUp(digits, point, point + places);
        string whole = point > 0 ? digits.PadRight(point, '0').Substring(0, point) : "0";
        var fraction = new StringBuilder();
        if (point < 0) fraction.Append('0', Math.Min(-point, places));
        if (digits.Length > Math.Max(point, 0)) fraction.Append(digits.Substring(Math.Max(point, 0)));
        string text = fraction.ToString().PadRight(places, '0');
        if (text.Length > places) text = text.Substring(0, places);
        whole = Grouped(whole, flags);
        return places == 0 ? whole : whole + "." + text;
    }

    static string ExpText(double v, int places)
    {
        (string digits, int point) = Decimal(v);
        if (digits.Length == 0) return (places == 0 ? "0" : "0." + new string('0', places)) + "e+00";
        (digits, point) = RoundUp(digits, point, places + 1);
        digits = digits.PadRight(places + 1, '0');
        int exp = point - 1;
        string head = digits.Substring(0, 1) + (places == 0 ? "" : "." + digits.Substring(1, places));
        return head + "e" + (exp < 0 ? "-" : "+") + Math.Abs(exp).ToString("00");
    }

    // Set.of and Map.of: no nulls, no repeats, no changes. Java shuffles their order afresh on each run, so with two or more items a
    // walk throws NotSupportedException: no order written here could be the one Java shows.
    public static JHashSet<T> setOf<T>(params T[] items)
    {
        var set = new JHashSet<T>();
        foreach (T x in items)
        {
            if (x is null) throw new NullReferenceException();
            if (!set.Add(x)) throw new ArgumentException("duplicate element: " + JavaRt.Str((object)x));
        }
        set.Freeze();
        return set;
    }

    public static JHashMap<K, V> mapOf<K, V>(params (K key, V value)[] items)
    {
        var map = new JHashMap<K, V>();
        foreach ((K key, V value) in items)
        {
            if (key is null || value is null) throw new NullReferenceException();
            if (map.ContainsKey(key)) throw new ArgumentException("duplicate key: " + JavaRt.Str((object)key));
            map[key] = value;
        }
        map.Freeze();
        return map;
    }
}

// java.lang.Enum, for an enum with fields or methods: a class whose constants are its static fields.
public abstract class JEnum : IComparable
{
    public string JName { get; internal set; }
    public int JOrdinal { get; internal set; }
    public override string ToString() => JName;
    public int CompareTo(object other) => JOrdinal - ((JEnum)other).JOrdinal;
}

// java.util.Iterator.
public interface JIterator<T>
{
    bool hasNext();
    T next();
}

// A .NET enumerator as a Java iterator, which must know whether there is a next item before it is asked for it.
sealed class JEnumeratorIterator<T> : JIterator<T>
{
    readonly IEnumerator<T> items;
    bool looked, has;

    internal JEnumeratorIterator(IEnumerator<T> items) { this.items = items; }

    public bool hasNext()
    {
        if (!looked) { has = items.MoveNext(); looked = true; }
        return has;
    }

    public T next()
    {
        if (!hasNext()) throw new InvalidOperationException();
        looked = false;
        return items.Current;
    }
}

public class JHashMap<K, V> : IDictionary<K, V>
{
    sealed class Node
    {
        public int hash;
        public K key;
        public V value;
        public Node next, before, after;
    }

    const int MaxCapacity = 1 << 30;
    Node[] table;
    int size, threshold;
    readonly float loadFactor = 0.75f;
    readonly bool linked;
    Node head, tail;
    bool treeBin;      // a bin Java would have made a tree
    bool frozen;       // Set.of or Map.of

    internal void Freeze() => frozen = true;
    internal IEnumerable<K> AllKeys() { foreach (Node e in Walk()) yield return e.key; }
    void Unchanged() { if (frozen) throw new NotSupportedException(); }

    public JHashMap() { }
    public JHashMap(int capacity) : this(capacity, 0.75f) { }

    public JHashMap(int capacity, float loadFactor)
    {
        if (capacity < 0) throw new ArgumentException("Illegal initial capacity: " + capacity);
        if (loadFactor <= 0 || float.IsNaN(loadFactor)) throw new ArgumentException("Illegal load factor: " + loadFactor);
        this.loadFactor = loadFactor;
        threshold = TableSizeFor(capacity);
    }

    public JHashMap(IDictionary<K, V> from) => PutAll(from);

    protected JHashMap(bool linked) { this.linked = linked; }
    protected JHashMap(int capacity, float loadFactor, bool linked) : this(capacity, loadFactor) { this.linked = linked; }

    static int TableSizeFor(int capacity)
    {
        int n = capacity <= 1 ? 0 : (int)(uint.MaxValue >> System.Numerics.BitOperations.LeadingZeroCount((uint)(capacity - 1)));
        return n >= MaxCapacity ? MaxCapacity : n + 1;
    }

    static int Spread(K key)
    {
        if (key is null) return 0;
        int h = JLib.Hash(key);
        return h ^ (int)((uint)h >> 16);
    }

    protected void PutAll(IDictionary<K, V> from)
    {
        int s = from.Count;
        if (s > 0)
        {
            if (table == null)
            {
                double wanted = Math.Ceiling(s / (double)loadFactor);
                int t = wanted < MaxCapacity ? (int)wanted : MaxCapacity;
                if (t > threshold) threshold = TableSizeFor(t);
            }
            else
            {
                while (s > threshold && table.Length < MaxCapacity) Resize();
            }
            foreach (KeyValuePair<K, V> e in from) Put(e.Key, e.Value);
        }
    }

    Node[] Resize()
    {
        Node[] old = table;
        int oldCap = old?.Length ?? 0, oldThr = threshold, newCap, newThr = 0;
        if (oldCap > 0)
        {
            if (oldCap >= MaxCapacity)
            {
                threshold = int.MaxValue;
                return old;
            }
            newCap = oldCap << 1;
            if (newCap < MaxCapacity && oldCap >= 16) newThr = oldThr << 1;
        }
        else if (oldThr > 0)
        {
            newCap = oldThr;
        }
        else
        {
            newCap = 16;
            newThr = 12;
        }
        if (newThr == 0)
        {
            float ft = newCap * loadFactor;
            newThr = newCap < MaxCapacity && ft < MaxCapacity ? (int)ft : int.MaxValue;
        }
        threshold = newThr;
        var fresh = new Node[newCap];
        table = fresh;
        for (int j = 0; j < oldCap; j++)
        {
            Node loHead = null, loTail = null, hiHead = null, hiTail = null;
            for (Node e = old[j], next; e != null; e = next)     // each bin splits in two, its order kept
            {
                next = e.next;
                e.next = null;
                if ((e.hash & oldCap) == 0)
                {
                    if (loTail == null) loHead = e; else loTail.next = e;
                    loTail = e;
                }
                else
                {
                    if (hiTail == null) hiHead = e; else hiTail.next = e;
                    hiTail = e;
                }
            }
            fresh[j] = loHead;
            fresh[j + oldCap] = hiHead;
        }
        return fresh;
    }

    Node Find(K key)
    {
        if (table == null) return null;
        int h = Spread(key);
        for (Node e = table[h & (table.Length - 1)]; e != null; e = e.next)
            if (e.hash == h && JLib.Equal(key, e.key)) return e;
        return null;
    }

    Node Made(int hash, K key, V value)
    {
        var n = new Node { hash = hash, key = key, value = value };
        if (linked)
        {
            n.before = tail;
            if (tail == null) head = n; else tail.after = n;
            tail = n;
        }
        return n;
    }

    void Put(K key, V value)
    {
        Unchanged();
        Node[] tab = table ?? Resize();
        int h = Spread(key), i = h & (tab.Length - 1);
        Node p = tab[i];
        if (p == null)
        {
            tab[i] = Made(h, key, value);
        }
        else
        {
            for (int count = 0; ; count++)
            {
                if (p.hash == h && JLib.Equal(key, p.key))
                {
                    p.value = value;
                    return;
                }
                if (p.next == null)
                {
                    p.next = Made(h, key, value);
                    if (count >= 7)
                    {
                        if (tab.Length < 64) Resize();              // (as Java: a small table grows instead of making a tree)
                        else if (!linked) treeBin = true;           // (a linked map's order is its links, whatever the bin is)
                    }
                    break;
                }
                p = p.next;
            }
        }
        if (++size > threshold) Resize();
    }

    // As HashMap.merge and compute add a key: the table grows first, on the size before this key, and the key goes to the head of its bin.
    internal void InsertAtHead(K key, V value)
    {
        Unchanged();
        Node[] tab = size > threshold || table == null ? Resize() : table;
        int h = Spread(key), i = h & (tab.Length - 1), count = 0;
        for (Node e = tab[i]; e != null; e = e.next) count++;
        Node n = Made(h, key, value);
        n.next = tab[i];
        tab[i] = n;
        if (count >= 7)
        {
            if (tab.Length < 64) Resize();
            else if (!linked) treeBin = true;
        }
        size++;
    }

    public bool Remove(K key)
    {
        Unchanged();
        if (table == null) return false;
        int h = Spread(key), i = h & (table.Length - 1);
        Node prev = null;
        for (Node e = table[i]; e != null; prev = e, e = e.next)
        {
            if (e.hash != h || !JLib.Equal(key, e.key)) continue;
            if (prev == null) table[i] = e.next; else prev.next = e.next;
            if (linked)
            {
                if (e.before == null) head = e.after; else e.before.after = e.after;
                if (e.after == null) tail = e.before; else e.after.before = e.before;
            }
            size--;
            return true;
        }
        return false;
    }

    IEnumerable<Node> Nodes()
    {
        if (treeBin) throw new NotSupportedException("this HashMap has a bin Java keeps as a tree: its order cannot be reproduced");
        if (frozen && size > 1) throw new NotSupportedException("Java's Set.of and Map.of change their order from run to run");
        return Walk();
    }

    IEnumerable<Node> Walk()
    {
        if (linked)
        {
            for (Node e = head; e != null; e = e.after) yield return e;
        }
        else if (table != null)
        {
            Node[] tab = table;
            for (int i = 0; i < tab.Length; i++)
                for (Node e = tab[i]; e != null; e = e.next) yield return e;
        }
    }

    public V this[K key]
    {
        get
        {
            Node e = Find(key);
            if (e == null) throw new KeyNotFoundException();
            return e.value;
        }
        set => Put(key, value);
    }

    public int Count => size;
    public bool IsReadOnly => false;
    public ICollection<K> Keys => new JMapView<K>(() => KeyItems(), () => size, x => ContainsKey(x));
    public ICollection<V> Values => new JMapView<V>(() => ValueItems(), () => size, x => { foreach (Node e in Walk()) if (JLib.Equal(x, e.value)) return true; return false; });
    IEnumerable<K> KeyItems() { foreach (Node e in Nodes()) yield return e.key; }
    IEnumerable<V> ValueItems() { foreach (Node e in Nodes()) yield return e.value; }

    public void Add(K key, V value)
    {
        if (Find(key) != null) throw new ArgumentException("the key is there already");
        Put(key, value);
    }

    public void Add(KeyValuePair<K, V> item) => Add(item.Key, item.Value);
    public bool ContainsKey(K key) => Find(key) != null;

    public bool TryGetValue(K key, out V value)
    {
        Node e = Find(key);
        value = e != null ? e.value : default!;
        return e != null;
    }

    public void Clear()
    {
        Unchanged();
        if (table != null) Array.Clear(table, 0, table.Length);   // (the table keeps its size, as Java's does)
        size = 0;
        head = tail = null;
    }

    public bool Contains(KeyValuePair<K, V> item)
    {
        Node e = Find(item.Key);
        return e != null && JLib.Equal(e.value, item.Value);
    }

    public void CopyTo(KeyValuePair<K, V>[] array, int at)
    {
        foreach (Node e in Nodes()) array[at++] = new KeyValuePair<K, V>(e.key, e.value);
    }

    public bool Remove(KeyValuePair<K, V> item) => Contains(item) && Remove(item.Key);

    public IEnumerator<KeyValuePair<K, V>> GetEnumerator()
    {
        foreach (Node e in Nodes()) yield return new KeyValuePair<K, V>(e.key, e.value);
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    public override string ToString()
    {
        var b = new StringBuilder("{");
        bool first = true;
        foreach (Node e in Nodes())
        {
            if (!first) b.Append(", ");
            first = false;
            b.Append(ReferenceEquals(e.key, this) ? "(this Map)" : JavaRt.Str((object)e.key!)).Append('=');
            b.Append(ReferenceEquals(e.value, this) ? "(this Map)" : JavaRt.Str((object)e.value!));
        }
        return b.Append('}').ToString();
    }

    public override bool Equals(object o)
    {
        if (ReferenceEquals(o, this)) return true;
        if (!(o is IDictionary<K, V> m) || m.Count != size) return false;
        foreach (Node e in Walk())
            if (!m.TryGetValue(e.key, out V v) || !JLib.Equal(e.value, v)) return false;
        return true;
    }

    public override int GetHashCode()
    {
        int h = 0;
        foreach (Node e in Walk()) h = unchecked(h + (JLib.Hash(e.key) ^ JLib.Hash(e.value)));
        return h;
    }
}

public class JLinkedHashMap<K, V> : JHashMap<K, V>
{
    public JLinkedHashMap() : base(true) { }
    public JLinkedHashMap(int capacity) : base(capacity, 0.75f, true) { }
    public JLinkedHashMap(int capacity, float loadFactor) : base(capacity, loadFactor, true) { }
    public JLinkedHashMap(IDictionary<K, V> from) : base(true) => PutAll(from);
}

// A map's keys or values, as they are now.
public sealed class JMapView<T> : ICollection<T>
{
    readonly Func<IEnumerable<T>> items;
    readonly Func<int> count;
    readonly Func<T, bool> has;

    internal JMapView(Func<IEnumerable<T>> items, Func<int> count, Func<T, bool> has)
    {
        this.items = items;
        this.count = count;
        this.has = has;
    }

    public int Count => count();
    public bool IsReadOnly => true;
    public bool Contains(T x) => has(x);
    public void CopyTo(T[] array, int at) { foreach (T x in items()) array[at++] = x; }
    public void Add(T x) => throw new NotSupportedException();
    public void Clear() => throw new NotSupportedException();
    public bool Remove(T x) => throw new NotSupportedException();
    public IEnumerator<T> GetEnumerator() => items().GetEnumerator();
    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    public override string ToString() => JSetText.Of(this);
}

static class JSetText
{
    internal static string Of<T>(IEnumerable<T> items)
    {
        var b = new StringBuilder("[");
        bool first = true;
        foreach (T x in items)
        {
            if (!first) b.Append(", ");
            first = false;
            b.Append(JavaRt.Str((object)x!));
        }
        return b.Append(']').ToString();
    }
}

public class JHashSet<T> : ISet<T>
{
    readonly JHashMap<T, bool> map;

    public JHashSet() { map = new JHashMap<T, bool>(); }
    public JHashSet(int capacity) { map = new JHashMap<T, bool>(capacity); }
    public JHashSet(int capacity, float loadFactor) { map = new JHashMap<T, bool>(capacity, loadFactor); }

    public JHashSet(IEnumerable<T> from)
    {
        var items = new List<T>(from);
        map = new JHashMap<T, bool>(Math.Max((int)(items.Count / .75f) + 1, 16));
        foreach (T x in items) Add(x);
    }

    protected JHashSet(JHashMap<T, bool> map) { this.map = map; }

    internal void Freeze() => map.Freeze();

    public int Count => map.Count;
    public bool IsReadOnly => false;

    public bool Add(T x)
    {
        if (map.ContainsKey(x)) return false;
        map[x] = true;
        return true;
    }

    void ICollection<T>.Add(T x) => Add(x);
    public bool Contains(T x) => map.ContainsKey(x);
    public bool Remove(T x) => map.Remove(x);
    public void Clear() => map.Clear();
    public void CopyTo(T[] array, int at) => map.Keys.CopyTo(array, at);
    public IEnumerator<T> GetEnumerator() => map.Keys.GetEnumerator();
    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    public void UnionWith(IEnumerable<T> other) { foreach (T x in other) Add(x); }
    public void ExceptWith(IEnumerable<T> other) { foreach (T x in new List<T>(other)) Remove(x); }

    public void IntersectWith(IEnumerable<T> other)
    {
        var keep = new JHashSet<T>(other);
        foreach (T x in new List<T>(this))
            if (!keep.Contains(x)) Remove(x);
    }

    public void SymmetricExceptWith(IEnumerable<T> other)
    {
        foreach (T x in new JHashSet<T>(other))
            if (!Remove(x)) Add(x);
    }

    public bool IsSubsetOf(IEnumerable<T> other)
    {
        var o = new JHashSet<T>(other);
        foreach (T x in map.AllKeys())
            if (!o.Contains(x)) return false;
        return true;
    }

    public bool IsSupersetOf(IEnumerable<T> other)
    {
        foreach (T x in other)
            if (!Contains(x)) return false;
        return true;
    }

    public bool IsProperSubsetOf(IEnumerable<T> other) { var o = new JHashSet<T>(other); return Count < o.Count && IsSubsetOf(o); }
    public bool IsProperSupersetOf(IEnumerable<T> other) { var o = new JHashSet<T>(other); return Count > o.Count && IsSupersetOf(o); }

    public bool Overlaps(IEnumerable<T> other)
    {
        foreach (T x in other)
            if (Contains(x)) return true;
        return false;
    }

    public bool SetEquals(IEnumerable<T> other) { var o = new JHashSet<T>(other); return Count == o.Count && IsSubsetOf(o); }

    public override string ToString() => JSetText.Of(this);
    public override bool Equals(object o) => ReferenceEquals(o, this) || (o is ISet<T> s && s.Count == Count && IsSupersetOf(s));

    public override int GetHashCode()
    {
        int h = 0;
        foreach (T x in map.AllKeys()) h = unchecked(h + JLib.Hash(x));
        return h;
    }
}

public class JLinkedHashSet<T> : JHashSet<T>
{
    public JLinkedHashSet() : base(new JLinkedHashMap<T, bool>()) { }
    public JLinkedHashSet(int capacity) : base(new JLinkedHashMap<T, bool>(capacity)) { }
    public JLinkedHashSet(int capacity, float loadFactor) : base(new JLinkedHashMap<T, bool>(capacity, loadFactor)) { }
    public JLinkedHashSet(IEnumerable<T> from) : base(new JLinkedHashMap<T, bool>()) => UnionWith(from);
}
