package java.util;

public class Stack<E> {
    private Object[] items = new Object[10];
    private int count;

    public Stack() {
    }

    public E push(E item) {
        if (count == items.length) {
            Object[] more = new Object[count * 2];
            for (int i = 0; i < count; i++) more[i] = items[i];
            items = more;
        }
        items[count++] = item;
        return item;
    }

    @SuppressWarnings("unchecked")
    public E pop() {
        if (count == 0) throw new EmptyStackException();
        E top = (E) items[--count];
        items[count] = null;
        return top;
    }

    @SuppressWarnings("unchecked")
    public E peek() {
        if (count == 0) throw new EmptyStackException();
        return (E) items[count - 1];
    }

    public boolean empty() {
        return count == 0;
    }

    public boolean isEmpty() {
        return count == 0;
    }

    public int size() {
        return count;
    }

    @SuppressWarnings("unchecked")
    public E get(int index) {
        if (index < 0 || index >= count) throw new ArrayIndexOutOfBoundsException("Array index out of range: " + index);
        return (E) items[index];
    }

    public int search(Object o) {
        for (int i = count - 1; i >= 0; i--) {
            if (o == null ? items[i] == null : o.equals(items[i])) return count - i;
        }
        return -1;
    }

    public void clear() {
        for (int i = 0; i < count; i++) items[i] = null;
        count = 0;
    }

    @Override public String toString() {
        StringBuilder b = new StringBuilder();
        b.append('[');
        for (int i = 0; i < count; i++) {
            if (i > 0) b.append(", ");
            b.append(String.valueOf(items[i]));
        }
        b.append(']');
        return b.toString();
    }
}
