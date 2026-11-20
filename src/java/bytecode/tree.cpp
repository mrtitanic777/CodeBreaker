// java/bytecode/tree.cpp -- see tree.hpp. The reading follows ASM's ClassReader (into the tree, frames skipped), the writing its ClassWriter with
// COMPUTE_MAXS (ASM is BSD-licensed, (c) INRIA, France Telecom; this is CodeBreaker's own code to the same model).
#include "java/bytecode/tree.hpp"

#include <algorithm>
#include <bit>
#include <format>
#include <map>
#include <set>
#include <stdexcept>
#include <unordered_map>

#include "core/error.hpp"
#include "core/io.hpp"
#include "java/classfile.hpp"

namespace cb::java::bytecode {

namespace {

constexpr std::string_view kBtReader = "class";
[[noreturn]] void bt_bad(const std::string& what) { throw FormatError(std::string(kBtReader), 0, what); }

}  // namespace

// ---- Type

const Type Type::VOID_TYPE{VOID, "V"}, Type::BOOLEAN_TYPE{BOOLEAN, "Z"}, Type::CHAR_TYPE{CHAR, "C"}, Type::BYTE_TYPE{BYTE, "B"},
    Type::SHORT_TYPE{SHORT, "S"}, Type::INT_TYPE{INT, "I"}, Type::FLOAT_TYPE{FLOAT, "F"}, Type::LONG_TYPE{LONG, "J"}, Type::DOUBLE_TYPE{DOUBLE, "D"};

namespace {

// The end of the field descriptor that starts at `at` (one past it); std::invalid_argument for a damaged one.
std::size_t bt_descriptor_end(const std::string& d, std::size_t at) {
    std::size_t i = at;
    while (i < d.size() && d[i] == '[') ++i;
    if (i >= d.size()) throw std::invalid_argument("a damaged descriptor: " + d);
    if (d[i] == 'L') {
        const std::size_t semi = d.find(';', i);
        if (semi == std::string::npos) throw std::invalid_argument("a damaged descriptor: " + d);
        return semi + 1;
    }
    return i + 1;
}

}  // namespace

Type Type::get_type(const std::string& d) {
    if (d.empty()) throw std::invalid_argument("an empty descriptor");
    switch (d[0]) {
        case 'V': return VOID_TYPE;
        case 'Z': return BOOLEAN_TYPE;
        case 'C': return CHAR_TYPE;
        case 'B': return BYTE_TYPE;
        case 'S': return SHORT_TYPE;
        case 'I': return INT_TYPE;
        case 'F': return FLOAT_TYPE;
        case 'J': return LONG_TYPE;
        case 'D': return DOUBLE_TYPE;
        case '[': return Type(ARRAY, d);
        case 'L': return Type(OBJECT, d);
        case '(': return Type(METHOD, d);
        default: throw std::invalid_argument("a damaged descriptor: " + d);
    }
}

Type Type::get_object_type(const std::string& internal) {
    if (!internal.empty() && internal[0] == '[') return Type(ARRAY, internal);
    return Type(OBJECT, "L" + internal + ";");
}

Type Type::get_method_type(const std::string& descriptor) { return Type(METHOD, descriptor); }

Type Type::get_method_type(const Type& return_type, const std::vector<Type>& argument_types) {
    std::string d = "(";
    for (const Type& t : argument_types) d += t.get_descriptor();
    d += ")" + return_type.get_descriptor();
    return Type(METHOD, d);
}

std::vector<Type> Type::get_argument_types(const std::string& md) {
    std::vector<Type> out;
    std::size_t i = 1;
    while (i < md.size() && md[i] != ')') {
        const std::size_t end = bt_descriptor_end(md, i);
        out.push_back(get_type(md.substr(i, end - i)));
        i = end;
    }
    return out;
}

Type Type::get_return_type(const std::string& md) {
    const std::size_t close = md.find(')');
    if (close == std::string::npos) throw std::invalid_argument("a damaged method descriptor: " + md);
    return get_type(md.substr(close + 1));
}

int Type::get_arguments_and_return_sizes(const std::string& md) {
    int arguments = 1;
    for (const Type& t : get_argument_types(md)) arguments += t.get_size();
    const Type r = get_return_type(md);
    return arguments << 2 | r.get_size();
}

int Type::get_dimensions() const {
    int n = 0;
    while (n < static_cast<int>(descriptor_.size()) && descriptor_[static_cast<std::size_t>(n)] == '[') ++n;
    return n;
}

Type Type::get_element_type() const { return get_type(descriptor_.substr(static_cast<std::size_t>(get_dimensions()))); }

std::string Type::get_class_name() const {
    switch (sort_) {
        case VOID: return "void";
        case BOOLEAN: return "boolean";
        case CHAR: return "char";
        case BYTE: return "byte";
        case SHORT: return "short";
        case INT: return "int";
        case FLOAT: return "float";
        case LONG: return "long";
        case DOUBLE: return "double";
        case ARRAY: {
            std::string s = get_element_type().get_class_name();
            for (int i = get_dimensions(); i > 0; --i) s += "[]";
            return s;
        }
        case OBJECT: {
            std::string s = get_internal_name();
            std::replace(s.begin(), s.end(), '/', '.');
            return s;
        }
        default: return descriptor_;
    }
}

std::string Type::get_internal_name() const {
    if (sort_ == OBJECT) return descriptor_.substr(1, descriptor_.size() - 2);
    return descriptor_;
}

std::string Type::get_descriptor() const { return descriptor_; }

int Type::get_opcode(int opcode) const {
    if (opcode == op::IALOAD || opcode == op::IASTORE) {
        switch (sort_) {
            case BOOLEAN:
            case BYTE: return opcode + (op::BALOAD - op::IALOAD);
            case CHAR: return opcode + (op::CALOAD - op::IALOAD);
            case SHORT: return opcode + (op::SALOAD - op::IALOAD);
            case INT: return opcode;
            case FLOAT: return opcode + (op::FALOAD - op::IALOAD);
            case LONG: return opcode + (op::LALOAD - op::IALOAD);
            case DOUBLE: return opcode + (op::DALOAD - op::IALOAD);
            case ARRAY:
            case OBJECT: return opcode + (op::AALOAD - op::IALOAD);
            default: throw std::invalid_argument("no array opcode for " + descriptor_);
        }
    }
    switch (sort_) {
        case VOID:
            if (opcode != op::IRETURN) throw std::invalid_argument("no opcode for void");
            return op::RETURN;
        case BOOLEAN:
        case BYTE:
        case CHAR:
        case SHORT:
        case INT: return opcode;
        case FLOAT: return opcode + (op::FRETURN - op::IRETURN);
        case LONG: return opcode + (op::LRETURN - op::IRETURN);
        case DOUBLE: return opcode + (op::DRETURN - op::IRETURN);
        case ARRAY:
        case OBJECT:
            if (opcode != op::ILOAD && opcode != op::ISTORE && opcode != op::IRETURN) throw std::invalid_argument("no such opcode for a reference");
            return opcode + (op::ARETURN - op::IRETURN);
        default: throw std::invalid_argument("no opcode for a method type");
    }
}

// ---- InsnList

InsnNode* InsnList::get(int index) const {
    InsnNode* n = first_;
    for (int i = 0; n != nullptr && i < index; ++i) n = n->next;
    if (n == nullptr) throw std::out_of_range("instruction index");
    return n;
}

int InsnList::index_of(const InsnNode* node) const {
    int i = 0;
    for (InsnNode* n = first_; n != nullptr; n = n->next, ++i) {
        if (n == node) return i;
    }
    return -1;
}

bool InsnList::contains(const InsnNode* node) const { return index_of(node) >= 0; }

void InsnList::add(InsnNode* node) {
    node->previous = last_;
    node->next = nullptr;
    if (last_ != nullptr) last_->next = node;
    else first_ = node;
    last_ = node;
    ++size_;
}

void InsnList::add(InsnList& list) {
    if (&list == this || list.size_ == 0) return;
    for (auto& p : list.owned_) owned_.push_back(std::move(p));
    list.owned_.clear();
    if (last_ == nullptr) {
        first_ = list.first_;
    } else {
        last_->next = list.first_;
        list.first_->previous = last_;
    }
    last_ = list.last_;
    size_ += list.size_;
    list.first_ = list.last_ = nullptr;
    list.size_ = 0;
}

void InsnList::insert(InsnNode* node) {
    node->previous = nullptr;
    node->next = first_;
    if (first_ != nullptr) first_->previous = node;
    else last_ = node;
    first_ = node;
    ++size_;
}

void InsnList::insert(InsnList& list) {
    if (&list == this || list.size_ == 0) return;
    for (auto& p : list.owned_) owned_.push_back(std::move(p));
    list.owned_.clear();
    if (first_ == nullptr) {
        last_ = list.last_;
    } else {
        first_->previous = list.last_;
        list.last_->next = first_;
    }
    first_ = list.first_;
    size_ += list.size_;
    list.first_ = list.last_ = nullptr;
    list.size_ = 0;
}

void InsnList::insert(InsnNode* previous, InsnNode* node) {
    InsnNode* following = previous->next;
    node->previous = previous;
    node->next = following;
    previous->next = node;
    if (following != nullptr) following->previous = node;
    else last_ = node;
    ++size_;
}

void InsnList::insert(InsnNode* previous, InsnList& list) {
    if (&list == this || list.size_ == 0) return;
    for (auto& p : list.owned_) owned_.push_back(std::move(p));
    list.owned_.clear();
    InsnNode* following = previous->next;
    previous->next = list.first_;
    list.first_->previous = previous;
    list.last_->next = following;
    if (following != nullptr) following->previous = list.last_;
    else last_ = list.last_;
    size_ += list.size_;
    list.first_ = list.last_ = nullptr;
    list.size_ = 0;
}

void InsnList::insert_before(InsnNode* next, InsnNode* node) {
    InsnNode* before = next->previous;
    node->previous = before;
    node->next = next;
    next->previous = node;
    if (before != nullptr) before->next = node;
    else first_ = node;
    ++size_;
}

void InsnList::insert_before(InsnNode* next, InsnList& list) {
    if (&list == this || list.size_ == 0) return;
    for (auto& p : list.owned_) owned_.push_back(std::move(p));
    list.owned_.clear();
    InsnNode* before = next->previous;
    list.first_->previous = before;
    list.last_->next = next;
    next->previous = list.last_;
    if (before != nullptr) before->next = list.first_;
    else first_ = list.first_;
    size_ += list.size_;
    list.first_ = list.last_ = nullptr;
    list.size_ = 0;
}

void InsnList::set(InsnNode* old_node, InsnNode* new_node) {
    new_node->previous = old_node->previous;
    new_node->next = old_node->next;
    if (old_node->previous != nullptr) old_node->previous->next = new_node;
    else first_ = new_node;
    if (old_node->next != nullptr) old_node->next->previous = new_node;
    else last_ = new_node;
    old_node->previous = old_node->next = nullptr;
}

void InsnList::remove(InsnNode* node) {
    if (node->previous != nullptr) node->previous->next = node->next;
    else first_ = node->next;
    if (node->next != nullptr) node->next->previous = node->previous;
    else last_ = node->previous;
    node->previous = node->next = nullptr;
    --size_;
}

void InsnList::clear() {
    for (InsnNode* n = first_; n != nullptr;) {
        InsnNode* following = n->next;
        n->previous = n->next = nullptr;
        n = following;
    }
    first_ = last_ = nullptr;
    size_ = 0;
}

// ---- reading

namespace {

using classfile::Tag;

class Reader {
public:
    explicit Reader(const classfile::ClassFile& cf) : cf_(cf) {}

