// fixes.cpp -- RDI's fixes: Transform (the global transformations), the visitors ClassInitAdder / AddExceptions / AccessFixer, Access, Exceptions and
// their loaders, and the util classes DescString / NodeHelper. See java/projects/minecraft/prep/fixes.hpp.
//
// Ported from RDI (org.mcphackers.rdi.injector.transform.Transform, injector.visitors.*, injector.data.Access / Exceptions / Data, util.DescString /
// NodeHelper), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include "java/projects/minecraft/prep/fixes.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

#include "java/decompiler/java_util.hpp"

namespace cb::java::minecraft::prep {

namespace {

namespace op = bc::op;
using Level = Access::Level;

struct StringHash {
    std::int32_t operator()(const std::string& s) const { return java_string_hash(s); }
};
struct RefHash {
    std::size_t operator()(const MemberReference& r) const { return static_cast<std::size_t>(static_cast<std::uint32_t>(reference_hash(r))); }
};

// util.OPHelper's isVarStore / reverseVarOpcode (area D owns OPHelper; these two are what decomposeVars needs of it)
bool is_var_store(int opcode) {
    return opcode == op::ASTORE || opcode == op::DSTORE || opcode == op::FSTORE || opcode == op::ISTORE || opcode == op::LSTORE;
}
bool is_var_load(int opcode) {
    return opcode == op::ALOAD || opcode == op::DLOAD || opcode == op::FLOAD || opcode == op::ILOAD || opcode == op::LLOAD;
}
int reverse_var_opcode(int opcode) {
    switch (opcode) {
        case op::AALOAD: return op::AASTORE;
        case op::BALOAD: return op::BASTORE;
        case op::CALOAD: return op::CASTORE;
        case op::IALOAD: return op::IASTORE;
        case op::SALOAD: return op::IASTORE;
        case op::DALOAD: return op::IASTORE;
        case op::FALOAD: return op::IASTORE;
        case op::LALOAD: return op::IASTORE;
        case op::ALOAD: return op::IASTORE;
        case op::DLOAD: return op::IASTORE;
        case op::FLOAD: return op::IASTORE;
        case op::ILOAD: return op::IASTORE;
        case op::LLOAD: return op::IASTORE;
        case op::AASTORE: return op::AALOAD;
        case op::BASTORE: return op::BALOAD;
        case op::CASTORE: return op::CALOAD;
        case op::IASTORE: return op::IALOAD;
        case op::SASTORE: return op::SALOAD;
        case op::DASTORE: return op::DALOAD;
        case op::FASTORE: return op::FALOAD;
        case op::LASTORE: return op::LALOAD;
        case op::ASTORE: return op::ALOAD;
        case op::DSTORE: return op::DLOAD;
        case op::FSTORE: return op::FLOAD;
        case op::ISTORE: return op::ILOAD;
        case op::LSTORE: return op::LLOAD;
        default: throw std::invalid_argument("Opcode not valid.");
    }
}

// String.split(regex) for a one-character literal delimiter: a string with no delimiter is its own one element; trailing empty strings go.
std::vector<std::string> java_split(const std::string& s, char delim) {
    std::vector<std::string> out;
    if (s.find(delim) == std::string::npos) return {s};
    std::size_t start = 0;
    for (;;) {
        const std::size_t at = s.find(delim, start);
        if (at == std::string::npos) { out.push_back(s.substr(start)); break; }
        out.push_back(s.substr(start, at - start));
        start = at + 1;
    }
    while (!out.empty() && out.back().empty()) out.pop_back();
    return out;
}

// String.trim: every character up to U+0020 goes from both ends
std::string java_trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && static_cast<unsigned char>(s[a]) <= ' ') ++a;
    while (b > a && static_cast<unsigned char>(s[b - 1]) <= ' ') --b;
    return s.substr(a, b - a);
}

// Files.readAllLines: lines end at \n, \r or \r\n
std::optional<std::vector<std::string>> read_all_lines(const std::filesystem::path& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in) return std::nullopt;
    std::stringstream ss;
    ss << in.rdbuf();
    const std::string text = ss.str();
    std::vector<std::string> lines;
    std::string cur;
    bool pending = false;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (c == '\n' || c == '\r') {
            if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') ++i;
            lines.push_back(cur);
            cur.clear();
            pending = false;
        } else {
            cur += c;
            pending = true;
        }
    }
    if (pending) lines.push_back(cur);
    return lines;
}

std::string substring(const std::string& s, std::size_t from, std::size_t to) {   // String.substring(begin, end) (StringIndexOutOfBounds checked)
    if (from > to || to > s.size()) throw std::out_of_range("begin " + std::to_string(from) + ", end " + std::to_string(to) + ", length " + std::to_string(s.size()));
    return s.substr(from, to - from);
}

}  // namespace

// ---- DescString

DescString::DescString(const std::string& desc) {
    const std::size_t end = desc.rfind(')');
    desc_ = substring(desc, 1, end);   // always starts with a parenthesis
}

std::string DescString::next_type() {
    const char type = desc_.at(start_index_);
    if (type == 'L') {
        // Object-type type: the description ends with a semicolon here, which has to be kept
        const std::size_t end_pos = desc_.find(';', start_index_) + 1;
        std::string ret = desc_.substr(start_index_, end_pos - start_index_);
        start_index_ = end_pos;
        return ret;
    }
    if (type == '[') {
        std::size_t type_position = std::string::npos;
        for (std::size_t i = start_index_ + 1; i < desc_.size(); ++i) {
            if (desc_[i] != '[') {
                type_position = i;
                break;
            }
        }
        if (type_position == std::string::npos) throw std::out_of_range("array index -1");   // (the original's ArrayIndexOutOfBounds)
        if (desc_[type_position] == 'L') {
            const std::size_t end_pos = desc_.find(';', start_index_) + 1;
            std::string ret = desc_.substr(start_index_, end_pos - start_index_);
            start_index_ = end_pos;
            return ret;
        }
        ++type_position;
        std::string ret = desc_.substr(start_index_, type_position - start_index_);
        start_index_ = type_position;
        return ret;
    }
    ++start_index_;   // a primitive: exactly one character
    return std::string(1, type);
}

