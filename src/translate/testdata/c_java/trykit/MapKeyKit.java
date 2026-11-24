import java.util.*;

// Map.containsKey takes any object: a key of another type is simply not there, and an array copy is shallow.
public class MapKeyKit {
    public static void main(String[] args) {
        Map<String, Integer> counts = new HashMap<>();
        counts.put("a", 1);
        Object asked = "a";
        Object other = Integer.valueOf(1);
        System.out.println(counts.containsKey(asked) + " " + counts.containsKey(other) + " " + counts.containsKey("b"));
        byte[] held = {1, 2, 3};
        byte[] copy = held.clone();
        copy[0] = 9;
        int[][] grid = {{1, 2}, {3, 4}};
        int[][] shallow = grid.clone();
        shallow[0][0] = 7;
        System.out.println(held[0] + " " + copy[0] + " " + grid[0][0] + " " + (shallow != grid));
    }
}
