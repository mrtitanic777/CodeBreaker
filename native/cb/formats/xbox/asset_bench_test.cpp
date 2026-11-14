// A timing of the asset readers on real and fixture files, for the speed record (docs/TYPED_CPP_SPEED.md); runs only with CB_XBOX_BENCH_DIR=<the proof folder>
// and prints the milliseconds per operation, to compare runs (testdata/assets/bench_assets, run from the checkout with the same files).
#include <chrono>
#include <cstdio>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace asset_bench_test {

using namespace cb;
using namespace cb::formats::xbox;

template <class F>
static void time_it(const char* name, int n, F&& f) {
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) f();
    const std::chrono::duration<double, std::milli> ms = std::chrono::steady_clock::now() - start;
    std::printf("%-28s %8.3f ms/op\n", name, ms.count() / n);
}

CB_TEST("formats.xbox.assets.timing_on_real_files", [] {
    const std::optional<std::string> dir = fs::get_env("CB_XBOX_BENCH_DIR");
    if (!dir) return;
    const std::filesystem::path mc = fs::from_text(*dir);
    const Bytes spa = fs::read_file(mc / "spa0.xdbf");
    std::vector<Bytes> scenes;
    for (int i = 0; i < 12; ++i) scenes.push_back(fs::read_file(mc / std::format("scene{:02}.xur", i)));
    const Bytes demo = fs::read_file(fs::from_text(asset_dump::fixture("xbe_demo.bin")));
    const Bytes xpr0 = fs::read_file(fs::from_text(asset_dump::fixture("xpr0.bin")));
    std::size_t sink = 0;
    time_it("xdbf spa parse+ach+build", 200, [&] {
        const xdbf::Database db = xdbf::Database::parse(spa);
        sink += db.counts().size() + db.achievements().size() + db.build().size();
    });
    time_it("xur 12 scenes strings", 100, [&] {
        for (const Bytes& d : scenes) sink += xur::Scene::parse(d).strings().size();
    });
    time_it("xbe parse+sections+pe", 500, [&] {
        const xbe::Xbe x = xbe::Xbe::parse(demo);
        sink += x.sections().size() + x.pe_image().size();
    });
    time_it("xpr0 parse+dds", 500, [&] { sink += xpr::to_dds(xpr::parse(xpr0)).size(); });
    time_it("bink+chd header (files)", 500, [&] {
        sink += bink::read_header_file(fs::from_text(asset_dump::fixture("t.bik"))).frames + chd::read_header_file(fs::from_text(asset_dump::fixture("t.chd"))).version;
    });
    std::printf("(%zu)\n", sink);
});

}  // namespace asset_bench_test
