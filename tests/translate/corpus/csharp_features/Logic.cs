// Parenthesised conditions: the source's grouping of && and || (and of patterns that lower to them) is kept in the C++.
using System;

public static class Logic
{
    static bool Both(bool a, bool b, bool c) => (a || b) && c;
    static bool Either(bool a, bool b, bool c) => a || (b && c);
    static bool Flat(bool a, bool b, bool c) => a || b && c;
    static bool Negated(bool a, bool b, bool c) => !(a && b) || c;
    static string Pick(int n) => n > 0 ? (n > 10 ? "big" : "small") : (n < -10 ? "very negative" : "negative");

    public static void Main()
    {
        bool[] v = { false, true };
        foreach (bool a in v)
            foreach (bool b in v)
                foreach (bool c in v)
                    Console.WriteLine($"{a} {b} {c}: {Both(a, b, c)} {Either(a, b, c)} {Flat(a, b, c)} {Negated(a, b, c)}");
        foreach (int n in new[] { 50, 5, -5, -50, 0 }) Console.WriteLine(Pick(n));
        int x = 6;
        Console.WriteLine((x > 3 || x < 0) && x % 2 == 0);
        Console.WriteLine(x > 3 || (x < 0 && x % 2 == 1));
        Console.WriteLine(((x & 2) != 0) == ((x | 1) > 6));
    }
}
