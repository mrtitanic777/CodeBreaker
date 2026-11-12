public class Sink {
    Box held;
    public void take(Box b) { int q = b.x; }
    public void keep(Box b) { this.held = b; }
    public void run() {}
}
