// java/decompiler/structs.hpp -- the classes being decompiled (and the library classes they refer to) as the decompiler holds them: a class with its
// constant pool, its fields and methods by "name descriptor", and the attributes of each; and the context that loads them from jars, zips, folders or
// class files.
//
//     StructContext ctx;
//     ctx.add_space("game.jar", /*own=*/true);                  // every class of the jar
//     const StructClass* c = ctx.get_class("net/minecraft/client/Minecraft");
//     const StructMethod* run = c->get_method("run", "()V");     // its code: run->code()
//
// Ported from Fernflower (de.fernflower.struct: StructMember, StructField, StructMethod, StructClass, StructContext, ContextUnit's loading),
// Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). A class is read once by java/classfile and kept in memory (the
// original's lazy reloading of pools and code is a memory saving that is not needed here). A method's bytecode is turned into instructions by the
// code layer; writing the decompiled units out is the driver's.
#pragma once

#include <exception>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "java/classfile.hpp"
#include "java/decompiler/attributes.hpp"
#include "java/decompiler/constant_pool.hpp"
#include "java/decompiler/keyed_list.hpp"

namespace cb::java::decompiler {

using Attributes = KeyedList<std::shared_ptr<Attribute>, std::string>;

class StructMember {
public:
    virtual ~StructMember() = default;
    int access_flags() const { return access_flags_; }
    const Attributes& attributes() const { return attributes_; }
    bool has_modifier(int modifier) const { return (access_flags_ & modifier) == modifier; }
    bool is_synthetic() const;
    // The attribute of that name, as its kind; null when there is none (or it is another kind).
    template <class A> const A* attribute(std::string_view name) const {
        const std::shared_ptr<Attribute>* a = attributes_.find(std::string(name));
        return a != nullptr ? dynamic_cast<const A*>(a->get()) : nullptr;
    }

protected:
    // The attributes the decompiler reads, made against `pool` (every LocalVariableTable merged into the first).
    Attributes read_attributes(const std::vector<classfile::Attribute>& list, const classfile::ClassFile& cf, const ConstantPool& pool);
    // One attribute; `read_attribute` of a method takes its Code apart first.
    virtual std::shared_ptr<Attribute> read_attribute(const classfile::Attribute& a, const std::string& name, const classfile::ClassFile& cf, const ConstantPool& pool);

    int access_flags_ = 0;
    Attributes attributes_;
};

class StructClass;

class StructField : public StructMember {
public:
    StructField(const classfile::Member& m, const classfile::ClassFile& cf, const StructClass& owner);
    const std::string& name() const { return name_; }
    const std::string& descriptor() const { return descriptor_; }

private:
    std::string name_, descriptor_;
};

class StructMethod : public StructMember {
public:
    StructMethod(const classfile::Member& m, const classfile::ClassFile& cf, StructClass& owner);
    StructClass& class_struct() const { return *owner_; }
    const std::string& name() const { return name_; }
    const std::string& descriptor() const { return descriptor_; }
    bool contains_code() const { return contains_code_; }
    int local_variables() const { return local_variables_; }
    // The method's code (only an own class's is kept): its bytes, its exception table and the rest of the Code attribute.
    const std::optional<classfile::Code>& code() const { return code_; }

protected:
    std::shared_ptr<Attribute> read_attribute(const classfile::Attribute& a, const std::string& name, const classfile::ClassFile& cf, const ConstantPool& pool) override;

private:
    StructClass* owner_;
    std::string name_, descriptor_;
    bool contains_code_ = false;
    int local_variables_ = 0;
    std::optional<classfile::Code> code_;
    std::optional<Attributes> code_attributes_;
};

class StructClass : public StructMember {
public:
    // The class in `bytes` (FormatError for one that is not a class). `own`: one being decompiled (its methods' code is kept).
    StructClass(ByteView bytes, bool own, std::shared_ptr<const PoolInterceptor> interceptor = nullptr);
    StructClass(const StructClass&) = delete;
    StructClass& operator=(const StructClass&) = delete;

