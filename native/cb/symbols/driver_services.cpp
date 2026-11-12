#include "cb/symbols/driver_services.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <tuple>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::driver_services {

namespace {

namespace ds_text = cb::text;

constexpr std::string_view kReader = "driver services";

[[noreturn]] void bad_table(const std::string& what) { throw FormatError(kReader, 0, what); }

std::uint32_t device_key(const std::string& key, std::string_view kind) {
    const auto n = ds_text::parse_int<std::uint32_t>(key, 16);
    if (!n) bad_table(std::format("the {} table has a key that is not a hex number: \"{}\"", kind, key));
    return *n;
}

std::string name_of(const knowledge::Value& v, std::string_view kind, const std::string& key) {
    if (v.is_null()) return {};
    const std::string* text = v.text();
    if (text == nullptr) bad_table(std::format("the {} table's entry {} is not a name", kind, key));
    return *text;
}

std::map<std::uint32_t, std::string> names_by_number(const knowledge::Rows& rows, std::string_view kind) {
    std::map<std::uint32_t, std::string> out;
    for (const auto& [key, value] : rows) out[device_key(key, kind)] = name_of(value, kind, key);
    return out;
}

}  // namespace

// ---- reading the table -------------------------------------------------------------------------------------------------

Table Table::from_rows(const knowledge::Rows& devices, const knowledge::Rows& services, const knowledge::Rows& drivers,
                       const knowledge::Rows& messages) {
    Table t;
    t.devices_ = names_by_number(devices, "device");
    t.drivers_ = names_by_number(drivers, "driver");
    t.messages_ = names_by_number(messages, "message");
    for (const auto& [key, value] : services) {
        const knowledge::Value::List* list = value.list();
        if (list == nullptr) bad_table(std::format("the service_table entry {} is not a list", key));
        std::vector<std::string>& names = t.services_[device_key(key, "service_table")];
        for (const knowledge::Value& v : *list) names.push_back(name_of(v, "service_table", key));
    }
    return t;
}

std::optional<std::string> Table::device_name(std::uint32_t device) const {
    for (const auto* names : {&devices_, &drivers_}) {
        const auto it = names->find(device);
        if (it != names->end() && !it->second.empty()) return it->second;
    }
    return std::nullopt;
}

std::optional<std::string> Table::service_name(std::uint32_t device, std::uint32_t service) const {
    const auto it = services_.find(device);
    if (it == services_.end()) return std::nullopt;
    const std::uint32_t n = service & kServiceNumberMask;
    if (n < it->second.size() && !it->second[n].empty()) return it->second[n];
    return std::nullopt;
}

std::string Table::label(std::uint32_t device, std::uint32_t service) const {
    if (const auto real = service_name(device, service)) return *real;
    const std::uint32_t n = service & kServiceNumberMask;
    if (const auto dev = device_name(device)) return std::format("{}_Service_{:04X}", ds_text::sanitize_identifier(*dev), n);
    return device == 1 ? std::format("VMM_{:04X}", n) : std::format("VxD_{:04X}_{:04X}", device, n);
}

std::optional<std::string> Table::message_name(std::uint32_t number) const {
    const auto it = messages_.find(number);
    if (it == messages_.end() || it->second.empty()) return std::nullopt;
    return it->second;
}

Table::Size Table::size() const {
    Size s;
    s.service_tables = services_.size();
    for (const auto& [device, names] : services_) {
        s.services_named += static_cast<std::size_t>(std::count_if(names.begin(), names.end(), [](const std::string& n) { return !n.empty(); }));
    }
    std::set<std::uint32_t> numbers;
    for (const auto& [n, name] : devices_) numbers.insert(n);
    for (const auto& [n, name] : drivers_) numbers.insert(n);
    s.devices_named = numbers.size();
    return s;
}

Coverage coverage(const Table& table, std::span<const ServiceCall> calls) {
    Coverage c;
    for (const ServiceCall& call : calls) {
        ++c.calls;
        if (table.service_name(call.device, call.service)) {
            ++c.named;
        } else {
            ++c.missing[{call.device, call.service & kServiceNumberMask}];
        }
    }
    return c;
}

