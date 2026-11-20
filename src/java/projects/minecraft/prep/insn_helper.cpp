// OPHelper, InsnHelper and IdentifyCall (constants.hpp). Ported from RDI (org.mcphackers.rdi.util.OPHelper / InsnHelper / IdentifyCall),
// MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "java/projects/minecraft/prep/constants.hpp"

namespace cb::java::minecraft::prep {

namespace {

using bc::InsnNode;
using bc::InsnList;
namespace op = bc::op;

[[noreturn]] void fail(const char* what) { throw std::runtime_error(what); }

std::array<int, 0xFF> make_stack_size_delta() {
    std::array<int, 0xFF> t{};   // zero for what is not listed, as Java's new int[0xFF]
    t[op::NOP] = 0;
    t[op::ACONST_NULL] = 1;
    t[op::ICONST_M1] = 1;
    t[op::ICONST_0] = 1;
    t[op::ICONST_1] = 1;
    t[op::ICONST_2] = 1;
    t[op::ICONST_3] = 1;
    t[op::ICONST_4] = 1;
    t[op::ICONST_5] = 1;
    t[op::LCONST_0] = 2;
    t[op::LCONST_1] = 1;
    t[op::FCONST_0] = 1;
    t[op::FCONST_1] = 1;
    t[op::FCONST_2] = 1;
    t[op::DCONST_0] = 2;
    t[op::DCONST_1] = 2;
    t[op::BIPUSH] = 1;
    t[op::SIPUSH] = 1;
    t[op::LDC] = 1;   // special case
    t[op::ILOAD] = 1;
    t[op::LLOAD] = 2;
    t[op::FLOAD] = 1;
    t[op::DLOAD] = 2;
    t[op::ALOAD] = 1;
    t[op::IALOAD] = -1;
    t[op::LALOAD] = 0;
    t[op::FALOAD] = -1;
    t[op::DALOAD] = 0;
    t[op::AALOAD] = -1;
    t[op::BALOAD] = -1;
    t[op::CALOAD] = -1;
    t[op::SALOAD] = -1;
    t[op::ISTORE] = -1;
    t[op::LSTORE] = -2;
    t[op::FSTORE] = -1;
    t[op::DSTORE] = -2;
    t[op::ASTORE] = -1;
    t[op::IASTORE] = -3;
    t[op::LASTORE] = -4;
    t[op::FASTORE] = -3;
    t[op::DASTORE] = -4;
    t[op::AASTORE] = -3;
    t[op::BASTORE] = -3;
    t[op::CASTORE] = -3;
    t[op::SASTORE] = -3;
    t[op::POP] = -1;
    t[op::POP2] = -2;
    t[op::DUP] = 1;
    t[op::DUP_X1] = 1;
    t[op::DUP_X2] = 1;
    t[op::DUP2] = 2;
    t[op::DUP2_X1] = 2;
    t[op::DUP2_X2] = 2;
    t[op::SWAP] = 0;
    t[op::IADD] = -1;
    t[op::LADD] = -2;
    t[op::FADD] = -1;
    t[op::DADD] = -2;
    t[op::ISUB] = -1;
    t[op::LSUB] = -2;
    t[op::FSUB] = -1;
    t[op::DSUB] = -2;
    t[op::IMUL] = -1;
    t[op::LMUL] = -2;
    t[op::FMUL] = -1;
    t[op::DMUL] = -2;
    t[op::IDIV] = -1;
    t[op::LDIV] = -2;
    t[op::FDIV] = -1;
    t[op::DDIV] = -2;
    t[op::IREM] = -1;
    t[op::LREM] = -2;
    t[op::FREM] = -1;
    t[op::DREM] = -2;
    t[op::INEG] = 0;
    t[op::LNEG] = 0;
    t[op::FNEG] = 0;
    t[op::DNEG] = 0;
    t[op::ISHL] = -1;
    t[op::LSHL] = -1;
    t[op::ISHR] = -1;
    t[op::LSHR] = -1;
    t[op::IUSHR] = -1;
    t[op::LUSHR] = -1;
    t[op::IAND] = -1;
    t[op::LAND] = -2;
    t[op::IOR] = -1;
    t[op::LOR] = -2;
    t[op::IXOR] = -1;
    t[op::LXOR] = -2;
    t[op::IINC] = 0;
    t[op::I2L] = 1;
    t[op::I2F] = 0;
    t[op::I2D] = 1;
    t[op::L2I] = -1;
    t[op::L2F] = -1;
    t[op::L2D] = 0;
    t[op::F2I] = 0;
    t[op::F2L] = 1;
    t[op::F2D] = 1;
    t[op::D2I] = -1;
    t[op::D2L] = 0;
    t[op::D2F] = -1;
    t[op::I2B] = 0;
    t[op::I2C] = 0;
    t[op::I2S] = 0;
    t[op::LCMP] = -3;
    t[op::FCMPL] = -1;
    t[op::FCMPG] = -1;
    t[op::DCMPL] = -3;
    t[op::DCMPG] = -3;
    t[op::IFEQ] = -1;
    t[op::IFNE] = -1;
    t[op::IFLT] = -1;
    t[op::IFGE] = -1;
    t[op::IFGT] = -1;
    t[op::IFLE] = -1;
    t[op::IF_ICMPEQ] = -2;
    t[op::IF_ICMPNE] = -2;
    t[op::IF_ICMPLT] = -2;
    t[op::IF_ICMPGE] = -2;
    t[op::IF_ICMPGT] = -2;
    t[op::IF_ICMPLE] = -2;
    t[op::IF_ACMPEQ] = -2;
    t[op::IF_ACMPNE] = -2;
    t[op::GOTO] = 0;
    t[op::JSR] = 1;
    t[op::RET] = 0;
    t[op::TABLESWITCH] = -1;
    t[op::LOOKUPSWITCH] = -1;
    t[op::IRETURN] = -1;
    t[op::LRETURN] = -2;
    t[op::FRETURN] = -1;
    t[op::DRETURN] = -2;
    t[op::ARETURN] = -1;
    t[op::RETURN] = 0;
    t[op::GETSTATIC] = 0;   // special case
    t[op::PUTSTATIC] = 0;   // special case
    t[op::GETFIELD] = 0;    // special case
    t[op::PUTFIELD] = 0;    // special case
    t[op::INVOKEVIRTUAL] = 0;    // special case
    t[op::INVOKESPECIAL] = 0;    // special case
    t[op::INVOKESTATIC] = 0;     // special case
    t[op::INVOKEINTERFACE] = 0;  // special case
    t[op::INVOKEDYNAMIC] = 0;    // special case
    t[op::NEW] = 1;
    t[op::NEWARRAY] = 0;
    t[op::ANEWARRAY] = 0;
    t[op::ARRAYLENGTH] = 0;
    t[op::ATHROW] = 0;   // special case
    t[op::CHECKCAST] = 0;
    t[op::INSTANCEOF] = 0;
    t[op::MONITORENTER] = -1;
    t[op::MONITOREXIT] = -1;
    t[op::MULTIANEWARRAY] = 0;   // special case
    t[op::IFNULL] = -1;
    t[op::IFNONNULL] = -1;
    return t;
}

const std::array<int, 0xFF>& stack_size_delta() {
    static const std::array<int, 0xFF> table = make_stack_size_delta();
    return table;
}

}  // namespace

// ---- OPHelper

int OPHelper::get_stack_size_delta(const InsnNode* insn) {
    const int opcode = insn->get_opcode();
    if (opcode == -1) return 0;
    if (insn->get_type() == InsnNode::LDC_INSN) {
        const auto* ldc = static_cast<const bc::LdcInsnNode*>(insn);
        if (std::holds_alternative<double>(ldc->cst) || std::holds_alternative<std::int64_t>(ldc->cst)) return 2;
        return 1;
    }
    if (insn->get_type() == InsnNode::METHOD_INSN) {
        const auto* invoke = static_cast<const bc::MethodInsnNode*>(insn);
        const int sizes = bc::Type::get_arguments_and_return_sizes(invoke->desc);
        int argument_sizes = sizes >> 2;
        if (opcode == op::INVOKESTATIC) argument_sizes--;
        const int return_size = sizes & 3;
        return return_size - argument_sizes;
    }
    if (insn->get_type() == InsnNode::FIELD_INSN) {
        const auto* field = static_cast<const bc::FieldInsnNode*>(insn);
        const int return_size = bc::Type::get_type(field->desc).get_size();
        if (opcode == op::GETSTATIC) return return_size;
        return return_size - 1;
    }
    if (opcode != op::ATHROW && opcode != op::MULTIANEWARRAY && opcode != op::INVOKEDYNAMIC) return stack_size_delta()[static_cast<std::size_t>(opcode)];
    throw std::runtime_error("Could not get stack size delta for " + std::to_string(opcode));
}

bool OPHelper::is_return(int opcode) {
    return opcode == op::RETURN || opcode == op::IRETURN || opcode == op::LRETURN || opcode == op::FRETURN || opcode == op::DRETURN ||
           opcode == op::ARETURN;
}

bool OPHelper::is_array_load(int opcode) {
    switch (opcode) {
    case op::AALOAD:
    case op::BALOAD:   // for booleans and bytes
    case op::CALOAD:
    case op::DALOAD:
    case op::FALOAD:
    case op::IALOAD:
    case op::LALOAD:
    case op::SALOAD:
        return true;
    default:
        return false;
    }
}

bool OPHelper::is_var_load(int opcode) {
    switch (opcode) {
    case op::ALOAD:
    case op::DLOAD:
    case op::FLOAD:
    case op::ILOAD:
    case op::LLOAD:
        return true;
    default:
        return false;
    }
}

int OPHelper::reverse_var_opcode(int opcode) {
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
    default: fail("Opcode not valid.");
    }
}

