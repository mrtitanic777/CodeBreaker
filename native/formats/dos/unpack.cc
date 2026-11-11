// unpack.cc -- undo any on-disk compression of a DOS program by letting its unpacker run.
//
// A compressed program starts in a small unpacker that rebuilds the real program in memory, fixes
// its segment references for where it was loaded, sets the stack, and jumps to the real entry
// point. That is run here, in real mode, on a bare megabyte with a program prefix in front, and
// stopped at that jump. Which jump it is follows from who wrote what: the unpacker's own code came
// from the file (generation 0); most unpackers first copy themselves out of the way (generation 1,
// written by generation 0) and the program they rebuild is then generation 2. Execution reaching
// generation 2 is the real entry point, and so is a jump out of the moved copy down to bytes of the
// file (a scheme that leaves the front of the program as it was stored). An unpacker that does not
// move itself rebuilds the program as generation 1; it is recognised when the code it jumped to
// -- code that is not a copy of bytes of the file -- makes its first call to DOS.
//
// Some programs are not compressed but still begin by moving themselves (COMMAND.COM slides down
// over its program prefix's unused end and continues there). `moved` runs such a start until it
// continues in bytes it wrote, and returns memory as it then is.
//
// The unpacker has by then applied the relocations for one load segment and thrown the table
// away. It is run twice at two load segments: every word that differs by exactly the distance
// between them is a relocation, and anything else that differs is reported as a failure.
#include <algorithm>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "../core/uc.h"
#include "dos.h"

namespace cbf::dos {

namespace {
constexpr int64_t kMemory = 0x110000;
constexpr int64_t kTop = 0xA000;             // first segment past conventional memory

struct State {
    int64_t cs, ip, ss, sp;
};

struct Run {
    bool moved;
    int64_t size;
    std::string module;
    bool copied = false;                     // the first written code to run is a copy of the file's
    int64_t load, base;
    uc_engine *uc = nullptr;
    std::vector<uint8_t> gen;                // who wrote each byte: 0 = the file
    std::vector<uint32_t> when;              // ... and when (a count of writes)
    int now = 0;                             // generation of the code running
    std::optional<State> first, found;       // state when written code first ran / at the real entry
    bool found_is_first = false;
    bool exited = false;
    int64_t stub_low = kMemory;              // start of the unpacker's moved copy
    uint32_t clock = 0;
    std::optional<std::string> error;

    ~Run() {
        if (uc) unicorn().close(uc);
    }

    uint64_t reg(int r) const {
        uint64_t v = 0;
        unicorn().reg_read(uc, r, &v);
        return v;
    }
    void set(int r, uint64_t v) const { unicorn().reg_write(uc, r, &v); }

    State state(uint64_t address) const {     // CS from the address: just after a far jump it can be the old one
        const int64_t ip = int64_t(reg(UC_X86_REG_IP));
        return State{((int64_t(address) - ip) >> 4) & 0xFFFF, ip, int64_t(reg(UC_X86_REG_SS)), int64_t(reg(UC_X86_REG_SP))};
    }

    static void on_block(uc_engine *u, uint64_t address, uint32_t, void *self) {
        static_cast<Run *>(self)->block(u, address);
    }
    static void on_write(uc_engine *u, uc_mem_type, uint64_t address, int size, int64_t, void *self) {
        static_cast<Run *>(self)->write(u, address, size);
    }
    static void on_interrupt(uc_engine *u, uint32_t number, void *self) {
        static_cast<Run *>(self)->interrupt(u, number);
    }

    void block(uc_engine *u, uint64_t address) {
        const int g = int64_t(address) < kMemory ? gen[size_t(address)] : 0;
        const bool left = now >= 1 && g == 0 && base <= int64_t(address) && int64_t(address) < stub_low;
        now = g;
        if (g >= 2 || left) {
            found = state(address);
            found_is_first = false;
            unicorn().emu_stop(u);
        } else if (g == 1 && !first) {
            first = state(address);
            char head[24];
            uc_err e = unicorn().mem_read(u, address, head, sizeof head);
            if (e != UC_ERR_OK) {                // (a raise inside a Python hook stops the run with it)
                error = unicorn().strerror(e);
                unicorn().emu_stop(u);
                return;
            }
            copied = module.find(std::string(head, sizeof head)) != std::string::npos;
            if (moved) {                         // all that was asked for
                found = first;
                found_is_first = true;
                unicorn().emu_stop(u);
            }
            int64_t low = int64_t(address);
            while (low > base && gen[size_t(low - 1)] == 1) --low;
            stub_low = low;                      // where the unpacker's moved copy starts
        }
    }

