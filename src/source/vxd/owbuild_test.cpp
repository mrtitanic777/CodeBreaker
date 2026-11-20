// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/owbuild: where the OpenWatcom tools are, the W3 bundle header, and what a build says when the tools are not there. A build
// with the real tools runs where the settings name them (SPLITTER.VXD, which builds into a 1935-byte driver with one object); the
// proof that the images are the recorded ones is in the report of the change.
#include "source/vxd/owbuild.hpp"

#include <tuple>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace vx = cb::source::vxd;
namespace ow = cb::source::vxd::owbuild;

namespace {

// A W3 bundle: a DOS stub whose header points at the W3 header at `lfa`, and a record for each (name, offset, size).
cb::Bytes ow_bundle(std::uint32_t lfa, std::initializer_list<std::tuple<const char*, std::uint32_t, std::uint32_t>> records) {
    cb::Bytes data(lfa + 16 + records.size() * 16, 0);
    data[0] = 'M', data[1] = 'Z';
    for (int k = 0; k < 4; ++k) data[0x3C + k] = static_cast<std::uint8_t>(lfa >> (8 * k));
    data[lfa] = 'W', data[lfa + 1] = '3';
    data[lfa + 4] = static_cast<std::uint8_t>(records.size());
    std::size_t at = lfa + 16;
    for (const auto& [name, offset, size] : records) {
        const std::string n = name;
        for (std::size_t i = 0; i < n.size(); ++i) data[at + i] = static_cast<std::uint8_t>(n[i]);
        for (int k = 0; k < 4; ++k) data[at + 8 + k] = static_cast<std::uint8_t>(offset >> (8 * k)), data[at + 12 + k] = static_cast<std::uint8_t>(size >> (8 * k));
        at += 16;
    }
    return data;
}

struct OwWorld {
    cb::fs::ScopedTempDir dir{"owbuildtest"};
    vx::DdkNames names;
    vx::masm::Toolchain no_tools;
    std::unique_ptr<vx::link::Environment> link;
    std::unique_ptr<ow::Environment> env;
    explicit OwWorld(ow::Tools tools) {
        link = std::make_unique<vx::link::Environment>(vx::link::Environment{names, no_tools, [](cb::ByteView, std::string_view, const std::filesystem::path&) { return vx::link::Linked{}; }});
        env = std::make_unique<ow::Environment>(ow::Environment{*link, std::move(tools)});
    }
};

}  // namespace

CB_TEST("source.vxd.owbuild.the_tools_of_an_openwatcom_tree", [] {
    // the setting names the tree's rel folder; the assembler is in the tree's own build folder, beside it
    const ow::Tools tools = ow::tools_from_settings("C:/cb", "D:/tools/open-watcom-v2/rel");
    CB_CHECK(tools.wasm == std::filesystem::path("D:/tools/open-watcom-v2") / "bld" / "wasm" / "nt386" / "wasm.exe");
    CB_CHECK(tools.wlink == std::filesystem::path("D:/tools/open-watcom-v2") / "rel" / "binnt64" / "wlink.exe");
    CB_CHECK(tools.watcom == std::filesystem::path("D:/tools/open-watcom-v2") / "rel");
    // no setting: the tree of the premise experiment, in the checkout
    const ow::Tools fallback = ow::tools_from_settings("C:/cb", "");
    CB_CHECK(fallback.wlink == std::filesystem::path("C:/cb") / "toolchains" / "openwatcom" / "rel" / "binnt64" / "wlink.exe");
});

CB_TEST("source.vxd.owbuild.the_header_of_a_w3_bundle", [] {
    const cb::Bytes bundle = ow_bundle(0x100, {{"VMM", 0x2000, 0x1800}, {"VKD     ", 0x4000, 0x900}, {"A", 0x6000, 7}});
    const std::vector<ow::Component> parts = ow::w3_components(bundle);
    CB_CHECK_EQ(parts.size(), std::size_t{3});
    CB_CHECK_EQ(parts[0].name, "VMM");
    CB_CHECK_EQ(parts[0].offset, std::uint32_t{0x2000});
    CB_CHECK_EQ(parts[0].size, std::uint32_t{0x1800});
    CB_CHECK_EQ(parts[1].name, "VKD");   // padded to eight with blanks: the blanks are not part of the name
    CB_CHECK_EQ(parts[2].name, "A");
    CB_CHECK_EQ(parts[2].size, std::uint32_t{7});
    // not a bundle: an LE driver, nothing at all, a bundle whose header the file does not hold
    CB_CHECK(ow::w3_components(cb::fs::read_file(cb::testing::data_path("formats/testdata/splitter.vxd"))).empty());
    CB_CHECK(ow::w3_components({}).empty());
    cb::Bytes cut = bundle;
    cut.resize(0x100 + 16 + 16);   // two records short
    CB_CHECK_THROWS_AS(cb::TruncatedError, ow::w3_components(cut));
});

