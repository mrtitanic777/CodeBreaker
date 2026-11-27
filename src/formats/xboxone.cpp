// cb-unity: alone
#include "formats/xboxone.hpp"

#include "formats/deflate.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/probe.hpp"
#include "formats/read.hpp"
#include "formats/zip.hpp"

namespace cb::formats::xboxone {

namespace {

constexpr std::string_view kReader = "xboxone";
constexpr std::uint64_t kMaxFile = 1ull << 30;       // a package CodeBreaker reads whole
constexpr std::uint64_t kMaxMember = 512ull << 20;   // one file of a package, inflated
constexpr std::uint16_t kMachineX64 = 0x8664;

std::string lower(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

bool ends_with_any(const std::string& name, std::initializer_list<std::string_view> endings) {
    const std::string l = lower(name);
    return std::ranges::any_of(endings, [&](std::string_view e) { return l.ends_with(e); });
}

// ---- a zip: formats/zip.hpp, read where it lies (zip64 packages are common; the CRCs are not looked at) -------------------

using Zip = ZipArchive;
using ZipEntry = ZipArchive::Entry;

Zip open_zip(ByteView data) { return ZipArchive::view(data, "package", {.zip64 = true, .check_crc = false}); }

// One member of a package, inflated (no larger than CodeBreaker reads).
Bytes read_member(const Zip& zip, const ZipEntry& e) {
    if (e.size > kMaxMember || e.compressed_size > kMaxFile) throw FormatError(kReader, e.header_offset, std::format("{} is larger than CodeBreaker reads", e.name));
    return zip.read(e);
}

// ---- the manifest ---------------------------------------------------------------------------------------------

// The value of attribute `name` of the first element `<tag ...>` in the XML text, or empty.
std::string attribute(const std::string& xml, std::string_view tag, std::string_view name) {
    const std::string open = "<" + std::string(tag);
    for (std::size_t at = xml.find(open); at != std::string::npos; at = xml.find(open, at + 1)) {
        const std::size_t after = at + open.size();
        if (after >= xml.size() || (xml[after] != ' ' && xml[after] != '\t' && xml[after] != '\r' && xml[after] != '\n' && xml[after] != '/' && xml[after] != '>')) continue;   // (another tag with this prefix)
        const std::size_t end = xml.find('>', at);
        const std::string_view element = std::string_view(xml).substr(at, end == std::string::npos ? std::string::npos : end - at);
        const std::string key = " " + std::string(name) + "=\"";
        const std::size_t k = element.find(key);
        if (k == std::string_view::npos) return {};
        const std::size_t value = k + key.size();
        const std::size_t close = element.find('"', value);
        return close == std::string_view::npos ? std::string() : std::string(element.substr(value, close - value));
    }
    return {};
}

bool is_pe64(ByteView file, bool* app_container = nullptr) {
    const auto lfanew = try_get<std::uint32_t>(file, 0x3C);
    if (file.size() < 0x40 || file[0] != 'M' || file[1] != 'Z' || !lfanew) return false;
    const std::uint64_t pe = *lfanew;
    if (try_get<std::uint32_t>(file, pe).value_or(0) != 0x00004550) return false;
    if (try_get<std::uint16_t>(file, pe + 4).value_or(0) != kMachineX64) return false;
    if (try_get<std::uint16_t>(file, pe + 24).value_or(0) != 0x20B) return false;
    if (app_container) *app_container = (try_get<std::uint16_t>(file, pe + 24 + 70).value_or(0) & 0x1000) != 0;
    return true;
}

struct Package {
    Program program;
};

Program from_package(const Zip& zip, const std::string& container, const std::string& wanted) {
    const ZipEntry* manifest = zip.find("AppxManifest.xml");
    if (!manifest) throw FormatError(kReader, 0, "the package has no AppxManifest.xml");
    const Bytes manifest_bytes = read_member(zip, *manifest);
    const std::string xml(reinterpret_cast<const char*>(manifest_bytes.data()), manifest_bytes.size());
    Program p;
    p.container = container;
    p.identity = attribute(xml, "Identity", "Name");
    p.publisher = attribute(xml, "Identity", "Publisher");
    p.version = attribute(xml, "Identity", "Version");
    p.architecture = attribute(xml, "Identity", "ProcessorArchitecture");
    for (const ZipEntry& e : zip.entries()) {
        if (ends_with_any(e.name, {".exe", ".dll"})) p.members.push_back({e.name, e.size});
        else if (!e.name.empty() && e.name.back() != '/') ++p.other_members;
    }
    std::string executable = wanted;
    if (executable.empty()) executable = attribute(xml, "Application", "Executable");
    std::ranges::replace(executable, '\\', '/');
    const ZipEntry* chosen = executable.empty() ? nullptr : zip.find(executable);
    if (!chosen && !wanted.empty()) throw FormatError(kReader, 0, std::format("the package has no member {}", wanted));
    if (!chosen) {   // no manifest executable that is in the package: the largest .exe
        for (const ZipEntry& e : zip.entries()) {
            if (ends_with_any(e.name, {".exe"}) && (!chosen || e.size > chosen->size)) chosen = &e;
        }
    }
    if (!chosen) throw FormatError(kReader, 0, wanted.empty() ? "the package holds no executable" : std::format("the package has no member {}", wanted));
    p.file = read_member(zip, *chosen);
    p.member = chosen->name;
    if (!is_pe64(p.file, &p.app_container)) throw FormatError(kReader, 0, std::format("{} is not an x64 PE32+ image", chosen->name));
    return p;
}

}  // namespace

Program parse(ByteView data, const std::string& name, const std::string& member) {
    if (data.size() >= 0x208 && std::memcmp(data.data() + 0x200, "msft-xvd", 8) == 0)
        throw FormatError(kReader, 0x200, "an Xbox package (XVD, .xvc / .msixvc): recognised, not handled -- its content is encrypted with the console's keys and CodeBreaker does not decrypt");
    if (data.size() >= 2 && data[0] == 'M' && data[1] == 'Z') {
        Program p;
        p.container = "PE32+ executable";
        p.file.assign(data.begin(), data.end());
        if (!is_pe64(p.file, &p.app_container)) throw FormatError(kReader, 0, "not an x64 PE32+ image");
        return p;
    }
    if (data.size() < 4 || data[0] != 'P' || data[1] != 'K') throw FormatError(kReader, 0, "not a PE32+ image or a package (zip)");
    const Zip outer = open_zip(data);
    const bool msix = ends_with_any(name, {".msix", ".msixbundle"});
    if (const ZipEntry* bundle = outer.find("AppxMetadata/AppxBundleManifest.xml")) {
        (void)bundle;
        // a bundle: the package that holds the x64 code
        const ZipEntry* inner = nullptr;
        for (const ZipEntry& e : outer.entries()) {
            if (!ends_with_any(e.name, {".msix", ".appx"})) continue;
            const bool x64 = lower(e.name).find("x64") != std::string::npos;
            if (!inner || (x64 && lower(inner->name).find("x64") == std::string::npos)) inner = &e;
        }
        if (!inner) throw FormatError(kReader, 0, "the bundle holds no package");
        const Bytes inner_bytes = read_member(outer, *inner);
        Program p = from_package(open_zip(inner_bytes), msix ? "MSIX bundle" : "APPX bundle", member);
        return p;
    }
    return from_package(outer, msix ? "MSIX package" : "APPX package", member);
}

Program read(const std::filesystem::path& path, const std::string& member) {
    const auto size = size_of(path);
    if (!size) throw NotFoundError(path, 2);
    if (is_encrypted_package(path)) throw FormatError(kReader, 0, "an encrypted Xbox package (.xvc / .msixvc / .eappx): recognised, not handled -- CodeBreaker does not decrypt");
    if (*size > kMaxFile) throw FormatError(kReader, 0, "the file is larger than CodeBreaker reads whole");
    return parse(fs::read_file(path), path.filename().string(), member);
}

pe::Image image_of(const Program& program) { return pe::Image::parse(program.file, program.member.empty() ? "pe" : program.member); }

bool is_encrypted_package(const std::filesystem::path& path) {
    if (has_extension(path, {".xvc", ".msixvc", ".eappx", ".eappxbundle", ".emsix", ".emsixbundle"})) return true;
    const auto at = read_prefix(path, 0x200, 8);
    return at && at->size() == 8 && std::memcmp(at->data(), "msft-xvd", 8) == 0;
}

bool probe(const std::filesystem::path& path) {
    try {
        if (is_encrypted_package(path)) return true;
        const auto head = read_prefix(path, 0, 8192);
        if (!head || head->size() < 0x40) return false;
        if ((*head)[0] == 'M' && (*head)[1] == 'Z') {
            bool app = false;
            return is_pe64(*head, &app) && app;   // (an x64 PE32+ built for the app sandbox: a UWP app's)
        }
        if ((*head)[0] != 'P' || (*head)[1] != 'K' || !has_extension(path, {".appx", ".msix", ".appxbundle", ".msixbundle"})) return false;
        // a package: its central directory names AppxManifest.xml (or a bundle's manifest)
        const auto size = size_of(path);
        if (!size || *size < 22 || *size > kMaxFile) return false;
        const std::uint64_t tail = std::min<std::uint64_t>(*size, 70000);
        const auto end = read_prefix(path, *size - tail, tail);
        if (!end) return false;
        const auto eocd = rfind(*end, Bytes{'P', 'K', 5, 6});
        if (!eocd) return false;
        std::uint64_t cd_size = get<std::uint32_t>(*end, *eocd + 12, kReader), cd_offset = get<std::uint32_t>(*end, *eocd + 16, kReader);
        if (cd_offset == 0xFFFFFFFFull || cd_size == 0xFFFFFFFFull) {   // zip64: the central directory's place is in the record the locator names
            if (*eocd < 20 || get<std::uint32_t>(*end, *eocd - 20, kReader) != 0x07064B50) return false;
            const std::uint64_t record = get<std::uint64_t>(*end, *eocd - 20 + 8, kReader);
            const auto r = read_prefix(path, record, 56);
            if (!r || r->size() < 56 || get<std::uint32_t>(*r, 0, kReader) != 0x06064B50) return false;
            cd_size = get<std::uint64_t>(*r, 40, kReader);
            cd_offset = get<std::uint64_t>(*r, 48, kReader);
        }
        if (cd_size > (64u << 20) || cd_offset > *size) return false;
        const auto cd = read_prefix(path, cd_offset, cd_size);
        if (!cd) return false;
        const std::string text(reinterpret_cast<const char*>(cd->data()), cd->size());
        return text.find("AppxManifest.xml") != std::string::npos || text.find("AppxBundleManifest.xml") != std::string::npos;
    } catch (const Error&) {
        return false;
    }
}

}  // namespace cb::formats::xboxone
