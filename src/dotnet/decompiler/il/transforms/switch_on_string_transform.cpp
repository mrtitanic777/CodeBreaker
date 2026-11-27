// dotnet/decompiler/il/transforms/switch_on_string_transform.cpp -- see switch_on_string_transform.hpp.
#include "dotnet/decompiler/il/transforms/switch_on_string_transform.hpp"

#include <algorithm>
#include <cassert>
#include <set>
#include <stdexcept>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using control_flow::SwitchDetection;
using typesystem::KnownTypeCode;
using util::LongInterval;
using util::LongSet;

namespace {

using OptString = SwitchOnStringTransform::OptString;

// string.IsNullOrEmpty
bool is_null_or_empty(const OptString& value) { return !value.has_value() || value->empty(); }

// C#'s `(T)inst` for a node that must be a T.
template <class T>
T* checked_cast(ILInstruction* inst) {
    T* result = as<T>(inst);
    if (result == nullptr) throw std::bad_cast();
    return result;
}

// The string as the UTF-16 code units C# counts and indexes.
std::u16string to_utf16(const std::string& s) {
    std::u16string result;
    for (std::size_t i = 0; i < s.size();) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        char32_t cp;
        int extra;
        if (c < 0x80) {
            cp = c;
            extra = 0;
        } else if ((c & 0xE0) == 0xC0) {
            cp = c & 0x1Fu;
            extra = 1;
        } else if ((c & 0xF0) == 0xE0) {
            cp = c & 0x0Fu;
            extra = 2;
        } else {
            cp = c & 0x07u;
            extra = 3;
        }
        ++i;
        for (int k = 0; k < extra && i < s.size(); ++k, ++i) cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3Fu);
        if (cp >= 0x10000) {
            cp -= 0x10000;
            result.push_back(static_cast<char16_t>(0xD800 + (cp >> 10)));
            result.push_back(static_cast<char16_t>(0xDC00 + (cp & 0x3FF)));
        } else {
            result.push_back(static_cast<char16_t>(cp));
        }
    }
    return result;
}

// char.ToString() as UTF-8 (a lone surrogate is written as its own three bytes)
std::string utf16_unit_to_string(char16_t unit) {
    std::string result;
    if (unit < 0x80) {
        result.push_back(static_cast<char>(unit));
    } else if (unit < 0x800) {
        result.push_back(static_cast<char>(0xC0 | (unit >> 6)));
        result.push_back(static_cast<char>(0x80 | (unit & 0x3F)));
    } else {
        result.push_back(static_cast<char>(0xE0 | (unit >> 12)));
        result.push_back(static_cast<char>(0x80 | ((unit >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (unit & 0x3F)));
    }
    return result;
}

}  // namespace

bool SwitchOnStringTransform::HashtableInitializer::add(const IField* field, HashtableInitializerInfo info) {
    if (find(field) != nullptr) return false;
    entries_.emplace_back(field, std::move(info));
    return true;
}

SwitchOnStringTransform::HashtableInitializerInfo* SwitchOnStringTransform::HashtableInitializer::find(const IField* field) {
    for (auto& entry : entries_)
        if (typesystem::member_equals(entry.first, field)) return &entry.second;
    return nullptr;
}

void SwitchOnStringTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().switch_statement_on_string) return;

    context_ = &context;
    auto* body = checked_cast<BlockContainer>(function->body());
    HashtableInitializer hashtable_initializers = scan_hashtable_initializer_blocks(body->entry_point());

    // a HashSet upstream, only walked for the sort: insertion order
    std::vector<BlockContainer*> changed_containers;
    std::unordered_set<BlockContainer*> changed_set;
    auto add_changed_container = [&](BlockContainer* container) {
        if (changed_set.insert(container).second) changed_containers.push_back(container);
    };

    for (ILInstruction* descendant : function->descendants()) {
        auto* block = as<Block>(descendant);
        if (block == nullptr) continue;
        bool changed = false;
        if (block->incoming_edge_count() == 0) continue;
        for (int i = block->instructions.count() - 1; i >= 0; i--) {
            if (simplify_cascading_if_statements(block->instructions, i)) {
                changed = true;
                continue;
            }
            if (simplify_csharp1_cascading_if_statements(block->instructions, i)) {
                changed = true;
                continue;
            }
            if (match_legacy_switch_on_string_with_hashtable(block, hashtable_initializers, i)) {
                changed = true;
                continue;
            }
            if (match_legacy_switch_on_string_with_dict(block->instructions, i)) {
                changed = true;
                continue;
            }
            if (match_roslyn_switch_on_string(block->instructions, i)) {
                changed = true;
                continue;
            }
            if (match_roslyn_switch_on_string_using_length_and_char(block, i)) {
                changed = true;
                continue;
            }
        }
        if (!changed) continue;
        SwitchDetection::simplify_switch_instruction(block, context);
        SwitchDetection::inline_switch_expression_default_case_throw_helper(block, context);
        if (auto* container = as<BlockContainer>(block->parent())) add_changed_container(container);
    }

    std::unordered_map<Block*, Block*> omitted_blocks;

    // Remove all transformed hashtable initializers from the entrypoint.
    for (auto& item : hashtable_initializers.entries()) {
        HashtableInitializerInfo& info = item.second;
        if (!info.transformed) continue;
        Block* actual = info.previous;
        if (auto found = omitted_blocks.find(info.previous); found != omitted_blocks.end()) actual = found->second;
        context.step("Remove hashtable initializer", actual);
        if (info.jump_to_next != nullptr) {
            actual->instructions.second_to_last_or_default()->replace_with(info.jump_to_next);
        }
        actual->instructions.last_or_default()->replace_with(make<Branch>(info.next));
        // Dictionary.Add: an ArgumentException for a block that is in already
        if (!omitted_blocks.emplace(info.containing_block, info.previous).second)
            throw std::invalid_argument("An item with the same key has already been added.");
        add_changed_container(body);
    }

    // If all initializer where removed, remove the initial null check as well.
    if (hashtable_initializers.count() > 0 && static_cast<int>(omitted_blocks.size()) == hashtable_initializers.count() &&
        body->entry_point()->instructions.count() == 2) {
        auto* if_inst = as<IfInstruction>(body->entry_point()->instructions[0]);
        Block* begin_of_method = nullptr;
        if (if_inst != nullptr && if_inst->true_inst()->match_branch(&begin_of_method) &&
            body->entry_point()->instructions[1]->match_branch(begin_of_method)) {
            context.step("Remove initial null check", body);
            body->entry_point()->instructions.remove_at(0);
        }
    }

    for (BlockContainer* container : changed_containers) container->sort_blocks(true);
}

SwitchOnStringTransform::HashtableInitializer SwitchOnStringTransform::scan_hashtable_initializer_blocks(Block* entry_point) {
    HashtableInitializer hashtables;
    if (entry_point->instructions.count() != 2) return hashtables;
    // match first block: checking compiler-generated Hashtable for null
    // if (comp(volatile.ldobj System.Collections.Hashtable(ldsflda $$method0x600003f-1) != ldnull)) br switchHeadBlock
    // br tableInitBlock
    ILInstruction* condition = nullptr;
    ILInstruction* branch_to_switch_head = nullptr;
    if (!(entry_point->instructions[0]->match_if_instruction(&condition, &branch_to_switch_head))) return hashtables;
    Block* table_init_block = nullptr;
    if (!entry_point->instructions[1]->match_branch(&table_init_block)) return hashtables;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    const IField* dict_field = nullptr;
    const IType* dictionary_type = nullptr;
    if (!(condition->match_comp_not_equals(&left, &right) && right->match_ld_null() &&
          match_dictionary_field_load(left, is_non_generic_hashtable, &dict_field, &dictionary_type)))
        return hashtables;
    Block* switch_head = nullptr;
    if (!branch_to_switch_head->match_branch(&switch_head)) return hashtables;
    // match second block: initialization of compiler-generated Hashtable
    // stloc table(newobj Hashtable..ctor(ldc.i4 capacity, ldc.f loadFactor))
    // call Add(ldloc table, ldstr value, box System.Int32(ldc.i4 index))
    // ... more calls to Add ...
    // volatile.stobj System.Collections.Hashtable(ldsflda $$method0x600003f - 1, ldloc table)
    // br switchHeadBlock
    if (table_init_block->incoming_edge_count() != 1 || table_init_block->instructions.count() < 3) return hashtables;
    Block* previous_block = entry_point;
    while (table_init_block != nullptr) {
        StringIndexList string_values;
        Block* block_after_this_init_block = nullptr;
        if (!extract_string_values_from_init_block(table_init_block, &string_values, &block_after_this_init_block, dictionary_type, dict_field, true))
            break;
        auto* next_hashtable_init_head = as<IfInstruction>(table_init_block->instructions.second_to_last_or_default());
        HashtableInitializerInfo info;
        info.labels = std::move(string_values);
        info.jump_to_next = next_hashtable_init_head;
        info.containing_block = table_init_block;
        info.previous = previous_block;
        info.next = block_after_this_init_block;
        info.transformed = false;
        // Dictionary.Add: an ArgumentException for a field that is in already
        if (!hashtables.add(dict_field, std::move(info))) throw std::invalid_argument("An item with the same key has already been added.");
        previous_block = table_init_block;
        // if there is another IfInstruction before the end of the block, it might be a jump to the next hashtable init block.
        // if (comp(volatile.ldobj System.Collections.Hashtable(ldsflda $$method0x600003f-2) != ldnull)) br switchHeadBlock
        if (next_hashtable_init_head != nullptr) {
            const IField* next_dict_field = nullptr;
            const IType* ignored_type = nullptr;
            if (!(next_hashtable_init_head->condition()->match_comp_not_equals(&left, &right) && right->match_ld_null() &&
                  match_dictionary_field_load(left, is_non_generic_hashtable, &next_dict_field, &ignored_type)))
                break;
            if (!next_hashtable_init_head->true_inst()->match_branch(switch_head)) break;
            table_init_block = block_after_this_init_block;
            dict_field = next_dict_field;
        } else {
            break;
        }
    }
    return hashtables;
}

