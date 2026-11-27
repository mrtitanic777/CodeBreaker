// dotnet/decompiler/il/control_flow/switch_detection.cxx -- see switch_detection.hpp.
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <functional>
#include <optional>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/tree_traversal.hpp"

#if __has_include("dotnet/decompiler/il/transforms/high_level_loop_transform.hpp")
#include "dotnet/decompiler/il/transforms/high_level_loop_transform.hpp"
#define CB_HAVE_HIGH_LEVEL_LOOP_TRANSFORM
#endif
#if __has_include("dotnet/decompiler/il/transforms/switch_on_string_transform.hpp")
#include "dotnet/decompiler/il/transforms/switch_on_string_transform.hpp"
#define CB_HAVE_SWITCH_ON_STRING_TRANSFORM
#endif
#if __has_include("dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp")
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#define CB_HAVE_NULLABLE_LIFTING_TRANSFORM
#endif

namespace cb::dotnet::decompiler::il::control_flow {

using util::LongSet;

// ---- LoopContext ---------------------------------------------------------------------------------------------------------------------

SwitchDetection::LoopContext::LoopContext(ControlFlowGraph& cfg, ControlFlowNode* context_node) {
    std::vector<ControlFlowNode*> loop_heads;

    std::function<void(ControlFlowNode*)> analyze = [&](ControlFlowNode* n) {
        if (n->visited) return;

        n->visited = true;
        if (n->dominates(context_node))
            loop_heads.push_back(n);
        else
            for (ControlFlowNode* successor : n->successors) analyze(successor);
    };
    for (ControlFlowNode* successor : context_node->successors) analyze(successor);
    for (ControlFlowNode* n : cfg.nodes()) n->visited = false;

    int l = 1;
    std::stable_sort(loop_heads.begin(), loop_heads.end(),
                     [](ControlFlowNode* a, ControlFlowNode* b) { return a->post_order_number < b->post_order_number; });
    for (ControlFlowNode* loop_head : loop_heads) {
        ControlFlowNode* continue_node = find_continue(loop_head);
        continue_depth_[continue_node] = l++;
    }
}

ControlFlowNode* SwitchDetection::LoopContext::find_continue(ControlFlowNode* loop_head) {
    // potential continue target (the only predecessor, other than the head itself, that the head dominates)
    ControlFlowNode* pred = nullptr;
    int matches = 0;
    for (ControlFlowNode* p : loop_head->predecessors) {
        if (p != loop_head && loop_head->dominates(p)) {
            pred = p;
            matches++;
        }
    }
    if (matches != 1) pred = nullptr;
    if (pred == nullptr) return loop_head;

#ifdef CB_HAVE_HIGH_LEVEL_LOOP_TRANSFORM
    // match for loop increment block
    if (pred->successors.size() == 1) {
        Block* target = nullptr;
        if (transforms::HighLevelLoopTransform::match_increment_block(pred->user_data, &target) && target == loop_head->user_data) return pred;
    }

    // match do-while condition
    if (pred->successors.size() <= 2) {
        Block* t1 = nullptr;
        Block* t2 = nullptr;
        if (transforms::HighLevelLoopTransform::match_do_while_condition_block(pred->user_data, &t1, &t2) &&
            (t1 == loop_head->user_data || t2 == loop_head->user_data))
            return pred;
    }
#else
    // WAITS: HighLevelLoopTransform (match_increment_block, match_do_while_condition_block)
#endif

    return loop_head;
}

bool SwitchDetection::LoopContext::match_continue(ControlFlowNode* node) const {
    int depth;
    return match_continue(node, &depth);
}

bool SwitchDetection::LoopContext::match_continue(ControlFlowNode* node, int depth) const {
    int found_depth;
    return match_continue(node, &found_depth) && depth == found_depth;
}

bool SwitchDetection::LoopContext::match_continue(ControlFlowNode* node, int* depth) const {
    auto it = continue_depth_.find(node);
    if (it == continue_depth_.end()) {
        *depth = 0;
        return false;
    }
    *depth = it->second;
    return true;
}

int SwitchDetection::LoopContext::get_continue_depth(ControlFlowNode* node) const {
    int depth;
    return match_continue(node, &depth) ? depth : 0;
}

std::vector<ControlFlowNode*> SwitchDetection::LoopContext::get_break_targets(ControlFlowNode* dominator) const {
    std::vector<ControlFlowNode*> result;
    util::pre_order(
        dominator,
        [this](ControlFlowNode* n) {
            std::vector<ControlFlowNode*> children;
            for (ControlFlowNode* c : n->dominator_tree_children)
                if (!match_continue(c)) children.push_back(c);
            return children;
        },
        [&](ControlFlowNode* n) {
            for (ControlFlowNode* successor : n->successors)
                if (!dominator->dominates(successor) && !match_continue(successor, 1)) result.push_back(successor);
        });
    return result;
}

// ---- SwitchDetection -----------------------------------------------------------------------------------------------------------------

void SwitchDetection::run(ILFunction* function, transforms::ILTransformContext& context) {
    if (!context.settings().sparse_integer_switch) return;
    context_ = &context;

    analysis_.allow_unreachable_cases = context.settings().remove_dead_code;

    for (ILInstruction* descendant : function->descendants()) {
        auto* container = as<BlockContainer>(descendant);
        if (container == nullptr) continue;
        current_container_ = container;
        control_flow_graph_.reset();

        bool block_container_needs_cleanup = false;
        for (Block* block : container->blocks) {
            context.cancellation_token.throw_if_cancellation_requested();
            process_block(block, &block_container_needs_cleanup);
        }
        if (block_container_needs_cleanup) {
            // if the original code has an unreachable switch-like condition
            // eg. if (i >= 0) { ... } else if (i == 2) { unreachable }
            // then the 'i == 2' block head gets consumed and the unreachable code needs deleting
            if (context.settings().remove_dead_code)
                container->sort_blocks(true);
            else
                container->blocks.remove_all([](Block* b) { return b->instructions.count() == 0; });
        }
    }
}

void SwitchDetection::process_block(Block* block, bool* block_container_needs_cleanup) {
    bool analysis_success = analysis_.analyze_block(block);
    if (analysis_success && use_csharp_switch()) {
        // complex multi-block switch that can be combined into a single SwitchInstruction
        ILInstruction* switch_value = make<LdLoc>(analysis_.switch_variable());
        if (!(switch_value->result_type() == StackType::I4 || switch_value->result_type() == StackType::I8)) {
            // switchValue must have a result type of either I4 or I8
            switch_value = make<Conv>(switch_value, PrimitiveType::I8, false, Sign::Signed);
        }
        auto* sw = make<SwitchInstruction>(switch_value);
        for (const auto& section : analysis_.sections) {
            auto* switch_section = make<SwitchSection>();
            switch_section->set_labels(section.first);
            switch_section->set_body(section.second);
            sw->sections.add(switch_section);
        }
        if (is<SwitchInstruction>(block->instructions.last())) {
            // we'll replace the switch
        } else {
            assert(is<IfInstruction>(block->instructions.second_to_last_or_default()));
            // Remove branch/leave after if; it's getting moved into a section.
            block->instructions.remove_at(block->instructions.count() - 1);
        }
        sw->add_il_range(*block->instructions[block->instructions.count() - 1]);
        block->instructions.set(block->instructions.count() - 1, sw);

        // mark all inner blocks that were converted to the switch statement for deletion
        for (Block* inner_block : analysis_.inner_blocks) {
            assert(inner_block->parent() == block->parent());
            inner_block->instructions.clear();
        }

        control_flow_graph_.reset();  // control flow graph is no-longer valid
        *block_container_needs_cleanup = true;
        sort_switch_sections(sw, *context_);
    } else {
        // 2nd pass of SimplifySwitchInstruction (after duplicating return blocks),
        // (1st pass was in ControlFlowSimplification)
        simplify_switch_instruction(block, *context_);
    }

    inline_switch_expression_default_case_throw_helper(block, *context_);
}

// call ThrowInvalidOperationException(...)
// leave IL_0000 (ldloc temp)
//
// -or-
//
// call ThrowSwitchExpressionException(...)
// leave IL_0000 (ldloc temp)
//
// -to-
//
// throw(newobj SwitchExpressionException(...))
void SwitchDetection::inline_switch_expression_default_case_throw_helper(Block* block, transforms::ILTransformContext& context) {
    // due to our use of basic blocks at this point, switch instructions can only appear as last instruction
    auto* sw = as<SwitchInstruction>(block->instructions.last_or_default());
    if (sw == nullptr) return;

    auto find_constructor = [&](const char* full_type_name, bool takes_object) -> const IMethod* {
        const IType* exception_type = typesystem::find_type(context.type_system(), typesystem::FullTypeName(full_type_name));
        std::vector<const IType*> types;
        if (takes_object) types.push_back(context.type_system().find_type(KnownTypeCode::Object));
        const std::size_t argument_count = types.size();

        for (const IMethod* ctor : exception_type->get_constructors([&](const IMethod* m) { return !m->is_static() && m->parameters().size() == argument_count; })) {
            bool found = true;
            for (std::size_t i = 0; i < argument_count; i++) {
                if (!typesystem::NormalizeTypeVisitor::ignore_nullability().equivalent_types(ctor->parameters()[i]->type(), types[i])) {
                    found = false;
                    break;
                }
            }
            if (found) return ctor;
        }

        return nullptr;
    };

    const IMethod* exception_ctor_table[2];
    exception_ctor_table[0] = find_constructor("System.InvalidOperationException", false);
    exception_ctor_table[1] = find_constructor("System.Runtime.CompilerServices.SwitchExpressionException", true);

    if (exception_ctor_table[0] == nullptr && exception_ctor_table[1] == nullptr) return;

    SwitchSection* default_section = sw->get_default_section();
    if (default_section == nullptr) return;
    auto* default_branch = as<Branch>(default_section->body());
    if (default_branch == nullptr) return;
    Block* default_block = default_branch->target_block();
    if (default_block == nullptr) return;

    auto match_throw_helper_call = [&](ILInstruction* inst, const IMethod** exception_ctor, ILInstruction** value) {
        *exception_ctor = nullptr;
        if (!match_switch_expression_throw_helper_call(inst, value)) return false;
        *exception_ctor = *value == nullptr ? exception_ctor_table[0] : exception_ctor_table[1];
        return *exception_ctor != nullptr;
    };

    auto& instructions = default_block->instructions;
    if (instructions.count() == 2 && (is<Branch>(instructions[1]) || is<Leave>(instructions[1]))) {
        ILInstruction* call = instructions[0];
        const IMethod* exception_ctor = nullptr;
        ILInstruction* value = nullptr;
        if (!match_throw_helper_call(call, &exception_ctor, &value)) return;
        context.step("SwitchExpressionDefaultCaseTransform", block->instructions[0]);
        auto* new_obj = make<NewObj>(exception_ctor);
        if (value != nullptr) new_obj->arguments().add(value);
        ILInstruction* old0 = instructions[0];
        ILInstruction* old1 = instructions[1];
        instructions.set(0, with_il_range(with_il_range(make<Throw>(new_obj), old0), old1));
        instructions.remove_at(1);
        default_section->set_is_compiler_generated_default_section(true);
    } else if (instructions.count() == 1 && is<Throw>(instructions[0]) && is<NewObj>(static_cast<Throw*>(instructions[0])->argument()) &&
               static_cast<NewObj*>(static_cast<Throw*>(instructions[0])->argument())->arguments().count() == 1 &&
               exception_ctor_table[1] != nullptr &&
               typesystem::member_equals(static_cast<NewObj*>(static_cast<Throw*>(instructions[0])->argument())->method, exception_ctor_table[1])) {
        default_section->set_is_compiler_generated_default_section(true);
    } else {
        return;
    }
}

// Matches a call to one of the compiler-generated throw helpers in <PrivateImplementationDetails> that implement the implicit default
// case of a switch expression. `value` is the switch value passed to ThrowSwitchExpressionException, or null for the parameterless
// ThrowInvalidOperationException helper (emitted when SwitchExpressionException is not available on the target framework).
bool SwitchDetection::match_switch_expression_throw_helper_call(ILInstruction* inst, ILInstruction** value) {
    *value = nullptr;
    auto* call = as<Call>(inst);
    if (call == nullptr) return false;
    if (call->method->declaring_type()->full_name() != "<PrivateImplementationDetails>") return false;
    switch (call->arguments().count()) {
        case 0:
            return call->method->name() == "ThrowInvalidOperationException";
        case 1:
            if (call->method->name() != "ThrowSwitchExpressionException") return false;
            *value = call->arguments()[0];
            return true;
        default:
            return false;
    }
}

bool SwitchDetection::is_switch_expression_throw_helper_block(Block* block) {
    auto& instructions = block->instructions;
    if (instructions.count() == 2 && (is<Branch>(instructions[1]) || is<Leave>(instructions[1]))) {
        ILInstruction* ignored = nullptr;
        return match_switch_expression_throw_helper_call(instructions[0], &ignored);
    }
    if (instructions.count() == 1) {
        auto* throw_inst = as<Throw>(instructions[0]);
        auto* new_obj = throw_inst != nullptr ? as<NewObj>(throw_inst->argument()) : nullptr;
        if (new_obj != nullptr) return new_obj->method->declaring_type()->full_name() == "System.Runtime.CompilerServices.SwitchExpressionException";
    }
    return false;
}

void SwitchDetection::simplify_switch_instruction(Block* block, transforms::ILTransformContext& context) {
    // due to our use of basic blocks at this point, switch instructions can only appear as last instruction
    auto* sw = as<SwitchInstruction>(block->instructions.last_or_default());
    if (sw == nullptr) return;

    // ControlFlowSimplification runs early (before any other control flow transforms).
    // Any switch instructions will only have branch instructions in the sections.

    // Combine sections with identical branch target:
    std::unordered_map<Block*, SwitchSection*> dict;  // branch target -> switch section
    sw->sections.remove_all([&](SwitchSection* section) {
        Block* target = nullptr;
        if (section->body()->match_branch(&target)) {
            auto it = dict.find(target);
            if (it != dict.end()) {
                SwitchSection* primary_section = it->second;
                primary_section->set_labels(primary_section->labels().union_with(section->labels()));
                primary_section->set_has_null_label(primary_section->has_null_label() || section->has_null_label());
                return true;  // remove this section
            } else {
                dict.emplace(target, section);
            }
        }
        return false;
    });
    adjust_labels(sw, context);
    sort_switch_sections(sw, context);
}

namespace {

// The first of a set's values (upstream's Values.FirstOrDefault()), without listing the values.
std::int64_t first_value(const LongSet& labels) { return labels.intervals().empty() ? 0 : labels.intervals().front().start; }

}  // namespace

void SwitchDetection::sort_switch_sections(SwitchInstruction* sw, transforms::ILTransformContext& context) {
    std::vector<SwitchSection*> sorted(sw->sections.begin(), sw->sections.end());
    if (context.settings().sort_switch_sections) {
        std::stable_sort(sorted.begin(), sorted.end(),
                         [](SwitchSection* a, SwitchSection* b) { return first_value(a->labels()) < first_value(b->labels()); });
    } else {
        auto key = [](SwitchSection* s) -> std::optional<int> {
            if (auto* b = as<Branch>(s->body())) return b->target_il_offset();
            if (auto* l = as<Leave>(s->body())) return l->start_il_offset();
            return std::nullopt;
        };
        std::stable_sort(sorted.begin(), sorted.end(), [&](SwitchSection* a, SwitchSection* b) {
            std::optional<int> ka = key(a);
            std::optional<int> kb = key(b);
            if (ka != kb) return ka < kb;  // null first
            return first_value(a->labels()) < first_value(b->labels());
        });
    }
    sw->sections.replace_list(sorted);
}

void SwitchDetection::adjust_labels(SwitchInstruction* sw, transforms::ILTransformContext& context) {
    auto* bop = as<BinaryNumericInstruction>(sw->value());
    std::int64_t val = 0;
    if (bop != nullptr && !bop->check_for_overflow && bop->right()->match_ldc_i(&val)) {
        // Move offset into labels:
        context.step("Move offset into switch labels", bop);
        std::int64_t offset;
        switch (bop->op) {
            case BinaryNumericOperator::Add:
                offset = static_cast<std::int64_t>(0 - static_cast<std::uint64_t>(val));
                break;
            case BinaryNumericOperator::Sub:
                offset = val;
                break;
            default:  // unknown bop.Operator
                return;
        }
        sw->set_value(bop->left());
        for (SwitchSection* section : sw->sections) section->set_labels(section->labels().add_offset(offset));
    }
}

namespace {
constexpr std::uint64_t max_values_per_section = 100;
}

// Tests whether we should prefer a switch statement over an if statement.
bool SwitchDetection::use_csharp_switch() {
    if (analysis_.inner_blocks.empty()) return false;
    const std::pair<LongSet, ILInstruction*>* default_section = nullptr;
    for (const auto& s : analysis_.sections) {
        if (s.first.count() > max_values_per_section) {
            default_section = &s;
            break;
        }
    }
    if (default_section == nullptr) {
        // no default section found?
        // This should never happen, as we'd need 2^64/MaxValuesPerSection sections to hit this case...
        return false;
    }
    const LongSet& default_section_key = default_section->first;
    for (const auto& s : analysis_.sections) {
        if (!s.first.set_equals(default_section_key) && s.first.count() > max_values_per_section) {
            // Only the default section is allowed to have tons of keys.
            // C# doesn't support "case 1 to 100000000", and we don't want to generate gigabytes of case labels.
            return false;
        }
    }

    // good enough indicator that the surrounding code also forms a switch statement
    if (analysis_.contains_il_switch() || match_roslyn_switch_on_string()) return true;

    // heuristic to determine if a block would be better represented as an if statement rather than switch
    int if_count = static_cast<int>(analysis_.inner_blocks.size()) + 1;
    int interval_count = 0;
    for (const auto& s : analysis_.sections)
        if (!s.first.set_equals(default_section_key)) interval_count += static_cast<int>(s.first.intervals().size());
    if (if_count < interval_count) return false;

    auto [flow_nodes, case_nodes] = analyze_control_flow();

    // don't create switch statements with only one non-default label when the corresponding condition tree is flat
    // it may be important that the switch-like conditions be inlined
    // for example, a loop condition: while (c == '\n' || c == '\r')
    if (analysis_.sections.size() == 2 && is_single_condition(flow_nodes, case_nodes)) return false;

    // if there is no ILSwitch, there's still many control flow patterns that
    // match a switch statement but were originally just regular if statements,
    // and converting them to switches results in poor quality code with goto statements
    //
    // If a single break target cannot be identified, then the equivalent switch statement would require goto statements.
    // These goto statements may be "goto case x" or "goto default", but these are a hint that the original code was not a switch,
    // and that the switch statement may be very poor quality.
    // Thus the rule of thumb is no goto statements if the original code didn't include them
    Block* break_block = nullptr;
    if (switch_uses_goto(flow_nodes, case_nodes, &break_block)) return false;

    // valid switch construction, all code can be inlined
    if (break_block == nullptr) return true;

    // The switch has a single break target and there is one more hint
    // The break target cannot be inlined, and should have the highest IL offset of everything targetted by the switch
    int max_offset = INT_MIN;
    for (const auto& s : analysis_.sections) {
        Block* b = nullptr;
        int offset = s.second->match_branch(&b) ? b->start_il_offset() : -1;
        if (offset > max_offset) max_offset = offset;
    }
    return break_block->start_il_offset() >= max_offset;
}

// stloc switchValueVar(call ComputeStringHash(switchValue))
bool SwitchDetection::match_roslyn_switch_on_string() {
    auto& insns = analysis_.root_block()->instructions;
#ifdef CB_HAVE_SWITCH_ON_STRING_TRANSFORM
    LdLoc* switch_value = nullptr;
    return insns.count() >= 3 &&
           transforms::SwitchOnStringTransform::match_compute_string_or_read_only_span_hash_call(insns[insns.count() - 3],
                                                                                                  analysis_.switch_variable(), &switch_value);
#else
    // WAITS: SwitchOnStringTransform (match_compute_string_or_read_only_span_hash_call)
    (void)insns;
    return false;
#endif
}

std::pair<std::vector<ControlFlowNode*>, std::vector<ControlFlowNode*>> SwitchDetection::analyze_control_flow() {
    if (control_flow_graph_ == nullptr)
        control_flow_graph_ = std::make_unique<ControlFlowGraph>(current_container_, context_->cancellation_token);

    ControlFlowNode* switch_head = control_flow_graph_->get_node(analysis_.root_block());
    loop_context_ = std::make_unique<LoopContext>(*control_flow_graph_, switch_head);

    std::vector<ControlFlowNode*> flow_nodes{switch_head};
    for (Block* inner_block : analysis_.inner_blocks) flow_nodes.push_back(control_flow_graph_->get_node(inner_block));

    // grab the control flow nodes for blocks targetted by each section
    std::vector<ControlFlowNode*> case_nodes;
    for (const auto& s : analysis_.sections) {
        Block* block = nullptr;
        if (!s.second->match_branch(&block)) continue;

        if (block->parent() == current_container_) {
            ControlFlowNode* node = control_flow_graph_->get_node(block);
            if (!loop_context_->match_continue(node)) case_nodes.push_back(node);
        }
    }

    add_null_case(flow_nodes, case_nodes);

    return {std::move(flow_nodes), std::move(case_nodes)};
}

// Determines if the analysed switch can be constructed without any gotos
bool SwitchDetection::switch_uses_goto(const std::vector<ControlFlowNode*>& flow_nodes, const std::vector<ControlFlowNode*>& case_nodes,
                                       Block** break_block) {
    auto contains = [](const std::vector<ControlFlowNode*>& list, ControlFlowNode* node) {
        return std::find(list.begin(), list.end(), node) != list.end();
    };
    // cases with predecessors that aren't part of the switch logic must either require "goto case" statements, or consist of a
    // single "break;"
    std::vector<ControlFlowNode*> external_cases;
    for (ControlFlowNode* c : case_nodes) {
        for (ControlFlowNode* n : c->predecessors) {
            if (!contains(flow_nodes, n)) {
                external_cases.push_back(c);
                break;
            }
        }
    }

    *break_block = nullptr;
    if (external_cases.size() > 1) return true;  // cannot have more than one break case without gotos

    // check that case nodes flow through a single point
    std::vector<ControlFlowNode*> break_targets;
    std::vector<ControlFlowNode*> visited_cases;
    for (ControlFlowNode* n : case_nodes) {
        if (contains(external_cases, n) || contains(visited_cases, n)) continue;
        visited_cases.push_back(n);
        for (ControlFlowNode* target : loop_context_->get_break_targets(n))
            if (!contains(break_targets, target)) break_targets.push_back(target);
    }

    // if there are multiple break targets, then gotos are required
    // if there are none, then the external case (if any) can be the break target
    if (break_targets.size() != 1) return break_targets.size() > 1;

    *break_block = break_targets.front()->user_data;

    // external case must consist of a single "break;"
    return external_cases.size() == 1 && *break_block != external_cases.front()->user_data;
}

// Does some of the analysis of SwitchOnNullableTransform to add the null case control flow to the results of SwitchAnalysis
void SwitchDetection::add_null_case(std::vector<ControlFlowNode*>& flow_nodes, std::vector<ControlFlowNode*>& case_nodes) {
    if (analysis_.root_block()->incoming_edge_count() != 1) return;

    // if (comp(logic.not(call get_HasValue(ldloca nullableVar))) br NullCase
    // br RootBlock
    ControlFlowNode* root_node = control_flow_graph_->get_node(analysis_.root_block());
    Block* nullable_block = root_node->predecessors.size() == 1 ? root_node->predecessors[0]->user_data : nullptr;
#ifdef CB_HAVE_NULLABLE_LIFTING_TRANSFORM
    ILInstruction* cond = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* get_has_value = nullptr;
    ILInstruction* nullable_inst = nullptr;
    if (nullable_block == nullptr || nullable_block->instructions.count() < 2 ||
        !nullable_block->instructions.last()->match_branch(analysis_.root_block()) ||
        nullable_block->instructions.second_to_last_or_default() == nullptr ||
        !nullable_block->instructions.second_to_last_or_default()->match_if_instruction(&cond, &true_inst) ||
        !cond->match_logic_not(&get_has_value) ||
        !transforms::NullableLiftingTransform::match_has_value_call(get_has_value, &nullable_inst)) {
        return;
    }

    // could check that nullableInst is ldloc or ldloca and that the switch variable matches a GetValueOrDefault
    // but the effect of adding an incorrect block to the flowBlock list would only be disasterous if it branched directly
    // to a candidate case block

    // must branch to a case label, otherwise we can proceed fine and let SwitchOnNullableTransform do all the work
    Block* null_block = nullptr;
    if (!true_inst->match_branch(&null_block) ||
        std::none_of(case_nodes.begin(), case_nodes.end(), [&](ControlFlowNode* n) { return n->user_data == null_block; }))
        return;

    // add the null case logic to the incoming flow blocks
    flow_nodes.push_back(control_flow_graph_->get_node(nullable_block));
#else
    // WAITS: NullableLiftingTransform (match_has_value_call)
    (void)nullable_block;
    (void)flow_nodes;
    (void)case_nodes;
#endif
}

// Pattern matching for short circuit expressions. A parent node p with two successors, n (the side to consider) and s (its sibling),
// where n has p as its only predecessor, is a flow node and has s as one of its (at most two) successors:
//
//   p: if (a) goto n; goto s;
//   n: if (b) goto c; goto s;
//
// can be simplified to one node p|n with the successors s and c:
//
//   p|n: if (a && b) goto c; goto s;
//
// Note that if n has only 1 successor, but is still a flow node, then a short circuit expression has a target (c) with no
// corresponding block (leave).
bool SwitchDetection::is_short_circuit(ControlFlowNode* parent, int side) {
    ControlFlowNode* node = parent->successors[static_cast<std::size_t>(side)];
    ControlFlowNode* sibling = parent->successors[static_cast<std::size_t>(side ^ 1)];

    if (!is_flow_node(node) || node->successors.size() > 2 || node->predecessors.size() != 1) return false;

    return std::find(node->successors.begin(), node->successors.end(), sibling) != node->successors.end();
}

// A flow node contains only two instructions, the first of which is an IfInstruction.
// A short circuit expression is comprised of a root block ending in an IfInstruction and one or more flow nodes
bool SwitchDetection::is_flow_node(ControlFlowNode* n) { return is<IfInstruction>(n->user_data->instructions.first_or_default()); }

// Determines whether the flowNodes can be reduced to a single condition via short circuit operators
bool SwitchDetection::is_single_condition(const std::vector<ControlFlowNode*>& flow_nodes, const std::vector<ControlFlowNode*>& case_nodes) {
    if (flow_nodes.size() == 1) return true;

    ControlFlowNode* root_node = control_flow_graph_->get_node(analysis_.root_block());
    root_node->visited = true;

    // search down the tree, marking nodes as visited while they continue the current condition
    ControlFlowNode* n = root_node;
    while (!n->successors.empty() && (n == root_node || is_flow_node(n))) {
        if (n->successors.size() == 1) {
            // if there is more than one case node, then a flow node with only one successor is not part of the initial condition
            if (case_nodes.size() > 1) break;

            n = n->successors[0];
        } else {
            // 2 successors
            if (is_short_circuit(n, 0))
                n = n->successors[0];
            else if (is_short_circuit(n, 1))
                n = n->successors[1];
            else
                break;
        }

        n->visited = true;
        if (loop_context_->match_continue(n)) break;
    }

    bool ret = std::all_of(flow_nodes.begin(), flow_nodes.end(), [](ControlFlowNode* f) { return f->visited; });
    reset_visited(control_flow_graph_->nodes());
    return ret;
}

void SwitchDetection::reset_visited(const std::vector<ControlFlowNode*>& nodes) {
    for (ControlFlowNode* n : nodes) n->visited = false;
}

}  // namespace cb::dotnet::decompiler::il::control_flow
