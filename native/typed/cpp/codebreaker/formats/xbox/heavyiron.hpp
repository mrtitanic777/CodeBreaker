// codebreaker/formats/xbox/heavyiron.hpp -- compat shim over cb/formats/xbox/heavyiron.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/heavyiron.{hpp,cpp}; this header keeps the old py:: surface for the ports that have
// not been refactored yet (xbox/volumes) and is deleted with the last of them. Conversion and forwarding only. What no caller
// used (the parse tree as structs, `replace`, `texture`, the byte reader) is not carried over: it is cb::formats::xbox::heavyiron.
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/heavyiron.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::heavyiron {

namespace cbh = cb::formats::xbox::heavyiron;

inline bool is_archive(const py::str& path) {
    return cb::compat::guard([&] { return cbh::is_archive(cb::fs::from_text(cb::compat::from_py(path))); });
}

// [(name, size)] and a reader, for formats/xbox/volumes.py.
struct VolumeListing {
    py::list<std::tuple<py::str, int64_t>> entries;
    std::function<py::bytes(const py::str&)> read;
};

inline VolumeListing volume_listing(const py::str& path) {
    return cb::compat::guard([&] {
        struct State {   // what the reader needs to outlive the call: the file, its tree and the names
            cb::Bytes data;
            cbh::Archive archive;
            std::map<std::string, const cbh::Asset*> by_name;
        };
        auto state = std::make_shared<State>();
        state->data = cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(path)));
        state->archive = cbh::Archive::parse(state->data);
        VolumeListing out;
        for (const cbh::VolumeEntry& e : cbh::volume_entries(state->archive)) {
            state->by_name[e.name] = e.asset;
            out.entries.append(std::make_tuple(cb::compat::to_py(e.name), static_cast<int64_t>(e.size)));
        }
        out.read = [state](const py::str& name) {
            return cb::compat::guard([&] {
                const auto it = state->by_name.find(cb::compat::from_py(name));
                if (it == state->by_name.end()) throw py::KeyError(name.utf8());
                return cb::compat::to_py(cbh::asset_bytes(state->data, *it->second));
            });
        };
        return out;
    });
}

inline const py::str MAIN_DOC = py::str(std::string(cbh::kUsage));

// `heavyiron ...`; `argv` is sys.argv[1:] -> the exit status.
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] { return int64_t(cbh::command(cb::compat::from_py(argv))); });
}

}  // namespace codebreaker::formats::xbox::heavyiron
