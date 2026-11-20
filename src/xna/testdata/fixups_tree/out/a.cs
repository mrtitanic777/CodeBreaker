(white).A = 64;
class A : B {
    public A(int x) : base(x)
    {
        Vector2 v = default;
        v = new Vector2(1f, 2f);
    }
}
internal class X
{
    internal struct Blob16 { }
    internal static Blob16 A/* Not supported: data(00) */;
}
