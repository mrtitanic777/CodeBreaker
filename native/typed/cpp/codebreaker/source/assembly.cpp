// codebreaker/source/assembly.cpp -- C++17 port of codebreaker.source.assembly (see assembly.hpp).

#include "codebreaker/source/assembly.hpp"

#include <mutex>

#include "codebreaker/build/driver.hpp"
#include "json.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::assembly {

namespace builder = codebreaker::build::driver;

const py::list<py::str> CONDITIONS = {py::str("o"),  py::str("no"), py::str("b"),  py::str("ae"), py::str("e"),  py::str("ne"),
                                      py::str("be"), py::str("a"),  py::str("s"),  py::str("ns"), py::str("p"),  py::str("np"),
                                      py::str("l"),  py::str("ge"), py::str("le"), py::str("g")};
const py::dict<py::str, py::str> SPELLING = {
    {py::str("pushal"), py::str("pushad")}, {py::str("popal"), py::str("popad")}, {py::str("int3"), py::str("int 3")},
    {py::str("retn"), py::str("ret")},      {py::str("fwait"), py::str("wait")},  {py::str("pushaw"), py::str("pusha")},
    {py::str("popaw"), py::str("popa")},    {py::str("retfw"), py::str("retf")},  {py::str("iretw"), py::str("iret")}};
const py::str STRING_OPS = py::str("^(rep |repe |repne )?(movs|cmps|stos|lods|scas|ins|outs)([bwd])\\b");
const py::str HEX_PATTERN = py::str("\\b0x([0-9a-f]+)\\b");

namespace {

// settings.state(name): a path inside the state folder, which is created if it is not there.
py::str state_(const py::str& name) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), name);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

std::mutex& probe_lock_() {
    static std::mutex m;
    return m;
}

}  // namespace

std::tuple<std::optional<py::str>, py::str> assemble(const py::str& source, const py::str& work_) {
    py::str wasm = builder::tool(py::str("wasm"));
    if (wasm.empty()) {
        return {std::nullopt, py::str("no assembler (settings.json: openwatcom)")};
    }
    py::str work = py::os::path::abspath(work_);   // the assembler runs with this as its cwd
    py::str obj = py::os::path::join(work, std::get<0>(py::os::path::splitext(py::os::path::basename(source))) + py::str(".obj"));
    if (py::os::path::exists(obj)) {
        py::os::remove(obj);
    }
    auto [code, log] = builder::_run(py::list<py::str>{wasm, py::str("-zq"), py::str("-fo=") + obj, py::os::path::abspath(source)}, work);
    return {(code == 0 && py::os::path::isfile(obj)) ? std::optional<py::str>(obj) : std::nullopt, log};
}

std::tuple<std::optional<py::str>, py::str> link(const py::list<py::str>& lines, const py::str& out_, const py::str& work_) {
    py::str wlink = builder::tool(py::str("wlink"));
    if (wlink.empty()) {
        return {std::nullopt, py::str("no linker (settings.json: openwatcom)")};
    }
    py::str work = py::os::path::abspath(work_);
    py::str out = py::os::path::abspath(out_);
    py::str script = std::get<0>(py::os::path::splitext(out)) + py::str(".lnk");
    {
        py::list<py::str> all = lines.copy();
        all.append(py::str("name ") + out);
        py::file f = py::open(script, py::str("w"), std::nullopt, std::nullopt, py::str("\r\n"));
        f.write(py::join(py::str("\n"), all) + py::str("\n"));
        f.close();
    }
    if (py::os::path::exists(out)) {
        py::os::remove(out);
    }
    auto [code, log] = builder::_run(py::list<py::str>{wlink, py::str("@") + script}, work);
    return {(code == 0 && py::os::path::isfile(out)) ? std::optional<py::str>(out) : std::nullopt, log};
}

py::str operands(const py::str& text) {
    static const py::re::Pattern hex = py::re::compile(HEX_PATTERN);
    return hex.sub([](const py::re::Match& m) { return py::str("0") + m.group(1).upper() + py::str("h"); }, text);
}

py::str mnemonic(const py::str& name) {
    return SPELLING.get(name, name);
}

