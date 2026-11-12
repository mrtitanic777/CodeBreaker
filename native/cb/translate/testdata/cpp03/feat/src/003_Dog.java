public class Dog extends Animal {
    String name() { return "dog"; }
    public int walk(int n) { return super.walk(n) + 1; }
    int outerRef() { return this.legs + super.legs + Animal.table[0] + Dog.table[1]; }
}
