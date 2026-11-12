// codebreaker/exact/draft/candidates.hpp -- compat shim for cb/exact/draft/candidates.hpp (docs/CPP_DESIGN.md 6.3). The
// functions convert and forward; deleted with its last old-layer consumer (prove, the draft package).
//
// candidates.py -- the model's text made the project's source: the engine's names, structures, longs, the frame's
// chars and buffers, the library's helpers, and every rewrite applied to make the candidates a compiler is given.
//
// What the passes learn - the old layer's tables of text.hpp (ARRAYS, STRUCTARR, UCHAR, BYTEG, FILEPTR, PTRSIZE,
// RANGES, LONGINFO) and this header's own (FPINFO, STRUCTPTR, FRAMEW, FRAME, FILLNAME) - stays in the old layer's shape.
// A pass is handed a cb::exact::draft::candidates::State made from them, and the tables are written back from it; that
// and the conversion of arguments is all the code here does. `prj` is the plain struct forms::Prj.
//
//   * `inv` (the model's checker) is the small interface Inv below: the C text of the code read in the checker's
//     byte-marking mode (nullopt: it raised).
//   * `widths` - {(pointer, offset)} and the globals read a byte at a time - are the two sets of ByteFields.
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/draft/candidates.hpp"
#include "codebreaker/exact/draft/forms.hpp"
#include "codebreaker/exact/standin.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::candidates {

using Widths = py::set<std::tuple<py::str, int64_t>>;

struct ByteFields {
    Widths fields;
    py::set<py::str> globals;
};

// the model's checker as byte_fields takes it
struct Inv {
    bool has_mark = false;   // hasattr(inv, "MARK")
    // inv.MARK = True; fs, params, stmts, pt, loc = inv.recover(code); inv.to_c("f", ..) -> the C text (nullopt: raised)
    std::function<std::optional<py::str>(const py::bytes& code)> checked_c;
};

// (the function's floating point: types, its value, any)
struct FpInfo {
    py::dict<py::str, py::str> types;      // "types": name -> type
    std::optional<py::str> ret;            // "ret"
    bool fp = false;                       // "fp"
    void clear() {
        types.clear();
        ret = std::nullopt;
        fp = false;
    }
};
inline FpInfo FPINFO;

inline const py::dict<py::str, py::str> STRUCTPTR{};           // (data the draft declares a struct pointer: localize)
inline const py::set<std::tuple<int64_t, py::str>> FRAMEW{};   // (the frame slots the code reads or writes, by width: (+-N, "byte" | "word" | "zx"))
inline const py::list<int64_t> FRAME = py::list<int64_t>{0};
inline const py::list<py::str> FILLNAME = py::list<py::str>{py::str("_pf")};

