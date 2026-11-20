// java/decompiler/instructions.cpp -- see instructions.hpp. Ported from Fernflower (de.fernflower.code, code.interpreter, struct.gen.DataPoint,
// StructMethod.parseBytecode), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
#include "java/decompiler/instructions.hpp"

#include <format>
#include <stdexcept>

#include "core/io.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

namespace {

// A row of the stack-effect table: `special` (the original's null row: worked out by the instruction), else what is read and written (none: null).
struct ItImpact {
    bool special;
    bool has_read;
    int nread;
    int read[3];
    bool has_write;
    int nwrite;
    int write[1];
};

#include "java/decompiler/instructions_tables.inc"

constexpr int kItIconst[] = {-1, 0, 1, 2, 3, 4, 5};
constexpr int kItLoadStore[] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
constexpr int kItLoads[] = {cc::opc_iload, cc::opc_lload, cc::opc_fload, cc::opc_dload, cc::opc_aload};
constexpr int kItStores[] = {cc::opc_istore, cc::opc_lstore, cc::opc_fstore, cc::opc_dstore, cc::opc_astore};
constexpr int kItArrayType[] = {cc::TYPE_BOOLEAN, cc::TYPE_CHAR, cc::TYPE_FLOAT, cc::TYPE_DOUBLE, cc::TYPE_BYTE, cc::TYPE_SHORT, cc::TYPE_INT, cc::TYPE_LONG};

std::string it_hex(int v) { return std::format("{:x}", static_cast<unsigned>(v)); }

}  // namespace

std::string_view opcode_name(int opcode) { return opcode >= 0 && opcode < 202 ? kItNames[opcode] : std::string_view(); }

// ---- Instruction

std::shared_ptr<Instruction> Instruction::make(int opcode, bool wide, int group, int bytecode_version, std::vector<int> operands) {
    auto instr = std::make_shared<Instruction>();
    if ((opcode >= cc::opc_ifeq && opcode <= cc::opc_if_acmpne) || opcode == cc::opc_ifnull || opcode == cc::opc_ifnonnull) instr->kind = Kind::If;
    else if (opcode == cc::opc_goto || opcode == cc::opc_jsr || opcode == cc::opc_goto_w || opcode == cc::opc_jsr_w) instr->kind = Kind::Jump;
    else if (opcode == cc::opc_tableswitch || opcode == cc::opc_lookupswitch) instr->kind = Kind::Switch;
    instr->opcode = opcode;
    instr->wide = wide;
    instr->group = group;
    instr->bytecode_version = bytecode_version;
    instr->operands = std::move(operands);
    return instr;
}

bool Instruction::can_fallthrough() const {
    return opcode != cc::opc_goto && opcode != cc::opc_goto_w && opcode != cc::opc_ret && !(opcode >= cc::opc_ireturn && opcode <= cc::opc_return) &&
           opcode != cc::opc_athrow && opcode != cc::opc_jsr && opcode != cc::opc_tableswitch && opcode != cc::opc_lookupswitch;
}

bool Instruction::equals_instruction(const Instruction& o) const { return opcode == o.opcode && wide == o.wide && operands == o.operands; }

void Instruction::init_instruction(InstructionSequence& seq) {
    if (is_jump()) {
        destination = seq.pointer_by_rel_offset(operand(0));
    } else if (is_switch()) {
        const int pref = opcode == cc::opc_tableswitch ? 3 : 2;
        int len = operands_count() - pref;
        default_dest = seq.pointer_by_rel_offset(operand(0));
        int low = 0;
        if (opcode == cc::opc_lookupswitch) len /= 2;
        else low = operand(1);
        destinations.assign(static_cast<std::size_t>(std::max(0, len)), 0);
        values.assign(static_cast<std::size_t>(std::max(0, len)), 0);
        for (int i = 0, k = 0; i < len; ++i, ++k) {
            if (opcode == cc::opc_lookupswitch) {
                values[static_cast<std::size_t>(i)] = operand(pref + k);
                ++k;
            } else {
                values[static_cast<std::size_t>(i)] = low + k;
            }
            destinations[static_cast<std::size_t>(i)] = seq.pointer_by_rel_offset(operand(pref + k));
        }
    }
}

