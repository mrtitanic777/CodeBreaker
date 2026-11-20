import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class HashOrder {
    // Java visits a HashMap in hash order, .NET's Dictionary in another: iterating one is declined
    static int walk() {
        Map<String, Integer> m = new HashMap<>();
        m.put("a", 1);
        int t = 0;
        for (int v : m.values()) t += v;
        return t;
    }

    static int sorted() {
        Map<String, Integer> m = new TreeMap<>();
        m.put("a", 1);
        int t = 0;
        for (int v : m.values()) t += v;
        return t;
    }
}
