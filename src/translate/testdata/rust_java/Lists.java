import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class Lists {
    static class Item {
        final String name;
        final int qty;
        Item(String name, int qty) { this.name = name; this.qty = qty; }
        public String toString() { return name + "x" + qty; }
    }

    static int sum(List<Integer> xs) {
        int s = 0;
        for (int x : xs) s += x;
        return s;
    }

    public static void main(String[] args) {
        List<Integer> nums = new ArrayList<>();
        for (int i = 1; i <= 10; i++) nums.add(i * i);
        System.out.println(nums + " size=" + nums.size() + " sum=" + sum(nums));
        nums.remove(0);
        nums.remove(Integer.valueOf(16));
        nums.set(1, 99);
        System.out.println(nums.get(1) + " " + nums.contains(99) + " " + nums.indexOf(25) + " " + nums.isEmpty());

        List<String> words = new ArrayList<>();
        words.add("pear"); words.add("apple"); words.add("fig");
        words.add(1, "kiwi");
        StringBuilder sb = new StringBuilder();
        for (String w : words) sb.append(w.charAt(0)).append(w.length());
        System.out.println(words + " " + sb);

        Map<String, Integer> counts = new HashMap<>();
        String text = "the quick brown fox jumps over the lazy dog the end";
        for (String w : text.split(" ")) {
            if (counts.containsKey(w)) counts.put(w, counts.get(w) + 1);
            else counts.put(w, 1);
        }
        System.out.println(counts);
        for (Map.Entry<String, Integer> e : counts.entrySet()) {
            if (e.getValue() > 1) System.out.println(e.getKey() + " -> " + e.getValue());
        }
        int total = 0;
        for (String k : counts.keySet()) total += counts.get(k);
        System.out.println(total + " " + counts.getOrDefault("cat", 0) + " " + counts.size());

        Set<Integer> seen = new HashSet<>();
        int dup = 0;
        for (int i = 0; i < 30; i++) if (!seen.add(i * 7 % 11)) dup++;
        System.out.println(seen + " dups=" + dup);

        List<Item> items = new ArrayList<>();
        items.add(new Item("bolt", 40));
        items.add(new Item("nut", 25));
        int qty = 0;
        for (Item it : items) qty += it.qty;
        System.out.println(items + " " + qty);
        Map<Integer, String> byQty = new HashMap<>();
        for (Item it : items) byQty.put(it.qty, it.name);
        System.out.println(byQty.get(25) + " " + byQty.get(26) + " " + byQty.values());
        System.out.println(words.get(10));
    }
}