bool OPHelper::is_var_similar_type(int opcode1, int opcode2) {
    switch (opcode1) {
    case op::AALOAD: return opcode2 == op::ALOAD;
    case op::BALOAD:
    case op::CALOAD:
    case op::IALOAD:
    case op::SALOAD: return opcode2 == op::ILOAD;
    case op::DALOAD: return opcode2 == op::DLOAD;
    case op::FALOAD: return opcode2 == op::FLOAD;
    case op::LALOAD: return opcode2 == op::LLOAD;
    case op::ALOAD: return opcode2 == op::AALOAD;
    case op::DLOAD: return opcode2 == op::DALOAD;
    case op::FLOAD: return opcode2 == op::FALOAD;
    case op::ILOAD: return opcode2 == op::IALOAD || opcode2 == op::SALOAD || opcode2 == op::BALOAD || opcode2 == op::CALOAD;
    case op::LLOAD: return opcode2 == op::LALOAD;
    // -- The same story for the store operation family --
    case op::AASTORE: return opcode2 == op::ASTORE;
    case op::BASTORE:
    case op::CASTORE:
    case op::IASTORE:
    case op::SASTORE: return opcode2 == op::ISTORE;
    case op::DASTORE: return opcode2 == op::DSTORE;
    case op::FASTORE: return opcode2 == op::FSTORE;
    case op::LASTORE: return opcode2 == op::LSTORE;
    case op::ASTORE: return opcode2 == op::ASTORE;
    case op::DSTORE: return opcode2 == op::DSTORE;
    case op::FSTORE: return opcode2 == op::FSTORE;
    case op::ISTORE: return opcode2 == op::IASTORE || opcode2 == op::SASTORE || opcode2 == op::BASTORE || opcode2 == op::CASTORE;
    case op::LSTORE: return opcode2 == op::LSTORE;
    default: fail("Opcode1 not valid.");
    }
}

