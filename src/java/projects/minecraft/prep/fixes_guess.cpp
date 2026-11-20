// fixes_guess.cpp -- RDI's GuessGenerics: the generic signatures of classes guessed from their bridge methods. See java/projects/minecraft/prep/fixes.hpp.
//
// Ported from RDI (org.mcphackers.rdi.injector.transform.GuessGenerics), MIT License, (c) MCPHackers.
//
// The original's HashMaps keyed by ClassNode (identity-hashed: no order a program can reproduce) are LinkedMaps; its HashSet<String> of generics, which
// are listed into the signatures, is a JavaHashSet (Java's order); a List<String> that holds nulls (the original's String[] wrapped by Arrays.asList)
// is a vector of optional strings (a null reads "null" where it is appended to a signature, as StringBuilder.append(null String)).
// cb-unity: alone (file-local helpers of the port)
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

#include "java/decompiler/java_util.hpp"
#include "java/projects/minecraft/prep/fixes.hpp"

namespace cb::java::minecraft::prep {

namespace {

namespace op = bc::op;
using decompiler::JavaHashSet;
using decompiler::LinkedMap;
using OptString = std::optional<std::string>;
using GenericList = std::vector<OptString>;

struct StringHash {
    std::int32_t operator()(const std::string& s) const { return java_string_hash(s); }
};
using StringSet = JavaHashSet<std::string, StringHash>;

struct RefHash {
    std::size_t operator()(const MemberReference& r) const { return static_cast<std::size_t>(static_cast<std::uint32_t>(reference_hash(r))); }
};

// A null key of a HashMap<String, ...> (the superName of a class that has none)
std::string key_of(const std::optional<std::string>& s) { return s ? *s : std::string("\x01null"); }

int opcode_of(const bc::InsnNode* insn) { return insn == nullptr ? -2 : insn->get_opcode(); }   // (the original throws on a null here: no valid method gets there)

GenericList to_generic_list(const std::vector<std::string>& v) {
    GenericList out;
    for (const std::string& s : v) out.push_back(s);
    return out;
}

[[noreturn]] void null_pointer(const char* what) { throw std::runtime_error(std::string("NullPointerException: ") + what); }

struct Bridge {
    bc::MethodNode* bridge_method;
    bc::MethodNode* invoke_method;
    bc::MethodNode* parent_invoke_method;
    bc::ClassNode* parent;
    GenericList generics;
    std::optional<std::vector<int>> generic_indexes;

    // genericsList == List {Entity, Model}; String[] generics == String[] {Entity, Model, null}; int[] indexes == int[] {1, 2, 0}
    std::vector<int> get_indexes(const GenericList& generics_list) const {
        std::vector<int> indexes(generics.size(), 0);
        for (std::size_t i = 0; i < generics.size(); ++i) {
            const OptString& val = generics[i];
            if (val) {
                const auto it = std::find(generics_list.begin(), generics_list.end(), val);
                indexes[i] = (it == generics_list.end() ? -1 : static_cast<int>(it - generics_list.begin())) + 1;
            }
        }
        return indexes;
    }
};

struct TreeNode {
    bc::ClassNode* class_node;
    TreeNode* parent = nullptr;
    std::vector<TreeNode*> interfaces;
    std::vector<TreeNode*> children;
    bool has_bridges = false;   // (the original's bridges != null)
    std::vector<Bridge> bridges;
    std::unordered_map<std::string, GenericList> generics;   // by key_of(class name)

    explicit TreeNode(bc::ClassNode* node) : class_node(node) {}
    bool has_parent() const { return parent != nullptr; }
    bool has_children() const { return !children.empty(); }
};

}  // namespace

struct GuessGenerics::Impl {
    ClassStorage* storage = nullptr;
    std::vector<std::unique_ptr<TreeNode>> owned;   // every TreeNode ever made (the original's garbage collector keeps those a later one replaced)
    std::vector<TreeNode*> root;                    // ClassTree
    std::unordered_map<MethodReference, std::string, RefHash> rename_map;

    TreeNode* make_tree_node(bc::ClassNode* node) {
        owned.push_back(std::make_unique<TreeNode>(node));
        return owned.back().get();
    }

    template <class F>
    void walk(TreeNode* node, F& delegate) {
        delegate(node);
        for (TreeNode* child : node->children) walk(child, delegate);
    }
    template <class F>
    void walk_all(F delegate) {
        for (TreeNode* node : root) walk(node, delegate);
    }

