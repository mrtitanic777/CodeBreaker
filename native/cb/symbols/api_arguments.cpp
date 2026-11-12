#include "cb/symbols/api_arguments.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>

#include "cb/core/text.hpp"
#include "cb/symbols/x86.hpp"

namespace cb::symbols::api_arguments {

namespace {

namespace aa_text = cb::text;

constexpr std::array<std::string_view, 5> kExtensions = {".dll", ".exe", ".drv", ".cpl", ".ocx"};

// The longest forwarder string read from the export directory.
constexpr std::size_t kForwarderBytes = 128;
// The most bytes an instruction takes.
constexpr std::size_t kInstructionBytes = 16;

bool all_of_class(std::string_view s, bool (*ok)(char)) { return !s.empty() && std::all_of(s.begin(), s.end(), ok); }
bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_name_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// An operand or immediate as Capstone prints it: decimal, or hexadecimal with 0x, either with a sign.
std::optional<std::int64_t> number(std::string_view text) { return aa_text::parse_int<std::int64_t>(text, 0); }

// `dword ptr [0x1000]`: the address of an import slot.
std::optional<std::uint64_t> import_slot(std::string_view ops) {
    constexpr std::string_view kPrefix = "dword ptr [0x";
    if (!ops.starts_with(kPrefix) || !ops.ends_with(']')) return std::nullopt;
    const std::string_view digits = ops.substr(kPrefix.size(), ops.size() - kPrefix.size() - 1);
    if (!all_of_class(digits, is_lower_hex)) return std::nullopt;
    return aa_text::parse_int<std::uint64_t>(digits, 16);
}

// `-?(0x)?[0-9a-f]+` as one whole.
bool is_plain_number(std::string_view s) {
    if (s.starts_with('-')) s.remove_prefix(1);
    if (s.starts_with("0x")) s.remove_prefix(2);
    return all_of_class(s, is_lower_hex);
}

// `esp, 0x10`: the constant an instruction adds to or takes from esp.
std::optional<std::int64_t> esp_constant(std::string_view ops) {
    constexpr std::string_view kPrefix = "esp, ";
    if (!ops.starts_with(kPrefix)) return std::nullopt;
    const std::string_view rest = ops.substr(kPrefix.size());
    return is_plain_number(rest) ? number(rest) : std::nullopt;
}

// `esp, [ebp - 0x10]`: the displacement from ebp the stack pointer is loaded with (0 for `[ebp]`).
std::optional<std::int64_t> esp_from_ebp(std::string_view ops) {
    constexpr std::string_view kPrefix = "esp, [ebp";
    if (!ops.starts_with(kPrefix) || !ops.ends_with(']')) return std::nullopt;
    std::string_view middle = ops.substr(kPrefix.size(), ops.size() - kPrefix.size() - 1);
    if (middle.empty()) return 0;
    if (!middle.starts_with(" - ") && !middle.starts_with(" + ")) return std::nullopt;
    const std::string_view digits = middle.substr(3);
    if (!is_plain_number(digits) || digits.starts_with('-')) return std::nullopt;
    const auto value = number(digits);
    if (!value) return std::nullopt;
    return middle[1] == '-' ? -*value : *value;
}

std::string_view basename_of(std::string_view path) {
    const std::size_t cut = path.find_last_of("\\/");
    return cut == std::string_view::npos ? path : path.substr(cut + 1);
}

}  // namespace

std::span<const std::string_view> module_extensions() { return kExtensions; }

std::string stem(std::string_view dll) {
    std::string name = aa_text::upper(basename_of(dll));
    for (const std::string_view ext : kExtensions) {
        if (aa_text::iends_with(name, ext)) return name.substr(0, name.size() - ext.size());
    }
    return name;
}

// ---- the module the scan reads ---------------------------------------------------------------------------------------

ExportingModule::ExportingModule(formats::pe::Image image) : image_(std::move(image)) {
    base_ = image_.header().image_base;
    for (const formats::pe::Export& e : image_.exports()) {
        ordinals_[e.ordinal] = e.rva;
        if (e.name && !e.name->empty()) exports_[*e.name] = e.rva;
    }
    for (const formats::pe::Import& i : image_.imports()) imports_[i.slot_va] = {i.dll, i.name};
    const formats::pe::DataDirectory dir = image_.directory(0);
    export_lo_ = dir.rva;
    export_hi_ = dir.rva + dir.size;
}

ByteView ExportingModule::bytes_at(std::int64_t rva, std::size_t n) const {
    if (rva < 0) return {};
    const auto offset = image_.rva_to_offset(static_cast<std::uint64_t>(rva));
    const ByteView data = image_.data();
    if (!offset || *offset >= data.size()) return {};
    return data.subspan(static_cast<std::size_t>(*offset), std::min<std::size_t>(n, data.size() - static_cast<std::size_t>(*offset)));
}

std::optional<std::uint32_t> ExportingModule::export_by_name(std::string_view name) const {
    const auto it = exports_.find(std::string(name));
    return it == exports_.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
}

std::optional<std::uint32_t> ExportingModule::export_by_ordinal(std::uint64_t ordinal) const {
    const auto it = ordinals_.find(ordinal);
    return it == ordinals_.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
}

const ExportingModule::ImportedName* ExportingModule::import_at(std::uint64_t slot_va) const {
    const auto it = imports_.find(slot_va);
    return it == imports_.end() ? nullptr : &it->second;
}

std::optional<std::string> ExportingModule::forwarder(std::uint32_t rva) const {
    if (rva < export_lo_ || rva >= export_hi_) return std::nullopt;
    const ByteView raw = bytes_at(rva, kForwarderBytes);
    const auto end = std::find(raw.begin(), raw.end(), std::uint8_t{0});
    std::string text = aa_text::from_latin1(raw.first(static_cast<std::size_t>(end - raw.begin())));
    if (text.find('.') == std::string::npos) return std::nullopt;
    return text;
}

// ---- the scan ----------------------------------------------------------------------------------------------------------

namespace {

// One path being walked: where it is, and what is known of the stack there.
struct Path {
    std::int64_t at = 0;
    std::optional<std::int64_t> depth;           // bytes pushed since the entry; empty: lost
    std::optional<std::int64_t> depth_at_frame;  // the depth at `mov ebp, esp`
    std::optional<std::int64_t> cl;              // the value last put in cl (the thunk compiler's stub index)
    bool exact = true;                           // the depth does not rest on a callee's answer
    bool exact_at_frame = true;                  // ... and it did not at `mov ebp, esp`
};

}  // namespace

std::optional<std::uint32_t> StackScanner::resolve(std::string_view dll, std::string_view name, int hops) {
    const std::shared_ptr<const ExportingModule> module = modules_.find(stem(dll));
    if (!module || name.empty()) return std::nullopt;
    std::optional<std::uint32_t> rva;
    if (name.starts_with('#')) {
        const auto ordinal = aa_text::parse_int<std::uint64_t>(name.substr(1), 10);
        if (ordinal) rva = module->export_by_ordinal(*ordinal);
    } else {
        rva = module->export_by_name(name);
    }
    if (!rva || *rva == 0) return std::nullopt;
    if (const auto forwarded = module->forwarder(*rva)) {
        if (hops <= 0) return std::nullopt;
        const std::size_t dot = forwarded->find('.');
        return resolve(std::string_view(*forwarded).substr(0, dot), std::string_view(*forwarded).substr(dot + 1), hops - 1);
    }
    return scan(*module, *rva, hops);
}

std::optional<std::uint32_t> StackScanner::scan(const ExportingModule& m, std::int64_t rva, int hops) {
    auto& memo = m.scanned_;
    if (const auto done = memo.find(rva); done != memo.end()) return done->second;
    memo[rva] = std::nullopt;   // the guard against a function that calls itself
    const platforms::Decoder& cs = decoders_.get(platforms::Arch::X86, 32, false);
    const auto base = static_cast<std::int64_t>(m.image_base());
    // The answers each kind of evidence gives; the first kind that has any decides.
    std::set<std::int64_t> thunk, strong, weak, by_cl;
    std::vector<Path> work = {Path{.at = rva, .depth = 0}};
    std::set<std::int64_t> seen;
    std::size_t budget = kMaxInstructions;

    // The bytes a callee through an import or a direct call pops.
    auto callee_through_import = [&](std::string_view ops) -> std::optional<std::int64_t> {
        const auto slot = import_slot(ops);
        const ExportingModule::ImportedName* imported = slot ? m.import_at(*slot) : nullptr;
        if (imported == nullptr || hops <= 0) return std::nullopt;
        const auto popped = resolve(imported->dll, imported->name, hops - 1);
        return popped ? std::optional<std::int64_t>(*popped) : std::nullopt;
    };

    while (!work.empty() && budget > 0 && strong.size() < 4) {
        Path p = work.back();
        work.pop_back();
        while (budget > 0) {
            if (seen.contains(p.at)) break;
            const ByteView code = m.bytes_at(p.at, kInstructionBytes);
            const auto insn = code.empty() ? std::nullopt : x86::decode_first(cs, code, static_cast<std::uint64_t>(base + p.at));
            if (!insn) break;
            seen.insert(p.at);
            --budget;
            const std::string& mn = insn->mnemonic;
            const std::string_view ops = insn->op_str;
            const std::int64_t next = p.at + insn->size;
            if (mn == "ret") {
                const auto n = ops.empty() ? std::optional<std::int64_t>(0) : number(ops);
                if (!n) break;
                // Depth that came through a call is only as good as the callee's answer: compiled code returns with the
                // stack balanced, so trust `ret N` itself there.
                if (!p.depth || (*p.depth != 0 && !p.exact)) {
                    weak.insert(*n);
                } else if (*n - *p.depth >= 0) {
                    strong.insert(*n - *p.depth);
                }
                break;
            }
            if (mn == "retf" || mn == "int3" || mn == "hlt" || mn == "iretd") break;
            if (mn == "jmp") {
                if (import_slot(ops)) {   // a tail call through an import
                    const auto r = callee_through_import(ops);
                    if (r) {
                        (p.depth ? strong : weak).insert(*r - p.depth.value_or(0));
                    } else if (p.cl && p.depth && *p.depth == 0 && p.exact) {
                        by_cl.insert(4 * *p.cl);
                    }
                    break;
                }
                if (const auto target = number(ops)) {
                    p.at = *target - base;
                    continue;
                }
                if (p.cl && p.depth && *p.depth == 0 && p.exact) by_cl.insert(4 * *p.cl);   // `mov cl, N / jmp [common]`
                break;
            }
            if (mn.starts_with('j') || mn.starts_with("loop")) {   // conditional: walk both ways
                if (const auto target = number(ops)) {
                    Path branch = p;
                    branch.at = *target - base;
                    work.push_back(branch);
                }
            } else if (mn == "call") {
                p.cl.reset();
                p.exact = false;
                std::optional<std::int64_t> r;
                if (import_slot(ops)) {
                    r = callee_through_import(ops);
                } else if (const auto target = number(ops); target && hops > 0) {
                    const auto popped = scan(m, *target - base, hops - 1);
                    if (popped) r = *popped;
                }
                p.depth = (p.depth && r) ? std::optional<std::int64_t>(*p.depth - *r) : std::nullopt;
            } else if (mn == "push") {
                if (p.depth) *p.depth += 4;
            } else if (mn == "pop") {
                p.depth = (!p.depth || ops == "esp") ? std::nullopt : std::optional<std::int64_t>(*p.depth - 4);
            } else if (mn == "pushal") {
                if (p.depth) *p.depth += 32;
            } else if (mn == "popal") {
                if (p.depth) *p.depth -= 32;
            } else if (mn == "pushfd" || mn == "popfd") {
                if (p.depth) *p.depth += mn == "pushfd" ? 4 : -4;
            } else if (mn == "leave") {   // the frame pointer restores a known depth
                p.depth = p.depth_at_frame ? std::optional<std::int64_t>(*p.depth_at_frame - 4) : std::nullopt;
                p.exact = p.exact_at_frame;
            } else if (mn == "mov" && ops == "ebp, esp") {
                p.depth_at_frame = p.depth;
                p.exact_at_frame = p.exact;
            } else if (mn == "mov" && ops == "esp, ebp") {
                p.depth = p.depth_at_frame;
                p.exact = p.exact_at_frame;
            } else if (mn == "mov" && ops.starts_with("edx, 0x") && p.cl) {
                // The thunk compiler's 32->16 stub: `mov cl, index / mov edx, argc<<16|flags / jmp common`. The argument
                // count is in the instruction.
                const auto flags = aa_text::parse_int<std::uint64_t>(ops.substr(5), 16);
                if (p.depth && p.exact && flags) thunk.insert(4 * static_cast<std::int64_t>(*flags >> 16) - *p.depth);
                break;
            } else if (mn == "mov" && ops.starts_with("cl, ")) {
                p.cl = number(ops.substr(4));
            } else if (ops.starts_with("esp, ") || ops == "esp") {
                const auto constant = esp_constant(ops);
                if ((mn == "sub" || mn == "add") && constant && p.depth) {
                    *p.depth += *constant * (mn == "sub" ? 1 : -1);
                } else if (const auto from_frame = esp_from_ebp(ops); mn == "lea" && from_frame && p.depth_at_frame) {
                    p.depth = *p.depth_at_frame - *from_frame;
                    p.exact = p.exact_at_frame;
                } else {
                    p.depth.reset();
                }
            }
            p.at = next;
        }
    }
    for (const std::set<std::int64_t>* evidence : {&thunk, &strong, &weak, &by_cl}) {
        std::set<std::int64_t> candidates;
        for (const std::int64_t c : *evidence) {
            if (c >= 0 && c % 4 == 0 && c <= 0xFFFFFFFFLL) candidates.insert(c);
        }
        if (candidates.empty()) continue;
        memo[rva] = candidates.size() == 1 ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(*candidates.begin())) : std::nullopt;
        break;
    }
    return memo[rva];
}

