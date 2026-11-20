// java/decompiler/expr_processor.cpp -- see expr_processor.hpp. Ported from Fernflower (de.fernflower.modules.decompiler.ExprProcessor), Copyright
// 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <set>
#include <stdexcept>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {

const VarType* const consts[] = {&VarType::INT, &VarType::FLOAT, &VarType::LONG, &VarType::DOUBLE, &VarType::CLASS, &VarType::STRING};
const VarType* const vartypes[] = {&VarType::INT, &VarType::LONG, &VarType::FLOAT, &VarType::DOUBLE, &VarType::OBJECT};
const VarType* const arrtypes[] = {&VarType::INT, &VarType::LONG, &VarType::FLOAT, &VarType::DOUBLE, &VarType::OBJECT, &VarType::BOOLEAN, &VarType::CHAR, &VarType::SHORT};

const int func1[] = {FunctionExprent::FUNCTION_ADD, FunctionExprent::FUNCTION_SUB, FunctionExprent::FUNCTION_MUL, FunctionExprent::FUNCTION_DIV,
                     FunctionExprent::FUNCTION_REM};
const int func2[] = {FunctionExprent::FUNCTION_SHL, FunctionExprent::FUNCTION_SHR, FunctionExprent::FUNCTION_USHR,
                     FunctionExprent::FUNCTION_AND, FunctionExprent::FUNCTION_OR,  FunctionExprent::FUNCTION_XOR};
const int func3[] = {FunctionExprent::FUNCTION_I2L, FunctionExprent::FUNCTION_I2F, FunctionExprent::FUNCTION_I2D, FunctionExprent::FUNCTION_L2I,
                     FunctionExprent::FUNCTION_L2F, FunctionExprent::FUNCTION_L2D, FunctionExprent::FUNCTION_F2I, FunctionExprent::FUNCTION_F2L,
                     FunctionExprent::FUNCTION_F2D, FunctionExprent::FUNCTION_D2I, FunctionExprent::FUNCTION_D2L, FunctionExprent::FUNCTION_D2F,
                     FunctionExprent::FUNCTION_I2B, FunctionExprent::FUNCTION_I2C, FunctionExprent::FUNCTION_I2S};
const int func4[] = {FunctionExprent::FUNCTION_LCMP, FunctionExprent::FUNCTION_FCMPL, FunctionExprent::FUNCTION_FCMPG, FunctionExprent::FUNCTION_DCMPL,
                     FunctionExprent::FUNCTION_DCMPG};
const int func5[] = {IfExprent::IF_EQ, IfExprent::IF_NE, IfExprent::IF_LT, IfExprent::IF_GE, IfExprent::IF_GT, IfExprent::IF_LE};
const int func6[] = {IfExprent::IF_ICMPEQ, IfExprent::IF_ICMPNE, IfExprent::IF_ICMPLT, IfExprent::IF_ICMPGE,
                     IfExprent::IF_ICMPGT, IfExprent::IF_ICMPLE, IfExprent::IF_ACMPEQ, IfExprent::IF_ACMPNE};
const int func7[] = {IfExprent::IF_NULL, IfExprent::IF_NONNULL};
const int func8[] = {MonitorExprent::MONITOR_ENTER, MonitorExprent::MONITOR_EXIT};
const int arr_type[] = {cc::TYPE_BOOLEAN, cc::TYPE_CHAR, cc::TYPE_FLOAT, cc::TYPE_DOUBLE, cc::TYPE_BYTE, cc::TYPE_SHORT, cc::TYPE_INT, cc::TYPE_LONG};
const int negifs[] = {IfExprent::IF_NE,     IfExprent::IF_EQ,     IfExprent::IF_GE,     IfExprent::IF_LT,     IfExprent::IF_LE,
                      IfExprent::IF_GT,     IfExprent::IF_NONNULL, IfExprent::IF_NULL,   IfExprent::IF_ICMPNE, IfExprent::IF_ICMPEQ,
                      IfExprent::IF_ICMPGE, IfExprent::IF_ICMPLT, IfExprent::IF_ICMPLE, IfExprent::IF_ICMPGT, IfExprent::IF_ACMPNE,
                      IfExprent::IF_ACMPEQ};
const char* const type_names[] = {"byte", "char", "double", "float", "int", "long", "short", "boolean"};

// opcode -> function (arraylength, checkcast, instanceof)
int map_consts(int opcode) {
    switch (opcode) {
        case cc::opc_arraylength: return FunctionExprent::FUNCTION_ARRAY_LENGTH;
        case cc::opc_checkcast: return FunctionExprent::FUNCTION_CAST;
        case cc::opc_instanceof: return FunctionExprent::FUNCTION_INSTANCEOF;
        default: throw std::runtime_error("no function for the opcode");
    }
}

}  // namespace

