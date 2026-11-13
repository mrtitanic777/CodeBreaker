package crt;

import java.util.ArrayList;
import java.util.List;

/**
 * Function pointers: a C function whose address the program takes is registered once and its address
 * is its index here; a call through the pointer looks the function up and calls it through the
 * functional interface its type names. Address 0 stays NULL.
 */
public final class Fn {
    private Fn() {
    }

    private static final List<Object> TABLE = new ArrayList<>(List.of(new Object()));

    public static int register(Object f) {
        TABLE.add(f);
        return (TABLE.size() - 1) * 16;
    }

    public static Object at(int address) {
        if (address == 0 || address % 16 != 0 || address / 16 >= TABLE.size()) {
            throw new IllegalStateException("a call through a function pointer that names no function: " + address);
        }
        return TABLE.get(address / 16);
    }
}
