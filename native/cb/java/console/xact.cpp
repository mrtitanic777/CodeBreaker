#include "cb/java/console/xact.hpp"

#include <algorithm>
#include <format>
#include <limits>
#include <system_error>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/crlf.hpp"
#include "cb/java/console/tree.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::xact {

namespace stdfs = std::filesystem;

namespace {

bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_space(char c) { return c == ' ' || (c >= '\t' && c <= '\r'); }

// Bytes [offset, offset+length) of the data, cut short where the data ends (a name read at the end of a damaged
// bank gives what is there). The clamp is deliberate; see the callers.
ByteView clamped(ByteView data, std::uint64_t offset, std::uint64_t length) {
    if (offset >= data.size()) return {};
    return data.subspan(static_cast<std::size_t>(offset), static_cast<std::size_t>(std::min<std::uint64_t>(length, data.size() - offset)));
}

std::uint32_t u32be_at(ByteView data, std::uint64_t offset) {
    io::ByteReader reader(data, "xact bank");
    reader.seek(offset);
    return reader.u32_be();
}

// The text of the bytes up to the first NUL, Latin-1 decoded.
std::string c_string(ByteView bytes) {
    std::size_t end = 0;
    while (end < bytes.size() && bytes[end] != 0) ++end;
    return text::from_latin1(bytes.first(end));
}

std::string magic_of(ByteView data) { return text::from_latin1(clamped(data, 0, 4)); }

}  // namespace

// ---- reading banks ----------------------------------------------------------------------------------------------

std::string_view codec_name(Codec codec) {
    switch (codec) {
        case Codec::Pcm: return "PCM";
        case Codec::Xma: return "XMA";
        case Codec::Adpcm: return "ADPCM";
        case Codec::Wma: return "WMA";
    }
    return "?";
}

MiniFormat decode_miniformat(std::uint32_t dword) {
    MiniFormat m;
    m.codec = static_cast<Codec>(dword & 0x3);
    m.channels = (dword >> 2) & 0x7;
    m.sample_rate = (dword >> 5) & 0x3FFFF;
    m.block_align = (dword >> 23) & 0xFF;
    m.bits = ((dword >> 31) & 1) != 0 ? 16 : 8;
    return m;
}

std::uint32_t pack_pcm_miniformat(std::uint32_t channels, std::uint32_t rate, std::uint32_t bits) {
    const std::uint32_t block = channels * (bits / 8);
    return ((channels & 0x7) << 2) | ((rate & 0x3FFFF) << 5) | ((block & 0xFF) << 23) | ((bits == 16 ? 1u : 0u) << 31);
}

std::string_view segment_name(SegmentId id) {
    switch (id) {
        case SegmentId::BankData: return "BANKDATA";
        case SegmentId::EntryMetadata: return "ENTRYMETADATA";
        case SegmentId::SeekTables: return "SEEKTABLES";
        case SegmentId::EntryNames: return "ENTRYNAMES";
        case SegmentId::EntryWaveData: return "ENTRYWAVEDATA";
    }
    return "?";
}

Xwb parse_xwb(ByteView data, std::string file_name) {
    Xwb bank;
    bank.file = std::move(file_name);
    bank.magic = magic_of(data);
    if (bank.magic != "DNBW" && bank.magic != "WBND")
        throw FormatError("xwb", 0, std::format("{}: not an XWB (magic '{}')", bank.file, bank.magic));
    io::ByteReader header(data, "xwb");
    header.seek(4);
    bank.version = header.u32_be();
    bank.header_version = header.u32_be();
    for (Segment& s : bank.segments) {
        s.offset = header.u32_be();
        s.length = header.u32_be();
    }

    const std::uint64_t bank_data = bank.segments[0].offset;
    bank.flags = u32be_at(data, bank_data);
    bank.entry_count = u32be_at(data, bank_data + 4);
    bank.name = c_string(clamped(data, bank_data + 8, 64));
    const std::uint32_t meta_elem = u32be_at(data, bank_data + 8 + 64);
    const std::uint32_t name_elem = u32be_at(data, bank_data + 8 + 64 + 4);

    // The non-compact 24-byte WAVEBANKENTRY: flagsAndDuration(4), miniFormat(4), play(8), loop(8).
    constexpr std::uint64_t kEntryBytes = 24;
    const std::uint64_t meta_off = bank.segments[1].offset;
    const std::uint64_t names_off = bank.segments[3].offset;
    const std::uint64_t stride = meta_elem >= kEntryBytes ? meta_elem : kEntryBytes;
    for (std::uint32_t i = 0; i < bank.entry_count; ++i) {
        const std::uint64_t entry_at = meta_off + i * stride;
        if (entry_at + kEntryBytes > data.size()) break;   // a damaged bank lists the entries it still has
        WaveEntry e;
        e.index = i;
        e.format = decode_miniformat(u32be_at(data, entry_at + 4));
        e.play_offset = u32be_at(data, entry_at + 8);
        e.play_length = u32be_at(data, entry_at + 12);
        if (names_off != 0 && name_elem != 0)   // per-entry names, if the bank kept them
            e.name = c_string(clamped(data, names_off + i * static_cast<std::uint64_t>(name_elem), name_elem));
        bank.entries.push_back(std::move(e));
    }
    return bank;
}

