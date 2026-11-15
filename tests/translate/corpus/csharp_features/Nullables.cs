using System;
using System.Collections.Generic;

public static class Nullables
{
    static int? Parse(string s)
    {
        if (s == "1") return 1;
        if (s == "2") return 2;
        return null;
    }

    static int? Add(int? a, int? b) => a + b;

    class Node { public string Name; public Node Next; public Node(string n, Node nx) { Name = n; Next = nx; } }

    public static void Main()
    {
        int? a = Parse("1"), b = Parse("x");
        Console.WriteLine(a.HasValue);
        Console.WriteLine(b.HasValue);
        Console.WriteLine(a ?? -1);
        Console.WriteLine(b ?? -1);
        Console.WriteLine(a.GetValueOrDefault() + b.GetValueOrDefault());
        Console.WriteLine(Add(a, a).Value);
        Console.WriteLine(Add(a, b).HasValue);
        if (a is int v) Console.WriteLine("v=" + v);
        double? d = null;
        d ??= 2.5;
        Console.WriteLine(d.Value);
        Node n = new Node("a", new Node("b", null));
        Console.WriteLine(n.Next?.Name);
        Console.WriteLine(n.Next?.Next?.Name ?? "none");
        Console.WriteLine(n.Next?.Next?.Name.Length ?? -1);
        bool? flag = null;
        Console.WriteLine(flag == null);
        Console.WriteLine(a == 1);
        Console.WriteLine(a < 2);
        string s = null;
        Console.WriteLine(s?.Length ?? 0);
        List<int?> xs = new List<int?> { 1, null, 3 };
        int total = 0;
        foreach (var x in xs) total += x ?? 10;
        Console.WriteLine(total);
    }
}
