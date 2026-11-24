package java.util.zip;

// DEFLATE decoding (RFC 1951) and CRC-32, for the gzip streams of CodeBreaker's Java library.
final class Inflate {
    static final int[] LENGTH_BASE = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
    static final int[] LENGTH_EXTRA = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
    static final int[] DISTANCE_BASE = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
    static final int[] DISTANCE_EXTRA = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
    private static final int[] ORDER = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

    private final byte[] in;
    private int at;
    private int bits;
    private int bitCount;
    private byte[] out = new byte[1024];
    private int size;

    Inflate(byte[] in, int start) {
        this.in = in;
        this.at = start;
    }

    int position() {
        return at;
    }

    int length() {
        return size;
    }

    byte[] bytes() {
        return out;
    }

    private int take(int count) throws ZipException {
        while (bitCount < count) {
            if (at >= in.length) throw new ZipException("invalid stored block lengths");
            bits |= (in[at++] & 0xff) << bitCount;
            bitCount += 8;
        }
        int value = count == 0 ? 0 : bits & ((1 << count) - 1);
        bits >>>= count;
        bitCount -= count;
        return value;
    }

    private void emit(int b) {
        if (size == out.length) {
            byte[] more = new byte[out.length * 2];
            for (int i = 0; i < size; i++) more[i] = out[i];
            out = more;
        }
        out[size++] = (byte) b;
    }

    // A canonical Huffman code: how many codes of each length, then the symbols in order.
    private static int[][] table(int[] lengths, int from, int n) {
        int[] counts = new int[16];
        for (int i = 0; i < n; i++) counts[lengths[from + i]]++;
        counts[0] = 0;
        int[] offsets = new int[16];
        for (int i = 1; i < 15; i++) offsets[i + 1] = offsets[i] + counts[i];
        int[] symbols = new int[n];
        for (int i = 0; i < n; i++) {
            int length = lengths[from + i];
            if (length != 0) symbols[offsets[length]++] = i;
        }
        return new int[][] {counts, symbols};
    }

    private int decode(int[][] code) throws ZipException {
        int value = 0;
        int first = 0;
        int index = 0;
        for (int length = 1; length <= 15; length++) {
            value |= take(1);
            int count = code[0][length];
            if (value - count < first) return code[1][index + (value - first)];
            index += count;
            first = (first + count) << 1;
            value <<= 1;
        }
        throw new ZipException("invalid code lengths set");
    }

    private void codes(int[][] literals, int[][] distances) throws ZipException {
        while (true) {
            int symbol = decode(literals);
            if (symbol < 256) {
                emit(symbol);
            } else if (symbol == 256) {
                return;
            } else {
                symbol -= 257;
                if (symbol >= 29) throw new ZipException("invalid literal/length code");
                int length = LENGTH_BASE[symbol] + take(LENGTH_EXTRA[symbol]);
                int d = decode(distances);
                if (d >= 30) throw new ZipException("invalid distance code");
                int distance = DISTANCE_BASE[d] + take(DISTANCE_EXTRA[d]);
                if (distance > size) throw new ZipException("invalid distance too far back");
                for (int i = 0; i < length; i++) emit(out[size - distance]);
            }
        }
    }

    private void dynamic() throws ZipException {
        int literalCount = take(5) + 257;
        int distanceCount = take(5) + 1;
        int codeCount = take(4) + 4;
        int[] lengths = new int[320];
        for (int i = 0; i < codeCount; i++) lengths[ORDER[i]] = take(3);
        int[][] lengthCode = table(lengths, 0, 19);
        int[] all = new int[320];
        int i = 0;
        while (i < literalCount + distanceCount) {
            int symbol = decode(lengthCode);
            if (symbol < 16) {
                all[i++] = symbol;
            } else {
                int previous = 0;
                int repeat;
                if (symbol == 16) {
                    if (i == 0) throw new ZipException("invalid bit length repeat");
                    previous = all[i - 1];
                    repeat = 3 + take(2);
                } else if (symbol == 17) {
                    repeat = 3 + take(3);
                } else {
                    repeat = 11 + take(7);
                }
                if (i + repeat > literalCount + distanceCount) throw new ZipException("invalid bit length repeat");
                for (int k = 0; k < repeat; k++) all[i++] = previous;
            }
        }
        codes(table(all, 0, literalCount), table(all, literalCount, distanceCount));
    }

    private void fixed() throws ZipException {
        int[] lengths = new int[320];
        for (int i = 0; i < 288; i++) lengths[i] = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
        for (int i = 0; i < 30; i++) lengths[288 + i] = 5;
        codes(table(lengths, 0, 288), table(lengths, 288, 30));
    }

    private void stored() throws ZipException {
        bits = 0;
        bitCount = 0;
        if (at + 4 > in.length) throw new ZipException("invalid stored block lengths");
        int length = (in[at] & 0xff) | ((in[at + 1] & 0xff) << 8);
        int check = (in[at + 2] & 0xff) | ((in[at + 3] & 0xff) << 8);
        at += 4;
        if (length != (~check & 0xffff) || at + length > in.length) throw new ZipException("invalid stored block lengths");
        for (int i = 0; i < length; i++) emit(in[at + i]);
        at += length;
    }

    void run() throws ZipException {
        boolean last = false;
        while (!last) {
            last = take(1) == 1;
            int type = take(2);
            if (type == 0) stored();
            else if (type == 1) fixed();
            else if (type == 2) dynamic();
            else throw new ZipException("invalid block type");
        }
        at -= bitCount / 8;
        bits = 0;
        bitCount = 0;
    }

    static int crc32(byte[] data, int length) {
        int crc = -1;
        for (int i = 0; i < length; i++) {
            crc ^= data[i] & 0xff;
            for (int k = 0; k < 8; k++) crc = (crc & 1) != 0 ? (crc >>> 1) ^ 0xEDB88320 : crc >>> 1;
        }
        return ~crc;
    }
}