    std::string utf8(std::uint16_t i) const { return classfile::text(cf_.utf8(i)); }
    std::optional<std::string> utf8_or_none(std::uint16_t i) const { return i == 0 ? std::nullopt : std::optional<std::string>(utf8(i)); }
    std::string class_name(std::uint16_t i) const { return classfile::text(cf_.class_name(i)); }
    std::optional<std::string> class_or_none(std::uint16_t i) const { return i == 0 ? std::nullopt : std::optional<std::string>(class_name(i)); }
    const classfile::Constant& at(std::uint16_t i) const {
        if (i == 0 || i >= cf_.pool.size()) bt_bad(std::format("the constant {} is out of the pool", i));
        return cf_.pool[i];
    }
    std::string attr_name(const classfile::Attribute& a) const { return classfile::text(cf_.utf8(a.name)); }

    void read_bootstrap_methods() {
        if (const classfile::Attribute* bm = cf_.find(cf_.attributes, "BootstrapMethods")) {
            io::ByteReader r(bm->data, "class.BootstrapMethods");
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) {
                Bootstrap b;
                b.method = r.u16_be();
                const std::uint16_t args = r.u16_be();
                for (std::uint16_t k = 0; k < args; ++k) b.arguments.push_back(r.u16_be());
                bootstrap_.push_back(std::move(b));
            }
        }
    }

    // [owner, name, desc] of a ref constant (Fieldref, Methodref, InterfaceMethodref)
    std::tuple<std::string, std::string, std::string> member_ref(std::uint16_t i) const {
        const classfile::Constant& c = at(i);
        const classfile::Constant& nat = at(c.b);
        return {class_name(c.a), utf8(nat.a), utf8(nat.b)};
    }

    Handle handle(std::uint16_t i) const {
        const classfile::Constant& c = at(i);
        if (c.tag != Tag::MethodHandle) bt_bad("not a method handle");
        const auto [owner, name, desc] = member_ref(c.a);
        return Handle{c.kind, owner, name, desc, at(c.a).tag == Tag::InterfaceMethodref};
    }

    Constant constant(std::uint16_t i) const {
        const classfile::Constant& c = at(i);
        switch (c.tag) {
            case Tag::Integer: return static_cast<std::int32_t>(static_cast<std::uint32_t>(c.value));
            case Tag::Float: return std::bit_cast<float>(static_cast<std::uint32_t>(c.value));
            case Tag::Long: return static_cast<std::int64_t>(c.value);
            case Tag::Double: return std::bit_cast<double>(c.value);
            case Tag::String: return utf8(c.a);
            case Tag::Class: return Type::get_object_type(utf8(c.a));
            case Tag::MethodType: return Type::get_method_type(utf8(c.a));
            case Tag::MethodHandle: return handle(i);
            case Tag::Dynamic: {
                auto d = std::make_shared<ConstantDynamic>();
                const classfile::Constant& nat = at(c.b);
                d->name = utf8(nat.a);
                d->descriptor = utf8(nat.b);
                if (c.a >= bootstrap_.size()) bt_bad("a dynamic constant's bootstrap method is not there");
                d->bootstrap_method = handle(bootstrap_[c.a].method);
                for (std::uint16_t a : bootstrap_[c.a].arguments) d->bootstrap_method_arguments.push_back(constant(a));
                return d;
            }
            default: bt_bad(std::format("the constant {} cannot be loaded", i));
        }
    }

    std::pair<Handle, std::vector<Constant>> bootstrap(std::uint16_t index) const {
        if (index >= bootstrap_.size()) bt_bad("an invokedynamic's bootstrap method is not there");
        std::vector<Constant> args;
        for (std::uint16_t a : bootstrap_[index].arguments) args.push_back(constant(a));
        return {handle(bootstrap_[index].method), std::move(args)};
    }

    // ---- annotations
    AnnotationValue element_value(io::ByteReader& r) const {
        AnnotationValue v;
        const char tag = static_cast<char>(r.u8());
        switch (tag) {
            case 'B': v.kind = AnnotationValue::BYTE; v.integral = static_cast<std::int8_t>(int_of(r.u16_be())); break;
            case 'C': v.kind = AnnotationValue::CHAR; v.integral = static_cast<std::uint16_t>(int_of(r.u16_be())); break;
            case 'S': v.kind = AnnotationValue::SHORT; v.integral = static_cast<std::int16_t>(int_of(r.u16_be())); break;
            case 'Z': v.kind = AnnotationValue::BOOLEAN; v.integral = int_of(r.u16_be()) != 0 ? 1 : 0; break;
            case 'I': v.kind = AnnotationValue::INT; v.integral = int_of(r.u16_be()); break;
            case 'J': v.kind = AnnotationValue::LONG; v.integral = static_cast<std::int64_t>(at(r.u16_be()).value); break;
            case 'F': v.kind = AnnotationValue::FLOAT; v.floating = std::bit_cast<float>(static_cast<std::uint32_t>(at(r.u16_be()).value)); break;
            case 'D': v.kind = AnnotationValue::DOUBLE; v.floating = std::bit_cast<double>(at(r.u16_be()).value); break;
            case 's': v.kind = AnnotationValue::STRING; v.text = utf8(r.u16_be()); break;
            case 'e': {
                v.kind = AnnotationValue::ENUM;
                v.enum_value.desc = utf8(r.u16_be());
                v.enum_value.value = utf8(r.u16_be());
                break;
            }
            case 'c': v.kind = AnnotationValue::CLASS; v.type = Type::get_type(utf8(r.u16_be())); break;
            case '@': v.kind = AnnotationValue::ANNOTATION; v.annotation = std::make_shared<AnnotationNode>(annotation(r)); break;
            case '[': {
                v.kind = AnnotationValue::ARRAY;
                const std::uint16_t n = r.u16_be();
                for (std::uint16_t i = 0; i < n; ++i) v.values.push_back(element_value(r));
                break;
            }
            default: bt_bad(std::format("an annotation element of the unknown tag '{}'", tag));
        }
        return v;
    }
    AnnotationNode annotation(io::ByteReader& r) const {
        AnnotationNode a;
        a.desc = utf8(r.u16_be());
        const std::uint16_t n = r.u16_be();
        for (std::uint16_t i = 0; i < n; ++i) {
            std::string name = utf8(r.u16_be());
            a.values.emplace_back(std::move(name), element_value(r));
        }
        return a;
    }
    std::vector<AnnotationNode> annotations(const classfile::Attribute& attr) const {
        io::ByteReader r(attr.data, "class.annotations");
        std::vector<AnnotationNode> out;
        const std::uint16_t n = r.u16_be();
        for (std::uint16_t i = 0; i < n; ++i) out.push_back(annotation(r));
        return out;
    }
    std::vector<std::vector<AnnotationNode>> parameter_annotations(const classfile::Attribute& attr, int& count) const {
        io::ByteReader r(attr.data, "class.parameter_annotations");
        count = r.u8();
        std::vector<std::vector<AnnotationNode>> out;
        for (int p = 0; p < count; ++p) {
            std::vector<AnnotationNode> list;
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) list.push_back(annotation(r));
            out.push_back(std::move(list));
        }
        return out;
    }
    // (the target_info's size by target_type: JVMS 4.7.20.1)
    static std::size_t target_info_size(io::ByteReader& r, std::uint8_t target) {
        switch (target) {
            case 0x00: case 0x01: return 1;                       // type parameter
            case 0x10: return 2;                                  // supertype
            case 0x11: case 0x12: return 2;                       // type parameter bound
            case 0x13: case 0x14: case 0x15: return 0;            // empty
            case 0x16: return 1;                                  // formal parameter
            case 0x17: return 2;                                  // throws
            default: bt_bad(std::format("a type annotation of code (target {:#x}) outside code", target));
        }
        (void)r;
    }
    std::vector<TypeAnnotationNode> type_annotations(const classfile::Attribute& attr) const {
        io::ByteReader r(attr.data, "class.type_annotations");
        std::vector<TypeAnnotationNode> out;
        const std::uint16_t n = r.u16_be();
        for (std::uint16_t i = 0; i < n; ++i) {
            TypeAnnotationNode t;
            const std::size_t from = r.pos();
            t.type_ref = r.u8();
            (void)r.bytes(target_info_size(r, static_cast<std::uint8_t>(t.type_ref)));
            const std::uint8_t path_length = r.u8();
            (void)r.bytes(std::size_t{path_length} * 2);
            const ByteView all(attr.data.data() + from + 1, r.pos() - from - 1);
            t.type_path.assign(all.begin(), all.end());
            static_cast<AnnotationNode&>(t) = annotation(r);
            out.push_back(std::move(t));
        }
        return out;
    }

