// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/ne_asm/segment.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <format>
#include <tuple>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::source::ne_asm::segment {

namespace ne = formats::ne;

namespace {

// ---- name tables ---------------------------------------------------------------------------------------------------------

struct NamedValue {
    std::uint32_t value;
    const char* name;
};

// The window messages the 16-bit modules dispatch on.
constexpr std::array kMessages = std::to_array<NamedValue>({
    {0x0001, "WM_CREATE"}, {0x0002, "WM_DESTROY"}, {0x0003, "WM_MOVE"}, {0x0005, "WM_SIZE"},
    {0x0006, "WM_ACTIVATE"}, {0x0007, "WM_SETFOCUS"}, {0x0008, "WM_KILLFOCUS"}, {0x000A, "WM_ENABLE"},
    {0x000B, "WM_SETREDRAW"}, {0x000C, "WM_SETTEXT"}, {0x000D, "WM_GETTEXT"}, {0x000E, "WM_GETTEXTLENGTH"},
    {0x000F, "WM_PAINT"}, {0x0010, "WM_CLOSE"}, {0x0011, "WM_QUERYENDSESSION"}, {0x0012, "WM_QUIT"},
    {0x0013, "WM_QUERYOPEN"}, {0x0014, "WM_ERASEBKGND"}, {0x0018, "WM_SHOWWINDOW"},
    {0x001A, "WM_WININICHANGE"}, {0x0020, "WM_SETCURSOR"}, {0x0021, "WM_MOUSEACTIVATE"},
    {0x0024, "WM_GETMINMAXINFO"}, {0x0081, "WM_NCCREATE"}, {0x0083, "WM_NCCALCSIZE"},
    {0x0084, "WM_NCHITTEST"}, {0x0085, "WM_NCPAINT"}, {0x0086, "WM_NCACTIVATE"},
    {0x0100, "WM_KEYDOWN"}, {0x0101, "WM_KEYUP"}, {0x0102, "WM_CHAR"}, {0x0104, "WM_SYSKEYDOWN"},
    {0x0105, "WM_SYSKEYUP"}, {0x0106, "WM_SYSCHAR"}, {0x0110, "WM_INITDIALOG"}, {0x0111, "WM_COMMAND"},
    {0x0112, "WM_SYSCOMMAND"}, {0x0113, "WM_TIMER"}, {0x0114, "WM_HSCROLL"}, {0x0115, "WM_VSCROLL"},
    {0x0116, "WM_INITMENU"}, {0x0200, "WM_MOUSEMOVE"}, {0x0201, "WM_LBUTTONDOWN"}, {0x0202, "WM_LBUTTONUP"},
    {0x0203, "WM_LBUTTONDBLCLK"}, {0x0204, "WM_RBUTTONDOWN"}, {0x0205, "WM_RBUTTONUP"},
    {0x0206, "WM_RBUTTONDBLCLK"}, {0x0207, "WM_MBUTTONDOWN"}, {0x0208, "WM_MBUTTONUP"},
    {0x0300, "WM_CUT"}, {0x0301, "WM_COPY"}, {0x0302, "WM_PASTE"}, {0x0303, "WM_CLEAR"}, {0x0304, "WM_UNDO"},
});

// The INT 21h functions the KERNEL wrappers call.
constexpr std::array kDosFunctions = std::to_array<NamedValue>({
    {0x02, "DISPLAY_CHAR"}, {0x09, "DISPLAY_STRING"}, {0x19, "GET_DEFAULT_DRIVE"}, {0x1A, "SET_DTA"},
    {0x25, "SET_VECTOR"}, {0x2A, "GET_DATE"}, {0x2C, "GET_TIME"}, {0x30, "GET_VERSION"}, {0x35, "GET_VECTOR"},
    {0x36, "GET_DISK_SPACE"}, {0x39, "MKDIR"}, {0x3A, "RMDIR"}, {0x3B, "CHDIR"}, {0x3C, "CREATE"}, {0x3D, "OPEN"},
    {0x3E, "CLOSE"}, {0x3F, "READ"}, {0x40, "WRITE"}, {0x41, "UNLINK"}, {0x42, "LSEEK"}, {0x43, "GET_SET_ATTR"},
    {0x44, "IOCTL"}, {0x47, "GET_CUR_DIR"}, {0x48, "ALLOC"}, {0x49, "FREE"}, {0x4A, "REALLOC"}, {0x4B, "EXEC"},
    {0x4C, "EXIT"}, {0x4E, "FINDFIRST"}, {0x4F, "FINDNEXT"}, {0x56, "RENAME"}, {0x57, "FILE_TIME"},
});

template <std::size_t N>
const char* lookup_name(const std::array<NamedValue, N>& table, std::uint32_t value) {
    const auto it = std::lower_bound(table.begin(), table.end(), value, [](const NamedValue& e, std::uint32_t v) { return e.value < v; });
    return it != table.end() && it->value == value ? it->name : nullptr;
}

// ---- reading the listing's text -------------------------------------------------------------------------------------------

// objdump writes a mnemonic padded to six columns and a space; the patterns below are written with that padding.
constexpr std::string_view kPushBp = "push   bp";
constexpr std::string_view kMovBpSp = "mov    bp,sp";

// The prologue scan (`push bp` followed by `mov bp,sp` starts a function) is OFF. The Python module this replaced wrote the
// `push bp` of its test with one space where objdump writes three, so the scan never found anything; switching it on adds
// 1,413 function starts to the Setup chain's modules (none to the 15 MINI.CAB ones), and the source those starts carve is
// what the byte-match gates were run on. state/work/typed-port/_bugs/LIST.md has the numbers; this is Logan's call.
constexpr bool kScanPushBpPrologue = false;

// <prefix><hex digits>; with `to_end` the digits are the rest of the text. The digits are lower case, as objdump writes them.
std::optional<std::uint32_t> hex_after(std::string_view text, std::string_view prefix, bool to_end) {
    if (!text.starts_with(prefix)) return std::nullopt;
    std::size_t end = prefix.size();
    while (end < text.size() && ((text[end] >= '0' && text[end] <= '9') || (text[end] >= 'a' && text[end] <= 'f'))) ++end;
    if (end == prefix.size() || (to_end && end != text.size())) return std::nullopt;
    return text::parse_int<std::uint32_t>(text.substr(prefix.size(), end - prefix.size()), 16);
}

// The target of "<mnemonic> <spaces>0x<hex>" (the whole text): a direct near call or jump.
std::optional<std::uint32_t> branch_target(std::string_view text, std::string_view mnemonic) {
    if (!text.starts_with(mnemonic)) return std::nullopt;
    std::size_t at = mnemonic.size();
    const std::size_t spaces_from = at;
    while (at < text.size() && text::is_space(text[at])) ++at;
    if (at == spaces_from) return std::nullopt;
    return hex_after(text.substr(at), "0x", true);
}

bool is_call_or_jump(std::string_view text) { return text.starts_with("call") || text.starts_with("jmp"); }

std::string message_name(std::uint32_t message) {
    if (const char* name = window_message_name(message)) return name;
    return std::format("msg_0x{:04x}", message);
}

// The data segment's bytes as the file holds them; none for a segment that has no data in the file.
ByteView stored_bytes(const ne::Module& module, const ne::Segment& segment) {
    if (segment.file_offset == 0) return {};
    return io::checked_range(module.data(), segment.file_offset, segment.length);
}

std::string args_text(const std::vector<std::uint32_t>& args) {
    std::vector<std::string> slots;
    for (const std::uint32_t a : args) slots.push_back(std::format("[bp+0x{:02x}]", a));
    return text::join(slots, ", ");
}

std::string joined_calls(const Seg& seg, const std::vector<Call>& calls, CallKind kind) {
    std::vector<std::string> shown;
    for (const Call& c : calls) {
        if (c.kind == kind) shown.push_back(describe_call(seg, c));
    }
    return text::join(shown, "; ");
}

const ne::Segment& numbered_segment(const std::vector<ne::Segment>& segments, std::size_t number) {
    if (number < 1 || number > segments.size()) throw UsageError(std::format("segment {} does not exist (the module has {})", number, segments.size()));
    return segments[number - 1];
}

// {ordinal: name} of the module's resident and non-resident names (the first name of each table is the module's own).
std::map<std::uint32_t, std::string> names_by_ordinal(const ne::Module& module) {
    std::map<std::uint32_t, std::string> out;
    const std::vector<ne::Name> resident = module.resident_names(), nonresident = module.nonresident_names();
    for (std::size_t i = 1; i < resident.size(); ++i) out[resident[i].ordinal] = resident[i].name;
    for (std::size_t i = 1; i < nonresident.size(); ++i) out[nonresident[i].ordinal] = nonresident[i].name;
    return out;
}

}  // namespace

