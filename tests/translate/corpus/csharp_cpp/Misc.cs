// Misc: abstract classes and interpolation, a generic class with a constraint and an indexer, out variables, long arithmetic, continue/break,
// a type pattern, a failed cast, IndexOutOfRangeException.
using System;
using System.Collections.Generic;

public abstract class Animal
{
    public string Name { get; }
    protected Animal(string name) { Name = name; }
    public abstract string Sound();
    public override string ToString() => $"{Name} says {Sound()}";
}
public class Dog : Animal { public Dog(string n) : base(n) { } public override string Sound() => "woof"; }
public class Cat : Animal { public Cat(string n) : base(n) { } public override string Sound() => "meow"; }

public class Counter<T> where T : notnull
{
    private readonly Dictionary<T, int> counts = new Dictionary<T, int>();
    public void Add(T item)
    {
        if (counts.TryGetValue(item, out int n)) counts[item] = n + 1; else counts[item] = 1;
    }
    public int this[T item] => counts.TryGetValue(item, out int n) ? n : 0;
    public int Total { get { int t = 0; foreach (var kv in counts) t += kv.Value; return t; } }
}

public static class Program
{
    static long Fact(int n) => n <= 1 ? 1 : n * Fact(n - 1);
    public static void Main()
    {
        var zoo = new List<Animal> { new Dog("Rex"), new Cat("Tom") };
        foreach (var a in zoo) Console.WriteLine(a);
        var c = new Counter<string>();
        foreach (var w in new[] { "a", "b", "a", "c", "a" }) c.Add(w);
        Console.WriteLine(c["a"] + " " + c["z"] + " " + c.Total);
        Console.WriteLine(Fact(15) + " " + Fact(20));
        int x = 0;
        for (int i = 0; i < 10; i++) { if (i % 3 == 0) continue; if (i > 7) break; x += i; }
        Console.WriteLine(x);
        object o = 3.5;
        Console.WriteLine(o is double d ? "double " + d : "other");
        try { object s = "x"; int bad = (int)s; }
        catch (InvalidCastException) { Console.WriteLine("bad cast"); }
        int[] arr = new int[3];
        try { arr[3] = 1; } catch (IndexOutOfRangeException e) { Console.WriteLine(e.Message); }
    }
}
