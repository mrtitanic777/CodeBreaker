// cb/source/classes_writer.cpp -- the C++ classes of a module, written as class declarations (classes_writer.hpp).

#include "cb/source/classes_writer.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::source::classes_writer {

namespace {

bool ascii_alnum(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

// A base that is a C++ name is written as it is; one known only as a numbered import of another library (`MFC42.800`) as a name made of
// its letters and digits.
std::string base_name(const std::string& base) {
    const bool is_name = !base.empty() && !(base[0] >= '0' && base[0] <= '9') && std::ranges::all_of(base, [](char c) { return c == '_' || ascii_alnum(c); });
    if (is_name) return base;
    std::string letters;
    for (const char c : base) {
        if (ascii_alnum(c)) letters += c;
    }
    return "imported_class_" + letters;
}

std::uint32_t be32(ByteView data, std::size_t at) {
    return static_cast<std::uint32_t>(data[at]) << 24 | static_cast<std::uint32_t>(data[at + 1]) << 16 | static_cast<std::uint32_t>(data[at + 2]) << 8 | data[at + 3];
}

}  // namespace

std::string classes_text(std::string_view module_file, std::string_view stem, std::span<const ClassRecord> classes, const FunctionNotes& notes) {
    const std::size_t named = static_cast<std::size_t>(std::ranges::count_if(classes, [](const ClassRecord& c) { return !c.name.starts_with("class_"); }));
    std::vector<std::string> lines = {
        std::format("// {} -- the C++ classes this module shows, as class declarations.", module_file),
        "//",
        std::format("// Read beside {}.c. Not compiled: each virtual function is the reconstructed function named", stem),
        "// in its comment, where its real parameters are. A class or function name the binary does",
        "// not state is a placeholder (class_<address of its table>, vf_<slot>).",
        "//",
        std::format("// {} classes; {} named by the binary itself.", classes.size(), named),
        ""};
    for (const ClassRecord& c : classes) {
        std::vector<std::string> bases;
        for (const std::string& b : c.bases) {
            if (!b.empty()) bases.push_back(base_name(b));
        }
        std::vector<std::string> facts = {std::format("{} virtual functions", c.methods.size()), std::format("table at {:x}", c.vtable)};
        if (c.size && *c.size != 0) facts.push_back(std::format("object of {:#x} bytes{}", *c.size, c.size_exact.value_or(true) ? "" : " at least"));
        if (c.com) facts.push_back("has the shape of a COM interface");
        if (c.message_map && *c.message_map != 0) facts.push_back(std::format("message map at {:x}", *c.message_map));
        lines.push_back("// " + text::join(facts, "; "));
        for (const std::string& e : c.evidence) lines.push_back("//   " + e);
        if (!c.installed_by.empty()) {
            std::vector<std::string> where;
            for (std::size_t i = 0; i < c.installed_by.size() && i < 8; ++i) where.push_back(notes.name_at(c.installed_by[i]));
            lines.push_back("//   constructed or destroyed in: " + text::join(where, ", "));
        }
        lines.push_back("class " + c.name + (bases.empty() ? std::string() : " : public " + bases[0]) + " {");
        if (bases.size() > 1) lines.push_back("    // further up: " + text::join(std::span<const std::string>(bases).subspan(1), " : "));
        lines.push_back("public:");
        for (std::size_t slot = 0; slot < c.methods.size(); ++slot) {
            const std::string status = notes.status_at(c.methods[slot]);
            const std::size_t pad = std::max<std::size_t>(1, 6 - std::to_string(slot).size());
            lines.push_back(std::format("    virtual int4 vf_{}();{}// {}{}", slot, std::string(pad, ' '), notes.name_at(c.methods[slot]),
                                        status.empty() ? std::string() : "  [" + status + "]"));
        }
        lines.push_back("};");
        lines.push_back("");
    }
    return text::join(lines, "\n");
}

std::optional<std::filesystem::path> write(std::span<const ClassRecord> classes, std::string_view module_file, std::string_view stem,
                                           const std::filesystem::path& out_dir, const FunctionNotes& notes) {
    if (classes.empty()) return std::nullopt;
    fs::make_dirs(out_dir);
    const std::filesystem::path out = out_dir / (std::string(stem) + "_classes.hpp");
    fs::write_file(out, as_bytes(classes_text(module_file, stem, classes, notes)));
    return out;
}

std::vector<ClassRecord> powerpc_tables(const PowerPcSource& image) {
    const std::set<std::uint32_t>& linked_functions = image.functions();
    const auto code = image.code().value_or(std::pair<std::uint32_t, std::uint32_t>{0, 0});
    // the table of function entries (.pdata) says by how much the addresses kept in the data differ from the functions': its first word
    // is the first function's address, as linked. The shift is the one that makes most of the table's words functions.
    std::uint32_t delta = 0;
    for (const PowerPcSource::Section& s : image.sections()) {
        if (s.name != ".pdata" || s.size < 8 || linked_functions.empty()) continue;
        const Bytes raw = image.bytes_at(s.va, s.size);
        std::set<std::uint32_t> table;
        for (std::size_t i = 0; i + 8 <= raw.size(); i += 8) {
            if (const std::uint32_t w = be32(raw, i); w != 0) table.insert(w);
        }
        const std::vector<std::uint32_t> linked(table.begin(), table.end());
        std::pair<std::size_t, std::uint32_t> best{0, 0};   // (words that are functions, the shift): the most, then the larger shift
        std::size_t starts_tried = 0;
        for (auto start = linked_functions.begin(); start != linked_functions.end() && starts_tried < 8; ++start, ++starts_tried) {
            for (std::size_t k = 0; k < linked.size() && k < 8; ++k) {
                const std::uint32_t d = *start - linked[k];
                std::size_t hits = 0;
                for (const std::uint32_t x : linked) hits += linked_functions.contains(x + d) ? 1 : 0;
                best = std::max(best, std::pair{hits, d});
            }
        }
        if (best.first * 2 >= linked.size()) delta = best.second;
    }
    std::set<std::uint32_t> starts;
    for (const std::uint32_t s : linked_functions) starts.insert(s - delta);

    std::vector<ClassRecord> out;
    for (const PowerPcSource::Section& s : image.sections()) {
        if (s.size == 0 || (s.va >= code.first && s.va < code.second) || s.name == ".pdata" || s.name == ".reloc" || s.name == ".edata" || s.name == ".idata") continue;
        const Bytes data = image.bytes_at(s.va, s.size);
        const std::size_t words = data.size() >= 8 ? data.size() / 4 : 0;
        std::vector<std::uint32_t> run;
        for (std::size_t i = 0; i <= words; ++i) {   // (a zero after the last word ends a run at the end)
            const std::uint32_t w = i < words ? be32(data, 4 * i) : 0;
            if (starts.contains(w)) {
                run.push_back(w);
                continue;
            }
            if (run.size() >= 2) {
                ClassRecord c;
                c.vtable = static_cast<Va>(s.va) + static_cast<Va>(4 * (i - run.size()));
                c.slots = static_cast<std::int64_t>(run.size());
                for (const std::uint32_t r : run) c.methods.push_back(static_cast<Va>(static_cast<std::uint32_t>(r + delta)));
                c.name = std::format("class_{:x}", c.vtable);
                c.evidence = {std::format("a table of {} of the image's own functions, in {}: a virtual table, or another table of functions "
                                          "(callbacks) -- the image does not say which",
                                          run.size(), s.name)};
                out.push_back(std::move(c));
            }
            run.clear();
        }
    }
    return out;
}

ImportThunks::ImportThunks(const formats::pe::Image& image) : image_(image) {
    try {
        for (const formats::pe::Import& i : image.imports()) slots_[i.slot_va] = {i.dll, i.name};
    } catch (const FormatError&) {   // (a damaged import table names no thunk)
        slots_.clear();
    }
}

ImportThunks::ImportThunks(const formats::pe::Image& image, std::span<const formats::pe::Import> imports) : image_(image) {
    for (const formats::pe::Import& i : imports) slots_[i.slot_va] = {i.dll, i.name};
}

std::optional<std::string> ImportThunks::behind(Va address, const symbols::ordinals::Ordinals& ordinals) const {
    const std::uint64_t base = image_.optional_header().image_base;
    if (address < 0 || static_cast<std::uint64_t>(address) < base) return std::nullopt;
    const std::optional<std::uint64_t> offset = image_.file_offset(static_cast<std::uint64_t>(address) - base);
    const ByteView data = image_.data();
    if (!offset || *offset + 6 > data.size() || data[*offset] != 0xFF || data[*offset + 1] != 0x25) return std::nullopt;   // jmp [slot]
    const std::uint64_t slot = static_cast<std::uint64_t>(data[*offset + 2]) | static_cast<std::uint64_t>(data[*offset + 3]) << 8 |
                               static_cast<std::uint64_t>(data[*offset + 4]) << 16 | static_cast<std::uint64_t>(data[*offset + 5]) << 24;
    const auto found = slots_.find(slot);
    if (found == slots_.end() || found->second.first.empty()) return std::nullopt;
    const std::string stem = text::upper(std::filesystem::path(found->second.first).stem().string());
    const std::string& name = found->second.second;
    if (name.size() > 1 && name[0] == '#' && std::ranges::all_of(name.substr(1), [](char c) { return c >= '0' && c <= '9'; })) {
        const std::string number = name.substr(1);
        const auto ordinal = text::parse_int<std::uint32_t>(number);
        const std::optional<std::string> known = ordinal ? ordinals.identifier(stem, *ordinal) : std::nullopt;
        return stem + "." + (known && !known->empty() ? *known : number);
    }
    return stem + "." + name;
}

}  // namespace cb::source::classes_writer
