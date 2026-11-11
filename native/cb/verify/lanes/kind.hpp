// cb/verify/lanes/kind.hpp -- what the reconstruction path asks of one kind of module.
//
// The path from a module to judged reconstructions of its functions is the same whatever the module is (decompile it,
// build the functions that have no verdict yet, run each beside the original, record what was found). What differs
// between kinds -- 32-bit or 16-bit Windows, PowerPC, drivers, real-mode programs, cartridges, Java programs -- is one
// small class per kind, deriving from `Kind`:
//
//     profile(path)      everything but the source that a verdict depends on: a digest (what results are filed
//                        under) and the settings it was made from
//     prepare(path, w)   the module written back as source: header text, the C (or assembly) of each function
//     build(p, path, w)  build it; the functions that can be judged, and why the others were not built
//     built.judge(name)  the verdict on one function; safe to call from several threads at once
//
// `Prepared` is what every kind says of a prepared module; a kind that has more to carry between its stages derives
// from it (ImageLane keeps the writer's result). The judge a build hands back is a closure over what that kind needs
// (the context of the whole build, the lane itself), so it needs no registry of names: it is valid for as long as the
// kind and the build are.
//
// The writers of the platforms (cb/source: the module written back as source, assembled and linked) are not part of
// this header: a lane is given its writer as a `Writer`, the function that does the platform's work.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/verify/verdicts.hpp"

namespace cb::verify::lanes {

// A kind's profile: the digest results are filed under, and the settings (human-readable) it was computed from.
struct Profile {
    std::string digest;
    std::string settings;
};

// What every kind says of a prepared module.
struct Prepared {
    Prepared() = default;
    Prepared(const Prepared&) = default;
    Prepared& operator=(const Prepared&) = default;
    virtual ~Prepared() = default;

    std::string header;                                     // the module's source text, as the file holds it
    std::vector<std::string> order;                         // function names, by address
    std::unordered_map<std::string, std::string> key;       // name -> its address as the store's keys write it
    std::vector<std::string> all_keys;                      // every function's key, sorted
    std::unordered_map<std::string, std::string> texts;     // name -> the text it is judged for
    std::string sig;                                        // what a verdict was reached for: the source, and the lane's revision
};

// What a build gives: the functions the judge can be asked about, the judge, and the functions that were not built.
struct Built {
    std::vector<std::string> items;                         // the names that can be judged, in build order
    std::function<Judgement(const std::string& name)> judge; // thread-safe; valid while the Kind and this Built live
    std::map<std::string, std::string> missing;             // name -> why it is not in the build (sorted: reports)
};

class Kind {
public:
    virtual ~Kind() = default;

    // The name results are filed under ("dos", "gb", "nes", ...).
    virtual const std::string& name() const = 0;
    virtual Profile profile(const std::filesystem::path& module_path) const = 0;
    virtual std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path,
                                              const std::filesystem::path& work) const = 0;
    virtual Built build(const Prepared& prepared, const std::filesystem::path& module_path,
                        const std::filesystem::path& work) const = 0;
};

// What a platform's writer says of the module it wrote back as source and rebuilt (the packager of
// codebreaker/platforms: source/<machine>_writer, source/dos_writer).
struct Written {
    std::filesystem::path source;                           // the source file written; empty: none
    std::map<std::uint32_t, std::string> functions;         // address -> name (ordered: addresses ascend)
    bool linked = false;                                    // the program was assembled and linked
    std::vector<std::string> notes;                         // what the build said, the last line last
    bool same = false;                                      // the built program is the original, byte for byte
    std::filesystem::path built;                            // the built file
};

// A note of the build, cut to what a verdict's reason can carry: 200 bytes, never inside a UTF-8 sequence.
inline std::string clip_note(std::string_view note) {
    constexpr std::size_t kMax = 200;
    if (note.size() <= kMax)
        return std::string(note);
    std::size_t end = kMax;
    while (end > 0 && (static_cast<unsigned char>(note[end]) & 0xC0) == 0x80)
        --end;
    return std::string(note.substr(0, end));
}

// Write `module_path` back as source into the folder `out_dir` and rebuild it there.
using Writer = std::function<Written(const std::filesystem::path& module_path, const std::filesystem::path& out_dir)>;

}  // namespace cb::verify::lanes
