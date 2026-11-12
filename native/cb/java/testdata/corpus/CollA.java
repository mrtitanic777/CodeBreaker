package t;
public class CollA {
    public void fill(List<Foo> out, int n) {
        out.add(null);
    }
    public void read(List<Foo> in) {
        in.size();
    }
    public void forward(List<Foo> l, Map<String, Foo> m, Foo[] arr) {
        fill(l, 3);
        m.put("a", null);
        arr.add(null);
    }
    public void same(List<Foo> p) {
        p. clear ();
    }
    public void notMutated(Set<Foo> s) {
        int n = s.size();
        other.add(s);
    }
}