bool OPHelper::is_var_store(int opcode) {
    switch (opcode) {
    case op::ASTORE:
    case op::DSTORE:
    case op::FSTORE:
    case op::ISTORE:
    case op::LSTORE:
        return true;
    default:
        return false;
    }
}

// ---- InsnHelper

std::vector<InsnNode*> InsnHelper::fill(InsnNode* insn, int size) {
    std::vector<InsnNode*> arr(static_cast<std::size_t>(size), nullptr);
    int i = 0;
    while (insn != nullptr && i < size) {
        arr[static_cast<std::size_t>(i)] = insn;
        insn = next_insn(insn);
        i++;
    }
    return arr;
}

std::vector<InsnNode*> InsnHelper::fill_backwards(InsnNode* insn, int size) {
    std::vector<InsnNode*> arr(static_cast<std::size_t>(size), nullptr);
    int i = size - 1;
    while (insn != nullptr && i >= 0) {
        arr[static_cast<std::size_t>(i)] = insn;
        insn = previous_insn(insn);
        i--;
    }
    return arr;
}

void InsnHelper::remove_range(InsnList& insns, InsnNode* first, InsnNode* last) {
    if (first == nullptr || last == nullptr) return;
    InsnNode* next = first;
    while (next != nullptr && next != last) {
        InsnNode* for_removal = next;
        next = next->get_next();
        insns.remove(for_removal);
    }
    insns.remove(last);
}

void InsnHelper::remove(InsnList& insns, const std::vector<InsnNode*>& to_remove) {
    for (InsnNode* insn : to_remove) insns.remove(insn);
}

std::vector<InsnNode*> InsnHelper::range(InsnNode* start, InsnNode* end) {
    std::vector<InsnNode*> list;
    InsnNode* insn = start;
    while (insn != end) {
        if (insn == nullptr) fail("range: the end is not after the start");   // (a NullPointerException in the original)
        list.push_back(insn);
        insn = insn->get_next();
    }
    list.push_back(end);
    return list;
}