Xwb read_xwb(const stdfs::path& path) { return parse_xwb(fs::read_file(path), fs::to_text(path.filename())); }

Xsb parse_xsb(ByteView data, std::string file_name) {
    Xsb bank;
    bank.file = std::move(file_name);
    bank.magic = magic_of(data);
    if (bank.magic != "KBDS" && bank.magic != "SDBK")
        throw FormatError("xsb", 0, std::format("{}: not an XSB (magic '{}')", bank.file, bank.magic));
    // The bank name and the cue names are null-terminated printable-ASCII runs; harvest them in order.
    std::vector<std::string> names;
    std::string current;
    auto flush = [&] {
        if (current.size() >= 3) names.push_back(current);
        current.clear();
    };
    for (std::size_t i = 16; i < data.size(); ++i) {
        const std::uint8_t c = data[i];
        if (c >= 32 && c < 127) current += static_cast<char>(c);
        else flush();
    }
    flush();
    if (!names.empty()) bank.name = names.front();
    for (std::size_t i = 1; i < names.size(); ++i)   // the bank-name repeats at the head are not cues
        if (names[i] != bank.name) bank.cue_names.push_back(names[i]);
    return bank;
}

Xsb read_xsb(const stdfs::path& path) { return parse_xsb(fs::read_file(path), fs::to_text(path.filename())); }

std::string dump(const stdfs::path& path) {
    const std::string ext = text::lower(fs::to_text(path.extension()));
    if (ext == ".xwb") {
        const Xwb w = read_xwb(path);
        std::string out = std::format("WaveBank \"{}\"  ({} v{})  {} entries", w.name, w.magic, w.version, w.entry_count);
        for (std::size_t i = 0; i < kSegmentCount; ++i)
            out += std::format("\n  segment {:<14} off=0x{:X} len=0x{:X}", segment_name(static_cast<SegmentId>(i)),
                               w.segments[i].offset, w.segments[i].length);
        out += "\n  entries:";
        constexpr std::size_t kShown = 64;
        for (std::size_t i = 0; i < w.entries.size() && i < kShown; ++i) {
            const WaveEntry& e = w.entries[i];
            out += std::format("\n    [{:>3}] {:<5} {}ch {:>6}Hz {:>2}bit  wave@0x{:X} len={}{}", e.index,
                               codec_name(e.format.codec), e.format.channels, e.format.sample_rate, e.format.bits,
                               e.play_offset, e.play_length,
                               e.name && !e.name->empty() ? std::format("  \"{}\"", *e.name) : std::string());
        }
        if (w.entry_count > kShown) out += std::format("\n    \xE2\x80\xA6 ({} more)", w.entry_count - kShown);
        return out;
    }
    if (ext == ".xsb") {
        const Xsb s = read_xsb(path);
        std::string out = std::format("SoundBank \"{}\"  ({})  {} cues", s.name, s.magic, s.cue_names.size());
        for (const std::string& c : s.cue_names) out += "\n  cue: " + c;
        return out;
    }
    throw UsageError(std::format("unknown bank type: {} (expected .xwb / .xsb)", fs::to_text(path)));
}

// ---- what the game needs ----------------------------------------------------------------------------------------

std::string cue_name(std::string_view event) {
    std::string out;
    bool in_run = false;
    for (char c : text::trim(event)) {
        if (c == '.' || is_space(c)) {
            if (!in_run) out += '_';
            in_run = true;
        } else {
            out += c;
            in_run = false;
        }
    }
    return out;
}

namespace {

void add_event(Needed& needed, const std::string& cue, const std::string& event) {
    std::vector<std::string>& events = needed[cue];
    if (std::find(events.begin(), events.end(), event) == events.end()) events.push_back(event);
}

}  // namespace

Needed needed_cues(const SoundDemand& demand) {
    Needed needed;
    // Sorted and without repeats, so the same game gives the same table however its source was scanned.
    const std::set<std::string> sounds(demand.sounds.begin(), demand.sounds.end());
    const std::set<std::string> families(demand.step_sounds.begin(), demand.step_sounds.end());
    for (const std::string& ev : sounds) add_event(needed, cue_name(ev), ev);                 // playSound("random.click") -> random_click
    for (const std::string& family : families)                                                // StepSound("stone") -> step_stone, dig_stone
        for (const char* prefix : {"step", "dig"}) add_event(needed, std::format("{}_{}", prefix, family), std::format("{}.{}", prefix, family));
    return needed;
}