    void write(uc_engine *, uint64_t address, int size) {
        const int64_t top = int64_t(reg(UC_X86_REG_SS)) * 16 + int64_t(reg(UC_X86_REG_SP));
        const int64_t a = int64_t(address);
        if ((-8 <= a - top && a - top < 64) || a + size > kMemory) return;   // the stack is not the program
        const uint8_t g = uint8_t(std::min(now + 1, 3));
        ++clock;
        for (int64_t i = a; i < a + size; ++i) gen[size_t(i)] = g, when[size_t(i)] = clock;
    }

    void interrupt(uc_engine *u, uint32_t number) {
        const uint64_t ah = (reg(UC_X86_REG_AX) >> 8) & 0xFF;
        const uint64_t flags = reg(UC_X86_REG_EFLAGS);
        if (number == 0x20 || (number == 0x21 && (ah == 0x00 || ah == 0x4C))) {
            exited = true;
            unicorn().emu_stop(u);
        } else if (number == 0x21 && ah == 0x30) {   // DOS version 7.10
            set(UC_X86_REG_AX, 0x0A07);
        } else if (number == 0x21 && (ah == 0x09 || ah == 0x02 || ah == 0x4A || ah == 0x25 || ah == 0x35 || ah == 0x1A)) {
            set(UC_X86_REG_EFLAGS, flags & ~uint64_t(1));
        } else if (now >= 1 && first && !copied) {    // the rebuilt program asks DOS for something
            found = first;
            found_is_first = true;
            unicorn().emu_stop(u);
        } else {
            set(UC_X86_REG_EFLAGS, flags & ~uint64_t(1));
        }
    }

    Run(const std::string &mod, const std::vector<int64_t> &relocations, int64_t cs, int64_t ip, int64_t ss, int64_t sp,
        int64_t load_, int64_t limit, bool moved_)
        : moved(moved_), size(int64_t(mod.size())), module(mod), load(load_), base(load_ * 16),
          gen(size_t(kMemory)), when(size_t(kMemory)) {
        const UcLib &U = unicorn();
        uc_err e = U.open(UC_ARCH_X86, UC_MODE_16, &uc);
        if (e != UC_ERR_OK) raise("UcError", U.strerror(e));
        U.mem_map(uc, 0, kMemory, UC_PROT_ALL);
        std::string img = mod;
        for (int64_t at : relocations) {          // struct.unpack_from / pack_into's rules
            const uint16_t v = uint16_t(Bytes(reinterpret_cast<const uint8_t *>(img.data()), img.size()).u16(at));
            if (at < 0) at += int64_t(img.size());
            const uint16_t r = uint16_t((v + load) & 0xFFFF);
            img[size_t(at)] = char(r), img[size_t(at + 1)] = char(r >> 8);
        }
        const int64_t psp = load - 0x10;
        uint8_t prefix[0x100] = {};
        prefix[0] = 0xCD, prefix[1] = 0x20;
        prefix[2] = uint8_t(kTop), prefix[3] = uint8_t(kTop >> 8);
        prefix[0x2C] = uint8_t(psp - 0x20), prefix[0x2D] = uint8_t((psp - 0x20) >> 8);   // an (empty) environment
        prefix[0x80] = 0x00, prefix[0x81] = 0x0D;
        U.mem_write(uc, uint64_t(psp * 16), prefix, sizeof prefix);
        U.mem_write(uc, uint64_t(base), img.data(), img.size());
        set(UC_X86_REG_DS, uint64_t(psp));
        set(UC_X86_REG_ES, uint64_t(psp));
        set(UC_X86_REG_SS, uint64_t((load + ss) & 0xFFFF));
        set(UC_X86_REG_SP, uint64_t(sp));
        set(UC_X86_REG_CS, uint64_t((load + cs) & 0xFFFF));
        uc_hook h;
        U.hook_add(uc, &h, UC_HOOK_BLOCK, reinterpret_cast<void *>(&Run::on_block), this, 1, 0);
        U.hook_add(uc, &h, UC_HOOK_MEM_WRITE, reinterpret_cast<void *>(&Run::on_write), this, 1, 0);
        U.hook_add(uc, &h, UC_HOOK_INTR, reinterpret_cast<void *>(&Run::on_interrupt), this, 1, 0);
        e = U.emu_start(uc, uint64_t(((load + cs) & 0xFFFF) * 16 + ip), 0, 0, size_t(limit));
        if (e != UC_ERR_OK && !error) error = U.strerror(e);
    }

