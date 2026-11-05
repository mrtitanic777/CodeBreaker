// zx.cc -- ZX Spectrum snapshots (.sna, .z80) and tapes (.tap): codebreaker/formats/zx.py.
// A Program comes back as the dict of its attributes; the shim makes the zx.Program.
#include <string>
#include <utility>
#include <vector>

#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::zx {
namespace {

constexpr int64_t kProgramStart = 0x5B00;
using Mem = std::vector<uint8_t>;
using Regs = std::vector<std::pair<std::string, int64_t>>;   // a dict, in its order

int64_t clampi(int64_t i, int64_t n) {
    if (i < 0) {
        i += n;
        return i < 0 ? 0 : i;
    }
    return i > n ? n : i;
}
// mem[a:b] = src, Python's slice assignment (the bytearray grows or shrinks to fit)
void splice(Mem &mem, int64_t a, int64_t b, Bytes src) {
    int64_t n = static_cast<int64_t>(mem.size());
    a = clampi(a, n);
    b = clampi(b, n);
    if (b < a) b = a;
    Mem out(mem.begin(), mem.begin() + a);
    out.insert(out.end(), src.begin(), src.end());
    out.insert(out.end(), mem.begin() + b, mem.end());
    mem.swap(out);
}
uint8_t mat(const Mem &m, int64_t i) { return Bytes(m).at(i); }
void set_reg(Regs &r, const std::string &k, int64_t v) {
    for (auto &kv : r)
        if (kv.first == k) {
            kv.second = v;
            return;
        }
    r.emplace_back(k, v);
}
const int64_t *get_reg(const Regs &r, const char *k) {
    for (auto &kv : r)
        if (kv.first == k) return &kv.second;
    return nullptr;
}

struct Entries {                 // {address: name}, in insertion order
    std::vector<std::pair<int64_t, std::string>> items;
    bool has(int64_t a) const {
        for (auto &kv : items)
            if (kv.first == a) return true;
        return false;
    }
    void setdefault(int64_t a, const std::string &n) {
        if (!has(a)) items.emplace_back(a, n);
    }
    void set(int64_t a, const std::string &n) {
        for (auto &kv : items)
            if (kv.first == a) {
                kv.second = n;
                return;
            }
        items.emplace_back(a, n);
    }
};

void write_program(Out &o, const std::string &path, const char *kind, const Regs &regs, const Mem &mem,
                   int64_t origin, Bytes image, const Entries &entries) {
    o.begin_dict();
    o.item_text("path", path);
    o.item_text("kind", kind);
    o.key("registers");
    o.begin_dict();
    for (auto &kv : regs) o.item(kv.first.c_str(), kv.second);
    o.end_dict();
    o.key("memory");
    o.bytes(Bytes(mem));
    o.item("origin", origin);
    o.key("image");
    o.bytes(image);
    o.key("entries");
    o.begin_dict();
    for (auto &kv : entries.items) {
        o.integer(kv.first);
        o.text(kv.second);
    }
    o.end_dict();
    o.end_dict();
}

void program(Out &o, const std::string &path, const char *kind, const Mem &mem, const Regs &regs, int64_t pc,
             const std::vector<std::pair<int64_t, std::string>> &extra = {}) {
    Bytes image = Bytes(mem).slice(kProgramStart, 0x10000);
    Entries e;
    if (kProgramStart <= pc && pc <= 0xFFFF) e.set(pc, "start");
    for (auto &kv : extra)
        if (kProgramStart <= kv.first && kv.first <= 0xFFFF) e.setdefault(kv.first, kv.second);
    const int64_t *im = get_reg(regs, "im");
    if (im && *im == 2) {
        int64_t at = (*get_reg(regs, "i") << 8) | 0xFF;
        int64_t handler = mat(mem, at) | (mat(mem, (at + 1) & 0xFFFF) << 8);
        if (kProgramStart <= handler && handler <= 0xFFFF) e.setdefault(handler, "interrupt");
    }
    write_program(o, path, kind, regs, mem, kProgramStart, image, e);
}

void sna(Out &o, Bytes data, const std::string &path) {
    if (data.size() < 27 + 0xC000) raise("ValueError", path + ": too short for a .sna");
    Bytes h = data.slice(0, 27);
    Regs r;
    const char *names[] = {"i", "l_", "h_", "e_", "d_", "c_", "b_", "f_", "a_", "l", "h", "e", "d", "c", "b"};
    for (int k = 0; k < 15; ++k) r.emplace_back(names[k], h.at(k));
    r.emplace_back("iy", h.u16(15));
    r.emplace_back("ix", h.u16(17));
    r.emplace_back("r", h.at(20));
    r.emplace_back("f", h.at(21));
    r.emplace_back("a", h.at(22));
    r.emplace_back("sp", h.u16(23));
    r.emplace_back("im", h.at(25));
    Mem mem(0x10000, 0);
    splice(mem, 0x4000, 0x10000, data.slice(27, 27 + 0xC000));
    int64_t pc;
    if (data.size() >= 27 + 0xC000 + 4) {
        pc = data.u16(27 + 0xC000);
    } else {
        int64_t sp = *get_reg(r, "sp");
        pc = mat(mem, sp) | (mat(mem, (sp + 1) & 0xFFFF) << 8);
        set_reg(r, "sp", (sp + 2) & 0xFFFF);
    }
    program(o, path, "snapshot", mem, r, pc);
}

Mem unrle(Bytes block, int64_t size) {
    Mem out;
    int64_t i = 0;
    while (i < block.size()) {
        if (block.at(i) == 0xED && i + 1 < block.size() && block.at(i + 1) == 0xED) {
            uint8_t v = block.at(i + 3);
            uint8_t n = block.at(i + 2);
            out.insert(out.end(), n, v);
            i += 4;
        } else {
            out.push_back(block.at(i));
            i += 1;
        }
        if (size && static_cast<int64_t>(out.size()) >= size) break;
    }
    return out;
}

void z80(Out &o, Bytes data, const std::string &path) {
    Bytes h = data.slice(0, 30);
    Regs r;
    r.emplace_back("a", h.at(0));
    r.emplace_back("f", h.at(1));
    r.emplace_back("c", h.at(2));
    r.emplace_back("b", h.at(3));
    r.emplace_back("l", h.at(4));
    r.emplace_back("h", h.at(5));
    r.emplace_back("sp", h.u16(8));
    r.emplace_back("i", h.at(10));
    {
        int64_t r11 = h.at(11), r12 = h.at(12);
        r.emplace_back("r", (r11 & 0x7F) | ((r12 & 1) << 7));
    }
    r.emplace_back("e", h.at(13));
    r.emplace_back("d", h.at(14));
    r.emplace_back("iy", h.u16(23));
    r.emplace_back("ix", h.u16(25));
    r.emplace_back("im", h.at(29) & 3);
    int64_t pc = h.u16(6);
    Mem mem(0x10000, 0);
    if (pc) {
        Bytes body = data.slice(30);
        Mem ram;
        if (h.at(12) & 0x20) {
            bool tail = body.size() >= 4 && body.slice(body.size() - 4).is("\x00\xED\xED\x00", 4);
            ram = unrle(tail ? body.slice(0, -4) : body, 0xC000);
        } else {
            ram = body.vec();
        }
        int64_t n = static_cast<int64_t>(ram.size()) < 0xC000 ? static_cast<int64_t>(ram.size()) : 0xC000;
        splice(mem, 0x4000, 0x4000 + n, Bytes(ram).slice(0, 0xC000));
        program(o, path, "snapshot", mem, r, pc);
        return;
    }
    int64_t extra = data.u16(30);
    pc = data.u16(32);
    int64_t hw = data.at(34);
    bool v2 = extra == 23;
    bool is128 = hw >= (v2 ? 3 : 4) && hw != 64;
    int64_t paged = is128 ? (data.at(35) & 7) : 0;
    int64_t at = 32 + extra;
    std::vector<std::pair<int64_t, Mem>> banks;     // a dict: a page met again keeps its place
    while (at + 3 <= data.size()) {
        int64_t length = data.u16(at), page = data.u8(at + 2);
        at += 3;
        int64_t take = length == 0xFFFF ? 0x4000 : length;
        Bytes raw = data.slice(at, at + take);
        at += take;
        Mem v = length == 0xFFFF ? raw.vec() : unrle(raw, 0x4000);
        bool found = false;
        for (auto &kv : banks)
            if (kv.first == page) {
                kv.second = v;
                found = true;
            }
        if (!found) banks.emplace_back(page, v);
    }
    std::vector<std::pair<int64_t, int64_t>> where;
    auto put = [&](int64_t page, int64_t addr) {
        for (auto &kv : where)
            if (kv.first == page) {
                kv.second = addr;
                return;
            }
        where.emplace_back(page, addr);
    };
    if (is128) {
        put(8, 0x4000);
        put(5, 0x8000);
        put(3 + paged, 0xC000);
    } else {
        put(8, 0x4000);
        put(4, 0x8000);
        put(5, 0xC000);
    }
    for (auto &w : where)
        for (auto &kv : banks)
            if (kv.first == w.first) splice(mem, w.second, w.second + 0x4000, Bytes(kv.second).slice(0, 0x4000));
    program(o, path, "snapshot", mem, r, pc);
}

std::vector<int64_t> usr_calls(Bytes basic) {
    std::vector<int64_t> out;
    int64_t i = 0;
    while (i < basic.size()) {
        if (basic.at(i) == 0xC0) {
            int64_t j = basic.find("\x0E", 1, i, i + 16);
            if (j >= 0 && j + 6 <= basic.size() && basic.at(j + 1) == 0) {
                out.push_back(basic.at(j + 3) | (basic.at(j + 4) << 8));
                i = j + 6;
                continue;
            }
        }
        i += 1;
    }
    return out;
}

void tap(Out &o, Bytes data, const std::string &path) {
    std::vector<Bytes> blocks;
    int64_t at = 0;
    while (at + 2 <= data.size()) {
        int64_t n = data.u16(at);
        blocks.push_back(data.slice(at + 2, at + 2 + n));
        at += 2 + n;
    }
    bool have_header = false, have_code = false;
    Bytes header, code_payload;
    int64_t code_origin = 0;
    std::string basic;
    for (Bytes b : blocks) {
        if (b.empty()) continue;
        if (b.at(0) == 0x00 && b.size() == 19) {
            header = b;
            have_header = true;
            continue;
        }
        if (b.at(0) == 0xFF && have_header) {
            int64_t kind = header.at(1), length = header.u16(12), p1 = header.u16(14);
            Bytes payload = b.slice(1, 1 + length);
            if (kind == 0) {
                basic += payload.str();
            } else if (kind == 3 && (!have_code || payload.size() > code_payload.size())) {
                code_origin = p1;
                code_payload = payload;
                have_code = true;
            }
            have_header = false;
        }
    }
    if (!have_code) raise("ValueError", path + ": no code block on the tape");
    std::vector<int64_t> usr = usr_calls(Bytes(basic));
    Mem mem(0x10000, 0);
    int64_t origin = code_origin;
    Bytes payload = code_payload;
    splice(mem, origin, origin + payload.size(), payload.slice(0, 0x10000 - origin));
    Entries e;
    for (size_t i = 0; i < usr.size(); ++i) {
        int64_t a = usr[i];
        if (origin <= a && a < origin + payload.size()) {
            char buf[16];
            std::snprintf(buf, sizeof buf, "usr_%04llX", static_cast<long long>(a));
            e.set(a, i == 0 ? std::string("start") : std::string(buf));
        }
    }
    if (e.items.empty()) e.set(origin, "start");
    write_program(o, path, "tape", Regs(), mem, origin, payload.slice(0, 0x10000 - origin), e);
}

std::string lower_ext(const std::string &path) {
    // os.path.splitext(path)[1].lower()
    size_t sep = path.find_last_of("/\\");
    size_t start = sep == std::string::npos ? 0 : sep + 1;
    size_t dot = path.rfind('.');
    if (dot == std::string::npos || dot < start) return "";
    size_t k = start;
    while (k < dot && path[k] == '.') ++k;   // leading dots do not start an extension
    if (k == dot) return "";
    std::string e = path.substr(dot);
    for (char &c : e)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return e;
}

void read_q(Call &c) {
    const std::string path = c.str_arg(0);
    std::string data;
    if (!read_file(path, data)) raise_open_error(path);
    std::string ext = lower_ext(path);
    if (ext == ".sna") sna(c.out, Bytes(data), path);
    else if (ext == ".z80") z80(c.out, Bytes(data), path);
    else if (ext == ".tap") tap(c.out, Bytes(data), path);
    else raise("ValueError", path + ": not a Spectrum file");
}

}  // namespace

CBF_QUESTIONS(zx) { add("zx.read", read_q); }

}  // namespace cbf::zx
