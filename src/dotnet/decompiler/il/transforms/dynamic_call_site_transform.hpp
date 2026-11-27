// dotnet/decompiler/il/transforms/dynamic_call_site_transform.hpp -- turns the "call site initialization pattern" the C# compiler emits
// for every `dynamic` operation (a static CallSite<T> cache field, a null check, the Binder.Xxx call that makes the binder and the
// Invoke of the site's Target delegate) into the dynamic instructions of the ILAst (dynamic.getmember, dynamic.invoke ...).
//
//     if (comp(ldsfld <>p__0 == ldnull)) br initBlock      dynamic.getmember Name(target)
//     ...callvirt Invoke(ldfld Target(ldsfld <>p__0), ...) =>
//
//     DynamicCallSiteTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: DynamicCallSiteTransform, from DynamicCallSiteTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The array initializer and Type.GetTypeFromHandle matching
// it relies on come from TransformArrayInitializers and TransformExpressionTrees.
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class DynamicCallSiteTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "DynamicCallSiteTransform"; }

    // Runs on one basic block of the async state machine's container (the callers sort the container's blocks later).
    static void run_on_basic_block(Block* block, ILTransformContext& context);

    enum class BinderMethodKind : std::uint8_t {
        BinaryOperation,
        Convert,
        GetIndex,
        GetMember,
        Invoke,
        InvokeConstructor,
        InvokeMember,
        IsEvent,
        SetIndex,
        SetMember,
        UnaryOperation,
    };

    struct CallSiteInfo {
        bool inverted = false;
        ILInstruction* branch_after_init = nullptr;
        IfInstruction* conditional_jump_to_init = nullptr;
        Block* init_block = nullptr;
        const IType* delegate_type = nullptr;
        BinderMethodKind kind = BinderMethodKind::BinaryOperation;
        CSharpBinderFlags flags = CSharpBinderFlags::None;
        ExpressionType operation = ExpressionType::Add;
        const IType* context = nullptr;
        const IType* convert_target_type = nullptr;
        std::vector<const IType*> type_arguments;
        std::vector<CSharpArgumentInfo> argument_infos;
        std::string member_name;
    };

private:
    // The call sites by their cache field (compared as members are).
    struct FieldHash {
        std::size_t operator()(const IField* field) const;
    };
    struct FieldEqual {
        bool operator()(const IField* a, const IField* b) const;
    };
    using CallSiteMap = std::unordered_map<const IField*, CallSiteInfo, FieldHash, FieldEqual>;

    static std::vector<BlockContainer*> transform_call_sites(BlockContainer* parent, const CallSiteMap& callsites, ILTransformContext& context);
    static bool is_single_definition_temporary(ILVariable* variable);
    static void find_dynamic_call_sites_in_block(Block* block, CallSiteMap& callsites, ILTransformContext& context);
    static ILInstruction* make_dynamic_instruction(const CallSiteInfo& callsite, CallVirt* target_invoke_call, std::vector<ILInstruction*>& dead_arguments);
    static bool scan_call_site_init_block(Block* call_site_init_block, const IField* call_site_cache_field, const IType* call_site_delegate_type,
                                          CallSiteInfo* call_site_info, Block** block_after_init, ILTransformContext& context);
    static bool extract_argument_info(ILInstruction* value, CallSiteInfo* call_site_info, int instruction_offset, ILVariable* variable,
                                      ILTransformContext& context);
    static bool match_call_site_cache_null_check(ILInstruction* condition, const IField** call_site_cache_field, const IType** call_site_delegate,
                                                bool* invert_branches);
};

}  // namespace cb::dotnet::decompiler::il::transforms
