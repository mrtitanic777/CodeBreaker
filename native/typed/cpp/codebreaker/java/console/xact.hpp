// codebreaker/java/console/xact.hpp -- C++ port of codebreaker.java.console.xact:
// XACT3 wave-bank (.xwb) / sound-bank (.xsb) tool for the Xbox 360 audio pipeline.
//
// 360 audio is XACT3 or XAudio2; the port uses XACT3 (Audio.reference routes SoundManager -> XACT cues).
// This tool reads/decompiles the banks (Phase 1, here) and -- Phase 2 -- builds them from Content.md's
// sound list + the .ogg/.wav sources. It is grounded in the banks the user authored by hand for the
// Alpha port (...\MinecraftAlpha\...\Content\Audio\sounds.xwb+.xsb, music.*).
//
// Format (Xbox 360 = BIG-ENDIAN; XACT stores the magic byte-reversed vs the PC LE 'WBND'/'SDBK'):
//   XWB  magic 'DNBW'  header{version, headerVersion, 5x(offset,length) segments:
//        BANKDATA/ENTRYMETADATA/SEEKTABLES/ENTRYNAMES/ENTRYWAVEDATA}
//        BANKDATA{flags, entryCount, name[64], metaElemSize, nameElemSize, alignment, compactFormat,...}
//        ENTRYMETADATA[entryCount] of WAVEBANKENTRY{ flagsAndDuration, miniFormat(packed DWORD),
//        PlayRegion(offset,length), LoopRegion(...) }  -- miniFormat packs tag/channels/rate/align/bits.
//   XSB  magic 'KBDS' ... trailing table of null-terminated CUE NAMES (the sound-event names).
//
// Notes on the port:
//  - The module's `from codebreaker.java.projects.minecraft import content` is the typed port
//    codebreaker::java::projects::minecraft::content (scan_assets).
//  - run_build runs the .bat files through py::subprocess::run (stdout and stderr in one pipe). Python's
//    Popen.communicate(timeout=1800) has no counterpart there: the C++ waits for the child without a limit.
//    Python's env copy with XEDK added is a temporary change of the process environment here (restored after).
//  - Where Python builds a `set` and iterates it (scan_ogg_sources' keys) the C++ iterates in sorted order;
//    Python's order there is the hash order of the strings and nothing reads it.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::java::console::xact {

// ---------------------------------------------------------------------------
int64_t _u32be(const py::bytes& b, int64_t o);

// Java sound EVENT ('step.grass', 'random.click') -> XACT cue base name ('step_grass',
// 'random_click'). Dots/spaces -> underscores; matches the toCue() convention in Audio.reference.
py::str _cue_name(const py::str& event);

extern const py::list<py::str> WAVEBANK_SEGMENTS;
extern const py::dict<int64_t, py::str> _FORMAT_TAG;

// What _decode_miniformat returns (a dict with these fixed keys).
struct MiniFormat {
    py::str codec;
    int64_t channels = 0;
    int64_t sample_rate = 0;
    int64_t block_align = 0;
    int64_t bits = 0;
};

// WAVEBANKMINIWAVEFORMAT packed DWORD -> (tag, channels, samplesPerSec, blockAlign, bitsPerSample).
// Layout: tag[0:2] channels[2:5] rate[5:23] blockAlign[23:31] bits[31:32].
MiniFormat _decode_miniformat(int64_t dw);

// One entry of a wave bank (the dict parse_xwb builds per entry: the keys in this order; 'name' only when
// the bank kept names).
struct WaveEntry {
    int64_t index = 0;
    int64_t play_offset = 0;
    int64_t play_length = 0;
    py::str codec;
    int64_t channels = 0;
    int64_t sample_rate = 0;
    int64_t block_align = 0;
    int64_t bits = 0;
    std::optional<py::str> name;
};

// What parse_xwb returns: {file, magic, version, header_version, name, flags, entry_count, segments, entries[]}.
struct Xwb {
    py::str file;
    py::str magic;
    int64_t version = 0;
    int64_t header_version = 0;
    py::str name;
    int64_t flags = 0;
    int64_t entry_count = 0;
    py::dict<py::str, std::tuple<int64_t, int64_t>> segments;   // segment name -> (offset, length), in file order
    py::list<WaveEntry> entries;
};

// Decompile an .xwb wave bank.
Xwb parse_xwb(const py::str& path);

// What parse_xsb returns: {file, magic, name, cue_names[]}.
struct Xsb {
    py::str file;
    py::str magic;
    py::str name;
    py::list<py::str> cue_names;
};

// Decompile an .xsb sound bank. Cue names are the sound EVENTS the game triggers (step_grass, cave
// chimes, ...). Extracts the trailing null-terminated name table.
Xsb parse_xsb(const py::str& path);

// ---------------------------------------------------------------------------
// Human-readable decompile of an .xwb or .xsb.
py::str dump(const py::str& path);

