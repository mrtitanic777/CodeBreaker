// ui/mcp_code.cpp -- the code itself for the MCP server: disassembly, cross-references and where an address is stored in the file, over a module
// as the analyses see it (analyze/image: a 32-bit module as it is, a 16-bit one as the decompiler lays it out).
//
// A module is read once and kept (an hour at most); its functions are the ones the file gives away (a 32-bit module's carve:
// formats/pe_functions) plus those discovery adds. Each tool answers a document: `disasm` the listing of a function or of the instructions from an
// address, `xrefs` what refers to an address or function and what a function calls. The listing lines read "address  [file offset]  bytes
// instruction ; what the instruction refers to" (a function, an import by name, a string).
//
// Design notes:
//   * a driver (LE) is not read here yet: its tools say so.
#include <algorithm>
#include <chrono>
#include <cstring>
#include <format>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "analyze/call_graph.hpp"
#include "analyze/image.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "decompile/describe.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "formats/pe_functions.hpp"
#include "symbols/ordinals.hpp"
#include "ui/mcp_wiring.hpp"

namespace cb::ui::mcp::wiring {
namespace {

namespace an = cb::analyze;
namespace cg = cb::analyze::call_graph;
using CodeVa = an::Va;

std::string code_hex(CodeVa v) { return std::format("{:x}", static_cast<std::uint64_t>(v)); }
std::string code_quoted(std::string_view text) { return "'" + std::string(text) + "'"; }

// An address as the tools take it: hex with or without 0x or a trailing h, in lower case.
std::string code_address_text(std::string_view text) {
    std::string a = text::lower(text::trim(text));
    if (a.starts_with("0x")) return a.substr(2);
    if (a.size() > 1 && a.ends_with('h')) {
        while (a.ends_with('h')) a.pop_back();
    }
    return a;
}

bool code_all_hex(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0 && !std::isupper(static_cast<unsigned char>(c)); });
}

// ---- one module -------------------------------------------------------------------------------------------------------------

struct CodeThunk {
    CodeVa slot = 0;
    std::string import;
};

struct CodeSite {
    CodeVa function = 0, site = 0;
    std::string how;
    friend bool operator<(const CodeSite& a, const CodeSite& b) { return std::tie(a.site, a.function, a.how) < std::tie(b.site, b.function, b.how); }
};

struct CodeImage {
    std::filesystem::path path;
    std::string name;   // the file's name in lower case
    bool pe = false;
    std::unique_ptr<an::Image> image;
    std::map<CodeVa, std::string> exports;
    CodeVa base = 0;
    std::optional<formats::pe::Image> file;   // a 32-bit module's file, for where an address is stored
    std::optional<cg::Graph> graph;
    std::optional<std::map<CodeVa, std::vector<CodeSite>>> index;
    std::map<CodeVa, std::optional<CodeThunk>> thunks;
    std::chrono::steady_clock::time_point made = std::chrono::steady_clock::now();

    bool has_name(CodeVa va) const { return image->is_function(va) && !image->function_name(va).empty(); }
    const std::string& name_of(CodeVa va) const { return image->function_name(va); }
};

// ---- reading a module -------------------------------------------------------------------------------------------------------

bool code_plain_symbol(std::string_view name) {
    auto first = [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '?' || c == '@' || c == '$'; };
    return !name.empty() && first(name[0]) && std::all_of(name.begin(), name.end(), [&](char c) { return first(c) || std::isdigit(static_cast<unsigned char>(c)); });
}

