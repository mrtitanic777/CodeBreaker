// cb-unity: alone
// Tests of cb::exact::plm86: Intel's PL/M-86 and ASM86 under the ISIS-II emulator, as a toolchain. What is tested without the kit
// (it is not in the repository): the variants' version and controls, the key, the cache keeping a compile (and the headers' case and line
// ends not mattering to it), the error for a kit that is not set up. With the kit (the CB_KIT_INTELPLM86 environment variable names its
// folder) the real PL/M-86 compiles a small module, and rejects one with an error in it.
//
// Hand-checked: the key (sha1 of name + version + controls + "p2", 12 digits), the cache key's parts, the tail, the
// "ok" rule (group 1 "0"), the 1500 characters of listing lines an error shows.
//
// Design notes: the include folders are not used; a profile that names no kit is a UsageError;
// the C++ cache is its own (it does not share keys with an older cache).
#include "exact/plm86.hpp"

#include <nlohmann/json.hpp>

#include "base/cache.hpp"
#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "exact/plugin.hpp"

namespace {

namespace plm = cb::exact::plm86;
namespace tc = cb::exact::toolchain;
namespace stdfs = std::filesystem;
namespace cfs = cb::fs;
using cb::as_bytes;

constexpr const char* kProfile = R"JSON({
  "name": "plm86", "plugin": "codebreaker.exact.plm86:PLM86", "kit": "intelplm86", "emulator": "isis",
  "default_variant": "link108",
  "variants": {"link108": {"version": "2.1", "controls": "MEDIUM OPTIMIZE(2) CODE"},
               "v1.2": {"version": "1.2", "controls": "SMALL"},
               "bare": {}},
  "tool": "plm86", "program": "PLM86", "source": "T.PLM", "tail": " T.PLM {controls}", "ok": "(\\d+) PROGRAM ERROR"
})JSON";

struct Fixture {
    cfs::ScopedTempDir dir{"cbplm86"};
    cfs::ScopedEnv no_kit{"CB_KIT_INTELPLM86", std::nullopt};
    tc::Locations locations;
    cb::base::cache::Config cache_config;
    std::unique_ptr<cb::base::cache::Cache> cache;
    Fixture() {
        locations.profiles = dir.path() / "profiles";
        locations.state = dir.path() / "state";
        locations.checkout_root = dir.path();
        cfs::write_text(locations.profiles / "plm86.json", kProfile);
        cache_config.state_dir = dir.path() / "cache";
        cache_config.shared_dir = dir.path() / "shared";
        cache = std::make_unique<cb::base::cache::Cache>(cache_config);
        cache->declare_kind(tc::kCacheKind, {cb::base::cache::Scope::State, "1"});
    }
    plm::Plm86 make(const std::string& variant = "") {
        tc::Context c;
        c.locations = &locations;
        c.cache = cache.get();
        return plm::Plm86(tc::read_profile(locations, "plm86"), variant.empty() ? std::nullopt : std::optional<std::string>(variant), c);
    }
};

}  // namespace

CB_TEST("exact.plm86.a_variant_says_its_version_and_controls", [] {
    Fixture f;
    const plm::Plm86 def = f.make();   // the profile's default variant
    CB_CHECK_EQ(def.version(), "2.1");
    CB_CHECK_EQ(def.controls(), "MEDIUM OPTIMIZE(2) CODE");
    const plm::Plm86 old = f.make("v1.2");
    CB_CHECK_EQ(old.version(), "1.2");
    CB_CHECK_EQ(old.controls(), "SMALL");
    // a variant that says nothing: version 2.1 and no controls
    const plm::Plm86 bare = f.make("bare");
    CB_CHECK_EQ(bare.version(), "2.1");
    CB_CHECK_EQ(bare.controls(), "");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)f.make("nonesuch"));
    // the key names a toolchain's compiles: the same for the same name, version and controls, another for another
    const std::string expect = cb::crypto::to_hex(cb::crypto::sha1(cb::to_bytes("plm86" "2.1" "MEDIUM OPTIMIZE(2) CODE" "p2"))).substr(0, 12);
    CB_CHECK_EQ(def.plugin_key(), expect);
    CB_CHECK_EQ(f.make("link108").plugin_key(), def.plugin_key());
    CB_CHECK(old.plugin_key() != def.plugin_key());
    // a profile with no file is NotFoundError
    f.locations.profiles = f.dir.path() / "nowhere";
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)plm::Plm86(tc::Profile{.name = "plm86"}, std::nullopt, [&] {
        tc::Context c;
        c.locations = &f.locations;
        return c;
    }()));
});

