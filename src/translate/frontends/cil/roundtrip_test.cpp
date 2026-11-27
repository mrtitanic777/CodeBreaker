// Tests of the CIL lane (translate/frontends/cil): stage 13 of docs/DOTNET_DECOMPILER_PLAN.md, ILAst -> L2 -> ILAst. Every method of the
// test assemblies is read, run through the whole transform pipeline, lowered to L2 and lifted back; the lifted function must write the
// same ILAst text (with IL ranges and child indices shown) as the original, and agree with it field by field where the text says nothing
// (use counts, edge and leave counts, prefixes, the variables' and the function's own state).
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <format>
#include <functional>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/metadata/metadata_reader.hpp"
#include "translate/frontends/cil/cil.hpp"

namespace cb::translate::frontends::cil {
namespace cil_roundtrip_test {

namespace dd = dotnet::decompiler;
namespace il = dotnet::decompiler::il;
namespace srm = dotnet::metadata;

class NoReferences final : public dd::metadata::IAssemblyResolver {
public:
    std::shared_ptr<dd::metadata::MetadataFile> resolve(const dd::metadata::IAssemblyReference&) override { return nullptr; }
    std::shared_ptr<dd::metadata::MetadataFile> resolve_module(const dd::metadata::MetadataFile&, std::string_view) override { return nullptr; }
};

std::string text_of(const il::ILFunction& f, bool ranges) {
    dd::PlainTextOutput output("\n");
    il::ILAstWritingOptions options;
    options.show_il_ranges = ranges;
    options.show_child_index_in_block = ranges;
    f.write_to(output, options);
    return output.text();
}

// Field by field, where the text is silent. `why` names the first difference.
class Deep {
public:
    bool same(const il::ILFunction& a, const il::ILFunction& b) { return function(a, b); }
    std::string why;

private:
    std::unordered_map<const void*, const void*> map_;   // original -> lifted (variables, blocks, containers)

