// cb/java/rules.hpp -- structural crash-class detectors for converted C++.
//
// These encode the bug classes that crash when porting Java to C++ across any target platform. Each rule is
// platform-aware: messages and applicability adapt to the selected target (Xbox 360, PC Windows, PS3, ...) using the
// metadata of platforms.hpp, and the target's profile supplies the heavy types and the nullable getters/fields.
//
// Each rule is a small, named, self-describing function, so it is individually testable and the report can explain
// *why* it fired. A rule looks at one converted file (and, when it needs it, its sibling header) and returns Findings.
//
// Covered (the cases plain text-marker / lint scanning misses):
//   uninit-array          new T[n] without () -- Java zero-fills, C++ does not
//   ctor-stub             empty / delegation-only constructor body
//   rule-of-three         owns heap (new in ctor) but missing dtor/copy control
//   value-copy-owner      a heavy/owning type passed or returned BY VALUE
//   virtual-call-in-ctor  ctor calls own virtual -> dispatches to base, not override
//   static-init-order     eager static init with cross-TU dependency
//   endianness            raw multi-byte cast in binary-IO file (big-endian only)
//   null-deref-getter     getter()->member where the getter can return NULL
//   null-deref-field      known-nullable field deref without null check
//   ... and the loop, leak, container, dispatch and debug rules listed in all_rules().
// plus one whole-tree pass: odr-duplicate-def.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/java/ledger.hpp"
#include "cb/java/profiles/package.hpp"

namespace cb::java::rules {

// What a rule found in one file.
struct Finding {
    std::uint32_t line = 0;
    ledger::Category category = ledger::Category::Risk;
    ledger::Severity severity = ledger::Severity::Med;
    std::string rule;      // the rule's id
    std::string message;
    std::string snippet;   // the offending source, stripped and clipped
};

using ClassMethodPairs = std::set<std::pair<std::string, std::string>>;

// What every rule knows about the target and the tree: built once per scan.
struct Context {
    std::string platform_label = "unknown";
    bool big_endian = false;
    std::string null_deref_msg = "NULL dereference -- undefined behaviour in C++ (Java would throw NPE)";
    // {(base class, method)} that a DESCENDANT overrides, built over the whole tree so virtual-call-in-ctor can flag only
    // genuinely overridden calls. nullopt: the hierarchy is unknown (a single-file check), so the rule stays conservative
    // and flags every call.
    std::optional<ClassMethodPairs> override_pairs;
    // from the target profile
    std::vector<std::string> heavy_types;
    std::vector<std::string> heavy_types_high;
    std::set<std::string, std::less<>> nullable_getters;
    std::set<std::string, std::less<>> nullable_fields;

    // The context of a platform key (empty or unknown: the generic wording) and an optional profile (nullptr: generic).
    static Context make(std::string_view platform_key, const profiles::Profile* profile = nullptr);
};

// One converted file, with its sibling header when it has one.
struct FileInput {
    std::string_view name;                  // "Block.cpp", "Block.h"
    std::string_view text;
    std::optional<std::string_view> header;   // the text of Block.h for Block.cpp
};

using Findings = std::vector<Finding>;

struct RuleInfo {
    std::string_view id;
    std::string_view description;
    Findings (*check)(const FileInput&, const Context&);
};

// Every per-file rule, in the order they run.
const std::vector<RuleInfo>& all_rules();

// Run one rule by id over one file; UsageError for an unknown id.
Findings check(std::string_view rule_id, const FileInput& file, const Context& context);

// The whole-tree hierarchy: {(ancestor class, method name)} for every method a DESCENDANT class declares in the
// headers of `output_dir` -- the calls that a base constructor invoking `this->method()` would REALLY dispatch wrong
// (Java calls the override; C++ calls the base). Over-capturing a name only makes the rule more conservative.
ClassMethodPairs build_override_pairs(const std::filesystem::path& output_dir);

// CROSS-FILE ODR: a class/struct DEFINED with DIFFERING bodies in more than one header is a one-definition-rule
// violation (the C4743 / mismatched-layout class). Per-file rules cannot see it. Returns how many were found.
std::size_t scan_odr(ledger::Ledger& ledger, const std::filesystem::path& output_dir);

struct ScanOptions {
    std::string platform;                          // the platform key the rules adapt to ("" = unknown)
    const profiles::Profile* profile = nullptr;    // the target's heavy types / nullable names (nullptr = generic)
};

// Run every structural rule over the .cpp/.h files of the converted tree, appending Events; returns how many.
std::size_t scan_rules(ledger::Ledger& ledger, const std::filesystem::path& output_dir, const ScanOptions& options = {});

}  // namespace cb::java::rules