const char* window_message_name(std::uint32_t message) { return lookup_name(kMessages, message); }
const char* dos_function_name(std::uint32_t function) { return lookup_name(kDosFunctions, function); }

std::string describe_call(const Seg& seg, const Call& call) {
    switch (call.kind) {
        case CallKind::System: return call.label + (call.name ? " = " + *call.name : "  [unidentified]");
        case CallKind::Internal: return std::format("-> {} (seg{}:0x{:04x})", seg.name_at(call.target), seg.index(), call.target);
        case CallKind::Far: return std::format("-> {} (other segment)", call.label);
    }
    return {};
}

// ---- ORDINALS.md -------------------------------------------------------------------------------------------------------------

Ordinals parse_ordinals(std::string_view markdown) {
    // `| MODULE.N | name |`, the name optionally in backticks.
    static const Regex row(R"(\|\s*([A-Za-z_][A-Za-z0-9_]*)\.(\d+)\s*\|\s*`?([^|`]+?)`?\s*\|)");
    Ordinals names;
    for (const std::string_view line : text::lines(markdown)) {
        const auto m = row.search(line);
        if (!m) continue;
        const auto number = text::parse_int<std::uint32_t>(m->group(2));
        if (!number) continue;
        names[std::format("{}.{}", m->group(1), *number)] = std::string(text::trim(m->group(3)));
    }
    return names;
}

Ordinals load_ordinals(const std::filesystem::path& file) {
    if (!std::filesystem::exists(file)) return {};
    return parse_ordinals(text::sanitize_utf8(fs::read_text(file)));
}