namespace {

// The cue name without a variant suffix: white space followed by digits at the end (" 2" .. " 4") is dropped.
std::string without_variant_suffix(const std::string& cue) {
    std::size_t end = cue.size();
    std::size_t digits = end;
    while (digits > 0 && is_digit(cue[digits - 1])) --digits;
    if (digits == end || digits == 0 || !is_space(cue[digits - 1])) return cue;
    std::size_t gap = digits;
    while (gap > 0 && is_space(cue[gap - 1])) --gap;
    return cue.substr(0, gap);
}

}  // namespace

CueCoverage cue_coverage(const SoundDemand& demand, const Xsb& bank) {
    CueCoverage cov;
    cov.needed = needed_cues(demand);
    std::set<std::string> authored;
    for (const std::string& c : bank.cue_names) authored.insert(without_variant_suffix(c));
    cov.authored.assign(authored.begin(), authored.end());
    for (const auto& [cue, events] : cov.needed)
        if (!authored.contains(cue)) cov.missing.push_back(cue);
    for (const std::string& cue : authored)
        if (!cov.needed.contains(cue)) cov.unused.push_back(cue);
    return cov;
}

std::string render_coverage(const CueCoverage& cov) {
    std::string out =
        "# AUDIO_COVERAGE.md -- XACT cue coverage\n\n"
        "Cues the game plays vs cues authored in the bank. A **MISSING** cue means the game calls it but the bank "
        "has no such entry -> XACT GetCueIndex fails -> the sound silently no-ops.\n\n";
    out += std::format("- needed: {}   authored: {}   **MISSING: {}**   unused-in-bank: {}\n\n", cov.needed.size(),
                       cov.authored.size(), cov.missing.size(), cov.unused.size());
    if (!cov.missing.empty()) {
        out += "## MISSING cues -- author these into the bank (or the sound is silent)\n\n| Cue | Triggered by |\n|---|---|\n";
        for (const std::string& c : cov.missing) {
            const std::vector<std::string>& events = cov.needed.at(c);
            const std::vector<std::string> shown(events.begin(), events.begin() + std::min<std::size_t>(3, events.size()));
            out += std::format("| `{}` | {} |\n", c, text::join(shown, ", "));
        }
        out += "\n";
    }
    return out;
}

// ---- authoring --------------------------------------------------------------------------------------------------

namespace {

using Lines = std::vector<std::string>;

void append(Lines& to, std::initializer_list<std::string> more) { to.insert(to.end(), more.begin(), more.end()); }

// An XACT3 Category block. The .xap grammar mirrors the XDK sample Source\Samples\Audio\XactBasicSound: XACTBLD
// REJECTS a compact form that leaves out any of the fields ("project file is missing one or more required values").
void category(Lines& out, const std::string& name, const std::string& parent, int background_music) {
    append(out, {"    Category", "    {", "        Name = " + name + ";", "        Public = 1;",
                 std::format("        Background Music = {};", background_music), "        Volume = 0;", "",
                 "        Category Entry", "        {"});
    if (!parent.empty()) out.push_back("            Name = " + parent + ";");
    append(out, {"        }", "", "        Instance Limit", "        {", "            Max Instances = 255;",
                 "            Behavior = 0;", "", "            Crossfade", "            {", "                Fade In = 0;",
                 "                Fade Out = 0;", "                Crossfade Type = 0;", "            }", "        }", "    }", ""});
}

std::string joined_lines(const Lines& lines) {
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i != 0) out += '\n';
        out += lines[i];
    }
    return out;
}

constexpr std::string_view kWaveBankName = "WaveBank";    // internal bank names, linked by the Sound's Wave Entry
constexpr std::string_view kSoundBankName = "SoundBank";

}  // namespace