    static std::string get_interfaces_sig(const std::vector<std::string>& interfaces, const std::unordered_map<std::string, GenericList>& generics) {
        std::string desc;
        for (const std::string& itf : interfaces) {
            desc += "L" + itf;
            const auto it = generics.find(itf);
            if (it != generics.end()) desc += GuessGenerics::get_generics(it->second, false);
            desc += ";";
        }
        return desc;
    }

    static std::string get_method_sig(const std::optional<std::vector<int>>& generic_indexes, const std::string& desc) {
        if (!generic_indexes) null_pointer("genericIndexes");
        const std::vector<int>& gi = *generic_indexes;
        const char start_char = 'T';
        std::string sig = "(";
        const std::vector<bc::Type> types = bc::Type::get_argument_types(desc);
        for (std::size_t i = 0; i < types.size(); ++i) {
            const int index = gi.at(i);
            if (index == 0) {
                sig += types[i].get_descriptor();
            } else {
                sig += "T";
                sig += static_cast<char>(start_char + index - 1);
                sig += ";";
            }
        }
        sig += ")";
        const bc::Type type = bc::Type::get_return_type(desc);
        const int index = gi.at(gi.size() - 1);
        if (index == 0) {
            sig += type.get_descriptor();
        } else {
            sig += "T";
            sig += static_cast<char>(start_char + index - 1);
            sig += ";";
        }
        return sig;
    }

    void apply_signatures(TreeNode* tree_node, std::vector<std::string>& interfaces) {
        bc::ClassNode* node = tree_node->class_node;
        interfaces.insert(interfaces.end(), node->interfaces.begin(), node->interfaces.end());
        const bool has_bridges = tree_node->has_bridges;
        std::vector<Bridge>& bridges = tree_node->bridges;
        const auto found = tree_node->generics.find(key_of(node->super_name));
        const GenericList* generics = found == tree_node->generics.end() ? nullptr : &found->second;
        const std::string interfaces_desc = get_interfaces_sig(node->interfaces, tree_node->generics);
        node->version = std::max(50, node->version);   // Opcodes.V1_6
        if (tree_node->has_parent()) {
            bc::ClassNode* parent_class = tree_node->parent->class_node;
            parent_class->version = std::max(50, parent_class->version);
        }
        const std::string super_name = node->super_name ? *node->super_name : std::string("null");   // (string concatenation of a null)
        const GenericList none;
        if (tree_node->has_parent() && tree_node->has_children()) {
            if (has_bridges) {
                if (generics == nullptr) null_pointer("generics");
                node->signature = GuessGenerics::get_generics(*generics, true) + "L" + super_name +
                                  GuessGenerics::get_generics(static_cast<int>(generics->size())) + ";" + interfaces_desc;
                for (Bridge& bridge : bridges) {
                    bridge.invoke_method->signature = get_method_sig(bridge.generic_indexes, bridge.invoke_method->desc);
                    if (bridge.parent_invoke_method != nullptr) {
                        bridge.parent_invoke_method->signature = get_method_sig(bridge.generic_indexes, bridge.parent_invoke_method->desc);
                    }
                }
            }
        } else if (tree_node->has_parent()) {
            node->signature = "L" + super_name + GuessGenerics::get_generics(generics != nullptr ? *generics : none, false) + ";" + interfaces_desc;
            if (has_bridges) {
                for (Bridge& bridge : bridges) {
                    if (bridge.parent_invoke_method != nullptr) {
                        bridge.parent_invoke_method->signature = get_method_sig(bridge.generic_indexes, bridge.parent_invoke_method->desc);
                    }
                }
            }
        } else if (tree_node->has_children()) {
            node->signature = GuessGenerics::get_generics(generics != nullptr ? *generics : none, true) + "L" + super_name + ";" + interfaces_desc;
        }
    }

