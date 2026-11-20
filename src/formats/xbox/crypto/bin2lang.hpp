// formats/xbox/crypto/bin2lang.hpp -- a binary as source code: the bytes as an array in Python, C, C++, C#, or PHP.
//
//     std::string source = bin2lang::format(key_bytes, bin2lang::Language::Cpp, "ExpPubKey");
//
// The command `crypto bin2lang <input> <output> [-l LANGUAGE] [-b BYTES_PER_LINE] [-v VARIABLE]` writes the source to <output> and prints it.
// C and C++ are the guarded `BYTE name[]` of format_key.hpp; the others are the language's own array literal, `per_line` bytes to a line,
// each byte `0xNN`, the last line without its trailing comma.
//
// Errors: FormatError for no data (there is no empty array to write); UsageError for a language name that is not one.
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::bin2lang {

enum class Language {
    Python,
    C,
    Cpp,
    CSharp,
    Php,       // the short array syntax ([...])
    PhpOld,    // array(...)
};

// "python"/"py", "c", "c++"/"cpp"/"cplusplus", "csharp"/"cs"/"c#", "php"/"php-new", "php-old" (case ignored).
std::optional<Language> language_from_name(std::string_view name);

std::string format(ByteView data, Language language, std::string_view variable = "data", std::size_t per_line = 16);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::bin2lang