// ---- NodeHelper

bc::FieldNode* NodeHelper::get_field(bc::ClassNode& node, const std::string& name, const std::string& desc) {
    for (bc::FieldNode& f : node.fields) {
        if (f.name == name && f.desc == desc) return &f;
    }
    return nullptr;
}

bc::MethodNode* NodeHelper::get_method(bc::ClassNode& node, const std::string& name, const std::string& desc) {
    for (bc::MethodNode& m : node.methods) {
        if (m.name == name && m.desc == desc) return &m;
    }
    return nullptr;
}

std::vector<bc::MethodNode*> NodeHelper::get_constructors(bc::ClassNode& node) {
    std::vector<bc::MethodNode*> constructors;
    for (bc::MethodNode& m : node.methods) {
        if (m.name == "<init>") constructors.push_back(&m);
    }
    return constructors;
}

// ---- Access, Exceptions

Access::Level Access::get_from_bytecode(int acc) {
    if ((acc & op::ACC_PRIVATE) == op::ACC_PRIVATE) return Level::PRIVATE;
    if ((acc & op::ACC_PROTECTED) == op::ACC_PROTECTED) return Level::PROTECTED;
    if ((acc & op::ACC_PUBLIC) == op::ACC_PUBLIC) return Level::PUBLIC;
    return Level::DEFAULT;
}

int Access::set_access(Level level, int acc) {
    acc &= ~Transform::VISIBILITY_MODIFIERS;
    acc |= level == Level::PRIVATE ? op::ACC_PRIVATE : 0;
    acc |= level == Level::PROTECTED ? op::ACC_PROTECTED : 0;
    acc |= level == Level::PUBLIC ? op::ACC_PUBLIC : 0;
    return acc;
}

std::optional<Access::Level> Access::get_level(const std::string& class_name) const {
    const auto it = classes_.find(class_name);
    return it == classes_.end() ? std::nullopt : std::optional<Level>(it->second);
}

std::optional<Access::Level> Access::get_field_level(const FieldReference& field) const {
    // Recreating reference without the descriptor
    const auto it = fields_.find(FieldReference{field.owner, field.name, ""});
    return it == fields_.end() ? std::nullopt : std::optional<Level>(it->second);
}

std::optional<Access::Level> Access::get_method_level(const MethodReference& method) const {
    const auto it = methods_.find(method);
    return it == methods_.end() ? std::nullopt : std::optional<Level>(it->second);
}

std::vector<std::string> Exceptions::get_exceptions(const MethodReference& ref) const {
    const auto it = exceptions_.find(ref);
    return it == exceptions_.end() ? std::vector<std::string>{} : it->second;
}

std::optional<Exceptions> load_exceptions(const std::filesystem::path& file) {
    Exceptions exceptions;
    const auto lines = read_all_lines(file);
    if (!lines) {
        std::fprintf(stderr, "java.io.IOException: cannot read %s\n", file.string().c_str());
        return std::nullopt;
    }
    for (std::string line : *lines) {
        line = java_trim(line);
        if (line.empty() || line.front() == '#') continue;
        // New: net/minecraft/client/Minecraft/startGame ()V org/lwjgl/LWJGLException java/io/IOException
        // Old: net/minecraft/client/Minecraft.startGame()V=org/lwjgl/LWJGLException,java/io/IOException
        const bool old_format = line.find('.') != std::string::npos;
        std::size_t idx;
        if (old_format) {
            idx = line.rfind('=');
        } else {
            const std::size_t first = line.find(' ');
            idx = line.find(' ', first + 1);   // (first + 1 is 0 when there is no space, as the original's indexOf(' ', -1 + 1))
        }
        if (idx == std::string::npos) continue;
        std::string s = substring(line, 0, idx);
        if (old_format) {
            std::replace(s.begin(), s.end(), '.', '/');
            std::string with_space;
            for (const char c : s) {
                if (c == '(') with_space += ' ';
                with_space += c;
            }
            s = with_space;
        }
        const std::size_t last_space = s.rfind(' ');
        if (last_space == std::string::npos) throw std::out_of_range("begin 0, end -1, length " + std::to_string(s.size()));
        const std::string s2 = s.substr(0, last_space);
        const std::string desc = s.substr(last_space + 1);
        const std::size_t last_slash = s2.rfind('/');
        if (last_slash == std::string::npos) throw std::out_of_range("begin 0, end -1, length " + std::to_string(s2.size()));
        const std::string name = s2.substr(last_slash + 1);
        const std::string owner = s2.substr(0, last_slash);
        exceptions.set_exceptions(MethodReference{owner, name, desc}, java_split(line.substr(idx + 1), old_format ? ',' : ' '));
    }
    return exceptions;
}

std::optional<Access> load_access(const std::filesystem::path& file) {
    Access access;
    const auto lines = read_all_lines(file);
    if (!lines) {
        std::fprintf(stderr, "java.io.IOException: cannot read %s\n", file.string().c_str());
        return std::nullopt;
    }
    for (std::string line : *lines) {
        line = java_trim(line);
        if (line.empty() || line.front() == '#') continue;
        // PUBLIC net/minecraft/client/Minecraft
        // PRIVATE net/minecraft/client/Minecraft fullscreen
        // PROTECTED net/minecraft/client/Minecraft startGame ()V
        const std::size_t idx = line.find(' ');
        if (idx == std::string::npos) throw std::out_of_range("begin 0, end -1, length " + std::to_string(line.size()));
        const std::string word = line.substr(0, idx);
        Level level;
        if (word == "PRIVATE") level = Level::PRIVATE;
        else if (word == "DEFAULT") level = Level::DEFAULT;
        else if (word == "PROTECTED") level = Level::PROTECTED;
        else if (word == "PUBLIC") level = Level::PUBLIC;
        else throw std::invalid_argument("No enum constant org.mcphackers.rdi.injector.data.Access.Level." + word);
        const std::vector<std::string> keys = java_split(java_trim(line.substr(idx + 1)), ' ');
        switch (keys.size()) {
            case 1: access.put_class(keys[0], level); break;
            case 2: access.put_field(keys[0], keys[1], level); break;
            case 3: access.put_method(MethodReference{keys[0], keys[1], keys[2]}, level); break;
            default: break;
        }
    }
    return access;
}

