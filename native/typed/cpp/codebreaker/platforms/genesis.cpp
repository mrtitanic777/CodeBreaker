// codebreaker/platforms/genesis.cpp -- C++17 port of codebreaker.platforms.genesis (see genesis.hpp).
#include "codebreaker/platforms/genesis.hpp"

#include "codebreaker/formats/genesis.hpp"

namespace codebreaker::platforms::genesis {

bool _recognise(const py::str& path) {
    return formats::genesis::is_genesis(path);
}

bool recognises(const Platform& platform, const py::str& path) {
    (void)platform;  // this platform's _recognise is the one above
    try {
        return _recognise(path);
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    }
}

static Platform make_platform() {
    Platform p;
    p.name = "genesis";
    p.title = "Genesis / Mega Drive";
    p.family = "68000";
    p.source = "asm";
    p.lane = py::str("codebreaker.verify.lanes.") + p.name;
    p.cpu.language = "68000:BE:32:default";
    p.cpu.bits = 32;
    p.cpu.endian = "big";
    p.cpu.decoder = "codebreaker.decompile.m68k";
    // tuple("D%d" % i for i in range(8)) + tuple("A%d" % i for i in range(8)) + ("SR",)
    for (int64_t i = 0; i < 8; ++i) p.cpu.registers.append(py::percent("D%d", i));
    for (int64_t i = 0; i < 8; ++i) p.cpu.registers.append(py::percent("A%d", i));
    p.cpu.registers.append(py::str("SR"));
    p.cpu.stack = "A7";
    p.container = "cartridge image (.md, .bin, .gen, .smd)";
    p.reader = Ref{"codebreaker.formats.genesis", "read"};
    p.engine = "CodeBreaker's own interpreter";
    p.runner = Ref{"codebreaker.emulate.m68k", "recorded"};
    p.language = "asm";
    p.toolchain = "CodeBreaker's own assembler";
    p.assembler = Ref{"codebreaker.build.m68k_asm", "build"};
    p.writer = Ref{"codebreaker.source.genesis_writer", "reconstruct"};
    p.convention_names.append(py::str("registers"));
    p.convention_arguments = "registers";
    p.convention_order = "registers";
    // results: D0-D7 and A0-A6
    for (int64_t i = 0; i < 8; ++i) p.results.append(py::percent("D%d", i));
    for (int64_t i = 0; i < 7; ++i) p.results.append(py::percent("A%d", i));
    p.pops = "caller";
    return p;
}

const Platform& PLATFORM() {
    static const Platform p = make_platform();
    return p;
}

}  // namespace codebreaker::platforms::genesis
