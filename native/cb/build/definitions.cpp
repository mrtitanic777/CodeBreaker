#include "cb/build/definitions.hpp"

#include <algorithm>
#include <format>
#include <tuple>

#include "cb/core/text.hpp"

namespace cb::build::definitions {

namespace {

// The file name without its last extension (a leading dot is part of the name).
std::string_view stem_of(std::string_view file_name) {
    const std::size_t dot = file_name.rfind('.');
    return dot == std::string_view::npos || dot == 0 ? file_name : file_name.substr(0, dot);
}

std::string_view kind_text(ExportKind kind) {
    switch (kind) {
        case ExportKind::Code: return "code";
        case ExportKind::Data: return "data";
        case ExportKind::Forward: return "forward";
    }
    return "?";
}

// The name an export is given where the linker needs one.
std::string name_or_ordinal(const Export& e) { return e.has_name() ? *e.name : std::format("ordinal_{}", e.ordinal); }

std::string rtrimmed(std::string_view s) { return std::string(text::rtrim(s)); }

}  // namespace

std::string Export::describe() const {
    return std::format("<export {} @{} {}>", has_name() ? *name : std::string("(no name)"), ordinal, kind_text(kind));
}

std::string Import::text() const {
    return std::holds_alternative<std::string>(what) ? std::get<std::string>(what) : std::to_string(std::get<std::uint32_t>(what));
}

std::string definition_file(const Interface& iface, std::string_view file_name) {
    const bool win16 = iface.kind == ModuleKind::Win16;
    std::vector<std::string> lines;
    lines.push_back(std::format("; {} -- module definition, read from the original's own tables.", text::lower(file_name)));
    lines.push_back(std::format("{} {}", iface.library ? "LIBRARY" : "NAME", iface.name));
    if (!iface.description.empty()) {
        lines.push_back(std::format("DESCRIPTION '{}'", text::replace_all(iface.description, "'", "''")));
    }
    if (win16) {
        lines.push_back("EXETYPE WINDOWS");
        lines.push_back(std::format("HEAPSIZE {}", iface.heap));
        if (iface.stack != 0) lines.push_back(std::format("STACKSIZE {}", iface.stack));
    }
    if (!iface.exports.empty()) lines.push_back("EXPORTS");
    for (const Export& e : iface.exports) {
        std::string line;
        if (e.kind == ExportKind::Forward) {
            line = std::format("    {} = {} @{}", name_or_ordinal(e), e.forward_target, e.ordinal);
        } else {
            line = std::format("    {:<32} @{}", name_or_ordinal(e), e.ordinal);
        }
        if (!e.has_name()) line += " NONAME";
        if (win16 && e.resident) line += " RESIDENTNAME";
        if (e.kind == ExportKind::Data) line += win16 ? "        ; data" : " DATA";
        lines.push_back(rtrimmed(line));
    }
    // The imports in the order of their text (an ordinal sorts as the decimal text it is written as): the .def
    // is read by people and diffed between builds.
    std::vector<const Import*> taken;
    for (const Import& i : iface.imports) taken.push_back(&i);
    std::stable_sort(taken.begin(), taken.end(), [](const Import* a, const Import* b) {
        return std::tuple(a->module, a->text()) < std::tuple(b->module, b->text());
    });
    if (!taken.empty()) lines.push_back("IMPORTS");
    for (const Import* i : taken) lines.push_back(std::format("    {}.{}", i->module, i->text()));
    return text::join(lines, "\n") + "\n";
}

Directives export_directives(const Interface& iface, const SymbolAt& symbol_at) {
    const bool win16 = iface.kind == ModuleKind::Win16;
    Directives out;
    for (const Export& e : iface.exports) {
        if (e.kind == ExportKind::Forward) {
            out.missing.push_back({e, std::format("forwarded to {}: this linker cannot forward", e.forward_target)});
            continue;
        }
        const std::optional<std::string> symbol = symbol_at(e);
        if (!symbol || symbol->empty()) {
            out.missing.push_back({e, e.kind == ExportKind::Data ? "exported data is not a symbol of its own"
                                                                  : "no function was reconstructed at its address"});
            continue;
        }
        std::string name = name_or_ordinal(e);
        if (!text::is_identifier(name)) name = "'" + name + "'";
        // NODATA: the entry is not marked as using the module's data segment -- the loader then leaves its
        // prologue alone, and the function runs on its caller's DS, as the original's
        out.lines.push_back(std::format("export {}.{}='{}'{}{}", name, e.ordinal, *symbol,
                                        win16 && e.resident ? " resident" : "",
                                        win16 && e.kind == ExportKind::Code && !e.shared_data ? " nodata" : ""));
    }
    return out;
}

std::string public_header(const Interface& iface, std::string_view file_name, const DeclarationAt& declaration_at) {
    const std::string_view stem = stem_of(file_name);
    const std::string guard = text::upper(text::sanitize_identifier(stem));
    const std::string head = text::lower(stem);
    std::vector<std::string> out;
    out.push_back(std::format("/* {}_public.h -- the functions {} exports, as reconstructed. Argument names and types are the", head,
                              iface.name));
    out.push_back(" * decompiler's; the export names and ordinals are the original's. */");
    out.push_back(std::format("#ifndef {}_PUBLIC_H", guard));
    out.push_back(std::format("#define {}_PUBLIC_H", guard));
    out.push_back("");
    for (const Export& e : iface.exports) {
        const std::string label = std::format("{} @{}", e.has_name() ? *e.name : std::string("(by ordinal only)"), e.ordinal);
        std::optional<std::string> declaration;
        if (e.kind == ExportKind::Code) declaration = declaration_at(e);
        if (declaration && !declaration->empty()) {
            out.push_back(std::format("{:<72} /* {} */", *declaration, label));
        } else {
            const std::string what = e.kind == ExportKind::Forward ? "forwarded to " + e.forward_target
                                     : e.kind == ExportKind::Data  ? "data"
                                                                   : "no reconstruction at its address";
            out.push_back(std::format("/* {}: {} */", label, what));
        }
    }
    out.push_back("");
    out.push_back("#endif");
    out.push_back("");
    return text::join(out, "\n");
}

}  // namespace cb::build::definitions
