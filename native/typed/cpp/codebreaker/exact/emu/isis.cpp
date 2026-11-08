// codebreaker/exact/emu/isis.cpp -- typed C++ port of codebreaker.exact.emu.isis.
#include "codebreaker/exact/emu/isis.hpp"

namespace codebreaker::exact::emu::isis {

namespace {
py::list<int64_t> make_par_() {
    py::list<int64_t> par;
    for (int64_t i = 0; i < 256; i++) par.append(py::bin(i).count(py::str("1")) % 2 == 0 ? 1 : 0);
    return par;
}
py::bytes nul_bytes_() { return py::bytes(std::string(1, '\0')); }   // b"\0"

// open(path, mode) as CPython does on Windows: a folder named with a trailing separator ("dir\", which is
// what an empty ISIS name makes) is "not found" there, where the bare folder name is "permission denied"
// (the runtime's py::open answers PermissionError for both).
py::file open_(const py::str& path, const char* mode) {
    try {
        return py::open(path, py::str(mode));
    } catch (const py::PermissionError&) {
        if (py::os::path::isdir(path) && (path.endswith(py::str("/")) || path.endswith(py::str("\\"))))
            throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
        throw;
    }
}
}  // namespace

const py::list<int64_t> PAR = make_par_();

Machine::Machine(const py::dict<int64_t, py::str>& drives_, const py::str& tail, bool echo_, std::ostream& out_)
    : m(py::bytearray::zeros(65536)),
      r{0, 0, 0, 0, 0, 0, 0, 0},                                         // B C D E H L - A
      sp(0xF6C0), pc(0), fz(0), fs(0), fp(0), fc(0), fa(0),
      drives(drives_), files(), console(), echo(echo_),
      cin(py::bytes("PROG") + tail.encode("latin1") + py::bytes("\r\n")),
      cpos(4),                                                           // RESCAN goes back to the program's name
      status(std::nullopt), steps(0), out(&out_) {}

// ---- files
PathResult Machine::path(const py::str& name_in) {
    py::str name = name_in.upper();
    if (name.startswith(py::str(":")) && name.slice(3, 4) == py::str(":")) {
        py::str dev = name.slice(1, 3);
        py::str rest = name.slice_from(4);
        if (dev[0] == py::str("F") && dev[1].isdigit()) {
            // (the default of .get is evaluated first: a KeyError for a missing drive 0, found drive or not)
            py::str dflt = drives.at(0);
            return {py::os::path::join(drives.get(py::int_from_str(dev[1]), dflt), rest), std::nullopt};
        }
        return {std::nullopt, dev};
    }
    return {py::os::path::join(drives.at(0), name), std::nullopt};
}

py::str Machine::str_at(int64_t a) {
    while (m[a] == 0x20) a += 1;
    int64_t e = a;
    while ((0x30 <= m[e] && m[e] <= 0x39) || (0x41 <= m[e] && m[e] <= 0x5A) || (0x61 <= m[e] && m[e] <= 0x7A) || m[e] == 0x3A ||
           m[e] == 0x2E)
        e += 1;
    return m.slice(a, e).view().decode("latin1");
}

int64_t Machine::w(int64_t a) { return m[a] | (m[(a + 1) & 0xFFFF] << 8); }

void Machine::ww(int64_t a, int64_t v) {
    m[a] = v & 255;
    m[(a + 1) & 0xFFFF] = (v >> 8) & 255;
}

int64_t Machine::load(const py::str& path_, int64_t bias) {
    py::file fh = open_(path_, "rb");
    py::bytes b = fh.read();
    fh.close();
    int64_t p = 0;
    int64_t entry = 0;
    while (p + 3 <= py::len(b)) {
        int64_t t = b[p];
        int64_t n = b[p + 1] | (b[p + 2] << 8);
        py::bytes body = b.slice(p + 3, p + 3 + n - 1);
        p += 3 + n;
        if (t == 0x06) {
            int64_t a = (body[1] | (body[2] << 8)) + bias;
            py::bytes d = body.slice_from(3);
            m.set_slice(a, a + py::len(d), d);
        } else if (t == 0x04) {
            entry = (body[2] | (body[3] << 8)) + bias;
        } else if (t == 0x0E) {
            break;
        }
    }
    return entry;
}

bool Machine::isis() {
    int64_t f = r[1];
    int64_t pb = (r[2] << 8) | r[3];
    auto P = [&](int64_t k) { return w(pb + 2 * k); };
    auto st = [&](int64_t k, int64_t v) { ww(P(k), v); };
    // the file number of a new file: the first of 2..98 not in use (StopIteration when there is none)
    auto free_number = [&]() -> int64_t {
        for (int64_t k = 2; k < 99; k++)
            if (!files.contains(k)) return k;
        throw StopIteration();
    };
    auto dump = [&](const py::ref<File>& fl) {   // open(fl[0], "wb").write(bytes(fl[3]))
        py::file fh = open_(*fl->path, "wb");
        fh.write(fl->data.view());
        fh.close();
    };

    if (f == 0) {                                             // OPEN(aftn^, name, access, echo, status^)
        py::str name = str_at(P(1));
        int64_t acc = P(2);
        auto [path_, dev] = path(name);
        if (dev) {
            std::optional<int64_t> a;
            if (*dev == py::str("CO")) a = 0;
            else if (*dev == py::str("CI")) a = 1;
            if (!a) {
                a = free_number();
                files.set(*a, std::make_shared<File>(File{std::nullopt, 0, dev, py::bytearray(), false}));
            }
            ww(P(0), *a);
            st(4, 0);
            return false;
        }
        if (acc == 1 && !py::os::path::exists(*path_)) {
            st(4, 13);
            return false;
        }
        py::bytearray data;
        if (acc != 2 && py::os::path::exists(*path_)) {
            py::file fh = open_(*path_, "rb");
            data = py::bytearray(fh.read());
            fh.close();
        }
        int64_t a = free_number();
        files.set(a, std::make_shared<File>(File{path_, 0, std::nullopt, data, acc != 1}));
        ww(P(0), a);
        st(4, 0);
    } else if (f == 1) {                                      // CLOSE(aftn, status^)
        int64_t a = P(0);
        py::ref<File> fl = files.pop(a, nullptr);
        if (fl && fl->dirty && fl->path && !fl->path->empty()) dump(fl);
        st(1, 0);
    } else if (f == 2) {                                      // DELETE(name, status^)
        auto [path_, dev] = path(str_at(P(0)));
        if (path_ && !path_->empty() && py::os::path::exists(*path_)) {
            py::os::remove(*path_);
            st(1, 0);
        } else {
            st(1, 13);
        }
    } else if (f == 3) {                                      // READ(aftn, buf^, count, actual^, status^)
        int64_t a = P(0), buf = P(1), cnt = P(2);
        py::bytes d;
        if (a == 1) {
            int64_t e = cin.find(py::bytes("\n"), cpos);
            e = e < 0 ? cin.size() : e + 1;
            d = cin.slice(cpos, py::min(e, cpos + cnt)).view();
            cpos += py::len(d);
        } else {
            py::ref<File> fl = files.get(a);
            if (!fl) {
                st(4, 2);
                return false;
            }
            d = !fl->dev ? fl->data.slice(fl->pos, fl->pos + cnt).view() : py::bytes();
            fl->pos += py::len(d);
        }
        m.set_slice(buf, buf + py::len(d), d);
        ww(P(3), py::len(d));
        st(4, 0);
    } else if (f == 4) {                                      // WRITE(aftn, buf^, count, status^)
        int64_t a = P(0), buf = P(1), cnt = P(2);
        py::bytes d = m.slice(buf, buf + cnt).view();
        if (a == 0) {
            console += d;
            if (echo) {
                *out << d.decode("latin1").utf8();
                out->flush();
            }
        } else {
            py::ref<File> fl = files.get(a);
            if (!fl) {
                st(3, 2);
                return false;
            }
            if (!fl->dev) {
                int64_t pos = fl->pos;
                if (pos > fl->data.size()) fl->data += py::bytes::zeros(pos - fl->data.size());
                fl->data.set_slice(pos, pos + cnt, d);
                fl->pos = pos + cnt;
                fl->dirty = true;
            }
        }
        st(3, 0);
    } else if (f == 5) {                                      // SEEK(aftn, mode, block^, byte^, status^)
        int64_t a = P(0), mode = P(1);
        py::ref<File> fl = files.get(a);
        if (!fl) {
            st(4, 2);
            return false;
        }
        int64_t blk = w(P(2)), byt = w(P(3));
        int64_t off = blk * 128 + byt;
        if (mode == 0) {
            ww(P(2), py::floordiv(fl->pos, 128));
            ww(P(3), py::mod(fl->pos, 128));
        } else if (mode == 1) {
            fl->pos = py::max(0, fl->pos - off);
        } else if (mode == 2) {
            fl->pos = off;
        } else if (mode == 3) {
            fl->pos = fl->pos + off;
        } else if (mode == 4) {
            fl->pos = fl->data.size();
        }
        if (fl->pos > fl->data.size() && fl->dirty) fl->data += py::bytes::zeros(fl->pos - fl->data.size());
        st(4, 0);
    } else if (f == 6) {                                      // LOAD(name, bias, retsw, entry^, status^)
        auto [path_, dev] = path(str_at(P(0)));
        if (!path_ || path_->empty() || !py::os::path::exists(*path_)) {
            st(4, 13);
            return false;
        }
        // the parameter block may lie where the program loads: read it first
        int64_t bias = P(1), retsw = P(2), entp = P(3), stp = P(4);
        ww(stp, 0);
        int64_t e = load(*path_, bias);
        if (retsw != 0) {                                     // retsw 1: control goes to the program loaded; 0: back to the caller
            pc = e;
            return true;
        }
        ww(entp, e);
    } else if (f == 7) {                                      // RENAME(old, new, status^)
        auto [a, dev_a] = path(str_at(P(0)));
        auto [b, dev_b] = path(str_at(P(1)));
        if (a && !a->empty() && py::os::path::exists(*a)) {
            if (!b) throw py::TypeError("replace: dst should be string, bytes or os.PathLike, not NoneType");
            py::os::replace(*a, *b);
            st(2, 0);
        } else {
            st(2, 13);
        }
    } else if (f == 9) {                                      // EXIT
        for (const auto& kv : files.items()) {
            const py::ref<File>& fl = std::get<1>(kv);
            if (fl->dirty && fl->path && !fl->path->empty()) dump(fl);
        }
        throw Exit();
    } else if (f == 11) {                                     // RESCAN
        cpos = 0;
        st(1, 0);
    } else if (f == 12) {                                     // ERROR(errnum, status^)
        int64_t errnum = P(0) & 255;
        int64_t where = w(sp);
        console += py::percent(py::str("ERROR %d USER PC %04X\r\n"), errnum, where).encode();
        st(1, 0);
    } else if (f == 14) {                                     // SPATH(name, info^, status^)
        py::str name = str_at(P(0));
        auto [path_, dev] = path(name);
        int64_t info = P(1);
        if (dev) {
            int64_t kind = 0x16;
            if (*dev == py::str("CO")) kind = 0x0D;
            else if (*dev == py::str("CI")) kind = 0x0C;
            else if (*dev == py::str("BB")) kind = 0x16;
            else if (*dev == py::str("LP")) kind = 0x15;
            else if (*dev == py::str("TO")) kind = 0x09;
            else if (*dev == py::str("TI")) kind = 0x08;
            m[info] = kind;
            m.set_slice(info + 1, info + 12, py::bytes::zeros(11));
        } else {
            py::str base = py::os::path::basename(*path_);
            auto [nm, sep, ex] = base.partition(py::str("."));
            m[info] = name.startswith(py::str(":F")) ? py::int_from_str(name[2]) : 0;
            m.set_slice(info + 1, info + 7, nm.encode().ljust(6, nul_bytes_()).slice(0, 6));
            m.set_slice(info + 7, info + 10, ex.encode().ljust(3, nul_bytes_()).slice(0, 3));
            m[info + 10] = 3;
            m[info + 11] = 0;
        }
        st(2, 0);
    } else if (f == 8 || f == 10 || f == 13) {                // ATTRIB, CONSOL, WHOCON: the status word only
        int64_t which = f == 8 ? 2 : f == 10 ? 3 : 1;
        ww(w(pb + 2 * which), 0);
    } else {
        throw py::RuntimeError(py::percent(py::str("ISIS call %d"), f));
    }
    return false;
}

// ---- the 8080
int64_t Machine::rd(int64_t i) { return i == 6 ? int64_t(m[(r[4] << 8) | r[5]]) : r[i]; }

void Machine::wr(int64_t i, int64_t v) {
    if (i == 6) m[(r[4] << 8) | r[5]] = v;
    else r[i] = v;
}

void Machine::szp(int64_t v) {
    fz = v == 0 ? 1 : 0;
    fs = v >> 7;
    fp = PAR[v];
}

void Machine::add(int64_t v, int64_t c) {
    int64_t a = r[7];
    int64_t t = a + v + c;
    fa = ((a & 15) + (v & 15) + c) >> 4;
    fc = t >> 8;
    t &= 255;
    r[7] = t;
    szp(t);
}

void Machine::sub(int64_t v, int64_t c, bool keep) {
    int64_t a = r[7];
    int64_t t = a - v - c;
    fa = ((a & 15) - (v & 15) - c) >= 0 ? 1 : 0;
    fc = t < 0 ? 1 : 0;
    t &= 255;
    szp(t);
    if (keep) r[7] = t;
}

void Machine::logic(int64_t t, int64_t ac) {
    r[7] = t;
    fc = 0;
    fa = ac;
    szp(t);
}

void Machine::push(int64_t v) {
    sp = (sp - 2) & 0xFFFF;
    m[sp] = v & 255;
    m[(sp + 1) & 0xFFFF] = v >> 8;
}

int64_t Machine::pop() {
    int64_t v = m[sp] | (m[(sp + 1) & 0xFFFF] << 8);
    sp = (sp + 2) & 0xFFFF;
    return v;
}

int64_t Machine::imm8() {
    int64_t v = m[pc];
    pc = (pc + 1) & 0xFFFF;
    return v;
}

int64_t Machine::imm16() {
    int64_t v = m[pc] | (m[(pc + 1) & 0xFFFF] << 8);
    pc = (pc + 2) & 0xFFFF;
    return v;
}

int64_t Machine::psw() { return (r[7] << 8) | (fs << 7) | (fz << 6) | (fa << 4) | (fp << 2) | 2 | fc; }

bool Machine::cond(int64_t k) {
    switch (k) {
        case 0: return !fz;
        case 1: return fz;
        case 2: return !fc;
        case 3: return fc;
        case 4: return !fp;
        case 5: return fp;
        case 6: return !fs;
        default: return fs;
    }
}

int64_t Machine::rp(int64_t k) { return k == 3 ? sp : (r[2 * k] << 8) | r[2 * k + 1]; }

void Machine::setrp(int64_t k, int64_t v) {
    if (k == 3) {
        sp = v;
    } else {
        r[2 * k] = v >> 8;
        r[2 * k + 1] = v & 255;
    }
}

void Machine::_exec(int64_t op) {
    int64_t hi = op >> 6, d = (op >> 3) & 7, sr = op & 7;
    int64_t k = d >> 1;
    if (op == 0x76) throw Exit();                              // HLT
    if (hi == 1) {                                             // MOV
        wr(d, rd(sr));
    } else if (hi == 2) {                                      // arithmetic and logic with a register
        switch (d) {
            case 0: add(rd(sr)); break;
            case 1: add(rd(sr), fc); break;
            case 2: sub(rd(sr)); break;
            case 3: sub(rd(sr), fc); break;
            case 4: logic(r[7] & rd(sr), ((r[7] | rd(sr)) >> 3) & 1); break;
            case 5: logic(r[7] ^ rd(sr)); break;
            case 6: logic(r[7] | rd(sr)); break;
            default: sub(rd(sr), 0, false); break;
        }
    } else if (hi == 0) {
        if (sr == 0) {                                         // NOP
        } else if (sr == 1 && !(d & 1)) {                      // LXI
            setrp(k, imm16());
        } else if (sr == 1) {                                  // DAD
            int64_t t = ((r[4] << 8) | r[5]) + rp(k);
            fc = t >> 16;
            r[4] = (t >> 8) & 255;
            r[5] = t & 255;
        } else if (sr == 2) {
            if (d == 0) {                                      // STAX B
                m[(r[0] << 8) | r[1]] = r[7];
            } else if (d == 1) {                               // LDAX B
                r[7] = m[(r[0] << 8) | r[1]];
            } else if (d == 2) {                               // STAX D
                m[(r[2] << 8) | r[3]] = r[7];
            } else if (d == 3) {                               // LDAX D
                r[7] = m[(r[2] << 8) | r[3]];
            } else if (d == 4) {                               // SHLD
                int64_t a = imm16();
                m[a] = r[5];
                m[(a + 1) & 0xFFFF] = r[4];
            } else if (d == 5) {                               // LHLD
                int64_t a = imm16();
                r[5] = m[a];
                r[4] = m[(a + 1) & 0xFFFF];
            } else if (d == 6) {                               // STA
                int64_t a = imm16();
                m[a] = r[7];
            } else {                                           // LDA
                r[7] = m[imm16()];
            }
        } else if (sr == 3) {                                  // INX / DCX
            if (!(d & 1)) setrp(k, (rp(k) + 1) & 0xFFFF);
            else setrp(k, (rp(k) - 1) & 0xFFFF);
        } else if (sr == 4) {                                  // INR
            int64_t v = (rd(d) + 1) & 255;
            wr(d, v);
            fa = (v & 15) == 0 ? 1 : 0;
            szp(v);
        } else if (sr == 5) {                                  // DCR
            int64_t v = (rd(d) - 1) & 255;
            wr(d, v);
            fa = (v & 15) == 15 ? 0 : 1;
            szp(v);
        } else if (sr == 6) {                                  // MVI
            wr(d, imm8());
        } else {
            if (d == 0) {                                      // RLC
                int64_t a = r[7];
                fc = a >> 7;
                r[7] = ((a << 1) | fc) & 255;
            } else if (d == 1) {                               // RRC
                int64_t a = r[7];
                fc = a & 1;
                r[7] = (a >> 1) | (fc << 7);
            } else if (d == 2) {                               // RAL
                int64_t a = r[7];
                int64_t c = fc;
                fc = a >> 7;
                r[7] = ((a << 1) | c) & 255;
            } else if (d == 3) {                               // RAR
                int64_t a = r[7];
                int64_t c = fc;
                fc = a & 1;
                r[7] = (a >> 1) | (c << 7);
            } else if (d == 4) {                               // DAA
                int64_t a = r[7];
                int64_t c = fc;
                int64_t adj = 0;
                if ((a & 15) > 9 || fa) adj = 6;
                if (a > 0x99 || c) {
                    adj |= 0x60;
                    c = 1;
                }
                fa = (a & 15) + (adj & 15) > 15 ? 1 : 0;
                a = (a + adj) & 255;
                r[7] = a;
                szp(a);
                fc = c;
            } else if (d == 5) {                               // CMA
                r[7] = r[7] ^ 255;
            } else if (d == 6) {                               // STC
                fc = 1;
            } else {                                           // CMC
                fc = fc ^ 1;
            }
        }
    } else {                                                   // hi == 3
        if (sr == 0) {                                         // Rcc
            if (cond(d)) pc = pop();
        } else if (sr == 1) {
            if (!(d & 1)) {
                if (k == 3) {                                  // POP PSW
                    int64_t v = pop();
                    r[7] = v >> 8;
                    fs = (v >> 7) & 1;
                    fz = (v >> 6) & 1;
                    fa = (v >> 4) & 1;
                    fp = (v >> 2) & 1;
                    fc = v & 1;
                } else {                                       // POP
                    setrp(k, pop());
                }
            } else if (d == 1) {                               // RET
                pc = pop();
            } else if (d == 5) {                               // PCHL
                pc = (r[4] << 8) | r[5];
            } else if (d == 7) {                               // SPHL
                sp = (r[4] << 8) | r[5];
            } else {                                           // (D9: RET)
                pc = pop();
            }
        } else if (sr == 2) {                                  // Jcc
            int64_t a = imm16();
            if (cond(d)) pc = a;
        } else if (sr == 3) {
            if (d == 0 || d == 1) {                            // JMP
                pc = imm16();
            } else if (d == 2 || d == 3) {                     // OUT / IN: the port byte is skipped
                imm8();
            } else if (d == 4) {                               // XTHL
                int64_t l = m[sp], h = m[(sp + 1) & 0xFFFF];
                m[sp] = r[5];
                m[(sp + 1) & 0xFFFF] = r[4];
                r[5] = l;
                r[4] = h;
            } else if (d == 5) {                               // XCHG
                std::swap(r[2], r[4]);
                std::swap(r[3], r[5]);
            }                                                  // DI, EI: nothing
        } else if (sr == 4) {                                  // Ccc
            int64_t a = imm16();
            if (cond(d)) {
                push(pc);
                pc = a;
            }
        } else if (sr == 5) {
            if (!(d & 1)) {                                    // PUSH
                if (k == 3) push(psw());
                else push(rp(k));
            } else {                                           // CALL
                int64_t a = imm16();
                push(pc);
                pc = a;
            }
        } else if (sr == 6) {                                  // arithmetic and logic with an immediate
            switch (d) {
                case 0: add(imm8()); break;
                case 1: add(imm8(), fc); break;
                case 2: sub(imm8()); break;
                case 3: sub(imm8(), fc); break;
                case 4: {
                    int64_t v = imm8();
                    logic(r[7] & v, ((r[7] | v) >> 3) & 1);
                    break;
                }
                case 5: logic(r[7] ^ imm8()); break;
                case 6: logic(r[7] | imm8()); break;
                default: sub(imm8(), 0, false); break;
            }
        } else {                                               // RST
            push(pc);
            pc = d * 8;
        }
    }
}

int64_t Machine::run(int64_t limit) {
    int64_t n = 0;
    try {
        while (n < limit) {
            int64_t pc_ = pc;
            if (pc_ == 0x40) {
                if (!isis()) pc = pop();
                n += 1;
                continue;
            }
            if (pc_ >= 0xF800) {
                if (pc_ == 0xF81B) {                           // MEMCK: top of memory in B:A (ASM86 wants at least F6C0)
                    r[7] = 0xC0;
                    r[0] = 0xF6;
                } else if (pc_ == 0xF809) {                    // CO
                    console.append(r[1]);
                } else if (pc_ == 0xF803) {                    // CI
                    r[7] = 0x0D;
                }
                pc = pop();
                n += 1;
                continue;
            }
            pc = (pc_ + 1) & 0xFFFF;
            _exec(m[pc_]);
            n += 1;
        }
    } catch (const Exit&) {
    }
    steps = n;
    return n;
}

Result run(const py::str& program, const py::str& tail, const py::dict<int64_t, py::str>& drives, bool echo, int64_t limit,
           std::ostream& out) {
    // program: host path of the ISIS object file; tail: the command line after the program name
    Machine M(drives, tail, echo, out);
    M.pc = M.load(program);
    int64_t n = M.run(limit);
    return Result(M.console.view().decode("latin1"), n < limit ? 0 : -1, n);
}

void main(const py::list<py::str>& argv, std::ostream& out) {
    py::list<py::str> a = argv.slice(1, py::len(argv));
    py::dict<int64_t, py::str> drives;
    while (!a.empty() && a[0].startswith(py::str("-d"))) {
        py::str dir = a[1];
        drives[py::int_from_str(a[0][2])] = dir;
        a = a.slice(2, py::len(a));
    }
    drives.setdefault(0, py::os::path::dirname(py::os::path::abspath(a[0])));
    Result r = run(a[0], py::str(" ") + py::join(py::str(" "), a.slice(1, py::len(a))), drives, true, 2000000000, out);
    out << "\n[" << r.steps << " instructions]\n";
}

}  // namespace codebreaker::exact::emu::isis
