// cb/knowledge/reads.hpp -- what a step read of the knowledge base, so that a changed piece makes stale exactly the
// facts made from it.
//
//     {
//         knowledge::Recording rec;                                     // (the engine does this around every step)
//         const auto argc = argument_count("USER32", "FindWindowA");    // asks the base; each ask calls record()
//         rec.reads()->sorted();                                        // {win32:argument_bytes.header:FindWindowA}
//     }
//     digests.digest(read);                                            // the piece's hash now, "absent" if not there
//
// A read is named "platform:kind:key" -- the piece's place in the base -- or "platform:kind:*" for a whole kind read at
// once (the runtime-library signatures, a table). The accessors that answer from the base call record(), so whatever
// asks them -- describe, c_text's argument sizes, the lanes' argument counts, the emulators' stand-ins -- is recorded
// with no change of its own. The engine runs each step inside a Recording and adds every read to the fact's inputs
// with the piece's digest: a fact is current only while each piece it read has the hash it had. A piece that is not
// there is "absent"; adding it later is a change too.
//
// The recording in force is per thread: the one piece of ambient context the engine's steps rely on (they cannot
// pass it through every accessor). Work handed to another thread captures it with `current_recording()` and puts it
// in force there with `AdoptRecording`; work handed to a worker process runs under `run_recorded`, which brings the
// reads back with the result (`encode_reads`/`decode_reads` carry them across the process boundary).
#pragma once

#include <compare>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/knowledge/sources.hpp"

namespace cb::knowledge {

// The key of a read that names a whole kind.
inline constexpr std::string_view kWholeKind = "*";
// What the digest of a piece that is not in the base is.
inline constexpr std::string_view kAbsent = "absent";
// The engine's name for an input that is a piece of the base.
inline constexpr std::string_view kReadInputKind = "kb";

struct Read {
    std::string platform;
    std::string kind;
    std::string key;   // kWholeKind for a whole kind

    // "platform:kind:key". The key may itself contain colons.
    std::string text() const;
    // The inverse: the first two colons separate the parts. nullopt when there are fewer than two colons.
    static std::optional<Read> parse(std::string_view text);
    bool whole_kind() const { return key == kWholeKind; }
    friend auto operator<=>(const Read&, const Read&) = default;
};

// The reads of one recording. Thread-safe: work on several threads records into one.
class Reads {
public:
    void add(Read read);
    void merge(std::span<const Read> reads);
    // Sorted, each once.
    std::vector<Read> sorted() const;
    std::size_t size() const;
    bool contains(const Read& read) const;

private:
    mutable std::mutex mutex_;
    std::set<Read> reads_;   // iterated (sorted) into the engine's inputs: the order is deterministic
};

// While it lives, every read made on this thread (and on threads that adopt it) goes into reads(). A recording
// inside another keeps its own: the outer one does not see the inner's reads (a fact made for a step is that fact's,
// not its asker's).
class Recording {
public:
    Recording();
    ~Recording();
    Recording(const Recording&) = delete;
    Recording& operator=(const Recording&) = delete;
    const std::shared_ptr<Reads>& reads() const { return reads_; }

private:
    std::shared_ptr<Reads> reads_;
    std::shared_ptr<Reads> previous_;
};

// The recording in force on this thread, or null.
std::shared_ptr<Reads> current_recording();
inline bool recording_active() { return current_recording() != nullptr; }

// Put a captured recording in force on this thread for a scope (the asker's recording, on a pool's thread).
class AdoptRecording {
public:
    explicit AdoptRecording(std::shared_ptr<Reads> reads);
    ~AdoptRecording();
    AdoptRecording(const AdoptRecording&) = delete;
    AdoptRecording& operator=(const AdoptRecording&) = delete;

private:
    std::shared_ptr<Reads> previous_;
};

// A piece was read (nothing happens outside a recording).
void record(std::string_view platform, std::string_view kind, std::string_view key);
void record_kind(std::string_view platform, std::string_view kind);
// Reads made elsewhere (a worker process) for the current recording.
void merge_reads(std::span<const Read> reads);

// ---- work handed to worker processes ----------------------------------------------------------------------
// A worker's result with the reads it made.
template <class T>
struct Carried {
    T value;
    std::vector<Read> reads;
};

// Run `work` under a fresh recording and return its result with the reads.
template <class Work>
auto run_recorded(Work&& work) -> Carried<std::invoke_result_t<Work>> {
    static_assert(!std::is_void_v<std::invoke_result_t<Work>>, "a recorded work item answers with a value");
    Recording recording;
    auto value = std::forward<Work>(work)();
    return {std::move(value), recording.reads()->sorted()};
}

// A carried result: its reads merged into the current recording, and its value.
template <class T>
T unwrap(Carried<T> carried) {
    merge_reads(carried.reads);
    return std::move(carried.value);
}

// The reads as bytes, to travel with a worker's answer; decode throws FormatError for damaged bytes.
Bytes encode_reads(std::span<const Read> reads);
std::vector<Read> decode_reads(ByteView data);

// ---- what a read is now --------------------------------------------------------------------------------------
// The hash of a read piece now. The base answers (`DigestOf`): the value's hash, kAbsent when it is not there, for a
// whole kind a hash of every piece's, for the name store a hash of what it answers. Before a kind's first digest in
// this object its freshener runs once: an authored kind is loaded again if its file changed, a derived one (the
// prototypes, the structures) brought up to date with the headers it is made from, so that a staleness check in a new
// process sees the piece as it would now be made. Thread-safe.
class ReadDigests {
public:
    using DigestOf = std::function<std::string(const Read&)>;
    using Freshener = std::function<void(const KindId&)>;

    // `answer` is the base's digest of a piece (the knowledge store's).
    explicit ReadDigests(DigestOf answer) : answer_(std::move(answer)) {}

    // How to bring a kind up to date before its pieces are hashed. A kind without one is read as it is.
    void on_kind(KindId kind, Freshener freshen);
    // A freshener for every authored kind of the catalogue (sources and store must outlive this object).
    void freshen_authored(AuthoredSources& sources, KindStore& store);

    std::string digest(const Read& read);

private:
    DigestOf answer_;
    std::mutex mutex_;
    std::vector<std::pair<KindId, Freshener>> fresheners_;   // looked up by kind, a handful: a vector is the map
    std::set<KindId> fresh_;                                  // kinds brought up to date by this object
};

}  // namespace cb::knowledge
