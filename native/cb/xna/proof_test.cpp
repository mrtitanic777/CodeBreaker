// The proof of the XNA/XACT converters on real game files: with CB_XNA_PROOF_DIR=<folder> holding files.txt (one path per line; .xnb, .xwb or .xsb),
// this converts every file with the native code into <folder>/native/<n>/ and writes what each made to <folder>/native_records.json, and prints the
// time it took. state/work/typed-port/xna_proof/compare.py runs the Python on the same list and compares the records and every byte written.
// Without the variable the test does nothing: the files are on the owner's machine (C:/Nexia360/Library, Games). CB_XNA_PROOF_FFMPEG names ffmpeg.
#include <chrono>
#include <cstdio>
#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/xna/xact.hpp"
#include "cb/xna/xnb.hpp"

namespace {

namespace stdfs = std::filesystem;

CB_TEST("xna.proof.real_files_are_converted_for_the_comparison_with_the_python", [] {
    const std::optional<std::string> folder = cb::fs::get_env("CB_XNA_PROOF_DIR");
    if (!folder) return;
    const stdfs::path dir = cb::fs::from_text(*folder);
    const std::optional<std::string> ffmpeg_text = cb::fs::get_env("CB_XNA_PROOF_FFMPEG");
    std::optional<stdfs::path> ffmpeg;
    if (ffmpeg_text) ffmpeg = cb::fs::from_text(*ffmpeg_text);
    const cb::xna::xnb::Environment env{ffmpeg};
    cb::Json records = cb::Json::array();
    double convert_ms = 0;
    std::size_t n = 0;
    std::map<std::string, double> by_kind;
    double sound_bank_ms = 0;   // reading every sound bank once, per repeat
    const std::string list = cb::fs::read_text(dir / "files.txt");
    for (const std::string_view line : cb::text::split(list, '\n')) {
        const std::string path(cb::text::trim(line));
        if (path.empty()) continue;
        const stdfs::path file = cb::fs::from_text(path), out = dir / "native" / std::to_string(n++);
        const std::string ext = cb::text::lower(cb::fs::to_text(file.extension()));
        const auto start = std::chrono::steady_clock::now();
        cb::Json record = cb::Json::object();
        try {
            if (ext == ".xnb") {
                record = cb::json::parse(cb::xna::xnb::convert(file, out, env));
            } else if (ext == ".xwb") {
                const std::size_t made = cb::xna::xact::decode_bank(file, out, ffmpeg, [](std::string_view) {});
                const cb::xna::xact::WaveBank bank = cb::xna::xact::read_wave_bank(cb::fs::read_file(file));
                record = {{"name", bank.name}, {"entries", bank.entries.size()}, {"wavs", made}};
            } else {
                record = cb::json::parse(cb::xna::xact::cues_json(cb::xna::xact::read_sound_bank(cb::fs::read_file(file))));
                // the reading alone, many times over: what a caller that wants the cues pays
                constexpr int kRepeats = 200;
                const auto begin = std::chrono::steady_clock::now();
                const cb::Bytes bytes = cb::fs::read_file(file);
                for (int i = 0; i < kRepeats; ++i) cb::xna::xact::read_sound_bank(bytes);
                sound_bank_ms += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - begin).count() / kRepeats;
            }
        } catch (const cb::Error& e) {
            record = {{"error", e.what()}};
        }
        const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
        convert_ms += ms;
        by_kind[ext] += ms;
        records.push_back(record);
    }
    cb::fs::write_text(dir / "native_records.json", records.dump(1));
    std::printf("xna proof: %zu files, %.1f ms\n", n, convert_ms);
    std::printf("  sound banks read (each of the .xsb once): %.3f ms\n", sound_bank_ms);
    for (const auto& [ext, ms] : by_kind) std::printf("  %s %.1f ms\n", ext.c_str(), ms);
});

}  // namespace
