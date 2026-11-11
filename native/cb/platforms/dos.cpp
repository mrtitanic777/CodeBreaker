#include "cb/platforms/dos.hpp"

namespace cb::platforms {

Platform dos() {
    Platform p;
    p.id = Id::Dos;
    p.name = "dos";
    p.title = "DOS programs";
    p.family = "x86";
    p.source = SourceLanguage::Asm;
    p.lane = "dos";
    p.steps = Steps::Lanes;
    p.loader.container = "MZ, COM, SYS";
    p.cpu = {"x86:LE:16:Real Mode:default", 16, Endian::Little, CapstoneDecoder{Arch::X86, 16},
             {"AX", "BX", "CX", "DX", "SI", "DI", "BP", "SP", "CS", "DS", "ES", "SS"}, "SP"};
    p.convention.names = {"__cdecl16near", "__cdecl16far"};
    p.convention.word = 2;
    p.convention.order = CallOrder::C;
    p.convention.results = {"AX", "DX:AX"};
    p.convention.pops = Pops::Caller;
    // no emulator: nothing is run
    p.compiler = {Language::Asm, "openwatcom wasm"};
    return p;
}

}  // namespace cb::platforms
