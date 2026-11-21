// Async: tasks wait on timers and on each other, so what they print has one order however busy the machine is (timers alone do not
// keep their order under load); an exception thrown in an async method is the awaiting method's;
// a method that finishes at once does not suspend its caller; Task.Run and WhenAll.
using System;
using System.Threading.Tasks;

public class Worker
{
    public string Name;
    public int Done;
    public Worker(string name) { Name = name; }

    public async Task<int> Work(int ms, int result, Task before)
    {
        Console.WriteLine(Name + " begins " + result);
        await before;
        await Task.Delay(ms);
        Done++;
        Console.WriteLine(Name + " finishes " + result);
        return result;
    }
}

public static class Tasks
{
    static async Task<int> Immediate(int x) { return x + 1; }

    static async Task<int> Chain(int n)
    {
        if (n == 0) return 0;
        int below = await Chain(n - 1);
        await Task.Yield();
        return below + n;
    }

    static async Task Thrower(int when)
    {
        await Task.Delay(when);
        throw new ArgumentException("late " + when);
    }

    static async Task<string> Guarded()
    {
        try
        {
            await Thrower(5);
            return "not reached";
        }
        catch (ArgumentException e)
        {
            return "recovered from " + e.Message;
        }
        finally
        {
            Console.WriteLine("guard finally");
        }
    }

    static async Task Main()
    {
        var w = new Worker("w");
        var fast = w.Work(10, 2, Task.CompletedTask);
        var middle = w.Work(10, 3, fast);
        var slow = w.Work(10, 1, middle);
        Console.WriteLine("started three");
        int last = await slow;
        Console.WriteLine("fast is " + await fast);
        Console.WriteLine("slow is " + last);
        Console.WriteLine("middle is " + await middle);
        Console.WriteLine("done count " + w.Done);

        Console.WriteLine(await Immediate(41));
        Console.WriteLine(await Chain(5));
        Console.WriteLine(await Guarded());

        var first = w.Work(5, 11, Task.CompletedTask);
        var second = w.Work(5, 12, first);
        var third = w.Work(5, 10, second);
        int[] all = await Task.WhenAll(third, first, second);
        Console.WriteLine(all[0] + " " + all[1] + " " + all[2]);

        int viaRun = await Task.Run(() => 6 * 7);
        Console.WriteLine("run " + viaRun);

        try { await Thrower(1); }
        catch (Exception e) { Console.WriteLine("main caught " + e.Message); }
        Console.WriteLine(Task.FromResult(9).Result);
    }
}
