// codebreaker/ui/terminal.hpp -- typed C++ port of codebreaker.ui.terminal.
//
// terminal.py -- CodeBreaker in the command prompt: a full-screen text interface, no window of its own.
//
//     python -m codebreaker            (with no command, in a terminal)
//     python -m codebreaker ui
//
// The corpus at a glance (every module, its verdicts as a bar), a module's functions with the
// reconstructed C, the assembly, who calls what and the judge's verdict beside each, and the long
// work (measure, decompile) started as jobs and followed live. It shows what the function store
// holds through the same calls the MCP server answers (ui/mcp_server), so the two never disagree --
// answered by the CodeBreaker service (ui/serve, started on demand) when it can be, else here.
//
// Notes on the port:
//   * The Python is built on Textual (a Python library); C++ has none. The screens, what each widget holds, the keys and the events
//     that follow them (what a key does to the screen, which store calls it makes, the notifications it raises) are ported as a model:
//     `App` is the application (a stack of screens, the focus, `press(key)`, `type(text)`), `Home`, `Module`, `Jobs`, `Help`, `Ask` and
//     `Info` its screens, each holding its widgets' state (the tables' rows and cursor, the Statics' content, the inputs' text, the
//     log's lines). A Textual worker thread is a plain call here (the screen is updated when the answer is back: what a settled
//     Textual screen holds). `snapshot()` says what a screen holds, line by line (the harness compares it with Textual's).
//   * Rich's `Text` / `Table.grid` / `Syntax` -- what the Python hands Textual to draw -- are the records `Text`, `Grid` and `Syntax`
//     (the text with its styled spans; the grid's columns and cells; the code, its lexer and options): they are what is compared.
//   * The timers (the screens' set_interval) are not run by the model: `Home::sweep_now`, `Jobs::refresh_jobs` and `Module::watch_measure`
//     are what a timer calls, and the caller calls them.
//   * `main()` is a console front end over the same model (the screen drawn as text, the keys read from the console); it is not
//     Textual's layout, colours or command palette (ctrl+p).
//   * The store (`Store`) is an interface: `STORE()` is the real one (the service's calls, as the Python's Store) unless a harness sets
//     another. The calls take and give ui::service::Json (the answers are mixed dicts, as the MCP server's).
//   * Home.load in the Python called sweep_now from its worker thread (a widget touched off the interface thread: a LookupError of
//     Textual's active app on Python 3.14, where a thread does not inherit the context). It is called on the interface thread now,
//     in the Python (ui/terminal.py) and here.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include "codebreaker/ui/service.hpp"
#include "py.hpp"

namespace codebreaker::ui::terminal {

using service::Json;

// ---------------------------------------------------------------------------------------------- look
// One colour per verdict, the same everywhere: the bars, the tables, the detail panes. The classes
// and how each is said are the one vocabulary's (verify/verdicts: LABEL, the short form).
const py::list<py::str>& VERDICTS();                       // V.ORDER + (V.IDENTIFIED,)
const py::dict<py::str, py::str>& COLOR();
const py::dict<py::str, py::str>& TITLE();                 // V.LABEL
const py::dict<py::str, py::str>& MARK();

// the Textual theme "codebreaker"
struct Theme {
    py::str name, primary, secondary, accent, warning, error, success, foreground, background, surface, panel;
    bool dark = true;
    py::dict<py::str, py::str> variables;
};
const Theme& THEME();

// The logo, as the markup Textual shows it, and the same as plain text.
extern const py::str LOGO;
py::str logo_plain();

// ---- what the screens hand Textual to draw
struct Span {
    int64_t start = 0, end = 0;
    py::str style;
};

// rich.text.Text: the text, its base style, and styled spans of it.
class Text {
  public:
    Text() = default;
    explicit Text(const py::str& plain, const py::str& style = py::str(""));
    py::str plain;
    py::str style;
    std::vector<Span> spans;

