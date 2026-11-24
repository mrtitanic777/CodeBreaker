import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

// Anonymous classes of the library's collections, filled by an initialiser block.
public class AnonLibKit {
    static final Map<String, Integer> TABLE = new LinkedHashMap<String, Integer>() {{
        put("one", 1);
        put("two", 2);
    }};

    static List<String> names(String first) {
        return new ArrayList<String>() {{
            add(first);
            add(first + "!");
        }};
    }

    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(4);
            add(5);
            add(size() * 10);
        }};
        Map<String, Integer> map = new HashMap<String, Integer>() {{
            put("k", 1);
            put("l", size());
        }};
        Set<Integer> set = new HashSet<Integer>(list) {{
            add(99);
            remove(4);
        }};
        System.out.println(list + " " + list.size() + " " + map + " " + map.get("l") + " " + set.contains(99) + " " + set.contains(4) + " " + set.size());
        System.out.println(TABLE + " " + names("hi"));
        list.add(6);
        System.out.println(list);
    }
}