std::string Instruction::to_string() const {
    std::string res = wide ? "@wide " : "";
    res += "@" + std::string(opcode_name(opcode));
    for (const int op : operands) res += op < 0 ? " -" + it_hex(-op) : " " + it_hex(op);
    return res;
}

// ---- InstructionSequence

std::shared_ptr<InstructionSequence> InstructionSequence::full(KeyedList<std::shared_ptr<Instruction>, int> instructions, std::shared_ptr<ExceptionTable> table) {
    auto seq = std::make_shared<InstructionSequence>();
    seq->instructions_ = std::move(instructions);
    seq->exception_table_ = std::move(table);
    for (const std::shared_ptr<ExceptionHandler>& h : seq->exception_table_->handlers) {
        h->from_instr = seq->pointer_by_abs_offset(h->from);
        h->to_instr = seq->pointer_by_abs_offset(h->to);
        h->handler_instr = seq->pointer_by_abs_offset(h->handler);
    }
    return seq;
}

std::shared_ptr<InstructionSequence> InstructionSequence::clone() const {
    auto seq = std::make_shared<InstructionSequence>();
    seq->instructions_ = instructions_;
    seq->pointer_ = pointer_;
    return seq;
}

void InstructionSequence::clear() {
    instructions_.clear();
    pointer_ = 0;
    exception_table_ = std::make_shared<ExceptionTable>();
}

void InstructionSequence::add_sequence(const InstructionSequence& seq) {
    for (std::size_t i = 0; i < seq.length(); ++i) add_instruction(seq.instr(i), -1);
}

int InstructionSequence::pointer_by_abs_offset(int offset) const {
    return instructions_.contains_key(offset) ? static_cast<int>(instructions_.index_by_key(offset)) : -1;
}

int InstructionSequence::pointer_by_rel_offset(int offset) const {
    const int abs = *instructions_.key(static_cast<std::size_t>(pointer_)) + offset;
    return instructions_.contains_key(abs) ? static_cast<int>(instructions_.index_by_key(abs)) : -1;
}

void InstructionSequence::set_pointer_by_abs_offset(int offset) {
    const int abs = *instructions_.key(static_cast<std::size_t>(pointer_)) + offset;
    if (instructions_.contains_key(abs)) pointer_ = static_cast<int>(instructions_.index_by_key(abs));
}

std::string InstructionSequence::to_string(int indent) const {
    std::string buf;
    for (std::size_t i = 0; i < instructions_.size(); ++i) {
        for (int k = 0; k < indent; ++k) buf += "   ";
        buf += std::to_string(offset(i)) + ": " + instructions_[i]->to_string() + "\n";
    }
    return buf;
}

// ---- parsing a method's code