std::string size_line(const Table::Size& size) {
    return std::format("{} service tables, {} services named, {} device numbers named", size.service_tables, size.services_named, size.devices_named);
}

std::string coverage_line(const Table& table, std::string_view file_name, const Coverage& c) {
    std::string out = std::format("{:<16} {:>4} service calls, {:>4} named", file_name, c.calls, c.named);
    std::size_t shown = 0;
    for (const auto& [key, n] : c.missing) {
        if (shown++ == 6) break;
        out += std::format("  {} x{}", table.label(key.first, key.second), n);
    }
    return out;
}

// ---- making the table -------------------------------------------------------------------------------------------------

namespace {

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_hex_digit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
bool is_decimal_digit(char c) { return c >= '0' && c <= '9'; }

std::size_t skip_space(std::string_view s, std::size_t at) {
    while (at < s.size() && ds_text::is_space(s[at])) ++at;
    return at;
}

std::size_t skip_word(std::string_view s, std::size_t at) {
    while (at < s.size() && is_word(s[at])) ++at;
    return at;
}

// The lines of a file as Python's text mode reads them: \n, \r\n and a lone \r end a line.
std::vector<std::string> file_lines(const std::filesystem::path& path) {
    const std::string text = ds_text::from_latin1(fs::read_file(path));
    std::vector<std::string> lines;
    std::string current;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (c == '\r' || c == '\n') {
            if (c == '\r' && i + 1 < text.size() && text[i + 1] == '\n') ++i;
            lines.push_back(std::move(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) lines.push_back(std::move(current));
    return lines;
}

constexpr std::string_view kIdSuffix = "_Device_ID";

// `NAME_Device_ID EQU value` or `#define NAME_Device_ID value`: the device's name as the file spells it and the value's text.
struct IdLine {
    std::string name;
    std::string value;
};

std::optional<IdLine> device_id_line(std::string_view line) {
    std::size_t at = skip_space(line, 0);
    // the C form
    if (ds_text::istarts_with(line.substr(at), "#define") && at + 7 < line.size() && ds_text::is_space(line[at + 7])) {
        std::size_t p = skip_space(line, at + 7);
        const std::size_t end = skip_word(line, p);
        const std::string_view word = line.substr(p, end - p);
        if (word.size() > kIdSuffix.size() && ds_text::iends_with(word, kIdSuffix) && end < line.size() && ds_text::is_space(line[end])) {
            const std::size_t v = skip_space(line, end);
            const std::size_t vend = skip_word(line, v);
            if (vend > v) return IdLine{std::string(word.substr(0, word.size() - kIdSuffix.size())), std::string(line.substr(v, vend - v))};
        }
    }
    // the assembly form: NAME_Device_ID, white space, EQU or =, white space, and the value up to a comment
    const std::size_t end = skip_word(line, at);
    const std::string_view word = line.substr(at, end - at);
    if (word.size() <= kIdSuffix.size() || !ds_text::iends_with(word, kIdSuffix)) return std::nullopt;
    if (end >= line.size() || !ds_text::is_space(line[end])) return std::nullopt;
    std::size_t p = skip_space(line, end);
    if (ds_text::istarts_with(line.substr(p), "equ")) {
        p += 3;
    } else if (p < line.size() && line[p] == '=') {
        p += 1;
    } else {
        return std::nullopt;
    }
    if (p >= line.size() || !ds_text::is_space(line[p])) return std::nullopt;
    const std::size_t blanks_end = skip_space(line, p);
    const std::size_t semicolon = std::min(line.find(';', blanks_end), line.size());
    std::string_view value;
    if (blanks_end < semicolon) {
        value = line.substr(blanks_end, semicolon - blanks_end);
    } else if (blanks_end - p >= 2) {
        value = line.substr(blanks_end - 1, 1);   // the value needs a character: the last blank gives one back
    } else {
        return std::nullopt;
    }
    return IdLine{std::string(word.substr(0, word.size() - kIdSuffix.size())), std::string(ds_text::trim(value))};
}

// `NAME EQU number` (the number: digits, then hex digits, then an optional H; a comment may follow): a plain equate.
struct Equate {
    std::string name;
    std::string number;
};

std::optional<Equate> equate_line(std::string_view line) {
    std::size_t at = skip_space(line, 0);
    const std::size_t end = skip_word(line, at);
    if (end == at || end >= line.size() || !ds_text::is_space(line[end])) return std::nullopt;
    std::size_t p = skip_space(line, end);
    if (ds_text::istarts_with(line.substr(p), "equ")) {
        p += 3;
    } else if (p < line.size() && line[p] == '=') {
        p += 1;
    } else {
        return std::nullopt;
    }
    if (p >= line.size() || !ds_text::is_space(line[p])) return std::nullopt;
    const std::size_t v = skip_space(line, p);
    if (v >= line.size() || !is_decimal_digit(line[v])) return std::nullopt;
    std::size_t q = v + 1;
    while (q < line.size() && is_hex_digit(line[q])) ++q;
    if (q < line.size() && (line[q] == 'h' || line[q] == 'H')) ++q;
    const std::string_view number = line.substr(v, q - v);
    const std::size_t rest = skip_space(line, q);
    if (rest < line.size() && line[rest] != ';') return std::nullopt;
    return Equate{std::string(line.substr(at, end - at)), std::string(number)};
}

// `Begin_Service_Table NAME` (NAME: the device's service prefix), when the line is not the macro's own definition.
std::optional<std::string> begin_table(std::string_view code) {
    const std::size_t at = skip_space(code, 0);
    constexpr std::string_view kBegin = "Begin_Service_Table";
    if (!ds_text::istarts_with(code.substr(at), kBegin)) return std::nullopt;
    const std::size_t blanks = at + kBegin.size();
    if (blanks >= code.size() || !ds_text::is_space(code[blanks])) return std::nullopt;
    const std::size_t v = skip_space(code, blanks);
    const std::size_t end = skip_word(code, v);
    if (end == v || ds_text::upper(code).find("MACRO") != std::string::npos) return std::nullopt;
    return std::string(code.substr(v, end - v));
}

bool is_end_table(std::string_view code) {
    const std::size_t at = skip_space(code, 0);
    constexpr std::string_view kEnd = "End_Service_Table";
    if (!ds_text::istarts_with(code.substr(at), kEnd)) return false;
    const std::size_t after = at + kEnd.size();
    return after >= code.size() || !is_word(code[after]);   // \b
}

// `NAME_Service  Service_Name` inside a table: the service's name.
std::optional<std::string> service_line(std::string_view code, std::string_view table_name) {
    const std::size_t at = skip_space(code, 0);
    if (!ds_text::istarts_with(code.substr(at), table_name)) return std::nullopt;
    std::size_t p = at + table_name.size();
    constexpr std::string_view kService = "_Service";
    if (!ds_text::istarts_with(code.substr(p), kService)) return std::nullopt;
    p += kService.size();
    if (p >= code.size() || !ds_text::is_space(code[p])) return std::nullopt;
    const std::size_t v = skip_space(code, p);
    const std::size_t end = skip_word(code, v);
    if (end == v) return std::nullopt;
    return std::string(code.substr(v, end - v));
}

}  // namespace

std::filesystem::path find_include_dir(std::string_view kit_folder, const std::filesystem::path& reference_folder) {
    std::vector<std::filesystem::path> candidates;
    if (!kit_folder.empty()) candidates.push_back(fs::from_text(kit_folder));
    candidates.push_back(reference_folder / "Microsoft Windows 95 DDK" / "INC32");
    candidates.push_back(reference_folder / "win95ddk" / "INC32");
    for (const std::filesystem::path& c : candidates) {
        std::error_code ec;
        if (std::filesystem::is_regular_file(c / "VMM.INC", ec)) return c;
    }
    return {};
}

std::optional<std::uint32_t> parse_device_number(std::string_view text, const std::map<std::string, std::uint32_t>& known) {
    std::string cleaned(text);
    std::replace(cleaned.begin(), cleaned.end(), '(', ' ');
    std::replace(cleaned.begin(), cleaned.end(), ')', ' ');
    std::uint64_t total = 0;
    for (const std::string_view raw : ds_text::split(cleaned, '+')) {
        const std::string_view part = ds_text::trim(raw);
        std::optional<std::uint64_t> value;
        if (part.starts_with("0x") && part.size() > 2 && std::all_of(part.begin() + 2, part.end(), is_hex_digit)) {
            value = ds_text::parse_int<std::uint64_t>(part.substr(2), 16);
            if (!value) return std::nullopt;
        } else if (part.size() > 1 && ds_text::iends_with(part, "h") && is_decimal_digit(part[0]) &&
                   std::all_of(part.begin(), part.end() - 1, is_hex_digit)) {
            value = ds_text::parse_int<std::uint64_t>(part.substr(0, part.size() - 1), 16);
            if (!value) return std::nullopt;
        } else if (!part.empty() && std::all_of(part.begin(), part.end(), is_decimal_digit)) {
            value = ds_text::parse_int<std::uint64_t>(part, 10);
            if (!value) return std::nullopt;
        } else if (const auto k = known.find(ds_text::upper(part)); k != known.end()) {
            value = k->second;
        } else {
            return std::nullopt;
        }
        total += *value;
        if (total > 0xFFFFFFFFull) return std::nullopt;
    }
    return static_cast<std::uint32_t>(total);
}

KitTables read_include_files(const std::filesystem::path& folder) {
    // the files: .INC first, then by name in capitals
    std::vector<std::pair<std::string, std::filesystem::path>> files;
    std::error_code ec;
    std::filesystem::directory_iterator it(folder, ec);
    if (ec) throw IoError(folder, ec.value(), "cannot list the folder: " + ec.message());
    for (const auto& entry : it) {
        const std::string name = entry.path().filename().string();
        if (ds_text::iends_with(name, ".INC") || ds_text::iends_with(name, ".H")) files.emplace_back(name, entry.path());
    }
    std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
        const bool a_inc = ds_text::iends_with(a.first, ".INC"), b_inc = ds_text::iends_with(b.first, ".INC");
        return a_inc != b_inc ? a_inc : ds_text::upper(a.first) < ds_text::upper(b.first);
    });
    std::vector<std::pair<std::string, std::vector<std::string>>> texts;
    for (const auto& [name, path] : files) texts.emplace_back(ds_text::upper(name), file_lines(path));

