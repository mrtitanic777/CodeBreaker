// codebreaker/ui/terminal.cpp -- C++ from codebreaker.ui.terminal (see terminal.hpp).

#include "codebreaker/ui/terminal.hpp"

#include <algorithm>
#include <cmath>
#include <conio.h>
#include <io.h>
#include <iostream>

#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"

namespace codebreaker::ui::terminal {

namespace V = codebreaker::verify::verdicts;

// ---------------------------------------------------------------------------------------------- look
namespace {

py::str S(const char* s) { return py::str(s); }

// round(x): Python's, to the nearest even on a tie
int64_t py_round(double x) { return static_cast<int64_t>(std::nearbyint(x)); }

// rich's strip_control_codes: the bell, backspace, vertical tab, form feed and carriage return are dropped from a Text
py::str strip_control_codes(const py::str& s) {
    std::u32string out;
    for (char32_t c : s.u32()) {
        if (c != 7 && c != 8 && c != 11 && c != 12 && c != 13) out.push_back(c);
    }
    return py::str(out);
}

// "{:,}".format(n)
py::str with_commas(int64_t n) {
    std::string digits = std::to_string(n < 0 ? -n : n), out;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) out.push_back(',');
        out.push_back(digits[i]);
    }
    return py::str((n < 0 ? "-" : "") + out);
}

// ---- JSON values as the Python's dicts: truthiness, str(), repr(), get
const Json* get(const Json& o, const char* key) { return o.is_obj() ? o.find(key) : nullptr; }

bool truthy(const Json* v) {
    if (!v) return false;
    switch (v->kind) {
        case Json::Null: return false;
        case Json::Bool: return v->b;
        case Json::Int: return v->i != 0;
        case Json::Float: return v->d != 0.0;
        case Json::Str: return !v->s.empty();
        default: return v->size() != 0;
    }
}

py::str jrepr(const Json& v);

py::str jstr(const Json& v) {
    switch (v.kind) {
        case Json::Null: return S("None");
        case Json::Bool: return S(v.b ? "True" : "False");
        case Json::Int: return py::str(std::to_string(v.i));
        case Json::Float: return py::repr(v.d);
        case Json::Str: return py::str(v.s);
        default: return jrepr(v);
    }
}

py::str jrepr(const Json& v) {
    switch (v.kind) {
        case Json::Str: return py::repr(py::str(v.s));
        case Json::Arr: {
            py::list<py::str> parts;
            for (const Json& x : v.a) parts.append(jrepr(x));
            return S("[") + py::join(S(", "), parts) + S("]");
        }
        case Json::Obj: {
            py::list<py::str> parts;
            for (size_t i = 0; i < v.keys.size(); ++i) parts.append(py::repr(py::str(v.keys[i])) + S(": ") + jrepr(v.a[i]));
            return S("{") + py::join(S(", "), parts) + S("}");
        }
        default: return jstr(v);
    }
}

// d.get(key) as a str ("" when it is not there or not a str)
py::str gstr(const Json& o, const char* key, const char* dflt = "") {
    const Json* v = get(o, key);
    return (v && v->is_str()) ? v->str() : S(dflt);
}

// an int of a JSON count (None is 0)
int64_t jint(const Json* v) {
    if (!v) return 0;
    if (v->kind == Json::Int) return v->i;
    if (v->kind == Json::Bool) return v->b ? 1 : 0;
    if (v->kind == Json::Float) return static_cast<int64_t>(v->d);
    return 0;
}

Json object_of_error(const std::string& text) {
    Json j = Json::object();
    j.set("error", Json(text));
    return j;
}

Cell cell_of(const py::str& s) { return s; }

}  // namespace

const py::list<py::str>& VERDICTS() {
    static const py::list<py::str> v = [] {
        py::list<py::str> out;
        for (const py::str& k : V::ORDER()) out.append(k);
        out.append(V::IDENTIFIED);
        return out;
    }();
    return v;
}

const py::dict<py::str, py::str>& COLOR() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        out.set(V::VERIFIED, S("#4ec9a0"));
        out.set(V::WEAK, S("#e5b454"));
        out.set(V::UNVERIFIED, S("#e06c75"));
        out.set(V::NOT_JUDGED, S("#7f8796"));
        out.set(V::NO_COMPILE, S("#c678dd"));
        out.set(V::NO_SOURCE, S("#4b5263"));
        out.set(V::IDENTIFIED, S("#61afef"));
        return out;
    }();
    return d;
}

const py::dict<py::str, py::str>& TITLE() { return V::LABEL(); }

const py::dict<py::str, py::str>& MARK() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        out.set(V::VERIFIED, S("●"));
        out.set(V::WEAK, S("◐"));
        out.set(V::UNVERIFIED, S("✕"));
        out.set(V::NOT_JUDGED, S("○"));
        out.set(V::NO_COMPILE, S("▲"));
        out.set(V::NO_SOURCE, S("·"));
        out.set(V::IDENTIFIED, S("◆"));
        return out;
    }();
    return d;
}

const Theme& THEME() {
    static const Theme t = [] {
        Theme x;
        x.name = S("codebreaker");
        x.primary = S("#61afef");
        x.secondary = S("#c678dd");
        x.accent = S("#4ec9a0");
        x.warning = S("#e5b454");
        x.error = S("#e06c75");
        x.success = S("#4ec9a0");
        x.foreground = S("#d7dae0");
        x.background = S("#16181d");
        x.surface = S("#1d2026");
        x.panel = S("#262a32");
        x.dark = true;
        x.variables.set(S("block-cursor-background"), S("#2c3a4d"));
        x.variables.set(S("block-cursor-foreground"), S("#ffffff"));
        x.variables.set(S("footer-key-foreground"), S("#61afef"));
        x.variables.set(S("input-selection-background"), S("#2c3a4d"));
        return x;
    }();
    return t;
}

const py::str LOGO(
    "[b #61afef]  ___         _     ___              _\n"
    " / __|___  __| |___| _ )_ _ ___ __ _| |_____ _ _\n"
    "| (__/ _ \\/ _` / -_) _ \\ '_/ -_) _` | / / -_) '_|\n"
    " \\___\\___/\\__,_\\___|___/_| \\___\\__,_|_\\_\\___|_|[/]");

py::str logo_plain() { return LOGO.replace(S("[b #61afef]"), S("")).replace(S("[/]"), S("")); }

// ---------------------------------------------------------------------------------------------- Text, Grid
Text::Text(const py::str& plain_, const py::str& style_) : plain(strip_control_codes(plain_)), style(style_) {}

Text& Text::append(const py::str& text, const py::str& style_) {
    if (py::len(text)) {
        py::str clean = strip_control_codes(text);
        int64_t offset = py::len(plain);
        int64_t n = py::len(clean);
        plain = plain + clean;
        if (py::truthy(style_) && n) spans.push_back(Span{offset, offset + n, style_});
    }
    return *this;
}

Text& Text::append(const Text& other) {
    if (py::len(other.plain)) {
        int64_t at = py::len(plain);
        if (py::truthy(other.style)) spans.push_back(Span{at, at + py::len(other.plain), other.style});
        plain = plain + other.plain;
        for (const Span& s : other.spans) spans.push_back(Span{s.start + at, s.end + at, s.style});
    }
    return *this;
}

Text Text::assemble(const std::vector<std::pair<py::str, py::str>>& parts) {
    Text t;
    for (const auto& p : parts) t.append(p.first, p.second);
    return t;
}

