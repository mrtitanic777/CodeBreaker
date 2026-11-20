// x86link_replay <corpus> <scratch> <watcom rel folder> -- every captured wlink run linked again in-process (replay.hpp); prints each
// case that differs (or cannot be replayed) and the count; exit 0 when every case that can be replayed is the same.
#include <cstdio>

#include "build/x86link/replay.hpp"
#include "core/fs.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::fprintf(stderr, "usage: x86link_replay <corpus> <scratch> <watcom rel folder>\n");
        return 2;
    }
    const auto outcomes = cb::build::x86link::replay::all(cb::fs::from_text(argv[1]), cb::fs::from_text(argv[2]), cb::fs::from_text(argv[3]));
    std::size_t same = 0, skipped = 0;
    for (const auto& o : outcomes) {
        if (o.same) {
            ++same;
            continue;
        }
        if (o.skipped) ++skipped;
        std::printf("  %s: %s\n", o.id.c_str(), o.why.c_str());
    }
    std::printf("%zu of %zu links the same (%zu not replayable)\n", same, outcomes.size() - skipped, skipped);
    return same + skipped == outcomes.size() && same > 0 ? 0 : 1;
}