// ===========================================================================
// BUILD side -- from the game's sound EVENTS to an XACT project / bank.
// ===========================================================================

// The XACT cues the converted game will request, derived from the sound EVENTS + step-sound
// families it references (via content.scan_assets). Returns {cue_name: [source_event(s)]}. This is
// the demand side; diff it against a bank's authored cues to find the silent-audio gaps.
py::dict<py::str, py::list<py::str>> needed_cues(const py::str& java_dir);

// What cue_coverage returns: the (needed, authored, missing, unused) dict.
struct CueCoverage {
    py::dict<py::str, py::list<py::str>> needed;
    py::list<py::str> authored;
    py::list<py::str> missing;
    py::list<py::str> unused;
};

// Diff the cues the game NEEDS against those AUTHORED in an existing sound bank. `missing` = cues the
// game plays that the bank lacks -> GetCueIndex fails -> the sound silently no-ops (the class
// Audio.reference warns about).
CueCoverage cue_coverage(const py::str& java_dir, const py::str& xsb_path);

py::str render_coverage(const CueCoverage& cov);

// --- .xap project generation (Route A: the XDK XACTBLD compiles it to .xwb/.xsb) ----------------

// Generate an XACT3 project (.xap) that AUTHORS the cues, each referencing a .wav in `wave_dir`.
// XACTBLD compiles it to <bank>.xwb + <bank>.xsb. Returns the cue list.
// only_cues: author ONLY these (the matched set).  cue_list: use this EXPLICIT cue list instead of the
// game's SFX cues (for the music bank).  cue_namer: cue Name mapper (default identity; music passes identity
// so calm1/2/3 stay bare to match XactAudio g_MusicCues).  sound_category: XACT category the Sounds go in
// ('Default' SFX / 'Music').  streaming: mark the Wave Bank streaming (music.xwb).
// (java_dir=None is only valid together with a cue_list: the game's cues need the Java directory.)
py::list<py::str> build_xap(const std::optional<py::str>& java_dir, const py::str& out_path,
                            const py::str& wave_dir = py::str("wav"), const py::str& bank = py::str("sounds"),
                            const std::optional<py::set<py::str>>& only_cues = std::nullopt,
                            const std::optional<py::list<py::str>>& cue_list = std::nullopt,
                            const std::function<py::str(const py::str&)>& cue_namer = nullptr,
                            const py::str& sound_category = py::str("Default"), bool streaming = false);

// Emit <bat_name>: run the XDK XACTBLD to compile <bank>.xap -> .xwb/.xsb for the 360.
py::str write_build_script(const py::str& out_dir, const py::str& bank = py::str("sounds"),
                           const py::str& bat_name = py::str("build_banks.bat"));

// --- direct PCM wave-bank writer (Route B: no XACTBLD; PCM only) --------------------------------

// Minimal RIFF/WAVE PCM reader -> (channels, rate, bits, data_bytes).
std::tuple<int64_t, int64_t, int64_t, py::bytes> _parse_wav(const py::str& path);

// Pack PCM params into a WAVEBANKMINIWAVEFORMAT DWORD (tag=0 PCM).
int64_t _miniformat(int64_t ch, int64_t rate, int64_t bits);

// One entry build_pcm_xwb wrote: {name, mini, off, len}.
struct PcmEntry {
    py::str name;
    int64_t mini = 0;
    int64_t off = 0;
    int64_t len = 0;
};

// Write a REAL Xbox-360 (big-endian) .xwb wave bank from PCM .wav files -- no XACTBLD needed.
// PCM only (XWMA/XMA compression needs the XDK encoder); good for small SFX / bring-up. Returns entries.
py::list<PcmEntry> build_pcm_xwb(const py::list<py::str>& wav_files, const py::str& out_xwb,
                                 const py::str& bank = py::str("sounds"));

// --- source matching: cue -> original .ogg file(s) ---------------------------------------------
// Resource-tree roots to strip so a file path lines up with an event name. Minecraft keeps the same
// sound under several roots across versions (`sound/`, `newsound/`, `music/`, `newmusic/`).
extern const py::list<py::str> _OGG_ROOTS;
// re.compile(r'(\d+)$'), built on first use rather than at program start
const py::re::Pattern& _TRAIL_NUM();

// The lookup keys a .ogg registers under. rel_path is forward-slash, relative to resources_dir.
// We strip a known root prefix, drop the `.ogg`, drop the trailing variant number (grass1 -> grass),
// and also register an underscores->slashes variant so `random/door_close` matches event
// `random.door.close`. Returns a set of slash-joined keys.
py::set<py::str> _ogg_keys(const py::str& rel_path);

// Index every .ogg under resources_dir by lookup key -> sorted[relative paths]. One key can map
// to several files (the numbered variants grass1..grass4).
py::dict<py::str, py::list<py::str>> scan_ogg_sources(const py::str& resources_dir);

