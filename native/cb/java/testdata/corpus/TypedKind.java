package t;
public enum TypedKind {
    A(1), B(2);
    private int v;
    TypedKind(int v) {
        this.v = v;
    }
}
