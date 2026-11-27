// dotnet/decompiler/il/transforms/delegate_construction.hpp -- turns the construction of a delegate over a compiler-generated method
// (an anonymous method or lambda) into a nested ILFunction: `newobj Delegate(target, ldftn method)` becomes the method's own ILAst,
// with its uses of `this` replaced by the target expression.
//
//     newobj Func<int>(ldloc displayClass, ldftn <Main>b__0)     =>     ILFunction <Main>b__0 { ... }
//
//     DelegateConstruction transform;
//     transform.run(function, context);
//     DelegateConstruction::match_delegate_construction(inst, &target_method, &target, &delegate_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: DelegateConstruction, DelegateConstruction.ReplaceDelegateTargetVisitor, from
// DelegateConstruction.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported:
// CSharpDecompiler.GetILTransforms (the transforms that run on the lambda's body before this one; only CombineExitsTransform runs
// after them, so a lambda body is not yet transformed as upstream's).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ITypeDefinition;
}

namespace cb::dotnet::decompiler::il::transforms {

class DelegateConstruction final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "DelegateConstruction"; }

    // Whether `inst` constructs a delegate (newobj Delegate(target, ldftn/ldvirtftn method), or ldvirtdelegate), and its parts. With
    // `allow_transformed`, an ILFunction may stand where the ldftn was.
    static bool match_delegate_construction(ILInstruction* inst, const IMethod** target_method, ILInstruction** target, const IType** delegate_type,
                                            bool allow_transformed = false);

    // Replaces loads of 'this' with the target expression. Async delegates use: ldobj(ldloca this).
    class ReplaceDelegateTargetVisitor final : public ILVisitor {
    public:
        ReplaceDelegateTargetVisitor(ILInstruction* target, ILVariable* this_variable) : target_(target), this_variable_(this_variable) {}

        void default_visit(ILInstruction& inst) override;
        void visit_il_function(ILFunction& function) override;
        void visit_ld_loc(LdLoc& inst) override;
        void visit_ld_obj(LdObj& inst) override;

    private:
        ILInstruction* target_;
        ILVariable* this_variable_;
    };

private:
    static bool is_anonymous_method(const typesystem::ITypeDefinition* decompiled_type_definition, const IMethod* method);
    ILFunction* transform_delegate_construction(ILInstruction* value, const IMethod* target_method, ILInstruction* target, const IType* delegate_type);
    static bool validate_delegate_target(ILInstruction* inst);

    ILTransformContext* context_ = nullptr;
    const typesystem::ITypeDefinition* current_type_definition_ = nullptr;  // upstream's decompilationContext.CurrentTypeDefinition
    std::vector<cb::dotnet::metadata::EntityHandle> active_methods_;                         // a stack (upstream's Stack<MethodDefinitionHandle>)
};

}  // namespace cb::dotnet::decompiler::il::transforms
