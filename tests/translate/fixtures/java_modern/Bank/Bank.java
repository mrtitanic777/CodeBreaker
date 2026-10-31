import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

// Classes, abstract methods, virtual calls (also from a constructor), custom exceptions, try/catch/finally,
// HashMap / LinkedHashMap / TreeMap iteration order, StringBuilder, String.format, static state.
public class Bank {
    static class InsufficientFunds extends Exception {
        final int shortBy;

        InsufficientFunds(String msg, int shortBy) {
            super(msg);
            this.shortBy = shortBy;
        }
    }

    abstract static class Account {
        static int opened = 0;
        protected final String id;
        protected long balance;
        protected final List<String> log = new ArrayList<>();

        Account(String id, long initial) {
            this.id = id;
            this.balance = initial;
            opened++;
            log.add("open:" + kind());
        }

        abstract String kind();

        abstract long fee(long amount);

        void withdraw(long amount) throws InsufficientFunds {
            long total = amount + fee(amount);
            if (total > balance) throw new InsufficientFunds("account " + id + " short", (int) (total - balance));
            balance -= total;
            log.add("w:" + amount);
        }

        void deposit(long amount) {
            if (amount <= 0) throw new IllegalArgumentException("bad amount " + amount);
            balance += amount;
            log.add("d:" + amount);
        }

        @Override
        public String toString() {
            return String.format("%s[%s] %d", kind(), id, balance);
        }
    }

    static class Checking extends Account {
        Checking(String id, long initial) { super(id, initial); }

        String kind() { return "checking"; }

        long fee(long amount) { return amount > 1000 ? 2 : 1; }
    }

    static class Savings extends Account {
        private final int rate;

        Savings(String id, long initial, int rate) {
            super(id, initial);
            this.rate = rate;
        }

        String kind() { return "savings"; }

        long fee(long amount) { return amount / 100; }

        void accrue() { balance += balance * rate / 1000; }
    }

    static long simulate(int seed) {
        long x = seed;
        Checking c = new Checking("c" + seed, 500);
        Savings s = new Savings("s" + seed, 2000, 15);
        long failures = 0;
        for (int i = 0; i < 25; i++) {
            x = (x * 6364136223846793005L + 1442695040888963407L);
            long amt = (x >>> 33) % 700;
            try {
                if ((x & 1) == 0) c.withdraw(amt); else s.withdraw(amt);
            } catch (InsufficientFunds e) {
                failures += e.shortBy;
            } finally {
                s.accrue();
            }
            if (i % 5 == 0) c.deposit(amt + 1);
        }
        return c.balance * 31 + s.balance + failures * 7;
    }

    static String wordFreq(String text) {
        Map<String, Integer> counts = new HashMap<>();
        for (String w : text.split(" ")) {
            if (w.isEmpty()) continue;
            counts.merge(w, 1, Integer::sum);
        }
        StringBuilder sb = new StringBuilder();
        for (Map.Entry<String, Integer> e : counts.entrySet()) sb.append(e.getKey()).append('=').append(e.getValue()).append(' ');
        return sb.toString().trim();
    }

    static String ordered(int n) {
        Map<Integer, String> lm = new LinkedHashMap<>();
        Map<Integer, String> tm = new TreeMap<>();
        for (int i = 0; i < n % 10; i++) {
            int k = (i * 7 + 3) % 11;
            lm.put(k, "v" + i);
            tm.put(k, "v" + i);
        }
        return lm.keySet() + "|" + tm.values();
    }

    static int hashOrder(int n) {
        Map<String, Integer> m = new HashMap<>();
        for (int i = 0; i < n % 40; i++) m.put("key" + (i * 13 % 17), i);
        int h = 0;
        for (String k : m.keySet()) h = h * 31 + k.hashCode();
        return h;
    }

    public static void main(String[] args) {
        Checking c = new Checking("A1", 100);
        Savings s = new Savings("B2", 5000, 20);
        System.out.println(c + " " + s + " opened=" + Account.opened);
        try {
            c.withdraw(50);
            c.withdraw(80);
        } catch (InsufficientFunds e) {
            System.out.println("failed: " + e.getMessage() + " by " + e.shortBy);
        } finally {
            System.out.println("after: " + c);
        }
        try {
            s.deposit(-5);
        } catch (IllegalArgumentException e) {
            System.out.println("rejected " + e.getMessage());
        }
        s.accrue();
        System.out.println(s + " " + c.log + " " + s.log);
        System.out.println(simulate(42) + " " + simulate(7));
        System.out.println(wordFreq("to be or not to be that is the question to be"));
        System.out.println(ordered(9) + " " + hashOrder(33));
    }
}