    // the equates, and the device ids still to be worked out (the first spelling of each name, in the order met)
    std::map<std::string, std::uint32_t> equates;
    std::vector<std::string> pending_order;
    std::unordered_map<std::string, std::string> pending, spelled, first;   // lookup only; the order is pending_order
    for (const auto& [file, lines] : texts) {
        for (const std::string& line : lines) {
            if (const auto id = device_id_line(line)) {
                const std::string key = ds_text::upper(id->name);
                if (pending.try_emplace(key, id->value).second) pending_order.push_back(key);
                spelled.try_emplace(key, id->name);
                first.try_emplace(key, file);
                continue;
            }
            if (const auto eq = equate_line(line)) {
                if (const auto v = parse_device_number(eq->number, {})) equates.try_emplace(ds_text::upper(eq->name), *v);
            }
        }
    }
    // a number may be another device's name: four rounds, each seeing what the one before worked out
    std::map<std::string, std::uint32_t> ids;
    for (int round = 0; round < 4; ++round) {
        std::map<std::string, std::uint32_t> known = equates;
        for (const auto& [name, v] : ids) known[name + "_DEVICE_ID"] = v;
        for (const std::string& name : pending_order) {
            if (ids.contains(name)) continue;
            if (const auto v = parse_device_number(pending.at(name), known)) ids[name] = *v;
        }
    }

