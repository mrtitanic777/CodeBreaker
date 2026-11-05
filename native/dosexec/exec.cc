// exec.cc -- the real-mode DOS executive (dos.py run()): load an MZ program at a PSP, keep the MCB
// arena in guest RAM, serve INT 20h/21h (console, files by handle and by FCB, memory, EXEC, the
// clock, the PSP calls) and run it on Unicorn to its exit.  With a DPMI host (dpmi.cc) the same
// engine runs a DOS-extended program; the host routes its interrupts back here.
#include "dosexec.h"
#include "cbdos.h"

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>

#include <unicorn/x86.h>

namespace cbd {

static constexpr uint64_t RAM = 0x110000;

Run::~Run() {
    for (auto &kv : files)
        if (kv.second.f) kv.second.f->close();
    for (auto &kv : fcbs)
        if (kv.second) kv.second->close();
    if (uc) UC.close(uc);
}

// ---- registers and memory ---------------------------------------------------------------------
uint64_t Run::R(int r) {
    uint64_t v[8] = {0};
    uc_err e = UC.reg_read(uc, r, v);
    if (e != UC_ERR_OK) throw uc_error(e);
    return v[0];
}

void Run::set_reg(int r, uint64_t value) {
    uint64_t v[8] = {value};
    uc_err e = UC.reg_write(uc, r, v);
    if (e != UC_ERR_OK) throw uc_error(e);
}

void Run::W(int r, uint64_t v) { set_reg(r, v & 0xFFFF); }

Bytes Run::rd(uint64_t a, size_t n) {
    Bytes b(n);
    if (n == 0) return b;
    if (a + n <= RAM && a < RAM) {                     // (the low megabyte is our own buffer)
        std::memcpy(b.data(), ram.data() + a, n);
        return b;
    }
    uc_err e = UC.mem_read(uc, a, b.data(), n);
    if (e != UC_ERR_OK) throw uc_error(e);
    return b;
}

uint8_t Run::rd1(uint64_t a) {
    if (a < RAM) return ram[size_t(a)];
    uint8_t b;
    uc_err e = UC.mem_read(uc, a, &b, 1);
    if (e != UC_ERR_OK) throw uc_error(e);
    return b;
}

uint16_t Run::rd16(uint64_t a) {
    Bytes b = rd(a, 2);
    return uint16_t(b[0] | b[1] << 8);
}

void Run::wr(uint64_t a, const uint8_t *d, size_t n) {
    if (n == 0) return;
    uc_err e = UC.mem_write(uc, a, d, n);              // (through Unicorn: code written is re-translated)
    if (e != UC_ERR_OK) throw uc_error(e);
}

void Run::stop() { UC.emu_stop(uc); }

void Run::carry(bool on, std::optional<uint64_t> ax) {
    uint64_t f = R(UC_X86_REG_EFLAGS);
    set_reg(UC_X86_REG_EFLAGS, on ? (f | 1) : (f & ~uint64_t(1)));
    if (ax) W(UC_X86_REG_AX, *ax);
}

uint64_t Run::lin(uint64_t seg, uint64_t off) {
    // real mode: segment*16 + offset; protected mode: the descriptor's base + offset
    if (pm) {
        auto it = desc.find(seg & 0xFFFC);
        if (it != desc.end()) return it->second.base + off;
    }
    return seg * 16 + off;
}

std::wstring Run::cstr(uint64_t seg, uint64_t off) {
    uint64_t a = lin(seg, off);
    std::wstring out;
    for (;;) {
        uint8_t c = rd1(a);
        if (c == 0) break;
        out += wchar_t(c);
        a++;
    }
    return out;
}

std::wstring Run::hostpath(const std::wstring &name) {
    std::wstring n = name;
    for (auto &c : n)
        if (c == L'/') c = L'\\';
    n = py_upper(n);
    if (n.size() > 1 && n[1] == L':') n = n.substr(2);
    size_t s = 0;
    while (s < n.size() && n[s] == L'\\') s++;
    n = n.substr(s);
    std::vector<std::wstring> parts;
    std::wstring cur;
    for (size_t i = 0; i <= n.size(); i++) {
        if (i == n.size() || n[i] == L'\\') {
            if (!cur.empty() && cur != L".") parts.push_back(cur);
            cur.clear();
        } else {
            cur += n[i];
        }
    }
    return path_join(workdir, parts);
}

uint64_t Run::pc() {
    if (host && (R(UC_X86_REG_CR0) & 1)) {
        auto it = desc.find(R(UC_X86_REG_CS) & 0xFFFC);
        return (it != desc.end() ? it->second.base : 0) + R(UC_X86_REG_EIP);
    }
    return R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP);
}

int64_t Run::newh() {
    // the next handle; past 19 (where a C library's table of files ends) the lowest free one
    if (nexth < 20) {
        nexth++;
        return nexth - 1;
    }
    for (int64_t h = 5; h < 255; h++)
        if (!files.count(h)) return h;
    throw PyErr{CBD_OTHER, 0, "StopIteration: no free handle", L""};
}

FileP Run::fsopen(const std::wstring &p, const char *mode) { return fs ? fs->open(p, mode) : disk_open(p, mode); }
bool Run::fsisfile(const std::wstring &p) { return fs ? fs->isfile(p) : disk_isfile(p); }
int64_t Run::fssize(const std::wstring &p) { return fs ? fs->getsize(p) : disk_getsize(p); }
void Run::fsremove(const std::wstring &p) { fs ? fs->remove(p) : disk_remove(p); }
void Run::fsreplace(const std::wstring &a, const std::wstring &b) { fs ? fs->replace(a, b) : disk_replace(a, b); }

static bool is_os(const PyErr &e) { return e.kind == CBD_OS; }

static void le(Bytes &b, size_t at, uint64_t v, int n) {
    for (int i = 0; i < n; i++) b[at + i] = uint8_t(v >> (8 * i));
}