bool SwitchOnStringTransform::simplify_cascading_if_statements(InstructionCollection<ILInstruction>& instructions, int& i) {
    // match first block: checking switch-value for null or first value (Roslyn)
    // if (call op_Equality(ldloc switchValueVar, ldstr value)) br firstBlock
    // -or-
    // if (comp(ldloc switchValueVar == ldnull)) br defaultBlock
    ILInstruction* condition = nullptr;
    ILInstruction* first_block_or_default_jump = nullptr;
    if (!instructions[i]->match_if_instruction(&condition, &first_block_or_default_jump)) return false;
    ILInstruction* next_case_jump = instructions[i + 1];
    ILInstruction* arg = nullptr;
    while (condition->match_logic_not(&arg)) {
        condition = arg;
        std::swap(first_block_or_default_jump, next_case_jump);
    }
    // match call to operator ==(string, string)
    ILVariable* switch_value_var = nullptr;
    OptString first_block_value;
    bool is_vb_compare_string = false;
    if (!match_string_equality_comparison(condition, &switch_value_var, &first_block_value, &is_vb_compare_string)) return false;
    if (is_vb_compare_string) {
        std::swap(first_block_or_default_jump, next_case_jump);
    }

    Block* first_block = nullptr;
    BlockContainer* ignored_container = nullptr;
    if (first_block_or_default_jump->match_branch(&first_block)) {
        // success
    } else if (first_block_or_default_jump->match_leave(&ignored_container)) {
        first_block = nullptr;
        // success
    } else {
        return false;
    }

    StringTargetList values;
    std::set<OptString> unique_values;
    int number_of_unique_matches_with_current_variable = 0;
    std::vector<Block*> case_blocks;
    case_blocks.push_back(checked_cast<Block>(instructions[i]->parent()));

    auto add_switch_section = [&](const OptString& value, ILInstruction* inst) {
        if (!unique_values.insert(value).second) return false;
        number_of_unique_matches_with_current_variable++;
        values.emplace_back(value, inst);
        return true;
    };

    ILInstruction* switch_value = nullptr;
    ILInstruction* first_target = first_block != nullptr ? static_cast<ILInstruction*>(first_block) : first_block_or_default_jump;
    if (is_vb_compare_string && is_null_or_empty(first_block_value)) {
        if (!add_switch_section(std::nullopt, first_target)) return false;
        if (!add_switch_section(std::string(), first_target)) return false;
    } else {
        if (!add_switch_section(first_block_value, first_target)) return false;
    }

    bool remove_extra_load = false;
    bool keep_assignment_before = false;
    ILVariable* other_switch_value_var = nullptr;
    ILInstruction* new_switch_value = nullptr;
    auto* stloc = i >= 1 ? as<StLoc>(instructions[i - 1]) : nullptr;
    if (i >= 1 && instructions[i - 1]->match_st_loc(switch_value_var, &switch_value)) {
        // stloc switchValueVar(switchValue)
        // if (call op_Equality(ldloc switchValueVar, ldstr value)) br firstBlock

        // Newer versions of Roslyn use extra variables:
        if (i >= 2 && switch_value->match_ld_loc(&other_switch_value_var) && other_switch_value_var->is_single_definition() &&
            other_switch_value_var->load_count() == 1 && instructions[i - 2]->match_st_loc(other_switch_value_var, &new_switch_value)) {
            switch_value = new_switch_value;
            remove_extra_load = true;
        }
    } else if (i >= 1 && stloc != nullptr) {
        if (stloc->value()->match_ld_loc(switch_value_var)) {
            // in case of optimized legacy code there are two stlocs:
            // stloc otherSwitchValueVar(ldloc switchValue)
            // stloc switchValueVar(ldloc otherSwitchValueVar)
            // if (call op_Equality(ldloc otherSwitchValueVar, ldstr value)) br firstBlock
            ILVariable* other_var = switch_value_var;
            switch_value_var = stloc->variable();
            number_of_unique_matches_with_current_variable = 0;
            if (i >= 2 && instructions[i - 2]->match_st_loc(other_var, &switch_value) && other_var->is_single_definition() &&
                other_var->load_count() == 2) {
                remove_extra_load = true;
            } else {
                switch_value = make<LdLoc>(other_var);
            }
        } else {
            // Variable before the start of the switch is not related to the switch.
            keep_assignment_before = true;
            switch_value = make<LdLoc>(switch_value_var);
        }
    } else {
        // Instruction before the start of the switch is not related to the switch.
        keep_assignment_before = true;
        switch_value = make<LdLoc>(switch_value_var);
    }
    if (!typesystem::is_known_type(switch_value_var->type(), KnownTypeCode::String)) {
        if (!context_->settings().switch_on_read_only_span_char) return false;
        if (!typesystem::is_known_type(switch_value_var->type(), KnownTypeCode::ReadOnlySpanOfT) &&
            !typesystem::is_known_type(switch_value_var->type(), KnownTypeCode::SpanOfT)) {
            return false;
        }
    }
    // if instruction must be followed by a branch to the next case
    Block* current_case_block = nullptr;
    if (!next_case_jump->match_branch(&current_case_block)) return false;
    // extract all cases and add them to the values list.
    ILInstruction* next_case_block = nullptr;
    do {
        OptString value;
        bool empty_string_equals_null = false;
        ILInstruction* block = nullptr;
        next_case_block = match_case_block(current_case_block, switch_value_var, &value, &empty_string_equals_null, &block);
        if (next_case_block == nullptr) break;
        if (empty_string_equals_null && is_null_or_empty(value)) {
            if (!add_switch_section(std::nullopt, block)) return false;
            if (!add_switch_section(std::string(), block)) return false;
        } else {
            if (!add_switch_section(value, block)) return false;
        }
        case_blocks.push_back(current_case_block);
        current_case_block = as<Block>(next_case_block);
    } while (current_case_block != nullptr);

    // Short if-chains are more plausibly hand-written than a switch, so we require
    // at least 3 cases - unless the chain ends in a compiler-generated throw helper,
    // which proves that the source was a switch expression with an implicit default case.
    if (values.size() < 3 && !(current_case_block != nullptr && SwitchDetection::is_switch_expression_throw_helper_block(current_case_block))) {
        return false;
    }
    context_->step("SimplifyCascadingIfStatements", instructions[i]);
    // if the switchValueVar is used in other places as well, do not eliminate the store.
    if (switch_value_var->load_count() > number_of_unique_matches_with_current_variable ||
        !validate_uses_of_switch_value_variable(switch_value_var, case_blocks)) {
        keep_assignment_before = true;
        remove_extra_load = false;  // prevent loads from being deleted after detecting that
                                    // we have to keep the assignment before the switch statement
        switch_value = make<LdLoc>(switch_value_var);
    }
    int offset = first_block == nullptr ? 1 : 0;
    SectionList sections;
    {
        int index = 0;
        for (std::size_t k = static_cast<std::size_t>(offset); k < values.size(); k++, index++) {
            auto* section = make<SwitchSection>();
            section->set_labels(LongSet(static_cast<std::int64_t>(index)));
            ILInstruction* section_body = values[k].second;
            if (auto* b = as<Block>(section_body))
                section->set_body(make<Branch>(b));
            else
                section->set_body(section_body->clone());
            sections.push_back(section);
        }
    }
    {
        auto* default_section = make<SwitchSection>();
        default_section->set_labels(LongSet(LongInterval(0, static_cast<std::int64_t>(sections.size()))).invert());
        if (current_case_block != nullptr)
            default_section->set_body(make<Branch>(current_case_block));
        else
            default_section->set_body(make<Leave>(as<BlockContainer>(next_case_block)));
        sections.push_back(default_section);
    }
    std::vector<OptString> string_map;
    for (std::size_t k = static_cast<std::size_t>(offset); k < values.size(); k++) string_map.push_back(values[k].first);
    auto* string_to_int = make<StringToInt>(switch_value, string_map, switch_value_var->type());
    auto* inst = make<SwitchInstruction>(string_to_int);
    inst->sections.add_range(sections);
    if (remove_extra_load) {
        inst->add_il_range(*instructions[i - 2]);
        instructions[i - 2]->replace_with(inst);
        instructions.remove_range(i - 1, 3);
        i -= 2;
    } else {
        if (keep_assignment_before) {
            inst->add_il_range(*instructions[i]);
            instructions[i]->replace_with(inst);
            instructions.remove_at(i + 1);
        } else {
            inst->add_il_range(*instructions[i - 1]);
            instructions[i - 1]->replace_with(inst);
            instructions.remove_range(i, 2);
            i--;
        }
    }
    context_->end_step(inst);
    return true;
}

bool SwitchOnStringTransform::validate_uses_of_switch_value_variable(ILVariable* switch_value_var, const std::vector<Block*>& case_blocks) {
    for (LdLoc* use : switch_value_var->load_instructions()) {
        bool is_valid = false;
        for (Block* case_block : case_blocks) {
            if (use->is_descendant_of(case_block)) is_valid = true;
        }
        if (!is_valid) return false;
    }

    return true;
}

