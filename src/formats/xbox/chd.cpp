#include "formats/xbox/chd.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::chd {

namespace {

constexpr std::string_view kChdReader = "chd";
constexpr std::uint32_t kChdSupportedVersion = 5;

// A compressor tag: four characters, big-endian in the header.
std::string chd_tag_text(std::uint32_t tag) {
    std::string s(4, ' ');
    for (std::size_t i = 0; i < 4; ++i) s[i] = static_cast<char>(tag >> (24 - 8 * i));
    return s;
}

const std::filesystem::path& chd_need_tool(const Tool& chdman) {
    if (!chdman) throw ToolError("chdman", 0, "", "not found: install MAME's tools (chdman.exe ships with MAME) and name it in settings.json (chdman) if it is not on PATH");
    return *chdman;
}

std::string chd_run(const Tool& chdman, std::vector<std::string> arguments) {
    arguments.insert(arguments.begin(), fs::to_text(chd_need_tool(chdman)));
    const process::ProcessResult r = process::run({.argv = std::move(arguments), .err = process::Capture::Merge});
    const std::string output(as_text(r.out));
    if (!r.ok()) throw ToolError("chdman", r.exit_code, output, "failed");
    return output;
}

std::uint64_t chd_written(const std::filesystem::path& file) {
    std::error_code ec;
    const std::uint64_t size = std::filesystem::is_regular_file(file, ec) ? std::filesystem::file_size(file, ec) : 0;
    if (size == 0) throw ToolError("chdman", 0, "", "reported success but wrote no output");
    return size;
}

}  // namespace

Header read_header(ByteView head) {
    io::ByteReader r(head, kChdReader);
    if (head.size() < kMagic.size() || as_text(head.first(kMagic.size())) != kMagic) throw FormatError(kChdReader, 0, "not a CHD file (bad magic)");
    r.skip(kMagic.size());
    Header h;
    h.header_len = r.u32_be();
    h.version = r.u32_be();
    if (h.version != kChdSupportedVersion) {
        throw UnsupportedError(kChdReader, 12, std::format("CHD v{} is not read here (chdman copy upgrades it to v5)", h.version));
    }
    for (int i = 0; i < 4; ++i) {
        const std::uint32_t tag = r.u32_be();
        if (tag) h.compressors.push_back(chd_tag_text(tag));
    }
    h.logical_bytes = r.u64_be();
    h.map_offset = r.u64_be();
    h.meta_offset = r.u64_be();
    h.hunk_bytes = r.u32_be();
    h.unit_bytes = r.u32_be();
    return h;
}

Header read_header_file(const std::filesystem::path& file) {
    const fs::MappedFile f(file);
    return read_header(f.view().first(std::min(f.view().size(), kHeaderBytes)));
}

std::uint64_t extract(const Tool& chdman, const std::filesystem::path& chd, const std::filesystem::path& iso) {
    chd_run(chdman, {"extractdvd", "-i", fs::to_text(chd), "-o", fs::to_text(iso), "-f"});
    return chd_written(iso);
}

std::uint64_t build(const Tool& chdman, const std::filesystem::path& iso, const std::filesystem::path& chd) {
    std::error_code ec;
    std::filesystem::remove(chd, ec);
    chd_run(chdman, {"createdvd", "-i", fs::to_text(iso), "-o", fs::to_text(chd)});
    return chd_written(chd);
}

std::string verify(const Tool& chdman, const std::filesystem::path& chd) { return chd_run(chdman, {"verify", "-i", fs::to_text(chd)}); }

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "chd";
    c.summary = "CHD (MAME) disc images: header info, and chdman-driven conversion and verify";
    c.area = "Xbox";
    const Tool tool = environment.chdman;
    c.subcommands.push_back(cli::Command{"info", "the header (no chdman)", {cli::Positional{"file", "the .chd"}}, [](const cli::Args& a) {
        const std::string path = a.required("file");
        const Header h = read_header_file(fs::from_text(path));
        std::string codecs;
        for (const std::string& tag : h.compressors) codecs += (codecs.empty() ? "" : ", ") + tag;
        out("CHD v{}: {}\n", h.version, path);
        out("logical size : {} bytes ({:.2f} GB)\n", h.logical_bytes, static_cast<double>(h.logical_bytes) / (1ull << 30));
        out("hunk / unit  : {} / {} bytes\n", h.hunk_bytes, h.unit_bytes);
        out("compressors  : {}\n", codecs.empty() ? "none" : codecs);
        out("on-disk size : {} bytes\n", fs::file_size(fs::from_text(path)));
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"toiso", "extract to an ISO (needs chdman)", {cli::Positional{"file", "the .chd"}, cli::Positional{"out", "the .iso"}}, [tool](const cli::Args& a) {
        out("extracted {} bytes -> {}\n", extract(tool, fs::from_text(a.required("file")), fs::from_text(a.required("out"))), a.required("out"));
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"tochd", "build a DVD CHD from an ISO (needs chdman)", {cli::Positional{"file", "the .iso"}, cli::Positional{"out", "the .chd"}}, [tool](const cli::Args& a) {
        out("built CHD ({} bytes) -> {}\n", build(tool, fs::from_text(a.required("file")), fs::from_text(a.required("out"))), a.required("out"));
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"verify", "chdman's integrity verify", {cli::Positional{"file", "the .chd"}}, [tool](const cli::Args& a) {
        const std::string report = verify(tool, fs::from_text(a.required("file")));
        out("{}\n", text::trim(report));
        return kExitOk;
    }});
    return c;
}

}  // namespace cb::formats::xbox::chd