std::unique_ptr<CodeImage> code_read_pe(const std::filesystem::path& path) {
    auto out = std::make_unique<CodeImage>();
    out->path = path;
    out->name = text::lower(path.filename().string());
    out->pe = true;
    formats::pe::Image pe = formats::pe::Image::open(path);
    if (pe.header().machine != formats::pe::kMachineI386) throw UsageError(std::format("{}: not an x86 module", path.filename().string()));
    out->base = static_cast<CodeVa>(pe.header().image_base);
    an::ImageSpec spec = an::image::pe_spec(pe);
    // the starts are the carve's (the exports, the entry point, the calls and prologues the sweep saw), named by the export or sub_<offset>
    const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(path);
    spec.functions.clear();
    spec.function_names.clear();
    for (const auto& [va, f] : carve.functions()) {
        const CodeVa start = static_cast<CodeVa>(va);
        spec.functions.emplace_back(start, static_cast<CodeVa>(f.end));
        spec.function_names[start] = f.name && code_plain_symbol(*f.name) ? *f.name : an::image::auto_name(an::ImageKind::Pe, out->base, start);
    }
    for (const auto& [va, name] : carve.exports()) out->exports[static_cast<CodeVa>(va)] = name;
    std::set<CodeVa> roots;
    for (const auto& [va, name] : carve.exports()) roots.insert(static_cast<CodeVa>(va));
    if (const auto entry = pe.entry_va()) roots.insert(static_cast<CodeVa>(*entry));
    spec.roots.assign(roots.begin(), roots.end());
    out->image = std::make_unique<an::Image>(an::image::load(std::move(spec), out->base));
    out->file.emplace(std::move(pe));
    return out;
}

std::unique_ptr<CodeImage> code_read_ne(Context& context, const std::filesystem::path& path) {
    auto out = std::make_unique<CodeImage>();
    out->path = path;
    out->name = text::lower(path.filename().string());
    const formats::ne::Module module = formats::ne::Module::open_file(path);
    decompile::decompiler::Decompiler& decompiler = context.kit().decompiler();
    const std::filesystem::path work = context.kit().scratch() / "mcp" / out->name;
    fs::make_dirs(work);
    decompile::describe::NeDescription d = decompile::describe::describe16(module, work, decompiler.context().environment->knowledge());

    an::image::NeLayout layout;
    const std::vector<formats::ne::Segment> segments = module.segments();
    std::map<int, const formats::ne::Segment*> by_index;
    for (const formats::ne::Segment& s : segments) by_index[s.index] = &s;
    for (const auto& [index, bytes] : d.images) {
        if (bytes.empty()) continue;
        const auto seg = by_index.find(static_cast<int>(index));
        if (seg == by_index.end()) continue;
        an::image::NeSegment s;
        s.index = static_cast<int>(index);
        s.bytes = bytes;
        s.code = !seg->second->is_data();
        s.file_length = seg->second->file_offset != 0 ? static_cast<std::int64_t>(seg->second->length) : 0;
        layout.segments.push_back(std::move(s));
    }
    for (const auto& [va, at] : d.where_map) {
        const auto name = d.names.find(va);
        layout.functions.push_back({static_cast<int>(at.seg), at.off, name != d.names.end() ? name->second : std::string()});
    }
    for (const auto& [seg, length] : d.code) layout.code_length[static_cast<int>(seg)] = static_cast<std::uint32_t>(length);

    // the imports by the stand-in addresses: "KERNEL_17" is KERNEL's export 17, named when its tables name it
    for (const std::string& raw : d.imports) {
        std::string name = raw;
        const std::size_t cut = raw.rfind('_');
        if (cut != std::string::npos && cut > 0 && cut + 1 < raw.size() &&
            std::all_of(raw.begin() + static_cast<std::ptrdiff_t>(cut) + 1, raw.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; })) {
            const std::string module_name = text::upper(raw.substr(0, cut));
            const std::uint32_t number = static_cast<std::uint32_t>(std::stoul(raw.substr(cut + 1)));
            const auto table = context.modules().table(module_name);
            std::string shown = "#" + raw.substr(cut + 1);
            if (table) {
                const auto real = table->names.find(number);
                if (real != table->names.end()) shown = real->second;
            }
            name = module_name + "!" + shown;
        }
        layout.imports.push_back(name);
    }
    // the exports and the program's entry are entered from outside
    const std::vector<formats::ne::Entry> entries = module.entries();
    std::map<std::uint32_t, formats::ne::Entry> by_ordinal;
    for (const formats::ne::Entry& e : entries) {
        if (e.kind == formats::ne::Entry::Kind::Unused) continue;
        by_ordinal[e.ordinal] = e;
        const CodeVa va = decompile::describe::address(e.segment, e.offset);
        const auto name = d.names.find(va);
        if (name == d.names.end() || !d.where_map.contains(va)) continue;
        layout.entry_points.emplace_back(static_cast<int>(e.segment), static_cast<std::uint32_t>(e.offset));
        if (!name->second.starts_with("sub_")) out->exports[va] = name->second;
    }
    const std::uint32_t cs = module.header().cs_ip >> 16, ip = module.header().cs_ip & 0xFFFF;
    if (cs != 0 && d.names.contains(decompile::describe::address(cs, ip))) layout.entry_points.emplace_back(static_cast<int>(cs), ip);
    layout.data_segment = static_cast<int>(d.data_segment);

    // where the loader writes an address: every place a relocation record (and its chain) names
    const std::vector<std::string> refs = module.module_refs();
    std::map<std::string, std::size_t> extern_index;
    for (std::size_t k = 0; k < d.imports.size(); ++k) extern_index.emplace(d.imports[k], k);
    const ByteView file = module.data();
    for (const formats::ne::Segment& s : segments) {
        for (const formats::ne::Relocation& r : module.relocations(s)) {
            std::optional<CodeVa> target;
            if (const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target)) {
                if (internal->segment == 0xFF) {
                    const auto e = by_ordinal.find(internal->entry);
                    if (e != by_ordinal.end()) target = decompile::describe::address(e->second.segment, e->second.offset);
                } else {
                    target = decompile::describe::address(internal->segment, internal->entry);
                }
            } else if (const auto* ordinal = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                const std::string mod = ordinal->module >= 1 && ordinal->module <= refs.size() ? refs[ordinal->module - 1u] : "?";
                const auto k = extern_index.find(text::replace_all(std::format("{}_{}", mod, ordinal->ordinal), "-", "_"));
                if (k != extern_index.end()) target = an::image::extern_address(k->second);
            } else if (const auto* named = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                std::string nm = module.imported_name(named->name_offset);
                for (char& c : nm)
                    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') c = '_';
                const auto k = extern_index.find(nm);
                if (k != extern_index.end()) target = an::image::extern_address(k->second);
            } else {
                continue;
            }
            an::Pointer kind;
            kind.kind = r.source == 3 ? an::PointerKind::Far : r.source == 2 ? an::PointerKind::Selector : r.source == 5 ? an::PointerKind::Offset : an::PointerKind::Byte;
            kind.target = target;
            std::uint64_t off = r.offset;
            for (int hops = 0; off != 0xFFFF && off + 1 < s.length && hops < 0x10000; ++hops) {
                layout.pointers.emplace_back(an::image::ne_address(s.index, static_cast<std::uint32_t>(off)), kind);
                if (r.additive()) break;
                const std::uint64_t at = s.file_offset + off;
                if (at + 2 > file.size()) break;
                off = static_cast<std::uint64_t>(file[at]) | (static_cast<std::uint64_t>(file[at + 1]) << 8);
            }
        }
    }
    out->image = std::make_unique<an::Image>(an::image::load(an::image::ne_spec(layout)));
    return out;
}

}  // namespace