namespace {

// AbstractInsnNode.clone(labels): a copy of one instruction, its labels those of the map (a label not in it: null), made by `dest`.
InsnNode* clone_insn(InsnList& dest, const InsnNode* insn, const std::unordered_map<const bc::LabelNode*, bc::LabelNode*>& labels) {
    const auto label_of = [&](const bc::LabelNode* l) -> bc::LabelNode* {
        const auto it = labels.find(l);
        return it == labels.end() ? nullptr : it->second;
    };
    switch (insn->get_type()) {
    case InsnNode::INSN: return dest.make<bc::ZeroInsnNode>(insn->get_opcode());
    case InsnNode::INT_INSN: return dest.make<bc::IntInsnNode>(insn->get_opcode(), static_cast<const bc::IntInsnNode*>(insn)->operand);
    case InsnNode::VAR_INSN: return dest.make<bc::VarInsnNode>(insn->get_opcode(), static_cast<const bc::VarInsnNode*>(insn)->var);
    case InsnNode::TYPE_INSN: return dest.make<bc::TypeInsnNode>(insn->get_opcode(), static_cast<const bc::TypeInsnNode*>(insn)->desc);
    case InsnNode::FIELD_INSN: {
        const auto* f = static_cast<const bc::FieldInsnNode*>(insn);
        return dest.make<bc::FieldInsnNode>(f->get_opcode(), f->owner, f->name, f->desc);
    }
    case InsnNode::METHOD_INSN: {
        const auto* m = static_cast<const bc::MethodInsnNode*>(insn);
        return dest.make<bc::MethodInsnNode>(m->get_opcode(), m->owner, m->name, m->desc, m->itf);
    }
    case InsnNode::INVOKE_DYNAMIC_INSN: {
        const auto* d = static_cast<const bc::InvokeDynamicInsnNode*>(insn);
        return dest.make<bc::InvokeDynamicInsnNode>(d->name, d->desc, d->bsm, d->bsm_args);
    }
    case InsnNode::JUMP_INSN: {
        const auto* j = static_cast<const bc::JumpInsnNode*>(insn);
        return dest.make<bc::JumpInsnNode>(j->get_opcode(), label_of(j->label));
    }
    case InsnNode::LABEL: return label_of(static_cast<const bc::LabelNode*>(insn));
    case InsnNode::LDC_INSN: return dest.make<bc::LdcInsnNode>(static_cast<const bc::LdcInsnNode*>(insn)->cst);
    case InsnNode::IINC_INSN: {
        const auto* i = static_cast<const bc::IincInsnNode*>(insn);
        return dest.make<bc::IincInsnNode>(i->var, i->incr);
    }
    case InsnNode::TABLESWITCH_INSN: {
        const auto* t = static_cast<const bc::TableSwitchInsnNode*>(insn);
        std::vector<bc::LabelNode*> ls;
        for (bc::LabelNode* l : t->labels) ls.push_back(label_of(l));
        return dest.make<bc::TableSwitchInsnNode>(t->min, t->max, label_of(t->dflt), std::move(ls));
    }
    case InsnNode::LOOKUPSWITCH_INSN: {
        const auto* t = static_cast<const bc::LookupSwitchInsnNode*>(insn);
        std::vector<bc::LabelNode*> ls;
        for (bc::LabelNode* l : t->labels) ls.push_back(label_of(l));
        return dest.make<bc::LookupSwitchInsnNode>(label_of(t->dflt), t->keys, std::move(ls));
    }
    case InsnNode::MULTIANEWARRAY_INSN: {
        const auto* m = static_cast<const bc::MultiANewArrayInsnNode*>(insn);
        return dest.make<bc::MultiANewArrayInsnNode>(m->desc, m->dims);
    }
    case InsnNode::LINE: {
        const auto* l = static_cast<const bc::LineNumberNode*>(insn);
        return dest.make<bc::LineNumberNode>(l->line, label_of(l->start));
    }
    case InsnNode::FRAME: break;
    }
    fail("clone: a frame is not modelled");
}

InsnList clone_all(const std::vector<const InsnNode*>& insns) {
    InsnList dest;
    std::unordered_map<const bc::LabelNode*, bc::LabelNode*> labels;
    for (const InsnNode* insn : insns) {
        if (insn->get_type() == InsnNode::LABEL) labels[static_cast<const bc::LabelNode*>(insn)] = dest.make<bc::LabelNode>();
    }
    for (const InsnNode* insn : insns) dest.add(clone_insn(dest, insn, labels));
    return dest;
}

}  // namespace

InsnList InsnHelper::clone(const InsnList& insn_list) {
    std::vector<const InsnNode*> v;
    for (const InsnNode* n = insn_list.get_first(); n != nullptr; n = n->get_next()) v.push_back(n);
    return clone_all(v);
}

InsnList InsnHelper::clone(const std::vector<InsnNode*>& insn_list) {
    return clone_all(std::vector<const InsnNode*>(insn_list.begin(), insn_list.end()));
}

