package sample;

public class Base {
	public int size;
	public int scale = 2;

	public Base(int var1) {
		this.size = var1 * this.scale;
	}

	public Base(int var1, int var2) {
		this(var1);
		this.scale += var2;
	}
}