namespace shim {

namespace nc = cb::exact::draft::candidates;
namespace nt = cb::exact::draft::text;
using cb::compat::from_py;
using cb::compat::guard;
using cb::compat::to_py;

inline std::string slot_name(nc::SlotWidth w) { return w == nc::SlotWidth::Byte ? "byte" : w == nc::SlotWidth::Word ? "word" : "zx"; }
inline nc::SlotWidth slot_width(const std::string& s) { return s == "byte" ? nc::SlotWidth::Byte : s == "word" ? nc::SlotWidth::Word : nc::SlotWidth::ZeroExtended; }

// The old tables as a State, and back (the tables are replaced by the State's).
inline nc::State state_from_old() {
    nc::State s;
    s.facts = forms::shim::facts_from_old();
    for (const auto& u : text::UCHAR) {
        if (std::holds_alternative<py::str>(u)) s.facts.unsigned_globals.insert(std::get<py::str>(u).utf8());
        else s.facts.unsigned_sites.insert({std::get<0>(std::get<std::tuple<py::str, int64_t>>(u)).utf8(), std::get<1>(std::get<std::tuple<py::str, int64_t>>(u))});
    }
    for (const py::str& g : text::BYTEG) s.facts.byte_globals.insert(g.utf8());
    for (const auto& r : text::RANGES) s.facts.ranges.insert(nt::Range{std::get<0>(r), std::get<1>(r), std::get<2>(r)});
    const auto& li = text::LONGINFO;
    if (li.unsigned_) for (const py::str& x : *li.unsigned_) s.facts.longs.unsigned_names.insert(x.utf8());
    s.facts.longs.ret = li.ret;
    if (li.fields) for (const int64_t x : *li.fields) s.facts.longs.fields.insert(x);
    if (li.globals) for (const py::str& x : *li.globals) s.facts.longs.globals.insert(x.utf8());
    if (li.funcs) for (const py::str& x : *li.funcs) s.facts.longs.funcs.insert(x.utf8());
    for (const py::str& k : STRUCTPTR.keys()) s.struct_pointers[k.utf8()] = STRUCTPTR.at(k).utf8();
    for (const auto& [off, w] : FRAMEW) s.frame_slots.insert(nc::FrameSlot{off, slot_width(w.utf8())});
    for (const py::str& k : FPINFO.types.keys()) s.fp.types[k.utf8()] = FPINFO.types.at(k).utf8();
    if (FPINFO.ret) s.fp.ret = FPINFO.ret->utf8();
    s.fp.fp = FPINFO.fp;
    return s;
}

template <class Set>
inline void clear_set(const Set& set) {
    std::vector<std::decay_t<decltype(*set.begin())>> items(set.begin(), set.end());
    for (const auto& x : items) const_cast<Set&>(set).discard(x);
}

inline void state_to_old(const nc::State& s) {
    text::ARRAYS.clear();
    text::STRUCTARR.clear();
    text::PTRSIZE.clear();
    text::FILEPTR.clear();
    forms::shim::facts_to_old(s.facts);
    clear_set(text::UCHAR);
    for (const auto& site : s.facts.unsigned_sites)
        text::UCHAR.add(std::variant<std::tuple<py::str, int64_t>, py::str>(std::in_place_index<0>, std::make_tuple(py::str(site.pointer), site.offset)));
    for (const std::string& g : s.facts.unsigned_globals) text::UCHAR.add(std::variant<std::tuple<py::str, int64_t>, py::str>(std::in_place_index<1>, py::str(g)));
    clear_set(text::BYTEG);
    for (const std::string& g : s.facts.byte_globals) text::BYTEG.add(py::str(g));
    clear_set(text::RANGES);
    for (const auto& r : s.facts.ranges) text::RANGES.add(std::make_tuple(r.lo, r.hi, r.above));
    text::LONGINFO.clear();
    const auto& l = s.facts.longs;
    if (!l.unsigned_names.empty()) {
        text::LONGINFO.unsigned_ = py::set<py::str>();
        for (const std::string& x : l.unsigned_names) text::LONGINFO.unsigned_->add(py::str(x));
    }
    text::LONGINFO.ret = l.ret;
    if (!l.fields.empty()) {
        text::LONGINFO.fields = py::set<int64_t>();
        for (const int64_t x : l.fields) text::LONGINFO.fields->add(x);
    }
    if (!l.globals.empty()) {
        text::LONGINFO.globals = py::set<py::str>();
        for (const std::string& x : l.globals) text::LONGINFO.globals->add(py::str(x));
    }
    if (!l.funcs.empty()) {
        text::LONGINFO.funcs = py::set<py::str>();
        for (const std::string& x : l.funcs) text::LONGINFO.funcs->add(py::str(x));
    }
    STRUCTPTR.clear();
    for (const auto& [k, v] : s.struct_pointers) STRUCTPTR.set(py::str(k), py::str(v));
    clear_set(FRAMEW);
    for (const auto& slot : s.frame_slots) FRAMEW.add(std::make_tuple(slot.offset, py::str(slot_name(slot.width))));
    FPINFO.types.clear();
    for (const auto& [k, v] : s.fp.types) FPINFO.types.set(py::str(k), py::str(v));
    FPINFO.ret = s.fp.ret ? std::optional<py::str>(py::str(*s.fp.ret)) : std::nullopt;
    FPINFO.fp = s.fp.fp;
}

// What the new passes read of the project, with the bytes of its image.
struct SettingsHolder {
    cb::Bytes image;
    nc::Settings settings;
    explicit SettingsHolder(const forms::Prj& prj) {
        image = from_py(prj.image);
        settings.kit = forms::shim::kit_of(prj);
        if (prj.frame_order) {
            cb::exact::draft::forms::FrameRule rule;
            if (prj.frame_order->mod && *prj.frame_order->mod > 0) rule.mod = static_cast<std::uint32_t>(*prj.frame_order->mod);
            settings.frame_order = rule;
        }
        settings.table_macros = forms::shim::specs_of(prj.table_macros);
        settings.data_macros = forms::shim::specs_of(prj.data_macros);
        settings.macros = forms::shim::specs_of(prj.macros);
        settings.fp_conversions = from_py(prj.fp_conversions);
        for (const py::str& k : prj.long_helpers.keys()) settings.long_helpers[k.utf8()] = prj.long_helpers.at(k).utf8();
        settings.image = image;
        settings.data_segment = cb::compat::int_from_py<std::uint32_t>(prj.dgroup);
        if (!prj.dnames) {   // (prj._dnames: made once)
            py::dict<int64_t, py::str> made;
            for (const auto& [address, name] : nc::data_names_of(standin::detail::symbols_to_new(prj.symbols))) made.set(static_cast<int64_t>(address), to_py(name));
            prj.dnames = made;
        }
        for (const int64_t k : prj.dnames->keys()) settings.data_names[static_cast<std::uint32_t>(k)] = prj.dnames->at(k).utf8();
    }
};

inline std::map<std::uint32_t, std::string> names_of(const py::dict<int64_t, py::str>& names) {
    std::map<std::uint32_t, std::string> out;
    for (const int64_t k : names.keys())
        if (k >= 0 && k <= 0xFFFFFFFFLL) out[static_cast<std::uint32_t>(k)] = names.at(k).utf8();
    return out;
}

inline std::set<nt::Site> sites_of(const Widths& w) {
    std::set<nt::Site> out;
    for (const auto& [p, o] : w) out.insert({p.utf8(), o});
    return out;
}

class OldMarking final : public nc::MarkingModel {
  public:
    explicit OldMarking(const Inv& inv) : inv_(inv) {}
    std::optional<std::string> marked_c(cb::ByteView code) const override {
        const auto t = inv_.checked_c(to_py(code));
        return t ? std::optional<std::string>(t->utf8()) : std::nullopt;
    }

