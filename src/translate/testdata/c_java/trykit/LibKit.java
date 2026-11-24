import java.lang.ref.Reference;
import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.EmptyStackException;
import java.util.List;
import java.util.Stack;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LibKit {
    static final Pattern CHUNK = Pattern.compile("c\\.(-?[0-9a-z]+)\\.(-?[0-9a-z]+)\\.dat");
    static final Pattern NUMBER = Pattern.compile("(-?)(0|([1-9]([0-9]*)))(\\.[0-9]+)?((e|E)(\\+|-)?[0-9]+)?");

    static class Item {
        final String name;

        Item(String name) {
            this.name = name;
        }

        @Override public String toString() {
            return "item " + name;
        }
    }

    static void show(Pattern p, String s) {
        Matcher m = p.matcher(s);
        System.out.println(p.pattern() + " / " + s + " -> " + m.matches());
    }

    public static void main(String[] args) {
        Stack st = new Stack();
        st.push("a");
        st.push(Integer.valueOf(7));
        st.push("c");
        System.out.println(st.peek() + " " + st.size() + " " + st.empty() + " " + st.search("a"));
        System.out.println(st.pop() + " " + st.pop() + " " + st.pop() + " " + st.isEmpty());
        try {
            st.pop();
        } catch (EmptyStackException e) {
            System.out.println("empty " + e.getClass().getName());
        }

        Matcher m = CHUNK.matcher("c.-1a.z9.dat");
        if (m.matches()) {
            System.out.println(m.group(1) + " " + m.group(2) + " " + Integer.parseInt(m.group(1), 36) + " " + Integer.parseInt(m.group(2), 36) + " " + m.groupCount());
        }
        show(CHUNK, "c.1.2.txt");
        show(NUMBER, "-12.5e+3");
        show(NUMBER, "012");
        show(NUMBER, "0");
        show(Pattern.compile("[0-9a-z]|([0-9a-z][0-9a-z])"), "a7");
        show(Pattern.compile("[0-9a-z]|([0-9a-z][0-9a-z])"), "a7z");

        Matcher w = Pattern.compile("(\\w+)@(\\w+)\\.com|\\d{2,3}").matcher("mail bob@home.com or 42 or 7 or 1234 end");
        while (w.find()) {
            System.out.println(w.start() + "-" + w.end() + " " + w.group() + " " + w.group(1) + " " + w.group(2));
        }
        System.out.println(Pattern.compile("a*?b").matcher("aaab").lookingAt() + " " + Pattern.compile("(a|ab)(c|bcd)(d*)").matcher("abcd").matches());
        System.out.println(Pattern.compile("\\s*,\\s*").matcher("a , b,c ,  d").replaceAll(";") + " " + Pattern.compile("(\\d)(\\d)").matcher("x1234y").replaceAll("$2$1"));
        System.out.println(Pattern.compile("^ab+$", Pattern.CASE_INSENSITIVE).matcher("ABbB").find() + " " + Pattern.compile("\\bcat\\b").matcher("a cat, concat").find());
        String[] parts = Pattern.compile("[,;]\\s*").split("a, b;c,,d");
        System.out.println(parts.length + " " + parts[0] + parts[1] + parts[2] + parts[3]);
        try {
            Pattern.compile("(ab");
        } catch (IllegalArgumentException e) {
            System.out.println("bad pattern");
        }

        Object[] boxes = {Byte.valueOf((byte) -3), Short.valueOf((short) 300), Boolean.valueOf(true), new Byte((byte) 9), new Short((short) -2)};
        for (Object b : boxes) System.out.print(b + " ");
        System.out.println();
        byte[] raw = "h\u00e9llo".getBytes();
        System.out.println(raw.length + " " + raw[1] + " " + raw[2]);
        List<String> one = new ArrayList<String>();
        one.add("x");
        one.add("y");
        List<String> two = new ArrayList<String>();
        two.add("x");
        two.add("y");
        System.out.println(one.equals(two) + " " + (one.hashCode() == two.hashCode()) + " " + one.hashCode() + " " + one.equals(new ArrayList<String>()));
        Item[] names = {new Item("p"), new Item("q")};
        List<Item> view = java.util.Arrays.asList(names);
        System.out.println(view + " " + view.size() + " " + view.equals(one));
        try {
            throw new OutOfMemoryError();
        } catch (OutOfMemoryError e) {
            System.out.println("no memory");
        }

        List list = new ArrayList();
        Reference ref = new SoftReference(list);
        ((List) ref.get()).add("kept");
        System.out.println(list + " " + (ref.get() == list));
        ref.clear();
        System.out.println(ref.get());
    }
}