// ---- Transform

void Transform::fix_inner_classes(ClassStorage& storage) {
    using decompiler::JavaHashMap;
    using InnerList = std::vector<bc::InnerClassNode>;
    std::unordered_map<std::string, bc::InnerClassNode> split_inner;
    std::unordered_set<std::string> enums;
    JavaHashMap<std::string, InnerList, StringHash> parents;   // iterated

    // Initial indexing sweep
    for (bc::ClassNode* node : storage) {
        parents.put(node->name, InnerList{});
        if (node->super_name && *node->super_name == "java/lang/Enum") enums.insert(node->name);   // Register enum
    }
    // Second sweep
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        // Sweep enum members
        if (node.super_name && enums.contains(*node.super_name)) {
            // Child of (abstract) enum
            bool skip = false;
            for (const bc::InnerClassNode& inner_node : node.inner_classes) {
                if (node.name == inner_node.name) {
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                // Apply fixup. 16400 = ACC_FINAL | ACC_ENUM
                bc::InnerClassNode inner_node{node.name, std::nullopt, std::nullopt, 16400};
                if (InnerList* list = parents.get(*node.super_name)) list->push_back(inner_node);
                node.outer_class = node.super_name;
                node.inner_classes.push_back(inner_node);
            }
        } else if (node.name.find('$') != std::string::npos) {
            // Partially unobfuscated inner class.
            // This operation cannot be performed during the first sweep
            bool skip = false;
            for (const bc::InnerClassNode& innernode : node.inner_classes) {
                if (innernode.name == node.name) {
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                std::size_t last_separator = node.name.rfind('$');
                const std::string outer_node = node.name.substr(0, last_separator++);
                const std::string inner_most = node.name.substr(last_separator);
                bc::InnerClassNode inner_class_node;
                if (!inner_most.empty() && std::all_of(inner_most.begin(), inner_most.end(), [](char c) { return c >= '0' && c <= '9'; })) {
                    // Anonymous class: we know that ACC_SUPER is invalid for inner classes, so we remove that flag
                    inner_class_node = bc::InnerClassNode{node.name, std::nullopt, std::nullopt, node.access & ~op::ACC_SUPER};
                    node.outer_class = outer_node;
                } else {
                    // We need to check for static inner classes. We already know that anonymous classes can never be static classes by
                    // definition, so we can skip that step for anonymous classes
                    bool static_inner_class = false;
                    bool implicit_static = false;
                    // Interfaces, Enums and Records are implicitly static
                    if (!static_inner_class) {
                        static_inner_class = (node.access & op::ACC_INTERFACE) != 0 || (node.access & op::ACC_RECORD) != 0 ||
                                             ((node.access & op::ACC_ENUM) != 0 && node.super_name && *node.super_name == "java/lang/Enum");
                        implicit_static = static_inner_class;
                    }
                    // Member classes of interfaces are implicitly static
                    if (!static_inner_class) {
                        const bc::ClassNode* outer_class_node = storage.get_class(outer_node);
                        static_inner_class = outer_class_node != nullptr && (outer_class_node->access & op::ACC_INTERFACE) != 0;
                        implicit_static = static_inner_class;
                    }
                    // The constructor of non-static inner classes must take in an instance of the outer class an argument
                    if (!static_inner_class) {
                        bool static_constructor = false;
                        for (const bc::MethodNode& method : node.methods) {
                            if (method.name == "<init>") {
                                const std::size_t outernode_len = outer_node.length();
                                if (outernode_len + 2 > method.desc.length()) {
                                    // The reference to the outer class cannot be passed in via a parameter as there is no space for it in the
                                    // descriptor, so the class has to be static
                                    static_constructor = true;
                                    break;
                                }
                                const std::string arg = method.desc.substr(2, outernode_len);
                                if (arg != outer_node) {
                                    // Has to be static. The other parameters are irrelevant as the outer class reference is always at first place.
                                    static_constructor = true;
                                    break;
                                }
                            }
                        }
                        if (static_constructor) {
                            static_inner_class = true;
                            implicit_static = false;
                        }
                    }
                    if (static_inner_class && !implicit_static) {
                        for (const bc::FieldNode& field : node.fields) {
                            if ((field.access & op::ACC_FINAL) != 0 && field.name.starts_with("this$")) {
                                std::fprintf(stderr, "Falsely identified %s as static inner class.\n", node.name.c_str());
                                static_inner_class = false;
                            }
                        }
                    }

                    int inner_class_access = node.access & ~op::ACC_SUPER;   // Super is not allowed for inner class nodes

                    // Don't fall to the temptation of adding ACC_STATIC to the class node. According the the ASM verifier it is not legal to do
                    // so. However the JVM does not seem care. Nonetheless, we are not adding it the access flags of the class, though we will
                    // add it in the inner class node
                    if (!static_inner_class) {
                        node.outer_class = outer_node;
                    } else {
                        inner_class_access |= op::ACC_STATIC;
                    }
                    inner_class_node = bc::InnerClassNode{node.name, outer_node, inner_most, inner_class_access};
                }
                if (InnerList* inner_nodes = parents.get(outer_node)) inner_nodes->push_back(inner_class_node);
                split_inner[node.name] = inner_class_node;
                node.inner_classes.push_back(inner_class_node);
            }
        }
    }
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        // General sweep
        std::vector<bc::InnerClassNode> inner_nodes_to_add;
        for (const bc::FieldNode& field : node.fields) {
            std::string descriptor = field.desc;
            if (descriptor.length() < 4) continue;   // Most likely a primitive
            if (descriptor[0] == '[') {
                descriptor = descriptor.substr(2, descriptor.length() - 1 - 2);   // Array
            } else {
                descriptor = descriptor.substr(1, descriptor.length() - 1 - 1);   // Non-array
            }
            const auto it = split_inner.find(descriptor);
            if (it != split_inner.end()) {
                if (!it->second.inner_name && !field.name.starts_with("this$")) {
                    // Not fatal, but worrying
                    std::fprintf(stderr, "Unlikely field descriptor for field \"%s\" with descriptor %s in class %s\n", field.name.c_str(),
                                 field.desc.c_str(), node.name.c_str());
                }
                inner_nodes_to_add.push_back(it->second);
            }
        }
        // Apply inner nodes
        std::unordered_set<std::string> entry_names;
        for (const bc::InnerClassNode& inner : inner_nodes_to_add) {
            if (entry_names.insert(inner.name).second) node.inner_classes.push_back(inner);
        }
    }
    // Add inner classes to the parent of the anonymous classes
    for (auto& [key, list] : parents.entries()) {
        // Remove duplicates
        std::unordered_set<std::string> entry_names;
        InnerList deduped;
        for (const bc::InnerClassNode& inner : list) {
            if (entry_names.insert(inner.name).second) deduped.push_back(inner);
        }
        bc::ClassNode* node = storage.get_class(key);
        for (const bc::InnerClassNode& inner_entry : deduped) {
            bool skip = false;
            for (const bc::InnerClassNode& inner : node->inner_classes) {
                if (inner.name == inner_entry.name) {
                    skip = true;
                    break;
                }
            }
            if (!skip) node->inner_classes.push_back(inner_entry);
        }
    }
}

