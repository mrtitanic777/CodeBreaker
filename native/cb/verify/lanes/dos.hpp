// cb/verify/lanes/dos.hpp -- what is particular to reconstructing a real-mode program.
//
// The program's source is assembly written from its image (the DOS writer); it is assembled and linked, and the
// result is compared with the original as a whole: every byte of the load module, the relocation table, the entry
// point and the stack.
//
// Nothing is run. A program that comes back identical IS the original, so each of its functions does what the
// original's does -- a stronger statement than any number of runs would give, and the verdict says that this is how
// it was reached. A program that does not come back identical leaves all of its functions unverified, with where it
// first differs.
//
// What writes and rebuilds the program, and its tools, are the platform's packager (cb/platforms/dos.hpp describes
// the platform; the writer and the tools' paths belong to the source and build layers), given to the lane as a
// `Writer` and a `Tools` function. A `Kind` (kind.hpp).
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>

#include "cb/verify/lanes/kind.hpp"

namespace cb::verify::lanes {

// The sentence of a verdict when the rebuilt program is the original.
inline constexpr std::string_view kSameProgram = "the program built from this source is the original, byte for byte";

class DosLane final : public Kind {
public:
    // The revision of what a DOS verdict depends on; bump it when that changes.
    static constexpr int kRev = 93;

    // The tools that assemble and link, by name ("wasm", "wlink") and where they are.
    using Tools = std::function<std::map<std::string, std::filesystem::path>()>;

    DosLane(Writer writer, Tools tools) : writer_(std::move(writer)), tools_(std::move(tools)) {}

    const std::string& name() const override;
    // The digest covers the revision and, for each tool, its size and modification time: a different assembler is a
    // different verdict.
    Profile profile(const std::filesystem::path& module_path) const override;
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path,
                                      const std::filesystem::path& work) const override;
    // The whole program is judged as one: every function of it is Equivalent, or none is (with the build's last note).
    Built build(const Prepared& prepared, const std::filesystem::path& module_path,
                const std::filesystem::path& work) const override;

    // What prepare() carries to build().
    struct PreparedProgram final : Prepared {
        Written written;
    };

    // Why a program was not built: the build's last note, cut to what a reason can carry, or a stated default.
    static std::string why_not_built(const Written& written);
    // Why a program that was built is not the original's: the same; empty when it is.
    static std::string why_differs(const Written& written);

    // The verdict of a function of a program that did (`same`) or did not (`why` says how) come back identical.
    static Judgement judge_function(bool same, const std::string& why);

private:
    Writer writer_;
    Tools tools_;
};

}  // namespace cb::verify::lanes