bc::LabelNode* InsnHelper::label_before(InsnNode* current) {
    current = current->get_previous();
    if (current == nullptr) fail("label_before: no instruction before");   // (a NullPointerException)
    while (current->get_type() == InsnNode::LINE) {   // Skip line number nodes
        current = current->get_previous();
        if (current == nullptr) fail("label_before: no instruction before");
    }
    if (current->get_type() == InsnNode::LABEL) return static_cast<bc::LabelNode*>(current);
    return nullptr;
}

InsnNode* InsnHelper::next_insn(InsnNode* current) {
    while ((current = current->get_next()) != nullptr && current->get_opcode() == -1) {}
    return current;
}

InsnNode* InsnHelper::previous_insn(InsnNode* current) {
    while ((current = current->get_previous()) != nullptr && current->get_opcode() == -1) {}
    return current;
}

bool InsnHelper::contains_invoke(const InsnList& insns, const bc::MethodInsnNode& invoke) {
    for (const InsnNode* insn = insns.get_first(); insn != nullptr; insn = insn->get_next()) {
        if (insn->get_opcode() == invoke.get_opcode()) {
            if (insn->get_type() != InsnNode::METHOD_INSN) fail("contains_invoke: not a method instruction");   // (ClassCastException)
            const auto* invoke2 = static_cast<const bc::MethodInsnNode*>(insn);
            if (invoke2->owner == invoke.owner && invoke2->name == invoke.name && invoke2->desc == invoke.desc) return true;
        }
    }
    return false;
}

void InsnHelper::add_try_catch(bc::MethodNode& method, InsnNode* start_insn, InsnNode* end_insn, std::optional<std::string> exception) {
    add_try_catch(method, start_insn, end_insn, nullptr, std::move(exception), nullptr);
}

void InsnHelper::add_try_catch(bc::MethodNode& method, InsnNode* start_insn, InsnNode* end_insn, InsnList* handle,
                               std::optional<std::string> exception) {
    add_try_catch(method, start_insn, end_insn, handle, std::move(exception), nullptr);
}

void InsnHelper::add_try_catch(bc::MethodNode& method, InsnNode* start_insn, InsnNode* end_insn, InsnList* handle,
                               std::optional<std::string> exception, const bc::TryCatchBlockNode* after_block) {
    InsnList& instructions = method.instructions;
    if (!instructions.contains(start_insn) || !instructions.contains(end_insn)) fail("Instruction does not belong to the method");
    bc::LabelNode* start = instructions.make<bc::LabelNode>();
    InsnList insert;
    bc::LabelNode* end = insert.make<bc::LabelNode>();
    bc::LabelNode* handler = insert.make<bc::LabelNode>();
    bc::LabelNode* after = insert.make<bc::LabelNode>();
    instructions.insert_before(start_insn, start);
    insert.add(end);
    insert.add(insert.make<bc::JumpInsnNode>(op::GOTO, after));
    insert.add(handler);
    if (handle == nullptr) insert.add(insert.make<bc::ZeroInsnNode>(op::POP));
    else insert.add(*handle);
    insert.add(after);
    instructions.insert(end_insn, insert);
    const bc::TryCatchBlockNode block{start, end, handler, std::move(exception)};
    if (after_block == nullptr) {
        method.try_catch_blocks.insert(method.try_catch_blocks.begin(), block);
    } else {
        std::ptrdiff_t index = -1;   // indexOf(afterBlock): the block itself
        for (std::size_t i = 0; i < method.try_catch_blocks.size(); ++i) {
            if (&method.try_catch_blocks[i] == after_block) { index = static_cast<std::ptrdiff_t>(i); break; }
        }
        method.try_catch_blocks.insert(method.try_catch_blocks.begin() + index + 1, block);
    }
}

int InsnHelper::get_free_index(const InsnList& instructions) { return get_free_index(instructions, nullptr); }

int InsnHelper::get_free_index(const InsnList& instructions, const InsnNode* until) {
    int last_free = 1;
    const InsnNode* insn = instructions.get_first();
    while (insn != nullptr) {
        if (insn == until) break;
        if (insn->get_type() == InsnNode::VAR_INSN) {
            const auto* var = static_cast<const bc::VarInsnNode*>(insn);
            last_free = std::max(last_free, var->var + std::abs(OPHelper::get_stack_size_delta(insn)));
        }
        insn = insn->get_next();
    }
    return last_free;
}

