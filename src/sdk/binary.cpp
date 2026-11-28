// sdk/binary.cpp -- binary.h over the format readers' one description of a file (formats/inspect).
#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <memory>
#include <system_error>
#include <tuple>
#include <variant>

#include "core/fs.hpp"
#include "formats/inspect.hpp"
#include "sdk/runtime.hpp"

namespace cb::sdk_impl {

namespace inspect = formats::inspect;

cb_address to_c(const inspect::Address& address) {
    if (const auto* linear = std::get_if<std::uint64_t>(&address)) return cb_address{*linear, 0, CB_ADDRESS_LINEAR};
    if (const auto* s = std::get_if<inspect::SegmentAddress>(&address)) {
        if (s->style == inspect::SegmentAddress::Style::NeSegment && s->segment == 0) return cb_address{0, 0, CB_ADDRESS_NONE};   // (1-based: 0 is none, a DLL's CS:IP)
        const std::uint32_t style = s->style == inspect::SegmentAddress::Style::NeSegment ? CB_ADDRESS_NE_SEGMENT
                                    : s->style == inspect::SegmentAddress::Style::RealMode ? CB_ADDRESS_REAL_MODE
                                                                                            : CB_ADDRESS_LE_OBJECT;
        return cb_address{s->offset, s->segment, style};
    }
    return cb_address{0, 0, CB_ADDRESS_NONE};
}

inspect::Address from_c(const cb_address& address) {
    switch (address.style) {
        case CB_ADDRESS_LINEAR: return address.offset;
        case CB_ADDRESS_NE_SEGMENT:
            return inspect::SegmentAddress{address.segment, static_cast<std::uint32_t>(address.offset), inspect::SegmentAddress::Style::NeSegment};
        case CB_ADDRESS_REAL_MODE:
            return inspect::SegmentAddress{address.segment, static_cast<std::uint32_t>(address.offset), inspect::SegmentAddress::Style::RealMode};
        case CB_ADDRESS_LE_OBJECT:
            return inspect::SegmentAddress{address.segment, static_cast<std::uint32_t>(address.offset), inspect::SegmentAddress::Style::Driver};
        default: return std::monostate{};
    }
}

namespace {

bool same_address(const cb_address& a, const cb_address& b) { return a.style == b.style && a.segment == b.segment && a.offset == b.offset; }
bool before(const cb_address& a, const cb_address& b) { return std::tie(a.segment, a.offset) < std::tie(b.segment, b.offset); }

// The processor a PE's machine field names, by the names of the one disassembler (source/processors).
const char* pe_arch(std::uint16_t machine) {
    switch (machine) {
        case 0x014C: return "x86-32";
        case 0x8664: return "x86-64";
        case 0x01F0:
        case 0x01F1:
        case 0x01F2: return "ppc";
        case 0x01C0: return "arm";
        case 0x01C2:
        case 0x01C4: return "armv7t";
        case 0xAA64: return "a64";
        case 0x0166: return "mips";
        default: return "";
    }
}

std::string arch_of(const inspect::Description& d) {
    return std::visit(
        [&](const auto& h) -> std::string {
            using H = std::decay_t<decltype(h)>;
            if constexpr (std::is_same_v<H, inspect::PeHeaders>) return pe_arch(h.machine_id);
            else if constexpr (std::is_same_v<H, inspect::NeHeaders> || std::is_same_v<H, formats::detect::DosHeader>) return "x86-16";
            else if constexpr (std::is_same_v<H, inspect::LeHeaders> || std::is_same_v<H, inspect::W3Headers>) return "x86-32";
            else return d.kind == std::optional<std::string>("dos") ? "x86-16" : "";
        },
        d.headers);
}

const char* text_or_null(const std::optional<std::string>& s) { return s ? s->c_str() : nullptr; }

// The records of a binary, pointing into its description (which does not move again).
void fill(cb_binary& b) {
    const inspect::Description& d = b.description;
    b.arch = arch_of(d);
    b.sections.reserve(d.sections.size());
    for (const inspect::Section& s : d.sections)
        b.sections.push_back(cb_section{s.name.c_str(), to_c(s.address), s.size, s.file_offset, s.file_size, s.code ? 1u : 0u});
    std::size_t imports = 0;
    for (const inspect::ImportModule& m : d.imports) imports += m.symbols.size();
    b.imports.reserve(imports);
    for (const inspect::ImportModule& m : d.imports)
        for (const inspect::ImportSymbol& s : m.symbols)
            b.imports.push_back(cb_import{m.module.c_str(), text_or_null(s.name), s.ordinal.value_or(0), s.ordinal ? 1u : 0u, m.delay ? 1u : 0u});
    b.exports.reserve(d.exports.symbols.size());
    for (const inspect::ExportSymbol& e : d.exports.symbols)
        b.exports.push_back(cb_export{text_or_null(e.name), e.ordinal, to_c(e.address), text_or_null(e.forward)});
    // the functions the file names: its entry point and its exports that are code (a forward is another module's)
    std::vector<cb_function> named;
    for (const inspect::ExportSymbol& e : d.exports.symbols) {
        const cb_address at = to_c(e.address);
        if (at.style == CB_ADDRESS_NONE || e.forward) continue;
        if (std::any_of(named.begin(), named.end(), [&](const cb_function& f) { return same_address(f.address, at); })) continue;
        named.push_back(cb_function{&b, e.name ? *e.name : std::format("ordinal_{}", e.ordinal), at});
    }
    if (d.summary) {
        const cb_address entry = to_c(d.summary->entry);
        if (entry.style != CB_ADDRESS_NONE && std::none_of(named.begin(), named.end(), [&](const cb_function& f) { return same_address(f.address, entry); }))
            named.push_back(cb_function{&b, "entry", entry});
    }
    std::stable_sort(named.begin(), named.end(), [](const cb_function& x, const cb_function& y) { return before(x.address, y.address); });
    for (cb_function& f : named) b.functions.push_back(std::move(f));
    b.named = b.functions.size();
}

}  // namespace
}  // namespace cb::sdk_impl