// ---- the DOS 1.x file control blocks --------------------------------------------------------------
// FCB: +0 drive, +1 name[8], +9 ext[3], +0C current block, +0E record size, +10 file size,
//      +20 current record, +21 random record (4 bytes)
void Run::int21_fcb(int ah, int al, uint64_t cx, uint64_t ds, uint64_t dx) {
    uint64_t fa = ds * 16 + dx;
    if (rd1(fa) == 0xFF) fa += 7;                      // extended FCB
    auto rb = [&](uint64_t o, size_t n) { return rd(fa + o, n); };
    auto rw = [&](uint64_t o) { Bytes b = rb(o, 2); return uint64_t(b[0] | b[1] << 8); };
    auto ww = [&](uint64_t o, uint64_t v) { uint8_t b[2] = {uint8_t(v), uint8_t(v >> 8)}; wr(fa + o, b, 2); };
    auto put4 = [&](uint64_t o, uint64_t v, size_t n) { Bytes b(4); le(b, 0, v, 4); b.resize(n); wr(fa + o, b); };
    std::wstring nm = rstrip_ws(latin1(rb(1, 8))), ex = rstrip_ws(latin1(rb(9, 3)));
    std::wstring hp = hostpath(nm + (ex.empty() ? L"" : L"." + ex));
    auto setal = [&](uint64_t v) { W(UC_X86_REG_AX, (uint64_t(ah) << 8) | (v & 0xFF)); };
    auto size_now = [](File &f) { int64_t cur = f.tell(); f.seek(0, 2); int64_t n_ = f.tell(); f.seek(cur, 0); return n_; };
    if (ah == 0x0F || ah == 0x16) {
        FileP f;
        try {
            if (ah == 0x0F) {
                if (!fsisfile(hp)) { setal(0xFF); return; }
                f = fsopen(hp, "r+b");
            } else {
                f = fsopen(hp, "w+b");
            }
        } catch (PyErr &e) {
            if (!is_os(e)) throw;
            setal(0xFF);
            return;
        }
        fcbs[fa] = f;
        ww(0x0C, 0);
        ww(0x0E, 0x80);
        put4(0x10, uint64_t(size_now(*f)), 4);
        uint8_t z = 0;
        wr(fa + 0x20, &z, 1);
        setal(0);
        return;
    }
    if (ah == 0x10) {
        auto it = fcbs.find(fa);
        if (it != fcbs.end()) {
            FileP f = it->second;
            fcbs.erase(it);
            if (f) f->close();
        }
        setal(0);
        return;
    }
    if (ah == 0x11 || ah == 0x12) {
        if (ah == 0x11 && (nm + ex).find(L'?') == std::wstring::npos && fsisfile(hp)) {
            Bytes d_(32, 0);
            d_[0] = rb(0, 1)[0];
            Bytes nmx = rb(1, 11);
            std::copy(nmx.begin(), nmx.end(), d_.begin() + 1);
            le(d_, 0x1C, uint64_t(fssize(hp)), 4);
            wr(dta, d_);
            setal(0);
        } else {
            setal(0xFF);
        }
        return;
    }
    if (ah == 0x13) {
        try {
            fsremove(hp);
            setal(0);
        } catch (PyErr &e) {
            if (!is_os(e)) throw;
            setal(0xFF);
        }
        return;
    }
    auto it = fcbs.find(fa);
    FileP f = it != fcbs.end() ? it->second : nullptr;
    if (!f) { setal(ah == 0x23 ? 0xFF : 1); return; }
    uint64_t rs = rw(0x0E);
    if (!rs) rs = 0x80;
    size_t rn = rs < 64 ? 4 : 3;
    uint64_t rnd;
    {
        Bytes b = rb(0x21, rn);
        rnd = 0;
        for (size_t i = 0; i < rn; i++) rnd |= uint64_t(b[i]) << (8 * i);
    }
    auto cur = [&]() { return rw(0x0C) * 128 + rb(0x20, 1)[0]; };
    auto seqpos = [&]() { return int64_t(cur() * rs); };
    auto setseq = [&](uint64_t rec) { ww(0x0C, rec / 128); uint8_t b = uint8_t(rec % 128); wr(fa + 0x20, &b, 1); };
    auto setsize = [&]() { put4(0x10, uint64_t(size_now(*f)), 4); };
    if (ah == 0x23) { put4(0x21, (uint64_t(size_now(*f)) + rs - 1) / rs, rn); setal(0); return; }
    if (ah == 0x24) { put4(0x21, cur(), rn); return; }
    if (ah == 0x14 || ah == 0x21) {                    // read one record (sequential / random)
        if (ah == 0x21) setseq(rnd);
        f->seek(seqpos(), 0);
        Bytes d_ = f->read(int64_t(rs));
        if (d_.empty()) { setal(1); return; }
        Bytes w_ = d_;
        w_.resize(size_t(rs), 0);
        wr(dta, w_);
        if (ah == 0x14) setseq(cur() + 1);
        setal(d_.size() == rs ? 0 : 3);
        return;
    }
    if (ah == 0x15 || ah == 0x22) {                    // write one record
        if (ah == 0x22) setseq(rnd);
        f->seek(seqpos(), 0);
        Bytes d_ = rd(dta, size_t(rs));
        f->write(d_.data(), d_.size());
        f->flush();
        setsize();
        if (ah == 0x15) setseq(cur() + 1);
        setal(0);
        return;
    }
    if (ah == 0x27) {                                  // random block read: CX records
        f->seek(int64_t(rnd * rs), 0);
        Bytes d_ = f->read(int64_t(cx * rs));
        uint64_t n_ = (d_.size() + rs - 1) / rs;
        if (!d_.empty()) {
            Bytes w_ = d_;
            w_.resize(size_t(n_ * rs), 0);
            wr(dta, w_);
        }
        W(UC_X86_REG_CX, n_);
        rnd += n_;
        setseq(rnd);
        put4(0x21, rnd, rn);
        setal(d_.size() == cx * rs ? 0 : ((d_.empty() || d_.size() % rs == 0) ? 1 : 3));
        return;
    }
    if (ah == 0x28) {                                  // random block write: CX records (0: set the size)
        f->seek(int64_t(rnd * rs), 0);
        if (cx == 0) {
            f->truncate_to(int64_t(rnd * rs));
        } else {
            Bytes d_ = rd(dta, size_t(cx * rs));
            f->write(d_.data(), d_.size());
        }
        f->flush();
        setsize();
        rnd += cx;
        setseq(rnd);
        put4(0x21, rnd, rn);
        setal(0);
        return;
    }
    (void)al;
}

