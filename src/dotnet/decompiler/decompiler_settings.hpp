// dotnet/decompiler/decompiler_settings.hpp -- the decompiler's settings: one public member per ILSpy setting (snake_case, with
// upstream's default), and the C# language versions that gate them.
//
//     DecompilerSettings settings(LanguageVersion::CSharp7);   // features of C# 8 and later are off
//     if (settings.use_debug_symbols) ...
//     LanguageVersion needed = settings.get_minimum_required_version();
//
// Ported from ILSpy (ICSharpCode.Decompiler: DecompilerSettings, with the SetLanguageVersion and GetMinimumRequiredVersion that
// ILSpy's source generator writes; CSharp.LanguageVersion), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Not ported: change notification (INotifyPropertyChanged), the settings UI's categories and descriptions, the
// obsolete LifetimeAnnotations alias (use scoped_ref), and CSharpFormattingOptions (it belongs to the C# output, which comes later).
#pragma once

namespace cb::dotnet::decompiler {

// The C# versions (the numbers are upstream's, so versions order as numbers do).
enum class LanguageVersion : int {
    CSharp1 = 1,
    CSharp2 = 2,
    CSharp3 = 3,
    CSharp4 = 4,
    CSharp5 = 5,
    CSharp6 = 6,
    CSharp7 = 7,
    CSharp7_1 = 701,
    CSharp7_2 = 702,
    CSharp7_3 = 703,
    CSharp8_0 = 800,
    CSharp9_0 = 900,
    CSharp10_0 = 1000,
    CSharp11_0 = 1100,
    CSharp12_0 = 1200,
    CSharp13_0 = 1300,
    CSharp14_0 = 1400,
    CSharp15_0 = 1500,
    Preview = 1500,
    Latest = 0x7FFFFFFF,
};

class DecompilerSettings {
public:
    // Equivalent to DecompilerSettings(LanguageVersion::Latest): every feature on (but the ones that default to off).
    DecompilerSettings() = default;
    // Initial settings appropriate for the language version. This does not make the output strictly use that version's features
    // (generics or ref locals cannot be removed from compiled code); the version is not stored.
    explicit DecompilerSettings(LanguageVersion language_version) { set_language_version(language_version); }

    // One-shot profile initializer: switches off every feature of a version newer than `language_version`. It does not switch
    // anything on, so a later call with a higher version does not bring features back.
    void set_language_version(LanguageVersion language_version);
    // The lowest language version that includes every enabled feature.
    LanguageVersion get_minimum_required_version() const;

    DecompilerSettings clone() const { return *this; }

