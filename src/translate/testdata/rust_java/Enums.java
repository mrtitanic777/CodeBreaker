import java.util.ArrayList;
import java.util.List;

public class Enums {
    enum Color { RED, GREEN, BLUE }

    enum Planet {
        MERCURY(3.303e+23, 2.4397e6), EARTH(5.976e+24, 6.37814e6);
        private final double mass, radius;
        Planet(double mass, double radius) { this.mass = mass; this.radius = radius; }
        double surfaceGravity() { return 6.67300E-11 * mass / (radius * radius); }
        public String toString() { return "Planet:" + name().toLowerCase() + "#" + ordinal(); }
    }

    interface Priced { int price(); }
    enum Size implements Priced {
        SMALL(1), LARGE(5);
        final int p;
        Size(int p) { this.p = p; }
        public int price() { return p; }
    }

    static String mood(Color c) {
        switch (c) {
            case RED: return "angry";
            case GREEN: return "calm";
            default: return "blue";
        }
    }

    public static void main(String[] args) {
        for (Color c : Color.values()) System.out.println(c + " " + c.ordinal() + " " + mood(c) + " " + c.name().length());
        Color x = Color.valueOf("GREEN");
        System.out.println(x == Color.GREEN);
        System.out.println(x.compareTo(Color.BLUE) + " " + x.equals(Color.RED));
        for (Planet p : Planet.values()) System.out.printf("%s %.2f%n", p, p.surfaceGravity());
        int total = 0;
        for (Size s : Size.values()) total += s.price();
        System.out.println(total);
        List<Color> list = new ArrayList<>();
        list.add(Color.BLUE); list.add(Color.RED);
        System.out.println(list);
        Color bad = Color.valueOf("PURPLE");
    }
}
