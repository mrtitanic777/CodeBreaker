// java/decompiler/class_writer.cpp -- see class_writer.hpp. Ported from Fernflower (de.fernflower.main.ClassWriter), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/class_writer.hpp"

#include <algorithm>
#include <stdexcept>

#include "java/decompiler/annotations.hpp"
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/renamer.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {

// Restores the context's current class node when the scope ends (the original's `finally`).
struct ClassNodeScope {
    explicit ClassNodeScope(ClassNode* node) : outer(DecompilerContext::current().current_class_node) { DecompilerContext::current().current_class_node = node; }
    ~ClassNodeScope() { DecompilerContext::current().current_class_node = outer; }
    ClassNodeScope(const ClassNodeScope&) = delete;
    ClassNodeScope& operator=(const ClassNodeScope&) = delete;
    ClassNode* outer;
};

// Restores the context's current method wrapper when the scope ends.
struct MethodWrapperScope {
    explicit MethodWrapperScope(MethodWrapper* wrapper) : outer(DecompilerContext::current().current_method_wrapper) {
        DecompilerContext::current().current_method_wrapper = wrapper;
    }
    ~MethodWrapperScope() { DecompilerContext::current().current_method_wrapper = outer; }
    MethodWrapperScope(const MethodWrapperScope&) = delete;
    MethodWrapperScope& operator=(const MethodWrapperScope&) = delete;
    MethodWrapper* outer;
};

// Decodes UTF-8 into the code points Java sees as characters (a supplementary one is two surrogates there, which are neither start nor part).
std::vector<std::uint32_t> decode_utf8(const std::string& s) {
    std::vector<std::uint32_t> out;
    for (std::size_t i = 0; i < s.size();) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        std::uint32_t cp = c;
        std::size_t len = 1;
        if (c >= 0xF0 && i + 3 < s.size() + 0) {
            cp = (c & 0x07u) << 18 | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12 | (static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6 |
                 (static_cast<unsigned char>(s[i + 3]) & 0x3Fu);
            len = 4;
        } else if (c >= 0xE0 && i + 2 < s.size()) {
            cp = (c & 0x0Fu) << 12 | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6 | (static_cast<unsigned char>(s[i + 2]) & 0x3Fu);
            len = 3;
        } else if (c >= 0xC0 && i + 1 < s.size()) {
            cp = (c & 0x1Fu) << 6 | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu);
            len = 2;
        }
        out.push_back(cp);
        i += len;
    }
    return out;
}

// Character.isJavaIdentifierStart. (Exact for ASCII; beyond it a printable character that is not a symbol, punctuation or a digit counts as a letter:
// there is no Unicode category table here.)
bool is_java_identifier_start(std::uint32_t c) {
    if (c < 0x80) return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '$' || c == '_';
    if (c > 0xFFFF) return false;   // (a surrogate in Java)
    if (c <= 0xBF) return c == 0xAA || c == 0xB5 || c == 0xBA || (c >= 0xA2 && c <= 0xA5);   // letters and currency of Latin-1
    if (c == 0xD7 || c == 0xF7) return false;
    if (c >= 0x2000 && c <= 0x2BFF) return c >= 0x20A0 && c <= 0x20CF;   // currency symbols only
    if (c >= 0x3000 && c <= 0x303F) return c == 0x3005 || c == 0x3006 || c == 0x3007 || (c >= 0x3021 && c <= 0x3029);
    if (c >= 0x0300 && c <= 0x036F) return false;   // combining marks
    return is_printable_unicode(c);
}

// Character.isJavaIdentifierPart.
bool is_java_identifier_part(std::uint32_t c) {
    if (c < 0x80) return is_java_identifier_start(c) || (c >= '0' && c <= '9') || c <= 8 || (c >= 0x0E && c <= 0x1B) || c == 0x7F;
    if (c >= 0x80 && c <= 0x9F) return true;   // identifier-ignorable controls
    if (c >= 0x0300 && c <= 0x036F) return true;
    if ((c >= 0x0660 && c <= 0x0669) || (c >= 0x06F0 && c <= 0x06F9) || (c >= 0x0966 && c <= 0x096F) || (c >= 0xFF10 && c <= 0xFF19)) return true;
    return is_java_identifier_start(c);
}

void append_utf8(std::string& out, std::uint32_t c) {
    if (c < 0x80) {
        out.push_back(static_cast<char>(c));
    } else if (c < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (c >> 6)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else if (c < 0x10000) {
        out.push_back(static_cast<char>(0xE0 | (c >> 12)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (c >> 18)));
        out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
}

// String.split(" ") (a trailing empty part is dropped).
std::vector<std::string> split_space(const std::string& s) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
        const std::size_t pos = s.find(' ', start);
        if (pos == std::string::npos) {
            parts.push_back(s.substr(start));
            break;
        }
        parts.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    while (!parts.empty() && parts.back().empty()) parts.pop_back();
    return parts;
}

}  // namespace

ClassWriter::ClassWriter() : ref14processor_(std::make_unique<ClassReference14Processor>()), interceptor_(DecompilerContext::current().pool_interceptor) {}

const std::vector<std::pair<int, std::string_view>>& ClassWriter::modifiers() {
    static const std::vector<std::pair<int, std::string_view>> m = {
        {cc::ACC_PUBLIC, "public"},     {cc::ACC_PROTECTED, "protected"}, {cc::ACC_PRIVATE, "private"},         {cc::ACC_ABSTRACT, "abstract"},
        {cc::ACC_STATIC, "static"},     {cc::ACC_FINAL, "final"},         {cc::ACC_STRICT, "strictfp"},         {cc::ACC_TRANSIENT, "transient"},
        {cc::ACC_VOLATILE, "volatile"}, {cc::ACC_SYNCHRONIZED, "synchronized"}, {cc::ACC_NATIVE, "native"}};
    return m;
}

