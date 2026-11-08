// codebreaker/java/console/xact.cpp -- C++ port of codebreaker.java.console.xact (see xact.hpp).

#include "codebreaker/java/console/xact.hpp"

#include <stdlib.h>

#include "codebreaker/java/projects/minecraft/content.hpp"

namespace codebreaker::java::console::xact {

namespace content = codebreaker::java::projects::minecraft::content;

// the whole file `f` opened 'rb' holds
static py::bytes read_file_(const py::str& path) {
    py::file f = py::open(path, py::str("rb"));
    py::bytes b = f.read();
    f.close();
    return b;
}

// open(path, 'w', encoding='utf-8').write(text)
static void write_text_(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"));
    f.write(text);
    f.close();
}

// ---------------------------------------------------------------------------
int64_t _u32be(const py::bytes& b, int64_t o) {
    return py::unpack_from1<int64_t>(">I", b, o);
}

py::str _cue_name(const py::str& event) {
    return py::re::sub(py::str(R"RX([.\s]+)RX"), py::str("_"), event.strip());
}

const py::list<py::str> WAVEBANK_SEGMENTS = {"BANKDATA", "ENTRYMETADATA", "SEEKTABLES", "ENTRYNAMES", "ENTRYWAVEDATA"};
const py::dict<int64_t, py::str> _FORMAT_TAG = [] {
    py::dict<int64_t, py::str> d;
    d.set(0, py::str("PCM"));
    d.set(1, py::str("XMA"));
    d.set(2, py::str("ADPCM"));
    d.set(3, py::str("WMA"));
    return d;
}();

MiniFormat _decode_miniformat(int64_t dw) {
    int64_t tag = dw & 0x3;
    int64_t channels = (dw >> 2) & 0x7;
    int64_t rate = (dw >> 5) & 0x3FFFF;
    int64_t block = (dw >> 23) & 0xFF;
    int64_t bits16 = (dw >> 31) & 0x1;
    MiniFormat m;
    m.codec = _FORMAT_TAG.get(tag, py::percent("?%d", tag));
    m.channels = channels;
    m.sample_rate = rate;
    m.block_align = block;
    m.bits = bits16 ? 16 : 8;
    return m;
}

// bytes.split(b'\x00', 1)[0].decode('latin-1', 'replace')
static py::str cstring_(const py::bytes& raw) {
    int64_t z = raw.find(py::bytes(std::string(1, '\0')));
    py::bytes head = z < 0 ? raw : raw.slice(0, z);
    return head.decode("latin-1", "replace");
}

Xwb parse_xwb(const py::str& path) {
    py::bytes b = read_file_(path);
    py::str magic = b.slice(0, 4).decode("latin-1");
    if (!(magic == py::str("DNBW") || magic == py::str("WBND"))) {
        throw py::ValueError(py::percent("%s: not an XWB (magic %r)", py::os::path::basename(path), magic));
    }
    int64_t version = _u32be(b, 4);
    int64_t header_version = _u32be(b, 8);
    py::dict<py::str, std::tuple<int64_t, int64_t>> segments;
    int64_t o = 12;
    for (const py::str& name : WAVEBANK_SEGMENTS) {
        segments.set(name, std::make_tuple(_u32be(b, o), _u32be(b, o + 4)));
        o += 8;
    }
    int64_t bd_off = std::get<0>(segments.at(py::str("BANKDATA")));
    int64_t flags = _u32be(b, bd_off);
    int64_t entry_count = _u32be(b, bd_off + 4);
    py::str bank_name = cstring_(b.slice(bd_off + 8, bd_off + 8 + 64));
    int64_t meta_elem = _u32be(b, bd_off + 8 + 64);
    int64_t name_elem = _u32be(b, bd_off + 8 + 64 + 4);

    // entry metadata (non-compact 24-byte WAVEBANKENTRY: flagsDur(4), miniFormat(4), play(8), loop(8))
    py::list<WaveEntry> entries;
    int64_t meta_off = std::get<0>(segments.at(py::str("ENTRYMETADATA")));
    int64_t names_off = std::get<0>(segments.at(py::str("ENTRYNAMES")));
    for (int64_t i = 0; i < entry_count; ++i) {
        int64_t eo = meta_off + i * (meta_elem >= 24 ? meta_elem : 24);
        if (eo + 24 > b.size()) {
            break;
        }
        int64_t mini = _u32be(b, eo + 4);
        int64_t play_off = _u32be(b, eo + 8);
        int64_t play_len = _u32be(b, eo + 12);
        MiniFormat mf = _decode_miniformat(mini);
        WaveEntry ent;
        ent.index = i;
        ent.play_offset = play_off;
        ent.play_length = play_len;
        ent.codec = mf.codec;
        ent.channels = mf.channels;
        ent.sample_rate = mf.sample_rate;
        ent.block_align = mf.block_align;
        ent.bits = mf.bits;
        if (names_off != 0 && name_elem != 0) {   // per-entry name (if the bank kept names)
            int64_t no = names_off + i * name_elem;
            ent.name = cstring_(b.slice(no, no + name_elem));
        }
        entries.append(ent);
    }
    Xwb w;
    w.file = py::os::path::basename(path);
    w.magic = magic;
    w.version = version;
    w.header_version = header_version;
    w.name = bank_name;
    w.flags = flags;
    w.entry_count = entry_count;
    w.segments = segments;
    w.entries = entries;
    return w;
}

Xsb parse_xsb(const py::str& path) {
    py::bytes b = read_file_(path);
    py::str magic = b.slice(0, 4).decode("latin-1");
    if (!(magic == py::str("KBDS") || magic == py::str("SDBK"))) {
        throw py::ValueError(py::percent("%s: not an XSB (magic %r)", py::os::path::basename(path), magic));
    }
    // bank name + cue names are null-terminated printable-ASCII runs; harvest them in order.
    py::list<py::str> names;
    std::string cur;
    for (int64_t c : b.slice_from(16)) {
        if (32 <= c && c < 127) {
            cur.push_back(static_cast<char>(c));
        } else {
            if (cur.size() >= 3) {
                names.append(py::str(cur));
            }
            cur.clear();
        }
    }
    if (cur.size() >= 3) {
        names.append(py::str(cur));
    }
    py::str bank_name = names.size() > 0 ? names[0] : py::str("");
    // drop the bank-name repeat(s) at the head; the rest are cue names
    py::list<py::str> cues;
    for (const py::str& n : names.slice_from(1)) {
        if (n != bank_name) {
            cues.append(n);
        }
    }
    Xsb s;
    s.file = py::os::path::basename(path);
    s.magic = magic;
    s.name = bank_name;
    s.cue_names = cues;
    return s;
}

// ---------------------------------------------------------------------------
py::str dump(const py::str& path) {
    py::str ext = std::get<1>(py::os::path::splitext(path)).lower();
    if (ext == py::str(".xwb")) {
        Xwb w = parse_xwb(path);
        py::list<py::str> L = {py::percent("WaveBank \"%s\"  (%s v%d)  %d entries", w.name, w.magic, w.version, w.entry_count)};
        for (const auto& [name, seg] : w.segments.items()) {
            L.append(py::percent("  segment %-14s off=0x%X len=0x%X", name, std::get<0>(seg), std::get<1>(seg)));
        }
        L.append("  entries:");
        for (const WaveEntry& e : w.entries.slice(0, 64)) {
            L.append(py::percent("    [%3d] %-5s %dch %6dHz %2dbit  wave@0x%X len=%d%s", e.index, e.codec, e.channels,
                                 e.sample_rate, e.bits, e.play_offset, e.play_length,
                                 (e.name && !e.name->empty()) ? py::percent("  \"%s\"", *e.name) : py::str("")));
        }
        if (w.entry_count > 64) {
            L.append(py::percent("    … (%d more)", w.entry_count - 64));
        }
        return py::join(py::str("\n"), L);
    }
    if (ext == py::str(".xsb")) {
        Xsb s = parse_xsb(path);
        py::list<py::str> L = {py::percent("SoundBank \"%s\"  (%s)  %d cues", s.name, s.magic, py::len(s.cue_names))};
        for (const py::str& c : s.cue_names) {
            L.append(py::percent("  cue: %s", c));
        }
        return py::join(py::str("\n"), L);
    }
    throw py::ValueError(py::percent("unknown bank type: %s (expected .xwb / .xsb)", path));
}

// ===========================================================================
// BUILD side -- from the game's sound EVENTS to an XACT project / bank.
// ===========================================================================
py::dict<py::str, py::list<py::str>> needed_cues(const py::str& java_dir) {
    content::Assets a = content::scan_assets(java_dir);
    py::dict<py::str, py::list<py::str>> cues;
    for (const py::str& ev : a.sounds.keys()) {   // playSound("random.click") -> random_click
        cues.setdefault(_cue_name(ev), py::list<py::str>()).append(ev);
    }
    for (const py::str& fam : a.step_sounds.keys()) {   // StepSound("stone") -> step_stone / dig_stone families
        for (const char* prefix : {"step", "dig"}) {
            cues.setdefault(py::percent("%s_%s", py::str(prefix), fam), py::list<py::str>())
                .append(py::percent("%s.%s", py::str(prefix), fam));
        }
    }
    return cues;
}

CueCoverage cue_coverage(const py::str& java_dir, const py::str& xsb_path) {
    py::dict<py::str, py::list<py::str>> needed = needed_cues(java_dir);
    py::set<py::str> authored;   // drop " 2".." 4" variant suffix
    for (const py::str& c : parse_xsb(xsb_path).cue_names) {
        authored.add(py::re::sub(py::str(R"RX(\s+\d+$)RX"), py::str(""), c));
    }
    py::set<py::str> needset = py::set<py::str>::from(needed.keys());
    CueCoverage cov;
    cov.needed = needed;
    cov.authored = py::sorted(authored);
    cov.missing = py::sorted(needset - authored);
    cov.unused = py::sorted(authored - needset);
    return cov;
}

py::str render_coverage(const CueCoverage& cov) {
    py::list<py::str> L = {
        "# AUDIO_COVERAGE.md -- XACT cue coverage", "",
        "Cues the game plays vs cues authored in the bank. A **MISSING** cue means the game calls it "
        "but the bank has no such entry -> XACT GetCueIndex fails -> the sound silently no-ops.",
        "",
        py::percent("- needed: %d   authored: %d   **MISSING: %d**   unused-in-bank: %d", py::len(cov.needed),
                    py::len(cov.authored), py::len(cov.missing), py::len(cov.unused)),
        ""};
    if (!cov.missing.empty()) {
        L.extend(py::list<py::str>{"## MISSING cues -- author these into the bank (or the sound is silent)", "",
                                   "| Cue | Triggered by |", "|---|---|"});
        for (const py::str& c : cov.missing) {
            L.append(py::percent("| `%s` | %s |", c, py::join(py::str(", "), cov.needed.at(c).slice(0, 3))));
        }
        L.append("");
    }
    return py::join(py::str("\n"), L) + "\n";
}

// --- .xap project generation (Route A: the XDK XACTBLD compiles it to .xwb/.xsb) ----------------
py::list<py::str> build_xap(const std::optional<py::str>& java_dir, const py::str& out_path, const py::str& wave_dir,
                            const py::str& bank, const std::optional<py::set<py::str>>& only_cues,
                            const std::optional<py::list<py::str>>& cue_list,
                            const std::function<py::str(const py::str&)>& cue_namer_in, const py::str& sound_category,
                            bool streaming) {
    // FIXED BUG audio-cue-name-mismatch: the XACT cue name must MATCH what Audio.cpp toCue() emits, which is
    // the UNDERSCORE form ("random.click" -> "random_click", "step.grass" -> "step_grass"). The old default
    // dotted it ("random.click"), so GetCueIndex("random_click") failed ("cue not found in cue table") and
    // every SFX was silent. Keep the cue name identical to the wave/sound name (underscore) = toCue's output.
    std::function<py::str(const py::str&)> cue_namer = cue_namer_in;
    if (!cue_namer) {
        cue_namer = [](const py::str& c) { return c; };
    }
    py::list<py::str> cues;
    if (cue_list) {
        cues = py::sorted(*cue_list);
    } else {
        if (!java_dir) {
            throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
        }
        cues = py::sorted(py::list<py::str>(needed_cues(*java_dir).keys()));
        if (only_cues) {   // author ONLY these cues (the matched set)
            py::list<py::str> kept;
            for (const py::str& c : cues) {
                if (only_cues->contains(c)) {
                    kept.append(c);
                }
            }
            cues = kept;
        }
    }
    // FIXED BUG audio-xap-missing-wav: author ONLY cues whose .wav was actually PRODUCED. Some source oggs are
    // 169-byte placeholders (failed downloads) that ffmpeg can't decode -> no wav -> XACTBLD hard-aborts on
    // ALL banks ("wave file could not be found"). match_sources matches on ogg existence, not validity, so we
    // additionally gate on the real wav. (When the wav dir isn't populated yet, keep the list as-is.)
    py::str wave_abs = py::os::path::join(py::os::path::dirname(py::os::path::abspath(out_path)), wave_dir);
    if (py::os::path::isdir(wave_abs)) {
        py::set<py::str> present;
        for (const py::str& f : py::os::listdir(wave_abs)) {
            if (f.lower().endswith(py::str(".wav"))) {
                present.add(std::get<0>(py::os::path::splitext(f)));
            }
        }
        py::list<py::str> kept;
        for (const py::str& c : cues) {
            if (present.contains(c)) {
                kept.append(c);
            }
        }
        if (!kept.empty()) {
            cues = kept;
        }
    }
    py::str wb("WaveBank"), sb("SoundBank");   // internal bank names (linked by the Sound's Wave Entry)

    // FIXED BUG audio-xap-grammar: XACT3 .xap blocks require the FULL field set the XDK XACTBLD validates.
    // The old compact form (`Category { Name=X; Volume=0; }`, `Track { Play Wave Event { Wave Entry {..} } }`,
    // `Sound Entry { Name=X; }`) is REJECTED with "project file is missing one or more required values", so no
    // .xwb/.xsb were ever built. Grammar mirrored from the XDK sample Source\Samples\Audio\XactBasicSound.
    auto category = [](const py::str& name, const std::optional<py::str>& parent, int64_t bgm) {
        py::list<py::str> out = {"    Category", "    {", py::percent("        Name = %s;", name), "        Public = 1;",
                                 py::percent("        Background Music = %d;", bgm), "        Volume = 0;", "",
                                 "        Category Entry", "        {"};
        if (parent) {
            out.append(py::percent("            Name = %s;", *parent));
        }
        out.extend(py::list<py::str>{"        }", "", "        Instance Limit", "        {",
                                     "            Max Instances = 255;", "            Behavior = 0;", "",
                                     "            Crossfade", "            {", "                Fade In = 0;",
                                     "                Fade Out = 0;", "                Crossfade Type = 0;",
                                     "            }", "        }", "    }", ""});
        return out;
    };

    // bare output filenames: XACTBLD runs from Content\Audio with output path '.', so a stored
    // 'Content\Audio\X' would nest to Content\Audio\Content\Audio\X.
    py::list<py::str> L = {"Signature = XACT3;", "Version = 18;", "Content Version = 46;", "Release = March 2008;", "",
                           "Options", "{", "    Verbose Report = 0;", "    Generate C/C++ Headers = 1;", "}", "",
                           "Global Settings", "{", "    Xbox File = Global.xgs;", "    Header File = Global.h;",
                           "    Exclude Category Names = 0;", "    Exclude Variable Names = 0;", ""};
    L.extend(category(py::str("Global"), std::nullopt, 0));
    L.extend(category(py::str("Default"), py::str("Global"), 0));
    L.extend(category(py::str("Music"), py::str("Global"), 1));
    L.extend(py::list<py::str>{"}", ""});
    // Wave Bank -- PCM (no compression preset): XACTBLD packs the ffmpeg 16-bit .wav as-is. `streaming`
    // marks it for CreateStreamingWaveBank (music.xwb: large tracks are streamed off disk, not RAM).
    L.extend(py::list<py::str>{"Wave Bank", "{", py::percent("    Name = %s;", wb), py::percent("    Xbox File = %s.xwb;", bank),
                               "    Xbox Bank Path Edited = 1;"});
    if (streaming) {
        L.extend(py::list<py::str>{"    Streaming = 1;"});
    }
    L.extend(py::list<py::str>{"    Entry Names = 1;", "    Seek Tables = 1;", ""});
    for (const py::str& c : cues) {
        L.extend(py::list<py::str>{"    Wave", "    {", py::percent("        Name = %s;", c),
                                   py::percent("        File = %s\\%s.wav;", wave_dir, c), "    }", ""});
    }
    L.extend(py::list<py::str>{"}", ""});
    // Sound Bank
    L.extend(py::list<py::str>{"Sound Bank", "{", py::percent("    Name = %s;", sb), py::percent("    Xbox File = %s.xsb;", bank),
                               py::percent("    Xbox Header File = %s.h;", bank), ""});
    // FIXED BUG audio-wave-entry-index: the Sound's Wave Entry index MUST be the wave's real position in the
    // wave bank (waves are authored in `cues` order below, so cue i -> wave i). It was hardcoded 0, so every
    // cue played wave #0 -> every SFX was the SAME wrong sound. Set Entry Index = the enumerated wave index.
    for (const auto& [_wi, c] : py::enumerate(cues)) {
        L.extend(py::list<py::str>{
            "    Sound", "    {", py::percent("        Name = %s;", c), "        Volume = 0;", "        Pitch = 0;",
            "        Priority = 0;", "", "        Category Entry", "        {",
            py::percent("            Name = %s;", sound_category), "        }", "", "        Track", "        {",
            "            Volume = 0;", "            Use Filter = 0;", "", "            Play Wave Event", "            {",
            "                Break Loop = 0;", "                Use Speaker Position = 0;",
            "                Use Center Speaker = 1;", "                New Speaker Position On Loop = 1;",
            "                Speaker Position Angle = 0.000000;", "                Speaker Position Arc = 0.000000;", "",
            "                Event Header", "                {", "                    Timestamp = 0;",
            "                    Relative = 0;", "                    Random Recurrence = 0;",
            "                    Random Offset = 0;", "                }", "", "                Wave Entry",
            "                {", py::percent("                    Bank Name = %s;", wb), "                    Bank Index = 0;",
            py::percent("                    Entry Name = %s;", c), py::percent("                    Entry Index = %d;", _wi),
            "                    Weight = 0;", "                    Weight Min = 0;", "                }",
            "            }", "        }", "    }", ""});
    }
    // FIXED BUG audio-cue-sound-index: the Cue's Sound Entry index MUST be the sound's real position in the
    // sound bank (sounds are authored in `cues` order above, so cue i -> sound i). It was hardcoded 0, so
    // EVERY cue resolved to Sound #0 (the first sorted cue) -> every SFX played the SAME wrong sound (e.g. the
    // button click fired break_grass1). Fixing the Wave Entry index alone was not enough: the Cue->Sound hop
    // was still collapsed to 0. Set Index = the enumerated sound index (== the matching wave index).
    for (const auto& [_si, c] : py::enumerate(cues)) {   // cue keyed by the DOTTED event so toCue() maps back
        py::str ev = cue_namer(c);
        L.extend(py::list<py::str>{
            "    Cue", "    {", py::percent("        Name = %s;", ev), "", "        Variation", "        {",
            "            Variation Type = 3;", "            Variation Table Type = 1;",
            "            New Variation on Loop = 0;", "        }", "", "        Sound Entry", "        {",
            py::percent("            Name = %s;", c), py::percent("            Index = %d;", _si),
            "            Weight Min = 0;", "            Weight Max = 0;", "        }", "    }", ""});
    }
    L.extend(py::list<py::str>{"}", ""});
    write_text_(out_path, py::join(py::str("\n"), L));
    return cues;
}

py::str write_build_script(const py::str& out_dir, const py::str& bank, const py::str& bat_name) {
    py::str bat = py::str("@echo off\r\n"
                          "rem Build the XACT audio banks for Xbox 360 (needs the Xbox 360 SDK on PATH).\r\n"
                          "set XACTBLD=\"%%XEDK%%\\bin\\win32\\xactbld3.exe\"\r\n"
                          // FIXED BUG audio-xactbld-args: XACTBLD needs /XBOX360 (arch is MANDATORY) and the project as a
                          // POSITIONAL arg. The old `/X:%s.xap` was parsed as an invalid "/X: do-not-build" option (no
                          // project) -> XACTBLD built nothing yet exited 0, so no .xwb/.xsb were ever produced.
                          "%%XACTBLD%% /L /F /XBOX360 %s.xap .\r\n"
                          "echo Built %s.xwb / %s.xsb into Content\\Audio\\\r\n");
    bat = py::percent(bat, bank, bank, bank);
    py::str p = py::os::path::join(out_dir, bat_name);
    write_text_(p, bat);
    return p;
}

// --- direct PCM wave-bank writer (Route B: no XACTBLD; PCM only) --------------------------------
std::tuple<int64_t, int64_t, int64_t, py::bytes> _parse_wav(const py::str& path) {
    py::bytes b = read_file_(path);
    if (!(b.slice(0, 4) == py::bytes("RIFF") && b.slice(8, 12) == py::bytes("WAVE"))) {
        throw py::ValueError(py::percent("%s: not a RIFF/WAVE file", py::os::path::basename(path)));
    }
    int64_t ch = 0, rate = 0, bits = 0;
    py::bytes data;
    int64_t o = 12;
    while (o + 8 <= b.size()) {
        py::bytes cid = b.slice(o, o + 4);
        int64_t sz = py::unpack_from1<int64_t>("<I", b, o + 4);
        py::bytes body = b.slice(o + 8, o + 8 + sz);
        if (cid == py::bytes("fmt ")) {
            ch = py::unpack_from1<int64_t>("<H", body, 2);
            rate = py::unpack_from1<int64_t>("<I", body, 4);
            bits = py::unpack_from1<int64_t>("<H", body, 14);
        } else if (cid == py::bytes("data")) {
            data = body;
        }
        o += 8 + sz + (sz & 1);
    }
    return {ch, rate, bits, data};
}

int64_t _miniformat(int64_t ch, int64_t rate, int64_t bits) {
    int64_t block = ch * (bits / 8);   // (bits is never negative: floor and truncating division agree)
    return (0 & 0x3) | ((ch & 0x7) << 2) | ((rate & 0x3FFFF) << 5) | ((block & 0xFF) << 23) |
           ((static_cast<int64_t>(bits == 16 ? 1 : 0) & 0x1) << 31);   // (int64_t: bit 31 must not turn negative)
}

py::list<PcmEntry> build_pcm_xwb(const py::list<py::str>& wav_files, const py::str& out_xwb, const py::str& bank) {
    py::list<PcmEntry> entries;
    py::bytearray wavedata;
    for (const py::str& path : wav_files) {
        auto [ch, rate, bits, data] = _parse_wav(path);
        int64_t pad = (2048 - wavedata.size() % 2048) % 2048;   // 360 wave alignment
        wavedata += py::bytes::zeros(pad);
        PcmEntry e;
        e.name = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
        e.mini = _miniformat(ch, rate, bits);
        e.off = wavedata.size();
        e.len = data.size();
        entries.append(e);
        wavedata += data;
    }
    int64_t n = entries.size();
    int64_t ENTRY = 24;
    // BANKDATA
    py::bytes bankdata = py::pack(">II", int64_t(0), n) + py::str(bank).encode("latin-1").slice(0, 64).ljust(64, py::bytes(std::string(1, '\0'))) +
                         py::pack(">III", ENTRY, int64_t(0), int64_t(2048));
    int64_t meta_off = 52 + bankdata.size();
    py::list<py::bytes> metas;
    for (const PcmEntry& e : entries) {
        metas.append(py::pack(">IIIIII", int64_t(4), e.mini, e.off, e.len, int64_t(0), int64_t(0)));
    }
    py::bytes meta = py::bytes().join(metas);
    int64_t wave_off = (meta_off + meta.size() + 2047) & ~int64_t(2047);
    py::bytes header = py::bytes("DNBW") + py::pack(">II", int64_t(46), int64_t(44));
    py::list<py::bytes> segs;
    for (const auto& [o, l] : std::vector<std::tuple<int64_t, int64_t>>{{52, bankdata.size()}, {meta_off, meta.size()}, {0, 0}, {0, 0}, {wave_off, wavedata.size()}}) {
        segs.append(py::pack(">II", o, l));
    }
    header = header + py::bytes().join(segs);
    py::bytearray out(header + bankdata + meta);
    int64_t gap = wave_off - out.size();
    out += py::bytes::zeros(gap > 0 ? gap : 0);
    out += wavedata.view();
    py::file f = py::open(out_xwb, py::str("wb"));
    f.write(out.view());
    f.close();
    return entries;
}

// --- source matching: cue -> original .ogg file(s) ---------------------------------------------
const py::list<py::str> _OGG_ROOTS = {"newsound", "sound", "newmusic", "music", "streaming", "resources"};

const py::re::Pattern& _TRAIL_NUM() {
    static const py::re::Pattern p = py::re::compile(py::str(R"RX((\d+)$)RX"));
    return p;
}

py::set<py::str> _ogg_keys(const py::str& rel_path) {
    py::str p = rel_path.lower().endswith(py::str(".ogg")) ? rel_path.slice(0, rel_path.size() - 4) : rel_path;
    py::list<py::str> parts = py::split(p, py::str("/"));
    if (!parts.empty() && _OGG_ROOTS.contains(parts[0])) {
        parts = parts.slice_from(1);
    }
    if (parts.empty()) {
        return py::set<py::str>();
    }
    parts[-1] = _TRAIL_NUM().sub(py::str(""), parts[-1]);   // grass1 -> grass, say3 -> say
    py::str base = py::join(py::str("/"), parts);
    py::set<py::str> keys;   // door_close  AND  door/close
    for (const py::str& k : {base, base.replace(py::str("_"), py::str("/"))}) {
        if (!k.empty()) {
            keys.add(k);
        }
    }
    return keys;
}

py::dict<py::str, py::list<py::str>> scan_ogg_sources(const py::str& resources_dir) {
    py::dict<py::str, py::set<py::str>> index;
    for (const auto& [dp, _dirs, files] : py::os::walk(resources_dir)) {
        for (const py::str& f : files) {
            if (!f.lower().endswith(py::str(".ogg"))) {
                continue;
            }
            py::str rel = py::os::path::relpath(py::os::path::join(dp, f), resources_dir).replace(py::os::sep, py::str("/"));
            for (const py::str& k : py::sorted(_ogg_keys(rel))) {
                index.setdefault(k, py::set<py::str>()).add(rel);
            }
        }
    }
    py::dict<py::str, py::list<py::str>> out;
    for (const auto& [k, v] : index.items()) {
        out.set(k, py::sorted(v));
    }
    return out;
}

Match match_sources(const py::str& java_dir, const py::str& resources_dir) {
    py::dict<py::str, py::list<py::str>> index = scan_ogg_sources(resources_dir);
    py::dict<py::str, py::list<py::str>> needed = needed_cues(java_dir);
    Match m;
    for (const py::str& cue : py::sorted(py::list<py::str>(needed.keys()))) {
        const py::list<py::str>& events = needed.at(cue);
        py::list<py::str> files;
        py::str used_event;
        for (const py::str& ev : events) {
            py::str key = ev.replace(py::str("."), py::str("/"));
            std::optional<py::list<py::str>> cand;
            if (index.contains(key)) {
                cand = index.at(key);
            }
            bool have = cand && !cand->empty();
            if (!have && key.startswith(py::str("dig/"))) {   // dig.grass -> step.grass (same samples)
                py::str alt = py::str("step/") + key.slice_from(py::str("dig/").size());
                cand = index.contains(alt) ? std::optional<py::list<py::str>>(index.at(alt)) : std::nullopt;
                have = cand && !cand->empty();
            }
            if (have) {
                files = *cand;
                used_event = ev;
                break;
            }
        }
        if (!files.empty()) {
            m.matched.set(cue, Matched{used_event, files});
        } else {
            m.missing.set(cue, events);
        }
    }
    m.index_keys = py::len(index);
    return m;
}

py::str write_ffmpeg_script(const Match& match, const py::str& out_dir, const py::str& resources_dir,
                            const py::str& wav_subdir, const py::str& bat_name) {
    py::str wav_dir = py::os::path::join(out_dir, wav_subdir);
    if (!py::os::path::isdir(wav_dir)) {
        py::os::makedirs(wav_dir);
    }
    py::list<py::str> lines = {"@echo off",
                               "rem Decode the matched Minecraft .ogg sources to 16-bit PCM .wav for the XACT banks.",
                               "rem Needs ffmpeg on PATH. Source: " + resources_dir,
                               "setlocal",
                               py::percent("set SRC=%s", resources_dir),
                               py::percent("set OUT=%s", wav_subdir),
                               ""};
    for (const py::str& cue : py::sorted(py::list<py::str>(match.matched.keys()))) {
        py::str src = match.matched.at(cue).files[0].replace(py::str("/"), py::str("\\"));
        lines.append(py::percent("ffmpeg -y -i \"%%SRC%%\\%s\" -ar 44100 -ac 1 -sample_fmt s16 \"%%OUT%%\\%s.wav\"", src, cue));
    }
    lines.extend(py::list<py::str>{"", py::percent("echo Decoded %d cues to %%OUT%%\\", py::len(match.matched)), "endlocal", ""});
    py::str p = py::os::path::join(out_dir, bat_name);
    write_text_(p, py::join(py::str("\r\n"), lines));
    return p;
}

std::optional<py::str> _find_resources_dir(const py::str& java_dir) {
    py::str d = py::os::path::abspath(java_dir);
    for (int64_t i = 0; i < 5; ++i) {
        for (const py::str& cand : {py::os::path::join(d, py::str("resources")), py::os::path::join(d, py::str("game"), py::str("resources"))}) {
            if (py::os::path::isdir(py::os::path::join(cand, py::str("newsound"))) ||
                py::os::path::isdir(py::os::path::join(cand, py::str("sound")))) {
                return cand;
            }
        }
        py::str parent = py::os::path::dirname(d);
        if (parent == d) {
            break;
        }
        d = parent;
    }
    return std::nullopt;
}

void _clean_audio_intermediates(const py::str& out_dir) {
    py::list<py::str> entries;
    try {
        entries = py::os::listdir(out_dir);
    } catch (const py::OSError&) {
        return;
    }
    bool any_xwb = false;
    for (const py::str& f : entries) {
        if (f.endswith(py::str(".xwb"))) {
            any_xwb = true;
        }
    }
    if (!any_xwb) {
        return;   // banks didn't build -> leave everything for a retry
    }
    py::shutil::rmtree(py::os::path::join(out_dir, py::str("wav")), true);
    for (const py::str& f : entries) {
        if (f.endswith(std::make_tuple(".bat", ".xap", ".h", ".xgs", ".md", ".txt"))) {
            try {
                py::os::remove(py::os::path::join(out_dir, f));
            } catch (const py::OSError&) {
            }
        }
    }
}

py::list<py::str> build_music(const py::str& out_dir, const py::str& resources_dir, const py::list<py::str>& tracks) {
    py::str music_src = py::os::path::join(resources_dir, py::str("music"));
    py::list<py::str> found;
    for (const py::str& t : tracks) {
        if (py::os::path::isfile(py::os::path::join(music_src, t + py::str(".ogg")))) {
            found.append(t);
        }
    }
    if (found.empty()) {
        return py::list<py::str>();
    }
    // synthetic match dict (files relative to resources_dir, like match_sources output) -> music_convert.bat
    Match match;
    for (const py::str& t : found) {
        match.matched.set(t, Matched{t, py::list<py::str>{py::percent("music/%s.ogg", t)}});
    }
    write_ffmpeg_script(match, out_dir, resources_dir, py::str("wav"), py::str("music_convert.bat"));
    build_xap(std::nullopt, py::os::path::join(out_dir, py::str("music.xap")), py::str("wav"), py::str("music"), std::nullopt,
              found, [](const py::str& c) { return c; }, py::str("Music"), true);
    write_build_script(out_dir, py::str("music"), py::str("music_banks.bat"));
    return found;
}

std::tuple<py::str, py::list<py::str>> build(const py::str& java_dir, const py::str& out_dir,
                                             const std::optional<py::str>& resources_dir, bool autorun,
                                             const std::optional<py::str>& sdk_root,
                                             const std::function<void(const py::str&)>& log) {
    if (!py::os::path::isdir(out_dir)) {
        py::os::makedirs(out_dir);
    }
    py::str xap = py::os::path::join(out_dir, py::str("sounds.xap"));
    // FIXED BUG audio-xap-missing-wav: match sources FIRST, then author ONLY the cues that have a source wav.
    // Authoring every needed cue made XACTBLD fail hard on `wav\<cue>.wav could not be found` for the few
    // cues with no source ogg (random_bow, random_door_*) -> the whole bank build aborted, no banks. Unmatched
    // cues are simply absent -> GetCueIndex fails at runtime -> XaPlayCue no-ops (that one sound is silent).
    std::optional<py::str> resources = resources_dir;
    if (!resources || resources->empty()) {
        resources = _find_resources_dir(java_dir);
    }
    std::optional<py::set<py::str>> avail;
    py::list<py::str> music;
    if (resources && !resources->empty()) {   // auto-emit the ogg->wav decode script if we can find the sources
        Match m = match_sources(java_dir, *resources);
        avail = py::set<py::str>::from(m.matched.keys());
        write_ffmpeg_script(m, out_dir, *resources);
        music = build_music(out_dir, *resources);   // SECOND bank: streaming music.xwb/.xsb (calm1/2/3)
    }
    write_build_script(out_dir);
    {
        py::file f = py::open(py::os::path::join(out_dir, py::str("AUDIO_COVERAGE.md")), py::str("w"), py::str("utf-8"));
        py::dict<py::str, py::list<py::str>> cov_needed = needed_cues(java_dir);
        py::list<py::str> items;
        for (const py::str& c : py::sorted(py::list<py::str>(cov_needed.keys()))) {
            items.append(py::percent("- `%s.wav`  <- %s", c, py::join(py::str(", "), cov_needed.at(c).slice(0, 2))));
        }
        f.write(py::percent("# AUDIO cues the game needs (%d)\n\nAuthor a .wav per cue under `wav/`, then run "
                            "`build_banks.bat` (XACTBLD).\n\n",
                            py::len(cov_needed)) +
                py::join(py::str("\n"), items) + py::str("\n"));
        f.close();
    }
    py::list<py::str> cues;
    if (autorun) {
        // Decode oggs -> wavs FIRST, author the xaps from the wavs that ACTUALLY landed (build_xap gates on
        // present wavs), THEN compile the banks. Ordering matters: some source oggs are placeholders that
        // ffmpeg drops, so the cue list must be finalized AFTER conversion, else XACTBLD aborts on all banks.
        run_build(out_dir, sdk_root, log, py::list<py::str>{"convert_audio.bat", "music_convert.bat"});
        // FIXED BUG audio-sfx-streaming: the SFX bank MUST be built streaming (Streaming=1). XactAudio::
        // loadSFXBank uses CreateStreamingWaveBank (no in-memory fallback, unlike loadMusicBank), so a
        // non-streaming sounds.xwb triggers XACT "Attempting to stream an in-memory wavebank" -> DbgBreakPoint.
        cues = build_xap(java_dir, xap, py::str("wav"), py::str("sounds"), avail, std::nullopt, nullptr, py::str("Default"), true);
        if (!music.empty()) {
            build_music(out_dir, *resources);   // re-author music.xap now that calmN.wav exist
        }
        run_build(out_dir, sdk_root, log, py::list<py::str>{"build_banks.bat", "music_banks.bat"});
        _clean_audio_intermediates(out_dir);   // ship only the .xwb/.xsb banks, not wav/ + scripts
    } else {
        cues = build_xap(java_dir, xap, py::str("wav"), py::str("sounds"), avail, std::nullopt, nullptr, py::str("Default"), true);
    }
    return {xap, cues};
}

// sets one variable of this process's environment for a scope and puts it back (Python hands the child a copy
// of os.environ with XEDK added; the child here is started with the process's own environment)
namespace {
struct ScopedEnv {
    std::wstring key;
    std::optional<std::wstring> old;
    bool active = false;
    ScopedEnv(const wchar_t* k, const py::str& value) : key(k) {
        const wchar_t* cur = _wgetenv(k);
        if (cur) {
            old = std::wstring(cur);
        }
        _wputenv_s(k, py::subprocess::to_wide(value).c_str());
        active = true;
    }
    ~ScopedEnv() {
        if (active) {
            _wputenv_s(key.c_str(), old ? old->c_str() : L"");
        }
    }
};
}  // namespace

std::tuple<bool, py::list<py::str>> run_build(const py::str& out_dir, const std::optional<py::str>& sdk_root,
                                              const std::function<void(const py::str&)>& log,
                                              const std::optional<py::list<py::str>>& only) {
    std::function<void(const py::str&)> _log = log ? log : [](const py::str&) {};
    py::list<py::str> lines;
    std::optional<py::str> xedk = py::os::environ_::get(py::str("XEDK"));
    std::optional<ScopedEnv> scoped;
    if (sdk_root && !sdk_root->empty() && !(xedk && !xedk->empty())) {
        scoped.emplace(L"XEDK", *sdk_root);
        xedk = *sdk_root;
    }

    auto emit = [&](const py::str& m) {
        lines.append(m);
        _log(m);
    };

    auto _run = [&](const py::str& bat, const std::function<bool()>& tool_ok, const py::str& tool_hint) -> bool {
        py::str p = py::os::path::join(out_dir, bat);
        if (!py::os::path::isfile(p)) {
            emit(py::percent("[audio] skip %s (not generated)", bat));
            return true;
        }
        if (!tool_ok()) {
            emit(py::percent("[audio] SKIP %s -- %s not found; the .bat is written, run it later.", bat, tool_hint));
            return true;
        }
        try {
            // FIXED BUG audio-autorun-cwd: pass the FULL path to the .bat. `cmd /c convert_audio.bat`
            // (bare name) fails with "not recognized" when cwd-search is disabled, so the banks never built.
            py::subprocess::RunOptions o;
            o.cwd = out_dir;
            o.text = false;
            o.stdout_ = py::subprocess::Stream::Capture;
            o.stderr_ = py::subprocess::Stream::Stdout;
            py::subprocess::CompletedProcess r = py::subprocess::run(py::list<py::str>{"cmd", "/c", p}, o);
            py::str txt = r.stdout_bytes ? r.stdout_bytes->decode("utf-8", "replace") : py::str("");
            emit(py::percent("[audio] %s -> exit %d", bat, r.returncode));
            if (r.returncode != 0) {
                emit(txt.slice(-800, txt.size()));
            }
            return r.returncode == 0;
        } catch (const py::Exception& e) {   // noqa: BLE001 -- audio must never abort the convert
            emit(py::percent("[audio] %s failed: %s", bat, py::str(e.what())));
            return false;
        }
    };

    auto have_xactbld = [&]() {
        return xedk && !xedk->empty() &&
               py::os::path::isfile(py::os::path::join(*xedk, py::str("bin"), py::str("win32"), py::str("xactbld3.exe")));
    };
    auto have_ffmpeg = []() { return py::shutil::which(py::str("ffmpeg")).has_value(); };
    // convert bats (ffmpeg) before bank bats (XACTBLD); a missing .bat is skipped gracefully by _run.
    struct Step {
        const char* name;
        std::function<bool()> chk;
        const char* hint;
    };
    std::vector<Step> bats = {{"convert_audio.bat", have_ffmpeg, "ffmpeg (add it to PATH)"},
                              {"music_convert.bat", have_ffmpeg, "ffmpeg (add it to PATH)"},
                              {"build_banks.bat", have_xactbld, "XACTBLD (set %XEDK% / install the Xbox 360 SDK)"},
                              {"music_banks.bat", have_xactbld, "XACTBLD (set %XEDK% / install the Xbox 360 SDK)"}};
    bool ok = true;
    for (const Step& s : bats) {
        if (!only || only->contains(py::str(s.name))) {
            ok = _run(py::str(s.name), s.chk, py::str(s.hint)) && ok;
        }
    }
    return {ok, lines};
}

}  // namespace codebreaker::java::console::xact