using namespace cb::sdk_impl;

extern "C" {

cb_status cb_address_format(const cb_address* address, char* buffer, size_t size, size_t* needed) {
    return guarded("cb_address_format", [&]() -> cb_status {
        if (address == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_address_format", "the address is NULL");
        if (buffer == nullptr && size != 0) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_address_format", "the buffer is NULL but its size is not 0");
        const std::string text = cb::formats::inspect::address_text(from_c(*address));
        if (needed != nullptr) *needed = text.size() + 1;
        if (text.size() + 1 > size)
            return fail(CB_ERROR_BUFFER_TOO_SMALL, "cb_address_format",
                        std::format("the text \"{}\" needs {} bytes with its NUL, the buffer has {}", text, text.size() + 1, size));
        std::memcpy(buffer, text.c_str(), text.size() + 1);
        return CB_OK;
    });
}

cb_status cb_binary_open(const char* path, cb_binary** out) {
    if (out != nullptr) *out = nullptr;
    return guarded("cb_binary_open", [&]() -> cb_status {
        if (const cb_status s = require_init("cb_binary_open"); s != CB_OK) return s;
        if (path == nullptr || out == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_binary_open", "the path or the out pointer is NULL");
        const std::filesystem::path file = cb::fs::from_text(path);
        auto b = std::make_unique<cb_binary>();
        b->file = file;
        b->path = path;
        b->description = cb::formats::inspect::describe(file);
        if (!b->description.ok) {   // (only then is it asked why: a file that reads costs no look at the file system besides the read)
            std::error_code ec;
            if (!std::filesystem::exists(file, ec))
                return fail(CB_ERROR_NOT_FOUND, "cb_binary_open", std::format("{}: no such file (check the path; it is UTF-8, relative to the working folder)", path));
            if (std::filesystem::is_directory(file, ec))
                return fail(CB_ERROR_INVALID_ARGUMENT, "cb_binary_open", std::format("{} is a folder: open one of the files in it", path));
        }
        fill(*b);
        *out = b.release();
        return CB_OK;
    });
}

void cb_binary_close(cb_binary* binary) { delete binary; }

const char* cb_binary_path(const cb_binary* binary) { return binary ? binary->path.c_str() : ""; }

const char* cb_binary_format(const cb_binary* binary) { return binary && binary->description.format ? binary->description.format->c_str() : ""; }

const char* cb_binary_kind(const cb_binary* binary) { return binary && binary->description.kind ? binary->description.kind->c_str() : ""; }

const char* cb_binary_arch(const cb_binary* binary) { return binary ? binary->arch.c_str() : ""; }

uint64_t cb_binary_size(const cb_binary* binary) { return binary ? binary->description.size : 0; }

uint64_t cb_binary_image_base(const cb_binary* binary) {
    return binary && binary->description.summary ? binary->description.summary->image_base : 0;
}

cb_address cb_binary_entry(const cb_binary* binary) {
    return binary && binary->description.summary ? to_c(binary->description.summary->entry) : cb_address{0, 0, CB_ADDRESS_NONE};
}

const cb_section* cb_binary_sections(const cb_binary* binary, size_t* count) {
    if (count != nullptr) *count = binary ? binary->sections.size() : 0;
    return binary && !binary->sections.empty() ? binary->sections.data() : nullptr;
}

const cb_import* cb_binary_imports(const cb_binary* binary, size_t* count) {
    if (count != nullptr) *count = binary ? binary->imports.size() : 0;
    return binary && !binary->imports.empty() ? binary->imports.data() : nullptr;
}

const cb_export* cb_binary_exports(const cb_binary* binary, size_t* count) {
    if (count != nullptr) *count = binary ? binary->exports.size() : 0;
    return binary && !binary->exports.empty() ? binary->exports.data() : nullptr;
}

size_t cb_binary_function_count(const cb_binary* binary) { return binary ? binary->named : 0; }

const cb_function* cb_binary_function(const cb_binary* binary, size_t index) {
    return binary && index < binary->named ? &binary->functions[index] : nullptr;
}

cb_status cb_binary_function_find(const cb_binary* binary, const char* name, const cb_function** out) {
    if (out != nullptr) *out = nullptr;
    return guarded("cb_binary_function_find", [&]() -> cb_status {
        if (binary == nullptr || name == nullptr || out == nullptr)
            return fail(CB_ERROR_INVALID_ARGUMENT, "cb_binary_function_find", "the binary, the name or the out pointer is NULL");
        for (const cb_function& f : binary->functions)
            if (f.name == name) {
                *out = &f;
                return CB_OK;
            }
        return fail(CB_ERROR_NOT_FOUND, "cb_binary_function_find",
                    std::format("{} names no function {} (cb_binary_function lists the {} it names; cb_binary_function_at takes an address)",
                                binary->path, name, binary->named));
    });
}

cb_status cb_binary_function_at(cb_binary* binary, cb_address address, const cb_function** out) {
    if (out != nullptr) *out = nullptr;
    return guarded("cb_binary_function_at", [&]() -> cb_status {
        if (binary == nullptr || out == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_binary_function_at", "the binary or the out pointer is NULL");
        if (address.style == CB_ADDRESS_NONE || address.style > CB_ADDRESS_LE_OBJECT)
            return fail(CB_ERROR_INVALID_ARGUMENT, "cb_binary_function_at", std::format("the address's style {} is not a place (cb_address_style)", address.style));
        for (const cb_function& f : binary->functions)
            if (same_address(f.address, address)) {
                *out = &f;
                return CB_OK;
            }
        // (named as the decompiler names a function it finds: sub_10001000, sub_1_0116)
        const std::string name = address.style == CB_ADDRESS_LINEAR ? std::format("sub_{:x}", address.offset)
                                                                    : std::format("sub_{:x}_{:04x}", address.segment, address.offset);
        binary->functions.push_back(cb_function{binary, name, address});
        *out = &binary->functions.back();
        return CB_OK;
    });
}

const char* cb_function_name(const cb_function* function) { return function ? function->name.c_str() : ""; }

cb_address cb_function_address(const cb_function* function) { return function ? function->address : cb_address{0, 0, CB_ADDRESS_NONE}; }

cb_binary* cb_function_binary(const cb_function* function) { return function ? function->owner : nullptr; }

}  // extern "C"