void ClassWriter::invoke_processors(ClassNode* node) {
    ClassWrapper* wrapper = node->get_wrapper();
    const StructClass* cl = wrapper->get_class_struct();

    InitializerProcessor::extract_initializers(wrapper);

    if (node->type == ClassNode::CLASS_ROOT && DecompilerContext::option(pref::DECOMPILE_CLASS_1_4)) {
        ref14processor_->process_class_references(node);
    }

    if (cl->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM)) {
        EnumProcessor::clear_enum(wrapper);
    }

    if (DecompilerContext::option(pref::DECOMPILE_ASSERTIONS)) {
        AssertProcessor::build_assertions(node);
    }
}

void ClassWriter::class_lambda_to_java(ClassNode* node, TextBuffer& buffer, Exprent* method_object, int indent, BytecodeMappingTracer& orig_tracer) {
    ClassWrapper* wrapper = node->get_wrapper();
    if (wrapper == nullptr) return;

    const bool lambda_to_anonymous = DecompilerContext::option(pref::LAMBDA_TO_ANONYMOUS_CLASS);

    {
        ClassNodeScope scope(node);

        BytecodeMappingTracer tracer(orig_tracer.get_current_source_line());

        const StructClass* cl = wrapper->get_class_struct();

        DecompilerContext::get_logger().start_write_class(node->simple_name.value_or("null"));

        if (node->lambda_information->is_method_reference) {
            if (!node->lambda_information->is_content_method_static && method_object != nullptr) {
                // reference to a virtual method
                buffer.append(method_object->to_java(indent, tracer));
            } else {
                // reference to a static method
                buffer.append(ExprProcessor::get_cast_type_name(VarType::parse(node->lambda_information->content_class_name, false)));
            }

            buffer.append("::");
            buffer.append(node->lambda_information->content_method_name);
        } else {
            // lambda method
            const StructMethod* mt = cl->get_method(node->lambda_information->content_method_key);
            MethodWrapper* method_wrapper = wrapper->get_method_wrapper(mt->name(), mt->descriptor());
            const MethodDescriptor md_content = MethodDescriptor::parse(node->lambda_information->content_method_descriptor);
            const MethodDescriptor md_lambda = MethodDescriptor::parse(node->lambda_information->method_descriptor);

            if (!lambda_to_anonymous) {
                buffer.append('(');

                bool first_parameter = true;
                int index = node->lambda_information->is_content_method_static ? 0 : 1;
                const int start_index = static_cast<int>(md_content.params.size()) - static_cast<int>(md_lambda.params.size());

                for (int i = 0; i < static_cast<int>(md_content.params.size()); ++i) {
                    if (i >= start_index) {
                        if (!first_parameter) buffer.append(", ");

                        const std::optional<std::string> parameter_name = method_wrapper->varproc->get_var_name(VarVersionPair(index, 0));
                        buffer.append(!parameter_name ? "param" + std::to_string(index) : *parameter_name);   // null iff decompiled with errors

                        first_parameter = false;
                    }

                    index += md_content.params[static_cast<std::size_t>(i)].stack_size;
                }

                buffer.append(") ->");
            }

            buffer.append(" {").append_line_separator();
            tracer.increment_current_source_line();

            method_lambda_to_java(node, wrapper, mt, buffer, indent + 1, !lambda_to_anonymous, tracer);

            buffer.append_indent(indent).append("}");

            add_tracer(cl, mt, tracer);
        }
    }

    DecompilerContext::get_logger().end_write_class();
}

void ClassWriter::add_tracer(const StructClass* cls, const StructMethod* method, BytecodeMappingTracer& tracer) {
    const LineNumberTableAttribute* line_number_table = method->attribute<LineNumberTableAttribute>(attr::LINE_NUMBER_TABLE);
    tracer.set_line_number_table(line_number_table);
    DecompilerContext::current().bytecode_source_mapper->add_tracer(cls->qualified_name, InterpreterUtil::make_unique_key(method->name(), method->descriptor()), tracer);
}

