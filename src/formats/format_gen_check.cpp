#include "formats/format_gen_check.hpp"

#include <algorithm>
#include <utility>

#include "core/error.hpp"

namespace cb::formats::gen {

// Written by format_gen into the build folder (formats_gen/formats/gen/registry.cpp): one entry per generated reader.
struct GeneratedReader {
    const char* format;
    std::vector<std::pair<std::string, std::string>> (*values)(ByteView data);
};
extern const GeneratedReader kGeneratedReaders[];

}  // namespace cb::formats::gen

namespace cb::formats::engine {

namespace {

using Values = std::vector<std::pair<std::string, std::string>>;

// The engine tree in the terms of the generated values(): a number or let its value, a str, bytes or struct
// "@offset+size", an array "[count]".
void gen_check_values(const Node& n, const std::string& path, Values& out) {
    switch (n.kind) {
        case Kind::Int:
        case Kind::Let: out.emplace_back(path, std::to_string(n.as_signed())); return;
        case Kind::Str:
        case Kind::Bytes:
        case Kind::Struct: out.emplace_back(path, "@" + std::to_string(n.offset) + "+" + std::to_string(n.size)); break;
        case Kind::Array: out.emplace_back(path, "[" + std::to_string(n.children.size()) + "]"); break;
    }
    for (const Node& c : n.children) {
        if (n.kind == Kind::Array) gen_check_values(c, path + "[" + c.name + "]", out);
        else gen_check_values(c, path.empty() ? c.name : path + "." + c.name, out);
    }
}

// What a read gives: its values, or the class of its error.
template <class F>
std::string gen_check_outcome(F&& read, Values& values) {
    try {
        values = read();
    } catch (const TruncatedError&) {
        return "truncated";
    } catch (const FormatError&) {
        return "format error";
    }
    std::sort(values.begin(), values.end());
    return "read (" + std::to_string(values.size()) + " values)";
}

}  // namespace

std::vector<GenComparison> compare_generated(const Registry& registry, ByteView data) {
    std::vector<GenComparison> out;
    for (const gen::GeneratedReader* g = gen::kGeneratedReaders; g->format; ++g) {
        if (!registry.has(g->format)) continue;
        GenComparison c;
        c.format = g->format;
        Values engine, generated;
        c.engine = gen_check_outcome(
            [&] {
                ReadOptions options;
                options.keep_source = false;
                const Tree tree = registry.read(g->format, data, options);
                Values v;
                gen_check_values(tree.root, "", v);
                return v;
            },
            engine);
        c.generated = gen_check_outcome([&] { return g->values(data); }, generated);
        c.agree = c.engine == c.generated && engine == generated;
        if (!c.agree && c.engine == c.generated) {
            const auto [a, b] = std::mismatch(engine.begin(), engine.end(), generated.begin(), generated.end());
            c.difference = a != engine.end() ? "engine " + a->first + " = " + a->second : "";
            if (b != generated.end()) c.difference += (c.difference.empty() ? "" : ", ") + std::string("generated ") + b->first + " = " + b->second;
        }
        out.push_back(std::move(c));
    }
    return out;
}

}  // namespace cb::formats::engine