    const classfile::ClassFile& cf() const { return cf_; }

private:
    std::int32_t int_of(std::uint16_t i) const { return static_cast<std::int32_t>(static_cast<std::uint32_t>(at(i).value)); }

    struct Bootstrap {
        std::uint16_t method = 0;
        std::vector<std::uint16_t> arguments;
    };
    const classfile::ClassFile& cf_;
    std::vector<Bootstrap> bootstrap_;
};

// The access flags an attribute adds (ASM folds Deprecated and Synthetic into them, Record too).
int bt_flag_of(const std::string& name) {
    if (name == "Deprecated") return op::ACC_DEPRECATED;
    if (name == "Synthetic") return op::ACC_SYNTHETIC;
    return 0;
}

void bt_read_code(const Reader& rd, const classfile::Attribute& attr, MethodNode& m) {
    const classfile::Code code = classfile::decode_code(attr.data);
    m.max_stack = code.max_stack;
    m.max_locals = code.max_locals;
    const auto length = static_cast<std::uint32_t>(code.bytes.size());
    std::map<std::uint32_t, LabelNode*> labels;
    InsnList& list = m.instructions;
    const auto label = [&](std::int64_t offset) -> LabelNode* {
        if (offset < 0 || offset > length) bt_bad(std::format("a code offset {} past the code", offset));
        auto& l = labels[static_cast<std::uint32_t>(offset)];
        if (l == nullptr) l = list.make<LabelNode>();
        return l;
    };
    // the labels every offset that is referred to needs, made first so their order is the code's
    for (const classfile::Instruction& in : code.instructions) {
        const int o = in.opcode;
        if ((o >= op::IFEQ && o <= op::JSR) || o == op::IFNULL || o == op::IFNONNULL || o == 200 || o == 201) label(in.index);
        if (o == op::TABLESWITCH || o == op::LOOKUPSWITCH) {
            label(in.default_target);
            for (std::int32_t t : in.targets) label(t);
        }
    }
    for (const classfile::ExceptionEntry& e : code.exceptions) {
        label(e.start);
        label(e.end);
        label(e.handler);
    }
    std::multimap<std::uint32_t, int> lines;   // offset -> line, in the table's order
    struct Lv {
        std::uint16_t start, length, name, desc, index;
    };
    std::vector<Lv> lvt, lvtt;
    for (const classfile::Attribute& a : code.attributes) {
        const std::string name = rd.attr_name(a);
        io::ByteReader r(a.data, "class.Code.attribute");
        if (name == "LineNumberTable") {
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) {
                const std::uint16_t pc = r.u16_be();
                const std::uint16_t line = r.u16_be();
                label(pc);
                lines.emplace(pc, line);
            }
        } else if (name == "LocalVariableTable" || name == "LocalVariableTypeTable") {
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) {
                Lv v{r.u16_be(), r.u16_be(), r.u16_be(), r.u16_be(), r.u16_be()};
                label(v.start);
                label(std::int64_t{v.start} + v.length);
                (name == "LocalVariableTable" ? lvt : lvtt).push_back(v);
            }
        }
    }
    const auto place = [&](std::uint32_t offset) {
        if (const auto it = labels.find(offset); it != labels.end()) {
            list.add(it->second);
            const auto [from, to] = lines.equal_range(offset);
            for (auto l = from; l != to; ++l) list.add(list.make<LineNumberNode>(l->second, it->second));
        }
    };
    const classfile::ClassFile& cf = rd.cf();
    for (const classfile::Instruction& in : code.instructions) {
        place(in.offset);
        const int o = in.opcode;
        InsnNode* node = nullptr;
        if (o >= 26 && o <= 45) node = list.make<VarInsnNode>(op::ILOAD + (o - 26) / 4, (o - 26) % 4);
        else if (o >= 59 && o <= 78) node = list.make<VarInsnNode>(op::ISTORE + (o - 59) / 4, (o - 59) % 4);
        else if ((o >= op::ILOAD && o <= op::ALOAD) || (o >= op::ISTORE && o <= op::ASTORE) || o == op::RET) node = list.make<VarInsnNode>(o, in.index);
        else if (o == op::BIPUSH || o == op::SIPUSH || o == op::NEWARRAY) node = list.make<IntInsnNode>(o, in.index);
        else if (o == op::LDC || o == 19 || o == 20) node = list.make<LdcInsnNode>(rd.constant(static_cast<std::uint16_t>(in.index)));
        else if (o == op::IINC) node = list.make<IincInsnNode>(in.index, in.extra);
        else if ((o >= op::IFEQ && o <= op::JSR) || o == op::IFNULL || o == op::IFNONNULL) node = list.make<JumpInsnNode>(o, label(in.index));
        else if (o == 200) node = list.make<JumpInsnNode>(op::GOTO, label(in.index));
        else if (o == 201) node = list.make<JumpInsnNode>(op::JSR, label(in.index));
        else if (o == op::TABLESWITCH) {
            std::vector<LabelNode*> ls;
            for (std::int32_t t : in.targets) ls.push_back(label(t));
            node = list.make<TableSwitchInsnNode>(in.low, in.low + static_cast<int>(in.targets.size()) - 1, label(in.default_target), std::move(ls));
        } else if (o == op::LOOKUPSWITCH) {
            std::vector<LabelNode*> ls;
            for (std::int32_t t : in.targets) ls.push_back(label(t));
            node = list.make<LookupSwitchInsnNode>(label(in.default_target), std::vector<int>(in.keys.begin(), in.keys.end()), std::move(ls));
        } else if (o >= op::GETSTATIC && o <= op::PUTFIELD) {
            auto [owner, name, desc] = rd.member_ref(static_cast<std::uint16_t>(in.index));
            node = list.make<FieldInsnNode>(o, std::move(owner), std::move(name), std::move(desc));
        } else if (o >= op::INVOKEVIRTUAL && o <= op::INVOKEINTERFACE) {
            const bool itf = cf.pool.at(static_cast<std::size_t>(in.index)).tag == Tag::InterfaceMethodref;
            auto [owner, name, desc] = rd.member_ref(static_cast<std::uint16_t>(in.index));
            node = list.make<MethodInsnNode>(o, std::move(owner), std::move(name), std::move(desc), itf);
        } else if (o == op::INVOKEDYNAMIC) {
            const classfile::Constant& c = rd.at(static_cast<std::uint16_t>(in.index));
            const classfile::Constant& nat = rd.at(c.b);
            auto [bsm, args] = rd.bootstrap(c.a);
            node = list.make<InvokeDynamicInsnNode>(rd.utf8(nat.a), rd.utf8(nat.b), std::move(bsm), std::move(args));
        } else if (o == op::NEW || o == op::ANEWARRAY || o == op::CHECKCAST || o == op::INSTANCEOF) {
            node = list.make<TypeInsnNode>(o, rd.class_name(static_cast<std::uint16_t>(in.index)));
        } else if (o == op::MULTIANEWARRAY) {
            node = list.make<MultiANewArrayInsnNode>(rd.class_name(static_cast<std::uint16_t>(in.index)), in.extra);
        } else {
            node = list.make<ZeroInsnNode>(o);
        }
        list.add(node);
    }
    place(length);
    for (const classfile::ExceptionEntry& e : code.exceptions) {
        m.try_catch_blocks.push_back({label(e.start), label(e.end), label(e.handler), rd.class_or_none(e.catch_type)});
    }
    for (const Lv& v : lvt) {
        LocalVariableNode lv{rd.utf8(v.name), rd.utf8(v.desc), std::nullopt, label(v.start), label(std::int64_t{v.start} + v.length), v.index};
        for (const Lv& t : lvtt) {
            if (t.start == v.start && t.index == v.index) lv.signature = rd.utf8(t.desc);
        }
        m.local_variables.push_back(std::move(lv));
    }
}

}  // namespace