    // the service tables by device name, then by file
    std::vector<std::string> table_order;
    std::map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> tables;
    for (const auto& [file, lines] : texts) {
        std::optional<std::string> current;
        for (const std::string& line : lines) {
            const std::string_view code = std::string_view(line).substr(0, line.find(';'));
            if (const auto begin = begin_table(code)) {
                current = begin;
                const std::string key = ds_text::upper(*begin);
                auto& by_file = tables[key];
                if (by_file.empty() && std::find(table_order.begin(), table_order.end(), key) == table_order.end()) table_order.push_back(key);
                if (std::none_of(by_file.begin(), by_file.end(), [&](const auto& f) { return f.first == file; })) by_file.emplace_back(file, std::vector<std::string>{});
                continue;
            }
            if (current && is_end_table(code)) {
                current.reset();
            } else if (current) {
                if (const auto service = service_line(code, *current)) {
                    for (auto& f : tables[ds_text::upper(*current)]) {
                        if (f.first == file) f.second.push_back(*service);
                    }
                }
            }
        }
    }

    KitTables out;
    // several names, one number: the kit's main list (VMM.INC) names it
    std::vector<std::string> by_origin;
    for (const auto& [name, v] : ids) by_origin.push_back(name);
    std::sort(by_origin.begin(), by_origin.end(), [&](const std::string& a, const std::string& b) {
        return std::make_tuple(first.at(a) != "VMM.INC", first.at(a), a) < std::make_tuple(first.at(b) != "VMM.INC", first.at(b), b);
    });
    for (const std::string& name : by_origin) {
        if (ids.at(name) > 0 && ids.at(name) < 0x10000) out.devices.try_emplace(ids.at(name), spelled.at(name));
    }
    for (const std::string& name : table_order) {
        const auto number = ids.find(name);
        const std::vector<std::string>* best = nullptr;   // the first longest
        for (const auto& [file, names] : tables.at(name)) {
            if (best == nullptr || names.size() > best->size()) best = &names;
        }
        if (number == ids.end() || best == nullptr || best->empty()) continue;
        const auto have = out.services.find(number->second);
        if (best->size() > (have == out.services.end() ? 0 : have->second.size())) {
            out.services[number->second] = *best;
            const auto spelling = spelled.find(name);
            out.devices[number->second] = spelling == spelled.end() ? name : spelling->second;
        }
    }
    return out;
}

