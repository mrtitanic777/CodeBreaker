// build/module_build32.cpp -- link a 32-bit Windows module (a PE image) from its reconstruction.
//
// Every function is compiled on its own against the module's header, the module's data is C laid out as the original has it (build/data),
// and the linker is told the original's base, subsystem, imports (by name, by ordinal, or by a name C cannot spell) and exports.

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "build/module_internal.hpp"
#include "core/fs.hpp"
#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "formats/omf.hpp"
#include "formats/pe.hpp"
#include "translate/l2.hpp"

namespace cb::build::module {

using namespace internal;

namespace ct = decompile::c_text;

std::optional<stdfs::path> ModuleBuilder::build32(Built& b, const stdfs::path& original, const ct::Compilable32& m, const stdfs::path& folder) const {
    const Services& s = services_;
    const watcom::OpenWatcom& ow = s.open_watcom;
    const stdfs::path wcc = ow.cc(watcom::Target::Win32), wlink = ow.tool("wlink");
    if (wcc.empty()) throw UsageError("OpenWatcom's 32-bit compiler is not set up (settings.json: openwatcom)");
    const Env env = ow.environment(watcom::Target::Win32);
    const functions::Compiler cc{wcc, env};
    functions::FunctionBuilder& builder = *s.builder;
    std::vector<std::string> cflags;
    for (std::string_view f : ct::cflags()) cflags.emplace_back(f);
    cflags.push_back("-e=20");
    const formats::pe::Image pe = formats::pe::Image::open(original);
    const std::uint64_t image_base = pe.optional_header().image_base;
    const definitions::Interface iface = s.interface_of(original);

    std::map<std::string, std::uint64_t> va;   // the inverse of by_va
    for (const auto& [address, name] : m.by_va) va[name] = static_cast<std::uint64_t>(address);
    std::vector<std::string> placed;
    for (const std::string& n : m.order) {
        if (va.contains(n)) placed.push_back(n);
    }
    b.with_text = placed.size();
    std::map<std::uint64_t, std::string> at;
    for (const std::string& n : placed) at[va.at(n)] = n;
    const auto extra = [&](const std::string& stem, const std::string& text) { return builder.compile(cc, cflags, folder, stem, text); };
    const data::FunctionAt32 at_get = [&](std::uint64_t address) -> std::optional<std::string> {
        const auto it = at.find(address);
        return it == at.end() ? std::nullopt : std::optional<std::string>(it->second);
    };
    const std::string file_name = original.filename().string();

    // Exported data is cut out of its section as an object under its own name. That is tried first; if the compiler does not keep the pieces of a
    // section together, the sections are written whole and the data exports are reported missing.
    std::map<std::uint64_t, std::string> exported;
    for (const definitions::Export& e : iface.exports) {
        const auto* address = std::get_if<std::uint64_t>(&e.address);
        if (e.kind == definitions::ExportKind::Data && e.has_name() && text::is_identifier(*e.name) && !m.texts.contains(*e.name) && address != nullptr)
            exported[*address] = *e.name;
    }
    std::string header = m.preamble;
    functions::Texts texts(m.texts.begin(), m.texts.end());
    std::optional<std::string> data_object;
    std::vector<std::map<std::uint64_t, std::string>> attempts;
    if (!exported.empty()) attempts.push_back(exported);
    attempts.emplace_back();
    std::shared_ptr<data::Data> d;
    for (const auto& named : attempts) {
        d = std::make_shared<data::Data>(data::data32(pe, file_name, at_get, named));
        b.data = d;
        if (!d->movable) {
            b.data_error = "the module has no relocation table, so nothing says which numbers are addresses";
            exported.clear();
            break;
        }
        const data::Relocated relocated = data::relocate_header(m.preamble, *d);
        header = relocated.text;
        b.lost_addresses = relocated.lost.size();   // globals that are in no carried section
        fs::write_text(folder / "m.h", header);
        const std::string why = extra("data", d->source);
        if (!why.empty()) {
            b.data_error = "the data does not compile: " + why;
        } else if (data::layout_holds(fs::read_file(folder / "data.obj"), *d)) {
            data_object = "data.obj";
            exported = named;
            break;
        } else {
            b.data_error = "the compiler did not keep the pieces of a section together";
        }
    }
    if (data_object) {
        b.data_error.clear();
        const std::vector<std::uint64_t> starts = s.functions32 ? s.functions32(original) : std::vector<std::uint64_t>();   // every function the lane carves
        const auto constants = data::address_constants(pe, starts);
        texts.clear();
        for (const std::string& n : placed) {
            const auto addresses = constants.find(va.at(n));
            const data::Relocated relocated = data::relocate_text(m.texts.at(n), addresses != constants.end() ? addresses->second : std::set<std::uint64_t>(), *d, at_get);
            texts[n] = relocated.text;
            b.lost_addresses += relocated.lost.size();
        }
    } else {
        header = m.preamble;
        exported.clear();
    }
    const functions::Compiled compiled = builder.compile_all(cc, cflags, header, texts, placed, folder);
    const std::map<std::string, std::string>& stems = compiled.stems;
    b.rejected = compiled.dropped;
    std::vector<std::string> objects;
    for (const std::string& n : placed) {
        if (const auto stem = stems.find(n); stem != stems.end()) objects.push_back(stem->second + ".obj");
    }
    if (data_object) objects.push_back(*data_object);
    const std::map<std::string, PublicSymbol> publics = publics_of(s, folder, stems);
    std::optional<std::string> entry;
    if (pe.header().entry_rva != 0) entry = at_get(image_base + pe.header().entry_rva);
    std::optional<std::string> start;
    if (entry && publics.contains(*entry)) {
        start = publics.at(*entry).symbol;
        b.entry = *entry + ", the function at the original's entry point";
    } else if (pe.header().is_dll()) {
        const std::string why = extra("entry", "int __stdcall codebreaker_entry(int4 module, int4 reason, int4 reserved)\n{\n  return 1;\n}\n");
        if (!why.empty()) {   // the header itself does not compile
            b.error = "the module's header does not compile: " + why;
            return std::nullopt;
        }
        objects.push_back("entry.obj");
        start = "_codebreaker_entry@12";
        b.entry = pe.header().entry_rva != 0 ? "a stand-in that reports success (the original's entry point has no reconstruction)"
                                              : "a stand-in that reports success (the original has no entry point)";
    } else {
        b.entry = "NONE: the program's entry point has no reconstruction";
    }
    constexpr std::uint16_t kConsoleSubsystem = 3;
    std::vector<std::string> lines = {std::string("format windows nt") + (pe.header().is_dll() ? " dll" : ""),
                                      std::string("runtime ") + (pe.optional_header().subsystem == kConsoleSubsystem ? "console" : "windows") + "=4.0",
                                      "name m.dll",
                                      "option quiet, map, caseexact",
                                      std::format("option modname='{}'", iface.name),
                                      std::format("option offset=0x{:X}", image_base),
                                      "libpath " + fs::to_text(ow.lib_dir(watcom::Target::Win32))};
    if (start && !start->empty()) lines.push_back("option start=" + quote_symbol(*start));
    for (const std::string& o : objects) lines.push_back("file " + o);
    if (!resources::read_module(original).resources.empty()) {
        b.resources = resources::rebuild(ow, original, folder);
        if (b.resources->link_res) lines.push_back("option resource=" + b.resources->link_res->filename().string());
    }
    // the functions the original imports by name, bound to the same DLL; then those by ordinal (an ordinal is written as a bare number: `'MOD'.'@17'`
    // would be read as "take directives from the file 17"). An import whose own name is not a C name -- `_Name@12`, a C++ name -- is in the C text
    // with every other character turned into `_`; it is bound back here.
    static const Regex ordinal_import(R"(_?([A-Za-z0-9]+)_ord_(\d+)(?:@\d+)?)");
    const auto imported_by_ordinal = [&](const std::string& module, std::uint32_t ordinal) {
        return std::any_of(iface.imports.begin(), iface.imports.end(), [&](const definitions::Import& i) {
            const auto* number = std::get_if<std::uint32_t>(&i.what);
            return i.module == module && number != nullptr && *number == ordinal;
        });
    };
    std::map<std::string, std::pair<std::string, std::string>> odd;   // C spelling -> (module, the name)
    for (const definitions::Import& i : iface.imports) {
        const auto* name = std::get_if<std::string>(&i.what);
        if (name != nullptr && !text::is_identifier(*name)) odd[text::sanitize_identifier(*name)] = {i.module, *name};
    }
    const std::set<std::string> import_names(m.imports.begin(), m.imports.end());
    std::set<std::string> seen;
    for (const std::string& o : objects) {
        const Bytes obj = fs::read_file(folder / o);
        std::vector<std::string> binds = s.import_lines32 ? s.import_lines32(obj, original, import_names) : std::vector<std::string>();
        for (const std::string& sym : formats::omf::externs(obj)) {
            if (const auto mm = ordinal_import.full_match(sym)) {
                const std::string module = text::upper(mm->group(1));
                const std::uint32_t ordinal = text::parse_int<std::uint32_t>(mm->group(2)).value_or(0);
                if (imported_by_ordinal(module, ordinal)) binds.push_back(std::format("import '{}' '{}'.{}", sym, module, ordinal));
            }
            std::string plain = sym.starts_with("_") ? sym.substr(1) : sym;
            if (const std::size_t at_sign = plain.rfind('@'); at_sign != std::string::npos && at_sign + 1 < plain.size() &&
                std::all_of(plain.begin() + static_cast<std::ptrdiff_t>(at_sign) + 1, plain.end(), [](char ch) { return ch >= '0' && ch <= '9'; }))
                plain.erase(at_sign);   // @\d+$
            if (const auto name = odd.find(plain); name != odd.end()) binds.push_back(std::format("import '{}' '{}'.'{}'", sym, name->second.first, name->second.second));
        }
        for (const std::string& line : binds) {
            if (!line.empty() && seen.insert(line).second) lines.push_back(line);
        }
    }
    const definitions::SymbolAt symbol_at = [&](const definitions::Export& e) -> std::optional<std::string> {
        const auto* address = std::get_if<std::uint64_t>(&e.address);
        const std::uint64_t addr = address != nullptr ? *address : 0;
        if (e.kind == definitions::ExportKind::Data) {
            const auto it = exported.find(addr);
            return it == exported.end() ? std::nullopt : std::optional<std::string>("_" + it->second);
        }
        const auto fn = at.find(addr);
        if (fn == at.end() || !publics.contains(fn->second)) return std::nullopt;
        return publics.at(fn->second).symbol;
    };
    {
        definitions::Directives directives = definitions::export_directives(iface, symbol_at);
        lines.insert(lines.end(), directives.lines.begin(), directives.lines.end());
        b.exports_missing = std::move(directives.missing);
    }
    for (std::string_view x : text::split(watcom::kRuntimeLibrary, '\n')) {
        if (!text::trim(x).empty()) lines.emplace_back(text::trim(x));
    }
    const std::optional<stdfs::path> image = link_resolving(
        b, wlink, env, folder, lines, [&](const std::string& symbol) { return stand_in_from_header(header, symbol); }, extra);
    b.stand_in_addresses.clear();
    for (const auto& [name, why] : b.rejected) b.stand_in_addresses.insert(Address(va.at(name)));
    // (each export declared as the back end wrote its function's heading from L2: m.declarations, as the 16-bit build does -- not read back off the
    // text; a function with no C has none)
    const std::string base = text::lower(original.stem().string());
    fs::write_text(folder / (base + ".def"), definitions::definition_file(iface, file_name));
    fs::write_text(folder / (base + "_public.h"), definitions::public_header(iface, file_name, [&](const definitions::Export& e) -> std::optional<std::string> {
        const auto* address = std::get_if<std::uint64_t>(&e.address);
        const auto fn = at.find(address != nullptr ? *address : 0);
        if (fn == at.end()) return std::nullopt;
        const auto declared = m.declarations.find(fn->second);
        return declared == m.declarations.end() ? std::nullopt : std::optional<std::string>(declared->second);
    }));
    // the module as one L2 tree, whatever each function came back as (docs/IR_UNIFICATION.md step 4)
    b.l2_arena = std::make_shared<Arena>();
    b.l2 = module_document32(*b.l2_arena, b.name, m);
    json::save(folder / "module.l2.json", translate::l2::to_json(*b.l2), json::Style::PythonCompatible);
    return image;
}

}  // namespace cb::build::module
