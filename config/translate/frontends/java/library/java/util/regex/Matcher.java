package java.util.regex;

// Runs a Pattern over a text by backtracking: each node tries itself, then the rest of the pattern, and gives its captures back when the rest fails.
public final class Matcher {
    private final Pattern pattern;
    private String text;
    private int length;
    private int[] starts;
    private int[] ends;
    private int[] counts;
    private int[] lastAt;
    private boolean wholeText;
    private int matchEnd;
    private boolean matched;
    private int first = -1;
    private int last;

    Matcher(Pattern pattern, String text) {
        this.pattern = pattern;
        this.text = text;
        this.length = text.length();
        int groups = pattern.groupCount() + 1;
        starts = new int[groups];
        ends = new int[groups];
        counts = new int[pattern.loopCount()];
        lastAt = new int[pattern.loopCount()];
        clear();
    }

    private void clear() {
        for (int i = 0; i < starts.length; i++) {
            starts[i] = -1;
            ends[i] = -1;
        }
    }

    public Pattern pattern() {
        return pattern;
    }

    public Matcher reset() {
        first = -1;
        last = 0;
        matched = false;
        clear();
        return this;
    }

    public Matcher reset(CharSequence input) {
        text = input.toString();
        length = text.length();
        return reset();
    }

    public boolean matches() {
        return attempt(0, true);
    }

    public boolean lookingAt() {
        return attempt(0, false);
    }

    public boolean find() {
        int from = last;
        if (from == first) from++;
        if (from > length) {
            matched = false;
            first = -1;
            return false;
        }
        return search(from);
    }

    public boolean find(int from) {
        if (from < 0 || from > length) throw new IndexOutOfBoundsException("Illegal start index");
        reset();
        return search(from);
    }

    private boolean search(int from) {
        for (int s = from; s <= length; s++) {
            if (attempt(s, false)) return true;
        }
        matched = false;
        first = -1;
        return false;
    }

    private boolean attempt(int at, boolean whole) {
        clear();
        wholeText = whole;
        if (run(pattern.start, at)) {
            starts[0] = at;
            ends[0] = matchEnd;
            first = at;
            last = matchEnd;
            matched = true;
            return true;
        }
        matched = false;
        first = -1;
        return false;
    }

    public int groupCount() {
        return pattern.groupCount();
    }

    private void need() {
        if (!matched) throw new IllegalStateException("No match found");
    }

    private void checkGroup(int group) {
        need();
        if (group < 0 || group > pattern.groupCount()) throw new IndexOutOfBoundsException("No group " + group);
    }

    public String group() {
        return group(0);
    }

    public String group(int group) {
        checkGroup(group);
        if (starts[group] < 0 || ends[group] < 0) return null;
        return text.substring(starts[group], ends[group]);
    }

    public int start() {
        return start(0);
    }

    public int start(int group) {
        checkGroup(group);
        return ends[group] < 0 ? -1 : starts[group];
    }

    public int end() {
        return end(0);
    }

    public int end(int group) {
        checkGroup(group);
        return ends[group] < 0 ? -1 : ends[group];
    }

    public String replaceAll(String replacement) {
        reset();
        StringBuilder out = new StringBuilder();
        int copied = 0;
        while (find()) {
            out.append(text.substring(copied, start()));
            expand(out, replacement);
            copied = end();
        }
        out.append(text.substring(copied, length));
        return out.toString();
    }

    public String replaceFirst(String replacement) {
        reset();
        if (!find()) return text;
        StringBuilder out = new StringBuilder();
        out.append(text.substring(0, start()));
        expand(out, replacement);
        out.append(text.substring(end(), length));
        return out.toString();
    }

    // the replacement text: \x is x, $n is the nth group
    private void expand(StringBuilder out, String replacement) {
        int i = 0;
        while (i < replacement.length()) {
            char c = replacement.charAt(i++);
            if (c == '\\') {
                if (i >= replacement.length()) throw new IllegalArgumentException("character to be escaped is missing");
                out.append(replacement.charAt(i++));
            } else if (c == '$') {
                if (i >= replacement.length() || replacement.charAt(i) < '0' || replacement.charAt(i) > '9')
                    throw new IllegalArgumentException("Illegal group reference");
                int g = replacement.charAt(i++) - '0';
                if (g > pattern.groupCount()) throw new IndexOutOfBoundsException("No group " + g);
                while (i < replacement.length() && replacement.charAt(i) >= '0' && replacement.charAt(i) <= '9') {
                    int more = g * 10 + (replacement.charAt(i) - '0');
                    if (more > pattern.groupCount()) break;
                    g = more;
                    i++;
                }
                String part = group(g);
                if (part != null) out.append(part);
            } else {
                out.append(c);
            }
        }
    }