void ExprProcessor::process_statement(RootStatement* root, const StructClass* cl) {
    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    // collect finally entry points
    std::set<std::string> set_finally_short_range_entry_points;
    for (const auto& entry : dgraph->map_short_range_finally_paths) {
        for (const FinallyPathWrapper& finwrap : entry.second) set_finally_short_range_entry_points.insert(finwrap.entry);
    }

    std::set<std::string> set_finally_long_range_entry_paths;
    for (const auto& entry : dgraph->map_long_range_finally_paths) {
        for (const FinallyPathWrapper& finwrap : entry.second) set_finally_long_range_entry_paths.insert(finwrap.source + "##" + finwrap.entry);
    }

    std::map<std::string, VarExprent*> map_catch;
    collect_catch_vars(root, flatthelper, map_catch);

    std::map<DirectNode*, std::map<std::optional<std::string>, PrimitiveExprsList>> map_data;

    std::deque<DirectNode*> stack;
    std::deque<std::deque<std::string>> stack_entry_point;

    stack.push_back(dgraph->first);
    stack_entry_point.emplace_back();

    std::map<std::optional<std::string>, PrimitiveExprsList> first_map;
    first_map.emplace(std::nullopt, PrimitiveExprsList());
    map_data.emplace(dgraph->first, std::move(first_map));

    while (!stack.empty()) {
        DirectNode* node = stack.front();
        stack.pop_front();
        std::deque<std::string> entrypoints = std::move(stack_entry_point.front());
        stack_entry_point.pop_front();

        PrimitiveExprsList catch_data;
        PrimitiveExprsList* data;
        const auto catch_it = map_catch.find(node->id);
        if (catch_it != map_catch.end()) {
            catch_data = get_expression_data(catch_it->second);
            data = &catch_data;
        } else {
            data = &map_data.at(node).at(build_entry_point_key(entrypoints));
        }

        BasicBlockStatement* block = node->block;
        if (block != nullptr) {
            process_block(block, *data, cl);
            block->set_exprents(data->get_lst_exprents());
        }

        const std::optional<std::string> current_entrypoint = entrypoints.empty() ? std::nullopt : std::optional<std::string>(entrypoints.back());

        for (DirectNode* nd : node->succs) {
            bool is_successor = true;

            if (current_entrypoint && dgraph->map_long_range_finally_paths.contains(node->id)) {
                is_successor = false;

                for (const FinallyPathWrapper& finwraplong : dgraph->map_long_range_finally_paths.at(node->id)) {
                    if (finwraplong.source == *current_entrypoint && finwraplong.destination == nd->id) {
                        is_successor = true;
                        break;
                    }
                }
            }

            if (is_successor) {
                std::map<std::optional<std::string>, PrimitiveExprsList>& map_succ = map_data[nd];

                std::deque<std::string> ndentrypoints = entrypoints;

                if (set_finally_long_range_entry_paths.contains(node->id + "##" + nd->id)) {
                    ndentrypoints.push_back(node->id);
                } else if (!set_finally_short_range_entry_points.contains(nd->id) && dgraph->map_long_range_finally_paths.contains(node->id)) {
                    if (ndentrypoints.empty()) throw std::runtime_error("NoSuchElementException");
                    ndentrypoints.pop_back();   // currentEntrypoint should not be null at this point
                }

                // handling of entry point loops
                int succ_entry_index = -1;
                for (std::size_t k = 0; k < ndentrypoints.size(); k++) {
                    if (ndentrypoints[k] == nd->id) {
                        succ_entry_index = static_cast<int>(k);
                        break;
                    }
                }
                if (succ_entry_index >= 0) {   // we are in a loop (e.g. continue in a finally block), drop all entry points in the list beginning with succ_entry_index
                    for (int elements_to_remove = static_cast<int>(ndentrypoints.size()) - succ_entry_index; elements_to_remove > 0; elements_to_remove--) {
                        ndentrypoints.pop_back();
                    }
                }

                const std::optional<std::string> ndentrykey = build_entry_point_key(ndentrypoints);
                if (!map_succ.contains(ndentrykey)) {
                    map_succ.emplace(ndentrykey, copy_var_exprents(data->copy_stack()));

                    stack.push_back(nd);
                    stack_entry_point.push_back(std::move(ndentrypoints));
                }
            }
        }
    }

    init_statement_exprents(root);
}

// FIXME: Ugly code, to be rewritten. A tuple class is needed.
std::optional<std::string> ExprProcessor::build_entry_point_key(const std::deque<std::string>& entrypoints) {
    if (entrypoints.empty()) return std::nullopt;
    std::string buffer;
    for (const std::string& point : entrypoints) {
        buffer.append(point);
        buffer.append(":");
    }
    return buffer;
}

