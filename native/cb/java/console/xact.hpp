// cb/java/console/xact.hpp -- XACT3 wave-bank (.xwb) / sound-bank (.xsb) tool for the Xbox 360 audio pipeline.
//
// 360 audio is XACT3 or XAudio2; the port uses XACT3 (Audio.reference routes SoundManager -> XACT cues). This module
// reads/decompiles the banks and builds them from the game's sound list and the .ogg/.wav sources. It is grounded in
// the banks the user authored by hand for the Alpha port (Content/Audio/sounds.xwb + .xsb, music.*).
//
// Format (Xbox 360 = BIG-ENDIAN; XACT stores the magic byte-reversed vs the PC LE 'WBND'/'SDBK'):
//   XWB  magic 'DNBW'  header{version, headerVersion, 5 x (offset,length) segments:
//        BANKDATA / ENTRYMETADATA / SEEKTABLES / ENTRYNAMES / ENTRYWAVEDATA}
//        BANKDATA{flags, entryCount, name[64], metaElemSize, nameElemSize, alignment, ...}
//        ENTRYMETADATA[entryCount] of WAVEBANKENTRY{ flagsAndDuration, miniFormat (a packed DWORD),
//        PlayRegion(offset,length), LoopRegion(...) } -- miniFormat packs tag/channels/rate/align/bits.
//   XSB  magic 'KBDS' ... a trailing table of null-terminated CUE NAMES (the sound-event names).
//
// What the game plays comes in as a SoundDemand (the sound events and step-sound families its source mentions); the
// Minecraft project module that scans the source produces it. Three kinds of work live here:
//   * read:   parse_xwb / parse_xsb / dump, and the coverage diff of the cues the game needs against a bank's cues;
//   * author: the .xap project XACTBLD compiles, its build script, and a direct PCM .xwb writer that needs no XDK;
//   * source: match each needed cue to the original .ogg files, write the ffmpeg script that decodes them, and run it
//             all (build / run_build), skipping any step whose tool is not installed.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::java::console::xact {

// ---- reading banks ------------------------------------------------------------------------------------------

enum class Codec { Pcm, Xma, Adpcm, Wma };

// "PCM", "XMA", "ADPCM", "WMA".
std::string_view codec_name(Codec codec);

// What a wave's packed WAVEBANKMINIWAVEFORMAT DWORD says: tag[0:2] channels[2:5] rate[5:23] blockAlign[23:31]
// bits[31:32] (one bit: 8 or 16).
struct MiniFormat {
    Codec codec = Codec::Pcm;
    std::uint32_t channels = 0;
    std::uint32_t sample_rate = 0;
    std::uint32_t block_align = 0;
    std::uint32_t bits = 8;
};
MiniFormat decode_miniformat(std::uint32_t dword);

// The packed DWORD of PCM parameters (tag 0), the inverse of decode_miniformat for PCM.
std::uint32_t pack_pcm_miniformat(std::uint32_t channels, std::uint32_t rate, std::uint32_t bits);

// A segment of a wave bank file.
struct Segment {
    std::uint32_t offset = 0;
    std::uint32_t length = 0;
};

enum class SegmentId { BankData, EntryMetadata, SeekTables, EntryNames, EntryWaveData };
inline constexpr std::size_t kSegmentCount = 5;
// "BANKDATA", "ENTRYMETADATA", "SEEKTABLES", "ENTRYNAMES", "ENTRYWAVEDATA".
std::string_view segment_name(SegmentId id);

struct WaveEntry {
    std::size_t index = 0;
    std::uint32_t play_offset = 0;
    std::uint32_t play_length = 0;
    MiniFormat format;
    std::optional<std::string> name;   // present when the bank kept entry names
};

struct Xwb {
    std::string file;                  // the file's name
    std::string magic;                 // "DNBW" (360) or "WBND" (PC)
    std::uint32_t version = 0;
    std::uint32_t header_version = 0;
    std::string name;                  // the bank's name
    std::uint32_t flags = 0;
    std::uint32_t entry_count = 0;     // as the header says; `entries` holds the ones the file actually has
    std::array<Segment, kSegmentCount> segments{};
    std::vector<WaveEntry> entries;
};

// Decompile a wave bank. FormatError when the magic is neither DNBW nor WBND; TruncatedError when the header
// is cut short. An entry table that runs past the end of the file ends the list there (a damaged bank still lists
// what it can).
Xwb parse_xwb(ByteView data, std::string file_name);
Xwb read_xwb(const std::filesystem::path& path);

struct Xsb {
    std::string file;
    std::string magic;                 // "KBDS" (360) or "SDBK" (PC)
    std::string name;                  // the bank's name
    std::vector<std::string> cue_names;   // the sound events the game triggers (step_grass, ...), in file order
};

// Decompile a sound bank: the null-terminated name table after the header (printable runs of three or more
// characters; the first is the bank's name, repeats of it are dropped). FormatError for a bad magic.
Xsb parse_xsb(ByteView data, std::string file_name);
Xsb read_xsb(const std::filesystem::path& path);

