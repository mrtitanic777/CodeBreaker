package net.minecraft.src;

import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class ColorizerFoliage {
	private static final int[] foliageColorBuffer = new int[65536];

	public static int getFoliageColor(double var0, double var2) {
		var2 *= var0;
		int var4 = (int)((1.0D - var0) * 255.0D);
		int var5 = (int)((1.0D - var2) * 255.0D);
		return foliageColorBuffer[var5 << 8 | var4];
	}

	static {
		try {
			BufferedImage var0 = ImageIO.read(ColorizerFoliage.class.getResource("/misc/foliagecolor.png"));
			var0.getRGB(0, 0, 256, 256, foliageColorBuffer, 0, 256);
		} catch (Exception var1) {
			var1.printStackTrace();
		}

	}
}
