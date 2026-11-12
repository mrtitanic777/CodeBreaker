// cb/decompile/c_text_win16.cpp -- a 16-bit Windows module made compilable: what the module says of each function that its C is
// written with (read from the function's own instructions), each function written by the C back end, and what the module decides
// over all of them -- near or far, the globals, the selectors used as values, the data read through CS, the declarations.
#include <algorithm>
#include <format>
#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/intmath.hpp"
#include "cb/core/json.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/c_text_internal.hpp"

namespace cb::decompile::c_text {

namespace {

// ---- reading a function's instructions -------------------------------------------------------------------------------------

// code[a:b], the way a range past either end gives what is there.
ByteView w16_slice(ByteView code, std::int64_t a, std::int64_t b) {
    const std::int64_t n = static_cast<std::int64_t>(code.size());
    a = std::clamp<std::int64_t>(a, 0, n);
    b = std::clamp<std::int64_t>(b, a, n);
    return code.subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(b - a));
}

// A number as the disassembler writes one: `0x1c` or `28`.
std::optional<std::int64_t> w16_number(std::string_view s) {
    if (s.starts_with("0x")) return text::parse_int<std::int64_t>(s.substr(2), 16);
    return text::parse_int<std::int64_t>(s, 10);
}

bool w16_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// `<reg>, ptr [bp + <n>]`, whole: (reg, n).
std::optional<std::pair<std::string, std::int64_t>> w16_les_operand(std::string_view op) {
    const std::size_t comma = op.find(", ptr [bp + ");
    if (comma == std::string_view::npos || !op.ends_with("]") || comma == 0) return std::nullopt;
    const std::string_view reg = op.substr(0, comma);
    if (!std::all_of(reg.begin(), reg.end(), w16_word_char)) return std::nullopt;
    const std::size_t from = comma + 12;
    const auto n = w16_number(op.substr(from, op.size() - 1 - from));
    if (!n) return std::nullopt;
    return std::make_pair(std::string(reg), *n);
}

// How wide the original reads through the far pointer it takes as argument n: `les reg, [bp+N]`, then the first `es:[reg...]`
// through it. -> {a<n>: bytes}
std::map<std::string, std::int64_t> w16_far_argument_widths(ByteView code, std::int64_t start, std::int64_t end, bool far_fn) {
    const std::int64_t first = far_fn ? 6 : 4;   // (the first argument above the return address)
    std::map<std::string, std::int64_t> out;
    std::map<std::string, std::string> held;
    for (const describe::Insn& ins : describe::decode16(w16_slice(code, start, end), start, false)) {
        if (ins.mn == "les") {
            if (const auto operand = w16_les_operand(ins.op_str)) {
                const std::int64_t n = floor_div<std::int64_t>(operand->second - first, 2) + 1;
                if (n >= 1) held[operand->first] = "a" + std::to_string(n);
                continue;
            }
        }
        std::vector<std::string> registers;
        for (const auto& [reg, arg] : held) registers.push_back(reg);
        for (const std::string& reg : registers) {
            // `(byte|word|dword) ptr es:[<reg>` and then the end of the word
            std::size_t best = std::string::npos;
            std::int64_t width = 0;
            struct Width {
                const char* name;
                std::int64_t bytes;
            };
            constexpr Width kWidths[] = {{"byte", 1}, {"word", 2}, {"dword", 4}};
            for (const auto& [name, bytes] : kWidths) {
                const std::string needle = std::string(name) + " ptr es:[" + reg;
                for (std::size_t at = ins.op_str.find(needle); at != std::string::npos; at = ins.op_str.find(needle, at + 1)) {
                    const std::size_t after = at + needle.size();
                    if (after < ins.op_str.size() && w16_word_char(ins.op_str[after])) continue;
                    if (at < best) {
                        best = at;
                        width = bytes;
                    }
                    break;
                }
            }
            if (best != std::string::npos) {
                out.try_emplace(held.at(reg), width);
                held.erase(reg);
                continue;
            }
            // the register set again (`reg ,` at the start of the operands), or gone
            const std::string_view ops = text::ltrim(ins.op_str);
            const bool set_again = ops.starts_with(reg) && text::ltrim(ops.substr(reg.size())).starts_with(",");
            if (set_again || ins.mn == "call" || ins.mn == "lcall" || ins.mn == "ret" || ins.mn == "retf") held.erase(reg);
        }
    }
    return out;
}

// The offsets from BP of the argument words a function loads as far pointers: `les bx, [bp+6]` takes the word at +6 as an offset
// and the one above it as its segment.
std::set<std::int64_t> w16_far_loads(ByteView code, std::int64_t start, std::int64_t end) {
    std::set<std::int64_t> found;
    for (const describe::Insn& ins : describe::decode16(w16_slice(code, start, end), start, false)) {
        if (ins.mn != "les" && ins.mn != "lds") continue;
        const std::size_t at = ins.op_str.find("[bp + ");
        const std::size_t close = ins.op_str.find(']', at == std::string::npos ? 0 : at);
        if (at == std::string::npos || close == std::string::npos) continue;
        if (const auto n = w16_number(std::string_view(ins.op_str).substr(at + 6, close - at - 6))) found.insert(*n);
    }
    return found;
}

// Whether a function's prologue loads DS with its own data segment: `mov ax, DGROUP` then `mov ds, ax`, among its first instructions
// (before or after `enter`/`push bp`, saves).
bool w16_loads_dgroup(ByteView code, std::int64_t off, std::int64_t dgroup) {
    bool loaded = false;
    for (const describe::Insn& i : describe::decode16(w16_slice(code, off, off + 24), off, false)) {
        if (i.mn == "mov" && i.op_str == std::format("ax, 0x{:x}", dgroup)) {
            loaded = true;
        } else if (i.mn == "mov" && i.op_str == "ds, ax") {
            return loaded;
        } else if (!(i.mn == "push" || i.mn == "enter" || i.mn == "inc" || i.mn == "nop" || i.mn == "sub" || (i.mn == "mov" && i.op_str == "bp, sp"))) {
            return false;
        }
    }
    return false;
}

// Does the function at `off` set up BP as its frame -- Windows' far prologue, `push bp / mov bp, sp`, or `enter`?
bool w16_keeps_frame(ByteView code, std::int64_t off) {
    const ByteView head = w16_slice(code, off, off + 3);
    auto is = [&](std::initializer_list<std::uint8_t> bytes) { return std::equal(head.begin(), head.end(), bytes.begin(), bytes.end()); };
    return describe::windows_far_prologue(code, off) || is({0x55, 0x8b, 0xec}) || is({0x55, 0x89, 0xe5}) || (!head.empty() && head[0] == 0xC8);
}

// ---- what the back end reports, as the library's documents ----------------------------------------------------------------

Json w16_tuple(std::initializer_list<Json> items) { return Json{{"$t", Json(items)}}; }
Json w16_set(const std::vector<Json>& items) { return Json{{"$s", Json(items)}}; }

Json w16_heading_json(const Heading& h) {
    Json w = Json::object();
    if (h.params) {
        Json list = Json::array();
        for (const auto& [name, type] : h.params->list) list.push_back(w16_tuple({name, type}));
        w["params"] = w16_tuple({h.params->text, list});
    } else {
        w["params"] = nullptr;
    }
    Json types = Json::object();
    for (const auto& [name, type] : h.types) types[name] = type;
    w["types"] = types;
    Json calls = Json::array();
    for (const auto& [callee, args] : h.calls) calls.push_back(w16_tuple({callee, Json(args)}));
    w["calls"] = calls;
    w["declaration"] = h.declaration;
    w["fastcall"] = h.fastcall;
    return w;
}

// One function to be written: the Python's tuple (va, distance, model, seg, off, end, dgroup, known, by_selector, callee_pops, shared)
struct Item16 {
    Va va = 0;
    std::optional<std::string> distance, model;
    std::int64_t seg = 0, off = 0, end = 0;
    std::optional<Va> dgroup;
    std::optional<FunctionNotes> known;
    bool by_selector = false;
    bool callee_pops = false;
    SelectorImports selector_values;
    OrderedMap<std::string, std::string> extra;   // what the module decided of it since ("frame_slots" ...): plans by key
};

// What writing one function came to: its text (none where the back end declines), its flags in the module, its notes as the back end
// may have changed them, and what its passes found for the module.
struct Written16 {
    Va va = 0;
    std::optional<std::string> text;
    std::set<std::string> flags;
    FunctionNotes known;
    ModuleFacts found;
};

// One function's C written from its L2 document by the C back end, with what the module says of it (module_facts16).
Written16 w16_write(Services& services, const SegmentImages& images, const std::optional<SegmentImages>& asm_images, const Item16& item) {
    Written16 w;
    w.va = item.va;
    FunctionNotes known = item.known ? *item.known : FunctionNotes();
    const std::int64_t* returned = known.pointers.find("return");
    const bool far_return = returned != nullptr && *returned == 4;
    std::optional<ByteView> code;
    if (const auto it = images.find(item.seg); it != images.end()) code = ByteView(it->second);
    ModuleFacts facts = module_facts16(code, item.off, item.end, item.distance, item.model, item.dgroup, far_return, item.seg);
    facts.selector_structs = item.by_selector;
    facts.callee_pops = item.callee_pops;
    facts.without_notes = true;   // (the module's C: no notes above a function)
    facts.selector_values = item.selector_values;
    facts.plans = item.extra;
    w.text = services.c_from_l2(std::nullopt, known, asm_images, &facts);
    if (known.from_l2) {
        w.found = facts;
        w.found.path = ModuleFacts::Path::Win16;
        const Json left = facts.left ? json::parse(*facts.left) : Json::object();
        w.flags = services.library().flags(internal::document_json(known), left);
    }
    w.known = std::move(known);
    return w;
}

std::string w16_hex(std::int64_t v) { return std::format("{:x}", v); }

const std::string kNoL2 = "it has no L2";

}  // namespace