std::map<std::uint32_t, std::string> read_messages(const std::filesystem::path& folder) {
    std::map<std::uint32_t, std::string> out;
    bool inside = false;
    for (const std::string& line : file_lines(folder / "VMM.INC")) {
        // NAME EQU 00xxH, from the start of the line
        const std::size_t end = skip_word(line, 0);
        if (end == 0 || end >= line.size() || !ds_text::is_space(line[end])) continue;
        std::size_t p = skip_space(line, end);
        if (!ds_text::istarts_with(std::string_view(line).substr(p), "EQU")) continue;
        p += 3;
        if (p >= line.size() || !ds_text::is_space(line[p])) continue;
        p = skip_space(line, p);
        const std::string_view value = std::string_view(line).substr(p);
        if (value.size() < 5 || !value.starts_with("00") || !is_hex_digit(value[2]) || !is_hex_digit(value[3]) ||
            (value[4] != 'H' && value[4] != 'h') || skip_space(value, 5) != value.size()) {
            continue;
        }
        const std::string name = ds_text::upper(line.substr(0, end));
        if (name == "SYS_CRITICAL_INIT") inside = true;
        if (name == "MAX_SYSTEM_CONTROL") break;
        if (inside) out.try_emplace(*ds_text::parse_int<std::uint32_t>(value.substr(2, 2), 16), name);
    }
    return out;
}

std::string format_table(const KitTables& kit, const std::map<std::uint32_t, std::string>& corpus_drivers,
                         const std::map<std::uint32_t, std::string>& messages) {
    auto hex_key = [](std::uint32_t n) { return std::format("{:04X}", n); };
    Json doc = Json::object();
    doc["source"] =
        "Windows 95 driver kit include files (INC32): Begin_Service_Table lists and *_Device_ID equates; \"drivers\" from the corpus "
        "drivers' own descriptors";
    Json devices = Json::object(), services = Json::object(), drivers = Json::object(), msgs = Json::object();
    for (const auto& [n, name] : kit.devices) devices[hex_key(n)] = name;
    for (const auto& [n, names] : kit.services) services[hex_key(n)] = names;
    for (const auto& [n, name] : corpus_drivers) {
        if (!kit.devices.contains(n)) drivers[hex_key(n)] = name;
    }
    for (const auto& [n, name] : messages) msgs[hex_key(n)] = name;
    doc["devices"] = devices;
    doc["services"] = services;
    doc["drivers"] = drivers;
    doc["messages"] = msgs;
    return doc.dump(1, ' ', true) + "\n";
}

}  // namespace cb::symbols::driver_services
