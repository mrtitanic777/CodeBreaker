// Sieve: arrays (one-dimensional, jagged, rectangular), recursion, while/do/for loops, break, continue
// and goto, a static constructor, a nested class, constants, a top-down merge sort, Fibonacci with a
// memo table, Pascal's triangle, matrix multiplication.
using System;

namespace Corpus.Sieve
{
    public static class Program
    {
        const int Limit = 60;
        static readonly int[] memo;
        static int calls;

        static Program()
        {
            memo = new int[50];
        }

        class Counter
        {
            public int Hits;
            public void Hit() { Hits++; }
        }

        static int Fib(int n)
        {
            calls++;
            if (n < 2)
                return n;
            if (memo[n] != 0)
                return memo[n];
            return memo[n] = Fib(n - 1) + Fib(n - 2);
        }

        static void MergeSort(int[] a, int[] tmp, int lo, int hi)
        {
            if (hi - lo < 2)
                return;
            int mid = (lo + hi) / 2;
            MergeSort(a, tmp, lo, mid);
            MergeSort(a, tmp, mid, hi);
            int i = lo, j = mid, k = lo;
            while (i < mid && j < hi)
                tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
            while (i < mid)
                tmp[k++] = a[i++];
            while (j < hi)
                tmp[k++] = a[j++];
            for (k = lo; k < hi; k++)
                a[k] = tmp[k];
        }

        public static void Main()
        {
            bool[] composite = new bool[Limit + 1];
            int count = 0;
            for (int p = 2; p <= Limit; p++)
            {
                if (composite[p])
                    continue;
                count++;
                for (int q = p * p; q <= Limit; q += p)
                    composite[q] = true;
            }
            Console.WriteLine("primes up to " + Limit + ": " + count);

            Console.WriteLine("fib 40 = " + Fib(40) + " in " + calls + " calls");

            int[] data = { 9, 3, 7, 1, 8, 2, 6, 5, 4, 0 };
            MergeSort(data, new int[data.Length], 0, data.Length);
            Console.WriteLine("sorted " + string.Join(" ", data));

            int[][] pascal = new int[8][];
            for (int r = 0; r < pascal.Length; r++)
            {
                pascal[r] = new int[r + 1];
                pascal[r][0] = pascal[r][r] = 1;
                for (int c = 1; c < r; c++)
                    pascal[r][c] = pascal[r - 1][c - 1] + pascal[r - 1][c];
            }
            Console.WriteLine("pascal row 7: " + string.Join(" ", pascal[7]));

            int[,] m = new int[3, 3];
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++)
                    m[r, c] = r * 3 + c;
            int[,] sq = new int[3, 3];
            for (int r = 0; r < 3; r++)
                for (int c = 0; c < 3; c++)
                {
                    int sum = 0;
                    for (int k = 0; k < 3; k++)
                        sum += m[r, k] * m[k, c];
                    sq[r, c] = sum;
                }
            Console.WriteLine("m^2 diag " + sq[0, 0] + " " + sq[1, 1] + " " + sq[2, 2] + " rank " + sq.Rank);

            var counter = new Counter();
            int n = 27;
            do
            {
                n = n % 2 == 0 ? n / 2 : 3 * n + 1;
                counter.Hit();
                if (counter.Hits > 1000)
                    goto done;
            } while (n != 1);
            Console.WriteLine("collatz 27 steps " + counter.Hits);
        done:
            int found = -1;
            for (int i = 0; i < 100; i++)
            {
                if (i * i > 500)
                {
                    found = i;
                    break;
                }
            }
            Console.WriteLine("first square over 500: " + found);
        }
    }
}
