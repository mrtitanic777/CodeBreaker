package sample;

// Synthetic fixture (original). Static utility methods and a static array initialized in a block.
public class MathHelper {
	private static float[] SIN_TABLE = new float[65536];

	public static float sqrt_float(float var0) {
		return (float)Math.sqrt((double)var0);
	}

	public static int floor_double(double var0) {
		int var2 = (int)var0;
		return var0 < (double)var2 ? var2 - 1 : var2;
	}

	static {
		for (int var0 = 0; var0 < 65536; ++var0) {
			SIN_TABLE[var0] = (float)Math.sin((double)var0 * Math.PI * 2.0D / 65536.0D);
		}
	}
}
