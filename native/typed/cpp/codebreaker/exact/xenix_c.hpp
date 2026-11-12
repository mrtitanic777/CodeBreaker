// codebreaker/exact/xenix_c.hpp -- COMPAT SHIM over cb/exact/xenix_c.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/exact/xenix_c.{hpp,cpp}: the XENIX C compiler (cpp, c0, c1, c2, as) as a toolchain in the revision Microsoft built
// LINK and LIB with -- c1's assembly rules, the patched c1 and c2, c2's block-move hooks. This header keeps the old names over the old
// toolchain port, header-only: `XenixC` is still a toolchain::Toolchain (so that toolchain::load() hands it out as the plugin
// "codebreaker.exact.xenix_c:XenixC"), and its compile is the new module's, the answer kept in the cache the old callers shared. The hooks
// are the new module's behind the old emulator's Machine.
//
// DIVERGES: not kept, internals of the module that only its own harness called: PAIRS, OPS, JBR/CBR/JMP/LABEL, `_List`, `_one_block`,
// `_swap`, `_compile` (compile() runs the passes); a variant's `patch` that is not a boolean is false; the key is made from the profile's JSON
// text, not Python's repr of its items; a letter that is no rule is a UsageError in `rules` (the Python raised KeyError).
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/exact/xenix_c.hpp"
#include "codebreaker/exact/emu/xenix.hpp"
#include "codebreaker/exact/plugin_shim.hpp"
#include "codebreaker/exact/toolchain.hpp"
#include "py.hpp"

namespace codebreaker::exact::xenix_c {

namespace nw = ::cb::exact::xenix_c;
namespace xenix = codebreaker::exact::emu::xenix;

inline const py::str CACHE_VERSION = py::str(std::string(nw::kCacheVersion));

// an unsigned word made a long: this c1 loads the low register of the pair and clears the high one; Microsoft's made the long in AX:DX and
// moved it to the pair if it was wanted in one
inline py::str rule_u(const py::str& t) { return ::cb::compat::to_py(nw::rule_u(t.utf8())); }
// a ++ or -- put off until its byte is fetched: Microsoft's c1 does it when the byte is the value wanted
inline py::str rule_d(const py::str& t) { return ::cb::compat::to_py(nw::rule_d(t.utf8())); }
// op (R),M with M in memory (no 8086 instruction): Microsoft's c1 brought M to AX first
inline py::str rule_m(const py::str& t) { return ::cb::compat::to_py(nw::rule_m(t.utf8())); }
// a char made an int OR'ed with a constant: Microsoft's c1 did it on the word
inline py::str rule_b(const py::str& t) { return ::cb::compat::to_py(nw::rule_b(t.utf8())); }

// RULES = {"U": rule_u, "D": rule_d, "M": rule_m, "B": rule_b}
inline const py::dict<py::str, std::function<py::str(const py::str&)>> RULES = {
    {py::str("U"), [](const py::str& t) { return rule_u(t); }},
    {py::str("D"), [](const py::str& t) { return rule_d(t); }},
    {py::str("M"), [](const py::str& t) { return rule_m(t); }},
    {py::str("B"), [](const py::str& t) { return rule_b(t); }}};

namespace detail_shim {

namespace nx = ::cb::exact::emu::xenix;

inline int64_t unicorn_id(nx::Reg r) {
    switch (r) {
        case nx::Reg::Ax: return UC_X86_REG_AX;
        case nx::Reg::Bx: return UC_X86_REG_BX;
        case nx::Reg::Cx: return UC_X86_REG_CX;
        case nx::Reg::Dx: return UC_X86_REG_DX;
        case nx::Reg::Si: return UC_X86_REG_SI;
        case nx::Reg::Di: return UC_X86_REG_DI;
        case nx::Reg::Bp: return UC_X86_REG_BP;
        case nx::Reg::Sp: return UC_X86_REG_SP;
        case nx::Reg::Ip: return UC_X86_REG_IP;
        case nx::Reg::Cs: return UC_X86_REG_CS;
        case nx::Reg::Ds: return UC_X86_REG_DS;
        case nx::Reg::Es: return UC_X86_REG_ES;
        case nx::Reg::Ss: return UC_X86_REG_SS;
        case nx::Reg::Flags: break;
    }
    return UC_X86_REG_EFLAGS;
}

// The new Machine over an old one (a hook of the new module run by the old emulator).
class NewMachine final : public nx::Machine {
  public:
    explicit NewMachine(xenix::Machine& m) : m_(m) {}
    std::uint32_t reg(nx::Reg r) const override { return static_cast<std::uint32_t>(m_.reg_read(unicorn_id(r))); }
    void set_reg(nx::Reg r, std::uint32_t value) override { m_.reg_write(unicorn_id(r), value); }
    ::cb::Bytes read(std::uint32_t address, std::size_t size) const override {
        return ::cb::compat::from_py(m_.mem_read(address, static_cast<int64_t>(size)));
    }
    void write(std::uint32_t address, ::cb::ByteView data) override { m_.mem_write(address, ::cb::compat::to_py(data)); }
    void stop() override { m_.emu_stop(); }

