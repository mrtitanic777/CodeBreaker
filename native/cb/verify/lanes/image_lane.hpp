// cb/verify/lanes/image_lane.hpp -- the lane of a program image written back as its assembly source: a cartridge, a
// tape block, a ROM. The NES, Master System, Game Boy, Genesis, ZX Spectrum and MSX lanes are each one of these,
// giving only what is particular to their machine.
//
//     ImageLane lane({.kind = "gb", .rev = 1, .states = kStates, .effects = effects, .what = kWhat,
//                     .start = start, .writer = gb_writer});
//
// The program's source is written by its platform's writer and assembled; a program that comes back identical IS the
// original, so each of its functions does what the original's does. When it is not identical -- the source was
// edited -- each function is run in both, from the same starting states, and what it did is compared:
// `effects(subject, entry, state)` is what the run showed (how it ended, the registers, the memory it left, the
// hardware it touched), and `what` names each place in it, so a difference says which.
//
// A lane is a `Kind` (kind.hpp): profile, prepare, build; the build's judge is the per-function comparison above.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/verify/lanes/kind.hpp"

namespace cb::verify::lanes {

// Where a run starts: the registers and the seed of how memory is filled, by name ("a", "x", "fill"). Sorted by
// name: it is part of the lane's profile and is written into a verdict's detail.
using State = std::map<std::string, std::int64_t>;

// One place in what a run showed: a number (a register), text (how the run ended), bytes (memory as it was left), or
// lines of text (the hardware accesses in order).
using Effect = std::variant<std::int64_t, std::string, Bytes, std::vector<std::string>>;
using Effects = std::vector<Effect>;

// The stated sentence of a verdict when the rebuilt program is the original.
inline constexpr std::string_view kSameImage = "the cartridge built from this source is the original, byte for byte";

struct ImageLaneSpec {
    std::string kind;                         // the name results are filed under (its platform's)
    int rev = 0;                              // bump when what a verdict depends on changes
    std::vector<State> states;                // where each run starts (part of every verdict's profile)
    // What running `entry` of the program at `subject` from `state` showed. The same number of places every time.
    std::function<Effects(const std::filesystem::path& subject, std::uint32_t entry, const State& state)> effects;
    std::vector<std::string> what;            // a name for each place in the effects
    std::function<std::string(const State&)> start;   // the state, as a verdict's detail says it
    Writer writer;                            // the platform's: the program back as source, assembled
    // The two programs `effects` runs: (original, rebuilt). Default: the original's path and the built file's.
    std::function<std::pair<std::filesystem::path, std::filesystem::path>(const std::filesystem::path& module_path,
                                                                           const Written& written)> subjects;
    unsigned key_digits = 4;                  // how many hex digits an address has in the store's keys
    std::string same = std::string(kSameImage);
};

class ImageLane final : public Kind {
public:
    // What prepare() carries to build().
    struct PreparedImage final : Prepared {
        Written written;
        std::unordered_map<std::string, std::uint32_t> by_name;   // name -> address
    };

    explicit ImageLane(ImageLaneSpec spec);

    const std::string& name() const override { return spec_.kind; }
    // The digest is over the kind, the revision and every starting state.
    Profile profile(const std::filesystem::path& module_path) const override;
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path,
                                      const std::filesystem::path& work) const override;
    // A program that did not build leaves every function in `missing`, with the build's last note.
    Built build(const Prepared& prepared, const std::filesystem::path& module_path,
                const std::filesystem::path& work) const override;

    // The two programs a function is run in: the lane's `subjects` of the module and the written program, else the
    // module's own path and the built file.
    std::pair<std::filesystem::path, std::filesystem::path> subjects_of(const std::filesystem::path& module_path,
                                                                        const Written& written) const;

    // One function: Equivalent when the whole program is the original; else the first state from which the two
    // programs' runs differ says Divergent, and the reason names the first place that differs ("RAM-differs").
    Judgement judge_function(bool same, const std::filesystem::path& original, const std::filesystem::path& rebuilt,
                             std::uint32_t entry) const;

private:
    ImageLaneSpec spec_;
};

}  // namespace cb::verify::lanes
