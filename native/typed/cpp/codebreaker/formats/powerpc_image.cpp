// codebreaker/formats/powerpc_image.cpp -- typed C++ of codebreaker.formats.powerpc_image (see powerpc_image.hpp).
//
// The reads of the image are native/formats/core (bytes.cc, text.cc: cbf::Bytes, which keeps Python's rules --
// IndexError past the end, struct.error for a value that does not fit); the analysis of
// native/formats/win/powerpc_image.cc, a question handler that answered in pickled values, is written out here
// on typed values.

#include "codebreaker/formats/powerpc_image.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/formats/pe.hpp"
#include "json.hpp"
#include "../../../../formats/core/bytes.h"

namespace codebreaker::formats::powerpc_image {

namespace {

namespace cb = ::cbf;

// ---- the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") throw py::KeyError(py::repr(py::str(m)));
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "ZeroDivisionError") throw py::ZeroDivisionError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "AttributeError") throw py::AttributeError(m);
    if (k == "OSError") throw py::OSError(m);
    if (k == "AssertionError") throw py::AssertionError(m);
    throw py::RuntimeError(m);
}

// runs a call into the bytes reader, its exceptions made the Python's
template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cb::PyError& e) {
        raise_py(e);
    }
}

constexpr uint32_t kMflrR12 = 0x7D8802A6, kBlr = 0x4E800020, kMtctrR11 = 0x7D6903A6, kBctr = 0x4E800420;

struct Span {
    int64_t va, off, n;
};

bool offset_of(const std::vector<Span>& spans, int64_t va, int64_t& off) {
    for (const Span& s : spans)
        if (s.va <= va && va < s.va + s.n) {
            off = s.off + (va - s.va);
            return true;
        }
    return false;
}

// img.word(va): a big-endian word, or none
bool word_at(cb::Bytes data, const std::vector<Span>& spans, int64_t va, uint32_t& w) {
    int64_t off;
    if (!offset_of(spans, va, off) || off + 4 > data.size()) return false;
    w = data.be32(off);
    return true;
}

std::string dec(int64_t v) { return std::to_string(v); }

// ---- the function table (.pdata): (begin, packed) records that name a function
struct Rec {
    int64_t begin, packed;
};

std::vector<Rec> table(cb::Bytes data, bool has_off, int64_t off, int64_t size) {
    std::vector<Rec> out;
    for (int64_t i = 0; i < size / 8; ++i) {
        if (!has_off || off + 8 * i + 8 > data.size()) break;
        int64_t begin = data.be32(off + 8 * i), packed = data.be32(off + 8 * i + 4);
        if (begin && ((packed >> 8) & 0x3FFFFF)) out.push_back({begin, packed});
    }
    return out;
}

// 2: a function's own first instruction; 1: something that is an instruction; 0: not code
int64_t looks_like_code(cb::Bytes data, bool has, int64_t off) {
    if (!has || off < 0 || off + 4 > data.size()) return 0;
    uint32_t w = data.be32(off);
    if (w == kMflrR12 || (w >> 16) == 0x9421) return 2;
    uint32_t op = w >> 26;
    return (w && op != 0 && op != 1 && op != 5 && op != 6) ? 1 : 0;
}

// The register save/restore runs: each is a run of store (or load) instructions that ends in a known tail;
// every word of it is named for the register it stores, and those entered by `bl` are marked.
void runs(const std::vector<uint32_t>& words, int64_t base, std::map<int64_t, std::string>& names, std::set<int64_t>& by_call) {
    int64_t n = static_cast<int64_t>(words.size());
    auto back = [&](int64_t i, uint32_t opcode, uint32_t ra, const char* name, bool mark, bool ds) {
        int64_t j = i - 1;
        while (j >= 0 && (words[j] >> 26) == opcode && ((words[j] >> 16) & 31) == ra && (!ds || (words[j] & 3) == 0)) {
            names[base + 4 * j] = std::string(name) + "_" + dec((words[j] >> 21) & 31);
            if (mark) by_call.insert(base + 4 * j);
            --j;
        }
    };
    for (int64_t i = 0; i < n; ++i) {
        uint32_t w = words[i];
        if (w == 0x9181FFF8 && i + 1 < n && words[i + 1] == kBlr) {
            back(i, 62, 1, "__savegprlr", true, true);
        } else if (w == 0x8181FFF8 && i + 2 < n && words[i + 1] == 0x7D8803A6 && words[i + 2] == kBlr) {
            back(i, 58, 1, "__restgprlr", false, true);
        } else if (w == kBlr && i >= 2) {
            if ((words[i - 1] >> 26) == 54 && ((words[i - 1] >> 16) & 31) == 12 && (words[i - 2] >> 26) == 54)
                back(i, 54, 12, "__savefpr", true, false);
            else if ((words[i - 1] >> 26) == 50 && ((words[i - 1] >> 16) & 31) == 12 && (words[i - 2] >> 26) == 50)
                back(i, 50, 12, "__restfpr", true, false);
        }
    }
}