ClassNode read(ByteView data, std::string_view name) {
    const classfile::ClassFile cf = classfile::parse(data, name);
    Reader rd(cf);
    rd.read_bootstrap_methods();
    ClassNode c;
    c.version = cf.minor << 16 | cf.major;
    c.access = cf.access;
    c.name = rd.class_name(cf.this_class);
    c.super_name = rd.class_or_none(cf.super_class);
    for (std::uint16_t i : cf.interfaces) c.interfaces.push_back(rd.class_name(i));
    for (const classfile::Attribute& a : cf.attributes) {
        const std::string an = rd.attr_name(a);
        io::ByteReader r(a.data, "class.attribute");
        if (an == "SourceFile") c.source_file = rd.utf8(r.u16_be());
        else if (an == "SourceDebugExtension") c.source_debug = classfile::text(std::string(a.data.begin(), a.data.end()));
        else if (an == "Signature") c.signature = rd.utf8(r.u16_be());
        else if (an == "EnclosingMethod") {
            c.outer_class = rd.class_name(r.u16_be());
            if (const std::uint16_t nat = r.u16_be(); nat != 0) {
                c.outer_method = rd.utf8(rd.at(nat).a);
                c.outer_method_desc = rd.utf8(rd.at(nat).b);
            }
        } else if (an == "InnerClasses") {
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) {
                InnerClassNode ic;
                ic.name = rd.class_name(r.u16_be());
                ic.outer_name = rd.class_or_none(r.u16_be());
                ic.inner_name = rd.utf8_or_none(r.u16_be());
                ic.access = r.u16_be();
                c.inner_classes.push_back(std::move(ic));
            }
        } else if (an == "NestHost") c.nest_host_class = rd.class_name(r.u16_be());
        else if (an == "NestMembers" || an == "PermittedSubclasses") {
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) (an == "NestMembers" ? c.nest_members : c.permitted_subclasses).push_back(rd.class_name(r.u16_be()));
        } else if (an == "Record") {
            c.access |= op::ACC_RECORD;
            const std::uint16_t n = r.u16_be();
            for (std::uint16_t i = 0; i < n; ++i) {
                RecordComponentNode rc;
                rc.name = rd.utf8(r.u16_be());
                rc.descriptor = rd.utf8(r.u16_be());
                const std::uint16_t na = r.u16_be();
                for (std::uint16_t k = 0; k < na; ++k) {
                    classfile::Attribute ra;
                    ra.name = r.u16_be();
                    const ByteView d = r.bytes(r.u32_be());
                    ra.data.assign(d.begin(), d.end());
                    const std::string rn = rd.attr_name(ra);
                    if (rn == "Signature") rc.signature = rd.utf8(static_cast<std::uint16_t>(ra.data.at(0) << 8 | ra.data.at(1)));
                    else if (rn == "RuntimeVisibleAnnotations") rc.visible_annotations = rd.annotations(ra);
                    else if (rn == "RuntimeInvisibleAnnotations") rc.invisible_annotations = rd.annotations(ra);
                    else if (rn == "RuntimeVisibleTypeAnnotations") rc.visible_type_annotations = rd.type_annotations(ra);
                    else if (rn == "RuntimeInvisibleTypeAnnotations") rc.invisible_type_annotations = rd.type_annotations(ra);
                    else rc.attrs.emplace_back(rn, ra.data);
                }
                c.record_components.push_back(std::move(rc));
            }
        } else if (an == "Module") c.module = ModuleNode{a.data};
        else if (an == "RuntimeVisibleAnnotations") c.visible_annotations = rd.annotations(a);
        else if (an == "RuntimeInvisibleAnnotations") c.invisible_annotations = rd.annotations(a);
        else if (an == "RuntimeVisibleTypeAnnotations") c.visible_type_annotations = rd.type_annotations(a);
        else if (an == "RuntimeInvisibleTypeAnnotations") c.invisible_type_annotations = rd.type_annotations(a);
        else if (an == "BootstrapMethods") {}   // (read into the instructions and constants that use it)
        else if (const int flag = bt_flag_of(an)) c.access |= flag;
        else c.attrs.emplace_back(an, a.data);
    }
    for (const classfile::Member& f : cf.fields) {
        FieldNode fn;
        fn.access = f.access;
        fn.name = rd.utf8(f.name);
        fn.desc = rd.utf8(f.descriptor);
        for (const classfile::Attribute& a : f.attributes) {
            const std::string an = rd.attr_name(a);
            io::ByteReader r(a.data, "class.field.attribute");
            if (an == "Signature") fn.signature = rd.utf8(r.u16_be());
            else if (an == "ConstantValue") fn.value = rd.constant(r.u16_be());
            else if (an == "RuntimeVisibleAnnotations") fn.visible_annotations = rd.annotations(a);
            else if (an == "RuntimeInvisibleAnnotations") fn.invisible_annotations = rd.annotations(a);
            else if (an == "RuntimeVisibleTypeAnnotations") fn.visible_type_annotations = rd.type_annotations(a);
            else if (an == "RuntimeInvisibleTypeAnnotations") fn.invisible_type_annotations = rd.type_annotations(a);
            else if (const int flag = bt_flag_of(an)) fn.access |= flag;
            else fn.attrs.emplace_back(an, a.data);
        }
        c.fields.push_back(std::move(fn));
    }
    for (const classfile::Member& mm : cf.methods) {
        MethodNode m;
        m.access = mm.access;
        m.name = rd.utf8(mm.name);
        m.desc = rd.utf8(mm.descriptor);
        for (const classfile::Attribute& a : mm.attributes) {
            const std::string an = rd.attr_name(a);
            io::ByteReader r(a.data, "class.method.attribute");
            if (an == "Code") bt_read_code(rd, a, m);
            else if (an == "Signature") m.signature = rd.utf8(r.u16_be());
            else if (an == "Exceptions") {
                const std::uint16_t n = r.u16_be();
                for (std::uint16_t i = 0; i < n; ++i) m.exceptions.push_back(rd.class_name(r.u16_be()));
            } else if (an == "MethodParameters") {
                const std::uint8_t n = r.u8();
                for (std::uint8_t i = 0; i < n; ++i) {
                    ParameterNode p;
                    p.name = rd.utf8_or_none(r.u16_be());
                    p.access = r.u16_be();
                    m.parameters.push_back(std::move(p));
                }
            } else if (an == "AnnotationDefault") m.annotation_default = rd.element_value(r);
            else if (an == "RuntimeVisibleAnnotations") m.visible_annotations = rd.annotations(a);
            else if (an == "RuntimeInvisibleAnnotations") m.invisible_annotations = rd.annotations(a);
            else if (an == "RuntimeVisibleParameterAnnotations") m.visible_parameter_annotations = rd.parameter_annotations(a, m.visible_annotable_parameter_count);
            else if (an == "RuntimeInvisibleParameterAnnotations") m.invisible_parameter_annotations = rd.parameter_annotations(a, m.invisible_annotable_parameter_count);
            else if (an == "RuntimeVisibleTypeAnnotations") m.visible_type_annotations = rd.type_annotations(a);
            else if (an == "RuntimeInvisibleTypeAnnotations") m.invisible_type_annotations = rd.type_annotations(a);
            else if (const int flag = bt_flag_of(an)) m.access |= flag;
            else m.attrs.emplace_back(an, a.data);
        }
        c.methods.push_back(std::move(m));
    }
    return c;
}

// ---- writing

namespace {

// The constant pool built as the parts are written, each constant once.
class Pool {
public:
    Pool() { pool_.emplace_back(); }

    std::uint16_t utf8(const std::string& s) {
        return intern("U" + s, [&] { return classfile::Constant{Tag::Utf8, classfile::modified(s), 0, 0, 0, 0}; });
    }
    std::uint16_t klass(const std::string& internal) {
        const std::uint16_t u = utf8(internal);
        return intern("C" + internal, [&] { return classfile::Constant{Tag::Class, {}, 0, u, 0, 0}; });
    }
    std::uint16_t string(const std::string& s) {
        const std::uint16_t u = utf8(s);
        return intern("S" + s, [&] { return classfile::Constant{Tag::String, {}, 0, u, 0, 0}; });
    }
    std::uint16_t integer(std::int32_t v) {
        return intern(std::format("I{}", v), [&] { return classfile::Constant{Tag::Integer, {}, static_cast<std::uint32_t>(v), 0, 0, 0}; });
    }
    std::uint16_t float_(float v) {
        const std::uint32_t bits = std::bit_cast<std::uint32_t>(v);
        return intern(std::format("F{}", bits), [&] { return classfile::Constant{Tag::Float, {}, bits, 0, 0, 0}; });
    }
    std::uint16_t long_(std::int64_t v) {
        return intern(std::format("J{}", v), [&] { return classfile::Constant{Tag::Long, {}, static_cast<std::uint64_t>(v), 0, 0, 0}; }, true);
    }
    std::uint16_t double_(double v) {
        const std::uint64_t bits = std::bit_cast<std::uint64_t>(v);
        return intern(std::format("D{}", bits), [&] { return classfile::Constant{Tag::Double, {}, bits, 0, 0, 0}; }, true);
    }
    std::uint16_t name_and_type(const std::string& name, const std::string& desc) {
        const std::uint16_t n = utf8(name), d = utf8(desc);
        return intern("N" + name + "\x01" + desc, [&] { return classfile::Constant{Tag::NameAndType, {}, 0, n, d, 0}; });
    }
    std::uint16_t member(Tag tag, const std::string& owner, const std::string& name, const std::string& desc) {
        const std::uint16_t c = klass(owner), nt = name_and_type(name, desc);
        return intern(std::format("M{}{}\x01{}\x01{}", static_cast<int>(tag), owner, name, desc), [&] { return classfile::Constant{tag, {}, 0, c, nt, 0}; });
    }
    std::uint16_t method_type(const std::string& desc) {
        const std::uint16_t d = utf8(desc);
        return intern("T" + desc, [&] { return classfile::Constant{Tag::MethodType, {}, 0, d, 0, 0}; });
    }
    std::uint16_t handle(const Handle& h) {
        const Tag ref_tag = h.tag <= op::H_PUTSTATIC ? Tag::Fieldref : h.is_interface ? Tag::InterfaceMethodref : Tag::Methodref;
        const std::uint16_t ref = member(ref_tag, h.owner, h.name, h.desc);
        return intern(std::format("H{}\x01{}", h.tag, ref), [&] { return classfile::Constant{Tag::MethodHandle, {}, 0, ref, 0, static_cast<std::uint8_t>(h.tag)}; });
    }
    std::uint16_t bootstrap_method(const Handle& h, const std::vector<Constant>& args) {
        std::vector<std::uint16_t> entry{handle(h)};
        for (const Constant& a : args) entry.push_back(constant(a));
        std::string key;
        for (std::uint16_t v : entry) key += std::format("{},", v);
        const auto [it, added] = bootstrap_index_.emplace(key, static_cast<std::uint16_t>(bootstrap_.size()));
        if (added) bootstrap_.push_back(std::move(entry));
        return it->second;
    }
    std::uint16_t dynamic(Tag tag, const std::string& name, const std::string& desc, const Handle& bsm, const std::vector<Constant>& args) {
        const std::uint16_t b = bootstrap_method(bsm, args);
        const std::uint16_t nt = name_and_type(name, desc);
        return intern(std::format("Y{}\x01{}\x01{}", static_cast<int>(tag), b, nt), [&] { return classfile::Constant{tag, {}, 0, b, nt, 0}; });
    }
    std::uint16_t constant(const Constant& c) {
        return std::visit(
            [&](const auto& v) -> std::uint16_t {
                using V = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<V, std::int32_t>) return integer(v);
                else if constexpr (std::is_same_v<V, float>) return float_(v);
                else if constexpr (std::is_same_v<V, std::int64_t>) return long_(v);
                else if constexpr (std::is_same_v<V, double>) return double_(v);
                else if constexpr (std::is_same_v<V, std::string>) return string(v);
                else if constexpr (std::is_same_v<V, Type>) return v.get_sort() == Type::METHOD ? method_type(v.get_descriptor()) : klass(v.get_internal_name());
                else if constexpr (std::is_same_v<V, Handle>) return handle(v);
                else return dynamic(Tag::Dynamic, v->name, v->descriptor, v->bootstrap_method, v->bootstrap_method_arguments);
            },
            c);
    }
    static bool is_wide(const Constant& c) { return std::holds_alternative<std::int64_t>(c) || std::holds_alternative<double>(c); }

