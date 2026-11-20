// xna/command.cpp -- see command.hpp.
#include "xna/command.hpp"

#include "base/converted.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "xna/xact.hpp"
#include "xna/xnb.hpp"

namespace cb::xna::command {

namespace {

namespace stdfs = std::filesystem;

// A program named by a setting; nothing when the setting is not there.
std::optional<stdfs::path> setting_path(const cb::config::Config& config, std::string_view key) {
    const std::string value = config.text(key);
    if (value.empty()) return std::nullopt;
    return fs::from_text(value);
}

// ffmpeg: the setting, else the program of that name (process::run finds it on PATH).
std::optional<stdfs::path> ffmpeg_of(const cb::config::Config& config) { return setting_path(config, "ffmpeg").value_or(stdfs::path("ffmpeg")); }

stdfs::path converted_root(const cb::config::Config& config) { return config.path("converted").value_or(config.checkout_root() / "Converted"); }

}  // namespace

run::Hooks run_hooks(const Environment& environment) {
    const xnb::Environment xnb_env{ffmpeg_of(environment.config)};
    run::Hooks hooks;
    hooks.convert_xnb = [xnb_env](const stdfs::path& file, const stdfs::path& out_dir) { return xnb::convert(file, out_dir, xnb_env); };
    hooks.convert_effect_file = [](const stdfs::path& file, const stdfs::path& out_dir) { return xnb::convert_effect_file(file, out_dir); };
    return hooks;
}

port::Hooks port_hooks(const Environment& environment) {
    const std::optional<stdfs::path> ffmpeg = ffmpeg_of(environment.config);
    port::Hooks hooks;
    hooks.decode_wave_bank = [ffmpeg](const stdfs::path& bank, const stdfs::path& out_dir) {
        xact::decode_bank(bank, out_dir, ffmpeg, [](std::string_view line) { cb::out("{}\n", line); });
    };
    hooks.wave_bank_name = [](const stdfs::path& bank) { return xact::read_wave_bank(fs::read_file(bank)).name; };
    hooks.sound_bank_cues = [](const stdfs::path& bank) { return xact::cues_json(xact::read_sound_bank(fs::read_file(bank))); };
    hooks.rename_readers = [](ByteView file, const std::function<std::string(const std::string&)>& rename) { return xnb::rename_readers(file, rename); };
    return hooks;
}

run::Environment run_environment(const Environment& environment) {
    const cb::config::Config& config = environment.config;
    run::Tools tools;
    tools.ilspycmd = setting_path(config, "ilspycmd");
    tools.de4dot = setting_path(config, "de4dot");
    tools.checkout_root = config.checkout_root();
    tools.state = config.state();
    return run::Environment{tools, base::converted::Store(converted_root(config))};
}

port::Tools port_tools(const Environment& environment) {
    const cb::config::Config& config = environment.config;
    port::Tools tools;
    tools.runtime_dir = config.checkout_root() / "config" / "xna" / "runtime";
    tools.apistubs_dir = config.checkout_root() / "config" / "xna" / "apistubs";
    tools.state = config.state();
    tools.overlays = config.path("xna_overlays");
    tools.mgfxc = setting_path(config, "mgfxc");
    tools.ffmpeg = setting_path(config, "ffmpeg");
    return tools;
}

int run(std::span<const std::string> args, const Environment& environment) {
    const base::converted::Store store(converted_root(environment.config));
    const port::Tools tools = port_tools(environment);
    const port::Hooks hooks = port_hooks(environment);
    const auto port_command = [&](std::span<const std::string> rest) { return port::main(rest, store, tools, hooks); };
    const int code = xna::run::main(args, run_environment(environment), run_hooks(environment), port_command);
    flush_out();
    return code;
}

std::function<int(std::span<const std::string>)> runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return run(args, environment); };
}

}  // namespace cb::xna::command
