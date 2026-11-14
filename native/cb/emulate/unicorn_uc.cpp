#include "cb/emulate/unicorn_uc.hpp"

#include <algorithm>
#include <exception>
#include <format>
#include <functional>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

namespace cb::emulate::unicorn_uc {

using emu::unicorn::Api;

namespace {

constexpr std::pair<std::string_view, int> kX86Registers[] = {
#include "cb/emulate/tracer/x86_registers.inc"
};

// Unicorn keeps global state (the translator's code buffer) that opening, closing and mapping touch, and the gate opens machines on
// several threads: those four calls take this lock. It guards the library, not any engine, so it is the library's, not a cache.
std::mutex& library_lock() {
    static std::mutex lock;
    return lock;
}

class UnicornUc;

struct CodeHook {
    UnicornUc* owner;
    std::function<void(core16::Uc&, std::uint64_t, std::uint32_t)> callback;
};

class SavedContext final : public core16::Uc::Context {
public:
    SavedContext(const Api& api, uc_context* context) : api_(api), context_(context) {}
    SavedContext(const SavedContext&) = delete;
    SavedContext& operator=(const SavedContext&) = delete;
    ~SavedContext() override { api_.uc_context_free(context_); }
    uc_context* get() const { return context_; }

private:
    const Api& api_;
    uc_context* context_;
};

class UnicornUc final : public core16::Uc {
public:
    UnicornUc(const Api& api, uc_mode mode) : api_(api) {
        const std::lock_guard<std::mutex> hold(library_lock());
        check(api_.uc_open(UC_ARCH_X86, mode, &uc_), "uc_open");
    }
    ~UnicornUc() override {
        const std::lock_guard<std::mutex> hold(library_lock());
        api_.uc_close(uc_);
    }
    UnicornUc(const UnicornUc&) = delete;
    UnicornUc& operator=(const UnicornUc&) = delete;

    void* handle() const override { return uc_; }

    void mem_map(std::uint64_t address, std::uint64_t size) override {
        const std::lock_guard<std::mutex> hold(library_lock());
        check(api_.uc_mem_map(uc_, address, static_cast<std::size_t>(size), UC_PROT_ALL), "uc_mem_map");
    }
    void mem_unmap(std::uint64_t address, std::uint64_t size) override {
        const std::lock_guard<std::mutex> hold(library_lock());
        check(api_.uc_mem_unmap(uc_, address, static_cast<std::size_t>(size)), "uc_mem_unmap");
    }
    Bytes mem_read(std::uint64_t address, std::uint64_t size) override {
        Bytes buffer(static_cast<std::size_t>(size));
        check(api_.uc_mem_read(uc_, address, buffer.data(), buffer.size()), "uc_mem_read");
        return buffer;
    }
    void mem_write(std::uint64_t address, ByteView data) override {
        check(api_.uc_mem_write(uc_, address, data.data(), data.size()), "uc_mem_write");
    }
    std::uint64_t reg_read(int reg) override {
        std::uint64_t value[2] = {0, 0};   // (room for a register wider than a word: only the first is used)
        check(api_.uc_reg_read(uc_, reg, value), "uc_reg_read");
        return value[0];
    }
    // ST0 reads back as one integer taken as a float, so that a run is judged as it always was.
    // (An x87 register is 80 bits: its mantissa and a sign and exponent word. The first word is the mantissa.)
    double reg_read_float(int reg) override { return static_cast<double>(reg_read(reg)); }
    void reg_write(int reg, std::uint64_t value) override { check(api_.uc_reg_write(uc_, reg, &value), "uc_reg_write"); }
    void reg_write_descriptor(int reg, const Descriptor& value) override {
        uc_x86_mmr mmr{};
        mmr.selector = static_cast<std::uint16_t>(value.selector);
        mmr.base = value.base;
        mmr.limit = value.limit;
        mmr.flags = value.flags;
        check(api_.uc_reg_write(uc_, reg, &mmr), "uc_reg_write");
    }
    std::unique_ptr<Context> context_save() override {
        uc_context* context = nullptr;
        check(api_.uc_context_alloc(uc_, &context), "uc_context_alloc");
        auto saved = std::make_unique<SavedContext>(api_, context);   // (freed if the save fails)
        check(api_.uc_context_save(uc_, context), "uc_context_save");
        return saved;
    }
    void context_restore(const Context& context) override {
        check(api_.uc_context_restore(uc_, static_cast<const SavedContext&>(context).get()), "uc_context_restore");
    }
    void ctl_flush_tb() override { check(api_.uc_ctl(uc_, UC_CTL_WRITE(UC_CTL_TB_FLUSH, 0)), "uc_ctl"); }
    void emu_start(std::uint64_t begin, std::uint64_t until, std::uint64_t timeout_us, std::uint64_t count) override {
        pending_ = nullptr;
        const uc_err result = api_.uc_emu_start(uc_, begin, until, timeout_us, static_cast<std::size_t>(count));
        if (pending_) {   // a callback threw: the run was stopped, and the exception is the answer
            const std::exception_ptr thrown = std::exchange(pending_, nullptr);
            std::rethrow_exception(thrown);
        }
        check(result, "uc_emu_start");
    }
    void emu_stop() override { check(api_.uc_emu_stop(uc_), "uc_emu_stop"); }
    void hook_add_code(std::function<void(core16::Uc&, std::uint64_t, std::uint32_t)> callback, std::uint64_t begin,
                       std::uint64_t end) override {
        code_hooks_.push_back(std::make_unique<CodeHook>(CodeHook{this, std::move(callback)}));
        uc_hook hook = 0;
        check(api_.uc_hook_add(uc_, &hook, UC_HOOK_CODE, reinterpret_cast<void*>(&code_hook), code_hooks_.back().get(), begin, end),
              "uc_hook_add");
    }
    void hook_add_insn_in(std::function<std::uint32_t(core16::Uc&, std::uint32_t, std::uint32_t)> callback) override {
        in_callback_ = std::move(callback);
        uc_hook hook = 0;
        check(api_.uc_hook_add(uc_, &hook, UC_HOOK_INSN, reinterpret_cast<void*>(&in_hook), this, std::uint64_t{1}, std::uint64_t{0},
                               static_cast<int>(UC_X86_INS_IN)),
              "uc_hook_add");
    }
    void hook_add_insn_out(std::function<void(core16::Uc&, std::uint32_t, std::uint32_t, std::uint32_t)> callback) override {
        out_callback_ = std::move(callback);
        uc_hook hook = 0;
        check(api_.uc_hook_add(uc_, &hook, UC_HOOK_INSN, reinterpret_cast<void*>(&out_hook), this, std::uint64_t{1}, std::uint64_t{0},
                               static_cast<int>(UC_X86_INS_OUT)),
              "uc_hook_add");
    }

private:
    void check(uc_err code, std::string_view what) const {
        if (code != UC_ERR_OK) throw UcFault(static_cast<int>(code), std::format("{}: {}", what, api_.uc_strerror(code)));
    }
    // A callback that throws: the run stops at the next instruction and emu_start() throws what it threw.
    void fail(uc_engine* uc) {
        pending_ = std::current_exception();
        api_.uc_emu_stop(uc);
    }

