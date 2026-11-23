import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// Reflection as a game's registry uses it: class literals as keys, making an object from a class, and asking what an object is.
public class ReflectKit {
    interface Living { }

    static class World {
        final String name;
        public World() { this("default"); }
        public World(String name) { this.name = name; }
    }

    static abstract class Entity {
        final World world;
        public Entity(World world) { this.world = world; }
        abstract String sound();
    }

    static class Pig extends Entity implements Living {
        public Pig(World world) { super(world); }
        @Override String sound() { return "oink in " + world.name; }
    }

    static class Cow extends Entity implements Living {
        public Cow(World world) { super(world); }
        @Override String sound() { return "moo"; }
    }

    static class Arrow extends Entity {
        public Arrow(World world) { super(world); }
        @Override String sound() { return "thud"; }
    }

    static class Baby extends Pig {
        public Baby(World world) { super(world); }
        @Override String sound() { return "squeak"; }
    }

    static final Map<String, Class<?>> byName = new HashMap<>();
    static final Map<Class<?>, Integer> ids = new HashMap<>();

    static void add(Class<?> c, String name, int id) {
        byName.put(name, c);
        ids.put(c, id);
    }

    public static void main(String[] args) throws Exception {
        add(Pig.class, "Pig", 90);
        add(Cow.class, "Cow", 92);
        add(Arrow.class, "Arrow", 10);
        add(Baby.class, "Baby", 91);
        World w = new World("overworld");
        for (String name : new String[] {"Pig", "Cow", "Arrow", "Baby"}) {
            Entity e = (Entity) byName.get(name).getConstructor(new Class[] {World.class}).newInstance(new Object[] {w});
            System.out.println(name + " " + e.sound() + " " + ids.get(e.getClass()) + " " + Living.class.isInstance(e) + " " + Pig.class.isAssignableFrom(e.getClass()) + " " + Entity.class.isAssignableFrom(e.getClass()));
        }
        World made = World.class.newInstance();
        System.out.println(made.name + " " + (made.getClass() == World.class) + " " + (new Pig(w).getClass() == Baby.class));
        System.out.println(Pig.class.isAssignableFrom(Baby.class) + " " + Baby.class.isAssignableFrom(Pig.class) + " " + Living.class.isAssignableFrom(Baby.class) + " " + Living.class.isInterface());
        System.out.println(new Baby(w).getClass().getSuperclass() == Pig.class);
        List<Entity> all = new ArrayList<>();
        all.add(new Pig(w));
        all.add(new Arrow(w));
        all.add(new Baby(w));
        int living = 0;
        for (Entity e : all) {
            if (Living.class.isInstance(e)) living++;
        }
        System.out.println(living);
        try {
            Entity.class.newInstance();
        } catch (InstantiationException ex) {
            System.out.println("cannot make it");
        }
    }
}
