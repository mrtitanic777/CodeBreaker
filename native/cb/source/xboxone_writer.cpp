// cb-unity: alone
// cb/source/xboxone_writer.cpp -- an Xbox One / Series PE32+ as NASM source that builds back into it (xboxone_writer.hpp).

#include "cb/source/xboxone_writer.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/source/x64_writer.hpp"

namespace cb::source::xboxone_writer {

namespace iw = image_writer;
namespace xw = x64_writer;
namespace pe = formats::pe;

namespace {

std::uint32_t le32(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | static_cast<std::uint32_t>(b[at + 1]) << 8 | static_cast<std::uint32_t>(b[at + 2]) << 16 | static_cast<std::uint32_t>(b[at + 3]) << 24;
}

// The label a name is made into: the characters NASM takes (a mangled name has `?` and `@`, which it does).
std::string label_of(const std::string& name) {
    std::string out;
    for (const char c : name.starts_with('#') ? "ordinal_" + name.substr(1) : name)   // (an import by ordinal is named "#151")
        out += (std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.' || c == '$' || c == '@' || c == '?' || c == '~' || c == '#') ? c : '_';
    return out;
}

}  // namespace

iw::Written write(const formats::xboxone::Program& program, std::string_view file_name, const std::filesystem::path& folder) {
    const pe::Image img = formats::xboxone::image_of(program);
    xw::Image image;
    image.name = std::filesystem::path(program.member.empty() ? std::string(file_name) : program.member).stem().string();
    image.file = program.file;
    const ByteView file(image.file);
    const pe::OptionalHeader opt = img.optional_header();
    const std::uint64_t base = opt.image_base;

    // the headers and the sections' raw bytes, in file order, each clipped where an earlier one covers the file
    struct Piece {
        std::uint64_t offset, size, rva;
        bool code;
    };
    std::vector<Piece> pieces;
    pieces.push_back({0, std::min<std::uint64_t>(opt.headers_size, file.size()), 0, false});
    for (const pe::Section& s : img.sections()) {
        if (s.raw_size == 0 || s.raw_ptr >= file.size()) continue;
        pieces.push_back({s.raw_ptr, std::min<std::uint64_t>(s.raw_size, file.size() - s.raw_ptr), s.virtual_address, s.is_code()});
    }
    std::ranges::sort(pieces, [](const Piece& a, const Piece& b) { return a.offset < b.offset; });
    std::uint64_t cursor = 0;
    for (const Piece& p : pieces) {
        const std::uint64_t begin = std::max(p.offset, cursor), end = p.offset + p.size;
        if (begin >= end) continue;
        if (p.rva + (end - p.offset) > 0xFFFFFFFFull) {
            iw::Written r;
            r.notes.push_back("a section lies above 4 GB of the image; the source writer addresses a program in 32 bits");
            return r;
        }
        if (begin > cursor) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(begin - cursor), 0, false, false});
        image.runs.push_back({static_cast<std::uint32_t>(begin), static_cast<std::uint32_t>(end - begin), static_cast<std::uint32_t>(p.rva + (begin - p.offset)), true, p.code});
        cursor = end;
    }
    if (cursor < file.size()) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(file.size() - cursor), 0, false, false});
    if (image.runs.empty()) {
        iw::Written r;
        r.notes.push_back("the image has no section with bytes in the file");
        return r;
    }
    const auto in_code = [&](std::uint64_t rva) {
        return std::ranges::any_of(image.runs, [&](const xw::Run& r) { return r.code && rva >= r.address && rva - r.address < r.size; });
    };

    // names: the exports, then the import slots and the thunks that jump through them
    try {
        for (const pe::Export& e : img.exports()) {
            if (e.name && in_code(e.rva)) image.functions.emplace(e.rva, label_of(*e.name));
        }
    } catch (const Error&) {   // (a damaged export table: the functions are found without names)
    }
    std::map<std::uint64_t, std::string> slots;   // rva -> import name
    try {
        for (const pe::Import& im : img.imports()) {
            if (im.slot_va < base || im.slot_va - base > 0xFFFFFFFFull) continue;
            const std::string name = label_of(im.name);
            slots[im.slot_va - base] = name;
            image.data.emplace(static_cast<std::uint32_t>(im.slot_va - base), "__imp_" + name);
        }
    } catch (const Error&) {
    }
    for (const xw::Run& r : image.runs) {
        if (!r.code) continue;
        for (std::uint64_t o = r.offset; o + 6 <= r.offset + r.size; ++o) {
            if (file[o] != 0xFF || file[o + 1] != 0x25) continue;   // jmp qword [rip + disp32]
            const std::uint64_t at = r.address + (o - r.offset);
            const std::int64_t target = static_cast<std::int64_t>(at) + 6 + static_cast<std::int32_t>(le32(file, o + 2));
            if (target < 0) continue;
            const auto it = slots.find(static_cast<std::uint64_t>(target));
            if (it == slots.end()) continue;
            std::uint64_t start = at;
            if (o >= r.offset + 4 && file[o - 4] == 0xF3 && file[o - 3] == 0x0F && file[o - 2] == 0x1E && file[o - 1] == 0xFA) start -= 4;
            image.functions.emplace(static_cast<std::uint32_t>(start), it->second);
        }
    }

    // entries: the entry point, the exports, and every function the exception directory begins
    std::set<std::uint32_t> entries;
    if (opt.entry_point != 0 && in_code(opt.entry_point)) entries.insert(opt.entry_point);
    for (const auto& [rva, name] : image.functions) {
        if (in_code(rva)) entries.insert(rva);
    }
    const pe::DataDirectory exceptions = img.directory(3);
    if (exceptions.rva != 0) {
        if (const auto table = img.read_rva(exceptions.rva, exceptions.size - exceptions.size % 12)) {
            for (std::size_t k = 0; k + 12 <= table->size(); k += 12) {
                const std::uint32_t begin = le32(*table, k);
                if (begin != 0 && in_code(begin)) entries.insert(begin);
            }
        }
    }
    if (opt.entry_point != 0 && in_code(opt.entry_point)) image.entries.push_back(opt.entry_point);
    for (const std::uint32_t a : entries) {
        if (a != opt.entry_point) image.entries.push_back(a);
    }
    image.first_entry_name = "entry";

    image.comments[0] = std::format("PE32+ image, x64, image base 0x{:X}, entry RVA 0x{:X}, {} bytes of headers", base, opt.entry_point, opt.headers_size);
    if (const auto lfanew = le32(file, 0x3C); lfanew < file.size()) image.comments[lfanew] = "PE signature, file header, optional header, data directories and the section table";
    std::string imports;
    try {
        for (const pe::ImportModule& m : img.import_table(false)) imports += (imports.empty() ? "" : ", ") + m.module;
    } catch (const Error&) {
    }
    image.head = std::format("; {}.asm -- written by CodeBreaker from {}\n; {} for the Xbox One / Series{}{}, {} bytes, entry RVA 0x{:X}\n; imports {}", image.name,
                             program.member.empty() ? std::filesystem::path(file_name).filename().string() : std::string(file_name) + ":" + program.member, program.container,
                             program.identity.empty() ? "" : ", " + program.identity, program.version.empty() ? "" : " " + program.version, file.size(), opt.entry_point,
                             imports.empty() ? "none" : imports);
    return xw::write(image, folder);
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const std::string& member) {
    return write(formats::xboxone::read(file, member), file.filename().string(), folder);
}

}  // namespace cb::source::xboxone_writer
