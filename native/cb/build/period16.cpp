#include "cb/build/period16.hpp"

#include <algorithm>
#include <format>

#include "cb/build/fields.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::build::period16 {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kReader = "period16";
constexpr unsigned kDefaultAlignShift = 9;   // an NE alignment shift of 0 means 512-byte sectors

bool has_extension(const std::string& name, std::initializer_list<std::string_view> extensions) {
    return std::any_of(extensions.begin(), extensions.end(), [&](std::string_view e) { return text::iends_with(name, e); });
}

void require_tool(const stdfs::path& p, std::string_view setting) {
    std::error_code ec;
    if (p.empty() || !stdfs::exists(p, ec)) throw NotFoundError(p, 2, std::format("{} is not set up (settings.json: {})", setting, setting));
}

// The sector alignment shift of an NE header at `ne`.
unsigned alignment_shift(ByteView file, std::uint64_t ne) {
    const unsigned shift = u16_at(file, ne + 0x32, kReader);
    return shift == 0 ? kDefaultAlignShift : shift;
}

}  // namespace

std::optional<stdfs::path> find_kit(const stdfs::path& checkout_root) {
    const stdfs::path dir = checkout_root / "toolchains" / "period" / "libs" / "win31-kit";
    std::error_code ec;
    return stdfs::exists(dir / "MDLLCEW.LIB", ec) ? std::optional(dir) : std::nullopt;
}

std::optional<stdfs::path> find_libentry(const Tools& tools) {
    std::error_code ec;
    if (tools.win31_kit && stdfs::exists(*tools.win31_kit / "LIBENTRY.OBJ", ec)) return *tools.win31_kit / "LIBENTRY.OBJ";
    if (tools.libentry_fallback && stdfs::exists(*tools.libentry_fallback / "LIBENTRY.OBJ", ec)) {
        return *tools.libentry_fallback / "LIBENTRY.OBJ";
    }
    return std::nullopt;
}

std::string response_file(const stdfs::path& folder) {
    std::error_code ec;
    if (!stdfs::is_directory(folder, ec)) throw NotFoundError(folder, 3, "not a folder");
    std::vector<std::string> found;
    for (const auto& entry : stdfs::directory_iterator(folder)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (text::iends_with(name, ".lnk")) found.push_back(name);
    }
    if (found.empty()) throw NotFoundError(folder, 2, "no linker response file (.lnk)");
    return *std::min_element(found.begin(), found.end());   // UTF-8 order is code point order
}

std::optional<stdfs::path> build(const DosRunner& runner, const Tools& tools, const stdfs::path& folder,
                                 const std::optional<stdfs::path>& work_in, std::string_view flags) {
    require_tool(tools.dosbox_x, "dosbox_x");
    require_tool(tools.msc6, "msc6");
    require_tool(tools.msvc152, "msvc152");
    if (flags.find_first_of("&|<>^%\"\r\n") != std::string_view::npos) {
        throw UsageError(std::format("compiler flags '{}' hold a character DOS reads as a command", flags));
    }
    const std::string lnk_name = response_file(folder);   // before anything is created
    const stdfs::path work = work_in && !work_in->empty() ? *work_in : fs::ScopedTempDir("build16").release();
    fs::make_dirs(work);
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(folder)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (has_extension(name, {".c", ".h", ".lnk", ".def", ".obj"})) {
            stdfs::copy_file(entry.path(), work / entry.path().filename(), stdfs::copy_options::overwrite_existing);
        }
    }
    const stdfs::path stub = tools.msvc152 / "BIN" / "WINSTUB.EXE";   // the .def's STUB
    if (stdfs::exists(stub, ec)) stdfs::copy_file(stub, work / stub.filename(), stdfs::copy_options::overwrite_existing);

    const std::string lnk = text::from_latin1(fs::read_file(work / lnk_name));
    static const Regex object_names(R"(([A-Za-z_]\w*)\.obj)", RegexFlags::IgnoreCase);
    static const Regex output_names(R"((\w+\.(?:dll|exe|drv)))", RegexFlags::IgnoreCase);
    static const Regex wants_library(R"(mdllcew\.lib)", RegexFlags::IgnoreCase);
    std::vector<std::string> objs;
    object_names.for_each_match(lnk, [&](const RegexMatch& m) {
        objs.emplace_back(m.group(1));
        return true;
    });
    std::string out_name = "out.dll";
    if (const auto m = output_names.search(lnk)) out_name = std::string(m->group(1));

    const bool needs_libentry = std::any_of(objs.begin(), objs.end(), [](const std::string& o) { return text::iequals(o, "libentry"); });
    if (needs_libentry && !stdfs::exists(work / "LIBENTRY.OBJ", ec)) {
        if (const auto entry = find_libentry(tools)) stdfs::copy_file(*entry, work / "LIBENTRY.OBJ", stdfs::copy_options::overwrite_existing);
    }
    if (tools.win31_kit && wants_library.test(lnk)) {   // LINK looks in the current folder first
        for (const char* lib : {"MDLLCEW.LIB", "LIBW.LIB"}) {
            if (stdfs::exists(*tools.win31_kit / lib, ec)) stdfs::copy_file(*tools.win31_kit / lib, work / lib, stdfs::copy_options::overwrite_existing);
        }
    }

    std::vector<std::string> modules;   // the objects that have a source to compile
    for (const std::string& m : objs) {
        if (stdfs::exists(work / (m + ".c"), ec)) modules.push_back(m);
    }
    DosRunSpec spec;
    for (const std::string& m : modules) spec.commands.push_back("del " + m + ".obj");
    spec.commands.push_back("del " + out_name);
    spec.commands.push_back("del done.txt");
    for (const std::string& m : modules) spec.commands.push_back(std::format("cl /c {} {}.c >> CL.TXT", flags, m));
    spec.commands.push_back("link @" + lnk_name + " > LINK.TXT");
    spec.commands.push_back("rc " + out_name + " > RC.TXT");
    // windows.h is in the SDK's (Visual C++ 1.52c's) INCLUDE, the C library's headers in Microsoft C 6's
    spec.cpu.insert_or_assign("cycles", "max");
    spec.variables.insert_or_assign("PATH", "C:\\BIN;E:\\BIN");
    spec.variables.insert_or_assign("INCLUDE", "C:\\INCLUDE;E:\\INCLUDE");
    spec.variables.insert_or_assign("LIB", "D:\\;E:\\LIB");
    spec.mounts = {{"c", tools.msc6}, {"e", tools.msvc152}, {"d", work}};
    spec.artifacts = {out_name};
    spec.timeout = std::chrono::seconds(600);
    spec.work = work;
    (void)runner.run(spec);
    const stdfs::path dll = work / out_name;
    return stdfs::exists(dll, ec) ? std::optional(dll) : std::nullopt;
}

