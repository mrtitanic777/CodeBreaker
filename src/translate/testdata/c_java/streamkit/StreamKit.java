import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

// What the C runtime's streams must do as Java does: laziness and its order, short-circuiting, each stage once, the collectors.
public class StreamKit {
    static int calls = 0;

    public static void main(String[] args) {
        List<String> words = new ArrayList<>();
        words.add("pear"); words.add("fig"); words.add("apple"); words.add("kiwi"); words.add("fig"); words.add("banana");

        System.out.println(words.stream().filter(w -> { System.out.println("filter " + w); return w.length() > 3; })
                .map(w -> { System.out.println("map " + w); return w + "!"; }).limit(2).collect(Collectors.joining(",", "<", ">")));
        System.out.println(words.stream().sorted().distinct().collect(Collectors.toList()));
        System.out.println(words.stream().skip(1).limit(3).count());
        System.out.println(words.stream().mapToInt(String::length).sum());
        System.out.println(words.stream().anyMatch(w -> { calls++; return w.equals("fig"); }) + " " + calls);
        System.out.println(words.stream().allMatch(w -> w.length() > 2) + " " + words.stream().noneMatch(w -> w.isEmpty()));
        System.out.println(words.stream().reduce("", (a, b) -> a + b.charAt(0)));
        System.out.println(words.stream().flatMap(w -> IntStream.range(0, w.length() % 3).mapToObj(i -> w + i)).collect(Collectors.joining(" ")));
        Map<Integer, List<String>> byLength = words.stream().collect(Collectors.groupingBy(String::length));
        System.out.println(byLength.get(3) + " " + byLength.get(4) + " " + byLength.size());
        Map<Integer, Long> counts = words.stream().collect(Collectors.groupingBy(String::length, Collectors.counting()));
        System.out.println(counts.get(3) + " " + counts.get(6));

        System.out.println(IntStream.rangeClosed(1, 5).map(x -> x * x).sum());
        System.out.println(IntStream.range(0, 20).filter(x -> x % 3 == 0).boxed().collect(Collectors.toList()));
        System.out.println(IntStream.range(0, 10).skip(7).reduce(100, (a, b) -> a - b));
        System.out.println(IntStream.rangeClosed(2147483646, 2147483647).count());
        int[] made = IntStream.range(0, 6).map(x -> 5 - x).sorted().limit(3).toArray();
        System.out.println(made.length + " " + made[0] + " " + made[2]);
        System.out.println(IntStream.range(0, 3).mapToObj(i -> "n" + i).collect(Collectors.joining()));

        java.util.stream.Stream<String> once = words.stream();
        once.count();
        try {
            once.count();
        } catch (IllegalStateException e) {
            System.out.println(e.getMessage());
        }
    }
}
