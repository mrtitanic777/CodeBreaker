// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/formats/ppc_program.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::ppc_program {

namespace {

// A stretch of the file that is something in particular; spans may overlap (a section inside the program header table's reach), and the
// tiling keeps the first by priority.
struct Span {
    std::size_t offset = 0, size = 0;
    std::uint32_t address = 0;
    Role role = Role::Other;
    std::string name;
    int priority = 0;   // lower wins
};

// The spans cut into regions that follow one another and cover [0, file_size): an overlap is given to the span that wins it, a hole is `Other`.
std::vector<Region> tile(std::size_t file_size, std::vector<Span> spans) {
    std::ranges::stable_sort(spans, [](const Span& a, const Span& b) {
        if (a.offset != b.offset) return a.offset < b.offset;
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.size > b.size;
    });
    std::vector<Region> out;
    std::size_t pos = 0;
    const auto gap = [&](std::size_t to) {
        if (to > pos) out.push_back({pos, to - pos, static_cast<std::uint32_t>(pos), Role::Other, "gap"});
        pos = std::max(pos, to);
    };
    for (const Span& s : spans) {
        const std::size_t end = std::min(s.offset + s.size, file_size);
        if (s.size == 0 || end <= pos) continue;
        const std::size_t start = std::max(s.offset, pos);
        gap(start);
        const bool loaded = s.role == Role::Code || s.role == Role::Data;
        out.push_back({start, end - start, loaded ? static_cast<std::uint32_t>(s.address + (start - s.offset)) : static_cast<std::uint32_t>(start), s.role, s.name});
        pos = end;
    }
    gap(file_size);
    return out;
}

}  // namespace

std::string_view console_text(Console console) { return console == Console::GameCube ? "GameCube" : console == Console::Wii ? "Wii" : "Wii U"; }
std::string_view lane_name(Console console) { return console == Console::GameCube ? "gamecube" : console == Console::Wii ? "wii" : "wiiu"; }
std::string_view cpu_text(Console console) { return console == Console::GameCube ? "Gekko" : console == Console::Wii ? "Broadway" : "Espresso"; }
std::string_view kind_text(Kind kind) { return kind == Kind::Dol ? "DOL" : kind == Kind::Elf ? "ELF" : "RPX"; }

std::vector<Program::Piece> Program::loaded() const {
    std::vector<Piece> out;
    for (const Region& r : regions)
        if (r.role == Role::Code || r.role == Role::Data) out.push_back({r.address, ByteView(file).subspan(r.offset, r.size)});
    return out;
}

const Region* Program::code_at(std::uint32_t address) const {
    for (const Region& r : regions)
        if (r.role == Role::Code && address >= r.address && address - r.address < r.size) return &r;
    return nullptr;
}

std::optional<Program> parse(ByteView bytes) {
    try {
        Program p;
        std::vector<Span> spans;
        if (ppc_elf::is_ppc_elf(bytes)) {
            ppc_elf::Elf elf = ppc_elf::parse(bytes);
            p.kind = elf.wiiu ? Kind::Rpx : Kind::Elf;
            p.console = elf.wiiu ? Console::WiiU : elf.console() == dol::Console::Wii ? Console::Wii : Console::GameCube;
            p.file = elf.data;
            p.entry = elf.entry;
            p.segments = elf.segments();
            spans.push_back({0, ppc_elf::kHeaderSize, 0, Role::Header, "elf header", 0});
            if (elf.phnum != 0) spans.push_back({elf.phoff, std::size_t{elf.phnum} * ppc_elf::kProgramHeaderSize, 0, Role::Header, "program headers", 0});
            if (elf.shnum != 0) spans.push_back({elf.shoff, std::size_t{elf.shnum} * ppc_elf::kSectionHeaderSize, 0, Role::Header, "section headers", 0});
            for (const ppc_elf::SectionHeader& s : elf.sections) {
                if (s.type == ppc_elf::kTypeNobits || s.type == 0 || s.size == 0) continue;
                const bool loaded = (s.flags & ppc_elf::kFlagAlloc) != 0 && s.address != 0;
                spans.push_back({s.offset, s.size, s.address, loaded ? ((s.flags & ppc_elf::kFlagExec) != 0 ? Role::Code : Role::Data) : Role::Other, s.name.empty() ? "section" : s.name, 1});
            }
            if (elf.sections.empty()) {   // (no section headers: the loadable segments)
                for (const ppc_segment::Segment& s : p.segments) spans.push_back({s.offset, s.size, s.address, s.code ? Role::Code : Role::Data, s.name, 1});
            }
            p.elf = std::move(elf);
        } else {
            dol::Dol d = dol::parse(bytes);
            p.kind = Kind::Dol;
            p.console = d.console() == dol::Console::Wii ? Console::Wii : Console::GameCube;
            p.file = d.data;
            p.entry = d.entry;
            p.segments = d.segments();
            spans.push_back({0, dol::kHeaderSize, 0, Role::Header, "dol header", 0});
            for (const ppc_segment::Segment& s : p.segments) spans.push_back({s.offset, s.size, s.address, s.code ? Role::Code : Role::Data, s.name, 1});
            p.dol = std::move(d);
        }
        p.regions = tile(p.file.size(), std::move(spans));
        return p;
    } catch (const Error&) {
        return std::nullopt;
    }
}

std::optional<Program> read(const std::filesystem::path& path) {
    try {
        const fs::MappedFile file(path);
        return parse(file.view());
    } catch (const Error&) {
        return std::nullopt;
    }
}

bool probe(const std::filesystem::path& path, Console console) {
    // (a cheap test first: the ELF identification or a DOL header that reads as one, then the whole file for the console)
    const bool elf = ppc_elf::probe(path);
    if (!elf && !dol::probe(path)) return false;
    const std::optional<Program> p = read(path);
    return p && p->console == console;
}

}  // namespace cb::formats::ppc_program