bool SwitchOnStringTransform::simplify_csharp1_cascading_if_statements(InstructionCollection<ILInstruction>& instructions, int& i) {
    if (i < 1) return false;
    // match first block:
    // stloc switchValueVar(ldloc temp)
    // if (comp(ldloc temp == ldnull)) br defaultBlock
    // br isInternedBlock
    ILInstruction* condition = nullptr;
    ILInstruction* default_block_jump = nullptr;
    if (!(instructions[i]->match_if_instruction(&condition, &default_block_jump))) return false;
    Block* is_interned_block = nullptr;
    if (!instructions[i + 1]->match_branch(&is_interned_block)) return false;
    Block* default_or_null_block = nullptr;
    if (!default_block_jump->match_branch(&default_or_null_block)) return false;
    ILInstruction* temp_load = nullptr;
    ILVariable* temp = nullptr;
    if (!(condition->match_comp_equals_null(&temp_load) && temp_load->match_ld_loc(&temp))) return false;
    if (!(temp->kind() == VariableKind::StackSlot && temp->load_count() == 2)) return false;
    ILVariable* switch_value_var = nullptr;
    ILInstruction* switch_value = nullptr;
    if (!(instructions[i - 1]->match_st_loc(&switch_value_var, &switch_value) && switch_value->match_ld_loc(temp))) return false;
    // match isInternedBlock:
    // stloc switchValueVarCopy(call IsInterned(ldloc switchValueVar))
    // if (comp(ldloc switchValueVarCopy == ldstr "case1")) br caseBlock1
    // br caseHeader2
    if (is_interned_block->incoming_edge_count() != 1 || is_interned_block->instructions.count() != 3) return false;
    ILVariable* switch_value_var_copy = nullptr;
    ILInstruction* arg = nullptr;
    if (!(is_interned_block->instructions[0]->match_st_loc(&switch_value_var_copy, &arg) && is_is_interned_call(as<Call>(arg), &arg) &&
          arg->match_ld_loc(switch_value_var)))
        return false;
    switch_value_var = switch_value_var_copy;
    int condition_offset = 1;
    Block* current_case_block = is_interned_block;
    StringTargetList values;

    if (!switch_value_var_copy->is_single_definition()) return false;

    // each case starts with:
    // if (comp(ldloc switchValueVar == ldstr "case label")) br caseBlock
    // br currentCaseBlock

    ILInstruction* case_block_jump = nullptr;
    while (current_case_block->instructions[condition_offset]->match_if_instruction(&condition, &case_block_jump)) {
        if (current_case_block->instructions.count() != condition_offset + 2) break;
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (!condition->match_comp_equals(&left, &right)) break;
        if (!left->match_ld_loc(switch_value_var)) break;
        std::string value;
        if (!right->match_ld_str(&value)) break;
        Block* case_block = nullptr;
        if (!(case_block_jump->match_branch(&case_block) || case_block_jump->match_leave(as<BlockContainer>(current_case_block->parent())))) break;
        if (!current_case_block->instructions[condition_offset + 1]->match_branch(&current_case_block)) break;
        condition_offset = 0;
        values.emplace_back(value, case_block_jump->clone());
    }

    if (static_cast<int>(values.size()) != switch_value_var_copy->load_count()) return false;
    context_->step("SimplifyCSharp1CascadingIfStatements", instructions[i]);

    // switch contains case null:
    if (current_case_block != default_or_null_block) {
        values.emplace_back(std::nullopt, make<Branch>(default_or_null_block));
    }

    SectionList sections;
    {
        int index = 0;
        for (auto& entry : values) {
            auto* section = make<SwitchSection>();
            section->set_labels(LongSet(static_cast<std::int64_t>(index)));
            section->set_body(entry.second);
            sections.push_back(section);
            index++;
        }
    }
    {
        auto* default_section = make<SwitchSection>();
        default_section->set_labels(LongSet(LongInterval(0, static_cast<std::int64_t>(sections.size()))).invert());
        default_section->set_body(make<Branch>(current_case_block));
        sections.push_back(default_section);
    }
    std::vector<OptString> string_map;
    for (auto& entry : values) string_map.push_back(entry.first);
    auto* string_to_int = make<StringToInt>(switch_value, string_map, context_->type_system().find_type(KnownTypeCode::String));
    auto* inst = make<SwitchInstruction>(string_to_int);
    inst->sections.add_range(sections);

    inst->add_il_range(*instructions[i - 1]);
    instructions[i]->replace_with(inst);
    instructions.remove_at(i + 1);
    instructions.remove_at(i - 1);

    context_->end_step(inst);
    return true;
}

bool SwitchOnStringTransform::is_is_interned_call(Call* call, ILInstruction** argument) {
    if (call != nullptr && typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::String) && call->method->is_static() &&
        call->method->name() == "IsInterned" && call->arguments().count() == 1) {
        *argument = call->arguments()[0];
        return true;
    }
    *argument = nullptr;
    return false;
}

/// Each case consists of two blocks:
/// 1. block:
/// if (call op_Equality(ldloc switchVariable, ldstr value)) br caseBlock
/// br nextBlock
/// -or-
/// if (comp(ldloc switchValueVar == ldnull)) br nextBlock
/// br caseBlock
/// 2. block is caseBlock
/// This method matches the above pattern or its inverted form:
/// the call to ==(string, string) is wrapped in logic.not and the branch targets are reversed.
/// Returns the next block that follows in the block-chain.
/// The switchVariable is updated if the value gets copied to a different variable.
/// See comments below for more info.
ILInstruction* SwitchOnStringTransform::match_case_block(Block* current_block, ILVariable* switch_variable, OptString* value,
                                                         bool* empty_string_equals_null, ILInstruction** case_block_or_leave) {
    *value = std::nullopt;
    *case_block_or_leave = nullptr;
    *empty_string_equals_null = false;

    if (current_block->incoming_edge_count() != 1 || current_block->instructions.count() != 2) return nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* case_block_branch = nullptr;
    ILInstruction* next_block_branch = nullptr;
    if (!current_block->match_if_at_end_of_block(&condition, &case_block_branch, &next_block_branch)) return nullptr;
    bool is_vb_compare_string = false;
    if (!match_string_equality_comparison(condition, switch_variable, value, &is_vb_compare_string)) {
        return nullptr;
    }
    if (is_vb_compare_string) {
        std::swap(case_block_branch, next_block_branch);
        *empty_string_equals_null = true;
    }
    Block* case_block = nullptr;
    BlockContainer* ignored_container = nullptr;
    if (case_block_branch->match_branch(&case_block)) {
        *case_block_or_leave = case_block;
    } else if (case_block_branch->match_leave(&ignored_container)) {
        *case_block_or_leave = case_block_branch;
    } else {
        return nullptr;
    }
    Block* next_block = nullptr;
    BlockContainer* block_container = nullptr;
    if (next_block_branch->match_branch(&next_block)) {
        // success
        return next_block;
    } else if (next_block_branch->match_leave(&block_container)) {
        // success
        return block_container;
    } else {
        return nullptr;
    }
}

/// Matches the C# 2.0 switch-on-string pattern, which uses Dictionary<string, int>.
bool SwitchOnStringTransform::match_legacy_switch_on_string_with_dict(InstructionCollection<ILInstruction>& instructions, int& i) {
    // match first block: checking switch-value for null:
    // (In some cases, i.e., if switchValueVar is a parameter, the initial store is optional.)
    // stloc switchValueVar(switchValue)
    // if (comp(ldloc switchValueVar == ldnull)) br nullCase
    // br nextBlock
    ILInstruction* condition = nullptr;
    ILInstruction* exit_block_jump = nullptr;
    if (!instructions[i]->match_if_instruction(&condition, &exit_block_jump)) return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!(condition->match_comp_equals(&left, &right) && right->match_ld_null())) return false;
    // Extract switchValueVar
    ILVariable* switch_value_var = nullptr;
    if (!left->match_ld_loc(&switch_value_var) || !switch_value_var->is_single_definition()) return false;
    // If the switchValueVar is a stack slot and there is an assignment involving it right before the
    // switch-value null-check, we use the previously assigned variable as switchValueVar.
    ILInstruction* switch_value = nullptr;
    auto* extra_store = as<StLoc>(instructions.element_at_or_default(i - 1));
    if (switch_value_var->kind() == VariableKind::StackSlot && extra_store != nullptr && extra_store->value()->match_ld_loc(switch_value_var)) {
        switch_value_var = extra_store->variable();
        switch_value = extra_store->value();
    } else {
        switch_value = nullptr;
    }
    if (!typesystem::is_known_type(switch_value_var->type(), KnownTypeCode::String)) return false;
    // either br nullCase or leave container
    BlockContainer* leave_container = nullptr;
    Block* null_value_case_block = nullptr;
    if (!exit_block_jump->match_branch(&null_value_case_block) && !exit_block_jump->match_leave(&leave_container)) return false;
    auto* next_block_jump = as<Branch>(instructions.element_at_or_default(i + 1));
    if (next_block_jump == nullptr || next_block_jump->target_block()->incoming_edge_count() != 1) return false;
    // match second block: checking compiler-generated Dictionary<string, int> for null
    // if (comp(volatile.ldobj System.Collections.Generic.Dictionary`2[[System.String],[System.Int32]](ldsflda $$method0x600000c-1) != ldnull)) br caseNullBlock
    // br dictInitBlock
    Block* next_block = next_block_jump->target_block();
    ILInstruction* try_get_value_block_jump = nullptr;
    if (next_block->instructions.count() != 2 || !next_block->instructions[0]->match_if_instruction(&condition, &try_get_value_block_jump)) return false;
    Block* try_get_value_block = nullptr;
    if (!try_get_value_block_jump->match_branch(&try_get_value_block)) return false;
    Block* dict_init_block = nullptr;
    if (!next_block->instructions[1]->match_branch(&dict_init_block) || dict_init_block->incoming_edge_count() != 1) return false;
    const IField* dict_field = nullptr;
    const IType* dictionary_type = nullptr;
    if (!(condition->match_comp_not_equals(&left, &right) && right->match_ld_null() &&
          match_dictionary_field_load(left, is_string_to_int_dictionary, &dict_field, &dictionary_type)))
        return false;
    // match third block: initialization of compiler-generated Dictionary<string, int>
    // stloc dict(newobj Dictionary..ctor(ldc.i4 valuesLength))
    // call Add(ldloc dict, ldstr value, ldc.i4 index)
    // ... more calls to Add ...
    // volatile.stobj System.Collections.Generic.Dictionary`2[[System.String],[System.Int32]](ldsflda $$method0x600003f-1, ldloc dict)
    // br switchHeadBlock
    if (dict_init_block->incoming_edge_count() != 1 || dict_init_block->instructions.count() < 3) return false;
    StringIndexList string_values;
    Block* block_after_init = nullptr;
    if (!extract_string_values_from_init_block(dict_init_block, &string_values, &block_after_init, dictionary_type, dict_field, false)) return false;
    if (try_get_value_block != block_after_init) return false;
    // match fourth block: TryGetValue on compiler-generated Dictionary<string, int>
    // if (logic.not(call TryGetValue(volatile.ldobj System.Collections.Generic.Dictionary`2[[System.String],[System.Int32]](ldsflda $$method0x600000c-1), ldloc switchValueVar, ldloca switchIndexVar))) br defaultBlock
    // br switchBlock
    if (try_get_value_block->incoming_edge_count() != 2 || try_get_value_block->instructions.count() != 2) return false;
    ILInstruction* default_block_jump = nullptr;
    if (!try_get_value_block->instructions[0]->match_if_instruction(&condition, &default_block_jump)) return false;
    Block* default_block = nullptr;
    BlockContainer* ignored_container = nullptr;
    if (!default_block_jump->match_branch(&default_block) &&
        !((leave_container != nullptr && default_block_jump->match_leave(leave_container)) || default_block_jump->match_leave(&ignored_container)))
        return false;
    ILInstruction* arg = nullptr;
    const IField* dict_field2 = nullptr;
    const IType* ignored_type = nullptr;
    CallInstruction* c = nullptr;
    if (condition->match_logic_not(&arg)) c = as<CallInstruction>(arg);
    if (!(c != nullptr && c->method->name() == "TryGetValue" &&
          match_dictionary_field_load(c->arguments().first(), is_string_to_int_dictionary, &dict_field2, &ignored_type) &&
          typesystem::member_equals(dict_field2, dict_field)))
        return false;
    ILVariable* switch_index_var = nullptr;
    if (!c->arguments()[1]->match_ld_loc(switch_value_var) || !c->arguments()[2]->match_ld_loca(&switch_index_var)) return false;
    Block* switch_block = nullptr;
    if (!try_get_value_block->instructions[1]->match_branch(&switch_block)) return false;
    // match fifth block: switch-instruction block
    // switch (ldloc switchVariable) {
    // 	case [0..1): br caseBlock1
    //  ... more cases ...
    // 	case [long.MinValue..0),[13..long.MaxValue]: br defaultBlock
    // }
    // mcs has a bug: when there is only one case it still generates the full-blown Dictionary<string, int> pattern,
    // but uses only a simple if statement instead of the switch instruction.
    if (switch_block->incoming_edge_count() != 1 || switch_block->instructions.count() == 0) return false;
    SectionList sections;
    if (auto* switch_inst = as<SwitchInstruction>(switch_block->instructions[0])) {
        if (switch_block->instructions.count() != 1) return false;
        if (!switch_inst->value()->match_ld_loc(switch_index_var)) return false;
        for (SwitchSection* section : switch_inst->sections) sections.push_back(section);
    } else if (auto* if_inst = as<IfInstruction>(switch_block->instructions[0])) {
        if (switch_block->instructions.count() != 2) return false;
        if (!if_inst->condition()->match_comp_equals(&left, &right)) return false;
        if (!left->match_ld_loc(switch_index_var)) return false;
        if (!right->match_ldc_i4(0)) return false;
        auto* section1 = make<SwitchSection>();
        section1->set_body(if_inst->true_inst());
        section1->set_labels(LongSet(static_cast<std::int64_t>(0)));
        sections.push_back(with_il_range(section1, if_inst));
        auto* section2 = make<SwitchSection>();
        section2->set_body(switch_block->instructions[1]);
        section2->set_labels(LongSet(static_cast<std::int64_t>(0)).invert());
        sections.push_back(with_il_range(section2, switch_block->instructions[1]));
    }
    // mcs: map sections without a value to the default section, if possible
    if (!fix_cases_without_value(sections, string_values)) return false;
    // switch contains case null:
    if (null_value_case_block != nullptr && null_value_case_block != default_block) {
        if (!add_null_section(sections, string_values, null_value_case_block)) {
            return false;
        }
    } else if (leave_container != nullptr && !default_block_jump->match_leave(leave_container)) {
        if (!add_null_section(sections, string_values, checked_cast<Leave>(exit_block_jump))) {
            return false;
        }
    }
    context_->step("MatchLegacySwitchOnStringWithDict", instructions[i]);
    bool keep_assignment_before = false;
    if (switch_value_var->load_count() > 2 || switch_value == nullptr) {
        switch_value = make<LdLoc>(switch_value_var);
        keep_assignment_before = true;
    }
    auto* string_to_int = make<StringToInt>(switch_value, string_values, switch_value_var->type());
    auto* inst = make<SwitchInstruction>(string_to_int);
    inst->sections.add_range(sections);
    instructions[i + 1]->replace_with(inst);
    if (keep_assignment_before) {
        // delete if (comp(ldloc switchValueVar == ldnull))
        inst->add_il_range(*instructions[i]);
        instructions.remove_at(i);
        i--;
    } else {
        // delete both the if and the assignment before
        inst->add_il_range(*instructions[i - 1]);
        instructions.remove_range(i - 1, 2);
        i -= 2;
    }
    context_->end_step(inst);
    return true;
}