PrimitiveExprsList ExprProcessor::copy_var_exprents(PrimitiveExprsList data) {
    ExprentStack& stack = data.get_stack();
    copy_entries(stack);
    return data;
}

void ExprProcessor::copy_entries(std::vector<Exprent*>& stack) {
    for (std::size_t i = 0; i < stack.size(); i++) stack[i] = stack[i]->copy();
}

void ExprProcessor::copy_entries(ListStack<Exprent*>& stack) { copy_entries(stack.items); }

void ExprProcessor::collect_catch_vars(Statement* stat, FlattenStatementsHelper& flatthelper, std::map<std::string, VarExprent*>& map) {
    std::vector<VarExprent*>* lst = nullptr;

    if (stat->type == Statement::TYPE_CATCHALL) {
        CatchAllStatement* catchall = static_cast<CatchAllStatement*>(stat);
        if (!catchall->is_finally()) lst = &catchall->get_vars();
    } else if (stat->type == Statement::TYPE_TRYCATCH) {
        lst = &static_cast<CatchStatement*>(stat)->get_vars();
    }

    if (lst != nullptr) {
        for (std::size_t i = 1; i < stat->get_stats().size(); i++) {
            map[flatthelper.get_map_destination_nodes().at(stat->get_stats().get(i)->id)[0].value()] = lst->at(i - 1);
        }
    }

    for (Statement* st : stat->get_stats().items()) collect_catch_vars(st, flatthelper, map);
}

void ExprProcessor::init_statement_exprents(Statement* stat) {
    stat->init_exprents();

    for (Statement* st : stat->get_stats().items()) init_statement_exprents(st);
}

