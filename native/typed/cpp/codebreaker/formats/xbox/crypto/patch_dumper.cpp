// codebreaker/formats/xbox/crypto/patch_dumper.cpp -- C++ port of codebreaker.formats.xbox.crypto.patch_dumper.

#include "codebreaker/formats/xbox/crypto/patch_dumper.hpp"

#include <capstone/capstone.h>

#include "codebreaker/platforms/package.hpp"

#include <cstdio>
#include <iostream>
#include <optional>

namespace codebreaker::formats::xbox::crypto::patch_dumper {

const char* const PATCHES_PATH = R"(X:\Xbox 360\xeBuild_1.21\Builds\17559 - FB\bin\patches_g2mzephyr.bin)";
const char* const OUTPUT_PATH = "Output/decompiled_patches.asm";

namespace {

// f"0x{value:08X}"
py::str hex08(int64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "0x%08llX", static_cast<unsigned long long>(value));
    return py::str(buf);
}

// (addr,) = unpack(">I", fr.read(4))
int64_t read_u32(const py::file& fr) {
    return std::get<0>(py::unpack<int64_t>(">I", fr.read(4)));
}

}  // namespace

// md.disasm(code, address) of platforms.decoder(32, arch="ppc"), as Instructions
static py::list<Instruction> decode_ppc(const py::bytes& code, int64_t address) {
    py::list<Instruction> out;
    platforms::decoder(32, py::str("ppc"), false)->each(code.raw(), address, 0, [&](const cs_insn& c) {
        out.append(Instruction{py::str(std::string(c.mnemonic)), py::str(std::string(c.op_str))});
        return true;
    });
    return out;
}

void main(const py::str& patches_path, const py::str& output_path) { main(Decoder(decode_ppc), patches_path, output_path); }

// py -m pip install capstone / sudo pip3 install capstone (the decoder: platforms::decoder, or the caller's; see the header)
void main(const Decoder& md, const py::str& patches_path, const py::str& output_path) {
    py::file fw = py::open(output_path, py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
    {
        py::file fr = py::open(patches_path, py::str("rb"));
        fr.seek(0, 2);
        int64_t patch_size = fr.tell();
        fr.seek(0);
        fw.write(py::str("# Fuck if I know if this is accurate :)\n\n"));
        fw.write(py::str(".include \"macros.S\"\n\n"));
        while (fr.tell() != patch_size) {
            while (true) {
                int64_t addr = read_u32(fr);
                if (addr == 0xFFFFFFFF) {
                    fw.write(py::str("# done with patch section\n\n"));
                    break;
                }
                // print(f"0x{addr:08X}:")
                int64_t size = read_u32(fr);
                fw.write(py::str("MAKEPATCH ") + hex08(addr) + py::str("\n"));
                fw.write(py::str("0:\n"));
                for (int64_t i = 0; i < size; i++) {
                    py::bytes code = fr.read(4);
                    py::list<Instruction> disasm = md(code, addr + (i * 4));
                    if (disasm.size() > 0) {
                        // print(f"\t0x{addr + (i * 4):08X} -> {code.hex()}")
                        for (int64_t k = 0; k < disasm.size(); k++) {
                            const Instruction& x = disasm[k];
                            fw.write((py::str("\t") + x.mnemonic + py::str(" ") + x.op_str).strip(py::str(" ")) + py::str("\n"));
                        }
                    } else {
                        // print(f"\t#;// 0x{addr + (i * 4):08X} -> 0x{code.hex()}", file=fw)
                        fw.write(py::str("\t.long 0x") + code.hex() + py::str("\n"));
                    }
                }
                fw.write(py::str("9:\n\n"));
            }
        }
        fr.close();
    }
    fw.close();
    std::cout << "Done!" << "\n";
}

}  // namespace codebreaker::formats::xbox::crypto::patch_dumper