// ---- 32-bit ----------------------------------------------------------------------------------------------------------

std::optional<std::uint32_t> Win32Arguments::stack_bytes(std::string_view dll, std::string_view name) {
    if (const auto declared = memory_.header_bytes(name)) return declared;
    const std::string module = stem(dll);
    if (const auto known = memory_.scanned(module, name)) return known->bytes;
    const auto found = scanner_.resolve(module, name, kMaxHops);
    memory_.remember(module, name, found);
    return found;
}

std::optional<std::uint32_t> Win32Arguments::argc(std::string_view dll, std::string_view name) {
    const auto bytes = stack_bytes(dll, name);
    if (!bytes || *bytes % 4 != 0) return std::nullopt;
    return *bytes / 4;
}

// ---- 16-bit ----------------------------------------------------------------------------------------------------------

std::optional<ApiName> parse_api_name(std::string_view api) {
    const std::size_t sep = api.find_last_of("._");
    if (sep == std::string_view::npos || sep == 0) return std::nullopt;
    const std::string_view module = api.substr(0, sep);
    std::string_view tail = api.substr(sep + 1);
    if (tail.starts_with('@')) tail.remove_prefix(1);
    if (!all_of_class(module, is_name_char) || !all_of_class(tail, is_digit)) return std::nullopt;
    const auto ordinal = aa_text::parse_int<std::uint32_t>(tail, 10);
    if (!ordinal) return std::nullopt;
    return ApiName{std::string(module), *ordinal};
}