void ExprProcessor::process_block(BasicBlockStatement* stat, PrimitiveExprsList& data, const StructClass* cl) {
    const ConstantPool& pool = cl->pool();
    const BootstrapMethodsAttribute* bootstrap = cl->attribute<BootstrapMethodsAttribute>(attr::BOOTSTRAP_METHODS);

    BasicBlock* block = stat->get_block();

    ExprentStack& stack = data.get_stack();
    std::vector<Exprent*>& exprlist = data.get_lst_exprents();

    const InstructionSequence& seq = *block->seq;

    for (std::size_t i = 0; i < seq.length(); i++) {
        const Instruction& instr = *seq.instr(i);

        const int bytecode_offset = block->old_offset(i);
        Offsets bytecode_offsets;
        if (bytecode_offset >= 0) bytecode_offsets = std::set<int>{bytecode_offset};

        switch (instr.opcode) {
            case cc::opc_aconst_null:
                push_ex(stack, exprlist, make<ConstExprent>(VarType::NULL_TYPE, ConstExprent::Value{}, bytecode_offsets));
                break;
            case cc::opc_bipush:
            case cc::opc_sipush:
                push_ex(stack, exprlist, make<ConstExprent>(instr.operand(0), true, bytecode_offsets));
                break;
            case cc::opc_lconst_0:
            case cc::opc_lconst_1:
                push_ex(stack, exprlist, make<ConstExprent>(VarType::LONG, ConstExprent::Value(static_cast<std::int64_t>(instr.opcode - cc::opc_lconst_0)), bytecode_offsets));
                break;
            case cc::opc_fconst_0:
            case cc::opc_fconst_1:
            case cc::opc_fconst_2:
                push_ex(stack, exprlist, make<ConstExprent>(VarType::FLOAT, ConstExprent::Value(static_cast<float>(instr.opcode - cc::opc_fconst_0)), bytecode_offsets));
                break;
            case cc::opc_dconst_0:
            case cc::opc_dconst_1:
                push_ex(stack, exprlist, make<ConstExprent>(VarType::DOUBLE, ConstExprent::Value(static_cast<double>(instr.opcode - cc::opc_dconst_0)), bytecode_offsets));
                break;
            case cc::opc_ldc:
            case cc::opc_ldc_w:
            case cc::opc_ldc2_w: {
                const PooledConstant* cn = pool.constant(static_cast<std::size_t>(instr.operand(0)));
                if (const PrimitiveConstant* prim = dynamic_cast<const PrimitiveConstant*>(cn)) {
                    push_ex(stack, exprlist, make<ConstExprent>(*consts[prim->type - cc::CONSTANT_Integer], prim->value, bytecode_offsets));
                } else if (const LinkConstant* link = dynamic_cast<const LinkConstant*>(cn)) {
                    // TODO: for now treat Links as Strings
                    push_ex(stack, exprlist, make<ConstExprent>(VarType::STRING, ConstExprent::Value(link->elementname), bytecode_offsets));
                }
                break;
            }
            case cc::opc_iload:
            case cc::opc_lload:
            case cc::opc_fload:
            case cc::opc_dload:
            case cc::opc_aload:
                push_ex(stack, exprlist, make<VarExprent>(instr.operand(0), *vartypes[instr.opcode - cc::opc_iload], var_processor_));
                break;
            case cc::opc_iaload:
            case cc::opc_laload:
            case cc::opc_faload:
            case cc::opc_daload:
            case cc::opc_aaload:
            case cc::opc_baload:
            case cc::opc_caload:
            case cc::opc_saload: {
                Exprent* index = stack.pop();
                Exprent* arr = stack.pop();

                std::optional<VarType> vartype;
                switch (instr.opcode) {
                    case cc::opc_laload:
                        vartype = VarType::LONG;
                        break;
                    case cc::opc_daload:
                        vartype = VarType::DOUBLE;
                        break;
                    default:
                        break;
                }

                push_ex(stack, exprlist, make<ArrayExprent>(arr, index, *arrtypes[instr.opcode - cc::opc_iaload], bytecode_offsets), vartype);
                break;
            }
            case cc::opc_istore:
            case cc::opc_lstore:
            case cc::opc_fstore:
            case cc::opc_dstore:
            case cc::opc_astore: {
                Exprent* top = stack.pop();
                const int varindex = instr.operand(0);
                VarExprent* target = make<VarExprent>(varindex, *vartypes[instr.opcode - cc::opc_istore], var_processor_);
                AssignmentExprent* assign = make<AssignmentExprent>(target, top, bytecode_offsets);
                exprlist.push_back(assign);
                break;
            }
            case cc::opc_iastore:
            case cc::opc_lastore:
            case cc::opc_fastore:
            case cc::opc_dastore:
            case cc::opc_aastore:
            case cc::opc_bastore:
            case cc::opc_castore:
            case cc::opc_sastore: {
                Exprent* value = stack.pop();
                Exprent* index_store = stack.pop();
                Exprent* arr_store = stack.pop();
                ArrayExprent* target = make<ArrayExprent>(arr_store, index_store, *arrtypes[instr.opcode - cc::opc_iastore], bytecode_offsets);
                AssignmentExprent* arrassign = make<AssignmentExprent>(target, value, bytecode_offsets);
                exprlist.push_back(arrassign);
                break;
            }
            case cc::opc_iadd:
            case cc::opc_ladd:
            case cc::opc_fadd:
            case cc::opc_dadd:
            case cc::opc_isub:
            case cc::opc_lsub:
            case cc::opc_fsub:
            case cc::opc_dsub:
            case cc::opc_imul:
            case cc::opc_lmul:
            case cc::opc_fmul:
            case cc::opc_dmul:
            case cc::opc_idiv:
            case cc::opc_ldiv:
            case cc::opc_fdiv:
            case cc::opc_ddiv:
            case cc::opc_irem:
            case cc::opc_lrem:
            case cc::opc_frem:
            case cc::opc_drem:
                push_ex(stack, exprlist, make<FunctionExprent>(func1[(instr.opcode - cc::opc_iadd) / 4], stack, bytecode_offsets));
                break;
            case cc::opc_ishl:
            case cc::opc_lshl:
            case cc::opc_ishr:
            case cc::opc_lshr:
            case cc::opc_iushr:
            case cc::opc_lushr:
            case cc::opc_iand:
            case cc::opc_land:
            case cc::opc_ior:
            case cc::opc_lor:
            case cc::opc_ixor:
            case cc::opc_lxor:
                push_ex(stack, exprlist, make<FunctionExprent>(func2[(instr.opcode - cc::opc_ishl) / 2], stack, bytecode_offsets));
                break;
            case cc::opc_ineg:
            case cc::opc_lneg:
            case cc::opc_fneg:
            case cc::opc_dneg:
                push_ex(stack, exprlist, make<FunctionExprent>(FunctionExprent::FUNCTION_NEG, stack, bytecode_offsets));
                break;
            case cc::opc_iinc: {
                VarExprent* vevar = make<VarExprent>(instr.operand(0), VarType::INT, var_processor_);
                Exprent* var_copy = vevar->copy();
                ConstExprent* amount = make<ConstExprent>(VarType::INT, ConstExprent::Value(static_cast<std::int32_t>(std::abs(instr.operand(1)))), std::nullopt);
                FunctionExprent* func =
                    make<FunctionExprent>(instr.operand(1) < 0 ? FunctionExprent::FUNCTION_SUB : FunctionExprent::FUNCTION_ADD,
                                          std::vector<Exprent*>{var_copy, amount}, bytecode_offsets);
                exprlist.push_back(make<AssignmentExprent>(vevar, func, bytecode_offsets));
                break;
            }
            case cc::opc_i2l:
            case cc::opc_i2f:
            case cc::opc_i2d:
            case cc::opc_l2i:
            case cc::opc_l2f:
            case cc::opc_l2d:
            case cc::opc_f2i:
            case cc::opc_f2l:
            case cc::opc_f2d:
            case cc::opc_d2i:
            case cc::opc_d2l:
            case cc::opc_d2f:
            case cc::opc_i2b:
            case cc::opc_i2c:
            case cc::opc_i2s:
                push_ex(stack, exprlist, make<FunctionExprent>(func3[instr.opcode - cc::opc_i2l], stack, bytecode_offsets));
                break;
            case cc::opc_lcmp:
            case cc::opc_fcmpl:
            case cc::opc_fcmpg:
            case cc::opc_dcmpl:
            case cc::opc_dcmpg:
                push_ex(stack, exprlist, make<FunctionExprent>(func4[instr.opcode - cc::opc_lcmp], stack, bytecode_offsets));
                break;
            case cc::opc_ifeq:
            case cc::opc_ifne:
            case cc::opc_iflt:
            case cc::opc_ifge:
            case cc::opc_ifgt:
            case cc::opc_ifle:
                exprlist.push_back(make<IfExprent>(negifs[func5[instr.opcode - cc::opc_ifeq]], stack, bytecode_offsets));
                break;
            case cc::opc_if_icmpeq:
            case cc::opc_if_icmpne:
            case cc::opc_if_icmplt:
            case cc::opc_if_icmpge:
            case cc::opc_if_icmpgt:
            case cc::opc_if_icmple:
            case cc::opc_if_acmpeq:
            case cc::opc_if_acmpne:
                exprlist.push_back(make<IfExprent>(negifs[func6[instr.opcode - cc::opc_if_icmpeq]], stack, bytecode_offsets));
                break;
            case cc::opc_ifnull:
            case cc::opc_ifnonnull:
                exprlist.push_back(make<IfExprent>(negifs[func7[instr.opcode - cc::opc_ifnull]], stack, bytecode_offsets));
                break;
            case cc::opc_tableswitch:
            case cc::opc_lookupswitch: {
                Exprent* switch_value = stack.pop();
                exprlist.push_back(make<SwitchExprent>(switch_value, bytecode_offsets));
                break;
            }
            case cc::opc_ireturn:
            case cc::opc_lreturn:
            case cc::opc_freturn:
            case cc::opc_dreturn:
            case cc::opc_areturn:
            case cc::opc_return:
            case cc::opc_athrow: {
                Exprent* exit_value = instr.opcode == cc::opc_return ? nullptr : stack.pop();
                VarType ret_type;   // (the original's null for a throw)
                if (instr.opcode != cc::opc_athrow) ret_type = DecompilerContext::current().current_method_descriptor.value().ret;
                exprlist.push_back(make<ExitExprent>(instr.opcode == cc::opc_athrow ? ExitExprent::EXIT_THROW : ExitExprent::EXIT_RETURN, exit_value, ret_type,
                                                     bytecode_offsets));
                break;
            }
            case cc::opc_monitorenter:
            case cc::opc_monitorexit: {
                Exprent* monitor_value = stack.pop();
                exprlist.push_back(make<MonitorExprent>(func8[instr.opcode - cc::opc_monitorenter], monitor_value, bytecode_offsets));
                break;
            }
            case cc::opc_checkcast:
            case cc::opc_instanceof:
                stack.push(make<ConstExprent>(VarType::parse(pool.primitive(static_cast<std::size_t>(instr.operand(0)))->get_string(), true), ConstExprent::Value{},
                                              std::nullopt));
                [[fallthrough]];
            case cc::opc_arraylength:
                push_ex(stack, exprlist, make<FunctionExprent>(map_consts(instr.opcode), stack, bytecode_offsets));
                break;
            case cc::opc_getstatic:
            case cc::opc_getfield: {
                Exprent* instance = instr.opcode == cc::opc_getstatic ? nullptr : stack.pop();
                push_ex(stack, exprlist, make<FieldExprent>(*pool.link(static_cast<std::size_t>(instr.operand(0))), instance, bytecode_offsets));
                break;
            }
            case cc::opc_putstatic:
            case cc::opc_putfield: {
                Exprent* valfield = stack.pop();
                Exprent* instance = instr.opcode == cc::opc_putstatic ? nullptr : stack.pop();
                Exprent* exprfield = make<FieldExprent>(*pool.link(static_cast<std::size_t>(instr.operand(0))), instance, bytecode_offsets);
                exprlist.push_back(make<AssignmentExprent>(exprfield, valfield, bytecode_offsets));
                break;
            }
            case cc::opc_invokevirtual:
            case cc::opc_invokespecial:
            case cc::opc_invokestatic:
            case cc::opc_invokeinterface:
            case cc::opc_invokedynamic:
                if (instr.opcode != cc::opc_invokedynamic || instr.bytecode_version >= cc::BYTECODE_JAVA_7) {
                    const LinkConstant* invoke_constant = pool.link(static_cast<std::size_t>(instr.operand(0)));

                    int dynamic_invokation_type = -1;
                    if (instr.opcode == cc::opc_invokedynamic && bootstrap != nullptr) {
                        const std::vector<const PooledConstant*>& bootstrap_arguments = bootstrap->method_arguments(static_cast<std::size_t>(invoke_constant->index1));
                        const LinkConstant* content_method_handle = dynamic_cast<const LinkConstant*>(bootstrap_arguments.at(1));
                        if (content_method_handle == nullptr) throw std::runtime_error("ClassCastException");
                        dynamic_invokation_type = content_method_handle->index1;
                    }

                    InvocationExprent* exprinv = make<InvocationExprent>(instr.opcode, *invoke_constant, stack, dynamic_invokation_type, bytecode_offsets);

                    if (exprinv->get_descriptor().ret.type == cc::TYPE_VOID) exprlist.push_back(exprinv);
                    else push_ex(stack, exprlist, exprinv);
                }
                break;
            case cc::opc_new:
            case cc::opc_anewarray:
            case cc::opc_multianewarray: {
                const int dimensions = (instr.opcode == cc::opc_new) ? 0 : (instr.opcode == cc::opc_anewarray) ? 1 : instr.operand(1);
                VarType arr_type_v = VarType::parse(pool.primitive(static_cast<std::size_t>(instr.operand(0)))->get_string(), true);
                if (instr.opcode != cc::opc_multianewarray) arr_type_v = arr_type_v.resize_array_dim(arr_type_v.array_dim + dimensions);
                push_ex(stack, exprlist, make<NewExprent>(arr_type_v, stack, dimensions, bytecode_offsets));
                break;
            }
            case cc::opc_newarray:
                push_ex(stack, exprlist, make<NewExprent>(VarType(arr_type[instr.operand(0) - 4], 1), stack, 1, bytecode_offsets));
                break;
            case cc::opc_dup:
                push_ex(stack, exprlist, stack.get_by_offset(-1)->copy());
                break;
            case cc::opc_dup_x1:
                insert_by_offset_ex(-2, stack, exprlist, -1);
                break;
            case cc::opc_dup_x2:
                if (stack.get_by_offset(-2)->get_expr_type().stack_size == 2) insert_by_offset_ex(-2, stack, exprlist, -1);
                else insert_by_offset_ex(-3, stack, exprlist, -1);
                break;
            case cc::opc_dup2:
                if (stack.get_by_offset(-1)->get_expr_type().stack_size == 2) {
                    push_ex(stack, exprlist, stack.get_by_offset(-1)->copy());
                } else {
                    push_ex(stack, exprlist, stack.get_by_offset(-2)->copy());
                    push_ex(stack, exprlist, stack.get_by_offset(-2)->copy());
                }
                break;
            case cc::opc_dup2_x1:
                if (stack.get_by_offset(-1)->get_expr_type().stack_size == 2) {
                    insert_by_offset_ex(-2, stack, exprlist, -1);
                } else {
                    insert_by_offset_ex(-3, stack, exprlist, -2);
                    insert_by_offset_ex(-3, stack, exprlist, -1);
                }
                break;
            case cc::opc_dup2_x2:
                if (stack.get_by_offset(-1)->get_expr_type().stack_size == 2) {
                    if (stack.get_by_offset(-2)->get_expr_type().stack_size == 2) insert_by_offset_ex(-2, stack, exprlist, -1);
                    else insert_by_offset_ex(-3, stack, exprlist, -1);
                } else {
                    if (stack.get_by_offset(-3)->get_expr_type().stack_size == 2) {
                        insert_by_offset_ex(-3, stack, exprlist, -2);
                        insert_by_offset_ex(-3, stack, exprlist, -1);
                    } else {
                        insert_by_offset_ex(-4, stack, exprlist, -2);
                        insert_by_offset_ex(-4, stack, exprlist, -1);
                    }
                }
                break;
            case cc::opc_swap:
                insert_by_offset_ex(-2, stack, exprlist, -1);
                stack.pop();
                break;
            case cc::opc_pop:
            case cc::opc_pop2:
                stack.pop();
                break;
            default:
                break;
        }
    }
}