    void find_bridges() {
        LinkedMap<const bc::ClassNode*, TreeNode*> tree_map;
        for (bc::ClassNode* node : *storage) {
            std::vector<Bridge> bridges;
            LinkedMap<const bc::ClassNode*, StringSet> all_generics;
            LinkedMap<const bc::ClassNode*, StringSet> all_generics_parent;
            for (bc::MethodNode& method : node->methods) {
                if ((method.access & op::ACC_SYNTHETIC) == 0) continue;
                // May not have bridge modifier? WTF
                bc::InsnNode* insn = method.instructions.get_first();
                while (insn != nullptr && (insn->kind == bc::InsnNode::LABEL || insn->kind == bc::InsnNode::LINE)) insn = insn->get_next();
                if (opcode_of(insn) != op::ALOAD) continue;
                const auto* aload_this = static_cast<const bc::VarInsnNode*>(insn);
                if (aload_this->var != 0) continue;
                StringSet generics;
                StringSet generics_parent;
                insn = insn->get_next();
                const std::vector<bc::Type> types = bc::Type::get_argument_types(method.desc);
                GenericList generic_types(types.size() + 1);
                std::size_t i = 0;
                bool next_method = false;
                for (const bc::Type& type : types) {
                    if (type.get_opcode(op::ILOAD) != opcode_of(insn)) {
                        next_method = true;
                        break;
                    }
                    insn = insn->get_next();
                    if (opcode_of(insn) == op::CHECKCAST) {
                        const auto* typeinsn = static_cast<const bc::TypeInsnNode*>(insn);
                        const std::string desc = "L" + typeinsn->desc + ";";
                        generic_types[i] = desc;
                        generics.add(desc);
                        generics_parent.add(type.get_descriptor());
                        insn = insn->get_next();
                    }
                    i++;
                }
                if (next_method) continue;
                if (opcode_of(insn) != op::INVOKEVIRTUAL) continue;
                bc::MethodNode* invoke_parent_method = nullptr;
                const auto* invokevirtual = static_cast<const bc::MethodInsnNode*>(insn);
                bc::ClassNode* parent_class = node->super_name ? storage->get_class(*node->super_name) : nullptr;
                if (parent_class != nullptr) {
                    for (bc::MethodNode& m : parent_class->methods) {
                        if (m.name == method.name && m.desc == method.desc && (m.access & op::ACC_SYNTHETIC) == 0) {
                            invoke_parent_method = &m;
                            break;
                        }
                    }
                }
                if (invoke_parent_method == nullptr) {
                    bool found = false;
                    for (const std::string& itf : node->interfaces) {
                        bc::ClassNode* itf_node = storage->get_class(itf);
                        if (itf_node == nullptr) continue;
                        for (bc::MethodNode& m : itf_node->methods) {
                            if (m.name == method.name && m.desc == method.desc && (m.access & op::ACC_SYNTHETIC) == 0) {
                                invoke_parent_method = &m;
                                parent_class = itf_node;
                                found = true;
                                break;
                            }
                        }
                        if (found) break;
                    }
                }
                if (invoke_parent_method == nullptr) parent_class = nullptr;
                insn = insn->get_next();
                const bc::Type ret_type = bc::Type::get_return_type(method.desc);
                if (ret_type.get_opcode(op::IRETURN) != opcode_of(insn)) continue;
                const bc::Type ret_type2 = bc::Type::get_return_type(invokevirtual->desc);
                if (ret_type.get_sort() == bc::Type::OBJECT && ret_type2.get_sort() == bc::Type::OBJECT && !(ret_type == ret_type2)) {
                    const std::string desc = ret_type2.get_descriptor();
                    generic_types[generic_types.size() - 1] = desc;
                    generics.add(desc);
                }
                insn = insn->get_next();
                bc::MethodNode* invoke_method = nullptr;
                for (bc::MethodNode& m : node->methods) {
                    if (m.name == invokevirtual->name && m.desc == invokevirtual->desc && (m.access & op::ACC_SYNTHETIC) == 0) {
                        invoke_method = &m;
                        break;
                    }
                }
                if (invoke_method == nullptr) continue;

                if (StringSet* set = all_generics_parent.get(parent_class)) {
                    for (const std::string& s : generics_parent.items()) set->add(s);   // addAll
                } else {
                    all_generics_parent.put(parent_class, generics_parent);
                }
                if (StringSet* set = all_generics.get(parent_class)) {
                    for (const std::string& s : generics.items()) set->add(s);
                } else {
                    all_generics.put(parent_class, generics);
                }
                bridges.push_back(Bridge{&method, invoke_method, invoke_parent_method, parent_class, std::move(generic_types), std::nullopt});
            }
            if (!bridges.empty()) {
                TreeNode* tree_node = make_tree_node(node);
                tree_map.put(node, tree_node);
                tree_node->has_bridges = true;
                tree_node->bridges = std::move(bridges);
                bc::ClassNode* parent = node->super_name ? storage->get_class(*node->super_name) : nullptr;
                const StringSet* set = all_generics.get(parent);
                if (parent != nullptr && set != nullptr) {
                    const GenericList generics_self = to_generic_list(set->items());
                    tree_node->generics[parent->name] = generics_self;
                    for (Bridge& bridge : tree_node->bridges) {
                        if (bridge.parent == parent) bridge.generic_indexes = bridge.get_indexes(generics_self);
                    }
                    TreeNode* const* found = tree_map.get(parent);
                    TreeNode* tree_node_parent = found != nullptr ? *found : make_tree_node(parent);
                    tree_node->parent = tree_node_parent;
                    const StringSet* gp = all_generics_parent.get(parent);
                    if (gp == nullptr) null_pointer("allGenericsParent.get(parent)");
                    tree_node->generics[key_of(parent->super_name)] = to_generic_list(gp->items());
                    tree_map.put(parent, tree_node_parent);
                }
                // TODO (the original's)
                for (const std::string& itf : node->interfaces) {
                    bc::ClassNode* parent2 = storage->get_class(itf);
                    const StringSet* set2 = all_generics.get(parent2);
                    if (parent2 == nullptr || set2 == nullptr) continue;
                    const GenericList generics_self = to_generic_list(set2->items());
                    tree_node->generics[parent2->name] = generics_self;
                    for (Bridge& bridge : tree_node->bridges) {
                        if (bridge.parent == parent2) bridge.generic_indexes = bridge.get_indexes(generics_self);
                    }
                    TreeNode* const* found = tree_map.get(parent2);
                    TreeNode* tree_node_parent = found != nullptr ? *found : make_tree_node(parent2);
                    tree_node->interfaces.push_back(tree_node_parent);
                    const StringSet* gp = all_generics_parent.get(parent2);
                    if (gp == nullptr) null_pointer("allGenericsParent.get(parent2)");
                    tree_node->generics[key_of(parent2->super_name)] = to_generic_list(gp->items());
                    tree_map.put(parent2, tree_node_parent);
                }
            }
        }
        std::vector<TreeNode*> tree_nodes;
        for (const auto& [key, tree_node] : tree_map) tree_nodes.push_back(tree_node);
        for (bc::ClassNode* node : *storage) {
            TreeNode* const* own = tree_map.get(node);
            TreeNode* tree_node = own != nullptr ? *own : nullptr;
            bc::ClassNode* class_node = node->super_name ? storage->get_class(*node->super_name) : nullptr;
            TreeNode* const* found_parent = tree_map.get(class_node);
            TreeNode* parent = found_parent != nullptr ? *found_parent : nullptr;
            if (parent != nullptr) {
                if (tree_node != nullptr) {
                    parent->children.push_back(tree_node);
                } else {
                    TreeNode* new_node = make_tree_node(node);
                    new_node->parent = parent;
                    const auto val = parent->generics.find(key_of(parent->class_node->super_name));
                    if (val != parent->generics.end()) new_node->generics[key_of(node->super_name)] = val->second;
                    parent->children.push_back(new_node);
                    tree_nodes.push_back(new_node);
                }
            }
        }
        root.clear();
        for (TreeNode* node : tree_nodes) {   // ClassTree
            if (!node->has_parent()) root.push_back(node);
        }
    }