// ---- INT 21h ------------------------------------------------------------------------------------
static bool is_dev(const std::wstring &up) {
    return up == L"CON" || up == L"NUL" || up == L"PRN" || up == L"AUX" || up == L"LST";
}

static std::wstring rstrip_colon(std::wstring s) {
    while (!s.empty() && s.back() == L':') s.pop_back();
    return s;
}

static bool in_set(uint8_t c, const char *set) { return c && strchr(set, c) != nullptr; }

Bytes load_overlay(const Bytes &ob, uint64_t rfac);

void Run::int21() {
    uint64_t ax = R(UC_X86_REG_AX);
    int ah = int(ax >> 8), al = int(ax & 0xFF);
    uint64_t bx = R(UC_X86_REG_BX), cx = R(UC_X86_REG_CX), dx = R(UC_X86_REG_DX);
    // ds/es: a protected-mode INT 21h reflected to real mode (INT 31h 0300) sets these overrides to the
    // selectors whose descriptors map the client's real-mode segments
    uint64_t ds = ov_ds ? *ov_ds : R(UC_X86_REG_DS);
    uint64_t es = ov_es ? *ov_es : R(UC_X86_REG_ES);
    auto Wseg = [&](bool is_es, uint64_t v) {
        if (is_es ? bool(ov_es) : bool(ov_ds)) (is_es ? out_es : out_ds) = v;
        else W(is_es ? UC_X86_REG_ES : UC_X86_REG_DS, v);
    };
    if (S.debug) fprintf(stderr, "int21 ax=%04llx bx=%04llx cx=%04llx dx=%04llx\n",
                         (unsigned long long)ax, (unsigned long long)bx, (unsigned long long)cx, (unsigned long long)dx);
    if (ah == 0x4C || ah == 0x00) { exit = ah == 0x4C ? al : 0; throw DosExit(); }
    if (ah == 0x02 || ah == 0x06) {
        if (ah == 0x06 && (dx & 0xFF) == 0xFF) { carry(false); W(UC_X86_REG_AX, ax & 0xFF00); return; }
        console.push_back(uint8_t(dx & 0xFF));
        return;
    }
    if (ah == 0x09) {
        uint64_t a = lin(ds, dx);
        for (;;) {
            uint8_t c = rd1(a);
            if (c == 0x24) break;
            console.push_back(c);
            a++;
        }
        return;
    }
    if (ah == 0x0A) {                                  // buffered line input
        uint64_t a = lin(ds, dx);
        int mx = rd1(a);
        Bytes line;
        while (!in.empty()) {
            uint8_t c = in.front();
            in.pop_front();
            if (c == 0x0A) continue;
            if (c == 0x0D) break;
            line.push_back(c);
        }
        line.resize(std::min(line.size(), size_t(std::max(mx - 1, 0))));
        console.insert(console.end(), line.begin(), line.end());
        console.push_back('\r');                       // (DOS echoes the line, and the carriage return)
        Bytes o;
        o.push_back(uint8_t(line.size()));
        o.insert(o.end(), line.begin(), line.end());
        o.push_back('\r');
        wr(a + 1, o);
        return;
    }
    if (ah == 0x01 || ah == 0x07 || ah == 0x08) {
        uint8_t c = 0x0D;
        if (!in.empty()) { c = in.front(); in.pop_front(); }
        W(UC_X86_REG_AX, (uint64_t(ah) << 8) | c);
        return;
    }
    if (ah == 0x0B) { W(UC_X86_REG_AX, (uint64_t(ah) << 8) | (in.empty() ? 0 : 0xFF)); return; }
    if (ah == 0x0E) { W(UC_X86_REG_AX, (uint64_t(ah) << 8) | 3); return; }
    if (ah == 0x19) { W(UC_X86_REG_AX, (uint64_t(ah) << 8) | uint64_t(int64_t(S.drive))); return; }
    if (ah == 0x1A) { dta = lin(ds, dx); return; }
    if (ah == 0x0F || ah == 0x10 || ah == 0x11 || ah == 0x12 || ah == 0x13 || ah == 0x14 || ah == 0x15 ||
        ah == 0x16 || ah == 0x21 || ah == 0x22 || ah == 0x23 || ah == 0x24 || ah == 0x27 || ah == 0x28) {
        int21_fcb(ah, al, cx, ds, dx);
        return;
    }
    if (ah == 0x17) {                                  // rename by FCB: old name at +1, new name at +11h
        uint64_t fa = ds * 16 + dx;
        if (rd1(fa) == 0xFF) fa += 7;
        auto nm = [&](uint64_t o) {
            std::wstring n = rstrip_ws(latin1(rd(fa + o, 8))), e = rstrip_ws(latin1(rd(fa + o + 8, 3)));
            return n + (e.empty() ? L"" : L"." + e);
        };
        std::wstring a = nm(1), b = nm(0x11);
        try {
            fsreplace(hostpath(a), hostpath(b));
            W(UC_X86_REG_AX, 0x1700);
        } catch (PyErr &e) {
            if (!is_os(e)) throw;
            W(UC_X86_REG_AX, 0x17FF);
        }
        return;
    }
    if (ah == 0x29) {                                  // parse a file name at DS:SI into the FCB at ES:DI
        uint64_t si_ = R(UC_X86_REG_SI), di_ = R(UC_X86_REG_DI), a_ = ds * 16 + si_, q = a_;
        if (al & 1) { while (in_set(rd1(q), " \t;,=+")) q++; }
        else { while (in_set(rd1(q), " \t")) q++; }
        uint8_t drive = 0;
        if (rd1(q + 1) == 0x3A && py_isalpha(wchar_t(rd1(q)))) { drive = rd1(q) & 0x1F; q += 2; }
        static const char *term = ".\"/\\[]:<>|+=;,";
        auto field = [&](size_t n, bool &wild) {
            Bytes out;
            wild = false;
            while (out.size() < n) {
                uint8_t c = rd1(q);
                if (c == 0x2A) { out.resize(n, '?'); q++; wild = true; break; }
                if (c <= 0x20 || in_set(c, term)) break;
                if (c == 0x3F) wild = true;
                out.push_back((c >= 0x61 && c <= 0x7A) ? uint8_t(c - 32) : c);
                q++;
            }
            while (rd1(q) > 0x20 && !in_set(rd1(q), term)) q++;
            out.resize(n, ' ');
            return out;
        };
        bool w1 = false, w2 = false;
        Bytes name_ = field(8, w1), ext_ = {' ', ' ', ' '};
        if (rd1(q) == 0x2E) { q++; ext_ = field(3, w2); }
        Bytes o;
        o.push_back(drive);
        o.insert(o.end(), name_.begin(), name_.end());
        o.insert(o.end(), ext_.begin(), ext_.end());
        wr(es * 16 + di_, o);
        W(UC_X86_REG_SI, si_ + (q - a_));
        W(UC_X86_REG_AX, (uint64_t(ah) << 8) | ((w1 || w2) ? 1 : 0));
        return;
    }
    if (ah == 0x25) { vec[al] = {ds, dx}; return; }
    if (ah == 0x35) {
        auto it = vec.find(al);
        auto v = it != vec.end() ? it->second : std::pair<uint64_t, uint64_t>(0, 0);
        Wseg(true, v.first);
        W(UC_X86_REG_BX, v.second);
        return;
    }
    if (ah == 0x2A) { W(UC_X86_REG_CX, 1989); W(UC_X86_REG_DX, 0x0110); W(UC_X86_REG_AX, uint64_t(ah) << 8); return; }
    if (ah == 0x2B) {                                  // set date: accepted when valid
        bool ok_ = 1980 <= cx && cx <= 2099 && 1 <= (dx >> 8) && (dx >> 8) <= 12 && 1 <= (dx & 0xFF) && (dx & 0xFF) <= 31;
        W(UC_X86_REG_AX, (uint64_t(ah) << 8) | (ok_ ? 0 : 0xFF));
        return;
    }
    if (ah == 0x2D) {
        bool ok_ = (cx >> 8) < 24 && (cx & 0xFF) < 60 && (dx >> 8) < 60;
        W(UC_X86_REG_AX, (uint64_t(ah) << 8) | (ok_ ? 0 : 0xFF));
        return;
    }
    if (ah == 0x2C) { W(UC_X86_REG_CX, 0x0C00); W(UC_X86_REG_DX, 0); return; }
    if (ah == 0x30) { W(UC_X86_REG_AX, 0x1E03); W(UC_X86_REG_BX, 0); W(UC_X86_REG_CX, 0); return; }   // DOS 3.30
    if (ah == 0x33) { W(UC_X86_REG_DX, dx & 0xFF00); return; }
    if (ah == 0x37) { W(UC_X86_REG_DX, (dx & 0xFF00) | 0x2F); W(UC_X86_REG_AX, ax & 0xFF00); return; }
    if (ah == 0x38) { carry(false); return; }
    if (ah == 0x3C || ah == 0x3D || ah == 0x5B) {
        std::wstring name = cstr(ds, dx);
        std::wstring up = rstrip_colon(py_upper(name));
        if (S.debug) fprintf(stderr, "open ah=%02x al=%02x '%s' -> '%s'\n", ah, al, utf8(name).c_str(), utf8(hostpath(name)).c_str());
        if (is_dev(up)) {
            int64_t h = newh();
            Handle e;
            e.dev = up;
            e.has_dev = true;
            files[h] = e;
            carry(false, uint64_t(h));
            return;
        }
        std::wstring hp = hostpath(name);
        FileP f;
        try {
            if (ah == 0x3D) {
                if (!fsisfile(hp)) { carry(true, 2); return; }
                f = fsopen(hp, (al & 3) ? "r+b" : "rb");
            } else {
                f = fsopen(hp, "w+b");
            }
        } catch (PyErr &e) {
            if (!is_os(e)) throw;
            carry(true, 5);
            return;
        }
        int64_t h = newh();
        Handle e;
        e.f = f;
        e.name = name;
        files[h] = e;
        carry(false, uint64_t(h));
        return;
    }
    if (ah == 0x3E) {
        auto it = files.find(int64_t(bx));
        if (it != files.end()) {
            FileP f = it->second.f;
            files.erase(it);
            if (f) f->close();
        }
        carry(false);
        return;
    }
    if (ah == 0x3F) {
        auto it = files.find(int64_t(bx));
        Bytes data;
        if (bx == 0 || (it != files.end() && it->second.has_dev && it->second.dev == L"CON")) {
            while (!in.empty() && data.size() < cx) {
                uint8_t c = in.front();
                in.pop_front();
                data.push_back(c);
                if (c == 0x0A) break;
            }
        } else if (it != files.end() && it->second.f) {
            data = it->second.f->read(int64_t(cx));
        }
        wr(lin(ds, dx), data);
        carry(false, data.size());
        return;
    }
    if (ah == 0x40) {
        Bytes data = rd(lin(ds, dx), size_t(cx));
        auto it = files.find(int64_t(bx));
        if (bx == 1 || bx == 2 || (it != files.end() && it->second.has_dev && it->second.dev == L"CON")) {
            console.insert(console.end(), data.begin(), data.end());
        } else if (it != files.end() && it->second.f) {
            if (cx == 0) it->second.f->truncate();
            else it->second.f->write(data.data(), data.size());
        }
        carry(false, cx);
        return;
    }
    if (ah == 0x41) {
        std::wstring p = hostpath(cstr(ds, dx));
        try {
            fsremove(p);
            carry(false);
        } catch (PyErr &e) {
            if (!is_os(e)) throw;
            carry(true, 2);
        }
        return;
    }
    if (ah == 0x42) {
        auto it = files.find(int64_t(bx));
        if (it != files.end() && it->second.f) {
            File &f = *it->second.f;
            int64_t off = int64_t((cx << 16) | dx);
            if (al && (off & 0x80000000)) off -= int64_t(1) << 32;
            f.seek(off, al);
            int64_t p = f.tell();
            W(UC_X86_REG_DX, uint64_t(p >> 16));
            carry(false, uint64_t(p) & 0xFFFF);
        } else {
            W(UC_X86_REG_DX, 0);
            carry(false, 0);
        }
        return;
    }
    if (ah == 0x43) {
        bool no = !fsisfile(hostpath(cstr(ds, dx)));
        bool no2 = !fsisfile(hostpath(cstr(ds, dx)));
        carry(no, no2 ? std::optional<uint64_t>(2) : std::nullopt);
        W(UC_X86_REG_CX, 0x20);
        return;
    }
    if (ah == 0x44) {
        if (al == 0) {
            auto it = files.find(int64_t(bx));
            bool dev = bx <= 4 || (it != files.end() && it->second.has_dev);
            W(UC_X86_REG_DX, dev ? 0x80D3 : 0x0000);
            carry(false);
        } else {
            carry(false, 0);
        }
        return;
    }
    if (ah == 0x45 || ah == 0x46) { carry(false, bx); return; }
    if (ah == 0x47) { uint8_t z = 0; wr(lin(ds, R(UC_X86_REG_SI)), &z, 1); carry(false); return; }
    if (ah == 0x48 || ah == 0x49 || ah == 0x4A) {      // memory: the MCB arena in guest RAM
        try {
            if (ah == 0x48) {
                auto s_ = mcb_alloc(*this, first_mcb, int64_t(bx), PSP, strategy);
                if (!s_) { W(UC_X86_REG_BX, uint64_t(mcb_largest(*this, first_mcb))); carry(true, 8); }
                else carry(false, uint64_t(*s_));
            } else if (ah == 0x49) {
                mcb_free(*this, first_mcb, int64_t(es));
                carry(false);
            } else {
                auto got = mcb_resize(*this, first_mcb, int64_t(es), int64_t(bx));
                if (!got) carry(false);
                else { W(UC_X86_REG_BX, uint64_t(*got)); carry(true, 8); }
            }
        } catch (ArenaError &e) {
            carry(true, uint64_t(e.code));
        }
        return;
    }
    if (ah == 0x58) {                                  // the allocation strategy (UMBs: none, not linked)
        if (al == 0) carry(false, uint64_t(strategy));
        else if (al == 1) { strategy = int64_t(bx & 0xFF); carry(false); }
        else if (al == 2) carry(false, 0);
        else if (al == 3) carry(false);
        else carry(true, 1);
        return;
    }
    if (ah == 0x67) { carry(false); return; }
    if (ah == 0x52) { Wseg(true, DOSSEG); W(UC_X86_REG_BX, LOL_OFF); return; }   // list of lists
    if (ah == 0x4E) {                                  // find first: an exact name only
        std::wstring name = cstr(ds, dx), hp = hostpath(name);
        if (name.find(L'*') != std::wstring::npos || name.find(L'?') != std::wstring::npos || !fsisfile(hp)) {
            carry(true, 18);
            return;
        }
        Bytes d(43, 0);
        d[0x15] = 0x20;
        le(d, 0x1A, uint64_t(fssize(hp)), 4);
        Bytes base;
        if (!to_latin1(py_upper(basename(hp)), base))
            throw PyErr{CBD_OTHER, 0, "UnicodeEncodeError: 'latin-1' codec can't encode a character", L""};
        if (base.size() > 12) base.resize(12);
        std::copy(base.begin(), base.end(), d.begin() + 0x1E);
        wr(dta, d);
        carry(false, 0);
        return;
    }
    if (ah == 0x4F) { carry(true, 18); return; }
    if (ah == 0x56) {                                  // rename DS:DX to ES:DI
        std::wstring old = hostpath(cstr(ds, dx));
        std::wstring nw = hostpath(cstr(es, R(UC_X86_REG_DI)));
        if (!fsisfile(old)) { carry(true, 2); return; }
        if (fsisfile(nw)) { carry(true, 5); return; }
        fsreplace(old, nw);
        carry(false);
        return;
    }
    if (ah == 0x57) { W(UC_X86_REG_CX, 0); W(UC_X86_REG_DX, 0x1221); carry(false); return; }
    if (ah == 0x59) { W(UC_X86_REG_AX, 2); return; }
    if (ah == 0x62 || ah == 0x51) { W(UC_X86_REG_BX, cur_psp); return; }
    if (ah == 0x50) { cur_psp = bx; return; }
    if (ah == 0x26 || ah == 0x55) {                    // create a (child) PSP at DX, from the current one
        Bytes nw = rd(cur_psp * 16, 256);
        if (ah == 0x55) {
            le(nw, 2, R(UC_X86_REG_SI) & 0xFFFF, 2);
            le(nw, 0x16, cur_psp & 0xFFFF, 2);
            le(nw, 0x32, 20, 2);
            le(nw, 0x34, 0x18, 2);
            le(nw, 0x36, dx, 2);
            cur_psp = dx;
        }
        wr(dx * 16, nw);
        if (ah == 0x55) W(UC_X86_REG_AX, (uint64_t(ah) << 8) | 0xF0);
        return;
    }
    if (ah == 0x0D || ah == 0x68) return;
    if (ah == 0x87) { W(UC_X86_REG_AX, ax & 0xFF00); return; }   // no such function on MS-DOS (XENIX answers it)
    if (ah == 0x4B) {                                  // EXEC: the child runs to its end in an emulator of its own
        std::wstring name = cstr(ds, dx), hp = hostpath(name);
        if (!fsisfile(hp)) { carry(true, 2); return; }
        if (al == 3) {                                 // load an overlay: the image (relocated) at a segment, not run
            Bytes p = rd(lin(es, bx), 4);
            uint64_t lseg = p[0] | p[1] << 8, rfac = p[2] | p[3] << 8;
            Bytes ob = fs ? fs->read(hp) : disk_read(hp);
            Bytes oimg = load_overlay(ob, rfac);
            wr(lseg * 16, oimg);
            if (S.debug) fprintf(stderr, "overlay %s at %04llx reloc %04llx: %zu bytes\n", utf8(name).c_str(),
                                 (unsigned long long)lseg, (unsigned long long)rfac, oimg.size());
            carry(false);
            return;
        }
        uint64_t blk = es * 16 + bx;
        Bytes pb = rd(blk, 6);
        uint64_t envs = pb[0] | pb[1] << 8, toff = pb[2] | pb[3] << 8, tseg = pb[4] | pb[5] << 8;
        uint8_t n_ = rd1(tseg * 16 + toff);
        Bytes tail = rd(tseg * 16 + toff + 1, n_);
        Bytes cenv = env;
        if (envs) {
            uint64_t a = envs * 16;
            Bytes e;
            for (;;) {
                Bytes two = rd(a, 2);
                if ((two[0] == 0 && two[1] == 0) || e.size() >= 4096) break;
                e.push_back(rd1(a));
                a++;
            }
            e.push_back(0);
            cenv = e;
        }
        size_t s = 0;
        while (s < tail.size() && tail[s] == ' ') s++;
        Run c;
        c.exe = hp;
        c.workdir = workdir;
        c.cmdline.assign(tail.begin() + s, tail.end());
        c.stdin_data.assign(in.begin(), in.end());
        c.max_blocks = max_blocks;
        c.env = cenv;
        c.PSP = PSP;
        c.fast = fast;
        c.fs = fs;
        c.S = S;
        c.go();
        console.insert(console.end(), c.console.begin(), c.console.end());
        child = c.exit ? *c.exit : 0;
        if (S.debug) fprintf(stderr, "exec %s -> %lld\n", utf8(name).c_str(), (long long)child);
        carry(false);
        return;
    }
    if (ah == 0x4D) { W(UC_X86_REG_AX, uint64_t(child) & 0xFF); return; }
    if (ah == 0x63) {                                  // the table of lead bytes (two-byte characters): none
        uint8_t z[2] = {0, 0};
        wr(uint64_t(PSP) * 16 + 0x48, z, 2);
        Wseg(false, PSP);
        W(UC_X86_REG_SI, 0x48);
        carry(false);
        return;
    }
    throw runtime_error(fmt("INT 21h AH=%02X not implemented (AX=%04X)", ah, unsigned(ax)));
}