void ExprProcessor::push_ex(ExprentStack& stack, std::vector<Exprent*>& exprlist, Exprent* exprent) { push_ex(stack, exprlist, exprent, std::nullopt); }

void ExprProcessor::push_ex(ExprentStack& stack, std::vector<Exprent*>& exprlist, Exprent* exprent, const std::optional<VarType>& vartype) {
    const int varindex = VarExprent::STACK_BASE + stack.size();
    VarExprent* var = make<VarExprent>(varindex, !vartype ? exprent->get_expr_type() : *vartype, var_processor_);
    var->set_stack(true);

    exprlist.push_back(make<AssignmentExprent>(var, exprent, std::nullopt));
    stack.push(var->copy());
}

void ExprProcessor::insert_by_offset_ex(int offset, ExprentStack& stack, std::vector<Exprent*>& exprlist, int copyoffset) {
    const int base = VarExprent::STACK_BASE + stack.size();

    std::vector<Exprent*> lst;
    for (int i = -1; i >= offset; i--) {
        Exprent* varex = stack.pop();
        VarExprent* varnew = make<VarExprent>(base + i + 1, varex->get_expr_type(), var_processor_);
        varnew->set_stack(true);

        exprlist.push_back(make<AssignmentExprent>(varnew, varex, std::nullopt));
        lst.insert(lst.begin(), varnew->copy());
    }

    Exprent* exprent = lst.at(static_cast<std::size_t>(static_cast<int>(lst.size()) + copyoffset))->copy();
    VarExprent* var = make<VarExprent>(base + offset, exprent->get_expr_type(), var_processor_);
    var->set_stack(true);

    exprlist.push_back(make<AssignmentExprent>(var, exprent, std::nullopt));
    lst.insert(lst.begin(), var->copy());

    for (Exprent* expr : lst) stack.push(expr);
}