// ---- the reader ---------------------------------------------------------------------------------------------------------------

namespace {

class CodeReader {
public:
    explicit CodeReader(std::shared_ptr<Context> context) : context_(std::move(context)) {}

    Json disasm(const std::filesystem::path& path, std::string_view addr, std::int64_t count);
    Json xrefs(const std::filesystem::path& path, std::string_view addr);
    std::optional<std::pair<std::int64_t, std::optional<std::int64_t>>> locate(const std::filesystem::path& path, std::string_view addr);

private:
    CodeImage& image(const std::filesystem::path& path);
    std::string import_name(std::string_view text);
    CodeVa place(CodeImage& ci, std::string_view text);
    const std::optional<CodeThunk>& thunk(CodeImage& ci, CodeVa va);
    std::string label(CodeImage& ci, CodeVa va);
    std::optional<CodeVa> destination(CodeImage& ci, const an::Instruction& ins);
    std::string note(CodeImage& ci, const an::Instruction& ins, std::optional<CodeVa> fn_start);
    std::optional<std::string> ascii_at(CodeImage& ci, CodeVa va);
    std::optional<std::int64_t> file_offset(CodeImage& ci, CodeVa va);
    const std::map<CodeVa, std::vector<CodeSite>>& index_of(CodeImage& ci);