    std::vector<classfile::Constant>& entries() { return pool_; }
    const std::vector<std::vector<std::uint16_t>>& bootstrap_methods() const { return bootstrap_; }

private:
    template <class Make>
    std::uint16_t intern(const std::string& key, Make make, bool two_slots = false) {
        if (const auto it = index_.find(key); it != index_.end()) return it->second;
        if (pool_.size() + (two_slots ? 2 : 1) > 65535) bt_bad("the constant pool is full");
        const auto at = static_cast<std::uint16_t>(pool_.size());
        pool_.push_back(make());
        if (two_slots) pool_.emplace_back();
        index_.emplace(key, at);
        return at;
    }
    std::vector<classfile::Constant> pool_;
    std::unordered_map<std::string, std::uint16_t> index_;
    std::vector<std::vector<std::uint16_t>> bootstrap_;
    std::unordered_map<std::string, std::uint16_t> bootstrap_index_;
};

struct Writer {
    Pool pool;
    int major = 0;

    classfile::Attribute attribute(const std::string& name, Bytes data) { return {pool.utf8(name), std::move(data)}; }
    classfile::Attribute u16_attribute(const std::string& name, std::uint16_t v) {
        io::ByteWriter w;
        w.u16_be(v);
        return attribute(name, w.take());
    }

    void element_value(io::ByteWriter& w, const AnnotationValue& v) {
        switch (v.kind) {
            case AnnotationValue::BYTE: w.u8('B'); w.u16_be(pool.integer(static_cast<std::int32_t>(v.integral))); break;
            case AnnotationValue::BOOLEAN: w.u8('Z'); w.u16_be(pool.integer(static_cast<std::int32_t>(v.integral))); break;
            case AnnotationValue::CHAR: w.u8('C'); w.u16_be(pool.integer(static_cast<std::int32_t>(v.integral))); break;
            case AnnotationValue::SHORT: w.u8('S'); w.u16_be(pool.integer(static_cast<std::int32_t>(v.integral))); break;
            case AnnotationValue::INT: w.u8('I'); w.u16_be(pool.integer(static_cast<std::int32_t>(v.integral))); break;
            case AnnotationValue::LONG: w.u8('J'); w.u16_be(pool.long_(v.integral)); break;
            case AnnotationValue::FLOAT: w.u8('F'); w.u16_be(pool.float_(static_cast<float>(v.floating))); break;
            case AnnotationValue::DOUBLE: w.u8('D'); w.u16_be(pool.double_(v.floating)); break;
            case AnnotationValue::STRING: w.u8('s'); w.u16_be(pool.utf8(v.text)); break;
            case AnnotationValue::ENUM:
                w.u8('e');
                w.u16_be(pool.utf8(v.enum_value.desc));
                w.u16_be(pool.utf8(v.enum_value.value));
                break;
            case AnnotationValue::CLASS: w.u8('c'); w.u16_be(pool.utf8(v.type->get_descriptor())); break;
            case AnnotationValue::ANNOTATION: w.u8('@'); annotation(w, *v.annotation); break;
            case AnnotationValue::ARRAY:
                w.u8('[');
                w.u16_be(static_cast<std::uint16_t>(v.values.size()));
                for (const AnnotationValue& e : v.values) element_value(w, e);
                break;
        }
    }
    void annotation(io::ByteWriter& w, const AnnotationNode& a) {
        w.u16_be(pool.utf8(a.desc));
        w.u16_be(static_cast<std::uint16_t>(a.values.size()));
        for (const auto& [name, value] : a.values) {
            w.u16_be(pool.utf8(name));
            element_value(w, value);
        }
    }
    void annotations(std::vector<classfile::Attribute>& out, const std::string& name, const std::vector<AnnotationNode>& list) {
        if (list.empty()) return;
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(list.size()));
        for (const AnnotationNode& a : list) annotation(w, a);
        out.push_back(attribute(name, w.take()));
    }
    void type_annotations(std::vector<classfile::Attribute>& out, const std::string& name, const std::vector<TypeAnnotationNode>& list) {
        if (list.empty()) return;
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(list.size()));
        for (const TypeAnnotationNode& t : list) {
            w.u8(static_cast<std::uint8_t>(t.type_ref));
            w.bytes(t.type_path);
            annotation(w, t);
        }
        out.push_back(attribute(name, w.take()));
    }
    void parameter_annotations(std::vector<classfile::Attribute>& out, const std::string& name, int count, const std::vector<std::vector<AnnotationNode>>& list) {
        if (list.empty()) return;
        io::ByteWriter w;
        w.u8(static_cast<std::uint8_t>(count != 0 ? count : static_cast<int>(list.size())));
        for (const auto& p : list) {
            w.u16_be(static_cast<std::uint16_t>(p.size()));
            for (const AnnotationNode& a : p) annotation(w, a);
        }
        out.push_back(attribute(name, w.take()));
    }
    // Synthetic and Deprecated as attributes (ASM: Synthetic only before Java 5, where the flag does not exist), the flags written without them
    void flags_attributes(std::vector<classfile::Attribute>& out, int access) {
        if ((access & op::ACC_SYNTHETIC) != 0 && major < 49) out.push_back(attribute("Synthetic", {}));
        if ((access & op::ACC_DEPRECATED) != 0) out.push_back(attribute("Deprecated", {}));
    }
    std::uint16_t written_access(int access) const {
        int a = access & ~(op::ACC_DEPRECATED | op::ACC_RECORD);
        if (major < 49) a &= ~op::ACC_SYNTHETIC;
        return static_cast<std::uint16_t>(a & 0xFFFF);
    }
    void raw(std::vector<classfile::Attribute>& out, const std::vector<std::pair<std::string, Bytes>>& attrs) {
        for (const auto& [name, data] : attrs) out.push_back(attribute(name, data));
    }

    Bytes code(const MethodNode& m);
};

// ---- a method's code: laid out, encoded, max stack and locals computed

