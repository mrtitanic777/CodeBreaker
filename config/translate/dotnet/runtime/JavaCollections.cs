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

    // List.of: no nulls.
    public static List<T> listOf<T>(params T[] items)
    {
        foreach (T x in items)
            if (x is null) throw new NullReferenceException();
        return new List<T>(items);
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

    public bool Remove(K key)
    {
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
        foreach (T x in this)
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
        foreach (T x in this) h = unchecked(h + JLib.Hash(x));
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
