// A driver for the Java lane's Calc, Base and Sub (written for the C# kit's judge): calls every method on
// a few inputs and prints what comes back.
public class CalcMain {
	public static void main(String[] args) {
		Calc c = new Calc();
		int[] inputs = {0, 1, -1, 7, 13, 100, -2147483648, 2147483647};
		for (int i = 0; i < inputs.length; i++) {
			int x = inputs[i];
			System.out.println("fill(" + x + ")=" + c.fill(x) + " label=" + c.label(x) + " guarded=" + c.guarded(x)
					+ " cleanup=" + c.cleanup(x) + " search=" + c.search(x) + " count=" + c.count(x));
			System.out.println("  mix=" + Calc.mix((long)x * 31L, x) + " wrap=" + Calc.wrap((float)x / 3.0F)
					+ " sums=" + Calc.sums(x) + " tabled=" + Calc.tabled(x) + " made=" + Calc.made(x)
					+ " build=" + Sub.build(x));
		}
		System.out.println("total=" + Calc.total + " name=" + c.name);
	}
}
