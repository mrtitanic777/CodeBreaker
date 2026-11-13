// The rows the fact table keeps of each analysis, the names of its counts, and what a module carries of resources.
#include <algorithm>
#include <format>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/classes.hpp"
#include "cb/analyze/conventions.hpp"
#include "cb/analyze/dispatch.hpp"
#include "cb/analyze/globals.hpp"
#include "cb/analyze/observe.hpp"
#include "cb/analyze/report_internal.hpp"
#include "cb/analyze/resources.hpp"
#include "cb/analyze/strings.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::analyze::report::detail {

using knowledge::Value;

// ---- the names of the counts ----------------------------------------------------------------------------------------------

Counts call_graph_counts(const call_graph::Counts& c) {
    return {{"functions", c.functions}, {"functions_found_from_calls", c.functions_found_from_calls}, {"edges", c.edges}, {"call_sites", c.call_sites}, {"import_call_sites", c.import_call_sites}, {"tail_jumps", c.tail_jumps}, {"indirect_call_sites", c.indirect_call_sites}, {"imports_used", c.imports_used}, {"leaf_functions", c.leaf_functions}, {"recursive_groups", c.recursive_groups}, {"address_taken_functions", c.address_taken_functions}, {"data_references", c.data_references}, {"roots", c.roots}, {"reached_from_roots", c.reached_from_roots}, {"reached_with_data_tables", c.reached_with_data_tables}, {"unreferenced_functions", c.unreferenced_functions}, {"code_bytes_decoded", c.code_bytes_decoded}, {"code_section_bytes_not_reached", c.code_section_bytes_not_reached}};
}

Counts conventions_counts(const conventions::Counts& c) {
    return {{"functions", c.functions}, {"convention_known", c.convention_known}, {"callee_removes_arguments", c.callee_removes_arguments}, {"caller_removes_arguments", c.caller_removes_arguments}, {"argument_count_known", c.argument_count_known}, {"argument_count_from_return", c.argument_count_from_return}, {"argument_count_from_callers", c.argument_count_from_callers}, {"argument_count_from_frame", c.argument_count_from_frame}, {"register_arguments", c.register_arguments}, {"this_in_ecx", c.this_in_ecx}, {"result_known", c.result_known}, {"result_none", c.result_none}, {"import_thunks", c.import_thunks}, {"jump_thunks", c.jump_thunks}, {"tail_calling", c.tail_calling}, {"empty_or_constant", c.empty_or_constant}, {"leaf", c.leaf}, {"with_loops", c.with_loops}, {"inline_string_operations", c.inline_string_operations}, {"floating_point", c.floating_point}, {"variadic", c.variadic}, {"system_called_procedures", c.system_called_procedures}, {"duplicate_groups", c.duplicate_groups}, {"duplicate_functions", c.duplicate_functions}, {"bridge_imports", c.bridge_imports}};
}

Counts globals_counts(const globals::Counts& c) {
    return {{"globals", c.globals}, {"typed_beyond_size", c.typed_beyond_size}, {"pointers", c.pointers}, {"function_pointers", c.function_pointers}, {"strings", c.strings}, {"arrays", c.arrays}, {"counters", c.counters}, {"flags", c.flags}, {"results_of_system_calls", c.results_of_system_calls}, {"floats", c.floats}, {"uninitialised", c.uninitialised}, {"written_never_read", c.written_never_read}, {"read_never_written", c.read_never_written}, {"pointer_tables", c.pointer_tables}, {"record_tables", c.record_tables}, {"table_entries", c.table_entries}, {"function_table_entries", c.function_table_entries}};
}

Counts strings_counts(const strings::Counts& c) {
    return {{"strings", c.strings}, {"referenced", c.referenced}, {"wide", c.wide}, {"functions_using_strings", c.functions_using_strings}, {"string_uses", c.string_uses}, {"stack_strings", c.stack_strings}, {"formatter_call_sites", c.formatter_call_sites}, {"format_strings_read", c.format_strings_read}, {"own_printf_like_functions", c.own_printf_like_functions}, {"format_strings_at_own_functions", c.format_strings_at_own_functions}, {"arguments_typed_by_formats", c.arguments_typed_by_formats}};
}

