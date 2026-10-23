// XboxCompat.cs -- small things an Xbox 360 XNA game calls that a PC has no use for, or that the decompiler writes in
// a form C# does not take.
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace System.Threading
{
    // Thread.SetProcessorAffinity: the Xbox 360's hardware-thread pinning (cores 1, 3, 4, 5); the PC schedules threads
    // itself.
    public static class XboxThreadExtensions
    {
        public static void SetProcessorAffinity(this Thread thread, params int[] hardwareThreads) { }
    }
}

namespace Microsoft.Xna.Framework
{
    // A value type constructed in place (IL: ldloca v; call T::.ctor) can come out of ILSpy as `v._002Ector(args)`
    // (_002E is '.'); the original was `v = new T(args)`. xna/fixups.py rewrites the ones it can place; these cover
    // the XNA types it could not.
    public static class CtorArtifactExtensions
    {
        public static void _002Ector(this ref Vector2 v, float x, float y) => v = new Vector2(x, y);
        public static void _002Ector(this ref Vector3 v, float x, float y, float z) => v = new Vector3(x, y, z);
        public static void _002Ector(this ref Vector3 v, Vector2 xy, float z) => v = new Vector3(xy, z);
        public static void _002Ector(this ref Vector4 v, float x, float y, float z, float w) => v = new Vector4(x, y, z, w);
        public static void _002Ector(this ref Color c, int r, int g, int b) => c = new Color(r, g, b);
        public static void _002Ector(this ref Color c, int r, int g, int b, int a) => c = new Color(r, g, b, a);
        public static void _002Ector(this ref Color c, float r, float g, float b) => c = new Color(r, g, b);
        public static void _002Ector(this ref Color c, float r, float g, float b, float a) => c = new Color(r, g, b, a);
        public static void _002Ector(this ref Color c, Vector3 rgb) => c = new Color(rgb);
        public static void _002Ector(this ref Color c, Vector4 rgba) => c = new Color(rgba);
        public static void _002Ector(this ref Point p, int x, int y) => p = new Point(x, y);
        public static void _002Ector(this ref Quaternion q, Vector3 v, float w) => q = new Quaternion(v, w);
        public static void _002Ector(this ref Quaternion q, float x, float y, float z, float w) => q = new Quaternion(x, y, z, w);
        public static void _002Ector(this ref Rectangle rc, int x, int y, int w, int h) => rc = new Rectangle(x, y, w, h);
        public static void _002Ector(this ref Ray r, Vector3 position, Vector3 direction) => r = new Ray(position, direction);
        public static void _002Ector(this ref BoundingBox b, Vector3 min, Vector3 max) => b = new BoundingBox(min, max);
        public static void _002Ector(this ref BoundingSphere b, Vector3 center, float radius) => b = new BoundingSphere(center, radius);
        public static void _002Ector(this ref Viewport v, int x, int y, int w, int h) => v = new Viewport(x, y, w, h);
    }
}
