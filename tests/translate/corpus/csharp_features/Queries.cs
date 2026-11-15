using System;
using System.Collections.Generic;
using System.Linq;

public class Person
{
    public string Name; public int Age; public string City;
    public Person(string n, int a, string c) { Name = n; Age = a; City = c; }
}

public static class Queries
{
    public static void Main()
    {
        var people = new List<Person> {
            new Person("Ann", 31, "Oslo"), new Person("Bob", 25, "Rome"),
            new Person("Cid", 40, "Oslo"), new Person("Dee", 19, "Rome"), new Person("Eve", 52, "Paris") };
        var adults = from p in people where p.Age >= 30 orderby p.Name descending select p.Name;
        foreach (var n in adults) Console.WriteLine(n);
        var q = from p in people group p by p.City into g orderby g.Key select g.Key + ":" + g.Count();
        foreach (var s in q) Console.WriteLine(s);
        var total = (from p in people select p.Age).Sum();
        Console.WriteLine(total);
        var pairs = from a in new[] { 1, 2, 3 } from b in new[] { 10, 20 } where a * b > 15 select a * b;
        Console.WriteLine(string.Join(",", pairs));
        var letted = from p in people let dec = p.Age / 10 where dec >= 3 select p.Name + dec;
        Console.WriteLine(string.Join(",", letted));
        var joined = from p in people join c in new[] { "Oslo", "Paris" } on p.City equals c select p.Name + "@" + c;
        Console.WriteLine(string.Join(",", joined));
        Console.WriteLine(people.Where(p => p.City == "Rome").Select(p => p.Age).Max());
        Console.WriteLine(people.Any(p => p.Age > 50));
        Console.WriteLine(string.Join(",", people.OrderBy(p => p.Age).Select(p => p.Name).Take(3)));
    }
}
