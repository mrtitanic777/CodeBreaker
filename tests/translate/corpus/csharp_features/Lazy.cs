// Deferred execution: an iterator or a query runs as it is read, not when it is made. The prints show the order.
using System;
using System.Collections.Generic;
using System.Linq;

public class Counter
{
    private int step;
    public Counter(int step) { this.step = step; }

    // an instance iterator: it reads a field as it runs
    public IEnumerable<int> Multiples(int n)
    {
        for (int i = 1; i <= n; i++)
        {
            Console.WriteLine("  produce " + i * step);
            yield return i * step;
        }
    }
}

public class Tree
{
    public int Value;
    public Tree Left, Right;
    public Tree(int v, Tree l, Tree r) { Value = v; Left = l; Right = r; }

    // a recursive iterator: each level is an iterator of its own
    public IEnumerable<int> InOrder()
    {
        if (Left != null) foreach (var x in Left.InOrder()) yield return x;
        yield return Value;
        if (Right != null) foreach (var x in Right.InOrder()) yield return x;
    }
}

public static class Lazy
{
    static IEnumerable<int> Naturals()
    {
        int n = 0;
        while (true) yield return n++;
    }

    static IEnumerable<string> Noisy(string name, int count)
    {
        Console.WriteLine(name + " starts");
        try
        {
            for (int i = 0; i < count; i++) yield return name + i;
        }
        finally
        {
            Console.WriteLine(name + " disposed");
        }
    }

    static IEnumerable<int> Failing()
    {
        yield return 1;
        throw new InvalidOperationException("generator failed");
    }

    public static void Main()
    {
        Console.WriteLine("take from an endless sequence");
        Console.WriteLine(string.Join(",", Naturals().Where(x => x % 3 == 0).Select(x => x * x).Take(5)));

        Console.WriteLine("made, not yet read");
        var seq = new Counter(10).Multiples(3);
        Console.WriteLine("reading");
        foreach (var x in seq) Console.WriteLine("got " + x);
        Console.WriteLine("reading again");
        Console.WriteLine(seq.Sum());

        Console.WriteLine("interleaved");
        foreach (var s in Noisy("a", 2)) Console.WriteLine(s);
        foreach (var s in Noisy("b", 5)) { Console.WriteLine(s); if (s == "b1") break; }
        Console.WriteLine(Noisy("c", 3).First());

        Console.WriteLine("exceptions");
        try { foreach (var x in Failing()) Console.WriteLine(x); }
        catch (InvalidOperationException e) { Console.WriteLine("caught " + e.Message); }

        Tree t = new Tree(4, new Tree(2, new Tree(1, null, null), new Tree(3, null, null)), new Tree(6, new Tree(5, null, null), new Tree(7, null, null)));
        Console.WriteLine(string.Join(" ", t.InOrder()));

        var log = new List<string>();
        var query = new[] { 3, 1, 2 }.Select(x => { log.Add("select " + x); return x * 2; }).Where(x => { log.Add("where " + x); return x > 2; });
        log.Add("built");
        foreach (var v in query) log.Add("use " + v);
        Console.WriteLine(string.Join("; ", log));
        Console.WriteLine(Enumerable.Range(1, 6).Skip(2).Take(3).Reverse().Count());
    }
}
