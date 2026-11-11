// codebreaker/emulate/unicorn_uc.cpp -- the Unicorn-backed core16::Uc (see unicorn_uc.hpp).
#include "codebreaker/emulate/unicorn_uc.hpp"

#include <windows.h>

#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

#include <exception>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/emulate/native_trace.hpp"

namespace codebreaker::emulate::unicorn_uc {
namespace {

using native_trace::UcError;

struct Lib {
    decltype(&uc_open) open = nullptr;
    decltype(&uc_close) close = nullptr;
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_mem_unmap) mem_unmap = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_emu_stop) emu_stop = nullptr;
    decltype(&uc_strerror) strerror = nullptr;
    decltype(&uc_hook_add) hook_add = nullptr;
    decltype(&uc_context_alloc) context_alloc = nullptr;
    decltype(&uc_context_save) context_save = nullptr;
    decltype(&uc_context_restore) context_restore = nullptr;
    decltype(&uc_context_free) context_free = nullptr;
    decltype(&uc_ctl) ctl = nullptr;
};
Lib L;
bool g_loaded = false;

template <class T> bool sym(HMODULE m, const char* name, T& slot) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    return slot != nullptr;
}

const Lib& lib() {
    // (nobody called load_unicorn(): Python's `import unicorn` finds the package itself, so look for its DLL as it would)
    if (!g_loaded) load_unicorn(base::capstone_dll::find_unicorn());
    return L;
}

void check(uc_err e) {
    if (e != UC_ERR_OK) throw UcError(static_cast<int64_t>(e));
}

struct x86_reg_row {
    const char* name;
    int id;
};
const x86_reg_row REGS[] = {
#include "codebreaker/emulate/unicorn_x86_regs.inc"
};

// Unicorn keeps global state (the TCG code buffer) that opening, closing and mapping touch: the gate opens machines on several
// threads (Python's unicorn binding is called under the GIL-held ctypes layer), so those four calls take one lock.
std::mutex& uc_global() {
    static std::mutex m;
    return m;
}

class UnicornUc;

struct HookData {
    UnicornUc* owner;
    std::function<void(core16::Uc&, int64_t, int64_t)> callback;
};

class UnicornUc : public core16::Uc {
  public:
    explicit UnicornUc(int mode) {
        const Lib& l = lib();
        std::lock_guard<std::mutex> hold(uc_global());
        check(l.open(UC_ARCH_X86, static_cast<uc_mode>(mode), &uc_));
    }
    ~UnicornUc() override {
        if (uc_) {
            std::lock_guard<std::mutex> hold(uc_global());
            L.close(uc_);
        }
    }
    UnicornUc(const UnicornUc&) = delete;
    UnicornUc& operator=(const UnicornUc&) = delete;

