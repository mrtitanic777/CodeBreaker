// dotnet/decompiler/il/transforms/switch_on_string_transform.hpp -- detects the switch-on-string patterns the C# (and Visual Basic) compilers
// emit and turns each into one ILAst switch over a `string.to.int` of the switched value: cascading `string == "x"` ifs, the C# 1
// String.IsInterned chain, the C# 2 compiler-generated Dictionary<string, int> and Hashtable lookups, Roslyn's ComputeStringHash switch and
// its length and character switches, with the span flavours.
//
//     if (call op_Equality(ldloc s, ldstr "a")) ...           switch (string.to.int(ldloc s) { "a": 0, "b": 1, ... }) {
//     ... a chain of at least three string comparisons  =>      case [0..1): br caseA  ...
//
//     SwitchOnStringTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: SwitchOnStringTransform, from SwitchOnStringTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). A C# `string` that may be null is a std::optional<std::string>
// here (null is the null case of the switch), and the lengths and characters Roslyn's character switches count are UTF-16 code units.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/control_flow/switch_analysis.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class SwitchOnStringTransform final : public IILTransform {
public:
    // A string that may be null.
    using OptString = std::optional<std::string>;
    // List<(string, int)>: the strings of a compiler-generated dictionary with their indices.
    using StringIndexList = std::vector<std::pair<OptString, int>>;
    // List<(string, ILInstruction)>: a case's string with the block or leave it jumps to.
    using StringTargetList = std::vector<std::pair<OptString, ILInstruction*>>;
    using SectionList = std::vector<SwitchSection*>;

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "SwitchOnStringTransform"; }

    // Matches `stloc targetVar(call ComputeStringHash(ldloc switchValue))`, or the ComputeSpanHash and ComputeReadOnlySpanHash forms.
    static bool match_compute_string_or_read_only_span_hash_call(ILInstruction* inst, ILVariable* target_var, LdLoc** switch_value);

private:
    // What is known of one compiler-generated Hashtable initializer block (upstream's tuple in a Dictionary<IField, ...>).
    struct HashtableInitializerInfo {
        StringIndexList labels;
        IfInstruction* jump_to_next = nullptr;
        Block* containing_block = nullptr;
        Block* previous = nullptr;
        Block* next = nullptr;
        bool transformed = false;
    };
    // A Dictionary<IField, HashtableInitializerInfo>: walked in the order the entries were added.
    class HashtableInitializer {
    public:
        // false (and no change) when a field equal to `field` is in already
        bool add(const IField* field, HashtableInitializerInfo info);
        HashtableInitializerInfo* find(const IField* field);
        int count() const { return static_cast<int>(entries_.size()); }
        std::vector<std::pair<const IField*, HashtableInitializerInfo>>& entries() { return entries_; }

    private:
        std::vector<std::pair<const IField*, HashtableInitializerInfo>> entries_;
    };
    using TypeMatcher = std::function<bool(const IType*)>;

    HashtableInitializer scan_hashtable_initializer_blocks(Block* entry_point);
    bool simplify_cascading_if_statements(InstructionCollection<ILInstruction>& instructions, int& i);
    static bool validate_uses_of_switch_value_variable(ILVariable* switch_value_var, const std::vector<Block*>& case_blocks);
    bool simplify_csharp1_cascading_if_statements(InstructionCollection<ILInstruction>& instructions, int& i);
    static bool is_is_interned_call(Call* call, ILInstruction** argument);
    ILInstruction* match_case_block(Block* current_block, ILVariable* switch_variable, OptString* value, bool* empty_string_equals_null,
                                    ILInstruction** case_block_or_leave);
    bool match_legacy_switch_on_string_with_dict(InstructionCollection<ILInstruction>& instructions, int& i);
    static bool fix_cases_without_value(SectionList& sections, const StringIndexList& string_values);
    static bool add_null_section(SectionList& sections, StringIndexList& string_values, Block* null_value_case_block);
    static bool add_null_section(SectionList& sections, StringIndexList& string_values, ILInstruction* body);
    static bool match_dictionary_field_load(ILInstruction* inst, const TypeMatcher& type_matcher, const IField** dict_field,
                                            const IType** dictionary_type);
    static bool extract_string_values_from_init_block(Block* block, StringIndexList* values, Block** block_after_init, const IType* dictionary_type,
                                                      const IField* dictionary_field, bool is_hashtable_pattern);
    static bool match_add_call(const IType* dictionary_type, ILInstruction* inst, ILVariable* dict_var, int* index, std::string* value);
    static bool is_string_to_int_dictionary(const IType* dictionary_type);
    static bool is_non_generic_hashtable(const IType* dictionary_type);
    bool match_legacy_switch_on_string_with_hashtable(Block* block, HashtableInitializer& hashtable_initializers, int& i);
    bool find_hashtable_init_block(Block* entry_point, StringIndexList* string_values, const IField** dict_field, Block** block_after_this_init_block,
                                   ILInstruction** this_switch_init_jump_inst, ILInstruction** next_switch_init_jump_inst);
    bool match_roslyn_switch_on_string(InstructionCollection<ILInstruction>& instructions, int& i);
    bool match_roslyn_switch_on_string_using_length_and_char(Block* block, int i);
    static bool is_null_check_in_default_block(ILInstruction** exit_or_default, ILVariable* switch_var, Block** null_value_case_block);
    bool match_roslyn_case_block_head(Block* target, ILVariable* switch_value_var, ILInstruction** body_or_leave, Block** default_or_exit_block,
                                      OptString* string_value, bool* empty_string_equals_null);
    bool match_roslyn_empty_string_case_block_head(Block* target, ILVariable* switch_value_var, ILInstruction** body_or_leave,
                                                   Block** default_or_exit_block);
    static bool match_string_length_call(ILInstruction* inst, ILVariable* switch_value_var);
    bool match_string_equality_comparison(ILInstruction* condition, ILVariable* variable, OptString* string_value, bool* is_vb_compare_string);
    bool match_string_equality_comparison(ILInstruction* condition, ILVariable** variable, OptString* string_value, bool* is_vb_compare_string);

    ILTransformContext* context_ = nullptr;
    control_flow::SwitchAnalysis analysis_;
};

}  // namespace cb::dotnet::decompiler::il::transforms
