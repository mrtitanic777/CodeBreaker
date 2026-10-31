// verdicts.cc -- the verdict vocabulary (verify/verdicts.py), word for word.
#include "verdicts.h"

#include <algorithm>
#include <cctype>

namespace cbe {
namespace V {

// ---- 1. what a judge says ----------------------------------------------------------------------
const char* EQUIVALENT = "equivalent";
const char* CONSISTENT = "consistent";
const char* DIVERGENT = "divergent";
const char* INCONCLUSIVE = "inconclusive";
const char* NO_COMPILE = "no-compile";
const char* NO_SOURCE = "no-source";
const std::vector<std::string> VERDICTS = {EQUIVALENT, CONSISTENT, DIVERGENT, INCONCLUSIVE, NO_COMPILE, NO_SOURCE};
const std::vector<std::string> GOOD = {EQUIVALENT, CONSISTENT};

const std::vector<std::pair<std::string, std::string>> OLD = {
    {"no-synth", NO_SOURCE}, {"same", EQUIVALENT}, {"alike", EQUIVALENT}, {"differs", DIVERGENT},
    {"different", DIVERGENT}, {"not run", INCONCLUSIVE}, {"not-run", INCONCLUSIVE},
    {"not judged", INCONCLUSIVE}, {"untested", INCONCLUSIVE}, {"does not compile", NO_COMPILE},
    {"not compiled", NO_COMPILE}};

const std::vector<std::pair<std::string, std::string>> SAID = {
    {EQUIVALENT, "same"}, {CONSISTENT, "same (both fail)"}, {DIVERGENT, "differs"}, {INCONCLUSIVE, "not run"},
    {NO_COMPILE, "does not compile"}, {NO_SOURCE, "no source"}};

// ---- 2. what it amounts to ---------------------------------------------------------------------
const char* VERIFIED = "verified";
const char* WEAK = "weak";
const char* UNVERIFIED = "unverified";
const char* NOT_JUDGED = "not-judged";
const char* IDENTIFIED = "identified";
const std::vector<std::string> ORDER = {VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED, NO_COMPILE, NO_SOURCE};
const std::vector<std::string> SCORED = {VERIFIED, WEAK, IDENTIFIED, UNVERIFIED, NOT_JUDGED, NO_COMPILE, NO_SOURCE};
const std::vector<std::string> BUILT = {VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED};

const std::vector<std::pair<std::string, std::string>> TITLE = {
    {VERIFIED, "VERIFIED"}, {WEAK, "VERIFIED (weak)"}, {UNVERIFIED, "UNVERIFIED"}, {NOT_JUDGED, "NOT JUDGED"},
    {NO_COMPILE, "DOES NOT COMPILE"}, {NO_SOURCE, "NO SOURCE"}, {IDENTIFIED, "IDENTIFIED (runtime library)"}};
const std::vector<std::pair<std::string, std::string>> LABEL = {
    {VERIFIED, "verified"}, {WEAK, "weak"}, {UNVERIFIED, "unverified"}, {NOT_JUDGED, "not judged"},
    {NO_COMPILE, "does not compile"}, {NO_SOURCE, "no source"}, {IDENTIFIED, "runtime library"}};
const std::vector<std::pair<std::string, std::string>> CLASS = {
    {EQUIVALENT, VERIFIED}, {CONSISTENT, WEAK}, {DIVERGENT, UNVERIFIED}, {INCONCLUSIVE, NOT_JUDGED},
    {NO_COMPILE, NO_COMPILE}, {NO_SOURCE, NO_SOURCE}};

const std::vector<std::pair<std::string, std::string>> WEAK_LIMITS = {
    {"fault-only", "the two sides only ever fail alike; neither returned normally"},
    {"low-coverage", "under half of the original's instructions ran"},
    {"truncated", "a run was cut off at the step limit"},
    {"unobserved", "some inputs could not be run on one side"},
    {"writes-capped", "too many memory writes to compare them all"},
    {"stub-callee", "a function it calls was a stand-in, not that function's reconstruction"},
    {"argc-unknown", "an API call was compared by name only"},
    {"calls-capped", "it makes more calls than are compared"},
    {"methods-partial", "only some of the class's methods could be run beside the original (java)"}};

const std::vector<std::pair<std::string, std::string>> CLASS_OLD = {
    {"not judged", NOT_JUDGED}, {"does not compile", NO_COMPILE}, {"no source", NO_SOURCE},
    {"runtime library", IDENTIFIED}, {"run-time library", IDENTIFIED}, {"verified (weak)", WEAK},
    {"identified (runtime library)", IDENTIFIED}};

// ---- 3. a byte comparison ----------------------------------------------------------------------
const char* IDENTICAL = "identical";
const char* SAME_SIZE = "same size";
const char* DIFFERENT = "different";
const char* NO_EXTENT = "no original extent";
const std::vector<std::string> MATCHES = {IDENTICAL, SAME_SIZE, DIFFERENT, NO_EXTENT, NO_COMPILE};
const std::vector<std::pair<std::string, std::string>> MATCH_OLD = {
    {"does not compile", NO_COMPILE}, {"same length, different", SAME_SIZE}, {"different length", DIFFERENT},
    {"run-time library", IDENTIFIED}, {"runtime library", IDENTIFIED}};
const std::vector<std::pair<std::string, std::string>> MATCH_LABEL = {
    {IDENTICAL, "identical"}, {SAME_SIZE, "same size"}, {DIFFERENT, "different"},
    {NO_EXTENT, "no original extent"}, {NO_COMPILE, "does not compile"}, {IDENTIFIED, "runtime library"}};

static const std::string* lookup(const std::vector<std::pair<std::string, std::string>>& t, const std::string& k) {
  for (auto& kv : t)
    if (kv.first == k) return &kv.second;
  return nullptr;
}

std::string verdict(const std::string& word) {
  const std::string* got = lookup(OLD, word);
  return got ? *got : word;
}

static std::string strip(const std::string& s) {
  // str.strip(): the ASCII whitespace the vocabulary's words could carry
  size_t a = 0, b = s.size();
  while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\n' || s[a] == '\r' || s[a] == '\f' || s[a] == '\v')) a++;
  while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\n' || s[b - 1] == '\r' || s[b - 1] == '\f' ||
                   s[b - 1] == '\v'))
    b--;
  return s.substr(a, b - a);
}

