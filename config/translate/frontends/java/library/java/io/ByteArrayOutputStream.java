package java.io;

public class ByteArrayOutputStream extends OutputStream {
    protected byte[] buf;
    protected int count;

    public ByteArrayOutputStream() {
        this(32);
    }

    public ByteArrayOutputStream(int size) {
        if (size < 0) throw new IllegalArgumentException("Negative initial size: " + size);
        buf = new byte[size];
    }

    private void room(int wanted) {
        if (wanted <= buf.length) return;
        byte[] more = new byte[Math.max(buf.length * 2, wanted)];
        for (int i = 0; i < count; i++) more[i] = buf[i];
        buf = more;
    }

    @Override public void write(int b) {
        room(count + 1);
        buf[count++] = (byte) b;
    }

    @Override public void write(byte[] b, int off, int len) {
        if (off < 0 || len < 0 || len > b.length - off) throw new IndexOutOfBoundsException();
        room(count + len);
        for (int i = 0; i < len; i++) buf[count + i] = b[off + i];
        count += len;
    }

    public void writeBytes(byte[] b) {
        write(b, 0, b.length);
    }

    public void writeTo(OutputStream out) throws IOException {
        out.write(buf, 0, count);
    }

    public void reset() {
        count = 0;
    }

    public byte[] toByteArray() {
        byte[] out = new byte[count];
        for (int i = 0; i < count; i++) out[i] = buf[i];
        return out;
    }

    public int size() {
        return count;
    }

    // Text in the platform's character set is not carried: the comparison with the JVM refuses a program that prints it.
    @Override public String toString() {
        throw new UnsupportedOperationException("ByteArrayOutputStream.toString");
    }

    @Override public void close() throws IOException {
    }
}