void ClassWriter::class_to_java(ClassNode* node, TextBuffer& buffer, int indent, BytecodeMappingTracer* tracer) {
    {
        ClassNodeScope scope(node);

        int start_line = tracer != nullptr ? tracer->get_current_source_line() : 0;
        BytecodeMappingTracer dummy_tracer(start_line);

        // last minute processing
        invoke_processors(node);

        ClassWrapper* wrapper = node->get_wrapper();
        const StructClass* cl = wrapper->get_class_struct();

        DecompilerContext::get_logger().start_write_class(cl->qualified_name);

        // write class definition
        const std::size_t start_class_def = buffer.length();
        write_class_definition(node, buffer, indent);

        //      // count lines in class definition the easiest way
        //      startLine = buffer.substring(start_class_def).toString().split(lineSeparator, -1).length - 1;

        bool has_content = false;

        // fields
        bool enum_fields = false;

        dummy_tracer.increment_current_source_line(buffer.count_lines(start_class_def));

        for (const auto& fd_ptr : cl->fields()) {
            const StructField* fd = fd_ptr.get();
            const bool hide = (fd->is_synthetic() && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) ||
                              wrapper->get_hidden_members().contains(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
            if (hide) continue;

            const bool is_enum = fd->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
            if (is_enum) {
                if (enum_fields) {
                    buffer.append(',').append_line_separator();
                    dummy_tracer.increment_current_source_line();
                }
                enum_fields = true;
            } else if (enum_fields) {
                buffer.append(';');
                buffer.append_line_separator();
                buffer.append_line_separator();
                dummy_tracer.increment_current_source_line(2);
                enum_fields = false;
            }

            field_to_java(wrapper, cl, fd, buffer, indent + 1, dummy_tracer);   // FIXME (the original's): insert real tracer

            has_content = true;
        }

        if (enum_fields) {
            buffer.append(';').append_line_separator();
            dummy_tracer.increment_current_source_line();
        }

        // FIXME (the original's): fields don't matter at the moment
        start_line += buffer.count_lines(start_class_def);

        // methods
        for (const auto& mt_ptr : cl->methods()) {
            const StructMethod* mt = mt_ptr.get();
            const bool hide = (mt->is_synthetic() && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) ||
                              (mt->has_modifier(cc::ACC_BRIDGE) && DecompilerContext::option(pref::REMOVE_BRIDGE)) ||
                              wrapper->get_hidden_members().contains(InterpreterUtil::make_unique_key(mt->name(), mt->descriptor()));
            if (hide) continue;

            const std::size_t position = buffer.length();
            const int stored_line = start_line;
            if (has_content) {
                buffer.append_line_separator();
                ++start_line;
            }
            BytecodeMappingTracer method_tracer(start_line);
            const bool method_skipped = !method_to_java(node, mt, buffer, indent + 1, method_tracer);
            if (!method_skipped) {
                has_content = true;
                add_tracer(cl, mt, method_tracer);
                start_line = method_tracer.get_current_source_line();
            } else {
                buffer.set_length(position);
                start_line = stored_line;
            }
        }

        // member classes
        for (ClassNode* inner : node->nested) {
            if (inner->type == ClassNode::CLASS_MEMBER) {
                const StructClass* inner_cl = inner->class_struct;
                const bool is_synthetic = (inner->access & cc::ACC_SYNTHETIC) != 0 || inner_cl->is_synthetic() || inner->nameless_constructor_stub;
                const bool hide = (is_synthetic && DecompilerContext::option(pref::REMOVE_SYNTHETIC)) || wrapper->get_hidden_members().contains(inner_cl->qualified_name);
                if (hide) continue;

                if (has_content) {
                    buffer.append_line_separator();
                    ++start_line;
                }
                BytecodeMappingTracer class_tracer(start_line);
                class_to_java(inner, buffer, indent + 1, &class_tracer);
                start_line = buffer.count_lines();

                has_content = true;
            }
        }

        buffer.append_indent(indent).append('}');

        if (node->type != ClassNode::CLASS_ANONYMOUS) buffer.append_line_separator();
    }

    DecompilerContext::get_logger().end_write_class();
}

void ClassWriter::write_class_definition(ClassNode* node, TextBuffer& buffer, int indent) {
    if (node->type == ClassNode::CLASS_ANONYMOUS) {
        buffer.append(" {").append_line_separator();
        return;
    }

    ClassWrapper* wrapper = node->get_wrapper();
    const StructClass* cl = wrapper->get_class_struct();

    int flags = node->type == ClassNode::CLASS_ROOT ? cl->access_flags() : node->access;
    const bool is_deprecated = cl->attributes().contains_key(std::string("Deprecated"));
    const bool is_synthetic = (flags & cc::ACC_SYNTHETIC) != 0 || cl->attributes().contains_key(std::string("Synthetic"));
    const bool is_enum = DecompilerContext::option(pref::DECOMPILE_ENUM) && (flags & cc::ACC_ENUM) != 0;
    const bool is_interface = (flags & cc::ACC_INTERFACE) != 0;
    const bool is_annotation = (flags & cc::ACC_ANNOTATION) != 0;

    if (is_deprecated) append_deprecation(buffer, indent);

    if (const auto* renamer = dynamic_cast<const RenamerPoolInterceptor*>(interceptor_.get())) {
        const std::optional<std::string> old_name = renamer->get_old_name(cl->qualified_name);
        append_rename_comment(buffer, old_name, MType::CLASS, indent);
    }

    if (is_synthetic) append_comment(buffer, "synthetic class", indent);

    append_annotations(buffer, *cl, indent);

    buffer.append_indent(indent);

    if (is_enum) {
        // remove abstract and final flags (JLS 8.9 Enums)
        flags &= ~cc::ACC_ABSTRACT;
        flags &= ~cc::ACC_FINAL;
    }

    append_modifiers(buffer, flags, CLASS_ALLOWED, is_interface, CLASS_EXCLUDED);

    if (is_enum) {
        buffer.append("enum ");
    } else if (is_interface) {
        if (is_annotation) buffer.append('@');
        buffer.append("interface ");
    } else {
        buffer.append("class ");
    }

    std::optional<GenericClassDescriptor> descriptor;
    if (DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES)) {
        const GenericSignatureAttribute* attr_sig = cl->attribute<GenericSignatureAttribute>("Signature");
        if (attr_sig != nullptr) descriptor = generics::parse_class_signature(attr_sig->signature());
    }

    buffer.append(node->simple_name.value_or("null"));

    if (descriptor && !descriptor->fparameters.empty()) append_type_parameters(buffer, descriptor->fparameters, descriptor->fbounds);

    buffer.append(' ');

    if (!is_enum && !is_interface && cl->super_class != nullptr) {
        const VarType supertype = VarType::parse(cl->super_class->get_string(), true);
        if (!(VarType::OBJECT == supertype)) {
            buffer.append("extends ");
            if (descriptor) {
                buffer.append(generics::cast_type_name(descriptor->superclass, current_name_context()));
            } else {
                buffer.append(ExprProcessor::get_cast_type_name(supertype));
            }
            buffer.append(' ');
        }
    }

    if (!is_annotation) {
        const std::vector<int>& interfaces = cl->interfaces();
        if (!interfaces.empty()) {
            buffer.append(is_interface ? "extends " : "implements ");
            for (std::size_t i = 0; i < interfaces.size(); ++i) {
                if (i > 0) buffer.append(", ");
                if (descriptor) {
                    buffer.append(generics::cast_type_name(descriptor->superinterfaces.at(i), current_name_context()));
                } else {
                    buffer.append(ExprProcessor::get_cast_type_name(VarType::parse(cl->get_interface(i), true)));
                }
            }
            buffer.append(' ');
        }
    }

    buffer.append('{').append_line_separator();
}

