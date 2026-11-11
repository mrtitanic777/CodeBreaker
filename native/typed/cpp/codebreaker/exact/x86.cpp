// codebreaker/exact/x86.cpp -- C++ from codebreaker.exact.x86 (see x86.hpp).

#include "codebreaker/exact/x86.hpp"

#include <algorithm>

#include <capstone/capstone.h>

#include "codebreaker/platforms/package.hpp"

namespace codebreaker::exact::x86 {

namespace {

Decoder& decoder_() {
    static Decoder d;
    return d;
}

// _CS.disasm(code, address) taken for its first instruction: next(..., None)
// (an installed decoder, set_decoder, overrides platforms.decoder(16))
std::optional<Insn> first_insn_(const py::bytes& code, int64_t address) {
    if (decoder_()) return decoder_()(code, address);
    std::optional<Insn> out;
    platforms::decoder(16, py::str("x86"), false)->each(code.raw(), address, 1, [&](const cs_insn& c) {
        out = Insn(static_cast<int64_t>(c.address), py::bytes(std::string(reinterpret_cast<const char*>(c.bytes), c.size)),
                   py::str(std::string(c.mnemonic)), py::str(std::string(c.op_str)));
        return false;
    });
    return out;
}

py::bytes one_byte_(int64_t v) { return py::bytes(std::string(1, static_cast<char>(v))); }

}  // namespace

Insn::Insn(int64_t a, const py::bytes& b, const py::str& m, const py::str& o) {
    address = a;
    bytes = b;
    mnemonic = m;
    op_str = o;
    size = py::len(b);
}

void set_decoder(Decoder d) { decoder_() = std::move(d); }

py::list<Insn> disasm(const py::bytes& img, int64_t at, int64_t end, bool emulator) {
    py::list<Insn> out;
    int64_t a = at;
    while (a < end) {
        py::bytes b = img.slice(a, a + 2);
        if (emulator && b.slice(0, 1) == one_byte_(0xCD) && py::len(b) == 2) {
            if (0x34 <= b[1] && b[1] <= 0x3B) {
                std::optional<Insn> x = first_insn_(one_byte_(0xD8 + b[1] - 0x34) + img.slice(a + 2, a + 16), a + 1);
                if (x) {
                    py::str m = x->mnemonic.startswith(py::str("f")) ? x->mnemonic : py::str("f") + x->mnemonic;
                    out.append(Insn(a, img.slice(a, a + 1 + x->size), m, x->op_str));
                    a += 1 + x->size;
                    continue;
                }
            } else if (b[1] == 0x3D) {
                out.append(Insn(a, b, py::str("fwait"), py::str("")));
                a += 2;
                continue;
            } else if (b[1] == 0x3C && a + 2 < py::len(img)) {
                py::str sg;
                switch (img[a + 2] & 0xC0) {
                    case 0x00: sg = py::str("es"); break;
                    case 0x40: sg = py::str("cs"); break;
                    case 0x80: sg = py::str("ss"); break;
                    default: sg = py::str("ds"); break;
                }
                std::optional<Insn> x = first_insn_(one_byte_(0xD8 | (img[a + 2] & 7)) + img.slice(a + 3, a + 16), a + 2);
                if (x) {
                    out.append(Insn(a, img.slice(a, a + 2 + x->size), x->mnemonic, sg + py::str(": ") + x->op_str));
                    a += 2 + x->size;
                    continue;
                }
            }
        }
        std::optional<Insn> x = first_insn_(img.slice(a, py::min(a + 16, end)), a);
        if (!x) {
            out.append(Insn(a, img.slice(a, a + 1), py::str("db"), py::percent(py::str("0x%02x"), img[a])));
            a += 1;
            continue;
        }
        out.append(Insn(x->address, x->bytes, x->mnemonic, x->op_str));
        a += x->size;
    }
    return out;
}

py::dict<int64_t, int64_t> tables(const py::list<Insn>& insns, const py::bytes& img, int64_t lo, int64_t hi, int64_t base) {
    static const py::re::Pattern pat(py::str("\\+ 0x([0-9a-f]+)\\]"), 0);
    py::dict<int64_t, int64_t> out;
    for (const Insn& i : insns) {
        if (i.mnemonic == "jmp" && i.op_str.contains(py::str("cs:["))) {
            std::optional<py::re::Match> m = pat.search(i.op_str);
            if (!m) {
                continue;
            }
            int64_t t = base + py::int_from_str(m->group(1), 16);
            int64_t k = t;
            while (lo <= k && k < hi && k + 2 <= hi) {
                int64_t target = base + (img[k] | img[k + 1] << 8);
                if (!(lo <= target && target < hi)) {
                    break;
                }
                k += 2;
            }
            if (k > t) {
                out.set(t, k - t);
            }
        }
    }
    return out;
}

py::list<Insn> body(const py::bytes& img, int64_t lo, int64_t hi, bool emulator, int64_t base) {
    py::list<Insn> first = disasm(img, lo, hi, emulator);
    py::dict<int64_t, int64_t> tb = tables(first, img, lo, hi, base);
    if (!py::len(tb)) {
        return first;
    }
    py::list<Insn> out;
    int64_t a = lo;
    std::vector<int64_t> order = tb.keys();
    std::sort(order.begin(), order.end());
    order.push_back(hi);
    for (int64_t t : order) {
        if (t > a) {
            out.extend(disasm(img, a, t, emulator));
        }
        a = py::max(a, t + tb.get(t, 0));
    }
    return out;
}

}  // namespace codebreaker::exact::x86