    std::shared_ptr<Context> context_;
    std::mutex lock_;
    std::map<std::string, std::unique_ptr<CodeImage>> kept_;
};

CodeImage& CodeReader::image(const std::filesystem::path& path) {
    const std::string key = fs::to_text(path);
    const auto hit = kept_.find(key);
    if (hit != kept_.end() && std::chrono::steady_clock::now() - hit->second->made < std::chrono::hours{1}) return *hit->second;
    const std::optional<base::Lane> lane = context_->corpus().lane(path);
    std::unique_ptr<CodeImage> made;
    // (a module that cannot be read says what kind of failure it was)
    try {
        if (lane == base::Lane::Pe) {
            made = code_read_pe(path);
        } else if (lane == base::Lane::Ne) {
            made = code_read_ne(*context_, path);
        } else if (lane == base::Lane::Le) {
            throw UsageError(std::format("{}: a driver is not read by this server yet (the 32-bit and 16-bit modules are)", path.filename().string()));
        } else {
            throw UsageError(std::format("{}: not a 32-bit module, a 16-bit module or a driver", path.filename().string()));
        }
    } catch (const UsageError& e) {
        throw UsageError(std::string("ValueError: ") + e.what());
    } catch (const FormatError& e) {
        throw UsageError(std::string("ValueError: ") + e.what());
    } catch (const IoError& e) {
        throw UsageError(std::string("OSError: ") + e.what());
    }
    CodeImage& ref = *made;
    kept_[key] = std::move(made);
    return ref;
}

// "KERNEL32!#17" as "KERNEL32!CommonUnimpStub (#17)" when the number has a known name.
std::string CodeReader::import_name(std::string_view text_in) {
    const std::size_t bang = text_in.find('!');
    if (bang == std::string_view::npos) return std::string(text_in);
    const std::string_view module = text_in.substr(0, bang), nm = text_in.substr(bang + 1);
    if (nm.size() < 2 || nm[0] != '#' || !std::all_of(nm.begin() + 1, nm.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; })) return std::string(text_in);
    const std::uint32_t ordinal = static_cast<std::uint32_t>(std::stoul(std::string(nm.substr(1))));
    std::optional<std::string> real = context_->bench().ordinals().name(module, ordinal);
    if (!real)
        if (const auto doc = symbols::ordinals::documented(module, ordinal)) real = doc->name;
    if (!real) return std::string(text_in);
    return std::format("{}!{} (#{})", module, *real, ordinal);
}

// The address `text` stands for in an image: hex (0x optional; an RVA below a 32-bit module's base counts from the base), segment:offset in a
// 16-bit module, or a name.
CodeVa CodeReader::place(CodeImage& ci, std::string_view text_in) {
    const std::string a = code_address_text(text_in);
    if (const std::size_t colon = a.find(':'); colon != std::string::npos && ci.image->bits() == 16 && code_all_hex(a.substr(0, colon)) && code_all_hex(a.substr(colon + 1)))
        return decompile::describe::address(static_cast<CodeVa>(std::stoull(a.substr(0, colon), nullptr, 16)), static_cast<CodeVa>(std::stoull(a.substr(colon + 1), nullptr, 16)));
    if (code_all_hex(a) && a.size() <= 16) {
        const CodeVa va = static_cast<CodeVa>(std::stoull(a, nullptr, 16));
        return ci.image->flat() && va < ci.base ? va + ci.base : va;
    }
    const std::string low = text::lower(text::trim(text_in));
    for (const CodeVa va : ci.image->functions()) {
        const std::string& nm = ci.name_of(va);
        if (!nm.empty() && text::lower(nm) == low) return va;
    }
    for (const auto& [va, nm] : ci.exports)
        if (!nm.empty() && text::lower(nm) == low) return va;
    // names given elsewhere: the store, other modules
    {
        std::lock_guard hold(context_->lock());
        const symbols::names::ModuleNames named = context_->bench().namer().of_module(ci.path);
        for (const symbols::names::FunctionName& f : named.functions)
            if (!f.name.empty() && text::lower(f.name) == low) return static_cast<CodeVa>(f.address);
    }
    throw UsageError(std::format("no address or function named {} in {} (an address in hex, or a name `names` lists)", code_quoted(text_in), ci.name));
}

