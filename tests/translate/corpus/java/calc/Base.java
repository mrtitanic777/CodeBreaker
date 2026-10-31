// From the Java lane's test suite (tests/translate/test_cpp03_bodies.py): a class the C++03 back end is judged on.
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
