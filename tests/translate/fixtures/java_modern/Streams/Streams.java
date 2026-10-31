import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;
import java.util.function.IntBinaryOperator;
import java.util.function.Predicate;
import java.util.function.Supplier;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

// Lambdas, method references, streams, collectors, Optional, Comparator combinators.
public class Streams {
    record Person(String name, int age) {}

    static int sumOfSquares(int n) {
        return IntStream.rangeClosed(1, Math.abs(n) % 100).map(x -> x * x).sum();
    }

    static long countEven(int n) {
        return IntStream.range(0, Math.abs(n) % 500).filter(x -> x % 2 == 0).count();
    }

    static String joined(int n) {
        return IntStream.range(0, Math.abs(n) % 12).mapToObj(Integer::toString).collect(Collectors.joining(",", "[", "]"));
    }

    static int applyTwice(int a, int b) {
        IntBinaryOperator op = (x, y) -> x * 31 + y;
        return op.applyAsInt(op.applyAsInt(a, b), b);
    }

    static String upperWords(String s) {
        return Arrays.stream(s.split(" ")).filter(w -> !w.isEmpty()).map(String::toUpperCase)
                .collect(Collectors.joining("-"));
    }

    static int firstOver(int limit) {
        Optional<Integer> f = IntStream.iterate(1, x -> x * 3).boxed().filter(x -> x > limit % 100000).findFirst();
        return f.orElse(-1);
    }

    static int composed(int x) {
        Function<Integer, Integer> add = v -> v + 7;
        Function<Integer, Integer> twice = v -> v * 2;
        Predicate<Integer> big = v -> v > 50;
        int r = twice.apply(add.apply(x % 1000));
        return big.test(r) ? r : -r;
    }

    static int counter(int n) {
        int[] box = {0};
        Runnable inc = () -> box[0] += 2;
        for (int i = 0; i < n % 50; i++) inc.run();
        Supplier<Integer> get = () -> box[0];
        return get.get();
    }

    static List<Person> people() {
        List<Person> ps = new ArrayList<>();
        ps.add(new Person("Ada", 36));
        ps.add(new Person("Linus", 21));
        ps.add(new Person("Grace", 85));
        ps.add(new Person("Alan", 41));
        ps.add(new Person("Barbara", 36));
        return ps;
    }

    public static void main(String[] args) {
        System.out.println(sumOfSquares(10) + " " + countEven(11) + " " + joined(5));
        List<Person> ps = people();
        List<String> names = ps.stream().filter(p -> p.age() > 30).map(Person::name).sorted().collect(Collectors.toList());
        System.out.println(names);
        ps.sort(Comparator.comparingInt(Person::age).thenComparing(Person::name));
        for (Person p : ps) System.out.print(p.name() + ":" + p.age() + " ");
        System.out.println();
        Map<Integer, List<Person>> byAge = ps.stream().collect(Collectors.groupingBy(Person::age));
        System.out.println(byAge.get(36).size() + " " + byAge.containsKey(21) + " " + byAge.size());
        Optional<Person> oldest = ps.stream().max(Comparator.comparingInt(Person::age));
        System.out.println(oldest.map(Person::name).orElse("nobody"));
        int total = ps.stream().mapToInt(Person::age).sum();
        System.out.println("total age " + total + " avg " + ps.stream().mapToInt(Person::age).average().getAsDouble());
        System.out.println(upperWords("the quick  brown fox"));
        System.out.println(firstOver(1000) + " " + composed(30) + " " + counter(7));
        List<Integer> nums = Arrays.asList(5, 3, 8, 1, 9, 2);
        System.out.println(nums.stream().reduce(0, Integer::sum) + " "
                + nums.stream().anyMatch(v -> v > 8) + " " + nums.stream().allMatch(v -> v > 0));
        StringBuilder sb = new StringBuilder();
        nums.forEach(v -> sb.append(v).append(';'));
        System.out.println(sb);
    }
}