bool SwitchOnStringTransform::fix_cases_without_value(SectionList& sections, const StringIndexList& string_values) {
    auto has_label = [&](SwitchSection* section) {
        // section.Labels.Values.Any(i => stringValues.Any(value => i == value.Item2)): is one of the indices in the labels
        for (const auto& value : string_values)
            if (section->labels().contains(value.second)) return true;
        return false;
    };

    // Pick the section with the most labels as default section.
    // And collect all sections that have no value mapped to them.
    if (sections.empty()) throw UsageError("Sequence contains no elements");
    SwitchSection* default_section = sections.front();
    SectionList sections_without_labels;
    for (SwitchSection* section : sections) {
        if (section == default_section) continue;
        if (section->labels().count() > default_section->labels().count()) {
            if (!has_label(default_section)) sections_without_labels.push_back(default_section);
            default_section = section;
            continue;
        }
        if (!has_label(section)) sections_without_labels.push_back(section);
    }

    for (SwitchSection* section : sections_without_labels) {
        default_section->set_labels(default_section->labels().union_with(section->labels()));
        if (section->has_null_label()) default_section->set_has_null_label(true);
        auto it = std::find(sections.begin(), sections.end(), section);
        if (it != sections.end()) sections.erase(it);
    }

    return true;
}

bool SwitchOnStringTransform::add_null_section(SectionList& sections, StringIndexList& string_values, Block* null_value_case_block) {
    return add_null_section(sections, string_values, static_cast<ILInstruction*>(make<Branch>(null_value_case_block)));
}

bool SwitchOnStringTransform::add_null_section(SectionList& sections, StringIndexList& string_values, ILInstruction* body) {
    if (string_values.empty()) throw UsageError("Sequence contains no elements");
    int max_index = string_values.front().second;
    for (const auto& item : string_values) max_index = std::max(max_index, item.second);
    LongSet label(static_cast<std::int64_t>(max_index) + 1);
    SectionList possible_conflicts;
    for (SwitchSection* sec : sections)
        if (sec->labels().overlaps(label)) possible_conflicts.push_back(sec);
    if (possible_conflicts.size() > 1) {
        return false;
    } else if (possible_conflicts.size() == 1) {
        if (possible_conflicts[0]->labels().count() == 1) return false;  // cannot remove only label
        possible_conflicts[0]->set_labels(possible_conflicts[0]->labels().except_with(label));
    }
    string_values.emplace_back(std::nullopt, static_cast<int>(label.values().front()));
    auto* section = make<SwitchSection>();
    section->set_labels(label);
    section->set_body(body);
    sections.push_back(section);
    return true;
}

/// Matches 'volatile.ldobj dictionaryType(ldsflda dictField)'
bool SwitchOnStringTransform::match_dictionary_field_load(ILInstruction* inst, const TypeMatcher& type_matcher, const IField** dict_field,
                                                          const IType** dictionary_type) {
    *dict_field = nullptr;
    *dictionary_type = nullptr;
    ILInstruction* dictionary_field_load = nullptr;
    return inst->match_ld_obj(&dictionary_field_load, dictionary_type) && type_matcher(*dictionary_type) &&
           dictionary_field_load->match_lds_flda(dict_field) &&
           (is_compiler_generated_or_is_in_compiler_generated_class(*dict_field) || (*dict_field)->name().starts_with("$$method"));
}

/// Matches and extracts values from Add-call sequences.
bool SwitchOnStringTransform::extract_string_values_from_init_block(Block* block, StringIndexList* values, Block** block_after_init,
                                                                    const IType* dictionary_type, const IField* dictionary_field,
                                                                    bool is_hashtable_pattern) {
    values->clear();
    *block_after_init = nullptr;
    // stloc dictVar(newobj Dictionary..ctor(ldc.i4 valuesLength))
    // -or-
    // stloc dictVar(newobj Hashtable..ctor(ldc.i4 capacity, ldc.f4 loadFactor))
    ILVariable* dict_var = nullptr;
    ILInstruction* new_obj_dict = nullptr;
    if (!(block->instructions[0]->match_st_loc(&dict_var, &new_obj_dict) && is<NewObj>(new_obj_dict))) return false;
    auto* new_obj = as<NewObj>(new_obj_dict);
    if (!typesystem::type_equals(new_obj->method->declaring_type(), dictionary_type)) return false;
    std::int32_t values_length = 0;
    if (new_obj->arguments().count() == 2) {
        if (!new_obj->arguments()[0]->match_ldc_i4(&values_length)) return false;
        if (!new_obj->arguments()[1]->match_ldc_f4(0.5f)) return false;
    } else if (new_obj->arguments().count() == 1) {
        if (!new_obj->arguments()[0]->match_ldc_i4(&values_length)) return false;
    }
    if (values_length < 0) return false;
    values->reserve(static_cast<std::size_t>(values_length));
    int i = 0;
    int index = 0;
    std::string value;
    while (match_add_call(dictionary_type, block->instructions[i + 1], dict_var, &index, &value)) {
        values->emplace_back(value, index);
        i++;
    }
    // final store to compiler-generated variable:
    // volatile.stobj dictionaryType(ldsflda dictionaryField, ldloc dictVar)
    ILInstruction* load_field = nullptr;
    ILInstruction* dict_var_load = nullptr;
    const IType* dict_type = nullptr;
    const IField* dict_field = nullptr;
    if (!(block->instructions[i + 1]->match_st_obj(&load_field, &dict_var_load, &dict_type) && typesystem::type_equals(dict_type, dictionary_type) &&
          load_field->match_lds_flda(&dict_field) && typesystem::member_equals(dict_field, dictionary_field) && dict_var_load->match_ld_loc(dict_var)))
        return false;
    if (is_hashtable_pattern && is<IfInstruction>(block->instructions[i + 2])) {
        return block->instructions[i + 3]->match_branch(block_after_init);
    }
    return block->instructions[i + 2]->match_branch(block_after_init);
}

/// call Add(ldloc dictVar, ldstr value, ldc.i4 index)
/// -or-
/// call Add(ldloc dictVar, ldstr value, box System.Int32(ldc.i4 index))
bool SwitchOnStringTransform::match_add_call(const IType* dictionary_type, ILInstruction* inst, ILVariable* dict_var, int* index, std::string* value) {
    value->clear();
    *index = -1;
    auto* c = as<CallInstruction>(inst);
    if (!(c != nullptr && c->method->name() == "Add" && c->arguments().count() == 3)) return false;
    if (!c->arguments()[0]->match_ld_loc(dict_var)) return false;
    if (!c->arguments()[1]->match_ld_str(value)) {
        const IField* field = nullptr;
        if (c->arguments()[1]->match_lds_fld(&field) && typesystem::is_known_type(field->declaring_type(), KnownTypeCode::String) &&
            field->name() == "Empty") {
            *value = "";
        } else {
            return false;
        }
    }
    if (!(typesystem::type_equals(c->method->declaring_type(), dictionary_type) && !c->method->is_static())) return false;
    ILInstruction* arg = nullptr;
    const IType* ignored_type = nullptr;
    return (c->arguments()[2]->match_ldc_i4(index) || (c->arguments()[2]->match_box(&arg, &ignored_type) && arg->match_ldc_i4(index)));
}

