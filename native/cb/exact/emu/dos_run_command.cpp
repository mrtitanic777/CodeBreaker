#include "cb/exact/emu/dos_run_command.hpp"

#include <filesystem>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/base/config.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/emu/dos.hpp"
#include "cb/exact/emu/native_dosexec.hpp"

namespace cb::exact::emu::dos_run {

namespace stdfs = std::filesystem;

namespace {

// The file under `dir` a MemFS key names, as a path relative to `dir` ("" when it is not under it).
std::string under(const stdfs::path& dir, const std::string& key) {
    const std::string base = dos::MemFS::key(dir);
    if (key.size() <= base.size() || key.compare(0, base.size(), base) != 0) return {};
    std::string rest = key.substr(base.size());
    while (!rest.empty() && (rest.front() == '\\' || rest.front() == '/')) rest.erase(rest.begin());
    return rest;
}

int run(const cli::Args& a) {
    const stdfs::path program = stdfs::absolute(fs::from_text(a.required("program")));
    const stdfs::path dir = stdfs::absolute(fs::from_text(a.value_or("dir", ".")));
    std::string tail;
    for (const std::string& t : a.values("args")) tail += " " + t;

    const native_dosexec::Opened opened = native_dosexec::open(native_dosexec::locate(config::current()), native_dosexec::limits_from_environment());
    if (!opened.usable()) throw UnsupportedError(fs::to_text(program), 0, opened.why.empty() ? std::string("the DOS executive cannot run here") : opened.why.front());

    dos::MemFS files;   // (what the run writes stays here: the disk under the folder is only read)
    dos::RunSpec spec;
    spec.exe = program;
    spec.workdir = dir;
    spec.command_tail = tail;
    spec.trace_calls = a.flag("trace-calls");
    spec.files = &files;
    const dos::Result r = opened.executive->run(spec);

    nlohmann::json out;
    out["program"] = fs::to_text(program);
    out["exit"] = r.exit ? nlohmann::json(*r.exit) : nlohmann::json(nullptr);
    out["console"] = text::from_latin1(r.console);
    out["blocks"] = r.steps;
    nlohmann::json made = nlohmann::json::array();
    const std::optional<std::string> save = a.value("save");
    for (const auto& [key, bytes] : files.files()) {
        if (!bytes) continue;
        const std::string name = under(dir, key);
        std::error_code ec;
        const stdfs::path on_disk = dir / fs::from_text(name);
        if (!name.empty() && stdfs::is_regular_file(on_disk, ec) && fs::read_file(on_disk) == *bytes) continue;   // (read, not changed)
        made.push_back({{"file", name.empty() ? key : name}, {"bytes", bytes->size()}, {"sha1", crypto::to_hex(crypto::sha1(*bytes))}});
        if (save && !name.empty()) {
            const stdfs::path to = fs::from_text(*save) / fs::from_text(name);
            fs::make_dirs(to.parent_path());
            fs::write_file(to, *bytes);
        }
    }
    out["files"] = made;
    nlohmann::json gone = nlohmann::json::array();
    for (const std::string& key : files.gone()) gone.push_back(under(dir, key).empty() ? key : under(dir, key));
    out["deleted"] = gone;
    if (spec.trace_calls) {
        nlohmann::json calls = nlohmann::json::object();
        for (const auto& [address, n] : r.calls) calls[std::format("{:05X}", address)] = n;
        out["calls"] = calls;
    }
    const std::string text = out.dump(1);
    if (const auto file = a.value("out")) fs::write_text(fs::from_text(*file), text);
    else cb::out("{}\n", text);
    cb::err("{}\n", dos::Executive::summary(r));
    return r.exit ? static_cast<int>(*r.exit) : 3;
}

}  // namespace

cli::Command command() {
    return cli::Command{
        "dos-run",
        "run a DOS program on CodeBreaker's own DOS executive (no DOSBox-X): what it printed and which files it wrote",
        {cli::Option{"--dir", "", "its drive C: (default: the current folder); it is only read", cli::Arg::Value, "DIR"},
         cli::Option{"--save", "", "write the files it made or changed into this folder", cli::Arg::Value, "DIR"},
         cli::Option{"--trace-calls", "", "count the calls of each address"},
         cli::Option{"--out", "", "write the report (JSON) here instead of printing it", cli::Arg::Value, "FILE"},
         cli::Positional{"program", "the DOS program (.exe or .com)"},
         cli::Positional{"args", "its command line", false, true}},
        run,
        {},
        "Run",
        "",
        true};
}

}  // namespace cb::exact::emu::dos_run
