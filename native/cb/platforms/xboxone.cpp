// cb-unity: alone
#include "cb/platforms/xboxone.hpp"

namespace cb::platforms {

Platform xboxone() {
    Platform p;
    p.id = Id::Machine;
    p.name = "xboxone";
    p.title = "Xbox One / Series";
    p.family = "x86";
    p.source = SourceLanguage::Asm;
    p.lane = "xboxone";
    p.steps = Steps::Lanes;
    p.loader.container = "PE32+ in an .appx / .msix / .msixbundle package (zip), or loose; an encrypted .xvc / .msixvc is recognised and not handled";
    p.cpu = {"x86:LE:64:default", 64, Endian::Little, CapstoneDecoder{Arch::X86, 64},
             {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"}, "RSP"};
    p.convention.names = {"__ms_abi"};
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
