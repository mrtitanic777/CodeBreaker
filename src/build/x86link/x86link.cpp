// build/x86link/x86link.cpp -- see x86link.hpp. The host side of wlink's IDE interface (support/watcom/h/idedll.h), as
// wlink.exe's own driver is (support/watcom/c/idedrv.c with maindrv.c): init, pass the init info, run once with the command
// line, fini; messages through the same callbacks, formatted by the same IdeMsgFormat.
#include "build/x86link/x86link.hpp"

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <optional>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/working_folder.hpp"

#ifdef _WIN32
extern "C" {
#include "build/x86link/support/watcom/h/idedll.h"   // (IdeMsgFormat: support/watcom/c/idemsgpr.c, the text wlink.exe prints)
}
#endif

namespace cb::build::x86link {

namespace stdfs = std::filesystem;

#ifdef _WIN32

namespace {

std::mutex& the_lock() {
    static std::mutex m;
    return m;
}

// What one run gathers, and the environment it sees (the callbacks have only the handle: this, as the IDECBHdl).
struct Host {
    Run run;
    std::map<std::string, std::string> environment;
    std::string held;   // the value GetInfo hands back (it must outlive the call)
};

IDEBool IDEAPI print_message(IDECBHdl hdl, const char* message) {
    Host& h = *static_cast<Host*>(hdl);
    h.run.err += message;
    h.run.err += '\n';
    return false;
}

IDEBool IDEAPI print_with_crlf(IDECBHdl hdl, const char* message) {
    static_cast<Host*>(hdl)->run.err += message;
    return false;
}

IDEBool IDEAPI print_with_info(IDECBHdl hdl, IDEMsgInfo* info) {
    Host& h = *static_cast<Host*>(hdl);
    char text[512];
    IdeMsgFormat(hdl, info, text, sizeof(text), &print_with_info);
    switch (info->severity) {   // as wlink.exe: banner, debug and plain notes on stdout; the rest on stderr
        case IDEMSGSEV_BANNER:
        case IDEMSGSEV_DEBUG:
        case IDEMSGSEV_NOTE_MSG: h.run.out += text; h.run.out += '\n'; break;
        default: h.run.err += text; h.run.err += '\n'; break;
    }
    return false;
}

IDEBool IDEAPI progress_message(IDECBHdl, const char*) { return false; }

IDEBool IDEAPI get_info(IDECBHdl hdl, IDEInfoType type, IDEGetInfoWParam extra, IDEGetInfoLParam lparam) {
    if (type != IDE_GET_ENV_VAR) return true;
    Host& h = *static_cast<Host*>(hdl);
    const std::string name = reinterpret_cast<const char*>(extra);
    std::optional<std::string> value;
    for (const auto& [k, v] : h.environment) {
        if (_stricmp(k.c_str(), name.c_str()) == 0) value = v;
    }
    if (!value) value = fs::get_env(name);
    auto* out = reinterpret_cast<const char**>(lparam);
    if (!value) {
        *out = nullptr;
        return true;
    }
    h.held = *value;
    *out = h.held.c_str();
    return false;
}

}  // namespace

bool available() { return true; }

Run link(const std::vector<std::string>& args, const stdfs::path& folder, const std::map<std::string, std::string>& environment) {
    std::lock_guard lock(the_lock());
    Host host;
    host.environment = environment;
    // wlink.exe reads its system definitions from wlink.lnk beside itself (found through PATH: cmdline.c, FindPath); in
    // this program that file is OpenWatcom's, <WATCOM>/binnt64/wlink.lnk, named through WLINK_LNK unless the run names one
    auto named = [&](const char* name) -> std::optional<std::string> {
        for (const auto& [k, v] : environment)
            if (_stricmp(k.c_str(), name) == 0) return v;
        return fs::get_env(name);
    };
    if (!named("WLINK_LNK")) {
        if (const auto watcom = named("WATCOM")) {
            for (const char* bin : {"binnt64", "binnt"}) {
                const stdfs::path lnk = fs::from_text(*watcom) / bin / "wlink.lnk";
                std::error_code ec;
                if (stdfs::is_regular_file(lnk, ec)) {
                    host.environment["WLINK_LNK"] = fs::to_text(lnk);
                    break;
                }
            }
        }
    }
    IDECallBacks callbacks{};
    callbacks.PrintMessage = &print_message;
    callbacks.PrintWithCRLF = &print_with_crlf;
    callbacks.PrintWithInfo = &print_with_info;
    callbacks.GetInfo = &get_info;
    callbacks.ProgressMessage = &progress_message;

    // the command line wlink.exe would have after its own name (maindrv.c: _bgetcmd), and never empty (an empty one makes
    // wlink read the process's own command line)
    std::string command = process::command_line(args);
    if (command.empty()) command = " ";

    const WorkingFolder in(folder);   // (wlink opens its files relative to the process's)
    IDEDllHdl dll = nullptr;
    if (IDEInitDLL(&host, &callbacks, &dll)) throw Error("x86link: the linker would not start");
    IDEInitInfo info{};
    info.ver = IDE_CUR_INFO_VER;   // as wlink.exe's driver passes it
    info.ignore_env = 0;
    info.cmd_line_has_files = 1;
    info.console_output = 0;
    if (IDEPassInitInfo(dll, &info)) {
        IDEFiniDLL(dll);
        throw Error("x86link: the linker refused its settings");
    }
    IDEBool fatal = 0;
    const int runcode = IDERunYourSelf(dll, command.c_str(), &fatal);
    IDEFiniDLL(dll);
    host.run.exit_code = (fatal || runcode != 0) ? 1 : 0;   // maindrv.c: EXIT_SUCCESS only for IDEDRV_SUCCESS
    return std::move(host.run);
}

void install() {
    process::add_in_process([](const process::ProcessSpec& spec) -> std::optional<process::ProcessResult> {
        std::string program = stdfs::path(spec.argv[0]).filename().string();
        for (char& ch : program) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        if (program != "wlink.exe" && program != "wlink") return std::nullopt;
        if (const auto choice = fs::get_env("CB_WLINK"); choice && *choice == "exe") return std::nullopt;
        const auto started = std::chrono::steady_clock::now();
        const Run r = link(std::vector<std::string>(spec.argv.begin() + 1, spec.argv.end()),
                           spec.cwd ? *spec.cwd : stdfs::current_path(), spec.env);
        process::ProcessResult out;
        out.exit_code = static_cast<std::uint32_t>(r.exit_code);
        out.out.assign(r.out.begin(), r.out.end());
        out.err.assign(r.err.begin(), r.err.end());
        out.elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started);
        return out;
    });
}

#else

bool available() { return false; }

void install() {}

Run link(const std::vector<std::string>&, const stdfs::path&, const std::map<std::string, std::string>&) {
    throw Error("x86link: the in-process linker is built on Windows only");
}

#endif

}  // namespace cb::build::x86link