std::optional<std::uint32_t> Win16Arguments::arg_bytes(std::string_view module, std::uint32_t ordinal) {
    const std::string name = aa_text::upper(module);
    const std::string key = std::format("{}.{}", name, ordinal);
    if (const auto known = memory_.remembered(key)) return known->bytes;
    std::optional<std::uint32_t> value;
    const auto found = exports_.find(name, ordinal);
    if (found && found->what) {
        const ExportReturn& r = *found->what;
        if (r.forwarded) {
            value = r.forwarded;
        } else if (r.far_return && r.pop >= 0 && r.pop <= 0xFFFFFFFFLL) {
            value = static_cast<std::uint32_t>(r.pop);
        }
    }
    memory_.remember(key, value, std::format("{}'s code", found ? aa_text::lower(basename_of(found->file)) : name));
    return value;
}

std::optional<std::uint32_t> Win16Arguments::of(std::string_view api) {
    const auto parsed = parse_api_name(api);
    return parsed ? arg_bytes(parsed->module, parsed->ordinal) : std::nullopt;
}

std::string Win16Arguments::table_text() {
    std::string out = "{";
    bool first = true;
    for (const auto& [key, bytes] : memory_.table()) {
        if (!first) out += ", ";
        first = false;
        out += '"';
        for (const char c : key) {
            if (c == '"' || c == '\\') out += '\\';
            out += c;
        }
        out += "\": ";
        out += bytes ? std::to_string(*bytes) : "null";
    }
    out += '}';
    return out;
}

}  // namespace cb::symbols::api_arguments