// ---- interrupts ---------------------------------------------------------------------------------
void Run::dos_int(int intno) {
    if (intno == 0x21) { int21(); return; }
    if (intno == 0x20) { exit = 0; throw DosExit(); }
    if (intno == 0x10 || intno == 0x1A || intno == 0x12 || intno == 0x11 || intno == 0x16 || intno == 0x2F) return;
    if (intno == 0x15) {                               // BIOS system services: no extended memory by INT 15h
        if (S.debug) fprintf(stderr, "int15 ax=%04llx\n", (unsigned long long)R(UC_X86_REG_AX));
        if ((R(UC_X86_REG_AX) >> 8) == 0x88) { W(UC_X86_REG_AX, 0); carry(false); }
        else { W(UC_X86_REG_AX, (uint64_t(0x86) << 8) | (R(UC_X86_REG_AX) & 0xFF)); carry(true); }
        return;
    }
    auto it = vec.find(intno);
    if (it != vec.end() && it->second != std::pair<uint64_t, uint64_t>(0, 0)) {
        // a handler the program installed itself (an 8087 emulator's INT 34h-3Dh): do what the processor does
        uint64_t sp_ = (R(UC_X86_REG_SP) - 6) & 0xFFFF;
        Bytes fr(6);
        le(fr, 0, R(UC_X86_REG_IP), 2);
        le(fr, 2, R(UC_X86_REG_CS), 2);
        le(fr, 4, R(UC_X86_REG_EFLAGS) & 0xFFFF, 2);
        wr(R(UC_X86_REG_SS) * 16 + sp_, fr);
        W(UC_X86_REG_SP, sp_);
        auto so = it->second;
        set_reg(UC_X86_REG_EFLAGS, R(UC_X86_REG_EFLAGS) & ~uint64_t(0x300));
        W(UC_X86_REG_CS, so.first);
        W(UC_X86_REG_IP, so.second);
        again = true;
        stop();
        return;
    }
    uint64_t at = pc();
    Bytes code = rd(at, 12);
    throw runtime_error(fmt("INT %02Xh not implemented (at %04llx:%08llx = %llx: %s)", intno,
                            (unsigned long long)R(UC_X86_REG_CS), (unsigned long long)R(UC_X86_REG_EIP),
                            (unsigned long long)at, hex_str(code.data(), code.size()).c_str()));
}