// One matched cue: {'event':.., 'files':[..]}.
struct Matched {
    py::str event;
    py::list<py::str> files;
};

// What match_sources returns: {'matched': {cue: {'event':.., 'files':[..]}}, 'missing': {cue:[events]},
// 'index_keys': n}. (build_music's synthetic match fills only `matched`.)
struct Match {
    py::dict<py::str, Matched> matched;
    py::dict<py::str, py::list<py::str>> missing;
    int64_t index_keys = 0;
};

// Map each needed cue to the original .ogg file(s) that supply it. Handles two early-Minecraft
// quirks: `dig.*` reuses the `step.*` samples (no separate dig oggs), and multi-word leaf names are
// stored with an underscore (`door_close.ogg`) rather than a sub-folder.
Match match_sources(const py::str& java_dir, const py::str& resources_dir);

// Emit convert_audio.bat: an ffmpeg command per matched cue that decodes its .ogg to a 16-bit
// PCM .wav named after the cue, under out_dir\<wav_subdir>\. That .wav is what build_banks.bat /
// build_pcm_xwb consumes. Uses the FIRST variant file per cue (bank builder handles variants
// separately). Returns the .bat path.
py::str write_ffmpeg_script(const Match& match, const py::str& out_dir, const py::str& resources_dir,
                            const py::str& wav_subdir = py::str("wav"),
                            const py::str& bat_name = py::str("convert_audio.bat"));

// Best-effort locate the original .ogg resources tree near the java source (a sibling/ancestor
// `resources` folder with a `newsound` or `sound` child). Returns the path or None.
std::optional<py::str> _find_resources_dir(const py::str& java_dir);

// FIXED BUG assets-in-project-root: the audio out dir IS the deploy dir (Debug\Content\Audio, shipped
// to the console), so after the banks build, drop the build intermediates -- the wav/ dir alone is ~60MB
// (the decoded music) of pure shippable junk. Keep ONLY the runtime .xwb/.xsb banks. Only runs once the
// banks exist, so a failed build never nukes the sources.
void _clean_audio_intermediates(const py::str& out_dir);

// Set up the STREAMING music bank (music.xwb/.xsb) from the calm* background tracks. Cues are the bare
// track names (calm1/2/3) to match XactAudio's g_MusicCues playlist, placed in the Music category. Writes
// music_convert.bat (ogg->wav) + music.xap + music_banks.bat. Returns the tracks actually found.
// FIXED BUG audio-no-music-bank: `build` only ever authored the SFX bank; music.xwb/.xsb were never built,
// so XactAudio::loadMusicBank failed -> g_musicDisabled -> no music. This adds the second (music) bank.
py::list<py::str> build_music(const py::str& out_dir, const py::str& resources_dir,
                              const py::list<py::str>& tracks = py::list<py::str>{"calm1", "calm2", "calm3"});

// One-shot XACT3 audio build for a converted game: write the .xap project, the XACTBLD build script,
// the ogg->wav ffmpeg script, and AUDIO_COVERAGE.md. When `autorun` is set, ALSO run the scripts
// (convert_audio.bat then build_banks.bat) so the .xwb/.xsb banks are produced as part of conversion --
// no manual step. `log` is an optional callable(str) for progress. Returns (xap_path, cues).
std::tuple<py::str, py::list<py::str>> build(const py::str& java_dir, const py::str& out_dir,
                                             const std::optional<py::str>& resources_dir = std::nullopt,
                                             bool autorun = false,
                                             const std::optional<py::str>& sdk_root = std::nullopt,
                                             const std::function<void(const py::str&)>& log = nullptr);

// J2X-BUGFIX 2026-08-13 audio-never-built: the wizard audio step only wrote convert_audio.bat and never
// generated the .xap/build_banks.bat, so the XACT banks were never produced -> no game audio. build() now
// runs the FULL XACT3 build, and run_build() (below) AUTORUNS both bats (ffmpeg then XACTBLD), skipping
// gracefully if a tool is missing. Content path is Content\Audio\ (not Content\resources).
//
// AUTORUN the two audio scripts in out_dir: convert_audio.bat (ffmpeg ogg->wav) then build_banks.bat
// (XACTBLD xap -> sounds.xwb/.xsb). Each step is SKIPPED gracefully (never fails the conversion) if its
// tool is missing -- ffmpeg on PATH, or XACTBLD at %XEDK%\bin\win32\xactbld3.exe. Returns (ok, lines).
std::tuple<bool, py::list<py::str>> run_build(const py::str& out_dir,
                                              const std::optional<py::str>& sdk_root = std::nullopt,
                                              const std::function<void(const py::str&)>& log = nullptr,
                                              const std::optional<py::list<py::str>>& only = std::nullopt);

}  // namespace codebreaker::java::console::xact