    std::string read(int64_t at, int64_t n) const {
        std::string out(size_t(std::max<int64_t>(n, 0)), '\0');
        if (n > 0) unicorn().mem_read(uc, uint64_t(at), out.data(), out.size());
        return out;
    }

    // the rebuilt program: from the load segment up to the unpacker's moved copy, the end of
    // compressed data an unpacker moved up out of the way left off
    std::string image() const {
        int64_t end;
        int want;
        if (found_is_first) {                    // an unpacker that did not move
            end = kTop * 16, want = 1;
            if (moved) {                         // ... or a program that only moved: all of it
                int64_t last = 0;
                for (int64_t i = base; i < end; ++i)
                    if (gen[size_t(i)]) last = i;
                const int64_t top = std::max(base + size, (last ? last : 0) + 1);
                return read(base, top - base);
            }
        } else {
            end = std::min(stub_low, kTop * 16), want = 2;
        }
        while (end > base && gen[size_t(end - 1)] < want) --end;
        const int64_t middle = base + (end - base) / 2;
        std::vector<uint32_t> head, body;
        for (int64_t i = base; i < std::min(base + 16, end); ++i)
            if (gen[size_t(i)] >= want) head.push_back(when[size_t(i)]);
        for (int64_t i = middle; i < std::min(middle + 16, end); ++i)
            if (gen[size_t(i)] >= want) body.push_back(when[size_t(i)]);
        uint32_t since = 0;
        if (head.size() == 16 && !body.empty()) {
            const uint32_t h = *std::min_element(head.begin(), head.end());
            if (h < *std::min_element(body.begin(), body.end())) since = h;
        }
        while (end > base && (gen[size_t(end - 1)] < want || when[size_t(end - 1)] < since)) --end;
        return read(base, end - base);
    }
};

uint16_t word(const std::string &s, size_t i) { return uint16_t(uint8_t(s[i]) | (uint8_t(s[i + 1]) << 8)); }
}  // namespace

// codebreaker/emulate/unpack.py's unpacked(): RuntimeError when no real entry point is reached or
// the two runs do not agree
Exepack unpack_emulated(const std::string &module, const std::vector<int64_t> &relocations, int64_t cs, int64_t ip,
                        int64_t ss, int64_t sp, bool moved, int64_t limit) {
    std::unique_ptr<Run> runs[2];
    const int64_t loads[2] = {0x1000, 0x1731};
    for (int k = 0; k < 2; ++k) {
        runs[k] = std::make_unique<Run>(module, relocations, cs, ip, ss, sp, loads[k], limit, moved);
        Run &r = *runs[k];
        if (!r.found)
            raise("RuntimeError", "no entry point reached (" +
                                      (r.error ? *r.error : r.exited ? std::string("the unpacker ended the program")
                                                                     : std::string("instruction limit")) + ")");
    }
    Run &a = *runs[0], &b = *runs[1];
    const std::string one = a.image(), two = b.image();
    if (one.size() != two.size())
        raise("RuntimeError", "two runs rebuilt " + std::to_string(one.size()) + " and " + std::to_string(two.size()) + " bytes");
    const int64_t delta = (b.load - a.load) & 0xFFFF;
    std::string img = one;
    std::set<int64_t> out;
    const size_t n = one.size();
    size_t i = 0;
    int64_t stray = 0;
    while (i < n) {
        if (one[i] == two[i]) {
            ++i;
            continue;
        }
        const int64_t w1 = i + 2 <= n ? word(one, i) : -1;
        const int64_t w2 = i + 2 <= n ? word(two, i) : -1;
        if (w1 >= 0 && ((w2 - w1) & 0xFFFF) == delta) {
            out.insert(int64_t(i));
            const uint16_t r = uint16_t((w1 - a.load) & 0xFFFF);
            img[i] = char(r), img[i + 1] = char(r >> 8);
            i += 2;
        } else {                                 // (the low bytes of a relocated word always differ)
            ++stray;
            ++i;
        }
    }
    if (stray > 16) raise("RuntimeError", std::to_string(stray) + " bytes differ between two runs beyond relocations");
    const State fa = *a.found, fb = *b.found;
    const int64_t rel0 = (fa.cs - a.load) & 0xFFFF, rel1 = (fb.cs - b.load) & 0xFFFF;
    if (rel0 != rel1 || fa.ip != fb.ip) raise("RuntimeError", "two runs reached different entry points");
    return Exepack{img, std::vector<int64_t>(out.begin(), out.end()), rel0, fa.ip, (fa.ss - a.load) & 0xFFFF, fa.sp};
}

}  // namespace cbf::dos