void ClassWriter::field_to_java(ClassWrapper* wrapper, const StructClass* cl, const StructField* fd, TextBuffer& buffer, int indent, BytecodeMappingTracer& tracer) {
    const std::size_t start = buffer.length();
    const bool is_interface = cl->has_modifier(cc::ACC_INTERFACE);
    const bool is_deprecated = fd->attributes().contains_key(std::string("Deprecated"));
    const bool is_enum = fd->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);

    if (is_deprecated) append_deprecation(buffer, indent);

    if (const auto* renamer = dynamic_cast<const RenamerPoolInterceptor*>(interceptor_.get())) {
        const std::optional<std::string> old_name = renamer->get_old_name(cl->qualified_name + " " + fd->name() + " " + fd->descriptor());
        append_rename_comment(buffer, old_name, MType::FIELD, indent);
    }

    if (fd->is_synthetic()) append_comment(buffer, "synthetic field", indent);

    append_annotations(buffer, *fd, indent);

    buffer.append_indent(indent);

    if (!is_enum) append_modifiers(buffer, fd->access_flags(), FIELD_ALLOWED, is_interface, FIELD_EXCLUDED);

    const VarType field_type = VarType::parse(fd->descriptor(), false);

    std::optional<GenericFieldDescriptor> descriptor;
    if (DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES)) {
        const GenericSignatureAttribute* attr_sig = fd->attribute<GenericSignatureAttribute>("Signature");
        if (attr_sig != nullptr) descriptor = generics::parse_field_signature(attr_sig->signature());
    }

    if (!is_enum) {
        if (descriptor) {
            buffer.append(generics::cast_type_name(descriptor->type, current_name_context()));
        } else {
            buffer.append(ExprProcessor::get_cast_type_name(field_type));
        }
        buffer.append(' ');
    }

    buffer.append(fd->name());

    tracer.increment_current_source_line(buffer.count_lines(start));

    Exprent* initializer;
    if (fd->has_modifier(cc::ACC_STATIC)) {
        initializer = wrapper->get_static_field_initializers().get_with_key(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
    } else {
        initializer = wrapper->get_dynamic_field_initializers().get_with_key(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
    }

    if (initializer != nullptr) {
        if (is_enum && initializer->type == Exprent::EXPRENT_NEW) {
            auto* nexpr = static_cast<NewExprent*>(initializer);
            nexpr->set_enum_const(true);
            buffer.append(nexpr->to_java(indent, tracer));
        } else {
            buffer.append(" = ");
            // FIXME (the original's): special case field initializer. Can map to more than one method (constructor) and bytecode intruction.
            buffer.append(initializer->to_java(indent, tracer));
        }
    } else if (fd->has_modifier(cc::ACC_FINAL) && fd->has_modifier(cc::ACC_STATIC)) {
        const ConstantValueAttribute* attr_cv = fd->attribute<ConstantValueAttribute>(attr::CONSTANT_VALUE);
        if (attr_cv != nullptr) {
            const PrimitiveConstant* constant = cl->pool().primitive(static_cast<std::size_t>(attr_cv->index()));
            buffer.append(" = ");
            buffer.append(make<ConstExprent>(field_type, constant->value, std::nullopt)->to_java(indent, tracer));
        }
    }

    if (!is_enum) {
        buffer.append(";").append_line_separator();
        tracer.increment_current_source_line();
    }
}

void ClassWriter::method_lambda_to_java(ClassNode* lambda_node, ClassWrapper* class_wrapper, const StructMethod* mt, TextBuffer& buffer, int indent, bool code_only,
                                        BytecodeMappingTracer& tracer) {
    MethodWrapper* method_wrapper = class_wrapper->get_method_wrapper(mt->name(), mt->descriptor());

    MethodWrapperScope scope(method_wrapper);

    const std::string method_name = lambda_node->lambda_information->method_name;
    const MethodDescriptor md_content = MethodDescriptor::parse(lambda_node->lambda_information->content_method_descriptor);
    const MethodDescriptor md_lambda = MethodDescriptor::parse(lambda_node->lambda_information->method_descriptor);

    if (!code_only) {
        buffer.append_indent(indent);
        buffer.append("public ");
        buffer.append(method_name);
        buffer.append("(");

        bool first_parameter = true;
        int index = lambda_node->lambda_information->is_content_method_static ? 0 : 1;
        const int start_index = static_cast<int>(md_content.params.size()) - static_cast<int>(md_lambda.params.size());

        for (int i = 0; i < static_cast<int>(md_content.params.size()); ++i) {
            if (i >= start_index) {
                if (!first_parameter) buffer.append(", ");

                std::string type_name = ExprProcessor::get_cast_type_name(md_content.params[static_cast<std::size_t>(i)].copy());
                if (ExprProcessor::UNDEFINED_TYPE_STRING == type_name && DecompilerContext::option(pref::UNDEFINED_PARAM_TYPE_OBJECT)) {
                    type_name = ExprProcessor::get_cast_type_name(VarType::OBJECT);
                }

                buffer.append(type_name);
                buffer.append(" ");

                const std::optional<std::string> parameter_name = method_wrapper->varproc->get_var_name(VarVersionPair(index, 0));
                buffer.append(!parameter_name ? "param" + std::to_string(index) : *parameter_name);   // null iff decompiled with errors

                first_parameter = false;
            }

            index += md_content.params[static_cast<std::size_t>(i)].stack_size;
        }

        buffer.append(") {").append_line_separator();

        indent += 1;
    }

    if (!method_wrapper->decompiled_with_errors) {
        RootStatement* root = class_wrapper->get_method_wrapper(mt->name(), mt->descriptor())->root;
        if (root != nullptr) {   // check for existence
            try {
                buffer.append(root->to_java(indent, tracer));
            } catch (const std::exception& ex) {
                DecompilerContext::get_logger().write_message("Method " + mt->name() + " " + mt->descriptor() + " couldn't be written.", ex);
                method_wrapper->decompiled_with_errors = true;
            }
        }
    }

    if (method_wrapper->decompiled_with_errors) {
        buffer.append_indent(indent);
        buffer.append("// $FF: Couldn't be decompiled");
        buffer.append_line_separator();
    }

    if (!code_only) {
        indent -= 1;

        buffer.append_indent(indent);
        buffer.append('}');
        buffer.append_line_separator();
    }
}

std::string ClassWriter::to_valid_java_identifier(const std::string& name) {
    if (name.empty()) return name;

    bool changed = false;
    std::string res;
    res.reserve(name.size());
    const std::vector<std::uint32_t> chars = decode_utf8(name);
    for (std::size_t i = 0; i < chars.size(); ++i) {
        const std::uint32_t c = chars[i];
        // (a supplementary character is two surrogate chars in Java: each is replaced)
        const int units = c > 0xFFFF ? 2 : 1;
        if ((i == 0 && !is_java_identifier_start(c)) || (i > 0 && !is_java_identifier_part(c))) {
            changed = true;
            for (int u = 0; u < units; ++u) res.append("_");
        } else {
            append_utf8(res, c);
        }
    }
    if (!changed) return name;

    return res + "/* $FF was: " + name + "*/";
}

bool ClassWriter::method_to_java(ClassNode* node, const StructMethod* mt, TextBuffer& buffer, int indent, BytecodeMappingTracer& tracer) {
    ClassWrapper* wrapper = node->get_wrapper();
    const StructClass* cl = wrapper->get_class_struct();
    MethodWrapper* method_wrapper = wrapper->get_method_wrapper(mt->name(), mt->descriptor());

    bool hide_method = false;
    const std::size_t start_index_method = buffer.length();

    {
        MethodWrapperScope scope(method_wrapper);

        const bool is_interface = cl->has_modifier(cc::ACC_INTERFACE);
        const bool is_annotation = cl->has_modifier(cc::ACC_ANNOTATION);
        const bool is_enum = cl->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
        const bool is_deprecated = mt->attributes().contains_key(std::string("Deprecated"));
        bool clinit = false, init = false, dinit = false;

        const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());

        int flags = mt->access_flags();
        if ((flags & cc::ACC_NATIVE) != 0) {
            flags &= ~cc::ACC_STRICT;   // compiler bug: a strictfp class sets all methods to strictfp
        }
        if (cc::CLINIT_NAME == mt->name()) {
            flags &= cc::ACC_STATIC;   // ignore all modifiers except 'static' in a static initializer
        }

        if (is_deprecated) append_deprecation(buffer, indent);

        if (const auto* renamer = dynamic_cast<const RenamerPoolInterceptor*>(interceptor_.get())) {
            const std::optional<std::string> old_name = renamer->get_old_name(cl->qualified_name + " " + mt->name() + " " + mt->descriptor());
            append_rename_comment(buffer, old_name, MType::METHOD, indent);
        }

        const bool is_synthetic = (flags & cc::ACC_SYNTHETIC) != 0 || mt->attributes().contains_key(std::string("Synthetic"));
        const bool is_bridge = (flags & cc::ACC_BRIDGE) != 0;
        if (is_synthetic) append_comment(buffer, "synthetic method", indent);
        if (is_bridge) append_comment(buffer, "bridge method", indent);

        append_annotations(buffer, *mt, indent);

        // Try append @Override after all other annotations
        if (DecompilerContext::option(pref::OVERRIDE_ANNOTATION) && cl->bytecode_version() >= cc::BYTECODE_JAVA_5 && cc::INIT_NAME != mt->name() &&
            cc::CLINIT_NAME != mt->name() && !mt->has_modifier(cc::ACC_STATIC) && !mt->has_modifier(cc::ACC_PRIVATE)) {
            // Search superclasses for methods that match the name and descriptor of this one.
            // Make sure not to search the current class otherwise it will return the current method itself!
            // TODO (the original's): record overrides
            const bool is_override = search_for_method(cl, mt->name(), md, false);
            if (is_override) {
                buffer.append_indent(indent);
                buffer.append("@Override");
                buffer.append_line_separator();
            }
        }

        buffer.append_indent(indent);

        append_modifiers(buffer, flags, METHOD_ALLOWED, is_interface, METHOD_EXCLUDED);

        if (is_interface && mt->contains_code()) {
            // 'default' modifier (Java 8)
            buffer.append("default ");
        }

        std::string name = mt->name();
        if (cc::INIT_NAME == name) {
            if (node->type == ClassNode::CLASS_ANONYMOUS) {
                name = "";
                dinit = true;
            } else {
                name = node->simple_name.value_or("null");
                init = true;
            }
        } else if (cc::CLINIT_NAME == name) {
            name = "";
            clinit = true;
        }

        std::optional<GenericMethodDescriptor> descriptor;
        if (DecompilerContext::option(pref::DECOMPILE_GENERIC_SIGNATURES)) {
            const GenericSignatureAttribute* attr_sig = mt->attribute<GenericSignatureAttribute>("Signature");
            if (attr_sig != nullptr) {
                descriptor = generics::parse_method_signature(attr_sig->signature());
                if (descriptor) {
                    int actual_params = static_cast<int>(md.params.size());
                    const SignatureFields& sig_fields = method_wrapper->signature_fields;
                    if (sig_fields) {
                        actual_params = 0;
                        for (const std::optional<VarVersionPair>& field : *sig_fields) {
                            if (!field) ++actual_params;
                        }
                    } else if (is_enum && init) {
                        actual_params -= 2;
                    }

                    if (actual_params != static_cast<int>(descriptor->params.size())) {
                        const std::string message = "Inconsistent generic signature in method " + mt->name() + " " + mt->descriptor() + " in " + cl->qualified_name;
                        DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                        descriptor.reset();
                    }
                }
            }
        }

        bool throws_exceptions = false;
        int param_count = 0;

        if (!clinit && !dinit) {
            const bool this_var = !mt->has_modifier(cc::ACC_STATIC);

            if (descriptor && !descriptor->fparameters.empty()) {
                append_type_parameters(buffer, descriptor->fparameters, descriptor->fbounds);
                buffer.append(' ');
            }

            if (!init) {
                if (descriptor) {
                    buffer.append(generics::cast_type_name(descriptor->ret, current_name_context()));
                } else {
                    buffer.append(ExprProcessor::get_cast_type_name(md.ret));
                }
                buffer.append(' ');
            }

            buffer.append(to_valid_java_identifier(name));
            buffer.append('(');

            // parameters
            const SignatureFields& sign_fields = method_wrapper->signature_fields;

            int last_visible_parameter_index = -1;
            for (int i = 0; i < static_cast<int>(md.params.size()); ++i) {
                if (!sign_fields || !sign_fields->at(static_cast<std::size_t>(i))) last_visible_parameter_index = i;
            }

            bool first_parameter = true;
            int index = is_enum && init ? 3 : this_var ? 1 : 0;
            const bool has_descriptor = descriptor.has_value();
            const int start = is_enum && init && !has_descriptor ? 2 : 0;
            const int params = has_descriptor ? static_cast<int>(descriptor->params.size()) : static_cast<int>(md.params.size());
            for (int i = start; i < params; ++i) {
                if (has_descriptor || (!sign_fields || !sign_fields->at(static_cast<std::size_t>(i)))) {
                    if (!first_parameter) buffer.append(", ");

                    append_parameter_annotations(buffer, mt, param_count);

                    if (method_wrapper->varproc->get_var_final(VarVersionPair(index, 0)) == VarTypeProcessor::VAR_EXPLICIT_FINAL) buffer.append("final ");

                    if (descriptor) {
                        GenericType parameter_type = descriptor->params.at(static_cast<std::size_t>(i));

                        const bool is_var_arg = (i == last_visible_parameter_index && mt->has_modifier(cc::ACC_VARARGS) && parameter_type.array_dim > 0);
                        if (is_var_arg) parameter_type = parameter_type.decrease_array_dim();

                        std::string type_name = generics::cast_type_name(parameter_type, current_name_context());
                        if (ExprProcessor::UNDEFINED_TYPE_STRING == type_name && DecompilerContext::option(pref::UNDEFINED_PARAM_TYPE_OBJECT)) {
                            type_name = ExprProcessor::get_cast_type_name(VarType::OBJECT);
                        }

                        buffer.append(type_name);

                        if (is_var_arg) buffer.append("...");
                    } else {
                        VarType parameter_type = md.params[static_cast<std::size_t>(i)];

                        const bool is_var_arg = (i == last_visible_parameter_index && mt->has_modifier(cc::ACC_VARARGS) && parameter_type.array_dim > 0);
                        if (is_var_arg) parameter_type = parameter_type.decrease_array_dim();

                        std::string type_name = ExprProcessor::get_cast_type_name(parameter_type);
                        if (ExprProcessor::UNDEFINED_TYPE_STRING == type_name && DecompilerContext::option(pref::UNDEFINED_PARAM_TYPE_OBJECT)) {
                            type_name = ExprProcessor::get_cast_type_name(VarType::OBJECT);
                        }

                        buffer.append(type_name);

                        if (is_var_arg) buffer.append("...");
                    }

                    buffer.append(' ');
                    const std::optional<std::string> parameter_name = method_wrapper->varproc->get_var_name(VarVersionPair(index, 0));
                    buffer.append(!parameter_name ? "param" + std::to_string(index) : *parameter_name);   // null iff decompiled with errors

                    first_parameter = false;
                    ++param_count;
                }

                index += md.params[static_cast<std::size_t>(i)].stack_size;
            }

            buffer.append(')');

            const ExceptionsAttribute* attr_exc = mt->attribute<ExceptionsAttribute>("Exceptions");
            if ((descriptor && !descriptor->exceptions.empty()) || attr_exc != nullptr) {
                throws_exceptions = true;
                buffer.append(" throws ");

                if (attr_exc == nullptr) throw std::runtime_error("NullPointerException");
                for (std::size_t i = 0; i < attr_exc->throws_exceptions().size(); ++i) {
                    if (i > 0) buffer.append(", ");
                    if (descriptor && !descriptor->exceptions.empty()) {
                        const GenericType& type = descriptor->exceptions.at(i);
                        buffer.append(generics::cast_type_name(type, current_name_context()));
                    } else {
                        const VarType type = VarType::parse(attr_exc->exception_class_name(i, cl->pool()), true);
                        buffer.append(ExprProcessor::get_cast_type_name(type));
                    }
                }
            }
        }

        tracer.increment_current_source_line(buffer.count_lines(start_index_method));

        if ((flags & (cc::ACC_ABSTRACT | cc::ACC_NATIVE)) != 0) {   // native or abstract method (explicit or interface)
            if (is_annotation) {
                const AnnotationDefaultAttribute* attr_def = mt->attribute<AnnotationDefaultAttribute>("AnnotationDefault");
                if (attr_def != nullptr) {
                    buffer.append(" default ");
                    BytecodeMappingTracer dummy;   // dummy tracer
                    buffer.append(attr_def->get_default_value()->to_java(indent + 1, dummy));
                }
            }

            buffer.append(';');
            buffer.append_line_separator();
            tracer.increment_current_source_line();
        } else {
            if (!clinit && !dinit) buffer.append(' ');

            // We do not have line information for method start, lets have it here for now
            const LineNumberTableAttribute* line_number_table = mt->attribute<LineNumberTableAttribute>(attr::LINE_NUMBER_TABLE);
            if (line_number_table != nullptr && DecompilerContext::option(pref::USE_DEBUG_LINE_NUMBERS)) {
                buffer.set_current_line(line_number_table->first_line() - 1);
            }
            buffer.append('{').append_line_separator();
            tracer.increment_current_source_line();

            RootStatement* root = wrapper->get_method_wrapper(mt->name(), mt->descriptor())->root;

            if (root != nullptr && !method_wrapper->decompiled_with_errors) {   // check for existence
                try {
                    const int start_line = tracer.get_current_source_line();

                    TextBuffer code = root->to_java(indent + 1, tracer);

                    hide_method = (clinit || dinit || hide_constructor(wrapper, init, throws_exceptions, param_count)) && code.length() == 0;

                    if (!hide_method && line_number_table != nullptr && DecompilerContext::option(pref::USE_DEBUG_LINE_NUMBERS)) {
                        map_lines(code, *line_number_table, tracer, start_line);
                    }

                    buffer.append(code);
                } catch (const std::exception& ex) {
                    DecompilerContext::get_logger().write_message("Method " + mt->name() + " " + mt->descriptor() + " couldn't be written.", ex);
                    method_wrapper->decompiled_with_errors = true;
                }
            }

            if (method_wrapper->decompiled_with_errors) {
                buffer.append_indent(indent + 1);
                buffer.append("// $FF: Couldn't be decompiled");
                buffer.append_line_separator();
                tracer.increment_current_source_line();
            }

            buffer.append_indent(indent).append('}').append_line_separator();
            tracer.increment_current_source_line();
        }
    }

    // save total lines
    // TODO (the original's): optimize
    //tracer.setCurrentSourceLine(buffer.countLines(start_index_method));

    return !hide_method;
}

