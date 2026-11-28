// build/x86cc/x86cc.cpp -- see x86cc.hpp. The host side of the compilers' IDE interface (support/idedll.h), as wcc.exe's
// and wcc386.exe's own driver is (watcom/c/idedrv.c with maindrv.c, STATIC_LINKAGE): init, pass the init info, run once
// with the command line, fini; messages through the same callbacks, formatted by the same IdeMsgFormat. Each compiler's
// entry points carry its prefix (generated/<t>/cbnames.h renamed them, so both live in this program).
#include "build/x86cc/x86cc.hpp"

#include <cctype>
#include <chrono>
#include <cstring>
#include <mutex>
#include <optional>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"

#ifdef _WIN32
extern "C" {
#include "build/x86cc/support/idedll.h"
#include "core/working_folder.hpp"

#define CB_WCC_ENTRIES(p)                                                                                                  \
    IDEBool IDEAPI p##IDEInitDLL(IDECBHdl cbhdl, IDECallBacks* cb, IDEDllHdl* hdl);                                      \
    IDEBool IDEAPI p##IDEPassInitInfo(IDEDllHdl hdl, IDEInitInfo* info);                                                 \
    void IDEAPI p##IDEFiniDLL(IDEDllHdl hdl);                                                                            \
    int IDEAPI p##IDERunYourSelf(IDEDllHdl hdl, const char* opts, IDEBool* fatalerr);                                    \
    void p##IdeMsgFormat(IDECBHdl hdl, const IDEMsgInfo* info, char* buffer, size_t len, IDEMsgInfoFn displayer);   \
    void IDEAPI p##IDEFreeRun(IDEDllHdl hdl);
CB_WCC_ENTRIES(wcci86_)
CB_WCC_ENTRIES(wcc386_)
}
// the bodies the next compiles take from their caller (front/stmt/cstmt.cpp: L2Statement)
extern "C" void wcci86_L2SetBodies(cb::build::x86cc::BodySource* bodies);
extern "C" void wcc386_L2SetBodies(cb::build::x86cc::BodySource* bodies);
#endif

