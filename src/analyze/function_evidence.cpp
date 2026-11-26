// analyze/function_evidence.cpp -- see function_evidence.hpp.
#include "analyze/function_evidence.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "analyze/call_graph.hpp"
#include "analyze/report_internal.hpp"
#include "analyze/strings.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/pe.hpp"
#include "formats/resources.hpp"

namespace cb::analyze::function_evidence {

namespace {

namespace cg = call_graph;

// Does the instruction go through an import's slot (`jmp [slot]`, `call [slot]`), and which?
std::optional<Va> import_slot(const Image& image, const Instruction& ins) {
    if (ins.target && image.is_import(*ins.target)) return ins.target;
    if (!ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Memory && ins.operands[0].base.empty() && ins.operands[0].index.empty() &&
        image.is_import(ins.operands[0].value))
        return ins.operands[0].value;
    return std::nullopt;
}

bool frame_register(const std::string& reg) { return reg == "ebp" || reg == "esp" || reg == "bp" || reg == "sp"; }

// An instruction a function that only passes its arguments on may hold besides the call: the frame's, and moves of the arguments.
bool passes_arguments(const Instruction& ins) {
    if (ins.mnemonic == "nop" || ins.mnemonic == "leave") return true;
    if (ins.mnemonic == "push" || ins.mnemonic == "pop") return !ins.operands.empty() && ins.operands[0].kind != Operand::Kind::Immediate;
    if (ins.mnemonic == "mov" && ins.operands.size() == 2 && ins.operands[0].kind == Operand::Kind::Register) {
        const Operand& from = ins.operands[1];
        if (from.kind == Operand::Kind::Register) return true;
        return from.kind == Operand::Kind::Memory && frame_register(from.base) && from.index.empty();
    }
    return false;
}

// An import's name without its module, its stdcall decoration and its A/W ending: "USER32!RegisterClassExA" -> "RegisterClassEx".
std::string api_stem(std::string_view import_name) {
    const std::size_t bang = import_name.rfind('!');
    std::string name(bang == std::string_view::npos ? import_name : import_name.substr(bang + 1));
    if (const std::size_t at = name.rfind('@'); at != std::string::npos && at > 1) {
        name.resize(at);
        if (name.starts_with('_')) name.erase(0, 1);
    }
    return name;
}

// The titles of a 32-bit module's dialog templates, by "#<id>" or by upper-case name; none for a module whose resources cannot be read.
std::map<std::string, std::string> dialog_titles(const std::filesystem::path& module) {
    namespace fr = formats::resources;
    std::map<std::string, std::string> out;
    try {
        const Bytes file = fs::read_file(module);
        const formats::pe::Image pe = formats::pe::Image::parse(ByteView(file), fs::to_text(module));
        std::vector<fr::PeResourceEntry> entries;
        for (const formats::pe::ResourceType& t : pe.resource_tree()) {
            if (t.id != std::optional<std::uint32_t>(fr::kDialog)) continue;
            for (const formats::pe::ResourceItem& item : t.items) {
                fr::PeResourceEntry e;
                e.type = fr::Ident(*t.id);
                e.name = item.id ? fr::Ident(*item.id) : fr::Ident(item.name.value_or(std::string()));
                e.language = item.language;
                e.file_offset = item.file_offset;
                e.size = item.size;
                entries.push_back(std::move(e));
            }
        }
        for (const fr::Resource& r : fr::from_pe_entries(file, entries)) {
            try {
                const std::string title = fr::dialog(ByteView(r.data), true).title;
                if (title.empty()) continue;
                const std::string key = std::holds_alternative<std::uint32_t>(r.name) ? "#" + std::to_string(std::get<std::uint32_t>(r.name))
                                                                                       : text::upper(std::get<std::string>(r.name));
                out.emplace(key, title);   // (the first language)
            } catch (const FormatError&) {
            }
        }
    } catch (const Error&) {
    }
    return out;
}

// Where a module's functions hand their own functions' addresses to imports that keep them to call back (32-bit code): the arguments pushed
// before a call through an import (`call [slot]`, or `call esi` after `mov esi, [slot]`), and a WNDCLASS filled in before `RegisterClass*`.
void find_handed(Image& image, std::vector<symbols::FunctionEvidence>& out, const std::map<std::string, std::string>& titles) {
    std::map<Va, symbols::FunctionEvidence*> by_start;
    for (symbols::FunctionEvidence& e : out) by_start[static_cast<Va>(e.address)] = &e;
    const auto text_at = [&](Va va) -> std::optional<std::string> {
        if (by_start.contains(va)) return std::nullopt;
        const std::optional<strings::Found> s = strings::string_at(image, va, 2);
        if (!s) return std::nullopt;
        return s->text;
    };
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        std::map<std::string, Va> loaded;   // register -> the import slot it was loaded from
        for (std::size_t c = 0; c < fn.insns.size(); ++c) {
            const Instruction& ins = fn.insns[c];
            if (ins.flow != Flow::Call) {
                for (const std::string& w : ins.writes) loaded.erase(w);
                if (ins.mnemonic == "mov" && ins.operands.size() == 2 && ins.operands[0].kind == Operand::Kind::Register && ins.operands[1].kind == Operand::Kind::Memory &&
                    ins.operands[1].base.empty() && ins.operands[1].index.empty() && image.is_import(ins.operands[1].value))
                    loaded[ins.operands[0].reg] = ins.operands[1].value;
                continue;
            }
            std::optional<Va> slot = import_slot(image, ins);
            if (!slot && !ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Register)
                if (const auto at = loaded.find(ins.operands[0].reg); at != loaded.end()) slot = at->second;
            loaded.clear();   // (a call may change any register the convention lets it)
            if (!slot) continue;
            const std::string import_name = image.import_name(*slot);
            // the arguments, nearest the call first: argument 1 is the last pushed
            std::vector<std::optional<Va>> args;
            for (std::size_t j = c; j-- > 0 && c - j <= 24 && args.size() < 8;) {
                const Instruction& p = fn.insns[j];
                if (p.flow != Flow::None) break;
                if (p.mnemonic != "push") continue;
                args.push_back(!p.operands.empty() && p.operands[0].kind == Operand::Kind::Immediate ? std::optional<Va>(p.operands[0].value) : std::nullopt);
            }
            for (std::size_t i = 0; i < args.size(); ++i) {
                if (!args[i]) continue;
                const auto target = by_start.find(*args[i]);
                if (target == by_start.end() || *args[i] == start) continue;
                symbols::HandedEvidence h;
                h.import_name = import_name;
                h.argument = static_cast<int>(i) + 1;
                h.by = static_cast<std::uint64_t>(start);
                for (std::size_t k = 0; k < args.size(); ++k) {
                    if (k == i || !args[k]) continue;
                    if (const std::optional<std::string> t = text_at(*args[k])) h.texts[static_cast<int>(k) + 1] = *t;
                    else if (!by_start.contains(*args[k])) h.numbers[static_cast<int>(k) + 1] = static_cast<std::int32_t>(*args[k]);
                }
                const std::string api = api_stem(import_name);
                if (h.argument == 4 && (api.starts_with("DialogBoxParam") || api.starts_with("CreateDialogParam"))) {
                    std::string key;
                    if (const auto t = h.texts.find(2); t != h.texts.end()) key = text::upper(t->second);
                    else if (const auto n = h.numbers.find(2); n != h.numbers.end() && n->second > 0 && n->second < 0x10000) key = "#" + std::to_string(n->second);
                    if (const auto title = titles.find(key); !key.empty() && title != titles.end()) h.caption = title->second;
                }
                if (std::find(target->second->handed_to.begin(), target->second->handed_to.end(), h) == target->second->handed_to.end())
                    target->second->handed_to.push_back(std::move(h));
            }
            // a window class: `mov [s+4], offset WndProc` ... `mov [s+36], offset "ClassName"` (WNDCLASS; WNDCLASSEX at 8 and 40), then RegisterClass*
            const std::string stem = api_stem(import_name);
            const bool registers = stem == "RegisterClass" || stem == "RegisterClassA" || stem == "RegisterClassW" || stem == "RegisterClassEx" ||
                                   stem == "RegisterClassExA" || stem == "RegisterClassExW";
            if (!registers) continue;
            std::map<std::pair<std::string, Va>, Va> stores;   // (base, displacement) -> the immediate stored there
            for (std::size_t j = c; j-- > 0 && c - j <= 64;) {
                const Instruction& m = fn.insns[j];
                if (m.flow == Flow::Call) break;
                if (m.mnemonic == "mov" && m.operands.size() == 2 && m.operands[0].kind == Operand::Kind::Memory && m.operands[0].index.empty() &&
                    m.operands[1].kind == Operand::Kind::Immediate)
                    stores.emplace(std::pair{m.operands[0].base, m.operands[0].value}, m.operands[1].value);
            }
            for (const auto& [where, value] : stores) {
                const auto target = by_start.find(value);
                if (target == by_start.end()) continue;
                symbols::HandedEvidence h;
                h.import_name = import_name;
                h.field = "lpfnWndProc";
                h.by = static_cast<std::uint64_t>(start);
                if (const auto name = stores.find({where.first, where.second + 32}); name != stores.end())
                    if (const std::optional<std::string> t = text_at(name->second)) h.texts[0] = *t;
                if (std::find(target->second->handed_to.begin(), target->second->handed_to.end(), h) == target->second->handed_to.end())
                    target->second->handed_to.push_back(std::move(h));
            }
        }
    }
}

}  // namespace

