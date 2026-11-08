// codebreaker/source/image_writer.cpp -- typed C++ of codebreaker.source.image_writer.

#include "codebreaker/source/image_writer.hpp"

#include <algorithm>

#include "py_subprocess.hpp"

namespace codebreaker::source::image_writer {

namespace asm_ = codebreaker::translate::backends::asm_;

Function::Function(const py::str& name) {
    this->name = name;
}

int64_t Result::count(const py::str& key) const {
    if (key == py::str("code bytes")) {
        return code_bytes;
    }
    const Extra& v = more.at(key);   // (KeyError when absent)
    if (const int64_t* n = std::get_if<int64_t>(&v)) {
        return *n;
    }
    if (const bool* b = std::get_if<bool>(&v)) {
        return *b ? 1 : 0;
    }
    throw py::TypeError("%d format: a real number is required, not str");
}

Assembly::Assembly(const py::list<py::str>& lines, const std::optional<py::str>& syntax_, const std::optional<py::str>& module_,
                   const py::str& name, const py::str& reason)
    : py::list<py::str>(lines.copy()) {
    this->syntax = syntax_;
    this->module = module_;
    _parts.push_back(Part{0, name, reason, std::nullopt, std::nullopt});
}

void Assembly::part(const py::str& name, const py::str& reason, const std::optional<Origin>& origin, const std::optional<int64_t>& length) {
    _parts.push_back(Part{size(), name, reason, origin, length});
}

asm_::Document Assembly::document() const {
    py::list<asm_::Unit> out;
    std::vector<int64_t> ends;
    for (size_t i = 1; i < _parts.size(); ++i) {
        ends.push_back(_parts[i].start);
    }
    ends.push_back(size());
    for (size_t k = 0; k < _parts.size(); ++k) {
        int64_t start = _parts[k].start;
        int64_t end = ends[k];
        if (end > start) {
            // l2.fragment(...): a MachineFragment (its origin, reason and length are kept by the L2 module; the
            // assembly back end reads its node, text, name and syntax)
            asm_::Unit u;
            u.node = py::str("MachineFragment");
            u.name = _parts[k].name;
            u.syntax = syntax;
            u.asm_ = py::join(py::str("\n"), slice(start, end));
            out.append(u);
        }
    }
    // l2.module(out)
    asm_::Document d;
    d.node = py::str("Module");
    d.units = out;
    return d;
}

py::dict<int64_t, Function> functions(const py::list<int64_t>& starts, const py::dict<int64_t, py::str>& names, const py::str& fmt) {
    py::dict<int64_t, Function> out;
    for (int64_t a : starts) {
        names.setdefault(a, py::percent(fmt, a));
        out.set(a, Function(names.at(a)));
    }
    return out;
}

py::dict<int64_t, py::str> labels(const py::set<int64_t>& targets, const py::dict<int64_t, py::str>& names,
                                  const std::function<bool(int64_t)>& line_start, const py::str& fmt) {
    py::dict<int64_t, py::str> out;
    std::vector<int64_t> ts(targets.begin(), targets.end());
    std::sort(ts.begin(), ts.end());
    for (int64_t a : ts) {
        if (line_start(a)) {
            out.set(a, names.get(a, py::percent(fmt, a)));
        }
    }
    for (const auto& [a, n] : names.items()) {
        if (line_start(a)) {
            out.set(a, n);
        }
    }
    return out;
}

py::list<py::str> data_lines(const py::list<int64_t>& data, const py::str& db, const py::str& fill) {
    py::list<py::str> out;
    int64_t i = 0;
    while (i < py::len(data)) {
        int64_t run = 1;
        while (i + run < py::len(data) && data[i + run] == data[i]) {
            run += 1;
        }
        if (run >= 16) {
            out.append(py::str("        ") + py::percent(fill, run, data[i]));
            i += run;
            continue;
        }
        py::list<int64_t> row;
        while (i < py::len(data) && py::len(row) < 16) {
            int64_t same = 1;
            while (i + same < py::len(data) && data[i + same] == data[i]) {
                same += 1;
            }
            if (same >= 16) {
                break;
            }
            row.append(data[i]);
            i += 1;
        }
        out.append(py::percent("        %s %s", db, py::join(py::str(","), _items(row))));
    }
    return out;
}

py::list<py::str> data_lines(const py::bytes& data, const py::str& db, const py::str& fill) {
    py::list<int64_t> items;
    for (int64_t i = 0; i < py::len(data); ++i) {
        items.append(static_cast<int64_t>(data[i]));
    }
    return data_lines(items, db, fill);
}

bool _printable(int64_t b) {
    return 32 <= b && b < 127 && (b != 0x22 && b != 0x5C);
}

py::list<py::str> _items(const py::list<int64_t>& row) {
    py::list<py::str> items;
    int64_t i = 0;
    while (i < py::len(row)) {
        int64_t j = i;
        while (j < py::len(row) && _printable(row[j])) {
            j += 1;
        }
        if (j - i >= 4) {
            items.append(py::percent("\"%s\"", py::bytes::from_list(row.slice(i, j)).decode("ascii", "strict")));
            i = j;
        } else {
            items.append(py::percent("$%02X", row[i]));
            i += 1;
        }
    }
    return items;
}

Assembly lines(const py::list<py::str>& first, const std::optional<py::str>& syntax, const std::optional<py::str>& module) {
    return Assembly(first, syntax, module);
}

py::str text(const Assembly& out) {
    return asm_::module(out.document());
}

py::list<py::str> commented(const py::list<py::str>& lines, const py::str& what) {
    py::list<py::str> result;
    int64_t i = 0;
    for (const py::str& line : lines) {
        result.append(i == 0 ? py::percent("%s  ; %s", line, what) : line);
        i += 1;
    }
    return result;
}

Result _result(const py::str& folder, const py::str& name, const py::str& source, const py::dict<int64_t, Function>& functions,
               int64_t code_bytes, const py::dict<py::str, Extra>& more) {
    py::str path = py::os::path::join(folder, name);
    {
        py::file f = py::open(path, py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
        f.write(source);
        f.close();
    }
    Result r;
    r.source = path;
    r.functions = functions;
    r.linked = false;
    r.same = false;
    r.code_bytes = code_bytes;
    for (const auto& [k, v] : more.items()) {
        const std::string key = k.utf8();
        if (key == "source" && std::holds_alternative<py::str>(v)) {
            r.source = std::get<py::str>(v);
        } else if ((key == "linked" || key == "same") && std::holds_alternative<bool>(v)) {
            (key == "linked" ? r.linked : r.same) = std::get<bool>(v);
        } else if (key == "code bytes" && std::holds_alternative<int64_t>(v)) {
            r.code_bytes = std::get<int64_t>(v);
        } else if (key == "built" && std::holds_alternative<py::str>(v)) {
            r.built = std::get<py::str>(v);
        } else {
            r.more.set(k, v);
        }
    }
    return r;
}

std::optional<py::bytes> assembled(Result& r, const std::function<py::bytes()>& build, const py::bytes& original,
                                   const std::optional<py::str>& built_path, const py::str& what,
                                   const std::function<bool(const py::bytes&)>& same) {
    py::bytes built;
    try {
        built = build();
    } catch (const py::ValueError& e) {
        r.notes.append(py::percent("does not assemble: %s", py::str(e.what())));
        return std::nullopt;
    }
    r.linked = true;
    if (built_path.has_value() && py::truthy(*built_path)) {
        {
            py::file f = py::open(*built_path, py::str("wb"));
            f.write(built);
            f.close();
        }
        r.built = built_path;
    }
    r.same = same ? same(built) : built == original;
    if (!r.same) {
        std::optional<int64_t> at = compare::image::first_difference(built, original);
        r.notes.append(py::percent("the built %s differs from the original at byte %#x", what, !at.has_value() ? py::len(original) : *at));
    }
    return built;
}

// The x86 files written whole -------------------------------------------------------------------
const int64_t ROUNDS = 30;

py::str nasm() {
    py::str exe = py::or_(py::settings::get_str(py::str("nasm")), "");
    return py::os::path::isfile(exe) ? exe : "";
}

std::tuple<std::optional<py::str>, py::str, py::str> assemble_flat(const py::str& source, const py::str& work, const py::str& name) {
    py::str asm_path = py::os::path::join(work, name + py::str(".asm"));
    py::str out = py::os::path::join(work, name);
    py::str mp = py::os::path::join(work, name + py::str(".map"));
    {
        py::file f = py::open(asm_path, py::str("w"), py::str("latin1"), std::nullopt, py::str("\n"));
        f.write(py::percent("[map symbols %s]\n", mp.replace(py::str("\\"), py::str("/"))) + source);
        f.close();
    }
    for (const py::str& p : {out, mp}) {
        if (py::os::path::exists(p)) {
            py::os::remove(p);
        }
    }
    py::subprocess::RunOptions opts;
    opts.cwd = work;
    opts.errors = "replace";
    py::subprocess::CompletedProcess r = py::subprocess::run(
        py::list<py::str>{nasm(), py::str("-f"), py::str("bin"), py::str("-w-all"), py::str("-o"), out, asm_path}, opts);
    std::optional<py::str> built = (r.returncode == 0 && py::os::path::isfile(out)) ? std::optional<py::str>(out) : std::nullopt;
    return std::make_tuple(built, *r.stderr_ + *r.stdout_, mp);
}

py::dict<py::str, std::tuple<int64_t, int64_t>> nasm_map(const py::str& path) {
    py::dict<py::str, std::tuple<int64_t, int64_t>> syms;
    py::file f = py::open(path, py::str("r"), py::str("latin1"));
    for (const py::str& line : f) {
        std::optional<py::re::Match> m = py::re::match(py::str("^\\s*([0-9A-F]+)\\s+([0-9A-F]+)\\s+(\\S+)\\s*$"), line);
        if (m.has_value()) {
            syms.set((*m).group(3), std::make_tuple(py::int_parse((*m).group(2), 16), py::int_parse((*m).group(1), 16)));
        }
    }
    f.close();
    return syms;
}

py::str _tail(const py::str& msg, int64_t n) {
    return msg.slice(-n, py::len(msg));
}

int64_t flat_report(const py::str& path, const FlatReport& r) {
    int64_t n = py::max(static_cast<int64_t>(1), r.instructions);
    py::print(py::percent("%s: %d instructions, %d (%d%%) written as bytes; %d round%s; %s", py::os::path::basename(path), r.instructions,
                          r.raw, 100 * r.raw / n, r.rounds, r.rounds == 1 ? py::str("") : py::str("s"),
                          r.same ? py::str("builds back THE SAME") : (r.image.has_value() && py::truthy(*r.image)) ? py::str("built, differs") : py::str("not built")));
    py::print(py::percent("  source: %s", r.source));
    return r.same ? 0 : 1;
}

int64_t main(const py::list<py::str>& argv, const std::function<Result(const py::str&, const py::str&)>& reconstruct, const py::str& doc,
             const py::str& unit, const py::str& total) {
    if (!py::truthy(argv)) {
        py::print(py::split(doc, py::str("\n\n"))[0]);
        return 2;
    }
    py::str folder = py::len(argv) > 1 ? argv[1] : std::get<0>(py::os::path::splitext(argv[0])) + py::str("_source");
    Result r = reconstruct(argv[0], folder);
    py::print(py::percent("%s: %d functions, %d of %d %s reached as code -> %s", py::os::path::basename(argv[0]), py::len(r.functions),
                          r.code_bytes, r.count(total), unit, r.source));
    py::str rebuilt = r.same ? py::str("identical to the original")
                             : py::or_(py::join(py::str("; "), r.notes), py::str("differs"));
    py::print(py::percent("  rebuilt: %s", rebuilt));
    return r.same ? 0 : 1;
}

}  // namespace codebreaker::source::image_writer
