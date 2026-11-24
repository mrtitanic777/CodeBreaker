package java.lang.ref;

// A reference that keeps what it holds: nothing here is ever collected behind the program's back.
public abstract class Reference<T> {
    private Object referent;

    Reference(T referent) {
        this.referent = referent;
    }

    @SuppressWarnings("unchecked")
    public T get() {
        return (T) referent;
    }

    public void clear() {
        referent = null;
    }
}