    std::string qualified_name;
    const PrimitiveConstant* super_class = nullptr;   // null for java/lang/Object (it lives in the pool)

    bool has_field(const std::string& name, const std::string& descriptor) const { return get_field(name, descriptor) != nullptr; }
    StructField* get_field(const std::string& name, const std::string& descriptor) const;
    StructMethod* get_method(const std::string& key) const;
    StructMethod* get_method(const std::string& name, const std::string& descriptor) const;
    const std::string& get_interface(std::size_t i) const { return interface_names_.at(i); }
    const std::vector<int>& interfaces() const { return interfaces_; }
    const std::vector<std::string>& interface_names() const { return interface_names_; }
    const KeyedList<std::shared_ptr<StructMethod>, std::string>& methods() const { return methods_; }
    const KeyedList<std::shared_ptr<StructField>, std::string>& fields() const { return fields_; }
    const ConstantPool& pool() const { return *pool_; }
    bool is_own() const { return own_; }
    int minor_version() const { return minor_; }
    int major_version() const { return major_; }
    bool is_version_ge_1_5() const { return major_ > 48 || (major_ == 48 && minor_ > 0); }
    bool is_version_ge_1_7() const { return major_ >= 51; }
    int bytecode_version() const;

private:
    bool own_;
    int minor_ = 0, major_ = 0;
    std::unique_ptr<ConstantPool> pool_;
    std::vector<int> interfaces_;
    std::vector<std::string> interface_names_;
    KeyedList<std::shared_ptr<StructField>, std::string> fields_;
    KeyedList<std::shared_ptr<StructMethod>, std::string> methods_;
};

// A source of classes: a folder, a jar or a zip, with the classes it holds and its other entries (copied to the output beside the decompiled source).
struct ContextUnit {
    static constexpr int TYPE_FOLDER = 0, TYPE_JAR = 1, TYPE_ZIP = 2;
    int type = TYPE_FOLDER;
    std::string archive_path;   // the folder the archive is in (relative)
    std::string filename;       // a folder: its relative path; an archive: its file name
    bool own = true;
    std::vector<StructClass*> classes;
    std::vector<std::string> class_entries;                          // each class's file or entry name
    std::vector<std::string> dir_entries;
    std::vector<std::pair<std::string, std::string>> other_entries;   // (full path of the file or archive, entry name)
    std::optional<std::string> manifest;                             // META-INF/MANIFEST.MF as it was
};

class StructContext {
public:
    // An ERROR of reading, with what was thrown (the original's writeMessage(message, throwable): the message, then the exception on a line of its own).
    using Logger = std::function<void(const std::string& message, const std::exception& cause)>;
    explicit StructContext(Logger log = {}, std::shared_ptr<const PoolInterceptor> interceptor = nullptr);

    StructClass* get_class(const std::string& name) const;
    // A file (a .class, a .jar, a .zip, anything else as an other entry) or a folder of them, `own`: to be decompiled (else a library).
    void add_space(const std::filesystem::path& file, bool own);
    const std::map<std::string, std::unique_ptr<StructClass>>& classes() const { return classes_; }
    const std::map<std::string, ContextUnit>& units() const { return units_; }

private:
    void add_space(const std::string& path, const std::filesystem::path& file, bool own, int level);
    void add_archive(const std::string& external_path, const std::filesystem::path& file, int type, bool own);
    void add_class(ContextUnit& unit, ByteView bytes, bool own, const std::string& entry, const std::filesystem::path& file);

    Logger log_;
    std::shared_ptr<const PoolInterceptor> interceptor_;
    std::map<std::string, ContextUnit> units_;
    std::map<std::string, std::unique_ptr<StructClass>> classes_;
};

}  // namespace cb::java::decompiler
