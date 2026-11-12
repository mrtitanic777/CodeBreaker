public class Sub extends Base {
	public int extra = 7;

	public Sub(int var1) {
		super(var1 + 1);
		this.extra += this.size;
	}

	public static int build(int var0) {
		Sub var1 = new Sub(var0);
		return var1.extra * 3 + var1.size;
	}
}
