// The fix passes and the coverage scanners over a few MB of converted-looking C++ text (the fixtures repeated): a loose
// bound that catches an accidental quadratic, and the timings docs/TYPED_CPP_SPEED.md records (printed to stderr).
#include <chrono>
#include <cstdio>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/applet_ping.hpp"
#include "cb/java/projects/minecraft/platform_coverage.hpp"
#include "cb/java/projects/minecraft/render_fix.hpp"
#include "cb/java/projects/minecraft/sound_fix.hpp"

namespace {

using namespace cb;
using Clock = std::chrono::steady_clock;

std::string speed_fixture(const char* name) {
    return fs::read_text(testing::data_path(std::string("java/projects/minecraft/testdata/fixes/") + name));
}

double speed_ms(Clock::time_point from) { return std::chrono::duration<double, std::milli>(Clock::now() - from).count(); }

CB_TEST("java.minecraft.fixes.speed", [] {
    std::string corpus;
    const std::string chunk = speed_fixture("applet_setlevel.in.cpp") + speed_fixture("sound_sfx.in.cpp") +
                              speed_fixture("sound_music.in.cpp") + speed_fixture("render_update.in.cpp");
    for (int i = 0; i < 1000; ++i) corpus += chunk;
    std::fprintf(stderr, "    fix passes over %.1f MB: ", static_cast<double>(corpus.size()) / (1 << 20));

    auto t0 = Clock::now();
    const auto ping = java::projects::minecraft::applet_ping::rewrite(corpus);
    const double ping_ms = speed_ms(t0);
    t0 = Clock::now();
    const auto sound = java::projects::minecraft::sound_fix::rewrite(corpus);
    const double sound_ms = speed_ms(t0);
    t0 = Clock::now();
    const auto render = java::projects::minecraft::render_fix::rewrite(corpus);
    const double render_ms = speed_ms(t0);
    t0 = Clock::now();
    const auto provided = java::projects::minecraft::platform_coverage::provided_symbols(corpus);
    const double provided_ms = speed_ms(t0);
    t0 = Clock::now();
    const auto used = java::projects::minecraft::platform_coverage::used_holder_symbols(corpus, "GL11");
    const double used_ms = speed_ms(t0);
    std::fprintf(stderr, "applet %.0f ms, sound %.0f ms, render %.0f ms, provided_symbols %.0f ms, used_holder_symbols %.0f ms\n",
                 ping_ms, sound_ms, render_ms, provided_ms, used_ms);

    CB_CHECK_EQ(ping.sites, std::size_t{1000});
    CB_CHECK_EQ(sound.sfx, std::size_t{2000});
    CB_CHECK_EQ(sound.music, std::size_t{1000});
    CB_CHECK_EQ(render.update, std::size_t{1000});
    CB_CHECK_EQ(render.hurt, std::size_t{1000});
    CB_CHECK(!provided.empty());
    CB_CHECK(used.empty());
    // a loose ceiling: a linear pass over a few MB takes tens of milliseconds, not seconds
    CB_CHECK(ping_ms < 5000 && sound_ms < 5000 && render_ms < 5000 && provided_ms < 5000 && used_ms < 5000);
});

}  // namespace
