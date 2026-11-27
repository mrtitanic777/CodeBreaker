// dotnet/decompiler/humanizer/vocabulary.hpp -- the rules for turning an English word into its plural or singular form: regular
// expression rules (tried from the last added to the first), irregular words ("person" and "people") and words that have no plural
// ("fish"). ILSpy names the variable of a foreach loop after the singular of the collection it loops over.
//
//     Vocabulary vocabulary;
//     vocabulary.add_plural("$", "s");
//     vocabulary.add_singular("s$", "");
//     vocabulary.singularize("items", false);       // "item"
//     vocabularies::get_default().singularize("children", false);   // "child"
//
// Ported from Humanizer (Humanizer.Inflections: Vocabulary, vendored in ILSpy's ICSharpCode.Decompiler/Humanizer, from
// Humanizer/Vocabulary.cs and StringHumanizeExtensions.cs), Copyright (c) .NET Foundation and Contributors, MIT (see the LICENSE of
// Humanizer in ILSpy). Differences: the rules are ECMAScript regular expressions (std::regex), which have no lookbehind, so the one
// rule that starts with `(?<!^[a-z])` is matched with the same condition written out; words are UTF-8.
#pragma once

#include <functional>
#include <optional>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

namespace cb::dotnet::decompiler::humanizer {

// A container for exceptions to simple pluralization/singularization rules. vocabularies::get_default() contains an extensive list
// of rules for US English.
class Vocabulary {
public:
    Vocabulary();

    // Adds a word to the vocabulary which cannot easily be pluralized/singularized by RegEx, e.g. "person" and "people". With
    // `match_ending` the words are matched on their own as well as at the end of longer words.
    void add_irregular(const std::string& singular, const std::string& plural, bool match_ending = true);
    // Adds an uncountable word to the vocabulary, e.g. "fish". Will be ignored when plurality is changed.
    void add_uncountable(const std::string& word);
    // Adds a rule to the vocabulary that does not follow trivial rules for pluralization, e.g. "bus" -> "buses". The pattern is matched
    // case-insensitively, e.g. "(bus)es$", the replacement is a RegEx replacement, e.g. "$1".
    void add_plural(const std::string& rule, const std::string& replacement);
    // Adds a rule that does not follow trivial rules for singularization, e.g. "vertices/indices -> "vertex/index".
    void add_singular(const std::string& rule, const std::string& replacement);

    // Pluralizes the provided input considering irregular words. Normally you call it on singular words; but if you're unsure call it
    // with `input_is_known_to_be_singular` false.
    std::string pluralize(const std::string& word, bool input_is_known_to_be_singular = true) const;
    // Singularizes the provided input considering irregular words. Normally you call it on plural words; but if you're unsure call it
    // with `input_is_known_to_be_plural` false. `skip_simple_words` skips singularizing single words that have an 's' on the end.
    std::string singularize(const std::string& word, bool input_is_known_to_be_plural = true, bool skip_simple_words = false) const;

private:
    struct Rule {
        Rule(const std::string& pattern, std::string replacement);
        // The replaced word, or none when the rule does not match.
        std::optional<std::string> apply(const std::string& word) const;

        std::regex regex;
        std::string replacement;
        // A `(?<!^[a-z])` lookbehind the rule started with: the match must not start at index 1 of a word that starts with a letter.
        bool not_after_first_letter = false;
    };

    std::optional<std::string> apply_rules(const std::vector<Rule>& rules, const std::optional<std::string>& word, bool skip_first_rule) const;
    bool is_uncountable(const std::string& word) const;
    // If the word is the letter s, singular or plural, the letter s singular.
    std::optional<std::string> letter_s(const std::string& word) const;

    std::vector<Rule> plurals_;
    std::vector<Rule> singulars_;
    std::unordered_set<std::string> uncountables_;  // lower-cased (the comparison ignores case)
    std::regex letter_s_;
};

}  // namespace cb::dotnet::decompiler::humanizer
