package sample;

// A driver for the Java lane's fixtures (written for the C# kit's judge): MathHelper's static table and
// floor, Vec3D's arithmetic and null return, AxisAlignedBB's intersection test.
public class FixtureMain {
	public static void main(String[] args) {
		double[] ds = {0.0, 0.5, -0.5, 1.999, -1.001, 12345.678, -98765.4321};
		for (double d : ds) {
			System.out.println("floor(" + d + ")=" + MathHelper.floor_double(d) + " sqrt=" + MathHelper.sqrt_float((float)Math.abs(d)));
		}
		Vec3D a = new Vec3D(1.0, 2.0, 3.0);
		Vec3D b = new Vec3D(4.5, -2.0, 0.25);
		Vec3D d = a.subtract(b);
		System.out.println("subtract " + d.xCoord + " " + d.yCoord + " " + d.zCoord);
		Vec3D m = a.getIntermediateWithXValue(b, 2.0);
		System.out.println("intermediate " + (m == null ? "null" : m.xCoord + "," + m.yCoord + "," + m.zCoord));
		Vec3D n = a.getIntermediateWithXValue(new Vec3D(1.0000001, 0.0, 0.0), 2.0);
		System.out.println("near " + (n == null ? "null" : "made"));
		AxisAlignedBB box = new AxisAlignedBB(0, 0, 0, 1, 1, 1);
		AxisAlignedBB[] others = {new AxisAlignedBB(0.5, 0.5, 0.5, 2, 2, 2), new AxisAlignedBB(1, 0, 0, 2, 1, 1),
				new AxisAlignedBB(-1, -1, -1, 0.01, 0.01, 0.01), new AxisAlignedBB(0.2, 3, 0.2, 0.8, 4, 0.8)};
		for (AxisAlignedBB o : others) {
			System.out.println("intersects " + box.intersectsWith(o));
		}
		System.out.println("pool " + Vec3D.nextVector + " " + Vec3D.vectorList.length);
	}
}