void Run::intr(int intno) {
    if (host && host->handle(intno)) return;           // DPMI: INT 2Fh/1687h, INT 31h, the switch
    dos_int(intno);
}

static void hook_intr(uc_engine *, uint32_t intno, void *user) {
    Run &r = *static_cast<Run *>(user);
    try {
        r.intr(int(intno));
    } catch (DosExit &) {
        r.stop();
    } catch (PyErr &e) {
        r.err.push_back(e);
        r.stop();
    } catch (ArenaError &e) {
        r.err.push_back(PyErr{CBD_OTHER, 0, fmt("ArenaError: %d", e.code), L""});
        r.stop();
    } catch (std::exception &e) {
        r.err.push_back(PyErr{CBD_OTHER, 0, e.what(), L""});
        r.stop();
    }
}

static void hook_block(uc_engine *, uint64_t addr, uint32_t, void *user) {
    Run &r = *static_cast<Run *>(user);
    r.blocks.insert(addr);
    r.n++;
    if (r.n > r.max_blocks) {
        size_t k = r.console.size() > 400 ? r.console.size() - 400 : 0;
        r.err.push_back(runtime_error("block limit at image " + py_hex(int64_t(addr) - int64_t(r.load) * 16, 5) +
                                      "; console so far: " + bytes_repr(r.console.data() + k, r.console.size() - k)));
        r.stop();
    }
}