// ---- disassembly --------------------------------------------------------------------------------------------------------------

std::vector<Row> disassemble(ByteView code, std::uint32_t base, const fixups::Objdump& objdump) {
    std::vector<Row> rows;
    if (code.empty()) return rows;
    for (const fixups::ListingLine& line : fixups::parse_objdump(objdump(code, base))) {
        if (line.offset) rows.push_back({*line.offset, line.instruction});
    }
    return rows;
}

// ---- Seg ------------------------------------------------------------------------------------------------------------------------

Seg::Seg(const ne::Module& module, std::size_t index, SharedOrdinals ordinals, NameOverrides overrides, const fixups::Objdump& objdump)
    : index_(index), ordinals_(std::move(ordinals)), overrides_(std::move(overrides)) {
    const std::vector<ne::Segment> segments = module.segments();
    info_ = numbered_segment(segments, index);
    modules_ = module.module_refs();
    fixups_ = fixups::build_fixup_map(module, info_, modules_);
    const ByteView stored = stored_bytes(module, info_);
    code_.assign(stored.begin(), stored.end());
    rows_ = disassemble(code_, 0, objdump);
    for (std::size_t i = 0; i < rows_.size(); ++i) row_at_[rows_[i].offset] = i;   // an offset met twice keeps the later row

    // Exported entries that land here, named by the module's name tables.
    const std::map<std::uint32_t, std::string> by_ordinal = names_by_ordinal(module);
    const std::vector<ne::Entry> entries = module.entries();
    for (const ne::Entry& e : entries) {
        if (e.kind == ne::Entry::Kind::Unused || e.segment != index_) continue;
        const auto name = by_ordinal.find(e.ordinal);
        if (name != by_ordinal.end() && !name->second.empty()) export_names_[e.offset] = name->second;
    }
    // An exported entry point is a function start by definition, named or not: the stripped system modules name none of them.
    for (const ne::Entry& e : entries) {
        if (e.kind != ne::Entry::Kind::Unused && e.segment == index_) starts_.insert(e.offset);
    }
    find_starts();
    carve();
}

void Seg::find_starts() {
    for (const auto& [offset, name] : export_names_) starts_.insert(offset);
    // (2) internal relocation targets that point into this segment
    for (const auto& [site, target] : fixups_) {
        if (target.segment && target.segment->segment == index_) starts_.insert(target.segment->offset);
    }
    // (3) intra-segment near-call targets
    for (const Row& row : rows_) {
        if (const auto target = branch_target(row.text, "call")) starts_.insert(*target);
    }
    // (4) the prologue `push bp; mov bp,sp` (off: see kScanPushBpPrologue)
    for (std::size_t i = 0; kScanPushBpPrologue && i + 1 < rows_.size(); ++i) {
        if (rows_[i].text == kPushBp && rows_[i + 1].text.starts_with(kMovBpSp)) starts_.insert(rows_[i].offset);
    }
    // (5) the other standard far-proc frame, `enter N,0`: the GDI and USER modules use it beside the push-bp form, and
    // without it a function that no call of the segment reaches (called through a far pointer or from another segment) is
    // not a start and the carve of the function before it swallows it.
    for (const Row& row : rows_) {
        if (row.text.starts_with("enter ")) starts_.insert(row.offset);
    }
    // (6) the shared routine that two or more short thunks tail-jump to (GDI's Get* accessors `mov cl,IMM; jmp 0x45fe`).
    // Strictly a thunk: the jmp must lie within 8 bytes of a known start, so a jump deep inside a function (a shared epilogue,
    // local control flow) is not a boundary and the carve is not split into thousands of fake functions.
    const std::vector<std::uint32_t> known(starts_.begin(), starts_.end());
    std::map<std::uint32_t, std::size_t> thunk_jumps;
    for (const Row& row : rows_) {
        const auto target = branch_target(row.text, "jmp");
        if (!target) continue;
        const auto after = std::upper_bound(known.begin(), known.end(), row.offset);
        if (after == known.begin()) continue;
        if (row.offset - *(after - 1) <= 8) ++thunk_jumps[*target];
    }
    for (const auto& [target, jumps] : thunk_jumps) {
        if (jumps >= 2) starts_.insert(target);
    }
    // Only an address that starts an instruction can start a function.
    std::erase_if(starts_, [&](std::uint32_t s) { return !row_at_.contains(s); });
}

void Seg::carve() {
    for (auto it = starts_.begin(); it != starts_.end(); ++it) {
        const auto next = std::next(it);
        functions_[*it] = Func{*it, next != starts_.end() ? *next : info_.length, name_at(*it)};
    }
}

std::string Seg::name_at(std::uint32_t offset) const {
    if (const auto e = export_names_.find(offset); e != export_names_.end()) return e->second;
    if (const auto o = overrides_.find(offset); o != overrides_.end()) return o->second;
    return std::format("sub_{:04x}", offset);
}

