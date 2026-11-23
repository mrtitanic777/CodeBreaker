package java.lang.reflect;

public class InvocationTargetException extends ReflectiveOperationException {
    private final Throwable target;

    public InvocationTargetException(Throwable target) {
        this.target = target;
    }

    public Throwable getTargetException() {
        return target;
    }

    @Override public Throwable getCause() {
        return target;
    }
}
