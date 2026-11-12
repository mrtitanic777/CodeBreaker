package t;

public class BadBlocks {
    static int a;
    static {
        a = 1;
    }
    static {
        a = 2;
    }
}