    static std::uint32_t in_hook(uc_engine* uc, std::uint32_t port, int size, void* user) {
        UnicornUc* self = static_cast<UnicornUc*>(user);
        try {
            return self->in_callback_(*self, port, static_cast<std::uint32_t>(size));
        } catch (...) {
            self->fail(uc);
            return 0;
        }
    }
    static void out_hook(uc_engine* uc, std::uint32_t port, int size, std::uint32_t value, void* user) {
        UnicornUc* self = static_cast<UnicornUc*>(user);
        try {
            self->out_callback_(*self, port, static_cast<std::uint32_t>(size), value);
        } catch (...) {
            self->fail(uc);
        }
    }
    static void code_hook(uc_engine* uc, std::uint64_t address, std::uint32_t size, void* user) {
        CodeHook* hook = static_cast<CodeHook*>(user);
        try {
            hook->callback(*hook->owner, address, size);
        } catch (...) {
            hook->owner->fail(uc);
        }
    }

    const Api& api_;
    uc_engine* uc_ = nullptr;
    std::exception_ptr pending_;
    std::vector<std::unique_ptr<CodeHook>> code_hooks_;
    std::function<std::uint32_t(core16::Uc&, std::uint32_t, std::uint32_t)> in_callback_;
    std::function<void(core16::Uc&, std::uint32_t, std::uint32_t, std::uint32_t)> out_callback_;
};

}  // namespace

std::string describe_error(int code) { return emu::unicorn::api().uc_strerror(static_cast<uc_err>(code)); }

std::span<const std::pair<std::string_view, int>> x86_registers() { return kX86Registers; }

std::optional<int> x86_register(std::string_view name) {
    static const std::unordered_map<std::string_view, int> table(std::begin(kX86Registers), std::end(kX86Registers));   // (lookup only)
    const auto it = table.find(name);
    if (it == table.end()) return std::nullopt;
    return it->second;
}

namespace {

// A copy of unicorn.dll loaded by path, with its table.
struct LoadedCopy {
    DynLib lib;
    Api api;
    explicit LoadedCopy(const std::filesystem::path& path) : lib(DynLib::load(path)) {
#define CB_X(name) api.name = lib.function<decltype(&::name)>(#name);
        CB_UNICORN_FUNCTIONS(CB_X)
#undef CB_X
    }
};

// The copies loaded by path, for the life of the program (an emulator library is never unloaded under a thread that might call it).
const Api& copy_at(const std::filesystem::path& path) {
    static std::mutex lock;
    static std::map<std::filesystem::path, std::unique_ptr<LoadedCopy>> copies;
    const std::lock_guard<std::mutex> hold(lock);
    auto& slot = copies[path.lexically_normal()];
    if (!slot) slot = std::make_unique<LoadedCopy>(path);
    return slot->api;
}

}  // namespace

std::unique_ptr<core16::Uc> open_x86_from(const std::filesystem::path& dll, int bits) { return open_x86(bits, copy_at(dll)); }

void load_library(const std::filesystem::path& dll) { (void)copy_at(dll); }

std::string describe_error_from(const std::filesystem::path& dll, int code) { return copy_at(dll).uc_strerror(static_cast<uc_err>(code)); }

std::unique_ptr<core16::Uc> open_x86(int bits, const emu::unicorn::Api& library) {
    if (bits != 16 && bits != 32 && bits != 64) throw UsageError(std::format("an x86 engine is 16, 32 or 64 bits, not {}", bits));
    return std::make_unique<UnicornUc>(library, bits == 16 ? UC_MODE_16 : bits == 64 ? UC_MODE_64 : UC_MODE_32);
}

}  // namespace cb::emulate::unicorn_uc