// one reading of the file: the layout it assumes, where the file's bytes sit, the table's records
struct Layout {
    bool memory = false;
    std::vector<Span> spans;
    std::vector<Rec> records;
    std::vector<int64_t> starts;
    int64_t score = 0;
};

Layout layout_of(bool memory, int64_t base, int64_t len, const py::list<pe::Section>& sections, std::vector<Rec> records,
                 cb::Bytes data) {
    Layout l;
    l.memory = memory;
    if (memory) {
        l.spans.push_back({base, 0, len});
    } else {
        for (const pe::Section& s : sections.vec()) {
            int64_t size = std::min(s.rawsize, s.vsize ? s.vsize : s.rawsize);
            if (size && s.rawptr) l.spans.push_back({base + s.vaddr, s.rawptr, size});
        }
    }
    for (const Rec& r : records) l.starts.push_back(r.begin);
    l.records = std::move(records);
    for (int64_t s : l.starts) {
        int64_t off = 0;
        bool has = offset_of(l.spans, s, off);
        l.score += looks_like_code(data, has, off);
    }
    return l;
}

// powerpc.read(data, base, rva, size, off_in_file_or_None, entry, sections): fills the image's attributes
void analyse(Image& im, cb::Bytes data, int64_t base, int64_t rva, int64_t size, std::optional<int64_t> in_file, int64_t entry,
             const py::list<pe::Section>& sections) {
    bool has_file_off = in_file.has_value();
    int64_t file_off = has_file_off ? *in_file : 0;
    Layout lay[2];                                       // [0] = file layout, [1] = memory layout
    for (int memory = 1; memory >= 0; --memory) {
        std::vector<Rec> records = memory ? table(data, true, rva, size) : table(data, has_file_off, file_off, size);
        lay[memory] = layout_of(memory != 0, base, data.size(), sections, std::move(records), data);
    }
    bool memory;
    if (lay[1].starts.empty() && lay[0].starts.empty()) {
        int64_t end = 0;
        for (const pe::Section& s : sections.vec()) end = std::max(end, s.vaddr + std::max<int64_t>(s.vsize, 1));
        memory = data.size() >= end;
    } else {
        memory = lay[1].score > lay[0].score;
    }
    const Layout& sel = lay[memory ? 1 : 0];

    im.memory_layout = memory;
    for (const Span& s : sel.spans) im.spans.append(std::make_tuple(s.va, s.off, s.n));
    struct Sec {
        std::string name;
        int64_t va, size, chars;
    };
    std::vector<Sec> secs;
    for (const pe::Section& s : sections.vec()) {
        secs.push_back({s.name.utf8(), base + s.vaddr, s.vsize ? s.vsize : s.rawsize, s.chars});
        im.sections.append(std::make_tuple(s.name, base + s.vaddr, s.vsize ? s.vsize : s.rawsize, s.chars));
    }
    for (const Rec& r : sel.records)
        im.extents.set(r.begin, std::make_tuple(((r.packed >> 8) & 0x3FFFFF) * 4, (r.packed & 0xFF) * 4, (r.packed >> 31) != 0));
    im.entry = entry ? base + entry : 0;

    bool have_code = false;
    int64_t lo = 0, hi = 0;
    for (const Sec& s : secs)
        if ((s.chars & 0x20000000) || s.name == ".text") {
            if (!have_code) {
                lo = s.va;
                hi = s.va + s.size;
                have_code = true;
            } else {
                lo = std::min(lo, s.va);
                hi = std::max(hi, s.va + s.size);
            }
        }
    if (!have_code) {
        im.code = std::make_tuple<int64_t, int64_t>(0, 0);
        return;
    }
    // bytes_at(lo, hi - lo)
    int64_t boff = 0, avail = 0;
    if (offset_of(sel.spans, lo, boff)) avail = data.slice(boff, boff + (hi - lo)).size();
    int64_t count = std::max<int64_t>(0, std::min(hi - lo, avail)) / 4;
    std::vector<uint32_t> words(static_cast<size_t>(count));
    for (int64_t i = 0; i < count; ++i) words[i] = data.be32(boff + 4 * i);
    std::map<int64_t, std::string> imp;
    for (int64_t i = 0; i + 3 < count; ++i)
        if (words[i + 2] == kMtctrR11 && words[i + 3] == kBctr && (words[i] >> 24) == 1 && (words[i + 1] >> 24) == 2)
            imp[lo + 4 * i] = "import" + dec((words[i] >> 16) & 0xFF) + "_ord_" + dec(words[i] & 0xFFFF);
    std::map<int64_t, std::string> names;
    std::set<int64_t> by_call;
    runs(words, lo, names, by_call);
    std::set<int64_t> found(sel.starts.begin(), sel.starts.end());
    for (int64_t i = 0; i < count; ++i) {
        uint32_t w = words[i];
        if ((w >> 26) == 18 && (w & 3) == 1) {           // bl
            int64_t d = w & 0x03FFFFFC;
            if (d & 0x02000000) d -= 0x04000000;
            int64_t t = lo + 4 * i + d;
            if (lo <= t && t < hi) found.insert(t);
        }
    }
    for (int64_t va : by_call) im.inline_.set(va, py::str(names[va]));
    for (int64_t va : found)
        if (!names.count(va) && !imp.count(va) && lo <= va && va < hi) im.functions.append(va);
    for (const auto& kv : imp) im.imports.set(kv.first, py::str(kv.second));
    im.code = std::make_tuple(lo, hi);
    py::dict<int64_t, py::str> rn;
    for (const auto& kv : names) rn.set(kv.first, py::str(kv.second));
    im.run_names = rn;
}