std::vector<symbols::FunctionEvidence> of_module(const std::filesystem::path& module, const report::Environment& environment) {
    report::detail::Loaded loaded = report::detail::load_module(module, environment);
    Image& image = *loaded.image;
    const cg::Graph graph = cg::build(image);
    strings::Table table = strings::build_table(image, graph);
    const std::vector<strings::FunctionUses> uses = strings::find_uses(image, graph, table);
    std::map<Va, const strings::FunctionUses*> uses_of;
    for (const strings::FunctionUses& u : uses) uses_of[u.function] = &u;

    std::vector<symbols::FunctionEvidence> out;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        symbols::FunctionEvidence e;
        e.address = static_cast<std::uint64_t>(start);
        e.instructions = static_cast<int>(fn.insns.size());
        e.bytes = static_cast<int>(std::max<Va>(0, fn.end - fn.start));
        for (std::size_t i = 0; i < fn.insns.size() && i < static_cast<std::size_t>(symbols::kListingMost); ++i) e.listing.push_back(fn.insns[i].text);

        // calls: each distinct target once, in the order reached
        if (const auto calls = graph.calls.find(start); calls != graph.calls.end()) {
            for (const cg::Call& c : calls->second) {
                symbols::CallEvidence call;
                if (!c.destination) {
                    call.kind = symbols::CallEvidence::Kind::Indirect;
                } else if (image.is_import(*c.destination)) {
                    call.kind = symbols::CallEvidence::Kind::Import;
                    call.import_name = image.import_name(*c.destination);
                } else {
                    call.kind = symbols::CallEvidence::Kind::Function;
                    call.target = static_cast<std::uint64_t>(*c.destination);
                }
                const auto same = std::find_if(e.calls.begin(), e.calls.end(), [&](const symbols::CallEvidence& x) {
                    return x.kind == call.kind && x.import_name == call.import_name && x.target == call.target;
                });
                if (same != e.calls.end() && call.kind != symbols::CallEvidence::Kind::Indirect) ++same->sites;
                else e.calls.push_back(std::move(call));
            }
        }
        if (const auto callers = graph.callers.find(start); callers != graph.callers.end())
            for (const Va caller : callers->second) e.callers.push_back(static_cast<std::uint64_t>(caller));

        // strings, each once, in order of use
        if (const auto u = uses_of.find(start); u != uses_of.end()) {
            std::set<std::string> seen;
            for (const strings::Use& use : u->second->uses)
                if (seen.insert(use.text).second) e.strings.push_back(use.text);
        }

        // the shape: only passes its arguments on to one import
        int branches = 0, returns = 0, import_calls = 0;
        bool only_forwarding = !fn.insns.empty() && fn.tables.empty();
        bool touches_arguments = false;
        for (const Instruction& ins : fn.insns) {
            if (ins.flow == Flow::Call || ins.flow == Flow::Jump || ins.flow == Flow::CondJump) {
                ++branches;
                if (ins.flow != Flow::CondJump && import_slot(image, ins)) ++import_calls;
                else only_forwarding = false;
            } else if (ins.flow == Flow::Return) {
                ++returns;
            } else if (passes_arguments(ins)) {
                if (ins.mnemonic != "nop" && !(ins.mnemonic == "push" && ins.operands[0].kind == Operand::Kind::Register && frame_register(ins.operands[0].reg)) &&
                    !(ins.mnemonic == "pop" && ins.operands[0].kind == Operand::Kind::Register && frame_register(ins.operands[0].reg)) && ins.mnemonic != "leave" &&
                    !(ins.mnemonic == "mov" && ins.operands[0].kind == Operand::Kind::Register && ins.operands[1].kind == Operand::Kind::Register &&
                      frame_register(ins.operands[0].reg) && frame_register(ins.operands[1].reg)))
                    touches_arguments = true;
            } else {
                only_forwarding = false;
            }
        }
        if (only_forwarding && branches == 1 && import_calls == 1 && returns <= 1) {
            e.forwards_to_import = true;
            e.forwards_purely = !touches_arguments;
        }
        // the shape: only passes its arguments on to one other function of the module
        {
            int jumps = 0, rets = 0;
            std::optional<Va> destination;
            bool only = !fn.insns.empty() && fn.insns.size() <= 12 && fn.tables.empty();
            for (const Instruction& ins : fn.insns) {
                if (ins.flow == Flow::Call || ins.flow == Flow::Jump) {
                    ++jumps;
                    if (ins.target && !import_slot(image, ins) && *ins.target != start) destination = *ins.target;
                    else only = false;
                } else if (ins.flow == Flow::Return) {
                    ++rets;
                } else if (ins.flow == Flow::CondJump || !passes_arguments(ins)) {
                    only = false;
                }
            }
            if (only && jumps == 1 && rets <= 1 && destination) e.forwards_to_function = static_cast<std::uint64_t>(*destination);
        }
        out.push_back(std::move(e));
    }
    if (image.bits() == 32) find_handed(image, out, loaded.kind == ImageKind::Pe ? dialog_titles(module) : std::map<std::string, std::string>());
    return out;
}

}  // namespace cb::analyze::function_evidence