const std::optional<CodeThunk>& CodeReader::thunk(CodeImage& ci, CodeVa va) {
    const auto hit = ci.thunks.find(va);
    if (hit != ci.thunks.end()) return hit->second;
    std::optional<CodeThunk> out;
    const an::Span* s = ci.image->span(va);
    if (s != nullptr && s->executable) {
        const an::Function one = ci.image->linear(va, 1);
        if (!one.insns.empty()) {
            const an::Instruction& ins = one.insns.front();
            if (ins.flow == an::Flow::Jump && !ins.target && !ins.operands.empty() && ins.operands[0].kind == an::Operand::Kind::Memory && ins.operands[0].base.empty() &&
                ins.operands[0].index.empty()) {
                const CodeVa slot = ins.operands[0].value;
                if (ci.image->is_import(slot)) out = CodeThunk{slot, ci.image->import_name(slot)};
            }
        }
    }
    return ci.thunks.emplace(va, std::move(out)).first->second;
}

// A function's name, with the import it only jumps to; else name+offset, else the address.
std::string CodeReader::label(CodeImage& ci, CodeVa va) {
    const std::optional<CodeThunk>& t = thunk(ci, va);
    if (ci.has_name(va)) return t ? std::format("{} (thunk: {})", ci.name_of(va), import_name(t->import)) : ci.name_of(va);
    if (t) return std::format("thunk at {}: {}", code_hex(va), import_name(t->import));
    if (const std::optional<CodeVa> owner = ci.image->owner(va)) return std::format("{}+{}", ci.name_of(*owner), code_hex(va - *owner));
    return code_hex(va);
}

// Where a call or jump goes: the function, or the address standing for the import; none when it is decided at run time.
std::optional<CodeVa> CodeReader::destination(CodeImage& ci, const an::Instruction& ins) {
    if (ins.target) return ins.target;
    if (!ins.operands.empty() && ins.operands[0].kind == an::Operand::Kind::Memory && ins.operands[0].base.empty() && ins.operands[0].index.empty()) {
        const CodeVa slot = ins.operands[0].value;
        if (ci.image->is_import(slot)) return slot;
    }
    return std::nullopt;
}

// The text stored at an address, when there is some (4 printable characters or more; code sections hold strings too).
std::optional<std::string> CodeReader::ascii_at(CodeImage& ci, CodeVa va) {
    const an::Span* s = ci.image->span(va);
    if (s == nullptr || va - s->va >= s->initialised) return std::nullopt;
    const std::size_t from = static_cast<std::size_t>(va - s->va);
    const std::size_t to = std::min<std::size_t>(s->data.size(), from + 40);
    std::string text;
    for (std::size_t i = from; i < to && s->data[i] != 0; ++i) text.push_back(static_cast<char>(s->data[i]));
    if (text.size() >= 4 && std::all_of(text.begin(), text.end(), [](char c) { return c >= 32 && c < 127; })) return text;
    return std::nullopt;
}

// What an instruction's destination or address operand stands for.
std::string CodeReader::note(CodeImage& ci, const an::Instruction& ins, std::optional<CodeVa> fn_start) {
    const bool branch = ins.flow == an::Flow::Call || ins.flow == an::Flow::Jump || ins.flow == an::Flow::CondJump;
    if (branch) {
        const std::optional<CodeVa> dest = destination(ci, ins);
        if (dest && ci.image->is_import(*dest)) return import_name(ci.image->import_name(*dest));
        if (dest && ins.flow != an::Flow::CondJump) {
            const std::optional<CodeVa> owner = ci.image->owner(*dest);
            if (ci.has_name(*dest) || (owner && owner != fn_start)) return label(ci, *dest);
            if (!owner) return std::format("{}: no known function starts there", code_hex(*dest));
        }
    }
    for (const an::Operand& op : ins.operands) {
        std::optional<CodeVa> va;
        if (op.kind == an::Operand::Kind::Immediate && op.size == 4) va = op.value;
        else if (op.kind == an::Operand::Kind::Memory && op.base.empty()) va = op.value;
        if (!va) continue;
        if (ci.image->is_import(*va)) return import_name(ci.image->import_name(*va));
        if (const std::optional<std::string> t = ascii_at(ci, *va)) return "\"" + *t + "\"";
        if (ci.has_name(*va)) return ci.name_of(*va);
    }
    return "";
}

