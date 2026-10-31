// From the Java lane's test suite (tests/translate/test_cpp03_bodies.py): a class the C++03 back end is judged on.
import java.util.ArrayList;
import java.util.List;

public class Calc {
	public static int total;
	public static int[] table;
	private int[] cells = new int[8];

	static {
		table = new int[4];
		table[1] = 5;
		total = table[1] * 2;
	}
	private List items = new ArrayList();
	public String name = "calc";

	public Calc() {
	}

	public int fill(int var1) {
		int var2 = 0;
		for (int var3 = 0; var3 < this.cells.length; ++var3) {
			this.cells[var3] = var1 * var3 - (var3 >>> 1);
			var2 += this.cells[var3];
		}
		total += var2;
		return var2 > 100 ? var2 % 7 : -var2;
	}

	public static long mix(long var0, int var2) {
		long var3 = var0 ^ (long)var2 << 3;
		while (var3 > 1000L) {
			var3 >>>= 2;
		}
		return var3 >>> 1;
	}

	public static float wrap(float var0) {
		return var0 % 3.5F + (float)Math.floor((double)var0);
	}

	public String label(int var1) {
		String var2 = var1 < 0 ? null : this.name + ":" + var1;
		return var2 == null ? "none" : var2 + (char)(65 + (var1 & 7));
	}

	public int guarded(int var1) {
		Calc var2 = var1 > 0 ? null : this;
		try {
			return var2.cells.length + var1;
		} catch (Exception var3) {
			return -7;
		}
	}

	public int cleanup(int var1) {
		int var2 = 0;
		try {
			var2 = 100 / (var1 & 3);
		} catch (ArithmeticException var3) {
			var2 = -1;
		} finally {
			total += 1;
		}
		return var2 + total;
	}

	public int search(int var1) {
		int var2 = 0;
		label9:
		for (int var3 = 0; var3 < 5; ++var3) {
			for (int var4 = 0; var4 < 5; ++var4) {
				if (var3 * var4 == (var1 & 15)) {
					var2 = var3 * 10 + var4;
					break label9;
				}
				if (var4 > var3) {
					continue label9;
				}
			}
		}
		return var2;
	}

	public static int sum(int... var0) {
		int var1 = 0;
		for (int var2 = 0; var2 < var0.length; ++var2) {
			var1 += var0[var2];
		}
		return var1;
	}

	public static int sums(int var0) {
		return sum() + sum(var0) + sum(var0, 2, 3) + sum(new int[]{var0, var0});
	}

	public static int tabled(int var0) {
		return table[var0 & 3] + total;
	}

	public static int made(int var0) {
		Base var1 = new Base(var0, 3);
		return var1.size + var1.scale;
	}

	public int count(int var1) {
		this.items.add(this.name);
		switch (var1 & 3) {
			case 0:
				return this.items.size();
			case 1:
				this.items.clear();
			default:
				return -1;
		}
	}
}
