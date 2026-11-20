// cb/app/convert_command.cpp -- `codebreaker convert FILE [--to LANG] [--out DIR]`: the window's pipeline on the command line, its stages and percentages
// printed; and the little a per-user setting file keeps for the window (the language last chosen).
#include <atomic>
#include <format>
#include <span>

#include <nlohmann/json.hpp>

#include "cb/app/convert.hpp"
#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"

namespace cb::app::convert {

std::filesystem::path converted_root() {
    const config::Config& config = config::current();
    return config.path("converted").value_or(std::filesystem::path());
}

// ---- the per-user setting ----------------------------------------------------------------------------------------------------------------

namespace {

// <application data>/CodeBreaker/app.json: %LOCALAPPDATA% on Windows, $XDG_CONFIG_HOME or ~/.config elsewhere; CB_APP_SETTINGS names a file instead.
std::filesystem::path cv_settings_file() {
    if (const auto named = fs::get_env("CB_APP_SETTINGS"); named && !named->empty()) return fs::from_text(*named);
    if (const auto local = fs::get_env("LOCALAPPDATA"); local && !local->empty()) return fs::from_text(*local) / "CodeBreaker" / "app.json";
    if (const auto xdg = fs::get_env("XDG_CONFIG_HOME"); xdg && !xdg->empty()) return fs::from_text(*xdg) / "codebreaker" / "app.json";
    if (const auto home = fs::get_env("HOME"); home && !home->empty()) return fs::from_text(*home) / ".config" / "codebreaker" / "app.json";
    return {};
}

}  // namespace

std::filesystem::path settings_file() { return cv_settings_file(); }

std::string remembered_target() {
    try {
        const std::filesystem::path file = cv_settings_file();
        std::error_code ec;
        if (file.empty() || !std::filesystem::is_regular_file(file, ec)) return "original";
        const Json settings = json::load(file);
        if (settings.is_object() && settings.contains("language") && settings["language"].is_string()) {
            const std::string id = settings["language"].get<std::string>();
            if (!id.empty()) return id;
        }
    } catch (const std::exception&) {   // (an unreadable setting is no setting)
    }
    return "original";
}

void remember_target(const std::string& id) {
    try {
        const std::filesystem::path file = cv_settings_file();
        if (file.empty()) return;
        Json settings = Json::object();
        std::error_code ec;
        if (std::filesystem::is_regular_file(file, ec)) {
            try {
                settings = json::load(file);
            } catch (const std::exception&) {
            }
            if (!settings.is_object()) settings = Json::object();
        }
        settings["language"] = id;
        fs::make_dirs(file.parent_path());
        json::save(file, settings);
    } catch (const std::exception&) {   // (not being able to remember is not an error)
    }
}

// ---- the command -----------------------------------------------------------------------------------------------------------------------------

namespace {

// The choices of `--to` for a file, one per line, unsupported ones with their reason.
std::string cv_choices(const std::filesystem::path& file) {
    std::string text;
    for (const translate::capability::Target& t : targets_for(file)) {
        text += std::format("  {:<9} {}", t.id, t.name);
        if (!t.supported) text += "  (not available: " + t.reason + ")";
        text += "\n";
    }
    return text;
}

constexpr const char* kCvUsage = "usage: codebreaker convert FILE [--to LANG] [--out DIR] [--languages]\n";

}  // namespace

int run_command(std::span<const std::string> args) {
    Request request;
    request.converted_root = converted_root();
    std::string to = "original";
    bool list_languages = false;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out" && i + 1 < args.size()) {
            request.out_dir = fs::from_text(args[++i]);
        } else if (args[i] == "--to" && i + 1 < args.size()) {
            to = args[++i];
        } else if (args[i] == "--languages") {
            list_languages = true;
        } else if (request.input.empty() && !args[i].starts_with("--")) {
            request.input = fs::from_text(args[i]);
        } else {
            out_text(kCvUsage);
            return 2;
        }
    }
    if (request.input.empty()) {
        out_text(
            "usage: codebreaker convert FILE [--to LANG] [--out DIR] [--languages]\n\n"
            "Turn one file back into source code, as the CodeBreaker window does. The stages the file passes through and the share of the\n"
            "work done are printed; the source goes next to FILE as <file name>-converted/ (or under the folder the setting converted\n"
            "names, or to DIR), in a folder that did not exist before: a numbered\n"
            "sibling (name_2, name_3 ...) when it would have. Exit code 0: done; 1: failed; 3: CodeBreaker cannot convert this kind of file.\n\n"
            "FILE may also be source code (.cs, .py ...): --to LANG says which language to translate it to (original, the default, keeps the\n"
            "file's own source and is for programs). The translator is asked which languages it can write for FILE; the translation is built and\n"
            "run beside the original, and the run is done only when both do the same (otherwise it fails and nothing is kept). --languages\n"
            "lists the languages the translator can write for FILE, with the reason for each it cannot, and converts nothing.\n");
        return 2;
    }
    if (list_languages) {   // what the translator can write for this file, asked of the translator (nothing is converted)
        out_text("The languages for " + fs::to_text(request.input.filename()) + ":\n" + cv_choices(request.input));
        return 0;
    }
    {
        // a language the translator has never heard of is a mistake in the command: list the choices
        const std::vector<translate::capability::Target> list = targets_for(request.input);
        if (translate::capability::find_target(list, to) == nullptr) {
            out_text("convert: unknown language \"" + to + "\". The choices for this file:\n" + cv_choices(request.input));
            out_text(kCvUsage);
            return 2;
        }
        to = translate::capability::find_target(list, to)->id;
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
    const Outcome done = convert(request, standard_planner(to), say, never);
    switch (done.state) {
        case State::Done:
            out("100%  Done\n{} files written to {}\n", done.files, fs::to_text(done.output));
            if (!done.checked.empty()) out("Checked: {}\n", done.checked);
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