    Text& append(const py::str& text, const py::str& style = py::str(""));
    Text& append(const Text& other);
    static Text assemble(const std::vector<std::pair<py::str, py::str>>& parts);
    bool empty() const { return plain.empty(); }
};

struct Grid;
// rich.syntax.Syntax: code, the lexer and the options the screen gives it (in the order given)
struct Syntax {
    py::str code, lexer;
    std::vector<std::pair<py::str, py::str>> options;    // "theme" "monokai", "line_numbers" "True" ...
};
struct Markup {
    py::str markup;          // Text.from_markup(...)
};
using Content = std::variant<py::str, Text, std::shared_ptr<Grid>, Syntax, Markup>;

// rich.table.Table.grid(...): no box, no header; the columns and the cells
struct Column {
    py::str style;
    py::str justify = py::str("left");
    std::optional<int64_t> width, ratio;
};
struct Grid {
    int64_t pad_top = 0, pad_right = 0, pad_bottom = 0, pad_left = 0;
    bool expand = false;
    std::vector<Column> columns;
    std::vector<std::vector<Content>> rows;

    // Table.grid(padding=(vertical, horizontal), expand=...)
    explicit Grid(int64_t vertical = 0, int64_t horizontal = 0, bool expand = false);
    void add_column(const py::str& style = py::str(""), const py::str& justify = py::str("left"), std::optional<int64_t> width = std::nullopt,
                    std::optional<int64_t> ratio = std::nullopt);
    void add_row(const std::vector<Content>& cells);
};
using GridP = std::shared_ptr<Grid>;

// What a Static (or a table cell) holds, in one line: the text and its styled spans, the grid's columns and cells, the code and its lexer.
py::str describe(const Content& c);

// Verdict counts as one bar of coloured blocks, each verdict its share of the width.
Text bar(const py::dict<py::str, int64_t>& counts, int64_t width = 24, const py::list<py::str>& keys = VERDICTS());
py::str pct(int64_t n, int64_t total);
// `● 80 verified  ◐ 29 weak ...` for the verdicts present.
Text legend(const py::dict<py::str, int64_t>& counts, const py::list<py::str>& keys = VERDICTS());
// A 16-bit `seg:off` as the flat address the analyses use (segment n at the made-up selector
// 1000h + 8(n-1), as the description gives it); anything else as it is.
py::str flat_address(const py::str& addr);

// ---------------------------------------------------------------------------------------------- the store
// The calls the screens make, as the MCP server answers them (ui/mcp_server): by the service when it runs
// (ui/service call: the same process and the same answers as every MCP server), else here.
class Store {
  public:
    virtual ~Store() = default;
    virtual Json overview();
    virtual Json latest(const py::str& sha);
    virtual Json functions(const py::str& module, int64_t limit);
    virtual Json function(const py::str& module, const py::str& addr);
    virtual Json xrefs(const py::str& module, const py::str& addr);
    virtual Json disasm(const py::str& module, const py::str& addr);
    virtual Json triage(const py::str& module);
    virtual Json jobs();
    virtual Json job(const py::str& job_id, std::optional<int64_t> offset = std::nullopt, std::optional<int64_t> tail = std::nullopt);
    virtual Json stop_job(const py::str& job_id);
    virtual Json run_command(const py::str& command, const py::list<py::str>& args);
    virtual Json decompile_function(const py::str& module, const py::str& addr, int64_t wait);
    virtual Json name_function(const py::str& module, const py::str& addr, const py::str& name);
};
// STORE: the one the screens use
std::shared_ptr<Store>& STORE();

// False while the server is still working a slow answer out in the background.
bool ready(const Json& answer);

// ---------------------------------------------------------------------------------------------- widgets
using Cell = std::variant<py::str, Text>;

struct Row {
    py::str key;
    std::vector<Cell> cells;
};

// textual.widgets.DataTable (row cursor): its columns, rows and the row the cursor is on.
struct Table {
    std::vector<py::str> columns;
    std::vector<Row> rows;
    int64_t cursor = 0;
    int64_t row_count() const { return static_cast<int64_t>(rows.size()); }
    void clear();
    void add_row(const std::vector<Cell>& cells, const py::str& key);
};

// ---------------------------------------------------------------------------------------------- the screens
class App;

class Screen {
  public:
    virtual ~Screen() = default;
    App* app = nullptr;
    py::str focused;          // the id of the widget with the focus ("" for none)
    virtual py::str kind() const = 0;
    // a modal screen: the app's keys (j, ?) are not reached while it shows
    virtual bool modal() const { return false; }
    virtual void on_mount() {}
    // the screen's own keys (its BINDINGS); true when it took the key
    virtual bool key(const py::str& key) = 0;
    // the focused widget's keys: an Input's text, a table's cursor and its "enter"; true when it took the key
    virtual bool widget_key(const py::str& key) = 0;
    // what the screen holds, one line each (see the harness)
    virtual void snapshot(std::vector<py::str>& out) const = 0;
};

// one function row of a module's table
struct FnRow {
    py::str addr, name, cls;          // name: "" for none
    std::optional<py::str> earlier;   // the verdict it last had, where it has none now
};

// the corpus: every module the store holds, its verdicts as a bar; the corpus totals beside
class Home : public Screen {
  public:
    struct ModRow {
        py::dict<py::str, int64_t> counts;     // the verdicts (the library functions apart), "identified", "functions"
        py::str name, sha, kind, path;
        int64_t functions = 0;
    };
    std::vector<ModRow> rows;
    Content logo, totals, sweep;
    Table modules;
    py::str find_value, mod_count;
    bool find_selected = false;      // the search's text is selected (it got the focus)