  private:
    xenix::Machine& m_;
};

}  // namespace detail_shim

// {address: hook} of codemove: "R" leaves a loop's block where it is, "S" swaps a block over one other
inline py::dict<int64_t, xenix::Hook> c2_hooks(const py::str& mode = py::str("RS")) {
    py::dict<int64_t, xenix::Hook> out;
    for (const auto& [at, hook] : nw::c2_hooks(mode.utf8())) {
        const auto h = hook;
        out.set(static_cast<int64_t>(at), xenix::Hook([h](xenix::Machine& m, int64_t cs, int64_t ds) {
                    detail_shim::NewMachine adapter(m);
                    h(adapter, static_cast<std::uint16_t>(cs), static_cast<std::uint16_t>(ds));
                }));
    }
    return out;
}

// the c1 without its stack probe (FormatError -- the Python's RuntimeError -- if this is not the c1 the patch was written for)
inline py::bytes _c1(const py::bytes& b) { return ::cb::compat::to_py(::cb::ByteView(nw::patched_c1(::cb::compat::from_py(b)))); }
// (the same, for c2)
inline py::bytes _c2(const py::bytes& c2) { return ::cb::compat::to_py(::cb::ByteView(nw::patched_c2(::cb::compat::from_py(c2)))); }

class XenixC : public toolchain::Toolchain {
  public:
    XenixC(const toolchain::Profile& profile, const std::optional<py::str>& variant_in = std::nullopt)
        : toolchain::Toolchain(profile, std::nullopt),
          impl_(std::make_shared<nw::XenixC>(plugin_shim::new_profile(profile), variant_in ? std::optional<std::string>(variant_in->utf8()) : std::nullopt,
                                             plugin_shim::context())) {
        name = profile.name;
        variant = ::cb::compat::to_py(impl_->variant());
        key = ::cb::compat::to_py(impl_->plugin_key());
        rules = ::cb::compat::to_py(impl_->rules());
        patch = impl_->patch();
        c2move = ::cb::compat::to_py(impl_->c2move());
    }

    py::str key;       // sha1(the profile + variant + CACHE_VERSION)[:12]
    py::str rules;     // the variant's c1 rules ("UDMB")
    bool patch;        // the variant's patched c1 and c2
    py::str c2move;    // the variant's c2 hooks ("RS")

    // Toolchain::compile as the modules use it (headers given, cache on)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                          const py::list<py::str>& include,
                                                          const std::optional<py::str>& switches) const override {
        return compile(src, std::optional<py::dict<py::str, py::bytes>>(headers), include, switches, true);
    }

    // compile(src, headers=None, include=(), switches=None, cache=True) -> (the object file or None, what the passes printed)
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const std::optional<py::dict<py::str, py::bytes>>& headers = std::nullopt,
                                                          const py::list<py::str>& include = py::list<py::str>(),
                                                          const std::optional<py::str>& switches = std::nullopt, bool use_cache = true) const {
        (void)switches;   // (not used, as in the Python)
        const ::cb::Bytes bytes = ::cb::compat::from_py(src);
        const auto options = plugin_shim::options_of(headers ? &*headers : nullptr, include, std::nullopt);
        return plugin_shim::kept(::cb::compat::to_py(impl_->cache_key(bytes, options)), use_cache,
                                 [&] { return plugin_shim::result_of(impl_->build(bytes, options)); });
    }

  private:
    std::shared_ptr<nw::XenixC> impl_;
};

// puts XenixC where toolchain::load() finds it: the plugin "codebreaker.exact.xenix_c:XenixC"
inline void register_plugin() {
    toolchain::register_plugin(py::str(std::string(nw::kPluginName)),
                               [](const toolchain::Profile& p, const std::optional<py::str>& variant) -> std::shared_ptr<toolchain::Toolchain> {
                                   return std::make_shared<XenixC>(p, variant);
                               });
}

}  // namespace codebreaker::exact::xenix_c
