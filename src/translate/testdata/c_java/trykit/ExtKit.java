import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class ExtKit {
    static final class Names extends ArrayList {
        final Iterable source;

        Names(Iterable source) {
            this.source = source;
            Iterator it = source.iterator();
            while (it.hasNext()) this.add((String) it.next());
        }
    }

    static class Fields extends HashMap {
        final String tag;

        Fields(String tag, List keys) {
            this.tag = tag;
            for (int i = 0; i < keys.size(); i++) this.put(keys.get(i), Integer.valueOf(i));
        }
    }

    public static void main(String[] args) {
        List raw = new ArrayList();
        raw.add("b");
        raw.add("a");
        Names n = new Names(raw);
        List asList = n;
        System.out.println(n.size() + " " + n + " " + asList.get(1) + " " + n.source.iterator().hasNext());
        Fields f = new Fields("t", raw);
        Map m = f;
        System.out.println(f.size() + " " + m.get("a") + " " + f.tag + " " + f.containsKey("b"));
        for (Object o : n) System.out.print(o + ";");
        System.out.println();
    }
}