std::vector<std::string> write_xap(const stdfs::path& out_path, std::vector<std::string> cues, const XapOptions& options) {
    std::sort(cues.begin(), cues.end());
    cues.erase(std::unique(cues.begin(), cues.end()), cues.end());

    // Author ONLY cues whose .wav was actually PRODUCED: some source oggs are 169-byte placeholders (failed downloads)
    // that ffmpeg cannot decode, so no wav exists, and XACTBLD hard-aborts on ALL banks ("wave file could not be
    // found"). Matching sources checks that an ogg exists, not that it is valid. When the wav folder is not
    // populated yet, the list stands as it is.
    std::error_code ec;
    const stdfs::path wave_abs = stdfs::absolute(out_path, ec).parent_path() / options.wave_dir;
    if (stdfs::is_directory(wave_abs, ec)) {
        std::set<std::string> present;
        for (stdfs::directory_iterator it(wave_abs, ec), end; !ec && it != end; it.increment(ec))
            if (text::iequals(fs::to_text(it->path().extension()), ".wav")) present.insert(fs::to_text(it->path().stem()));
        std::vector<std::string> kept;
        for (const std::string& c : cues)
            if (present.contains(c)) kept.push_back(c);
        if (!kept.empty()) cues = std::move(kept);
    }

    const std::string wb(kWaveBankName), sb(kSoundBankName);
    Lines L = {"Signature = XACT3;", "Version = 18;", "Content Version = 46;", "Release = March 2008;", "",
               "Options", "{", "    Verbose Report = 0;", "    Generate C/C++ Headers = 1;", "}", "",
               "Global Settings", "{", "    Xbox File = Global.xgs;", "    Header File = Global.h;",
               "    Exclude Category Names = 0;", "    Exclude Variable Names = 0;", ""};
    category(L, "Global", "", 0);
    category(L, "Default", "Global", 0);
    category(L, "Music", "Global", 1);
    append(L, {"}", ""});

    // Wave Bank -- PCM (no compression preset): XACTBLD packs the ffmpeg 16-bit .wav as it is. `streaming` marks it for
    // CreateStreamingWaveBank (music.xwb: large tracks are streamed off disk, not held in RAM).
    append(L, {"Wave Bank", "{", "    Name = " + wb + ";", "    Xbox File = " + options.bank + ".xwb;", "    Xbox Bank Path Edited = 1;"});
    if (options.streaming) L.push_back("    Streaming = 1;");
    append(L, {"    Entry Names = 1;", "    Seek Tables = 1;", ""});
    for (const std::string& c : cues)
        append(L, {"    Wave", "    {", "        Name = " + c + ";", "        File = " + options.wave_dir + "\\" + c + ".wav;", "    }", ""});
    append(L, {"}", ""});

    // Sound Bank. A Sound's Wave Entry index MUST be the wave's real position in the wave bank (waves are authored in
    // `cues` order, so cue i is wave i); it was once hardcoded 0, so every effect played wave #0, the SAME wrong sound.
    append(L, {"Sound Bank", "{", "    Name = " + sb + ";", "    Xbox File = " + options.bank + ".xsb;",
               "    Xbox Header File = " + options.bank + ".h;", ""});
    for (std::size_t i = 0; i < cues.size(); ++i) {
        const std::string& c = cues[i];
        append(L, {"    Sound", "    {", "        Name = " + c + ";", "        Volume = 0;", "        Pitch = 0;",
                   "        Priority = 0;", "", "        Category Entry", "        {",
                   "            Name = " + options.sound_category + ";", "        }", "", "        Track", "        {",
                   "            Volume = 0;", "            Use Filter = 0;", "", "            Play Wave Event",
                   "            {", "                Break Loop = 0;", "                Use Speaker Position = 0;",
                   "                Use Center Speaker = 1;", "                New Speaker Position On Loop = 1;",
                   "                Speaker Position Angle = 0.000000;", "                Speaker Position Arc = 0.000000;", "",
                   "                Event Header", "                {", "                    Timestamp = 0;",
                   "                    Relative = 0;", "                    Random Recurrence = 0;",
                   "                    Random Offset = 0;", "                }", "", "                Wave Entry",
                   "                {", "                    Bank Name = " + wb + ";", "                    Bank Index = 0;",
                   "                    Entry Name = " + c + ";", std::format("                    Entry Index = {};", i),
                   "                    Weight = 0;", "                    Weight Min = 0;", "                }",
                   "            }", "        }", "    }", ""});
    }
    // A Cue's Sound Entry index likewise MUST be the sound's real position in the sound bank (cue i is sound i); left
    // at 0, every cue resolved to the first sorted cue's sound and the button click fired break_grass1.
    for (std::size_t i = 0; i < cues.size(); ++i) {
        const std::string& c = cues[i];
        const std::string cue = options.cue_namer ? options.cue_namer(c) : c;
        append(L, {"    Cue", "    {", "        Name = " + cue + ";", "", "        Variation", "        {",
                   "            Variation Type = 3;", "            Variation Table Type = 1;",
                   "            New Variation on Loop = 0;", "        }", "", "        Sound Entry", "        {",
                   "            Name = " + c + ";", std::format("            Index = {};", i), "            Weight Min = 0;",
                   "            Weight Max = 0;", "        }", "    }", ""});
    }
    append(L, {"}", ""});
    textutil::write_text(out_path, to_crlf(joined_lines(L)));
    return cues;
}

