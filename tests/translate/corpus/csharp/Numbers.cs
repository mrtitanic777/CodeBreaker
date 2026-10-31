// Numbers: integer semantics a translation must keep -- wrapping overflow, checked overflow, shifts
// (arithmetic, logical, masked counts), unsigned arithmetic, byte and sbyte, division and remainder of
// negatives, char arithmetic, float-to-int conversion, long math, compound assignment narrowing.
using System;

namespace Corpus.Numbers
{
    public static class Program
    {
        static int Hash(string s)
        {
            int h = 17;
            foreach (char c in s)
                h = h * 31 + c;
            return h;
        }

        static uint Rotl(uint x, int n) => (x << n) | (x >> (32 - n));

        public static void Main()
        {
            int big = int.MaxValue;
            big += 1;
            Console.WriteLine("wrap " + big + " " + unchecked(int.MinValue - 1));
            try
            {
                int z = checked(int.MaxValue + Hash("x"));
                Console.WriteLine("no overflow " + z);
            }
            catch (OverflowException)
            {
                Console.WriteLine("checked overflow");
            }

            int neg = -20;
            Console.WriteLine("shifts " + (neg >> 2) + " " + (neg >>> 28) + " " + (1 << 33) + " " + (1L << 33));
            Console.WriteLine("div " + (-7 / 2) + " " + (-7 % 2) + " " + (7 % -2) + " " + (-7L / 2L));

            uint u = 0xFFFFFFF0u;
            u += 32;
            Console.WriteLine("unsigned " + u + " " + Rotl(0x80000001u, 4) + " " + (uint.MaxValue / 3u));
            ulong ul = ulong.MaxValue;
            Console.WriteLine("ulong " + ul + " " + (ul >> 60));

            byte b = 250;
            b += 10;
            int i200 = 200, i70000 = 70000, minus1 = -1;
            sbyte sb = (sbyte)i200;
            short sh = (short)i70000;
            Console.WriteLine("narrow " + b + " " + sb + " " + sh + " " + (byte)minus1);

            char c = 'A';
            c += (char)2;
            int code = c + 1;
            Console.WriteLine("char " + c + " " + code + " " + (char)(code + 1) + " " + (int)'z');

            double d = 3.99, huge = 2.5e9;
            Console.WriteLine("conv " + (int)d + " " + (int)(-d) + " " + (long)1e18 + " " + (int)huge);

            long fact = 1;
            for (int i = 1; i <= 20; i++)
                fact *= i;
            Console.WriteLine("fact20 " + fact + " hash " + Hash("translator"));

            int x = 7;
            x <<= 3;
            x ^= 0x55;
            x %= 13;
            Console.WriteLine("compound " + x + " " + (x++ + ++x) + " " + x);
            Console.WriteLine("bits " + (~0x0F & 0xFF) + " " + (0x0F | 0x30) + " " + (6 ^ 3));
        }
    }
}
