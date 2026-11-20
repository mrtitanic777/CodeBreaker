import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

/** Library calls through the API-mapping registry: String, StringBuilder, collections, Math, Integer, System. */
public class Calls {
    static String describe(String s, List<String> names, Map<String, Integer> counts) {
        int n = s.length();
        String sub = s.substring(1, 3).toUpperCase();
        char c = s.charAt(0);
        boolean same = s.equals("x");
        int idx = s.indexOf("a");
        names.add(sub);
        String first = names.get(0);
        names.remove(0);
        counts.put(s, 1);
        boolean has = counts.containsKey(s);
        int m = Math.max(n, Math.abs(idx));
        int p = Integer.parseInt("12");
        String t = Integer.toString(p) + String.valueOf(m);
        StringBuilder sb = new StringBuilder();
        sb.append(c).append(n).append(sub);
        System.out.println(sb.toString() + t + first + same + has);
        System.err.println("err");
        int[] arr = {3, 1, 2};
        Arrays.sort(arr);
        Collections.sort(names);
        return t + names.size() + names.isEmpty();
    }

    static int sorted(TreeSet<Integer> ts) {
        int total = 0;
        for (int x : ts) total += x;
        return total;
    }

    static int hashed(Set<Integer> hs) {
        int total = 0;
        for (int x : hs) total += x;
        return total;
    }

    static List<String> make() {
        List<String> out = new ArrayList<>();
        Set<Integer> seen = new HashSet<>();
        seen.add(1);
        out.add("a" + seen.size());
        return out;
    }
}