std::filesystem::path write_build_script(const stdfs::path& out_dir, std::string_view bank, std::string_view bat_name) {
    // XACTBLD needs /XBOX360 (the architecture is MANDATORY) and the project as a POSITIONAL argument: the old
    // `/X:<bank>.xap` was read as an invalid "/X: do-not-build" option with no project, so XACTBLD built nothing yet
    // exited 0 and no .xwb/.xsb was ever produced.
    const std::string bat = std::format(
        "@echo off\n"
        "rem Build the XACT audio banks for Xbox 360 (needs the Xbox 360 SDK on PATH).\n"
        "set XACTBLD=\"%XEDK%\\bin\\win32\\xactbld3.exe\"\n"
        "%XACTBLD% /L /F /XBOX360 {0}.xap .\n"
        "echo Built {0}.xwb / {0}.xsb into Content\\Audio\\\n",
        bank);
    const stdfs::path path = out_dir / bat_name;
    textutil::write_text(path, to_crlf(bat));
    return path;
}

Wav parse_wav(ByteView file, std::string_view name) {
    constexpr std::string_view kReader = "wav";
    if (file.size() < 12 || as_text(file.first(4)) != "RIFF" || as_text(file.subspan(8, 4)) != "WAVE")
        throw FormatError(kReader, 0, std::format("{}: not a RIFF/WAVE file", name));
    Wav wav;
    bool have_format = false;
    std::uint64_t at = 12;
    while (at + 8 <= file.size()) {
        io::ByteReader chunk(file, kReader);
        chunk.seek(at);
        const std::string_view id = as_text(chunk.bytes(4));
        const std::uint32_t size = chunk.u32();
        const ByteView body = clamped(file, at + 8, size);   // a last chunk may claim more than the file holds
        if (id == "fmt ") {
            io::ByteReader fmt(body, kReader);
            const std::uint16_t tag = fmt.u16();
            wav.channels = fmt.u16();
            wav.rate = fmt.u32();
            fmt.skip(6);   // byte rate and block align
            wav.bits = fmt.u16();
            // 1 = PCM; 0xFFFE = WAVE_FORMAT_EXTENSIBLE, which ffmpeg writes for more than 16 bits or two channels
            if (tag != 1 && tag != 0xFFFE) throw UnsupportedError(kReader, 0, std::format("{}: not PCM (format tag {})", name, tag));
            have_format = true;
        } else if (id == "data") {
            wav.data.assign(body.begin(), body.end());   // the last data chunk wins
        }
        at += 8 + static_cast<std::uint64_t>(size) + (size & 1);
    }
    if (!have_format) throw FormatError(kReader, 0, std::format("{}: no fmt chunk", name));
    return wav;
}

std::vector<PcmEntry> build_pcm_xwb(const std::vector<stdfs::path>& wav_files, const stdfs::path& out_xwb, std::string_view bank) {
    constexpr std::uint32_t kAlignment = 2048;        // the 360's wave alignment
    constexpr std::uint32_t kEntryBytes = 24;
    constexpr std::uint32_t kHeaderBytes = 52;        // magic, versions and the five segment descriptors
    constexpr std::size_t kBankNameBytes = 64;
    Bytes wave_data;
    std::vector<PcmEntry> entries;
    for (const stdfs::path& path : wav_files) {
        const Wav wav = parse_wav(fs::read_file(path), fs::to_text(path.filename()));
        wave_data.resize((wave_data.size() + kAlignment - 1) / kAlignment * kAlignment);
        entries.push_back({fs::to_text(path.stem()), pack_pcm_miniformat(wav.channels, wav.rate, wav.bits),
                           static_cast<std::uint32_t>(wave_data.size()), static_cast<std::uint32_t>(wav.data.size())});
        cb::append(wave_data, wav.data);
        if (wave_data.size() > std::numeric_limits<std::uint32_t>::max())
            throw UnsupportedError("xwb", 0, "wave data past 4 GB does not fit an XWB");
    }
    const std::size_t count = entries.size();

    io::ByteWriter bank_data;
    bank_data.u32_be(0);                                          // flags
    bank_data.u32_be(static_cast<std::uint32_t>(count));
    Bytes name_field = text::to_latin1(bank);
    name_field.resize(kBankNameBytes);                            // cut or zero-padded to 64 bytes
    bank_data.bytes(name_field);
    bank_data.u32_be(kEntryBytes);
    bank_data.u32_be(0);                                          // no entry names
    bank_data.u32_be(kAlignment);

    const std::uint32_t meta_off = kHeaderBytes + static_cast<std::uint32_t>(bank_data.size());
    io::ByteWriter meta;
    for (const PcmEntry& e : entries) {
        meta.u32_be(4);                                           // flagsAndDuration
        meta.u32_be(e.mini);
        meta.u32_be(e.offset);
        meta.u32_be(e.length);
        meta.u32_be(0);                                           // no loop region
        meta.u32_be(0);
    }
    const std::uint32_t wave_off =
        (meta_off + static_cast<std::uint32_t>(meta.size()) + kAlignment - 1) / kAlignment * kAlignment;

    io::ByteWriter out;
    out.bytes(as_bytes("DNBW"));
    out.u32_be(46);                                               // version
    out.u32_be(44);                                               // header version
    const Segment segments[kSegmentCount] = {{kHeaderBytes, static_cast<std::uint32_t>(bank_data.size())},
                                             {meta_off, static_cast<std::uint32_t>(meta.size())},
                                             {0, 0},
                                             {0, 0},
                                             {wave_off, static_cast<std::uint32_t>(wave_data.size())}};
    for (const Segment& s : segments) {
        out.u32_be(s.offset);
        out.u32_be(s.length);
    }
    out.bytes(bank_data.data());
    out.bytes(meta.data());
    out.zeros(wave_off - out.size());
    out.bytes(wave_data);
    fs::write_file(out_xwb, out.data());
    return entries;
}