int Transform::fix_switch_maps(ClassStorage& storage) {
    // The deobf name will be something like $SwitchMap$org$bukkit$Material
    std::unordered_map<FieldReference, std::string, RefHash> deobf_names;

    // index switch map classes - or at least their candidates
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        if (node.super_name && *node.super_name == "java/lang/Object" && node.interfaces.empty()) {
            if (node.fields.size() == 1 && node.methods.size() == 1) {
                bc::MethodNode& method = node.methods[0];
                bc::FieldNode& field = node.fields[0];
                if (method.name == "<clinit>" && method.desc == "()V" && field.desc == "[I" && (field.access & op::ACC_STATIC) != 0) {
                    const FieldReference field_ref{node.name, field.name, field.desc};
                    std::optional<std::string> enum_name;
                    bc::InsnNode* instruction = method.instructions.get_first();
                    while (instruction != nullptr) {
                        if (instruction->kind == bc::InsnNode::FIELD_INSN && instruction->get_opcode() == op::GETSTATIC) {
                            const auto* field_instruction = static_cast<const bc::FieldInsnNode*>(instruction);
                            if (field_ref == FieldReference{field_instruction->owner, field_instruction->name, field_instruction->desc}) {
                                bc::InsnNode* next = instruction->get_next();
                                while (next != nullptr && (next->kind == bc::InsnNode::FRAME || next->kind == bc::InsnNode::LABEL)) {
                                    next = next->get_next();   // ASM is sometimes not so nice
                                }
                                if (next != nullptr && next->kind == bc::InsnNode::FIELD_INSN && next->get_opcode() == op::GETSTATIC) {
                                    const std::string& next_owner = static_cast<const bc::FieldInsnNode*>(next)->owner;
                                    if (!enum_name) {
                                        enum_name = next_owner;
                                    } else if (*enum_name != next_owner) {
                                        enum_name.reset();
                                        break;   // It may not be a switchmap field
                                    }
                                }
                            }
                        }
                        instruction = instruction->get_next();
                    }
                    if (enum_name) {
                        if (field_ref.name.find('$') == std::string::npos) {
                            // The deobf name will be something like $SwitchMap$org$bukkit$Material
                            std::string flat = *enum_name;
                            std::replace(flat.begin(), flat.end(), '/', '$');
                            const std::string new_name = "$SwitchMap$" + flat;
                            deobf_names[field_ref] = new_name;
                            instruction = method.instructions.get_first();
                            // Remap references within this class
                            while (instruction != nullptr) {
                                if (instruction->kind == bc::InsnNode::FIELD_INSN) {
                                    auto* field_insn = static_cast<bc::FieldInsnNode*>(instruction);
                                    if ((field_insn->get_opcode() == op::GETSTATIC || field_insn->get_opcode() == op::PUTSTATIC) && field_insn->owner == node.name &&
                                        field_ref == FieldReference{field_insn->owner, field_insn->name, field_insn->desc}) {
                                        field_insn->name = new_name;
                                    }
                                }
                                instruction = instruction->get_next();
                            }
                            // Remap the actual field declaration. Switch maps can only contain a single field and we have already obtained said field
                            field.name = new_name;
                        }
                    }
                }
            }
        }
    }

    // Rename references to the field
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        const std::unordered_set<std::string> added_inner_class_nodes;   // (the original never adds to it)
        for (bc::MethodNode& method : node.methods) {
            bc::InsnNode* instruction = method.instructions.get_first();
            while (instruction != nullptr) {
                if (instruction->kind == bc::InsnNode::FIELD_INSN && instruction->get_opcode() == op::GETSTATIC) {
                    auto* field_instruction = static_cast<bc::FieldInsnNode*>(instruction);
                    if (field_instruction->owner == node.name) {   // I have no real idea what I was doing here
                        instruction = instruction->get_next();
                        continue;
                    }
                    const FieldReference f_ref{field_instruction->owner, field_instruction->name, field_instruction->desc};
                    const auto found = deobf_names.find(f_ref);
                    if (found != deobf_names.end()) {
                        field_instruction->name = found->second;
                        if (!added_inner_class_nodes.contains(f_ref.owner)) {
                            const bc::InnerClassNode inner_class_node{f_ref.owner, node.name, std::nullopt, op::ACC_STATIC ^ op::ACC_SYNTHETIC ^ op::ACC_FINAL};
                            bc::ClassNode* outer_node = storage.get_class(f_ref.owner);
                            if (outer_node != nullptr) outer_node->inner_classes.push_back(inner_class_node);
                            bc::ClassNode* outermost_classnode = nullptr;
                            if (node.outer_class) outermost_classnode = storage.get_class(*node.outer_class);
                            if (outermost_classnode == nullptr) {
                                for (const bc::InnerClassNode& inner : node.inner_classes) {
                                    if (inner.name == node.name && inner.outer_name) {
                                        outermost_classnode = storage.get_class(*inner.outer_name);
                                        break;
                                    }
                                }
                            }
                            if (outermost_classnode != nullptr) outermost_classnode->inner_classes.push_back(inner_class_node);
                            node.inner_classes.push_back(inner_class_node);
                        }
                    }
                }
                instruction = instruction->get_next();
            }
        }
    }

    return static_cast<int>(deobf_names.size());
}