bool SwitchOnStringTransform::is_string_to_int_dictionary(const IType* dictionary_type) {
    if (dictionary_type->full_name() != "System.Collections.Generic.Dictionary") return false;
    if (dictionary_type->type_arguments().size() != 2) return false;
    return typesystem::is_known_type(dictionary_type->type_arguments()[0], KnownTypeCode::String) &&
           typesystem::is_known_type(dictionary_type->type_arguments()[1], KnownTypeCode::Int32);
}

bool SwitchOnStringTransform::is_non_generic_hashtable(const IType* dictionary_type) {
    if (dictionary_type->full_name() != "System.Collections.Hashtable") return false;
    if (!dictionary_type->type_arguments().empty()) return false;
    return true;
}

bool SwitchOnStringTransform::match_legacy_switch_on_string_with_hashtable(Block* block, HashtableInitializer& hashtable_initializers, int& i) {
    // match first block: checking switch-value for null
    // stloc tmp(ldloc switch-value)
    // stloc switchVariable(ldloc tmp)
    // if (comp(ldloc tmp == ldnull)) br nullCaseBlock
    // br getItemBlock
    if (block->instructions.count() != i + 4) return false;
    ILVariable* tmp = nullptr;
    ILInstruction* switch_value = nullptr;
    if (!block->instructions[i]->match_st_loc(&tmp, &switch_value)) return false;
    ILVariable* switch_variable = nullptr;
    ILInstruction* tmp_load = nullptr;
    if (!block->instructions[i + 1]->match_st_loc(&switch_variable, &tmp_load) || !tmp_load->match_ld_loc(tmp)) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* null_case_block_branch = nullptr;
    if (!block->instructions[i + 2]->match_if_instruction(&condition, &null_case_block_branch)) return false;
    Block* get_item_block = nullptr;
    Block* null_case_block = nullptr;
    if (!block->instructions[i + 3]->match_branch(&get_item_block) ||
        !(null_case_block_branch->match_branch(&null_case_block) || is<Leave>(null_case_block_branch)))
        return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!(condition->match_comp_equals(&left, &right) && right->match_ld_null() && left->match_ld_loc(tmp))) return false;
    // match second block: get_Item on compiler-generated Hashtable
    // stloc tmp2(call get_Item(volatile.ldobj System.Collections.Hashtable(ldsflda $$method0x600003f - 1), ldloc switchVariable))
    // stloc switchVariable(ldloc tmp2)
    // if (comp(ldloc tmp2 == ldnull)) br defaultCaseBlock
    // br switchBlock
    if (get_item_block->incoming_edge_count() != 1 || get_item_block->instructions.count() != 4) return false;
    ILVariable* tmp2 = nullptr;
    ILInstruction* get_item = nullptr;
    if (!(get_item_block->instructions[0]->match_st_loc(&tmp2, &get_item) && is<Call>(get_item) && as<Call>(get_item)->method->name() == "get_Item"))
        return false;
    auto* get_item_call = as<Call>(get_item);
    ILVariable* switch_variable2 = nullptr;
    ILInstruction* tmp2_load = nullptr;
    if (!get_item_block->instructions[1]->match_st_loc(&switch_variable2, &tmp2_load) || !tmp2_load->match_ld_loc(tmp2)) return false;
    if (!ILVariableEqualityComparer{}(switch_variable, switch_variable2)) return false;
    ILInstruction* default_block_branch = nullptr;
    if (!get_item_block->instructions[2]->match_if_instruction(&condition, &default_block_branch)) return false;
    Block* switch_block = nullptr;
    Block* default_block = nullptr;
    if (!get_item_block->instructions[3]->match_branch(&switch_block) ||
        !(default_block_branch->match_branch(&default_block) || is<Leave>(default_block_branch)))
        return false;
    if (!(condition->match_comp_equals(&left, &right) && right->match_ld_null() && left->match_ld_loc(tmp2))) return false;
    const IField* dict_field = nullptr;
    const IType* ignored_type = nullptr;
    if (!(get_item_call->arguments().count() == 2 &&
          match_dictionary_field_load(get_item_call->arguments()[0], is_non_generic_hashtable, &dict_field, &ignored_type) &&
          get_item_call->arguments()[1]->match_ld_loc(switch_variable)))
        return false;
    // Check if there is a hashtable init block at the beginning of the method
    HashtableInitializerInfo* info = hashtable_initializers.find(dict_field);
    if (info == nullptr) return false;
    StringIndexList& string_values = info->labels;
    // match third block: switch-instruction block
    // switch (ldobj System.Int32(unbox System.Int32(ldloc switchVariable))) {
    // 	case [0..1): br caseBlock1
    //  ... more cases ...
    // 	case [long.MinValue..0),[13..long.MaxValue]: br defaultBlock
    // }
    if (switch_block->incoming_edge_count() != 1 || switch_block->instructions.count() != 1) return false;
    auto* switch_inst = as<SwitchInstruction>(switch_block->instructions[0]);
    ILInstruction* target = nullptr;
    const IType* ldobj_type = nullptr;
    ILInstruction* arg = nullptr;
    const IType* unbox_type = nullptr;
    if (!(switch_inst != nullptr && switch_inst->value()->match_ld_obj(&target, &ldobj_type) && target->match_unbox(&arg, &unbox_type) &&
          arg->match_ld_loc(switch_variable2) && typesystem::is_known_type(ldobj_type, KnownTypeCode::Int32) &&
          typesystem::type_equals(unbox_type, ldobj_type)))
        return false;
    SectionList sections;
    for (SwitchSection* section : switch_inst->sections) sections.push_back(section);
    // switch contains case null:
    if (!is<Leave>(null_case_block_branch) && null_case_block != default_block) {
        if (!add_null_section(sections, string_values, null_case_block)) {
            return false;
        }
    }
    context_->step("MatchLegacySwitchOnStringWithHashtable", block->instructions[i]);
    auto* string_to_int = make<StringToInt>(switch_value, string_values, context_->type_system().find_type(KnownTypeCode::String));
    auto* inst = make<SwitchInstruction>(string_to_int);
    inst->sections.add_range(sections);
    inst->add_il_range(*block->instructions[i]);
    block->instructions[i]->replace_with(inst);
    block->instructions.remove_range(i + 1, 3);
    context_->end_step(inst);
    info->transformed = true;
    return true;
}

bool SwitchOnStringTransform::find_hashtable_init_block(Block* entry_point, StringIndexList* string_values, const IField** dict_field,
                                                        Block** block_after_this_init_block, ILInstruction** this_switch_init_jump_inst,
                                                        ILInstruction** next_switch_init_jump_inst) {
    string_values->clear();
    *dict_field = nullptr;
    *block_after_this_init_block = nullptr;
    *next_switch_init_jump_inst = nullptr;
    *this_switch_init_jump_inst = nullptr;
    if (entry_point->instructions.count() != 2) return false;
    // match first block: checking compiler-generated Hashtable for null
    // if (comp(volatile.ldobj System.Collections.Hashtable(ldsflda $$method0x600003f-1) != ldnull)) br switchHeadBlock
    // br tableInitBlock
    ILInstruction* condition = nullptr;
    ILInstruction* branch_to_switch_head = nullptr;
    if (!(entry_point->instructions[0]->match_if_instruction(&condition, &branch_to_switch_head))) return false;
    Block* table_init_block = nullptr;
    if (!entry_point->instructions[1]->match_branch(&table_init_block)) return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    const IType* dictionary_type = nullptr;
    if (!(condition->match_comp_not_equals(&left, &right) && right->match_ld_null() &&
          match_dictionary_field_load(left, is_non_generic_hashtable, dict_field, &dictionary_type)))
        return false;
    Block* switch_head = nullptr;
    if (!branch_to_switch_head->match_branch(&switch_head)) return false;
    *this_switch_init_jump_inst = entry_point->instructions[0];
    // match second block: initialization of compiler-generated Hashtable
    // stloc table(newobj Hashtable..ctor(ldc.i4 capacity, ldc.f loadFactor))
    // call Add(ldloc table, ldstr value, box System.Int32(ldc.i4 index))
    // ... more calls to Add ...
    // volatile.stobj System.Collections.Hashtable(ldsflda $$method0x600003f - 1, ldloc table)
    // br switchHeadBlock
    if (table_init_block->incoming_edge_count() != 1 || table_init_block->instructions.count() < 3) return false;
    if (!extract_string_values_from_init_block(table_init_block, string_values, block_after_this_init_block, dictionary_type, *dict_field, true))
        return false;
    // if there is another IfInstruction before the end of the block, it might be a jump to the next hashtable init block.
    // if (comp(volatile.ldobj System.Collections.Hashtable(ldsflda $$method0x600003f-2) != ldnull)) br switchHeadBlock
    if (auto* next_hashtable_init_head = as<IfInstruction>(table_init_block->instructions.second_to_last_or_default())) {
        const IField* next_switch_init_field = nullptr;
        const IType* ignored_type = nullptr;
        if (!(next_hashtable_init_head->condition()->match_comp_not_equals(&left, &right) && right->match_ld_null() &&
              match_dictionary_field_load(left, is_non_generic_hashtable, &next_switch_init_field, &ignored_type)))
            return false;
        if (!next_hashtable_init_head->true_inst()->match_branch(switch_head)) return false;
        *next_switch_init_jump_inst = next_hashtable_init_head;
    }
    return true;
}

