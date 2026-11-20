public enum Tool {
    WOOD(1, "w"), STONE(2, "s");
    private int lvl;
    private String nm;
    Tool(int l, String n) { lvl = l; nm = n; }
    int level() { return lvl; }
    static Tool pick(int i) { return values()[i]; }
}
