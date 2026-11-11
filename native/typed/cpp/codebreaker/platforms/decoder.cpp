// codebreaker/platforms/decoder.cpp -- the decoder of codebreaker/platforms/__init__.py (see package.hpp).
//
// platforms.decoder(bits, arch, detail): capstone's engine for a processor, one of each kind per thread. It is the one
// place a decoder is made; the DLL is loaded by emulate/capstone_x86 (found as `import capstone` finds it, or named
// with load_capstone). It is a file of its own so that a module that only decodes does not link the registry.
#include <capstone/capstone.h>

#include <map>

#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::platforms {

using py::str;

py::list<Insn> Decoder::disasm(const py::bytes& code, int64_t address) const {
    py::list<Insn> out;
    std::string raw = code.raw();
    each(raw, address, 0, [&](const cs_insn& c) {
        Insn x;
        x.address = static_cast<int64_t>(c.address);
        x.size = c.size;
        x.mnemonic = str(std::string(c.mnemonic));
        x.op_str = str(std::string(c.op_str));
        x.bytes = py::bytes(std::string(reinterpret_cast<const char*>(c.bytes), c.size));
        out.append(x);
        return true;
    });
    return out;
}

cs_insn* Decoder::raw(const uint8_t* code, size_t size, int64_t address, size_t count, size_t& n) const {
    cs_insn* insn = nullptr;
    n = emulate::capstone_x86::api().disasm(static_cast<csh>(handle), code, size, static_cast<uint64_t>(address), count, &insn);
    return insn;
}

void Decoder::release(cs_insn* insn, size_t n) const {
    if (insn) emulate::capstone_x86::api().free_(insn, n);
}

void Decoder::each(const std::string& code, int64_t address, size_t count, const std::function<bool(const cs_insn&)>& visit) const {
    size_t n = 0;
    cs_insn* insn = raw(reinterpret_cast<const uint8_t*>(code.data()), code.size(), address, count, n);
    try {
        for (size_t i = 0; i < n; i++)
            if (!visit(insn[i])) break;
    } catch (...) {
        release(insn, n);
        throw;
    }
    release(insn, n);
}

const char* Decoder::reg_name(uint32_t reg) const {
    return emulate::capstone_x86::api().reg_name(static_cast<csh>(handle), reg);
}

std::optional<std::tuple<std::vector<uint16_t>, std::vector<uint16_t>>> Decoder::regs_access(const cs_insn& insn) const {
    cs_regs read, written;
    uint8_t nread = 0, nwritten = 0;
    if (emulate::capstone_x86::api().regs_access(static_cast<csh>(handle), &insn, read, &nread, written, &nwritten) != CS_ERR_OK)
        return std::nullopt;
    return std::make_tuple(std::vector<uint16_t>(read, read + nread), std::vector<uint16_t>(written, written + nwritten));
}

std::shared_ptr<Decoder> decoder(int64_t bits, const str& arch, bool detail) {
    // one per thread and option set; making one is a thousand times dearer than decoding with it
    thread_local std::map<std::tuple<std::string, int64_t, bool>, std::shared_ptr<Decoder>> held;
    auto key = std::make_tuple(arch.utf8(), bits, detail);
    auto it = held.find(key);
    if (it != held.end()) return it->second;
    const emulate::capstone_x86::Api& a = emulate::capstone_x86::api();
    cs_arch ca;
    int mode;
    if (arch == str("x86")) {
        ca = CS_ARCH_X86;
        if (bits == 16) mode = CS_MODE_16;
        else if (bits == 32) mode = CS_MODE_32;
        else if (bits == 64) mode = CS_MODE_64;
        else throw py::KeyError(std::to_string(bits));
    } else if (arch == str("ppc")) {
        ca = CS_ARCH_PPC;
        if (bits == 32) mode = CS_MODE_32;
        else if (bits == 64) mode = CS_MODE_64;
        else throw py::KeyError(std::to_string(bits));
        mode |= CS_MODE_BIG_ENDIAN;
    } else {
        throw py::ValueError("no capstone decoder for " + py::repr(arch).utf8());
    }
    csh h = 0;
    if (a.open(ca, static_cast<cs_mode>(mode), &h) != CS_ERR_OK) throw py::RuntimeError("cs_open failed");
    a.option(h, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
    auto d = std::make_shared<Decoder>();
    d->arch = arch;
    d->bits = bits;
    d->detail = detail;
    d->handle = static_cast<uintptr_t>(h);
    held[key] = d;
    return d;
}

std::shared_ptr<Decoder> decoder(const Cpu& cpu, bool detail) {
    if (const auto* c = std::get_if<Cpu::Capstone>(&cpu.decoder)) return decoder(c->bits, c->arch, detail);
    throw py::ValueError(py::repr(cpu.language).utf8() + " declares no decoder");
}

}  // namespace codebreaker::platforms