Counts structures_counts(const structures::Counts& c) {
    return {{"structures", c.structures}, {"fields", c.fields}, {"with_exact_size", c.with_exact_size}, {"pointer_fields", c.pointer_fields}, {"function_pointer_fields", c.function_pointer_fields}, {"array_fields", c.array_fields}, {"seen_through_this", c.seen_through_this}, {"seen_through_global_pointer", c.seen_through_global_pointer}, {"unified_across_functions", c.unified_across_functions}, {"largest_fields", c.largest_fields}, {"functions_touching_a_structure", c.functions_touching_a_structure}};
}

Counts dispatch_counts(const dispatch::Counts& c) {
    return {{"jump_tables", c.jump_tables}, {"jump_table_cases", c.jump_table_cases}, {"jump_tables_with_bound", c.jump_tables_with_bound}, {"compare_chains", c.compare_chains}, {"compare_chain_cases", c.compare_chain_cases}, {"switches_on_an_argument", c.switches_on_an_argument}, {"message_dispatchers", c.message_dispatchers}, {"messages_handled", c.messages_handled}, {"named_messages", c.named_messages}, {"command_dispatchers", c.command_dispatchers}, {"command_ids", c.command_ids}, {"keyword_dispatchers", c.keyword_dispatchers}, {"keywords", c.keywords}, {"interrupt_sites", c.interrupt_sites}, {"named_services", c.named_services}, {"driver_control_messages", c.driver_control_messages}};
}

Counts resources_counts(const resources::Counts& c) {
    return {{"resource_types", c.resource_types}, {"resources_carried", c.resources_carried}, {"loads_read", c.loads_read}, {"present_in_module", c.present_in_module}, {"absent_from_module", c.absent_from_module}, {"system_stock", c.system_stock}, {"distinct_resources_referenced", c.distinct_resources_referenced}, {"functions_loading_resources", c.functions_loading_resources}, {"dialog_control_uses", c.dialog_control_uses}, {"distinct_controls", c.distinct_controls}};
}

Counts observe_counts(const observe::Counts& c) {
    return {{"functions_run", c.functions_run}, {"returned", c.returned}, {"faulted", c.faulted}, {"cut_off", c.cut_off}, {"with_pointer_arguments", c.with_pointer_arguments}, {"fields_observed", c.fields_observed}, {"nested_pointers_followed", c.nested_pointers_followed}, {"static_fields_confirmed", c.static_fields_confirmed}, {"fields_only_seen_running", c.fields_only_seen_running}, {"structures_with_a_field_confirmed", c.structures_with_a_field_confirmed}, {"indirect_calls_resolved", c.indirect_calls_resolved}, {"function_starts_found", c.function_starts_found}, {"fields_passed_to_system_calls", c.fields_passed_to_system_calls}, {"removed_bytes_agree_with_static", c.removed_bytes_agree_with_static}, {"removed_bytes_differ", c.removed_bytes_differ}, {"library_idioms", c.library_idioms}};
}

// ---- the rows ---------------------------------------------------------------------------------------------------------------

namespace {

Value text_list(const std::vector<std::string>& items) {
    Value::List list;
    for (const std::string& s : items) list.emplace_back(s);
    return Value(std::move(list));
}

Value address_value(Va va) { return Value(static_cast<std::int64_t>(va)); }

void push_row(Section& section, ImageKind kind, std::optional<Va> at, std::string_view fact_kind, Value::Map fields) {
    Value::Map m;
    m.insert_or_assign("addr", at ? address_value(*at) : Value());
    m.insert_or_assign("kind", Value(fact_kind));
    for (auto& [k, v] : fields) m.insert_or_assign(k, std::move(v));
    section.rows.push_back({facts::key(kind, at), std::string(fact_kind), Value(std::move(m))});
    section.row_at.push_back(at);
}

}  // namespace

void call_graph_rows(Section& s, const call_graph::Report& r, ImageKind kind) {
    for (const call_graph::CallsFact& f : r.facts)
        push_row(s, kind, f.addr, "calls",
                 {{"callees", text_list(f.callees)}, {"imports", text_list(f.imports)}, {"indirect", Value(f.indirect)}, {"callers", text_list(f.callers)},
                  {"address_taken", Value(f.address_taken)}, {"reached", Value(f.reached)}});
}

