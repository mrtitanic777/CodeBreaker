// sdk/runtime.cpp -- cb.h: the version, cb_init / cb_shutdown, the status names, cb_last_error and cb_string.
#include "sdk/runtime.hpp"

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <format>
#include <new>
#include <stdexcept>
#include <system_error>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/dynlib.hpp"
#include "core/fs.hpp"
#include "engine/lane_kit.hpp"
#include "sdk/module_folder.hpp"

namespace cb::sdk_impl {
namespace {

namespace stdfs = std::filesystem;

struct Runtime {
    std::mutex lock;                       // cb_init, cb_shutdown, making the kit
    std::atomic<int> users{0};
    stdfs::path root, state;
    std::unique_ptr<engine::LaneKit> kit;
    std::mutex decompile;
};

Runtime& runtime() {
    static Runtime r;
    return r;
}

thread_local std::string last_error;

// The folder CodeBreaker's data is in, when cb_init is not told: CB_CHECKOUT_ROOT; the installed layout (bin/cbsdk.dll beside
// share/codebreaker); else the checkout this library was built in.
stdfs::path default_root() {
    if (const auto env = fs::get_env("CB_CHECKOUT_ROOT"); env && !env->empty()) return fs::from_text(*env);
    std::error_code ec;
    const stdfs::path installed = module_folder().parent_path() / "share" / "codebreaker";
    if (stdfs::is_directory(installed / "config", ec)) return installed;
    return config::find_checkout();
}

stdfs::path default_state() {
    if (const auto env = fs::get_env("CB_STATE"); env && !env->empty()) return fs::from_text(*env);
#ifdef _WIN32
    if (const auto local = fs::get_env("LOCALAPPDATA"); local && !local->empty()) return fs::from_text(*local) / "CodeBreaker" / "sdk";
#else
    if (const auto cache = fs::get_env("XDG_CACHE_HOME"); cache && !cache->empty()) return fs::from_text(*cache) / "codebreaker" / "sdk";
    if (const auto home = fs::get_env("HOME"); home && !home->empty()) return fs::from_text(*home) / ".cache" / "codebreaker" / "sdk";
#endif
    return stdfs::temp_directory_path() / "codebreaker-sdk";
}

// Sets a setting the engines read for this process, unless it is set already.
void set_default(std::string_view name, const stdfs::path& value) {
    if (const auto env = fs::get_env(name); env && !env->empty()) return;
    fs::set_env(name, fs::to_text(value));
}

bool same_folder(const stdfs::path& a, const stdfs::path& b) {
    std::error_code ec;
    return stdfs::weakly_canonical(a, ec) == stdfs::weakly_canonical(b, ec);
}

}  // namespace

bool initialized() { return runtime().users.load() > 0; }
const std::filesystem::path& data_root() { return runtime().root; }
const std::filesystem::path& state_dir() { return runtime().state; }
std::mutex& decompile_lock() { return runtime().decompile; }

engine::LaneKit& kit() {
    Runtime& r = runtime();
    std::lock_guard hold(r.lock);
    if (!r.kit) r.kit = std::make_unique<engine::LaneKit>(config::Config::load(r.root));
    return *r.kit;
}

cb_status fail(cb_status status, std::string_view call, std::string_view message) {
    last_error = std::format("{}: {}", call, message);
    return status;
}

cb_status require_init(std::string_view call) {
    if (initialized()) return CB_OK;
    return fail(CB_ERROR_NOT_INITIALIZED, call, "the library is not set up: call cb_init(NULL) once before any other call (cb.h)");
}

cb_status from_exception(std::string_view call) noexcept {
    try {
        try {
            throw;
        } catch (const NotFoundError& e) {
            return fail(CB_ERROR_NOT_FOUND, call, e.what());
        } catch (const IoError& e) {
            return fail(CB_ERROR_IO, call, e.what());
        } catch (const UnsupportedError& e) {
            return fail(CB_ERROR_UNSUPPORTED, call, e.what());
        } catch (const FormatError& e) {
            return fail(CB_ERROR_FORMAT, call, e.what());
        } catch (const ParseError& e) {
            return fail(CB_ERROR_FORMAT, call, e.what());
        } catch (const ToolError& e) {
            return fail(CB_ERROR_TOOL, call, e.what());
        } catch (const UsageError& e) {
            return fail(CB_ERROR_INVALID_ARGUMENT, call, e.what());
        } catch (const std::bad_alloc&) {
            return fail(CB_ERROR_INTERNAL, call, "out of memory");
        } catch (const std::exception& e) {
            return fail(CB_ERROR_INTERNAL, call, std::format("{} (a bug in CodeBreaker: please report it with this message)", e.what()));
        } catch (...) {
            return fail(CB_ERROR_INTERNAL, call, "an unknown exception (a bug in CodeBreaker: please report it)");
        }
    } catch (...) {
        return CB_ERROR_INTERNAL;   // (formatting the message failed: out of memory)
    }
}

cb_string make_string(std::string_view text) {
    if (text.empty()) return cb_string{nullptr, 0};
    char* data = static_cast<char*>(std::malloc(text.size() + 1));
    if (data == nullptr) throw std::bad_alloc();
    std::memcpy(data, text.data(), text.size());
    data[text.size()] = '\0';
    return cb_string{data, text.size()};
}

}  // namespace cb::sdk_impl

