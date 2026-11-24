public class Errs {
    static class InsufficientFunds extends Exception {
        private final int shortBy;
        InsufficientFunds(String msg, int shortBy) { super(msg); this.shortBy = shortBy; }
        int getShortBy() { return shortBy; }
    }
    static class Account {
        private int balance;
        Account(int b) { balance = b; }
        void withdraw(int n) throws InsufficientFunds {
            if (n > balance) throw new InsufficientFunds("need more", n - balance);
            balance -= n;
        }
        int getBalance() { return balance; }
    }

    static int parse(String s) {
        try {
            return Integer.parseInt(s);
        } catch (NumberFormatException e) {
            System.out.println("bad number: " + e.getMessage());
            return -1;
        } finally {
            System.out.println("parsed " + s);
        }
    }

    static int depth(int n) {
        try {
            if (n == 0) throw new IllegalStateException("bottom");
            return depth(n - 1) + 1;
        } finally {
            if (n % 2 == 0) System.out.println("unwind " + n);
        }
    }

    static String risky(int kind) {
        try {
            switch (kind) {
                case 0: return "fine";
                case 1: int[] a = new int[2]; a[5] = 1; return "no";
                case 2: Object o = null; return o.toString();
                case 3: return "" + (10 / (kind - 3));
                case 4: throw new UnsupportedOperationException("nope");
                default: String s = null; return s.trim();
            }
        } catch (ArrayIndexOutOfBoundsException e) {
            return "AIOOBE " + e.getMessage();
        } catch (NullPointerException | ArithmeticException e) {
            return "multi " + (e instanceof NullPointerException);
        } catch (RuntimeException e) {
            return "RTE " + e.getMessage();
        }
    }

    public static void main(String[] args) {
        System.out.println(parse("123"));
        System.out.println(parse("12x"));
        Account acc = new Account(100);
        try {
            acc.withdraw(30);
            acc.withdraw(100);
            System.out.println("not reached");
        } catch (InsufficientFunds e) {
            System.out.println(e.getMessage() + " by " + e.getShortBy());
        }
        for (int k = 0; k < 6; k++) System.out.println(k + ": " + risky(k));
        try {
            System.out.println(depth(4));
        } catch (IllegalStateException e) {
            System.out.println("caught " + e);
        }
        int count = 0;
        for (int i = 0; i < 5; i++) {
            try {
                if (i == 2) continue;
                count += i;
            } finally {
                count += 100;
            }
        }
        System.out.println(count);
        throw new RuntimeException("fatal: " + acc.getBalance());
    }
}
