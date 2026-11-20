// symbols/c_library.cpp -- see c_library.hpp.
#include "symbols/c_library.hpp"

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"

namespace cb::symbols::c_library {

namespace {

std::uint32_t size_of_kind(char kind, std::uint32_t pointer_bytes) {
    switch (kind) {
        case 'v': return 0;
        case 'l': case 'f': return 4;
        case 'd': return 8;
        case 'p': return pointer_bytes;
        default: return 2;   // char, int, unsigned: a word on the stack
    }
}

}  // namespace

std::uint32_t Prototype::result_bytes(std::uint32_t pointer_bytes) const {
    if (result == 'd' || result == 'f') return 0;
    return size_of_kind(result, pointer_bytes);
}

std::vector<std::string> Prototype::argument_sizes(std::uint32_t pointer_bytes) const {
    std::vector<std::string> out;
    if (variadic) return out;
    for (const char kind : arguments) {
        if (kind == 'p' && pointer_bytes == 4) out.push_back("4p");
        else out.push_back(std::to_string(size_of_kind(kind, pointer_bytes)));
    }
    return out;
}

CLibrary CLibrary::parse(std::string_view json_text, std::string_view source) {
    CLibrary library;
    Json doc;
    try {
        doc = Json::parse(std::string(json_text));
    } catch (const Json::exception& e) {
        throw ParseError(source, 0, 0, e.what());
    }
    const auto functions = doc.find("functions");
    if (functions == doc.end() || !functions->is_object()) throw ParseError(source, 0, 0, "no \"functions\" object");
    for (const auto& [name, row] : functions->items()) {
        if (!row.is_array() || (row.size() != 3 && row.size() != 4) || !row[0].is_string() || !row[1].is_string() || !row[2].is_string() ||
            row[1].get<std::string>().size() != 1 || (row.size() == 4 && !row[3].is_string()))
            throw ParseError(source, 0, 0, name + ": not [header, result, arguments, pointer types]");
        Prototype p;
        p.header = row[0].get<std::string>();
        p.result = row[1].get<std::string>()[0];
        const std::string kinds = row[2].get<std::string>();
        for (const std::string_view kind : text::split(kinds, ',')) {
            if (kind.empty()) continue;
            if (kind == "...") p.variadic = true;
            else p.arguments.push_back(kind[0]);
        }
        if (row.size() == 4) {
            const std::string types = row[3].get<std::string>();
            for (const std::string_view type : text::split(types, ',')) p.pointers.emplace_back(type);
            p.pointers.resize(p.arguments.size());
        }
        library.functions_.emplace(name, std::move(p));
    }
    return library;
}

CLibrary CLibrary::load(const std::filesystem::path& file) { return parse(fs::read_text(file), fs::to_text(file)); }

const Prototype* CLibrary::find(std::string_view name) const {
    const auto it = functions_.find(name);
    return it == functions_.end() ? nullptr : &it->second;
}

}  // namespace cb::symbols::c_library
