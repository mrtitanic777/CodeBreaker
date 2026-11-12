#include "cb/translate/backends/module_file.hpp"

#include <algorithm>
#include <format>

#include "cb/core/text.hpp"

namespace cb::translate::backends::module_file {

namespace {

namespace as = assembly;

std::vector<std::string> status_lines(const Facts& facts, std::string_view lead) {
    std::vector<std::string> out;
    const double total = static_cast<double>(std::max<std::size_t>(1, facts.total));
    for (const Status& s : facts.statuses)
        out.push_back(std::format("{}  {:<28} {:5d}  {:5.1f}%", lead, s.title, s.count,
                                  100.0 * static_cast<double>(s.count) / total));
    return out;
}

void append_lines(std::vector<std::string>& to, std::vector<std::string> more) {
    for (auto& s : more) to.push_back(std::move(s));
}

std::string whole_exports(const Whole& w) {
    if (w.exports_same == w.exports_total) return "all the original's exports match";
    return std::format("{} of {} exports match", w.exports_same, w.exports_total);
}

std::string document_name(const ir::Node* node) {
    return std::format("document {}", node != nullptr ? ir::kind_name(*node) : std::string_view("None"));
}

}  // namespace

Result<std::vector<Part>> parts(const ir::Node* module) {
    const auto* m = ir::get<ir::Module>(module);
    if (m == nullptr) return as::Declined(document_name(module));
    std::vector<Part> out;
    out.reserve(m->units.size());
    for (const ir::Node* unit : m->units) {
        if (const auto* method = ir::get<ir::MethodDecl>(unit)) {
            out.push_back({method->comments.empty() ? std::string() : method->comments.front(), method->text.value_or("")});
        } else if (const auto* comment = ir::get<ir::Comment>(unit)) {
            out.push_back({comment->text.value_or(""), std::nullopt});
        } else {
            return as::Declined(document_name(unit));
        }
    }
    return out;
}

std::vector<std::string> banner(const Facts& facts) {
    const bool powerpc = facts.lane == Lane::PowerPc;
    std::vector<std::string> out = {
        std::format("/* {} -- reconstructed source.", facts.name), " *",
        std::format(" * STATUS, generated {} by CodeBreaker (verifier settings {}).", facts.date, facts.digest),
        std::format(" * {} functions:", facts.total)};
    append_lines(out, status_lines(facts, " * "));
    if (facts.whole && facts.whole->linked) {
        const Whole& w = *facts.whole;
        append_lines(out, {" *",
                           std::format(" * Builds as a whole module: YES ({}). Every original export is present; a function the",
                                       whole_exports(w)),
                           " * compiler rejected, or a call to code with no reconstruction, is a stand-in that returns 0",
                           std::format(" * ({} rejected, {} absent). The built image is {}.", w.rejected, w.absent, w.built),
                           " * Runs in place of the original: NOT TESTED as a whole.", " *"});
    } else {
        append_lines(
            out,
            {" *",
             " * Builds as a whole module: " + (facts.whole ? std::format("NOT YET ({}).", facts.whole->error)
                                                             : std::format("not built here (run `codebreaker build {}`).", facts.name)),
             " * Functions are compiled and checked one at a time; the module's data (globals, tables,",
             " * resources) is referred to by its address in the original unless the whole module is built.",
             " * Runs in place of the original: NOT TESTED.", " *",
             std::format(" * VERIFIED = rebuilt from this text with {} and run beside the original in {}:",
                         powerpc ? "a PowerPC compiler" : "OpenWatcom", powerpc ? "a PowerPC interpreter" : "an x86 emulator"),
             " * same return value, same calls out of the module with the same arguments, same memory changed,",
             " * same stack and saved registers, on every input tried. It is evidence, not proof. Names of",
             " * internal functions (sub_...) and locals are placeholders; no original names are claimed.",
             " * Written in after judging, changing no code: the SDK's names for constants passed to",
             std::format(" * Windows (with their values in {}.h), and names for locals, parameters and globals", facts.stem),
             " * taken from the Windows functions they are passed to or hold the result of.",
             " * IDENTIFIED = the function's bytes are those of a function in a library that came with the",
             " * compiler; it is named after that function. It is reconstructed and judged all the same."});
    }
    if (facts.real != 0) {
        append_lines(out, {std::format(" * Imports by number are written with their real names ({} of them; {}.h lists the",
                                       facts.real, facts.stem),
                           " * module and number of each). The text that was built and judged calls them MODULE_number."});
    }
    out.push_back(" */");
    out.push_back("");
    if (facts.header) out.push_back(std::format("#include \"{}.h\"", facts.stem));
    out.push_back("");
    return out;
}

std::string part_text(const Part& part) {
    // a status that holds "*/" would end its own comment and leave the rest as C: the text is broken apart there
    const std::string status = text::replace_all(part.status, "*/", "* /");
    if (!part.text) return std::format("/* {} */\n", status);
    return std::format("/* {} */\n{}\n", status, *part.text);
}

std::vector<File> c(std::span<const Part> parts, const Facts& facts, std::size_t split) {
    const std::vector<std::string> head = banner(facts);
    std::vector<std::string> texts;
    texts.reserve(parts.size());
    for (const Part& p : parts) texts.push_back(part_text(p));

    // one file, or runs of `split` functions
    const bool whole = split == 0 || texts.size() <= split;
    const std::size_t per = whole ? texts.size() : split;
    const std::size_t count = whole ? 1 : (texts.size() + split - 1) / split;

    std::vector<File> out;
    for (std::size_t n = 0; n < count; ++n) {
        const std::size_t from = n * per;
        const std::size_t to = std::min(texts.size(), from + per);
        std::vector<std::string> top;
        if (n == 0) {
            top = head;
        } else {
            top = {std::format("/* {} -- reconstructed source, part {} of {} (the status is in {}_1.c). */", facts.name, n + 1,
                               count, facts.stem),
                   ""};
            for (const std::string& line : head)
                if (line.starts_with("#include")) top.push_back(line);
            top.push_back("");
        }
        const std::vector<std::string> piece(texts.begin() + static_cast<std::ptrdiff_t>(from),
                                             texts.begin() + static_cast<std::ptrdiff_t>(to));
        out.push_back({count == 1 ? facts.stem + ".c" : std::format("{}_{}.c", facts.stem, n + 1),
                       text::join(top, "\n") + "\n" + text::join(piece, "\n")});
    }
    return out;
}

Result<std::vector<as::Unit>> source_units(const ir::Node* module) {
    const auto* m = ir::get<ir::Module>(module);
    if (m == nullptr) return as::Declined(document_name(module));
    std::vector<as::Unit> out;
    out.reserve(m->units.size());
    for (const ir::Node* unit : m->units) {
        if (const auto* f = ir::get<ir::MachineFragment>(unit)) {
            out.push_back(as::MachineFragment{f->name.value_or(""), f->assembly, f->within.has_value()});
        } else if (const auto* c = ir::get<ir::Comment>(unit)) {
            out.push_back(as::Comment{c->text.value_or(""), ""});
        } else {
            out.push_back(as::OtherUnit{unit != nullptr ? std::string(ir::kind_name(*unit)) : "None"});
        }
    }
    return out;
}

std::vector<std::string> asm_banner(const Facts& facts) {
    std::vector<std::string> out = {std::format("{} -- reconstructed source.", facts.name), "",
                                    std::format("STATUS, generated {} by CodeBreaker (verifier settings {}).", facts.date, facts.digest),
                                    std::format("{} entry points or functions:", facts.total)};
    append_lines(out, status_lines(facts, ""));
    if (!facts.how.empty()) {
        out.push_back("");
        out.emplace_back(text::utf8_prefix(facts.how, 160));
    }
    for (const Row& r : facts.rows) out.push_back(std::format("  {:<10} {:<28} {} {}", r.address, r.name, r.title, r.what));
    out.push_back("");
    return out;
}

Result<std::string> assembly_file(std::span<const as::Unit> source, const Facts& facts) {
    as::Document doc;
    for (const std::string& line : asm_banner(facts)) doc.units.push_back(as::Comment{line, ""});
    if (source.empty()) {
        doc.units.push_back(as::MachineFragment{"", "; (no source could be written)\n", false});
    } else {
        doc.units.insert(doc.units.end(), source.begin(), source.end());
    }
    return as::module(doc);
}

}  // namespace cb::translate::backends::module_file
