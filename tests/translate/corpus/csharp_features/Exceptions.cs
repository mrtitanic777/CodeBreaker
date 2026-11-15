using System;

public class AppException : Exception
{
    public int Code;
    public AppException(string m, int code) : base(m) { Code = code; }
}

public static class Exceptions
{
    static int Risky(int n)
    {
        try
        {
            Console.WriteLine("enter " + n);
            if (n == 0) throw new AppException("zero", 7);
            if (n < 0) throw new InvalidOperationException("neg");
            return 100 / (n - 1);
        }
        finally
        {
            Console.WriteLine("leave " + n);
        }
    }

    static int Early()
    {
        try { return 1; }
        finally { Console.WriteLine("early finally"); }
    }

    static void Rethrow()
    {
        try { throw new Exception("inner"); }
        catch (Exception e) { Console.WriteLine("caught " + e.Message); throw; }
    }

    public static void Main()
    {
        foreach (int n in new[] { 2, 0, -1, 1 })
        {
            try { Console.WriteLine(Risky(n)); }
            catch (AppException e) { Console.WriteLine("app " + e.Message + " " + e.Code); }
            catch (InvalidOperationException e) { Console.WriteLine("inv " + e.Message); }
            catch (DivideByZeroException) { Console.WriteLine("div0"); }
            finally { Console.WriteLine("done " + n); }
        }
        Console.WriteLine(Early());
        try { Rethrow(); } catch (Exception e) { Console.WriteLine("outer " + e.Message); }
        for (int i = 0; i < 3; i++)
        {
            try { if (i == 1) continue; Console.WriteLine("body " + i); }
            finally { Console.WriteLine("f" + i); }
        }
        try { try { throw new Exception("a"); } finally { Console.WriteLine("inner finally"); } }
        catch (Exception e) { Console.WriteLine(e.Message); }
    }
}