static void hook_code(uc_engine *, uint64_t addr, uint32_t, void *user) {
    Run &r = *static_cast<Run *>(user);
    uint8_t b0;
    if (addr < RAM) b0 = r.ram[size_t(addr)];
    else if (UC.mem_read(r.uc, addr, &b0, 1) != UC_ERR_OK) return;
    if (b0 == 0xE8 || b0 == 0x9A) r.calls[addr]++;
}

// ---- the program's image ------------------------------------------------------------------------
static PyErr struct_error(const std::string &m) { return PyErr{CBD_STRUCT, 0, m, L""}; }

static uint16_t u16(const Bytes &b, size_t at) {
    if (at + 2 > b.size())
        throw struct_error(fmt("unpack_from requires a buffer of at least %zu bytes for unpacking 2 bytes at offset %zu (actual buffer size is %zu)", at + 2, at, b.size()));
    return uint16_t(b[at] | b[at + 1] << 8);
}

static void relocate(Bytes &image, const Bytes &b, uint64_t nreloc, uint64_t relofs, uint64_t fac) {
    for (uint64_t k = 0; k < nreloc; k++) {
        uint64_t off = u16(b, size_t(relofs + 4 * k)), seg = u16(b, size_t(relofs + 4 * k + 2));
        uint64_t a = seg * 16 + off;
        if (a + 2 > image.size())
            throw struct_error(fmt("pack_into requires a buffer of at least %llu bytes for packing 2 bytes at offset %llu (actual buffer size is %zu)",
                                   (unsigned long long)(a + 2), (unsigned long long)a, image.size()));
        uint64_t v = (uint64_t(image[a] | image[a + 1] << 8) + fac) & 0xFFFF;
        image[a] = uint8_t(v);
        image[a + 1] = uint8_t(v >> 8);
    }
}