// Where a 32-bit module's address is in its file, or none (not stored there, or not a PE).
std::optional<std::int64_t> CodeReader::file_offset(CodeImage& ci, CodeVa va) {
    if (!ci.pe || !ci.file || va < ci.base) return std::nullopt;
    const std::optional<std::uint64_t> off = ci.file->rva_to_offset(static_cast<std::uint64_t>(va - ci.base));
    return off ? std::optional<std::int64_t>(static_cast<std::int64_t>(*off)) : std::nullopt;
}

std::optional<std::pair<std::int64_t, std::optional<std::int64_t>>> CodeReader::locate(const std::filesystem::path& path, std::string_view addr) {
    std::lock_guard hold(lock_);
    CodeImage& ci = image(path);
    const CodeVa va = place(ci, addr);
    return std::pair<std::int64_t, std::optional<std::int64_t>>{va, file_offset(ci, va)};
}

// The instructions at an address or function name. At a function's start (count 0): the whole function as the analyses decode it, following its
// branches. Anywhere else, or with `count`: that many instructions read straight on from there (64 when count is 0).
Json CodeReader::disasm(const std::filesystem::path& path, std::string_view addr, std::int64_t count) {
    std::lock_guard hold(lock_);
    CodeImage& ci = image(path);
    const CodeVa va = place(ci, addr);
    const std::int64_t n = std::max<std::int64_t>(1, std::min<std::int64_t>(count != 0 ? count : 64, 4000));
    Json out = Json::object();
    std::vector<an::Instruction> insns;
    std::string how;
    if (ci.has_name(va) && count == 0) {
        const an::Function& fn = ci.image->function(va);
        insns = fn.insns;
        how = "the function, as decoded from its start";
        out["function"] = ci.name_of(va);
        out["start"] = code_hex(va);
        out["end"] = code_hex(fn.end);
        out["limit"] = code_hex(fn.limit);
    } else {
        if (ci.image->span(va) == nullptr) throw UsageError(std::format("{} is not in {}", code_hex(va), ci.name));
        insns = ci.image->linear(va, static_cast<int>(n)).insns;
        how = std::format("{} instructions read straight on", insns.size());
        const std::optional<CodeVa> owner = ci.image->owner(va);
        out["inside"] = owner ? Json(std::format("{}+{}", ci.name_of(*owner), code_hex(va - *owner))) : Json(nullptr);
    }
    out["module"] = ci.name;
    out["addr"] = code_hex(va);
    out["how"] = how;
    out["count"] = insns.size();
    std::string listing;
    for (std::size_t i = 0; i < insns.size(); ++i) {
        const an::Instruction& ins = insns[i];
        const Bytes code = ci.image->read(ins.addr, ins.size);
        std::string hex;
        for (std::size_t k = 0; k < code.size(); ++k) hex += std::format("{}{:02X}", k ? " " : "", code[k]);
        const std::optional<std::int64_t> off = ci.pe ? file_offset(ci, ins.addr) : std::nullopt;
        const std::string what = note(ci, ins, va);
        std::string line = std::format("{}{}  {:<20} {:<34}{}", code_hex(ins.addr), off ? std::format("  {:05x}", *off) : std::string(), hex, ins.text, what.empty() ? std::string() : "; " + what);
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t')) line.pop_back();
        if (i) listing += "\n";
        listing += line;
    }
    out["listing"] = listing;
    return out;
}

