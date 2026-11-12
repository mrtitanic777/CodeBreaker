public class Use {
    public static Kind k = Kind.A;
    public static Tool t = Tool.WOOD;
    public static int[] table = new int[]{1, 2, 3};
    public static int[][] t2 = {{1}, {2, 3}};
    public static String[] strs = new String[]{"a", null};
    public static Kind[] kinds = new Kind[]{Kind.A, Kind.B};
    public int legs = 4;

    static {
        table[0] = 9;
    }

    {
        legs++;
    }

    public int run(Kind kk) {
        int r = Kind.values().length + kk.ordinal() + Tool.WOOD.level() + Tool.pick(1).level();
        switch (kk) {
            case A: r++; break;
            case B: r += 2; break;
            default: r--;
        }
        Kind other = Kind.NULL;
        if (kk == other || kk == Kind.TRUE) r++;
        for (Tool tt : Tool.values()) r += tt.level();
        return r + table[1] + t2[1][1] + strs.length + kinds.length;
    }
}