int Transform::guess_anonymous_inner_classes(ClassStorage& storage) {
    // Class name -> referenced class, method (iterated: a LinkedHashMap)
    using Invoker = std::optional<Pair<std::string, bc::MethodNode*>>;
    decompiler::LinkedMap<std::string, Invoker> candidates;
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        if ((node.access & VISIBILITY_MODIFIERS) != 0) continue;   // Anonymous inner classes are always package-private
        bool skip_class = false;
        const bc::FieldNode* outer_class_reference = nullptr;
        for (const bc::FieldNode& field : node.fields) {
            constexpr int required_flags = op::ACC_SYNTHETIC | op::ACC_FINAL;
            if ((field.access & required_flags) == required_flags && (field.access & VISIBILITY_MODIFIERS) == 0) {
                if (outer_class_reference != nullptr) {
                    skip_class = true;
                    break;   // short-circuit
                }
                outer_class_reference = &field;
            }
        }
        if (skip_class || outer_class_reference == nullptr) continue;
        // anonymous classes can only have a single constructor since they are only created at a single spot. However they also have to have a
        // constructor so they can pass the outer class reference
        const bc::MethodNode* constructor = nullptr;
        for (const bc::MethodNode& method : node.methods) {
            if (method.name == "<init>") {
                if (constructor != nullptr) {
                    skip_class = true;   // cannot have multiple constructors
                    break;
                }
                if ((method.access & VISIBILITY_MODIFIERS) != 0) {
                    skip_class = true;   // The constructor should be package - protected
                    break;
                }
                constructor = &method;
            }
        }
        if (skip_class || constructor == nullptr) continue;   // require a single constructor, not more, not less
        // since we have the potential reference to the outer class and we know that it has to be set via the constructor's parameter, we can check
        // whether this is the case here
        DescString desc(constructor->desc);
        skip_class = true;
        while (desc.has_next()) {
            const std::string type = desc.next_type();
            if (type == outer_class_reference->desc) {
                skip_class = false;
                break;
            }
        }
        if (skip_class) continue;
        const std::size_t dollar_index = node.name.find('$');
        if (dollar_index != std::string::npos) {
            if (dollar_index + 1 >= node.name.size()) throw std::out_of_range("Index " + std::to_string(dollar_index + 1) + " out of bounds");
            const char after = node.name[dollar_index + 1];
            if (!(after >= '0' && after <= '9')) continue;   // Unobfuscated class that is 100% not anonymous
        }
        candidates.put(node.name, std::nullopt);
    }

    // Make sure that the constructor is only invoked in a single class, which should be the outer class
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        for (bc::MethodNode& method : node.methods) {
            for (bc::InsnNode* instruction = method.instructions.get_first(); instruction != nullptr; instruction = instruction->get_next()) {
                if (instruction->kind != bc::InsnNode::METHOD_INSN) continue;
                const auto* method_invocation = static_cast<const bc::MethodInsnNode*>(instruction);
                if (method_invocation->name != "<init>") continue;
                const std::string& owner = method_invocation->owner;
                if (candidates.contains_key(owner)) {
                    if (owner == node.name) {
                        candidates.remove(owner);   // this is no really valid anonymous class
                    } else {
                        const Invoker invoker = *candidates.get(owner);
                        if (!invoker) {
                            candidates.put(owner, Pair<std::string, bc::MethodNode*>::of(node.name, &method));
                        } else if (invoker->left != node.name || invoker->right->name != method.name || invoker->right->desc != method.desc) {
                            // constructor referenced by multiple classes, cannot be valid
                            candidates.remove(owner);
                        }
                    }
                }
            }
        }
    }

    // If another class has a field reference to the potential anonymous class, and that field is not synthetic, then the class is likely not anonymous.
    for (bc::ClassNode* node_ptr : storage) {
        for (const bc::FieldNode& field : node_ptr->fields) {
            if (field.desc.length() == 1 || (field.access & op::ACC_SYNTHETIC) != 0) continue;
            const std::size_t bracket = field.desc.rfind('[');
            const std::size_t after_brackets = bracket == std::string::npos ? 0 : bracket + 1;   // lastIndexOf('[') + 1
            if (field.desc.at(after_brackets) != 'L') continue;
            // Now technically, they are still inner classes. Just regular ones and they are not static ones however not adding them as a inner
            // class has no effect in recomplieabillity so we will not really care about it just yet.
            const std::string class_name = substring(field.desc, after_brackets + 1, field.desc.length() - 1);
            candidates.remove(class_name);
        }
    }

    int added_inners = 0;
    for (const auto& [inner, outer] : candidates) {
        if (!outer) continue;
        bc::ClassNode* inner_node = storage.get_class(inner);
        bc::ClassNode* outernode = storage.get_class(outer->left);
        const bc::MethodNode* outer_method = outer->right;
        if (outernode == nullptr) continue;
        bool has_inner_class_info_inner = false;
        for (const bc::InnerClassNode& icn : inner_node->inner_classes) {
            if (icn.name == inner) {
                has_inner_class_info_inner = true;
                break;
            }
        }
        bool has_inner_class_info_outer = false;
        for (const bc::InnerClassNode& icn : outernode->inner_classes) {
            if (icn.name == inner) {
                has_inner_class_info_outer = true;
                break;
            }
        }
        if (has_inner_class_info_inner && has_inner_class_info_outer) continue;
        const bc::InnerClassNode new_inner_class_node{inner, std::nullopt, std::nullopt, 16400};
        if (!has_inner_class_info_inner) {
            inner_node->outer_method = outer_method->name;
            inner_node->outer_method_desc = outer_method->desc;
            inner_node->outer_class = outernode->name;
            inner_node->inner_classes.push_back(new_inner_class_node);
        }
        if (!has_inner_class_info_outer) outernode->inner_classes.push_back(new_inner_class_node);
        added_inners++;
    }
    return added_inners;
}

