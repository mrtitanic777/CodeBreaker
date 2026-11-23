package java.io;

public class DataOutputStream extends FilterOutputStream implements DataOutput {
    protected int written;

    public DataOutputStream(OutputStream out) {
        super(out);
    }

    private void count(int n) {
        int next = written + n;
        written = next < 0 ? Integer.MAX_VALUE : next;
    }

    @Override public void write(int b) throws IOException {
        out.write(b);
        count(1);
    }

    @Override public void write(byte[] b) throws IOException {
        write(b, 0, b.length);
    }

    @Override public void write(byte[] b, int off, int len) throws IOException {
        out.write(b, off, len);
        count(len);
    }

    @Override public void flush() throws IOException {
        out.flush();
    }

    @Override public final void writeBoolean(boolean v) throws IOException {
        write(v ? 1 : 0);
    }

    @Override public final void writeByte(int v) throws IOException {
        write(v);
    }

    @Override public final void writeShort(int v) throws IOException {
        write((v >>> 8) & 0xff);
        write(v & 0xff);
    }

    @Override public final void writeChar(int v) throws IOException {
        writeShort(v);
    }

    @Override public final void writeInt(int v) throws IOException {
        write((v >>> 24) & 0xff);
        write((v >>> 16) & 0xff);
        write((v >>> 8) & 0xff);
        write(v & 0xff);
    }

    @Override public final void writeLong(long v) throws IOException {
        writeInt((int) (v >>> 32));
        writeInt((int) v);
    }

    @Override public final void writeFloat(float v) throws IOException {
        writeInt(Float.floatToIntBits(v));
    }

    @Override public final void writeDouble(double v) throws IOException {
        writeLong(Double.doubleToLongBits(v));
    }

    @Override public final void writeBytes(String s) throws IOException {
        for (int i = 0; i < s.length(); i++) write((byte) s.charAt(i));
    }

    @Override public final void writeChars(String s) throws IOException {
        for (int i = 0; i < s.length(); i++) writeChar(s.charAt(i));
    }

    // Java's modified UTF-8: a two-byte length, NUL as two bytes, and each UTF-16 unit on its own.
    @Override public final void writeUTF(String s) throws IOException {
        int length = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            length += c >= 1 && c <= 0x7f ? 1 : c <= 0x7ff ? 2 : 3;
        }
        if (length > 65535) throw new UTFDataFormatException("encoded string too long: " + length + " bytes");
        writeShort(length);
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c >= 1 && c <= 0x7f) {
                write(c);
            } else if (c <= 0x7ff) {
                write(0xc0 | (c >> 6));
                write(0x80 | (c & 0x3f));
            } else {
                write(0xe0 | (c >> 12));
                write(0x80 | ((c >> 6) & 0x3f));
                write(0x80 | (c & 0x3f));
            }
        }
    }

    public final int size() {
        return written;
    }
}
