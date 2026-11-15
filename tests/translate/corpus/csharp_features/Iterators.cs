using System;
using System.Collections.Generic;

public static class Iterators
{
    static IEnumerable<int> Count(int from, int to)
    {
        for (int i = from; i < to; i++) yield return i;
    }

    static IEnumerable<int> Evens(IEnumerable<int> src)
    {
        foreach (int x in src)
        {
            if (x % 2 != 0) continue;
            yield return x;
        }
    }

    static IEnumerable<string> Words()
    {
        yield return "alpha";
        yield return "beta";
        yield break;
    }

    static IEnumerable<int> Fib(int n)
    {
        int a = 0, b = 1;
        while (true)
        {
            if (n-- == 0) yield break;
            yield return a;
            int t = a + b; a = b; b = t;
        }
    }

    static IEnumerable<int> WithFinally()
    {
        try
        {
            yield return 1;
            yield return 2;
        }
        finally
        {
            Console.WriteLine("cleanup");
        }
    }

    public static void Main()
    {
        foreach (var x in Count(0, 5)) Console.Write(x + " ");
        Console.WriteLine();
        foreach (var x in Evens(Count(0, 10))) Console.Write(x + " ");
        Console.WriteLine();
        foreach (var w in Words()) Console.WriteLine(w);
        int sum = 0;
        foreach (var f in Fib(10)) sum += f;
        Console.WriteLine(sum);
        foreach (var x in WithFinally()) Console.WriteLine(x);
        foreach (var x in WithFinally()) { Console.WriteLine(x); break; }
    }
}