    // ---- the search

    private static boolean isWord(int c) {
        return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    private boolean wordBoundary(int i) {
        boolean before = i > 0 && isWord(text.charAt(i - 1));
        boolean after = i < length && isWord(text.charAt(i));
        return before != after;
    }

    private static int lower(int c) {
        return c >= 'A' && c <= 'Z' ? c + 32 : c;
    }

    private static int upper(int c) {
        return c >= 'a' && c <= 'z' ? c - 32 : c;
    }

    private boolean inRanges(int[] ranges, int c) {
        for (int k = 0; k < ranges.length; k += 2) {
            if (c >= ranges[k] && c <= ranges[k + 1]) return true;
        }
        return false;
    }

    private boolean inSet(Pattern.Node n, int c) {
        boolean in = inRanges(n.ranges, c);
        if (!in && pattern.ignoresCase()) in = inRanges(n.ranges, lower(c)) || inRanges(n.ranges, upper(c));
        return in != n.negated;
    }

    private boolean run(Pattern.Node n, int i) {
        while (true) {
            int kind = n.kind;
            if (kind == Pattern.ACCEPT) {
                if (wholeText && i != length) return false;
                matchEnd = i;
                return true;
            } else if (kind == Pattern.CHAR) {
                if (i >= length) return false;
                int c = text.charAt(i);
                if (c != n.ch && !(pattern.ignoresCase() && lower(c) == lower(n.ch))) return false;
                i++;
                n = n.next;
            } else if (kind == Pattern.ANY) {
                if (i >= length) return false;
                int c = text.charAt(i);
                if (c == 10 || c == 13 || c == 0x85 || c == 0x2028 || c == 0x2029) return false;
                i++;
                n = n.next;
            } else if (kind == Pattern.SET) {
                if (i >= length || !inSet(n, text.charAt(i))) return false;
                i++;
                n = n.next;
            } else if (kind == Pattern.BOL) {
                if (i != 0) return false;
                n = n.next;
            } else if (kind == Pattern.EOL) {
                boolean atEnd = i == length || (i == length - 1 && text.charAt(i) == '\n') || (i == length - 2 && text.charAt(i) == '\r' && text.charAt(i + 1) == '\n');
                if (!atEnd) return false;
                n = n.next;
            } else if (kind == Pattern.WORDB) {
                if (!wordBoundary(i)) return false;
                n = n.next;
            } else if (kind == Pattern.NOTWORDB) {
                if (wordBoundary(i)) return false;
                n = n.next;
            } else if (kind == Pattern.OPEN) {
                int g = n.group;
                int was = starts[g];
                int wasEnd = ends[g];
                starts[g] = i;
                ends[g] = -1;
                if (run(n.next, i)) return true;
                starts[g] = was;
                ends[g] = wasEnd;
                return false;
            } else if (kind == Pattern.CLOSE) {
                int g = n.group;
                int was = ends[g];
                ends[g] = i;
                if (run(n.next, i)) return true;
                ends[g] = was;
                return false;
            } else if (kind == Pattern.ALT) {
                for (int k = 0; k < n.alts.length; k++) {
                    if (run(n.alts[k], i)) return true;
                }
                return false;
            } else if (kind == Pattern.LOOP) {
                int id = n.id;
                int count = counts[id];
                int at = lastAt[id];
                counts[id] = 0;
                lastAt[id] = -1;
                if (iterate(n, i)) return true;
                counts[id] = count;
                lastAt[id] = at;
                return false;
            } else if (kind == Pattern.LOOPEND) {
                return iterate(n.loop, i);
            } else {
                return false;
            }
        }
    }

    // at the head of a loop that has gone round counts[id] times: once more, or on
    private boolean iterate(Pattern.Node n, int i) {
        int id = n.id;
        int count = counts[id];
        int at = lastAt[id];
        if (count < n.min) {
            counts[id] = count + 1;
            lastAt[id] = i;
            if (run(n.body, i)) return true;
            counts[id] = count;
            lastAt[id] = at;
            return false;
        }
        boolean more = (n.max < 0 || count < n.max) && i != at;
        if (n.greedy) {
            if (more) {
                counts[id] = count + 1;
                lastAt[id] = i;
                if (run(n.body, i)) return true;
                counts[id] = count;
                lastAt[id] = at;
            }
            return run(n.next, i);
        }
        if (run(n.next, i)) return true;
        if (more) {
            counts[id] = count + 1;
            lastAt[id] = i;
            if (run(n.body, i)) return true;
            counts[id] = count;
            lastAt[id] = at;
        }
        return false;
    }
}
