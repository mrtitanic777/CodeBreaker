// codebreaker/invert/quad_dump.cpp -- C++17 port of codebreaker.invert.quad_dump (see quad_dump.hpp).

#include "codebreaker/invert/quad_dump.hpp"

#include "py_os.hpp"

namespace codebreaker::invert::quad_dump {

const py::str DOC = py::str(
    "quad_dump.py - structural parser for Lattice C 2.12 quad files (<base>.q).\n"
    "\n"
    "Usage: python quad_dump.py <file.q>\n"
    "\n"
    "Record framing (from LC2's reader @0x28c5 and confirmed by tiling every sample):\n"
    "  each quad record = [opcode:1][length:1][payload: `length` bytes]\n"
    "The reader reads 2 bytes (opcode,length) then `length` more, then parses up to 3\n"
    "operands from the payload. A function is a run of records starting at FUNC-BEGIN\n"
    "(0x23); the module ends at a 0xff terminator, after which come symbol trailer bytes\n"
    "and the NUL-terminated name pool.\n"
    "\n"
    "Opcode map (differentially verified via dynamic analysis; see\n"
    "re/notes/05-quad-format-and-harness.md, 07-quad-framing.md):\n");

const py::dict<int64_t, py::str> OPC = {
    // unary
    {0x0a, py::str("neg")},
    {0x0b, py::str("lnot")},
    {0x0c, py::str("compl")},
    // binary (0x10-0x1f)
    {0x10, py::str("mul")},
    {0x11, py::str("div")},
    {0x12, py::str("mod")},
    {0x13, py::str("add")},
    {0x14, py::str("sub")},
    {0x15, py::str("shl")},
    {0x16, py::str("shr")},
    {0x17, py::str("lt")},
    {0x18, py::str("le")},
    {0x19, py::str("gt")},
    {0x1a, py::str("ge")},
    {0x1b, py::str("eq")},
    {0x1c, py::str("ne")},
    {0x1d, py::str("and")},
    {0x1e, py::str("xor")},
    {0x1f, py::str("or")},
    // statements / structure (>=0x23)
    {0x23, py::str("FUNC-BEGIN")},
    {0x24, py::str("RETURN")},
    {0x20, py::str("CALL")},
    {0x2c, py::str("CBRANCH")},
    {0x2e, py::str("LABEL")},
    {0x2f, py::str("LOOP")},
    {0x3f, py::str("LABEL(named)")},
    {0x40, py::str("GOTO")},
};

int64_t namepool_start(const py::bytes& d) {
    int64_t i = py::len(d);
    while (i > 0 && ((32 <= d[i - 1] && d[i - 1] < 127) || d[i - 1] == 0)) {
        i -= 1;
    }
    return i;
}

py::str fmt_payload(const py::bytes& p) {
    // operands are packed little-endian words; show a compact view
    py::list<py::str> items;
    for (int64_t b : p) {
        items.append(py::percent("%02x", b));
    }
    return py::join(py::str(" "), items);
}

void main(const py::str& path) {
    py::bytes d;
    {
        py::file f = py::open_rb(path);
        d = f.read();
    }
    int64_t n = py::len(d);
    int64_t pool_off = namepool_start(d);
    py::list<py::str> names;
    for (const py::bytes& s : py::split(d.slice_from(pool_off), py::bytes(std::string(1, '\0')))) {
        if (!s.empty()) {
            names.append(s.decode("latin1"));
        }
    }
    py::print(py::str("== ") + path + py::str(" (") + py::to_str(n) + py::str(" bytes) =="));
    py::list<int64_t> type_size_tbl;
    for (int64_t i = 0; i < 10; ++i) {
        type_size_tbl.append(py::unpack_from1<int64_t>("<H", d, 4 + 2 * i));
    }
    {
        // (one print of two f-strings: nothing is written if reading either part raises)
        py::str line = py::str("0x00 magic=") + py::to_str(py::unpack_from1<int64_t>("<H", d, 0)) + py::str("  ") +
                       py::str("0x04 type-size-tbl=") + py::to_str(type_size_tbl);
        py::print(line);
    }
    {
        py::str line = py::str("0x4e symbolcount=") + py::to_str(py::unpack_from1<int64_t>("<H", d, 0x4e)) + py::str("  ") +
                       py::str("name-pool@0x") + py::format(pool_off, "x") + py::str("=") + py::to_str(names);
        py::print(line);
    }
    py::print(py::str("\n-- quad records  [off] opcode(len) : payload --"));
    int64_t o = 0x5c;
    bool broke = false;
    while (o + 2 <= pool_off) {
        int64_t op = d[o];
        int64_t ln = d[o + 1];
        if (op == 0xff) {   // module/function terminator (not a quad)
            py::print(py::str("  0x") + py::format(o, "03x") + py::str(": 0xff TERMINATOR (len=") + py::to_str(ln) +
                      py::str("); ") + py::to_str(pool_off - o) + py::str(" trailer bytes to name pool"));
            broke = true;
            break;
        }
        py::bytes payload = d.slice(o + 2, o + 2 + ln);
        py::str name = OPC.get(op, py::str("op") + py::format(op, "02x"));
        py::print(py::str("  0x") + py::format(o, "03x") + py::str(": ") + py::format(name, "<12") + py::str("(") +
                  py::format(ln, "2d") + py::str(") : ") + fmt_payload(payload));
        o += 2 + ln;
    }
    if (!broke) {
        py::print(py::str("  (reached 0x") + py::format(o, "x") + py::str("; name pool at 0x") + py::format(pool_off, "x") +
                  py::str(")"));
    }
}

int64_t cli(const py::list<py::str>& argv) {
    if (py::len(argv) < 2) {
        std::vector<int64_t> keys = OPC.keys();
        std::sort(keys.begin(), keys.end());
        py::list<py::str> items;
        for (int64_t k : keys) {
            items.append(py::format(k, "#04x") + py::str("=") + OPC.at(k));
        }
        py::print(DOC + py::str("\n  ") + py::join(py::str(", "), items));
        return 1;   // sys.exit(1)
    }
    main(argv[1]);
    return 0;
}

}  // namespace codebreaker::invert::quad_dump