void strings_rows(Section& s, const strings::Report& r, ImageKind kind) {
    for (const strings::StringFact& f : r.strings)
        push_row(s, kind, std::nullopt, "string", {{"at", address_value(f.at)}, {"text", Value(f.text)}, {"wide", Value(f.wide)}, {"referenced", Value(f.referenced)}});
    for (const strings::StringUsesFact& f : r.string_uses) push_row(s, kind, f.function, "string_uses", {{"strings", text_list(f.strings)}});
    for (const strings::StackString& f : r.stack_strings)
        push_row(s, kind, f.function, "stack_string", {{"where", Value(f.where)}, {"text", Value(f.text)}, {"wide", Value(f.wide)}});
    for (const strings::FormatFact& f : r.formats) {
        Value::Map m{{"site", address_value(f.site)}, {"function", Value(f.api)}, {"format", Value(f.format)}};   // (the routine that takes the format, as the Python's row names it)
        m.insert_or_assign("arguments", f.arguments ? text_list(*f.arguments) : Value());
        push_row(s, kind, f.function, "format", std::move(m));
    }
    for (const strings::PrintfLikeFact& f : r.printf_like)
        push_row(s, kind, f.function, "printf_like", {{"format_argument", Value(f.format_argument)}});
}

void globals_rows(Section& s, const globals::Report& r, ImageKind kind) {
    for (const globals::GlobalFact& f : r.globals) {
        Value::List values;
        for (const std::int64_t v : f.values) values.emplace_back(v);
        Value initial;
        switch (f.initial.kind) {
        case globals::Initial::Kind::Uninitialised: initial = Value(std::string("uninitialised")); break;
        case globals::Initial::Kind::Value: initial = Value(static_cast<std::int64_t>(f.initial.value)); break;
        case globals::Initial::Kind::Address: initial = Value(f.initial.label); break;
        }
        push_row(s, kind, std::nullopt, "global",
                 {{"at", address_value(f.at)}, {"size", Value(f.size)}, {"readers", Value(f.readers)}, {"writers", Value(f.writers)}, {"taken", Value(f.taken)},
                  {"notes", text_list(f.notes)}, {"values", Value(std::move(values))}, {"writable", Value(f.writable)}, {"extent", Value(f.extent)},
                  {"initial", std::move(initial)}, {"type", Value(f.type)}});
    }
    for (const globals::TableFact& f : r.tables) {
        Value::Map holds;
        for (const auto& [k, n] : f.holds) holds.insert_or_assign(k, Value(n));
        push_row(s, kind, std::nullopt, "pointer_table",
                 {{"at", address_value(f.at)}, {"count", Value(f.count)}, {"stride", Value(f.stride)}, {"shape", Value(std::string(globals::name_of(f.shape)))},
                  {"holds", Value(std::move(holds))}, {"first", text_list(f.first)}});
    }
}

void structures_rows(Section& s, const structures::Report& r, ImageKind kind) {
    for (const structures::StructureFact& f : r.facts) {
        Value::Map fields;   // "+offset" (hex) -> the field, in the order of the offsets
        for (const structures::FieldFact& x : f.fields)
            fields.insert_or_assign(std::format("+{:x}", x.offset), Value(Value::Map{{"size", address_value(x.size)}, {"type", Value(x.type)}, {"reads", Value(x.reads)},
                                                                                       {"writes", Value(x.writes)}, {"notes", text_list(x.notes)}}));
        push_row(s, kind, std::nullopt, "structure",
                 {{"name", Value(f.name)}, {"size", address_value(f.size)}, {"exact_size", Value(f.exact_size)}, {"fields", Value(std::move(fields))},
                  {"seen_as", text_list(f.seen_as)}, {"pointers_unified", Value(f.pointers_unified)}, {"functions", text_list(f.functions)},
                  {"passed_to", text_list(f.passed_to)}});
    }
}


