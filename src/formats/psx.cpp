#include "formats/psx.hpp"

#include <algorithm>
#include <cstring>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/text.hpp"
#include "formats/iso9660.hpp"
#include "formats/probe.hpp"

namespace cb::formats::psx {

namespace {

constexpr std::string_view kReader = "psx";
constexpr std::size_t kMarkerAt = 0x4C;

bool has_magic(ByteView data) { return data.size() >= kMagic.size() && std::memcmp(data.data(), kMagic.data(), kMagic.size()) == 0; }

}  // namespace

Bytes Exe::memory() const {
    Bytes ram(kRamSize, 0);
    const ByteView program = text();
    const std::size_t at = header.t_addr & kRamMask;
    if (at < ram.size()) std::copy_n(program.begin(), std::min(program.size(), ram.size() - at), ram.begin() + static_cast<std::ptrdiff_t>(at));
    return ram;
}

std::vector<std::uint32_t> Exe::functions(const Decoder& decode) const {
    const Bytes ram = memory();
    const std::uint32_t begin = header.t_addr & kRamMask;
    const std::uint32_t end = std::min<std::uint32_t>(begin + static_cast<std::uint32_t>(text().size()), kRamSize);
    std::set<std::uint32_t> starts;
    const std::uint32_t entry = header.pc0 & kRamMask;
    if (entry >= begin && entry < end && entry % 4 == 0) starts.insert(entry);
    const std::vector<std::uint32_t> found = find_functions(ram, std::move(starts), decode, {.begin = begin, .end = end});
    std::vector<std::uint32_t> out;
    for (const std::uint32_t a : found)
        if (a >= begin && a < end && a % 4 == 0) out.push_back(a | region());
    return out;
}

Exe parse(ByteView data) {
    if (!has_magic(data)) throw FormatError(kReader, 0, "not a PS-X EXE (no \"PS-X EXE\" at the start)");
    if (data.size() < kHeaderSize) throw FormatError(kReader, 0, "shorter than the 2048-byte header");
    Exe exe;
    exe.data.assign(data.begin(), data.end());
    io::ByteReader r(exe.data, kReader);
    r.seek(0x10);
    Header& h = exe.header;
    h.pc0 = r.u32();
    h.gp0 = r.u32();
    h.t_addr = r.u32();
    h.t_size = r.u32();
    h.data_addr = r.u32();
    h.data_size = r.u32();
    h.bss_addr = r.u32();
    h.bss_size = r.u32();
    h.sp_base = r.u32();
    h.sp_offset = r.u32();
    std::size_t n = kMarkerAt;
    while (n < kHeaderSize && exe.data[n] != 0) ++n;
    h.marker.assign(reinterpret_cast<const char*>(exe.data.data()) + kMarkerAt, n - kMarkerAt);
    return exe;
}

std::optional<std::string> boot_file(std::string_view system_cnf, std::string_view key, std::string* value) {
    for (const std::string_view line : text::lines(system_cnf)) {
        const std::string_view t = text::trim(line);
        if (t.size() <= key.size() || !text::iequals(t.substr(0, key.size()), key)) continue;
        const std::string_view rest = text::trim(t.substr(key.size()));
        if (rest.empty() || rest.front() != '=') continue;   // (BOOT2 is not BOOT)
        const std::string_view v = text::trim(rest.substr(1));
        if (value) *value = std::string(v);
        std::string path(v.substr(0, v.find_first_of(" \t")));   // (arguments follow the file, after a space)
        if (const std::size_t colon = path.find(':'); colon != std::string::npos) path = path.substr(colon + 1);
        if (const std::size_t semi = path.find(';'); semi != std::string::npos) path = path.substr(0, semi);
        for (char& c : path)
            if (c == '\\') c = '/';
        if (path.empty()) return std::nullopt;
        if (path.front() != '/') path.insert(path.begin(), '/');
        return path;
    }
    return std::nullopt;
}

Exe parse_disc(const disc::Sectors& sectors) {
    const iso9660::Volume volume(sectors);
    std::string boot;
    std::string file = "/PSX.EXE";
    try {
        const Bytes cnf = volume.read("/SYSTEM.CNF");
        const std::optional<std::string> named = boot_file(as_text(cnf), "BOOT", &boot);
        if (!named) throw FormatError(kReader, 0, "SYSTEM.CNF names no program to boot (no BOOT line)");
        file = *named;
    } catch (const NotFoundError&) {   // no SYSTEM.CNF: the BIOS boots PSX.EXE
    }
    Bytes program;
    try {
        program = volume.read(file);
    } catch (const NotFoundError&) {
        throw FormatError(kReader, 0, boot.empty() ? "no SYSTEM.CNF and no PSX.EXE: not a PlayStation disc" : file + ", which SYSTEM.CNF boots, is not on the disc");
    }
    Exe exe = parse(program);
    exe.boot_file = file;
    exe.boot = boot;
    return exe;
}

Exe read(const std::filesystem::path& path) {
    if (disc::is_image_name(path)) {
        const std::optional<Bytes> head = read_prefix(path, 0, kMagic.size());
        if (!head || !has_magic(*head)) return parse_disc(disc::Sectors::open(path));
    }
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kMagic.size());
    if (head && has_magic(*head)) return true;
    if (!disc::is_image_name(path)) return false;
    try {
        parse_disc(disc::Sectors::open(path));
        return true;
    } catch (const Error&) {
        return false;
    }
}

}  // namespace cb::formats::psx
