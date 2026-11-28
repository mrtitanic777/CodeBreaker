// source/console_executable.cpp -- the console writer's `executable` shape (console_writer.hpp): a program in a loader's format (an ELF, a PSP
// module, an EBOOT.PBP holding one) written whole by the engine for its container, the console's facts given as data: its processor, what the
// program is (the head), how an address is seen (the views of memory) and which addresses are RAM, the hardware's names (the format's table)
// and the names the container gives. The PlayStation 2 and the PSP (container `mips-elf`: source/mips_elf_writer).

#include "source/console_shapes.hpp"

#include <format>

#include "core/error.hpp"
#include "formats/mips_elf.hpp"
#include "source/mips_elf_writer.hpp"

namespace cb::source::console_writer {

namespace {

decompile::mips::Variant mips_variant(const Description& d) {
    if (d.executable.variant == "r5900") return decompile::mips::Variant::R5900;
    if (d.executable.variant == "allegrex") return decompile::mips::Variant::Allegrex;
    throw UsageError(std::format("console platform {}: the executable shape has no MIPS variant {}", d.name, d.executable.variant));
}

}  // namespace

image_writer::Written write_executable(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder) {
    const Executable& shape = d.executable;
    if (shape.container != "mips-elf" || !program.contained.mips_elf) {
        throw UsageError(std::format("console platform {}: the executable shape has no container {}", d.name, shape.container));
    }
    mips_elf_writer::Machine m;
    m.variant = mips_variant(d);
    m.cpu = shape.cpu;
    m.description = fill(shape.description, values);
    m.hardware = program.contained.hardware;
    m.in_ram = [&shape](std::uint32_t a) {
        std::uint32_t p = a;
        for (const AddressView& v : shape.views) {
            if (a >= v.from && a < v.to) {
                p = v.mask ? a & v.mask : a - v.minus;
                break;
            }
        }
        for (const Range& r : shape.ram) {
            if (p >= r.from && p < r.to) return true;
        }
        for (const Range& r : shape.ram_virtual) {
            if (a >= r.from && a < r.to) return true;
        }
        return false;
    };
    m.names = program.contained.names;
    m.extension = fill(shape.extension, values);
    return mips_elf_writer::write(m, program.image, *program.contained.mips_elf, program.contained.elf_offset, std::get<std::string>(values.at("file")), folder);
}

}  // namespace cb::source::console_writer