// ---- source matching --------------------------------------------------------------------------------------------

std::vector<std::string> ogg_keys(std::string_view rel_path) {
    static constexpr std::array<std::string_view, 6> kRoots = {"newsound", "sound", "newmusic",
                                                                "music",    "streaming", "resources"};
    std::string_view p = rel_path;
    if (text::iends_with(p, ".ogg")) p.remove_suffix(4);
    std::vector<std::string_view> parts = text::split(p, '/');
    if (!parts.empty() && std::find(kRoots.begin(), kRoots.end(), parts.front()) != kRoots.end()) parts.erase(parts.begin());
    if (parts.empty()) return {};
    std::string leaf(parts.back());                               // grass1 -> grass, say3 -> say
    while (!leaf.empty() && is_digit(leaf.back())) leaf.pop_back();
    std::string base;
    for (std::size_t i = 0; i + 1 < parts.size(); ++i) base.append(parts[i]).append("/");
    base += leaf;
    std::set<std::string> keys = {base, text::replace_all(base, "_", "/")};   // door_close AND door/close
    keys.erase("");
    return {keys.begin(), keys.end()};
}

std::map<std::string, std::vector<std::string>> scan_ogg_sources(const stdfs::path& resources_dir) {
    std::map<std::string, std::set<std::string>> index;
    tree::walk(resources_dir, {}, [&](const tree::Listing& l) {
        const std::string rel_dir = fs::to_text(l.dir.lexically_relative(resources_dir));
        for (const std::string& f : l.files) {
            if (!text::iends_with(f, ".ogg")) continue;
            const std::string rel = rel_dir == "." ? f : rel_dir + "/" + f;
            for (const std::string& key : ogg_keys(rel)) index[key].insert(rel);
        }
        return true;
    });
    std::map<std::string, std::vector<std::string>> out;
    for (auto& [key, files] : index) out[key].assign(files.begin(), files.end());
    return out;
}

Match match_sources(const SoundDemand& demand, const stdfs::path& resources_dir) {
    const std::map<std::string, std::vector<std::string>> index = scan_ogg_sources(resources_dir);
    Match match;
    match.index_keys = index.size();
    for (const auto& [cue, events] : needed_cues(demand)) {
        const std::vector<std::string>* files = nullptr;
        const std::string* used_event = nullptr;
        for (const std::string& ev : events) {
            const std::string key = text::replace_all(ev, ".", "/");
            auto at = index.find(key);
            // dig.grass -> step.grass: the same samples
            if (at == index.end() && key.starts_with("dig/")) at = index.find("step/" + key.substr(4));
            if (at != index.end() && !at->second.empty()) {
                files = &at->second;
                used_event = &ev;
                break;
            }
        }
        if (files) match.matched[cue] = {*used_event, *files};
        else match.missing[cue] = events;
    }
    return match;
}

std::filesystem::path write_ffmpeg_script(const Match& match, const stdfs::path& out_dir, const stdfs::path& resources_dir,
                                          std::string_view wav_subdir, std::string_view bat_name) {
    fs::make_dirs(out_dir / wav_subdir);
    Lines lines = {"@echo off", "rem Decode the matched Minecraft .ogg sources to 16-bit PCM .wav for the XACT banks.",
                   "rem Needs ffmpeg on PATH. Source: " + resources_dir.u8string(), "setlocal",
                   "set SRC=" + resources_dir.u8string(), "set OUT=" + std::string(wav_subdir), ""};
    for (const auto& [cue, source] : match.matched) {
        // the first variant of the cue; the bank builder handles the others separately
        const std::string src = text::replace_all(source.files.front(), "/", "\\");
        lines.push_back(
            std::format("ffmpeg -y -i \"%SRC%\\{}\" -ar 44100 -ac 1 -sample_fmt s16 \"%OUT%\\{}.wav\"", src, cue));
    }
    append(lines, {"", std::format("echo Decoded {} cues to %OUT%\\", match.matched.size()), "endlocal", ""});
    const stdfs::path path = out_dir / bat_name;
    textutil::write_text(path, to_crlf(joined_lines(lines)));
    return path;
}

