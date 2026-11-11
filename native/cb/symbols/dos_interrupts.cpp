// cb/symbols/dos_interrupts.cpp -- names for DOS and BIOS services (see dos_interrupts.hpp).
#include "cb/symbols/dos_interrupts.hpp"

#include <cctype>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/knowledge/sources.hpp"

namespace cb::symbols::dos_interrupts {
namespace {

using std::string;
using std::string_view;

[[noreturn]] void bad_shape(string_view source, const string& what) { throw ParseError(source, 0, 0, what); }

const Json& member(const Json& object, const char* key, string_view source, const string& where) {
    const auto it = object.find(key);
    if (it == object.end()) bad_shape(source, where + ": no \"" + key + "\"");
    return *it;
}

string text_of(const Json& j, string_view source, const string& where) {
    if (!j.is_string()) bad_shape(source, where + ": not a string");
    return j.get<string>();
}

// ["Name", "register arguments"]
template <class Hit>
Hit hit_of(const Json& j, string_view source, const string& where) {
    if (!j.is_array() || j.size() != 2) bad_shape(source, where + ": not a [name, arguments] pair");
    return Hit{text_of(j[0], source, where), text_of(j[1], source, where)};
}

string hex2(std::uint32_t v) { return std::format("{:02X}", v); }

}  // namespace

ServiceTable ServiceTable::parse(std::string_view json_text, std::string_view source) {
    const Json doc = json::parse(json_text);
    if (!doc.is_object()) bad_shape(source, "the table is not an object");
    ServiceTable table;
    for (const auto& [key, row] : doc.items()) {
        const auto number = text::parse_int<std::uint32_t>(key, 16);
        if (!number) bad_shape(source, "interrupt \"" + key + "\" is not a hex number");
        if (!row.is_object()) bad_shape(source, "interrupt " + key + ": not an object");
        Entry entry;
        entry.name = text_of(member(row, "name", source, key), source, key);
        if (const auto it = row.find("arguments"); it != row.end()) entry.arguments = text_of(*it, source, key);
        if (const auto it = row.find("services"); it != row.end()) {
            entry.has_services = true;
            if (!it->is_object()) bad_shape(source, key + ": services is not an object");
            for (const auto& [ah, hit] : it->items()) entry.services.emplace(ah, hit_of<Hit>(hit, source, key + "/" + ah));
        }
        if (const auto it = row.find("select"); it != row.end()) entry.select_ax = text_of(*it, source, key) == "AX";
        if (const auto it = row.find("al"); it != row.end()) {
            if (!it->is_object()) bad_shape(source, key + ": al is not an object");
            for (const auto& [ah, subs] : it->items()) {
                if (!subs.is_object()) bad_shape(source, key + "/" + ah + ": not an object");
                for (const auto& [al, hit] : subs.items()) {
                    entry.al[ah].emplace(al, hit_of<Hit>(hit, source, key + "/" + ah + "/" + al));
                }
            }
        }
        table.entries_[*number] = std::move(entry);
    }
    return table;
}

ServiceTable ServiceTable::load(const std::filesystem::path& file) {
    return parse(fs::read_text(file), fs::to_text(file));
}

const ServiceTable& ServiceTable::standard() {
    static const ServiceTable table =
        load(knowledge::DataFolders::from_environment().symbols / "dos_interrupts.json");
    return table;
}

Service ServiceTable::name(std::uint32_t number, std::optional<std::uint32_t> ah, std::optional<std::uint32_t> al) const {
    const auto found = entries_.find(number);
    if (found == entries_.end()) return {std::format("int_{:02X}h", number), "", false};
    const Entry& entry = found->second;
    if (!entry.has_services) return {entry.name, entry.arguments, true};
    if (entry.select_ax) ah = ah == 0u ? al : std::nullopt;   // the mouse: the number is all of AX
    if (!ah) return {entry.name, "service number not a constant here", false};
    const Hit* hit = nullptr;
    if (al) {   // a subfunction table, when the service has one
        if (const auto sub = entry.al.find(hex2(*ah)); sub != entry.al.end()) {
            if (const auto s = sub->second.find(hex2(*al)); s != sub->second.end()) hit = &s->second;
        }
    }
    if (hit == nullptr) {
        if (const auto s = entry.services.find(hex2(*ah)); s != entry.services.end()) hit = &s->second;
    }
    if (hit == nullptr) return {std::format("{}_AH_{:02X}h", entry.name, *ah), "", false};
    return {entry.name + "_" + hit->name, hit->arguments, true};
}

std::string ServiceTable::describe(const Interrupt& interrupt) const {
    const Service s = name(interrupt.number, interrupt.ah, interrupt.al);
    string regs;
    if (interrupt.ah) regs += std::format(" AH={:02X}h", *interrupt.ah);
    if (interrupt.al) regs += std::format(" AL={:02X}h", *interrupt.al);
    return std::format("int {:02X}h{}  {}{}", interrupt.number, regs, s.name,
                       s.arguments.empty() ? string() : "(" + s.arguments + ")");
}

std::string ServiceTable::annotate(std::string_view text, std::span<const Interrupt> interrupts) const {
    if (interrupts.empty()) return string(text);
    std::map<std::uint32_t, std::set<string>> by_number;   // the services each interrupt is asked for, in this function
    for (const Interrupt& i : interrupts) by_number[i.number].insert(name(i.number, i.ah, i.al).name);

    string out = "/* interrupt calls, in address order:";
    for (const Interrupt& i : interrupts) out += std::format("\n     {:05X}  {}", i.address, describe(i));
    out += " */\n";
    // `swi(0xNN)` gets its service as a comment where the function asks that interrupt for one service only
    const size_t n = text.size();
    size_t i = 0;
    while (i < n) {
        const size_t at = text.find("swi(0x", i);
        if (at == string_view::npos) break;
        out.append(text.substr(i, at - i));
        size_t p = at + 6;
        const size_t digits = p;
        const auto is_hex = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); };
        while (p < n && is_hex(text[p])) ++p;
        const bool call = p > digits && p < n && text[p] == ')' && (at == 0 || !(std::isalnum(static_cast<unsigned char>(text[at - 1])) || text[at - 1] == '_'));
        if (!call) {   // not a call (or part of a longer word): copied as it is
            out.append(text.substr(at, 6));
            i = at + 6;
            continue;
        }
        out.append(text.substr(at, p + 1 - at));
        const auto number = text::parse_int<std::uint32_t>(text.substr(digits, p - digits), 16);
        if (number) {
            if (const auto names = by_number.find(*number); names != by_number.end() && names->second.size() == 1) {
                out += " /* " + *names->second.begin() + " */";
            }
        }
        i = p + 1;
    }
    out.append(text.substr(i));
    return out;
}

