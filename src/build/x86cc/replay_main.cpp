// x86cc_replay <corpus> <scratch> <watcom rel folder> -- every captured wcc / wcc386 run compiled again in-process
// (replay.hpp); prints each case that differs and the count; exit 0 when every case is the same.
#include <cstdio>

#include "build/x86cc/replay.hpp"
#include "core/fs.hpp"

int main(int argc, char** argv) {
    if (argc != 4) {
        std::fprintf(stderr, "usage: x86cc_replay <corpus> <scratch> <watcom rel folder>\n");
        return 2;
    }
    const auto outcomes =
        cb::build::x86cc::replay::all(cb::fs::from_text(argv[1]), cb::fs::from_text(argv[2]), cb::fs::from_text(argv[3]));
    std::size_t same = 0;
    for (const auto& o : outcomes) {
        if (o.same) {
            ++same;
        } else {
            std::printf("  %s: %s\n", o.id.c_str(), o.why.c_str());
        }
    }
    std::printf("%zu of %zu compiles the same\n", same, outcomes.size());
    return same == outcomes.size() && same > 0 ? 0 : 1;
}
