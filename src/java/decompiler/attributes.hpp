// java/decompiler/attributes.hpp -- the attributes of a class, field or method the decompiler reads: each made from the class file's bytes by its name
// (an attribute the decompiler does not read is not made), and read against the constant pool.
//
//     std::unique_ptr<Attribute> a = Attribute::create("LineNumberTable", bytes, pool);   // null for a name the decompiler skips
//     dynamic_cast<LineNumberTableAttribute&>(*a).find_line_number(17);
//
// Ported from Fernflower (de.fernflower.struct.attr), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The
// annotation attributes (RuntimeVisible/Invisible[Parameter|Type]Annotations, AnnotationDefault) are made and keep their bytes; their contents are read
// into expressions by the expression layer, which is ported after this one.
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "java/decompiler/constant_pool.hpp"

namespace cb::java::decompiler {

namespace attr {
inline constexpr std::string_view CODE = "Code", INNER_CLASSES = "InnerClasses", SIGNATURE = "Signature", ANNOTATION_DEFAULT = "AnnotationDefault",
                                  EXCEPTIONS = "Exceptions", ENCLOSING_METHOD = "EnclosingMethod",
                                  RUNTIME_VISIBLE_ANNOTATIONS = "RuntimeVisibleAnnotations", RUNTIME_INVISIBLE_ANNOTATIONS = "RuntimeInvisibleAnnotations",
                                  RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS = "RuntimeVisibleParameterAnnotations",
                                  RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS = "RuntimeInvisibleParameterAnnotations",
                                  RUNTIME_VISIBLE_TYPE_ANNOTATIONS = "RuntimeVisibleTypeAnnotations",
                                  RUNTIME_INVISIBLE_TYPE_ANNOTATIONS = "RuntimeInvisibleTypeAnnotations", LOCAL_VARIABLE_TABLE = "LocalVariableTable",
                                  CONSTANT_VALUE = "ConstantValue", BOOTSTRAP_METHODS = "BootstrapMethods", SYNTHETIC = "Synthetic",
                                  DEPRECATED = "Deprecated", LINE_NUMBER_TABLE = "LineNumberTable", SOURCE_FILE = "SourceFile";
}

class Attribute {
public:
    virtual ~Attribute() = default;
    // The attribute of that name, read from `info`; null for a name the decompiler does not read. FormatError when the bytes are cut off.
    static std::unique_ptr<Attribute> create(std::string_view name, ByteView info, const ConstantPool& pool);
    const std::string& name() const { return name_; }
    const Bytes& info() const { return info_; }

protected:
    virtual void init_content(const ConstantPool& pool) { (void)pool; }

private:
    std::string name_;
    Bytes info_;
};

class ConstantValueAttribute : public Attribute {
public:
    int index() const { return index_; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    int index_ = 0;
};

class ExceptionsAttribute : public Attribute {
public:
    const std::vector<int>& throws_exceptions() const { return throws_; }
    std::string exception_class_name(std::size_t index, const ConstantPool& pool) const;

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::vector<int> throws_;
};

class InnerClassesAttribute : public Attribute {
public:
    struct Entry {
        int inner_class = 0, outer_class = 0, inner_name = 0, access = 0;           // the indexes and the flags as written
        std::string inner_class_name;                                              // and the names (none when the index is 0)
        std::optional<std::string> outer_class_name, simple_name;
    };
    const std::vector<Entry>& entries() const { return entries_; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::vector<Entry> entries_;
};

class LineNumberTableAttribute : public Attribute {
public:
    int first_line() const { return line_info_.size() > 1 ? line_info_[1] : -1; }
    // The line of the last entry starting at or before `pc`; -1 when none does.
    int find_line_number(int pc) const;
    const std::vector<int>& raw_data() const { return line_info_; }   // start pc, line, start pc, line ...

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::vector<int> line_info_;
};

class LocalVariableTableAttribute : public Attribute {
public:
    const std::map<int, std::string>& var_names() const { return names_; }   // by local variable index
    void add_local_variable_table(const LocalVariableTableAttribute& other);

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::map<int, std::string> names_;
};

class SourceFileAttribute : public Attribute {
public:
    const std::string& file_name() const { return file_name_; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::string file_name_;
};

class GenericSignatureAttribute : public Attribute {
public:
    const std::string& signature() const { return signature_; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::string signature_;
};

class EnclosingMethodAttribute : public Attribute {
public:
    const std::string& class_name() const { return class_name_; }
    const std::optional<std::string>& method_name() const { return method_name_; }
    const std::optional<std::string>& method_descriptor() const { return method_descriptor_; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::string class_name_;
    std::optional<std::string> method_name_, method_descriptor_;
};

class BootstrapMethodsAttribute : public Attribute {
public:
    std::size_t methods_number() const { return method_refs_.size(); }
    const LinkConstant* method_reference(std::size_t index) const { return method_refs_[index]; }
    const std::vector<const PooledConstant*>& method_arguments(std::size_t index) const { return method_arguments_[index]; }

protected:
    void init_content(const ConstantPool& pool) override;

private:
    std::vector<const LinkConstant*> method_refs_;                        // (they live in the pool)
    std::vector<std::vector<const PooledConstant*>> method_arguments_;
};

// (RuntimeVisible/InvisibleAnnotations, the parameter and type ones, AnnotationDefault: annotations.hpp.)

}  // namespace cb::java::decompiler