void dispatch_rows(Section& s, const dispatch::Report& r, ImageKind kind) {
    const auto keyed = [](const std::vector<std::pair<Va, Va>>& items, const char* format) {   // value -> target, the value written as the Python did
        Value::Map map;
        for (const auto& [value, target] : items)
            map.insert_or_assign(std::string(format) == "hex" ? std::format("{:#x}", value) : std::to_string(value), address_value(target));
        return Value(std::move(map));
    };
    const auto messages = [](const std::vector<dispatch::HandledMessage>& items) {   // message name -> handler
        Value::Map map;
        for (const dispatch::HandledMessage& m : items) map.insert_or_assign(m.name, address_value(m.handler));
        return Value(std::move(map));
    };
    for (const dispatch::Fact& fact : r.facts) {
        if (const auto* sw = std::get_if<dispatch::SwitchFact>(&fact)) {
            push_row(s, kind, sw->function, "switch",
                     {{"on", Value(sw->on)}, {"site", sw->site ? address_value(*sw->site) : Value()}, {"table", sw->table ? address_value(*sw->table) : Value()},
                      {"default", sw->default_target ? address_value(*sw->default_target) : Value()}, {"cases", keyed(sw->cases, "hex")}});
        } else if (const auto* mf = std::get_if<dispatch::MessageFact>(&fact)) {
            push_row(s, kind, mf->function, "message_dispatch",
                     {{"procedure", mf->procedure ? Value(*mf->procedure) : Value()}, {"why", Value(mf->why)}, {"messages", messages(mf->messages)},
                      {"commands", keyed(mf->commands, "decimal")}});
        } else if (const auto* sv = std::get_if<dispatch::ServicesFact>(&fact)) {
            push_row(s, kind, sv->function, "system_services", {{"services", text_list(sv->services)}});
        } else if (const auto* dc = std::get_if<dispatch::DriverControlFact>(&fact)) {
            push_row(s, kind, dc->procedure, "driver_control", {{"device", dc->device ? Value(*dc->device) : Value()}, {"handles", messages(dc->handles)}});
        } else if (const auto* kf = std::get_if<dispatch::KeywordFunctionFact>(&fact)) {   // (a function that compares its argument with words)
            push_row(s, kind, kf->keyword.function, "keyword_dispatch",
                     {{"function", address_value(kf->keyword.function)}, {"compares_with", Value(kf->keyword.compares_with)}, {"keywords", text_list(kf->keyword.keywords)}});
        } else if (const auto* kt = std::get_if<dispatch::KeywordTableFact>(&fact)) {      // (a table in the data pairing words with handlers)
            Value::List handlers;
            for (const Va h : kt->keyword.handlers) handlers.push_back(address_value(h));
            push_row(s, kind, std::nullopt, "keyword_dispatch",
                     {{"function", Value()}, {"table", address_value(kt->keyword.table)}, {"keywords", text_list(kt->keyword.keywords)}, {"handlers", Value(std::move(handlers))}});
        }
    }
}

void classes_rows(Section& s, const classes::Analysis& r, ImageKind kind) {
    const auto optional_address = [](const std::optional<Va>& va) { return va ? address_value(*va) : Value(); };
    for (const classes::Fact& fact : r.facts) {
        if (const auto* c = std::get_if<classes::ClassFact>(&fact)) {
            Value::Map m{{"name", Value(c->name)}, {"vtable", address_value(c->vtable)}, {"virtual_functions", Value(c->virtual_functions)},
                         {"methods", text_list(c->methods)}, {"bases", text_list(c->bases)}, {"evidence", text_list(c->evidence)},
                         {"size", optional_address(c->size)}, {"constructors_or_destructors", text_list(c->constructors_or_destructors)},
                         {"com_interface", Value(c->com_interface)}};
            if (c->layout) m.insert_or_assign("layout", Value(*c->layout));
            if (c->fields) m.insert_or_assign("fields", Value(*c->fields));
            if (c->message_map) m.insert_or_assign("message_map", address_value(*c->message_map));
            if (c->size_exact) m.insert_or_assign("size_exact", Value(*c->size_exact));
            push_row(s, kind, std::nullopt, "class", std::move(m));
        } else if (const auto* mfc = std::get_if<classes::MfcClassFact>(&fact)) {
            const classes::RuntimeClass& rc = mfc->record;
            push_row(s, kind, std::nullopt, "mfc_class",
                     {{"at", address_value(rc.at)}, {"name", Value(rc.name)}, {"size", Value(rc.size)}, {"schema", rc.schema ? Value(*rc.schema) : Value()},
                      {"creatable", Value(rc.creatable)}, {"base", rc.base ? Value(*rc.base) : Value()}});
        } else if (const auto* map = std::get_if<classes::MessageMapFact>(&fact)) {
            push_row(s, kind, std::nullopt, "message_map",
                     {{"at", address_value(map->at)}, {"class", map->class_name ? Value(*map->class_name) : Value()}, {"entries", text_list(map->entries)},
                      {"base_map", optional_address(map->base_map)}});
        } else if (const auto* t = std::get_if<classes::CxxTableFact>(&fact)) {
            Value::List blocks;
            for (const classes::TryBlockFact& b : t->try_blocks) blocks.emplace_back(Value::Map{{"states", Value(b.states)}, {"catches", text_list(b.catches)}});
            push_row(s, kind, t->function, "cxx_exception_table",
                     {{"at", address_value(t->at)}, {"states", Value(t->states)}, {"cleanups", Value(t->cleanups)}, {"try_blocks", Value(std::move(blocks))}});
        } else if (const auto* f = std::get_if<classes::ExceptionFrameFact>(&fact)) {
            Value::Map m{{"handler", Value(f->handler)}};
            if (f->scopes && !f->scopes->empty()) {
                Value::List scopes;
                for (const classes::ScopeFact& sc : *f->scopes)
                    scopes.emplace_back(Value::Map{{"enclosing", optional_address(sc.enclosing)}, {"kind", Value(sc.kind)}, {"filter", optional_address(sc.filter)},
                                                   {"handler", address_value(sc.handler)}});
                m.insert_or_assign("scopes", Value(std::move(scopes)));
            }
            push_row(s, kind, f->function, "exception_frame", std::move(m));
        }
    }
}