// {address: [(function, site, how)]}: every direct call, jump and import-slot use in the code.
const std::map<CodeVa, std::vector<CodeSite>>& CodeReader::index_of(CodeImage& ci) {
    if (ci.index) return *ci.index;
    std::map<CodeVa, std::vector<CodeSite>> out;
    for (const CodeVa start : std::vector<CodeVa>(ci.image->functions())) {
        const an::Function& fn = ci.image->function(start);
        for (const an::Instruction& ins : fn.insns) {
            const bool jump = ins.flow == an::Flow::Call || ins.flow == an::Flow::Jump;
            if (ins.target && (jump || (ins.flow == an::Flow::CondJump && !(fn.start <= *ins.target && *ins.target < fn.limit))))
                out[*ins.target].push_back({fn.start, ins.addr, ins.flow == an::Flow::Call ? "call" : ins.flow == an::Flow::Jump ? "jmp" : "jcc"});
            if (!ins.operands.empty() && ins.operands[0].kind == an::Operand::Kind::Memory && ins.operands[0].base.empty() && ins.operands[0].index.empty()) {
                const CodeVa slot = ins.operands[0].value;
                if (ci.image->is_import(slot)) {
                    const char* flow = ins.flow == an::Flow::Call ? "call" : ins.flow == an::Flow::Jump ? "jmp" : ins.flow == an::Flow::CondJump ? "jcc" : ins.flow == an::Flow::Return ? "ret" : "reads";
                    out[slot].push_back({fn.start, ins.addr, std::string(flow) + " [slot]"});
                }
            }
        }
    }
    ci.index = std::move(out);
    return *ci.index;
}

