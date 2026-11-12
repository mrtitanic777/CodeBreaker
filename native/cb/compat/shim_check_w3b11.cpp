// Compile-only check of the shims at the old header paths of the console writers (source/nes_writer, genesis_writer, gb_writer, sms_writer,
// z80_writer): the old py:: surfaces over cb/source/*. The old image_writer::Result they return is the old port's, which cannot be linked into
// cb_tests, so nothing here runs; it is in cb_shim_check so the shims stay honest until their last consumers are refactored. (The shims are
// header-only: including them compiles every definition in them.)
#include "codebreaker/source/gb_writer.hpp"
#include "codebreaker/source/genesis_writer.hpp"
#include "codebreaker/source/nes_writer.hpp"
#include "codebreaker/source/sms_writer.hpp"
#include "codebreaker/source/z80_writer.hpp"

namespace shim_check_w3b11 {

inline void touch() {
    namespace s = codebreaker::source;
    (void)&s::nes_writer::reconstruct;
    (void)&s::nes_writer::main;
    (void)&s::nes_writer::HARDWARE;
    (void)&s::nes_writer::DOC;
    (void)&s::genesis_writer::reconstruct;
    (void)&s::genesis_writer::main;
    (void)&s::genesis_writer::HARDWARE;
    (void)&s::gb_writer::reconstruct;
    (void)&s::gb_writer::main;
    (void)&s::gb_writer::REGISTERS;
    (void)&s::gb_writer::MBC_REGISTERS;
    (void)&s::sms_writer::reconstruct;
    (void)&s::sms_writer::main;
    (void)&s::sms_writer::PORTS;
    (void)&s::sms_writer::GG_PORTS;
    (void)&s::sms_writer::REGISTERS;
    (void)&s::sms_writer::_slot;
    (void)&s::z80_writer::write;
}

}  // namespace shim_check_w3b11
