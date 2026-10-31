package net.minecraft.src;

import java.util.Random;

// A driver (written for the C# kit's judge) for real Minecraft Alpha classes: NoiseGenerator,
// NoiseGeneratorPerlin, NoiseGeneratorOctaves and MathHelper, taken from the decompiled source when the
// machine has it (binaries/java/original_java; it is not in the repository). World generation from a
// seed: the same numbers only if java.util.Random, the double arithmetic and Java's text for doubles
// are all carried exactly.
public class NoiseMain {
	public static void main(String[] args) {
		Random rand = new Random(1234567L);
		NoiseGeneratorOctaves low = new NoiseGeneratorOctaves(rand, 16);
		NoiseGeneratorOctaves high = new NoiseGeneratorOctaves(rand, 8);
		for (int i = 0; i < 6; i++) {
			double x = i * 13.7 - 40.0;
			System.out.println("noise " + i + " " + low.generateNoise(x, x * 0.5) + " " + high.generateNoise(x * 2.0, -x));
		}
		double[] field = low.generateNoiseOctaves(null, 16.0, 0.0, 32.0, 5, 3, 5, 684.412, 684.412, 684.412);
		double sum = 0.0;
		for (int i = 0; i < field.length; i++) {
			sum += field[i];
		}
		System.out.println("field " + field.length + " sum " + sum + " first " + field[0] + " last " + field[field.length - 1]);
		double[] flat = high.generateNoiseOctaves2D(null, 3, 7, 4, 4, 1.121, 1.121, 0.5);
		System.out.println("flat " + flat[0] + " " + flat[5] + " " + flat[15]);
		for (int a = 0; a < 5; a++) {
			float t = a * 0.7f;
			System.out.println("trig " + MathHelper.sin(t) + " " + MathHelper.cos(t) + " " + MathHelper.sqrt_float(t) + " "
					+ MathHelper.floor_double(t * -3.3) + " " + MathHelper.bucketInt(a * 7 - 15, 4) + " " + MathHelper.abs_max(-t, 2.0));
		}
	}
}