bool ClassWriter::search_for_method(const StructClass* cl, const std::string& name, const MethodDescriptor& md, bool search) {
    // Didn't find the class or the library containing the class wasn't loaded, can't search
    if (cl == nullptr) return false;

    const auto& methods = cl->methods();

    if (search) {
        // If we're allowed to search, iterate through the methods and try to find matches
        for (const auto& method : methods) {
            // Match against name, descriptor, and whether or not the found method is static.
            // TODO (the original's): We are not handling generics or superclass parameters and return types
            if (md == MethodDescriptor::parse(method->descriptor()) && name == method->name() && !method->has_modifier(cc::ACC_STATIC)) return true;
        }
    }

    // If we have a superclass that's not Object, search that as well
    if (cl->super_class != nullptr) {
        const StructClass* super_class = DecompilerContext::current().struct_context->get_class(cl->super_class->get_string());

        const bool found_in_super_class = search_for_method(super_class, name, md, true);

        if (found_in_super_class) return true;
    }

    // Search all of the interfaces implemented by this class for the method
    for (const std::string& iface_name : cl->interface_names()) {
        const StructClass* iface = DecompilerContext::current().struct_context->get_class(iface_name);

        const bool found_in_iface = search_for_method(iface, name, md, true);

        if (found_in_iface) return true;
    }

    // We didn't manage to find anything, return
    return false;
}