// A human-readable decompile of an .xwb or .xsb (chosen by the extension, case-insensitive); UsageError for any other
// extension. At most the first 64 wave entries are listed.
std::string dump(const std::filesystem::path& path);

// ---- what the game needs ------------------------------------------------------------------------------------

// The sounds the converted game plays, as its source mentions them.
struct SoundDemand {
    std::vector<std::string> sounds;        // sound events: "random.click", "step.grass" (playSound arguments)
    std::vector<std::string> step_sounds;   // step-sound families: "stone", "grass" (StepSound("stone"))
};

// Java sound EVENT ('step.grass', 'random.click') -> XACT cue base name ('step_grass', 'random_click'): runs of dots
// and white space become one underscore. Matches the toCue() convention in Audio.reference.
std::string cue_name(std::string_view event);

// The XACT cues the converted game will request, each with the events that ask for it (in sorted order): one per
// sound event, plus step_<family> and dig_<family> for every step-sound family. This is the demand side; diff it
// against a bank's authored cues to find the silent-audio gaps.
using Needed = std::map<std::string, std::vector<std::string>>;
Needed needed_cues(const SoundDemand& demand);

struct CueCoverage {
    Needed needed;
    std::vector<std::string> authored;   // the bank's cues with their " 2".." 4" variant suffix dropped, sorted
    std::vector<std::string> missing;    // needed but not authored: GetCueIndex fails, the sound silently no-ops
    std::vector<std::string> unused;     // authored but never needed
};
CueCoverage cue_coverage(const SoundDemand& demand, const Xsb& bank);

// AUDIO_COVERAGE.md text for a coverage.
std::string render_coverage(const CueCoverage& coverage);

// ---- authoring ----------------------------------------------------------------------------------------------

struct XapOptions {
    std::string wave_dir = "wav";             // where the .wav files are, relative to the .xap
    std::string bank = "sounds";              // the bank's file name: <bank>.xwb / <bank>.xsb
    std::string sound_category = "Default";   // "Default" for effects, "Music" for music
    bool streaming = false;                   // mark the wave bank streaming (music.xwb; sounds.xwb too, see build())
    // The name a Cue is given for a cue's sound; the sound and wave keep the cue name itself. Identity when empty.
    // XactAudio's playlist cues (calm1/2/3) and toCue()'s underscore names both use the cue name as it is.
    std::function<std::string(std::string_view)> cue_namer;
};

// Write an XACT3 project (.xap) that AUTHORS the cues (sorted), each referencing <wave_dir>/<cue>.wav; XACTBLD
// compiles it to <bank>.xwb + <bank>.xsb. Only cues whose .wav was actually produced are authored when the wave
// folder exists next to the .xap and holds any of them: some source oggs are placeholders ffmpeg cannot decode, and
// one missing .wav makes XACTBLD abort ALL banks. Each Sound's Wave Entry index and each Cue's Sound Entry index is
// the cue's real position in the bank (cue i -> wave i -> sound i). Returns the cues authored.
std::vector<std::string> write_xap(const std::filesystem::path& out_path, std::vector<std::string> cues,
                                   const XapOptions& options = {});

// Write <bat_name>: run the XDK XACTBLD to compile <bank>.xap -> .xwb/.xsb for the 360. Returns its path.
std::filesystem::path write_build_script(const std::filesystem::path& out_dir, std::string_view bank = "sounds",
                                         std::string_view bat_name = "build_banks.bat");

// A PCM wave file's format and samples.
struct Wav {
    std::uint32_t channels = 0;
    std::uint32_t rate = 0;
    std::uint32_t bits = 0;
    Bytes data;
};

// A minimal RIFF/WAVE PCM reader. FormatError for anything that is not RIFF/WAVE or has no fmt chunk;
// UnsupportedError for a non-PCM format.
Wav parse_wav(ByteView file, std::string_view name = "wav");

struct PcmEntry {
    std::string name;            // the wave's file name without extension
    std::uint32_t mini = 0;      // its packed mini format
    std::uint32_t offset = 0;    // into the bank's wave data
    std::uint32_t length = 0;
};

// Write a REAL Xbox-360 (big-endian) .xwb wave bank from PCM .wav files -- no XACTBLD needed. PCM only (XWMA/XMA
// compression needs the XDK encoder); good for small effects and bring-up. Waves are aligned to 2048 bytes.
std::vector<PcmEntry> build_pcm_xwb(const std::vector<std::filesystem::path>& wav_files,
                                    const std::filesystem::path& out_xwb, std::string_view bank = "sounds");

// ---- source matching: cue -> original .ogg files ------------------------------------------------------------

// The lookup keys a .ogg registers under. `rel_path` is forward-slash, relative to the resources folder: a known
// root prefix (newsound, sound, newmusic, music, streaming, resources) is stripped, so is the ".ogg" and the trailing
// variant number (grass1 -> grass), and an underscores-to-slashes variant is added so `random/door_close` matches
// the event `random.door.close`. Sorted.
std::vector<std::string> ogg_keys(std::string_view rel_path);

