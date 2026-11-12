// codebreaker/formats/media.cpp -- what the shim of media.hpp cannot be header-only for (COMPAT(py): remove when
// formats.xbox.volumes is refactored): volumes() puts the old Xbox volumes first, and xbox/volumes.hpp includes media.hpp.
// The reading, writing and matching is native/cb/formats/media.{hpp,cpp}.

#include "codebreaker/formats/media.hpp"

#include "codebreaker/formats/xbox/volumes.hpp"

namespace codebreaker::formats::media {

namespace {

// The errors of the new reader as the old code raised them: a malformed or unusable volume was a ValueError.
template <class F>
decltype(auto) value_errors(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const cb::UnsupportedError& e) {
        throw py::ValueError(e.what());
    } catch (const cb::UsageError& e) {
        throw py::ValueError(e.what());
    } catch (const cb::IoError& e) {
        cb::compat::rethrow_as_py(e);
    } catch (const cb::Error& e) {
        throw py::ValueError(e.what());   // "the volume is full", "the root folder is full"
    }
}

}  // namespace

py::list<std::shared_ptr<Volume>> volumes(const py::str& path, bool long_names) {
    // (first: an Xbox 360 disc also carries an ISO-9660 video part)
    std::optional<xbox::volumes::VolumeList> found = xbox::volumes::volumes(path, long_names);
    if (found && !found->empty()) return *found;
    py::list<std::shared_ptr<Volume>> out;
    const std::filesystem::path file = cb::compat::from_py(path);
    cb::compat::guard([&] {
        for (std::unique_ptr<cm::Volume>& v : cm::volumes(file, long_names)) {
            switch (v->kind()) {
            case cm::VolumeKind::Fat12:
            case cm::VolumeKind::Fat16:
            case cm::VolumeKind::Fat32: {
                auto fat = std::shared_ptr<cm::FatVolume>(static_cast<cm::FatVolume*>(v.release()));
                out.append(std::make_shared<FatVolume>(_Source(path, static_cast<int64_t>(fat->source().offset())), fat));
                break;
            }
            case cm::VolumeKind::Iso9660: {
                auto iso = std::shared_ptr<cm::IsoVolume>(static_cast<cm::IsoVolume*>(v.release()));
                out.append(std::make_shared<IsoVolume>(_Source(path, static_cast<int64_t>(iso->source().offset())), iso));
                break;
            }
            case cm::VolumeKind::Bundle: out.append(std::make_shared<BundleVolume>(path)); break;
            }
        }
    });
    return out;
}

py::list<std::tuple<py::str, py::str>> put(const py::str& path, const py::dict<py::str, py::bytes>& files, int64_t volume) {
    std::vector<cm::NewFile> given;
    for (const auto& kv : files.items()) given.push_back({cb::compat::from_py(std::get<0>(kv)), cb::compat::from_py(std::get<1>(kv))});
    const int64_t count = static_cast<int64_t>(volumes(path).size());
    if (volume < 0) volume += count;      // (a negative number counts from the end)
    if (volume < 0 || volume >= count) throw py::IndexError("list index out of range");
    py::list<std::tuple<py::str, py::str>> out;
    value_errors([&] {
        for (const cm::PutResult& r : cm::put(std::filesystem::path(cb::compat::from_py(path)), given, static_cast<std::size_t>(volume)))
            out.append(std::make_tuple(cb::compat::to_py(r.path), py::str(r.outcome == cm::PutOutcome::Added ? "added" : "replaced")));
    });
    return out;
}

int64_t extract(const py::str& path, const py::str& folder, const py::list<py::str>& names, bool long_names) {
    int64_t n = 0;
    int64_t i = 0;
    for (const std::shared_ptr<Volume>& v : volumes(path, long_names)) {
        py::str base = i == 0 ? folder : py::os::path::join(folder, py::percent(py::str("volume%d"), i + 1));
        ++i;
        for (const Entry& e : v->files()) {
            bool wanted = names.empty();
            if (!wanted) {
                int64_t slash = e.path.rfind(py::str("/"));
                py::str last = slash < 0 ? e.path : e.path.slice(slash + 1, e.path.size());
                for (const py::str& w : names)
                    if (fnmatch(e.path, w) || fnmatch(last, w)) {
                        wanted = true;
                        break;
                    }
            }
            if (e.is_dir || !wanted) continue;
            py::str out = py::os::path::join(base, _safe(e.path));
            py::str dir = py::os::path::dirname(out);
            py::os::makedirs(dir.empty() ? py::str(".") : dir, 0777, true);
            py::file f = py::file::open(out, py::str("wb"));
            f.write(v->read(e));
            f.close();
            ++n;
        }
    }
    return n;
}

const py::str MAIN_DOC(
    "python -m codebreaker media list <image> [--boot] [--plain]\n"
    "python -m codebreaker media extract <image> <folder> [name ...] [--plain]\n"
    "\n"
    "List or extract the files of a floppy, hard-disk or CD image, or of a Windows 1.x boot\n"
    "image. --boot also prints each volume's boot sector fields; --plain uses 8.3 / primary names.");

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> flags, args;
    for (const py::str& a : argv) (a.startswith(py::str("--")) ? flags : args).append(a);
    if (args.size() < 2 || !(args[0] == py::str("list") || args[0] == py::str("extract")) || (args[0] == py::str("extract") && args.size() < 3)) {
        py::print(MAIN_DOC);
        return 2;
    }
    bool long_names = !flags.contains(py::str("--plain"));
    if (args[0] == py::str("extract")) {
        py::print(py::percent(py::str("%d files written to %s"), extract(args[1], args[2], args.slice_from(3), long_names), args[2]));
        return 0;
    }
    for (const std::shared_ptr<Volume>& v : volumes(args[1], long_names)) {
        py::list<Entry> files = v->files();
        int64_t nfiles = 0, nfolders = 0, nbytes = 0;
        for (const Entry& e : files) {
            if (!e.is_dir) {
                ++nfiles;
                nbytes += e.size;
            } else ++nfolders;
        }
        py::print(py::percent(py::str("%s volume %r%s: %d files, %d folders, %d bytes"), v->kind, v->label,
                              v->where.empty() ? py::str("") : py::percent(py::str(" (%s)"), v->where), nfiles, nfolders, nbytes));
        if (flags.contains(py::str("--boot"))) {
            for (const auto& kv : v->boot.items()) {
                const BootValue& value = std::get<1>(kv);
                py::str shown = std::holds_alternative<py::str>(value) ? std::get<py::str>(value) : py::to_str(std::get<int64_t>(value));
                py::print(py::percent(py::str("    %-20s %s"), std::get<0>(kv), shown));
            }
        }
        for (const Entry& e : files)
            py::print(py::percent(py::str("  %10s  %-16s  %s"), e.is_dir ? py::str("<folder>") : py::to_str(e.size), e.when,
                                  long_names ? e.path : e.short_));
    }
    return 0;
}

}  // namespace codebreaker::formats::media
