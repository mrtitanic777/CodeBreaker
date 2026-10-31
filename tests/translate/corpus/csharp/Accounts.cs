// Accounts: the part of C# every static, garbage-collected target shares -- classes and inheritance,
// abstract and overridden members, auto and computed properties, static state, a custom exception,
// try/catch/finally, List<T>, foreach, switch on ints and strings, string methods, char arithmetic,
// integer wrapping, booleans as text.
using System;
using System.Collections.Generic;

namespace Corpus.Accounts
{
    public class LimitExceeded : Exception
    {
        public LimitExceeded(string message) : base(message)
        {
        }
    }

    public abstract class Account
    {
        private static int opened;
        public string Owner { get; private set; }
        public int Balance { get; protected set; }
        public int Number { get; }

        protected Account(string owner, int balance)
        {
            opened++;
            Number = 1000 + opened;
            Owner = owner;
            Balance = balance;
        }

        public static int Opened => opened;
        public abstract int Limit { get; }
        public virtual string Kind => "account";
        public bool Overdrawn => Balance < 0;

        public void Withdraw(int amount)
        {
            if (Balance - amount < -Limit)
                throw new LimitExceeded(Owner + " cannot withdraw " + amount);
            Balance -= amount;
        }

        public void Deposit(int amount)
        {
            Balance += amount;
        }

        public override string ToString()
        {
            return Kind + " " + Number + " " + Owner + " " + Balance;
        }
    }

    public class Checking : Account
    {
        public Checking(string owner, int balance) : base(owner, balance)
        {
        }

        public override int Limit => 100;
        public override string Kind => "checking";
    }

    public class Savings : Account
    {
        public int Rate { get; set; }

        public Savings(string owner, int balance, int rate) : base(owner, balance)
        {
            Rate = rate;
        }

        public override int Limit => 0;

        public void AddInterest()
        {
            Deposit(Balance * Rate / 100);
        }
    }

    public static class Program
    {
        static int Score(string command)
        {
            switch (command)
            {
                case "deposit":
                    return 1;
                case "withdraw":
                    return 2;
                default:
                    return 0;
            }
        }

        static string Grade(int n)
        {
            switch (n / 10)
            {
                case 10:
                case 9:
                    return "A";
                case 8:
                    return "B";
                default:
                    return "C";
            }
        }

        public static void Main()
        {
            List<Account> accounts = new List<Account>();
            accounts.Add(new Checking("ann", 50));
            accounts.Add(new Savings("bob", 1000, 5));
            accounts.Add(new Checking("cid", 0));
            int failures = 0;
            for (int i = 0; i < accounts.Count; i++)
            {
                Account a = accounts[i];
                try
                {
                    a.Withdraw(120);
                    Console.WriteLine("withdrew from " + a.Owner + ", overdrawn " + a.Overdrawn);
                }
                catch (LimitExceeded e)
                {
                    failures++;
                    Console.WriteLine("refused: " + e.Message);
                }
                finally
                {
                    Console.WriteLine(a.ToString());
                }
            }
            Savings s = (Savings)accounts[1];
            s.Rate = 10;
            s.AddInterest();
            Console.WriteLine("after interest " + s.Balance + " rate " + s.Rate);
            int total = 0;
            foreach (Account a in accounts)
                total += a.Balance;
            Console.WriteLine("total " + total + " opened " + Account.Opened + " failures " + failures);
            Console.WriteLine("scores " + Score("deposit") + Score("withdraw") + Score("audit") + " grades " + Grade(95) + Grade(81) + Grade(42));
            string name = "Translator";
            Console.WriteLine(name.ToUpper() + " " + name.Substring(2, 4) + " " + name.Length + " " + name.Contains("late") + " " + (name == "Translator"));
            char c = 'a';
            int code = c + 3;
            Console.WriteLine("char " + (char)code + " " + code);
            int big = 2147483647;
            big = big + 1;
            Console.WriteLine("wrap " + big + " " + (big - 1) + " " + (7 / 2) + " " + (-7 % 3));
        }
    }
}