std::shared_ptr<InstructionSequence> parse_bytecode(const StructMethod& method) {
    const StructClass& cls = method.class_struct();
    const ConstantPool& pool = cls.pool();
    const classfile::Code& code = *method.code();
    const int bytecode_version = cls.bytecode_version();
    io::ByteReader in(code.bytes, "class.Code.code");
    KeyedList<std::shared_ptr<Instruction>, int> instructions;
    const int length = static_cast<int>(code.bytes.size());
    for (int i = 0; i < length;) {
        const int offset = i;
        int opcode = in.u8();
        int group = cc::GROUP_GENERAL;
        const bool wide = opcode == cc::opc_wide;
        if (wide) {
            ++i;
            opcode = in.u8();
        }
        std::vector<int> operands;
        if (opcode >= cc::opc_iconst_m1 && opcode <= cc::opc_iconst_5) {
            operands.push_back(kItIconst[opcode - cc::opc_iconst_m1]);
            opcode = cc::opc_bipush;
        } else if (opcode >= cc::opc_iload_0 && opcode <= cc::opc_aload_3) {
            operands.push_back(kItLoadStore[opcode - cc::opc_iload_0]);
            opcode = kItLoads[(opcode - cc::opc_iload_0) / 4];
        } else if (opcode >= cc::opc_istore_0 && opcode <= cc::opc_astore_3) {
            operands.push_back(kItLoadStore[opcode - cc::opc_istore_0]);
            opcode = kItStores[(opcode - cc::opc_istore_0) / 4];
        } else {
            switch (opcode) {
                case cc::opc_bipush:
                    operands.push_back(static_cast<std::int8_t>(in.u8()));
                    ++i;
                    break;
                case cc::opc_ldc:
                case cc::opc_newarray:
                    operands.push_back(in.u8());
                    ++i;
                    break;
                case cc::opc_sipush:
                case cc::opc_ifeq:
                case cc::opc_ifne:
                case cc::opc_iflt:
                case cc::opc_ifge:
                case cc::opc_ifgt:
                case cc::opc_ifle:
                case cc::opc_if_icmpeq:
                case cc::opc_if_icmpne:
                case cc::opc_if_icmplt:
                case cc::opc_if_icmpge:
                case cc::opc_if_icmpgt:
                case cc::opc_if_icmple:
                case cc::opc_if_acmpeq:
                case cc::opc_if_acmpne:
                case cc::opc_goto:
                case cc::opc_jsr:
                case cc::opc_ifnull:
                case cc::opc_ifnonnull:
                    if (opcode != cc::opc_sipush) group = cc::GROUP_JUMP;
                    operands.push_back(in.i16_be());
                    i += 2;
                    break;
                case cc::opc_ldc_w:
                case cc::opc_ldc2_w:
                case cc::opc_getstatic:
                case cc::opc_putstatic:
                case cc::opc_getfield:
                case cc::opc_putfield:
                case cc::opc_invokevirtual:
                case cc::opc_invokespecial:
                case cc::opc_invokestatic:
                case cc::opc_new:
                case cc::opc_anewarray:
                case cc::opc_checkcast:
                case cc::opc_instanceof:
                    operands.push_back(in.u16_be());
                    i += 2;
                    if (opcode >= cc::opc_getstatic && opcode <= cc::opc_putfield) group = cc::GROUP_FIELDACCESS;
                    else if (opcode >= cc::opc_invokevirtual && opcode <= cc::opc_invokestatic) group = cc::GROUP_INVOCATION;
                    break;
                case cc::opc_invokedynamic:
                    if (cls.is_version_ge_1_7()) {   // (unused in Java 6 and before)
                        operands.push_back(in.u16_be());
                        (void)in.bytes(2);
                        group = cc::GROUP_INVOCATION;
                        i += 4;
                    }
                    break;
                case cc::opc_iload:
                case cc::opc_lload:
                case cc::opc_fload:
                case cc::opc_dload:
                case cc::opc_aload:
                case cc::opc_istore:
                case cc::opc_lstore:
                case cc::opc_fstore:
                case cc::opc_dstore:
                case cc::opc_astore:
                case cc::opc_ret:
                    if (wide) {
                        operands.push_back(in.u16_be());
                        i += 2;
                    } else {
                        operands.push_back(in.u8());
                        ++i;
                    }
                    if (opcode == cc::opc_ret) group = cc::GROUP_RETURN;
                    break;
                case cc::opc_iinc:
                    if (wide) {
                        operands.push_back(in.u16_be());
                        operands.push_back(in.i16_be());
                        i += 4;
                    } else {
                        operands.push_back(in.u8());
                        operands.push_back(static_cast<std::int8_t>(in.u8()));
                        i += 2;
                    }
                    break;
                case cc::opc_goto_w:
                case cc::opc_jsr_w:
                    opcode = opcode == cc::opc_jsr_w ? cc::opc_jsr : cc::opc_goto;
                    operands.push_back(in.i32_be());
                    group = cc::GROUP_JUMP;
                    i += 4;
                    break;
                case cc::opc_invokeinterface:
                    operands.push_back(in.u16_be());
                    operands.push_back(in.u8());
                    (void)in.bytes(1);
                    group = cc::GROUP_INVOCATION;
                    i += 4;
                    break;
                case cc::opc_multianewarray:
                    operands.push_back(in.u16_be());
                    operands.push_back(in.u8());
                    i += 3;
                    break;
                case cc::opc_tableswitch: {
                    const int pad = (4 - (i + 1) % 4) % 4;
                    (void)in.bytes(static_cast<std::size_t>(pad));
                    i += pad;
                    operands.push_back(in.i32_be());
                    i += 4;
                    const int low = in.i32_be();
                    operands.push_back(low);
                    i += 4;
                    const int high = in.i32_be();
                    operands.push_back(high);
                    i += 4;
                    for (int j = 0; j < high - low + 1; ++j) {
                        operands.push_back(in.i32_be());
                        i += 4;
                    }
                    group = cc::GROUP_SWITCH;
                    break;
                }
                case cc::opc_lookupswitch: {
                    const int pad = (4 - (i + 1) % 4) % 4;
                    (void)in.bytes(static_cast<std::size_t>(pad));
                    i += pad;
                    operands.push_back(in.i32_be());
                    i += 4;
                    const int npairs = in.i32_be();
                    operands.push_back(npairs);
                    i += 4;
                    for (int j = 0; j < npairs; ++j) {
                        operands.push_back(in.i32_be());
                        i += 4;
                        operands.push_back(in.i32_be());
                        i += 4;
                    }
                    group = cc::GROUP_SWITCH;
                    break;
                }
                case cc::opc_ireturn:
                case cc::opc_lreturn:
                case cc::opc_freturn:
                case cc::opc_dreturn:
                case cc::opc_areturn:
                case cc::opc_return:
                case cc::opc_athrow: group = cc::GROUP_RETURN; break;
                default: break;
            }
        }
        instructions.add_with_key(Instruction::make(opcode, wide, group, bytecode_version, std::move(operands)), offset);
        ++i;
    }
    // the exception table
    auto table = std::make_shared<ExceptionTable>();
    for (const classfile::ExceptionEntry& e : code.exceptions) {
        auto h = std::make_shared<ExceptionHandler>();
        h->from = e.start;
        h->to = e.end;
        h->handler = e.handler;
        h->class_index = e.catch_type;
        if (e.catch_type != 0) {
            const PrimitiveConstant* c = pool.primitive(e.catch_type);
            h->exception_class = c != nullptr ? c->get_string() : std::string();
        }
        table->handlers.push_back(std::move(h));
    }
    std::shared_ptr<InstructionSequence> seq = InstructionSequence::full(std::move(instructions), std::move(table));
    // the jumps and switches resolved to instruction indexes (from the last, as the original walks them)
    int i = static_cast<int>(seq->length()) - 1;
    seq->set_pointer(i);
    while (i >= 0) {
        const std::shared_ptr<Instruction>& instr = seq->instr(static_cast<std::size_t>(i--));
        if (instr->group != cc::GROUP_GENERAL) instr->init_instruction(*seq);
        seq->add_to_pointer(-1);
    }
    return seq;
}

