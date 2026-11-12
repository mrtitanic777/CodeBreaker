package t;
public class NoEscape {
    int x;
    public NoEscape() {
        this.x = 1;
        this.go();
        this ->go();
        this (2);
    }
    void go() {
    }
}