// ---- what the module says of a function ----------------------------------------------------------------------------------

std::vector<std::int64_t> ss_store_places(ByteView code, std::int64_t start, std::int64_t end) {
    std::vector<std::int64_t> places;
    for (const describe::Insn& ins : describe::decode16(w16_slice(code, start, end), start, false)) {
        // `word ptr [bp - <n>], ss`, whole
        constexpr std::string_view kHead = "word ptr [bp - ", kTail = "], ss";
        if (ins.mn != "mov" || !ins.op_str.starts_with(kHead) || !ins.op_str.ends_with(kTail) || ins.op_str.size() < kHead.size() + kTail.size()) continue;
        const auto n = w16_number(std::string_view(ins.op_str).substr(kHead.size(), ins.op_str.size() - kHead.size() - kTail.size()));
        if (n) places.push_back(*n + 2);
    }
    return places;
}

ModuleFacts module_facts16(std::optional<ByteView> code, std::int64_t off, std::int64_t end, const std::optional<std::string>& distance,
                           const std::optional<std::string>& model, const std::optional<Va>& dgroup, bool far_return,
                           const std::optional<std::int64_t>& seg) {
    ModuleFacts f;
    f.path = ModuleFacts::Path::Win16;
    f.have_code = code.has_value();
    f.distance = distance;
    f.model = model;
    f.far_return = far_return;
    if (seg) f.selector = describe::selector(*seg);
    if (code) {
        f.widths_far = w16_far_argument_widths(*code, off, end, true);
        f.widths_near = w16_far_argument_widths(*code, off, end, false);
        f.loads = w16_far_loads(*code, off, end);
        const ByteView head = w16_slice(*code, off, off + 3);
        auto is = [&](std::initializer_list<std::uint8_t> bytes) { return std::equal(head.begin(), head.end(), bytes.begin(), bytes.end()); };
        f.exported = is({0x8c, 0xd8, 0x90}) || is({0x1e, 0x58, 0x90});   // mov ax, ds / nop (either encoding)
        f.loadds = dgroup && w16_loads_dgroup(*code, off, *dgroup);
        f.ss_stores = ss_store_places(*code, off, end);
    }
    return f;
}

