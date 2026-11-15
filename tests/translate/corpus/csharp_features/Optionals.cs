// Nullable value types: lifted operators (null in, null out; a relational operator is false), mixed with plain values, in
// compound assignments, in text, the null-coalescing chain, .Value on null, nullable bool in conditions.
using System;
using System.Collections.Generic;

public struct Money
{
    public int Cents;
    public Money(int c) { Cents = c; }
}

public static class Optionals
{
    static int? Find(int[] xs, int target)
    {
        for (int i = 0; i < xs.Length; i++) if (xs[i] == target) return i;
        return null;
    }

    static string Show(int? x) => x.HasValue ? "some " + x.Value : "none";

    public static void Main()
    {
        int? a = 5, b = null, c = 7;
        Console.WriteLine(Show(a + b));
        Console.WriteLine(Show(a + c));
        Console.WriteLine(Show(a * 2));
        Console.WriteLine(Show(10 - a));
        Console.WriteLine(Show(a - b));
        Console.WriteLine(Show(-a));
        Console.WriteLine(a < c);
        Console.WriteLine(a > c);
        Console.WriteLine(a < b);
        Console.WriteLine(a >= b);
        Console.WriteLine(a == b);
        Console.WriteLine(b == null);
        Console.WriteLine(a != c);
        Console.WriteLine(a == 5);
        Console.WriteLine(b ?? a ?? c);
        Console.WriteLine(b ?? 100);
        int? total = 0;
        foreach (int? x in new int?[] { 1, null, 3 }) total += x;
        Console.WriteLine(Show(total));
        int? sum = 0;
        foreach (int? x in new int?[] { 1, 2, 3 }) sum += x;
        Console.WriteLine(Show(sum));
        Console.WriteLine("a is " + a + " and b is [" + b + "]");
        int[] data = { 4, 8, 15, 16, 23, 42 };
        Console.WriteLine(Show(Find(data, 15)));
        Console.WriteLine(Show(Find(data, 99)));
        Console.WriteLine(Find(data, 99).GetValueOrDefault(-1));
        Console.WriteLine(Find(data, 23).GetValueOrDefault(-1));
        bool? flag = true;
        if (flag == true) Console.WriteLine("flag is true");
        flag = null;
        if (flag != true) Console.WriteLine("flag is not true");
        if (!flag.HasValue) Console.WriteLine("flag has no value");
        double? d = 2.5;
        Console.WriteLine(Show((int?)(d * 4)));
        Console.WriteLine((d ?? 0) + 1);
        long? big = 3000000000;
        Console.WriteLine(big + 1);
        Money? m = new Money(250);
        Console.WriteLine(m.HasValue ? m.Value.Cents : -1);
        Money? none = null;
        Console.WriteLine(none.HasValue);
        try
        {
            int v = b.Value;
            Console.WriteLine(v);
        }
        catch (InvalidOperationException)
        {
            Console.WriteLine("no value");
        }
        string name = null;
        Console.WriteLine(name?.Length);
        Console.WriteLine((name ?? "anonymous").ToUpper());
        List<string> names = null;
        Console.WriteLine(names?.Count ?? -1);
        names = new List<string> { "x", "yy" };
        Console.WriteLine(names?.Count ?? -1);
    }
}