    void* handle() const override { return uc_; }
    void mem_map(int64_t address, int64_t size) override {
        std::lock_guard<std::mutex> hold(uc_global());
        check(L.mem_map(uc_, static_cast<uint64_t>(address), static_cast<size_t>(size), UC_PROT_ALL));
    }
    void mem_unmap(int64_t address, int64_t size) override {
        std::lock_guard<std::mutex> hold(uc_global());
        check(L.mem_unmap(uc_, static_cast<uint64_t>(address), static_cast<size_t>(size)));
    }
    py::bytes mem_read(int64_t address, int64_t size) override {
        std::string buf(static_cast<size_t>(size), '\0');
        check(L.mem_read(uc_, static_cast<uint64_t>(address), &buf[0], static_cast<size_t>(size)));
        return py::bytes(buf);
    }
    void mem_write(int64_t address, const py::bytes& data) override {
        check(L.mem_write(uc_, static_cast<uint64_t>(address), data.raw().data(), data.raw().size()));
    }
    int64_t reg_read(int64_t reg) override {
        uint64_t v[2] = {0, 0};
        check(L.reg_read(uc_, static_cast<int>(reg), v));
        return static_cast<int64_t>(v[0]);
    }
    // float(uc.reg_read(UC_X86_REG_ST0)): the binding hands the register back as one integer
    double reg_read_float(int64_t reg) override {
        uint64_t v[2] = {0, 0};
        check(L.reg_read(uc_, static_cast<int>(reg), v));
        return static_cast<double>(v[0]);
    }
    void reg_write(int64_t reg, int64_t value) override {
        uint64_t v = static_cast<uint64_t>(value);
        check(L.reg_write(uc_, static_cast<int>(reg), &v));
    }
    void reg_write_descriptor(int64_t reg, const std::tuple<int64_t, int64_t, int64_t, int64_t>& value) override {
        uc_x86_mmr m;
        m.selector = static_cast<uint16_t>(std::get<0>(value));
        m.base = static_cast<uint64_t>(std::get<1>(value));
        m.limit = static_cast<uint32_t>(std::get<2>(value));
        m.flags = static_cast<uint32_t>(std::get<3>(value));
        check(L.reg_write(uc_, static_cast<int>(reg), &m));
    }
    std::shared_ptr<void> context_save() override {
        uc_context* c = nullptr;
        check(L.context_alloc(uc_, &c));
        uc_err e = L.context_save(uc_, c);
        if (e != UC_ERR_OK) {
            L.context_free(c);
            throw UcError(static_cast<int64_t>(e));
        }
        return std::shared_ptr<void>(c, [](void* p) { L.context_free(static_cast<uc_context*>(p)); });
    }
    void context_restore(const std::shared_ptr<void>& context) override {
        check(L.context_restore(uc_, static_cast<uc_context*>(context.get())));
    }
    void ctl_flush_tb() override { check(L.ctl(uc_, UC_CTL_WRITE(UC_CTL_TB_FLUSH, 0))); }
    void emu_start(int64_t begin, int64_t until, int64_t timeout, int64_t count) override {
        pending_ = nullptr;
        uc_err e = L.emu_start(uc_, static_cast<uint64_t>(begin), static_cast<uint64_t>(until),
                               static_cast<uint64_t>(timeout), static_cast<size_t>(count));
        if (pending_) {
            std::exception_ptr p = pending_;
            pending_ = nullptr;
            std::rethrow_exception(p);
        }
        check(e);
    }
    void emu_stop() override { check(L.emu_stop(uc_)); }
    void hook_add_code(const std::function<void(core16::Uc&, int64_t, int64_t)>& callback, int64_t begin,
                       int64_t end) override {
        hooks_.push_back(std::make_unique<HookData>(HookData{this, callback}));
        uc_hook hh = 0;
        check(L.hook_add(uc_, &hh, UC_HOOK_CODE, reinterpret_cast<void*>(&code_hook), hooks_.back().get(),
                         static_cast<uint64_t>(begin), static_cast<uint64_t>(end)));
    }

    void hook_add_insn_in(const std::function<int64_t(core16::Uc&, int64_t, int64_t)>& callback) override {
        in_cb_ = callback;
        uc_hook hh = 0;
        check(L.hook_add(uc_, &hh, UC_HOOK_INSN, reinterpret_cast<void*>(&in_hook), this, uint64_t(1), uint64_t(0),
                         static_cast<int>(UC_X86_INS_IN)));
    }
    void hook_add_insn_out(const std::function<void(core16::Uc&, int64_t, int64_t, int64_t)>& callback) override {
        out_cb_ = callback;
        uc_hook hh = 0;
        check(L.hook_add(uc_, &hh, UC_HOOK_INSN, reinterpret_cast<void*>(&out_hook), this, uint64_t(1), uint64_t(0),
                         static_cast<int>(UC_X86_INS_OUT)));
    }

