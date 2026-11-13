// codebreaker/emulate/unicorn_uc.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/emulate/unicorn_uc.{hpp,cpp}; this
// header keeps the old py:: surface for the ports and the dynamic exe that still use it: the old `core16::Uc` (int64 addresses, py::bytes)
// as an adapter over the new engine, and the functions that make one. Conversion and forwarding only.
//
// The old port called the unicorn.dll the Python package carries (load_unicorn handed over its path once, else base::capstone_dll::find_unicorn
// found it), and the recorder (native_trace's resolve_dlls) is pointed at that same copy: the adapter's engines are of that copy too
// (unicorn_uc::open_x86_from), not of cb/core's, so engines and recorder agree. A Unicorn error is native_trace::UcError, thrown by
// throw_uc_error (native_trace.hpp, where UcError is). A callback that throws stops the run and comes out of emu_start(), as it did.
//
//     unicorn_uc::load_unicorn("C:/.../unicorn/lib/unicorn.dll");
//     auto uc = unicorn_uc::open_x86(32);          // unicorn.Uc(UC_ARCH_X86, UC_MODE_32)
// COMPAT(py): remove with the last of emulate/emulator, windows16, driver, native_shadow.
#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/unicorn_uc.hpp"
#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/core16_types.hpp"
#include "py.hpp"

namespace codebreaker::emulate::unicorn_uc {

namespace nu = ::cb::emulate::unicorn_uc;

// Throws native_trace::UcError(code) (unicorn.UcError): defined in native_trace.hpp, which this header ends by including.
[[noreturn]] inline void throw_uc_error(int64_t code);

namespace shim_detail {

// The unicorn.dll in use: handed over by load_unicorn, else the one the Python package carries.
inline std::filesystem::path& chosen() {
    static std::filesystem::path path;
    return path;
}
inline std::mutex& chosen_lock() {
    static std::mutex lock;
    return lock;
}
inline const std::filesystem::path& dll() {
    const std::lock_guard<std::mutex> hold(chosen_lock());
    if (chosen().empty()) chosen() = std::filesystem::path(base::capstone_dll::find_unicorn().utf8());   // (nobody called load_unicorn: look as Python's import would)
    return chosen();
}

// A Unicorn error as the old code throws it.
template <class F>
decltype(auto) uc_guard(F&& f) {
    return ::cb::compat::guard([&]() -> decltype(auto) {
        try {
            return std::forward<F>(f)();
        } catch (const nu::UcFault& fault) {   // (before guard turns every cb::Error into the old exception of its kind)
            throw_uc_error(fault.code());
        }
    });
}

// The old Uc (int64 addresses, py::bytes) over a new engine.
class OldUc final : public core16::Uc {
  public:
    explicit OldUc(std::unique_ptr<::cb::core16::Uc> impl) : impl_(std::move(impl)) {}
    // over an engine somebody else owns (a stand-in is handed the run's own): it must outlive this
    explicit OldUc(::cb::core16::Uc& borrowed) : impl_(std::shared_ptr<::cb::core16::Uc>(std::shared_ptr<void>(), &borrowed)) {}