Grid::Grid(int64_t vertical, int64_t horizontal, bool expand_)
    : pad_top(vertical), pad_right(horizontal), pad_bottom(vertical), pad_left(horizontal), expand(expand_) {}

void Grid::add_column(const py::str& style, const py::str& justify, std::optional<int64_t> width, std::optional<int64_t> ratio) {
    Column c;
    c.style = style;
    c.justify = justify;
    c.width = width;
    c.ratio = ratio;
    columns.push_back(c);
}

void Grid::add_row(const std::vector<Content>& cells) {
    while (columns.size() < cells.size()) columns.push_back(Column());   // (a row wider than the columns makes more)
    std::vector<Content> row = cells;
    while (row.size() < columns.size()) row.push_back(py::str(""));
    rows.push_back(row);
}

Text bar(const py::dict<py::str, int64_t>& counts, int64_t width, const py::list<py::str>& keys) {
    int64_t total = 0;
    for (const py::str& k : keys) total += counts.get(k, 0);
    Text out;
    if (!total) return out.append(py::repeat(py::str("─"), width), S("#3a3f4b"));
    int64_t used = 0;
    int64_t last = py::len(keys) - 1;
    for (int64_t i = 0; i <= last; ++i) {
        const py::str& k = keys[i];
        int64_t n = counts.get(k, 0);
        int64_t cells = i < last ? py_round(static_cast<double>(width * n) / static_cast<double>(total)) : width - used;
        cells = std::max<int64_t>(0, std::min(cells, width - used));
        if (n && !cells && used < width) cells = 1;   // (every verdict present gets at least a sliver)
        out.append(py::repeat(py::str("█"), cells), COLOR().get(k, S("")));
        used += cells;
    }
    if (used < width) out.append(py::repeat(py::str("█"), width - used), S("#3a3f4b"));
    return out;
}

py::str pct(int64_t n, int64_t total) {
    if (!total) return S("–");
    return py::percent(S("%d%%"), py_round(100.0 * static_cast<double>(n) / static_cast<double>(total)));
}

Text legend(const py::dict<py::str, int64_t>& counts, const py::list<py::str>& keys) {
    Text out;
    for (const py::str& k : keys) {
        int64_t n = counts.get(k, 0);
        if (n) out.append(py::percent(S("%s %d %s   "), MARK().get(k, S("")), n, TITLE().get(k, S(""))), COLOR().get(k, S("")));
    }
    return out;
}

py::str flat_address(const py::str& addr) {
    if (addr.contains(S(":"))) {
        auto [seg, sep, off] = addr.partition(S(":"));
        (void)sep;
        try {
            int64_t s = py::int_parse(seg, 10), o = py::int_parse(off, 16);
            char buf[32];
            std::snprintf(buf, sizeof buf, "%llx", static_cast<unsigned long long>(((0x1000 + 8 * (s - 1)) << 16) | o));
            return py::str(buf);
        } catch (const py::ValueError&) {
            return addr;
        }
    }
    return addr;
}

// ---------------------------------------------------------------------------------------------- the store
namespace {

Json call_tool(const char* name, const Json& args) { return service::call(py::str(std::string("mcp.") + name), args); }

}  // namespace

Json Store::overview() { return service::call(S("store.overview")); }

Json Store::latest(const py::str& sha) {
    Json a = Json::object();
    a.set("sha", Json(sha));
    return service::call(S("store.latest"), a);
}

Json Store::functions(const py::str& module, int64_t limit) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("limit", Json(limit));
    return call_tool("functions", a);
}

Json Store::function(const py::str& module, const py::str& addr) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("addr", Json(addr));
    return call_tool("function", a);
}

Json Store::xrefs(const py::str& module, const py::str& addr) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("addr", Json(addr));
    return call_tool("xrefs", a);
}

Json Store::disasm(const py::str& module, const py::str& addr) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("addr", Json(addr));
    return call_tool("disasm", a);
}

Json Store::triage(const py::str& module) {
    Json a = Json::object();
    a.set("module", Json(module));
    return call_tool("triage", a);
}

Json Store::jobs() { return call_tool("jobs", Json::object()); }

Json Store::job(const py::str& job_id, std::optional<int64_t> offset, std::optional<int64_t> tail) {
    Json a = Json::object();
    a.set("job_id", Json(job_id));
    if (offset) a.set("offset", Json(*offset));
    if (tail) a.set("tail", Json(*tail));
    return call_tool("job", a);
}

Json Store::stop_job(const py::str& job_id) {
    Json a = Json::object();
    a.set("job_id", Json(job_id));
    return call_tool("stop_job", a);
}

Json Store::run_command(const py::str& command, const py::list<py::str>& args) {
    Json a = Json::object();
    a.set("command", Json(command));
    a.set("args", Json::list_of(args));
    return call_tool("run_command", a);
}

Json Store::decompile_function(const py::str& module, const py::str& addr, int64_t wait) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("addr", Json(addr));
    a.set("wait", Json(wait));
    return call_tool("decompile_function", a);
}

Json Store::name_function(const py::str& module, const py::str& addr, const py::str& name) {
    Json a = Json::object();
    a.set("module", Json(module));
    a.set("addr", Json(addr));
    a.set("name", Json(name));
    return call_tool("name_function", a);
}

std::shared_ptr<Store>& STORE() {
    static std::shared_ptr<Store> s = std::make_shared<Store>();
    return s;
}

bool ready(const Json& answer) {
    return !(answer.is_obj() && answer.find("status") && answer.find("status")->is_str() && answer.find("status")->s == "computing");
}

// ---------------------------------------------------------------------------------------------- widgets
void Table::clear() {
    rows.clear();
    cursor = 0;
}

void Table::add_row(const std::vector<Cell>& cells, const py::str& key) {
    for (const Row& r : rows) {
        if (r.key == key) throw py::ValueError("The row key " + py::repr(key).utf8() + " already exists.");   // (textual's DuplicateKey)
    }
    rows.push_back(Row{key, cells});
}

namespace {

// Textual's Input: what a key does to the text (`selected`: all of it is selected, as when the Input got the focus); false when the key is
// not the Input's
bool edit_input(py::str& value, bool& selected, const py::str& key) {
    static const std::vector<std::pair<const char*, const char*>> names = {{"slash", "/"}, {"question_mark", "?"}, {"full_stop", "."}, {"underscore", "_"},
                                                                          {"space", " "}, {"minus", "-"}, {"colon", ":"}, {"comma", ","}, {"backslash", "\\"}};
    py::str ch = key;
    for (const auto& n : names)
        if (key == py::str(n.first)) ch = py::str(n.second);
    if (py::len(ch) == 1) {
        value = selected ? ch : value + ch;       // (Textual's Input selects all of its text when it gets the focus)
        selected = false;
        return true;
    }
    if (key == S("backspace")) {
        if (selected) value = S("");
        else if (py::len(value)) value = value.slice(0, py::len(value) - 1);
        selected = false;
        return true;
    }
    return false;
}

// Textual's DataTable (row cursor): up and down move the cursor and highlight the row (home and end scroll: the cursor stays); true when
// the key is the table's
bool table_key(Table& t, const py::str& key, const std::function<void(const py::str&)>& highlighted, const std::function<void(const py::str&)>& selected) {
    auto move = [&](int64_t to) {
        if (t.row_count() && to != t.cursor && to >= 0 && to < t.row_count()) {
            t.cursor = to;
            if (highlighted) highlighted(t.rows[static_cast<size_t>(to)].key);
        }
    };
    if (key == S("down")) {
        move(t.cursor + 1);
        return true;
    }
    if (key == S("up")) {
        move(t.cursor - 1);
        return true;
    }
    if (key == S("enter")) {
        if (t.row_count() && selected) selected(t.rows[static_cast<size_t>(t.cursor)].key);
        return true;
    }
    return false;
}

// the cursor moved to row 0 of a refilled table: the row is highlighted (Textual posts the event once the rows are in)
void highlight_first(Table& t, const std::function<void(const py::str&)>& highlighted) {
    if (t.row_count()) {
        t.cursor = 0;
        highlighted(t.rows[0].key);
    }
}

Cell text_cell(const py::str& text, const py::str& style) { return Text(text, style); }

}  // namespace

