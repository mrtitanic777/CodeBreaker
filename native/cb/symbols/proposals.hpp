// cb/symbols/proposals.hpp -- proposed names for the functions nothing names: from rules that need no judgement, and from an assistant that reads
// a function's evidence packet -- kept apart from names until a person accepts them.
//
//     names lz32.dll --propose               the rules run over the module; what they find is stored as proposals
//     names lz32.dll --packets [--c]         for each unnamed function the evidence an assistant needs, one JSON object a line
//     names lz32.dll --suggest 1:0116 NAME --describe "..." --confidence 70      an assistant's proposal, stored the same way
//     names lz32.dll --proposals             what is proposed, accepted and rejected
//     names lz32.dll --accept 1:0116         the proposal becomes the function's name (--accept all [--min-confidence 80])
//     names lz32.dll --reject 1:0116
//
// A proposal is a store::Proposal (store/proposals.hpp): the name, a one-line description, the rule or assistant that made it, its confidence in
// percent and the evidence it rests on. It changes nothing a reader of names sees until it is accepted: the `names` listing shows an unnamed
// function as unnamed (its proposal is shown beside it), and the source writer, the version matcher and the judge read names only.
//
// The rules propose only where the code says the name outright, and say how sure they are:
//
//     import-wrapper      the function only passes its arguments on to one imported function (a `jmp [import]`, or a call and a return):
//                         "<Import>_thunk". 90% when it does not touch its arguments at all, 80% when it moves them. (The suffix keeps the
//                         name from being the import's own, which the source would then declare twice.)
//     string-reference    the function uses a message that opens with its own name, "OpenWidget: cannot find ..." or "CloseWidget() failed":
//                         the name in the message, 50% (a message may name the function that failed, not the one that reports it).
//
// Names the rules or an assistant propose are valid C identifiers (letters, digits, `_`, and `::` for a member); a proposal whose name another
// function of the module already has is not made (and not accepted), so the names of a module stay one to a function.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/cli.hpp"
#include "cb/store/proposals.hpp"
#include "cb/symbols/evidence.hpp"
#include "cb/symbols/names.hpp"

namespace cb::symbols::proposals {

using Address = names::Address;

// The percent below which a proposal is shown for a person to accept rather than accepted with the rest (`--accept all`).
inline constexpr int kReviewBelow = 80;

// A name a proposal may carry: letters, digits, `_`, and `::` between them; not empty, not starting with a digit.
bool valid_name(std::string_view name);

// ---- the rules (each over one function's evidence; the module, key and body are the caller's to fill in) --------------------------------------
std::optional<store::Proposal> import_wrapper(const FunctionEvidence& evidence);
std::optional<store::Proposal> string_reference(const FunctionEvidence& evidence);
// A function that only passes its arguments on to one other function of the module, whose name is `target_name`: "<target>_thunk", 70%.
// (No proposal for a target with no name of its own, or one that is itself a thunk.)
std::optional<store::Proposal> function_forward(const FunctionEvidence& evidence, std::string_view target_name);
// The most confident of the rules' proposals (the first, on a tie). `target_name`: what the function it forwards to is called ("" when unknown).
// `basic`: only the first two rules (import-wrapper, string-reference), as phase 1 had them (the baseline the measurement compares with).
std::optional<store::Proposal> by_rules(const FunctionEvidence& evidence, std::string_view target_name = {}, bool basic = false);

// How well the rules name functions whose names are known: the functions a module's own export table, ordinal table or debug symbols name are
// hidden from the rules (their evidence goes in, their name stays out) and what the rules say is compared with the truth.
struct Measure {
    std::size_t truth = 0;      // functions with a name the module itself gives
    std::size_t proposed = 0;   // of those, functions a rule proposed a name for
    std::size_t exact = 0;      // proposals whose name, without the rule's suffix and decoration, is the truth (case ignored)
    std::size_t related = 0;    // ... or contains it, or is contained in it
    std::map<std::string, std::array<std::size_t, 3>> by_rule;   // per rule: proposed, exact, related
    std::vector<std::string> misses;                              // "<truth> <- <proposal> (<rule>)" for the proposals that are not related
};
// The name without a rule's suffix, a leading underscore or a stdcall decoration, lower case: what proposals and truth are compared by.
std::string comparable(std::string_view name);

// The name in a message that opens with the function's own name ("LZInit: bad header", "[ReadBlock] failed", "WriteBlock() failed"), or "".
std::string name_in_message(std::string_view message);

class Proposals {
public:
    struct Services {
        names::Namer& namer;
        store::ProposalStore& store;
        const base::Corpus& corpus;
        // The evidence of every function of a module (analyze/function_evidence).
        std::function<std::vector<FunctionEvidence>(const std::filesystem::path&)> evidence;
        // The decompiled C of the functions at the addresses, by address (a packet carries it with --c); none given: packets have no C.
        std::function<std::map<Address, std::string>(const std::filesystem::path&, std::span<const Address>)> decompile = {};
    };

    explicit Proposals(Services services);

    // Run the rules over the functions the module has no name for and store what they propose (a function with a proposal a person has decided
    // is left alone). -> the proposals made.
    std::vector<store::Proposal> propose(const std::filesystem::path& module);
    // Store a proposal made by an assistant for the function at `address`. UsageError for a name that is not valid or that another function
    // has, for a confidence outside 0..100, or for an address that is not a function of the module.
    store::Proposal suggest(const std::filesystem::path& module, Address address, std::string_view name, std::string_view description, int confidence);
    // The proposals of a module, by address.
    std::vector<store::Proposal> list(const std::filesystem::path& module);
    // Make the proposal the function's name. -> false when the function has no proposal (or has been rejected: say --accept after --suggest).
    bool accept(const std::filesystem::path& module, Address address);
    // Accept every proposed one of at least `least` percent. -> how many.
    std::size_t accept_all(const std::filesystem::path& module, int least);
    bool reject(const std::filesystem::path& module, Address address);

    // Run the rules over the module as if its own names were not there and compare (see Measure). Stores nothing. `basic`: the phase 1 rules.
    Measure measure(const std::filesystem::path& module, bool basic);

    // The packets for the unnamed functions, one JSON object per line: where it is, its calls and callers (named as far as the namer names them),
    // its strings, its instructions, its decompiled C with `with_c`, and its proposal if it has one. At most `limit` (0: all).
    std::string packets(const std::filesystem::path& module, bool with_c, std::size_t limit);

    // `names MODULE --propose|--packets|--suggest|--proposals|--accept|--reject`: the exit code.
    int execute(const cli::Args& args);

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

}  // namespace cb::symbols::proposals