py::list<py::str> data_lines(const py::bytes& data, int64_t per_line) {
    py::list<py::str> out;
    int64_t at = 0;

    auto plain = [&](const py::bytes& chunk) {
        for (int64_t k = 0; k < chunk.size(); k += per_line) {
            py::list<py::str> items;
            for (int64_t i = k; i < std::min(k + per_line, chunk.size()); ++i) {
                items.append(py::percent("0%02Xh", chunk[i]));
            }
            out.append(py::str("        db   ") + py::join(py::str(", "), items));
        }
    };
    // re.finditer(rb"[\x20-\x26\x28-\x7e]{5,}", data): the runs of printable bytes, the quote itself aside, five or more long
    auto printable = [](int64_t b) { return b >= 0x20 && b <= 0x7e && b != 0x27; };
    int64_t i = 0;
    const int64_t n = data.size();
    while (i < n) {
        if (!printable(data[i])) {
            ++i;
            continue;
        }
        int64_t j = i;
        while (j < n && printable(data[j])) {
            ++j;
        }
        if (j - i >= 5) {
            plain(data.slice(at, i));
            py::str text = data.slice(i, j).decode("latin1");
            for (int64_t k = 0; k < text.size(); k += 60) {
                out.append(py::str("        db   '") + text.slice(k, k + 60) + py::str("'"));
            }
            at = j;
        }
        i = j;
    }
    plain(data.slice_from(at));
    return out;
}

py::str _key(const Candidate& candidate, const py::str& mode) {
    return py::percent("%s|%s|%s|%s", mode, std::get<0>(candidate), std::get<1>(candidate).hex(), std::get<2>(candidate).hex());
}

py::dict<py::str, bool> _known() {
    try {
        py::file f = py::open(state_(py::str("assembly_probes.json")), py::str("r"), py::str("utf-8"));
        py::str text = f.read_text();
        f.close();
        py::V doc = py::json_parse(text.utf8());
        py::dict<py::str, bool> known;
        if (doc.is_dict()) {
            for (const auto& kv : doc.items()) {
                known.set(py::str(kv.first.str()), py::truthy(kv.second));
            }
        }
        return known;
    } catch (const py::OSError&) {
        return {};
    } catch (const py::ValueError&) {
        return {};
    } catch (const py::PyError&) {   // (the JSON reader's bad-JSON error)
        return {};
    }
}

py::set<Candidate> probe(const py::list<Candidate>& candidates, const py::set<py::str>& labels, const py::str& outdir,
                         const py::str& mode, const py::set<py::str>& near) {
    std::lock_guard<std::mutex> lock(probe_lock_());
    return _probe(candidates, labels, outdir, mode, near);
}

