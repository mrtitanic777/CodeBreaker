// java/decompiler/renamer.cpp -- see renamer.hpp. Ported from Fernflower (de.fernflower.modules.renamer: IdentifierConverter, ConverterHelper,
// PoolInterceptor), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include "java/decompiler/renamer.hpp"

#include <deque>
#include <set>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"

namespace cb::java::decompiler {

namespace {

// The length of the text in Java's UTF-16 code units.
int utf16_length(const std::string& s) {
    int n = 0;
    for (const unsigned char c : s) {
        if ((c & 0xC0) == 0x80) continue;   // a continuation byte
        n += c >= 0xF0 ? 2 : 1;
    }
    return n;
}

bool is_digit(char c) { return c >= '0' && c <= '9'; }

}  // namespace

// ---- ConverterHelper

const std::set<std::string>& ConverterHelper::keywords() {
    static const std::set<std::string> k = {
        "abstract", "do", "if", "package", "synchronized", "boolean", "double", "implements", "private", "this", "break", "else", "import",
        "protected", "throw", "byte", "extends", "instanceof", "public", "throws", "case", "false", "int", "return", "transient", "catch",
        "final", "interface", "short", "true", "char", "finally", "long", "static", "try", "class", "float", "native", "strictfp", "void",
        "const", "for", "new", "super", "volatile", "continue", "goto", "null", "switch", "while", "default", "assert", "enum"};
    return k;
}

bool ConverterHelper::to_be_renamed(Type element_type, const std::optional<std::string>& class_name, const std::optional<std::string>& element,
                                    const std::optional<std::string>& descriptor) {
    (void)descriptor;
    const std::optional<std::string>& value = element_type == Type::ELEMENT_CLASS ? class_name : element;
    return !value || value->empty() || utf16_length(*value) <= 2 || keywords().contains(*value) || is_digit((*value)[0]);
}

// TODO (the original's): consider possible conflicts with not renamed classes, fields and methods!
std::string ConverterHelper::get_next_class_name(const std::string& full_name, const std::optional<std::string>& short_name) {
    (void)full_name;
    if (!short_name) return "class_" + std::to_string(class_counter_++);

    std::size_t index = 0;
    while (is_digit(short_name->at(index))) ++index;

    if (index == 0 || index == short_name->size()) {
        return "class_" + std::to_string(class_counter_++);
    }
    const std::string name = short_name->substr(index);
    if (set_non_standard_class_names_.contains(name)) {
        return "Inner" + name + "_" + std::to_string(class_counter_++);
    }
    set_non_standard_class_names_.insert(name);
    return "Inner" + name;
}

std::string ConverterHelper::get_next_field_name(const std::string& class_name, const std::string& field, const std::string& descriptor) {
    (void)class_name, (void)field, (void)descriptor;
    return "field_" + std::to_string(field_counter_++);
}

std::string ConverterHelper::get_next_method_name(const std::string& class_name, const std::string& method, const std::string& descriptor) {
    (void)class_name, (void)method, (void)descriptor;
    return "method_" + std::to_string(method_counter_++);
}

std::string ConverterHelper::get_simple_class_name(const std::string& full_name) {
    const std::size_t i = full_name.rfind('/');
    return i == std::string::npos ? full_name : full_name.substr(i + 1);
}

std::string ConverterHelper::replace_simple_class_name(const std::string& full_name, const std::string& new_name) {
    const std::size_t i = full_name.rfind('/');
    return (i == std::string::npos ? std::string() : full_name.substr(0, i + 1)) + new_name;
}

// ---- RenamerPoolInterceptor

void RenamerPoolInterceptor::add_name(const std::string& old_name, const std::string& new_name) {
    map_old_to_new_names_[old_name] = new_name;
    map_new_to_old_names_[new_name] = old_name;
}

std::optional<std::string> RenamerPoolInterceptor::get_name(const std::string& old_name) const {
    const auto it = map_old_to_new_names_.find(old_name);
    return it == map_old_to_new_names_.end() ? std::nullopt : std::optional<std::string>(it->second);
}

std::optional<std::string> RenamerPoolInterceptor::get_old_name(const std::string& new_name) const {
    const auto it = map_new_to_old_names_.find(new_name);
    return it == map_new_to_old_names_.end() ? std::nullopt : std::optional<std::string>(it->second);
}

// ---- IdentifierConverter

std::shared_ptr<RenamerPoolInterceptor> IdentifierConverter::rename(StructContext& context) {
    context_ = &context;

    const std::string user_class = DecompilerContext::property(pref::USER_RENAMER_CLASS);
    if (!user_class.empty() && user_renamer_) helper_owner_ = user_renamer_(user_class);
    if (!helper_owner_) helper_owner_ = std::make_unique<ConverterHelper>();
    helper_ = helper_owner_.get();

    interceptor_ = std::make_shared<RenamerPoolInterceptor>(helper_);

    build_inheritance_tree();
    rename_all_classes();
    rename_interfaces();
    rename_classes();

    return interceptor_;
}

void IdentifierConverter::rename_classes() {
    const std::vector<ClassWrapperNode*> lst_classes = get_reverse_post_order_list_iterative(root_classes_);

    std::map<std::string, std::map<std::string, std::string>> class_name_maps;

    for (ClassWrapperNode* node : lst_classes) {
        const StructClass* cl = node->get_class_struct();

        std::map<std::string, std::string> names;

        // merge information on super class
        if (cl->super_class != nullptr) {
            const auto it = class_name_maps.find(cl->super_class->get_string());
            if (it != class_name_maps.end()) {
                for (const auto& [k, v] : it->second) names[k] = v;
            }
        }

        // merge information on interfaces
        for (const std::string& if_name : cl->interface_names()) {
            const auto it = interface_name_maps_.find(if_name);
            if (it != interface_name_maps_.end()) {
                for (const auto& [k, v] : it->second) names[k] = v;
            } else {
                const StructClass* clintr = context_->get_class(if_name);
                if (clintr != nullptr) {
                    for (const auto& [k, v] : process_external_interface(clintr)) names[k] = v;
                }
            }
        }

        rename_class_identifiers(cl, names);

        if (!node->get_subclasses().empty()) class_name_maps[cl->qualified_name] = names;
    }
}

std::map<std::string, std::string> IdentifierConverter::process_external_interface(const StructClass* cl) {
    std::map<std::string, std::string> names;

    for (const std::string& if_name : cl->interface_names()) {
        const auto it = interface_name_maps_.find(if_name);
        if (it != interface_name_maps_.end()) {
            for (const auto& [k, v] : it->second) names[k] = v;
        } else {
            const StructClass* clintr = context_->get_class(if_name);
            if (clintr != nullptr) {
                for (const auto& [k, v] : process_external_interface(clintr)) names[k] = v;
            }
        }
    }

    rename_class_identifiers(cl, names);

    return names;
}

void IdentifierConverter::rename_interfaces() {
    const std::vector<ClassWrapperNode*> lst_interfaces = get_reverse_post_order_list_iterative(root_interfaces_);

    std::map<std::string, std::map<std::string, std::string>> interface_name_maps;

    // rename methods and fields
    for (ClassWrapperNode* node : lst_interfaces) {
        const StructClass* cl = node->get_class_struct();

        std::map<std::string, std::string> names;

        // merge information on super interfaces
        for (const std::string& if_name : cl->interface_names()) {
            const auto it = interface_name_maps.find(if_name);
            if (it != interface_name_maps.end()) {
                for (const auto& [k, v] : it->second) names[k] = v;
            }
        }

        rename_class_identifiers(cl, names);

        interface_name_maps[cl->qualified_name] = names;
    }

    interface_name_maps_ = std::move(interface_name_maps);
}

void IdentifierConverter::rename_all_classes() {
    // order not important
    std::vector<ClassWrapperNode*> lst_all_classes = get_reverse_post_order_list_iterative(root_interfaces_);
    const std::vector<ClassWrapperNode*> more = get_reverse_post_order_list_iterative(root_classes_);
    lst_all_classes.insert(lst_all_classes.end(), more.begin(), more.end());

    // rename all interfaces and classes
    for (ClassWrapperNode* node : lst_all_classes) rename_class(node->get_class_struct());
}

void IdentifierConverter::rename_class(const StructClass* cl) {
    if (!cl->is_own()) return;

    const std::string class_old_full_name = cl->qualified_name;

    // TODO (the original's): rename packages

    const std::string cl_simple_name = ConverterHelper::get_simple_class_name(class_old_full_name);
    if (helper_->to_be_renamed(IIdentifierRenamer::Type::ELEMENT_CLASS, cl_simple_name, std::nullopt, std::nullopt)) {
        std::string class_new_full_name;
        do {
            const std::string classname = helper_->get_next_class_name(class_old_full_name, ConverterHelper::get_simple_class_name(class_old_full_name));
            class_new_full_name = ConverterHelper::replace_simple_class_name(class_old_full_name, classname);
        } while (context_->classes().contains(class_new_full_name));

        interceptor_->add_name(class_old_full_name, class_new_full_name);
    }
}

void IdentifierConverter::rename_class_identifiers(const StructClass* cl, std::map<std::string, std::string>& names) {
    // all classes are already renamed
    const std::string class_old_full_name = cl->qualified_name;
    std::string class_new_full_name = interceptor_->get_name(class_old_full_name).value_or(class_old_full_name);

    // methods
    std::set<std::string> set_method_names;
    for (const auto& md : cl->methods()) set_method_names.insert(md->name());

    const auto& methods = cl->methods();
    for (std::size_t i = 0; i < methods.size(); ++i) {
        const StructMethod* mt = methods[i].get();
        const std::string key = *methods.key(i);

        const bool is_private = mt->has_modifier(cc::ACC_PRIVATE);

        std::string name = mt->name();
        if (!cl->is_own() || mt->has_modifier(cc::ACC_NATIVE)) {
            // external and native methods must not be renamed
            if (!is_private) names[key] = name;
        } else if (helper_->to_be_renamed(IIdentifierRenamer::Type::ELEMENT_METHOD, class_old_full_name, name, mt->descriptor())) {
            if (is_private || !names.contains(key)) {
                do {
                    name = helper_->get_next_method_name(class_old_full_name, name, mt->descriptor());
                } while (set_method_names.contains(name));

                if (!is_private) names[key] = name;
            } else {
                name = names.at(key);
            }

            interceptor_->add_name(class_old_full_name + " " + mt->name() + " " + mt->descriptor(),
                                   class_new_full_name + " " + name + " " + build_new_descriptor(false, mt->descriptor()));
        }
    }

    // external fields are not being renamed
    if (!cl->is_own()) return;

    // fields
    // FIXME (the original's): should overloaded fields become the same name?
    std::set<std::string> set_field_names;
    for (const auto& fd : cl->fields()) set_field_names.insert(fd->name());

    for (const auto& fd : cl->fields()) {
        if (helper_->to_be_renamed(IIdentifierRenamer::Type::ELEMENT_FIELD, class_old_full_name, fd->name(), fd->descriptor())) {
            std::string new_name;
            do {
                new_name = helper_->get_next_field_name(class_old_full_name, fd->name(), fd->descriptor());
            } while (set_field_names.contains(new_name));

            interceptor_->add_name(class_old_full_name + " " + fd->name() + " " + fd->descriptor(),
                                   class_new_full_name + " " + new_name + " " + build_new_descriptor(true, fd->descriptor()));
        }
    }
}

std::optional<std::string> IdentifierConverter::build_new_classname(const std::string& class_name) const { return interceptor_->get_name(class_name); }

std::string IdentifierConverter::build_new_descriptor(bool is_field, const std::string& descriptor) const {
    const NewClassNameBuilder builder = as_builder();
    std::optional<std::string> new_descriptor;
    if (is_field) new_descriptor = FieldDescriptor::parse(descriptor).build_new_descriptor(builder);
    else new_descriptor = MethodDescriptor::parse(descriptor).build_new_descriptor(builder);
    return new_descriptor ? *new_descriptor : descriptor;
}

std::vector<ClassWrapperNode*> IdentifierConverter::get_reverse_post_order_list_iterative(const std::vector<ClassWrapperNode*>& roots) {
    std::vector<ClassWrapperNode*> res;

    std::vector<ClassWrapperNode*> stack_node;
    std::vector<int> stack_index;

    std::set<ClassWrapperNode*> set_visited;

    for (ClassWrapperNode* root : roots) {
        stack_node.push_back(root);
        stack_index.push_back(0);
    }

    while (!stack_node.empty()) {
        ClassWrapperNode* node = stack_node.back();
        int index = stack_index.back();
        stack_index.pop_back();

        set_visited.insert(node);

        const std::vector<ClassWrapperNode*>& lst_subs = node->get_subclasses();

        for (; index < static_cast<int>(lst_subs.size()); ++index) {
            ClassWrapperNode* sub = lst_subs[static_cast<std::size_t>(index)];
            if (!set_visited.contains(sub)) {
                stack_index.push_back(index + 1);
                stack_node.push_back(sub);
                stack_index.push_back(0);

                break;
            }
        }

        if (index == static_cast<int>(lst_subs.size())) {
            res.insert(res.begin(), node);

            stack_node.pop_back();
        }
    }

    return res;
}

void IdentifierConverter::build_inheritance_tree() {
    std::map<std::string, ClassWrapperNode*> nodes;
    const auto& classes = context_->classes();

    std::vector<ClassWrapperNode*> root_classes;
    std::vector<ClassWrapperNode*> root_interfaces;

    for (const auto& entry : classes) {
        const StructClass* cl = entry.second.get();
        if (!cl->is_own()) continue;

        std::deque<const StructClass*> stack;
        std::deque<ClassWrapperNode*> stack_sub_nodes;

        stack.push_back(cl);
        stack_sub_nodes.push_back(nullptr);

        while (!stack.empty()) {
            const StructClass* cl_str = stack.front();
            stack.pop_front();
            ClassWrapperNode* child = stack_sub_nodes.front();
            stack_sub_nodes.pop_front();

            ClassWrapperNode* node = nullptr;
            const auto found = nodes.find(cl_str->qualified_name);
            if (found != nodes.end()) node = found->second;
            const bool is_new_node = node == nullptr;

            if (is_new_node) {
                node = make<ClassWrapperNode>(cl_str);
                nodes[cl_str->qualified_name] = node;
            }

            if (child != nullptr) node->add_subclass(child);

            if (!is_new_node) {
                break;
            }

            const bool is_interface = cl_str->has_modifier(cc::ACC_INTERFACE);
            bool found_parent = false;

            if (is_interface) {
                for (const std::string& if_name : cl_str->interface_names()) {
                    const auto it = classes.find(if_name);
                    if (it != classes.end()) {
                        stack.push_back(it->second.get());
                        stack_sub_nodes.push_back(node);

                        found_parent = true;
                    }
                }
            } else if (cl_str->super_class != nullptr) {   // null iff java/lang/Object
                const auto it = classes.find(cl_str->super_class->get_string());
                if (it != classes.end()) {
                    stack.push_back(it->second.get());
                    stack_sub_nodes.push_back(node);

                    found_parent = true;
                }
            }

            if (!found_parent) {   // no super class or interface
                (is_interface ? root_interfaces : root_classes).push_back(node);
            }
        }
    }

    root_classes_ = std::move(root_classes);
    root_interfaces_ = std::move(root_interfaces);
}

}  // namespace cb::java::decompiler