bool SwitchOnStringTransform::match_roslyn_switch_on_string(InstructionCollection<ILInstruction>& instructions, int& i) {
    if (i >= instructions.count() - 1) return false;
    // stloc switchValueVar(switchValue)
    // if (comp(ldloc switchValueVar == ldnull)) br nullCase
    // br nextBlock
    InstructionCollection<ILInstruction>* switch_block_instructions = &instructions;
    int switch_block_instructions_offset = i;
    Block* null_value_case_block = nullptr;
    ILInstruction* inst_for_null_check = nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* exit_block_jump = nullptr;
    if (instructions[i]->match_if_instruction(&condition, &exit_block_jump) && condition->match_comp_equals_null(&inst_for_null_check)) {
        auto* next_block_jump = as<Branch>(instructions[i + 1]);
        if (next_block_jump == nullptr || next_block_jump->target_block()->incoming_edge_count() != 1) return false;
        if (!exit_block_jump->match_branch(&null_value_case_block)) return false;
        switch_block_instructions = &next_block_jump->target_block()->instructions;
        switch_block_instructions_offset = 0;
    }
    // stloc switchValueVar(call ComputeStringHash(switchValueLoad))
    // switch (ldloc switchValueVar) {
    // 	case [211455823..211455824): br caseBlock1
    //  ... more cases ...
    // 	case [long.MinValue..-365098645),...,[1697255802..long.MaxValue]: br defaultBlock
    // }
    SwitchInstruction* switch_inst = nullptr;
    ILVariable* switch_value_var = nullptr;
    LdLoc* switch_value_load = nullptr;
    if (!(switch_block_instructions_offset + 1 < switch_block_instructions->count() &&
          (switch_inst = as<SwitchInstruction>((*switch_block_instructions)[switch_block_instructions_offset + 1])) != nullptr &&
          switch_inst->value()->match_ld_loc(&switch_value_var) &&
          match_compute_string_or_read_only_span_hash_call((*switch_block_instructions)[switch_block_instructions_offset], switch_value_var,
                                                           &switch_value_load))) {
        return false;
    }

    if (inst_for_null_check != nullptr && !inst_for_null_check->match_ld_loc(switch_value_load->variable())) {
        return false;
    }

    StringTargetList string_values;
    SwitchSection* default_section = switch_inst->get_default_section();
    Block* exit_or_default_block = nullptr;
    BlockContainer* ignored_container = nullptr;
    if (!(default_section->body()->match_branch(&exit_or_default_block) || default_section->body()->match_leave(&ignored_container))) return false;
    for (SwitchSection* section : switch_inst->sections) {
        if (section == default_section) continue;
        // extract target block
        Block* target = nullptr;
        if (!section->body()->match_branch(&target)) return false;
        OptString string_value;
        bool empty_string_equals_null = false;
        ILInstruction* target_or_leave = nullptr;
        Block* current_exit_block = nullptr;
        if (match_roslyn_empty_string_case_block_head(target, switch_value_load->variable(), &target_or_leave, &current_exit_block)) {
            string_value = std::string();
            empty_string_equals_null = false;
        } else if (!match_roslyn_case_block_head(target, switch_value_load->variable(), &target_or_leave, &current_exit_block, &string_value,
                                                 &empty_string_equals_null)) {
            return false;
        }

        if (current_exit_block != exit_or_default_block) return false;
        if (empty_string_equals_null && is_null_or_empty(string_value)) {
            string_values.emplace_back(std::nullopt, target_or_leave);
            string_values.emplace_back(std::string(), target_or_leave);
        } else {
            string_values.emplace_back(string_value, target_or_leave);
        }
    }

    if (null_value_case_block != nullptr && exit_or_default_block != null_value_case_block) {
        string_values.emplace_back(std::nullopt, null_value_case_block);
    }
    // In newer Roslyn versions (>=3.7) the null check appears in the default case, not prior to the switch.
    ILInstruction* exit_or_default = exit_or_default_block;
    bool has_null_value = std::any_of(string_values.begin(), string_values.end(), [](const auto& pair) { return !pair.first.has_value(); });
    if (!has_null_value && is_null_check_in_default_block(&exit_or_default, switch_value_load->variable(), &null_value_case_block)) {
        string_values.emplace_back(std::nullopt, null_value_case_block);
    }
    exit_or_default_block = static_cast<Block*>(exit_or_default);

    context_->step("MatchRoslynSwitchOnString", switch_value_load);
    if (exit_or_default_block != nullptr) {
        // change TargetBlock in case it was modified by IsNullCheckInDefaultBlock()
        checked_cast<Branch>(default_section->body())->set_target_block(exit_or_default_block);
    }
    ILInstruction* switch_value_inst = switch_value_load;

    auto replace_with_switch_instruction = [&](int offset) {
        LongSet default_label = LongSet(LongInterval(0, static_cast<std::int64_t>(string_values.size()))).invert();
        std::vector<OptString> values(string_values.size());
        SectionList sections(string_values.size(), nullptr);
        for (std::size_t idx = 0; idx < string_values.size(); idx++) {
            values[idx] = string_values[idx].first;
            ILInstruction* body_instruction = string_values[idx].second;
            ILInstruction* body = body_instruction;
            if (auto* b = as<Block>(body_instruction)) body = make<Branch>(b);
            auto* section = make<SwitchSection>();
            section->set_labels(LongSet(static_cast<std::int64_t>(idx)));
            section->set_body(body);
            sections[idx] = section;
        }
        auto* new_switch = make<SwitchInstruction>(make<StringToInt>(switch_value_inst, values, switch_value_load->variable()->type()));
        new_switch->sections.add_range(sections);
        auto* default_new_section = make<SwitchSection>();
        default_new_section->set_labels(default_label);
        default_new_section->set_body(default_section->body());
        default_new_section->set_is_compiler_generated_default_section(default_section->is_compiler_generated_default_section());
        new_switch->sections.add(default_new_section);
        instructions[offset]->replace_with(new_switch);
        context_->end_step(new_switch);
        return new_switch;
    };

    if (&instructions == switch_block_instructions) {
        // stloc switchValueLoadVariable(switchValue)
        // stloc switchValueVar(call ComputeStringHash(ldloc switchValueLoadVariable))
        // switch (ldloc switchValueVar) {
        bool keep_assignment_before;
        // if the switchValueLoad.Variable is only used in the compiler generated case equality checks, we can remove it.
        ILInstruction* switch_value_tmp = nullptr;
        if (i >= 1 && instructions[i - 1]->match_st_loc(switch_value_load->variable(), &switch_value_tmp) &&
            switch_value_load->variable()->is_single_definition() && switch_value_load->variable()->load_count() == switch_inst->sections.count()) {
            switch_value_inst = switch_value_tmp;
            keep_assignment_before = false;
        } else {
            keep_assignment_before = true;
        }
        // replace stloc switchValueVar(call ComputeStringHash(...)) with new switch instruction
        SwitchInstruction* new_switch = replace_with_switch_instruction(i);
        // remove old switch instruction
        new_switch->add_il_range(*instructions[i + 1]);
        instructions.remove_at(i + 1);
        // remove extra assignment
        if (!keep_assignment_before) {
            new_switch->add_il_range(*instructions[i - 1]);
            instructions.remove_range(i - 1, 1);
            i -= 1;
        }
    } else {
        bool keep_assignment_before;
        // if the switchValueLoad.Variable is only used in the compiler generated case equality checks, we can remove it.
        ILVariable* temporary = nullptr;
        ILInstruction* temporary_value = nullptr;
        ILInstruction* temp_load = nullptr;
        if (i >= 2 && instructions[i - 2]->match_st_loc(&temporary, &temporary_value) &&
            instructions[i - 1]->match_st_loc(switch_value_load->variable(), &temp_load) && temp_load->match_ld_loc(temporary)) {
            switch_value_inst = temporary_value;
            keep_assignment_before = false;
        } else {
            keep_assignment_before = true;
        }
        // replace null check with new switch instruction
        SwitchInstruction* new_switch = replace_with_switch_instruction(i);
        new_switch->add_il_range(*switch_inst);
        // remove jump instruction to switch block
        new_switch->add_il_range(*instructions[i + 1]);
        instructions.remove_at(i + 1);
        // remove extra assignment
        if (!keep_assignment_before) {
            new_switch->add_il_range(*instructions[i - 2]);
            instructions.remove_range(i - 2, 2);
            i -= 2;
        }
    }

    return true;
}

