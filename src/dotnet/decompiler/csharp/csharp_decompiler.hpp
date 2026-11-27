// dotnet/decompiler/csharp/csharp_decompiler.hpp -- CSharpDecompiler, the main class of the C# decompiler engine: decompiles a whole module, some
// types, or single members into a C# syntax tree (and the tree to text). It reads each method body into the ILAst, runs the IL transform
// pipeline, builds C# statements from the result, and runs the AST transform pipeline over the tree of the declarations.
//
//     DecompilerSettings settings;
//     CSharpDecompiler decompiler("Game.exe", settings);
//     std::string cs = decompiler.decompile_whole_module_as_string();
//     syntax::SyntaxTree* tree = decompiler.decompile_type(FullTypeName("Game.Program"));
//     bool hidden = CSharpDecompiler::member_is_hidden(&file, method_handle, settings);   // compiler-generated, handled by a transform
//
// Instances are not thread-safe (the transform instances are not): use separate instances to decompile members in parallel.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: CSharpDecompiler, from CSharpDecompiler.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
//
// Differences from upstream:
// - Memory: a syntax tree, the ILAst and everything made on the way belong to the decompiler's ILArena (made current for each public
//   entry point); the trees returned stay valid until the decompiler is destroyed.
// - The settings are held by pointer and must outlive the decompiler; the constructors that open a file take them non-const because
//   upstream sets LoadInMemory on them.
// - Errors: upstream's DecompilerException is DecompilerError. get_error_comment_lines takes the text of the error as to_string()/what()
//   (a .NET stack trace has no counterpart here).
// - Not ported: the Stepper (debug steps: the step log does nothing in the transform contexts either), DecompilerEventSource tracing, the
//   debug info provider (kept in debug_info_provider, but ILReader and ILTransformContext have no slot for it yet), CSharpDecompiler(PEFile ...) stream options (the file is opened by
//   PEFile::open).
// - SignatureBlobComparer (used by member_is_hidden for accessor forwarding helpers) has no port of its own yet: the .cxx keeps a private copy.
//   Upstream's private FindAttribute (unused) is not ported; the namespaces of a decompile run are taken in name order (the collector's set
//   has no insertion order), and top-level types sorted with sort_types are compared ordinally, not with the culture's string order.
// - get_type_system_options is DecompilerTypeSystem.GetOptions(settings) (that class takes the options only).
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax_tree.hpp"
#include "dotnet/decompiler/debug_info/sequence_point.hpp"
#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/decompiler_exception.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/metadata/code_mapping_info.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/partial_type_info.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}  // namespace cb::dotnet::decompiler
namespace cb::dotnet::decompiler::debug_info {
class IDebugInfoProvider;
}  // namespace cb::dotnet::decompiler::debug_info
namespace cb::dotnet::decompiler::documentation {
class IDocumentationProvider;
}
namespace cb::dotnet::decompiler::il::transforms {
class IILTransform;
class ILTransformContext;
}  // namespace cb::dotnet::decompiler::il::transforms
namespace cb::dotnet::decompiler::csharp::transforms {
class IAstTransform;
}
namespace cb::dotnet::decompiler::csharp::type_system {
class UsingScope;
}
namespace cb::dotnet::decompiler::csharp::syntax {
class TypeSystemAstBuilder;
class EntityDeclaration;
class BlockStatement;
struct ExtensionGroup;
}  // namespace cb::dotnet::decompiler::csharp::syntax

namespace cb::dotnet::decompiler::csharp {

namespace srm = cb::dotnet::metadata;

class RecordDecompiler;

class L2Lane;

class CSharpDecompiler {
public:
    // The decompiler of a file: opens it, loads its references through a UniversalAssemblyResolver, and sets settings.load_in_memory.
    CSharpDecompiler(const std::filesystem::path& file_name, DecompilerSettings& settings);
    // The same with the given resolver for the references.
    CSharpDecompiler(const std::filesystem::path& file_name, metadata::IAssemblyResolver& assembly_resolver, DecompilerSettings& settings);
    // A decompiler of a module that is already open (the decompiler keeps it).
    CSharpDecompiler(std::shared_ptr<const metadata::MetadataFile> module, metadata::IAssemblyResolver& assembly_resolver,
                     const DecompilerSettings& settings);
    // A decompiler of the main module of a type system (made with the options of get_type_system_options, or a cached one: std::invalid_argument
    // for an uncached one). The type system must outlive the decompiler.
    CSharpDecompiler(const typesystem::DecompilerTypeSystem& type_system, const DecompilerSettings& settings);
    ~CSharpDecompiler();
    CSharpDecompiler(const CSharpDecompiler&) = delete;
    CSharpDecompiler& operator=(const CSharpDecompiler&) = delete;

