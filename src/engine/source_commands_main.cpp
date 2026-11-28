// engine/source_commands_main.cpp -- the source writers' commands as a program of their own: the proof harness of source/commands.hpp.
//
//     cb_source_commands vxd <tool> ...             cb_source_commands ne-asm <tool> ...
//     cb_source_commands driver-source <driver> ... cb_source_commands dos-source <program> ...
//     cb_source_commands dos-build <folder> ...     cb_source_commands driver-judge <rebuilt> <original>
//
// The application registers the same commands in its own table (cb/app/native_commands); this runs them over the settings of the checkout
// (CB_CHECKOUT_ROOT, CB_<SETTING>) and the corpus they name, and nothing else.
#include <memory>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/log.hpp"
#include "formats/detect.hpp"
#include "engine/source_commands.hpp"

namespace {

std::shared_ptr<cb::base::Corpus> corpus_of(const cb::config::Config& config) {
    cb::base::FormatProbes probes;
    probes.lane = [](const std::filesystem::path& p) -> std::optional<cb::base::Lane> {
        const auto lane = cb::formats::detect::lane(p);
        if (!lane) return std::nullopt;
        switch (*lane) {
        case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
        case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
        case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
        case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<cb::base::Corpus>(config.products(), std::move(probes));
}

int run(const std::vector<std::string>& args) {
    if (args.empty()) {
        cb::err("usage: cb_source_commands vxd|ne-asm|driver-source|dos-source|dos-build|driver-judge <arguments>\n");
        return cb::kExitUsage;
    }
    const cb::config::Config config = cb::config::current();
    const cb::engine::source_commands::Production production(config, corpus_of(config));
    const std::string& name = args.front();
    const std::vector<std::string> rest(args.begin() + 1, args.end());
    if (name == "vxd") return production.vxd()(rest);
    if (name == "ne-asm") return production.ne_asm()(rest);
    if (name == "driver-source") return production.driver_source()(rest);
    if (name == "dos-source") return production.dos_source()(rest);
    cb::cli::Command command;
    if (name == "dos-build") command = production.dos_build();
    else if (name == "driver-judge") command = production.driver_judge();
    else {
        cb::err("unknown command '{}'\n", name);
        return cb::kExitUsage;
    }
    return cb::cli::Cli("cb_source_commands", {command}).run(args);
}

}  // namespace

int main(int argc, char** argv) {
    int code = cb::kExitInternal;
    try {
        code = run(std::vector<std::string>(argv + 1, argv + argc));
    } catch (const cb::Error& e) {
        cb::flush_out();
        cb::err("{}\n", e.what());
        code = e.exit_code();
    } catch (const std::exception& e) {
        cb::flush_out();
        cb::err("internal error: {}\n", e.what());
    }
    cb::flush_out();
    return code;
}
