// A test input of the .NET lane (src/dotnet): one assembly that has a row in most metadata tables. Built by
// testdata/build.cmd into Sample.dll (AnyCPU, PE32) and Sample64.dll (x64, PE32+); the .dll files are checked in.
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

[assembly: CLSCompliant(false)]
[module: Sample.Marker("module")]

namespace Sample
{
    [AttributeUsage(AttributeTargets.All, AllowMultiple = true)]
    public sealed class MarkerAttribute : Attribute
    {
        public MarkerAttribute(string text) { Text = text; }
        public MarkerAttribute(int number, Type type, params object[] rest) { Number = number; Kind = type; Rest = rest; }
        public string Text { get; }
        public int Number { get; }
        public Type Kind { get; }
        public object[] Rest { get; }
        public double Weight;
    }

    public enum Colour : byte { Red = 1, Green = 2, Blue = 4 }

    [Flags]
    public enum Access : long { None = 0, Read = 1, Write = 2, All = Read | Write }

    public interface IShape
    {
        double Area { get; }
        string Describe(int detail);
        event EventHandler Changed;
    }

    [StructLayout(LayoutKind.Explicit, Size = 16, Pack = 4)]
    public struct Overlay
    {
        [FieldOffset(0)] public int Low;
        [FieldOffset(4)] public int High;
        [FieldOffset(0)] public long Both;
        [FieldOffset(8), MarshalAs(UnmanagedType.R8)] public double Real;
    }

    [Marker(7, typeof(List<int>), "a", 2, Colour.Green, null, Weight = 1.5)]
    public class Circle : IShape, IComparable<Circle>
    {
        public const string Unit = "cm";
        public const double Pi = 3.14159265358979;
        public static readonly int[] Primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
        private double radius;
        public event EventHandler Changed;

        public Circle(double radius) { this.radius = radius; }

        public double Area => Pi * radius * radius;

        public double Radius
        {
            get => radius;
            set { radius = value; Changed?.Invoke(this, EventArgs.Empty); }
        }

        public string Describe(int detail) => detail > 0 ? $"circle r={radius}{Unit}" : "circle";

        public int CompareTo(Circle other) => other == null ? 1 : radius.CompareTo(other.radius);

        public int this[int index] => Primes[index % Primes.Length];

        public static Circle operator +(Circle a, Circle b) => new Circle(a.radius + b.radius);
    }

    public class Box<T> where T : class, IShape, new()
    {
        public T Item;
        public List<KeyValuePair<string, T[]>> History = new();

        public TResult Map<TResult>(Func<T, TResult> f) where TResult : struct => f(Item);

        public class Nested<U> { public Dictionary<T, U> Table; }
    }

    public static class Native
    {
        [DllImport("kernel32.dll", EntryPoint = "GetTickCount", SetLastError = true)]
        public static extern uint Ticks();

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        public static extern int MessageBoxW(IntPtr hwnd, [MarshalAs(UnmanagedType.LPWStr)] string text, string caption, uint type);
    }

    public static class Code
    {
        public static int Loops(int n, ref int total, out string note, in long limit)
        {
            int sum = 0;
            for (int i = 0; i < n; i++)
            {
                switch (i % 5)
                {
                    case 0: sum += i; break;
                    case 1: sum -= 1; break;
                    case 2: sum ^= i << 2; break;
                    case 3: sum *= 3; break;
                    default: sum = (int)(sum & limit); break;
                }
            }
            total += sum;
            note = "sum: " + sum;
            return sum;
        }

        public static string Guarded(object o)
        {
            try
            {
                try { return ((IShape)o).Describe(1); }
                catch (InvalidCastException e) when (e.Message.Length > 3) { return "cast"; }
                finally { Console.WriteLine("inner"); }
            }
            catch (Exception) { return "other"; }
        }

        public static unsafe int Pointer(int* p, int[] array)
        {
            fixed (int* q = array) { return *p + *q + sizeof(Overlay); }
        }

        public static async Task<int> Later(int x)
        {
            await Task.Delay(1);
            return x * 2;
        }

        public static IEnumerable<int> Count(int to)
        {
            for (int i = 0; i < to; i++) yield return i;
        }

        public static T Pick<T>(T a, T b, bool first) => first ? a : b;

        public static int UsePick() => Pick<int>(1, 2, true) + Pick("a", "b", false).Length;

        public static long Big() => 0x123456789ABCDEFL + (long)'x' + (long)2.5f;

        public static Func<int, int> Closure(int k) => x => x + k;
    }
}