    // ---- configuration ------------------------------------------------------------------------------------------------------

    // Token to check for requested cancellation of the decompilation.
    util::CancellationToken cancellation_token;
    // The optional provider for debug info (not owned).
    const debug_info::IDebugInfoProvider* debug_info_provider = nullptr;
    // The optional provider for XML documentation strings (not owned).
    documentation::IDocumentationProvider* documentation_provider = nullptr;

    // The type system created from the main module and referenced modules.
    const typesystem::DecompilerTypeSystem& type_system() const { return *type_system_; }
    // The method bodies as L2 (stage 14; not upstream's API): null until the first body.
    const L2Lane* l2_lane() const { return l2_lane_.get(); }
    // IL transforms and C# AST transforms: the built-in ones, which can be changed.
    std::vector<std::unique_ptr<il::transforms::IILTransform>>& il_transforms() { return il_transforms_; }
    std::vector<std::unique_ptr<transforms::IAstTransform>>& ast_transforms() { return ast_transforms_; }

    // Returns all built-in transforms of the ILAst pipeline.
    static std::vector<std::unique_ptr<il::transforms::IILTransform>> get_il_transforms();
    // Pre-yield/await transforms.
    static std::vector<std::unique_ptr<il::transforms::IILTransform>> early_il_transforms(bool aggressively_duplicate_return_blocks = false);
    // Returns all built-in transforms of the C# AST pipeline.
    static std::vector<std::unique_ptr<transforms::IAstTransform>> get_ast_transforms();
    // The type system options the settings ask for (DecompilerTypeSystem.GetOptions).
    static typesystem::TypeSystemOptions get_type_system_options(const DecompilerSettings& settings);

    // ---- errors -------------------------------------------------------------------------------------------------------------

    // Method bodies that could not be decompiled. Instead of aborting the surrounding type, such a member is emitted with the error text
    // in place of its body (see get_error_comment_lines) and the error is collected here, so callers decompiling many members (the
    // project exporter above all) can tell the user how many members are affected. Cleared by every public decompile entry point.
    const std::vector<DecompilerError>& errors() const { return errors_; }

    // Where users are asked to report decompilation failures; part of the error text emitted into the output.
    static constexpr const char* kDecompilationErrorReportUrl = "https://github.com/icsharpcode/ILSpy/issues/new";
    // The headline a front end puts above the list of failures it recovered from.
    static std::vector<std::string> get_error_summary_lines(int error_count);
    // The one-line description of a single failure.
    static std::string get_error_headline(const DecompilerError& error);
    // `error` as the lines of a comment block: an explanation, the request to report it, and the full text of the error.
    static std::vector<std::string> get_error_comment_lines(const std::exception& error);

    // ---- what to hide ---------------------------------------------------------------------------------------------------------

    // Whether a member should be hidden from the decompiled code. This is used to exclude compiler-generated code that is handled by
    // transforms from the output. `member` is the metadata handle of a TypeDef, MethodDef or FieldDef (any other kind is not hidden); the
    // settings say what is hidden (e.g. if async methods are not transformed, async state machines are included in the decompiled code).
    static bool member_is_hidden(const metadata::MetadataFile* module, srm::EntityHandle member, const DecompilerSettings& settings);
    static bool is_transparent_identifier(const std::string& identifier);

    // Whether a type requires that its methods be ordered precisely as they were originally defined (COM interop: the v-table is fixed by
    // the ABI), and its members in that order (by the metadata token of the method).
    bool requires_native_ordering(const typesystem::ITypeDefinition* type_def) const;
    std::vector<const typesystem::IMember*> get_members_with_native_ordering(const typesystem::ITypeDefinition* type_def) const;

    // ---- decompiling ----------------------------------------------------------------------------------------------------------

    // Assembly and module attributes.
    syntax::SyntaxTree* decompile_module_and_assembly_attributes();
    std::string decompile_module_and_assembly_attributes_to_string();

    // The whole module in a single syntax tree; with sort_types the top-level types are sorted by namespace and name, else they are in
    // metadata order.
    syntax::SyntaxTree* decompile_whole_module_as_single_file(bool sort_types = false);
    std::string decompile_whole_module_as_string();

