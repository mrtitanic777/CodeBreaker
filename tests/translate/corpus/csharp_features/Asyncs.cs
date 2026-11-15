using System;
using System.Threading.Tasks;

public static class Asyncs
{
    static async Task<int> Double(int x)
    {
        await Task.Yield();
        return x * 2;
    }

    static async Task<int> Sum(int n)
    {
        int s = 0;
        for (int i = 1; i <= n; i++) s += await Double(i);
        return s;
    }

    static async Task Greet(string who)
    {
        Console.WriteLine("hello " + who);
        await Task.Delay(1);
        Console.WriteLine("bye " + who);
    }

    static async Task<string> Fail()
    {
        await Task.Delay(1);
        throw new InvalidOperationException("boom");
    }

    static async Task Run()
    {
        Console.WriteLine(await Sum(4));
        await Greet("ann");
        try { await Fail(); } catch (InvalidOperationException e) { Console.WriteLine("caught " + e.Message); }
        var t1 = Double(5); var t2 = Double(6);
        Console.WriteLine(await t1 + await t2);
        int[] r = await Task.WhenAll(Double(1), Double(2), Double(3));
        Console.WriteLine(r[0] + r[1] + r[2]);
    }

    public static void Main()
    {
        Run().GetAwaiter().GetResult();
        Console.WriteLine(Sum(3).Result);
    }
}
