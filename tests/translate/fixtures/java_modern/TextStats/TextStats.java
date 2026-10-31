import java.util.Arrays;

// Text processing on arrays: char counts, Character tests, Arrays.sort, StringBuilder, text blocks,
// integer edge cases (overflow, shifts, division), labelled loops, a classic switch with fall-through.
public class TextStats {
    static final String POEM = """
            Shall I compare thee
            to a summer's day?
            Thou art more lovely
            and more temperate.
            """;

    static int vowels(String s) {
        int n = 0;
        for (char c : s.toLowerCase().toCharArray()) {
            switch (c) {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                    n++;
                    break;
                default:
                    break;
            }
        }
        return n;
    }

    static String histogram(String s) {
        int[] counts = new int[26];
        for (int i = 0; i < s.length(); i++) {
            char c = Character.toLowerCase(s.charAt(i));
            if (c >= 'a' && c <= 'z') counts[c - 'a']++;
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 26; i++) if (counts[i] > 0) sb.append((char) ('a' + i)).append(counts[i]);
        return sb.toString();
    }

    static String reverseWords(String s) {
        String[] parts = s.trim().split(" ");
        StringBuilder sb = new StringBuilder();
        for (int i = parts.length - 1; i >= 0; i--) {
            if (parts[i].isEmpty()) continue;
            if (sb.length() > 0) sb.append(' ');
            sb.append(new StringBuilder(parts[i]).reverse());
        }
        return sb.toString();
    }

    static String sortedDigits(int v) {
        char[] cs = Integer.toString(v).toCharArray();
        Arrays.sort(cs);
        return new String(cs);
    }

    static int mix(int a, int b) {
        int h = a * 0x9E3779B1 + (b << 7) - (b >>> 3) + (a >> 5);
        return h ^ (h >>> 16);
    }

    static long mixLong(long a, int s) {
        return (a << s) ^ (a >>> (s + 3)) ^ (a * 1000003L);
    }

    static int safeDiv(int a, int b) {
        try {
            return a / b + a % b;
        } catch (ArithmeticException e) {
            return Integer.MIN_VALUE;
        }
    }

    static int saturate(double d) {
        return (int) (d * 1e9);
    }

    static int firstPair(int n) {
        int found = -1;
        outer:
        for (int i = 1; i < 50; i++) {
            for (int j = i + 1; j < 50; j++) {
                if ((i * j) % (Math.abs(n % 97) + 3) == 0 && i + j > 10) {
                    found = i * 100 + j;
                    break outer;
                }
                if (j > i + 20) continue outer;
            }
        }
        return found;
    }

    static boolean isPalindrome(String s) {
        int i = 0, j = s.length() - 1;
        while (i < j) {
            while (i < j && !Character.isLetterOrDigit(s.charAt(i))) i++;
            while (i < j && !Character.isLetterOrDigit(s.charAt(j))) j--;
            if (Character.toLowerCase(s.charAt(i)) != Character.toLowerCase(s.charAt(j))) return false;
            i++;
            j--;
        }
        return true;
    }

    static String caesar(String s, int k) {
        StringBuilder sb = new StringBuilder();
        int shift = Math.floorMod(k, 26);
        for (char c : s.toCharArray()) {
            if (Character.isUpperCase(c)) sb.append((char) ('A' + (c - 'A' + shift) % 26));
            else if (Character.isLowerCase(c)) sb.append((char) ('a' + (c - 'a' + shift) % 26));
            else sb.append(c);
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        String[] lines = POEM.split("\n");
        System.out.println(lines.length + " lines, " + vowels(POEM) + " vowels");
        for (String line : lines) System.out.println(line.length() + ": " + reverseWords(line));
        System.out.println(histogram(POEM));
        System.out.println(sortedDigits(907153) + " " + mix(12345, 678) + " " + mixLong(-987654321L, 9));
        System.out.println(safeDiv(17, 5) + " " + safeDiv(3, 0) + " " + safeDiv(Integer.MIN_VALUE, -1));
        System.out.println(saturate(3.5) + " " + saturate(-1e300) + " " + saturate(Double.NaN));
        System.out.println(firstPair(12) + " " + isPalindrome("A man, a plan, a canal: Panama") + " " + caesar("Hello, World", 29));
        int x = Integer.MAX_VALUE;
        x++;
        long big = Long.MAX_VALUE;
        big += 2;
        System.out.println(x + " " + big + " " + (char) 65 + (int) 'z' + " " + (byte) 200 + " " + (short) 70000);
        System.out.println(1.0 / 3 + " " + 100.0 + " " + 1e-5 + " " + 12345678.9 + " " + (float) 0.1 + " " + -0.0);
    }
}
