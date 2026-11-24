public class SwitchTry {
    static String run(int k) {
        String out = "";
        for (int i = 0; i < 3; i++) {
            try {
                switch (k + i) {
                    case 1: out += "a"; break;
                    case 2: out += "b"; continue;
                    default: out += "c";
                }
                out += "|";
            } finally {
                out += i;
            }
        }
        return out;
    }

    public static void main(String[] args) {
        System.out.println(run(0) + " " + run(1));
    }
}
