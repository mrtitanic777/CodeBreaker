#include "cb/formats/xbox/summary.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/xbox/bink.hpp"
#include "cb/formats/xbox/chd.hpp"
#include "cb/formats/xbox/volumes.hpp"
#include "cb/formats/xbox/xbe.hpp"
#include "cb/formats/xbox/xdbf.hpp"
#include "cb/formats/xbox/xpr.hpp"
#include "cb/formats/xbox/xur.hpp"

namespace cb::formats::xbox {

namespace {

// A text in quotes, as it is shown in the volume line.
std::string summary_quoted(const std::string& s) {
    const char quote = (s.find('\'') != std::string::npos && s.find('"') == std::string::npos) ? '"' : '\'';
    std::string out(1, quote);
    for (const char c : s) {
        if (c == quote || c == '\\') out += '\\';
        out += c;
    }
    out += quote;
    return out;
}

SummaryFacts summary_container(const std::filesystem::path& path) {
    SummaryFacts out;
    auto opened = volumes::open(path);
    const std::size_t count = opened ? opened->size() : 0;
    out.emplace_back("volumes", std::to_string(count));
    for (std::size_t i = 0; i < std::min<std::size_t>(count, 4); ++i) {
        media::Volume& v = *(*opened)[i];
        const std::string key = count == 1 ? "volume" : std::format("volume {}", i + 1);
        std::uint64_t files = 0, bytes = 0;
        for (const media::Entry& e : v.files()) {
            if (e.is_dir) continue;
            ++files;
            bytes += e.size;
        }
        out.emplace_back(key, std::format("{} {}: {} files, {} bytes{}", media::kind_name(v.kind()), summary_quoted(v.label()), files, bytes,
                                          v.where().empty() ? std::string() : " (" + v.where() + ")"));
        for (const media::BootField& f : v.boot()) out.emplace_back(count > 1 ? key + " " + f.name : f.name, f.text);
    }
    return out;
}

SummaryFacts summary_xbe(ByteView file) {
    const xbe::Xbe x = xbe::Xbe::parse(file);
    return {{"title", x.title_name},
            {"title id", std::format("{:08X}", x.title_id)},
            {"base", std::format("0x{:08X}", x.base)},
            {"image size", std::to_string(x.size_of_image)},
            {"sections", std::to_string(x.section_count)},
            {"region", std::format("0x{:X}", x.region)},
            {"allowed media", std::format("0x{:X}", x.allowed_media)}};
}

SummaryFacts summary_xdbf(ByteView file) {
    const xdbf::Database db = xdbf::Database::parse(file);
    SummaryFacts out = {{"version", std::format("0x{:X}", db.version)}, {"entries", std::to_string(db.entries.size())}, {"free", std::to_string(db.free_count)}};
    for (const auto& [ns, n] : db.counts()) out.emplace_back("namespace " + xdbf::namespace_name(ns), std::to_string(n));
    const std::vector<xdbf::Achievement> all = db.achievements();
    std::uint64_t unlocked = 0, score = 0;
    for (const xdbf::Achievement& a : all) {
        unlocked += a.unlocked ? 1 : 0;
        score += a.score;
    }
    out.emplace_back("achievements", std::format("{} ({} unlocked, {}G)", all.size(), unlocked, score));
    return out;
}

SummaryFacts summary_xpr(ByteView file) {
    const xpr::Bundle b = xpr::parse(file);
    SummaryFacts out = {{"container", std::string(xpr::container_name(b.container))}};
    if (b.container == xpr::Container::Xpr0) {
        out.emplace_back("xfmt", std::to_string(b.xfmt));
        out.emplace_back("format_name", b.format_name);
        if (b.dds_format) out.emplace_back("dds_fmt", std::string(texture::info(*b.dds_format).name));
        out.emplace_back("width", std::to_string(b.width));
        out.emplace_back("height", std::to_string(b.height));
        out.emplace_back("mipmaps", std::to_string(b.mipmaps));
        out.emplace_back("cubemap", b.cubemap ? "true" : "false");
        out.emplace_back("data_offset", std::to_string(b.data_offset));
        out.emplace_back("total", std::to_string(b.total));
    } else {
        out.emplace_back("header_size", std::to_string(b.header_size));
        out.emplace_back("data_size", std::to_string(b.data_size));
        out.emplace_back("data_offset", std::to_string(b.data_offset));
    }
    return out;
}

// An XACT wave bank's header facts. (The full reader is cb/xna/xact.hpp; the summary needs four fields, and the formats area does not link xna.)
SummaryFacts summary_wave_bank(ByteView file) {
    constexpr std::uint32_t kLastPlausibleVersion = 0x10000;
    if (file.size() < 8) throw TruncatedError("xwb", 0, 8, file.size());
    bool big = true;
    std::uint32_t version = 0;
    for (const bool order : {true, false}) {   // the version is a small number in the right byte order
        io::ByteReader r(file, "xwb");
        r.skip(4);
        const std::uint32_t v = order ? r.u32_be() : r.u32();
        if (v >= 1 && v <= kLastPlausibleVersion) {
            big = order;
            version = v;
            break;
        }
    }
    if (!version) throw FormatError("xwb", 4, "cannot tell the byte order");
    io::ByteReader r(file, "xwb");
    r.seek(version >= 42 ? 12 : 8);   // the segment table (after the header version of newer banks)
    const std::uint32_t data_at = big ? r.u32_be() : r.u32();
    r.seek(data_at);
    r.skip(4);   // flags
    const std::uint32_t entries = big ? r.u32_be() : r.u32();
    const ByteView name = r.bytes(version >= 42 ? 64 : 16);   // ended by the first zero
    const auto name_end = std::find(name.begin(), name.end(), std::uint8_t{0});
    const std::string bank = text::from_latin1(name.first(static_cast<std::size_t>(name_end - name.begin())));
    return {{"version", std::to_string(version)}, {"byte order", big ? "big" : "little"}, {"entries", std::to_string(entries)}, {"bank_name", bank}};
}

SummaryFacts summary_xur(ByteView file) {
    const xur::Scene s = xur::Scene::parse(file);
    std::string tags;
    for (const xur::Section& sec : s.sections) tags += (tags.empty() ? "" : " ") + xur::tag_text(sec.tag);
    return {{"version", std::to_string(s.version)},
            {"tool version", std::to_string(s.tool_version)},
            {"sections", std::to_string(s.sections.size())},
            {"section tags", tags}};
}

SummaryFacts summary_bink(ByteView file) {
    const bink::Header h = bink::read_header(file.first(std::min<std::size_t>(file.size(), 64)));
    std::string fps = text::format_double(h.fps);
    if (fps.find('.') == std::string::npos) fps += ".0";
    return {{"generation", std::to_string(h.generation)}, {"codec", h.codec},
            {"file_size", std::to_string(h.file_size)},   {"frames", std::to_string(h.frames)},
            {"largest_frame", std::to_string(h.largest_frame)}, {"width", std::to_string(h.width)},
            {"height", std::to_string(h.height)},         {"fps", fps},
            {"video_flags", std::format("0x{:08X}", h.video_flags)}, {"audio_tracks", std::to_string(h.audio_tracks)}};
}

SummaryFacts summary_chd(ByteView file) {
    const chd::Header h = chd::read_header(file.first(std::min(file.size(), chd::kHeaderBytes)));
    std::string codecs;
    for (const std::string& tag : h.compressors) codecs += (codecs.empty() ? "" : ", ") + tag;
    return {{"version", std::to_string(h.version)},       {"header_len", std::to_string(h.header_len)},
            {"logical_bytes", std::to_string(h.logical_bytes)}, {"hunk_bytes", std::to_string(h.hunk_bytes)},
            {"unit_bytes", std::to_string(h.unit_bytes)}, {"compressors", codecs},
            {"meta_offset", std::to_string(h.meta_offset)}, {"map_offset", std::to_string(h.map_offset)}};
}

struct SummaryReader {
    std::string_view key;   // found in the format line
    SummaryFacts (*read)(ByteView);
};
constexpr SummaryReader kSummaryReaders[] = {{"XBE", summary_xbe},       {"XDBF", summary_xdbf}, {"resource bundle", summary_xpr}, {"wave bank", summary_wave_bank},
                                             {"XUI scene", summary_xur}, {"Bink", summary_bink}, {"CHD", summary_chd}};

}  // namespace

SummaryFacts summary(const std::filesystem::path& path, const detect::Detected& match) {
    if (std::filesystem::is_directory(path) || match.command == "media") return summary_container(path);
    for (const SummaryReader& r : kSummaryReaders) {
        if (match.format.find(r.key) == std::string::npos) continue;
        const fs::MappedFile file(path);
        return r.read(file.view());
    }
    return {};
}

}  // namespace cb::formats::xbox