  private:
    const Inv& inv_;
};

}  // namespace shim

namespace shim {

// Runs a pass over the old tables: the State made from them, the pass, the tables written back.
template <class F>
auto with_state(F&& pass) {
    return guard([&] {
        nc::State state = state_from_old();
        auto result = pass(state);
        state_to_old(state);
        return result;
    });
}

inline std::optional<py::str> opt_text(const std::optional<std::string>& s) { return s ? std::optional<py::str>(to_py(*s)) : std::nullopt; }

}  // namespace shim

// the pointers the text goes through to fields (`p->fN`, N its offset) declared pointers to a struct of those fields
inline py::str structs(py::str text, const Widths& widths) {
    return shim::to_py(shim::with_state([&](shim::nc::State& s) { return shim::nc::structs(text.utf8(), shim::sites_of(widths), s); }));
}

// [(source, registers left)] - the model's text as the project's source, then the variants
inline py::list<std::tuple<py::str, py::list<py::str>>> candidates(const forms::Prj& prj, const py::str& text, int64_t at, const py::dict<int64_t, py::str>& names,
                                                                    const py::list<py::str>& regs, const py::dict<py::str, int64_t>& regparam,
                                                                    const ByteFields& widths = ByteFields()) {
    shim::SettingsHolder holder(prj);
    auto caches = forms::shim::caches_from_old(prj);
    shim::nc::ByteFields w;
    w.sites = shim::sites_of(widths.fields);
    for (const py::str& g : widths.globals) w.globals.insert(g.utf8());
    shim::nc::RegisterParams params;
    for (const py::str& k : regparam.keys()) params.insert_or_assign(k.utf8(), static_cast<int>(regparam.at(k)));
    const auto made = shim::with_state([&](shim::nc::State& s) {
        return shim::nc::candidates(holder.settings, text.utf8(), cb::compat::int_from_py<std::uint32_t>(at), shim::names_of(names), shim::from_py(regs), params, w,
                                    cb::compat::int_from_py<std::uint32_t>(FRAME[0]), s, caches);
    });
    forms::shim::caches_to_old(caches);
    py::list<std::tuple<py::str, py::list<py::str>>> out;
    for (const auto& c : made) out.append({shim::to_py(c.source), shim::to_py(c.registers_left)});
    return out;
}

// the fields the code reads or writes a byte of (fills FRAMEW and text::RANGES)
inline ByteFields byte_fields(const Inv& inv, const py::bytes& code) {
    return shim::guard([&] {
        ByteFields out;
        if (!inv.has_mark) return out;
        const shim::OldMarking model(inv);
        const cb::Bytes raw = shim::from_py(code);
        const auto found = shim::nc::byte_fields(&model, raw);
        if (!found) return out;
        for (const auto& s : found->sites) out.fields.add(std::make_tuple(shim::to_py(s.pointer), s.offset));
        for (const std::string& g : found->globals) out.globals.add(shim::to_py(g));
        shim::clear_set(FRAMEW);
        for (const auto& slot : found->frame_slots) FRAMEW.add(std::make_tuple(slot.offset, py::str(shim::slot_name(slot.width))));
        shim::clear_set(text::RANGES);
        for (const auto& r : found->ranges) text::RANGES.add(std::make_tuple(r.lo, r.hi, r.above));
        return out;
    });
}

