package java.util.regex;

// Regular expressions as a small backtracking matcher: literals, ., classes ([a-z], \d \w \s and their opposites), anchors (^ $ \b \B),
// groups ((...) and (?:...)), alternation, and the greedy and lazy ? * + {n,m}. It is written once in Java and goes to every language with the program.
public final class Pattern {
    public static final int CASE_INSENSITIVE = 2;

    // node kinds
    static final int ACCEPT = 0;
    static final int CHAR = 1;
    static final int ANY = 2;
    static final int SET = 3;
    static final int BOL = 4;
    static final int EOL = 5;
    static final int WORDB = 6;
    static final int NOTWORDB = 7;
    static final int OPEN = 8;
    static final int CLOSE = 9;
    static final int ALT = 10;
    static final int LOOP = 11;
    static final int LOOPEND = 12;
    static final int EMPTY = 13;

    // syntax kinds, before they are built into nodes
    private static final int S_SEQ = 100;
    private static final int S_ALT = 101;
    private static final int S_GROUP = 102;
    private static final int S_REPEAT = 103;
    private static final int S_ATOM = 104;

    static final class Node {
        int kind;
        Node next;
        int ch;
        int[] ranges;      // SET: pairs of first and last character
        boolean negated;
        int group;         // OPEN, CLOSE
        Node[] alts;       // ALT
        Node body;         // LOOP
        Node loop;         // LOOPEND
        int min;
        int max;
        boolean greedy;
        int id;

        Node(int kind, Node next) {
            this.kind = kind;
            this.next = next;
        }
    }

    private static final class Syntax {
        int kind;
        Node atom;         // S_ATOM: a node to copy, its next is set when built
        Syntax[] kids;
        int group;
        int min;
        int max;
        boolean greedy;
    }

    private final String source;
    private final int flags;
    private int at;
    private int groups;
    private int loops;
    Node start;

    private Pattern(String regex, int flags) {
        this.source = regex;
        this.flags = flags;
        Syntax tree = alternation();
        if (at < source.length()) {
            if (source.charAt(at) == ')') throw error("Unmatched closing ')'", at - 1 < 0 ? 0 : at - 1);
            throw error("Unexpected character", at);
        }
        start = build(tree, new Node(ACCEPT, null));
    }

    public static Pattern compile(String regex) {
        return new Pattern(regex, 0);
    }

    public static Pattern compile(String regex, int flags) {
        return new Pattern(regex, flags);
    }

    public static boolean matches(String regex, CharSequence input) {
        return compile(regex).matcher(input).matches();
    }

    public Matcher matcher(CharSequence input) {
        return new Matcher(this, input.toString());
    }

    public String pattern() {
        return source;
    }

    public int flags() {
        return flags;
    }

    @Override public String toString() {
        return source;
    }

    int groupCount() {
        return groups;
    }

    int loopCount() {
        return loops;
    }

    boolean ignoresCase() {
        return (flags & CASE_INSENSITIVE) != 0;
    }

    public String[] split(CharSequence input) {
        String text = input.toString();
        Matcher m = matcher(text);
        String[] parts = new String[8];
        int count = 0;
        int index = 0;
        while (m.find()) {
            if (m.end() == 0) continue;   // (a match of nothing at the start makes no empty first piece)
            if (count == parts.length) {
                String[] more = new String[count * 2];
                for (int i = 0; i < count; i++) more[i] = parts[i];
                parts = more;
            }
            parts[count++] = text.substring(index, m.start());
            index = m.end();
        }
        if (index == 0) return new String[] {text};
        if (count == parts.length) {
            String[] more = new String[count + 1];
            for (int i = 0; i < count; i++) more[i] = parts[i];
            parts = more;
        }
        parts[count++] = text.substring(index);
        while (count > 0 && parts[count - 1].length() == 0) count--;
        String[] out = new String[count];
        for (int i = 0; i < count; i++) out[i] = parts[i];
        return out;
    }

    // ---- reading the expression

    private PatternSyntaxException error(String what, int index) {
        return new PatternSyntaxException(what, source, index);
    }

    private Syntax seq(Syntax[] kids) {
        Syntax s = new Syntax();
        s.kind = S_SEQ;
        s.kids = kids;
        return s;
    }

    private Syntax alternation() {
        Syntax[] branches = new Syntax[4];
        int count = 0;
        while (true) {
            Syntax one = sequence();
            if (count == branches.length) {
                Syntax[] more = new Syntax[count * 2];
                for (int i = 0; i < count; i++) more[i] = branches[i];
                branches = more;
            }
            branches[count++] = one;
            if (at < source.length() && source.charAt(at) == '|') {
                at++;
                continue;
            }
            break;
        }
        if (count == 1) return branches[0];
        Syntax[] kids = new Syntax[count];
        for (int i = 0; i < count; i++) kids[i] = branches[i];
        Syntax s = new Syntax();
        s.kind = S_ALT;
        s.kids = kids;
        return s;
    }