    void* handle() const override { return impl_->handle(); }
    void mem_map(int64_t address, int64_t size) override { call([&] { impl_->mem_map(static_cast<uint64_t>(address), static_cast<uint64_t>(size)); }); }
    void mem_unmap(int64_t address, int64_t size) override { call([&] { impl_->mem_unmap(static_cast<uint64_t>(address), static_cast<uint64_t>(size)); }); }
    py::bytes mem_read(int64_t address, int64_t size) override {
        return call([&] { return ::cb::compat::to_py(::cb::ByteView(impl_->mem_read(static_cast<uint64_t>(address), static_cast<uint64_t>(size)))); });
    }
    void mem_write(int64_t address, const py::bytes& data) override {
        call([&] { impl_->mem_write(static_cast<uint64_t>(address), ::cb::ByteView(::cb::compat::from_py(data))); });
    }
    int64_t reg_read(int64_t reg) override { return call([&] { return static_cast<int64_t>(impl_->reg_read(static_cast<int>(reg))); }); }
    double reg_read_float(int64_t reg) override { return call([&] { return impl_->reg_read_float(static_cast<int>(reg)); }); }
    void reg_write(int64_t reg, int64_t value) override { call([&] { impl_->reg_write(static_cast<int>(reg), static_cast<uint64_t>(value)); }); }
    void reg_write_descriptor(int64_t reg, const std::tuple<int64_t, int64_t, int64_t, int64_t>& value) override {
        call([&] {
            impl_->reg_write_descriptor(static_cast<int>(reg), {static_cast<uint32_t>(std::get<0>(value)), static_cast<uint32_t>(std::get<1>(value)),
                                                               static_cast<uint32_t>(std::get<2>(value)), static_cast<uint32_t>(std::get<3>(value))});
        });
    }
    std::shared_ptr<void> context_save() override {
        return call([&] { return std::shared_ptr<void>(std::shared_ptr<::cb::core16::Uc::Context>(impl_->context_save())); });
    }
    void context_restore(const std::shared_ptr<void>& context) override {
        call([&] { impl_->context_restore(*std::static_pointer_cast<::cb::core16::Uc::Context>(context)); });
    }
    void ctl_flush_tb() override { call([&] { impl_->ctl_flush_tb(); }); }
    void emu_start(int64_t begin, int64_t until, int64_t timeout, int64_t count) override {
        call([&] { impl_->emu_start(static_cast<uint64_t>(begin), static_cast<uint64_t>(until), static_cast<uint64_t>(timeout), static_cast<uint64_t>(count)); });
    }
    void emu_stop() override { call([&] { impl_->emu_stop(); }); }
    void hook_add_code(const std::function<void(core16::Uc&, int64_t, int64_t)>& callback, int64_t begin, int64_t end) override {
        call([&] {
            impl_->hook_add_code(
                [this, callback](::cb::core16::Uc&, uint64_t address, uint32_t size) { callback(*this, static_cast<int64_t>(address), static_cast<int64_t>(size)); },
                static_cast<uint64_t>(begin), static_cast<uint64_t>(end));
        });
    }
    void hook_add_insn_in(const std::function<int64_t(core16::Uc&, int64_t, int64_t)>& callback) override {
        call([&] {
            impl_->hook_add_insn_in([this, callback](::cb::core16::Uc&, uint32_t port, uint32_t size) {
                return static_cast<uint32_t>(callback(*this, static_cast<int64_t>(port), static_cast<int64_t>(size)));
            });
        });
    }
    void hook_add_insn_out(const std::function<void(core16::Uc&, int64_t, int64_t, int64_t)>& callback) override {
        call([&] {
            impl_->hook_add_insn_out([this, callback](::cb::core16::Uc&, uint32_t port, uint32_t size, uint32_t value) {
                callback(*this, static_cast<int64_t>(port), static_cast<int64_t>(size), static_cast<int64_t>(value));
            });
        });
    }
    void speed_up() override { impl_->speed_up(); }

  private:
    // (not guard: a py:: exception thrown by a callback comes out of emu_start() as it went in)
    template <class F>
    decltype(auto) call(F&& f) {
        try {
            return std::forward<F>(f)();
        } catch (const nu::UcFault& fault) {
            throw_uc_error(fault.code());
        }
    }
    std::shared_ptr<::cb::core16::Uc> impl_;
};

}  // namespace shim_detail

// Load unicorn.dll from `dll_path` (once).  RuntimeError when it cannot be loaded.
inline void load_unicorn(const py::str& dll_path) {
    {
        const std::lock_guard<std::mutex> hold(shim_detail::chosen_lock());
        if (!shim_detail::chosen().empty()) return;
        shim_detail::chosen() = std::filesystem::path(dll_path.utf8());
    }
    try {
        nu::load_library(shim_detail::chosen());
    } catch (const ::cb::Error& e) {
        throw py::RuntimeError(std::string("cannot load unicorn.dll: ") + e.what());
    }
}
inline bool loaded() {
    const std::lock_guard<std::mutex> hold(shim_detail::chosen_lock());
    return !shim_detail::chosen().empty();
}

// str(unicorn.UcError(err)): uc_strerror
inline py::str strerror(int64_t err) { return shim_detail::uc_guard([&] { return py::str(nu::describe_error_from(shim_detail::dll(), static_cast<int>(err))); }); }

// unicorn.x86_const.UC_X86_REG_<NAME>: the id of an x86 register by its (upper-case) name, None when there is none.
inline std::optional<int64_t> x86_reg(const py::str& name) {
    const std::optional<int> id = nu::x86_register(name.utf8());
    if (!id) return std::nullopt;
    return *id;
}

// unicorn.x86_const as {"UC_X86_REG_<NAME>": id} (what native_trace hands the recorder)
inline py::dict<py::str, int64_t> x86_register_table() {
    py::dict<py::str, int64_t> out;
    for (const auto& [name, id] : nu::x86_registers()) out.set(py::str("UC_X86_REG_") + py::str(std::string(name)), id);
    return out;
}

// A new x86 engine: bits 16, 32 or 64 (UC_MODE_16 / 32 / 64).
inline std::shared_ptr<core16::Uc> open_x86(int64_t bits) {
    return shim_detail::uc_guard([&] {
        return std::shared_ptr<core16::Uc>(std::make_shared<shim_detail::OldUc>(nu::open_x86_from(shim_detail::dll(), bits == 16 ? 16 : bits == 64 ? 64 : 32)));
    });
}

}  // namespace codebreaker::emulate::unicorn_uc

// UcError is native_trace's: its header defines throw_uc_error above and includes this one for the register table.
#include "codebreaker/emulate/native_trace.hpp"