std::vector<Segment> segments(ByteView file) {
    const std::uint32_t lfa = u32_at(file, 0x3C, kReader);
    const std::uint16_t count = u16_at(file, lfa + 0x1CULL, kReader);
    const std::uint64_t table = std::uint64_t{u16_at(file, lfa + 0x22ULL, kReader)} + lfa;
    const unsigned shift = alignment_shift(file, lfa);
    std::vector<Segment> out;
    for (std::uint64_t i = 0; i < count; ++i) {
        const std::uint16_t sector = u16_at(file, table + i * 8, kReader);
        const std::uint16_t length = u16_at(file, table + i * 8 + 2, kReader);
        // sector 0: the segment has no image in the file
        out.push_back({length, sector == 0 ? ByteView{} : io::checked_range(file, std::uint64_t{sector} << shift, length)});
    }
    return out;
}

Comparison compare(ByteView built, ByteView original) {
    const std::vector<Segment> b = segments(built);
    const std::vector<Segment> o = segments(original);
    Comparison out;
    out.total = o.size();
    out.lines.push_back(std::format("built: {} segments, {} bytes   original: {} segments, {} bytes", b.size(), built.size(),
                                    o.size(), original.size()));
    const std::size_t count = std::max(b.size(), o.size());
    for (std::size_t i = 0; i < count; ++i) {
        const std::optional<std::uint16_t> bl = i < b.size() ? std::optional(b[i].length) : std::nullopt;
        const std::optional<std::uint16_t> ol = i < o.size() ? std::optional(o[i].length) : std::nullopt;
        std::string verdict;
        if (i < b.size() && i < o.size() && equal_bytes(b[i].bytes, o[i].bytes)) {
            verdict = "identical";
            ++out.identical;
        } else {
            verdict = bl == ol ? "same length, different" : "different length";
        }
        out.lines.push_back(std::format("  segment {:<2} built {:<5} original {:<5} {}", i + 1,
                                        bl ? std::to_string(*bl) : std::string("-"), ol ? std::to_string(*ol) : std::string("-"),
                                        verdict));
    }
    out.lines.push_back(std::format("{} of {} segments identical", out.identical, o.size()));
    return out;
}

Bytes normalize(ByteView file, ByteView donor) {
    ByteView stub = donor;
    if (donor.size() >= 2 && donor[0] == 'M' && donor[1] == 'Z') {
        const std::uint32_t lfanew = u32_at(donor, 0x3C, kReader);
        if (lfanew > 0 && lfanew <= donor.size()) stub = donor.first(lfanew);
    }
    const std::uint64_t ne = u32_at(file, 0x3C, kReader);
    const std::uint64_t new_ne = stub.size();
    if (new_ne % 16 != 0 || new_ne > ne) {
        throw FormatError(kReader, new_ne, std::format("the donor's stub (0x{:x}) cannot replace this one (0x{:x})", new_ne, ne));
    }
    const std::uint64_t shift = ne - new_ne;
    const unsigned align = alignment_shift(file, ne);
    if (shift % (std::uint64_t{1} << align) != 0) {
        throw FormatError(kReader, new_ne, std::format("the stubs differ by 0x{:x}, not a whole number of sectors", shift));
    }
    const std::int64_t sectors = static_cast<std::int64_t>(shift >> align);
    if (ne > file.size()) throw TruncatedError(kReader, ne, 0x40, 0);

    Bytes out(stub.begin(), stub.end());
    append(out, file.subspan(ne));
    put_u32_at(out, 0x3C, static_cast<std::int64_t>(new_ne), kReader);   // e_lfanew is a dword (a raw donor stub may have junk in its high word)
    const std::uint16_t count = u16_at(out, new_ne + 0x1C, kReader);
    const std::uint16_t table = u16_at(out, new_ne + 0x22, kReader);
    for (std::uint64_t i = 0; i < count; ++i) {
        const std::uint64_t p = new_ne + table + i * 8;
        const std::uint16_t sector = u16_at(out, p, kReader);
        if (sector != 0) put_u16_at(out, p, std::int64_t{sector} - sectors, kReader);   // 0: no image in the file
    }
    const std::uint32_t nonresident = u32_at(out, new_ne + 0x2C, kReader);
    if (nonresident != 0) put_u32_at(out, new_ne + 0x2C, std::int64_t{nonresident} - static_cast<std::int64_t>(shift), kReader);
    const std::uint16_t fast = u16_at(out, new_ne + 0x38, kReader);
    if (fast != 0) put_u16_at(out, new_ne + 0x38, std::int64_t{fast} - sectors, kReader);
    return out;
}

}  // namespace cb::build::period16
