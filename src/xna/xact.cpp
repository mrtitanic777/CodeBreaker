// xna/xact.cpp -- see xact.hpp.
#include "xna/xact.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "formats/xwb.hpp"

namespace cb::xna::xact {

namespace {

// The reader name of each file kind, for messages.
constexpr std::string_view kWaveBank = "xact wave bank";
constexpr std::string_view kSoundBank = "xact sound bank";

void put_u16(Bytes& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
}
void put_u32(Bytes& b, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}

// A cursor over a bank in either byte order.
class Cursor {
public:
    Cursor(ByteView data, bool big, std::string_view name) : r_(data, name), big_(big) {}
    std::uint32_t u8() { return r_.u8(); }
    std::uint32_t u16() { return big_ ? r_.u16_be() : r_.u16(); }
    std::uint32_t u32() { return big_ ? r_.u32_be() : r_.u32(); }
    void skip(std::uint64_t n) { r_.skip(n); }
    // A position past the end is allowed (a damaged bank's offsets say so): the next read there is the truncation, where a sound that
    // cannot be read is given up and the rest of the bank goes on.
    void seek(std::uint64_t at) { r_.seek(std::min<std::uint64_t>(at, r_.size())); }
    std::size_t pos() const { return r_.pos(); }

private:
    io::ByteReader r_;
    bool big_;
};

}  // namespace

ByteView WaveBank::stream(const WaveEntry& entry) const {
    const std::uint64_t start = std::uint64_t{wave_data_offset} + entry.offset;
    if (start >= data.size()) return {};
    return ByteView(data).subspan(start, std::min<std::uint64_t>(entry.length, data.size() - start));
}

WaveBank read_wave_bank(Bytes data) {
    if (data.size() < 8) throw FormatError(kWaveBank, 0, "not an XWB");
    const formats::xwb::Header header = formats::xwb::read_header(data, kWaveBank);
    WaveBank bank;
    bank.big_endian = header.big_endian;
    bank.version = header.version;
    bank.name = header.name;
    bank.wave_data_offset = header.wave_data_offset;
    bank.data = std::move(data);
    Cursor r(bank.data, bank.big_endian, kWaveBank);
    const std::uint32_t element = header.meta_element_size != 0 ? header.meta_element_size : 24;
    for (std::uint32_t i = 0; i < header.entry_count; ++i) {
        r.seek(std::uint64_t{header.segments[1].offset} + std::uint64_t{i} * element + 4);
        const formats::xwb::MiniFormat format = formats::xwb::decode_miniformat(r.u32());
        WaveEntry e;
        e.index = i;
        e.codec = format.codec;
        e.channels = format.channels;
        e.rate = format.sample_rate;
        e.align = format.block_align;
        e.offset = r.u32();
        e.length = r.u32();
        bank.entries.push_back(e);
    }
    return bank;
}

Bytes pcm_to_wav(ByteView pcm, std::uint32_t channels, std::uint32_t rate, std::uint32_t bits) {
    Bytes samples(pcm.begin(), pcm.end());
    if (bits == 16) {   // the Xbox 360's samples are big-endian
        for (std::size_t i = 0; i + 1 < samples.size(); i += 2) std::swap(samples[i], samples[i + 1]);
    }
    Bytes out{'R', 'I', 'F', 'F'};
    put_u32(out, static_cast<std::uint32_t>(36 + samples.size()));
    for (const char c : std::string_view("WAVEfmt ")) out.push_back(static_cast<std::uint8_t>(c));
    put_u32(out, 16);
    put_u16(out, 1);
    put_u16(out, channels);
    put_u32(out, rate);
    put_u32(out, rate * channels * bits / 8);
    put_u16(out, channels * bits / 8);
    put_u16(out, bits);
    for (const char c : std::string_view("data")) out.push_back(static_cast<std::uint8_t>(c));
    put_u32(out, static_cast<std::uint32_t>(samples.size()));
    out.insert(out.end(), samples.begin(), samples.end());
    return out;
}

Bytes wrap_xma2(ByteView data, std::uint32_t channels, std::uint32_t rate, std::uint32_t block) {
    if (channels == 0) channels = 2;
    if (rate == 0) rate = 44100;
    const std::uint32_t streams = (channels + 1) / 2;
    const std::uint32_t block_count = static_cast<std::uint32_t>((data.size() + block - 1) / block);
    Bytes fmt;
    put_u16(fmt, kFormatXma2);
    put_u16(fmt, channels);
    put_u32(fmt, rate);
    put_u32(fmt, rate * channels * 2);
    put_u16(fmt, channels * 2);
    put_u16(fmt, 16);
    put_u16(fmt, 34);   // XMA2WAVEFORMATEX's tail
    put_u16(fmt, streams);
    put_u32(fmt, 0);
    put_u32(fmt, 0);
    put_u32(fmt, block);
    for (int i = 0; i < 4; ++i) put_u32(fmt, 0);
    fmt.push_back(0);
    fmt.push_back(4);
    put_u16(fmt, block_count);
    Bytes body{'W', 'A', 'V', 'E', 'f', 'm', 't', ' '};
    put_u32(body, static_cast<std::uint32_t>(fmt.size()));
    body.insert(body.end(), fmt.begin(), fmt.end());
    for (const char c : std::string_view("data")) body.push_back(static_cast<std::uint8_t>(c));
    put_u32(body, static_cast<std::uint32_t>(data.size()));
    body.insert(body.end(), data.begin(), data.end());
    Bytes out{'R', 'I', 'F', 'F'};
    put_u32(out, static_cast<std::uint32_t>(body.size()));
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

std::uint64_t decode_to_wav(ByteView xma_or_riff, const std::filesystem::path& out_wav, const std::optional<std::filesystem::path>& ffmpeg,
                            std::uint32_t channels, std::uint32_t rate, std::uint32_t block) {
    if (!ffmpeg) throw ToolError("ffmpeg", 0, "ffmpeg not found: install it and/or name it in settings.json (ffmpeg)");
    const bool wrapped = xma_or_riff.size() >= 4 && xma_or_riff[0] == 'R' && xma_or_riff[1] == 'I' && xma_or_riff[2] == 'F' && xma_or_riff[3] == 'F';
    const Bytes riff = wrapped ? Bytes(xma_or_riff.begin(), xma_or_riff.end()) : wrap_xma2(xma_or_riff, channels, rate, block);
    fs::ScopedTempDir folder{"cb-xma"};
    const std::filesystem::path in = folder.path() / "in.xma.wav";
    fs::write_file(in, riff);
    // 16-bit PCM in a plain WAVEFORMATEX: with the decoder's own layout ffmpeg writes WAVE_FORMAT_EXTENSIBLE, which MonoGame's (and many
    // players') WAV readers do not take
    const process::ProcessResult r = process::run({.argv = {fs::to_text(*ffmpeg), "-y", "-hide_banner", "-loglevel", "error", "-i", fs::to_text(in), "-c:a",
                                                            "pcm_s16le", "-ac", std::to_string(std::min<std::uint32_t>(channels, 2)), fs::to_text(out_wav)},
                                                   .err = process::Capture::Merge});
    std::error_code ec;
    if (r.exit_code != 0 || !std::filesystem::exists(out_wav, ec))
        throw ToolError("ffmpeg", r.exit_code, "XMA2 decode failed:\n" + std::string(r.out.begin(), r.out.end()));
    return std::filesystem::file_size(out_wav, ec);
}

std::size_t decode_bank(const std::filesystem::path& bank_file, const std::filesystem::path& out_dir, const std::optional<std::filesystem::path>& ffmpeg,
                        const std::function<void(std::string_view)>& say) {
    const WaveBank bank = read_wave_bank(fs::read_file(bank_file));
    fs::make_dirs(out_dir);
    std::size_t written = 0;
    for (const WaveEntry& e : bank.entries) {
        const ByteView data = bank.stream(e);
        const std::string stem = std::format("stream_{:03}", e.index);
        const std::uint32_t channels = e.channels != 0 ? e.channels : 2, rate = e.rate != 0 ? e.rate : 44100;
        if (e.codec == Codec::Pcm) {
            fs::write_file(out_dir / (stem + ".wav"), pcm_to_wav(data, channels, rate));
        } else if (e.codec == Codec::Xma) {
            try {
                decode_to_wav(data, out_dir / (stem + ".wav"), ffmpeg, channels, rate);
            } catch (const Error& ex) {
                fs::write_file(out_dir / (stem + ".xma"), Bytes(data.begin(), data.end()));
                if (say) say(std::format("  stream {}: XMA decode failed ({}); wrote raw .xma", e.index, ex.what()));
                continue;
            }
        } else {
            std::string ext(codec_name(e.codec));
            std::transform(ext.begin(), ext.end(), ext.begin(), [](char c) { return static_cast<char>(c | 0x20); });
            fs::write_file(out_dir / (stem + "." + ext), Bytes(data.begin(), data.end()));
            continue;
        }
        ++written;
    }
    return written;
}

// ---- sound banks ------------------------------------------------------------------------------------------------------------

namespace {

using Waves = std::vector<std::pair<std::int64_t, std::int64_t>>;

struct Clip {
    Waves waves;
    std::int64_t variation = 0;
    std::optional<std::vector<std::int64_t>> weights;
    std::int64_t loop = 0;
};

std::vector<Clip> clip_waves(Cursor& r) {
    r.skip(1);
    const std::uint32_t events_at = r.u32();
    r.skip(4);
    const std::size_t back = r.pos();
    r.seek(events_at);
    std::vector<Clip> out;
    const std::uint32_t n = r.u8();
    for (std::uint32_t k = 0; k < n; ++k) {
        const std::uint32_t info = r.u32();
        r.skip(2);
        const std::uint32_t kind = info & 0x1F;
        Clip c;
        if (kind == 1 || kind == 4) {
            r.skip(2);
            const std::uint32_t track = r.u16(), bank = r.u8(), loop = r.u8();
            r.skip(kind == 1 ? 4 : 4 + 4 + 2 + 16 + 2);
            c.waves.emplace_back(bank, track);
            c.loop = loop;
        } else if (kind == 3 || kind == 6) {
            r.skip(2);
            const std::uint32_t loop = r.u8();
            r.skip(kind == 3 ? 4 : 4 + 4 + 2 + 16 + 2);
            const std::uint32_t count = r.u16(), flags = r.u8();
            r.skip(5);
            std::vector<std::int64_t> weights;
            for (std::uint32_t j = 0; j < count; ++j) {
                const std::uint32_t track = r.u16(), bank = r.u8(), lo = r.u8(), hi = r.u8();
                c.waves.emplace_back(bank, track);
                weights.push_back(std::int64_t{hi} - lo);
            }
            c.variation = flags & 0xF;
            c.weights = weights;
            c.loop = loop;
        } else if (kind == 8) {
            r.skip(2 + 1 + 4 + 9);
            continue;
        } else {
            throw FormatError(kSoundBank, r.pos(), std::format("clip event {}", kind));
        }
        out.push_back(std::move(c));
    }
    r.seek(back);
    return out;
}

Cue sound(Cursor& r) {
    const std::uint32_t flags = r.u8();
    const bool complex = (flags & 1) != 0;
    Cue s;
    s.category = r.u16();
    r.skip(1 + 2 + 1 + 2);
    std::uint32_t clips = 0;
    if (complex) {
        clips = r.u8();
    } else {
        const std::uint32_t track = r.u16(), bank = r.u8();
        s.waves.emplace_back(bank, track);
    }
    if ((flags & 0xE) != 0) {
        const std::size_t start = r.pos();
        const std::uint32_t size = r.u16();
        r.seek(start + size);
    }
    if ((flags & 0x10) != 0) r.skip(7);
    if (complex) {
        std::vector<Clip> found;
        for (std::uint32_t k = 0; k < clips; ++k) {
            std::vector<Clip> more = clip_waves(r);
            found.insert(found.end(), more.begin(), more.end());
        }
        s.waves.clear();
        if (!found.empty()) {
            bool all_none = true;
            std::int64_t loop = found[0].loop;
            for (const Clip& f : found) {
                if (f.weights) all_none = false;
                loop = std::max(loop, f.loop);
                s.waves.insert(s.waves.end(), f.waves.begin(), f.waves.end());
            }
            if (!all_none) {
                std::vector<std::int64_t> w;
                for (const Clip& f : found) {
                    if (f.weights && !f.weights->empty()) w.insert(w.end(), f.weights->begin(), f.weights->end());
                    else w.insert(w.end(), f.waves.size(), 1);
                }
                s.weights = w;
            }
            s.variation = found[0].variation;
            s.loop = loop;
        }
    }
    return s;
}

}  // namespace

SoundBank read_sound_bank(ByteView data) {
    if (data.size() < 4) throw FormatError(kSoundBank, 0, "not an XACT sound bank");
    const std::string_view magic(reinterpret_cast<const char*>(data.data()), 4);
    bool big;
    if (magic == "SDBK") big = false;
    else if (magic == "KBDS") big = true;
    else throw FormatError(kSoundBank, 0, "not an XACT sound bank");
    Cursor r(data, big, kSoundBank);
    r.seek(4);
    r.skip(2 + 2 + 2 + 4 + 4 + 1);
    const std::uint32_t simple = r.u16(), complex = r.u16();
    r.skip(4);
    const std::uint32_t banks = r.u8();
    r.skip(2);
    const std::uint32_t names_len = r.u32();
    const std::uint32_t simple_at = r.u32(), complex_at = r.u32(), names_at = r.u32();
    r.skip(12);
    const std::uint32_t banks_at = r.u32();
    SoundBank out;
    for (std::uint32_t i = 0; i < banks; ++i) {
        const std::size_t at = std::size_t{banks_at} + 64 * i;
        if (at + 64 > data.size()) throw TruncatedError(kSoundBank, at, 64, data.size() > at ? data.size() - at : 0);
        std::string name;
        for (std::size_t k = 0; k < 64 && data[at + k] != 0; ++k) name.push_back(static_cast<char>(data[at + k]));
        out.wave_banks.push_back(std::move(name));
    }
    std::vector<std::string> names;
    if (names_at <= data.size()) {
        const std::size_t end = std::min<std::size_t>(data.size(), std::size_t{names_at} + names_len);
        std::string current;
        for (std::size_t i = names_at; i < end; ++i) {
            if (data[i] == 0) {
                names.push_back(std::move(current));
                current.clear();
            } else {
                current.push_back(static_cast<char>(data[i]));
            }
        }
        names.push_back(std::move(current));
    }
    const auto put = [&](const std::string& name, Cue cue) { out.cues.insert_or_assign(name, std::move(cue)); };
    r.seek(simple_at);
    for (std::uint32_t i = 0; i < simple; ++i) {
        r.skip(1);
        const std::uint32_t sound_at = r.u32();
        const std::size_t back = r.pos();
        r.seek(sound_at);
        const std::string name = i < names.size() ? names[i] : std::format("cue{}", i);
        try {
            put(name, sound(r));
        } catch (const FormatError&) {   // a sound whose layout is not known is listed without waves
            put(name, Cue{});
        }
        r.seek(back);
    }
    r.seek(complex_at);
    for (std::uint32_t k = 0; k < complex; ++k) {
        const std::string name = simple + k < names.size() ? names[simple + k] : std::format("cue{}", simple + k);
        const std::uint32_t flags = r.u8();
        if (((flags >> 2) & 1) != 0) {
            const std::uint32_t sound_at = r.u32();
            r.skip(4);
            const std::size_t back = r.pos();
            r.seek(sound_at);
            try {
                put(name, sound(r));
            } catch (const FormatError&) {
                put(name, Cue{});
            }
            r.seek(back);
        } else {
            const std::uint32_t table_at = r.u32();
            r.skip(4);
            const std::size_t back = r.pos();
            r.seek(table_at);
            const std::uint32_t count = r.u16(), vflags = r.u16();
            r.skip(1 + 2 + 1);
            const std::uint32_t kind = (vflags >> 3) & 7;
            Cue cue;
            std::vector<std::int64_t> weights;
            try {
                for (std::uint32_t j = 0; j < count; ++j) {
                    if (kind == 0 || kind == 4) {
                        const std::uint32_t track = r.u16(), bank = r.u8();
                        if (kind == 0) {
                            const std::uint32_t lo = r.u8(), hi = r.u8();
                            weights.push_back(std::int64_t{hi} - lo);
                        }
                        cue.waves.emplace_back(bank, track);
                    } else if (kind == 1 || kind == 3) {
                        const std::uint32_t sound_at = r.u32();
                        if (kind == 1) {
                            const std::uint32_t lo = r.u8(), hi = r.u8();
                            weights.push_back(std::int64_t{hi} - lo);
                        } else {
                            r.skip(12);
                        }
                        const std::size_t here = r.pos();
                        r.seek(sound_at);
                        const Cue s = sound(r);
                        cue.category = s.category;
                        cue.waves.insert(cue.waves.end(), s.waves.begin(), s.waves.end());
                        cue.loop = std::max(cue.loop, s.loop);
                        r.seek(here);
                    } else {
                        throw FormatError(kSoundBank, r.pos(), std::format("variation kind {}", kind));
                    }
                }
            } catch (const FormatError&) {   // what was read before the damage stays
            }
            cue.variation = vflags & 7;
            if (!weights.empty()) cue.weights = weights;
            put(name, std::move(cue));
            r.seek(back);
        }
        r.skip(1 + 2 + 2 + 1);
    }
    return out;
}

std::string cues_json(const SoundBank& bank) {
    Json doc = Json::object();
    doc["wave_banks"] = bank.wave_banks;
    Json cues = Json::object();
    for (const auto& [name, cue] : bank.cues) {
        Json c = Json::object();
        Json waves = Json::array();
        for (const auto& [b, t] : cue.waves) waves.push_back(Json::array({b, t}));
        c["waves"] = waves;
        c["variation"] = cue.variation;
        c["weights"] = cue.weights ? Json(*cue.weights) : Json(nullptr);
        c["loop"] = cue.loop;
        c["category"] = cue.category;
        cues[name] = c;
    }
    doc["cues"] = cues;
    return json::dump(doc, json::Style::Pretty);
}

}  // namespace cb::xna::xact
