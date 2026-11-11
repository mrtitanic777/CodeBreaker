// codebreaker/invert/early_c_forward.cpp -- C++17 port of codebreaker.invert.early_c_forward (see early_c_forward.hpp).

#include "codebreaker/invert/early_c_forward.hpp"

#include <vector>

namespace codebreaker::invert::early_c_forward {

namespace quad_align = codebreaker::invert::quad_align;

const py::dict<int64_t, int64_t> ALU_MEM = {{0x13, 0x03}, {0x14, 0x2b}, {0x1d, 0x23}, {0x1f, 0x0b}, {0x1e, 0x33}};   // ax, [mem]  (mod=01 rm=110 [bp+d8] -> +0x46)
const py::dict<int64_t, int64_t> ALU_IMM = {{0x13, 0x05}, {0x14, 0x2d}, {0x1d, 0x25}, {0x1f, 0x0d}, {0x1e, 0x35}};   // ax, imm16
const py::dict<int64_t, int64_t> RELJCC = {{0x17, 0x7c}, {0x18, 0x7e}, {0x19, 0x7f}, {0x1a, 0x7d}, {0x1b, 0x74}, {0x1c, 0x75}};
const py::set<int64_t> BINCLASS = {0x13, 0x14, 0x1d, 0x1f, 0x1e, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x10, 0x11, 0x12, 0x15, 0x16};
const py::dict<int64_t, int64_t> INV_JCC = {{0x17, 0x7d}, {0x18, 0x7f}, {0x19, 0x7e}, {0x1a, 0x7c}, {0x1b, 0x75}, {0x1c, 0x74}};   // if(cond) -> skip if NOT cond

namespace {

// "%02x" % v
py::str hx(int64_t v) { return py::percent("%02x", v); }

// "%02x" % v for a value that may be None: the TypeError the Python raises for it
py::str hx(const std::optional<int64_t>& v) {
    if (!v) throw py::TypeError("%x format: an integer is required, not NoneType");
    return hx(*v);
}

// op["key"]: KeyError for a key the dict lacks
int64_t key(const std::optional<int64_t>& v) {
    if (!v) throw py::KeyError("key not found");
    return *v;
}

}  // namespace

py::bytes bs(std::initializer_list<py::str> h) {
    std::string o;
    for (const py::str& x : h) {
        o += py::bytes::fromhex(x.replace(py::str(" "), py::str(""))).raw();
    }
    return py::bytes(o);
}

py::bytes w16(int64_t v) {
    return bs({hx(v & 0xff), hx((v >> 8) & 0xff)});
}

// --- operand parsing / classification ------------------------------------
py::list<Operand> parse_operands(const py::bytes& payload) {
    py::list<Operand> ops;
    int64_t i = 0;
    while (i < py::len(payload)) {
        int64_t k = payload[i];
        if (k == 0x04 && i + 1 < py::len(payload) && payload[i + 1] == 0x80) {
            Operand o;
            o.k = py::str("const");
            o.val = payload[i + 3] | (payload[i + 4] << 8);
            ops.append(o);
            i += 7;
        } else if (k == 0x0a && i + 1 < py::len(payload) && payload[i + 1] == 0x00 && i + 11 <= py::len(payload)) {
            Operand o;
            o.k = py::str("node");
            o.id = payload[i + 3] | (payload[i + 4] << 8);
            o.ty = payload[i + 5];
            ops.append(o);
            i += 11;
        } else if (k == 0x0a && i + 1 < py::len(payload) && payload[i + 1] == 0x20 && i + 11 <= py::len(payload)) {
            Operand o;
            o.k = py::str("deref");
            o.id = payload[i + 3] | (payload[i + 4] << 8);
            o.off = payload[i + 9];
            ops.append(o);
            i += 11;   // deref = 11B, field offset at [+9]
        } else if (k == 0x0e && i + 15 <= py::len(payload)) {
            Operand o;
            o.k = py::str("temp");
            ops.append(o);
            i += 15;   // temp = 11B + 4B result back-link (XX 01 00 00)
        } else if (k == 0x00 && i + 1 < py::len(payload) && (payload[i + 1] == 0x04 || payload[i + 1] == 0x0a || payload[i + 1] == 0x0e)) {
            i += 1;   // skip 1-byte inter-operand separator
        } else {
            break;
        }
    }
    return ops;
}

bool Ctx::is_param(int64_t nid) const { return nid > fn; }

int64_t Ctx::frame(int64_t nid) const { return fs + 4 + psz * (nid - fn - 1); }

py::bytes emit_binop_long(int64_t opcode, const py::list<Operand>& srcs, const Ctx& c) {
    const Operand& a = srcs[0];
    const Operand& b = srcs[1];
    int64_t af = c.frame(key(a.id));
    int64_t bf = c.frame(key(b.id));   // long: lo=af, hi=af+2
    if (opcode == 0x13) {
        return bs({"8b46", hx(af), "0346", hx(bf), "8b5e", hx(af + 2), "135e", hx(bf + 2)});
    }
    if (opcode == 0x14) {
        return bs({"8b46", hx(af), "2b46", hx(bf), "8b5e", hx(af + 2), "1b5e", hx(bf + 2)});
    }
    if (opcode == 0x10 || opcode == 0x11) {   // mul/div via CX helper (call = reloc placeholder), result mov ax,bx
        return bs({"8b46", hx(af + 2), "8b5e", hx(af), "8b4e", hx(bf + 2), "8b56", hx(bf), "e8 0000", "8bc3"});
    }
    throw py::NotImplementedError("long binop " + hx(opcode).utf8());
}

// --- action emitters ------------------------------------------------------
py::bytes load_ax(const Operand& op, const Ctx& c) {
    if (op.k == py::str("const")) {
        int64_t v = key(op.val);
        return v == 0 ? bs({"33c0"}) : bs({"b8"}) + w16(v);
    }
    if (op.k == py::str("node")) {
        int64_t id = key(op.id);
        return c.is_param(id) ? bs({"8b46", hx(c.frame(id))}) : bs({"a1"}) + w16(0);
    }
    if (op.k == py::str("deref")) {   // ptr is a param -> si; then [si]/[si+off]
        py::bytes out = bs({"8b76", hx(c.frame(key(op.id)))});
        int64_t off = key(op.off);
        return out + (off == 0 ? bs({"8b04"}) : bs({"8b44", hx(off)}));
    }
    return py::bytes();   // temp already in ax
}

py::bytes store_ax(const Operand& op, const Ctx& c) {
    if (op.k == py::str("node")) {
        int64_t id = key(op.id);
        return !c.is_param(id) ? bs({"a3"}) + w16(0) : bs({"8946", hx(c.frame(id))});
    }
    if (op.k == py::str("deref")) {
        py::bytes out = bs({"8b76", hx(c.frame(key(op.id)))});
        int64_t off = key(op.off);
        return out + (off == 0 ? bs({"8904"}) : bs({"8944", hx(off)}));
    }
    throw py::NotImplementedError("store " + op.k.utf8());
}

py::bytes alu_ax(int64_t opcode, const Operand& op, const Ctx& c) {   // ax <op>= operand
    if (op.k == py::str("const")) {
        return bs({hx(ALU_IMM.at(opcode))}) + w16(key(op.val));
    }
    if (op.k == py::str("node") && c.is_param(key(op.id))) {
        return bs({hx(ALU_MEM.at(opcode)), "46", hx(c.frame(key(op.id)))});
    }
    if (op.k == py::str("node")) {
        return bs({hx(ALU_MEM.at(opcode)), "06"}) + w16(0);   // ax,[global]
    }
    throw py::NotImplementedError("alu operand " + op.k.utf8());
}

py::bytes emit_binop(int64_t opcode, const py::list<Operand>& srcs, const Ctx& c) {
    const Operand& a = srcs[0];
    const Operand& b = srcs[1];
    if (ALU_MEM.contains(opcode)) {
        return load_ax(a, c) + alu_ax(opcode, b, c);
    }
    std::optional<int64_t> af = (a.k == py::str("node") && c.is_param(key(a.id))) ? std::optional<int64_t>(c.frame(key(a.id))) : std::nullopt;
    std::optional<int64_t> bf = (b.k == py::str("node") && c.is_param(key(b.id))) ? std::optional<int64_t>(c.frame(key(b.id))) : std::nullopt;
    if (RELJCC.contains(opcode)) {
        py::str x = hx(af);
        py::str y = hx(bf);
        return bs({"8b46", x, "3b46", y, hx(RELJCC.at(opcode)), "04", "33db", "eb03", "33db", "43", "8bc3"});
    }
    if (opcode == 0x10) {
        py::str x = hx(af);
        py::str y = hx(bf);
        return bs({"8b46", x, "f76e", y});
    }
    if (opcode == 0x11) {
        py::str x = hx(af);
        py::str y = hx(bf);
        return bs({"8b46", x, "99", "f77e", y});
    }
    if (opcode == 0x12) {
        py::str x = hx(af);
        py::str y = hx(bf);
        return bs({"8b46", x, "99", "f77e", y, "8bc2"});
    }
    if (opcode == 0x15) {
        py::str y = hx(bf);
        py::str x = hx(af);
        return bs({"8b4e", y, "8b46", x, "d3e0"});
    }
    if (opcode == 0x16) {
        py::str y = hx(bf);
        py::str x = hx(af);
        return bs({"8b4e", y, "8b46", x, "d3f8"});
    }
    throw py::NotImplementedError("binop " + hx(opcode).utf8());
}

int64_t parse_funcbegin(const py::bytes& pl) { return pl[3] | (pl[4] << 8); }

// items: ('b',bytes) | ('L',name) | ('j',opcode,name) short rel8. Two-pass.
py::bytes resolve(const py::list<Item>& items) {
    py::dict<py::str, int64_t> pos;
    int64_t off = 0;
    for (const Item& it : items) {
        if (it.kind == py::str("b")) {
            off += py::len(it.data);
        } else if (it.kind == py::str("L")) {
            pos.set(it.name, off);
        } else if (it.kind == py::str("j")) {
            off += 2;
        }
    }
    std::string out;
    off = 0;
    for (const Item& it : items) {
        if (it.kind == py::str("b")) {
            out += it.data.raw();
            off += py::len(it.data);
        } else if (it.kind == py::str("L")) {
            // (nothing to emit)
        } else if (it.kind == py::str("j")) {
            int64_t rel = pos.at(it.name) - (off + 2);
            out.push_back(static_cast<char>(it.opcode));
            out.push_back(static_cast<char>(rel & 0xff));
            off += 2;
        }
    }
    return py::bytes(out);
}

namespace {

Item item_b(const py::bytes& d) {
    Item it;
    it.kind = py::str("b");
    it.data = d;
    return it;
}

Item item_l(const py::str& name) {
    Item it;
    it.kind = py::str("L");
    it.name = name;
    return it;
}

Item item_j(int64_t opcode, const py::str& name) {
    Item it;
    it.kind = py::str("j");
    it.opcode = opcode;
    it.name = name;
    return it;
}

// list.pop() on a plain stack: IndexError when empty
py::str pop_name(std::vector<py::str>& st) {
    if (st.empty()) throw py::IndexError("pop from empty list");
    py::str x = st.back();
    st.pop_back();
    return x;
}

}  // namespace

py::bytes model(const py::list<QuadRecord>& func) {
    int64_t fn = parse_funcbegin(std::get<1>(func[0]));
    py::list<QuadRecord> body = func.slice_from(1);
    py::list<py::list<Operand>> binops_pl;
    for (const QuadRecord& q : body) {
        if (BINCLASS.contains(std::get<0>(q))) {
            binops_pl.append(parse_operands(std::get<1>(q)));
        }
    }
    bool long_mode = false;
    for (const auto& ops : binops_pl) {
        for (const Operand& o : ops) {
            if (o.ty && *o.ty == 4) long_mode = true;
        }
    }
    bool has_bin = !binops_pl.empty();
    int64_t fs = has_bin ? (long_mode ? 4 : 2) : 0;
    Ctx c(fn, fs, long_mode ? 4 : 2);
    py::bytes epi = (fs ? bs({"83c4", hx(fs)}) : py::bytes()) + bs({"5dc3"});
    py::list<Item> items;
    items.append(item_b(bs({"55"}) + (fs ? bs({"83ec", hx(fs), "3b26 0000", "7703", "e9 0000"}) : py::bytes()) + bs({"8bec"})));
    std::vector<py::str> lstack;
    int64_t lc = 0;
    std::vector<py::str> loopstack;
    int64_t i = 0;
    while (i < py::len(body)) {
        int64_t op = std::get<0>(body[i]);
        const py::bytes pl = std::get<1>(body[i]);
        // peephole: x = x +/- 1  (sub/add by const 1, assigned back to same var) -> inc/dec [x]
        if (op == 0x13 || op == 0x14) {
            py::list<Operand> ops = parse_operands(pl);
            if (py::len(ops) >= 2 && ops[0].k == py::str("node") && c.is_param(ops[0].id ? *ops[0].id : -1) &&
                ops[1].k == py::str("const") && ops[1].val && *ops[1].val == 1 && i + 1 < py::len(body) &&
                std::get<0>(body[i + 1]) == 0x00) {
                py::list<Operand> ao = parse_operands(std::get<1>(body[i + 1]));
                if (py::len(ao) >= 2 && ao[1].k == py::str("node") && ao[1].id && *ao[1].id == key(ops[0].id)) {
                    items.append(item_b(bs({"ff", op == 0x13 ? "46" : "4e", hx(c.frame(key(ops[0].id)))})));
                    i += 2;
                    continue;
                }
            }
        }
        if (BINCLASS.contains(op) && RELJCC.contains(op) && i + 1 < py::len(body) && std::get<0>(body[i + 1]) == 0x2c) {
            py::list<Operand> ops = parse_operands(pl);
            const Operand& a = ops[0];
            const Operand& b = ops[1];
            py::bytes first = load_ax(a, c);
            items.append(item_b(first + bs({"3b46", hx(c.frame(key(b.id)))})));
            py::str nm = py::percent("L%d", lc);
            lc += 1;
            lstack.push_back(nm);
            items.append(item_j(INV_JCC.at(op), nm));
            i += 2;
            continue;
        }
        if (BINCLASS.contains(op)) {
            py::list<Operand> ops = parse_operands(pl);
            py::list<Operand> two = ops.slice(0, 2);
            items.append(item_b(long_mode ? emit_binop_long(op, two, c) : emit_binop(op, two, c)));
        } else if (op == 0x00) {
            py::list<Operand> o = parse_operands(pl);
            py::bytes ld = load_ax(o[0], c);
            items.append(item_b(ld + store_ax(o[1], c)));
        } else if (op == 0x2e) {   // LABEL
            items.append(item_l(pop_name(lstack)));
        } else if (op == 0x2f) {   // LOOP top
            py::str nm = py::percent("T%d", lc);
            lc += 1;
            loopstack.push_back(nm);
            items.append(item_l(nm));
        } else if (op == 0x30) {   // while-test: load cond; test; je exit
            py::list<Operand> o = parse_operands(pl);
            py::str ex = py::percent("X%d", lc);
            lc += 1;
            loopstack.push_back(ex);
            items.append(item_b(load_ax(o[0], c) + bs({"85c0"})));
            items.append(item_j(0x74, ex));
        } else if (op == 0x31) {   // loop-back: jmp top; then exit label
            py::str ex = pop_name(loopstack);
            py::str top = pop_name(loopstack);
            items.append(item_j(0xeb, top));
            items.append(item_l(ex));
        } else if (op == 0x24) {   // RETURN
            py::list<Operand> o = parse_operands(pl);
            py::bytes b;
            if (!o.empty() && o[0].k != py::str("temp")) {
                b = load_ax(o[0], c);
            }
            items.append(item_b(b + epi));
        }
        i += 1;
    }
    return resolve(items);
}

namespace {

// bytes.hex(' '): the bytes as hex pairs separated by single spaces
py::str hex_spaced(const py::bytes& b) {
    std::string out;
    static const char* d = "0123456789abcdef";
    bool first = true;
    for (int64_t v : b) {
        if (!first) out.push_back(' ');
        first = false;
        out.push_back(d[v >> 4]);
        out.push_back(d[v & 15]);
    }
    return py::str(out);
}

}  // namespace

void main(const py::list<py::str>& argv) {
    py::list<py::list<QuadRecord>> quads = quad_align::parse_quads(argv[1]);
    py::list<std::tuple<py::str, int64_t, py::bytes>> objf;
    for (const auto& f : quad_align::parse_obj_funcs(argv[2])) {
        py::bytes head = std::get<2>(f).slice_to(1);
        if (head == py::bytes("\x55") || head == py::bytes("\x53") || head == py::bytes("\x8b") || head == py::bytes("\xb8")) {
            objf.append(f);
        }
    }
    int64_t ok = 0;
    for (int64_t i = 0; i < py::len(objf); ++i) {
        const auto& [nm, po, code] = objf[i];
        (void)po;
        py::bytes pred;
        try {
            pred = i < py::len(quads) ? model(quads[i]) : py::bytes();
        } catch (const py::Exception&) {
            pred = py::bytes();
        }
        bool m = pred == code;
        ok += m;
        py::print(py::percent("  %-7s %-5s", nm, m ? py::str("OK") : py::str("FAIL")) +
                  (m ? py::str("") : py::percent("  pred %s\n            orig %s", hex_spaced(pred), hex_spaced(code))));
    }
    py::print(py::str("\nMC2 FORWARD MODEL: ") + py::to_str(ok) + py::str("/") + py::to_str(py::len(objf)) + py::str(" BYTE-EXACT"));
}

}  // namespace codebreaker::invert::early_c_forward