std::optional<Call> Seg::resolve_call(std::uint32_t offset, std::string_view instruction) const {
    constexpr std::uint32_t kLongestInstruction = 7;   // a fix-up anywhere in the instruction's bytes names its target
    for (std::uint32_t k = offset; k < offset + kLongestInstruction; ++k) {
        const auto site = fixups_.find(k);
        if (site == fixups_.end()) continue;
        const fixups::Target& target = site->second;
        if (target.segment) {
            if (target.segment->segment == index_) return Call{CallKind::Internal, target.label, std::nullopt, target.segment->offset};
            return Call{CallKind::Far, target.label, std::nullopt, 0};
        }
        const auto name = ordinals_ ? ordinals_->find(target.label) : Ordinals::const_iterator();
        const bool named = ordinals_ && name != ordinals_->end() && !name->second.empty();
        return Call{CallKind::System, target.label, named ? std::optional<std::string>(name->second) : std::nullopt, 0};
    }
    if (const auto target = branch_target(instruction, "call")) return Call{CallKind::Internal, {}, std::nullopt, *target};   // an intra-segment near call
    return std::nullopt;
}

FunctionRows Seg::function_rows(std::uint32_t start) const {
    FunctionRows out;
    if (const auto f = functions_.find(start); f != functions_.end()) {
        out.func = f->second;
    } else {
        // Not a known start: a span to the next known one.
        const auto next = starts_.upper_bound(start);
        out.func = Func{start, next != starts_.end() ? *next : info_.length, name_at(start)};
    }
    const auto first = row_at_.find(start);
    if (first == row_at_.end()) return out;
    std::size_t last = first->second;
    while (last < rows_.size() && rows_[last].offset < out.func.end) ++last;
    // trim the trailing padding (nop / int3 / the bytes that do not decode)
    while (last > first->second && (rows_[last - 1].text == "nop" || rows_[last - 1].text == "int3" || rows_[last - 1].text == "(bad)")) --last;
    out.rows = std::span<const Row>(rows_).subspan(first->second, last - first->second);
    return out;
}

std::vector<Call> Seg::calls_in(std::uint32_t start) const {
    std::vector<Call> calls;
    std::set<std::tuple<CallKind, std::string, std::uint32_t>> seen;
    for (const Row& row : function_rows(start).rows) {
        if (!is_call_or_jump(row.text)) continue;
        const auto call = resolve_call(row.offset, row.text);
        if (!call) continue;
        // a jump is a (tail) call only when it leaves via a fix-up or to another function: a jump inside the function is local control flow
        if (row.text.starts_with("jmp") && call->kind == CallKind::Internal && !functions_.contains(call->target)) continue;
        const std::string key = call->kind == CallKind::Internal ? std::string() : call->label;
        if (seen.insert({call->kind, key, call->kind == CallKind::Internal ? call->target : 0}).second) calls.push_back(*call);
    }
    return calls;
}

std::vector<std::uint32_t> Seg::args_used(std::uint32_t start) const {
    constexpr std::string_view kSlot = "[bp+0x";
    std::set<std::uint32_t> slots;
    for (const Row& row : function_rows(start).rows) {
        std::string_view text = row.text;
        for (std::size_t at = text.find(kSlot); at != std::string_view::npos; at = text.find(kSlot, at)) {
            std::size_t end = at + kSlot.size();
            while (end < text.size() && ((text[end] >= '0' && text[end] <= '9') || (text[end] >= 'a' && text[end] <= 'f'))) ++end;
            const auto slot = text::parse_int<std::uint32_t>(text.substr(at + kSlot.size(), end - at - kSlot.size()), 16);
            if (slot && end > at + kSlot.size() && end < text.size() && text[end] == ']') {
                slots.insert(*slot);
                at = end + 1;
            } else {
                at += 1;
            }
        }
    }
    std::vector<std::uint32_t> out;
    for (const std::uint32_t s : slots) {
        if (s >= 4) out.push_back(s);
    }
    return out;
}

std::optional<std::uint32_t> Seg::word_at(std::uint32_t offset) const {
    if (code_.size() < 2 || offset > code_.size() - 2) return std::nullopt;
    return static_cast<std::uint32_t>(code_[offset]) | (static_cast<std::uint32_t>(code_[offset + 1]) << 8);
}

namespace {

// The handler of the message compare at rows[i]: the target of the `je` after it; of the `jmp` after a `jne` (or, with no
// jmp, the instruction right after the jne: the handler follows inline).
std::optional<std::uint32_t> handler_after(std::span<const Row> rows, std::size_t i) {
    for (std::size_t j = 0; j < 3 && i + 1 + j < rows.size(); ++j) {
        const std::string& text = rows[i + 1 + j].text;
        if (const auto t = hex_after(text, "je     0x", false)) return t;
        if (hex_after(text, "jne    0x", false)) {
            const std::size_t k = i + 1 + j + 1;
            if (k >= rows.size()) return std::nullopt;
            if (const auto t = hex_after(rows[k].text, "jmp    0x", false)) return t;
            return rows[k].offset;
        }
    }
    return std::nullopt;
}

}  // namespace

