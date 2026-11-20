// emulate/unicorn_uc.hpp -- an x86 engine on Unicorn, as the core16::Uc the emulators, the shadows and the lanes run on.
//
//     auto uc = unicorn_uc::open_x86(32);           // a 32-bit engine (16 and 64 too)
//     uc->mem_map(0x1000, 0x1000);
//     uc->mem_write(0x1000, code);
//     uc->emu_start(0x1000, 0x1000 + code.size());
//     std::uint64_t eax = uc->reg_read(*unicorn_uc::x86_register("EAX"));
//
// The engine calls the prebuilt unicorn.dll through cb/core's table (`cb::emu::unicorn::api()`), or through the table it is handed:
// a caller whose other parts hold engines of another copy of the library (the recorder's, pointed at it by tracer::use_libraries)
// must make its engines from that copy too. Every call that fails throws `UcFault`, which carries Unicorn's own error number and
// text. A callback that throws stops the run (uc_emu_stop) and the exception comes out of emu_start().
//
// An engine belongs to one thread; opening, closing and mapping take one lock between engines, since Unicorn keeps global state
// (the code buffer of the translator) that they touch.
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "core/error.hpp"
#include "core16_types.hpp"

namespace cb::emu::unicorn {
struct Api;
const Api& api();
}  // namespace cb::emu::unicorn

namespace cb::emulate::unicorn_uc {

// A Unicorn call failed: `code()` is the uc_err it gave, the message Unicorn's text for it.
class UcFault : public EmulationError {
public:
    UcFault(int code, const std::string& message) : EmulationError(message), code_(code) {}
    int code() const { return code_; }
    CB_ERROR_BOILERPLATE(UcFault)

private:
    int code_;
};

// Unicorn's text for an error number (uc_strerror).
std::string describe_error(int code);

// The id of an x86 register by its upper-case name without Unicorn's prefix ("EAX", "EFLAGS", "ST0"); nothing when there is none.
std::optional<int> x86_register(std::string_view name);

// Every x86 register Unicorn names, as (name, id), by name.
std::span<const std::pair<std::string_view, int>> x86_registers();

// A new x86 engine: `bits` is 16, 32 or 64. UsageError for another number.
std::unique_ptr<core16::Uc> open_x86(int bits, const emu::unicorn::Api& library = emu::unicorn::api());

// The same on the copy of unicorn.dll at `dll`, for a caller whose recorder was pointed at that copy (tracer::use_libraries): engines
// and the functions that use them must come from one copy. Each path is loaded once and kept for the program. NotFoundError or
// IoError when it will not load, UnsupportedError for one that lacks a function.
std::unique_ptr<core16::Uc> open_x86_from(const std::filesystem::path& dll, int bits);
// Loads the copy at `dll` (see open_x86_from) without making an engine.
void load_library(const std::filesystem::path& dll);
// Unicorn's text for an error number, from the copy at `dll`.
std::string describe_error_from(const std::filesystem::path& dll, int code);

}  // namespace cb::emulate::unicorn_uc