CB_TEST("source.vxd.owbuild.without_the_tools_a_build_says_so", [] {
    OwWorld world(ow::tools_from_settings(std::filesystem::path("nowhere"), "nowhere/open-watcom-v2/rel"));
    const cb::formats::le::Module splitter = cb::formats::le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd"));
    const ow::Result r = ow::build_vxd(*world.env, splitter, "SPLITTER", world.dir.path() / "out");
    CB_CHECK(!r.ok);
    CB_CHECK_EQ(r.error, "OpenWatcom wasm/wlink not built");
    CB_CHECK(!std::filesystem::exists(world.dir.path() / "out"));   // nothing was written
    // a bundle build of a file that is not a bundle
    cb::fs::write_file(world.dir.path() / "not-a-bundle.vxd", cb::fs::read_file(cb::testing::data_path("formats/testdata/splitter.vxd")));
    const ow::BundleResult b = ow::build_w3(*world.env, world.dir.path() / "not-a-bundle.vxd", world.dir.path() / "out");
    CB_CHECK(!b.ok);
    CB_CHECK_EQ(b.error, "not a W3 bundle");
});

CB_TEST("source.vxd.owbuild.the_command_says_what_happened_to_the_first_driver", [] {
    OwWorld world(ow::tools_from_settings(std::filesystem::path("nowhere"), "nowhere/open-watcom-v2/rel"));
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(world.dir.path() / "corpus")};
    cb::fs::make_dirs(world.dir.path() / "corpus");
    cb::base::Products products;
    products.target = "win95";
    products.target_folder = world.dir.path() / "corpus";
    products.checkout_root = world.dir.path() / "CodeBreaker";
    products.state = world.dir.path() / "state";
    const cb::base::Corpus corpus(products);
    const ow::CommandEnvironment env{*world.env, corpus, world.dir.path() / "default-out"};
    const std::string splitter = cb::fs::to_text(cb::testing::data_path("formats/testdata/splitter.vxd"));
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(ow::main(env, std::vector<std::string>{splitter, "--out", cb::fs::to_text(world.dir.path() / "out")}), cb::kExitOk);
        CB_CHECK_EQ(cap.out(), "splitter: FAIL OpenWatcom wasm/wlink not built\n");
    }
    {
        cb::testing::CapturedStd cap;   // (a name the corpus does not know is taken as a path, which is not there)
        CB_CHECK_THROWS_AS(cb::Error, ow::main(env, std::vector<std::string>{"nonesuch.vxd"}));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(ow::main(env, std::vector<std::string>{}), cb::kExitUsage);
    }
});

CB_TEST("source.vxd.owbuild.splitter_builds_with_the_real_tools", [] {
    // only where the OpenWatcom tree is set up (CB_OPENWATCOM or settings.json); the byte-exact objects come from x86asm
    const cb::config::Config& config = cb::config::current();
    const ow::Tools tools = ow::tools_from_settings(config.checkout_root(), config.text("openwatcom"));
    std::error_code ec;
    if (!std::filesystem::is_regular_file(tools.wasm, ec) || !std::filesystem::is_regular_file(tools.wlink, ec)) return;
    OwWorld world(tools);
    const vx::masm::Toolchain nasm_tools = vx::masm::Toolchain::standard({}, config.checkout_root());
    const vx::link::Environment link{world.names, nasm_tools, world.link->link};
    const ow::Environment env{link, tools};
    const cb::formats::le::Module splitter = cb::formats::le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd"));
    const ow::Result r = ow::build_vxd(env, splitter, "SPLITTER", world.dir.path() / "out");
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.objects, std::size_t{1});
    CB_CHECK(std::filesystem::is_regular_file(world.dir.path() / "out" / "splitter" / "splitter.vxd"));
    const cb::formats::le::Module built = cb::formats::le::Module::open(world.dir.path() / "out" / "splitter" / "splitter.vxd");
    CB_CHECK_EQ(built.objects().size(), std::size_t{1});
});
