public class EnumObject {
    // a Java enum with fields and a constructor is an enum of objects: declined
    enum Planet {
        MERCURY(3.3e23), EARTH(5.9e24);
        final double mass;
        Planet(double mass) { this.mass = mass; }
    }

    enum Color { RED, GREEN }

    static int ok() { return Color.values().length; }
}