void ClassWriter::map_lines(TextBuffer& code, const LineNumberTableAttribute& table, BytecodeMappingTracer& tracer, int start_line) {
    // build line start offsets map
    std::map<int, std::set<int>> line_start_offsets;   // (the offsets of a line, sorted: "requires natural sorting!")
    for (const auto& [offset, line] : tracer.get_mapping()) {
        const int line_number = line - start_line;
        line_start_offsets[line_number].insert(offset);
    }

    const std::string line_separator = DecompilerContext::new_line_separator();

    const std::string& text = code.original_text();
    std::size_t pos = text.find(line_separator);
    int line_number = 0;
    while (pos != std::string::npos) {
        const auto start_offsets = line_start_offsets.find(line_number);
        if (start_offsets != line_start_offsets.end()) {
            for (const int offset : start_offsets->second) {
                const int number = table.find_line_number(offset);
                if (number >= 0) {
                    code.set_line_mapping(number, static_cast<int>(pos));
                    break;
                }
            }
        }

        pos = text.find(line_separator, pos + 1);
        ++line_number;
    }
}

bool ClassWriter::hide_constructor(ClassWrapper* wrapper, bool init, bool throws_exceptions, int param_count) {
    if (!init || throws_exceptions || param_count > 0 || !DecompilerContext::option(pref::HIDE_DEFAULT_CONSTRUCTOR)) return false;

    int count = 0;
    for (const auto& mt : wrapper->get_class_struct()->methods()) {
        if (cc::INIT_NAME == mt->name()) {
            if (++count > 1) return false;
        }
    }

    return true;
}

