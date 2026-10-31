// Text: StringBuilder, string methods (Substring, IndexOf, Replace, Split, ToUpper, Trim, Contains,
// StartsWith), char tests, string comparison by value, a word-frequency count, a Caesar cipher, a
// palindrome test, string.Format with indexes.
using System;
using System.Collections.Generic;
using System.Text;

namespace Corpus.Text
{
    public static class Program
    {
        static string Caesar(string s, int k)
        {
            var sb = new StringBuilder();
            foreach (char ch in s)
            {
                if (char.IsLetter(ch) && ch <= 'z')
                {
                    char a = char.IsUpper(ch) ? 'A' : 'a';
                    sb.Append((char)(a + (ch - a + k + 26) % 26));
                }
                else
                {
                    sb.Append(ch);
                }
            }
            return sb.ToString();
        }

        static bool Palindrome(string s)
        {
            int i = 0, j = s.Length - 1;
            while (i < j)
            {
                if (s[i] != s[j])
                    return false;
                i++;
                j--;
            }
            return true;
        }

        public static void Main()
        {
            string text = "  The quick brown fox jumps over the lazy dog  ";
            string t = text.Trim();
            Console.WriteLine("[" + t + "] " + t.Length);
            Console.WriteLine(t.Substring(4, 5) + "|" + t.IndexOf("fox") + "|" + t.Replace("dog", "cat") + "|" + t.ToUpper());
            Console.WriteLine(t.Contains("jumps") + " " + t.StartsWith("The") + " " + t.EndsWith("dog"));

            string[] words = t.Split(' ');
            var freq = new Dictionary<char, int>();
            foreach (string w in words)
            {
                char first = char.ToLower(w[0]);
                if (freq.ContainsKey(first))
                    freq[first]++;
                else
                    freq[first] = 1;
            }
            Console.WriteLine("words " + words.Length + " t-words " + freq['t'] + " o-words " + freq['o']);

            string enc = Caesar("Hello, World!", 3);
            Console.WriteLine(enc + " -> " + Caesar(enc, -3));
            Console.WriteLine("racecar " + Palindrome("racecar") + ", rocket " + Palindrome("rocket"));

            string a = "ab";
            string b = "a" + "b".ToString();
            Console.WriteLine("same " + (a == b) + " cmp " + string.CompareOrdinal("apple", "apricot"));

            var sb = new StringBuilder("x");
            for (int i = 0; i < 5; i++)
                sb.Append(i).Append(',');
            sb.Length = sb.Length - 1;
            sb.Insert(0, "<").Append('>');
            Console.WriteLine(sb.ToString() + " len " + sb.Length);
            Console.WriteLine(string.Format("{1}-{0}-{1}", "a", 7));
            int digits = 0;
            foreach (char ch in "a1b22c333")
                if (char.IsDigit(ch))
                    digits++;
            Console.WriteLine("digits " + digits);
        }
    }
}
