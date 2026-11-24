package java.io;

// CodeBreaker's java.io, written in Java so that every translation carries the same one: a program that uses these classes is translated with them.
public abstract class InputStream implements Closeable {
    public abstract int read() throws IOException;

    public int read(byte[] b) throws IOException {
        return read(b, 0, b.length);
    }

    public int read(byte[] b, int off, int len) throws IOException {
        if (off < 0 || len < 0 || len > b.length - off) throw new IndexOutOfBoundsException();
        if (len == 0) return 0;
        int c = read();
        if (c == -1) return -1;
        b[off] = (byte) c;
        int i = 1;
        try {
            for (; i < len; i++) {
                c = read();
                if (c == -1) break;
                b[off + i] = (byte) c;
            }
        } catch (IOException ignored) {
            // what was read so far is the answer, as the JDK's is
        }
        return i;
    }

    public byte[] readAllBytes() throws IOException {
        byte[] all = new byte[256];
        int n = 0;
        while (true) {
            if (n == all.length) {
                byte[] more = new byte[all.length * 2];
                for (int i = 0; i < n; i++) more[i] = all[i];
                all = more;
            }
            int got = read(all, n, all.length - n);
            if (got < 0) break;
            n += got;
        }
        byte[] out = new byte[n];
        for (int i = 0; i < n; i++) out[i] = all[i];
        return out;
    }

    public long skip(long n) throws IOException {
        long left = n;
        while (left > 0) {
            if (read() < 0) break;
            left--;
        }
        return n > 0 ? n - left : 0;
    }

    public int available() throws IOException {
        return 0;
    }

    public void close() throws IOException {
    }

    public void mark(int readlimit) {
    }

    public void reset() throws IOException {
        throw new IOException("mark/reset not supported");
    }

    public boolean markSupported() {
        return false;
    }
}
