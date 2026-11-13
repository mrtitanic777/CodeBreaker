// cb-unity: alone (test helpers named for this module)
// A harness, not a test of behaviour: runs `identify` and `names` as the program would, over the real corpus and the real configuration, and
// writes what they print and how long each took, to compare with what the Python prints (docs: state/work/typed-port/_bugs/LIST.md's proof notes).
//
//     CB_PROOF_WORK=<dir> cb_tests_symbols --filter symbols.proof
//
// <dir>/commands.txt holds one run per line: `<output file name> <command> <arguments...>` (a command is identify or names). Each output goes to
// <dir>/<output file name>; <dir>/times.txt gets `<output file name> <milliseconds>`. Without CB_PROOF_WORK this passes without doing anything.
#include <chrono>
#include <sstream>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/detect.hpp"
#include "cb/symbols/identify.hpp"
#include "cb/symbols/names.hpp"

namespace stdfs = std::filesystem;

CB_TEST("symbols.proof.run_the_commands_listed_in_the_work_folder", [] {
    const std::optional<std::string> work = cb::fs::get_env("CB_PROOF_WORK");
    if (!work) return;
    const stdfs::path dir = cb::fs::from_text(*work);
    const cb::config::Config& config = cb::config::current();
    cb::base::FormatProbes probes;
    probes.lane = [](const stdfs::path& p) -> std::optional<cb::base::Lane> {
        const std::optional<cb::formats::detect::Lane> l = cb::formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
        case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
        case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
        case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
        case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
        }
        return std::nullopt;
    };
    cb::base::Corpus corpus(config.products(), probes);
    cb::symbols::Environment environment;
    environment.corpus = &corpus;
    environment.config = &config;
    // (`names --ordinals` and `identify --learn` write the checkout's authored files; a proof run writes beside its output)
    environment.ordinal_file = dir / "ordinal_names.json";
    environment.signature_file = dir / "runtime_signatures.json";
    std::istringstream lines(cb::fs::read_text(dir / "commands.txt"));
    std::string times;
    for (std::string line; std::getline(lines, line);) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::istringstream words(line);
        std::string file, command, word;
        if (!(words >> file >> command)) continue;
        std::vector<std::string> args{command};
        while (words >> word) args.push_back(word);
        const cb::cli::Command declared = command == "identify" ? cb::symbols::identify::command(environment) : cb::symbols::names::command(environment);
        const cb::cli::Cli program("codebreaker", {declared});
        std::string out;
        int code = 0;
        const auto started = std::chrono::steady_clock::now();
        {
            cb::testing::CapturedStd captured;
            code = program.run(args);
            out = captured.out();
        }
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started).count();
        cb::fs::write_file(dir / file, cb::ByteView(reinterpret_cast<const std::uint8_t*>(out.data()), out.size()));
        times += file + " " + std::to_string(ms) + " ms, exit " + std::to_string(code) + "\n";
    }
    cb::fs::write_file(dir / "times.txt", cb::ByteView(reinterpret_cast<const std::uint8_t*>(times.data()), times.size()));
});
