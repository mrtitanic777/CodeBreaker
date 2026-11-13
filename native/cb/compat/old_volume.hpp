// cb/compat/old_volume.hpp -- a volume of the old media shim (the Xbox containers' volumes included) as the new module reads one.
// Conversion and forwarding only. Deleted with the last shim that includes it (the old coverage/reach).
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/media.hpp"
#include "codebreaker/formats/media.hpp"
#include "py.hpp"

namespace cb::compat {

class OldVolume : public cb::formats::media::Volume {
public:
    explicit OldVolume(std::shared_ptr<codebreaker::formats::media::Volume> v) : old_(std::move(v)) {
        label_ = from_py(old_->label);
        where_ = from_py(old_->where);
        for (const auto& kv : old_->boot.items()) {
            cb::formats::media::BootField f;
            f.name = from_py(std::get<0>(kv));
            const auto& value = std::get<1>(kv);
            if (std::holds_alternative<py::str>(value)) {
                f.text = from_py(std::get<py::str>(value));
            } else {
                f.number = std::get<int64_t>(value);
                f.text = std::to_string(*f.number);
            }
            boot_.push_back(std::move(f));
        }
    }
    cb::formats::media::VolumeKind kind() const override { return cb::formats::media::VolumeKind::Iso9660; }
    const std::vector<cb::formats::media::Entry>& files() override {
        if (!entries_) {
            entries_.emplace();
            try {
                for (const auto& e : old_->files()) {
                    olds_.push_back(e);
                    cb::formats::media::Entry n;
                    n.path = from_py(e.path);
                    n.size = static_cast<std::uint64_t>(e.size);
                    n.is_dir = e.is_dir;
                    entries_->push_back(std::move(n));
                }
            } catch (const py::Exception& e) {
                throw cb::FormatError("media", 0, e.what());
            }
        }
        return *entries_;
    }
    cb::Bytes read(const cb::formats::media::Entry& entry) override {
        try {
            return from_py(old_->read(olds_.at(static_cast<std::size_t>(&entry - entries_->data()))));
        } catch (const py::Exception& e) {
            throw cb::FormatError("media", 0, e.what());
        }
    }

private:
    std::shared_ptr<codebreaker::formats::media::Volume> old_;
    std::optional<std::vector<cb::formats::media::Entry>> entries_;
    std::vector<codebreaker::formats::media::Entry> olds_;
};

}  // namespace cb::compat