void Transform::fix_access(ClassStorage& storage) {
    std::unordered_map<std::string, Level> class_accesses;
    std::unordered_map<FieldReference, Pair<bc::FieldNode*, Level>, RefHash> field_accesses;
    std::unordered_map<MethodReference, Pair<bc::MethodNode*, Level>, RefHash> method_accesses;

    const auto super_of = [&](const bc::ClassNode* n) -> bc::ClassNode* { return n->super_name ? storage.get_class(*n->super_name) : nullptr; };

    // Find all invalid access points
    for (bc::ClassNode* node_ptr : storage) {
        bc::ClassNode& node = *node_ptr;
        for (const bc::FieldNode& field : node.fields) {
            // Checking field type
            const bc::Type type = bc::Type::get_type(field.desc);
            if (type.get_sort() != bc::Type::OBJECT) continue;
            const bc::ClassNode* class_node = storage.get_class(type.get_internal_name());
            if (class_node == nullptr) continue;
            const Level access = Access::get_from_bytecode(class_node->access);
            if (access == Level::DEFAULT) {
                if (!ClassStorage::in_one_package(node.name, class_node->name)) class_accesses[class_node->name] = Level::PUBLIC;
            }
        }
        for (bc::MethodNode& method : node.methods) {
            // Checking method argument and return types
            std::vector<bc::Type> types = bc::Type::get_argument_types(method.desc);
            types.push_back(bc::Type::get_return_type(method.desc));
            for (const bc::Type& type : types) {
                if (type.get_sort() != bc::Type::OBJECT) continue;
                if (class_accesses.contains(type.get_internal_name())) continue;
                const bc::ClassNode* class_node = storage.get_class(type.get_internal_name());
                if (class_node == nullptr) continue;
                const Level access = Access::get_from_bytecode(class_node->access);
                if (access == Level::DEFAULT) {
                    if (!ClassStorage::in_one_package(node.name, type.get_internal_name())) class_accesses[class_node->name] = Level::PUBLIC;
                }
            }
            for (bc::InsnNode* insn : method.instructions) {
                if (insn->kind == bc::InsnNode::METHOD_INSN) {
                    const auto* invoke = static_cast<const bc::MethodInsnNode*>(insn);
                    const MethodReference ref{invoke->owner, invoke->name, invoke->desc};
                    bc::MethodNode* method_node = storage.get_method(ref);
                    if (method_node == nullptr) continue;
                    if (!class_accesses.contains(invoke->owner)) {
                        const bc::ClassNode* class_node = storage.get_class(invoke->owner);
                        if (class_node != nullptr) {
                            const Level access = Access::get_from_bytecode(class_node->access);
                            if (access == Level::DEFAULT) {
                                if (!ClassStorage::in_one_package(node.name, invoke->owner)) class_accesses[class_node->name] = Level::PUBLIC;
                            }
                        }
                    }
                    if (method_accesses.contains(ref)) continue;
                    const Level access_level = Access::get_from_bytecode(method_node->access);
                    if (access_level == Level::PUBLIC) continue;
                    if (access_level == Level::PRIVATE) {
                        if (node.name != invoke->owner) method_accesses.insert_or_assign(ref, Pair<bc::MethodNode*, Level>::of(method_node, Level::PUBLIC));
                        continue;
                    }
                    // DEFAULT or PROTECTED
                    if (access_level == Level::PROTECTED) {
                        bool is_super = false;
                        for (const bc::ClassNode* cl_node = &node; cl_node != nullptr; cl_node = super_of(cl_node)) {
                            if (cl_node->name == invoke->owner) is_super = true;
                        }
                        if (is_super) continue;
                    }
                    if (!ClassStorage::in_one_package(node.name, invoke->owner)) {
                        method_accesses.insert_or_assign(ref, Pair<bc::MethodNode*, Level>::of(method_node, Level::PUBLIC));
                    }
                    continue;
                }

                if (insn->kind == bc::InsnNode::FIELD_INSN) {
                    const auto* field = static_cast<const bc::FieldInsnNode*>(insn);
                    const FieldReference ref{field->owner, field->name, field->desc};
                    bc::FieldNode* field_node = storage.get_field(ref);
                    if (field_node == nullptr) continue;
                    if (!class_accesses.contains(field->owner)) {
                        const bc::ClassNode* class_node = storage.get_class(field->owner);
                        if (class_node != nullptr) {
                            const Level access = Access::get_from_bytecode(class_node->access);
                            if (access == Level::DEFAULT) {
                                if (!ClassStorage::in_one_package(node.name, field->owner)) class_accesses[class_node->name] = Level::PUBLIC;
                            }
                        }
                    }
                    if (field_accesses.contains(ref)) continue;
                    const Level access_level = Access::get_from_bytecode(field_node->access);
                    if (access_level == Level::PUBLIC) continue;
                    if (access_level == Level::PRIVATE) {
                        if (node.name != field->owner) field_accesses.insert_or_assign(ref, Pair<bc::FieldNode*, Level>::of(field_node, Level::PUBLIC));
                        continue;
                    }
                    if (access_level == Level::PROTECTED) {
                        bool is_super = false;
                        for (const bc::ClassNode* cl_node = &node; cl_node != nullptr; cl_node = super_of(cl_node)) {
                            if (cl_node->name == field->owner) is_super = true;
                        }
                        if (is_super) continue;
                    }
                    if (!ClassStorage::in_one_package(node.name, field->owner)) {
                        field_accesses.insert_or_assign(ref, Pair<bc::FieldNode*, Level>::of(field_node, Level::PUBLIC));
                    }
                    continue;
                }
            }
        }
    }

    // Apply all access modifiers
    for (const auto& [name, level] : class_accesses) {
        bc::ClassNode* node = storage.get_class(name);
        if (node == nullptr) continue;
        node->access = Access::set_access(level, node->access);
    }
    for (const auto& [ref, pair] : field_accesses) pair.left->access = Access::set_access(pair.right, pair.left->access);
    for (const auto& [ref, pair] : method_accesses) pair.left->access = Access::set_access(pair.right, pair.left->access);
}

