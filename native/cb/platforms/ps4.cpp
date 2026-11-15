// cb-unity: alone
#include "cb/platforms/ps4.hpp"

namespace cb::platforms {

Platform ps4() {
    Platform p;
    p.id = Id::Machine;
    p.name = "ps4";
    p.title = "PlayStation 4";
    p.family = "x86";
    p.source = SourceLanguage::Asm;
    p.lane = "ps4";
    p.steps = Steps::Lanes;
    p.loader.container = "ELF, or an unencrypted SELF (eboot.bin) holding one; a .pkg is recognised and not handled";
    p.cpu = {"x86:LE:64:default", 64, Endian::Little, CapstoneDecoder{Arch::X86, 64},
             {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"}, "RSP"};
    p.convention.names = {"__sysv_abi"};
    p.convention.arguments = InRegisters{};
    p.convention.word = 8;
    p.convention.order = CallOrder::Registers;
    p.convention.results = {"RAX", "RDX"};
    p.convention.pops = Pops::Caller;
    p.convention.preserved = {"RBX", "RBP", "RSP", "R12", "R13", "R14", "R15"};
    p.emulator = {Engine::Unicorn, true};
    p.compiler = {Language::Asm, "NASM"};
    return p;
}

}  // namespace cb::platforms
