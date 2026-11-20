// decompile/asm_value.hpp -- the numeric expression the small-CPU assemblers of the Odyssey 2 (8048) and Channel F (F8) read.
//
//     asm_value::eval("label+2", labels, lenient)       `$1F`, `0x1F`, `%101`, decimal, labels, `+` and `-`, and `<` / `>` before a term for its low / high byte
//
// With `lenient`, a name that is not in `labels` is 0 (an assembler's first pass needs only sizes). Throws ParseError for anything it cannot read.
#pragma once

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>

#include "core/error.hpp"
#include "core/text.hpp"
#include "decompile/labels.hpp"

namespace cb::decompile::asm_value {

inline std::int64_t eval(std::string_view expr, const NameLabels& labels, bool lenient = false) {
    const std::string_view e = text::trim(expr);
    if (e.empty()) throw ParseError("", 0, 0, "an operand is missing");
    std::int64_t total = 0;
    std::size_t i = 0;
    int sign = 1;
    bool expect_term = true;
    while (i < e.size()) {
        if (e[i] == ' ' || e[i] == '\t') {
            ++i;
            continue;
        }
        if (e[i] == '+' || e[i] == '-') {
            if (e[i] == '-') sign = -sign;
            ++i;
            expect_term = true;
            continue;
        }
        if (!expect_term) throw ParseError("", 0, 0, "cannot read '" + std::string(e) + "' as a value");
        bool low = false, high = false;
        if (e[i] == '<') { low = true; ++i; }
        else if (e[i] == '>') { high = true; ++i; }
        std::size_t j = i;
        while (j < e.size() && (std::isalnum(static_cast<unsigned char>(e[j])) || e[j] == '_' || e[j] == '.' || e[j] == '$' || e[j] == '%')) ++j;
        const std::string_view term = e.substr(i, j - i);
        if (term.empty()) throw ParseError("", 0, 0, "cannot read '" + std::string(e) + "' as a value");
        std::int64_t v = 0;
        const auto number = [&](std::string_view digits, int base) {
            const auto p = text::parse_int<std::int64_t>(digits, base);
            if (!p) throw ParseError("", 0, 0, "cannot read '" + std::string(term) + "' as a number");
            return *p;
        };
        if (term[0] == '$' && term.size() > 1) v = number(term.substr(1), 16);
        else if (term[0] == '%' && term.size() > 1) v = number(term.substr(1), 2);
        else if (term.size() > 2 && term[0] == '0' && (term[1] == 'x' || term[1] == 'X')) v = number(term.substr(2), 16);
        else if (std::isdigit(static_cast<unsigned char>(term[0]))) v = number(term, 10);
        else if (const auto it = labels.find(term); it != labels.end()) v = it->second;
        else if (lenient) v = 0;
        else throw ParseError("", 0, 0, "unknown name '" + std::string(term) + "'");
        if (low) v &= 0xFF;
        if (high) v = (v >> 8) & 0xFF;
        total += sign * v;
        sign = 1;
        expect_term = false;
        i = j;
    }
    return total;
}

}  // namespace cb::decompile::asm_value