    // The given types, each in a namespace declaration (unlike decompile, which does not add one).
    syntax::SyntaxTree* decompile_types(const std::vector<srm::TypeDefinitionHandle>& types);
    std::string decompile_types_as_string(const std::vector<srm::TypeDefinitionHandle>& types);
    // The given type (only a type of the main module: UsageError for another, UsageError for one that is not found).
    syntax::SyntaxTree* decompile_type(const typesystem::FullTypeName& full_type_name);
    std::string decompile_type_as_string(const typesystem::FullTypeName& full_type_name);
    // The specified types and/or members (a TypeDef, MethodDef, FieldDef, PropertyDef or EventDef; any other kind: UsageError).
    syntax::SyntaxTree* decompile(const std::vector<srm::EntityHandle>& definitions);
    std::string decompile_as_string(const std::vector<srm::EntityHandle>& definitions);
    // A type, method or property of C# 14 extension blocks written as such.
    syntax::SyntaxTree* decompile_extension(srm::EntityHandle handle);

    // The context an IL transform is run in for `function`.
    std::unique_ptr<il::transforms::ILTransformContext> create_il_transform_context(il::ILFunction* function);

    // The "code mappings" of a TypeDef or MethodDef: which compiler-generated parts (lambdas, state machine methods, local functions)
    // belong to which method (see CodeMappingInfo). Made for the declaring type of `member`.
    static std::shared_ptr<const metadata::CodeMappingInfo> get_code_mapping_info(const metadata::MetadataFile& module, srm::EntityHandle member);

    // Declares what a partial type of the project is made of (the members another file declares are left out of this one).
    void add_partial_type_definition(const PartialTypeInfo& info);

    // Sequence points for the nodes of a syntax tree (only right for nodes that carry line and column information).
    static util::InsertionOrderedMap<il::ILFunction*, std::vector<debug_info::SequencePoint>> create_sequence_points(syntax::SyntaxTree* syntax_tree);

    // ---- used by the builders and transforms ------------------------------------------------------------------------------

    // Decompiles the body of `method` to ILAst for structural analysis, e.g. for recognizing compiler-generated code (see RecordDecompiler,
    // AutoEventDecompiler). Runs the IL transform pipeline with a fixed set of decompiler settings, so the resulting shape is independent
    // of the user-visible settings, and stops before the late transforms (variable naming etc.) that are only needed for code output.
    // nullptr for a method without a body. Needs a current ILArena.
    static il::Block* decompile_body_for_analysis(const typesystem::IMethod* method, const typesystem::DecompilerTypeSystem& type_system,
                                                  const util::CancellationToken& cancellation_token);

    // Parameter annotations (the ILVariable of each parameter) and the function itself on the declaration.
    static void add_annotations_to_declaration(const typesystem::IMethod* method, syntax::EntityDeclaration* entity_decl, il::ILFunction* function,
                                               int parameter_offset = 0);
    // async/iterator modifiers and attributes of a decompiled method (an iterator without any yield gets a yield break).
    static void clean_up_method_declaration(syntax::EntityDeclaration* entity_decl, syntax::BlockStatement* body, il::ILFunction* function,
                                            bool decompile_body = true);
    // Removes the attributes of that type (and a section left empty); whether there was one.
    static bool remove_attribute(syntax::EntityDeclaration* entity_decl, typesystem::KnownAttribute attribute_type);
    // [CompilerFeatureRequired("feature")]
    static bool remove_compiler_feature_required_attribute(syntax::EntityDeclaration* entity_decl, const std::string& feature);
    // [Obsolete("message")]
    static bool remove_obsolete_attribute(syntax::EntityDeclaration* entity_decl, const std::string& message);
    // A [FixedBuffer] field: its element type and count.
    static bool is_fixed_field(const typesystem::IField* field, const typesystem::IType** type, int* element_count);
    // A WinForms designer method (InitializeComponent), which is decompiled with settings that keep it round-trippable.
    static bool is_windows_forms_initialize_component_method(const typesystem::IMethod* method);

private:
    struct TypeState;

    void initialize();
    std::unique_ptr<DecompileRun> create_decompile_run(std::unordered_set<std::string> namespaces);
    void run_transforms(syntax::AstNode* root_node, DecompileRun& decompile_run, const typesystem::ITypeResolveContext& decompilation_context);
    std::string syntax_tree_to_string(syntax::SyntaxTree* syntax_tree);
    void do_decompile_module_and_assembly_attributes(DecompileRun& decompile_run, syntax::SyntaxTree* syntax_tree);
    void do_decompile_types(const std::vector<srm::TypeDefinitionHandle>& types, DecompileRun& decompile_run,
                            const typesystem::SimpleTypeResolveContext& decompilation_context, syntax::SyntaxTree* syntax_tree);
    const typesystem::ITypeDefinition* find_common_declaring_type_definition(const typesystem::ITypeDefinition* a, const typesystem::ITypeDefinition* b) const;