CB_TEST("exact.plm86.a_compile_that_was_kept_is_not_run_again", [] {
    Fixture f;
    const plm::Plm86 t = f.make();
    const std::string source = "TEST: DO;\nEND TEST;\n";
    // what the cache holds under the key of (this source with CR LF, the controls, the toolchain's key) is the answer
    const std::string key = cb::exact::plugin::compile_key(cb::exact::plugin::crlf(as_bytes(source)), {}, t.controls(), t.plugin_key());
    f.cache->keep(tc::kCacheKind, key, cb::exact::plugin::Built{cb::to_bytes("OBJ"), "console text"});
    const auto got = t.compile(as_bytes(source), {});   // the kit is not set up: running it would throw, so this came from the cache
    CB_CHECK(got.object.has_value() && *got.object == cb::to_bytes("OBJ"));
    CB_CHECK_EQ(got.console, "console text");
    // the same source with other line ends is the same source
    const auto lf = t.compile(as_bytes(std::string("TEST: DO;\r\nEND TEST;\r\n")), {});
    CB_CHECK(lf.object.has_value());
    // other controls are another compile: it runs, and the kit is not there
    CB_CHECK_THROWS_AS(cb::UsageError, (void)t.compile(as_bytes(source), {.switches = "SMALL"}));
    // the headers' names are upper case and their line ends CR LF, whatever they were given as
    const std::string with_header_key = cb::exact::plugin::compile_key(
        cb::exact::plugin::crlf(as_bytes(source)), {{"DEFS.LIT", cb::exact::plugin::crlf(as_bytes(std::string("LITERALLY 'X'\n")))}}, t.controls(), t.plugin_key());
    f.cache->keep(tc::kCacheKind, with_header_key, cb::exact::plugin::Built{cb::to_bytes("OBJ2"), ""});
    const auto h = t.compile(as_bytes(source), {.headers = {{"defs.lit", cb::to_bytes("LITERALLY 'X'\n")}}});
    CB_CHECK(h.object.has_value() && *h.object == cb::to_bytes("OBJ2"));
    // a compile that failed is kept too, with what the tool said
    const std::string failed_key = cb::exact::plugin::compile_key(cb::exact::plugin::crlf(as_bytes(std::string("BAD"))), {}, t.controls(), t.plugin_key());
    f.cache->keep(tc::kCacheKind, failed_key, cb::exact::plugin::Built{std::nullopt, "1 PROGRAM ERROR"});
    const auto bad = t.compile(as_bytes(std::string("BAD")), {});
    CB_CHECK(!bad.object.has_value());
    CB_CHECK_EQ(bad.console, "1 PROGRAM ERROR");
});

CB_TEST("exact.plm86.a_kit_that_is_not_set_up_is_said", [] {
    Fixture f;
    const plm::Plm86 t = f.make();
    CB_CHECK_THROWS_AS(cb::UsageError, (void)t.compile(as_bytes(std::string("TEST: DO; END TEST;")), {}));
});

CB_TEST("exact.plm86.registered_as_a_plugin_by_the_name_the_profile_gives", [] {
    Fixture f;
    // (no runner: the plugin does not use the generic passes)
    tc::Toolchains toolchains(f.locations, nullptr, f.cache.get());
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)toolchains.load("plm86"));
    plm::register_plugin(toolchains);
    const auto loaded = toolchains.load("plm86", "v1.2");
    CB_CHECK(std::dynamic_pointer_cast<plm::Plm86>(loaded) != nullptr);
    CB_CHECK_EQ(std::dynamic_pointer_cast<plm::Plm86>(loaded)->controls(), "SMALL");
});

// With the real kit: a small module compiles to an OMF-86 object; one with an error does not, and the console says why.
CB_TEST("exact.plm86.the_real_compiler", [] {
    const auto kit = cfs::get_env("CB_KIT_INTELPLM86");
    if (!kit || !stdfs::is_directory(cfs::from_text(*kit))) return;   // (not set up: nothing to run)
    cfs::ScopedTempDir dir("cbplm86real");
    tc::Locations locations;
    locations.profiles = cb::testing::data_path("../config/exact/toolchains");
    locations.state = dir.path() / "state";
    locations.checkout_root = dir.path();
    const plm::Plm86 t(tc::read_profile(locations, "plm86"), std::nullopt, [&] {
        tc::Context c;
        c.locations = &locations;
        return c;
    }());
    const std::string good =
        "TEST: DO;\r\nDECLARE COUNT WORD PUBLIC;\r\nBUMP: PROCEDURE PUBLIC;\r\n    COUNT = COUNT + 1;\r\nEND BUMP;\r\nEND TEST;\r\n";
    const auto built = t.compile(as_bytes(good), {});
    CB_CHECK(built.object.has_value());
    if (built.object) {
        CB_CHECK(!built.object->empty() && (*built.object)[0] == 0x80);   // an OMF-86 object begins with a THEADR record
        CB_CHECK(cb::text::from_latin1(*built.object).find("BUMP") != std::string::npos);
    }
    const auto bad = t.compile(as_bytes(std::string("TEST: DO;\r\nCOUNT = 1;\r\nEND TEST;\r\n")), {});
    CB_CHECK(!bad.object.has_value());
    CB_CHECK(!bad.console.empty());
});