    void rename_references() {
        walk_all([this](TreeNode* tree_node) {
            if (!tree_node->has_bridges) return;
            for (Bridge& pair : tree_node->bridges) {
                if (pair.parent_invoke_method == nullptr) {
                    const std::string rename = pair.bridge_method->name;
                    rename_map[MethodReference{tree_node->class_node->name, pair.invoke_method->name, pair.invoke_method->desc}] = rename;
                    pair.invoke_method->name = rename;
                    continue;
                }
                const std::string rename = pair.parent_invoke_method->name;
                rename_map[MethodReference{tree_node->class_node->name, pair.bridge_method->name, pair.bridge_method->desc}] = rename;
                pair.bridge_method->name = rename;
                rename_map[MethodReference{tree_node->class_node->name, pair.invoke_method->name, pair.invoke_method->desc}] = rename;
                pair.invoke_method->name = rename;
            }
        });
        for (bc::ClassNode* node : *storage) {
            for (bc::MethodNode& method : node->methods) {
                for (bc::InsnNode* insn : method.instructions) {
                    if (insn->kind != bc::InsnNode::METHOD_INSN) continue;
                    auto* invoke = static_cast<bc::MethodInsnNode*>(insn);
                    const auto it = rename_map.find(MethodReference{invoke->owner, invoke->name, invoke->desc});
                    if (it != rename_map.end()) invoke->name = it->second;
                }
            }
        }
    }

