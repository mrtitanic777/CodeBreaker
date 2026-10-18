package sample;

// Synthetic fixture (original). A class with two overloaded constructors (arity 2 and 3) and a
// static final array field with an initializer, to exercise ctor discovery and static-array parsing.
public class Block {
	public static final Block[] blocksList = new Block[256];
	public int blockID;

	public Block(int var1, Material var2) {
		this.blockID = var1;
	}

	public Block(int var1, int var2, Material var3) {
		this.blockID = var1;
	}
}
