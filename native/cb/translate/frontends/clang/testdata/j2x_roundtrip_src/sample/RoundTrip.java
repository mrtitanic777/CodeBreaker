package sample;

// The round trip's driver (written for this corpus): calls the Java lane's fixture classes
// (tests/java/fixtures/java: Vec3D, MathHelper, AxisAlignedBB) and the C++03 back end's test classes
// (tests/translate/test_cpp03_bodies.py: Base, Sub) and prints what they return, so the original and
// the Java -> C++ -> Java translation can be compared by their output.
public class RoundTrip {
	static int fib(int n) {
		int a = 0;
		int b = 1;
		for (int i = 0; i < n; ++i) {
			int t = a + b;
			a = b;
			b = t;
		}
		return a;
	}

	static long mix(long x, int y) {
		long v = x ^ (long)y << 3;
		while (v > 1000L) {
			v >>>= 2;
		}
		return v >>> 1;
	}

	public static void main(String[] args) {
		int total = 0;
		for (int i = 0; i < 12; ++i) {
			total += fib(i);
			System.out.println("fib " + i + " = " + fib(i) + " mix " + mix(123456789L * (long)i, i));
		}
		System.out.println("total " + total);
		Vec3D a = new Vec3D(1.0D, 2.0D, 3.0D);
		Vec3D b = new Vec3D(4.5D, -1.0D, 0.25D);
		Vec3D d = a.subtract(b);
		System.out.println("subtract " + d.xCoord + " " + d.yCoord + " " + d.zCoord);
		Vec3D m = a.getIntermediateWithXValue(b, 2.0D);
		System.out.println("intermediate " + (m == null ? "null" : m.xCoord + " " + m.yCoord));
		Vec3D n = a.getIntermediateWithXValue(a, 2.0D);
		System.out.println("intermediate " + (n == null ? "null" : "set"));
		for (int i = -3; i <= 3; ++i) {
			double x = (double)i * 1.75D;
			System.out.println("floor " + x + " = " + MathHelper.floor_double(x) + " sqrt " + MathHelper.sqrt_float((float)(i * i)));
		}
		AxisAlignedBB box1 = new AxisAlignedBB(0.0D, 0.0D, 0.0D, 1.0D, 1.0D, 1.0D);
		AxisAlignedBB box2 = new AxisAlignedBB(0.5D, 0.5D, 0.5D, 2.0D, 2.0D, 2.0D);
		AxisAlignedBB box3 = new AxisAlignedBB(1.5D, 0.0D, 0.0D, 2.0D, 1.0D, 1.0D);
		System.out.println("intersects " + box1.intersectsWith(box2) + " " + box1.intersectsWith(box3));
		Base base = new Base(5, 3);
		System.out.println("base " + base.size + " " + base.scale);
		System.out.println("sub " + Sub.build(4) + " " + Sub.build(-2));
	}
}
