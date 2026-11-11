// codebreaker/formats/cab.cpp -- see cab.hpp.

#include "codebreaker/formats/cab.hpp"

#include <algorithm>
#include <cstdio>

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::cab {

namespace {

namespace na = native_ask;

const na::Own& own() {
    static const na::Own o = na::raises<CabError>("CabError");
    return o;
}

// print(text, file=sys.stderr)
void eprint(const py::str& text) {
    std::string s = text.utf8() + "\n";
    std::fwrite(s.data(), 1, s.size(), stderr);
}

}  // namespace

const py::dict<int64_t, py::str> COMPRESSION = {{0, py::str("NONE")}, {1, py::str("MSZIP")}, {2, py::str("QUANTUM")}, {3, py::str("LZX")}};

Cabinet::Cabinet(const py::str& path_, const CabInfo& info_, const py::list<Entry>& files_,
                 const py::list<std::tuple<int64_t, int64_t, int64_t>>& folders, int64_t block_reserve)
    : path(path_), info(info_), files(files_), _folders(folders), _block_reserve(block_reserve) {}

int64_t Cabinet::folder_of(const Entry& entry) const {
    if (entry.folder == FROM_PREV || entry.folder == PREV_AND_NEXT) return 0;
    return entry.folder == TO_NEXT ? _folders.size() - 1 : entry.folder;
}

Cabinet read(const py::str& path) {
    // _by_path: no data, the path (os.fspath) the argument; the answer is (info, files, folders, block_reserve)
    using Row = std::tuple<py::str, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, py::str>;
    auto answer = na::ask<std::tuple<CabInfo, py::list<Row>, py::list<std::tuple<int64_t, int64_t, int64_t>>, int64_t>>(
        "cab.read", py::bytes(), own(), path);
    py::list<Entry> files;
    for (const Row& r : std::get<1>(answer)) {              // Entry(*e)
        Entry e;
        std::tie(e.name, e.size, e.folder, e.offset, e.date, e.time, e.attributes, e.compression) = r;
        files.append(e);
    }
    return Cabinet(path, std::get<0>(answer), files, std::get<2>(answer), std::get<3>(answer));
}

int64_t checksum(const py::bytes& payload, int64_t uncompressed) {
    return na::ask<int64_t>("cab.checksum", py::bytes(), own(), payload, uncompressed);
}

py::dict<py::str, py::str> extract(const py::str& path, const py::str& outdir, const std::optional<py::list<py::str>>& names) {
    return na::ask<py::dict<py::str, py::str>>("cab.extract", py::bytes(), own(), path, outdir, names);
}

int64_t write(const py::list<std::tuple<py::str, py::bytes>>& files, const py::str& out_path, const py::str& compress, int64_t set_id,
              int64_t index, const std::optional<Chain>& prev, const std::optional<Chain>& next, bool short_blocks) {
    namespace nat = py::native;
    nat::Args a;
    a.put(files);
    a.put(out_path);
    a.put(compress);
    a.put(set_id);
    a.put(index);
    for (const std::optional<Chain>* c : {&prev, &next}) {          // tuple(prev) if isinstance(prev, (list, tuple)) else prev
        if (!*c) a.put(std::nullopt);
        else std::visit([&](const auto& v) { a.put(v); }, **c);
    }
    a.put(short_blocks);
    nat::Val v = na::ask_blob("cab.write", py::bytes(), own(), a.done());
    return nat::read<int64_t>(v);
}

// --- command -------------------------------------------------------------------------------
py::list<std::tuple<py::str, py::bytes>> _gather(const py::list<py::str>& paths) {
    py::list<std::tuple<py::str, py::bytes>> files;
    for (const py::str& path : paths) {
        if (py::os::path::isdir(path)) {
            // for folder, _dirs, names in sorted(os.walk(path)): (the walk's tuples sorted: by folder, which are all different)
            std::vector<std::tuple<py::str, py::list<py::str>>> walked;
            for (const auto& step : py::os::walk(path)) walked.emplace_back(std::get<0>(step), std::get<2>(step));
            std::stable_sort(walked.begin(), walked.end(), [](const auto& x, const auto& y) { return std::get<0>(x) < std::get<0>(y); });
            for (const auto& step : walked) {
                std::vector<py::str> names;
                for (const py::str& n : std::get<1>(step)) names.push_back(n);
                std::sort(names.begin(), names.end());
                for (const py::str& name : names) {
                    py::str full = py::os::path::join(std::get<0>(step), name);
                    py::bytes data = py::open_rb(full).read();
                    files.append(std::make_tuple(py::os::path::relpath(full, path).replace(py::str("/"), py::str("\\")), data));
                }
            }
        } else {
            py::bytes data = py::open_rb(path).read();
            files.append(std::make_tuple(py::os::path::basename(path), data));
        }
    }
    return files;
}

int64_t main(const py::list<py::str>& argv) {
    auto S = [](const char* s) { return py::str(s); };
    bool store = false;
    py::list<py::str> args;
    for (const py::str& a : argv) {
        if (a == S("--store")) store = true;
        else args.append(a);
    }
    // (main.__doc__: the docstring with its common indentation removed, as CPython 3.13+ compiles it)
    const char* usage =
        "python -m codebreaker cab list <file.cab>\n"
        "python -m codebreaker cab extract <file.cab> <outdir> [names...]\n"
        "python -m codebreaker cab create <out.cab> <file-or-folder>... [--store]";
    try {
        if (args.size() == 2 && args[0] == S("list")) {
            Cabinet c = read(args[1]);
            const CabInfo& info = c.info;
            py::str folders = py::join(S(", "), info.folders);
            py::print(py::percent(S("%s: set %d, cabinet %d, %d bytes, folders: %s"), py::os::path::basename(args[1]), info.set_id,
                                  info.index, info.size, folders.empty() ? S("none") : folders));
            for (int pass = 0; pass < 2; ++pass) {
                const std::optional<py::str>& v = pass == 0 ? info.prev : info.next;
                if (v && !v->empty()) py::print(py::percent(S("  %s cabinet: %s"), S(pass == 0 ? "prev" : "next"), *v));
            }
            int64_t total = 0;
            for (const Entry& e : c.files) {
                py::str mark = e.folder == FROM_PREV ? S("  (from previous cabinet)")
                               : e.folder == TO_NEXT ? S("  (into next cabinet)")
                               : e.folder == PREV_AND_NEXT ? S("  (from previous, into next)") : S("");
                py::print(py::percent(S("%10d  %04d-%02d-%02d  %-8s %s%s"), e.size, 1980 + (e.date >> 9), (e.date >> 5) & 15, e.date & 31,
                                      e.compression, e.name, mark));
                total += e.size;
            }
            py::print(py::percent(S("%d files, %d bytes"), c.files.size(), total));
            return 0;
        }
        if (args.size() >= 3 && args[0] == S("extract")) {
            py::list<py::str> rest;
            for (int64_t k = 3; k < args.size(); ++k) rest.append(args[k]);
            auto done = extract(args[1], args[2], rest.empty() ? std::nullopt : std::optional<py::list<py::str>>(rest));
            py::set<py::str> have;
            for (const py::str& d : done.keys()) have.add(d.lower());
            py::list<py::str> missing;
            for (const py::str& n : rest)
                if (!have.contains(n.lower())) missing.append(n);
            for (const py::str& name : missing) eprint(py::str("not in the cabinet: ") + name);
            py::print(py::percent(S("extracted %d files to %s"), done.size(), args[2]));
            return missing.empty() ? 0 : 1;
        }
        if (args.size() >= 3 && args[0] == S("create")) {
            py::list<py::str> srcs;
            for (int64_t k = 2; k < args.size(); ++k) srcs.append(args[k]);
            auto files = _gather(srcs);
            int64_t size = write(files, args[1], store ? S("none") : S("mszip"));
            py::print(py::percent(S("wrote %s: %d files, %d bytes"), args[1], files.size(), size));
            return 0;
        }
    } catch (const CabError& problem) {
        eprint(py::percent(S("cab: %s"), py::str(problem.what())));
        return 1;
    } catch (const py::OSError& problem) {
        eprint(py::percent(S("cab: %s"), py::str(problem.what())));
        return 1;
    }
    eprint(S(usage));
    return 2;
}

}  // namespace codebreaker::formats::cab