using namespace cb::sdk_impl;

extern "C" {

uint32_t cb_version(void) { return CB_SDK_VERSION; }

const char* cb_version_string(void) { return CB_SDK_VERSION_STRING; }

const char* cb_status_name(cb_status status) {
    switch (status) {
        case CB_OK: return "CB_OK";
        case CB_ERROR_INVALID_ARGUMENT: return "CB_ERROR_INVALID_ARGUMENT";
        case CB_ERROR_NOT_INITIALIZED: return "CB_ERROR_NOT_INITIALIZED";
        case CB_ERROR_NOT_FOUND: return "CB_ERROR_NOT_FOUND";
        case CB_ERROR_IO: return "CB_ERROR_IO";
        case CB_ERROR_FORMAT: return "CB_ERROR_FORMAT";
        case CB_ERROR_UNSUPPORTED: return "CB_ERROR_UNSUPPORTED";
        case CB_ERROR_TOOL: return "CB_ERROR_TOOL";
        case CB_ERROR_BUFFER_TOO_SMALL: return "CB_ERROR_BUFFER_TOO_SMALL";
        case CB_ERROR_INTERNAL: return "CB_ERROR_INTERNAL";
    }
    return "CB_ERROR_UNKNOWN";
}

const char* cb_last_error(void) { return last_error.c_str(); }

cb_status cb_init(const cb_init_options* options) {
    return guarded("cb_init", [&]() -> cb_status {
        if (options != nullptr && options->struct_size != sizeof(cb_init_options))
            return fail(CB_ERROR_INVALID_ARGUMENT, "cb_init",
                        std::format("options.struct_size is {}, this library takes {}: make the options with CB_INIT_OPTIONS_INIT, and build "
                                    "against the headers of the library you run with (cb_version)",
                                    options->struct_size, sizeof(cb_init_options)));
        const char* given_root = options != nullptr ? options->data_root : nullptr;
        const char* given_state = options != nullptr ? options->state_dir : nullptr;
        Runtime& r = runtime();
        std::lock_guard hold(r.lock);
        if (r.users.load() > 0) {
            if ((given_root != nullptr && !same_folder(stdfs::absolute(cb::fs::from_text(given_root)), r.root)) ||
                (given_state != nullptr && !same_folder(stdfs::absolute(cb::fs::from_text(given_state)), r.state)))
                return fail(CB_ERROR_INVALID_ARGUMENT, "cb_init",
                            std::format("the library is already set up with data root {} and state folder {}: a later cb_init may not name "
                                        "others (pass NULL, or the same folders)",
                                        cb::fs::to_text(r.root), cb::fs::to_text(r.state)));
            ++r.users;
            return CB_OK;
        }
        const stdfs::path root = stdfs::absolute(given_root != nullptr ? cb::fs::from_text(given_root) : default_root());
        std::error_code ec;
        if (!stdfs::is_directory(root / "config", ec))
            return fail(CB_ERROR_NOT_FOUND, "cb_init",
                        std::format("{} has no config folder: give cb_init_options.data_root the folder the SDK installed as share/codebreaker "
                                    "(or a CodeBreaker checkout)",
                                    cb::fs::to_text(root)));
        const stdfs::path state = stdfs::absolute(given_state != nullptr ? cb::fs::from_text(given_state) : default_state());
        stdfs::create_directories(state, ec);
        if (!stdfs::is_directory(state, ec))
            return fail(CB_ERROR_IO, "cb_init",
                        std::format("the state folder {} cannot be made ({}): give cb_init_options.state_dir a folder this program may write to",
                                    cb::fs::to_text(state), ec.message()));
        // the settings the engines read (base/config: CB_<KEY>), for this process
        set_default("CB_CHECKOUT_ROOT", root);
        set_default("CB_STATE", state);
        set_default("CB_KNOWLEDGE_DB", state / "knowledge.v2.sqlite");
        if (stdfs::is_directory(root / "languages", ec)) set_default("CB_LANGUAGES_DIR", root / "languages");
        const stdfs::path bin = module_folder();
        if (stdfs::is_regular_file(bin / "decompiler.exe", ec)) set_default("CB_DECOMPILER_BIN", bin);
        if (stdfs::is_regular_file(bin / cb::DynLib::file_name_for("capstone"), ec)) set_default("CB_THIRD_PARTY_BIN", bin);
        r.root = root;
        r.state = state;
        ++r.users;
        return CB_OK;
    });
}

void cb_shutdown(void) {
    Runtime& r = runtime();
    std::unique_ptr<cb::engine::LaneKit> last;
    {
        std::lock_guard hold(r.lock);
        if (r.users.load() == 0) return;
        if (--r.users == 0) last = std::move(r.kit);
    }
    std::lock_guard hold(r.decompile);   // (a decompile that is still running finishes first)
    last.reset();
}

void cb_string_free(cb_string* string) {
    if (string == nullptr) return;
    std::free(string->data);
    string->data = nullptr;
    string->size = 0;
}

}  // extern "C"
