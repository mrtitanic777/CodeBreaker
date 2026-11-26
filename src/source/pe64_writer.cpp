// cb-unity: alone
// source/pe64_writer.cpp -- a 64-bit Windows program as NASM source that builds back into it (pe64_writer.hpp).

#include "source/pe64_writer.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <map>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/pe.hpp"
#include "source/x64_writer.hpp"

namespace cb::source::pe64_writer {

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

iw::Written refused(std::string why) {
    iw::Written r;
    r.notes.push_back(std::move(why));
    return r;
}

}  // namespace

iw::Written write(ByteView file_bytes, std::string_view file_name, const std::filesystem::path& folder, const Options& options) {
    const pe::Image img = pe::Image::parse(file_bytes, file_name);
    if (!img.header().is_plus || img.header().machine != pe::kMachineX64) return refused("not an x64 PE32+ image");
    xw::Image image;
    image.name = options.name.empty() ? std::filesystem::path(std::string(file_name)).stem().string() : options.name;
    image.file = Bytes(file_bytes.begin(), file_bytes.end());
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
        if (p.rva + (end - p.offset) > 0xFFFFFFFFull) return refused("a section lies above 4 GB of the image; the source writer addresses a program in 32 bits");
        if (begin > cursor) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(begin - cursor), 0, false, false});
        image.runs.push_back({static_cast<std::uint32_t>(begin), static_cast<std::uint32_t>(end - begin), static_cast<std::uint32_t>(p.rva + (begin - p.offset)), true, p.code});
        cursor = end;
    }
    if (cursor < file.size()) image.runs.push_back({static_cast<std::uint32_t>(cursor), static_cast<std::uint32_t>(file.size() - cursor), 0, false, false});
    if (image.runs.empty()) return refused("the image has no section with bytes in the file");
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
    std::set<std::string> slot_names;
    try {
        for (const pe::Import& im : img.imports()) {
            if (im.slot_va < base || im.slot_va - base > 0xFFFFFFFFull) continue;
            std::string name = label_of(im.name);
            // (two modules may export a name each; the second slot is told apart by its module)
            if (!slot_names.insert(name).second) name = label_of(std::filesystem::path(im.dll).stem().string()) + "_" + name;
            slot_names.insert(name);
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
            if (o >= r.offset + 4 && file[o - 4] == 0xF3 && file[o - 3] == 0x0F && file[o - 2] == 0x1E && file[o - 1] == 0xFA) start -= 4;   // (endbr64 before it)
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
    image.head = std::format("; {}.asm -- written by CodeBreaker from {}\n; {}, {} bytes, entry RVA 0x{:X}\n; imports {}", image.name, file_name,
                             options.kind.empty() ? std::string("64-bit Windows program") : options.kind, file.size(), opt.entry_point, imports.empty() ? "none" : imports);
    return xw::write(image, folder);
}

iw::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const Options& options) {
    const Bytes bytes = fs::read_file(file);
    return write(bytes, fs::to_text(file.filename()), folder, options);
}

}  // namespace cb::source::pe64_writer
