// Linq: method syntax (Where, Select, OrderBy, Sum, Count, Any, First) and query syntax over a list of
// objects, an object initializer and a collection initializer.
using System;
using System.Collections.Generic;
using System.Linq;

namespace Corpus.Linq
{
    public class Person
    {
        public string Name { get; set; } = "";
        public int Age { get; set; }
        public string City { get; set; } = "";
    }

    public static class Program
    {
        public static void Main()
        {
            var people = new List<Person>
            {
                new Person { Name = "Ann", Age = 34, City = "Oslo" },
                new Person { Name = "Bob", Age = 19, City = "Rome" },
                new Person { Name = "Cid", Age = 52, City = "Oslo" },
                new Person { Name = "Dee", Age = 27, City = "Lima" },
            };

            var adults = people.Where(p => p.Age >= 21).Select(p => p.Name).ToList();
            Console.WriteLine("adults: " + string.Join(" ", adults));

            int total = people.Sum(p => p.Age);
            Console.WriteLine("total age " + total + ", count in Oslo " + people.Count(p => p.City == "Oslo"));

            var byAge = people.OrderBy(p => p.Age).Select(p => p.Name + ":" + p.Age);
            Console.WriteLine(string.Join(", ", byAge));

            var query = from p in people
                        where p.Name.Length == 3 && p.Age < 40
                        orderby p.Name descending
                        select p.Name.ToUpper();
            Console.WriteLine("query: " + string.Join("|", query));

            Console.WriteLine("any over 50: " + people.Any(p => p.Age > 50) + ", first in Lima: " + people.First(p => p.City == "Lima").Name);

            int[] squares = Enumerable.Range(1, 6).Select(i => i * i).Where(i => i % 2 == 0).ToArray();
            Console.WriteLine("even squares: " + string.Join(",", squares));
        }
    }
}