    void verify_indexes() {
        walk_all([this](TreeNode* tree_node) {
            (void)this;
            LinkedMap<const bc::MethodNode*, std::optional<std::vector<int>>> method_indexes;
            for (TreeNode* child : tree_node->children) {
                if (!child->has_bridges) continue;
                for (Bridge& bridge : child->bridges) {
                    if (bridge.parent_invoke_method == nullptr) continue;
                    std::optional<std::vector<int>>* cur = method_indexes.get(bridge.parent_invoke_method);
                    if (cur == nullptr || !cur->has_value()) {
                        method_indexes.put(bridge.parent_invoke_method, bridge.generic_indexes);
                    } else {
                        const std::vector<int> indexes = **cur;
                        if (!bridge.generic_indexes) null_pointer("bridge.genericIndexes");
                        for (std::size_t i = 0; i < indexes.size(); ++i) {
                            if (indexes[i] < bridge.generic_indexes->at(i)) method_indexes.put(bridge.parent_invoke_method, bridge.generic_indexes);
                        }
                    }
                }
            }
            bool verify_generics = true;
            for (TreeNode* child : tree_node->children) {
                std::vector<TreeNode*> tree_nodes;
                tree_nodes.push_back(tree_node);
                tree_nodes.insert(tree_nodes.end(), child->interfaces.begin(), child->interfaces.end());
                for (TreeNode* parent : tree_nodes) {
                    if (parent == nullptr || !child->has_bridges) continue;
                    GenericList child_generics(1);
                    GenericList generics(1);
                    for (Bridge& bridge : child->bridges) {
                        if (bridge.parent != parent->class_node) continue;
                        const std::optional<std::vector<int>>* found = method_indexes.get(bridge.parent_invoke_method);
                        if (found == nullptr || !found->has_value()) continue;
                        const std::vector<int> indexes = **found;
                        bridge.generic_indexes = indexes;
                        for (std::size_t i = 0; i < indexes.size(); ++i) {
                            if (indexes[i] > 0) {
                                const auto idx = static_cast<std::size_t>(indexes[i]);
                                child_generics = child_generics.size() < idx ? array_copy(child_generics, idx) : child_generics;
                                child_generics[idx - 1] = bridge.generics.at(i);
                                if (verify_generics) {
                                    generics = generics.size() < idx ? array_copy(generics, idx) : generics;
                                    if (i == indexes.size() - 1) {
                                        const bc::Type type = bc::Type::get_return_type(bridge.parent_invoke_method->desc);
                                        generics[idx - 1] = type.get_descriptor();
                                    } else {
                                        const std::vector<bc::Type> types = bc::Type::get_argument_types(bridge.parent_invoke_method->desc);
                                        generics[idx - 1] = types.at(i).get_descriptor();
                                    }
                                }
                            }
                        }
                    }
                    child->generics[parent->class_node->name] = child_generics;
                    if (verify_generics) {
                        parent->generics[key_of(parent->class_node->super_name)] = generics;
                        verify_generics = false;
                    }
                }
            }
        });
    }
};

GuessGenerics::GuessGenerics(ClassStorage& storage) : storage_(&storage) {}

GuessGenerics::~GuessGenerics() = default;

void GuessGenerics::transform(ClassStorage& storage) {
    storage_ = &storage;
    impl_ = std::make_unique<Impl>();
    impl_->storage = &storage;
    impl_->find_bridges();
    impl_->verify_indexes();
    std::vector<std::string> interfaces;
    impl_->walk_all([&](TreeNode* tree_node) { impl_->apply_signatures(tree_node, interfaces); });
    impl_->rename_references();
}

std::string GuessGenerics::get_generics(int size) {
    if (size == 0) return "";
    char start_char = 'T';
    std::string generics_self = "<";
    for (int i = 0; i < size; ++i) {
        generics_self += 'T';
        generics_self += start_char;
        generics_self += ";";
        start_char += 1;
    }
    generics_self += ">";
    return generics_self;
}

std::string GuessGenerics::get_generics(const std::vector<std::optional<std::string>>& generics, bool parameterized) {
    if (generics.empty()) return "";
    char start_char = 'T';
    std::string generics_self = "<";
    for (const std::optional<std::string>& s : generics) {
        if (parameterized) {
            generics_self += start_char;
            generics_self += ":";
            generics_self += s ? *s : std::string("null");
            start_char += 1;
        } else {
            generics_self += s ? *s : std::string("null");
        }
    }
    generics_self += ">";
    return generics_self;
}

}  // namespace cb::java::minecraft::prep
