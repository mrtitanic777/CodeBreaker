// The ranges of UTF-16 code units Fernflower's InterpreterUtil.isPrintableUnicode accepts, under the JDK that runs the reference.
public class Printable {
  public static void main(String[] a) {
    StringBuilder b = new StringBuilder();
    int start = -1, n = 0;
    for (int c = 0; c <= 0x10000; c++) {
      boolean p = false;
      if (c <= 0xFFFF) {
        int t = Character.getType((char)c);
        p = t != Character.UNASSIGNED && t != Character.LINE_SEPARATOR && t != Character.PARAGRAPH_SEPARATOR && t != Character.CONTROL
            && t != Character.FORMAT && t != Character.PRIVATE_USE && t != Character.SURROGATE;
      }
      if (p && start < 0) start = c;
      if (!p && start >= 0) {
        b.append(String.format("{0x%04X, 0x%04X},", start, c - 1));
        if (++n % 8 == 0) b.append('\n'); else b.append(' ');
        start = -1;
      }
    }
    System.out.print(b);
    System.err.println(n + " ranges");
  }
}
