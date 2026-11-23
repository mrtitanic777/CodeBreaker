package java.lang.reflect;

// A constructor found through CodeBreaker's java.lang.Class.
public final class Constructor<T> {
    private final Class.Maker maker;

    public Constructor(Class.Maker maker) {
        this.maker = maker;
    }

    @SuppressWarnings("unchecked")
    public T newInstance(Object... args) throws InstantiationException, IllegalAccessException, InvocationTargetException {
        return (T) maker.make(args);
    }
}
