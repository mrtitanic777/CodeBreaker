import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Churn {
    static class Node {
        int value;
        Node next;
        Node(int value, Node next) { this.value = value; this.next = next; }
    }

    static Node keep = null;

    static long checksum(Node n) {
        long s = 0;
        for (; n != null; n = n.next) s += n.value;
        return s;
    }

    public static void main(String[] args) {
        for (int i = 0; i < 1000; i++) keep = new Node(i, keep);
        long expect = checksum(keep);
        Node[] ring = new Node[64];
        long total = 0;
        Map<Integer, String> table = new HashMap<>();
        for (int round = 0; round < 200000; round++) {
            int[] big = new int[5000];
            big[round % 5000] = round;
            total += big[round % 5000];
            Node n = new Node(round, null);
            ring[round % 64] = n;
            List<String> words = new ArrayList<>();
            for (int w = 0; w < 4; w++) words.add("w" + round + "_" + w);
            if (round % 1000 == 0) table.put(round, words.get(3));
            if (checksum(keep) != expect) { System.out.println("LOST"); return; }
        }
        long ringSum = 0;
        for (Node n : ring) ringSum += n.value;
        System.out.println(total + " " + ringSum + " " + checksum(keep) + " " + table.size() + " " + table.get(199000));
    }
}
