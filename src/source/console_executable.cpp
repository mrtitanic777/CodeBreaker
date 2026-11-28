// source/console_executable.cpp -- the console writer's `executable` shape (console_writer.hpp): a program in a loader's format (an ELF, a PSP
// module, an EBOOT.PBP holding one) written whole by the engine for its container, the console's facts given as data: its processor, what the
// program is (the head), how an address is seen (the views of memory) and which addresses are RAM, the hardware's names (the format's table)
// and the names the container gives. The PlayStation 2 and the PSP (container `mips-elf`: source/mips_elf_writer); the Saturn and the
// Dreamcast (container `sh2`: source/sh2_writer, a disc's IP.BIN and first program in one source, the header field by field).

#include "source/console_shapes.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <mutex>

#include "core/error.hpp"
#include "core/text.hpp"
#include "formats/mips_elf.hpp"
#include "knowledge/package.hpp"
#include "source/mips_elf_writer.hpp"
#include "source/sh2_writer.hpp"

namespace cb::source::console_writer {

namespace {

decompile::mips::Variant mips_variant(const Description& d) {
    if (d.executable.variant == "r5900") return decompile::mips::Variant::R5900;
    if (d.executable.variant == "allegrex") return decompile::mips::Variant::Allegrex;
    throw UsageError(std::format("console platform {}: the executable shape has no MIPS variant {}", d.name, d.executable.variant));
}

// The machine's registers by physical address (the knowledge base's section, with the SH7604's own when the description says so), read once.
const std::map<std::uint32_t, std::string>& sh2_registers(const Description& d) {
    static std::mutex lock;
    static std::map<std::string, std::map<std::uint32_t, std::string>, std::less<>> tables;
    std::lock_guard hold(lock);
    if (const auto it = tables.find(d.name); it != tables.end()) return it->second;
    std::map<std::uint32_t, std::string> t = knowledge::standard().hardware_names(d.platform, d.executable.registers);
    if (d.executable.chip_registers) {
        for (const auto& [a, n] : sh2_writer::chip_registers()) t[a] = n;
    }
    return tables.emplace(d.name, std::move(t)).first->second;
}

// A header field's line: a longword (big-endian) as a number, a text as `.ascii`, zeros as `.space`, other bytes as `.byte`.
std::string field_line(const VectorField& f, ByteView v) {
    if (f.as == "long") {
        const std::uint32_t n = static_cast<std::uint32_t>(v[0]) << 24 | static_cast<std::uint32_t>(v[1]) << 16 | static_cast<std::uint32_t>(v[2]) << 8 | v[3];
        return std::format("        .long 0x{:08X}    ; {}", n, f.what);
    }
    if (std::ranges::all_of(v, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"' && c != '\\'; })) return std::format("        .ascii \"{}\"    ; {}", as_text(v), f.what);
    if (std::ranges::all_of(v, [](std::uint8_t c) { return c == 0; })) return std::format("        .space {}    ; {}", f.size, f.what);
    std::vector<std::string> items;
    for (const std::uint8_t c : v) items.push_back(std::format("0x{:02X}", c));
    return std::format("        .byte {}    ; {}", text::join(items, ", "), f.what);
}

image_writer::Written write_sh2(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder) {
    const Executable& shape = d.executable;
    sh2_writer::Image image = *program.contained.sh2;
    image.bytes = program.image;
    image.sh4 = shape.sh4;
    image.hardware = &sh2_registers(d);
    image.is_variable = [&shape, own = program.contained.own](std::uint32_t a) {
        const auto in = [a](const Range& r) { return a >= r.from && a < r.to; };
        return std::ranges::any_of(shape.ram, in) && !std::ranges::any_of(own, in);
    };
    image.module = std::get<std::string>(values.at("file"));
    image.head = head_comment(d, values);
    const Bytes& bytes = image.bytes;
    image.special = [&shape, &bytes](std::uint32_t off, std::vector<std::string>& lines) -> std::uint32_t {
        for (const VectorField& f : shape.fields) {
            if (f.at != off) continue;
            lines.push_back(field_line(f, ByteView(bytes).subspan(f.at, f.size)));
            return f.size;
        }
        return 0;
    };
    return sh2_writer::write(image, std::get<std::string>(values.at("stem")), folder, fill(shape.built, values));
}

}  // namespace

image_writer::Written write_executable(const Description& d, const Loaded& program, const Values& values, const std::filesystem::path& folder) {
    const Executable& shape = d.executable;
    if (shape.container == "sh2" && program.contained.sh2) return write_sh2(d, program, values, folder);
    if (shape.container == "ps3" && program.contained.ps3) return ps3_code::write(d, program, values, folder);
    if (shape.container == "gekko" && program.contained.gekko) return gekko_code::write(d, program, values, folder);
    if (shape.container == "vita" && program.contained.vita) return vita_code::write(d, program, values, folder);
    if (shape.container == "n3ds" && program.contained.n3ds) return n3ds_code::write(d, program, values, folder);
    if (shape.container == "switch" && program.contained.nswitch) return switch_code::write(d, program, values, folder);
    if (shape.container == "nds" && program.contained.nds) return nds_code::write(d, program, values, folder);
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
