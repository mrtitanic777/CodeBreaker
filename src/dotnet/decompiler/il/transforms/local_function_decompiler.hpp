// dotnet/decompiler/il/transforms/local_function_decompiler.hpp -- recognizes the C# 7 local functions the compiler hides as static or
// instance methods with a generated name (`<Main>g__Add|0_0`), reads each into a nested ILFunction declared where its first use needs
// it, hands its closure parameters (a `ref` to a display struct) back to the captured variables, and turns the calls and delegate
// constructions over it into calls and references to the local function.
//
//     call <Main>g__Add|0_0(ref displayStruct, 1)              =>     call Add(1)         (Add declared in the scope of displayStruct)
//
//     LocalFunctionDecompiler transform;
//     transform.run(function, context);
//     LocalFunctionDecompiler::is_local_function_method(method, context);
//
// Local functions can be used in method calls (call and callvirt), as part of the delegate construction pattern
// (`newobj Delegate(target, ldftn method)`) or by `ldftn` alone. As they can be declared practically anywhere, the declaration location is
// inferred from the use-sites; after all use-sites are collected the ILAst of the local function is made and added to the parent
// function, then the use-sites are transformed, nested local functions are handled, and local functions that capture variables are moved to
// their declaration scope.
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: LocalFunctionDecompiler, from LocalFunctionDecompiler.cs;
// ICSharpCode.Decompiler.Metadata: FindTypeDecoder, its handle-only form), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler)..
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ITypeDefinition;
class IParameter;
class ITypeResolveContext;
}  // namespace cb::dotnet::decompiler::typesystem

namespace cb::dotnet::decompiler::il::transforms {

namespace srm = cb::dotnet::metadata;

class LocalFunctionDecompiler final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "LocalFunctionDecompiler"; }

    // Whether `parameter` is a closure parameter: a `ref` to a display struct of the type tree of `current_type_definition`.
    static bool is_closure_parameter(const typesystem::IParameter* parameter, const typesystem::ITypeResolveContext& context);
    static bool is_closure_parameter(const typesystem::IParameter* parameter, const typesystem::ITypeDefinition* current_type_definition);
    // `newobj Delegate(target, ldftn localFunction)`
    static bool is_local_function_reference(NewObj* inst, ILTransformContext& context);
    static bool is_local_function_method(const IMethod* method, ILTransformContext& context);
    // With a context, only methods of the context's module are local functions.
    static bool is_local_function_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method_handle, ILTransformContext* context = nullptr);
    // Whether the local function has a compiler-generated struct as a by-reference parameter (so it cannot stay private).
    static bool local_function_needs_accessibility_change(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method_handle);
    static bool is_local_function_display_class(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type_handle, ILTransformContext* context = nullptr);
    // The names are "<callerName>g__functionName|x_y" (newer Roslyn) or "<callerName>g__functionNamex_y" (older).
    static bool parse_local_function_name(const std::string& name, std::string* caller_name, std::string* function_name);

private:
    struct LocalFunctionInfo {
        std::vector<ILInstruction*> use_sites;
        const IMethod* method = nullptr;
        ILFunction* definition = nullptr;
        // All synthesized call-site arguments grouped by the parameter index. A map instead of an array, because -1 is used for the
        // this parameter and there might be many non-synthesized arguments in between.
        util::InsertionOrderedMap<int, std::vector<ILInstruction*>> local_function_arguments;
    };
    using LocalFunctions = util::InsertionOrderedMap<std::uint32_t, std::shared_ptr<LocalFunctionInfo>>;

    void replace_references_to_display_class_this(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions);
    void determine_capture_and_declaration_scopes(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions);
    void transform_use_sites(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions);
    void propagate_closure_parameter_arguments(LocalFunctions& local_functions);
    ILInstruction* find_compatible_argument(LocalFunctionInfo& info, const std::vector<ILInstruction*>& arguments, bool ignore_structure = false);
    ILVariable* resolve_ancestor_scope_reference(ILInstruction* inst);
    static ILFunction* get_declaring_function(ILFunction* local_function);
    bool try_validate_skip_count(LocalFunctionInfo& info, int* skip_count);
    void find_use_sites(ILFunction* function, ILTransformContext& context, LocalFunctions& local_functions);
    void handle_use_site(const IMethod* target_method, ILInstruction* inst, ILTransformContext& context, LocalFunctions& local_functions);
    ILFunction* read_local_function_definition(ILFunction* root_function, const IMethod* target_method, int skip_count);
    int get_skip_count(ILFunction* root_function, const IMethod* target_method);
    const IMethod* reduce_to_local_function(const IMethod* method, int type_parameters_to_remove);
    static void transform_to_local_function_reference(ILFunction* function, CallInstruction* use_site);
    Call* transform_to_local_function_invocation(const IMethod* reduced_method, CallInstruction* use_site);
    void determine_capture_and_declaration_scope(LocalFunctionInfo& info, ILInstruction* use_site);
    bool determine_capture_and_declaration_scope(LocalFunctionInfo& info, int parameter_index, ILInstruction* arg);
    bool is_in_nested_local_function(BlockContainer* declaration_scope, ILFunction* function);

    ILTransformContext* context_ = nullptr;
    const typesystem::ITypeDefinition* current_type_definition_ = nullptr;  // upstream's resolveContext.CurrentTypeDefinition
};

}  // namespace cb::dotnet::decompiler::il::transforms
