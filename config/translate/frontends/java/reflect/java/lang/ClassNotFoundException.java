package java.lang;

public class ClassNotFoundException extends ReflectiveOperationException {
    public ClassNotFoundException() {
    }

    public ClassNotFoundException(String message) {
        super(message);
    }
}
