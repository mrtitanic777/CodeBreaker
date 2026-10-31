// Inventory: top-level statements, interfaces with a default member, an abstract generic repository,
// operator overloading, an implicit conversion, a static extension method, ref and out parameters,
// a property with a validating setter, lock, a generic interface constraint.
using System;
using System.Collections.Generic;

var store = new Store();
store.Add(new Item("bolt", 25, new Price(15)));
store.Add(new Item("nut", 100, new Price(5)));
store.Add(new Item("gear", 3, new Price(1250)));
Console.WriteLine("value " + store.TotalValue());
Price p = new Price(199) + new Price(1);
int cents = p;
Console.WriteLine("price sum " + p.Cents + " as int " + cents + " cheaper " + (new Price(3) < new Price(4)));
int q = 10;
store.Take("bolt", ref q);
Console.WriteLine("took " + q + " left " + store.Count("bolt"));
if (store.TryFind("gear", out Item? g))
    Console.WriteLine("found " + g.Name.Shout() + " x" + g.Quantity);
try
{
    g!.Quantity = -1;
}
catch (ArgumentException e)
{
    Console.WriteLine("rejected: " + e.Message);
}
ILabel l = g!;
Console.WriteLine(l.Label() + " / " + l.Short());

public interface ILabel
{
    string Label();
    string Short() => Label().Substring(0, 3);
}

public readonly struct Price
{
    public int Cents { get; }
    public Price(int cents) { Cents = cents; }
    public static Price operator +(Price a, Price b) => new Price(a.Cents + b.Cents);
    public static bool operator <(Price a, Price b) => a.Cents < b.Cents;
    public static bool operator >(Price a, Price b) => a.Cents > b.Cents;
    public static implicit operator int(Price p) => p.Cents;
}

public class Item : ILabel
{
    private int quantity;
    public string Name { get; }
    public Price Unit { get; }

    public int Quantity
    {
        get => quantity;
        set
        {
            if (value < 0)
                throw new ArgumentException("negative quantity for " + Name);
            quantity = value;
        }
    }

    public Item(string name, int quantity, Price unit)
    {
        Name = name;
        Quantity = quantity;
        Unit = unit;
    }

    public string Label() => Name + " x" + quantity;
}

public abstract class Repository<T> where T : class, ILabel
{
    protected readonly List<T> items = new List<T>();
    private readonly object gate = new object();

    public void Add(T item)
    {
        lock (gate)
        {
            items.Add(item);
        }
    }

    public abstract string KeyOf(T item);

    public bool TryFind(string key, out T? found)
    {
        foreach (var x in items)
        {
            if (KeyOf(x) == key)
            {
                found = x;
                return true;
            }
        }
        found = null;
        return false;
    }
}

public class Store : Repository<Item>
{
    public override string KeyOf(Item item) => item.Name;

    public int TotalValue()
    {
        int sum = 0;
        foreach (var i in items)
            sum += i.Quantity * i.Unit;
        return sum;
    }

    public int Count(string name)
    {
        return TryFind(name, out var it) ? it!.Quantity : 0;
    }

    public void Take(string name, ref int howMany)
    {
        if (TryFind(name, out var it))
        {
            int n = Math.Min(howMany, it!.Quantity);
            it.Quantity -= n;
            howMany = n;
        }
    }
}

public static class TextExtensions
{
    public static string Shout(this string s) => s.ToUpperInvariant() + "!";
}
