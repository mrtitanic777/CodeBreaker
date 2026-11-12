package t;

public class Chain {
    private Hop first;

    public Chain(Hop h) {
        first = h;
    }

    public int deep() {
        return first.next.value;
    }
}