inline py::str frame_chars(py::str text) {
    return shim::to_py(shim::with_state([&](shim::nc::State& s) { return shim::nc::frame_chars(text.utf8(), s); }));
}
inline std::optional<py::str> buffers(py::str text) { return shim::opt_text(shim::guard([&] { return shim::nc::buffers(text.utf8()); })); }
inline py::str long_returns(const py::str& text) { return shim::to_py(shim::guard([&] { return shim::nc::long_returns(text.utf8()); })); }
inline py::str long_pairs(const py::str& text) {
    return shim::to_py(shim::with_state([&](shim::nc::State& s) { return shim::nc::long_pairs(text.utf8(), s); }));
}
inline py::str long_ladders(const py::str& text) { return shim::to_py(shim::guard([&] { return shim::nc::long_ladders(text.utf8()); })); }
inline py::str local_pointers(py::str text) { return shim::to_py(shim::guard([&] { return shim::nc::local_pointers(text.utf8()); })); }
inline std::optional<py::str> void_variant(const py::str& text) { return shim::opt_text(shim::guard([&] { return shim::nc::void_variant(text.utf8()); })); }
inline py::str register_variables(const py::str& text, const py::list<py::str>& regs, const py::dict<py::str, int64_t>& regparam) {
    return shim::to_py(shim::guard([&] {
        shim::nc::RegisterParams params;
        for (const py::str& k : regparam.keys()) params.insert_or_assign(k.utf8(), static_cast<int>(regparam.at(k)));
        return shim::nc::register_variables(text.utf8(), shim::from_py(regs), params);
    }));
}
inline std::optional<py::str> register_slots(const py::str& text) {
    return shim::opt_text(shim::guard([&] { return shim::nc::register_slots(text.utf8(), forms::REGSLOTS[0]); }));
}
inline py::list<py::str> unsigned_locals(const py::str& text, int64_t most = 3) {
    return shim::to_py(shim::guard([&] { return shim::nc::unsigned_locals(text.utf8(), static_cast<std::size_t>(most)); }));
}
inline std::optional<py::str> registers_last(const py::str& text) {
    return shim::opt_text(shim::guard([&] { return shim::nc::registers_last(text.utf8(), forms::REGSLOTS[0]); }));
}
inline py::list<py::str> string_variants(const forms::Prj& prj, const py::str& text) {
    return shim::to_py(shim::guard([&] {
        shim::SettingsHolder holder(prj);
        return shim::nc::string_variants(holder.settings, text.utf8());
    }));
}

// the model's text with the engine's names and the declarations it needs -> (source, registers left)
inline std::tuple<py::str, py::list<py::str>> localize(const forms::Prj& prj, py::str text, int64_t at, const py::dict<int64_t, py::str>& names) {
    shim::SettingsHolder holder(prj);
    auto caches = forms::shim::caches_from_old(prj);
    const auto made = shim::with_state([&](shim::nc::State& s) {
        return shim::nc::localize(holder.settings, text.utf8(), cb::compat::int_from_py<std::uint32_t>(at), shim::names_of(names), s, caches);
    });
    forms::shim::caches_to_old(caches);
    return {shim::to_py(made.source), shim::to_py(made.registers_left)};
}

inline py::str long_helpers(const py::str& text, const py::dict<py::str, py::str>& helpers) {
    return shim::to_py(shim::guard([&] {
        std::map<std::string, std::string> h;
        for (const py::str& k : helpers.keys()) h[k.utf8()] = helpers.at(k).utf8();
        return shim::nc::long_helpers(text.utf8(), h);
    }));
}
inline py::str fp_conversions(py::str text, const py::list<py::str>& routines) {
    return shim::to_py(shim::with_state([&](shim::nc::State& s) { return shim::nc::fp_conversions(text.utf8(), shim::from_py(routines), s); }));
}
inline py::str fp_types(const py::str& text) {
    return shim::to_py(shim::with_state([&](shim::nc::State& s) { return shim::nc::fp_types(text.utf8(), s.fp); }));
}
inline py::str fill_frame(const py::str& text, int64_t fs, bool regs_count = true) {
    const bool by_place = FILLNAME[0].utf8().find('%') != std::string::npos;
    return shim::to_py(shim::guard([&] {
        return shim::nc::fill_frame(text.utf8(), cb::compat::int_from_py<std::uint32_t>(fs), regs_count,
                                    by_place ? shim::nc::FillerName::ByPlace : shim::nc::FillerName::Fixed);
    }));
}

}  // namespace codebreaker::exact::draft::candidates
