// powerpc_image.cc -- what a PowerPC PE image states about its own code: the function table,
// import stubs, register save/restore runs, bl targets, and the virtual methods run-time type
// information names.  codebreaker/formats/powerpc_image.py.  The shim asks the PE reader (also
// C++) for the header facts and keeps the Image object and its word/offset/bytes_at.
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../core/build.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::ppc {
namespace {

constexpr uint32_t kMflrR12 = 0x7D8802A6, kBlr = 0x4E800020, kMtctrR11 = 0x7D6903A6, kBctr = 0x4E800420;

struct Span {
    int64_t va, off, n;
};
struct Sec {
    std::string name;
    int64_t va, size, chars;
};

struct Img {
    bool memory = false;
    std::vector<Span> spans;
    std::vector<Sec> sections;
    std::vector<std::pair<int64_t, std::vector<int64_t>>> extents;   // start -> (size, prologue, handler)
    std::vector<int64_t> starts;
};

bool offset_of(const std::vector<Span> &spans, int64_t va, int64_t &off) {
    for (auto &s : spans)
        if (s.va <= va && va < s.va + s.n) {
            off = s.off + (va - s.va);
            return true;
        }
    return false;
}
// img.word(va): a big-endian word, or none
bool word_at(Bytes data, const std::vector<Span> &spans, int64_t va, uint32_t &w) {
    int64_t off;
    if (!offset_of(spans, va, off) || off + 4 > data.size()) return false;
    w = data.be32(off);
    return true;
}

struct Rec {
    int64_t begin, packed;
};
std::vector<Rec> table(Bytes data, bool has_off, int64_t off, int64_t size) {
    std::vector<Rec> out;
    for (int64_t i = 0; i < size / 8; ++i) {
        if (!has_off || off + 8 * i + 8 > data.size()) break;
        int64_t begin = data.be32(off + 8 * i), packed = data.be32(off + 8 * i + 4);
        if (begin && ((packed >> 8) & 0x3FFFFF)) out.push_back({begin, packed});
    }
    return out;
}

int looks_like_code(Bytes data, bool has, int64_t off) {
    if (!has || off < 0 || off + 4 > data.size()) return 0;
    uint32_t w = data.be32(off);
    if (w == kMflrR12 || (w >> 16) == 0x9421) return 2;
    uint32_t op = w >> 26;
    return (w && op != 0 && op != 1 && op != 5 && op != 6) ? 1 : 0;
}

void runs(const std::vector<uint32_t> &words, int64_t base, std::map<int64_t, std::string> &names, std::set<int64_t> &by_call) {
    int64_t n = static_cast<int64_t>(words.size());
    auto back = [&](int64_t i, uint32_t opcode, uint32_t ra, const char *name, bool mark, bool ds) {
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

Img make_image(bool memory, int64_t base, int64_t len, const Value &sections, const std::vector<Rec> &records) {
    Img img;
    img.memory = memory;
    if (memory) {
        img.spans.push_back({base, 0, len});
    } else {
        for (const Value &s : sections.items) {
            int64_t rawsize = s.get("rawsize")->as_int(), vsize = s.get("vsize")->as_int(), rawptr = s.get("rawptr")->as_int();
            int64_t size = std::min(rawsize, vsize ? vsize : rawsize);
            if (size && rawptr) img.spans.push_back({base + s.get("vaddr")->as_int(), rawptr, size});
        }
    }
    for (const Value &s : sections.items) {
        int64_t vsize = s.get("vsize")->as_int(), rawsize = s.get("rawsize")->as_int();
        img.sections.push_back({s.get("name")->as_str(), base + s.get("vaddr")->as_int(), vsize ? vsize : rawsize, s.get("chars")->as_int()});
    }
    for (auto &r : records) {
        img.starts.push_back(r.begin);
        std::vector<int64_t> e = {((r.packed >> 8) & 0x3FFFFF) * 4, (r.packed & 0xFF) * 4, (r.packed >> 31) ? 1 : 0};
        bool found = false;
        for (auto &kv : img.extents)
            if (kv.first == r.begin) {
                kv.second = e;
                found = true;
            }
        if (!found) img.extents.push_back({r.begin, e});
    }
    return img;
}

Value spans_value(const std::vector<Span> &spans) {
    Value v = vlist();
    for (auto &s : spans) push(v, vtuple_of({vint(s.va), vint(s.off), vint(s.n)}));
    return v;
}

// powerpc.read(data, base, rva, size, off_in_file_or_None, entry, sections)
void read(Call &c) {
    Bytes data = c.data;
    int64_t base = c.int_arg(0), rva = c.int_arg(1), size = c.int_arg(2), entry = c.int_arg(4);
    bool has_file_off = !c.arg(3).is_none();
    int64_t file_off = has_file_off ? c.int_arg(3) : 0;
    const Value &sections = c.arg(5);

    struct Scored {
        int64_t score;
        std::vector<int64_t> starts;
        Img img;
    };
    Scored scored[2];                                   // [0] = file layout, [1] = memory layout
    for (int memory = 1; memory >= 0; --memory) {
        std::vector<Rec> records = memory ? table(data, true, rva, size) : table(data, has_file_off, file_off, size);
        std::vector<int64_t> starts;
        for (auto &r : records) starts.push_back(r.begin);
        Img img = make_image(memory, base, data.size(), sections, records);
        int64_t sum = 0;
        for (int64_t s : starts) {
            int64_t off;
            bool has = offset_of(img.spans, s, off);
            sum += looks_like_code(data, has, off);
        }
        scored[memory] = {sum, starts, std::move(img)};
    }
    bool memory;
    if (scored[1].starts.empty() && scored[0].starts.empty()) {
        int64_t end = 0;
        for (const Value &s : sections.items) {
            int64_t e = s.get("vaddr")->as_int() + std::max<int64_t>(s.get("vsize")->as_int(), 1);
            end = std::max(end, e);
        }
        memory = data.size() >= end;
    } else {
        memory = scored[1].score > scored[0].score;
    }
    Scored &sel = scored[memory ? 1 : 0];
    Img &img = sel.img;

    Value out = vdict();
    put(out, "memory_layout", vbool(memory));
    put(out, "spans", spans_value(img.spans));
    Value secs = vlist();
    for (auto &s : img.sections) push(secs, vtuple_of({vstr(s.name), vint(s.va), vint(s.size), vint(s.chars)}));
    put(out, "sections", secs);
    Value extents = vdict();
    for (auto &kv : img.extents)
        put(extents, kv.first, vtuple_of({vint(kv.second[0]), vint(kv.second[1]), vbool(kv.second[2] != 0)}));
    Value entry_value = vint(entry ? base + entry : 0);

    bool have_code = false;
    int64_t lo = 0, hi = 0;
    for (auto &s : img.sections)
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
    Value functions = vlist(), imports = vdict(), inl = vdict();
    if (!have_code) {
        put(out, "functions", functions);
        put(out, "imports", imports);
        put(out, "inline", inl);
        put(out, "code", vtuple_of({vint(0), vint(0)}));
        put(out, "extents", extents);
        put(out, "entry", entry_value);
        emit(c.out, out);
        return;
    }
    // bytes_at(lo, hi - lo)
    int64_t boff, avail = 0;
    if (offset_of(img.spans, lo, boff)) avail = data.slice(boff, boff + (hi - lo)).size();
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
        if ((w >> 26) == 18 && (w & 3) == 1) {
            int64_t d = w & 0x03FFFFFC;
            if (d & 0x02000000) d -= 0x04000000;
            int64_t t = lo + 4 * i + d;
            if (lo <= t && t < hi) found.insert(t);
        }
    }
    for (int64_t va : by_call) put(inl, va, vstr(names[va]));
    for (int64_t va : found)
        if (!names.count(va) && !imp.count(va) && lo <= va && va < hi) push(functions, vint(va));
    for (auto &kv : imp) put(imports, kv.first, vstr(kv.second));
    put(out, "functions", functions);
    put(out, "imports", imports);
    put(out, "inline", inl);
    put(out, "code", vtuple_of({vint(lo), vint(hi)}));
    put(out, "extents", extents);
    put(out, "entry", entry_value);
    Value rn = vdict();
    for (auto &kv : names) put(rn, kv.first, vstr(kv.second));
    put(out, "run_names", rn);
    emit(c.out, out);
}

// powerpc.virtual(data, spans, functions) -> {address: (class, slot)}
bool library_class(const std::string &n) {
    return n.find("@std@@") != std::string::npos || n.find("@tr1@") != std::string::npos || n.find("@stlp") != std::string::npos;
}
std::string class_name(const std::string &decorated) {
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

void virtual_methods(Call &c) {
    Bytes data = c.data;
    std::vector<Span> spans;
    for (const Value &s : c.arg(0).items) spans.push_back({s[0].as_int(), s[1].as_int(), s[2].as_int()});
    std::set<int64_t> starts;
    for (const Value &f : c.arg(1).items) starts.insert(f.as_int());
    auto at = [&](int64_t off, int64_t &va) {
        for (auto &s : spans)
            if (s.off <= off && off < s.off + s.n) {
                va = s.va + (off - s.off);
                return true;
            }
        return false;
    };
    auto word = [&](int64_t va, uint32_t &w) { return word_at(data, spans, va, w); };
    std::map<int64_t, std::string> descriptors;
    int64_t n = data.size();
    for (int64_t i = 0; i + 4 < n;) {
        // r"\.\?A[VU][\w@?$]{1,300}?\x00"
        if (data.at(i) == '.' && data.at(i + 1) == '?' && data.at(i + 2) == 'A' && (data.at(i + 3) == 'V' || data.at(i + 3) == 'U')) {
            int64_t j = i + 4, k = 0;
            while (j < n && name_char(data.at(j)) && k < 301) {
                ++j;
                ++k;
            }
            if (k >= 1 && k <= 300 && j < n && data.at(j) == 0) {
                int64_t name_va;
                if (at(i, name_va)) {
                    int64_t td = name_va - 8;
                    uint32_t w4, w0;
                    bool has4 = word(td + 4, w4), has0 = word(td, w0);
                    int64_t offv;
                    if (has4 && w4 == 0 && offset_of(spans, has0 ? w0 : 0, offv)) descriptors[td] = data.slice(i, j).str();
                }
                i = j + 1;
                continue;
            }
        }
        ++i;
    }
    std::map<int64_t, std::string> locators;
    for (auto &s : spans)
        for (int64_t i = 0; i < s.n - 19; i += 4)
            if (data.be32(s.off + i) == 0) {
                int64_t td = data.be32(s.off + i + 12);
                auto it = descriptors.find(td);
                if (it != descriptors.end()) locators[s.va + i] = it->second;
            }
    Value out = vdict();
    for (auto &s : spans)
        for (int64_t i = 0; i < s.n - 3; i += 4) {
            auto it = locators.find(data.be32(s.off + i));
            if (it == locators.end() || library_class(it->second)) continue;
            std::string cls = class_name(it->second);
            int64_t slot = 0;
            uint32_t w;
            while (word(s.va + i + 4 + 4 * slot, w) && starts.count(w)) {
                if (!lookup(out, vint(w))) put(out, w, vtuple_of({vstr(cls), vint(slot)}));
                ++slot;
            }
        }
    emit(c.out, out);
}

}  // namespace

CBF_QUESTIONS(powerpc) {
    add("powerpc.read", read);
    add("powerpc.virtual", virtual_methods);
}

}  // namespace cbf::ppc
