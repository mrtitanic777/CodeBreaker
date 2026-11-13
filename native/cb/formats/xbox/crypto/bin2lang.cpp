#include "cb/formats/xbox/crypto/bin2lang.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/format_key.hpp"

namespace cb::formats::xbox::crypto::bin2lang {

namespace {

// The rows of the array body: a tab, the bytes of one line, a comma after every row but the last.
std::string rows_of(ByteView data, std::size_t per_line) {
    std::string out;
    for (std::size_t at = 0; at < data.size(); at += per_line) {
        const std::size_t end = std::min(data.size(), at + per_line);
        out += '\t';
        for (std::size_t i = at; i < end; ++i) out += std::format("{}0x{:02X}", i == at ? "" : ", ", data[i]);
        out += end < data.size() ? ",\n" : "\n";
    }
    return out;
}

}  // namespace

std::optional<Language> language_from_name(std::string_view name) {
    const std::string n = text::lower(name);
    if (n == "python" || n == "py") return Language::Python;
    if (n == "c") return Language::C;
    if (n == "c++" || n == "cpp" || n == "cplusplus") return Language::Cpp;
    if (n == "csharp" || n == "cs" || n == "c#") return Language::CSharp;
    if (n == "php" || n == "php-new") return Language::Php;
    if (n == "php-old") return Language::PhpOld;
    return std::nullopt;
}

std::string format(ByteView data, Language language, std::string_view variable, std::size_t per_line) {
    if (data.empty()) throw FormatError("bin2lang", 0, "nothing to embed: the input is empty");
    per_line = std::max<std::size_t>(per_line, 1);
    const std::string rows = rows_of(data, per_line);
    switch (language) {
        case Language::Python: return std::format("{} = bytearray([\n{}])\n", variable, rows);
        case Language::C:
        case Language::Cpp: return format_key::byte_array_source(data, variable, per_line);
        case Language::CSharp: return std::format("#region {0}\nbyte[] {0} = {{\n{1}}};\n#endregion\n", variable, rows);
        case Language::Php: return std::format("${} = [\n{}];\n", variable, rows);
        case Language::PhpOld: return std::format("${} = array(\n{});\n", variable, rows);
    }
    return {};
}

cli::Command command() {
    return cli::Command{
        "bin2lang",
        "a binary as an array in Python, C, C++, C# or PHP source",
        {cli::Positional{"input", "the binary to embed"},
         cli::Positional{"output", "the file to write the source to"},
         cli::Option{"--language", "-l", "python (default), c, c++, csharp, php, php-old", cli::Arg::Value, "LANGUAGE", "python"},
         cli::Option{"--bytes", "-b", "bytes per line (default 16)", cli::Arg::Value, "N", "16"},
         cli::Option{"--variable", "-v", "the name of the variable (default output)", cli::Arg::Value, "NAME", "output"}},
        [](const cli::Args& args) {
            const std::string name = args.required("language");
            const std::optional<Language> language = language_from_name(name);
            if (!language) throw UsageError("unknown language '" + name + "' (python, c, c++, csharp, php, php-old)");
            const std::optional<std::size_t> per_line = args.integer<std::size_t>("bytes");
            const std::string source = format(fs::read_file(fs::from_text(args.required("input"))), *language, args.required("variable"), per_line.value_or(16));
            fs::write_text(fs::from_text(args.required("output")), source);
            cb::out_text(source);
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::bin2lang
