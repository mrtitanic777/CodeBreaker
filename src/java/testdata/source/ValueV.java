package t;

public class ValueV {
    public double x;
    public double y;
    public Foo link;

    public ValueV(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double len() {
        return x + y;
    }
}