namespace cb::build::x86cc {

namespace stdfs = std::filesystem;

#ifdef _WIN32

namespace {

std::mutex& the_lock() {
    static std::mutex m;
    return m;
}

struct Entries {
    IDEBool(IDEAPI* init)(IDECBHdl, IDECallBacks*, IDEDllHdl*);
    IDEBool(IDEAPI* pass_info)(IDEDllHdl, IDEInitInfo*);
    void(IDEAPI* fini)(IDEDllHdl);
    int(IDEAPI* run)(IDEDllHdl, const char*, IDEBool*);
    void (*format)(IDECBHdl, const IDEMsgInfo*, char*, size_t, IDEMsgInfoFn);
    void(IDEAPI* free_run)(IDEDllHdl);   // frees the run the compiler gave back at the end of a compile
    void (*set_bodies)(BodySource*);     // the bodies the caller gives for the next compiles (null: none)
    mutable IDEDllHdl spare = nullptr;   // the run of the last compile: new again, for the next (IDEInitDLL takes it)
};

const Entries& entries(Target t) {
    static const Entries i86{&wcci86_IDEInitDLL, &wcci86_IDEPassInitInfo, &wcci86_IDEFiniDLL, &wcci86_IDERunYourSelf,
                             &wcci86_IdeMsgFormat, &wcci86_IDEFreeRun, &wcci86_L2SetBodies};
    static const Entries i386{&wcc386_IDEInitDLL, &wcc386_IDEPassInitInfo, &wcc386_IDEFiniDLL, &wcc386_IDERunYourSelf,
                              &wcc386_IdeMsgFormat, &wcc386_IDEFreeRun, &wcc386_L2SetBodies};
    // (the runs kept go with the program)
    static const struct Free {
        const Entries& a;
        const Entries& b;
        ~Free() {
            for (const Entries* e : {&a, &b})
                if (e->spare != nullptr) e->free_run(e->spare);
        }
    } freer{i86, i386};
    (void)freer;
    return t == Target::i86 ? i86 : i386;
}

// What one run gathers, and the environment it sees. The host's handle (IDECBHdl) is this: the compiler hands it back in every
// callback.
struct Host {
    Run run;
    std::map<std::string, std::string> environment;
    std::string held;   // the value GetInfo hands back (it must outlive the call)
    const Entries* entries = nullptr;
};
Host& host_of(IDECBHdl hdl) { return *static_cast<Host*>(hdl); }

IDEBool IDEAPI print_message(IDECBHdl hdl, const char* message) {
    Host& h = host_of(hdl);
    h.run.err += message;
    h.run.err += '\n';
    return false;
}

IDEBool IDEAPI print_with_crlf(IDECBHdl hdl, const char* message) {
    host_of(hdl).run.err += message;
    return false;
}

IDEBool IDEAPI print_with_info(IDECBHdl hdl, IDEMsgInfo* info) {
    Host& h = host_of(hdl);
    char text[512];
    h.entries->format(hdl, info, text, sizeof(text), &print_with_info);
    switch (info->severity) {   // as the driver: banner, debug and plain notes on stdout; the rest on stderr
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
    Host& h = host_of(hdl);
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

// Zeroes the stack below the caller: the frames of the compiler that follow start on zeros, as a new process's do.
#if defined(_MSC_VER)
__declspec(noinline)
#else
__attribute__((noinline))
#endif
void clear_stack() {
    volatile char pad[256 * 1024];
    for (std::size_t i = 0; i < sizeof(pad); i += 64) std::memset(const_cast<char*>(pad) + i, 0, 64);
}

}  // namespace

bool available() { return true; }

Run compile(Target target, const std::vector<std::string>& args, const stdfs::path& folder,
            const std::map<std::string, std::string>& environment, BodySource* bodies) {
    std::lock_guard lock(the_lock());
    Host host;
    host.environment = environment;
    host.entries = &entries(target);

    IDECallBacks callbacks{};
    callbacks.PrintMessage = &print_message;
    callbacks.PrintWithCRLF = &print_with_crlf;
    callbacks.PrintWithInfo = &print_with_info;
    callbacks.GetInfo = &get_info;
    callbacks.ProgressMessage = &progress_message;

    // the command line the program would have after its own name (maindrv.c: _bgetcmd)
    const std::string command = process::command_line(args);

    clear_stack();   // (what a compile reads from a frame it never wrote is zero, as on a new process's stack)

    const WorkingFolder in(folder);   // (the compiler opens its files relative to the process's)
    IDEDllHdl dll = host.entries->spare;   // (the run kept from the last compile, if there was one)
    // (the compiler's state, every variable that was a global or a static of it, is made new by IDEInitDLL: the handle it returns
    // names it, and IDEFiniDLL frees it)
    if (host.entries->init(&host, &callbacks, &dll)) throw Error("x86cc: the compiler would not start");
    struct Fini {
        void (IDEAPI* fini)(IDEDllHdl);
        IDEDllHdl hdl;
        ~Fini() { if (hdl != nullptr) fini(hdl); }
    } fini{host.entries->fini, dll};
    host.entries->spare = dll;   // (IDEFiniDLL gives the run back new: it is the next compile's)
    IDEInitInfo info{};
    info.ver = IDE_CUR_INFO_VER;   // as the driver passes it (idedrv.c), its output never a console here
    info.ignore_env = 0;
    info.cmd_line_has_files = 1;
    info.console_output = 0;
    if (host.entries->pass_info(dll, &info)) throw Error("x86cc: the compiler refused its settings");
    IDEBool fatal = 0;
    struct Given {   // the caller's bodies, for this compile only
        void (*set)(BodySource*);
        Given(void (*s)(BodySource*), BodySource* b) : set(s) { set(b); }
        ~Given() { set(nullptr); }
    } given{host.entries->set_bodies, bodies};
    const int runcode = host.entries->run(dll, command.c_str(), &fatal);
    host.entries->fini(dll);
    fini.hdl = nullptr;
    host.run.exit_code = (fatal || runcode != 0) ? 1 : 0;   // maindrv.c: EXIT_SUCCESS only for IDEDRV_SUCCESS
    return std::move(host.run);
}

void install() {
    process::add_in_process([](const process::ProcessSpec& spec) -> std::optional<process::ProcessResult> {
        std::string program = stdfs::path(spec.argv[0]).filename().string();
        for (char& ch : program) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        Target target;
        if (program == "wcc.exe" || program == "wcc") {
            target = Target::i86;
        } else if (program == "wcc386.exe" || program == "wcc386") {
            target = Target::i386;
        } else {
            return std::nullopt;
        }
        if (const auto choice = fs::get_env("CB_WCC"); choice && *choice == "exe") return std::nullopt;
        const auto started = std::chrono::steady_clock::now();
        const Run r = compile(target, std::vector<std::string>(spec.argv.begin() + 1, spec.argv.end()),
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

Run compile(Target, const std::vector<std::string>&, const stdfs::path&, const std::map<std::string, std::string>&, BodySource*) {
    throw Error("x86cc: the in-process compilers are built on Windows only");
}

#endif

}  // namespace cb::build::x86cc
