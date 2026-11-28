// sdk/decompile.cpp -- decompile.h over the one decompiler (decompile/decompiler), wired by the engine's lane kit.
#include <format>
#include <mutex>
#include <string>
#include <variant>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "engine/lane_kit.hpp"
#include "sdk/runtime.hpp"

namespace cb::sdk_impl {
namespace {

namespace dc = decompile::decompiler;

// The options a call was given: NULL, or a cb_decompile_options of this version asking for C.
cb_status decompile_options_check(std::string_view call, const cb_decompile_options* options) {
    if (options == nullptr) return CB_OK;
    if (options->struct_size != sizeof(cb_decompile_options))
        return fail(CB_ERROR_INVALID_ARGUMENT, call,
                    std::format("options.struct_size is {}, this library takes {}: make the options with CB_DECOMPILE_OPTIONS_INIT", options->struct_size,
                                sizeof(cb_decompile_options)));
    if (options->language != nullptr && text::lower(options->language) != "c")
        return fail(CB_ERROR_UNSUPPORTED, call, std::format("the language \"{}\" is not written by this version: ask for \"c\" (or NULL)", options->language));
    return CB_OK;
}

// The decompiler, when it can run.
cb_status decompiler_ready(std::string_view call, dc::Decompiler*& out) {
    dc::Decompiler& d = kit().decompiler();
    if (!d.available())
        return fail(CB_ERROR_NOT_FOUND, call,
                    std::format("the native decompiler cannot run: {} and the processor descriptions in {} must both be there (an installed SDK "
                                "has them in bin/ and share/codebreaker/languages)",
                                fs::to_text(d.context().settings.program), fs::to_text(d.context().settings.languages)));
    out = &d;
    return CB_OK;
}

// The binary's whole C, made once and kept with it; CB_ERROR_UNSUPPORTED for a file the decompiler does not take.
cb_status module_c(std::string_view call, cb_binary& b, dc::Decompiler& d, const std::string*& text) {
    if (!b.module_c) {
        dc::AsC c;
        try {
            c = d.as_c(b.file);
        } catch (const UsageError& e) {
            return fail(CB_ERROR_UNSUPPORTED, call, std::format("{} is not an executable this version decompiles ({})", b.path, e.what()));
        }
        b.module_info = cb_module_info{c.decompiled, c.found, c.library};
        b.module_c = std::move(c.text);
    }
    text = &*b.module_c;
    return CB_OK;
}

bool x86_pe(const cb_binary& b) {
    const auto* pe = std::get_if<formats::inspect::PeHeaders>(&b.description.headers);
    return pe != nullptr && pe->machine_id == 0x014C;
}

}  // namespace
}  // namespace cb::sdk_impl

using namespace cb::sdk_impl;

extern "C" {

cb_status cb_decompiler_check(void) {
    return guarded("cb_decompiler_check", []() -> cb_status {
        if (const cb_status s = require_init("cb_decompiler_check"); s != CB_OK) return s;
        cb::decompile::decompiler::Decompiler* d = nullptr;
        return decompiler_ready("cb_decompiler_check", d);
    });
}

cb_status cb_module_decompile(cb_binary* binary, const cb_decompile_options* options, cb_string* out, cb_module_info* info) {
    if (out != nullptr) *out = cb_string{nullptr, 0};
    return guarded("cb_module_decompile", [&]() -> cb_status {
        if (const cb_status s = require_init("cb_module_decompile"); s != CB_OK) return s;
        if (binary == nullptr || out == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_module_decompile", "the binary or the out string is NULL");
        if (const cb_status s = decompile_options_check("cb_module_decompile", options); s != CB_OK) return s;
        std::lock_guard hold(decompile_lock());
        cb::decompile::decompiler::Decompiler* d = nullptr;
        if (const cb_status s = decompiler_ready("cb_module_decompile", d); s != CB_OK) return s;
        const std::string* text = nullptr;
        if (const cb_status s = module_c("cb_module_decompile", *binary, *d, text); s != CB_OK) return s;
        *out = make_string(*text);
        if (info != nullptr) *info = binary->module_info;
        return CB_OK;
    });
}

cb_status cb_function_decompile(const cb_function* function, const cb_decompile_options* options, cb_string* out) {
    if (out != nullptr) *out = cb_string{nullptr, 0};
    return guarded("cb_function_decompile", [&]() -> cb_status {
        if (const cb_status s = require_init("cb_function_decompile"); s != CB_OK) return s;
        if (function == nullptr || out == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_function_decompile", "the function or the out string is NULL");
        if (const cb_status s = decompile_options_check("cb_function_decompile", options); s != CB_OK) return s;
        cb_binary& b = *function->owner;
        const cb_address at = function->address;
        std::lock_guard hold(decompile_lock());
        cb::decompile::decompiler::Decompiler* d = nullptr;
        if (const cb_status s = decompiler_ready("cb_function_decompile", d); s != CB_OK) return s;
        if (x86_pe(b) && at.style == CB_ADDRESS_LINEAR) {
            // a 32-bit x86 function by itself: the module made compilable for that one address
            const std::vector<cb::decompile::decompiler::Va> starts{static_cast<cb::decompile::decompiler::Va>(at.offset)};
            const cb::decompile::c_text::Compilable32 made = d->compilable32(b.file, starts);
            const auto name = made.by_va.find(starts.front());
            if (name == made.by_va.end() || !made.texts.contains(name->second))
                return fail(CB_ERROR_NOT_FOUND, "cb_function_decompile",
                            std::format("{}: the decompiler made no function at 0x{:x} (is it the start of one? cb_binary_function lists those the "
                                        "file names)",
                                        b.path, at.offset));
            *out = make_string(made.texts.at(name->second));
            return CB_OK;
        }
        // any other kind: the function cut out of its module's C, by the address the decompiler names it with (sub_10001000, sub_10_0124: the
        // segment as it writes it, in decimal), else by its own name (an export is written under the name it is exported by)
        const std::string place = at.style == CB_ADDRESS_LINEAR ? std::format("{:x}", at.offset) : std::format("{}:{:x}", at.segment, at.offset);
        const std::string* text = nullptr;
        if (const cb_status s = module_c("cb_function_decompile", b, *d, text); s != CB_OK) return s;
        std::optional<std::string> cut = cb::decompile::decompiler::function_in(*text, place);
        if (!cut && !function->name.starts_with("sub_")) cut = cb::decompile::decompiler::function_in(*text, function->name);
        if (!cut)
            return fail(CB_ERROR_NOT_FOUND, "cb_function_decompile",
                        std::format("{}: no function at {} ({}) in the module's C (cb_module_decompile gives all of it)", b.path, place, function->name));
        *out = make_string(*cut);
        return CB_OK;
    });
}

}  // extern "C"