std::string klass(const std::string& word) {
  std::string w = strip(word);
  if (lookup(TITLE, w)) return w;
  std::string low = w;
  for (auto& c : low) c = (char)tolower((unsigned char)c);
  if (lookup(TITLE, low)) return low;
  const std::string* got = lookup(CLASS_OLD, low);
  return got ? *got : w;
}

std::string match(const std::string& word) {
  const std::string* got = lookup(MATCH_OLD, word);
  return got ? *got : word;
}

std::pair<std::string, std::vector<std::string>> classify(const std::string* word, const std::string& limits) {
  std::string v = word ? verdict(*word) : std::string();
  std::vector<std::string> weak;
  size_t i = 0;
  while (i <= limits.size()) {
    size_t j = limits.find(',', i);
    if (j == std::string::npos) j = limits.size();
    std::string c = limits.substr(i, j - i);
    if (!c.empty() && lookup(WEAK_LIMITS, c)) weak.push_back(c);
    i = j + 1;
  }
  if (word && v == EQUIVALENT) return {weak.empty() ? VERIFIED : WEAK, weak};
  if (word && v == CONSISTENT) {
    if (weak.empty()) weak.push_back("fault-only");
    return {WEAK, weak};
  }
  const std::string* cls = word ? lookup(CLASS, v) : nullptr;
  return {cls ? *cls : std::string(NO_SOURCE), {}};
}

int order_index(const std::string& cls) {
  auto it = std::find(ORDER.begin(), ORDER.end(), cls);
  if (it == ORDER.end()) return -1;
  return (int)(it - ORDER.begin());
}

bool good(const std::string& word) {
  std::string v = verdict(word);
  return v == EQUIVALENT || v == CONSISTENT;
}

}  // namespace V
}  // namespace cbe