// ---------------------------------------------------------------------------------------------- Home
void Home::on_mount() {
    modules.columns = {S("module"), S("kind"), S("functions"), S("verified"), S("verdicts")};
    logo = Markup{LOGO};
    totals = py::str("");
    sweep = py::str("");
    focused = S("modules");           // (the keys go to the list, not the search)
    totals = Text(S("reading the store ..."), S("#7f8796"));
    load();
}

void Home::load(bool refresh) {
    (void)refresh;
    Json mods;
    try {
        mods = STORE()->overview();
    } catch (const py::Exception& e) {
        totals = Text(py::str(e.what()), S("#e06c75"));
        return;
    }
    rows.clear();
    for (const Json& m : mods.a) {
        ModRow r;
        const Json* latest = get(m, "latest");
        if (latest && latest->is_obj()) {
            for (size_t i = 0; i < latest->keys.size(); ++i) r.counts.set(py::str(latest->keys[i]), jint(&latest->a[i]));
        }
        r.name = gstr(m, "name");
        r.sha = gstr(m, "sha");
        r.kind = gstr(m, "kind");
        r.path = gstr(m, "path");
        r.functions = jint(get(m, "functions"));
        int64_t identified = jint(get(m, "identified"));
        r.counts.set(S("functions"), r.functions);
        r.counts.set(S("identified"), identified);
        r.counts.set(S("no-source"), std::max<int64_t>(0, r.counts.get(S("no-source"), 0) - identified));   // (a library function: not ours to judge)
        rows.push_back(r);
    }
    loaded();
    sweep_now();      // (on the interface thread)
}

void Home::loaded() {
    fill();
    py::dict<py::str, int64_t> totals_;
    py::list<py::str> keys = VERDICTS().copy();     // VERDICTS + ("functions",)
    keys.append(S("functions"));
    for (const py::str& k : keys) {
        int64_t sum = 0;
        for (const ModRow& r : rows) sum += r.counts.get(k, 0);
        totals_.set(k, sum);
    }
    auto g = std::make_shared<Grid>(0, 1);
    g->add_column(S(""), S("right"));
    g->add_column();
    g->add_row({Text(py::percent(S("%d"), static_cast<int64_t>(rows.size())), S("bold #ffffff")), Text(S("modules in the store"), S("#9da5b4"))});
    g->add_row({Text(with_commas(totals_.get(S("functions"), 0)), S("bold #ffffff")), Text(S("functions"), S("#9da5b4"))});
    g->add_row({py::str(""), py::str("")});
    for (const py::str& k : VERDICTS()) {
        if (totals_.get(k, 0)) {
            g->add_row({Text(with_commas(totals_.get(k, 0)), COLOR().get(k, S(""))),
                        Text(py::percent(S("%s %s"), MARK().get(k, S("")), TITLE().get(k, S(""))), COLOR().get(k, S("")))});
        }
    }
    g->add_row({py::str(""), py::str("")});
    g->add_row({py::str(""), bar(totals_, 28)});
    g->add_row({py::str(""), Text(S("\neach function by the last verdict\nit had, under any judge"), S("#7f8796"))});
    totals = g;
}

void Home::sweep_now() {
    Text out;
    Json js = STORE()->jobs();
    const Json* list = get(js, "jobs");
    if (list) {
        for (const Json& j : list->a) {
            if (gstr(j, "status") != S("running")) continue;
            if (out.empty()) out.append(S("\nrunning\n"), S("bold #e5b454"));
            int64_t seconds = jint(get(j, "seconds"));
            const Json* m = get(j, "module");
            out.append(py::percent(S("  ◷ %s %s  %dm %02ds\n"), gstr(j, "action"), m ? jstr(*m) : S("None"), py::floordiv(seconds, 60), py::mod(seconds, 60)),
                       S("#e5b454"));
        }
    }
    sweep = out;
}

void Home::fill() {
    py::str find = find_value.strip().lower();
    modules.clear();
    int64_t n = 0;
    for (const ModRow& r : rows) {
        if (py::truthy(find) && !r.name.lower().contains(find)) continue;
        int64_t total = r.functions;
        modules.add_row({text_cell(py::truthy(r.name) ? r.name : S("?"), S("#ffffff")), text_cell(r.kind, S("#7f8796")),
                         cell_of(py::percent(S("%d"), total)), text_cell(pct(r.counts.get(S("verified"), 0), total), COLOR().get(S("verified"), S(""))),
                         bar(r.counts, 24)},
                        r.sha);
        ++n;
    }
    mod_count = py::percent(S(" %d of %d modules  ·  enter opens one  ·  ctrl+p opens any"), n, static_cast<int64_t>(rows.size()));
}

py::str Home::module_of(const py::str& sha) const {
    for (const ModRow& r : rows) {
        if (r.sha == sha) return (py::truthy(r.path) && py::os::path::exists(r.path)) ? r.path : r.name;
    }
    return sha;
}

void Home::go(const py::str& text_) {
    py::str text = text_.strip().strip(S("\""));
    if (py::truthy(text) && py::os::path::exists(text)) {
        app->push_screen(std::make_shared<Module>(text));
    } else if (modules.row_count() == 1) {
        app->push_screen(std::make_shared<Module>(module_of(modules.rows[0].key)));
    } else {
        focused = S("modules");
    }
}

void Home::action_find() {
    focused = S("find");
    find_selected = true;
}
void Home::action_leave_find() { focused = S("modules"); }

void Home::action_refresh() {
    app->notify(S("working the numbers out again"), S(""), S("information"), 2);
    load(true);
}

void Home::action_open() {
    if (modules.row_count()) app->push_screen(std::make_shared<Module>(module_of(modules.rows[static_cast<size_t>(modules.cursor)].key)));
}

bool Home::key(const py::str& k) {
    if (k == S("q")) {
        app->action_quit();
    } else if (k == S("slash")) {
        action_find();
    } else if (k == S("r")) {
        action_refresh();
    } else if (k == S("enter")) {
        action_open();
    } else if (k == S("escape")) {
        action_leave_find();
    } else {
        return false;
    }
    return true;
}

bool Home::widget_key(const py::str& k) {
    if (focused == S("find")) {
        if (k == S("enter")) {
            go(find_value);
            return true;
        }
        if (edit_input(find_value, find_selected, k)) {
            fill();    // (Input.Changed)
            return true;
        }
        return false;
    }
    if (focused == S("modules")) {
        return table_key(modules, k, nullptr, [this](const py::str& sha) { app->push_screen(std::make_shared<Module>(module_of(sha))); });
    }
    return false;
}