std::optional<stdfs::path> find_resources_dir(const stdfs::path& java_dir) {
    std::error_code ec;
    stdfs::path dir = stdfs::absolute(java_dir, ec).lexically_normal();
    if (!dir.has_filename() && dir != dir.root_path()) dir = dir.parent_path();
    for (int up = 0; up < 5; ++up) {
        for (const stdfs::path& cand : {dir / "resources", dir / "game" / "resources"})
            if (stdfs::is_directory(cand / "newsound", ec) || stdfs::is_directory(cand / "sound", ec)) return cand;
        stdfs::path parent = dir.parent_path();
        if (parent == dir || parent.empty()) break;
        dir = std::move(parent);
    }
    return std::nullopt;
}

void clean_audio_intermediates(const stdfs::path& out_dir) {
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (stdfs::directory_iterator it(out_dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec)) files.push_back(it->path());
    const bool banks_built =
        std::any_of(files.begin(), files.end(), [](const stdfs::path& f) { return f.extension() == ".xwb"; });
    if (!banks_built) return;   // the banks did not build: leave everything for a retry
    stdfs::remove_all(out_dir / "wav", ec);
    for (const stdfs::path& f : files) {
        const std::string ext = fs::to_text(f.extension());
        if (ext == ".bat" || ext == ".xap" || ext == ".h" || ext == ".xgs" || ext == ".md" || ext == ".txt")
            stdfs::remove(f, ec);
    }
}

std::vector<std::string> build_music(const stdfs::path& out_dir, const stdfs::path& resources_dir,
                                     const std::vector<std::string>& tracks) {
    std::vector<std::string> found;
    std::error_code ec;
    for (const std::string& t : tracks)
        if (stdfs::is_regular_file(resources_dir / "music" / (t + ".ogg"), ec)) found.push_back(t);
    if (found.empty()) return {};
    // the sources, as match_sources would give them (files relative to the resources folder) -> music_convert.bat
    Match match;
    for (const std::string& t : found) match.matched[t] = {t, {"music/" + t + ".ogg"}};
    write_ffmpeg_script(match, out_dir, resources_dir, "wav", "music_convert.bat");
    XapOptions xap;
    xap.bank = "music";
    xap.sound_category = "Music";
    xap.streaming = true;
    write_xap(out_dir / "music.xap", found, xap);
    write_build_script(out_dir, "music", "music_banks.bat");
    return found;
}

// ---- running the build ------------------------------------------------------------------------------------------

namespace {

// Is there an executable of that name on PATH (with any of PATHEXT's extensions)?
bool on_path(std::string_view tool) {
    const std::string path_var = fs::get_env("PATH").value_or("");
    const std::string ext_var = fs::get_env("PATHEXT").value_or(".COM;.EXE;.BAT;.CMD");
    std::error_code ec;
    for (std::string_view dir : text::split(path_var, ';')) {
        if (dir.empty()) continue;
        for (std::string_view ext : text::split(ext_var, ';'))
            if (stdfs::is_regular_file(fs::from_text(dir) / (std::string(tool) + std::string(ext)), ec)) return true;
    }
    return false;
}

}  // namespace

RunResult run_build(const stdfs::path& out_dir, const std::optional<stdfs::path>& sdk_root, const LogFn& log,
                    const std::optional<std::vector<std::string>>& only) {
    RunResult result;
    auto emit = [&](const std::string& message) {
        result.lines.push_back(message);
        if (log) log(message);
    };

    process::ProcessSpec base;
    std::string xedk = fs::get_env("XEDK").value_or("");
    if (sdk_root && xedk.empty()) {
        xedk = fs::to_text(*sdk_root);
        base.env["XEDK"] = xedk;   // only the child sees it
    }
    auto have_xactbld = [&] {
        std::error_code ec;
        return !xedk.empty() && stdfs::is_regular_file(fs::from_text(xedk) / "bin" / "win32" / "xactbld3.exe", ec);
    };
    auto have_ffmpeg = [] { return on_path("ffmpeg"); };

    struct Step {
        std::string_view bat;
        std::function<bool()> tool_ok;
        std::string_view hint;
    };
    // the convert scripts (ffmpeg) before the bank scripts (XACTBLD); a script that was not written is skipped
    const std::vector<Step> steps = {
        {"convert_audio.bat", have_ffmpeg, "ffmpeg (add it to PATH)"},
        {"music_convert.bat", have_ffmpeg, "ffmpeg (add it to PATH)"},
        {"build_banks.bat", have_xactbld, "XACTBLD (set %XEDK% / install the Xbox 360 SDK)"},
        {"music_banks.bat", have_xactbld, "XACTBLD (set %XEDK% / install the Xbox 360 SDK)"},
    };
    constexpr std::chrono::minutes kTimeout(30);

    for (const Step& step : steps) {
        if (only && std::find(only->begin(), only->end(), step.bat) == only->end()) continue;
        const stdfs::path script = out_dir / step.bat;
        std::error_code ec;
        if (!stdfs::is_regular_file(script, ec)) {
            emit(std::format("[audio] skip {} (not generated)", step.bat));
            continue;
        }
        if (!step.tool_ok()) {
            emit(std::format("[audio] SKIP {} -- {} not found; the .bat is written, run it later.", step.bat, step.hint));
            continue;
        }
        // Audio must never abort the conversion: a script that cannot even be started is one failed step. The FULL path
        // of the script is passed: `cmd /c convert_audio.bat` (a bare name) fails with "not recognized" when the current
        // folder is not searched, and the banks never built.
        process::ProcessSpec spec = base;
        stdfs::path native_script = script;   // cmd reads a forward slash as the start of a switch
        native_script.make_preferred();
        spec.argv = {"cmd", "/c", native_script.u8string()};
        spec.cwd = out_dir;
        spec.err = process::Capture::Merge;
        spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(kTimeout);
        try {
            const process::ProcessResult r = process::run(spec);
            if (r.timed_out) {
                emit(std::format("[audio] {} failed: timed out after {} minutes", step.bat, kTimeout.count()));
                result.ok = false;
                continue;
            }
            emit(std::format("[audio] {} -> exit {}", step.bat, r.exit_code));
            if (r.exit_code != 0) {
                const std::string output = text::sanitize_utf8(as_text(r.out));
                emit(output.size() > 800 ? output.substr(output.size() - 800) : output);   // the end of the log
                result.ok = false;
            }
        } catch (const ToolError& e) {
            emit(std::format("[audio] {} failed: {}", step.bat, e.what()));
            result.ok = false;
        }
    }
    return result;
}

