// Collections: a binary tree, Dictionary iteration, a struct with value semantics and an array of them, an enum switch, recursion, number
// formatting (double, float, long, ToString("F2")), StringBuilder with chars, null strings and ??, Array.Sort, List Sum/Min/Max.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public enum Color { Red, Green, Blue }

public interface IVisitor<T> { T Visit(Node n); }

public class Node
{
    public int Value;
    public Node Left, Right;
    public Node(int v) { Value = v; }
    public void Insert(int v)
    {
        if (v < Value) { if (Left == null) Left = new Node(v); else Left.Insert(v); }
        else { if (Right == null) Right = new Node(v); else Right.Insert(v); }
    }
    public void InOrder(List<int> into)
    {
        if (Left != null) Left.InOrder(into);
        into.Add(Value);
        if (Right != null) Right.InOrder(into);
    }
}

public struct Point
{
    public double X, Y;
    public Point(double x, double y) { X = x; Y = y; }
    public double Dist(Point o) { double dx = X - o.X, dy = Y - o.Y; return Math.Sqrt(dx * dx + dy * dy); }
    public override string ToString() { return "(" + X + ", " + Y + ")"; }
}

public static class Program
{
    static string Describe(Color c)
    {
        switch (c)
        {
            case Color.Red: return "warm";
            case Color.Blue: return "cool";
            default: return "neutral";
        }
    }

    static int Gcd(int a, int b) { return b == 0 ? a : Gcd(b, a % b); }

    public static void Main()
    {
        var root = new Node(50);
        foreach (int v in new[] { 30, 70, 20, 40, 60, 80, 35 }) root.Insert(v);
        var sorted = new List<int>();
        root.InOrder(sorted);
        Console.WriteLine(string.Join(" ", sorted));

        var counts = new Dictionary<string, int>();
        foreach (var w in "the cat and the hat and the bat".Split(' '))
            counts[w] = counts.ContainsKey(w) ? counts[w] + 1 : 1;
        foreach (var kv in counts) Console.WriteLine(kv.Key + "=" + kv.Value);

        Point a = new Point(0, 0), b = new Point(3, 4);
        Console.WriteLine("dist " + a.Dist(b) + " " + b);
        Point[] pts = new Point[3];
        pts[1] = b;
        Console.WriteLine(pts[0] + " " + pts[1]);

        foreach (Color c in new[] { Color.Red, Color.Green, Color.Blue })
            Console.WriteLine(c + " is " + Describe(c) + " " + (int)c);

        Console.WriteLine("gcd " + Gcd(84, 36) + " " + 10 / 3.0 + " " + (float)1 / 3 + " " + 7L * 1000000000L);
        double avg = sorted.Sum() / (double)sorted.Count;
        Console.WriteLine($"avg {avg.ToString("F2")} max {sorted.Max()} min {sorted.Min()} n={sorted.Count}");
        var sb = new StringBuilder();
        for (char ch = 'a'; ch <= 'e'; ch++) sb.Append(ch).Append(ch - 'a');
        Console.WriteLine(sb);
        string s = null;
        Console.WriteLine((s ?? "none") + " " + (s == null) + " " + string.IsNullOrEmpty(s));
        int[] arr = { 5, 3, 9, 1 };
        Array.Sort(arr);
        Console.WriteLine(string.Join(",", arr) + " " + arr.Length);
    }
}