std::string ExprProcessor::get_type_name(const VarType& type) { return get_type_name(type, true); }

std::string ExprProcessor::get_type_name(const VarType& type, bool get_short) {
    const int tp = type.type;

    if (tp <= cc::TYPE_BOOLEAN) return type_names[tp];
    if (tp == cc::TYPE_UNKNOWN) return std::string(UNKNOWN_TYPE_STRING);   // INFO: should not occur
    if (tp == cc::TYPE_NULL) return std::string(NULL_TYPE_STRING);         // INFO: should not occur
    if (tp == cc::TYPE_VOID) return "void";
    if (tp == cc::TYPE_OBJECT) {
        std::string ret = build_java_class_name(type.value.value());
        if (get_short) ret = DecompilerContext::current().import_collector->get_short_name(ret);
        // the collector's "null" is the original's null (an anonymous class has no simple name; a class can never be named null)
        if (ret == "null") ret = std::string(UNDEFINED_TYPE_STRING);   // FIXME (the original's): a warning should be logged
        return ret;
    }

    throw std::runtime_error("invalid type");
}

std::string ExprProcessor::get_cast_type_name(const VarType& type) { return get_cast_type_name(type, true); }

std::string ExprProcessor::get_cast_type_name(const VarType& type, bool get_short) {
    std::string s = get_type_name(type, get_short);
    int dim = type.array_dim;
    while (dim-- > 0) s += "[]";
    return s;
}