BuildResult build(const SoundDemand& demand, const stdfs::path& out_dir, const BuildOptions& options) {
    fs::make_dirs(out_dir);
    BuildResult result;
    result.xap = out_dir / "sounds.xap";
    // Match the sources FIRST, then author ONLY the cues that have a source wav: authoring every needed cue made
    // XACTBLD fail hard on `wav\<cue>.wav could not be found` for the few cues with no source ogg (random_bow, the
    // door sounds), so the whole bank build aborted and no banks were made. An unmatched cue is simply absent;
    // GetCueIndex fails at runtime and that one sound is silent.
    std::optional<stdfs::path> resources = options.resources_dir;
    if (!resources && options.java_dir) resources = find_resources_dir(*options.java_dir);
    std::optional<std::set<std::string>> available;
    std::vector<std::string> music;
    if (resources) {   // the ogg -> wav decode script, if the sources can be found
        const Match m = match_sources(demand, *resources);
        available.emplace();
        for (const auto& [cue, source] : m.matched) available->insert(cue);
        write_ffmpeg_script(m, out_dir, *resources);
        music = build_music(out_dir, *resources);   // the SECOND bank: streaming music.xwb/.xsb (calm1/2/3)
    }
    write_build_script(out_dir);

    const Needed needed = needed_cues(demand);
    std::string coverage = std::format(
        "# AUDIO cues the game needs ({})\n\nAuthor a .wav per cue under `wav/`, then run `build_banks.bat` (XACTBLD).\n\n",
        needed.size());
    for (const auto& [cue, events] : needed) {
        const std::vector<std::string> shown(events.begin(), events.begin() + std::min<std::size_t>(2, events.size()));
        coverage += std::format("- `{}.wav`  <- {}\n", cue, text::join(shown, ", "));
    }
    textutil::write_text(out_dir / "AUDIO_COVERAGE.md", coverage);

    auto sfx_cues = [&] {
        std::vector<std::string> cues;
        for (const auto& [cue, events] : needed)
            if (!available || available->contains(cue)) cues.push_back(cue);
        return cues;
    };
    XapOptions sfx;
    sfx.streaming = true;
    if (options.autorun) {
        // Decode the oggs to wavs FIRST, author the xaps from the wavs that ACTUALLY landed (write_xap gates on the
        // wavs present), THEN compile the banks. The order matters: some source oggs are placeholders that ffmpeg
        // drops, so the cue list must be final after conversion or XACTBLD aborts on all banks.
        run_build(out_dir, options.sdk_root, options.log, std::vector<std::string>{"convert_audio.bat", "music_convert.bat"});
        result.cues = write_xap(result.xap, sfx_cues(), sfx);
        if (!music.empty()) build_music(out_dir, *resources);   // re-author music.xap now that calmN.wav exist
        run_build(out_dir, options.sdk_root, options.log, std::vector<std::string>{"build_banks.bat", "music_banks.bat"});
        clean_audio_intermediates(out_dir);   // ship only the .xwb/.xsb banks, not wav/ and the scripts
    } else {
        result.cues = write_xap(result.xap, sfx_cues(), sfx);
    }
    return result;
}

}  // namespace cb::java::console::xact
