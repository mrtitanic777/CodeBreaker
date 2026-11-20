#include "build/dos_tools.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::build::dos_tools {

namespace {

// Text written for DOS: every line end is CR LF (one already CR LF stays one).
std::string dos_text(std::string_view s) {
    std::string out;
    out.reserve(s.size() + s.size() / 16);
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\n' && (i == 0 || s[i - 1] != '\r')) out += '\r';
        out += s[i];
    }
    return out;
}

// The DOSBox mounts of a spec: its own, plus the work folder as d: unless one is named.
std::vector<DosMount> mounts_of(const DosRunSpec& spec, const std::filesystem::path& work) {
    std::vector<DosMount> mounts = spec.mounts;
    const bool has_d = std::any_of(mounts.begin(), mounts.end(), [](const DosMount& m) { return m.drive == "d"; });
    if (!has_d) mounts.push_back({"d", work});
    return mounts;
}

std::string dos_path(const std::filesystem::path& p) {
    return text::replace_all(fs::to_text(p), "/", "\\");
}

}  // namespace

Emulator Emulator::from_config(const config::Config& config) {
    Emulator e;
    if (const auto path = config.path("dosbox_x")) e.dosbox_x = *path;
    const auto forbidden = fs::get_env("CB_NO_DOSBOX");
    e.forbidden = forbidden && !forbidden->empty();
    return e;
}

std::optional<std::filesystem::path> Emulator::program() const {
    if (forbidden || dosbox_x.empty()) return std::nullopt;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(dosbox_x, ec)) return std::nullopt;
    return dosbox_x;
}

std::optional<std::filesystem::path> masm_root(const config::Config& config) {
    const auto root = config.path("masm");
    if (!root) return std::nullopt;
    std::error_code ec;
    if (!std::filesystem::is_regular_file(*root / "BIN" / "MASM.EXE", ec)) return std::nullopt;
    return root;
}

std::string batch_text(const DosRunSpec& spec) {
    std::string out = "@echo off\r\n";
    for (const std::string& line : spec.commands) out += line + "\r\n";
    out += "echo _DONE_ > done.txt\r\n";
    return out;
}

std::string dosbox_conf(const DosRunSpec& spec, const std::filesystem::path& work) {
    const std::vector<DosMount> mounts = mounts_of(spec, work);
    std::string conf = "[cpu]";
    if (spec.cpu.empty()) {
        conf += "\ncycles=20000";
    } else {
        for (const auto& [k, v] : spec.cpu) conf += "\n" + k + "=" + v;
    }
    if (!spec.dos.empty()) {
        conf += "\n[dos]";
        for (const auto& [k, v] : spec.dos) conf += "\n" + k + "=" + v;
    }
    conf += "\n[autoexec]";
    for (const DosMount& m : mounts) conf += "\nmount " + m.drive + " \"" + dos_path(m.folder) + "\"";
    for (const auto& [k, v] : spec.variables) conf += "\nset " + k + "=" + v;
    // the last drive listed is the current one
    conf += "\n" + mounts.back().drive + ":\ncall BLD.BAT\nexit\n";
    return dos_text(conf);
}

DosRunResult DosboxRunner::run(const DosRunSpec& spec) const {
    DosRunResult result;
    const auto dosbox = emulator_.program();
    if (!dosbox) return result;

    const std::filesystem::path work =
        spec.work && !spec.work->empty() ? *spec.work : fs::ScopedTempDir("cb_dos").release();
    fs::make_dirs(work);
    result.work = work;

    for (const auto& [name, data] : spec.files) {
        if (const auto* text = std::get_if<std::string>(&data)) fs::write_text(work / name, dos_text(*text));
        else fs::write_file(work / name, std::get<Bytes>(data));
    }
    fs::write_text(work / "BLD.BAT", batch_text(spec));
    fs::write_text(work / "r.conf", dosbox_conf(spec, work));

    // a stale artifact (or the done marker) from an earlier run in this folder would pass for this run's
    std::error_code ec;
    for (const std::string& name : spec.artifacts) std::filesystem::remove(work / name, ec);
    std::filesystem::remove(work / "done.txt", ec);

    process::ProcessSpec p;
    p.argv = {fs::to_text(*dosbox), "-conf", fs::to_text(work / "r.conf"), "-exit"};
    if (!fs::get_env("SDL_VIDEODRIVER")) p.env["SDL_VIDEODRIVER"] = "dummy";
    p.out = process::Capture::Discard;
    p.err = process::Capture::Discard;
    p.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(spec.timeout);
    try {
        const process::ProcessResult r = process::run(p);
        if (r.timed_out) return result;
    } catch (const ToolError&) {
        return result;   // DOSBox could not be started: nothing was made
    }

    for (const std::string& name : spec.artifacts) {
        const std::filesystem::path file = work / name;
        if (std::filesystem::exists(file, ec)) result.made[name] = fs::read_file(file);
    }
    result.ok = result.made.size() == spec.artifacts.size();
    return result;
}

}  // namespace cb::build::dos_tools
