// Records and patterns: positional records (value equality, with-less use, ToString), type patterns,
// property patterns, relational and constant patterns, a switch expression, `is null`, nullable value
// types and the null-coalescing operator, `as`.
using System;

namespace Corpus.Records
{
    public abstract record Shape;
    public record Circle(double R) : Shape;
    public record Box(int W, int H) : Shape;
    public record Named(string Label, int Size);

    public static class Program
    {
        static string Classify(Shape s) => s switch
        {
            Circle c when c.R > 10 => "big circle",
            Circle c => "circle " + c.R,
            Box { W: 0 } => "flat box",
            Box b => "box " + (b.W * b.H),
            _ => "unknown",
        };

        static string Grade(int score) => score switch
        {
            >= 90 => "A",
            >= 80 and < 90 => "B",
            < 0 => "invalid",
            _ => "C",
        };

        static string Day(int d) => d switch
        {
            0 => "sun",
            6 => "sat",
            _ => "weekday",
        };

        static int? Find(int[] xs, int want)
        {
            for (int i = 0; i < xs.Length; i++)
                if (xs[i] == want)
                    return i;
            return null;
        }

        public static void Main()
        {
            Shape[] shapes = { new Circle(2.5), new Circle(11), new Box(0, 5), new Box(3, 4) };
            foreach (var s in shapes)
                Console.WriteLine(Classify(s));

            Console.WriteLine(Grade(95) + Grade(85) + Grade(-1) + Grade(10));
            Console.WriteLine(Day(0) + " " + Day(3) + " " + Day(6));

            var a = new Named("x", 1);
            var b = new Named("x", 1);
            Console.WriteLine("equal " + (a == b) + " " + a.Equals(b) + " " + a);

            object o = "hello";
            if (o is string str && str.Length > 3)
                Console.WriteLine("long string " + str);
            string? maybe = null;
            Console.WriteLine("null? " + (maybe is null) + " len " + (maybe?.Length ?? -1));
            maybe = "abc";
            Console.WriteLine("len " + (maybe?.Length ?? -1));

            int[] xs = { 5, 8, 13 };
            int? at = Find(xs, 8);
            int? none = Find(xs, 4);
            Console.WriteLine("found " + (at ?? -1) + " missing " + (none ?? -1) + " has " + at.HasValue + " " + none.HasValue);

            object n = 42;
            string? asText = n as string;
            Console.WriteLine("as " + (asText == null ? "null" : asText));
        }
    }
}