std::vector<Dispatch> Seg::dispatch(std::uint32_t start) const {
    constexpr std::size_t kLookBack = 20;   // how far before the jump table's `jmp` its range check is looked for
    const std::span<const Row> rows = function_rows(start).rows;
    std::vector<Dispatch> out;
    std::set<std::uint32_t> seen;
    const auto add = [&](std::uint32_t message, std::uint32_t handler) {
        if (seen.insert(message).second) out.push_back({message, handler});
    };
    bool ax_is_message = false;

    for (std::size_t i = 0; i < rows.size(); ++i) {
        const std::string& text = rows[i].text;
        // the dialog form: a direct compare of the message slot
        if (const auto value = hex_after(text, "cmp    WORD PTR [bp+0xc],0x", true)) {
            if (const auto handler = handler_after(rows, i); handler && !seen.contains(*value)) add(*value, *handler);
            continue;
        }
        // the jump-table form: jmp WORD cs:[bx+TAB]
        if (const auto table = hex_after(text, "jmp    WORD PTR cs:[bx+0x", false)) {
            std::optional<std::uint32_t> count;
            std::uint32_t base = 0;
            bool saw_message = false;
            for (std::size_t k = i; k-- > (i > kLookBack ? i - kLookBack : 0);) {   // look back for the range check
                const std::string& before = rows[k].text;
                if (const auto n = hex_after(before, "cmp    ax,0x", true); n && !count) count = n;
                if (const auto b = hex_after(before, "sub    ax,0x", true)) base += *b;
                if (before.starts_with("mov    ax,WORD PTR [bp+0xc]")) saw_message = true;
            }
            if (count && saw_message) {
                // the table's words, one handler per message; the first word outside the segment ends it
                for (std::uint32_t index = 0; index <= *count; ++index) {
                    const auto handler = word_at(*table + 2 * index);
                    if (!handler) break;
                    add(index + base, *handler);
                }
            }
            continue;
        }
        if (text.starts_with("mov    ax,WORD PTR [bp+0xc]")) {
            ax_is_message = true;
            continue;
        }
        // an instruction that replaces ax ends the message's life in it
        const bool replaces_ax = text.starts_with("mov    ax,") || text.starts_with("pop    ax") || text.starts_with("lea    ax") ||
                                 text.starts_with("call") || text.starts_with("xor    ax,ax") || text.starts_with("sub    ax,ax") ||
                                 (text.starts_with("xchg ") && text.find("ax", 5) != std::string::npos);
        if (replaces_ax) ax_is_message = false;
        const auto value = hex_after(text, "cmp    ax,0x", true);
        if (!value || !ax_is_message) continue;
        if (const auto handler = handler_after(rows, i); handler && !seen.contains(*value)) add(*value, *handler);
    }
    return out;
}

std::vector<std::string> Seg::handler_calls(std::uint32_t start, std::uint32_t handler, const std::vector<std::uint32_t>& handler_starts) const {
    const std::span<const Row> rows = function_rows(start).rows;
    std::unordered_map<std::uint32_t, std::size_t> at;
    for (std::size_t k = 0; k < rows.size(); ++k) at[rows[k].offset] = k;
    const auto first = at.find(handler);
    if (first == at.end()) return {};
    std::optional<std::uint32_t> next;   // the next handler
    for (const std::uint32_t s : handler_starts) {
        if (s > handler && (!next || s < *next)) next = s;
    }
    const std::set<std::uint32_t> starts(handler_starts.begin(), handler_starts.end());
    std::vector<std::string> out;
    std::set<std::tuple<CallKind, std::string, std::uint32_t>> seen;
    for (std::size_t k = first->second; k < rows.size(); ++k) {
        const Row& row = rows[k];
        if (row.offset != handler && starts.contains(row.offset)) break;
        if (row.text.starts_with("ret")) break;
        if (const auto t = hex_after(row.text, "jmp    0x", true); t && (*t < handler || (next && *t >= *next))) break;   // a tail jump to the shared epilogue
        if (!is_call_or_jump(row.text)) continue;
        const auto call = resolve_call(row.offset, row.text);
        if (!call) continue;
        std::string shown;
        if (call->kind == CallKind::System) shown = call->label + (call->name ? "=" + *call->name : " [unident]");
        else if (call->kind == CallKind::Internal) shown = "->" + name_at(call->target);
        else shown = "->" + call->label;
        const std::string key = call->kind == CallKind::Internal ? std::string() : call->label;
        if (seen.insert({call->kind, key, call->kind == CallKind::Internal ? call->target : 0}).second) out.push_back(std::move(shown));
    }
    return out;
}