  private:
    static uint32_t in_hook(uc_engine* uc, uint32_t port, int size, void* user) {
        UnicornUc* self = static_cast<UnicornUc*>(user);
        try {
            return static_cast<uint32_t>(self->in_cb_(*self, port, size));
        } catch (...) {
            self->pending_ = std::current_exception();
            L.emu_stop(uc);
            return 0;
        }
    }
    static void out_hook(uc_engine* uc, uint32_t port, int size, uint32_t value, void* user) {
        UnicornUc* self = static_cast<UnicornUc*>(user);
        try {
            self->out_cb_(*self, port, size, value);
        } catch (...) {
            self->pending_ = std::current_exception();
            L.emu_stop(uc);
        }
    }
    static void code_hook(uc_engine* uc, uint64_t address, uint32_t size, void* user) {
        HookData* h = static_cast<HookData*>(user);
        try {
            h->callback(*h->owner, static_cast<int64_t>(address), static_cast<int64_t>(size));
        } catch (...) {
            h->owner->pending_ = std::current_exception();
            L.emu_stop(uc);
        }
    }

    uc_engine* uc_ = nullptr;
    std::exception_ptr pending_;
    std::vector<std::unique_ptr<HookData>> hooks_;
    std::function<int64_t(core16::Uc&, int64_t, int64_t)> in_cb_;
    std::function<void(core16::Uc&, int64_t, int64_t, int64_t)> out_cb_;
};

}  // namespace

void load_unicorn(const py::str& dll_path) {
    if (g_loaded) return;
    std::wstring w;
    for (char32_t c : dll_path.u32()) {
        if (c >= 0x10000) {
            w.push_back(static_cast<wchar_t>(0xD800 + ((c - 0x10000) >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + ((c - 0x10000) & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    HMODULE u = LoadLibraryW(w.c_str());
    if (!u) throw py::RuntimeError("cannot load unicorn.dll (error " + std::to_string(GetLastError()) + ")");
    g_loaded = sym(u, "uc_open", L.open) && sym(u, "uc_close", L.close) && sym(u, "uc_reg_read", L.reg_read) &&
               sym(u, "uc_reg_write", L.reg_write) && sym(u, "uc_mem_read", L.mem_read) &&
               sym(u, "uc_mem_write", L.mem_write) && sym(u, "uc_mem_map", L.mem_map) &&
               sym(u, "uc_mem_unmap", L.mem_unmap) && sym(u, "uc_emu_start", L.emu_start) &&
               sym(u, "uc_emu_stop", L.emu_stop) && sym(u, "uc_strerror", L.strerror) &&
               sym(u, "uc_hook_add", L.hook_add) && sym(u, "uc_context_alloc", L.context_alloc) &&
               sym(u, "uc_context_save", L.context_save) && sym(u, "uc_context_restore", L.context_restore) &&
               sym(u, "uc_context_free", L.context_free) && sym(u, "uc_ctl", L.ctl);
    if (!g_loaded) throw py::RuntimeError("unicorn.dll lacks an entry point");
}

bool loaded() { return g_loaded; }

py::str strerror(int64_t err) { return py::str(lib().strerror(static_cast<uc_err>(err))); }

std::optional<int64_t> x86_reg(const py::str& name) {
    static const std::map<std::string, int> table = [] {
        std::map<std::string, int> t;
        for (const x86_reg_row& r : REGS) t[r.name] = r.id;
        return t;
    }();
    auto it = table.find(name.utf8());
    if (it == table.end()) return std::nullopt;
    return it->second;
}

py::dict<py::str, int64_t> x86_register_table() {
    py::dict<py::str, int64_t> out;
    for (const x86_reg_row& r : REGS) out.set(py::str("UC_X86_REG_") + py::str(r.name), r.id);
    return out;
}

std::shared_ptr<core16::Uc> open_x86(int64_t bits) {
    int mode = bits == 16 ? UC_MODE_16 : bits == 64 ? UC_MODE_64 : UC_MODE_32;
    return std::make_shared<UnicornUc>(mode);
}

}  // namespace codebreaker::emulate::unicorn_uc
