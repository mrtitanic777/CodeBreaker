public class InnerOuter {
    int count = 3;

    // an inner (non-static) class that reaches its outer instance: declined
    class Counter {
        int twice() { return count * 2; }
    }

    static int plain(int x) { return x + 1; }
}
