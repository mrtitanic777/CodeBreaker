package java.util.regex;

public class PatternSyntaxException extends IllegalArgumentException {
    private final String desc;
    private final String pattern;
    private final int index;

    public PatternSyntaxException(String desc, String regex, int index) {
        this.desc = desc;
        this.pattern = regex;
        this.index = index;
    }

    public int getIndex() {
        return index;
    }

    public String getDescription() {
        return desc;
    }

    public String getPattern() {
        return pattern;
    }

    @Override public String getMessage() {
        StringBuilder b = new StringBuilder();
        b.append(desc);
        if (index >= 0) {
            b.append(" near index ");
            b.append(index);
        }
        b.append('\n');
        b.append(pattern);
        if (index >= 0) {
            b.append('\n');
            for (int i = 0; i < index; i++) b.append(' ');
            b.append('^');
        }
        return b.toString();
    }
}