py::set<Candidate> _probe(const py::list<Candidate>& candidates, const py::set<py::str>& labels, const py::str& outdir,
                          const py::str& mode, const py::set<py::str>& near) {
    py::dict<py::str, bool> known = _known();
    py::set<Candidate> distinct = py::set<Candidate>::from(candidates);
    py::list<Candidate> todo;
    for (const Candidate& c : distinct) {
        if (!known.contains(_key(c, mode))) {
            todo.append(c);
        }
    }
    py::set<Candidate> refused;
    const bool flat = mode == py::str("flat32");
    py::str use = flat ? py::str("use32") : py::str("use16");
    py::list<py::str> head = flat ? py::list<py::str>{py::str(".386p"), py::str(".model flat")} : py::list<py::str>{py::str(".386p")};
    py::list<py::str> sorted_labels = py::list<py::str>::from(labels);
    std::sort(sorted_labels.vec().begin(), sorted_labels.vec().end(),
              [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    static const py::re::Pattern error_line = py::re::compile(py::str("\\((\\d+)\\)\\s*:\\s*Error"));
    for (int64_t attempt = 0; attempt < 16; ++attempt) {
        if (todo.empty()) {
            break;
        }
        py::list<py::str> lines = head.copy();
        lines.append(py::str("PD segment para public ") + use + py::str(" 'PXCODE'"));
        for (const py::str& n : sorted_labels) {
            lines.append(py::percent("%s label %s", n, near.contains(n) ? py::str("near") : py::str("byte")));
        }
        lines.append(py::str("PD ends"));
        lines.append(py::str("PC segment para public ") + use + py::str(" 'PCODE'"));
        if (!flat) {
            lines.append(py::str("        assume cs:PC, ds:nothing, es:nothing, ss:nothing"));
        }
        py::dict<int64_t, Candidate> line_of;
        for (int64_t n = 0; n < todo.size(); ++n) {
            const Candidate& c = todo[n];
            lines.append(py::percent("        org  %d", 32 * n));
            line_of.set(lines.size() + 1, c);
            lines.append(py::str("        ") + (refused.contains(c) ? py::str("nop") : std::get<0>(c)));
        }
        lines.append(py::str("PC ends"));
        lines.append(py::str("end"));
        py::str src = py::os::path::join(outdir, py::str("probe.asm"));
        {
            py::file f = py::open(src, py::str("w"), py::str("latin1"), std::nullopt, py::str("\r\n"));
            f.write(py::join(py::str("\n"), lines) + py::str("\n"));
            f.close();
        }
        auto [obj, messages] = assemble(src, outdir);
        if (obj) {
            py::bytes raw;
            {
                py::file f = py::open_rb(*obj);
                raw = f.read();
                f.close();
            }
            py::dict<int64_t, py::bytes> got = _last_segment_data(raw);
            for (int64_t n = 0; n < todo.size(); ++n) {
                const Candidate& c = todo[n];
                py::bytes out = got.get(32 * n, py::bytes());
                bool ok = !refused.contains(c) && out.size() == std::get<2>(c).size();
                if (ok) {
                    std::string kept;
                    const py::bytes& mask = std::get<2>(c);
                    for (int64_t i = 0; i < out.size(); ++i) {
                        if (!mask[i]) {
                            kept.push_back(static_cast<char>(out[i]));
                        }
                    }
                    ok = py::bytes(kept) == std::get<1>(c);
                }
                known.set(_key(c, mode), ok);
            }
            todo = py::list<Candidate>();
            break;
        }
        py::set<Candidate> failed;
        for (const auto& m : error_line.finditer(messages)) {
            int64_t number = py::int_parse(m.group(1), 10);
            if (line_of.contains(number)) {
                failed.add(line_of.at(number));
            }
        }
        if (failed.empty()) {   // the probe file itself is refused: nothing is known good
            for (const Candidate& c : todo) {
                known.set(_key(c, mode), false);
            }
            todo = py::list<Candidate>();
            break;
        }
        for (const Candidate& c : failed) {
            refused.add(c);
        }
    }
    {
        // json.dump(known, f, indent=0, sort_keys=True)
        std::vector<std::u32string> keys;
        std::vector<py::str> ordered;
        for (const py::str& k : known.keys()) {
            ordered.push_back(k);
        }
        std::sort(ordered.begin(), ordered.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
        std::string text = "{";
        for (size_t i = 0; i < ordered.size(); ++i) {
            text += (i ? ",\n" : "\n");
            py::json::put(text, ordered[i]);
            text += known.at(ordered[i]) ? ": true" : ": false";
        }
        text += ordered.empty() ? "}" : "\n}";
        py::file f = py::open(state_(py::str("assembly_probes.json")), py::str("w"), py::str("utf-8"));
        f.write(py::str(text));
        f.close();
    }
    py::set<Candidate> result;
    for (const Candidate& c : distinct) {
        if (known.get(_key(c, mode), false)) {
            result.add(c);
        }
    }
    return result;
}

py::dict<int64_t, py::bytes> _last_segment_data(const py::bytes& obj) {
    py::dict<int64_t, py::dict<int64_t, py::bytes>> chunks;
    int64_t i = 0, segs = 0;
    const int64_t n = obj.size();
    while (i + 3 <= n) {
        const int64_t kind = obj[i];
        const int64_t end = i + 3 + (obj[i + 1] | (obj[i + 2] << 8));
        if (end > n) {
            break;
        }
        if (kind == 0x98 || kind == 0x99) {
            ++segs;
        } else if (kind == 0xA0 || kind == 0xA1) {
            int64_t q = i + 4;
            int64_t off = 0;
            if (kind == 0xA1) {
                off = py::unpack_from1<int64_t>("<I", obj, q);
                q += 4;
            } else {
                off = py::unpack_from1<int64_t>("<H", obj, q);
                q += 2;
            }
            chunks.setdefault(obj[i + 3], py::dict<int64_t, py::bytes>()).set(off, obj.slice(q, end - 1));
        }
        i = end;
    }
    py::dict<int64_t, py::bytes> out;
    std::optional<int64_t> last;
    std::vector<std::tuple<int64_t, py::bytes>> sorted_items = chunks.get(segs, py::dict<int64_t, py::bytes>()).items();
    std::sort(sorted_items.begin(), sorted_items.end(),
              [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (const auto& [off, data] : sorted_items) {
        if (last && *last + out.at(*last).size() == off) {
            out.set(*last, out.at(*last) + data);
        } else {
            out.set(off, data);
            last = off;
        }
    }
    return out;
}

}  // namespace codebreaker::source::assembly