// ---- the interpreter

void DataPoint::set_variable(int index, const VarType& value) {
    if (index < 0) throw std::out_of_range("DataPoint variable");
    while (static_cast<int>(local_variables.size()) <= index) local_variables.push_back(VarType(cc::TYPE_NOTINITIALIZED));
    local_variables[static_cast<std::size_t>(index)] = value;
}

VarType DataPoint::variable(int index) const {
    if (index < 0) throw std::out_of_range("DataPoint variable");
    if (index < static_cast<int>(local_variables.size())) return local_variables[static_cast<std::size_t>(index)];
    return VarType(cc::TYPE_NOTINITIALIZED);
}

DataPoint DataPoint::initial(const StructMethod& method) {
    DataPoint point;
    const MethodDescriptor md = MethodDescriptor::parse(method.descriptor());
    int k = 0;
    if (!method.has_modifier(cc::ACC_STATIC)) point.set_variable(k++, VarType(cc::TYPE_OBJECT, 0, std::nullopt));
    for (const VarType& var : md.params) {
        point.set_variable(k++, var);
        if (var.stack_size == 2) point.set_variable(k++, VarType(cc::TYPE_GROUP2EMPTY));
    }
    return point;
}

void step_types(DataPoint& data, const Instruction& instr, const ConstantPool& pool) {
    ListStack<VarType>& stack = data.stack;
    const ItImpact& row = kItImpact[instr.opcode];
    if (!row.special) {
        if (row.has_read) {
            int depth = 0;
            for (int i = 0; i < row.nread; ++i) {
                ++depth;
                if (row.read[i] == cc::TYPE_LONG || row.read[i] == cc::TYPE_DOUBLE) ++depth;
            }
            stack.remove_multiple(depth);
        }
        if (row.has_write)
            for (int i = 0; i < row.nwrite; ++i) {
                stack.push(VarType(row.write[i]));
                if (row.write[i] == cc::TYPE_LONG || row.write[i] == cc::TYPE_DOUBLE) stack.push(VarType(cc::TYPE_GROUP2EMPTY));
            }
        return;
    }
    const auto class_name = [&pool](int index) {
        const PrimitiveConstant* c = pool.primitive(static_cast<std::size_t>(index));
        if (c == nullptr) throw std::runtime_error("no class constant");
        return c;
    };
    switch (instr.opcode) {
        case cc::opc_aconst_null: stack.push(VarType(cc::TYPE_NULL, 0, std::nullopt)); break;
        case cc::opc_ldc:
        case cc::opc_ldc_w:
        case cc::opc_ldc2_w: {
            const PooledConstant* constant = pool.constant(static_cast<std::size_t>(instr.operand(0)));
            if (constant == nullptr) throw std::runtime_error("ldc of no constant");
            switch (constant->type) {
                case cc::CONSTANT_Integer: stack.push(VarType(cc::TYPE_INT)); break;
                case cc::CONSTANT_Float: stack.push(VarType(cc::TYPE_FLOAT)); break;
                case cc::CONSTANT_Long:
                    stack.push(VarType(cc::TYPE_LONG));
                    stack.push(VarType(cc::TYPE_GROUP2EMPTY));
                    break;
                case cc::CONSTANT_Double:
                    stack.push(VarType(cc::TYPE_DOUBLE));
                    stack.push(VarType(cc::TYPE_GROUP2EMPTY));
                    break;
                case cc::CONSTANT_String: stack.push(VarType(cc::TYPE_OBJECT, 0, "java/lang/String")); break;
                case cc::CONSTANT_Class: stack.push(VarType(cc::TYPE_OBJECT, 0, "java/lang/Class")); break;
                case cc::CONSTANT_MethodHandle: stack.push(VarType::parse(static_cast<const LinkConstant*>(constant)->descriptor)); break;
                default: break;
            }
            break;
        }
        case cc::opc_aload: stack.push(data.variable(instr.operand(0))); break;
        case cc::opc_aaload: {
            const VarType var1 = *stack.pop(2);
            stack.push(VarType(var1.type, var1.array_dim - 1, var1.value));
            break;
        }
        case cc::opc_astore: data.set_variable(instr.operand(0), stack.pop()); break;
        case cc::opc_dup:
        case cc::opc_dup_x1:
        case cc::opc_dup_x2: {
            const int depth = 88 - instr.opcode;
            stack.insert_by_offset(depth, stack.get_by_offset(-1).copy());
            break;
        }
        case cc::opc_dup2:
        case cc::opc_dup2_x1:
        case cc::opc_dup2_x2: {
            const int depth = 90 - instr.opcode;
            stack.insert_by_offset(depth, stack.get_by_offset(-2).copy());
            stack.insert_by_offset(depth, stack.get_by_offset(-1).copy());
            break;
        }
        case cc::opc_swap: {
            const VarType var1 = stack.pop();
            stack.insert_by_offset(-1, var1);
            break;
        }
        case cc::opc_getfield:
        case cc::opc_getstatic: {
            if (instr.opcode == cc::opc_getfield) stack.pop();
            const LinkConstant* ck = pool.link(static_cast<std::size_t>(instr.operand(0)));
            if (ck == nullptr) throw std::runtime_error("no field reference");
            const VarType var1 = VarType::parse(ck->descriptor);
            stack.push(var1);
            if (var1.stack_size == 2) stack.push(VarType(cc::TYPE_GROUP2EMPTY));
            break;
        }
        case cc::opc_putfield:
        case cc::opc_putstatic: {
            if (instr.opcode == cc::opc_putfield) stack.pop();
            const LinkConstant* ck = pool.link(static_cast<std::size_t>(instr.operand(0)));
            if (ck == nullptr) throw std::runtime_error("no field reference");
            stack.pop(VarType::parse(ck->descriptor).stack_size);
            break;
        }
        case cc::opc_invokevirtual:
        case cc::opc_invokespecial:
        case cc::opc_invokeinterface:
        case cc::opc_invokestatic:
        case cc::opc_invokedynamic: {
            if (instr.opcode == cc::opc_invokevirtual || instr.opcode == cc::opc_invokespecial || instr.opcode == cc::opc_invokeinterface) stack.pop();
            if (instr.opcode != cc::opc_invokedynamic || instr.bytecode_version >= cc::BYTECODE_JAVA_7) {
                const LinkConstant* ck = pool.link(static_cast<std::size_t>(instr.operand(0)));
                if (ck == nullptr) throw std::runtime_error("no method reference");
                const MethodDescriptor md = MethodDescriptor::parse(ck->descriptor);
                for (const VarType& p : md.params) stack.pop(p.stack_size);
                if (md.ret.type != cc::TYPE_VOID) {
                    stack.push(md.ret);
                    if (md.ret.stack_size == 2) stack.push(VarType(cc::TYPE_GROUP2EMPTY));
                }
            }
            break;
        }
        case cc::opc_new: stack.push(VarType(cc::TYPE_OBJECT, 0, class_name(instr.operand(0))->get_string())); break;
        case cc::opc_newarray:
            stack.pop();
            stack.push(VarType(kItArrayType[instr.operand(0) - 4], 1).resize_array_dim(1));
            break;
        case cc::opc_athrow: {
            const VarType var1 = stack.pop();
            stack.clear();
            stack.push(var1);
            break;
        }
        case cc::opc_checkcast:
        case cc::opc_instanceof:
            stack.pop();
            stack.push(VarType(cc::TYPE_OBJECT, 0, class_name(instr.operand(0))->get_string()));
            break;
        case cc::opc_anewarray:
        case cc::opc_multianewarray: {
            const int dimensions = instr.opcode == cc::opc_anewarray ? 1 : instr.operand(1);
            stack.pop(dimensions);
            const PrimitiveConstant* cn = class_name(instr.operand(0));
            if (cn->is_array) {
                VarType var1(cc::TYPE_OBJECT, 0, cn->get_string());
                stack.push(var1.resize_array_dim(var1.array_dim + dimensions));
            } else {
                stack.push(VarType(cc::TYPE_OBJECT, dimensions, cn->get_string()));
            }
            break;
        }
        default: break;
    }
}

bool instance_of(const StructContext& context, const std::string& value_class, const std::string& ref_class) {
    if (value_class == ref_class) return true;
    const StructClass* cl = context.get_class(value_class);
    if (cl == nullptr) return false;
    if (cl->super_class != nullptr && instance_of(context, cl->super_class->get_string(), ref_class)) return true;
    for (const int i : cl->interfaces()) {
        const PrimitiveConstant* c = cl->pool().primitive(static_cast<std::size_t>(i));
        if (c != nullptr && instance_of(context, c->get_string(), ref_class)) return true;
    }
    return false;
}

}  // namespace cb::java::decompiler