    private Syntax sequence() {
        Syntax[] items = new Syntax[8];
        int count = 0;
        while (at < source.length()) {
            char c = source.charAt(at);
            if (c == '|' || c == ')') break;
            Syntax item = repeated(atom());
            if (count == items.length) {
                Syntax[] more = new Syntax[count * 2];
                for (int i = 0; i < count; i++) more[i] = items[i];
                items = more;
            }
            items[count++] = item;
        }
        Syntax[] kids = new Syntax[count];
        for (int i = 0; i < count; i++) kids[i] = items[i];
        return seq(kids);
    }

    private Syntax repeated(Syntax item) {
        while (at < source.length()) {
            char c = source.charAt(at);
            int min;
            int max;
            if (c == '*') {
                min = 0;
                max = -1;
                at++;
            } else if (c == '+') {
                min = 1;
                max = -1;
                at++;
            } else if (c == '?') {
                min = 0;
                max = 1;
                at++;
            } else if (c == '{') {
                int open = at;
                at++;
                int lo = number();
                if (lo < 0) throw error("Illegal repetition", open);
                min = lo;
                max = lo;
                if (at < source.length() && source.charAt(at) == ',') {
                    at++;
                    max = number();
                }
                if (at >= source.length() || source.charAt(at) != '}') throw error("Unclosed counted closure", at);
                at++;
            } else {
                break;
            }
            boolean greedy = true;
            if (at < source.length() && source.charAt(at) == '?') {
                greedy = false;
                at++;
            } else if (at < source.length() && source.charAt(at) == '+') {
                throw error("Possessive quantifiers are not supported", at);
            }
            Syntax r = new Syntax();
            r.kind = S_REPEAT;
            r.kids = new Syntax[] {item};
            r.min = min;
            r.max = max;
            r.greedy = greedy;
            item = r;
        }
        return item;
    }

    private int number() {
        int from = at;
        int v = 0;
        while (at < source.length() && source.charAt(at) >= '0' && source.charAt(at) <= '9') {
            v = v * 10 + (source.charAt(at) - '0');
            at++;
        }
        return at == from ? -1 : v;
    }

    private Syntax atomOf(Node n) {
        Syntax s = new Syntax();
        s.kind = S_ATOM;
        s.atom = n;
        return s;
    }

    private Syntax atom() {
        char c = source.charAt(at);
        if (c == '(') {
            int open = at;
            at++;
            int group = 0;
            if (at + 1 < source.length() && source.charAt(at) == '?' && source.charAt(at + 1) == ':') {
                at += 2;
            } else if (at < source.length() && source.charAt(at) == '?') {
                throw error("Unknown group type", at);
            } else {
                group = ++groups;
            }
            Syntax inside = alternation();
            if (at >= source.length() || source.charAt(at) != ')') throw error("Unclosed group", source.length());
            at++;
            Syntax g = new Syntax();
            g.kind = S_GROUP;
            g.kids = new Syntax[] {inside};
            g.group = group;
            return g;
        }
        if (c == '*' || c == '+' || c == '?') throw error("Dangling meta character '" + c + "'", at);
        at++;
        if (c == '.') return atomOf(new Node(ANY, null));
        if (c == '^') return atomOf(new Node(BOL, null));
        if (c == '$') return atomOf(new Node(EOL, null));
        if (c == '[') return atomOf(charClass());
        if (c == '\\') return escape();
        return literal(c);
    }

    private Syntax literal(int c) {
        Node n = new Node(CHAR, null);
        n.ch = c;
        return atomOf(n);
    }

    private Syntax escape() {
        if (at >= source.length()) throw error("Unexpected internal error", at);
        char c = source.charAt(at++);
        if (c == 'b') return atomOf(new Node(WORDB, null));
        if (c == 'B') return atomOf(new Node(NOTWORDB, null));
        if (c == 'd' || c == 'D' || c == 'w' || c == 'W' || c == 's' || c == 'S') {
            Node n = new Node(SET, null);
            n.ranges = predefined(c);
            return atomOf(n);
        }
        if (c >= '1' && c <= '9') throw error("Back references are not supported", at - 1);
        return literal(escaped(c));
    }