    std::vector<syntax::EntityDeclaration*> add_interface_impl_helpers(syntax::EntityDeclaration* member_decl, const typesystem::IMethod* method,
                                                                       syntax::TypeSystemAstBuilder& ast_builder);
    static syntax::Expression* forward_parameter(syntax::ParameterDeclaration* p);
    void set_new_modifier(syntax::EntityDeclaration* member);
    bool is_entry_point(const typesystem::IMethod* method) const;
    static void fix_parameter_names(syntax::EntityDeclaration* entity);
    bool is_covariant_return_override(const typesystem::IEntity* entity) const;
    EnumValueDisplayMode detect_best_enum_value_display_mode(const typesystem::ITypeDefinition* type_def, const metadata::MetadataFile& module) const;
    void add_defines_for_conditional_attributes(il::ILFunction* function, DecompileRun& decompile_run);

    syntax::EntityDeclaration* do_decompile(const typesystem::ITypeDefinition* type_def, DecompileRun& decompile_run,
                                            const typesystem::SimpleTypeResolveContext& decompilation_context, bool as_extension = false);
    syntax::EntityDeclaration* do_decompile(const typesystem::IMethod* method, DecompileRun& decompile_run,
                                            const typesystem::SimpleTypeResolveContext& decompilation_context, const typesystem::ExtensionInfo* extension_info);
    syntax::EntityDeclaration* do_decompile(const typesystem::IField* field, DecompileRun& decompile_run,
                                            const typesystem::SimpleTypeResolveContext& decompilation_context);
    syntax::EntityDeclaration* do_decompile(const typesystem::IProperty* property, DecompileRun& decompile_run,
                                            const typesystem::SimpleTypeResolveContext& decompilation_context, const typesystem::ExtensionInfo* extension_info);
    syntax::EntityDeclaration* do_decompile(const typesystem::IEvent* ev, DecompileRun& decompile_run, const typesystem::SimpleTypeResolveContext& decompilation_context);
    std::vector<syntax::EntityDeclaration*> decompile_parameterized_property(const typesystem::IProperty* property, DecompileRun& decompile_run,
                                                                             const typesystem::SimpleTypeResolveContext& decompilation_context,
                                                                             const typesystem::ExtensionInfo* extension_info);
    void decompile_body(const typesystem::IMethod* method, syntax::EntityDeclaration* entity_decl, DecompileRun& decompile_run,
                        const typesystem::SimpleTypeResolveContext& decompilation_context, const typesystem::ExtensionInfo* extension_info);

    const typesystem::DecompilerTypeSystem* type_system_ = nullptr;
    const typesystem::MetadataModule* module_ = nullptr;
    const srm::MetadataReader* metadata_ = nullptr;
    const DecompilerSettings* settings_ = nullptr;
    syntax::SyntaxTree* syntax_tree_ = nullptr;
    std::vector<std::unique_ptr<il::transforms::IILTransform>> il_transforms_;
    std::vector<std::unique_ptr<transforms::IAstTransform>> ast_transforms_;
    std::vector<DecompilerError> errors_;
    std::unordered_map<srm::TypeDefinitionHandle, PartialTypeInfo> partial_types_;
    // What this decompiler made on the way (a decompile run for an IL transform context).
    std::vector<std::unique_ptr<DecompileRun>> kept_runs_;
    // The documentation found for the module when no provider is set.
    std::shared_ptr<documentation::IDocumentationProvider> default_documentation_provider_;
    // What a decompiler made from a file owns.
    std::shared_ptr<const metadata::MetadataFile> owned_file_;
    std::unique_ptr<metadata::IAssemblyResolver> owned_resolver_;
    std::unique_ptr<typesystem::DecompilerTypeSystem> owned_type_system_;
    // the method bodies as L2 (stage 14, l2_body.hpp): made at the first body, after the type system, so it goes first
    std::unique_ptr<L2Lane> l2_lane_;
    // The syntax trees, ILAst and everything else made in the thread's current arena belong to this one.
    il::ILArena arena_;
};

}  // namespace cb::dotnet::decompiler::csharp
