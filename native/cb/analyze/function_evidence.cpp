// cb/analyze/function_evidence.cpp -- see function_evidence.hpp.
#include "cb/analyze/function_evidence.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/report_internal.hpp"
#include "cb/analyze/strings.hpp"

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
    return out;
}

}  // namespace cb::analyze::function_evidence