// What refers to an address or function, and what a function calls. `addr` may also be an import (MODULE!name, its name alone, or MODULE!#ordinal).
Json CodeReader::xrefs(const std::filesystem::path& path, std::string_view addr) {
    std::lock_guard hold(lock_);
    CodeImage& ci = image(path);
    if (!ci.graph) ci.graph = cg::build(*ci.image);
    const cg::Graph& g = *ci.graph;
    const auto& index = index_of(ci);

    std::vector<CodeVa> targets;
    try {
        targets.push_back(place(ci, addr));
    } catch (const UsageError&) {
        const std::string low = text::lower(text::trim(addr));
        std::vector<std::pair<CodeVa, std::string>> imports(ci.image->imports().begin(), ci.image->imports().end());
        std::sort(imports.begin(), imports.end());
        auto tail = [](std::string_view s) { return std::string(s.substr(s.rfind('!') == std::string_view::npos ? 0 : s.rfind('!') + 1)); };
        for (const auto& [slot, nm] : imports) {
            const std::string named = text::lower(import_name(nm));
            std::string named_tail = tail(named);
            if (const std::size_t paren = named_tail.find(" ("); paren != std::string::npos) named_tail.resize(paren);
            const std::string plain = text::lower(nm);
            if (low == plain || low == text::lower(tail(nm)) || low == named || low == named_tail) targets.push_back(slot);
        }
        if (targets.empty()) throw;
    }
    Json out = Json::object();
    out["module"] = ci.name;
    {
        std::string joined;
        for (const CodeVa t : targets) joined += (joined.empty() ? "" : " ") + code_hex(t);
        out["addr"] = joined;
    }
    const CodeVa va = targets.front();
    if (ci.image->is_import(va)) {
        out["import"] = import_name(ci.image->import_name(va));
        if (ci.image->bits() == 32) {   // the thunks that jump to it (jmp [slot]), function or not
            std::uint8_t pattern[6] = {0xFF, 0x25, static_cast<std::uint8_t>(va), static_cast<std::uint8_t>(va >> 8), static_cast<std::uint8_t>(va >> 16), static_cast<std::uint8_t>(va >> 24)};
            for (const an::Span& s : ci.image->spans()) {
                if (!s.executable) continue;
                for (std::size_t at = 0; at + 6 <= s.data.size(); ++at)
                    if (std::memcmp(s.data.data() + at, pattern, 6) == 0) targets.push_back(s.va + static_cast<CodeVa>(at));
            }
            if (targets.size() > 1) {
                Json thunks = Json::array();
                for (std::size_t i = 1; i < targets.size(); ++i) thunks.push_back(label(ci, targets[i]));
                out["thunks"] = std::move(thunks);
            }
        }
    } else if (ci.has_name(va)) {
        out["function"] = ci.name_of(va);
    } else if (ci.image->owner(va)) {
        out["inside"] = label(ci, va);
    }
    if (const auto ex = ci.exports.find(va); ex != ci.exports.end()) out["exported_as"] = ex->second;
    if (!ci.image->is_import(va))
        if (const std::optional<CodeThunk>& t = thunk(ci, va)) out["is"] = std::format("a thunk: jmp [{}] = {}", code_hex(t->slot), import_name(t->import));

    std::set<CodeSite> sites;
    for (const CodeVa t : targets) {
        const auto hit = index.find(t);
        if (hit != index.end()) sites.insert(hit->second.begin(), hit->second.end());
    }
    Json from_code = Json::array();
    for (const CodeSite& s : sites) {
        if (from_code.size() >= 500) break;
        from_code.push_back(Json{{"site", code_hex(s.site)}, {"in", label(ci, s.function)}, {"how", s.how}});
    }
    out["from_code"] = std::move(from_code);
    Json referred = Json::array();
    bool any_ref = false;
    for (const cg::Reference& r : g.refs) {
        if (std::find(targets.begin(), targets.end(), r.address) == targets.end()) continue;
        any_ref = true;
        if (referred.size() >= 500) continue;
        const an::Instruction* ins = ci.image->covering(r.instruction);
        const char* how = r.how == cg::Access::Read ? "reads" : r.how == cg::Access::Write ? "writes" : r.how == cg::Access::ReadWrite ? "reads and writes" : "takes the address";
        referred.push_back(Json{{"site", code_hex(r.instruction)}, {"in", label(ci, r.function)}, {"how", how}, {"ins", ins ? ins->text : std::string()}});
    }
    out["referred"] = std::move(referred);
    Json in_data = Json::array();
    if (const auto taken = g.taken.find(va); taken != g.taken.end())
        for (const cg::Taken& t : taken->second)
            if (!t.in_code && in_data.size() < 200) in_data.push_back(code_hex(t.at));
    out["in_data"] = in_data;
    if (ci.has_name(va)) {   // every call, and every jump that leaves the function
        const an::Function& fn = ci.image->function(va);
        Json calls = Json::array();
        for (const an::Instruction& ins : fn.insns) {
            if (ins.flow != an::Flow::Call && ins.flow != an::Flow::Jump) continue;
            const std::optional<CodeVa> d = destination(ci, ins);
            if (ins.flow == an::Flow::Jump) {
                const bool table = !d && std::any_of(fn.tables.begin(), fn.tables.end(), [&](const an::JumpTable& t) { return t.site == ins.addr; });
                const bool inside = d && !ci.image->is_import(*d) && fn.start <= *d && *d < fn.limit;
                if (table || inside) continue;
            }
            Json call = Json::object();
            call["site"] = code_hex(ins.addr);
            call["kind"] = ins.flow == an::Flow::Call ? "call" : "tail";
            call["to"] = !d ? Json("(decided at run time)") : ci.image->is_import(*d) ? Json(import_name(ci.image->import_name(*d))) : Json(label(ci, *d));
            calls.push_back(std::move(call));
        }
        out["calls"] = std::move(calls);
    }
    if (sites.empty() && !any_ref && in_data.empty()) {
        const bool root = std::find(ci.image->roots().begin(), ci.image->roots().end(), va) != ci.image->roots().end();
        out["say"] = ci.exports.contains(va) || root ? "nothing in the module refers to it: it is entered from outside (an export)" : "no code or data here refers to it";
    }
    return out;
}

}  // namespace

void fill_code(Services& services, const std::shared_ptr<Context>& context) {
    auto reader = std::make_shared<CodeReader>(context);
    services.disasm = [reader](const std::filesystem::path& p, std::string_view addr, std::int64_t count) { return reader->disasm(p, addr, count); };
    services.xrefs = [reader](const std::filesystem::path& p, std::string_view addr) { return reader->xrefs(p, addr); };
    services.locate = [reader](const std::filesystem::path& p, std::string_view addr) { return reader->locate(p, addr); };
}

}  // namespace cb::ui::mcp::wiring