    private int escaped(char c) {
        if (c == 't') return '\t';
        if (c == 'n') return '\n';
        if (c == 'r') return '\r';
        if (c == 'f') return '\f';
        if (c == '0') return 0;
        if (c == 'u') {
            if (at + 4 > source.length()) throw error("Illegal Unicode escape sequence", at);
            int v = 0;
            for (int i = 0; i < 4; i++) {
                int d = hex(source.charAt(at + i));
                if (d < 0) throw error("Illegal Unicode escape sequence", at + i);
                v = v * 16 + d;
            }
            at += 4;
            return v;
        }
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) throw error("Illegal/unsupported escape sequence", at - 1);
        return c;
    }

    private int hex(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    }

    // \d \w \s as ranges; the capital letters are the complement
    private int[] predefined(char c) {
        int[] base;
        if (c == 'd' || c == 'D') base = new int[] {'0', '9'};
        else if (c == 'w' || c == 'W') base = new int[] {'0', '9', 'A', 'Z', '_', '_', 'a', 'z'};
        else base = new int[] {'\t', '\r', ' ', ' '};
        if (c == 'd' || c == 'w' || c == 's') return base;
        int[] out = new int[base.length + 2];
        int n = 0;
        int next = 0;
        for (int i = 0; i < base.length; i += 2) {
            if (base[i] > next) {
                out[n++] = next;
                out[n++] = base[i] - 1;
            }
            next = base[i + 1] + 1;
        }
        out[n++] = next;
        out[n++] = 65535;
        int[] fit = new int[n];
        for (int i = 0; i < n; i++) fit[i] = out[i];
        return fit;
    }

    private Node charClass() {
        Node n = new Node(SET, null);
        int open = at - 1;
        if (at < source.length() && source.charAt(at) == '^') {
            n.negated = true;
            at++;
        }
        int[] ranges = new int[16];
        int count = 0;
        boolean first = true;
        while (true) {
            if (at >= source.length()) throw error("Unclosed character class", source.length() - 1);
            char c = source.charAt(at);
            if (c == ']' && !first) {
                at++;
                break;
            }
            first = false;
            at++;
            int lo;
            if (c == '\\') {
                if (at >= source.length()) throw error("Unclosed character class", source.length() - 1);
                char e = source.charAt(at++);
                if (e == 'd' || e == 'D' || e == 'w' || e == 'W' || e == 's' || e == 'S') {
                    int[] more = predefined(e);
                    while (count + more.length > ranges.length) ranges = grown(ranges);
                    for (int i = 0; i < more.length; i++) ranges[count++] = more[i];
                    continue;
                }
                lo = escaped(e);
            } else if (c == '[') {
                throw error("Nested character classes are not supported", at - 1);
            } else {
                lo = c;
            }
            int hi = lo;
            if (at + 1 < source.length() && source.charAt(at) == '-' && source.charAt(at + 1) != ']') {
                at++;
                char h = source.charAt(at++);
                if (h == '\\') {
                    if (at >= source.length()) throw error("Unclosed character class", source.length() - 1);
                    hi = escaped(source.charAt(at++));
                } else {
                    hi = h;
                }
                if (hi < lo) throw error("Illegal character range", at - 1);
            }
            if (count + 2 > ranges.length) ranges = grown(ranges);
            ranges[count++] = lo;
            ranges[count++] = hi;
        }
        int[] fit = new int[count];
        for (int i = 0; i < count; i++) fit[i] = ranges[i];
        n.ranges = fit;
        return n;
    }

    private int[] grown(int[] a) {
        int[] more = new int[a.length * 2];
        for (int i = 0; i < a.length; i++) more[i] = a[i];
        return more;
    }

    // ---- building nodes, from the last item back so that each knows what follows it

    private Node copyOf(Node a, Node next) {
        Node n = new Node(a.kind, next);
        n.ch = a.ch;
        n.ranges = a.ranges;
        n.negated = a.negated;
        return n;
    }

    private Node build(Syntax s, Node next) {
        if (s.kind == S_ATOM) return copyOf(s.atom, next);
        if (s.kind == S_SEQ) {
            Node n = next;
            for (int i = s.kids.length - 1; i >= 0; i--) n = build(s.kids[i], n);
            return n;
        }
        if (s.kind == S_ALT) {
            Node alt = new Node(ALT, next);
            alt.alts = new Node[s.kids.length];
            for (int i = 0; i < s.kids.length; i++) alt.alts[i] = build(s.kids[i], next);
            return alt;
        }
        if (s.kind == S_GROUP) {
            if (s.group == 0) return build(s.kids[0], next);
            Node close = new Node(CLOSE, next);
            close.group = s.group;
            Node open = new Node(OPEN, null);
            open.group = s.group;
            open.next = build(s.kids[0], close);
            return open;
        }
        Node loop = new Node(LOOP, next);
        loop.min = s.min;
        loop.max = s.max;
        loop.greedy = s.greedy;
        loop.id = loops++;
        Node end = new Node(LOOPEND, null);
        end.loop = loop;
        loop.body = build(s.kids[0], end);
        return loop;
    }
}
