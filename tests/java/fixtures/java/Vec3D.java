package sample;

// Synthetic fixture (original, hand-written for the j2ctool test suite -- NOT derived from any game
// source). A value-type 3D vector: a static pool ahead of the instance coords, one 3-arg ctor,
// a value-returning method (subtract) and a dual-use method that can return null
// (getIntermediateWithXValue) so the emitter must promote its return to a pointer.
public class Vec3D {
	public static int nextVector = 0;
	public static Vec3D[] vectorList = new Vec3D[256];
	public double xCoord;
	public double yCoord;
	public double zCoord;

	public Vec3D(double var1, double var3, double var5) {
		this.xCoord = var1;
		this.yCoord = var3;
		this.zCoord = var5;
	}

	public Vec3D subtract(Vec3D var1) {
		return new Vec3D(var1.xCoord - this.xCoord, var1.yCoord - this.yCoord, var1.zCoord - this.zCoord);
	}

	public Vec3D getIntermediateWithXValue(Vec3D var1, double var2) {
		double var4 = var1.xCoord - this.xCoord;
		if (var4 * var4 < 1.0E-7D) {
			return null;
		}
		return new Vec3D(var2, this.yCoord, this.zCoord);
	}
}