InsnNode* InsnHelper::int_insn(InsnList& owner, std::int32_t value) {
    switch (value) {
    case -1: return owner.make<bc::ZeroInsnNode>(op::ICONST_M1);
    case 0: return owner.make<bc::ZeroInsnNode>(op::ICONST_0);
    case 1: return owner.make<bc::ZeroInsnNode>(op::ICONST_1);
    case 2: return owner.make<bc::ZeroInsnNode>(op::ICONST_2);
    case 3: return owner.make<bc::ZeroInsnNode>(op::ICONST_3);
    case 4: return owner.make<bc::ZeroInsnNode>(op::ICONST_4);
    case 5: return owner.make<bc::ZeroInsnNode>(op::ICONST_5);
    default:
        if (value >= -128 && value <= 127) return owner.make<bc::IntInsnNode>(op::BIPUSH, value);
        if (value >= -32768 && value <= 32767) return owner.make<bc::IntInsnNode>(op::SIPUSH, value);
        return owner.make<bc::LdcInsnNode>(bc::Constant(value));
    }
}

InsnNode* InsnHelper::long_insn(InsnList& owner, std::int64_t value) {
    if (value == 0) return owner.make<bc::ZeroInsnNode>(op::LCONST_0);
    if (value == 1) return owner.make<bc::ZeroInsnNode>(op::LCONST_1);
    return owner.make<bc::LdcInsnNode>(bc::Constant(value));
}

InsnNode* InsnHelper::boolean_insn(InsnList& owner, bool value) { return owner.make<bc::ZeroInsnNode>(value ? op::ICONST_1 : op::ICONST_0); }

InsnNode* InsnHelper::float_insn(InsnList& owner, float value) {
    if (value == 0.0f) return owner.make<bc::ZeroInsnNode>(op::FCONST_0);   // (a negative zero too: it compares equal, as in Java)
    if (value == 1.0f) return owner.make<bc::ZeroInsnNode>(op::FCONST_1);
    if (value == 2.0f) return owner.make<bc::ZeroInsnNode>(op::FCONST_2);
    return owner.make<bc::LdcInsnNode>(bc::Constant(value));
}

InsnNode* InsnHelper::double_insn(InsnList& owner, double value) {
    if (value == 0.0) return owner.make<bc::ZeroInsnNode>(op::DCONST_0);
    if (value == 1.0) return owner.make<bc::ZeroInsnNode>(op::DCONST_1);
    return owner.make<bc::LdcInsnNode>(bc::Constant(value));
}

namespace {
const bc::Constant& ldc_cst(const InsnNode* insn) {
    if (insn->get_type() != InsnNode::LDC_INSN) fail("not an ldc");   // (ClassCastException)
    return static_cast<const bc::LdcInsnNode*>(insn)->cst;
}
}  // namespace

std::int32_t InsnHelper::int_value(const InsnNode* insn) {
    switch (insn->get_opcode()) {
    case op::ICONST_M1: return -1;
    case op::ICONST_0: return 0;
    case op::ICONST_1: return 1;
    case op::ICONST_2: return 2;
    case op::ICONST_3: return 3;
    case op::ICONST_4: return 4;
    case op::ICONST_5: return 5;
    case op::BIPUSH:
    case op::SIPUSH: return static_cast<const bc::IntInsnNode*>(insn)->operand;
    default: return std::get<std::int32_t>(ldc_cst(insn));
    }
}

std::int64_t InsnHelper::long_value(const InsnNode* insn) {
    switch (insn->get_opcode()) {
    case op::LCONST_0: return 0;
    case op::LCONST_1: return 1;
    default: return std::get<std::int64_t>(ldc_cst(insn));
    }
}

bool InsnHelper::boolean_value(const InsnNode* insn) { return insn->get_opcode() == op::ICONST_1; }

float InsnHelper::float_value(const InsnNode* insn) {
    switch (insn->get_opcode()) {
    case op::FCONST_0: return 0.0f;
    case op::FCONST_1: return 1.0f;
    case op::FCONST_2: return 2.0f;
    default: return std::get<float>(ldc_cst(insn));
    }
}

double InsnHelper::double_value(const InsnNode* insn) {
    switch (insn->get_opcode()) {
    case op::DCONST_0: return 0.0;
    case op::DCONST_1: return 1.0;
    default: return std::get<double>(ldc_cst(insn));
    }
}

InsnNode* InsnHelper::get_super(InsnNode* first) {
    InsnNode* insn = first;
    while (insn != nullptr) {
        InsnNode* prev = insn->get_previous();
        if (prev != nullptr && prev->get_opcode() == op::ALOAD && static_cast<bc::VarInsnNode*>(prev)->var == 0 && insn->get_opcode() == op::INVOKESPECIAL &&
            static_cast<bc::MethodInsnNode*>(insn)->name == "<init>") {
            break;
        }
        insn = insn->get_next();
    }
    return insn;
}

InsnNode* InsnHelper::get_last_return(InsnNode* last) {
    InsnNode* insn = last;
    while (insn != nullptr) {
        if (OPHelper::is_return(insn->get_opcode())) break;
        insn = insn->get_previous();
    }
    return insn;
}