    py::str kind() const override { return py::str("Home"); }
    void on_mount() override;
    // read the store: the rows, the totals, the jobs running
    void load(bool refresh = false);
    void loaded();
    // The jobs running, under the totals. (Not the server's sweep progress: reading it works out the whole corpus's ledger, minutes of
    // work that would starve the screen.)   [what the screen's timer calls]
    void sweep_now();
    void fill();
    // What to open for a row: its file where it is still there, else its name (two builds can share a name).
    py::str module_of(const py::str& sha) const;
    void go(const py::str& text);      // the search's enter
    void action_find();
    void action_leave_find();
    void action_refresh();
    void action_open();
    bool key(const py::str& key) override;
    bool widget_key(const py::str& key) override;
    void snapshot(std::vector<py::str>& out) const override;
};

// one module: its functions, and for the one picked its C, assembly, calls and verdict
class Module : public Screen {
  public:
    explicit Module(const py::str& module);
    py::str module;
    std::vector<FnRow> rows;
    std::optional<py::str> shown_filter;
    std::optional<py::str> picked_addr;
    std::optional<py::str> measuring;
    Json got;
    Content mod_head, source, asm_, calls, verdict;
    Table functions;
    py::str find_value, fn_count, active_tab = py::str("tab-source");
    bool find_selected = false;
    py::str title = py::str("CodeBreaker"), sub_title;

    py::str kind() const override { return py::str("Module"); }
    void on_mount() override;
    void load();
    void loaded(const Json& got);
    void fill();
    // a row of the table highlighted
    void picked(const py::str& addr);
    void detail(const py::str& addr);
    void show_detail(const py::str& addr, const py::str& key, const Json& got);
    static Content calls_view(const Json& xr);
    // The verdict in force, or the last one the function had, and what was said of it.
    static Content verdict_view(const Json& fn);
    void action_back();
    void action_find();
    void action_filter();
    void action_reload();
    void action_info();
    void action_measure();
    void watch_measure();              // [what the screen's timer calls]
    void action_decompile();
    void decompile(const py::str& addr);
    void action_name();
    bool key(const py::str& key) override;
    bool widget_key(const py::str& key) override;
    void snapshot(std::vector<py::str>& out) const override;
};

// the jobs started from here: each a `python -m codebreaker ...` process, its output live
class Jobs : public Screen {
  public:
    Table jobs;
    Content pane_title, log_title;
    std::vector<py::str> log;
    std::optional<py::str> shown;
    int64_t log_at = 0;

