#include "cb/knowledge/reads.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::knowledge {

namespace {

// The ambient recording of this thread (see the header for why this one piece of state is ambient).
thread_local std::shared_ptr<Reads> t_current;

constexpr const char* kReader = "knowledge reads";

}  // namespace

std::string Read::text() const { return platform + ":" + kind + ":" + key; }

std::optional<Read> Read::parse(std::string_view text) {
    const std::size_t first = text.find(':');
    if (first == std::string_view::npos) return std::nullopt;
    const std::size_t second = text.find(':', first + 1);
    if (second == std::string_view::npos) return std::nullopt;
    return Read{std::string(text.substr(0, first)), std::string(text.substr(first + 1, second - first - 1)),
                std::string(text.substr(second + 1))};
}

void Reads::add(Read read) {
    const std::lock_guard<std::mutex> lock(mutex_);
    reads_.insert(std::move(read));
}

void Reads::merge(std::span<const Read> reads) {
    const std::lock_guard<std::mutex> lock(mutex_);
    reads_.insert(reads.begin(), reads.end());
}

std::vector<Read> Reads::sorted() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return std::vector<Read>(reads_.begin(), reads_.end());
}

std::size_t Reads::size() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return reads_.size();
}

bool Reads::contains(const Read& read) const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return reads_.contains(read);
}

Recording::Recording() : reads_(std::make_shared<Reads>()), previous_(t_current) { t_current = reads_; }
Recording::~Recording() { t_current = previous_; }

std::shared_ptr<Reads> current_recording() { return t_current; }

AdoptRecording::AdoptRecording(std::shared_ptr<Reads> reads) : previous_(t_current) { t_current = std::move(reads); }
AdoptRecording::~AdoptRecording() { t_current = previous_; }

void record(std::string_view platform, std::string_view kind, std::string_view key) {
    if (t_current) t_current->add(Read{std::string(platform), std::string(kind), std::string(key)});
}

void record_kind(std::string_view platform, std::string_view kind) { record(platform, kind, kWholeKind); }

void merge_reads(std::span<const Read> reads) {
    if (t_current && !reads.empty()) t_current->merge(reads);
}

Bytes encode_reads(std::span<const Read> reads) {
    io::ByteWriter w;
    w.u32(static_cast<std::uint32_t>(reads.size()));
    for (const Read& r : reads) {
        for (const std::string* part : {&r.platform, &r.kind, &r.key}) {
            w.u32(static_cast<std::uint32_t>(part->size()));
            w.bytes(as_bytes(*part));
        }
    }
    return w.take();
}

std::vector<Read> decode_reads(ByteView data) {
    io::ByteReader r(data, kReader);
    const std::uint32_t count = r.u32();
    if (count > r.remaining() / 12) throw FormatError(kReader, 0, "more reads than the data can hold");
    std::vector<Read> out;
    out.reserve(count);
    auto text = [&r] {
        const std::uint32_t n = r.u32();
        return std::string(as_text(r.bytes(n)));
    };
    for (std::uint32_t i = 0; i < count; ++i) {
        Read read;
        read.platform = text();
        read.kind = text();
        read.key = text();
        out.push_back(std::move(read));
    }
    if (!r.eof()) throw FormatError(kReader, r.pos(), "bytes after the reads");
    return out;
}

void ReadDigests::on_kind(KindId kind, Freshener freshen) {
    const std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [k, f] : fresheners_) {
        if (k == kind) {
            f = std::move(freshen);
            return;
        }
    }
    fresheners_.emplace_back(std::move(kind), std::move(freshen));
}

void ReadDigests::freshen_authored(AuthoredSources& sources, KindStore& store) {
    for (const AuthoredKind& k : AuthoredSources::catalogue()) {
        on_kind(k.id, [&sources, &store](const KindId& id) { (void)sources.authored(id, store); });
    }
}

std::string ReadDigests::digest(const Read& read) {
    const KindId kind{read.platform, read.kind};
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        if (fresh_.insert(kind).second) {
            for (const auto& [k, freshen] : fresheners_) {
                if (k != kind) continue;
                // An isolation boundary: a kind that cannot be brought up to date is hashed as it stands.
                try {
                    freshen(kind);
                } catch (const Error& e) {
                    log::warn("knowledge " + kind.platform + ":" + kind.kind + ": not brought up to date: " + e.what());
                }
                break;
            }
        }
    }
    return answer_(read);
}

}  // namespace cb::knowledge
