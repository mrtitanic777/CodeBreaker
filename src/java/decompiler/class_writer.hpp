// java/decompiler/class_writer.hpp -- the writer of a class's Java source: its declaration (modifiers, generics, extends and implements), its fields
// with their initializers, its methods (signature, annotations, throws, body) and its nested classes, hidden members left out; and of a lambda written
// as an anonymous class.
//
//     ClassWriter writer;
//     TextBuffer out;
//     writer.class_to_java(root_node, out, /*indent=*/0, nullptr);   // "public class Foo { ... }"
//
// Ported from Fernflower (de.fernflower.main.ClassWriter), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). Where
// the original passes a tracer that may be null (the line mapping of the class's text) a pointer is taken; the others are references.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "java/decompiler/attributes.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/generics.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/annotations.hpp"
#include "java/decompiler/text_buffer.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class RenamerPoolInterceptor;   // renamer.hpp
class ClassReference14Processor;

class ClassWriter {
public:
    ClassWriter();

    // A lambda as an anonymous class (LAMBDA_TO_ANONYMOUS_CLASS) or as a lambda expression; `method_object` is what the lambda is applied to.
    void class_lambda_to_java(ClassNode* node, TextBuffer& buffer, Exprent* method_object, int indent, BytecodeMappingTracer& orig_tracer);
    // The class with its members; `tracer` (null for a root class): where the class's text starts, to number its lines.
    void class_to_java(ClassNode* node, TextBuffer& buffer, int indent, BytecodeMappingTracer* tracer);
    // A name made valid as a Java identifier ("<init>" and the like are replaced, a leading digit is escaped ...).
    static std::string to_valid_java_identifier(const std::string& name);

private:
    // The class-level passes before writing: initializers, class literals (1.4), enums, assertions.
    void invoke_processors(ClassNode* node);
    static void add_tracer(const StructClass* cls, const StructMethod* method, BytecodeMappingTracer& tracer);
    void write_class_definition(ClassNode* node, TextBuffer& buffer, int indent);
    void field_to_java(ClassWrapper* wrapper, const StructClass* cl, const StructField* fd, TextBuffer& buffer, int indent, BytecodeMappingTracer& tracer);
    static void method_lambda_to_java(ClassNode* lambda_node, ClassWrapper* class_wrapper, const StructMethod* mt, TextBuffer& buffer, int indent,
                                      bool code_only, BytecodeMappingTracer& tracer);
    // Writes a method; false when it is left out (hidden).
    bool method_to_java(ClassNode* node, const StructMethod* mt, TextBuffer& buffer, int indent, BytecodeMappingTracer& tracer);
    static bool search_for_method(const StructClass* cl, const std::string& name, const MethodDescriptor& md, bool search);
    static void map_lines(TextBuffer& code, const LineNumberTableAttribute& table, BytecodeMappingTracer& tracer, int start_line);
    static bool hide_constructor(ClassWrapper* wrapper, bool init, bool throws_exceptions, int param_count);
    static void append_deprecation(TextBuffer& buffer, int indent);

    enum class MType { CLASS, FIELD, METHOD };
    // "// $FF: renamed from: ..." when the renamer changed a name (none: nothing).
    static void append_rename_comment(TextBuffer& buffer, const std::optional<std::string>& old_name, MType type, int indent);
    static std::string get_type_print_out(const VarType& type);
    static void append_comment(TextBuffer& buffer, const std::string& comment, int indent);
    static void append_annotations(TextBuffer& buffer, const StructMember& mb, int indent);
    static void append_parameter_annotations(TextBuffer& buffer, const StructMethod* mt, int param);
    static void append_modifiers(TextBuffer& buffer, int flags, int allowed, bool is_interface, int excluded);
    static void append_type_parameters(TextBuffer& buffer, const std::vector<std::string>& parameters, const std::vector<std::vector<GenericType>>& bounds);

    static constexpr std::string_view ANNOTATION_ATTRIBUTES[2] = {attr::RUNTIME_VISIBLE_ANNOTATIONS, attr::RUNTIME_INVISIBLE_ANNOTATIONS};
    static constexpr std::string_view PARAMETER_ANNOTATION_ATTRIBUTES[2] = {attr::RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS,
                                                                           attr::RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS};
    // The modifiers in the order they are written.
    static const std::vector<std::pair<int, std::string_view>>& modifiers();

    static constexpr int CLASS_ALLOWED = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_ABSTRACT | cc::ACC_STATIC | cc::ACC_FINAL |
                                         cc::ACC_STRICT;
    static constexpr int FIELD_ALLOWED = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_STATIC | cc::ACC_FINAL | cc::ACC_TRANSIENT |
                                         cc::ACC_VOLATILE;
    static constexpr int METHOD_ALLOWED = cc::ACC_PUBLIC | cc::ACC_PROTECTED | cc::ACC_PRIVATE | cc::ACC_ABSTRACT | cc::ACC_STATIC | cc::ACC_FINAL |
                                          cc::ACC_SYNCHRONIZED | cc::ACC_NATIVE | cc::ACC_STRICT;
    static constexpr int CLASS_EXCLUDED = cc::ACC_ABSTRACT | cc::ACC_STATIC;
    static constexpr int FIELD_EXCLUDED = cc::ACC_PUBLIC | cc::ACC_STATIC | cc::ACC_FINAL;
    static constexpr int METHOD_EXCLUDED = cc::ACC_PUBLIC | cc::ACC_ABSTRACT;

    std::unique_ptr<ClassReference14Processor> ref14processor_;
    std::shared_ptr<const PoolInterceptor> interceptor_;   // the renamer's (a RenamerPoolInterceptor) or null: for "renamed from" comments
};

}  // namespace cb::java::decompiler