void Transform::decompose_vars(ClassStorage& storage) {
    for (bc::ClassNode* node : storage) {
        for (bc::MethodNode& method : node->methods) {
            bc::InsnNode* insn = method.instructions.get_first();
            while (insn != nullptr) {
                const int opcode = insn->get_opcode();
                if (opcode == op::DUP || opcode == op::DUP2) {
                    bc::InsnNode* insn2 = insn->get_next();
                    if (insn2 != nullptr && is_var_store(insn2->get_opcode()) &&
                        (((insn2->get_opcode() == op::DSTORE || insn2->get_opcode() == op::LSTORE) && opcode == op::DUP2) || opcode == op::DUP)) {
                        const auto* astore = static_cast<const bc::VarInsnNode*>(insn2);
                        method.instructions.insert(insn2, method.instructions.make<bc::VarInsnNode>(reverse_var_opcode(astore->get_opcode()), astore->var));
                        method.instructions.remove(insn);
                        insn = insn2;
                    }
                }
                insn = insn->get_next();
            }
        }
    }
}

void Transform::remove_var_self_assignment(ClassStorage& storage) {
    for (bc::ClassNode* node : storage) {
        for (bc::MethodNode& method : node->methods) {
            bc::InsnNode* insn = method.instructions.get_first();
            while (insn != nullptr) {
                if (is_var_load(insn->get_opcode())) {
                    bc::InsnNode* insn2 = insn->get_next();
                    if (insn2 != nullptr && is_var_store(insn2->get_opcode())) {
                        const auto* aload = static_cast<const bc::VarInsnNode*>(insn);
                        const auto* astore = static_cast<const bc::VarInsnNode*>(insn2);
                        bc::InsnNode* next = insn2->get_next();
                        if (aload->var == astore->var) {
                            method.instructions.remove(insn);
                            method.instructions.remove(insn2);
                            insn = next;
                            continue;
                        }
                    }
                }
                insn = insn->get_next();
            }
        }
    }
}

void Transform::set_major_version(ClassStorage& storage, int version) {
    for (bc::ClassNode* node : storage) node->version = version;
}

void Transform::restore_source_file(ClassStorage& storage) {
    for (bc::ClassNode* node : storage) {
        const std::size_t slash_index = node->name.rfind('/');
        node->source_file = (slash_index == std::string::npos ? node->name : node->name.substr(slash_index + 1)) + ".java";
    }
}

void Transform::strip_lvt(ClassStorage& storage) {
    for (bc::ClassNode* node : storage) {
        for (bc::MethodNode& method : node->methods) method.local_variables.clear();
    }
}

void Transform::strip_signatures(ClassStorage& storage) {
    for (bc::ClassNode* node : storage) {
        node->signature.reset();
        for (bc::MethodNode& method : node->methods) method.signature.reset();
        for (bc::FieldNode& field : node->fields) field.signature.reset();
    }
}