PrimitiveExprsList ExprProcessor::get_expression_data(VarExprent* var) {
    PrimitiveExprsList prlst;
    VarExprent* vartmp = make<VarExprent>(VarExprent::STACK_BASE, var->get_expr_type(), var->get_processor());
    vartmp->set_stack(true);

    Exprent* var_copy = var->copy();
    prlst.get_lst_exprents().push_back(make<AssignmentExprent>(vartmp, var_copy, std::nullopt));
    prlst.get_stack().push(vartmp->copy());
    return prlst;
}

bool ExprProcessor::ends_with_semikolon(Exprent* expr) {
    const int type = expr->type;
    return !(type == Exprent::EXPRENT_SWITCH || type == Exprent::EXPRENT_MONITOR || type == Exprent::EXPRENT_IF ||
             (type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(expr)->is_class_def()));
}

TextBuffer ExprProcessor::jmp_wrapper(Statement* stat, int indent, bool semicolon, BytecodeMappingTracer& tracer) {
    TextBuffer buf = stat->to_java(indent, tracer);

    const std::vector<StatEdge*> lst_succs = stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
    if (lst_succs.size() == 1) {
        StatEdge* edge = lst_succs[0];
        if (edge->get_type() != StatEdge::TYPE_REGULAR && edge->explicit_ && edge->get_destination()->type != Statement::TYPE_DUMMYEXIT) {
            buf.append_indent(indent);

            switch (edge->get_type()) {
                case StatEdge::TYPE_BREAK:
                    buf.append("break");
                    break;
                case StatEdge::TYPE_CONTINUE:
                    buf.append("continue");
                    break;
                default:
                    break;
            }

            if (edge->labeled) buf.append(" label").append(std::to_string(edge->closure->id));
            buf.append(";").append_line_separator();
            tracer.increment_current_source_line();
        }
    }

    if (buf.length() == 0 && semicolon) {
        buf.append_indent(indent).append(";").append_line_separator();
        tracer.increment_current_source_line();
    }

    return buf;
}

