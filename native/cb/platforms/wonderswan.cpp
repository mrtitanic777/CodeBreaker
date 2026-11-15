// cb-unity: alone
#include "cb/platforms/wonderswan.hpp"

namespace cb::platforms {

Platform wonderswan() {
    Platform p;
    p.id = Id::WonderSwan;
    p.name = "wonderswan";
    p.title = "WonderSwan / WonderSwan Color";
    p.family = "x86";
    p.source = SourceLanguage::Asm;
    p.lane = "wonderswan";
    p.steps = Steps::Lanes;
    p.loader.container = "cartridge (.ws, .wsc)";
    p.cpu = {"x86:LE:16:Real Mode:default", 16, Endian::Little, CapstoneDecoder{Arch::X86, 16},
             {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS", "FLAGS"}, "SP"};
    p.convention.names = {"__cdecl16near"};
    p.convention.word = 2;
    p.convention.order = CallOrder::C;
    p.convention.results = {"AX", "DX:AX"};
    p.convention.pops = Pops::Caller;
    p.emulator = {Engine::Unicorn, true};
    p.compiler = {Language::Asm, "NASM"};
    return p;
}

}  // namespace cb::platforms