// Every .ogg under the resources folder by lookup key -> sorted relative paths. One key can map to several
// files (the numbered variants grass1..grass4).
std::map<std::string, std::vector<std::string>> scan_ogg_sources(const std::filesystem::path& resources_dir);

struct Matched {
    std::string event;                 // the event whose files were used
    std::vector<std::string> files;    // relative to the resources folder, forward slashes
};

struct Match {
    std::map<std::string, Matched> matched;                    // cue -> its source
    std::map<std::string, std::vector<std::string>> missing;   // cue -> events, with no source file
    std::size_t index_keys = 0;
};

// Map each needed cue to the original .ogg files that supply it. Handles two early-Minecraft quirks: `dig.*` reuses
// the `step.*` samples (no separate dig oggs), and multi-word leaf names are stored with an underscore
// (door_close.ogg) rather than in a subfolder.
Match match_sources(const SoundDemand& demand, const std::filesystem::path& resources_dir);

// Write convert_audio.bat: an ffmpeg command per matched cue decoding its .ogg (the first variant) to a 16-bit mono
// 44.1 kHz PCM .wav named after the cue, into out_dir/<wav_subdir>/. That .wav is what build_banks.bat and
// build_pcm_xwb consume. Creates the wave folder. Returns the script's path.
std::filesystem::path write_ffmpeg_script(const Match& match, const std::filesystem::path& out_dir,
                                          const std::filesystem::path& resources_dir, std::string_view wav_subdir = "wav",
                                          std::string_view bat_name = "convert_audio.bat");

// Best-effort: the original .ogg resources tree near the Java source -- a `resources` (or `game/resources`) folder
// with a `newsound` or `sound` child, in the folder or up to four above it.
std::optional<std::filesystem::path> find_resources_dir(const std::filesystem::path& java_dir);

// Drop the build intermediates once the banks exist: the audio folder IS the deploy folder (Debug/Content/Audio,
// shipped to the console) and the wav/ folder alone is ~60 MB of decoded music. Keeps only the runtime .xwb/.xsb.
// Does nothing unless an .xwb is there, so a failed build never deletes the sources.
void clean_audio_intermediates(const std::filesystem::path& out_dir);

// Set up the STREAMING music bank (music.xwb/.xsb) from the calm* background tracks: cues are the bare track names
// (calm1/2/3) to match XactAudio's playlist, in the Music category. Writes music_convert.bat (ogg -> wav), music.xap and
// music_banks.bat. Returns the tracks found (empty, and nothing written, when none is).
std::vector<std::string> build_music(const std::filesystem::path& out_dir, const std::filesystem::path& resources_dir,
                                     const std::vector<std::string>& tracks = {"calm1", "calm2", "calm3"});

// ---- running the build --------------------------------------------------------------------------------------

using LogFn = std::function<void(std::string_view)>;

struct BuildOptions {
    std::optional<std::filesystem::path> resources_dir;   // the .ogg tree; else looked for from java_dir
    std::optional<std::filesystem::path> java_dir;        // where to look for the resources tree
    bool autorun = false;                                 // also run the scripts (ffmpeg, then XACTBLD)
    std::optional<std::filesystem::path> sdk_root;        // XEDK for XACTBLD when the environment has none
    LogFn log;
};

struct BuildResult {
    std::filesystem::path xap;
    std::vector<std::string> cues;
};

// One-shot XACT3 audio build for a converted game: write the .xap project, the XACTBLD build script, the ogg -> wav
// ffmpeg script, the music bank, and AUDIO_COVERAGE.md. With `autorun` it also runs the scripts so the banks are
// produced as part of the conversion: decode first, author the .xap from the wavs that ACTUALLY landed (so the cue
// list is final after conversion), then compile the banks and drop the intermediates. The effects bank is built
// streaming: XactAudio::loadSFXBank uses CreateStreamingWaveBank with no in-memory fallback, and an in-memory bank
// there ends in DbgBreakPoint.
BuildResult build(const SoundDemand& demand, const std::filesystem::path& out_dir, const BuildOptions& options = {});

struct RunResult {
    bool ok = true;
    std::vector<std::string> lines;   // the progress lines, as they were logged
};

// Run the audio scripts in out_dir: convert_audio.bat and music_convert.bat (ffmpeg), then build_banks.bat and
// music_banks.bat (XACTBLD at %XEDK%/bin/win32/xactbld3.exe). A step whose tool is missing, or whose script was not
// written, is skipped (never a failure: the conversion must not abort over audio). `only` limits it to the named
// scripts. The child gets XEDK=sdk_root unless the environment already has one; our own environment is not touched.
RunResult run_build(const std::filesystem::path& out_dir, const std::optional<std::filesystem::path>& sdk_root = {},
                    const LogFn& log = {}, const std::optional<std::vector<std::string>>& only = {});

}  // namespace cb::java::console::xact
