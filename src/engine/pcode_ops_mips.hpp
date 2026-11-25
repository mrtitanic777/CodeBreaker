// engine/pcode_ops_mips.hpp -- the PlayStation adapter's own semantics for the GTE (pcode_ops_mips.cpp).
#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace cb::engine::pcode_ops_mips {

// The GTE's register file: the 32 data registers and the 32 control registers.
struct Gte {
    std::array<std::uint32_t, 32> data{};
    std::array<std::uint32_t, 32> control{};
};

// A data register read or write is what MFC2/MTC2/LWC2/SWC2 do (the special registers: SXYP, IRGB, ORGB, LZCS ...), a control read or write what CFC2/CTC2 do, and a command is
// run by name (NCLIP AVSZ3 AVSZ4 SQR OP GPF GPL MVMVA); false: the name is a command not run.
std::uint32_t gte_register(const Gte& gte, unsigned n);
void gte_set_register(Gte& gte, unsigned n, std::uint32_t value);
std::uint32_t gte_control_register(const Gte& gte, unsigned n);
bool gte_run_command(Gte& gte, std::uint32_t command, std::string_view name);

}  // namespace cb::engine::pcode_ops_mips