void ClassWriter::append_deprecation(TextBuffer& buffer, int indent) { buffer.append_indent(indent).append("/** @deprecated */").append_line_separator(); }

void ClassWriter::append_rename_comment(TextBuffer& buffer, const std::optional<std::string>& old_name, MType type, int indent) {
    if (!old_name) return;

    buffer.append_indent(indent);
    buffer.append("// $FF: renamed from: ");

    switch (type) {
        case MType::CLASS:
            buffer.append(ExprProcessor::build_java_class_name(*old_name));
            break;

        case MType::FIELD: {
            const std::vector<std::string> f_parts = split_space(*old_name);
            const FieldDescriptor fd = FieldDescriptor::parse(f_parts.at(2));
            buffer.append(f_parts.at(1));
            buffer.append(' ');
            buffer.append(get_type_print_out(fd.type));
            break;
        }

        default: {
            const std::vector<std::string> m_parts = split_space(*old_name);
            const MethodDescriptor md = MethodDescriptor::parse(m_parts.at(2));
            buffer.append(m_parts.at(1));
            buffer.append(" (");
            bool first = true;
            for (const VarType& param_type : md.params) {
                if (!first) buffer.append(", ");
                first = false;
                buffer.append(get_type_print_out(param_type));
            }
            buffer.append(") ");
            buffer.append(get_type_print_out(md.ret));
        }
    }

    buffer.append_line_separator();
}