std::size_t ServiceTable::service_count() const {
    std::size_t total = 0;
    for (const auto& [number, entry] : entries_) {
        std::size_t rows = entry.services.size();
        for (const auto& [ah, subs] : entry.al) rows += subs.size();
        total += rows != 0 ? rows : 1;
    }
    return total;
}

int run(const ServiceTable& table, std::span<const std::string> args) {
    if (args.empty()) {
        out("{} interrupts, {} services\n", table.interrupt_count(), table.service_count());
        return kExitOk;
    }
    std::uint32_t numbers[3] = {0, 0, 0};
    const std::size_t given = std::min<std::size_t>(args.size(), 3);
    for (std::size_t i = 0; i < given; ++i) {
        const auto v = text::parse_int<std::uint32_t>(args[i], 16);
        if (!v) {
            err("dos-interrupts: \"{}\" is not a hex number\n", args[i]);
            return kExitUsage;
        }
        numbers[i] = *v;
    }
    const Service s = table.name(numbers[0], given > 1 ? std::optional(numbers[1]) : std::nullopt,
                                 given > 2 ? std::optional(numbers[2]) : std::nullopt);
    out("{}{}{}\n", s.name, s.arguments.empty() ? string() : "(" + s.arguments + ")", s.exact ? string() : "   [not exact]");
    return kExitOk;
}

}  // namespace cb::symbols::dos_interrupts