// ---------------------------------------------------------------------------------------------- snapshots
namespace {

py::str repr_list(const std::vector<py::str>& items) {
    py::list<py::str> parts;
    for (const py::str& s : items) parts.append(py::repr(s));
    return S("[") + py::join(S(", "), parts) + S("]");
}

py::str style_of(const py::str& s) { return s; }

py::str desc(const Content& c);

py::str desc_text(const Text& t) {
    py::list<py::str> spans;
    for (const Span& s : t.spans) spans.append(py::percent(S("(%d, %d, %s)"), s.start, s.end, py::repr(s.style)));
    return py::percent(S("T %s base=%s spans=[%s]"), py::repr(t.plain), py::repr(style_of(t.style)), py::join(S(", "), spans));
}

py::str opt_int(const std::optional<int64_t>& v) { return v ? py::str(std::to_string(*v)) : S("None"); }

py::str desc_grid(const Grid& g) {
    py::list<py::str> cols;
    for (const Column& c : g.columns) cols.append(py::percent(S("(%s, %s, %s, %s)"), py::repr(c.style), py::repr(c.justify), opt_int(c.width), opt_int(c.ratio)));
    py::list<py::str> rows;
    for (const auto& row : g.rows) {
        py::list<py::str> cells;
        for (const Content& c : row) cells.append(desc(c));
        rows.append(py::join(S("; "), cells));
    }
    return py::percent(S("G padding=(%d, %d, %d, %d) expand=%s cols=[%s] rows=[%s]"), g.pad_top, g.pad_right, g.pad_bottom, g.pad_left,
                       S(g.expand ? "True" : "False"), py::join(S(", "), cols), py::join(S(" | "), rows));
}

py::str desc_syntax(const Syntax& s) {
    std::vector<std::pair<py::str, py::str>> opts = s.options;
    std::sort(opts.begin(), opts.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    py::list<py::str> parts;
    for (const auto& o : opts) parts.append(o.first + S("=") + o.second);
    return py::percent(S("S %s lexer=%s %s"), py::repr(s.code), s.lexer, py::join(S(" "), parts));
}

py::str desc(const Content& c) {
    if (const py::str* s = std::get_if<py::str>(&c)) return S("str ") + py::repr(*s);
    if (const Text* t = std::get_if<Text>(&c)) return desc_text(*t);
    if (const GridP* g = std::get_if<GridP>(&c)) return desc_grid(**g);
    if (const Syntax* s = std::get_if<Syntax>(&c)) return desc_syntax(*s);
    return S("markup-plain ") + py::repr(logo_plain());
}

}  // namespace

py::str describe(const Content& c) { return desc(c); }

namespace {

py::str desc_cell(const Cell& c) {
    if (const py::str* s = std::get_if<py::str>(&c)) return S("str ") + py::repr(*s);
    return desc_text(std::get<Text>(c));
}

void snap_table(std::vector<py::str>& out, const char* id, const Table& t) {
    out.push_back(py::percent(S("  table #%s cols=%s cursor=%s rows=%d"), S(id), repr_list(t.columns), t.row_count() ? py::str(std::to_string(t.cursor)) : S("None"),
                              t.row_count()));
    int64_t i = 0;
    for (const Row& r : t.rows) {
        py::list<py::str> cells;
        for (const Cell& c : r.cells) cells.append(desc_cell(c));
        out.push_back(py::percent(S("    row %d key=%s [%s]"), i++, py::repr(r.key), py::join(S(" | "), cells)));
    }
}

void snap_static(std::vector<py::str>& out, const char* what, const char* id, const Content& c) {
    out.push_back(py::percent(S("  %s %s %s"), S(what), id ? S((std::string("#") + id).c_str()) : S("-"), desc(c)));
}

void snap_input(std::vector<py::str>& out, const char* id, const py::str& value, const char* placeholder) {
    out.push_back(py::percent(S("  input #%s value=%s placeholder=%s"), S(id), py::repr(value), py::repr(S(placeholder))));
}

}  // namespace

void Home::snapshot(std::vector<py::str>& out) const {
    out.push_back(S("screen Home title='CodeBreaker' sub_title='readable source from any binary'"));
    snap_static(out, "static", "logo", logo);
    snap_static(out, "static", "totals", totals);
    snap_static(out, "static", "sweep", sweep);
    snap_input(out, "find", find_value, "find a module, or type a path and press enter");
    snap_table(out, "modules", modules);
    snap_static(out, "label", "mod-count", mod_count);
}

// ---------------------------------------------------------------------------------------------- Module
Module::Module(const py::str& module_) : module(module_) {
    mod_head = py::str("");
    source = asm_ = calls = verdict = py::str("");
    functions.columns = {S(""), S("address"), S("name"), S("verdict")};
    sub_title = py::os::path::basename(module);
}

void Module::on_mount() {
    focused = S("functions");
    mod_head = Text(py::percent(S("  loading %s ..."), module), S("#7f8796"));
    load();
}

namespace {
const std::vector<std::optional<const char*>>& FILTERS() {
    static const std::vector<std::optional<const char*>> f = {std::nullopt, "verified", "weak", "unverified", "no-compile", "not-judged", "no-source"};
    return f;
}
}  // namespace

void Module::load() {
    Json answer;
    try {
        answer = STORE()->functions(module, 5000);
        const Json* sha = get(answer, "sha");
        if (truthy(sha)) {
            Json earlier = STORE()->latest(sha->str());
            Json* rows_json = answer.find_mut("rows");
            if (rows_json) {
                for (Json& r : rows_json->a) {
                    if (gstr(r, "class") == S("no-source")) {
                        const Json* e = get(earlier, gstr(r, "addr").utf8().c_str());
                        py::str was = (e && e->is_str()) ? e->str() : S("no-source");
                        if (was != S("no-source")) r.set("earlier", Json(was));
                    }
                }
            }
        }
    } catch (const py::Exception& e) {
        answer = object_of_error(e.what());
    }
    got = answer;
    loaded(answer);
}

void Module::loaded(const Json& answer) {
    if (answer.find("error")) {
        mod_head = Text(S("  ") + gstr(answer, "error"), S("#e06c75"));
        return;
    }
    rows.clear();
    const Json* rj = get(answer, "rows");
    if (rj) {
        for (const Json& r : rj->a) {
            FnRow f;
            f.addr = gstr(r, "addr");
            f.name = gstr(r, "name");
            f.cls = gstr(r, "class");
            const Json* e = get(r, "earlier");
            if (truthy(e)) f.earlier = e->str();
            rows.push_back(f);
        }
    }
    py::dict<py::str, int64_t> c;
    const Json* cj = get(answer, "counts");
    if (cj && cj->is_obj()) {
        for (size_t i = 0; i < cj->keys.size(); ++i) c.set(py::str(cj->keys[i]), jint(&cj->a[i]));
    }
    int64_t total = c.get(S("functions"), 0) ? c.get(S("functions"), 0) : static_cast<int64_t>(rows.size());
    std::vector<const FnRow*> earlier;
    for (const FnRow& r : rows)
        if (r.earlier && py::truthy(*r.earlier)) earlier.push_back(&r);
    for (const FnRow* r : earlier) {   // (counted by the verdict they last had)
        c.set(S("no-source"), std::max<int64_t>(0, c.get(S("no-source"), 0) - 1));
        c.set(*r->earlier, c.get(*r->earlier, 0) + 1);
    }
    auto grid = std::make_shared<Grid>(0, 2, true);
    grid->add_column(S(""), S("left"), std::nullopt, 1);
    grid->add_column(S(""), S("right"));
    Text title = Text::assemble({{py::percent(S(" %s"), gstr(answer, "module")), S("bold #ffffff")},
                                 {py::percent(S("   %s"), gstr(answer, "kind_title")), S("#7f8796")},
                                 {py::percent(S("   %d functions"), total), S("#7f8796")}});
    int64_t good = c.get(S("verified"), 0) + c.get(S("weak"), 0);
    Text right(py::percent(S("%s verified or weak"), pct(good, total)), S("bold #4ec9a0"));
    grid->add_row({title, right});
    Text bar_line(S(" "));
    bar_line.append(bar(c, 60));
    grid->add_row({bar_line, Text(S(""))});
    Text legend_line(S(" "));
    legend_line.append(legend(c));
    grid->add_row({legend_line, Text(S(""))});
    if (!earlier.empty()) {
        grid->add_row({Text(py::percent(S(" %d functions have no verdict under the judge in force: shown dim, by the last one they had. m measures the "
                                          "module again."),
                                        static_cast<int64_t>(earlier.size())),
                            S("#e5b454")),
                       Text(S(""))});
    }
    mod_head = grid;
    fill();
}

void Module::fill() {
    py::str find = find_value.strip().lower();
    functions.clear();
    int64_t n = 0;
    for (const FnRow& r : rows) {
        py::str cls = (r.earlier && py::truthy(*r.earlier)) ? *r.earlier : r.cls;
        if (shown_filter && cls != py::str(*shown_filter)) continue;
        if (py::truthy(find) && !r.addr.lower().contains(find) && !r.name.lower().contains(find)) continue;
        py::str dim = r.earlier ? S(" dim") : S("");
        Cell name_cell = py::truthy(r.name) ? Cell(r.name) : Cell(Text(S("sub_") + r.addr.replace(S(":"), S("_")), S("#7f8796")));
        functions.add_row({Text(MARK().get(cls, S("?")), COLOR().get(cls, S("")) + dim), Text(r.addr, S("#9da5b4")), name_cell,
                           Text(TITLE().get(cls, cls), COLOR().get(cls, S("")) + dim)},
                          r.addr);
        ++n;
    }
    py::str what = shown_filter ? TITLE().get(py::str(*shown_filter), S("")) : S("all");
    fn_count = py::percent(S(" %d of %d shown  ·  filter: %s"), n, static_cast<int64_t>(rows.size()), what);
    if (n) highlight_first(functions, [this](const py::str& addr) { picked(addr); });
}

void Module::picked(const py::str& addr) {
    picked_addr = addr;
    for (Content* pane : {&source, &asm_, &calls, &verdict}) *pane = Text(S("working ..."), S("#7f8796"));
    detail(addr);
}

void Module::detail(const py::str& addr) {
    py::str flat = flat_address(addr);
    for (int stage = 0; stage < 3; ++stage) {
        if (!picked_addr || *picked_addr != addr) return;   // (moved on: this one is not shown)
        Json answer;
        try {
            if (stage == 0) answer = STORE()->function(module, addr);
            else if (stage == 1) answer = STORE()->xrefs(module, flat);
            else answer = STORE()->disasm(module, flat);
        } catch (const py::Exception& e) {
            answer = object_of_error(e.what());
        }
        show_detail(addr, S(stage == 0 ? "function" : stage == 1 ? "xrefs" : "asm"), answer);
    }
}

void Module::show_detail(const py::str& addr, const py::str& key, const Json& answer) {
    if (!picked_addr || addr != *picked_addr) return;
    if (key == S("function")) {
        // the source: the store's, or the latest reconstruction (with what earlier judges said of it)
        py::str src = gstr(answer, "source");
        if (!py::truthy(src)) {
            const Json* cands = get(answer, "candidates");
            if (cands) {
                for (const Json& c : cands->a) {
                    if (truthy(get(c, "source"))) {
                        src = gstr(c, "source");
                        break;
                    }
                }
            }
        }
        if (py::truthy(src)) {
            source = Syntax{src, S("c"), {{S("theme"), S("monokai")}, {S("line_numbers"), S("True")}, {S("word_wrap"), S("False")}, {S("background_color"), S("#16181d")}}};
        } else {
            py::str why = gstr(answer, "error");
            source = Text(py::truthy(why) ? why : S("no reconstructed source stored: d decompiles it now"), S("#7f8796"));
        }
        verdict = verdict_view(answer);
    } else if (key == S("asm")) {
        py::str listing = gstr(answer, "listing");
        if (py::truthy(listing)) {
            asm_ = Syntax{listing, S("nasm"), {{S("theme"), S("monokai")}, {S("background_color"), S("#16181d")}}};
        } else {
            const Json* e = get(answer, "error");
            asm_ = Text(e ? jstr(*e) : S("no listing"), S("#7f8796"));
        }
    } else {
        calls = calls_view(answer);
    }
}

Content Module::calls_view(const Json& xr) {
    if (xr.find("error")) return Text(jstr(*xr.find("error")), S("#e06c75"));
    auto g = std::make_shared<Grid>(0, 2);
    g->add_column(S("#7f8796"));
    g->add_column();
    g->add_column(S("#7f8796"));
    g->add_row({Text(S("called from"), S("bold #61afef")), py::str(""), py::str("")});
    const Json* from_code = get(xr, "from_code");
    if (truthy(from_code)) {
        for (const Json& r : from_code->a) {
            const Json* site = get(r, "site");
            const Json* in = get(r, "in");
            const Json* how = get(r, "how");
            g->add_row({site ? Content(jstr(*site)) : Content(py::str("")), Text(in ? jstr(*in) : S(""), S("#d7dae0")), how ? Content(jstr(*how)) : Content(py::str(""))});
        }
    } else {
        g->add_row({py::str(""), Text(S("(nothing in this module calls it)"), S("#d7dae0")), py::str("")});
    }
    const Json* in_data = get(xr, "in_data");
    if (truthy(in_data)) {
        g->add_row({Text(S("\nin data"), S("bold #61afef")), py::str(""), py::str("")});
        for (const Json& r : in_data->a) {
            const Json* site = get(r, "site");
            const Json* in = get(r, "in");
            g->add_row({site ? jstr(*site) : jstr(r), in ? jstr(*in) : S(""), py::str("")});
        }
    }
    g->add_row({Text(S("\ncalls"), S("bold #61afef")), py::str(""), py::str("")});
    const Json* calls = get(xr, "calls");
    if (truthy(calls)) {
        for (const Json& r : calls->a) {
            const Json* site = get(r, "site");
            const Json* to_j = get(r, "to");
            const Json* kind = get(r, "kind");
            py::str to = to_j ? jstr(*to_j) : S("");
            g->add_row({site ? Content(jstr(*site)) : Content(py::str("")), Text(to, to.contains(S("!")) ? S("#e5b454") : S("#d7dae0")),
                        kind ? Content(jstr(*kind)) : Content(py::str(""))});
        }
    } else {
        g->add_row({py::str(""), Text(S("(no calls)"), S("#d7dae0")), py::str("")});
    }
    return g;
}

Content Module::verdict_view(const Json& fn) {
    if (fn.find("error") && fn.size() == 1) return Text(jstr(*fn.find("error")), S("#e06c75"));
    auto g = std::make_shared<Grid>(0, 2, true);
    g->add_column(S("#7f8796"), S("right"), 14);
    g->add_column(S(""), S("left"), std::nullopt, 1);
    py::str cls = gstr(fn, "class");
    // [(candidate, older verdict)]
    std::vector<std::pair<const Json*, const Json*>> earlier;
    const Json* cands = get(fn, "candidates");
    if (cands) {
        for (const Json& c : cands->a) {
            const Json* older = get(c, "older_verdicts");
            if (older)
                for (const Json& v : older->a) earlier.emplace_back(&c, &v);
        }
    }
    if (py::truthy(cls) && cls != S("no-source")) {
        g->add_row({py::str("verdict"), Text(py::percent(S("%s %s"), MARK().get(cls, S("")), TITLE().get(cls, cls)), S("bold ") + COLOR().get(cls, S("")))});
    } else if (!earlier.empty()) {
        const Json& v = *earlier[0].second;
        const Json* limits = get(v, "limits");
        py::str last = V::classify(gstr(v, "verdict"), (limits && limits->is_str()) ? std::optional<py::str>(limits->str()) : std::nullopt).first;
        g->add_row({py::str("last verdict"),
                    Text::assemble({{py::percent(S("%s %s"), MARK().get(last, S("")), TITLE().get(last, last)), S("bold ") + COLOR().get(last, S(""))},
                                    {S("   by an earlier judge"), S("#7f8796")}})});
        if (truthy(get(v, "reason"))) g->add_row({py::str("why"), Text(gstr(v, "reason"))});
    } else {
        g->add_row({py::str("verdict"), Text(py::percent(S("%s %s"), MARK().get(S("no-source"), S("")), TITLE().get(S("no-source"), S(""))), S("#7f8796"))});
    }
    for (const char* k : {"reason", "why", "detail", "limits", "engine", "name"}) {
        const Json* v = get(fn, k);
        if (truthy(v)) g->add_row({py::str(k), Text(jstr(*v))});
    }
    if (cands) {
        size_t shown = 0;
        for (const Json& c : cands->a) {
            if (shown++ >= 3) break;
            g->add_row({py::str(""), py::str("")});
            const Json* engine = get(c, "engine");
            const Json* rev = get(c, "rev");
            g->add_row({py::str("made by"), Text(py::percent(S("%s, revision %s"), engine ? jstr(*engine) : S("None"), rev ? jstr(*rev) : S("None")))});
            const Json* older = get(c, "older_verdicts");
            if (older) {
                size_t n = 0;
                for (const Json& v : older->a) {
                    if (n++ >= 4) break;
                    const Json* verdict_word = get(v, "verdict");
                    g->add_row({py::str("earlier judge"),
                                Text::assemble({{verdict_word ? jstr(*verdict_word) : S("None"), S("#9da5b4")}, {S("  ") + gstr(v, "reason"), S("#7f8796")}})});
                }
            }
            if (truthy(get(c, "problem"))) g->add_row({py::str("problem"), Text(gstr(c, "problem"), S("#e06c75"))});
        }
    }
    if (truthy(get(fn, "say"))) {
        g->add_row({py::str(""), py::str("")});
        g->add_row({py::str(""), Text(gstr(fn, "say"), S("#7f8796"))});
    }
    return g;
}

void Module::action_back() {
    if (focused == S("fn-find")) {
        focused = S("functions");
    } else {
        app->pop_screen();
    }
}

void Module::action_find() {
    focused = S("fn-find");
    find_selected = true;
}

void Module::action_filter() {
    const auto& f = FILTERS();
    size_t i = 0;
    for (; i < f.size(); ++i) {
        bool same = (!shown_filter && !f[i]) || (shown_filter && f[i] && *shown_filter == py::str(*f[i]));
        if (same) break;
    }
    const auto& next = f[(i + 1) % f.size()];
    shown_filter = next ? std::optional<py::str>(py::str(*next)) : std::nullopt;
    fill();
}

void Module::action_reload() { load(); }

void Module::action_info() { app->push_screen(std::make_shared<Info>(module)); }

void Module::action_measure() {
    Json j;
    try {
        py::list<py::str> args;
        args.append(module);
        j = STORE()->run_command(S("measure"), args);
    } catch (const py::ValueError& e) {
        app->notify(py::str(e.what()), S(""), S("warning"));
        return;
    }
    measuring = gstr(j, "id");
    app->notify(py::percent(S("measuring %s: job %s (j shows it)"), module, *measuring), S("Started"));
}

void Module::watch_measure() {
    if (!measuring) return;
    Json answer = STORE()->job(*measuring, std::nullopt, 200);
    py::str status = gstr(answer, "status");
    if (status != S("running")) {
        bool ok = status == S("done");
        app->notify(py::percent(S("%s %s"), module, ok ? S("measured") : S("measure failed: j shows why")), S(""), ok ? S("information") : S("error"));
        measuring = std::nullopt;
        load();
    }
}

void Module::action_decompile() {
    if (picked_addr) {
        active_tab = S("tab-source");
        source = Text(S("decompiling ..."), S("#7f8796"));
        decompile(*picked_addr);
    }
}

void Module::decompile(const py::str& addr) {
    Json answer;
    for (int i = 0; i < 8; ++i) {   // (the server says "computing" past its wait)
        try {
            answer = STORE()->decompile_function(module, addr, 30);
        } catch (const py::Exception& e) {
            answer = object_of_error(e.what());
        }
        if (ready(answer)) break;
    }
    // decompiled(addr, got)
    if (!picked_addr || addr != *picked_addr) return;
    if (truthy(get(answer, "text"))) {
        source = Syntax{gstr(answer, "text"), S("c"), {{S("theme"), S("monokai")}, {S("line_numbers"), S("True")}, {S("background_color"), S("#16181d")}}};
        app->notify(S("decompiled now: not built or judged"), S(""), S("information"), 3);
    } else {
        py::str why = gstr(answer, "error");
        if (!py::truthy(why)) why = gstr(answer, "say");
        source = Text(py::truthy(why) ? why : S("nothing came back"), S("#e06c75"));
    }
}

void Module::action_name() {
    if (!picked_addr) return;
    py::str addr = *picked_addr;
    py::str now;
    for (const FnRow& r : rows) {
        if (r.addr == addr) {
            now = r.name;
            break;
        }
    }
    auto ask = std::make_shared<Ask>(py::percent(S("Name the function at %s"), addr), now);
    ask->callback = [this, addr](const std::optional<py::str>& name) {
        if (!name || !py::truthy(*name)) return;
        try {
            STORE()->name_function(module, addr, *name);
        } catch (const py::ValueError& e) {
            app->notify(py::str(e.what()), S(""), S("error"));
            return;
        }
        for (FnRow& r : rows)
            if (r.addr == addr) r.name = *name;
        fill();
        app->notify(py::percent(S("%s is %s now"), addr, *name));
    };
    app->push_screen(ask);
}

bool Module::key(const py::str& k) {
    if (k == S("escape") || k == S("q")) action_back();
    else if (k == S("slash")) action_find();
    else if (k == S("f")) action_filter();
    else if (k == S("m")) action_measure();
    else if (k == S("d")) action_decompile();
    else if (k == S("n")) action_name();
    else if (k == S("i")) action_info();
    else if (k == S("r")) action_reload();
    else return false;
    return true;
}

bool Module::widget_key(const py::str& k) {
    if (focused == S("fn-find")) {
        if (k == S("enter")) {
            focused = S("functions");     // (Input.Submitted)
            return true;
        }
        if (edit_input(find_value, find_selected, k)) {
            fill();    // (Input.Changed)
            return true;
        }
        return false;
    }
    if (focused == S("functions")) return table_key(functions, k, [this](const py::str& addr) { picked(addr); }, nullptr);
    return false;
}

void Module::snapshot(std::vector<py::str>& out) const {
    out.push_back(py::percent(S("screen Module title=%s sub_title=%s"), py::repr(title), py::repr(sub_title)));
    snap_static(out, "static", "mod-head", mod_head);
    snap_input(out, "fn-find", find_value, "find a function: name or address");
    snap_table(out, "functions", functions);
    snap_static(out, "label", "fn-count", fn_count);
    out.push_back(S("  tabs #detail active=") + py::repr(active_tab));
    snap_static(out, "static", "source", source);
    snap_static(out, "static", "asm", asm_);
    snap_static(out, "static", "calls", calls);
    snap_static(out, "static", "verdict", verdict);
}

// ---------------------------------------------------------------------------------------------- Jobs
void Jobs::on_mount() {
    jobs.columns = {S("job"), S("command"), S("module"), S("status"), S("time")};
    pane_title = py::str("[b]Jobs[/]  [dim]started from here[/]");
    log_title = py::str("output");
    focused = S("jobs");
    shown = std::nullopt;
    log_at = 0;
    refresh_jobs();
}

void Jobs::refresh_jobs() {
    Json js = STORE()->jobs();
    const Json* list = get(js, "jobs");
    std::vector<const Json*> rows;
    if (list)
        for (const Json& j : list->a) rows.push_back(&j);
    int64_t keep = jobs.cursor;
    jobs.clear();
    for (const Json* j : rows) {
        py::str status = gstr(*j, "status");
        py::str style = status == S("running") ? S("#e5b454") : status == S("done") ? S("#4ec9a0") : S("#e06c75");
        if (status != S("running") && status != S("done") && status != S("failed")) throw py::KeyError(py::repr(status).utf8());
        int64_t seconds = jint(get(*j, "seconds"));
        const Json* m = get(*j, "module");
        jobs.add_row({cell_of(gstr(*j, "id")), cell_of(gstr(*j, "action")), cell_of(truthy(m) ? jstr(*m) : S("")),
                      text_cell((status == S("running") ? S("◷ ") : S("")) + status, style),
                      cell_of(py::percent(S("%dm %02ds"), py::floordiv(seconds, 60), py::mod(seconds, 60)))},
                     gstr(*j, "id"));
    }
    if (!rows.empty()) {
        int64_t n = static_cast<int64_t>(rows.size());
        jobs.cursor = std::min(keep, n - 1);
        follow(gstr(*rows[static_cast<size_t>(std::min(std::max<int64_t>(keep, 0), n - 1))], "id"));
    } else {
        log_title = py::str("[dim]nothing started yet: m on a module measures it[/]");
    }
}

void Jobs::follow(const py::str& jid) {
    if (!shown || jid != *shown) {
        shown = jid;
        log_at = 0;
        log.clear();
    }
    Json got = STORE()->job(jid, log_at);
    py::str text = gstr(got, "log");
    if (py::truthy(text))
        for (const py::str& line : py::splitlines(text)) log.push_back(line);
    const Json* next = get(got, "next");
    if (next) log_at = jint(next);
    const Json* m = get(got, "module");
    log_title = py::percent(S("[b]%s %s[/]  [dim]%s[/]"), gstr(got, "action"), truthy(m) ? jstr(*m) : S(""), gstr(got, "status"));
}

void Jobs::picked(const py::str& jid) {
    if (!shown || jid != *shown) follow(jid);
}

void Jobs::action_stop() {
    if (shown) {
        STORE()->stop_job(*shown);
        app->notify(py::percent(S("stopped %s"), *shown));
    }
}

bool Jobs::key(const py::str& k) {
    if (k == S("escape") || k == S("q") || k == S("j")) app->pop_screen();
    else if (k == S("s")) action_stop();
    else return false;
    return true;
}

bool Jobs::widget_key(const py::str& k) {
    if (focused == S("jobs")) return table_key(jobs, k, [this](const py::str& jid) { picked(jid); }, nullptr);
    return false;
}

void Jobs::snapshot(std::vector<py::str>& out) const {
    out.push_back(S("screen Jobs title=None sub_title=None"));
    snap_static(out, "label", nullptr, pane_title);
    snap_table(out, "jobs", jobs);
    snap_static(out, "label", "jobs-log-title", log_title);
    out.push_back(S("  log #jobs-log lines=") + repr_list(log));
}

// ---------------------------------------------------------------------------------------------- Help, Ask, Info
void Help::on_mount() {
    logo = Markup{LOGO};
    auto keys_grid = std::make_shared<Grid>(0, 3);
    keys_grid->add_column(S("bold #61afef"), S("right"));
    keys_grid->add_column();
    const char* const rows[][2] = {{"everywhere", ""},
                                   {"ctrl+p", "command palette: open any module, run anything"},
                                   {"j", "jobs: what is running, its output live"},
                                   {"?", "this help"},
                                   {"q", "back, or quit on the first screen"},
                                   {"", ""},
                                   {"modules", ""},
                                   {"/", "find a module (or type a path and press enter)"},
                                   {"enter", "open the module"},
                                   {"r", "work the numbers out again"},
                                   {"", ""},
                                   {"a module", ""},
                                   {"/", "find a function by name or address"},
                                   {"f", "show only one verdict (again: the next)"},
                                   {"tab", "source, assembly, calls, verdict"},
                                   {"m", "measure: decompile, build and judge the module (a job)"},
                                   {"d", "decompile the function now, as it would be made today"},
                                   {"n", "name the function (every module with the same code gets it)"},
                                   {"i", "what the file is: format, sections, imports, exports"}};
    for (const auto& r : rows) keys_grid->add_row({py::str(r[0]), py::str(r[1])});
    keys = keys_grid;
    closing = py::str("[dim]escape to close[/]");
}

bool Help::key(const py::str& k) {
    if (k == S("escape") || k == S("q") || k == S("question_mark")) {
        app->dismiss(this);
        return true;
    }
    return false;
}

void Help::snapshot(std::vector<py::str>& out) const {
    out.push_back(S("screen Help title=None sub_title=None"));
    snap_static(out, "static", "help-logo", logo);
    snap_static(out, "static", nullptr, keys);
    snap_static(out, "label", nullptr, closing);
}

Ask::Ask(const py::str& question_, const py::str& value_) : question(question_), value(value_) {}

bool Ask::key(const py::str& k) {
    if (k == S("escape")) {
        app->dismiss(this, std::nullopt);
        return true;
    }
    return false;
}

bool Ask::widget_key(const py::str& k) {
    if (k == S("enter")) {
        py::str v = value.strip();
        app->dismiss(this, py::truthy(v) ? std::optional<py::str>(v) : std::nullopt);
        return true;
    }
    return edit_input(value, selected, k);
}

void Ask::snapshot(std::vector<py::str>& out) const {
    out.push_back(S("screen Ask title=None sub_title=None"));
    snap_static(out, "label", nullptr, question);
    snap_input(out, "ask-input", value, "");
    snap_static(out, "label", nullptr, py::str("[dim]enter to save · escape to cancel[/]"));
}

Info::Info(const py::str& module_) : module(module_) { body = py::str(""); }

void Info::on_mount() { load(); }

void Info::load() {
    Json t;
    try {
        t = STORE()->triage(module);
    } catch (const py::Exception& e) {
        t = object_of_error(e.what());
    }
    show(t);
}

void Info::show(const Json& t) {
    loading = false;
    if (t.find("error")) {
        body = Text(jstr(*t.find("error")), S("#e06c75"));
        return;
    }
    auto grid = std::make_shared<Grid>(0, 2);
    grid->add_column(S("#7f8796"), S("right"));
    grid->add_column();
    const Json* is_ = get(t, "is");
    if (truthy(is_) && is_->is_obj())
        for (size_t i = 0; i < is_->keys.size(); ++i) grid->add_row({py::str(is_->keys[i]), jstr(is_->a[i])});
    const Json* summary = get(t, "summary");
    if (truthy(summary) && summary->is_obj()) {
        for (size_t i = 0; i < summary->keys.size(); ++i) {
            const Json& v = summary->a[i];
            if ((v.kind == Json::Str && v.s.empty()) || v.kind == Json::Null) continue;
            grid->add_row({py::str(summary->keys[i]).replace(S("_"), S(" ")), jstr(v)});
        }
    }
    const Json* exports = get(t, "exports");
    const Json* imports = get(t, "imports");
    auto b = std::make_shared<Grid>();
    b->add_row({grid});
    if (truthy(exports)) {
        b->add_row({Text(py::percent(S("\nexports (%d)"), static_cast<int64_t>(exports->a.size())), S("bold #61afef"))});
        py::list<py::str> items;
        for (size_t i = 0; i < exports->a.size() && i < 400; ++i) items.append(jstr(exports->a[i]));
        b->add_row({Text(py::join(S("  "), items))});
    }
    if (truthy(imports) && imports->is_obj()) {
        b->add_row({Text(S("\nimports"), S("bold #61afef"))});
        for (size_t i = 0; i < imports->keys.size(); ++i) {
            py::list<py::str> names;
            for (const Json& n : imports->a[i].a) names.append(jstr(n));
            b->add_row({Text::assemble({{py::str(imports->keys[i]) + S("  "), S("bold")}, {py::join(S("  "), names), S("")}})});
        }
    }
    body = b;
}

bool Info::key(const py::str& k) {
    if (k == S("escape") || k == S("q") || k == S("i")) {
        app->dismiss(this);
        return true;
    }
    return false;
}

void Info::snapshot(std::vector<py::str>& out) const {
    out.push_back(S("screen Info title=None sub_title=None"));
    snap_static(out, "label", "info-title", py::percent(S("[b]%s[/]"), py::os::path::basename(module)));
    if (loading) out.push_back(S("  loading"));
    snap_static(out, "static", "info-body", body);
}

// ---------------------------------------------------------------------------------------------- the app
App::App(const std::optional<py::str>& module) : first(module) {}

void App::on_mount() {
    push_screen(std::make_shared<Home>());
    if (first) push_screen(std::make_shared<Module>(*first));
}

void App::notify(const py::str& message, const py::str& title, const py::str& severity, std::optional<int64_t> timeout) {
    notifications.push_back(Notification{message, title, severity, timeout});
}

void App::push_screen(const std::shared_ptr<Screen>& screen) {
    screen->app = this;
    stack.push_back(screen);
    screen->on_mount();
}

void App::pop_screen() {
    if (stack.size() > 1) stack.pop_back();
}

void App::dismiss(Screen* screen, const std::optional<py::str>& result) {
    std::function<void(const std::optional<py::str>&)> callback;
    if (screen->kind() == S("Ask")) callback = static_cast<Ask*>(screen)->callback;
    for (size_t i = 0; i < stack.size(); ++i) {
        if (stack[i].get() == screen) {
            stack.erase(stack.begin() + static_cast<std::ptrdiff_t>(i));
            break;
        }
    }
    if (callback) callback(result);
}

void App::action_jobs() {
    if (!screen() || screen()->kind() != S("Jobs")) push_screen(std::make_shared<Jobs>());
}

void App::action_help() { push_screen(std::make_shared<Help>()); }

void App::action_quit() {
    py::list<py::str> running;
    Json js = STORE()->jobs();
    const Json* list = get(js, "jobs");
    if (list) {
        for (const Json& j : list->a) {
            if (gstr(j, "status") == S("running") && gstr(j, "owner") == service::OWNER()) {
                const Json* m = get(j, "module");
                running.append(py::percent(S("%s %s"), gstr(j, "action"), m ? jstr(*m) : S("None")));
            }
        }
    }
    if (py::truthy(running) && !warned) {
        warned = true;
        notify(py::percent(S("%s still running: q again stops it and quits"), py::join(S(", "), running)), S(""), S("warning"), 6);
        return;
    }
    service::stop_mine();    // (only the jobs started from here)
    exited = true;
}

void App::press(const py::str& key) {
    Screen* s = screen();
    if (!s || exited) return;
    if (s->widget_key(key)) return;
    if (s->key(key)) return;
    if (key == S("ctrl+c")) action_quit();                       // (a priority binding: it reaches past a modal screen)
    else if (s->modal()) return;
    else if (key == S("j")) action_jobs();
    else if (key == S("question_mark")) action_help();
}

void App::type(const py::str& text) {
    for (char32_t c : text.u32()) press(py::str(std::u32string(1, c)));
}

std::vector<py::str> App::snapshot() const {
    std::vector<py::str> out;
    for (const auto& s : stack) s->snapshot(out);
    Screen* top = screen();
    out.push_back(S("focus ") + ((top && py::truthy(top->focused)) ? S("#") + top->focused : S("None")));
    return out;
}

// ---------------------------------------------------------------------------------------------- main
namespace {

// Textual's name for a key read from the console
py::str key_name(int c, int next) {
    if (c == 0 || c == 0xE0) {
        switch (next) {
            case 72: return S("up");
            case 80: return S("down");
            case 71: return S("home");
            case 79: return S("end");
            default: return S("");
        }
    }
    switch (c) {
        case 13: return S("enter");
        case 27: return S("escape");
        case 8: return S("backspace");
        case 3: return S("ctrl+c");
        case '/': return S("slash");
        case '?': return S("question_mark");
        default: break;
    }
    if (c >= 32 && c < 127) return py::str(std::string(1, static_cast<char>(c)));
    return S("");
}

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    // (the Python checks that Textual is installed; this front end has nothing to import)
    service::START = true;     // the service, started on demand (the Python starts it on a thread: service.client)
    App app(py::truthy(argv) ? std::optional<py::str>(argv[0]) : std::nullopt);
    app.on_mount();
    while (!app.exited) {
        std::vector<py::str> lines = app.snapshot();
        std::string frame = "\x1b[2J\x1b[H";
        size_t shown = 0;
        for (const py::str& l : lines) {
            if (shown++ >= 40) break;
            frame += l.slice(0, 160).utf8() + "\n";
        }
        for (const auto& n : app.notifications) frame += "! " + n.message.utf8() + "\n";
        app.notifications.clear();
        std::fwrite(frame.data(), 1, frame.size(), stdout);
        std::fflush(stdout);
        int c = _getch(), next = 0;
        if (c == 0 || c == 0xE0) next = _getch();
        py::str name = key_name(c, next);
        if (py::truthy(name)) app.press(name);
    }
    return 0;
}

}  // namespace codebreaker::ui::terminal