Bytes load_overlay(const Bytes &ob, uint64_t rfac) {
    if (ob.size() >= 2 && ((ob[0] == 'M' && ob[1] == 'Z') || (ob[0] == 'Z' && ob[1] == 'M'))) {
        uint64_t lp = u16(ob, 2), pg = u16(ob, 4), nr = u16(ob, 6), hp = u16(ob, 8), ro = u16(ob, 0x18);
        Bytes oimg = py_slice(ob, int64_t(hp * 16), int64_t(pg * 512) - (lp ? int64_t(512 - lp) : 0));
        relocate(oimg, ob, nr, ro, rfac);
        return oimg;
    }
    return ob;
}

// ---- the run (dos.py run) -----------------------------------------------------------------------
namespace {
struct Watchdog {                                      // one timer for a whole DPMI run (threading.Timer)
    std::mutex m;
    std::condition_variable cv;
    bool cancelled = false;
    std::thread t;
    void start(Run &r, double seconds) {
        t = std::thread([this, &r, seconds] {
            std::unique_lock<std::mutex> lk(m);
            if (!cv.wait_for(lk, std::chrono::duration<double>(seconds), [this] { return cancelled; })) {
                r.timedout = true;
                UC.emu_stop(r.uc);
            }
        });
    }
    void cancel() {
        {
            std::lock_guard<std::mutex> lk(m);
            cancelled = true;
        }
        cv.notify_all();
        if (t.joinable()) t.join();
    }
};
}  // namespace

