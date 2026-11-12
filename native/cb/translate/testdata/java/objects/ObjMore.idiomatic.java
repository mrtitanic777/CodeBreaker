// ObjMore.java -- translated from C by CodeBreaker (idiomatic tier: structs as classes, pointers as references).
import crt.*;

import static crt.LibC.*;

public final class ObjMore {

    /** struct Node */
    static class Node {
        int v;
        Node next;

        Node() {
        }

        Node(int v, Node next) {
            this.v = v;
            this.next = next;
        }
    }

    /** struct Grid */
    static class Grid {
        int[][] cells = new int[2][3];
        Node[] nodes = filled(new Node[2], Node::new);

        Grid() {
        }

        Grid(int[][] cells, Node[] nodes) {
            this.cells = cells;
            this.nodes = nodes;
        }
    }

    interface IntToInt {
        int call(int a0);
    }

    static final Node[] pool = filled(new Node[2], Node::new);
    static int mask = 255;

    static int twice(int x) {
        return x * 2;
    }

    static Node push(Node head, int v) {
        Node n = new Node();
        n.v = v;
        n.next = head;
        return n;
    }

    static int sum(Node h) {
        int t = 0;
        while (h != null) {
            t += h.v;
            h = h.next;
        }
        return t;
    }

    static int main() {
        byte[] msg = bytes("hi", 8);
        byte[] text = bytes("static");
        Grid g = new Grid(new int[][]{new int[]{1, 2, 3}, new int[]{4, 5, 6}}, new Node[]{new Node(7, null), new Node(8, null)});
        g.cells[1][2] = 9;
        IntToInt op = ObjMore::twice;
        Node list = push(push(null, 1), 2);
        byte b = (byte) 200;
        long big = 1L << 40;
        double d = 1.5;
        pool[1].v = 3;
        printf("%s %d\n", text, mask);
        return sum(list) + op.call(g.cells[1][2]) + (int) strlen(text) + (((b & 0xFF) > 100) ? 1 : 0) + (int) (big >> 38) + (int) d + msg[0] + g.nodes[1].v + pool[1].v;
    }

    public static void main(String[] args) {
        exit(main());
    }
}