std::string ExprProcessor::build_java_class_name(const std::string& name) {
    std::string res = name;
    std::replace(res.begin(), res.end(), '/', '.');

    if (res.find('$') != std::string::npos) {   // attempt to invoke foreign member classes correctly
        const StructClass* cl = DecompilerContext::current().struct_context->get_class(name);
        if (cl == nullptr || !cl->is_own()) std::replace(res.begin(), res.end(), '$', '.');
    }

    return res;
}

TextBuffer ExprProcessor::list_to_java(const std::optional<std::vector<Exprent*>>& lst, int indent, BytecodeMappingTracer& tracer) {
    if (!lst) return TextBuffer();
    return list_to_java(*lst, indent, tracer);
}

TextBuffer ExprProcessor::list_to_java(const std::vector<Exprent*>& lst, int indent, BytecodeMappingTracer& tracer) {
    if (lst.empty()) return TextBuffer();

    TextBuffer buf;

    for (Exprent* expr : lst) {
        TextBuffer content = expr->to_java(indent, tracer);

        if (content.length() > 0) {
            if (expr->type != Exprent::EXPRENT_VAR || !static_cast<VarExprent*>(expr)->is_class_def()) buf.append_indent(indent);

            buf.append(content);

            if (expr->type == Exprent::EXPRENT_MONITOR && static_cast<MonitorExprent*>(expr)->get_mon_type() == MonitorExprent::MONITOR_ENTER) {
                buf.append("{}");   // empty synchronized block
            }

            if (ends_with_semikolon(expr)) buf.append(";");

            buf.append_line_separator();
            tracer.increment_current_source_line();
        }
    }

    return buf;
}

ConstExprent* ExprProcessor::get_default_array_value(const VarType& arrtype) {
    ConstExprent* defaultval;
    if (arrtype.type == cc::TYPE_OBJECT || arrtype.array_dim > 0) {
        defaultval = make<ConstExprent>(VarType::NULL_TYPE, ConstExprent::Value{}, std::nullopt);
    } else if (arrtype.type == cc::TYPE_FLOAT) {
        defaultval = make<ConstExprent>(VarType::FLOAT, ConstExprent::Value(0.0f), std::nullopt);
    } else if (arrtype.type == cc::TYPE_LONG) {
        defaultval = make<ConstExprent>(VarType::LONG, ConstExprent::Value(std::int64_t{0}), std::nullopt);
    } else if (arrtype.type == cc::TYPE_DOUBLE) {
        defaultval = make<ConstExprent>(VarType::DOUBLE, ConstExprent::Value(0.0), std::nullopt);
    } else {   // integer types
        defaultval = make<ConstExprent>(0, true, std::nullopt);
    }
    return defaultval;
}

bool ExprProcessor::get_casted_exprent(Exprent* exprent, const VarType& left_type, TextBuffer& buffer, int indent, bool cast_null, BytecodeMappingTracer& tracer) {
    return get_casted_exprent(exprent, left_type, buffer, indent, cast_null, false, tracer);
}

bool ExprProcessor::get_casted_exprent(Exprent* exprent, const VarType& left_type, TextBuffer& buffer, int indent, bool cast_null, bool cast_always,
                                       BytecodeMappingTracer& tracer) {
    const VarType right_type = exprent->get_expr_type();

    TextBuffer res = exprent->to_java(indent, tracer);

    const bool cast = cast_always || (!left_type.is_superset(right_type) && (right_type == VarType::OBJECT || left_type.type != cc::TYPE_OBJECT)) ||
                      (cast_null && right_type.type == cc::TYPE_NULL && UNDEFINED_TYPE_STRING != get_type_name(left_type)) ||
                      (is_int_constant(exprent) && VarType::INT.is_strict_superset(left_type));

    if (cast) {
        if (exprent->get_precedence() >= FunctionExprent::get_precedence(FunctionExprent::FUNCTION_CAST)) res.enclose("(", ")");

        res.prepend("(" + get_cast_type_name(left_type) + ")");
    }

    buffer.append(res);

    return cast;
}

bool ExprProcessor::is_int_constant(Exprent* exprent) {
    if (exprent->type == Exprent::EXPRENT_CONST) {
        const ConstExprent* cexpr = static_cast<const ConstExprent*>(exprent);
        switch (cexpr->get_const_type().type) {
            case cc::TYPE_BYTE:
            case cc::TYPE_BYTECHAR:
            case cc::TYPE_SHORT:
            case cc::TYPE_SHORTCHAR:
            case cc::TYPE_INT:
                return true;
            default:
                break;
        }
    }
    return false;
}

}  // namespace cb::java::decompiler
