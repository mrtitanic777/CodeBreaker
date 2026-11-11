// cb/formats/dos_unpack.cpp -- undo any on-disk compression of a DOS program by letting its unpacker run.
//
// A compressed program starts in a small unpacker that rebuilds the real program in memory, fixes its segment
// references for where it was loaded, sets the stack, and jumps to the real entry point. That is run here, in real
// mode, on a bare megabyte with a program prefix in front, and stopped at that jump. Which jump it is follows from
// who wrote what: the unpacker's own code came from the file (generation 0); most unpackers first copy themselves
// out of the way (generation 1, written by generation 0) and the program they rebuild is then generation 2.
// Execution reaching generation 2 is the real entry point, and so is a jump out of the moved copy down to bytes of
// the file (a scheme that leaves the front of the program as it was stored). An unpacker that does not move itself
// rebuilds the program as generation 1; it is recognised when the code it jumped to -- code that is not a copy of
// bytes of the file -- makes its first call to DOS.
//
// Some programs are not compressed but still begin by moving themselves (COMMAND.COM slides down over its program
// prefix's unused end and continues there). `only_moved` runs such a start until it continues in bytes it wrote,
// and returns memory as it then is.
//
// The unpacker has by then applied the relocations for one load segment and thrown the table away. It is run twice
// at two load segments: every word that differs by exactly the distance between them is a relocation, and anything
// else that differs is reported as a failure.
#include <algorithm>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cb/core/unicorn_api.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/read.hpp"

namespace cb::formats::dos {

namespace {

constexpr std::int64_t kMemory = 0x110000;
constexpr std::int64_t kTop = 0xA000;  // first segment past conventional memory

struct State {
    std::int64_t cs = 0, ip = 0, ss = 0, sp = 0;
};

struct UcDeleter {
    void operator()(uc_engine* uc) const {
        if (uc) emu::unicorn::api().uc_close(uc);
    }
};

class Run {
public:
    Run(ByteView module, const std::vector<std::uint32_t>& relocations, std::uint32_t cs, std::uint32_t ip,
        std::uint32_t ss, std::uint32_t sp, std::uint32_t load, std::uint64_t limit, bool only_moved)
        : moved_(only_moved), size_(static_cast<std::int64_t>(module.size())), module_(module.begin(), module.end()),
          load_(load), base_(std::int64_t{load} * 16), gen_(static_cast<std::size_t>(kMemory)), when_(static_cast<std::size_t>(kMemory)) {
        const auto& u = api_;
        uc_engine* raw = nullptr;
        emu::unicorn::check(u.uc_open(UC_ARCH_X86, UC_MODE_16, &raw), "uc_open");
        uc_.reset(raw);
        emu::unicorn::check(u.uc_mem_map(raw, 0, kMemory, UC_PROT_ALL), "uc_mem_map");
        Bytes img = module_;
        for (std::uint32_t at : relocations) {
            if (at + 2 > img.size()) throw FormatError("dos-unpack", at, "relocation outside the module");
            const std::uint16_t v = static_cast<std::uint16_t>(img[at] | (img[at + 1] << 8));
            const std::uint16_t r = static_cast<std::uint16_t>((v + load) & 0xFFFF);
            img[at] = static_cast<std::uint8_t>(r);
            img[at + 1] = static_cast<std::uint8_t>(r >> 8);
        }
        const std::int64_t psp = std::int64_t{load} - 0x10;
        std::uint8_t prefix[0x100] = {};
        prefix[0] = 0xCD, prefix[1] = 0x20;
        prefix[2] = static_cast<std::uint8_t>(kTop), prefix[3] = static_cast<std::uint8_t>(kTop >> 8);
        prefix[0x2C] = static_cast<std::uint8_t>(psp - 0x20), prefix[0x2D] = static_cast<std::uint8_t>((psp - 0x20) >> 8);  // an (empty) environment
        prefix[0x80] = 0x00, prefix[0x81] = 0x0D;
        emu::unicorn::check(u.uc_mem_write(raw, static_cast<std::uint64_t>(psp * 16), prefix, sizeof prefix), "uc_mem_write");
        emu::unicorn::check(u.uc_mem_write(raw, static_cast<std::uint64_t>(base_), img.data(), img.size()), "uc_mem_write");
        set(UC_X86_REG_DS, static_cast<std::uint64_t>(psp));
        set(UC_X86_REG_ES, static_cast<std::uint64_t>(psp));
        set(UC_X86_REG_SS, (load + ss) & 0xFFFF);
        set(UC_X86_REG_SP, sp);
        set(UC_X86_REG_CS, (load + cs) & 0xFFFF);
        uc_hook h;
        emu::unicorn::check(u.uc_hook_add(raw, &h, UC_HOOK_BLOCK, reinterpret_cast<void*>(&Run::on_block), this, 1, 0), "uc_hook_add");
        emu::unicorn::check(u.uc_hook_add(raw, &h, UC_HOOK_MEM_WRITE, reinterpret_cast<void*>(&Run::on_write), this, 1, 0), "uc_hook_add");
        emu::unicorn::check(u.uc_hook_add(raw, &h, UC_HOOK_INTR, reinterpret_cast<void*>(&Run::on_interrupt), this, 1, 0), "uc_hook_add");
        const uc_err e = u.uc_emu_start(raw, ((std::uint64_t{load} + cs) & 0xFFFF) * 16 + ip, 0, 0, limit);
        if (e != UC_ERR_OK && !error_) error_ = u.uc_strerror(e);
    }

