import java.util.ArrayList;
import java.util.List;

// Records, a sealed interface, switch expressions, instanceof patterns, enhanced for, var, default methods.
public class Shapes {
    sealed interface Shape permits Circle, Rect, Square {
        double area();

        default String describe() {
            return name() + " with area " + String.format("%.2f", area());
        }

        String name();
    }

    record Circle(double r) implements Shape {
        public double area() { return Math.PI * r * r; }
        public String name() { return "circle"; }
    }

    record Rect(int w, int h) implements Shape {
        Rect {
            if (w < 0 || h < 0) throw new IllegalArgumentException("negative side");
        }
        public double area() { return w * h; }
        public String name() { return "rect"; }
    }

    record Square(int side) implements Shape {
        public double area() { return side * side; }
        public String name() { return "square"; }
    }

    enum Size { SMALL, MEDIUM, LARGE }

    static Size classify(double area) {
        if (area < 10) return Size.SMALL;
        if (area < 100) return Size.MEDIUM;
        return Size.LARGE;
    }

    static String sizeWord(int k) {
        Size s = Size.values()[Math.floorMod(k, 3)];
        return switch (s) {
            case SMALL -> "s";
            case MEDIUM -> "m";
            case LARGE -> {
                String big = "L";
                yield big + big;
            }
        };
    }

    static int sides(Object o) {
        if (o instanceof Square sq) return 4 + sq.side() * 0;
        if (o instanceof Rect r && r.w() != r.h()) return 4;
        if (o instanceof Circle) return 0;
        return -1;
    }

    static int rectArea(int w, int h) {
        var r = new Rect(Math.abs(w) % 1000, Math.abs(h) % 1000);
        return (int) r.area();
    }

    static boolean sameRect(int a, int b) {
        return new Rect(a & 15, b & 15).equals(new Rect(b & 15, a & 15));
    }

    static String rectText(int a, int b) {
        return new Rect(a & 255, b & 255).toString();
    }

    static int rectHash(int a, int b) {
        return new Rect(a & 255, b & 255).hashCode();
    }

    static String dayKind(String day) {
        return switch (day) {
            case "sat", "sun" -> "weekend";
            case "mon", "tue", "wed", "thu", "fri" -> "weekday";
            default -> "unknown";
        };
    }

    static int countKinds(int n) {
        List<Shape> shapes = new ArrayList<>();
        for (int i = 0; i < n % 20; i++) {
            shapes.add(switch (i % 3) {
                case 0 -> new Circle(i);
                case 1 -> new Rect(i, i + 1);
                default -> new Square(i);
            });
        }
        int total = 0;
        for (Shape s : shapes) {
            total += sides(s);
        }
        return total;
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Circle(1.5), new Rect(3, 4), new Square(5), new Rect(2, 2));
        double sum = 0;
        for (var s : shapes) {
            System.out.println(s.describe() + " -> " + classify(s.area()) + " sides=" + sides(s));
            sum += s.area();
        }
        System.out.println("total " + sum);
        System.out.println(new Rect(1, 2) + " " + new Circle(0.5) + " " + new Rect(1, 2).equals(new Rect(1, 2)));
        for (int k = 0; k < 4; k++) System.out.print(sizeWord(k) + ",");
        System.out.println();
        System.out.println(dayKind("sun") + " " + dayKind("wed") + " " + dayKind("xyz"));
        try {
            new Rect(-1, 2);
        } catch (IllegalArgumentException e) {
            System.out.println("caught " + e.getMessage());
        }
        System.out.println(countKinds(10));
    }
}