// What an instruction does to the stack (for those whose effect does not depend on a descriptor); ASM's Frame/MethodWriter STACK_SIZE_DELTA.
int bt_stack_delta(int o) {
    switch (o) {
        case op::NOP: return 0;
        case op::ACONST_NULL: case op::ICONST_M1: case op::ICONST_0: case op::ICONST_1: case op::ICONST_2: case op::ICONST_3: case op::ICONST_4:
        case op::ICONST_5: case op::FCONST_0: case op::FCONST_1: case op::FCONST_2: case op::BIPUSH: case op::SIPUSH: return 1;
        case op::LCONST_0: case op::LCONST_1: case op::DCONST_0: case op::DCONST_1: return 2;
        case op::ILOAD: case op::FLOAD: case op::ALOAD: return 1;
        case op::LLOAD: case op::DLOAD: return 2;
        case op::IALOAD: case op::FALOAD: case op::AALOAD: case op::BALOAD: case op::CALOAD: case op::SALOAD: return -1;
        case op::LALOAD: case op::DALOAD: return 0;
        case op::ISTORE: case op::FSTORE: case op::ASTORE: return -1;
        case op::LSTORE: case op::DSTORE: return -2;
        case op::IASTORE: case op::FASTORE: case op::AASTORE: case op::BASTORE: case op::CASTORE: case op::SASTORE: return -3;
        case op::LASTORE: case op::DASTORE: return -4;
        case op::POP: return -1;
        case op::POP2: return -2;
        case op::DUP: case op::DUP_X1: case op::DUP_X2: return 1;
        case op::DUP2: case op::DUP2_X1: case op::DUP2_X2: return 2;
        case op::SWAP: return 0;
        case op::IADD: case op::FADD: case op::ISUB: case op::FSUB: case op::IMUL: case op::FMUL: case op::IDIV: case op::FDIV: case op::IREM:
        case op::FREM: case op::ISHL: case op::ISHR: case op::IUSHR: case op::IAND: case op::IOR: case op::IXOR: return -1;
        case op::LADD: case op::DADD: case op::LSUB: case op::DSUB: case op::LMUL: case op::DMUL: case op::LDIV: case op::DDIV: case op::LREM:
        case op::DREM: case op::LAND: case op::LOR: case op::LXOR: return -2;
        case op::LSHL: case op::LSHR: case op::LUSHR: return -1;
        case op::INEG: case op::LNEG: case op::FNEG: case op::DNEG: return 0;
        case op::IINC: return 0;
        case op::I2L: case op::I2D: case op::F2L: case op::F2D: return 1;
        case op::L2I: case op::L2F: case op::D2I: case op::D2F: return -1;
        case op::I2F: case op::L2D: case op::F2I: case op::D2L: case op::I2B: case op::I2C: case op::I2S: return 0;
        case op::LCMP: case op::DCMPL: case op::DCMPG: return -3;
        case op::FCMPL: case op::FCMPG: return -1;
        case op::IFEQ: case op::IFNE: case op::IFLT: case op::IFGE: case op::IFGT: case op::IFLE: case op::IFNULL: case op::IFNONNULL: return -1;
        case op::IF_ICMPEQ: case op::IF_ICMPNE: case op::IF_ICMPLT: case op::IF_ICMPGE: case op::IF_ICMPGT: case op::IF_ICMPLE: case op::IF_ACMPEQ:
        case op::IF_ACMPNE: return -2;
        case op::GOTO: return 0;
        case op::JSR: return 1;
        case op::RET: return 0;
        case op::TABLESWITCH: case op::LOOKUPSWITCH: return -1;
        case op::IRETURN: case op::FRETURN: case op::ARETURN: return -1;
        case op::LRETURN: case op::DRETURN: return -2;
        case op::RETURN: return 0;
        case op::NEW: return 1;
        case op::NEWARRAY: case op::ANEWARRAY: case op::ARRAYLENGTH: case op::CHECKCAST: case op::INSTANCEOF: return 0;
        case op::ATHROW: case op::MONITORENTER: case op::MONITOREXIT: return -1;
        default: return 0;
    }
}

int bt_desc_size(const std::string& desc) { return Type::get_type(desc).get_size(); }

int bt_insn_delta(const InsnNode* n) {
    switch (n->kind) {
        case InsnNode::FIELD_INSN: {
            const auto* f = static_cast<const FieldInsnNode*>(n);
            const int size = bt_desc_size(f->desc);
            switch (f->opcode) {
                case op::GETSTATIC: return size;
                case op::PUTSTATIC: return -size;
                case op::GETFIELD: return size - 1;
                default: return -size - 1;
            }
        }
        case InsnNode::METHOD_INSN: {
            const auto* mi = static_cast<const MethodInsnNode*>(n);
            const int sizes = Type::get_arguments_and_return_sizes(mi->desc);
            const int args = (sizes >> 2) - (mi->opcode == op::INVOKESTATIC ? 1 : 0);
            return (sizes & 3) - args;
        }
        case InsnNode::INVOKE_DYNAMIC_INSN: {
            const int sizes = Type::get_arguments_and_return_sizes(static_cast<const InvokeDynamicInsnNode*>(n)->desc);
            return (sizes & 3) - ((sizes >> 2) - 1);
        }
        case InsnNode::LDC_INSN: return Pool::is_wide(static_cast<const LdcInsnNode*>(n)->cst) ? 2 : 1;
        case InsnNode::MULTIANEWARRAY_INSN: return 1 - static_cast<const MultiANewArrayInsnNode*>(n)->dims;
        default: return bt_stack_delta(n->opcode);
    }
}

bool bt_ends_flow(int o) {
    return o == op::GOTO || o == op::RET || o == op::ATHROW || (o >= op::IRETURN && o <= op::RETURN) || o == op::TABLESWITCH || o == op::LOOKUPSWITCH;
}

