// sdk/runtime.hpp -- what every part of the SDK's implementation shares: the library's state (cb_init), the guard that turns an
// engine's exception into a cb_status and cb_last_error's message, and the handle types behind the opaque C names.
//
// src/sdk is the C interface of include/cb over the engines: it includes only their public faces (formats/inspect, source/processors,
// decompile/decompiler, engine/lane_kit; sdk.includes checks it) and adds no work of its own to a call: records point into the engines'
// own results, kept on the handle.
#pragma once

#include <cstdint>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/binary.h"
#include "cb/cb.h"
#include "cb/decompile.h"
#include "cb/disasm.h"
#include "formats/inspect.hpp"

namespace cb::engine {
class LaneKit;
}

// ---- the handles ----------------------------------------------------------------------------------------------------------

struct cb_function {
    cb_binary* owner = nullptr;
    std::string name;
    cb_address address{};
};

struct cb_binary {
    std::filesystem::path file;
    std::string path;                                // as the caller gave it (UTF-8)
    cb::formats::inspect::Description description;   // the readers' description; the records below point into it
    std::string arch;
    std::vector<cb_section> sections;
    std::vector<cb_import> imports;
    std::vector<cb_export> exports;
    std::deque<cb_function> functions;               // the named ones first (address order), then those function_at made
    std::size_t named = 0;
    std::optional<std::string> module_c;             // the whole module's C, once made (decompile.cpp)
    cb_module_info module_info{};
};

namespace cb::sdk_impl {

// ---- the library's state --------------------------------------------------------------------------------------------------

// Whether cb_init has been called more often than cb_shutdown.
bool initialized();
// The data root and the state folder the first cb_init decided.
const std::filesystem::path& data_root();
const std::filesystem::path& state_dir();
// The engines' wiring (the decompiler and what it asks of the rest), made on first use, freed by the last cb_shutdown. The decompile
// calls hold decompile_lock() while they use it.
engine::LaneKit& kit();
std::mutex& decompile_lock();

// ---- errors ---------------------------------------------------------------------------------------------------------------

// Sets this thread's cb_last_error to "<call>: <message>" and returns `status`.
cb_status fail(cb_status status, std::string_view call, std::string_view message);
// CB_ERROR_NOT_INITIALIZED with the message that says to call cb_init, or CB_OK.
cb_status require_init(std::string_view call);

// The status an engine's exception stands for, with its message recorded for `call`.
cb_status from_exception(std::string_view call) noexcept;

// Runs `body` (which returns a cb_status) with every exception it throws turned into a status and a message.
template <class Body>
cb_status guarded(std::string_view call, Body&& body) noexcept {
    try {
        return body();
    } catch (...) {
        return from_exception(call);
    }
}

// A cb_string holding a copy of `text` (allocated so that cb_string_free frees it).
cb_string make_string(std::string_view text);

// ---- addresses ------------------------------------------------------------------------------------------------------------

cb_address to_c(const formats::inspect::Address& address);
formats::inspect::Address from_c(const cb_address& address);

}  // namespace cb::sdk_impl