bool SwitchOnStringTransform::match_roslyn_switch_on_string_using_length_and_char(Block* block, int i) {
    auto& instructions = block->instructions;
    // implements https://github.com/dotnet/roslyn/pull/66081
    // if (comp(ldloc switchValueVar == ldnull)) br nullCase
    // br nextBlock
    Block* switch_on_length_block = nullptr;
    int switch_on_length_block_start_offset = 0;
    Block* null_case = nullptr;
    ILInstruction* default_case = nullptr;
    bool default_is_compiler_generated = false;
    ILVariable* switch_value_var = nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* exit_block_jump = nullptr;
    ILInstruction* ldloc = nullptr;
    if (instructions[i]->match_if_instruction(&condition, &exit_block_jump) && condition->match_comp_equals_null(&ldloc) && is<LdLoc>(ldloc)) {
        switch_value_var = as<LdLoc>(ldloc)->variable();
        Block* next_block = nullptr;
        if (!instructions[i + 1]->match_branch(&next_block)) return false;
        BlockContainer* ignored_container = nullptr;
        if (!exit_block_jump->match_branch(&null_case) && !exit_block_jump->match_leave(&ignored_container)) return false;
        // if (comp(ldloc switchValueVar == ldnull)) br ...
        // br switchOnLengthBlock
        if (next_block->incoming_edge_count() == 1 && next_block->instructions[0]->match_if_instruction(&condition, &exit_block_jump) &&
            condition->match_comp_equals_null(&ldloc) && ldloc->match_ld_loc(switch_value_var)) {
            if (!next_block->instructions[1]->match_branch(&switch_on_length_block)) return false;
        } else {
            switch_on_length_block = next_block;
        }
        if (switch_on_length_block->incoming_edge_count() != 1) return false;
        switch_on_length_block_start_offset = 0;
    } else {
        switch_on_length_block = block;
        switch_value_var = nullptr;  // will be extracted in MatchSwitchOnLengthBlock
        switch_on_length_block_start_offset = i;
    }

    using SectionPairList = std::vector<std::pair<LongSet, ILInstruction*>>;

    auto inherit_compiler_generated_default_marker = [&](SwitchInstruction* inner) {
        // The char switches and the switch on the string length share the throw-helper
        // block of the implicit default case; whichever of them was processed first by
        // SwitchDetection carries the compiler-generated marker for it.
        for (SwitchSection* s : inner->sections) {
            Block* target = nullptr;
            if (s->is_compiler_generated_default_section() && s->body()->match_branch(&target) && target == default_case) {
                default_is_compiler_generated = true;
                break;
            }
        }
    };

    auto match_get_chars = [&](ILInstruction* instruction, ILVariable* value_var, int* index) {
        *index = -1;
        ILInstruction* target = nullptr;
        const IType* type = nullptr;
        if (context_->settings().switch_on_read_only_span_char && instruction->match_ld_obj(&target, &type) &&
            typesystem::is_known_type(type, KnownTypeCode::UInt16)) {
            auto* call = as<Call>(target);
            return call != nullptr &&
                   (typesystem::full_name_is(call->method, "System.ReadOnlySpan", "get_Item") ||
                    typesystem::full_name_is(call->method, "System.Span", "get_Item")) &&
                   call->arguments().count() == 2 && call->arguments()[0]->match_ld_loca(value_var) &&
                   call->arguments()[1]->match_ldc_i4(index) && *index >= 0;
        } else {
            auto* call = as<Call>(instruction);
            return call != nullptr && typesystem::full_name_is(call->method, "System.String", "get_Chars") && call->arguments().count() == 2 &&
                   call->arguments()[0]->match_ld_loc(value_var) && call->arguments()[1]->match_ldc_i4(index) && *index >= 0;
        }
    };

    auto match_switch_on_char_block = [&](Block* char_block, int length, ILVariable* value_var, StringTargetList* results) {
        results->clear();
        if (char_block->incoming_edge_count() != 1) return false;
        SwitchInstruction* switch_ = nullptr;
        SectionPairList sections;
        int index = 0;
        ILVariable* char_temp_var = nullptr;
        ILInstruction* get_chars_call = nullptr;
        switch (char_block->instructions.count()) {
        case 1:
            switch_ = as<SwitchInstruction>(char_block->instructions[0]);
            if (switch_ == nullptr) return false;
            if (!match_get_chars(switch_->value(), value_var, &index)) return false;
            inherit_compiler_generated_default_marker(switch_);
            for (SwitchSection* s : switch_->sections) sections.emplace_back(s->labels(), s->body());
            break;
        case 2:
            if (!char_block->instructions[0]->match_st_loc(&char_temp_var, &get_chars_call)) return false;
            if (!match_get_chars(get_chars_call, value_var, &index)) return false;
            switch_ = as<SwitchInstruction>(char_block->instructions[1]);
            if (switch_ == nullptr) return false;
            if (!switch_->value()->match_ld_loc(char_temp_var)) return false;
            inherit_compiler_generated_default_marker(switch_);
            for (SwitchSection* s : switch_->sections) sections.emplace_back(s->labels(), s->body());
            break;
        default:
            if (!analysis_.analyze_block(char_block)) {
                return false;
            }
            if (!char_block->instructions[0]->match_st_loc(&char_temp_var, &get_chars_call)) return false;
            if (!match_get_chars(get_chars_call, value_var, &index)) return false;
            if (analysis_.switch_variable() != char_temp_var) return false;
            sections = analysis_.sections;
            break;
        }
        if (index >= length) return false;
        bool has_default_section = false;
        for (auto& [labels, body] : sections) {
            if (labels.count() == 1) {
                char16_t ch = static_cast<char16_t>(labels.values().at(0));
                Block* target_block = nullptr;
                if (!body->match_branch(&target_block)) return false;
                if (length == 1) {
                    results->emplace_back(utf16_unit_to_string(ch), target_block);
                } else {
                    for (;;) {
                        // upstream throws a NullReferenceException for a case chain that leaves the switch
                        if (target_block == nullptr) throw UsageError("Object reference not set to an instance of an object.");
                        ILInstruction* body_or_leave = nullptr;
                        Block* exit = nullptr;
                        OptString string_value;
                        bool ignored_flag = false;
                        if (!match_roslyn_case_block_head(target_block, value_var, &body_or_leave, &exit, &string_value, &ignored_flag)) break;
                        std::u16string utf16 = to_utf16(string_value.value_or(std::string()));
                        if (static_cast<int>(utf16.size()) != length || utf16.at(static_cast<std::size_t>(index)) != ch) return false;
                        results->emplace_back(string_value, body_or_leave);
                        if (exit == null_case) break;
                        target_block = exit;
                    }
                }
            } else if (!has_default_section) {
                has_default_section = true;
            } else {
                return false;
            }
        }
        return !results->empty();
    };

    auto match_switch_on_length_block = [&](ILVariable** value_var, Block* length_block, int start_offset, SectionPairList* blocks) {
        blocks->clear();
        SwitchInstruction* switch_ = nullptr;
        ILInstruction* get_length_call = nullptr;
        ILVariable* length_var = nullptr;
        switch (length_block->instructions.count() - start_offset) {
        case 1:
            switch_ = as<SwitchInstruction>(length_block->instructions[start_offset]);
            if (switch_ == nullptr) return false;
            get_length_call = switch_->value();
            break;
        case 2:
            if (!length_block->instructions[start_offset]->match_st_loc(&length_var, &get_length_call)) return false;
            switch_ = as<SwitchInstruction>(length_block->instructions[start_offset + 1]);
            if (switch_ == nullptr) return false;
            if (!switch_->value()->match_ld_loc(length_var)) return false;
            break;
        case 3: {
            switch_ = nullptr;
            if (!length_block->instructions[start_offset]->match_st_loc(&length_var, &get_length_call)) return false;
            ILInstruction* cond = nullptr;
            ILInstruction* goto_length = nullptr;
            if (!length_block->instructions[start_offset + 1]->match_if_instruction(&cond, &goto_length)) return false;
            Block* target = nullptr;
            if (!goto_length->match_branch(&target)) return false;
            Block* goto_else = nullptr;
            if (!length_block->instructions[start_offset + 2]->match_branch(&goto_else)) return false;
            ILInstruction* lhs = nullptr;
            ILInstruction* rhs = nullptr;
            if (!cond->match_comp_equals(&lhs, &rhs)) {
                if (!cond->match_comp_not_equals(&lhs, &rhs)) return false;
                Block* t = target;
                target = goto_else;
                goto_else = t;
            }
            default_case = goto_else;
            int length = 0;
            if (!lhs->match_ld_loc(length_var) || !rhs->match_ldc_i4(&length)) return false;

            blocks->emplace_back(LongSet(static_cast<std::int64_t>(length)), target);
            blocks->emplace_back(LongSet(static_cast<std::int64_t>(length)).invert(), default_case);
            break;
        }
        default:
            return false;
        }
        auto* call = as<Call>(get_length_call);
        if (call == nullptr || call->arguments().count() != 1 || call->method->name() != "get_Length") {
            return false;
        }
        const typesystem::ITypeDefinition* declaring_type_definition = call->method->declaring_type_definition();
        // DeclaringTypeDefinition?.KnownTypeCode: None when there is no definition
        KnownTypeCode declaring_type_code = declaring_type_definition != nullptr ? declaring_type_definition->known_type_code() : KnownTypeCode::None;
        switch (declaring_type_code) {
        case KnownTypeCode::String:
            if (!call->arguments()[0]->match_ld_loc(*value_var)) return false;
            break;
        case KnownTypeCode::ReadOnlySpanOfT:
        case KnownTypeCode::SpanOfT:
            if (!context_->settings().switch_on_read_only_span_char) return false;
            if (!call->arguments()[0]->match_ld_loca(value_var)) return false;
            break;
        default:
            return false;
        }
        if (switch_ == nullptr) return true;
        blocks->clear();
        for (SwitchSection* section : switch_->sections) {
            if (section->has_null_label()) return false;
            Block* target = nullptr;
            BlockContainer* ignored_container = nullptr;
            if (!section->body()->match_branch(&target) && !section->body()->match_leave(&ignored_container)) return false;
            ILInstruction* target_inst = target != nullptr ? static_cast<ILInstruction*>(target) : section->body();
            if (section->labels().count() != 1) {
                if (default_case == nullptr) default_case = target_inst;
                if (default_case != target_inst) return false;
                // The default section of the switch on the string length carries the
                // compiler-generated marker if an implicit-default throw helper was
                // already inlined there; transfer it to the rebuilt switch.
                default_is_compiler_generated |= section->is_compiler_generated_default_section();
            } else {
                blocks->emplace_back(section->labels(), target_inst);
            }
        }
        return true;
    };

    SectionPairList blocks_by_length;
    if (!match_switch_on_length_block(&switch_value_var, switch_on_length_block, switch_on_length_block_start_offset, &blocks_by_length)) return false;
    StringTargetList string_values;
    for (auto& b : blocks_by_length) {
        if (b.first.count() != 1) {
            if (b.second != null_case) return false;
        } else {
            int length = static_cast<int>(b.first.intervals()[0].start);
            if (is<Leave>(b.second)) {
                // nothing to do
            } else if (auto* target_block = as<Block>(b.second)) {
                StringTargetList mapping;
                ILInstruction* body_or_leave = nullptr;
                Block* exit = nullptr;
                OptString string_value;
                bool ignored_flag = false;
                if (match_switch_on_char_block(target_block, length, switch_value_var, &mapping)) {
                    for (auto& item : mapping) {
                        bool known = std::any_of(string_values.begin(), string_values.end(), [&](const auto& x) { return x.first == item.first; });
                        if (!known) {
                            string_values.push_back(item);
                        } else {
                            return false;
                        }
                    }
                } else if (match_roslyn_case_block_head(target_block, switch_value_var, &body_or_leave, &exit, &string_value, &ignored_flag)) {
                    if (exit != default_case) return false;
                    bool known = std::any_of(string_values.begin(), string_values.end(), [&](const auto& x) { return x.first == string_value; });
                    if (!known) {
                        string_values.emplace_back(string_value, body_or_leave);
                    } else {
                        return false;
                    }
                } else if (length == 0) {
                    string_values.emplace_back(std::string(), b.second);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    }

    {
        bool has_null_value = std::any_of(string_values.begin(), string_values.end(), [](const auto& pair) { return !pair.first.has_value(); });
        if (!has_null_value) {
            Block* null_block = nullptr;
            if (is_null_check_in_default_block(&default_case, switch_value_var, &null_block)) {
                string_values.emplace_back(std::nullopt, null_block);
            } else if (null_case != nullptr && null_case != default_case) {
                string_values.emplace_back(std::nullopt, null_case);
            }
        }
    }

    context_->step("MatchRoslynSwitchOnStringUsingLengthAndChar", instructions[i]);
    LongSet default_label = LongSet(LongInterval(0, static_cast<std::int64_t>(string_values.size()))).invert();
    std::vector<OptString> values(string_values.size());
    SectionList sections(string_values.size(), nullptr);
    for (std::size_t idx = 0; idx < string_values.size(); idx++) {
        values[idx] = string_values[idx].first;
        ILInstruction* body_instruction = string_values[idx].second;
        ILInstruction* body = body_instruction;
        if (auto* b = as<Block>(body_instruction)) body = make<Branch>(b);
        auto* section = make<SwitchSection>();
        section->set_labels(LongSet(static_cast<std::int64_t>(idx)));
        section->set_body(body);
        sections[idx] = section;
    }
    auto* new_switch = make<SwitchInstruction>(make<StringToInt>(make<LdLoc>(switch_value_var), values, switch_value_var->type()));
    new_switch->sections.add_range(sections);
    auto* default_section = make<SwitchSection>();
    default_section->set_labels(default_label);
    if (auto* b2 = as<Block>(default_case))
        default_section->set_body(make<Branch>(b2));
    else
        default_section->set_body(default_case);
    default_section->set_is_compiler_generated_default_section(default_is_compiler_generated);
    new_switch->sections.add(default_section);
    new_switch->add_il_range(*instructions[i]);
    if (null_case != nullptr) {
        new_switch->add_il_range(*instructions[i + 1]);
    }
    instructions.set(i, new_switch);
    instructions.remove_range(i + 1, instructions.count() - (i + 1));
    context_->end_step(new_switch);
    return true;
}

/// Matches:
/// Block oldDefaultBlock (incoming: 1) {
///     if (comp.o(ldloc switchVar == ldnull)) br nullValueCaseBlock
///     br newDefaultBlock
/// }
bool SwitchOnStringTransform::is_null_check_in_default_block(ILInstruction** exit_or_default, ILVariable* switch_var, Block** null_value_case_block) {
    *null_value_case_block = nullptr;
    auto* exit_or_default_block = as<Block>(*exit_or_default);
    if (exit_or_default_block == nullptr) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* then_branch = nullptr;
    if (!exit_or_default_block->instructions[0]->match_if_instruction(&condition, &then_branch)) return false;
    ILInstruction* arg = nullptr;
    if (!(condition->match_comp_equals_null(&arg) && arg->match_ld_loc(switch_var))) return false;
    if (!then_branch->match_branch(null_value_case_block)) return false;
    if ((*null_value_case_block)->parent() != exit_or_default_block->parent()) return false;
    Block* else_block = nullptr;
    if (!exit_or_default_block->instructions[1]->match_branch(&else_block)) return false;
    if (else_block->parent() != exit_or_default_block->parent()) return false;
    *exit_or_default = else_block;
    return true;
}

/// Matches (and the negated version):
/// if (call op_Equality(ldloc switchValueVar, stringValue)) br body
/// br exit
bool SwitchOnStringTransform::match_roslyn_case_block_head(Block* target, ILVariable* switch_value_var, ILInstruction** body_or_leave,
                                                           Block** default_or_exit_block, OptString* string_value, bool* empty_string_equals_null) {
    *body_or_leave = nullptr;
    *default_or_exit_block = nullptr;
    *string_value = std::nullopt;
    *empty_string_equals_null = false;
    if (target->instructions.count() != 2) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* body_branch = nullptr;
    if (!target->instructions[0]->match_if_instruction(&condition, &body_branch)) {
        // Special case: sometimes we don't have an if, because bodyBranch==exitBranch
        // and the C# compiler optimized out the if.
        // Example:
        //  Block IL_0063 (incoming: 1) {
        //   call op_Equality(ldloc V_4, ldstr "rowno")
        //   leave IL_0000(nop)
        // }
        condition = target->instructions[0];
        body_branch = target->instructions[1];
    }
    ILInstruction* exit_branch = target->instructions[1];
    // Handle negated conditions first:
    ILInstruction* expr = nullptr;
    while (condition->match_logic_not(&expr)) {
        std::swap(exit_branch, body_branch);
        condition = expr;
    }
    bool is_vb_compare_string = false;
    if (!match_string_equality_comparison(condition, switch_value_var, string_value, &is_vb_compare_string)) return false;
    if (is_vb_compare_string) {
        std::swap(exit_branch, body_branch);
        *empty_string_equals_null = true;
    }
    BlockContainer* ignored_container = nullptr;
    if (!(exit_branch->match_branch(default_or_exit_block) || exit_branch->match_leave(&ignored_container))) return false;
    if (body_branch->match_leave(&ignored_container)) {
        *body_or_leave = body_branch;
        return true;
    }
    Block* body_block = nullptr;
    if (body_branch->match_branch(&body_block)) {
        *body_or_leave = body_block;
        return true;
    }
    return false;
}

/// Block target(incoming: 1) {
/// 	if (comp.o(ldloc switchValueVar == ldnull)) br exit
/// 	br lengthCheckBlock
/// }
///
/// Block lengthCheckBlock(incoming: 1) {
/// 	if (logic.not(call get_Length(ldloc switchValueVar))) br body
/// 	br exit
/// }
bool SwitchOnStringTransform::match_roslyn_empty_string_case_block_head(Block* target, ILVariable* switch_value_var, ILInstruction** body_or_leave,
                                                                        Block** default_or_exit_block) {
    *body_or_leave = nullptr;
    *default_or_exit_block = nullptr;
    if (target->instructions.count() != 2 || target->incoming_edge_count() != 1) return false;
    ILInstruction* null_comparison_condition = nullptr;
    ILInstruction* exit_branch = nullptr;
    if (!target->instructions[0]->match_if_instruction(&null_comparison_condition, &exit_branch)) return false;
    ILInstruction* arg = nullptr;
    if (!null_comparison_condition->match_comp_equals_null(&arg) || !arg->match_ld_loc(switch_value_var)) return false;
    Block* length_check_block = nullptr;
    if (!target->instructions[1]->match_branch(&length_check_block)) return false;
    if (length_check_block->instructions.count() != 2 || length_check_block->incoming_edge_count() != 1) return false;
    ILInstruction* length_check_condition = nullptr;
    ILInstruction* exit_branch2 = nullptr;
    if (!length_check_block->instructions[0]->match_if_instruction(&length_check_condition, &exit_branch2)) return false;
    ILInstruction* body_branch = nullptr;
    if (length_check_condition->match_logic_not(&arg)) {
        body_branch = exit_branch2;
        exit_branch2 = length_check_block->instructions[1];
        length_check_condition = arg;
    } else {
        body_branch = length_check_block->instructions[1];
    }
    BlockContainer* ignored_container = nullptr;
    if (!(exit_branch2->match_branch(default_or_exit_block) || exit_branch2->match_leave(&ignored_container))) return false;
    if (!match_string_length_call(length_check_condition, switch_value_var)) return false;
    if (!exit_branch->match(exit_branch2).success()) return false;
    if (body_branch->match_leave(&ignored_container)) {
        *body_or_leave = body_branch;
        return true;
    }
    Block* body_block = nullptr;
    if (body_branch->match_branch(&body_block)) {
        *body_or_leave = body_block;
        return true;
    }
    return false;
}

/// call get_Length(ldloc switchValueVar)
bool SwitchOnStringTransform::match_string_length_call(ILInstruction* inst, ILVariable* switch_value_var) {
    auto* call = as<Call>(inst);
    return call != nullptr && typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::String) && call->method->is_accessor() &&
           call->method->accessor_kind() == typesystem::MethodSemanticsAttributes::Getter && call->method->accessor_owner()->name() == "Length" &&
           call->arguments().count() == 1 && call->arguments()[0]->match_ld_loc(switch_value_var);
}

bool SwitchOnStringTransform::match_compute_string_or_read_only_span_hash_call(ILInstruction* inst, ILVariable* target_var, LdLoc** switch_value) {
    *switch_value = nullptr;
    ILInstruction* value = nullptr;
    if (!inst->match_st_loc(target_var, &value)) return false;
    auto* c = as<Call>(value);
    if (c != nullptr && c->arguments().count() == 1 &&
        (c->method->name() == "ComputeStringHash" || c->method->name() == "ComputeSpanHash" || c->method->name() == "ComputeReadOnlySpanHash") &&
        is_compiler_generated_or_is_in_compiler_generated_class(c->method)) {
        auto* ldloc = as<LdLoc>(c->arguments()[0]);
        if (ldloc == nullptr) return false;
        *switch_value = ldloc;
        return true;
    }
    return false;
}

/// Matches 'call string.op_Equality(ldloc(variable), ldstr stringValue)'
///      or 'comp(ldloc(variable) == ldnull)'
///      or 'call SequenceEqual(ldloc variable, call AsSpan(ldstr stringValue))'
bool SwitchOnStringTransform::match_string_equality_comparison(ILInstruction* condition, ILVariable* variable, OptString* string_value,
                                                               bool* is_vb_compare_string) {
    ILVariable* v = nullptr;
    return match_string_equality_comparison(condition, &v, string_value, is_vb_compare_string) && v == variable;
}

/// Matches 'call string.op_Equality(ldloc(variable), ldstr stringValue)'
///      or 'comp(ldloc(variable) == ldnull)'
///      or 'call SequenceEqual(ldloc variable, call AsSpan(ldstr stringValue))'
bool SwitchOnStringTransform::match_string_equality_comparison(ILInstruction* condition, ILVariable** variable, OptString* string_value,
                                                               bool* is_vb_compare_string) {
    *string_value = std::nullopt;
    *variable = nullptr;
    *is_vb_compare_string = false;
    while (auto* comp = as<Comp>(condition)) {
        if (!(comp->kind() == ComparisonKind::Inequality && comp->right()->match_ldc_i4(0))) break;
        // if (x != 0) == if (x)
        condition = comp->left();
    }
    if (auto* c = as<Call>(condition)) {
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (c->method->is_operator() && c->method->name() == "op_Equality" &&
            typesystem::is_known_type(c->method->declaring_type(), KnownTypeCode::String) && c->arguments().count() == 2) {
            left = c->arguments()[0];
            right = c->arguments()[1];
        } else if (c->method->is_static() && c->method->name() == "CompareString" &&
                   c->method->declaring_type()->full_name() == "Microsoft.VisualBasic.CompilerServices.Operators" && c->arguments().count() == 3) {
            left = c->arguments()[0];
            right = c->arguments()[1];
            // VB CompareString(): return 0 on equality -> condition is effectively negated.
            // Also, the empty string is considered equal to null.
            *is_vb_compare_string = true;
            if (!c->arguments()[2]->match_ldc_i4(0)) {
                // Option Compare Text: case insensitive comparison is not supported in C#
                return false;
            }
        } else if (c->method->is_static() && c->method->name() == "SequenceEqual" &&
                   c->method->declaring_type()->full_name() == "System.MemoryExtensions" && c->arguments().count() == 2) {
            left = c->arguments()[0];
            auto* as_span_call = as<Call>(c->arguments()[1]);
            if (as_span_call != nullptr && as_span_call->method->is_static() && as_span_call->method->name() == "AsSpan" &&
                as_span_call->method->declaring_type()->full_name() == "System.MemoryExtensions" && as_span_call->arguments().count() == 1) {
                right = as_span_call->arguments()[0];
            } else {
                return false;
            }
        } else {
            return false;
        }
        std::string text;
        if (!(left->match_ld_loc(variable) && right->match_ld_str(&text))) return false;
        *string_value = text;
        return true;
    } else if (ILInstruction* arg = nullptr; condition->match_comp_equals_null(&arg)) {
        *string_value = std::nullopt;
        return arg->match_ld_loc(variable);
    } else {
        return false;
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
