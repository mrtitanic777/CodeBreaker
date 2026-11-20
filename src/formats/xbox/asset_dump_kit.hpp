// The test kit of the asset readers' tests (xbe, xdbf, xpr, xur, bink, chd): the same one-line-per-fact dump
// testdata/assets/make_asset_goldens makes of the recorded readers, made here of ours, so a test compares line by line.
#pragma once

#include <format>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/xbox/bink.hpp"
#include "formats/xbox/chd.hpp"
#include "formats/xbox/xbe.hpp"
#include "formats/xbox/xdbf.hpp"
#include "formats/xbox/xpr.hpp"
#include "formats/xbox/xur.hpp"

namespace cb::formats::xbox::asset_dump {

using Lines = std::vector<std::string>;

inline std::string sha12(ByteView v) { return ::cb::crypto::to_hex(::cb::crypto::sha1(v)).substr(0, 12); }   // (fully named: formats::xbox has a crypto of its own)

inline std::string esc(const std::string& s) {
    std::string out;
    for (const char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

inline std::string fixture(std::string_view name) { return fs::to_text(testing::data_path(std::string("formats/xbox/testdata/assets/") + std::string(name))); }

// The goldens of one fixture: the lines of python_goldens.txt (the recorded dump) that start with "<name>.".
inline Lines golden(const std::string& name) {
    Lines out;
    const std::string body = fs::read_text(fs::from_text(fixture("python_goldens.txt")));
    for (const std::string_view line : text::lines(body)) {
        if (line.starts_with(name + ".")) out.emplace_back(line);
    }
    return out;
}

inline Lines dump_xdbf(const std::string& n, ByteView data) {
    const xdbf::Database db = xdbf::Database::parse(data);
    Lines out = {std::format("{}.version=0x{:X}", n, db.version), std::format("{}.entries={}", n, db.entries.size()),
                 std::format("{}.table={}", n, db.entry_table_len), std::format("{}.free={}", n, db.free_count),
                 std::format("{}.data_start=0x{:X}", n, db.data_start())};
    for (const auto& [ns, c] : db.counts()) out.push_back(std::format("{}.ns{}={}", n, ns, c));
    std::size_t i = 0;
    for (const xdbf::Entry& e : db.entries) out.push_back(std::format("{}.entry{}={},{:016X},{},{}", n, i++, e.ns, e.id, e.payload.size(), sha12(e.payload)));
    const std::vector<xdbf::Achievement> all = db.achievements();
    i = 0;
    for (const xdbf::Achievement& a : all) {
        out.push_back(std::format("{}.ach{}={},{},{},{},{},{}|{}|{}", n, i++, a.id, a.image, a.score, a.flags, a.unlocked ? 1 : 0, esc(a.name),
                                  esc(a.unlocked_desc), esc(a.locked_desc)));
    }
    out.push_back(std::format("{}.build={}", n, sha12(db.build())));
    if (!all.empty()) {
        xdbf::Database y = db;
        const bool ok = y.unlock_achievement(all[0].id, true);
        out.push_back(std::format("{}.unlock_first={},{}", n, ok ? 1 : 0, sha12(y.build())));
        y.unlock_achievement(all[0].id, false);
        out.push_back(std::format("{}.lock_first={}", n, sha12(y.build())));
    }
    xdbf::Database z = db;
    const std::size_t unlocked = z.unlock_all();
    out.push_back(std::format("{}.unlock_all={},{}", n, unlocked, sha12(z.build())));
    xdbf::Database w = db;
    w.set_payload(5, 0x1234, to_bytes("hello"));
    out.push_back(std::format("{}.set_new={}", n, sha12(w.build())));
    return out;
}

inline Lines dump_xbe(const std::string& n, ByteView data) {
    const xbe::Xbe x = xbe::Xbe::parse(data);
    const xbe::Unscrambled ep = x.entry_point(), kt = x.kernel_thunk();
    Lines out = {std::format("{}.title={}", n, esc(x.title_name)),
                 std::format("{}.title_id={:08X},{}", n, x.title_id, x.title_id_text()),
                 std::format("{}.base=0x{:X}", n, x.base),
                 std::format("{}.size={}", n, x.size_of_image),
                 std::format("{}.sections={}", n, x.section_count),
                 std::format("{}.region=0x{:X},{}", n, x.region, x.region_text()),
                 std::format("{}.media=0x{:X},{}", n, x.allowed_media, x.media_text()),
                 std::format("{}.entry={:08X},{}", n, ep.address, xbe::key_set_name(ep.keys)),
                 std::format("{}.thunk={:08X},{}", n, kt.address, xbe::key_set_name(kt.keys)),
                 std::format("{}.ratings=0x{:X},{},{}", n, x.ratings, x.version, x.disk_number)};
    std::size_t i = 0;
    for (const xbe::Section& s : x.sections()) {
        out.push_back(std::format("{}.section{}={},0x{:X},0x{:X},{},0x{:X},{}", n, i++, esc(s.name), s.flags, s.vaddr, s.vsize, s.raw_addr, s.raw_size));
    }
    try {
        const Bytes pe = x.pe_image();
        out.push_back(std::format("{}.pe={},{}", n, pe.size(), sha12(pe)));
    } catch (const Error&) {   // the verdict only: the messages differ
        out.push_back(n + ".pe=error");
    }
    return out;
}

inline Lines dump_xpr(const std::string& n, ByteView data) {
    const xpr::Bundle b = xpr::parse(data);
    Lines out = {std::format("{}.container={}", n, xpr::container_name(b.container))};
    if (b.container == xpr::Container::Xpr0) {
        out.push_back(std::format("{}.format_name={}", n, b.format_name));
        out.push_back(std::format("{}.dds_fmt={}", n, b.dds_format ? std::string(texture::info(*b.dds_format).name) : std::string("None")));
        out.push_back(std::format("{}.width={}", n, b.width));
        out.push_back(std::format("{}.height={}", n, b.height));
        out.push_back(std::format("{}.mipmaps={}", n, b.mipmaps));
        out.push_back(std::format("{}.cubemap={}", n, b.cubemap ? 1 : 0));
        out.push_back(std::format("{}.data_offset={}", n, b.data_offset));
        out.push_back(std::format("{}.total={}", n, b.total));
    } else {
        out.push_back(std::format("{}.data_offset={}", n, b.data_offset));
        out.push_back(std::format("{}.header_size={}", n, b.header_size));
        out.push_back(std::format("{}.data_size={}", n, b.data_size));
    }
    out.push_back(std::format("{}.pixels={},{}", n, b.pixels.size(), sha12(b.pixels)));
    try {
        if (b.container == xpr::Container::Xpr2) {
            // the recorded untile answers only as much of the surface as the (short) pixel data reaches, ours the whole surface: compare the part both have
            const Bytes dds = xpr::to_dds(b, 64, 32);
            out.push_back(std::format("{}.dds_prefix={}", n, sha12(ByteView(dds).first(std::min<std::size_t>(dds.size(), 1408)))));
        } else {
            const Bytes dds = xpr::to_dds(b);
            out.push_back(std::format("{}.dds={},{}", n, dds.size(), sha12(dds)));
        }
    } catch (const Error&) {
        out.push_back(n + ".dds=error");
    }
    return out;
}

inline Lines dump_xur(const std::string& n, ByteView data) {
    const xur::Scene x = xur::Scene::parse(data);
    Lines out = {std::format("{}.version={}", n, x.version), std::format("{}.flags={}", n, x.flags), std::format("{}.tool={}", n, x.tool_version),
                 std::format("{}.file_size={}", n, x.file_size), std::format("{}.sections={}", n, x.sections.size())};
    std::size_t i = 0;
    for (const xur::Section& s : x.sections) out.push_back(std::format("{}.section{}={},{},{}", n, i++, xur::tag_text(s.tag), s.offset, s.length));
    try {
        const std::vector<std::string> strs = x.strings();
        out.push_back(std::format("{}.strings={}", n, strs.size()));
        i = 0;
        for (const std::string& s : strs) out.push_back(std::format("{}.string{}={}", n, i++, esc(s)));
        out.push_back(std::format("{}.identity={}", n, equal_bytes(xur::set_strings(data, strs), data) ? 1 : 0));
        if (!strs.empty()) out.push_back(std::format("{}.set0={}", n, sha12(xur::set_string(data, 0, "Hello w\xC3\xB6rld"))));
    } catch (const Error&) {
        out.push_back(n + ".identity=error");
    }
    return out;
}

// A float that is whole prints "30.0".
inline std::string whole_float(double v) {
    std::string s = text::format_double(v);
    if (s.find('.') == std::string::npos) s += ".0";
    return s;
}

inline Lines dump_bink(const std::string& n, ByteView data) {
    const bink::Header h = bink::read_header(data.first(std::min<std::size_t>(data.size(), 64)));
    return {std::format("{}.generation={}", n, h.generation), std::format("{}.codec={}", n, h.codec),
            std::format("{}.file_size={}", n, h.file_size),   std::format("{}.frames={}", n, h.frames),
            std::format("{}.largest_frame={}", n, h.largest_frame), std::format("{}.width={}", n, h.width),
            std::format("{}.height={}", n, h.height),         std::format("{}.fps={}", n, whole_float(h.fps)),
            std::format("{}.video_flags=0x{:08X}", n, h.video_flags), std::format("{}.audio_tracks={}", n, h.audio_tracks)};
}

inline Lines dump_chd(const std::string& n, ByteView data) {
    const chd::Header h = chd::read_header(data.first(std::min(data.size(), chd::kHeaderBytes)));
    std::string codecs;
    for (const std::string& c : h.compressors) codecs += (codecs.empty() ? "" : ",") + c;
    return {std::format("{}.version={}", n, h.version),       std::format("{}.header_len={}", n, h.header_len),
            std::format("{}.logical_bytes={}", n, h.logical_bytes), std::format("{}.hunk_bytes={}", n, h.hunk_bytes),
            std::format("{}.unit_bytes={}", n, h.unit_bytes), std::format("{}.compressors={}", n, codecs),
            std::format("{}.meta_offset={}", n, h.meta_offset), std::format("{}.map_offset={}", n, h.map_offset)};
}

// The lines that differ, as one message ("" when equal).
inline std::string difference(const Lines& expected, const Lines& got) {
    std::string out;
    for (std::size_t i = 0; i < std::max(expected.size(), got.size()); ++i) {
        const std::string a = i < expected.size() ? expected[i] : "(none)", b = i < got.size() ? got[i] : "(none)";
        if (a != b) out += std::format("  want:   {}\n  ours:   {}\n", a, b);
    }
    return out;
}

}  // namespace cb::formats::xbox::asset_dump