namespace {

Value resource_id_value(const resources::ResourceId& id) {
    if (const auto* n = std::get_if<std::int64_t>(&id)) return Value(*n);
    return Value(std::get<std::string>(id));
}

}  // namespace

void resources_rows(Section& s, const resources::Report& r, ImageKind kind) {
    for (const resources::ResolvedLoad& l : r.loads) {
        Value present;
        if (l.presence == resources::Presence::Present) present = Value(true);
        else if (l.presence == resources::Presence::Absent) present = Value(false);
        push_row(s, kind, l.load.function, "resource_use",
                 {{"function", address_value(l.load.function)}, {"site", address_value(l.load.site)}, {"call", Value(l.load.call)},
                  {"type", l.load.type ? Value(*l.load.type) : Value()}, {"id", resource_id_value(l.load.id)}, {"present", std::move(present)}});
    }
    for (const resources::ControlUse& c : r.controls)
        push_row(s, kind, c.function, "dialog_control_use",
                 {{"function", address_value(c.function)}, {"site", address_value(c.site)}, {"call", Value(c.call)}, {"control", Value(c.control)}});
    Value::Map types;
    for (const auto& [t, n] : r.carried) types.insert_or_assign(t, Value(n));
    push_row(s, kind, std::nullopt, "resources_carried", {{"types", Value(std::move(types))}});
}

// ---- what a module carries ------------------------------------------------------------------------------------------------

std::map<std::string, std::set<std::variant<std::int64_t, std::string>>> carried_resources(const std::filesystem::path& path, ImageKind kind) {
    using Id = std::variant<std::int64_t, std::string>;
    std::map<std::string, std::set<Id>> out;
    if (kind == ImageKind::Pe) {
        const formats::pe::Image pe = formats::pe::Image::open(path);
        for (const formats::pe::ResourceType& t : pe.resource_tree()) {
            std::string name;
            if (t.id) {
                const std::int64_t n = *t.id;
                name = n == 1 ? "CURSOR" : n == 2 ? "BITMAP" : n == 3 ? "ICON" : n == 4 ? "MENU" : n == 5 ? "DIALOG" : n == 6 ? "STRING" : n == 9 ? "ACCELERATOR"
                       : n == 10 ? "RCDATA" : n == 12 ? "GROUP_CURSOR" : n == 14 ? "GROUP_ICON" : n == 16 ? "VERSION" : std::format("#{}", n);
            } else {
                name = t.name.value_or("");
            }
            for (const formats::pe::ResourceItem& item : t.items)
                out[name].insert(item.id ? Id(static_cast<std::int64_t>(*item.id)) : Id(text::upper(item.name.value_or(""))));
        }
    } else if (kind == ImageKind::Ne) {
        const formats::ne::Module module = formats::ne::Module::open_file(path);
        for (const formats::ne::Resource& r : module.resources()) {
            const std::string& ident = r.id;
            const auto number = ident.starts_with("#") ? text::parse_int<std::int64_t>(std::string_view(ident).substr(1), 10) : std::nullopt;
            out[r.type].insert(number ? Id(*number) : Id(text::upper(ident)));
        }
    }
    return out;
}

}  // namespace cb::analyze::report::detail
