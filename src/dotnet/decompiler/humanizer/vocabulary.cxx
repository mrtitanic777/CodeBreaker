// dotnet/decompiler/humanizer/vocabulary.cxx -- see vocabulary.hpp.
#include "dotnet/decompiler/humanizer/vocabulary.hpp"

#include <windows.h>

#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::humanizer {

namespace {

// char.ToUpper / char.ToLower (the invariant culture's mapping of one UTF-16 code unit)
char16_t map_case(char16_t c, DWORD flag) {
    wchar_t mapped = static_cast<wchar_t>(c);
    wchar_t source = static_cast<wchar_t>(c);
    if (LCMapStringEx(LOCALE_NAME_INVARIANT, flag, &source, 1, &mapped, 1, nullptr, nullptr, 0) != 1) return c;
    return static_cast<char16_t>(mapped);
}

// the text, lower-cased
std::string to_lower(const std::string& text) {
    std::u16string utf16 = util::utf16(text);
    for (char16_t& c : utf16) c = map_case(c, LCMAP_LOWERCASE);
    return util::utf8(utf16);
}

// char.IsUpper(word[0]) && char.IsLower(replacement[0]) ? char.ToUpper(replacement[0]) + replacement[1..] : replacement
std::string match_upper_case(const std::string& word, const std::string& replacement) {
    if (word.empty() || replacement.empty()) return replacement;
    std::u16string word16 = util::utf16(word);
    std::u16string replacement16 = util::utf16(replacement);
    if (word16.empty() || replacement16.empty()) return replacement;
    if (util::get_unicode_category(word16[0]) == util::UnicodeCategory::UppercaseLetter &&
        util::get_unicode_category(replacement16[0]) == util::UnicodeCategory::LowercaseLetter) {
        replacement16[0] = map_case(replacement16[0], LCMAP_UPPERCASE);
        return util::utf8(replacement16);
    }
    return replacement;
}

}  // namespace

Vocabulary::Vocabulary() : letter_s_("^([sS])[sS]*$", std::regex::ECMAScript) {}

Vocabulary::Rule::Rule(const std::string& pattern, std::string replacement_) : replacement(std::move(replacement_)) {
    // std::regex has no lookbehind: `(?<!^[a-z])` ("not directly after the first letter of the word") is checked in apply().
    static const std::string lookbehind = "(?<!^[a-z])";
    std::string effective_pattern = pattern;
    if (pattern.starts_with(lookbehind)) {
        effective_pattern = pattern.substr(lookbehind.size());
        not_after_first_letter = true;
    }
    regex = std::regex(effective_pattern, std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
}

std::optional<std::string> Vocabulary::Rule::apply(const std::string& word) const {
    std::smatch match;
    if (!std::regex_search(word, match, regex)) return std::nullopt;
    if (not_after_first_letter && match.position(0) == 1 && !word.empty() &&
        ((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z')))
        return std::nullopt;
    return std::regex_replace(word, regex, replacement);
}

void Vocabulary::add_irregular(const std::string& singular, const std::string& plural, bool match_ending) {
    if (match_ending) {
        std::string singular_substring = singular.substr(1);
        std::string plural_substring = plural.substr(1);
        add_plural("(" + std::string(1, singular[0]) + ")" + singular_substring + "$", "$1" + plural_substring);
        add_singular("(" + std::string(1, plural[0]) + ")" + plural_substring + "$", "$1" + singular_substring);
    } else {
        add_plural("^" + singular + "$", plural);
        add_singular("^" + plural + "$", singular);
    }
}

void Vocabulary::add_uncountable(const std::string& word) { uncountables_.insert(to_lower(word)); }

void Vocabulary::add_plural(const std::string& rule, const std::string& replacement) { plurals_.emplace_back(rule, replacement); }

void Vocabulary::add_singular(const std::string& rule, const std::string& replacement) { singulars_.emplace_back(rule, replacement); }

std::string Vocabulary::pluralize(const std::string& word, bool input_is_known_to_be_singular) const {
    std::optional<std::string> s = letter_s(word);
    if (s.has_value()) {
        return *s + "s";
    }

    std::optional<std::string> result = apply_rules(plurals_, word, false);

    if (input_is_known_to_be_singular) {
        return result.value_or(word);
    }

    std::optional<std::string> as_singular = apply_rules(singulars_, word, false);
    std::optional<std::string> as_singular_as_plural = apply_rules(plurals_, as_singular, false);
    if (as_singular.has_value() && *as_singular != word && *as_singular + "s" != word && as_singular_as_plural == word && result != word) {
        return word;
    }

    return result.value_or(std::string());
}

std::string Vocabulary::singularize(const std::string& word, bool input_is_known_to_be_plural, bool skip_simple_words) const {
    std::optional<std::string> s = letter_s(word);
    if (s.has_value()) {
        return *s;
    }

    std::optional<std::string> result = apply_rules(singulars_, word, skip_simple_words);

    if (input_is_known_to_be_plural) {
        return result.value_or(word);
    }

    // the Plurality is unknown so we should check all possibilities
    std::optional<std::string> as_plural = apply_rules(plurals_, word, false);
    if (as_plural == word || (as_plural.has_value() && word + "s" == *as_plural)) {
        return result.value_or(word);
    }

    std::optional<std::string> as_plural_as_singular = apply_rules(singulars_, as_plural, false);
    if (as_plural_as_singular != word || result == word) {
        return result.value_or(word);
    }

    return word;
}

std::optional<std::string> Vocabulary::apply_rules(const std::vector<Rule>& rules, const std::optional<std::string>& word, bool skip_first_rule) const {
    if (!word.has_value()) {
        return std::nullopt;
    }

    if (word->empty()) {
        return word;
    }

    if (is_uncountable(*word)) {
        return word;
    }

    std::optional<std::string> result = word;
    int end = skip_first_rule ? 1 : 0;
    for (int i = static_cast<int>(rules.size()) - 1; i >= end; i--) {
        result = rules[static_cast<std::size_t>(i)].apply(*word);
        if (result.has_value()) break;
    }

    if (!result.has_value()) {
        return std::nullopt;
    }

    return match_upper_case(*word, *result);
}

bool Vocabulary::is_uncountable(const std::string& word) const { return uncountables_.contains(to_lower(word)); }

std::optional<std::string> Vocabulary::letter_s(const std::string& word) const {
    std::smatch match;
    if (std::regex_match(word, match, letter_s_)) return match[1].str();
    return std::nullopt;
}

}  // namespace cb::dotnet::decompiler::humanizer