Bytes Writer::code(const MethodNode& m) {
    // the instructions in order; each label's place
    std::vector<const InsnNode*> insns;
    for (const InsnNode* n = m.instructions.get_first(); n != nullptr; n = n->next) insns.push_back(n);
    // pool indexes first: an ldc's form depends on its constant's index
    std::unordered_map<const InsnNode*, std::uint16_t> index;
    for (const InsnNode* n : insns) {
        switch (n->kind) {
            case InsnNode::LDC_INSN: index[n] = pool.constant(static_cast<const LdcInsnNode*>(n)->cst); break;
            case InsnNode::TYPE_INSN: index[n] = pool.klass(static_cast<const TypeInsnNode*>(n)->desc); break;
            case InsnNode::FIELD_INSN: {
                const auto* f = static_cast<const FieldInsnNode*>(n);
                index[n] = pool.member(Tag::Fieldref, f->owner, f->name, f->desc);
                break;
            }
            case InsnNode::METHOD_INSN: {
                const auto* mi = static_cast<const MethodInsnNode*>(n);
                index[n] = pool.member(mi->itf ? Tag::InterfaceMethodref : Tag::Methodref, mi->owner, mi->name, mi->desc);
                break;
            }
            case InsnNode::INVOKE_DYNAMIC_INSN: {
                const auto* d = static_cast<const InvokeDynamicInsnNode*>(n);
                index[n] = pool.dynamic(Tag::InvokeDynamic, d->name, d->desc, d->bsm, d->bsm_args);
                break;
            }
            case InsnNode::MULTIANEWARRAY_INSN: index[n] = pool.klass(static_cast<const MultiANewArrayInsnNode*>(n)->desc); break;
            default: break;
        }
    }
    const auto var_wide = [](int var) { return var > 255; };
    std::set<const InsnNode*> far;   // GOTO/JSR written as goto_w/jsr_w
    std::unordered_map<const InsnNode*, std::uint32_t> offset;
    const auto size_at = [&](const InsnNode* n, std::uint32_t at) -> std::uint32_t {
        switch (n->kind) {
            case InsnNode::LABEL:
            case InsnNode::LINE:
            case InsnNode::FRAME: return 0;
            case InsnNode::VAR_INSN: {
                const int v = static_cast<const VarInsnNode*>(n)->var;
                if (var_wide(v)) return 4;
                return (v <= 3 && n->opcode != op::RET) ? 1 : 2;
            }
            case InsnNode::IINC_INSN: {
                const auto* i = static_cast<const IincInsnNode*>(n);
                return (var_wide(i->var) || i->incr > 127 || i->incr < -128) ? 6 : 3;
            }
            case InsnNode::INT_INSN: return n->opcode == op::SIPUSH ? 3 : 2;
            case InsnNode::LDC_INSN: return (Pool::is_wide(static_cast<const LdcInsnNode*>(n)->cst) || index.at(n) > 255) ? 3 : 2;
            case InsnNode::TYPE_INSN:
            case InsnNode::FIELD_INSN: return 3;
            case InsnNode::METHOD_INSN: return n->opcode == op::INVOKEINTERFACE ? 5 : 3;
            case InsnNode::INVOKE_DYNAMIC_INSN: return 5;
            case InsnNode::MULTIANEWARRAY_INSN: return 4;
            case InsnNode::JUMP_INSN: return far.contains(n) ? 5 : 3;
            case InsnNode::TABLESWITCH_INSN: return 1 + (4 - (at + 1) % 4) % 4 + 12 + 4 * static_cast<std::uint32_t>(static_cast<const TableSwitchInsnNode*>(n)->labels.size());
            case InsnNode::LOOKUPSWITCH_INSN: return 1 + (4 - (at + 1) % 4) % 4 + 8 + 8 * static_cast<std::uint32_t>(static_cast<const LookupSwitchInsnNode*>(n)->labels.size());
            default: return 1;
        }
    };
    std::uint32_t length = 0;
    for (;;) {
        std::uint32_t at = 0;
        for (const InsnNode* n : insns) {
            offset[n] = at;
            at += size_at(n, at);
        }
        length = at;
        bool changed = false;
        for (const InsnNode* n : insns) {
            if (n->kind != InsnNode::JUMP_INSN || far.contains(n)) continue;
            const std::int64_t d = static_cast<std::int64_t>(offset.at(static_cast<const JumpInsnNode*>(n)->label)) - offset.at(n);
            if (d < -32768 || d > 32767) {
                if (n->opcode != op::GOTO && n->opcode != op::JSR) bt_bad("a conditional branch too far for 16 bits");
                far.insert(n);
                changed = true;
            }
        }
        if (!changed) break;
    }
    if (length > 65535) bt_bad("a method's code is over 64 KB: " + m.name);
    const auto at = [&](const LabelNode* l) -> std::uint32_t {
        const auto it = offset.find(l);
        if (it == offset.end()) bt_bad("a label that is not in the method's instructions");
        return it->second;
    };
    // encode
    io::ByteWriter c;
    for (const InsnNode* n : insns) {
        const std::uint32_t here = offset.at(n);
        switch (n->kind) {
            case InsnNode::LABEL:
            case InsnNode::LINE:
            case InsnNode::FRAME: break;
            case InsnNode::VAR_INSN: {
                const int v = static_cast<const VarInsnNode*>(n)->var;
                const int o = n->opcode;
                if (var_wide(v)) {
                    c.u8(196);
                    c.u8(static_cast<std::uint8_t>(o));
                    c.u16_be(static_cast<std::uint16_t>(v));
                } else if (v <= 3 && o != op::RET) {
                    c.u8(static_cast<std::uint8_t>(o < op::ISTORE ? 26 + (o - op::ILOAD) * 4 + v : 59 + (o - op::ISTORE) * 4 + v));
                } else {
                    c.u8(static_cast<std::uint8_t>(o));
                    c.u8(static_cast<std::uint8_t>(v));
                }
                break;
            }
            case InsnNode::IINC_INSN: {
                const auto* i = static_cast<const IincInsnNode*>(n);
                if (var_wide(i->var) || i->incr > 127 || i->incr < -128) {
                    c.u8(196);
                    c.u8(op::IINC);
                    c.u16_be(static_cast<std::uint16_t>(i->var));
                    c.u16_be(static_cast<std::uint16_t>(static_cast<std::int16_t>(i->incr)));
                } else {
                    c.u8(op::IINC);
                    c.u8(static_cast<std::uint8_t>(i->var));
                    c.u8(static_cast<std::uint8_t>(static_cast<std::int8_t>(i->incr)));
                }
                break;
            }
            case InsnNode::INT_INSN: {
                const int v = static_cast<const IntInsnNode*>(n)->operand;
                c.u8(static_cast<std::uint8_t>(n->opcode));
                if (n->opcode == op::SIPUSH) c.u16_be(static_cast<std::uint16_t>(static_cast<std::int16_t>(v)));
                else c.u8(static_cast<std::uint8_t>(v));
                break;
            }
            case InsnNode::LDC_INSN: {
                const std::uint16_t i = index.at(n);
                if (Pool::is_wide(static_cast<const LdcInsnNode*>(n)->cst)) {
                    c.u8(20);
                    c.u16_be(i);
                } else if (i > 255) {
                    c.u8(19);
                    c.u16_be(i);
                } else {
                    c.u8(op::LDC);
                    c.u8(static_cast<std::uint8_t>(i));
                }
                break;
            }
            case InsnNode::TYPE_INSN:
            case InsnNode::FIELD_INSN:
                c.u8(static_cast<std::uint8_t>(n->opcode));
                c.u16_be(index.at(n));
                break;
            case InsnNode::METHOD_INSN: {
                c.u8(static_cast<std::uint8_t>(n->opcode));
                c.u16_be(index.at(n));
                if (n->opcode == op::INVOKEINTERFACE) {
                    c.u8(static_cast<std::uint8_t>(Type::get_arguments_and_return_sizes(static_cast<const MethodInsnNode*>(n)->desc) >> 2));
                    c.u8(0);
                }
                break;
            }
            case InsnNode::INVOKE_DYNAMIC_INSN:
                c.u8(op::INVOKEDYNAMIC);
                c.u16_be(index.at(n));
                c.u16_be(0);
                break;
            case InsnNode::MULTIANEWARRAY_INSN:
                c.u8(op::MULTIANEWARRAY);
                c.u16_be(index.at(n));
                c.u8(static_cast<std::uint8_t>(static_cast<const MultiANewArrayInsnNode*>(n)->dims));
                break;
            case InsnNode::JUMP_INSN: {
                const std::int64_t d = static_cast<std::int64_t>(at(static_cast<const JumpInsnNode*>(n)->label)) - here;
                if (far.contains(n)) {
                    c.u8(n->opcode == op::GOTO ? 200 : 201);
                    c.u32_be(static_cast<std::uint32_t>(static_cast<std::int32_t>(d)));
                } else {
                    c.u8(static_cast<std::uint8_t>(n->opcode));
                    c.u16_be(static_cast<std::uint16_t>(static_cast<std::int16_t>(d)));
                }
                break;
            }
            case InsnNode::TABLESWITCH_INSN: {
                const auto* t = static_cast<const TableSwitchInsnNode*>(n);
                c.u8(op::TABLESWITCH);
                c.zeros((4 - (here + 1) % 4) % 4);
                c.u32_be(static_cast<std::uint32_t>(static_cast<std::int32_t>(at(t->dflt) - here)));
                c.u32_be(static_cast<std::uint32_t>(t->min));
                c.u32_be(static_cast<std::uint32_t>(t->max));
                for (const LabelNode* l : t->labels) c.u32_be(static_cast<std::uint32_t>(static_cast<std::int32_t>(at(l) - here)));
                break;
            }
            case InsnNode::LOOKUPSWITCH_INSN: {
                const auto* t = static_cast<const LookupSwitchInsnNode*>(n);
                c.u8(op::LOOKUPSWITCH);
                c.zeros((4 - (here + 1) % 4) % 4);
                c.u32_be(static_cast<std::uint32_t>(static_cast<std::int32_t>(at(t->dflt) - here)));
                c.u32_be(static_cast<std::uint32_t>(t->keys.size()));
                for (std::size_t k = 0; k < t->keys.size(); ++k) {
                    c.u32_be(static_cast<std::uint32_t>(t->keys[k]));
                    c.u32_be(static_cast<std::uint32_t>(static_cast<std::int32_t>(at(t->labels[k]) - here)));
                }
                break;
            }
            default: c.u8(static_cast<std::uint8_t>(n->opcode)); break;
        }
    }
    const Bytes bytes = c.take();

    // max locals (ASM, COMPUTE_MAXS): the arguments, every local an instruction or the variable table names
    int max_locals = Type::get_arguments_and_return_sizes(m.desc) >> 2;
    if ((m.access & op::ACC_STATIC) != 0) --max_locals;
    for (const InsnNode* n : insns) {
        if (n->kind == InsnNode::VAR_INSN) {
            const int o = n->opcode;
            const bool two = o == op::LLOAD || o == op::DLOAD || o == op::LSTORE || o == op::DSTORE;
            max_locals = std::max(max_locals, static_cast<const VarInsnNode*>(n)->var + (two ? 2 : 1));
        } else if (n->kind == InsnNode::IINC_INSN) {
            max_locals = std::max(max_locals, static_cast<const IincInsnNode*>(n)->var + 1);
        }
    }
    for (const LocalVariableNode& lv : m.local_variables) {
        const bool two = lv.desc == "J" || lv.desc == "D";
        max_locals = std::max(max_locals, lv.index + (two ? 2 : 1));
    }
    // max stack: the flow of the code, each instruction's entry height (a handler starts with its exception)
    std::unordered_map<const InsnNode*, std::size_t> position;
    for (std::size_t i = 0; i < insns.size(); ++i) position[insns[i]] = i;
    std::vector<int> height(insns.size(), -1);
    std::vector<std::size_t> work;
    const auto reach = [&](std::size_t i, int h) {
        if (i < insns.size() && height[i] < h) {
            height[i] = h;
            work.push_back(i);
        }
    };
    if (!insns.empty()) reach(0, 0);
    for (const TryCatchBlockNode& t : m.try_catch_blocks) reach(position.at(t.handler), 1);
    int max_stack = 0;
    while (!work.empty()) {
        const std::size_t i = work.back();
        work.pop_back();
        const InsnNode* n = insns[i];
        const int after = height[i] + bt_insn_delta(n);
        max_stack = std::max({max_stack, height[i], after});
        switch (n->kind) {
            case InsnNode::JUMP_INSN: {
                const auto* j = static_cast<const JumpInsnNode*>(n);
                reach(position.at(j->label), after);
                if (n->opcode == op::JSR) reach(i + 1, height[i]);   // (the subroutine returns with the stack as it was)
                else if (n->opcode != op::GOTO) reach(i + 1, after);
                break;
            }
            case InsnNode::TABLESWITCH_INSN: {
                const auto* t = static_cast<const TableSwitchInsnNode*>(n);
                reach(position.at(t->dflt), after);
                for (const LabelNode* l : t->labels) reach(position.at(l), after);
                break;
            }
            case InsnNode::LOOKUPSWITCH_INSN: {
                const auto* t = static_cast<const LookupSwitchInsnNode*>(n);
                reach(position.at(t->dflt), after);
                for (const LabelNode* l : t->labels) reach(position.at(l), after);
                break;
            }
            default:
                if (!bt_ends_flow(n->opcode)) reach(i + 1, after);
        }
    }

    io::ByteWriter w;
    w.u16_be(static_cast<std::uint16_t>(max_stack));
    w.u16_be(static_cast<std::uint16_t>(max_locals));
    w.u32_be(static_cast<std::uint32_t>(bytes.size()));
    w.bytes(bytes);
    w.u16_be(static_cast<std::uint16_t>(m.try_catch_blocks.size()));
    for (const TryCatchBlockNode& t : m.try_catch_blocks) {
        w.u16_be(static_cast<std::uint16_t>(at(t.start)));
        w.u16_be(static_cast<std::uint16_t>(at(t.end)));
        w.u16_be(static_cast<std::uint16_t>(at(t.handler)));
        w.u16_be(t.type ? pool.klass(*t.type) : 0);
    }
    std::vector<classfile::Attribute> attrs;
    {
        io::ByteWriter lines;
        std::uint16_t count = 0;
        for (const InsnNode* n : insns) {
            if (n->kind != InsnNode::LINE) continue;
            const auto* l = static_cast<const LineNumberNode*>(n);
            lines.u16_be(static_cast<std::uint16_t>(at(l->start)));
            lines.u16_be(static_cast<std::uint16_t>(l->line));
            ++count;
        }
        if (count > 0) {
            io::ByteWriter a;
            a.u16_be(count);
            a.bytes(lines.take());
            attrs.push_back(attribute("LineNumberTable", a.take()));
        }
    }
    if (!m.local_variables.empty()) {
        io::ByteWriter a, t;
        std::uint16_t typed = 0;
        a.u16_be(static_cast<std::uint16_t>(m.local_variables.size()));
        for (const LocalVariableNode& lv : m.local_variables) {
            const std::uint32_t s = at(lv.start), e = at(lv.end);
            a.u16_be(static_cast<std::uint16_t>(s));
            a.u16_be(static_cast<std::uint16_t>(e - s));
            a.u16_be(pool.utf8(lv.name));
            a.u16_be(pool.utf8(lv.desc));
            a.u16_be(static_cast<std::uint16_t>(lv.index));
            if (lv.signature) {
                t.u16_be(static_cast<std::uint16_t>(s));
                t.u16_be(static_cast<std::uint16_t>(e - s));
                t.u16_be(pool.utf8(lv.name));
                t.u16_be(pool.utf8(*lv.signature));
                t.u16_be(static_cast<std::uint16_t>(lv.index));
                ++typed;
            }
        }
        attrs.push_back(attribute("LocalVariableTable", a.take()));
        if (typed > 0) {
            io::ByteWriter tt;
            tt.u16_be(typed);
            tt.bytes(t.take());
            attrs.push_back(attribute("LocalVariableTypeTable", tt.take()));
        }
    }
    w.u16_be(static_cast<std::uint16_t>(attrs.size()));
    for (const classfile::Attribute& a : attrs) {
        w.u16_be(a.name);
        w.u32_be(static_cast<std::uint32_t>(a.data.size()));
        w.bytes(a.data);
    }
    return w.take();
}

}  // namespace

