package java.util.zip;

import java.io.ByteArrayOutputStream;
import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;

// A valid gzip stream, compressed with fixed Huffman codes over a hash of three-byte matches. Its bytes are not zlib's: a program that prints them is refused by the comparison with the JVM.
public class GZIPOutputStream extends FilterOutputStream {
    private final ByteArrayOutputStream pending = new ByteArrayOutputStream();
    private boolean finished;
    private int bits;
    private int bitCount;

    public GZIPOutputStream(OutputStream out) throws IOException {
        super(out);
        int[] header = {0x1f, 0x8b, 8, 0, 0, 0, 0, 0, 0, 0xff};
        for (int b : header) out.write(b);
    }

    public GZIPOutputStream(OutputStream out, int size) throws IOException {
        this(out);
        if (size <= 0) throw new IllegalArgumentException("buffer size <= 0");
    }

    @Override public void write(int b) throws IOException {
        if (finished) throw new IOException("write beyond end of stream");
        pending.write(b);
    }

    @Override public void write(byte[] b, int off, int len) throws IOException {
        if (finished) throw new IOException("write beyond end of stream");
        pending.write(b, off, len);
    }

    private void put(int value, int count) throws IOException {
        bits |= value << bitCount;
        bitCount += count;
        while (bitCount >= 8) {
            out.write(bits & 0xff);
            bits >>>= 8;
            bitCount -= 8;
        }
    }

    private static int reversed(int code, int length) {
        int r = 0;
        for (int i = 0; i < length; i++) {
            r = (r << 1) | (code & 1);
            code >>= 1;
        }
        return r;
    }

    private void literal(int symbol) throws IOException {
        if (symbol < 144) put(reversed(0x30 + symbol, 8), 8);
        else if (symbol < 256) put(reversed(0x190 + symbol - 144, 9), 9);
        else if (symbol < 280) put(reversed(symbol - 256, 7), 7);
        else put(reversed(0xC0 + symbol - 280, 8), 8);
    }

    private void match(int length, int distance) throws IOException {
        int code = 28;
        while (Inflate.LENGTH_BASE[code] > length) code--;
        literal(257 + code);
        if (Inflate.LENGTH_EXTRA[code] > 0) put(length - Inflate.LENGTH_BASE[code], Inflate.LENGTH_EXTRA[code]);
        int d = 29;
        while (Inflate.DISTANCE_BASE[d] > distance) d--;
        put(reversed(d, 5), 5);
        if (Inflate.DISTANCE_EXTRA[d] > 0) put(distance - Inflate.DISTANCE_BASE[d], Inflate.DISTANCE_EXTRA[d]);
    }

    public void finish() throws IOException {
        if (finished) return;
        finished = true;
        byte[] data = pending.toByteArray();
        put(1, 1);
        put(1, 2);
        int[] last = new int[1 << 15];
        for (int i = 0; i < last.length; i++) last[i] = -1;
        int at = 0;
        while (at < data.length) {
            int best = 0;
            int from = -1;
            if (at + 2 < data.length) {
                int hash = (((data[at] & 0xff) << 10) ^ ((data[at + 1] & 0xff) << 5) ^ (data[at + 2] & 0xff)) & 0x7fff;
                from = last[hash];
                last[hash] = at;
                if (from >= 0 && at - from <= 32768) {
                    int limit = Math.min(258, data.length - at);
                    while (best < limit && data[from + best] == data[at + best]) best++;
                }
            }
            if (best >= 3) {
                match(best, at - from);
                at += best;
            } else {
                literal(data[at] & 0xff);
                at++;
            }
        }
        literal(256);
        if (bitCount > 0) put(0, 8 - bitCount);
        int crc = Inflate.crc32(data, data.length);
        for (int i = 0; i < 4; i++) out.write((crc >>> (8 * i)) & 0xff);
        for (int i = 0; i < 4; i++) out.write((data.length >>> (8 * i)) & 0xff);
    }

    @Override public void flush() throws IOException {
        out.flush();
    }

    @Override public void close() throws IOException {
        finish();
        out.close();
    }
}