    bool fail(std::string what) {
        if (why.empty()) why = std::move(what);
        return false;
    }
    template <class T> bool mapped(const T* a, const T* b, const char* what) {
        if ((a == nullptr) != (b == nullptr)) return fail(std::format("{}: one is null", what));
        if (a == nullptr) return true;
        auto it = map_.find(a);
        if (it == map_.end()) return fail(std::format("{}: not mapped", what));
        return it->second == b || fail(std::format("{}: another one", what));
    }
    bool variable(const il::ILVariable& a, const il::ILVariable& b) {
        map_[&a] = &b;
        if (a.kind() != b.kind() || a.stack_type != b.stack_type || a.type() != b.type() || a.index() != b.index() || a.name != b.name ||
            a.has_generated_name != b.has_generated_name || a.is_ref_read_only != b.is_ref_read_only ||
            a.initial_value_is_initialized() != b.initial_value_is_initialized() || a.uses_initial_value() != b.uses_initial_value() ||
            a.state_machine_field != b.state_machine_field || a.remove_if_redundant != b.remove_if_redundant || a.load_count() != b.load_count() ||
            a.store_count() != b.store_count() || a.address_count() != b.address_count() || a.index_in_function() != b.index_in_function())
            return fail(std::format("variable {}", a.name.value_or("?")));
        return true;
    }
    bool function(const il::ILFunction& a, const il::ILFunction& b) {
        if (a.variables().count() != b.variables().count()) return fail("variable count");
        for (int i = 0; i < a.variables().count(); ++i) {
            if (!variable(*a.variables()[i], *b.variables()[i])) return false;
        }
        if (a.method != b.method || a.name != b.name || a.code_size != b.code_size || a.local_variable_signature_length != b.local_variable_signature_length ||
            a.warnings != b.warnings || a.is_iterator != b.is_iterator || a.state_machine_compiled_with_mono != b.state_machine_compiled_with_mono ||
            a.state_machine_compiled_with_legacy_visual_basic != b.state_machine_compiled_with_legacy_visual_basic ||
            a.async_return_type != b.async_return_type || a.move_next_method != b.move_next_method || a.reduced_method != b.reduced_method ||
            a.delegate_type != b.delegate_type || a.return_type != b.return_type || a.parameters != b.parameters ||
            a.sequence_point_candidates != b.sequence_point_candidates || a.kind() != b.kind() ||
            a.async_debug_info.catch_handler_offset != b.async_debug_info.catch_handler_offset ||
            a.async_debug_info.awaits_is_default != b.async_debug_info.awaits_is_default || a.captured_variables.size() != b.captured_variables.size())
            return fail(std::format("function {}", a.name));
        const auto list = [](const std::vector<const dd::typesystem::ITypeParameter*>* l) {
            return l != nullptr ? *l : std::vector<const dd::typesystem::ITypeParameter*>{};
        };
        if (list(a.generic_context.class_type_parameters) != list(b.generic_context.class_type_parameters) ||
            list(a.generic_context.method_type_parameters) != list(b.generic_context.method_type_parameters))
            return fail(std::format("generic context of {}", a.name));
        if (!instruction(*a.body(), *b.body())) return false;
        for (int i = 0; i < a.local_functions.count(); ++i) {
            if (i >= b.local_functions.count() || !instruction(*a.local_functions[i], *b.local_functions[i])) return fail("local functions");
        }
        for (const il::ILVariable* v : a.captured_variables) {
            auto it = map_.find(v);
            if (it == map_.end() || !b.captured_variables.contains(static_cast<il::ILVariable*>(const_cast<void*>(it->second)))) return fail("captured variables");
        }
        for (int i = 0; i < a.variables().count(); ++i) {
            if (!mapped<il::BlockContainer>(a.variables()[i]->capture_scope, b.variables()[i]->capture_scope, "capture scope")) return false;
        }
        return mapped<il::BlockContainer>(a.declaration_scope, b.declaration_scope, "declaration scope");
    }
    bool instruction(const il::ILInstruction& a, const il::ILInstruction& b);
};

bool Deep::instruction(const il::ILInstruction& a, const il::ILInstruction& b) {
    using il::OpCode;
    if (a.op_code != b.op_code) return fail(std::format("opcode {} vs {}", static_cast<int>(a.op_code), static_cast<int>(b.op_code)));
    if (a.start_il_offset() != b.start_il_offset() || a.end_il_offset() != b.end_il_offset()) return fail("IL range");
    if (a.result_type() != b.result_type()) return fail("result type");
    if (a.get_child_count() != b.get_child_count()) return fail("child count");
    if (a.op_code == OpCode::ILFunction && &a != &b) {
        // a nested function: compared whole (its children with it)
        return function(static_cast<const il::ILFunction&>(a), static_cast<const il::ILFunction&>(b));
    }
    switch (a.op_code) {
        case OpCode::BlockContainer: {
            const auto& x = static_cast<const il::BlockContainer&>(a);
            const auto& y = static_cast<const il::BlockContainer&>(b);
            map_[&x] = &y;
            for (int i = 0; i < x.blocks.count(); ++i) map_[x.blocks[i]] = y.blocks[i];
            if (x.kind() != y.kind() || x.leave_count() != y.leave_count()) return fail("container");
            break;
        }
        case OpCode::Block: {
            const auto& x = static_cast<const il::Block&>(a);
            const auto& y = static_cast<const il::Block&>(b);
            if (x.kind != y.kind || x.incoming_edge_count() != y.incoming_edge_count()) return fail("block");
            break;
        }
        case OpCode::Branch:
            if (!mapped<il::Block>(static_cast<const il::Branch&>(a).target_block(), static_cast<const il::Branch&>(b).target_block(), "branch")) return false;
            break;
        case OpCode::Leave:
            if (!mapped<il::BlockContainer>(static_cast<const il::Leave&>(a).target_container(), static_cast<const il::Leave&>(b).target_container(), "leave"))
                return false;
            break;
        case OpCode::StLoc: {
            const auto& x = static_cast<const il::StLoc&>(a);
            const auto& y = static_cast<const il::StLoc&>(b);
            if (x.is_stack_adjustment != y.is_stack_adjustment || x.il_stack_was_empty != y.il_stack_was_empty) return fail("stloc");
            break;
        }
        case OpCode::Call:
        case OpCode::CallVirt:
        case OpCode::NewObj: {
            const auto& x = static_cast<const il::CallInstruction&>(a);
            const auto& y = static_cast<const il::CallInstruction&>(b);
            if (x.method != y.method || x.is_tail != y.is_tail || x.constrained_to != y.constrained_to || x.il_stack_was_empty != y.il_stack_was_empty)
                return fail("call");
            break;
        }
        case OpCode::Conv: {
            const auto& x = static_cast<const il::Conv&>(a);
            const auto& y = static_cast<const il::Conv&>(b);
            if (x.kind != y.kind || x.check_for_overflow != y.check_for_overflow || x.input_type != y.input_type || x.input_sign != y.input_sign ||
                x.target_type != y.target_type || x.is_lifted() != y.is_lifted())
                return fail("conv");
            break;
        }
        case OpCode::LdcF4:
            if (std::memcmp(&static_cast<const il::LdcF4&>(a).value, &static_cast<const il::LdcF4&>(b).value, sizeof(float)) != 0) return fail("ldc.f4");
            break;
        case OpCode::LdcF8:
            if (std::memcmp(&static_cast<const il::LdcF8&>(a).value, &static_cast<const il::LdcF8&>(b).value, sizeof(double)) != 0) return fail("ldc.f8");
            break;
        case OpCode::LdcDecimal:
            if (!(static_cast<const il::LdcDecimal&>(a).value == static_cast<const il::LdcDecimal&>(b).value)) return fail("ldc.decimal");
            break;
        case OpCode::Throw:
            if (static_cast<const il::Throw&>(a).result_type_ != static_cast<const il::Throw&>(b).result_type_) return fail("throw");
            break;
        case OpCode::DefaultValue:
            if (static_cast<const il::DefaultValue&>(a).il_stack_was_empty != static_cast<const il::DefaultValue&>(b).il_stack_was_empty) return fail("default");
            break;
        case OpCode::LdFlda:
            if (static_cast<const il::LdFlda&>(a).delay_exceptions != static_cast<const il::LdFlda&>(b).delay_exceptions) return fail("ldflda");
            break;
        case OpCode::LdElema: {
            const auto& x = static_cast<const il::LdElema&>(a);
            const auto& y = static_cast<const il::LdElema&>(b);
            if (x.type() != y.type() || x.with_system_index != y.with_system_index || x.delay_exceptions != y.delay_exceptions || x.is_read_only != y.is_read_only)
                return fail("ldelema");
            break;
        }
        case OpCode::TryCatchHandler:
            if (!(static_cast<const il::TryCatchHandler&>(a).exception_specifier_il_range() == static_cast<const il::TryCatchHandler&>(b).exception_specifier_il_range()))
                return fail("catch specifier range");
            break;
        case OpCode::SwitchInstruction: {
            const auto& x = static_cast<const il::SwitchInstruction&>(a);
            const auto& y = static_cast<const il::SwitchInstruction&>(b);
            if (x.is_lifted != y.is_lifted || x.type != y.type) return fail("switch");
            break;
        }
        case OpCode::SwitchSection: {
            const auto& x = static_cast<const il::SwitchSection&>(a);
            const auto& y = static_cast<const il::SwitchSection&>(b);
            if (!x.labels().set_equals(y.labels()) || x.has_null_label() != y.has_null_label() ||
                x.is_compiler_generated_default_section() != y.is_compiler_generated_default_section())
                return fail("switch section");
            break;
        }
        case OpCode::MatchInstruction: {
            const auto& x = static_cast<const il::MatchInstruction&>(a);
            const auto& y = static_cast<const il::MatchInstruction&>(b);
            if (x.method() != y.method() || x.is_deconstruct_call != y.is_deconstruct_call || x.is_deconstruct_tuple != y.is_deconstruct_tuple ||
                x.check_type != y.check_type || x.check_not_null != y.check_not_null)
                return fail("match");
            break;
        }
        case OpCode::NullableUnwrap:
            if (static_cast<const il::NullableUnwrap&>(a).ref_input != static_cast<const il::NullableUnwrap&>(b).ref_input) return fail("nullable.unwrap");
            break;
        case OpCode::StringToInt:
            if (static_cast<const il::StringToInt&>(a).map() != static_cast<const il::StringToInt&>(b).map() ||
                static_cast<const il::StringToInt&>(a).expected_type() != static_cast<const il::StringToInt&>(b).expected_type())
                return fail("string.to.int");
            break;
        default: break;
    }
    // the variable an instruction names
    const auto* va = dynamic_cast<const il::IInstructionWithVariableOperand*>(&a);
    const auto* vb = dynamic_cast<const il::IInstructionWithVariableOperand*>(&b);
    if (va != nullptr && !mapped<il::ILVariable>(va->variable(), vb->variable(), "variable operand")) return false;
    for (int i = 0; i < a.get_child_count(); ++i) {
        const il::ILInstruction* ca = a.get_child(i);
        const il::ILInstruction* cb2 = b.get_child(i);
        if ((ca == nullptr) != (cb2 == nullptr)) return fail("a child is null");
        if (ca != nullptr && !instruction(*ca, *cb2)) return false;
    }
    return true;
}

struct Totals {
    std::size_t methods = 0, errors = 0, round_tripped = 0, text_differs = 0, deep_differs = 0, refused = 0;
    std::size_t arena_bytes = 0, max_arena_bytes = 0, max_nodes = 0;
    double pipeline_ms = 0, lower_ms = 0, lift_ms = 0;   // reading and the transforms; the lowering; the lifting   // the L2 trees' own memory (the nodes; what vectors and boxes hold is outside)
    LoweringStats lowering;
    std::string first_problem;
};

// Every method of the main module: read, `steps` transforms, then lowered and lifted. `out` (if given) gets the lifted functions'
// text as DecompilerDump writes it (`transforms N`), so it compares with the oracle.
Totals round_trip(const dd::typesystem::DecompilerTypeSystem& ts, int steps, std::string* out, const std::function<void(ir::Node*)>& mutate = {}) {
    const dd::typesystem::MetadataModule& module = ts.main_metadata_module();
    const dd::metadata::MetadataFile& file = *module.metadata_file();
    const srm::MetadataReader& md = module.metadata();
    const dd::DecompilerSettings settings;
    Totals totals;
    Arena l2;                  // one for the assembly, emptied for each method (its memory kept)
    Lowering lowering(l2);     // one for the assembly: its types and members are the type system's
    for (srm::MethodDefinitionHandle h : md.method_definitions()) {
        const srm::MethodDefinition def = md.get_method_definition(h);
        if (def.relative_virtual_address() == 0) continue;
        ++totals.methods;
        const std::string head = std::format("method {:08x} {}::{}\n", srm::get_token(h),
                                             md.get_string(md.get_type_definition(def.get_declaring_type()).name()), md.get_string(def.name()));
        if (out != nullptr) *out += head;
        il::ILArena arena;
        il::ILArenaScope scope(arena);
        try {
            const srm::MethodBodyBlock body = file.get_method_body(def.relative_virtual_address());
            il::ILReader reader(module);
            reader.use_debug_symbols = false;
            reader.use_ref_locals_for_accurate_order_of_evaluation = true;
            il::ILFunction* function = reader.read_il(h, body);
            auto all = il::transforms::get_il_transforms();
            all.resize(std::min<std::size_t>(all.size(), static_cast<std::size_t>(steps)));
            il::transforms::ILTransformContext context(function, ts, settings);
            const auto t0 = std::chrono::steady_clock::now();
            function->run_transforms(all, context);
            const auto t1 = std::chrono::steady_clock::now();
            totals.pipeline_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();
            il::ILFunction* back = nullptr;
            try {
                l2.reset();
                const std::size_t nodes_before = lowering.stats().nodes;
                const auto t2 = std::chrono::steady_clock::now();
                ir::Node* lowered = lowering.function(*function);
                if (mutate) mutate(lowered);
                const auto t3 = std::chrono::steady_clock::now();
                back = Lifting().function(lowered);
                const auto t4 = std::chrono::steady_clock::now();
                totals.lower_ms += std::chrono::duration<double, std::milli>(t3 - t2).count();
                totals.lift_ms += std::chrono::duration<double, std::milli>(t4 - t3).count();
                totals.arena_bytes += l2.bytes_used();
                totals.max_arena_bytes = std::max(totals.max_arena_bytes, l2.bytes_used());
                totals.max_nodes = std::max(totals.max_nodes, lowering.stats().nodes - nodes_before);
            } catch (const std::logic_error& e) {
                ++totals.refused;
                if (totals.first_problem.empty()) totals.first_problem = head + e.what();
            }
            if (back != nullptr) {
                ++totals.round_tripped;
                if (text_of(*function, true) != text_of(*back, true)) {
                    ++totals.text_differs;
                    if (totals.first_problem.empty()) totals.first_problem = head + "text:\n" + text_of(*function, true) + "\nlifted:\n" + text_of(*back, true);
                }
                Deep deep;
                if (!deep.same(*function, *back)) {
                    ++totals.deep_differs;
                    if (totals.first_problem.empty()) totals.first_problem = head + "deep: " + deep.why;
                }
            }
            if (out != nullptr) {
                dd::PlainTextOutput output("\n");
                (back != nullptr ? back : function)->write_to(output, il::ILAstWritingOptions{});
                *out += output.text();
                *out += '\n';
            }
        } catch (const srm::BadImageFormatError& ex) {
            ++totals.errors;
            if (out != nullptr) *out += "error BadImageFormatException: " + ex.message() + "\n";
        }
    }
    totals.lowering = lowering.stats();
    return totals;
}

void check_assembly(std::string_view name) {
    const std::shared_ptr<dd::metadata::PEFile> module = dd::metadata::PEFile::open(cb::testing::data_path(std::string("dotnet/testdata/inputs/") + std::string(name)));
    NoReferences resolver;
    const dd::typesystem::DecompilerTypeSystem ts(module, resolver);
    const Totals t = round_trip(ts, 1000, nullptr);
    CB_CHECK(t.methods > 0);
    CB_CHECK_EQ(t.refused, std::size_t{0});
    CB_CHECK_EQ(t.round_tripped + t.errors, t.methods);
    CB_CHECK_EQ(t.text_differs, std::size_t{0});
    CB_CHECK_EQ(t.deep_differs, std::size_t{0});
    if (!t.first_problem.empty()) cb::testing::record_failure(__FILE__, __LINE__, t.first_problem.substr(0, 4000));
}

CB_TEST("translate.cil.round_trip.sample", [] { check_assembly("Sample.dll"); });

// The check has teeth: a lowered tree with one kind of fact changed lifts to a function the text or the field comparison tells apart.
CB_TEST("translate.cil.round_trip.notices_a_changed_fact", [] {
    const std::shared_ptr<dd::metadata::PEFile> module = dd::metadata::PEFile::open(cb::testing::data_path("dotnet/testdata/inputs/Sample.dll"));
    NoReferences resolver;
    const dd::typesystem::DecompilerTypeSystem ts(module, resolver);
    const auto each = [](std::function<void(ir::Node&)> f) {
        return [f](ir::Node* root) {
            for (ir::Node* n : ir::walk(root)) f(*n);
        };
    };
    const std::pair<const char*, std::function<void(ir::Node*)>> mutations[] = {
        {"an origin dropped", each([](ir::Node& n) { n.origin.reset(); })},
        {"the flags cleared", each([](ir::Node& n) { n.flags = 0; })},
        {"a sign turned", each([](ir::Node& n) {
             if (auto* b = ir::get<ir::Binary>(&n); b != nullptr && b->is_signed) b->is_signed = !*b->is_signed;
         })},
        {"a stack kind changed", each([](ir::Node& n) {
             if (auto* b = ir::get<ir::Binary>(&n); b != nullptr && b->left_stk == ir::Stack::I4) b->left_stk = ir::Stack::I8;
         })},
        {"a call made virtual", each([](ir::Node& n) {
             if (auto* c = ir::get<ir::Call>(&n); c != nullptr && c->form == ir::CallForm::Direct) c->form = ir::CallForm::Virtual;
         })},
        {"a variable's index moved", each([](ir::Node& n) {
             if (auto* name = ir::get<ir::Name>(&n); name != nullptr && name->sym->index) name->sym->index = *name->sym->index + 1;
         })},
        {"a region's kind changed", each([](ir::Node& n) {
             if (auto* r = ir::get<ir::Region>(&n); r != nullptr && r->kind == ir::RegionKind::Normal) r->kind = ir::RegionKind::Loop;
         })},
    };
    for (const auto& [what, mutate] : mutations) {
        const Totals t = round_trip(ts, 1000, nullptr, mutate);
        const std::size_t noticed = t.text_differs + t.deep_differs + t.refused;
        if (noticed == 0) cb::testing::record_failure(__FILE__, __LINE__, std::string("not noticed: ") + what);
        CB_CHECK(noticed > 0);
    }
});
CB_TEST("translate.cil.round_trip.sample64", [] { check_assembly("Sample64.dll"); });

// ad hoc: CB_CIL_RT_IN=<assembly> CB_CIL_RT_OUT=<text> CB_CIL_RT_STEPS=<N> [CB_CIL_RT_STATS=<file>] writes the lifted functions' dump after N
// transforms (work/ilspy-l2/rtcompare.sh compares it with DecompilerDump's) and the counts of the round trip.
CB_TEST("translate.cil.round_trip_wip_dump", [] {
    const auto in = cb::fs::get_env("CB_CIL_RT_IN");
    const auto out = cb::fs::get_env("CB_CIL_RT_OUT");
    const auto steps = cb::fs::get_env("CB_CIL_RT_STEPS");
    if (!in || !out || !steps) return;
    const std::shared_ptr<dd::metadata::PEFile> module = dd::metadata::PEFile::open(std::filesystem::path(*in));
    NoReferences resolver;
    const dd::typesystem::DecompilerTypeSystem ts(module, resolver);
    std::string text;
    const Totals t = round_trip(ts, std::stoi(*steps), &text);
    std::ofstream(std::filesystem::path(*out), std::ios::binary) << text;
    if (const auto stats = cb::fs::get_env("CB_CIL_RT_STATS")) {
        std::ofstream s{std::filesystem::path(*stats), std::ios::binary};
        s << std::format("methods {} errors {} round_tripped {} refused {} text_differs {} deep_differs {} functions {} instructions {} nodes {} labels_uniquified {}"
                         " arena_bytes {} max_arena_bytes {} max_nodes {} sizeof_node {} sizeof_type {} sizeof_sym {} pipeline_ms {:.0f} lower_ms {:.0f} lift_ms {:.0f}\n",
                         t.methods, t.errors, t.round_tripped, t.refused, t.text_differs, t.deep_differs, t.lowering.functions, t.lowering.instructions,
                         t.lowering.nodes, t.lowering.labels_uniquified, t.arena_bytes, t.max_arena_bytes, t.max_nodes, sizeof(ir::Node), sizeof(ir::Type),
                         sizeof(ir::Sym), t.pipeline_ms, t.lower_ms, t.lift_ms);
        s << t.first_problem << '\n';
    }
});

}  // namespace cil_roundtrip_test
}  // namespace cb::translate::frontends::cil