std::optional<std::uint32_t> Seg::is_thunk(std::uint32_t start) const {
    const std::span<const Row> rows = function_rows(start).rows;
    if (rows.size() >= 3 && rows[0].text.starts_with("mov    bx,0x") && rows[1].text == "push   bx") return hex_after(rows[2].text, "jmp    0x", false);
    return std::nullopt;
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> find_entry(const ne::Module& module, std::uint32_t ordinal) {
    for (const ne::Entry& e : module.entries()) {
        if (e.ordinal == ordinal && e.kind != ne::Entry::Kind::Unused) return std::make_pair(std::uint32_t{e.segment}, std::uint32_t{e.offset});
    }
    return std::nullopt;
}

// ---- the annotated output -------------------------------------------------------------------------------------------------------

std::string annotate_row(const Seg& seg, const Row& row) {
    std::string line = std::format("    {:04x}:  {}", row.offset, row.text);
    const std::string& text = row.text;
    if (is_call_or_jump(text)) {
        if (const auto call = seg.resolve_call(row.offset, text)) {
            if (call->kind == CallKind::System) {
                line += "      ; " + call->label + (call->name ? " = " + *call->name : "  [UNIDENTIFIED — leave raw]");
            } else if (call->kind == CallKind::Internal && (seg.functions().contains(call->target) || text.starts_with("call"))) {
                line += "      ; -> " + seg.name_at(call->target);
            } else if (call->kind == CallKind::Far) {
                line += "      ; -> " + call->label;
            }
        }
    }
    // a DOS function number loaded into ah
    constexpr std::string_view kMovAh = "mov    ah,0x";
    if (text.starts_with(kMovAh)) {
        std::size_t end = kMovAh.size();
        while (end < text.size() && ((text[end] >= '0' && text[end] <= '9') || (text[end] >= 'a' && text[end] <= 'f'))) ++end;
        const std::string_view digits = text.substr(kMovAh.size(), end - kMovAh.size());
        if (const auto function = text::parse_int<std::uint32_t>(digits, 16)) {
            if (const char* name = dos_function_name(*function)) line += std::format("      ; AH={} DOS {}", digits, name);
        }
    }
    if (text.starts_with("int    0x21")) line += "      ; DOS call";
    return line;
}

std::string function_report(const Seg& seg, std::uint32_t start, bool body) {
    const FunctionRows f = seg.function_rows(start);
    const std::vector<std::uint32_t> args = seg.args_used(start);
    std::vector<Dispatch> dispatch = seg.dispatch(start);
    std::string out = std::format("==== {}  seg{}:0x{:04x}  ({} instrs, 0x{:04x}..0x{:04x})\n", f.func.name, seg.index(), start, f.rows.size(), start, f.func.end);
    if (!args.empty()) out += std::format("   args (PASCAL frame slots read): {}\n", args_text(args));
    if (!dispatch.empty()) {
        std::sort(dispatch.begin(), dispatch.end(), [](const Dispatch& a, const Dispatch& b) { return std::tie(a.message, a.handler) < std::tie(b.message, b.handler); });
        out += std::format("   WndProc dispatch ({} messages):\n", dispatch.size());
        for (const Dispatch& d : dispatch) out += std::format("      {:<20} 0x{:04x} -> handler @0x{:04x}\n", message_name(d.message), d.message, d.handler);
    }
    const std::vector<Call> calls = seg.calls_in(start);
    if (!calls.empty()) {
        if (const std::string s = joined_calls(seg, calls, CallKind::System); !s.empty()) out += "   syscalls: " + s + "\n";
        if (const std::string s = joined_calls(seg, calls, CallKind::Internal); !s.empty()) out += "   internal: " + s + "\n";
        if (const std::string s = joined_calls(seg, calls, CallKind::Far); !s.empty()) out += "   far:      " + s + "\n";
    }
    if (body) {
        out += "\n";
        for (const Row& row : f.rows) out += annotate_row(seg, row) + "\n";
    }
    out += "\n";
    return out;
}

std::vector<std::uint32_t> call_graph(const Seg& seg, std::uint32_t start) {
    std::vector<std::uint32_t> order;
    std::set<std::uint32_t> seen;
    std::deque<std::uint32_t> queue{start};
    while (!queue.empty()) {
        const std::uint32_t s = queue.front();
        queue.pop_front();
        if (!seen.insert(s).second) continue;
        order.push_back(s);
        for (const Call& call : seg.calls_in(s)) {
            if (call.kind == CallKind::Internal && !seen.contains(call.target)) queue.push_back(call.target);
        }
    }
    return order;
}

// ---- the reports ---------------------------------------------------------------------------------------------------------------------

std::string map_report(const ne::Module& module, const SharedOrdinals& ordinals, const fixups::Objdump& objdump) {
    const std::vector<ne::Name> resident = module.resident_names(), nonresident = module.nonresident_names();
    const std::vector<ne::Segment> segments = module.segments();
    std::string out = std::format("# {}   {}\n", resident.empty() ? "?" : resident.front().name, nonresident.empty() ? "" : nonresident.front().name);
    out += std::format("# imports: {}\n", text::join(module.module_refs(), ", "));
    out += std::format("# {} segments\n\n", segments.size());
    const std::map<std::uint32_t, std::string> by_ordinal = names_by_ordinal(module);
    out += "Exports:\n";
    std::vector<ne::Entry> entries = module.entries();
    std::stable_sort(entries.begin(), entries.end(), [](const ne::Entry& a, const ne::Entry& b) { return a.ordinal < b.ordinal; });
    for (const ne::Entry& e : entries) {
        if (e.kind == ne::Entry::Kind::Unused) continue;
        const auto name = by_ordinal.find(e.ordinal);
        out += std::format("   @{:<4} {:24} seg{}:0x{:04x}\n", e.ordinal, name != by_ordinal.end() ? name->second : std::string(), e.segment, e.offset);
    }
    out += "\n";
    for (const ne::Segment& s : segments) {
        if (s.is_data()) {
            out += std::format("seg{}: DATA 0x{:x} bytes\n", s.index, s.length);
            continue;
        }
        const Seg seg(module, s.index, ordinals, {}, objdump);
        std::size_t system = 0;
        for (const auto& [site, target] : seg.fixups()) system += target.segment ? 0 : 1;
        out += std::format("seg{}: CODE 0x{:x} bytes  {} functions discovered  {} fixups ({} syscalls)\n", s.index, s.length, seg.functions().size(),
                           seg.fixups().size(), system);
    }
    return out;
}

std::string list_report(const ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, const fixups::Objdump& objdump) {
    const Seg seg(module, segment, ordinals, {}, objdump);
    std::string out = std::format("# {} functions in seg{}\n\n", seg.functions().size(), segment);
    for (const auto& [start, func] : seg.functions()) out += function_report(seg, start, false);
    return out;
}

std::string function_listing(const ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t address, const fixups::Objdump& objdump) {
    return function_report(Seg(module, segment, ordinals, {}, objdump), address, true);
}

std::string skeleton(const ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t start, const fixups::Objdump& objdump) {
    constexpr std::uint32_t kInitDialog = 0x110;
    const Seg seg(module, segment, ordinals, {}, objdump);
    std::vector<Dispatch> dispatch = seg.dispatch(start);
    std::sort(dispatch.begin(), dispatch.end(), [](const Dispatch& a, const Dispatch& b) { return std::tie(a.message, a.handler) < std::tie(b.message, b.handler); });
    std::vector<std::uint32_t> handlers;
    for (const Dispatch& d : dispatch) handlers.push_back(d.handler);
    std::sort(handlers.begin(), handlers.end());
    const bool dialog = std::any_of(dispatch.begin(), dispatch.end(), [](const Dispatch& d) { return d.message == kInitDialog; });
    const char* handle = dialog ? "hDlg" : "hWnd";

    std::string out = std::format("/* AUTO-SKELETON from ne-asm segment --cskel  seg{}:0x{:04x}  ({}) */\n", segment, start, dialog ? "dialog proc" : "window proc");
    out += std::format("{} FAR PASCAL {}(HWND {}, unsigned message, WORD wParam, LONG lParam)\n{{\n    switch (message) {{\n", dialog ? "BOOL" : "long",
                       seg.name_at(start), handle);
    for (const Dispatch& d : dispatch) {
        const char* known = window_message_name(d.message);
        const std::vector<std::string> calls = seg.handler_calls(start, d.handler, handlers);
        out += std::format("    case {}:  /* @0x{:04x}  {} */\n        break;\n", known ? std::string(known) : std::format("0x{:04x}", d.message), d.handler,
                           calls.empty() ? std::string("(no resolved calls)") : "calls: " + text::join(calls, "; "));
    }
    if (dialog) {
        out += "    }\n    return FALSE;\n}\n";
    } else {
        out += std::format("    default:\n        return DefWindowProc({}, message, wParam, lParam); /* USER.107 */\n    }}\n    return 0L;\n}}\n", handle);
    }
    std::vector<std::uint32_t> reachable = call_graph(seg, start);
    std::erase(reachable, start);
    if (reachable.empty()) return out;
    out += std::format("\n/* --- {} reachable internal functions (documented-partial templates) --- */\n", reachable.size());
    for (const std::uint32_t s : reachable) {
        const FunctionRows f = seg.function_rows(s);
        const std::vector<std::uint32_t> args = seg.args_used(s);
        const std::vector<Call> calls = seg.calls_in(s);
        out += std::format("/* {}  seg{}:0x{:04x}  ({} instrs; args {})\n", f.func.name, seg.index(), s, f.rows.size(), args.empty() ? std::string("none") : args_text(args));
        if (const std::string c = joined_calls(seg, calls, CallKind::System); !c.empty()) out += " *   syscalls: " + c + "\n";
        if (const std::string c = joined_calls(seg, calls, CallKind::Internal); !c.empty()) out += " *   internal: " + c + "\n";
        out += " *   [documented partial] */\n";
    }
    return out;
}

std::string wndprocs_report(const ne::Module& module, const SharedOrdinals& ordinals, std::optional<std::size_t> only_segment, const fixups::Objdump& objdump) {
    const std::map<std::uint32_t, std::string> by_ordinal = names_by_ordinal(module);
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::string> exported;   // (segment, offset) -> export name
    for (const ne::Entry& e : module.entries()) {
        if (e.kind == ne::Entry::Kind::Unused) continue;
        const auto name = by_ordinal.find(e.ordinal);
        if (name != by_ordinal.end() && !name->second.empty()) exported[{e.segment, e.offset}] = name->second;
    }
    std::string out;
    for (const ne::Segment& s : module.segments()) {
        if (s.is_data() || (only_segment && *only_segment != 0 && s.index != *only_segment)) continue;
        const Seg seg(module, s.index, ordinals, {}, objdump);
        for (const auto& [start, func] : seg.functions()) {
            std::vector<Dispatch> dispatch = seg.dispatch(start);
            if (dispatch.size() < 2) continue;
            std::sort(dispatch.begin(), dispatch.end(), [](const Dispatch& a, const Dispatch& b) { return std::tie(a.message, a.handler) < std::tie(b.message, b.handler); });
            const auto name = exported.find({s.index, start});
            out += std::format("seg{}:0x{:04x}{}  - {} messages\n", s.index, start, name != exported.end() ? "  [export " + name->second + "]" : std::string(), dispatch.size());
            for (const Dispatch& d : dispatch) out += std::format("    {:<20} 0x{:04x} -> 0x{:04x}\n", message_name(d.message), d.message, d.handler);
            out += "\n";
        }
    }
    return out;
}

std::string trace_report(const ne::Module& module, const SharedOrdinals& ordinals, std::size_t segment, std::uint32_t start, const fixups::Objdump& objdump) {
    const Seg seg(module, segment, ordinals, {}, objdump);
    const std::vector<std::uint32_t> order = call_graph(seg, start);
    std::string out = std::format("# call graph from seg{}:0x{:04x} - {} reachable functions in this segment\n\n", segment, start, order.size());
    // the whole external API surface, by module and ordinal
    std::map<std::string, std::string> surface;
    for (const std::uint32_t s : order) {
        for (const Call& call : seg.calls_in(s)) {
            if (call.kind == CallKind::System) surface[call.label] = describe_call(seg, call);
        }
    }
    const auto sort_key = [](const std::string& label) {
        const std::size_t dot = label.find('.');
        const std::string module_part = label.substr(0, dot);
        const std::string rest = dot == std::string::npos ? std::string() : label.substr(dot + 1);
        const auto number = text::parse_int<std::uint32_t>(rest);
        return std::make_tuple(module_part, number ? 0 : 1, number.value_or(0), number ? std::string() : rest);
    };
    std::vector<const std::pair<const std::string, std::string>*> shown;
    for (const auto& entry : surface) shown.push_back(&entry);
    std::sort(shown.begin(), shown.end(), [&](const auto* a, const auto* b) { return sort_key(a->first) < sort_key(b->first); });
    out += "External API surface (all reachable functions):\n";
    for (const auto* entry : shown) out += "   " + entry->second + "\n";
    out += "\n" + std::string(70, '=') + "\n\n";
    for (const std::uint32_t s : order) out += function_report(seg, s, true);
    return out;
}

// ---- the command ------------------------------------------------------------------------------------------------------------------

int main(std::span<const std::string> args, const fixups::Objdump& objdump) {
    const cli::Command command{
        "segment",
        "a call-graph-tracing disassembler for the code segments of an NE module",
        {cli::Positional{"file", "an NE module"},
         cli::Positional{"segment", "the segment (from 1; default 1)", false},
         cli::Option{"--map", "", "exports, segments and the function inventory", cli::Arg::Flag},
         cli::Option{"--wndprocs", "", "the window procedures of the code segments", cli::Arg::Flag},
         cli::Option{"--cskel", "", "a C skeleton of the window or dialog procedure at this address", cli::Arg::Value, "ADDR"},
         cli::Option{"--list", "", "every function discovered in the segment", cli::Arg::Flag},
         cli::Option{"--fn", "", "the function at this address, annotated", cli::Arg::Value, "ADDR"},
         cli::Option{"--trace", "", "an ordinal or an address and its whole reachable call graph", cli::Arg::Value, "ORD|ADDR"},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const SharedOrdinals ordinals = std::make_shared<const Ordinals>(load_ordinals(fs::from_text(a.required("--ords"))));
            const ne::Module module = fixups::open_module(fs::from_text(a.required("file")));
            const auto segment = a.integer<std::size_t>("segment");
            if (a.flag("--map")) {
                out_text(map_report(module, ordinals, objdump));
            } else if (a.flag("--wndprocs")) {
                out_text(wndprocs_report(module, ordinals, segment, objdump));
            } else if (const auto skeleton_at = a.integer<std::uint32_t>("--cskel")) {
                out_text(skeleton(module, ordinals, segment.value_or(1), *skeleton_at, objdump));
            } else if (a.flag("--list")) {
                out_text(list_report(module, ordinals, segment.value_or(1), objdump));
            } else if (const auto function_at = a.integer<std::uint32_t>("--fn")) {
                out_text(function_listing(module, ordinals, segment.value_or(1), *function_at, objdump));
            } else if (const auto target = a.value("--trace")) {
                std::size_t seg = segment.value_or(1);
                std::uint32_t start = 0;
                if (target->starts_with("0x")) {
                    const auto traced_address = text::parse_int<std::uint32_t>(*target, 0);
                    if (!traced_address) throw UsageError("--trace expects an ordinal or an address, got '" + *target + "'");
                    start = *traced_address;
                } else {   // an ordinal: find its segment and offset
                    const auto ordinal = text::parse_int<std::uint32_t>(*target, 0);
                    if (!ordinal) throw UsageError("--trace expects an ordinal or an address, got '" + *target + "'");
                    const auto entry = find_entry(module, *ordinal);
                    if (!entry) {
                        out("ordinal {} not found in entry table\n", *target);
                        return kExitFailed;
                    }
                    seg = entry->first;
                    start = entry->second;
                }
                out_text(trace_report(module, ordinals, seg, start, objdump));
            } else {
                throw UsageError("choose one of --map, --wndprocs, --cskel, --list, --fn, --trace");
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("segment", {command});
    std::vector<std::string> line = {"segment"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::segment
