// verdicts.h -- the one verdict vocabulary (codebreaker/verify/verdicts.py): what a judge says of a
// pair, what a verdict amounts to (the function store's classes), and what a byte comparison says;
// older spellings read as the words now.
#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace cbe {
namespace V {

extern const char *EQUIVALENT, *CONSISTENT, *DIVERGENT, *INCONCLUSIVE, *NO_COMPILE, *NO_SOURCE;
extern const char *VERIFIED, *WEAK, *UNVERIFIED, *NOT_JUDGED, *IDENTIFIED;
extern const char *IDENTICAL, *SAME_SIZE, *DIFFERENT, *NO_EXTENT;

extern const std::vector<std::string> VERDICTS, GOOD, ORDER, SCORED, BUILT, MATCHES;
extern const std::vector<std::pair<std::string, std::string>> OLD, CLASS_OLD, MATCH_OLD, CLASS, TITLE, LABEL,
    SAID, MATCH_LABEL, WEAK_LIMITS;

// None stays None: `null` in, `null` out
std::string verdict(const std::string& word);
std::string klass(const std::string& word);
std::string match(const std::string& word);
// (class, weak reasons)
std::pair<std::string, std::vector<std::string>> classify(const std::string* word, const std::string& limits);
int order_index(const std::string& cls);           // ORDER.index(cls)
bool good(const std::string& word);

}  // namespace V
}  // namespace cbe