// ---- the virtual methods
bool library_class(const std::string& n) {
    return n.find("@std@@") != std::string::npos || n.find("@tr1@") != std::string::npos || n.find("@stlp") != std::string::npos;
}

// ".?AVCFoo@Bar@@" -> "Bar::Foo"
std::string class_name(const std::string& decorated) {
    std::vector<std::string> parts;
    std::string cur;
    std::string rest = decorated.size() > 4 ? decorated.substr(4) : "";
    for (char ch : rest) {
        if (ch == '@') {
            if (!cur.empty()) parts.push_back(cur);
            cur.clear();
        } else {
            cur += ch;
        }
    }
    if (!cur.empty()) parts.push_back(cur);
    if (parts.empty()) return decorated;
    std::string out;
    for (size_t k = parts.size(); k-- > 0;) out += parts[k] + (k ? "::" : "");
    return out;
}

bool name_char(uint8_t b) {
    return (b >= '0' && b <= '9') || (b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z') || b == '_' || b == '@' || b == '?' || b == '$';
}

py::dict<int64_t, std::tuple<py::str, int64_t>> find_virtual(cb::Bytes data, const py::list<std::tuple<int64_t, int64_t, int64_t>>& span_list,
                                                              const py::list<int64_t>& function_list) {
    std::vector<Span> spans;
    for (const auto& s : span_list.vec()) spans.push_back({std::get<0>(s), std::get<1>(s), std::get<2>(s)});
    std::set<int64_t> starts(function_list.vec().begin(), function_list.vec().end());
    auto word = [&](int64_t va, uint32_t& w) { return word_at(data, spans, va, w); };
    // the type descriptors: the decorated names (r"\.\?A[VU][\w@?$]{1,300}?\x00") with a null word after the vftable word
    std::map<int64_t, std::string> descriptors;
    int64_t n = data.size();
    for (int64_t i = 0; i + 4 < n;) {
        if (data.at(i) == '.' && data.at(i + 1) == '?' && data.at(i + 2) == 'A' && (data.at(i + 3) == 'V' || data.at(i + 3) == 'U')) {
            int64_t j = i + 4, k = 0;
            while (j < n && name_char(data.at(j)) && k < 301) {
                ++j;
                ++k;
            }
            if (k >= 1 && k <= 300 && j < n && data.at(j) == 0) {
                int64_t name_va = 0, mapped = 0;
                bool inside = false;
                for (const Span& s : spans)
                    if (s.off <= i && i < s.off + s.n) {
                        name_va = s.va + (i - s.off);
                        inside = true;
                        break;
                    }
                if (inside) {
                    int64_t td = name_va - 8;
                    uint32_t w4 = 0, w0 = 0;
                    bool has4 = word(td + 4, w4), has0 = word(td, w0);
                    if (has4 && w4 == 0 && offset_of(spans, has0 ? w0 : 0, mapped)) descriptors[td] = data.slice(i, j).str();
                }
                i = j + 1;
                continue;
            }
        }
        ++i;
    }
    // the locators: a zero signature word whose fourth word is a type descriptor
    std::map<int64_t, std::string> locators;
    for (const Span& s : spans)
        for (int64_t i = 0; i < s.n - 19; i += 4)
            if (data.be32(s.off + i) == 0) {
                int64_t td = data.be32(s.off + i + 12);
                auto it = descriptors.find(td);
                if (it != descriptors.end()) locators[s.va + i] = it->second;
            }
    // the tables: the word after a locator's address, then one word per function
    py::dict<int64_t, std::tuple<py::str, int64_t>> out;
    for (const Span& s : spans)
        for (int64_t i = 0; i < s.n - 3; i += 4) {
            auto it = locators.find(data.be32(s.off + i));
            if (it == locators.end() || library_class(it->second)) continue;
            py::str cls(class_name(it->second));
            int64_t slot = 0;
            uint32_t w;
            while (word(s.va + i + 4 + 4 * slot, w) && starts.count(w)) {
                if (!out.contains(w)) out.set(w, std::make_tuple(cls, slot));
                ++slot;
            }
        }
    return out;
}

// The address the image runs at: the XEX's load address when the file is a basefile taken from
// one (xex.basefile writes it beside the file), else the PE header's.
int64_t _load_base(const py::str& path, const pe::PEFile& pe_) {
    try {
        py::file f = py::open(path + py::str(".json"), py::str("r"), py::str("utf-8"));
        py::str text = f.read_text();
        f.close();
        py::V loaded;
        try {
            loaded = py::json_parse(text.utf8());
        } catch (const py::PyError& e) {
            if (e.type == "ValueError") throw py::ValueError(e.what());
            throw;
        }
        if (!loaded.is_dict()) throw py::AttributeError("object has no attribute 'get'");
        const py::V& b = loaded.get("base");
        if (b.is_none() || (b.is_intlike() && b.as_int() == 0)) return static_cast<int64_t>(pe_.image_base);
        if (!b.is_intlike()) throw py::TypeError("base is not an integer");
        return b.as_int();
    } catch (const py::OSError&) {
        return static_cast<int64_t>(pe_.image_base);
    } catch (const py::ValueError&) {
        return static_cast<int64_t>(pe_.image_base);
    }
}

py::dict<py::str, py::ref<Image>>& cache() {
    static py::dict<py::str, py::ref<Image>> c;
    return c;
}

}  // namespace