std::string ClassWriter::get_type_print_out(const VarType& type) {
    std::string type_text = ExprProcessor::get_cast_type_name(type, false);
    if (ExprProcessor::UNDEFINED_TYPE_STRING == type_text && DecompilerContext::option(pref::UNDEFINED_PARAM_TYPE_OBJECT)) {
        type_text = ExprProcessor::get_cast_type_name(VarType::OBJECT, false);
    }
    return type_text;
}

void ClassWriter::append_comment(TextBuffer& buffer, const std::string& comment, int indent) {
    if (DecompilerContext::option(pref::NO_COMMENT_OUTPUT)) return;
    buffer.append_indent(indent).append("// $FF: ").append(comment).append_line_separator();
}

void ClassWriter::append_annotations(TextBuffer& buffer, const StructMember& mb, int indent) {
    BytecodeMappingTracer tracer_dummy;   // FIXME (the original's): replace with a real one

    for (const std::string_view name : ANNOTATION_ATTRIBUTES) {
        const StructAnnotationAttribute* attribute = mb.attribute<StructAnnotationAttribute>(name);
        if (attribute != nullptr) {
            for (AnnotationExprent* annotation : attribute->get_annotations()) {
                buffer.append(annotation->to_java(indent, tracer_dummy)).append_line_separator();
            }
        }
    }
}

void ClassWriter::append_parameter_annotations(TextBuffer& buffer, const StructMethod* mt, int param) {
    BytecodeMappingTracer tracer_dummy;   // FIXME (the original's): replace with a real one

    for (const std::string_view name : PARAMETER_ANNOTATION_ATTRIBUTES) {
        const StructAnnotationParameterAttribute* attribute = mt->attribute<StructAnnotationParameterAttribute>(name);
        if (attribute != nullptr) {
            const std::vector<std::vector<AnnotationExprent*>>& annotations = attribute->get_param_annotations();
            if (static_cast<std::size_t>(param) < annotations.size()) {
                for (AnnotationExprent* annotation : annotations[static_cast<std::size_t>(param)]) {
                    buffer.append(annotation->to_java(0, tracer_dummy)).append(' ');
                }
            }
        }
    }
}

void ClassWriter::append_modifiers(TextBuffer& buffer, int flags, int allowed, bool is_interface, int excluded) {
    flags &= allowed;
    if (!is_interface) excluded = 0;
    for (const auto& [modifier, text] : modifiers()) {
        if ((flags & modifier) == modifier && (modifier & excluded) == 0) buffer.append(text).append(' ');
    }
}

void ClassWriter::append_type_parameters(TextBuffer& buffer, const std::vector<std::string>& parameters, const std::vector<std::vector<GenericType>>& bounds) {
    buffer.append('<');

    for (std::size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) buffer.append(", ");

        buffer.append(parameters[i]);

        const std::vector<GenericType>& parameter_bounds = bounds.at(i);
        if (parameter_bounds.size() > 1 || !("java/lang/Object" == parameter_bounds.at(0).value)) {
            buffer.append(" extends ");
            buffer.append(generics::cast_type_name(parameter_bounds.at(0), current_name_context()));
            for (std::size_t j = 1; j < parameter_bounds.size(); ++j) {
                buffer.append(" & ");
                buffer.append(generics::cast_type_name(parameter_bounds[j], current_name_context()));
            }
        }
    }

    buffer.append('>');
}

}  // namespace cb::java::decompiler
