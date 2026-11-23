import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// Nulls inside collections and arrays of boxes, and enum constants with bodies of their own.
public class HeldKit {
    enum Op {
        ADD("+") {
            @Override int apply(int a, int b) { return a + b; }
        },
        MUL("*") {
            @Override int apply(int a, int b) { return a * b; }
            @Override String label() { return "times " + super.label(); }
        },
        NEG("-") {
            private int calls = 0;
            @Override int apply(int a, int b) { calls++; return -a * calls; }
        };

        private final String symbol;
        Op(String symbol) { this.symbol = symbol; }
        abstract int apply(int a, int b);
        String label() { return symbol + ":" + name() + ":" + ordinal(); }
    }

    enum Light {
        RED, GREEN { @Override boolean go() { return true; } }, AMBER;
        boolean go() { return false; }
    }

    static Integer firstOr(List<Integer> list) {
        return list.isEmpty() ? null : list.get(0);
    }

    public static void main(String[] args) {
        for (Op op : Op.values()) System.out.println(op + " " + op.label() + " " + op.apply(3, 4) + " " + op.apply(5, 6));
        System.out.println(Op.valueOf("MUL").apply(6, 7) + " " + Op.ADD.compareTo(Op.NEG));
        for (Light l : Light.values()) {
            switch (l) {
                case GREEN: System.out.println("green " + l.go()); break;
                default: System.out.println(l + " " + l.go());
            }
        }

        List<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(null);
        list.add(3);
        System.out.println(list + " " + list.size() + " " + list.get(1) + " " + list.contains(null) + " " + list.indexOf(null));
        int sum = 0;
        for (Integer x : list) {
            if (x != null) sum += x;
        }
        System.out.println(sum + " " + firstOr(list) + " " + firstOr(new ArrayList<>()));
        try {
            int bad = list.get(1);
            System.out.println(bad);
        } catch (NullPointerException e) {
            System.out.println("npe list");
        }
        list.set(0, null);
        System.out.println(list);

        Map<String, Double> map = new HashMap<>();
        map.put("a", 1.5);
        map.put("b", null);
        System.out.println(map.get("a") + " " + map.get("b") + " " + map.get("c") + " " + map.containsKey("b") + " " + map.containsKey("c") + " " + map.size());

        Integer[] boxes = new Integer[3];
        boxes[1] = 7;
        System.out.println(boxes[0] + " " + boxes[1] + " " + (boxes[2] == null));
        try {
            int bad = boxes[0] + 1;
            System.out.println(bad);
        } catch (NullPointerException e) {
            System.out.println("npe array");
        }
        Double[][] grid = new Double[2][2];
        grid[1][0] = 2.5;
        System.out.println(grid[0][0] + " " + grid[1][0]);
        Integer[] given = {4, null, 6};
        int total = 0;
        for (Integer g : given) total += g == null ? 100 : g;
        System.out.println(total);
    }
}