std::optional<int64_t> Image::offset(int64_t va) const {
    for (const auto& s : spans.vec()) {
        int64_t a = std::get<0>(s), off = std::get<1>(s), n = std::get<2>(s);
        if (a <= va && va < a + n) return off + (va - a);
    }
    return std::nullopt;
}

std::optional<int64_t> Image::word(int64_t va) const {
    std::optional<int64_t> off = offset(va);
    if (!off || *off + 4 > data.size()) return std::nullopt;
    return native([&] { return static_cast<int64_t>(cb::Bytes(data.raw()).be32(*off)); });
}

py::bytes Image::bytes_at(int64_t va, int64_t size) const {
    std::optional<int64_t> off = offset(va);
    return off ? data.slice(*off, *off + size) : py::bytes();
}

py::ref<Image> read(const py::str& path_) {
    py::str path = py::os::path::abspath(path_);
    if (cache().contains(path)) return cache().at(path);
    pe::PEFile pe_ = pe::open_pe(path);
    if (pe_.machine != MACHINE) {
        char hexv[32];
        std::snprintf(hexv, sizeof hexv, "0x%llx", static_cast<unsigned long long>(pe_.machine));
        throw py::ValueError(py::os::path::basename(path).utf8() + ": not a PowerPC image (machine type " + hexv + ")");
    }
    py::file f = py::open_rb(path);
    py::bytes data = f.read();
    f.close();
    int64_t rva = 0, size = 0;
    if (pe_.datadirs.size() > 3) {
        rva = std::get<0>(pe_.datadirs[3]);
        size = std::get<1>(pe_.datadirs[3]);
    }
    std::optional<int64_t> in_file;
    try {
        in_file = pe_._rva_to_off(rva);
    } catch (const py::Exception&) {
        in_file = std::nullopt;
    }
    int64_t base = _load_base(path, pe_);
    py::ref<Image> img = std::make_shared<Image>();
    img->path = path;
    img->base = base;
    img->data = data;
    native([&] { analyse(*img, cb::Bytes(img->data.raw()), base, rva, size, in_file, pe_.entry, pe_.sections); });
    cache().set(path, img);
    return img;
}

py::dict<int64_t, std::tuple<py::str, int64_t>> virtual_methods(Image& img) {
    if (img._virtual) return *img._virtual;
    img._virtual = native([&] { return find_virtual(cb::Bytes(img.data.raw()), img.spans, img.functions); });
    return *img._virtual;
}

py::list<std::tuple<int64_t, py::bytes>> regions(const py::str& path) {
    py::ref<Image> img = read(path);
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const auto& s : img->spans.vec())
        out.append(std::make_tuple(std::get<0>(s), img->data.slice(std::get<1>(s), std::get<1>(s) + std::get<2>(s))));
    return out;
}

}  // namespace codebreaker::formats::powerpc_image
