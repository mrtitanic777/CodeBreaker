// dotnet/decompiler/decompiler_settings.cxx -- DecompilerSettings::set_language_version and get_minimum_required_version, which
// ILSpy's source generator writes from each setting's language version.
#include "dotnet/decompiler/decompiler_settings.hpp"

namespace cb::dotnet::decompiler {

void DecompilerSettings::set_language_version(LanguageVersion language_version) {
    // By default, all decompiler features are enabled. Disable some of them based on the language version:
    if (language_version < LanguageVersion::CSharp1) {
        decimal_constants = false;
        fixed_buffers = false;
        string_concat = false;
        automatic_events = false;
        using_statement = false;
        for_each_statement = false;
        lock_statement = false;
        switch_statement_on_string = false;
        sparse_integer_switch = false;
        using_declarations = false;
        array_initializers = false;
        expand_params_arguments = false;
        for_statement = false;
        do_while_statement = false;
    }
    if (language_version < LanguageVersion::CSharp2) {
        anonymous_methods = false;
        yield_return = false;
        lift_nullables = false;
        use_implicit_method_group_conversion = false;
        use_object_creation_of_generic_type_parameter = false;
    }
    if (language_version < LanguageVersion::CSharp3) {
        anonymous_types = false;
        use_lambda_syntax = false;
        expression_trees = false;
        automatic_properties = false;
        extension_methods = false;
        query_expressions = false;
        object_or_collection_initializers = false;
    }
    if (language_version < LanguageVersion::CSharp4) {
        dynamic = false;
        named_arguments = false;
        optional_arguments = false;
    }
    if (language_version < LanguageVersion::CSharp5) {
        async_await = false;
    }
    if (language_version < LanguageVersion::CSharp6) {
        await_in_catch_finally = false;
        null_propagation = false;
        getter_only_automatic_properties = false;
        dictionary_initializers = false;
        extension_methods_in_collection_initializers = false;
        string_interpolation = false;
        use_expression_body_for_calculated_getter_only_properties = false;
    }
    if (language_version < LanguageVersion::CSharp7) {
        use_ref_locals_for_accurate_order_of_evaluation = false;
        out_variables = false;
        discards = false;
        tuple_types = false;
        throw_expressions = false;
        tuple_conversions = false;
        local_functions = false;
        deconstruction = false;
        pattern_matching = false;
    }
    if (language_version < LanguageVersion::CSharp7_2) {
        ref_extension_methods = false;
        introduce_ref_modifiers_on_structs = false;
        introduce_readonly_and_in_modifiers = false;
        introduce_private_protected_accessibility = false;
        non_trailing_named_arguments = false;
    }
    if (language_version < LanguageVersion::CSharp7_3) {
        introduce_unmanaged_constraint = false;
        stack_alloc_initializers = false;
        pattern_based_fixed_statement = false;
        tuple_comparisons = false;
    }
    if (language_version < LanguageVersion::CSharp8_0) {
        switch_expressions = false;
        async_enumerator = false;
        use_enhanced_using = false;
        read_only_methods = false;
        async_using_and_for_each_statement = false;
        recursive_pattern_matching = false;
        static_local_functions = false;
        ranges = false;
        nullable_reference_types = false;
    }
    if (language_version < LanguageVersion::CSharp9_0) {
        native_integers = false;
        covariant_returns = false;
        init_accessors = false;
        record_classes = false;
        with_expressions = false;
        use_primary_constructor_syntax = false;
        function_pointers = false;
        for_each_with_get_enumerator_extension = false;
        pattern_combinators = false;
        relational_patterns = false;
    }
    if (language_version < LanguageVersion::CSharp10_0) {
        record_structs = false;
        struct_default_constructors_and_field_initializers = false;
        file_scoped_namespaces = false;
    }
    if (language_version < LanguageVersion::CSharp11_0) {
        numeric_int_ptr = false;
        scoped_ref = false;
        required_members = false;
        utf8_string_literals = false;
        switch_on_read_only_span_char = false;
        unsigned_right_shift = false;
        checked_operators = false;
    }
    if (language_version < LanguageVersion::CSharp12_0) {
        ref_read_only_parameters = false;
        use_primary_constructor_syntax_for_non_record_types = false;
        inline_arrays = false;
    }
    if (language_version < LanguageVersion::CSharp13_0) {
        params_collections = false;
    }
    if (language_version < LanguageVersion::CSharp14_0) {
        extension_members = false;
        first_class_span_types = false;
    }
}

LanguageVersion DecompilerSettings::get_minimum_required_version() const {
    if (extension_members || first_class_span_types)
        return LanguageVersion::CSharp14_0;
    if (params_collections)
        return LanguageVersion::CSharp13_0;
    if (ref_read_only_parameters || use_primary_constructor_syntax_for_non_record_types || inline_arrays)
        return LanguageVersion::CSharp12_0;
    if (numeric_int_ptr || scoped_ref || required_members || utf8_string_literals || switch_on_read_only_span_char || unsigned_right_shift || checked_operators)
        return LanguageVersion::CSharp11_0;
    if (record_structs || struct_default_constructors_and_field_initializers || file_scoped_namespaces)
        return LanguageVersion::CSharp10_0;
    if (native_integers || covariant_returns || init_accessors || record_classes || with_expressions || use_primary_constructor_syntax || function_pointers || for_each_with_get_enumerator_extension || pattern_combinators || relational_patterns)
        return LanguageVersion::CSharp9_0;
    if (switch_expressions || async_enumerator || use_enhanced_using || read_only_methods || async_using_and_for_each_statement || recursive_pattern_matching || static_local_functions || ranges || nullable_reference_types)
        return LanguageVersion::CSharp8_0;
    if (introduce_unmanaged_constraint || stack_alloc_initializers || pattern_based_fixed_statement || tuple_comparisons)
        return LanguageVersion::CSharp7_3;
    if (ref_extension_methods || introduce_ref_modifiers_on_structs || introduce_readonly_and_in_modifiers || introduce_private_protected_accessibility || non_trailing_named_arguments)
        return LanguageVersion::CSharp7_2;
    if (use_ref_locals_for_accurate_order_of_evaluation || out_variables || discards || tuple_types || throw_expressions || tuple_conversions || local_functions || deconstruction || pattern_matching)
        return LanguageVersion::CSharp7;
    if (await_in_catch_finally || null_propagation || getter_only_automatic_properties || dictionary_initializers || extension_methods_in_collection_initializers || string_interpolation || use_expression_body_for_calculated_getter_only_properties)
        return LanguageVersion::CSharp6;
    if (async_await)
        return LanguageVersion::CSharp5;
    if (dynamic || named_arguments || optional_arguments)
        return LanguageVersion::CSharp4;
    if (anonymous_types || use_lambda_syntax || expression_trees || automatic_properties || extension_methods || query_expressions || object_or_collection_initializers)
        return LanguageVersion::CSharp3;
    if (anonymous_methods || yield_return || lift_nullables || use_implicit_method_group_conversion || use_object_creation_of_generic_type_parameter)
        return LanguageVersion::CSharp2;
    if (decimal_constants || fixed_buffers || string_concat || automatic_events || using_statement || for_each_statement || lock_statement || switch_statement_on_string || sparse_integer_switch || using_declarations || array_initializers || expand_params_arguments || for_statement || do_while_statement)
        return LanguageVersion::CSharp1;
    return LanguageVersion::CSharp1;
}

}  // namespace cb::dotnet::decompiler