Bytes write(const ClassNode& c) {
    Writer wr;
    wr.major = c.version & 0xFFFF;
    classfile::ClassFile cf;
    cf.minor = static_cast<std::uint16_t>(static_cast<unsigned>(c.version) >> 16);
    cf.major = static_cast<std::uint16_t>(wr.major);
    cf.this_class = wr.pool.klass(c.name);
    cf.super_class = c.super_name ? wr.pool.klass(*c.super_name) : 0;
    for (const std::string& i : c.interfaces) cf.interfaces.push_back(wr.pool.klass(i));
    cf.access = wr.written_access(c.access);
    for (const FieldNode& f : c.fields) {
        classfile::Member m;
        m.access = wr.written_access(f.access);
        m.name = wr.pool.utf8(f.name);
        m.descriptor = wr.pool.utf8(f.desc);
        if (f.value) m.attributes.push_back(wr.u16_attribute("ConstantValue", wr.pool.constant(*f.value)));
        wr.flags_attributes(m.attributes, f.access);
        if (f.signature) m.attributes.push_back(wr.u16_attribute("Signature", wr.pool.utf8(*f.signature)));
        wr.annotations(m.attributes, "RuntimeVisibleAnnotations", f.visible_annotations);
        wr.annotations(m.attributes, "RuntimeInvisibleAnnotations", f.invisible_annotations);
        wr.type_annotations(m.attributes, "RuntimeVisibleTypeAnnotations", f.visible_type_annotations);
        wr.type_annotations(m.attributes, "RuntimeInvisibleTypeAnnotations", f.invisible_type_annotations);
        wr.raw(m.attributes, f.attrs);
        cf.fields.push_back(std::move(m));
    }
    for (const MethodNode& mn : c.methods) {
        classfile::Member m;
        m.access = wr.written_access(mn.access);
        m.name = wr.pool.utf8(mn.name);
        m.descriptor = wr.pool.utf8(mn.desc);
        if (mn.has_code()) m.attributes.push_back(classfile::Attribute{wr.pool.utf8("Code"), wr.code(mn)});
        if (!mn.exceptions.empty()) {
            io::ByteWriter w;
            w.u16_be(static_cast<std::uint16_t>(mn.exceptions.size()));
            for (const std::string& e : mn.exceptions) w.u16_be(wr.pool.klass(e));
            m.attributes.push_back(wr.attribute("Exceptions", w.take()));
        }
        wr.flags_attributes(m.attributes, mn.access);
        if (mn.signature) m.attributes.push_back(wr.u16_attribute("Signature", wr.pool.utf8(*mn.signature)));
        wr.annotations(m.attributes, "RuntimeVisibleAnnotations", mn.visible_annotations);
        wr.annotations(m.attributes, "RuntimeInvisibleAnnotations", mn.invisible_annotations);
        wr.type_annotations(m.attributes, "RuntimeVisibleTypeAnnotations", mn.visible_type_annotations);
        wr.type_annotations(m.attributes, "RuntimeInvisibleTypeAnnotations", mn.invisible_type_annotations);
        if (mn.annotation_default) {
            io::ByteWriter w;
            wr.element_value(w, *mn.annotation_default);
            m.attributes.push_back(wr.attribute("AnnotationDefault", w.take()));
        }
        wr.parameter_annotations(m.attributes, "RuntimeVisibleParameterAnnotations", mn.visible_annotable_parameter_count, mn.visible_parameter_annotations);
        wr.parameter_annotations(m.attributes, "RuntimeInvisibleParameterAnnotations", mn.invisible_annotable_parameter_count, mn.invisible_parameter_annotations);
        if (!mn.parameters.empty()) {
            io::ByteWriter w;
            w.u8(static_cast<std::uint8_t>(mn.parameters.size()));
            for (const ParameterNode& p : mn.parameters) {
                w.u16_be(p.name ? wr.pool.utf8(*p.name) : 0);
                w.u16_be(static_cast<std::uint16_t>(p.access));
            }
            m.attributes.push_back(wr.attribute("MethodParameters", w.take()));
        }
        wr.raw(m.attributes, mn.attrs);
        cf.methods.push_back(std::move(m));
    }
    std::vector<classfile::Attribute>& a = cf.attributes;
    if (c.source_file) a.push_back(wr.u16_attribute("SourceFile", wr.pool.utf8(*c.source_file)));
    if (c.source_debug) {
        const std::string m = classfile::modified(*c.source_debug);
        a.push_back(wr.attribute("SourceDebugExtension", Bytes(m.begin(), m.end())));
    }
    if (c.outer_class) {
        io::ByteWriter w;
        w.u16_be(wr.pool.klass(*c.outer_class));
        w.u16_be(c.outer_method ? wr.pool.name_and_type(*c.outer_method, c.outer_method_desc.value_or("")) : 0);
        a.push_back(wr.attribute("EnclosingMethod", w.take()));
    }
    wr.flags_attributes(a, c.access);
    if (c.signature) a.push_back(wr.u16_attribute("Signature", wr.pool.utf8(*c.signature)));
    wr.annotations(a, "RuntimeVisibleAnnotations", c.visible_annotations);
    wr.annotations(a, "RuntimeInvisibleAnnotations", c.invisible_annotations);
    wr.type_annotations(a, "RuntimeVisibleTypeAnnotations", c.visible_type_annotations);
    wr.type_annotations(a, "RuntimeInvisibleTypeAnnotations", c.invisible_type_annotations);
    if (c.nest_host_class) a.push_back(wr.u16_attribute("NestHost", wr.pool.klass(*c.nest_host_class)));
    const auto class_list = [&](const std::string& name, const std::vector<std::string>& list) {
        if (list.empty()) return;
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(list.size()));
        for (const std::string& s : list) w.u16_be(wr.pool.klass(s));
        a.push_back(wr.attribute(name, w.take()));
    };
    class_list("NestMembers", c.nest_members);
    class_list("PermittedSubclasses", c.permitted_subclasses);
    if ((c.access & op::ACC_RECORD) != 0 || !c.record_components.empty()) {
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(c.record_components.size()));
        for (const RecordComponentNode& rc : c.record_components) {
            w.u16_be(wr.pool.utf8(rc.name));
            w.u16_be(wr.pool.utf8(rc.descriptor));
            std::vector<classfile::Attribute> ra;
            if (rc.signature) ra.push_back(wr.u16_attribute("Signature", wr.pool.utf8(*rc.signature)));
            wr.annotations(ra, "RuntimeVisibleAnnotations", rc.visible_annotations);
            wr.annotations(ra, "RuntimeInvisibleAnnotations", rc.invisible_annotations);
            wr.type_annotations(ra, "RuntimeVisibleTypeAnnotations", rc.visible_type_annotations);
            wr.type_annotations(ra, "RuntimeInvisibleTypeAnnotations", rc.invisible_type_annotations);
            wr.raw(ra, rc.attrs);
            w.u16_be(static_cast<std::uint16_t>(ra.size()));
            for (const classfile::Attribute& x : ra) {
                w.u16_be(x.name);
                w.u32_be(static_cast<std::uint32_t>(x.data.size()));
                w.bytes(x.data);
            }
        }
        a.push_back(wr.attribute("Record", w.take()));
    }
    if (c.module) a.push_back(wr.attribute("Module", c.module->module_attribute));
    if (!c.inner_classes.empty()) {
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(c.inner_classes.size()));
        for (const InnerClassNode& ic : c.inner_classes) {
            w.u16_be(wr.pool.klass(ic.name));
            w.u16_be(ic.outer_name ? wr.pool.klass(*ic.outer_name) : 0);
            w.u16_be(ic.inner_name ? wr.pool.utf8(*ic.inner_name) : 0);
            w.u16_be(static_cast<std::uint16_t>(ic.access));
        }
        a.push_back(wr.attribute("InnerClasses", w.take()));
    }
    wr.raw(a, c.attrs);
    if (!wr.pool.bootstrap_methods().empty()) {
        io::ByteWriter w;
        w.u16_be(static_cast<std::uint16_t>(wr.pool.bootstrap_methods().size()));
        for (const auto& entry : wr.pool.bootstrap_methods()) {
            w.u16_be(entry[0]);
            w.u16_be(static_cast<std::uint16_t>(entry.size() - 1));
            for (std::size_t i = 1; i < entry.size(); ++i) w.u16_be(entry[i]);
        }
        a.push_back(wr.attribute("BootstrapMethods", w.take()));
    }
    cf.pool = std::move(wr.pool.entries());
    return classfile::write(cf);
}

}  // namespace cb::java::bytecode