    std::int64_t load() const { return load_; }
    const std::optional<State>& found() const { return found_; }
    bool exited() const { return exited_; }
    const std::optional<std::string>& error() const { return error_; }

    // The rebuilt program: from the load segment up to the unpacker's moved copy, less the end of compressed data
    // an unpacker moved up out of the way left off.
    Bytes image() const {
        std::int64_t end;
        int want;
        if (found_is_first_) {  // an unpacker that did not move
            end = kTop * 16, want = 1;
            if (moved_) {  // ... or a program that only moved: all of it
                std::int64_t last = 0;
                for (std::int64_t i = base_; i < end; ++i)
                    if (gen_[static_cast<std::size_t>(i)]) last = i;
                const std::int64_t top = std::max(base_ + size_, last + 1);
                return read(base_, top - base_);
            }
        } else {
            end = std::min(stub_low_, kTop * 16), want = 2;
        }
        while (end > base_ && gen_[static_cast<std::size_t>(end - 1)] < want) --end;
        const std::int64_t middle = base_ + (end - base_) / 2;
        std::vector<std::uint32_t> head, body;
        for (std::int64_t i = base_; i < std::min(base_ + 16, end); ++i)
            if (gen_[static_cast<std::size_t>(i)] >= want) head.push_back(when_[static_cast<std::size_t>(i)]);
        for (std::int64_t i = middle; i < std::min(middle + 16, end); ++i)
            if (gen_[static_cast<std::size_t>(i)] >= want) body.push_back(when_[static_cast<std::size_t>(i)]);
        std::uint32_t since = 0;
        if (head.size() == 16 && !body.empty()) {
            const std::uint32_t h = *std::min_element(head.begin(), head.end());
            if (h < *std::min_element(body.begin(), body.end())) since = h;
        }
        while (end > base_ && (gen_[static_cast<std::size_t>(end - 1)] < want || when_[static_cast<std::size_t>(end - 1)] < since)) --end;
        return read(base_, end - base_);
    }

private:
    static void on_block(uc_engine* u, std::uint64_t address, std::uint32_t, void* self) { static_cast<Run*>(self)->block(u, address); }
    static void on_write(uc_engine*, uc_mem_type, std::uint64_t address, int size, std::int64_t, void* self) {
        static_cast<Run*>(self)->write(address, size);
    }
    static void on_interrupt(uc_engine* u, std::uint32_t number, void* self) { static_cast<Run*>(self)->interrupt(u, number); }

    std::uint64_t reg(int r) const {
        std::uint64_t v = 0;
        api_.uc_reg_read(uc_.get(), r, &v);
        return v;
    }
    void set(int r, std::uint64_t v) const { api_.uc_reg_write(uc_.get(), r, &v); }

