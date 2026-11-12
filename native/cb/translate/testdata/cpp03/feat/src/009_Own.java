import java.util.*;

public class Own {
    public Box bb;
    public Box[] list;
    public List<Box> held = new ArrayList<Box>();

    public Own() { this.bb = new Box(0, 0); }
    public void setPos(int a, int b) { this.bb = new Box(a, b); }
    public void reset() { this.bb = Box.create(1); this.bb = Box.make(2); this.bb = new Box(3, 4); int q = 0; }
    public void twice() { this.bb = new Box(1, 1); this.bb = new Box(2, 2); }
    public void other(Box b) { this.bb = b; }
    public List<Box> cubes(Box area) { List<Box> out = new ArrayList<Box>(); out.add(new Box(1, 2)); return out; }
    public Box grow(int d) { return new Box(d, d); }

    public int use(Box area, Sink sink) {
        List<Box> c = this.cubes(area);
        int n = c.size();
        c.get(0).touch();
        int h = c.get(0).x;
        sink.take(this.grow(1));
        sink.keep(this.grow(2));
        new Box(5, 6).touch();
        this.list[0].touch();
        list[n].touch();
        return n + h;
    }

    public int use2(Box area) {
        List<Box> c = this.cubes(area);
        Box first = c.get(0);
        return first.x;
    }

    public void leakTemp() {
        new Sink().run();
        new Box(1, 2).touch();
        Thread t = new Thread();
        new Sink().keep(null);
    }
}
