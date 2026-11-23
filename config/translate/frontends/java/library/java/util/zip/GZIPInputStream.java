package java.util.zip;

import java.io.EOFException;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;

// The whole member is read and decoded when the stream is made, so a bad header or body is reported then.
public class GZIPInputStream extends FilterInputStream {
    private final byte[] data;
    private final int size;
    private int pos;
    private boolean closed;

    public GZIPInputStream(InputStream in) throws IOException {
        super(in);
        byte[] all = in.readAllBytes();
        if (all.length < 2) throw new EOFException();
        if ((all[0] & 0xff) != 0x1f || (all[1] & 0xff) != 0x8b) throw new ZipException("Not in GZIP format");
        if (all.length < 10) throw new EOFException();
        if (all[2] != 8) throw new ZipException("Unsupported compression method");
        int flags = all[3] & 0xff;
        int at = 10;
        if ((flags & 4) != 0) at += 2 + ((all[at] & 0xff) | ((all[at + 1] & 0xff) << 8));
        if ((flags & 8) != 0) {
            while (all[at] != 0) at++;
            at++;
        }
        if ((flags & 16) != 0) {
            while (all[at] != 0) at++;
            at++;
        }
        if ((flags & 2) != 0) at += 2;
        Inflate inflate = new Inflate(all, at);
        inflate.run();
        data = inflate.bytes();
        size = inflate.length();
        int end = inflate.position();
        if (end + 8 > all.length) throw new EOFException("Unexpected end of ZLIB input stream");
        int crc = 0;
        int length = 0;
        for (int i = 0; i < 4; i++) {
            crc |= (all[end + i] & 0xff) << (8 * i);
            length |= (all[end + 4 + i] & 0xff) << (8 * i);
        }
        if (crc != Inflate.crc32(data, size) || length != size) throw new ZipException("Corrupt GZIP trailer");
    }

    public GZIPInputStream(InputStream in, int size) throws IOException {
        this(in);
        if (size <= 0) throw new IllegalArgumentException("buffer size <= 0");
    }

    private void open() throws IOException {
        if (closed) throw new IOException("Stream closed");
    }

    @Override public int read() throws IOException {
        open();
        return pos < size ? data[pos++] & 0xff : -1;
    }

    @Override public int read(byte[] b, int off, int len) throws IOException {
        open();
        if (off < 0 || len < 0 || len > b.length - off) throw new IndexOutOfBoundsException();
        if (len == 0) return 0;
        if (pos >= size) return -1;
        int n = Math.min(len, size - pos);
        for (int i = 0; i < n; i++) b[off + i] = data[pos + i];
        pos += n;
        return n;
    }

    @Override public long skip(long n) throws IOException {
        open();
        long k = Math.min(n < 0 ? 0 : n, size - pos);
        pos += (int) k;
        return k;
    }

    @Override public int available() throws IOException {
        open();
        return pos < size ? 1 : 0;
    }

    @Override public boolean markSupported() {
        return false;
    }

    @Override public void mark(int readlimit) {
    }

    @Override public void reset() throws IOException {
        throw new IOException("mark/reset not supported");
    }

    @Override public void close() throws IOException {
        if (closed) return;
        closed = true;
        in.close();
    }
}
