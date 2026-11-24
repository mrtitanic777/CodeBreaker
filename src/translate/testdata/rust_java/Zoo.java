public class Zoo {
    interface Shape {
        double area();
        String name();
    }

    static abstract class Base implements Shape {
        protected int id;
        static int made = 0;
        Base() { id = ++made; }
        public String name() { return "shape#" + id; }
        public String toString() { return name() + " area=" + area(); }
    }

    static class Rect extends Base {
        private final double w, h;
        Rect(double w, double h) { super(); this.w = w; this.h = h; }
        public double area() { return w * h; }
    }

    static class Square extends Rect {
        Square(double s) { super(s, s); }
        @Override public String name() { return "square#" + id; }
    }

    static class Circle extends Base {
        double r;
        Circle(double r) { this.r = r; }
        public double area() { return Math.PI * r * r; }
        public boolean equals(Object o) {
            if (!(o instanceof Circle)) return false;
            return ((Circle) o).r == r;
        }
        public int hashCode() { return (int) r; }
    }

    static class Counter {
        private int count;
        private String label = "c";
        void inc() { count++; }
        void add(int n) { count += n; }
        int get() { return count; }
        Counter self() { return this; }
    }

    static class Animal {
        String name;
        Animal(String name) { this.name = name; }
        String sound() { return "..."; }
        String describe() { return name + " says " + sound(); }
    }
    static class Dog extends Animal {
        Dog(String n) { super(n); }
        String sound() { return "Woof"; }
        String describe() { return super.describe() + "!"; }
    }
    static class Cat extends Animal {
        Cat(String n) { super(n); }
        String sound() { return "Meow"; }
    }

    static int add(int a, int b) { return a + b; }
    static double add(double a, double b) { return a + b; }

    public static void main(String[] args) {
        Shape[] shapes = { new Rect(2, 3), new Square(4), new Circle(1.5) };
        double total = 0;
        for (Shape s : shapes) {
            System.out.println(s.name() + ": " + s.area());
            total += s.area();
        }
        System.out.println("total " + total);
        System.out.println(shapes[1]);
        Object o = shapes[2];
        System.out.println(o.equals(new Circle(1.5)) + " " + o.equals(shapes[0]) + " " + o.hashCode());
        Counter c = new Counter();
        c.inc(); c.inc(); c.add(5);
        System.out.println(c.self().get());
        Animal[] zoo = { new Dog("Rex"), new Cat("Tom"), new Animal("Thing") };
        for (Animal a : zoo) System.out.println(a.describe());
        System.out.println(add(1, 2) + " " + add(1.5, 2.5));
        Base b = (Base) shapes[0];
        System.out.println(b.name() + " " + Base.made);
        if (shapes[1] instanceof Rect) System.out.println("a rect");
        Rect r = (Rect) shapes[2];
    }
}