namespace {

// `compare[i]` as Java's (Integer) cast: null, or the int; any other operand is a ClassCastException.
std::optional<std::int32_t> integer_operand(const Operand& o) {
    if (o.kind == Operand::NONE) return std::nullopt;
    if (o.kind != Operand::INT) fail("compare_insn: an Integer was expected");
    return o.integer;
}

// o.equals(cst) for an Object o: the same box type and value (floating point by bits, a NaN equal to a NaN).
bool constant_equals(const Operand& o, const bc::Constant& cst) {
    switch (o.kind) {
    case Operand::INT: return std::holds_alternative<std::int32_t>(cst) && std::get<std::int32_t>(cst) == o.integer;
    case Operand::STRING: return std::holds_alternative<std::string>(cst) && std::get<std::string>(cst) == o.text;
    case Operand::HANDLE: return std::holds_alternative<bc::Handle>(cst) && std::get<bc::Handle>(cst) == o.handle;
    case Operand::CONSTANT: {
        const bc::Constant& a = *o.constant;
        if (a.index() != cst.index()) return false;
        if (std::holds_alternative<float>(a)) {
            const float x = std::get<float>(a), y = std::get<float>(cst);
            return (x != x && y != y) || std::memcmp(&x, &y, sizeof x) == 0;
        }
        if (std::holds_alternative<double>(a)) {
            const double x = std::get<double>(a), y = std::get<double>(cst);
            return (x != x && y != y) || std::memcmp(&x, &y, sizeof x) == 0;
        }
        return a == cst;
    }
    default: return false;
    }
}

}  // namespace

bool InsnHelper::compare_insn(const InsnNode* insn, int opcode, const std::vector<Operand>& compare) {
    if (insn == nullptr) return false;
    const bool opcode_equal = opcode == insn->get_opcode() || opcode == -1;
    if (!opcode_equal) return false;
    if (compare.empty() && opcode_equal) return true;
    bool matches = true;
    const auto string_matches = [&](const Operand& o, const std::string& s) { return o.kind == Operand::NONE || (o.kind == Operand::STRING && s == o.text); };
    switch (insn->get_type()) {
    case InsnNode::INSN: return matches;   // nothing to compare
    case InsnNode::INT_INSN: {
        const auto* i = static_cast<const bc::IntInsnNode*>(insn);
        if (compare.size() > 0) {
            const auto v = integer_operand(compare[0]);
            if (!v) fail("compare_insn: null Integer");   // (a NullPointerException)
            matches &= i->operand == *v;
        }
        return matches;
    }
    case InsnNode::VAR_INSN: {
        const auto* i = static_cast<const bc::VarInsnNode*>(insn);
        if (compare.size() > 0) {
            const auto v = integer_operand(compare[0]);
            if (!v) fail("compare_insn: null Integer");
            matches &= i->var == *v;
        }
        return matches;
    }
    case InsnNode::TYPE_INSN: {
        const auto* i = static_cast<const bc::TypeInsnNode*>(insn);
        if (compare.size() > 0) matches &= compare[0].kind == Operand::STRING && i->desc == compare[0].text;
        return matches;
    }
    case InsnNode::FIELD_INSN: {
        const auto* i = static_cast<const bc::FieldInsnNode*>(insn);
        if (compare.size() > 0) matches &= string_matches(compare[0], i->owner);
        if (compare.size() > 1) matches &= string_matches(compare[1], i->name);
        if (compare.size() > 2) matches &= string_matches(compare[2], i->desc);
        return matches;
    }
    case InsnNode::METHOD_INSN: {
        const auto* i = static_cast<const bc::MethodInsnNode*>(insn);
        if (compare.size() > 0) matches &= string_matches(compare[0], i->owner);
        if (compare.size() > 1) matches &= string_matches(compare[1], i->name);
        if (compare.size() > 2) matches &= string_matches(compare[2], i->desc);
        return matches;
    }
    case InsnNode::INVOKE_DYNAMIC_INSN: {
        const auto* i = static_cast<const bc::InvokeDynamicInsnNode*>(insn);
        if (compare.size() > 0) matches &= compare[0].kind == Operand::NONE || (compare[0].kind == Operand::HANDLE && i->bsm == compare[0].handle);
        if (compare.size() > 1) matches &= string_matches(compare[1], i->name);
        if (compare.size() > 2) matches &= string_matches(compare[2], i->desc);
        return matches;
    }
    case InsnNode::JUMP_INSN: {
        const auto* i = static_cast<const bc::JumpInsnNode*>(insn);
        if (compare.size() > 0) matches &= compare[0].label == i->label;
        return matches;
    }
    case InsnNode::LABEL: {
        if (compare.size() > 0) matches &= compare[0].label == static_cast<const bc::LabelNode*>(insn);
        return matches;
    }
    case InsnNode::LDC_INSN: {
        const auto* i = static_cast<const bc::LdcInsnNode*>(insn);
        if (compare.size() > 0) {
            const Operand& o = compare[0];
            matches &= (o.kind != Operand::NONE && constant_equals(o, i->cst));   // (an ldc's cst is never null)
        }
        return matches;
    }
    case InsnNode::IINC_INSN: {
        const auto* i = static_cast<const bc::IincInsnNode*>(insn);
        if (compare.size() > 0) {
            const auto v = integer_operand(compare[0]);
            matches &= !v || i->var == *v;
        }
        if (compare.size() > 1) {
            const auto v = integer_operand(compare[1]);
            matches &= !v || i->incr == *v;
        }
        return matches;
    }
    case InsnNode::TABLESWITCH_INSN: return matches;    // TODO in the original
    case InsnNode::LOOKUPSWITCH_INSN: return matches;   // TODO in the original
    case InsnNode::MULTIANEWARRAY_INSN: {
        const auto* i = static_cast<const bc::MultiANewArrayInsnNode*>(insn);
        if (compare.size() > 0) matches &= string_matches(compare[0], i->desc);
        if (compare.size() > 1) {
            const auto v = integer_operand(compare[0]);   // (the original reads compare[0] again, not compare[1])
            matches &= !v || i->dims == *v;
        }
        return matches;
    }
    case InsnNode::FRAME: return matches;
    case InsnNode::LINE: {
        const auto* i = static_cast<const bc::LineNumberNode*>(insn);
        if (compare.size() > 0) {
            const auto v = integer_operand(compare[0]);
            if (!v) fail("compare_insn: null Integer");
            matches &= i->line == *v;
        }
        return matches;
    }
    }
    return false;
}