void Run::go() {
    fast = fast && !trace_calls;                       // (fast and not instrument ... and not trace_calls)
    Bytes b = fs ? fs->read(exe) : disk_read(exe);
    if (!(b.size() >= 2 && ((b[0] == 'M' && b[1] == 'Z') || (b[0] == 'Z' && b[1] == 'M'))))
        throw PyErr{CBD_ASSERT, 0, "not an MZ executable", L""};
    if (b.size() < 26)
        throw struct_error(fmt("unpack_from requires a buffer of at least 26 bytes for unpacking 24 bytes at offset 2 (actual buffer size is %zu)", b.size()));
    uint64_t hv[12];
    for (int i = 0; i < 12; i++) hv[i] = u16(b, size_t(2 + 2 * i));
    uint64_t lastpage = hv[0], pages = hv[1], nreloc = hv[2], hdrp = hv[3], ss = hv[6], sp = hv[7], ip = hv[9],
             cs = hv[10], relofs = hv[11];
    int64_t size = int64_t(pages * 512) - (lastpage ? int64_t(512 - lastpage) : 0);
    Bytes image = py_slice(b, int64_t(hdrp * 16), size);
    load = PSP + 0x10;
    relocate(image, b, nreloc, relofs, uint64_t(load));

    ram.assign(0x200000, 0);
    uc_err e = UC.open(UC_ARCH_X86, UC_MODE_16, &uc);
    if (e != UC_ERR_OK) { uc = nullptr; throw uc_error(e); }
    e = UC.mem_map_ptr(uc, 0, RAM, 7, ram.data());
    if (e != UC_ERR_OK) throw uc_error(e);
    wr(uint64_t(load) * 16, image);

    // the environment block, and after its strings the program's own path, as DOS gives it
    int envseg = ENVSEG;
    std::wstring rel = relpath(abspath(exe), abspath(workdir));
    if (rel.compare(0, 2, L"..") == 0) rel = basename(exe);
    for (auto &c : rel)
        if (c == L'/') c = L'\\';
    Bytes relb;
    if (!to_latin1(py_upper(rel), relb))
        throw PyErr{CBD_OTHER, 0, "UnicodeEncodeError: 'latin-1' codec can't encode a character", L""};
    Bytes envb = env;
    envb.push_back(0);
    envb.push_back(1);
    envb.push_back(0);
    envb.push_back('C');
    envb.push_back(':');
    envb.push_back('\\');
    envb.insert(envb.end(), relb.begin(), relb.end());
    envb.push_back(0);
    wr(uint64_t(envseg) * 16, envb);
    // the arena: the environment, DOS's own block over any gap up to the program, the program's block
    int64_t envparas = std::min<int64_t>((int64_t(envb.size()) + 15) / 16, PSP - 1 - envseg);
    mcb_write(*this, envseg - 1, 'M', PSP, envparas);
    int64_t gap = PSP - 1 - (envseg + envparas);
    if (gap > 0) mcb_write(*this, envseg + envparas, 'M', MCB_DOS, gap - 1);
    mcb_write(*this, PSP - 1, 'Z', PSP, TOP - PSP);
    Bytes lol(LOL_SIZE, 0);                            // INT 21h 52h's list of lists: the first MCB just before it
    le(lol, 0, uint64_t(envseg - 1), 2);
    wr(uint64_t(DOSSEG) * 16 + LOL_OFF - 2, lol);
    Bytes psp(256, 0);
    psp[0] = 0xCD;
    psp[1] = 0x20;
    le(psp, 2, uint64_t(TOP), 2);
    le(psp, 0x2C, uint64_t(envseg), 2);
    le(psp, 0x16, uint64_t(PSP), 2);                   // (parent: itself, as the shell's)
    const uint8_t jft[5] = {1, 1, 1, 0, 2};            // the job file table: CON CON CON AUX PRN
    for (int i = 0; i < 20; i++) psp[0x18 + i] = i < 5 ? jft[i] : 0xFF;
    le(psp, 0x32, 20, 2);
    le(psp, 0x34, 0x18, 2);
    le(psp, 0x36, uint64_t(PSP), 2);
    psp[0x50] = 0xCD;
    psp[0x51] = 0x21;
    psp[0x52] = 0xCB;
    Bytes tail;
    if (!cmdline.empty()) {
        tail.push_back(' ');
        tail.insert(tail.end(), cmdline.begin(), cmdline.end());
    }
    if (tail.size() > 126) tail.resize(126);
    psp[0x80] = uint8_t(tail.size());
    std::copy(tail.begin(), tail.end(), psp.begin() + 0x81);
    psp[0x81 + tail.size()] = 0x0D;
    wr(uint64_t(PSP) * 16, psp);
    set_reg(UC_X86_REG_CS, cs + load);
    set_reg(UC_X86_REG_IP, ip);
    set_reg(UC_X86_REG_SS, ss + load);
    set_reg(UC_X86_REG_SP, sp);
    set_reg(UC_X86_REG_DS, uint64_t(PSP));
    set_reg(UC_X86_REG_ES, uint64_t(PSP));
    set_reg(UC_X86_REG_AX, 0);
    set_reg(UC_X86_REG_EFLAGS, 0x202);

    in.assign(stdin_data.begin(), stdin_data.end());
    dta = uint64_t(PSP) * 16 + 0x80;
    cur_psp = uint64_t(PSP);
    strategy = 0;
    pm = false;
    first_mcb = envseg - 1;

    if (want_dpmi) {
        host = std::make_unique<Host>(*this);
        host->attach();
    }
    uint64_t until = host ? 0x1000000 : 0x10FFFF;      // protected-mode code/tables live above 1 MB
    uc_hook h1, h2, h3;
    e = UC.hook_add(uc, &h1, UC_HOOK_INTR, (void *)hook_intr, this, 1, 0);
    if (e != UC_ERR_OK) throw uc_error(e);
    if (!fast) {
        e = UC.hook_add(uc, &h2, UC_HOOK_BLOCK, (void *)hook_block, this, 1, 0);
        if (e != UC_ERR_OK) throw uc_error(e);
    }
    uint64_t timeout = fast ? uint64_t(S.fast_limit * 1e6) : 0;
    bool use_watchdog = fast && host;
    double limit = std::max(S.fast_limit, S.dpmi_limit);
    if (use_watchdog) timeout = 0;
    if (trace_calls) {
        e = UC.hook_add(uc, &h3, UC_HOOK_CODE, (void *)hook_code, this, 1, 0);
        if (e != UC_ERR_OK) throw uc_error(e);
    }
    Watchdog wd;
    auto t_start = std::chrono::system_clock::now();
    if (use_watchdog) wd.start(*this, limit);
    try {
        e = UC.emu_start(uc, (cs + load) * 16 + ip, until, timeout, 0);
        if (e != UC_ERR_OK) throw uc_error(e);
        while (again && !exit && err.empty() && !timedout) {
            // (Unicorn's 16-bit mode takes a start address as CS*16 + IP in either CPU mode)
            again = false;
            e = UC.emu_start(uc, R(UC_X86_REG_CS) * 16 + R(UC_X86_REG_IP), until, timeout, 0);
            if (e != UC_ERR_OK) throw uc_error(e);
        }
        double took = std::chrono::duration<double>(std::chrono::system_clock::now() - t_start).count();
        if (fast && !exit && err.empty() && (timedout || (!use_watchdog && took >= S.fast_limit * 0.9))) {
            std::string extra;
            if (host)
                extra = fmt(" (cr0 %llx cs:eip %04llx:%08llx)", (unsigned long long)R(UC_X86_REG_CR0),
                            (unsigned long long)R(UC_X86_REG_CS), (unsigned long long)R(UC_X86_REG_EIP));
            size_t k = console.size() > 400 ? console.size() - 400 : 0;
            err.push_back(runtime_error("time limit at " + py_hex(int64_t(pc()) - int64_t(load) * 16, 5) + extra +
                                        "; console so far: " + bytes_repr(console.data() + k, console.size() - k)));
        }
    } catch (PyErr &x) {
        if (x.kind != CBD_UC) { wd.cancel(); throw; }
        err.push_back(runtime_error("cpu fault " + x.message + " at " + py_hex(int64_t(pc()), 5)));
    }
    wd.cancel();
    for (auto &kv : files)
        if (kv.second.f) kv.second.f->close();
    if (!err.empty() && !exit)
        throw err[0];
}

}  // namespace cbd
