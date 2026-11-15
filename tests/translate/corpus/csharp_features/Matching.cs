// Pattern matching beyond the type test: property patterns, nested ones, `is not null`, and / or / not, string and char constants,
// `when` guards with a captured variable, a switch statement whose sections share a body, a type pattern on a base type.
using System;

public enum Level { Low, Mid, High }

public class Point { public int X, Y; public Point(int x, int y) { X = x; Y = y; } }
public class Segment { public Point From, To; public Segment(Point f, Point t) { From = f; To = t; } }

public static class Matching
{
    static string Where(Point p) => p switch
    {
        { X: 0, Y: 0 } => "origin",
        { X: 0 } => "on the y axis",
        { Y: 0 } => "on the x axis",
        { X: var x, Y: var y } when x == y => "on the diagonal",
        { X: > 0, Y: > 0 } => "first quadrant",
        _ => "elsewhere"
    };

    static string Flat(Segment s) => s switch
    {
        { From: { X: var a }, To: { X: var b } } when a == b => "vertical",
        { From: { Y: var a }, To: { Y: var b } } when a == b => "horizontal",
        _ => "slanted"
    };

    static string Grade(int score) => score switch
    {
        >= 90 => "A",
        >= 80 and < 90 => "B",
        >= 70 and < 80 => "C",
        < 0 or > 100 => "invalid",
        _ => "F"
    };

    static string Vowel(char c)
    {
        switch (c)
        {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                return "vowel";
            case ' ':
                return "space";
            default:
                return char.IsLetter(c) ? "consonant" : "other";
        }
    }

    static string Command(string line)
    {
        switch (line)
        {
            case "quit":
            case "exit":
                return "bye";
            case string s when s.StartsWith("say "):
                return "saying " + s.Substring(4);
            case "":
                return "empty";
            case null:
                return "nothing";
            default:
                return "unknown " + line;
        }
    }

    static string Rate(Level level, int n)
    {
        return level switch
        {
            Level.Low => "low",
            Level.Mid when n > 5 => "mid, many",
            Level.Mid => "mid",
            _ => "high"
        };
    }

    static string Describe(object o)
    {
        if (o is null) return "null";
        if (o is Point { X: > 10 } big) return "big point " + big.X;
        if (o is Point p) return "point " + p.X + "," + p.Y;
        if (o is not string and not int) return "something else";
        if (o is int i && i is > 0 and < 10) return "digit " + i;
        return "other " + o;
    }

    public static void Main()
    {
        foreach (var p in new[] { new Point(0, 0), new Point(0, 4), new Point(3, 0), new Point(2, 2), new Point(1, 5), new Point(-1, 1) })
            Console.WriteLine(Where(p));
        Console.WriteLine(Flat(new Segment(new Point(1, 1), new Point(1, 9))));
        Console.WriteLine(Flat(new Segment(new Point(1, 2), new Point(5, 2))));
        Console.WriteLine(Flat(new Segment(new Point(1, 2), new Point(5, 3))));
        foreach (int s in new[] { 95, 85, 75, 10, -5, 101 }) Console.Write(Grade(s) + " ");
        Console.WriteLine();
        foreach (char c in "hi u!") Console.Write(Vowel(c) + ";");
        Console.WriteLine();
        foreach (string l in new[] { "quit", "say hello", "", "dance" }) Console.WriteLine(Command(l));
        Console.WriteLine(Command(null));
        Console.WriteLine(Rate(Level.Low, 1) + " " + Rate(Level.Mid, 9) + " " + Rate(Level.Mid, 2) + " " + Rate(Level.High, 0));
        Console.WriteLine(Describe(null));
        Console.WriteLine(Describe(new Point(50, 1)));
        Console.WriteLine(Describe(new Point(5, 1)));
        Console.WriteLine(Describe(3.5));
        Console.WriteLine(Describe(7));
        Console.WriteLine(Describe(70));
        Console.WriteLine(Describe("text"));
    }
}
