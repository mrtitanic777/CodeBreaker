using System;

public abstract class Shape { }
public class Circle : Shape { public double R; public Circle(double r) { R = r; } }
public class Rect : Shape { public double W, H; public Rect(double w, double h) { W = w; H = h; } }
public class Tri : Shape { public double B, H; public Tri(double b, double h) { B = b; H = h; } }

public static class Patterns
{
    static string Classify(int n) => n switch
    {
        < 0 => "negative",
        0 => "zero",
        > 0 and < 10 => "small",
        >= 10 and < 100 => "medium",
        _ => "large"
    };

    static double Area(Shape s) => s switch
    {
        Circle c => 3.0 * c.R * c.R,
        Rect r when r.W == r.H => r.W * r.W,
        Rect r => r.W * r.H,
        Tri t => 0.5 * t.B * t.H,
        _ => 0
    };

    static string Describe(object o)
    {
        switch (o)
        {
            case int i when i > 100: return "big int " + i;
            case int i: return "int " + i;
            case string s: return "string " + s.Length;
            case null: return "null";
            default: return "other";
        }
    }

    static string Day(int d) => d switch { 1 or 7 => "weekend", >= 2 and <= 6 => "weekday", _ => "?" };

    static string Kind(object o)
    {
        if (o is int n && n > 5) return "int>5";
        if (o is not string) return "not string";
        return "string";
    }

    public static void Main()
    {
        foreach (var n in new[] { -5, 0, 7, 42, 1000 }) Console.WriteLine(Classify(n));
        Shape[] shapes = { new Circle(2), new Rect(3, 3), new Rect(2, 5), new Tri(4, 5) };
        foreach (var s in shapes) Console.WriteLine(Area(s));
        Console.WriteLine(Describe(500)); Console.WriteLine(Describe(5)); Console.WriteLine(Describe("hey"));
        Console.WriteLine(Describe(null)); Console.WriteLine(Describe(2.5));
        for (int d = 0; d < 9; d += 4) Console.WriteLine(Day(d));
        Console.WriteLine(Kind(9)); Console.WriteLine(Kind(3)); Console.WriteLine(Kind("x"));
    }
}
