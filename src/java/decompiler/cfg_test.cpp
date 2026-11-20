// Tests of the decompiler's code layer (instructions, cfg, exception_deobfuscator, dominators, context): every method of a jar parsed into instructions
// and built into the control-flow graph the original builds before making statements. The golden is the original's own dump (testdata/oracle/
// GraphDump.java run on Fernflower built from its source), the same text this file writes.
#include "java/decompiler/cfg.hpp"

#include <format>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/decompiler/context.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {
namespace {

std::string gd_list(const std::vector<int>& v) {
    std::vector<std::string> s;
    for (const int x : v) s.push_back(std::to_string(x));
    return "[" + text::join(s, ", ") + "]";
}

std::string gd_dump(StructContext& ctx) {
    std::string out;
    for (const auto& [name, c] : ctx.classes()) {
        for (const auto& mt : c->methods()) {
            if (!mt->contains_code()) continue;
            out += "== " + c->qualified_name + " " + mt->name() + " " + mt->descriptor() + "\n";
            std::string body;
            try {
                const std::shared_ptr<InstructionSequence> seq = parse_bytecode(*mt);
                body += seq->to_string();
                for (const auto& h : seq->exception_table()->handlers)
                    body += std::format("handler {} {} {} {} {} {} {}\n", h->from, h->to, h->handler, h->from_instr, h->to_instr, h->handler_instr,
                                        h->exception_class.value_or("null"));
                const std::unique_ptr<ControlFlowGraph> graph = build_method_graph(*mt);
                body += graph->to_string();
                for (const auto& r : graph->exceptions()) {
                    std::string b = std::format("range handler={} types={} unique={} blocks=", r->handler->id,
                                                r->exception_types ? "[" + text::join(*r->exception_types, ", ") + "]" : std::string("null"),
                                                r->unique_exceptions_string().value_or("null"));
                    for (const BasicBlock* x : r->protected_range) b += std::to_string(x->id) + ",";
                    body += b + "\n";
                }
                std::string e = std::format("first={} last={} last_id={} obfuscated={} preds:", graph->first()->id, graph->last()->id, graph->last_id,
                                            exception_deobfuscator::has_obfuscated_exceptions(*graph) ? "true" : "false");
                for (const BasicBlock* x : graph->blocks()) {
                    e += " " + std::to_string(x->id) + "<";
                    for (const BasicBlock* p : x->preds) e += std::to_string(p->id) + ",";
                    e += "|";
                    for (const BasicBlock* p : x->pred_exceptions) e += std::to_string(p->id) + ",";
                    e += " offs=" + gd_list(x->instr_old_offsets);
                }
                body += e + "\n";
            } catch (const std::exception&) {
                body += "ERROR\n";   // (the original's dump writes what it had before the fault; a fault is compared as the line alone)
            }
            out += body;
        }
    }
    return out;
}

// The dumps compared method by method: the first method that differs, named.
std::string gd_first_difference(const std::string& got, const std::string& want) {
    const std::vector<std::string_view> a = text::split(got, '\n'), b = text::split(want, '\n');
    std::string method;
    for (std::size_t i = 0; i < std::max(a.size(), b.size()); ++i) {
        const std::string_view x = i < a.size() ? a[i] : "<end>", y = i < b.size() ? b[i] : "<end>";
        if (y.starts_with("== ")) method = std::string(y);
        if (x != y) return std::format("{} line {}: got '{}' want '{}'", method, i + 1, x, y);
    }
    return {};
}

void gd_check(const std::filesystem::path& jar, const std::filesystem::path& golden) {
    DecompilerContext dc;
    dc.properties[std::string(pref::NEW_LINE_SEPARATOR)] = "1";
    DecompilerScope scope(std::move(dc));
    StructContext ctx;
    ctx.add_space(jar, true);
    scope.context().struct_context = &ctx;
    const std::string got = gd_dump(ctx);
    const std::string want = fs::read_text(golden);
    const std::string diff = gd_first_difference(got, want);
    if (!diff.empty()) std::fprintf(stderr, "  %s\n", diff.c_str());
    CB_CHECK(diff.empty());
}

CB_TEST("java.decompiler.cfg.every_method_is_parsed_and_built_into_the_graph_the_original_builds", [] {
    gd_check(testing::data_path("java/retromcp/gradle/wrapper/gradle-wrapper.jar"), testing::data_path("java/decompiler/testdata/graph/gradle-wrapper.txt"));
    // a larger reference when it is there: the decompile engine's jar and the original's dump of it (work/fernflower-src)
    const std::filesystem::path big = testing::data_path("../work/fernflower-src/retromcp.graph.txt");
    std::error_code ec;
    if (std::filesystem::is_regular_file(big, ec)) gd_check(testing::data_path("java/retromcp/build/libs/retromcp.jar"), big);
});

CB_TEST("java.decompiler.cfg.java_hash_order", [] {
    CB_CHECK_EQ(java_string_hash("hello"), 99162322);
    CB_CHECK_EQ(java_string_hash(""), 0);
    CB_CHECK_EQ(java_string_hash("java/lang/Exception"), static_cast<std::int32_t>(0x527a44db));
    // a HashSet<String> iterates by bucket ("b" 98 -> 2, "a" 97 -> 1), whatever the order the strings went in
    CB_CHECK(java_hash_set_order({"b", "a", "b"}) == std::vector<std::string>({"a", "b"}));
});

}  // namespace
}  // namespace cb::java::decompiler
