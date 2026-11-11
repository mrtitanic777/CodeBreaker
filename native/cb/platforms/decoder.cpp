#include "cb/platforms/decoder.hpp"

#include <algorithm>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"

namespace cb::platforms {

namespace cs = cb::disasm::capstone;

Decoder Decoder::open(Arch arch, unsigned bits, bool detail) {
    cs_arch cs_architecture{};
    int mode = 0;
    switch (arch) {
        case Arch::X86:
            cs_architecture = CS_ARCH_X86;
            if (bits == 16) mode = CS_MODE_16;
            else if (bits == 32) mode = CS_MODE_32;
            else if (bits == 64) mode = CS_MODE_64;
            else throw UsageError(std::format("no 16/32/64-bit x86 decoder for {} bits", bits));
            break;
        case Arch::Ppc:
            cs_architecture = CS_ARCH_PPC;
            if (bits == 32) mode = CS_MODE_32;
            else if (bits == 64) mode = CS_MODE_64;
            else throw UsageError(std::format("no 32/64-bit PowerPC decoder for {} bits", bits));
            mode |= CS_MODE_BIG_ENDIAN;
            break;
    }
    const cs::Api& api = cs::api();
    csh handle = 0;
    cs::check(api.cs_open(cs_architecture, static_cast<cs_mode>(mode), &handle), "cs_open");
    cs::check(api.cs_option(handle, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF), "cs_option(detail)");
    return Decoder(arch, bits, detail, static_cast<std::uintptr_t>(handle));
}

Decoder::Decoder(Decoder&& other) noexcept
    : arch_(other.arch_), bits_(other.bits_), detail_(other.detail_), handle_(std::exchange(other.handle_, 0)) {}

Decoder& Decoder::operator=(Decoder&& other) noexcept {
    if (this != &other) {
        if (handle_ != 0) {
            csh h = static_cast<csh>(handle_);
            cs::api().cs_close(&h);
        }
        arch_ = other.arch_;
        bits_ = other.bits_;
        detail_ = other.detail_;
        handle_ = std::exchange(other.handle_, 0);
    }
    return *this;
}

Decoder::~Decoder() {
    if (handle_ != 0) {
        csh h = static_cast<csh>(handle_);
        cs::api().cs_close(&h);
    }
}

void Decoder::each(ByteView code, std::uint64_t address, std::size_t count,
                   const std::function<bool(const cs_insn&)>& visit) const {
    const cs::Api& api = cs::api();
    cs_insn* insn = nullptr;
    const std::size_t n = api.cs_disasm(static_cast<csh>(handle_), code.data(), code.size(), address, count, &insn);
    // free the records however the visitor leaves
    struct Release {
        const cs::Api& api;
        cs_insn* insn;
        std::size_t n;
        ~Release() {
            if (insn != nullptr) api.cs_free(insn, n);
        }
    } release{api, insn, n};
    for (std::size_t i = 0; i < n; ++i)
        if (!visit(insn[i])) break;
}

std::vector<Insn> Decoder::disasm(ByteView code, std::uint64_t address, std::size_t count) const {
    std::vector<Insn> out;
    each(code, address, count, [&out](const cs_insn& c) {
        Insn x;
        x.address = c.address;
        x.size = static_cast<std::uint8_t>(c.size);
        x.mnemonic = c.mnemonic;
        x.op_str = c.op_str;
        std::copy_n(c.bytes, std::min<std::size_t>(c.size, x.bytes.size()), x.bytes.begin());
        out.push_back(std::move(x));
        return true;
    });
    return out;
}

const char* Decoder::register_name(std::uint32_t reg) const {
    return cs::api().cs_reg_name(static_cast<csh>(handle_), reg);
}

std::optional<RegisterAccess> Decoder::register_access(const cs_insn& insn) const {
    cs_regs read{}, written{};
    std::uint8_t n_read = 0, n_written = 0;
    if (cs::api().cs_regs_access(static_cast<csh>(handle_), &insn, read, &n_read, written, &n_written) != CS_ERR_OK)
        return std::nullopt;
    return RegisterAccess{std::vector<std::uint16_t>(read, read + n_read), std::vector<std::uint16_t>(written, written + n_written)};
}

const Decoder& DecoderSet::get(Arch arch, unsigned bits, bool detail) {
    const auto key = std::make_tuple(arch, bits, detail);
    auto it = held_.find(key);
    if (it == held_.end()) it = held_.emplace(key, std::make_unique<Decoder>(Decoder::open(arch, bits, detail))).first;
    return *it->second;
}

}  // namespace cb::platforms