// ---- IdentifyCall

IdentifyCall::IdentifyCall(bc::MethodInsnNode* invoke) : invoke_insn_(invoke), arguments_(init_arguments()) {}

std::vector<IdentifyCall::Argument> IdentifyCall::init_arguments() {
    const bool is_static = invoke_insn_->get_opcode() == op::INVOKESTATIC;
    const std::vector<bc::Type> arg_types = bc::Type::get_argument_types(invoke_insn_->desc);
    std::vector<Argument> args;
    if (arg_types.empty()) return {};
    InsnNode* insn = invoke_insn_->get_previous();
    InsnNode* start = nullptr;
    InsnNode* end = insn;
    const auto step = [&](int& stack) {   // one turn of the original's walk
        if (insn == nullptr) fail("IdentifyCall: the walk back ran off the method");   // (a NullPointerException)
        stack -= OPHelper::get_stack_size_delta(insn);
        if (stack == 0) start = insn;
        insn = insn->get_previous();
    };
    for (int current_arg = static_cast<int>(arg_types.size()) - 1; current_arg >= 0; current_arg--) {
        int stack = arg_types[static_cast<std::size_t>(current_arg)].get_size();
        end = insn;
        while (stack > 0) step(stack);
        if (start != nullptr) {
            while (static_cast<int>(args.size()) <= current_arg) args.emplace_back(std::nullopt);
            args[static_cast<std::size_t>(current_arg)] = InsnHelper::range(start, end);
        }
    }
    if (!is_static) {
        int stack = 1;
        end = insn;
        while (stack > 0) step(stack);
        args.insert(args.begin(), InsnHelper::range(start, end));
    }
    return args;
}

const std::vector<InsnNode*>& IdentifyCall::get_argument(int index) const {
    if (index < 0 || static_cast<std::size_t>(index) >= arguments_.size()) throw std::out_of_range("argument index");
    const Argument& a = arguments_[static_cast<std::size_t>(index)];
    if (!a) fail("IdentifyCall: that argument was not found");
    return *a;
}

void IdentifyCall::remove_call(InsnList& insns, bc::MethodInsnNode* invoke) {
    for (const Argument& arg : IdentifyCall(invoke).get_arguments()) {
        if (!arg) fail("IdentifyCall: an argument was not found");   // (a NullPointerException)
        InsnHelper::remove(insns, *arg);
    }
    insns.remove(invoke);
}

std::optional<IdentifyCall> IdentifyCall::identify_call(const InsnList& insns, int opcode, const std::string& owner, const std::string& name,
                                                        const std::string& desc) {
    for (InsnNode* insn : insns) {
        if (InsnHelper::compare_insn(insn, opcode, {owner, name, desc})) return IdentifyCall(static_cast<bc::MethodInsnNode*>(insn));
    }
    return std::nullopt;
}

}  // namespace cb::java::minecraft::prep
