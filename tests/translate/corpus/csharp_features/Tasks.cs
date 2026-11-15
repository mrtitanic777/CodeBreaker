// Async: tasks that wait on timers wake in the order of their time; an exception thrown in an async method is the awaiting method's;
// a method that finishes at once does not suspend its caller; Task.Run and WhenAll.
using System;
using System.Threading.Tasks;

public class Worker
{
    public string Name;
    public int Done;
    public Worker(string name) { Name = name; }

    public async Task<int> Work(int ms, int result)
    {
        Console.WriteLine(Name + " begins " + result);
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
        var slow = w.Work(300, 1);
        var fast = w.Work(100, 2);
        var middle = w.Work(200, 3);
        Console.WriteLine("started three");
        Console.WriteLine("fast is " + await fast);
        Console.WriteLine("slow is " + await slow);
        Console.WriteLine("middle is " + await middle);
        Console.WriteLine("done count " + w.Done);

        Console.WriteLine(await Immediate(41));
        Console.WriteLine(await Chain(5));
        Console.WriteLine(await Guarded());

        int[] all = await Task.WhenAll(w.Work(60, 10), w.Work(20, 11), w.Work(40, 12));
        Console.WriteLine(all[0] + " " + all[1] + " " + all[2]);

        int viaRun = await Task.Run(() => 6 * 7);
        Console.WriteLine("run " + viaRun);

        try { await Thrower(1); }
        catch (Exception e) { Console.WriteLine("main caught " + e.Message); }
        Console.WriteLine(Task.FromResult(9).Result);
    }
}
