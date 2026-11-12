package t;
public class Registry {
    Foo other;
    public Foo get(int i) {
        return null;
    }
    public Foo find(String s) {
        if (s == null) {
            return  null ;
        }
        return other;
    }
    public int num() {
        return null;
    }
    public String str() {
        return null;
    }
    public List<Foo> list() {
        return null;
    }
    public Foo[] arr() {
        return null;
    }
    public Foo plain() {
        return other;
    }
    public Foo a() {
        return null;
    }
    public Foo notNull() {
        return nullable;
    }
    public Bar late() {
        returnnull;
        return null
    }
}