void Transform::merge(ClassStorage& storage, ClassStorage& storage2) {
    std::vector<bc::ClassNode*> nodes2;
    for (bc::ClassNode* n : storage2) nodes2.push_back(n);
    for (bc::ClassNode* node2 : nodes2) {
        bc::ClassNode* node1 = storage.get_class(node2->name);
        if (node1 == nullptr) {
            storage.add_class(storage2.release_class(node2));
            continue;
        }
        for (std::size_t i = 0; i < node2->interfaces.size(); ++i) {
            const std::string itf = node2->interfaces[i];
            const auto index_of = [&](const std::string& s) -> int {
                const auto it = std::find(node1->interfaces.begin(), node1->interfaces.end(), s);
                return it == node1->interfaces.end() ? -1 : static_cast<int>(it - node1->interfaces.begin());
            };
            if (index_of(itf) == -1) {
                int index = -1;
                int i2 = static_cast<int>(i) - 1;
                while (i2 >= 0 && index == -1) {
                    index = index_of(node2->interfaces[i2]);
                    i2--;
                }
                if (index == -1) {
                    node1->interfaces.push_back(itf);
                } else {
                    node1->interfaces.insert(node1->interfaces.begin() + index + 1, itf);
                }
            }
        }
        // (the references of node2's members are taken first: the members move out of it as they are added)
        std::vector<FieldReference> fields2;
        for (const bc::FieldNode& f : node2->fields) fields2.push_back(FieldReference{node2->name, f.name, f.desc});
        std::vector<MethodReference> methods2;
        for (const bc::MethodNode& m : node2->methods) methods2.push_back(MethodReference{node2->name, m.name, m.desc});

        std::vector<FieldReference> fields;
        for (const bc::FieldNode& f : node1->fields) fields.push_back(FieldReference{node1->name, f.name, f.desc});
        for (std::size_t i = 0; i < node2->fields.size(); ++i) {
            const FieldReference& field_ref = fields2[i];
            if (std::find(fields.begin(), fields.end(), field_ref) == fields.end()) {
                int i2 = static_cast<int>(i) - 1;
                int index = -1;
                std::optional<FieldReference> prev_field;
                while (i2 >= 0 && index == -1) {
                    if (prev_field) {
                        const auto it = std::find(fields.begin(), fields.end(), *prev_field);
                        index = it == fields.end() ? -1 : static_cast<int>(it - fields.begin());
                    }
                    prev_field = fields2[i2];
                    i2--;
                }
                if (index == -1) {
                    node1->fields.push_back(std::move(node2->fields[i]));
                    fields.push_back(field_ref);
                } else {
                    node1->fields.insert(node1->fields.begin() + index + 1, std::move(node2->fields[i]));
                    fields.insert(fields.begin() + index + 1, field_ref);
                }
            }
        }
        std::vector<MethodReference> methods;
        for (const bc::MethodNode& m : node1->methods) methods.push_back(MethodReference{node1->name, m.name, m.desc});
        for (std::size_t i = 0; i < node2->methods.size(); ++i) {
            const MethodReference& method_ref = methods2[i];
            if (std::find(methods.begin(), methods.end(), method_ref) == methods.end()) {
                int i2 = static_cast<int>(i) - 1;
                int index = -1;
                std::optional<MethodReference> prev_method;
                while (i2 >= 0 && index == -1) {
                    if (prev_method) {
                        const auto it = std::find(methods.begin(), methods.end(), *prev_method);
                        index = it == methods.end() ? -1 : static_cast<int>(it - methods.begin());
                    }
                    prev_method = methods2[i2];
                    i2--;
                }
                if (index == -1) {
                    node1->methods.push_back(std::move(node2->methods[i]));
                    methods.push_back(method_ref);
                } else {
                    node1->methods.insert(node1->methods.begin() + index + 1, std::move(node2->methods[i]));
                    methods.insert(methods.begin() + index + 1, method_ref);
                }
            }
        }
    }
}

// ---- the visitors

bc::MethodNode ClassInitAdder::create_method(const std::string& super_class, const std::string& name, const std::string& desc) {
    bc::MethodNode mn;
    mn.access = op::ACC_PRIVATE;
    mn.name = name;
    mn.desc = desc;
    bc::InsnList& insns = mn.instructions;
    int max_locals = 1;
    insns.add(insns.make<bc::VarInsnNode>(op::ALOAD, 0));
    const std::vector<bc::Type> types = bc::Type::get_argument_types(desc);
    int var_index = 1;
    for (std::size_t i = 0; i < types.size(); ++i) {
        insns.add(insns.make<bc::VarInsnNode>(types[i].get_opcode(op::ILOAD), var_index));
        max_locals++;
        var_index += types[i].get_size();
    }
    insns.add(insns.make<bc::MethodInsnNode>(op::INVOKESPECIAL, super_class, name, desc, false));
    insns.add(insns.make<bc::ZeroInsnNode>(bc::Type::get_return_type(desc).get_opcode(op::IRETURN)));
    mn.max_stack = var_index;   // visitMaxs(varIndex, maxLocals)
    mn.max_locals = max_locals;
    return mn;
}

void ClassInitAdder::visit_class(bc::ClassNode& node) {
    std::vector<std::string> constructors;
    for (const bc::MethodNode& method : node.methods) {
        if (method.name == "<init>") constructors.push_back(method.desc);
    }
    if (!node.super_name) return;
    const bc::ClassNode* super_class = storage_.get_class(*node.super_name);
    if (super_class == nullptr) return;
    if (!constructors.empty()) return;
    for (const bc::MethodNode& method : super_class->methods) {
        // Adding implicit constructor
        if (method.name == "<init>") {
            node.methods.insert(node.methods.begin(), create_method(*node.super_name, method.name, method.desc));
            break;
        }
    }
}

void AddExceptions::visit_method(bc::MethodNode& node) {
    if (node.name != "<clinit>") node.exceptions = process_exceptions(MethodReference{class_node_->name, node.name, node.desc}, node.exceptions);
}

std::vector<std::string> AddExceptions::process_exceptions(const MethodReference& ref, const std::vector<std::string>& exceptions) {
    const std::vector<std::string> known = context_.get_exceptions(ref);
    std::set<std::string> set(known.begin(), known.end());
    set.insert(exceptions.begin(), exceptions.end());
    if (set.size() > exceptions.size()) {
        std::vector<std::string> sorted(set.begin(), set.end());   // (a set of strings iterates sorted: Collections.sort of the original)
        context_.set_exceptions(ref, sorted);
        return sorted;
    }
    return exceptions;
}

void AccessFixer::visit_class(bc::ClassNode& node) {
    class_node_ = &node;
    const Level old = Access::get_from_bytecode(node.access);
    const std::optional<Level> new_level = context_.get_level(node.name);
    if (new_level && old != *new_level) node.access = Access::set_access(*new_level, node.access);
}

void AccessFixer::visit_field(bc::FieldNode& node) {
    const Level old = Access::get_from_bytecode(node.access);
    const std::optional<Level> new_level = context_.get_field_level(FieldReference{class_node_->name, node.name, node.desc});
    if (new_level && old != *new_level) node.access = Access::set_access(*new_level, node.access);
}

void AccessFixer::visit_method(bc::MethodNode& node) {
    const Level old = Access::get_from_bytecode(node.access);
    const std::optional<Level> new_level = context_.get_method_level(MethodReference{class_node_->name, node.name, node.desc});
    if (new_level && old != *new_level) node.access = Access::set_access(*new_level, node.access);
}

}  // namespace cb::java::minecraft::prep
