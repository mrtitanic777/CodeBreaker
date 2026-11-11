// cb/emulate/capstone_x86.cpp -- see capstone_x86.hpp.
#include "cb/emulate/capstone_x86.hpp"

#include <array>
#include <format>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"

namespace cb::emulate::capstone_x86 {

namespace {

// One engine per (width, detail) for the thread; the destructor closes them when the thread ends.
class ThreadEngines {
public:
    ThreadEngines() = default;
    ThreadEngines(const ThreadEngines&) = delete;
    ThreadEngines& operator=(const ThreadEngines&) = delete;
    ~ThreadEngines() {
        for (csh h : handles_) {
            if (h != 0) disasm::capstone::api().cs_close(&h);
        }
    }

    csh get(int bits, bool detail) {
        const std::size_t slot = slot_of(bits, detail);
        if (handles_[slot] == 0) {
            const auto& cs = disasm::capstone::api();
            const cs_mode mode = bits == 16 ? CS_MODE_16 : bits == 64 ? CS_MODE_64 : CS_MODE_32;
            csh h = 0;
            disasm::capstone::check(cs.cs_open(CS_ARCH_X86, mode, &h), "cs_open");
            cs.cs_option(h, CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
            handles_[slot] = h;
        }
        return handles_[slot];
    }

private:
    static std::size_t slot_of(int bits, bool detail) {
        if (bits != 16 && bits != 32 && bits != 64) {
            throw UsageError(std::format("capstone_x86: no {}-bit x86 mode (16, 32 or 64)", bits));
        }
        return static_cast<std::size_t>(bits / 32) * 2 + (detail ? 1 : 0);   // 16 -> 0, 32 -> 1, 64 -> 2
    }
    std::array<csh, 6> handles_{};
};

}  // namespace

std::size_t engine(int bits, bool detail) {
    thread_local ThreadEngines engines;
    return engines.get(bits, detail);
}

const std::filesystem::path& dll_path() { return disasm::capstone::library().path(); }

}  // namespace cb::emulate::capstone_x86
