import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

// java.io's data and byte-array streams and gzip: each value through bytes and back, the exact bytes, and how each kind of failure is reported.
public class DataKit {
    static String hex(byte[] b) {
        StringBuilder s = new StringBuilder();
        for (byte x : b) s.append(Integer.toHexString(x & 0xff)).append(' ');
        return s.toString();
    }

    static void through(DataOutput d) throws IOException {
        d.write(new byte[] {9, 8});
        d.write(new byte[] {1, 2, 3, 4}, 1, 2);
        d.write(300);
    }

    public static void main(String[] args) throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream(2);
        DataOutputStream out = new DataOutputStream(bytes);
        through(out);
        System.out.println(hex(bytes.toByteArray()) + out.size());
        bytes.reset();
        out.writeBoolean(true);
        out.writeByte(-2);
        out.writeShort(-3000);
        out.writeChar('世');
        out.writeInt(0x12345678);
        out.writeLong(-5L);
        out.writeFloat(1.5f);
        out.writeDouble(Math.E);
        out.writeUTF("aé世\u0000z");
        out.writeBytes("hi");
        out.writeChars("ok");
        byte[] all = bytes.toByteArray();
        System.out.println(all.length + " " + hex(all));

        DataInputStream in = new DataInputStream(new ByteArrayInputStream(all));
        System.out.println(in.readBoolean() + " " + in.readByte() + " " + in.readShort() + " " + (int) in.readChar() + " " + in.readInt() + " " + in.readLong() + " " + in.readFloat() + " " + in.readDouble());
        String text = in.readUTF();
        System.out.println(text.length() + " " + (int) text.charAt(1) + " " + (int) text.charAt(2) + " " + (int) text.charAt(3) + " " + in.available());
        byte[] rest = new byte[2];
        in.readFully(rest);
        System.out.println(hex(rest) + in.skipBytes(1) + " " + in.read() + " " + in.readUnsignedByte() + " " + in.read() + " " + in.available());
        try {
            in.readInt();
        } catch (EOFException e) {
            System.out.println("eof " + e.getMessage());
        }

        ByteArrayInputStream marked = new ByteArrayInputStream(new byte[] {1, 2, 3, 4, 5}, 1, 3);
        marked.read();
        marked.mark(0);
        System.out.println(marked.read() + " " + marked.available() + " " + marked.skip(9));
        marked.reset();
        System.out.println(marked.read() + " " + marked.markSupported());

        ByteArrayOutputStream packed = new ByteArrayOutputStream();
        byte[] plain = new byte[5000];
        for (int i = 0; i < plain.length; i++) plain[i] = (byte) (i % 7 * i % 31);
        try (GZIPOutputStream zip = new GZIPOutputStream(packed)) {
            zip.write(plain, 0, 3000);
            zip.write(plain, 3000, 2000);
        }
        byte[] zipped = packed.toByteArray();
        System.out.println((zipped[0] & 0xff) + " " + (zipped[1] & 0xff) + " " + (zipped.length < plain.length));
        byte[] again;
        try (GZIPInputStream unzip = new GZIPInputStream(new ByteArrayInputStream(zipped))) {
            again = unzip.readAllBytes();
        }
        boolean same = again.length == plain.length;
        for (int i = 0; same && i < again.length; i++) same = again[i] == plain[i];
        System.out.println(again.length + " " + same);
        try {
            new GZIPInputStream(new ByteArrayInputStream(new byte[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
        } catch (IOException e) {
            System.out.println("bad " + e.getMessage());
        }
        zipped[zipped.length - 6] ^= 1;
        try {
            new GZIPInputStream(new ByteArrayInputStream(zipped)).readAllBytes();
        } catch (IOException e) {
            System.out.println("corrupt " + e.getMessage());
        }
    }
}
