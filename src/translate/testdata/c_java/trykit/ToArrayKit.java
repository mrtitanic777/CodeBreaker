import java.util.*;

// Collection.toArray(T[]): into the given array when it has room (a null just after the last), else into a new one of the size.
public class ToArrayKit {
    public static void main(String[] args) {
        List<String> names = new ArrayList<>();
        names.add("a");
        names.add("b");
        String[] fresh = names.toArray(new String[0]);
        System.out.println(fresh.length + " " + fresh[0] + fresh[1]);
        String[] roomy = {"x", "y", "z", "w"};
        String[] same = names.toArray(roomy);
        System.out.println((same == roomy) + " " + roomy[0] + roomy[1] + " " + roomy[2] + " " + roomy[3]);
        Set<Integer> held = new TreeSet<>();
        held.add(3);
        held.add(1);
        Integer[] numbers = held.toArray(new Integer[2]);
        System.out.println(numbers[0] + numbers[1]);
    }
}
