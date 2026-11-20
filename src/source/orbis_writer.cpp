// cb-unity: alone
// source/orbis_writer.cpp -- a PlayStation 4 or 5 program as NASM source that builds back into it (orbis_writer.hpp).

#include "source/orbis_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "source/x64_writer.hpp"

namespace cb::source::orbis_writer {

namespace fo = formats::orbis;
namespace iw = image_writer;
namespace xw = x64_writer;

namespace {

const char* segment_type_name(std::uint32_t t) {
    switch (t) {
        case 0: return "NULL";
        case 1: return "LOAD";
        case 2: return "DYNAMIC";
        case 3: return "INTERP";
        case 4: return "NOTE";
        case 7: return "TLS";
        case 0x6474E550: return "GNU_EH_FRAME";
        case 0x6474E551: return "GNU_STACK";
        case 0x6474E552: return "GNU_RELRO";
        case fo::kPtSceDynlibData: return "SCE_DYNLIBDATA";
        case fo::kPtSceProcParam: return "SCE_PROC_PARAM";
        case 0x61000002: return "SCE_MODULE_PARAM";
        case fo::kPtSceRelro: return "SCE_RELRO";
        default: return "other";
    }
}

std::string flag_text(std::uint32_t f) { return std::string((f & 4) ? "R" : "-") + ((f & 2) ? "W" : "-") + ((f & 1) ? "X" : "-"); }

// A NID as a label: the base 64 digits `+` and `-` are not label characters.
std::string nid_label(const std::string& nid) {
    std::string out;
    for (const char c : nid) out += c == '+' ? "_p" : c == '-' ? "_m" : std::string(1, c);
    return out;
}

bool fits32(std::uint64_t v) { return v <= 0xFFFFFFFFull; }

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

}  // namespace

iw::Written write(const fo::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    xw::Image image;
    image.name = std::filesystem::path(file_name).stem().string();
    image.file = program.file;
    const ByteView file(image.file);

    // the loaded segments, in file order, each clipped where an earlier one already covers the file
    std::vector<const fo::Segment*> loaded;
    for (const fo::Segment& s : program.segments) {
        if (s.loaded() && s.file_size != 0 && s.offset < file.size()) loaded.push_back(&s);
    }
    std::ranges::sort(loaded, [](const fo::Segment* a, const fo::Segment* b) { return a->offset < b->offset; });
    iw::Written refusal;
    std::uint64_t cursor = 0;
    for (const fo::Segment* s : loaded) {
        std::uint64_t begin = std::max<std::uint64_t>(s->offset, cursor);
        const std::uint64_t end = std::min<std::uint64_t>(s->offset + s->file_size, file.size());
        if (begin >= end) continue;
        if (!fits32(s->address + s->memory_size)) {
            iw::Written r;
            r.notes.push_back("a loaded segment lies above 4 GB; the source writer addresses a program in 32 bits");
            return r;
        }
        if (begin > cursor) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(begin - cursor), 0, false, false});
        image.runs.push_back({static_cast<std::uint32_t>(begin), static_cast<std::uint32_t>(end - begin), static_cast<std::uint32_t>(s->address + (begin - s->offset)), true, s->code()});
        cursor = end;
    }
    if (cursor < file.size()) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(file.size() - cursor), 0, false, false});
    if (image.runs.empty()) {
        iw::Written r;
        r.notes.push_back("the program has no loaded segment");
        return r;
    }


    // names: the symbols, the imports' slots and the PLT stubs that jump through them
    for (const fo::Symbol& f : program.functions) {
        if (fits32(f.value)) image.functions.emplace(static_cast<std::uint32_t>(f.value), f.name);
    }
    for (const fo::Import& im : program.imports) {
        if (!fits32(im.slot)) continue;
        const std::string name = im.name.empty() ? "nid_" + nid_label(im.nid) : im.name;
        image.data.emplace(static_cast<std::uint32_t>(im.slot), "got_" + name);
    }
    for (const fo::Segment& s : program.segments) {
        if (!s.code() || s.offset >= file.size()) continue;
        const std::uint64_t end = std::min<std::uint64_t>(s.offset + s.file_size, file.size());
        for (std::uint64_t o = s.offset; o + 6 <= end; ++o) {
            if (file[o] != 0xFF || file[o + 1] != 0x25) continue;   // jmp qword [rip + disp32]
            const std::uint64_t at = s.address + (o - s.offset);
            const std::int64_t target = static_cast<std::int64_t>(at) + 6 + static_cast<std::int32_t>(le32(file, o + 2));
            if (target < 0) continue;
            const std::string slot_name = program.import_at(static_cast<std::uint64_t>(target));
            if (slot_name.empty()) continue;
            std::uint64_t start = at;
            if (o >= s.offset + 4 && file[o - 4] == 0xF3 && file[o - 3] == 0x0F && file[o - 2] == 0x1E && file[o - 1] == 0xFA) start -= 4;   // endbr64
            else if (o > s.offset && file[o - 1] == 0xF2) start -= 1;                                                                    // bnd
            if (!fits32(start)) continue;
            const fo::Import* im = nullptr;
            for (const fo::Import& candidate : program.imports) {
                if (candidate.slot == static_cast<std::uint64_t>(target)) im = &candidate;
            }
            const std::string name = im && im->name.empty() ? "nid_" + nid_label(im->nid) : slot_name;
            image.functions.emplace(static_cast<std::uint32_t>(start), name);
        }
    }

    // entries: the entry point, the initialisers, the symbols, and every code address a relocation stores in data
    std::set<std::uint32_t> entries;
    const auto add_entry = [&](std::uint64_t address) {
        if (fits32(address) && program.in_code(address)) entries.insert(static_cast<std::uint32_t>(address));
    };
    add_entry(program.entry);
    for (const std::uint64_t a : program.initializers) add_entry(a);
    for (const fo::Symbol& f : program.functions) add_entry(f.value);
    for (const fo::Relocation& r : program.relocations) {
        if (r.type == 8 && r.addend > 0) add_entry(static_cast<std::uint64_t>(r.addend));
    }
    for (const auto& [address, name] : image.functions) add_entry(address);
    image.entries.push_back(static_cast<std::uint32_t>(program.entry));
    for (const std::uint32_t a : entries) {
        if (a != program.entry) image.entries.push_back(a);
    }
    if (!fits32(program.entry) || !program.in_code(program.entry)) image.entries.erase(image.entries.begin());
    image.first_entry_name = "_start";

    // comments over the headers and the dynamic section
    image.comments[0] = std::format("ELF header: x86-64, type 0x{:X}, entry 0x{:X}", program.type, program.entry);
    std::uint64_t phoff = 0x40;
    if (file.size() >= 40) phoff = static_cast<std::uint64_t>(le32(file, 32)) | static_cast<std::uint64_t>(le32(file, 36)) << 32;
    if (phoff < file.size()) image.comments[static_cast<std::uint32_t>(phoff)] = "program headers";
    for (std::size_t k = 0; k < program.segments.size(); ++k) {
        const fo::Segment& s = program.segments[k];
        const std::uint64_t at = phoff + k * 56;
        if (at < file.size() && at != phoff) image.comments[static_cast<std::uint32_t>(at)] = std::format("program header {}: {} {} address 0x{:X} file 0x{:X}+0x{:X} memory 0x{:X}", k, segment_type_name(s.type), flag_text(s.flags), s.address, s.offset, s.file_size, s.memory_size);
        if (s.type == fo::kPtDynamic && s.offset < file.size()) image.comments[static_cast<std::uint32_t>(s.offset)] = "the dynamic section";
    }

    std::string needed;
    for (const std::string& n : program.needed) needed += (needed.empty() ? "" : ", ") + n;
    image.head = std::format("; {}.asm -- written by CodeBreaker from {}\n; {} for the PlayStation {}, {} bytes, entry 0x{:X}\n; {} imports{}{}", image.name, std::filesystem::path(file_name).filename().string(), program.container,
                             program.orbis || program.self ? "4" : "4 or 5", file.size(), program.entry, program.imports.size(), needed.empty() ? "" : ", needs ", needed);
    return xw::write(image, folder);
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return write(fo::read(file), file.filename().string(), folder);
}

}  // namespace cb::source::orbis_writer