    py::str kind() const override { return py::str("Jobs"); }
    void on_mount() override;
    void refresh_jobs();               // [what the screen's timer calls]
    void follow(const py::str& jid);
    void picked(const py::str& jid);
    void action_stop();
    bool key(const py::str& key) override;
    bool widget_key(const py::str& key) override;
    void snapshot(std::vector<py::str>& out) const override;
};

class Help : public Screen {
  public:
    Content logo, keys, closing;
    py::str kind() const override { return py::str("Help"); }
    bool modal() const override { return true; }
    void on_mount() override;
    bool key(const py::str& key) override;
    bool widget_key(const py::str&) override { return false; }
    void snapshot(std::vector<py::str>& out) const override;
};

// One line of text from the user (a name), or None.
class Ask : public Screen {
  public:
    Ask(const py::str& question, const py::str& value = py::str(""));
    py::str question, value;
    bool selected = true;            // its text is selected (it got the focus)
    std::function<void(const std::optional<py::str>&)> callback;
    py::str kind() const override { return py::str("Ask"); }
    bool modal() const override { return true; }
    void on_mount() override { focused = py::str("ask-input"); }
    bool key(const py::str& key) override;
    bool widget_key(const py::str& key) override;
    void snapshot(std::vector<py::str>& out) const override;
};

// What a file is: triage's summary, imports and exports.
class Info : public Screen {
  public:
    explicit Info(const py::str& module);
    py::str module;
    bool loading = true;
    Content body;
    py::str kind() const override { return py::str("Info"); }
    bool modal() const override { return true; }
    void on_mount() override;
    void load();
    void show(const Json& t);
    bool key(const py::str& key) override;
    bool widget_key(const py::str&) override { return false; }
    void snapshot(std::vector<py::str>& out) const override;
};

// ---------------------------------------------------------------------------------------------- the app
class App {
  public:
    explicit App(const std::optional<py::str>& module = std::nullopt);
    struct Notification {
        py::str message, title, severity;
        std::optional<int64_t> timeout;
    };
    std::vector<std::shared_ptr<Screen>> stack;
    std::vector<Notification> notifications;
    bool exited = false;
    bool warned = false;
    std::optional<py::str> first;

    void on_mount();
    void notify(const py::str& message, const py::str& title = py::str(""), const py::str& severity = py::str("information"),
                std::optional<int64_t> timeout = std::nullopt);
    void push_screen(const std::shared_ptr<Screen>& screen);
    void pop_screen();
    // a modal screen's answer (Ask: the text, or None): the screen goes and its callback is called
    void dismiss(Screen* screen, const std::optional<py::str>& result = std::nullopt);
    Screen* screen() const { return stack.empty() ? nullptr : stack.back().get(); }
    template <class T> T* find(const char* kind) const {
        for (auto it = stack.rbegin(); it != stack.rend(); ++it)
            if ((*it)->kind() == py::str(kind)) return static_cast<T*>(it->get());
        return nullptr;
    }
    void action_jobs();
    void action_help();
    // Quit; a job still running is said first (a second q stops it with the interface).
    void action_quit();
    // one key (Textual's names: "j", "question_mark", "escape", "enter", "down", "up", "backspace", "slash", or a character)
    void press(const py::str& key);
    // each character of the text as a key
    void type(const py::str& text);
    // every screen of the stack, then the focus
    std::vector<py::str> snapshot() const;
};

// python -m codebreaker ui [module]; `argv` is sys.argv[1:]. -> the exit status.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::ui::terminal