    // CS from the address: just after a far jump it can be the old one.
    State state(std::uint64_t address) const {
        const std::int64_t ip = static_cast<std::int64_t>(reg(UC_X86_REG_IP));
        return State{((static_cast<std::int64_t>(address) - ip) >> 4) & 0xFFFF, ip, static_cast<std::int64_t>(reg(UC_X86_REG_SS)),
                     static_cast<std::int64_t>(reg(UC_X86_REG_SP))};
    }

    void stop(uc_engine* u) { api_.uc_emu_stop(u); }

    void block(uc_engine* u, std::uint64_t address) {
        const std::int64_t a = static_cast<std::int64_t>(address);
        const int g = a < kMemory ? gen_[static_cast<std::size_t>(a)] : 0;
        const bool left = now_ >= 1 && g == 0 && base_ <= a && a < stub_low_;
        now_ = g;
        if (g >= 2 || left) {
            found_ = state(address);
            found_is_first_ = false;
            stop(u);
        } else if (g == 1 && !first_) {
            first_ = state(address);
            std::uint8_t head[24];
            const uc_err e = api_.uc_mem_read(u, address, head, sizeof head);
            if (e != UC_ERR_OK) {
                error_ = api_.uc_strerror(e);
                stop(u);
                return;
            }
            // the first written code to run is a copy of the file's own bytes
            copied_ = std::search(module_.begin(), module_.end(), head, head + sizeof head) != module_.end();
            if (moved_) {  // all that was asked for
                found_ = first_;
                found_is_first_ = true;
                stop(u);
            }
            std::int64_t low = a;
            while (low > base_ && gen_[static_cast<std::size_t>(low - 1)] == 1) --low;
            stub_low_ = low;  // where the unpacker's moved copy starts
        }
    }

    void write(std::uint64_t address, int size) {
        const std::int64_t top = static_cast<std::int64_t>(reg(UC_X86_REG_SS)) * 16 + static_cast<std::int64_t>(reg(UC_X86_REG_SP));
        const std::int64_t a = static_cast<std::int64_t>(address);
        if ((-8 <= a - top && a - top < 64) || a + size > kMemory) return;  // the stack is not the program
        const std::uint8_t g = static_cast<std::uint8_t>(std::min(now_ + 1, 3));
        ++clock_;
        for (std::int64_t i = a; i < a + size; ++i) {
            gen_[static_cast<std::size_t>(i)] = g;
            when_[static_cast<std::size_t>(i)] = clock_;
        }
    }

    void interrupt(uc_engine* u, std::uint32_t number) {
        const std::uint64_t ah = (reg(UC_X86_REG_AX) >> 8) & 0xFF;
        const std::uint64_t flags = reg(UC_X86_REG_EFLAGS);
        if (number == 0x20 || (number == 0x21 && (ah == 0x00 || ah == 0x4C))) {
            exited_ = true;
            stop(u);
        } else if (number == 0x21 && ah == 0x30) {  // DOS version 7.10
            set(UC_X86_REG_AX, 0x0A07);
        } else if (number == 0x21 && (ah == 0x09 || ah == 0x02 || ah == 0x4A || ah == 0x25 || ah == 0x35 || ah == 0x1A)) {
            set(UC_X86_REG_EFLAGS, flags & ~std::uint64_t{1});
        } else if (now_ >= 1 && first_ && !copied_) {  // the rebuilt program asks DOS for something
            found_ = first_;
            found_is_first_ = true;
            stop(u);
        } else {
            set(UC_X86_REG_EFLAGS, flags & ~std::uint64_t{1});
        }
    }

    Bytes read(std::int64_t at, std::int64_t n) const {
        Bytes out(static_cast<std::size_t>(std::max<std::int64_t>(n, 0)), 0);
        if (n > 0) emu::unicorn::api().uc_mem_read(uc_.get(), static_cast<std::uint64_t>(at), out.data(), out.size());
        return out;
    }

