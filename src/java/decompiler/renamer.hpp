// java/decompiler/renamer.hpp -- the identifier renamer (the original's RENAME_ENTITIES): the classes, fields and methods whose names a Java source cannot
// have (empty, one or two characters, a keyword, a leading digit) get new ones; the inheritance tree makes an overriding method take its parent's new
// name; the names go to the constant pool through a PoolInterceptor so every reference in the code reads as renamed.
//
//     IdentifierConverter conv;
//     std::shared_ptr<RenamerPoolInterceptor> names = conv.rename(context);   // then reload the classes with `names` (Fernflower::decompile_context)
//     names->get_name("a/b");                                                // "class_0" (the new full name), or nullopt
//
// Ported from Fernflower (de.fernflower.modules.renamer: IdentifierConverter, ConverterHelper, ClassWrapperNode, PoolInterceptor; and
// main.extern.IIdentifierRenamer), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). Two differences from the
// original: a class's pool is made with its interceptor (structs.hpp), so the original's `context.reloadContext()` after renaming is the caller's
// (a new StructContext made with the returned interceptor, the same spaces added again); and a user renamer class (USER_RENAMER_CLASS names a Java
// class that is loaded by name) is a factory the caller registers. The original's Java PoolInterceptor is RenamerPoolInterceptor here:
// constant_pool.hpp already has a PoolInterceptor, the abstract one the pool reads, which this one implements.
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "java/decompiler/constant_pool.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

// Decides what is renamed and what the new names are (the original's main.extern.IIdentifierRenamer).
class IIdentifierRenamer {
public:
    enum class Type { ELEMENT_CLASS, ELEMENT_FIELD, ELEMENT_METHOD };
    virtual ~IIdentifierRenamer() = default;
    // For a class `class_name` is its simple name and `element` is null; for a field or method `element` is its name. (Null is meaningful: it is renamed.)
    virtual bool to_be_renamed(Type element_type, const std::optional<std::string>& class_name, const std::optional<std::string>& element,
                               const std::optional<std::string>& descriptor) = 0;
    // `short_name` null: no simple name (an anonymous class).
    virtual std::string get_next_class_name(const std::string& full_name, const std::optional<std::string>& short_name) = 0;
    virtual std::string get_next_field_name(const std::string& class_name, const std::string& field, const std::string& descriptor) = 0;
    virtual std::string get_next_method_name(const std::string& class_name, const std::string& method, const std::string& descriptor) = 0;
};

// The default renamer: "class_N", "field_N", "method_N" (and "Inner<Name>" for a numbered inner class), for everything that is empty, at most two
// characters, a Java keyword or starts with a digit.
class ConverterHelper : public IIdentifierRenamer {
public:
    bool to_be_renamed(Type element_type, const std::optional<std::string>& class_name, const std::optional<std::string>& element,
                       const std::optional<std::string>& descriptor) override;
    std::string get_next_class_name(const std::string& full_name, const std::optional<std::string>& short_name) override;
    std::string get_next_field_name(const std::string& class_name, const std::string& field, const std::string& descriptor) override;
    std::string get_next_method_name(const std::string& class_name, const std::string& method, const std::string& descriptor) override;

    // "a/b/C$D" -> "C$D".
    static std::string get_simple_class_name(const std::string& full_name);
    // "a/b/C" and "X" -> "a/b/X".
    static std::string replace_simple_class_name(const std::string& full_name, const std::string& new_name);

private:
    static const std::set<std::string>& keywords();
    int class_counter_ = 0;
    int field_counter_ = 0;
    int method_counter_ = 0;
    std::set<std::string> set_non_standard_class_names_;
};

// The names the renamer chose: old -> new, and back, for a class ("a/b/C"), a field or a method ("a/b/C name descriptor").
class RenamerPoolInterceptor : public PoolInterceptor {
public:
    explicit RenamerPoolInterceptor(IIdentifierRenamer* helper) : helper_(helper) {}
    void add_name(const std::string& old_name, const std::string& new_name);
    std::optional<std::string> get_name(const std::string& old_name) const override;
    std::optional<std::string> get_old_name(const std::string& new_name) const;
    IIdentifierRenamer* get_helper() const { return helper_; }

private:
    IIdentifierRenamer* helper_;   // (owned by the IdentifierConverter that made this)
    std::map<std::string, std::string> map_old_to_new_names_, map_new_to_old_names_;
};

// A class in the inheritance tree of the classes being decompiled: its subclasses (the classes or interfaces that extend it).
class ClassWrapperNode : public ArenaObject {
public:
    explicit ClassWrapperNode(const StructClass* cl) : class_struct_(cl) {}
    void add_subclass(ClassWrapperNode* node) {
        node->set_superclass(this);
        subclasses_.push_back(node);
    }
    const StructClass* get_class_struct() const { return class_struct_; }
    std::vector<ClassWrapperNode*>& get_subclasses() { return subclasses_; }
    ClassWrapperNode* get_superclass() const { return superclass_; }
    void set_superclass(ClassWrapperNode* superclass) { superclass_ = superclass; }

private:
    const StructClass* class_struct_;
    ClassWrapperNode* superclass_ = nullptr;
    std::vector<ClassWrapperNode*> subclasses_;
};

class IdentifierConverter {
public:
    // Makes the user's renamer (USER_RENAMER_CLASS names it); null from the factory (or no factory): ConverterHelper.
    using RenamerFactory = std::function<std::unique_ptr<IIdentifierRenamer>(const std::string& user_class)>;
    explicit IdentifierConverter(RenamerFactory user_renamer = {}) : user_renamer_(std::move(user_renamer)) {}

    // Renames what needs it across the own classes of `context` and returns the names (the context is not changed: see the top comment).
    // std::runtime_error "Renaming failed!" as the original.
    std::shared_ptr<RenamerPoolInterceptor> rename(StructContext& context);
    // The renamer's new name for a class (the original's NewClassNameBuilder implementation).
    std::optional<std::string> build_new_classname(const std::string& class_name) const;
    NewClassNameBuilder as_builder() const {
        return [this](const std::string& class_name) { return build_new_classname(class_name); };
    }

private:
    void rename_classes();
    std::map<std::string, std::string> process_external_interface(const StructClass* cl);
    void rename_interfaces();
    void rename_all_classes();
    void rename_class(const StructClass* cl);
    void rename_class_identifiers(const StructClass* cl, std::map<std::string, std::string>& names);
    std::string build_new_descriptor(bool is_field, const std::string& descriptor) const;
    static std::vector<ClassWrapperNode*> get_reverse_post_order_list_iterative(const std::vector<ClassWrapperNode*>& roots);
    void build_inheritance_tree();

    RenamerFactory user_renamer_;
    StructContext* context_ = nullptr;
    std::unique_ptr<IIdentifierRenamer> helper_owner_;
    IIdentifierRenamer* helper_ = nullptr;
    std::shared_ptr<RenamerPoolInterceptor> interceptor_;
    std::vector<ClassWrapperNode*> root_classes_, root_interfaces_;
    std::map<std::string, std::map<std::string, std::string>> interface_name_maps_;
};

}  // namespace cb::java::decompiler