    // Use C# 9 nint/nuint types.
    bool native_integers = true;
    // Treat IntPtr/UIntPtr as nint/nuint.
    bool numeric_int_ptr = true;
    // Decompile C# 9 covariant return types.
    bool covariant_returns = true;
    // Use C# 9 init; property accessors.
    bool init_accessors = true;
    // Use C# 9 record classes.
    bool record_classes = true;
    // Use C# 10 record structs.
    bool record_structs = true;
    // Use field initializers in structs.
    bool struct_default_constructors_and_field_initializers = true;
    // Use C# 9 with initializer expressions.
    bool with_expressions = true;
    // Use primary constructor syntax with records.
    bool use_primary_constructor_syntax = true;
    // Use C# 9 delegate* unmanaged types.
    // If this option is disabled, function pointers will instead be decompiled with type `IntPtr`.
    bool function_pointers = true;
    // Use C# 11 scoped modifier.
    bool scoped_ref = true;
    // Use C# 11 required modifier.
    bool required_members = true;
    // Use C# 8 switch expressions.
    bool switch_expressions = true;
    // Use C# 10 file-scoped namespaces.
    bool file_scoped_namespaces = true;
    // Decompile anonymous methods/lambdas.
    bool anonymous_methods = true;
    // Decompile anonymous types.
    bool anonymous_types = true;
    // Use C# 3 lambda syntax if possible.
    bool use_lambda_syntax = true;
    // Decompile expression trees.
    bool expression_trees = true;
    // Decompile enumerators.
    bool yield_return = true;
    // Decompile use of the 'dynamic' type.
    bool dynamic = true;
    // Decompile async methods.
    bool async_await = true;
    // Decompile await in catch/finally blocks.
    // Only has an effect if  is enabled.
    bool await_in_catch_finally = true;
    // Decompile IAsyncEnumerator/IAsyncEnumerable.
    // Only has an effect if  is enabled.
    bool async_enumerator = true;
    // Decompile [DecimalConstant(...)] as simple literal values.
    bool decimal_constants = true;
    // Decompile C# 1.0 'public unsafe fixed int arr[10];' members.
    bool fixed_buffers = true;
    // Decompile 'string.Concat(a, b)' calls into 'a + b'.
    bool string_concat = true;
    // Use lifted operators for nullables.
    bool lift_nullables = true;
    // Decompile C# 6 ?. and ?[] operators.
    bool null_propagation = true;
    // Decompile automatic properties
    bool automatic_properties = true;
    // Decompile getter-only automatic properties
    bool getter_only_automatic_properties = true;
    // Decompile automatic events
    bool automatic_events = true;
    // Decompile using statements.
    bool using_statement = true;
    // Use enhanced using statements.
    bool use_enhanced_using = true;
    // Gets/Sets whether to use braces for single-statement-blocks.
    bool always_use_braces = true;
    // Decompile foreach statements.
    bool for_each_statement = true;
    // Support GetEnumerator extension methods in foreach.
    bool for_each_with_get_enumerator_extension = true;
    // Support params collections.
    bool params_collections = true;
    // Decompile lock statements.
    bool lock_statement = true;
    bool switch_statement_on_string = true;
    bool sparse_integer_switch = true;
    bool using_declarations = true;
    bool extension_methods = true;
    bool query_expressions = true;
    // Gets/Sets whether to use C# 2.0 method group conversions.
    // true: EventHandler h = this.OnClick;
    // false: EventHandler h = new EventHandler(this.OnClick);
    bool use_implicit_method_group_conversion = true;
    // Gets/Sets whether to use object creation expressions for generic types with new() constraint.
    // true: T t = new T();
    // false: T t = Activator.CreateInstance<T>()
    bool use_object_creation_of_generic_type_parameter = true;
    // Gets/Sets whether to always cast targets to explicitly implemented methods.
    // true: ((ISupportInitialize)pictureBox1).BeginInit();
    // false: pictureBox1.BeginInit();
    // default: false
    bool always_cast_targets_of_explicit_interface_implementation_calls = false;
    // Gets/Sets whether to always qualify member references.
    // true: this.DoSomething();
    // false: DoSomething();
    // default: false
    bool always_qualify_member_references = false;
    // Gets/Sets whether to always show enum member values.
    // true: enum Kind { A = 0, B = 1, C = 5 }
    // false: enum Kind { A, B, C = 5 }
    // default: false
    bool always_show_enum_member_values = false;
    // Gets/Sets whether to use variable names from debug symbols, if available.
    bool use_debug_symbols = true;
    // Gets/Sets whether to use array initializers.
    // If set to false, might produce non-compilable code.
    bool array_initializers = true;
    // Gets/Sets whether to use C# 3.0 object/collection initializers.
    bool object_or_collection_initializers = true;
    // Gets/Sets whether to use C# 6.0 dictionary initializers.
    // Only has an effect if ObjectOrCollectionInitializers is enabled.
    bool dictionary_initializers = true;
    // Gets/Sets whether to use C# 6.0 Extension Add methods in collection initializers.
    // Only has an effect if ObjectOrCollectionInitializers is enabled.
    bool extension_methods_in_collection_initializers = true;
    // Gets/Sets whether to use local ref variables in cases where this is necessary
    // for re-compilation with a modern C# compiler to reproduce the same behavior
    // as the original assembly produced with an old C# compiler that used an incorrect
    // order of evaluation.
    // See https://github.com/icsharpcode/ILSpy/issues/2050
    bool use_ref_locals_for_accurate_order_of_evaluation = true;
    // Gets/Sets whether to use C# 7.2 'ref' extension methods.
    bool ref_extension_methods = true;
    // Gets/Sets whether to use C# 6.0 string interpolation
    bool string_interpolation = true;
    // Gets/Sets whether to use C# 11.0 UTF-8 string literals
    bool utf8_string_literals = true;
    // Gets/Sets whether to use C# 11.0 switch on (ReadOnly)Span<char>
    bool switch_on_read_only_span_char = true;
    // Gets/Sets whether to use C# 11.0 unsigned right shift operator.
    bool unsigned_right_shift = true;
    // Gets/Sets whether to use C# 11.0 user-defined checked operators.
    bool checked_operators = true;
    // Gets/Sets whether to include XML documentation comments in the decompiled code.
    bool show_xml_documentation = true;
    bool fold_braces = false;
    bool expand_xml_documentation_comments = false;
    bool expand_member_definitions = false;
    bool expand_using_declarations = false;
    // Gets/Sets whether member bodies should be decompiled.
    bool decompile_member_bodies = true;
    // Gets/Sets whether simple calculated getter-only property declarations
    // should use expression body syntax.
    bool use_expression_body_for_calculated_getter_only_properties = true;
    // Gets/Sets whether out variable declarations should be used when possible.
    bool out_variables = true;
    // Gets/Sets whether discards should be used when possible.
    // Only has an effect if  is enabled.
    bool discards = true;
    // Gets/Sets whether IsByRefLikeAttribute should be replaced with 'ref' modifiers on structs.
    bool introduce_ref_modifiers_on_structs = true;
    bool introduce_readonly_and_in_modifiers = true;
    // Gets/Sets whether "private protected" should be used.
    bool introduce_private_protected_accessibility = true;
    bool read_only_methods = true;
    bool async_using_and_for_each_statement = true;
    bool introduce_unmanaged_constraint = true;
    // Gets/Sets whether C# 7.3 stackalloc initializers should be used.
    bool stack_alloc_initializers = true;
    // Gets/Sets whether C# 7.3 pattern based fixed statement should be used.
    bool pattern_based_fixed_statement = true;
    // Gets/Sets whether tuple type syntax (int, string)
    // should be used for System.ValueTuple.
    bool tuple_types = true;
    // Gets/Sets whether throw expressions should be used.
    bool throw_expressions = true;
    // Gets/Sets whether implicit conversions between tuples
    // should be used in the decompiled output.
    bool tuple_conversions = true;
    // Gets/Sets whether tuple comparisons should be detected.
    bool tuple_comparisons = true;
    // Gets/Sets whether named arguments should be used.
    bool named_arguments = true;
    // Gets/Sets whether C# 7.2 non-trailing named arguments should be used.
    bool non_trailing_named_arguments = true;
    // Gets/Sets whether optional arguments should be removed, if possible.
    bool optional_arguments = true;
    // Gets/Sets whether to expand params arguments by replacing explicit array creation
    // with individual values in method calls.
    bool expand_params_arguments = true;
    // Gets/Sets whether C# 7.0 local functions should be transformed.
    bool local_functions = true;
    // Gets/Sets whether C# 7.0 deconstruction should be detected.
    bool deconstruction = true;
    // Gets/Sets whether C# 7.0 pattern matching should be detected.
    bool pattern_matching = true;
    // Gets/Sets whether C# 8.0 recursive patterns should be detected.
    bool recursive_pattern_matching = true;
    // Gets/Sets whether C# 9.0 and, or, not patterns should be detected.
    bool pattern_combinators = true;
    // Gets/Sets whether C# 9.0 relational patterns should be detected.
    bool relational_patterns = true;
    // Gets/Sets whether C# 8.0 static local functions should be transformed.
    bool static_local_functions = true;
    // Gets/Sets whether C# 8.0 index and range syntax should be used.
    bool ranges = true;
    // Gets/Sets whether C# 8.0 nullable reference types are enabled.
    bool nullable_reference_types = true;
    bool show_debug_info = false;
    // Gets/Sets whether the decompiler can assume that 'ldlen; conv.i4.ovf'
    // does not throw an overflow exception.
    bool assume_array_length_fits_into_int32 = true;
    // Gets/Sets whether to use increment and decrement operators
    bool introduce_increment_and_decrement = true;
    // Gets/Sets whether to use assignment expressions such as in while ((count = Do()) != 0) ;
    bool make_assignment_expressions = true;
    bool remove_dead_code = false;
    bool remove_dead_stores = false;
    bool load_in_memory = false;
    bool throw_on_assembly_resolve_errors = true;
    bool apply_windows_runtime_projections = true;
    bool auto_load_assembly_references = true;
    // Gets/sets whether the decompiler should produce for loops.
    bool for_statement = true;
    // Gets/sets whether the decompiler should produce do-while loops.
    bool do_while_statement = true;
    // Gets/sets whether RequiresLocationAttribute on parameters should be replaced with 'ref readonly' modifiers.
    bool ref_read_only_parameters = true;
    // Use primary constructor syntax with classes and structs.
    bool use_primary_constructor_syntax_for_non_record_types = true;
    // Gets/Sets whether C# 12.0 inline array uses should be transformed.
    bool inline_arrays = true;
    // Gets/Sets whether C# 14.0 extension members should be transformed.
    bool extension_members = true;
    // Gets/Sets whether (ReadOnly)Span<T> should be treated like built-in types.
    bool first_class_span_types = true;
    // Gets/sets whether the decompiler should separate local variable declarations
    // from their initialization.
    bool separate_local_variable_declarations = false;
    // Gets or sets a value indicating whether the new SDK style format
    // shall be used for the generated project files.
    bool use_sdk_style_project_format = true;
    // Gets/sets whether namespaces and namespace-like identifiers should be split at '.'
    // and each part should produce a new level of nesting in the output directory structure.
    bool use_nested_directories_for_namespaces = false;
    bool aggressive_scalar_replacement_of_aggregates = false;
    // If set to false (the default), the decompiler will inline local variables only when they occur
    // in a context where the C# compiler is known to emit compiler-generated locals.
    // If set to true, the decompiler will inline local variables whenever possible.
    bool aggressive_inlining = false;
    // Always fully qualify namespaces using the "global::" prefix.
    bool always_use_global = false;
    // If set to false (the default), the decompiler will move field initializers at the start of constructors
    // to their respective field declarations (TransformFieldAndConstructorInitializers) only when the declaring
    // type has BeforeFieldInit or the member IsConst.
    // If set true, the decompiler will always move them regardless of the flags.
    bool always_move_initializer = false;
    // Sort custom attributes.
    bool sort_custom_attributes = false;
    // Sort switch sections by their label value instead of by IL offset.
    // Useful when diffing decompiler output across rebuilds of obfuscated assemblies,
    // where IL block layout is unstable but the case-to-value mapping is not.
    bool sort_switch_sections = false;
    // Check for overflow and underflow in operators.
    bool check_for_overflow_underflow = false;
};

}  // namespace cb::dotnet::decompiler