    const emu::unicorn::Api& api_ = emu::unicorn::api();  // looked up once: the hooks run on every block and write
    bool moved_;
    std::int64_t size_;
    Bytes module_;
    std::int64_t load_, base_;
    std::unique_ptr<uc_engine, UcDeleter> uc_;
    std::vector<std::uint8_t> gen_;       // who wrote each byte: 0 = the file
    std::vector<std::uint32_t> when_;     // ... and when (a count of writes)
    int now_ = 0;                         // generation of the code running
    bool copied_ = false;                 // the first written code to run is a copy of the file's
    std::optional<State> first_, found_;  // state when written code first ran / at the real entry
    bool found_is_first_ = false;
    bool exited_ = false;
    std::int64_t stub_low_ = kMemory;     // start of the unpacker's moved copy
    std::uint32_t clock_ = 0;
    std::optional<std::string> error_;
};

std::uint16_t word(const Bytes& s, std::size_t i) { return static_cast<std::uint16_t>(s[i] | (s[i + 1] << 8)); }

}  // namespace

Unpack unpack_emulated(ByteView module, const std::vector<std::uint32_t>& relocations, std::uint32_t cs, std::uint32_t ip,
                       std::uint32_t ss, std::uint32_t sp, bool only_moved, std::uint64_t instruction_limit) {
    std::unique_ptr<Run> runs[2];
    const std::uint32_t loads[2] = {0x1000, 0x1731};
    for (int k = 0; k < 2; ++k) {
        runs[k] = std::make_unique<Run>(module, relocations, cs, ip, ss, sp, loads[k], instruction_limit, only_moved);
        const Run& r = *runs[k];
        if (!r.found())
            throw EmulationError("no entry point reached (" +
                                 (r.error() ? *r.error() : r.exited() ? std::string("the unpacker ended the program") : std::string("instruction limit")) + ")");
    }
    const Run& a = *runs[0];
    const Run& b = *runs[1];
    const Bytes one = a.image(), two = b.image();
    if (one.size() != two.size())
        throw EmulationError("two runs rebuilt " + std::to_string(one.size()) + " and " + std::to_string(two.size()) + " bytes");
    const std::int64_t delta = (b.load() - a.load()) & 0xFFFF;
    Bytes img = one;
    std::set<std::uint32_t> relocs;  // sorted: the result lists them in order
    const std::size_t n = one.size();
    std::size_t i = 0;
    std::int64_t stray = 0;
    while (i < n) {
        if (one[i] == two[i]) {
            ++i;
            continue;
        }
        const std::int64_t w1 = i + 2 <= n ? word(one, i) : -1;
        const std::int64_t w2 = i + 2 <= n ? word(two, i) : -1;
        if (w1 >= 0 && ((w2 - w1) & 0xFFFF) == delta) {
            relocs.insert(static_cast<std::uint32_t>(i));
            const std::uint16_t r = static_cast<std::uint16_t>((w1 - a.load()) & 0xFFFF);
            img[i] = static_cast<std::uint8_t>(r);
            img[i + 1] = static_cast<std::uint8_t>(r >> 8);
            i += 2;
        } else {  // (the low bytes of a relocated word always differ)
            ++stray;
            ++i;
        }
    }
    if (stray > 16) throw EmulationError(std::to_string(stray) + " bytes differ between two runs beyond relocations");
    const State fa = *a.found(), fb = *b.found();
    const std::int64_t rel0 = (fa.cs - a.load()) & 0xFFFF, rel1 = (fb.cs - b.load()) & 0xFFFF;
    if (rel0 != rel1 || fa.ip != fb.ip) throw EmulationError("two runs reached different entry points");
    Unpack out;
    out.module = std::move(img);
    out.relocations.assign(relocs.begin(), relocs.end());
    out.cs = static_cast<std::uint32_t>(rel0);
    out.ip = static_cast<std::uint32_t>(fa.ip);
    out.ss = static_cast<std::uint32_t>((fa.ss - a.load()) & 0xFFFF);
    out.sp = static_cast<std::uint32_t>(fa.sp);
    return out;
}

}  // namespace cb::formats::dos
