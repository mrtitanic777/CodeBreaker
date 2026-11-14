// toolrun: runs the commands the switch-over moves off the old `codebreaker_native` extension and the `crypto` tools, with the surroundings the
// application's table will give them (the settings and the corpus), so each can be run on real inputs and timed.
// Not built by default (`cmake --build <dir> --target cb_toolrun`).
//
//     toolrun [--checkout DIR] <crypto <tool> ... | xbox-disc ... | xex-basic ... | xenon-config ... | switch-tables ... | corpus-executables>
//
// The key store folder, upstream's bin/ and the SDK are the settings `xbox360_keys`, `xbox360_crypto_bin` and `xbox360_sdk` (Keys/, bin/ and the SDK's
// default place when they are not set).
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/base/corpus_executables.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/xbox/crypto/tools.hpp"
#include "cb/formats/xbox/disc_command.hpp"
#include "cb/recompile/xenon_tools.hpp"

namespace {

namespace stdfs = std::filesystem;
using namespace cb;

std::shared_ptr<base::Corpus> corpus_of(const config::Config& config) {
    base::FormatProbes probes;
    probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
            case formats::detect::Lane::Pe: return base::Lane::Pe;
            case formats::detect::Lane::Ne: return base::Lane::Ne;
            case formats::detect::Lane::Le: return base::Lane::Le;
            case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<base::Corpus>(config.products(), std::move(probes));
}

}  // namespace

int main(int argc, char** argv) {
    std::vector<std::string> args(argv + 1, argv + argc);
    stdfs::path checkout;
    if (args.size() >= 2 && args[0] == "--checkout") {
        checkout = fs::from_text(args[1]);
        args.erase(args.begin(), args.begin() + 2);
    } else {
        checkout = config::find_checkout();
    }
    const config::Config config = config::Config::load(checkout);

    formats::xbox::crypto::Environment crypto_env;
    if (const auto keys = config.path("xbox360_keys")) crypto_env.keys = formats::xbox::crypto::keystore::KeyStore(*keys);
    if (const auto bin = config.path("xbox360_crypto_bin")) crypto_env.bin_dir = *bin;
    if (const auto sdk = config.path("xbox360_sdk")) crypto_env.sdk_bin_dir = *sdk / "bin" / "win32";

    const recompile::xenon_tools::Environment tools_env{config};
    std::vector<cli::Command> commands = {formats::xbox::crypto::command(crypto_env), formats::xbox::disc_command::command(),
                                          base::corpus_executables::command(base::corpus_executables::Environment{corpus_of(config)})};
    for (cli::Command& c : recompile::xenon_tools::commands(tools_env)) commands.push_back(std::move(c));
    try {
        return cli::Cli("toolrun", std::move(commands)).run(args);
    } catch (const std::exception& e) {
        cb::err("{}\n", e.what());
        return exit_code_of(e);
    }
}
