package java.io;

public class DataInputStream extends FilterInputStream implements DataInput {
    public DataInputStream(InputStream in) {
        super(in);
    }

    private int need() throws IOException {
        int c = in.read();
        if (c < 0) throw new EOFException();
        return c;
    }

    @Override public final int read(byte[] b) throws IOException {
        return in.read(b, 0, b.length);
    }

    @Override public final int read(byte[] b, int off, int len) throws IOException {
        return in.read(b, off, len);
    }

    @Override public final void readFully(byte[] b) throws IOException {
        readFully(b, 0, b.length);
    }

    @Override public final void readFully(byte[] b, int off, int len) throws IOException {
        if (off < 0 || len < 0 || len > b.length - off) throw new IndexOutOfBoundsException();
        int n = 0;
        while (n < len) {
            int got = in.read(b, off + n, len - n);
            if (got < 0) throw new EOFException();
            n += got;
        }
    }

    @Override public final int skipBytes(int n) throws IOException {
        int total = 0;
        while (total < n) {
            int skipped = (int) in.skip(n - total);
            if (skipped <= 0) break;
            total += skipped;
        }
        return total;
    }

    @Override public final boolean readBoolean() throws IOException {
        return need() != 0;
    }

    @Override public final byte readByte() throws IOException {
        return (byte) need();
    }

    @Override public final int readUnsignedByte() throws IOException {
        return need();
    }

    @Override public final short readShort() throws IOException {
        return (short) readUnsignedShort();
    }

    @Override public final int readUnsignedShort() throws IOException {
        int high = need();
        return (high << 8) | need();
    }

    @Override public final char readChar() throws IOException {
        return (char) readUnsignedShort();
    }

    @Override public final int readInt() throws IOException {
        int a = need();
        int b = need();
        int c = need();
        return (a << 24) | (b << 16) | (c << 8) | need();
    }

    @Override public final long readLong() throws IOException {
        long high = readInt();
        long low = readInt() & 0xffffffffL;
        return (high << 32) | low;
    }

    @Override public final float readFloat() throws IOException {
        return Float.intBitsToFloat(readInt());
    }

    @Override public final double readDouble() throws IOException {
        return Double.longBitsToDouble(readLong());
    }

    @Override public final String readUTF() throws IOException {
        return readUTF(this);
    }

    public static final String readUTF(DataInput from) throws IOException {
        int length = from.readUnsignedShort();
        byte[] bytes = new byte[length];
        from.readFully(bytes, 0, length);
        StringBuilder out = new StringBuilder(length);
        int at = 0;
        while (at < length) {
            int c = bytes[at] & 0xff;
            int kind = c >> 4;
            if (kind <= 7) {
                at += 1;
                out.append((char) c);
            } else if (kind == 12 || kind == 13) {
                at += 2;
                if (at > length) throw new UTFDataFormatException("malformed input: partial character at end");
                int c2 = bytes[at - 1];
                if ((c2 & 0xC0) != 0x80) throw new UTFDataFormatException("malformed input around byte " + at);
                out.append((char) (((c & 0x1F) << 6) | (c2 & 0x3F)));
            } else if (kind == 14) {
                at += 3;
                if (at > length) throw new UTFDataFormatException("malformed input: partial character at end");
                int c2 = bytes[at - 2];
                int c3 = bytes[at - 1];
                if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) throw new UTFDataFormatException("malformed input around byte " + (at - 1));
                out.append((char) (((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F)));
            } else {
                throw new UTFDataFormatException("malformed input around byte " + at);
            }
        }
        return out.toString();
    }
}
