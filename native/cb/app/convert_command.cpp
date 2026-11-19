// cb/app/convert_command.cpp -- `codebreaker convert FILE [--out DIR]`: the window's pipeline on the command line, its stages and percentages printed.
#include <atomic>
#include <format>
#include <span>

#include "cb/app/convert.hpp"
#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::app::convert {

std::filesystem::path converted_root() {
    const config::Config& config = config::current();
    return config.path("converted").value_or(config.checkout_root() / "Converted");
}

int run_command(std::span<const std::string> args) {
    Request request;
    request.converted_root = converted_root();
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out" && i + 1 < args.size()) {
            request.out_dir = fs::from_text(args[++i]);
        } else if (request.input.empty() && !args[i].starts_with("--")) {
            request.input = fs::from_text(args[i]);
        } else {
            out_text("usage: codebreaker convert FILE [--out DIR]\n");
            return 2;
        }
    }
    if (request.input.empty()) {
        out_text(
            "usage: codebreaker convert FILE [--out DIR]\n\n"
            "Turn one file back into source code, as the CodeBreaker window does. The stages the file passes through and the share of the\n"
            "work done are printed; the source goes to Converted/<file name>/ (or DIR), in a folder that did not exist before: a numbered\n"
            "sibling (name_2, name_3 ...) when it would have. Exit code 0: done; 1: failed; 3: CodeBreaker cannot convert this kind of file.\n");
        return 2;
    }
    State last_state = State::Idle;
    int last_percent = -1;
    const ProgressFn say = [&](const Progress& p) {
        if (is_final(p.state)) return;   // the closing line says it
        if (p.state == last_state && p.percent == last_percent) return;
        last_state = p.state;
        last_percent = p.percent;
        std::string line = std::format("{:3}%  {}", p.percent, p.label);
        if (!p.detail.empty()) line += ": " + p.detail;
        out("{}\n", line);
    };
    const std::atomic<bool> never{false};
    const Outcome done = convert(request, standard_planner(), say, never);
    switch (done.state) {
        case State::Done:
            out("100%  Done\n{} files written to {}\n", done.files, fs::to_text(done.output));
            return 0;
        case State::Unsupported:
            out("Unsupported: {}\n", done.message);
            return 3;
        case State::Cancelled:
            out("Cancelled\n");
            return 130;
        default:
            out("Failed: {}\n", done.message);
            return 1;
    }
}

}  // namespace cb::app::convert