// ---- the module ----------------------------------------------------------------------------------------------------------

Module16 compilable16(Services& services, const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Policy& policy) {
    const translate::native_l2::Library& library = services.library();
    Decompiled16 dec = services.decompile16(module, outdir);
    OrderedMap<std::string, std::string> texts = std::move(dec.texts);
    std::map<Va, std::string> by_va = std::move(dec.by_va);
    describe::NeDescription& d = *dec.description;
    // the module made: one L2 Module, each function a MethodDecl with its C, and what the 16-bit build needs said of the module
    // beside it
    Module16 m;
    m.made = translate::l2::Made(text::lower(module.filename().string()), "");
    m.description = dec.description;
    m.arena = dec.arena;
    for (const auto& [va, place] : d.where_map) m.where.emplace(va, place);
    std::map<Va, std::int64_t> sizes;
    std::vector<StructNote> struct_notes;      // the structures the decompiler recovered
    std::vector<std::string> selector_structs; // ... and those a segment holds
    const bool by_selector = services.recovers_structures(d) && policy.selector_structs;   // (with structures)
    std::map<Va, std::int64_t> elems;          // ... a pointer's -> bytes of what it points at
    std::map<Va, std::int64_t> ends;
    std::map<std::int64_t, std::vector<std::pair<std::int64_t, Va>>> starts;   // where each function's code stops: at the next one
    for (const auto& [va, place] : d.where_map) starts[place.seg].emplace_back(place.off, va);
    for (auto& [seg, list] : starts) {
        std::sort(list.begin(), list.end());
        const auto image = d.images.find(seg);
        const std::int64_t image_size = image == d.images.end() ? 0 : static_cast<std::int64_t>(image->second.size());
        for (std::size_t k = 0; k < list.size(); ++k) ends[list[k].second] = k + 1 < list.size() ? list[k + 1].first : image_size;
    }
    // each function's model as the description gives it: near or far is how it returns, and the C must say it (the decompiler leaves
    // the convention out of a head whose result is a pointer -- and in the medium model an unmarked function is far)
    std::map<std::string, std::optional<std::string>> distance;
    std::map<std::string, std::string> model_of;
    const std::vector<std::vector<std::string>> functions_said = d.records({"F"});
    for (const std::vector<std::string>& parts : functions_said) {
        if (parts.size() <= 4) continue;
        distance[parts[2]] = parts[4].ends_with("far") ? std::optional<std::string>("far") : parts[4].ends_with("near") ? std::optional<std::string>("near") : std::nullopt;
        if (parts[4].starts_with("__stdcall16") || parts[4].starts_with("__cdecl16")) model_of[parts[2]] = parts[4];
    }
    std::optional<Va> dgroup;
    if (d.data_segment != 0) dgroup = describe::selector(d.data_segment);
    const SegmentImages images = d.images;
    // Each function's C written from its L2 document by the C back end, with what the module says of it (the back end's passes on the
    // tree, its heading as the description and its instructions give it)
    std::optional<SegmentImages> asm_images;
    if (services.tree_images()) asm_images = services.segment_images(d);
    // the functions whose calls through pointers remove nothing themselves: the callee does (the arguments are listed in stack order, so
    // __stdcall lays them out as the original pushed them). Each call is kept by its own place -- the function the description named
    // for it may since have been found to be two: the call belongs to the one that holds it, of every function the module will have
    // (those the decompiler could not make among them)
    std::vector<Va> sites;
    for (const std::vector<std::string>& parts : d.records({"O"}))
        if (parts.size() > 4 && parts[4].starts_with("__stdcall")) sites.push_back(*text::parse_int<Va>(parts[1], 16));
    std::set<Va> start_set;
    for (const auto& [va, name] : by_va) start_set.insert(va);
    for (const std::vector<std::string>& parts : functions_said) {
        const Va va = *text::parse_int<Va>(parts[1], 16);
        if (!by_va.contains(va) && !texts.contains(parts[2]) && d.where_map.contains(va)) start_set.insert(va);
    }
    const std::vector<Va> all_starts(start_set.begin(), start_set.end());
    std::set<Va> callee_pops;
    for (const Va site : sites) {
        const auto after = std::upper_bound(all_starts.begin(), all_starts.end(), site);
        if (after == all_starts.begin()) continue;
        const Va start = *(after - 1);
        if ((start >> 16) == (site >> 16)) callee_pops.insert(start);
    }
    std::vector<Item16> items;
    for (const auto& [va, name] : by_va) {
        const SegOff place = d.where_map.at(va);
        Item16 item;
        item.va = va;
        if (const auto it = distance.find(name); it != distance.end()) item.distance = it->second;
        if (const auto it = model_of.find(name); it != model_of.end()) item.model = it->second;
        item.seg = place.seg;
        item.off = place.off;
        item.end = ends.contains(va) ? ends[va] : item.off;
        item.dgroup = dgroup;
        if (const auto it = d.notes.find(name); it != d.notes.end()) item.known = it->second;
        item.by_selector = by_selector;
        item.callee_pops = callee_pops.contains(va);
        item.selector_values = d.selector_imports;   // what the module says that every function is written with: its imports used for their value
        items.push_back(std::move(item));
    }
    std::vector<Written16> written(items.size());
    parallel::parallel_for(items.size(), [&](std::size_t i) { written[i] = w16_write(services, images, asm_images, items[i]); });

    // A function the decompiler could not make stays a function of the module: a body that says so, which the build replaces by the
    // function's own instructions where it was written in assembly, and otherwise reports as not rebuilt (failed). So is one whose C
    // the back end could not write from its L2 -- an Unsupported node, where the core's PrintL2 has no node yet (GDI's sub_22_0B96).
    std::map<std::string, std::set<std::string>> flags_of;
    std::map<std::string, ModuleFacts> found_of;
    const std::map<Va, std::string> first_by_va = by_va;
    for (std::size_t i = 0; i < items.size(); ++i) {
        const Va va = items[i].va;
        const std::string& name = first_by_va.at(va);
        Written16& wr = written[i];
        for (const Interrupt& in : wr.found.interrupts) m.interrupts.insert(in);
        found_of[name] = wr.found;
        d.notes[name] = wr.known;
        if (!wr.text) {
            m.failed[name] = wr.known.l2 != nullptr ? "its C could not be written from L2" : kNoL2;
            texts.erase(name);
            by_va.erase(va);
        } else {
            texts.insert_or_assign(name, *wr.text);
            flags_of[name] = wr.flags;
        }
    }
    for (const std::vector<std::string>& parts : functions_said) {
        const Va va = *text::parse_int<Va>(parts[1], 16);
        const std::string& name = parts[2];
        if (by_va.contains(va) || texts.contains(name) || !d.where_map.contains(va)) continue;
        const std::string model = (parts.size() > 4 && parts[4] != "inline") ? parts[4] : "__cdecl16near";
        const SegOff place = d.where_map.at(va);
        std::optional<ByteView> code;
        if (const auto it = images.find(place.seg); it != images.end()) code = ByteView(it->second);
        const ModuleFacts facts = module_facts16(code, place.off, ends.contains(va) ? ends[va] : place.off, std::nullopt, std::nullopt, dgroup, false, place.seg);
        const bool far = internal::has(model, "__stdcall16far") || internal::has(model, "__cdecl16far");
        std::vector<std::string> words{model};
        if (far && facts.exported) words.push_back("__export");
        if (facts.loadds) words.push_back("__loadds");
        texts.insert_or_assign(name, "xunknown2 " + text::join(words, " ") + " " + name + "()\n{\n  /* the decompiler could not make this function */\n  return 0;\n}");
        const std::string head = "xunknown2 " + text::join(words, " ") + " " + name;   // (its heading, as the back end reports one)
        ModuleFacts found;
        Heading h;
        h.params = Heading::Params{head, {}};
        h.declaration = head + "();";
        h.fastcall = internal::has(head, "__fastcall16");
        found.heading = h;
        found_of[name] = found;
        std::set<std::string> flags;
        if (far && facts.exported) flags.insert("exported_prologue");
        if (!far) flags.insert("near");
        flags_of[name] = flags;
        by_va[va] = name;
        m.failed.try_emplace(name, "the decompiler could not make it");
    }
    // helpers that work in their caller's frame: its places passed to them -- what the module decides from what writing each function
    // found (backends/c.caller_frame_plan), and the functions it names written again with it
    Json frames = Json::object();
    for (const auto& [va, name] : by_va) {
        bool keeps = false;
        if (const auto place = d.where_map.find(va); place != d.where_map.end()) {
            const auto image = d.images.find(place->second.seg);
            keeps = image != d.images.end() && w16_keeps_frame(image->second, place->second.off);
        }
        frames[name] = keeps;
    }
    Json reports = Json::object();
    for (const auto& [name, text] : texts) {
        const auto f = found_of.find(name);
        const std::string* frame = f == found_of.end() ? nullptr : f->second.plans.find("frame");
        reports[name] = frame != nullptr ? json::parse(*frame) : Json(nullptr);
    }
    Json plan = library.caller_frame_plan(reports, frames);
    if (!plan.is_object()) plan = Json::object();
    auto plan_update = [&](const std::string& name, const Json& extra) {
        Json& current = plan[name];
        if (!current.is_object()) current = Json::object();
        for (const auto& [key, value] : extra.items()) current[key] = value;
    };
    // ... and the module's own segments as values, where the function's code holds a loader fix-up for one (backends/c._segment_values:
    // which function is far is known once each is written)
    {
        Json function_list = Json::array(), near_names = Json::array(), where_pairs = Json::array(), site_pairs = Json::array();
        for (const auto& [va, name] : by_va) function_list.push_back(w16_tuple({va, name}));
        std::set<std::string> near_set;
        for (const auto& [name, flags] : flags_of)
            if (flags.contains("near")) near_set.insert(name);
        for (const std::string& n : near_set) near_names.push_back(n);
        for (const auto& [va, place] : d.where_map) where_pairs.push_back(Json::array({va, w16_tuple({place.seg, place.off})}));
        for (const auto& [seg, list] : internal::selector_sites(module)) {
            Json l = Json::array();
            for (const auto& [off, target] : list) l.push_back(w16_tuple({off, target}));
            site_pairs.push_back(Json::array({seg, l}));
        }
        std::vector<std::int64_t> code_segments;
        for (const auto& [seg, length] : d.code) code_segments.push_back(seg);
        std::vector<Json> other;
        for (const auto& [seg, bytes] : d.images)
            if (!d.code.contains(seg) && seg != d.data_segment) other.push_back(seg);
        const Json extras = library.segment_plan(function_list, Json{{"$s", near_names}}, Json{{"$d", where_pairs}}, Json{{"$d", site_pairs}}, code_segments,
                                                 d.data_segment, w16_set(other), describe::kFirstSelector);
        for (const auto& [name, extra] : extras.items()) plan_update(name, extra);
    }
    // ... and the data a function reads through CS from beside its code: by name, in its own code segment (backends/c._code_segment_reads
    // finds the numbers, code_segment_data lays it out)
    internal::CodeData cd;
    if (policy.code_data) {
        std::vector<std::pair<std::string, std::optional<std::string>>> read_reports;
        std::map<std::string, std::optional<SegOff>> where16;
        for (const auto& [va, name] : by_va) {
            const auto f = found_of.find(name);
            const std::string* reads = f == found_of.end() ? nullptr : f->second.plans.find("code_reads");
            read_reports.emplace_back(name, reads != nullptr ? std::optional<std::string>(*reads) : std::nullopt);
            const auto place = d.where_map.find(va);
            where16[name] = place == d.where_map.end() ? std::nullopt : std::optional<SegOff>(place->second);
        }
        cd = internal::code_segment_data(read_reports, where16, d, module);
    }
    for (const auto& [name, extra] : cd.plan) plan_update(name, json::parse(extra));
    // ... and the structures of global selectors: one for the module where every function's fields agree, else each function's own --
    // its uses then written by that name (backends/c._selector_structs, given "selector_renames")
    std::map<std::string, std::map<std::string, std::map<std::int64_t, std::string>>> shared_selectors;
    for (const auto& [va, name] : by_va) {
        const auto f = found_of.find(name);
        if (f == found_of.end()) continue;
        for (const auto& [structure, per] : f->second.selector_shared)
            for (const auto& [function, fields] : per) shared_selectors[structure][function] = fields;   // (.update)
    }
    const SharedSelectorStructs shared = shared_selector_structs(library, shared_selectors);
    for (const auto& [function, names] : shared.renames) {
        Json renames = Json::object();
        for (const auto& [name, own] : names) renames[name] = own;
        plan_update(function, Json{{"selector_renames", renames}});
    }
    std::map<std::string, const Item16*> item_of;
    for (const Item16& item : items)
        if (const auto it = by_va.find(item.va); it != by_va.end()) item_of[it->second] = &item;
    for (const auto& [name, extra] : plan.items()) {
        const auto it = item_of.find(name);
        if (it == item_of.end()) continue;
        Item16 again = *it->second;
        again.extra.clear();
        for (const auto& [key, value] : extra.items()) again.extra.insert_or_assign(key, value.dump());   // dict(shared, **extra)
        if (const auto notes = d.notes.find(name); notes != d.notes.end() && again.known) again.known->l2 = notes->second.l2;
        Written16 wr = w16_write(services, images, asm_images, again);
        if (!wr.text) continue;
        texts.insert_or_assign(name, *wr.text);
        found_of[name] = wr.found;
        d.notes[name] = wr.known;
    }
    const std::vector<Va> final_vas = [&] {
        std::vector<Va> v;
        for (const auto& [va, name] : by_va) v.push_back(va);
        return v;
    }();
    for (const Va va : final_vas) {
        const std::string& name = by_va.at(va);
        const std::string body(text::trim(texts.at(name)));
        const auto f = found_of.find(name);
        const ModuleFacts found = f != found_of.end() ? f->second : ModuleFacts();
        const FunctionNotes known = d.notes.contains(name) ? d.notes.at(name) : FunctionNotes();
        struct_notes.insert(struct_notes.end(), known.structs.begin(), known.structs.end());
        for (const core16::GlobalNote& g : known.globals) {
            const auto have = sizes.find(g.address);
            sizes[g.address] = g.bytes <= 4 ? std::max<std::int64_t>(have == sizes.end() ? 0 : have->second, g.bytes) : (have == sizes.end() ? 2 : have->second);
            if (g.elem == 1 || g.elem == 2 || g.elem == 4) elems[g.address] = g.elem;
        }
        if (const auto fl = flags_of.find(name); fl != flags_of.end()) {
            for (const std::string& flag : fl->second) {
                if (flag == "near") m.near.insert(name);
                else if (flag == "wide") m.wide.insert(name);
                else if (flag == "novalue") m.novalue.insert(name);
                else if (flag == "exported_prologue") m.exported_prologue.insert(name);
                else throw InternalError("the C back end reports a flag the module has no place for: " + flag);
            }
        }
        selector_structs.insert(selector_structs.end(), found.selector_decls.begin(), found.selector_decls.end());
        m.made.add(name, body, static_cast<std::uint64_t>(va));
        // (an import's value used bare is the import: backends/c._selector_imports)
        for (const auto& [import, how] : found.selector_imports) m.selector_imports[import] = how;
    }
    std::set<std::int64_t> other_data;
    for (const auto& [seg, bytes] : d.images)
        if (!d.code.contains(seg) && seg != d.data_segment) other_data.insert(seg);
    m.code_data = cd.defs;
    m.code_reads = cd.report;
    // what the functions name and call, as each was written (backends/c._names_used)
    std::vector<NamesUsed> used;
    for (const std::string& n : m.made.order()) {
        const auto f = found_of.find(n);
        used.push_back(f != found_of.end() && f->second.names ? *f->second.names : NamesUsed());
    }
    std::set<std::string> called;
    for (const NamesUsed& u : used) called.insert(u.called.begin(), u.called.end());
    const OrderedMap<std::string, std::string> made_texts = m.made.texts();
    for (const std::string& name : d.imports) {
        if (!called.contains(name) || made_texts.contains(name)) continue;
        if (const auto split = internal::split_import_symbol(name)) m.imports[name] = *split;
    }
    // Globals the decompiler names by address. In the data segment: an offset through DS, as in the original. In another segment (constants
    // kept beside the code, a second data segment): the module carries that segment's bytes, and the global is a place in them.
    std::vector<std::string> macros;
    for (const std::int64_t seg : other_data) {   // (a segment's selector used as a value)
        if (std::any_of(used.begin(), used.end(), [&](const NamesUsed& u) { return u.segments.contains(seg); })) m.segments[seg] = d.images.at(seg);
    }
    // a global the code calls through holds a function pointer -- near or far by its size
    std::set<std::string> called_through;
    std::set<std::pair<std::string, std::string>> globals_used;
    for (const NamesUsed& u : used) {
        called_through.insert(u.called_through.begin(), u.called_through.end());
        globals_used.insert(u.globals.begin(), u.globals.end());
    }
    const OrderedMap<std::uint64_t, std::string> made_by_va = m.made.by_va();
    for (const auto& [prefix, addr] : globals_used) {
        const Va va = *text::parse_int<Va>(addr, 16);
        const SegOff place = describe::where(va);
        const std::int64_t size = sizes.contains(va) ? sizes[va] : 2;
        std::string ty = internal::global_type16(prefix, size, elems.contains(va) ? std::optional<std::int64_t>(elems[va]) : std::nullopt);
        const std::string key = prefix + "Ram" + addr;
        if (called_through.contains(key)) ty = size == 4 ? "code __far *" : "code __near *";
        const auto selector_import = d.selector_imports.find(va >> 16);
        if (place.seg == static_cast<std::uint32_t>(d.data_segment) || (va >> 16) == 0) {
            // (segment 0: an address the core folded to a constant -- a table index after it copied a shared tail, LZEXPAND's sub_11_00AB
            // `cRam00000095` -- an offset through DS like the rest; it was declared nowhere and the C did not compile)
            macros.push_back("#define " + key + " (*(" + ty + " *)0x" + w16_hex((va >> 16) == 0 ? (va & 0xFFFF) : place.off) + ")");
        } else if ((va >> 16) == 0x40) {
            // through the selector 0040h itself: the BIOS's data, which Windows keeps at that selector
            macros.push_back("#define " + key + " (*(" + ty + " __far *)MK_FP(0x40, 0x" + w16_hex(va & 0xFFFF) + "))");
            m.bios_data = true;
        } else if (selector_import != d.selector_imports.end()) {
            // a place in a segment the module gets as an import (KERNEL's __0040H: the BIOS data): the import's selector, which the loader
            // fills in, and the offset
            const describe::SelectorImport& import = selector_import->second;
            const bool segment = import.use == describe::SelectorUse::Segment;
            m.selector_imports[import.symbol] = segment ? "segment" : "offset";
            macros.push_back("#define " + key + " (*(" + ty + " __far *)MK_FP(" + (segment ? "FP_SEG" : "FP_OFF") + "(&" + import.symbol + "), 0x" + w16_hex(va & 0xFFFF) + "))");
        } else {
            bool far_in_seg = false;
            std::optional<std::string> far_name;
            if (d.code.contains(place.seg)) {
                for (const auto& [v, n] : made_by_va) {
                    const auto where = m.where.find(static_cast<Va>(v));
                    if (where != m.where.end() && where->second.seg == place.seg && !m.near.contains(n)) {
                        far_in_seg = true;
                        if (!far_name || n < *far_name) far_name = n;
                    }
                }
            }
            const auto image = d.images.find(place.seg);
            if (far_in_seg) {
                // in one of the module's code segments (a variable kept beside the code): that segment itself, by the selector of a far
                // function in it -- not a copy of its bytes, which the code there would never see written
                macros.push_back("#define " + key + " (*(" + ty + " __far *)(((uint4)(uint2)((uint4)(void __far *)&" + *far_name + " >> 16) << 16) | 0x" +
                                 w16_hex(place.off) + "))");
            } else if (image != d.images.end() && place.off < image->second.size()) {
                m.segments[place.seg] = image->second;
                macros.push_back("#define " + key + " (*(" + ty + " __far *)(codebreaker_segment_" + std::to_string(place.seg) + " + 0x" + w16_hex(place.off) + "))");
            }
        }
    }
    // (data.c, compiled with CODEBREAKER_DATA, defines these segments with their pointers in place: the declaration as bytes is for the code)
    std::vector<std::string> decls;
    for (const auto& [n, bytes] : m.segments)
        decls.push_back("#ifndef CODEBREAKER_DATA\nextern uint1 __based(__segname(\"CODEBREAKER_SEGMENT_" + std::to_string(n) + "\")) codebreaker_segment_" +
                        std::to_string(n) + "[];\n#endif");
    if (!m.selector_imports.empty()) {   // imports used for their value
        decls.push_back("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))");
        decls.push_back("#define FP_SEG(p) ((uint2)((uint4)(void __far *)(p) >> 16))");
        decls.push_back("#define FP_OFF(p) ((uint2)(uint4)(void __far *)(p))");
        for (const auto& [n, how] : m.selector_imports) decls.push_back("extern uint1 __far " + n + ";  /* an import: its " + how + " is the value */");
        for (const auto& [n, how] : m.selector_imports)
            if (const auto split = internal::split_import_symbol(n)) m.imports[n] = *split;
    }
    for (const auto& [nm, import] : m.imports)
        if (!m.selector_imports.contains(nm)) decls.push_back("extern uint4 __far __stdcall " + nm + "();");
    // each function declared: with its parameters where every call agrees with its definition (backends/c.prototypes, from each one's
    // heading and calls as the back end wrote them)
    const std::vector<std::string> order_now = m.made.order();
    Json headings_json = Json::object();
    std::map<std::string, const Heading*> headings;
    for (const std::string& n : order_now) {
        const auto f = found_of.find(n);
        if (f != found_of.end() && f->second.heading) {
            headings[n] = &*f->second.heading;
            headings_json[n] = w16_heading_json(*f->second.heading);
        } else {
            headings_json[n] = Json::object();
        }
    }
    OrderedMap<std::string, std::string> agreed;
    if (policy.prototypes) agreed = library.prototypes(headings_json, order_now);
    for (const auto& [name, declaration] : agreed) m.prototyped.insert(name);
    // (each function's declaration with its argument list left open, from its heading as the back end wrote it from L2: the module's
    // public header declares its exports with them -- build/module)
    for (const std::string& n : order_now) {
        CB_ASSERT(headings.contains(n), "a function of the module has no heading from the back end: " + n);
        m.declarations[n] = headings.at(n)->declaration;
    }
    for (const std::string& n : order_now) {
        const std::string* a = agreed.find(n);
        decls.push_back(a != nullptr && !a->empty() ? *a : headings.at(n)->declaration);
    }
    for (const std::string& n : order_now)
        if (headings.at(n)->fastcall) decls.push_back("#ifdef __WATCOMC__\n#pragma aux (codebreaker_fastcall) " + n + ";\n#endif");
    std::vector<std::string> intrinsics;
    std::vector<std::string> intrinsic_names;
    for (const auto& [name, c] : intrinsics16()) intrinsic_names.push_back(name);
    std::sort(intrinsic_names.begin(), intrinsic_names.end());
    for (const std::string& n : intrinsic_names)
        if (called.contains(n)) intrinsics.push_back(intrinsics16().at(n));
    for (const std::string& h : interrupt_helpers(m.interrupts)) intrinsics.push_back(h);
    const bool any_mk_fp = std::any_of(used.begin(), used.end(), [](const NamesUsed& u) { return u.mk_fp; });
    if ((any_mk_fp || m.bios_data) && m.selector_imports.empty())
        intrinsics.push_back("#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))");
    selector_structs.insert(selector_structs.end(), shared.declarations.begin(), shared.declarations.end());
    if (!selector_structs.empty()) intrinsics.push_back(std::string(seg_ptr()));
    std::vector<std::string> all = intrinsics;
    all.insert(all.end(), macros.begin(), macros.end());
    for (const std::string& x : internal::struct_decls(struct_notes, header16(), true)) all.push_back(x);
    all.insert(all.end(), selector_structs.begin(), selector_structs.end());
    all.insert(all.end(), cd.decls.begin(), cd.decls.end());
    all.insert(all.end(), decls.begin(), decls.end());
    m.made.set_header(std::string(header16()) + "\n" + text::join(all, "\n") + "\n");
    return m;
}

}  // namespace cb::decompile::c_text
