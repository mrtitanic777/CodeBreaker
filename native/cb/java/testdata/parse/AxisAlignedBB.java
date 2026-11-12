package sample;

// Synthetic fixture (original). A value type with six double bounds and a couple of methods.
public class AxisAlignedBB {
	public double minX;
	public double minY;
	public double minZ;
	public double maxX;
	public double maxY;
	public double maxZ;

	public AxisAlignedBB(double var1, double var3, double var5, double var7, double var9, double var11) {
		this.minX = var1;
		this.minY = var3;
		this.minZ = var5;
		this.maxX = var7;
		this.maxY = var9;
		this.maxZ = var11;
	}

	public boolean intersectsWith(AxisAlignedBB var1) {
		if (var1.maxX <= this.minX || var1.minX >= this.maxX) {
			return false;
		}
		return var1.maxY > this.minY && var1.minY < this.maxY;
	}
}
