// lib_tui.cc -- the part of textual CodeBreaker's terminal interface (codebreaker/ui/terminal.py) uses, in C++
// over the Windows console (virtual-terminal output, console input records). textual itself is not translated:
// what is here is its behaviour for what terminal.py does, written small.
//
//   textual.app          App: run() / run_test() (Pilot: pause, press, click, resize_terminal), push_screen
//                        (a callback for dismiss), pop_screen, exit, screen / screen_stack, notify (toasts),
//                        call_from_thread, register_theme / theme, title / sub_title, CSS_PATH, BINDINGS, COMMANDS
//   textual.screen       Screen, ModalScreen (dismiss(result)), focus and the focus chain (tab / shift+tab)
//   textual.binding      Binding: textual's binding chain (priority bindings, the focused widget and its
//                        ancestors, a modal screen ending it, keys an Input consumes), action strings
//                        ("app.pop_screen", "dismiss(None)")
//   textual.command      Provider / Hit / Hits and the command palette (ctrl+p): providers' search and
//                        discover (async generators), textual's fuzzy Matcher; the system commands provider
//   textual.containers   Horizontal, Vertical, VerticalScroll (Container, ScrollableContainer)
//   textual.widgets      DataTable, Footer, Header, Input, Label, LoadingIndicator, RichLog, Static,
//                        TabbedContent, TabPane (with Tabs, Tab, ContentSwitcher inside, as textual composes it)
//   textual.theme        Theme: the colour system textual generates from it (the variables CSS uses)
//   textual (on, work)   message handlers by type and selector; workers in threads (exclusive, group)
//
// The CSS is textual's: selectors (type, #id, .class, :focus, descendant and child), the widgets' default
// CSS (theirs, flattened) under the app's CSS_PATH, variables from the theme, alpha over the parent's
// background, background-tint, borders (tall, round, outer, vkey ...), padding, margin, dock, the
// vertical / horizontal layout with fr / % / auto sizes, align. NO_COLOR makes it monochrome, as textual.
#include <windows.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <ctime>
#include <cstring>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <unordered_map>

#include "tui.hpp"

namespace py {

TypeObj* T(const var& v);   // object.cc
void print_exception_(const var& e);   // io.cc

namespace tui {

static var natf(const char* n, native_fn f) { return new_native(n, std::move(f)); }
static double now_s() {
    using namespace std::chrono;
    return duration<double>(steady_clock::now().time_since_epoch()).count();
}
static std::string lower(std::string s) {
    for (auto& c : s) c = (char)tolower((unsigned char)c);
    return s;
}
static std::vector<std::string> split_ws(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (isspace((unsigned char)c)) {
            if (!cur.empty()) out.push_back(cur), cur.clear();
        } else cur += c;
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}
static std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}
static void fix_halves(std::vector<Line>& rows);
static bool is_ident_char_(char32_t c) { return c == U'_' || (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z') || (c >= U'0' && c <= U'9'); }

// ------------------------------------------------------------------------------------------ CSS: values
struct Scalar {
    char u = 0;   // 0 unset, 'c' cells, 'f' fr, '%' percent, 'a' auto, 'w' vw, 'h' vh
    double v = 0;
};
struct Edge {
    std::string type;   // "" none
    Color c;
};
struct Styles {
    Scalar width, height, min_w, min_h, max_w, max_h;
    int pad[4] = {0, 0, 0, 0};   // top right bottom left
    int mar[4] = {0, 0, 0, 0};
    Edge border[4];
    Color bg, fg, tint;   // (unset: transparent / inherited / none)
    Style ts;             // text-style
    std::string dock, layout = "vertical", display = "block", visibility = "visible";
    std::string align_h = "left", align_v = "top", calign_h = "left", calign_v = "top";
    std::string overflow_x = "hidden", overflow_y = "hidden";
    int sb_v = 2, sb_h = 1;
};
static int bw(const Styles& s, int side) { return s.border[side].type.empty() || s.border[side].type == "none" || s.border[side].type == "hidden" ? 0 : 1; }

struct Compound {
    std::string type, id;
    std::vector<std::string> classes, pseudo;
};
struct Selector {
    std::vector<Compound> parts;
    std::vector<char> comb;   // between parts[i] and parts[i+1]: ' ' or '>'
    int a = 0, b = 0, c = 0;  // specificity: ids, classes and pseudo classes, types
};
struct Rule {
    std::vector<Selector> sels;
    std::vector<std::pair<std::string, std::string>> decls;
    int origin = 0;   // 0 the widgets' defaults, 1 the app's
    int order = 0;
};
static Selector parse_selector(const std::string& text) {
    Selector s;
    size_t i = 0, n = text.size();
    Compound cur;
    bool have = false;
    char pending = 0;
    auto flush = [&]() {
        if (!have) return;
        if (!s.parts.empty()) s.comb.push_back(pending ? pending : ' ');
        s.parts.push_back(cur);
        cur = Compound();
        have = false;
        pending = 0;
    };
    auto ident = [&]() {
        size_t a = i;
        while (i < n && (isalnum((unsigned char)text[i]) || text[i] == '-' || text[i] == '_')) i++;
        return text.substr(a, i - a);
    };
    while (i < n) {
        char c = text[i];
        if (isspace((unsigned char)c)) { flush(); i++; continue; }
        if (c == '>') { flush(); pending = '>'; i++; continue; }
        if (c == '#') { i++; cur.id = ident(); have = true; continue; }
        if (c == '.') { i++; cur.classes.push_back(ident()); have = true; continue; }
        if (c == ':') { i++; cur.pseudo.push_back(ident()); have = true; continue; }
        if (c == '*') { i++; have = true; continue; }
        if (c == '&') { i++; continue; }
        std::string t = ident();
        if (t.empty()) { i++; continue; }
        cur.type = t;
        have = true;
    }
    flush();
    for (auto& p : s.parts) {
        s.a += p.id.empty() ? 0 : 1;
        s.b += (int)(p.classes.size() + p.pseudo.size());
        s.c += p.type.empty() ? 0 : 1;
    }
    return s;
}
static void parse_css(const std::string& text0, int origin, std::vector<Rule>& rules) {
    std::string text;   // (comments out)
    for (size_t i = 0; i < text0.size(); i++) {
        if (text0[i] == '/' && i + 1 < text0.size() && text0[i + 1] == '*') {
            size_t e = text0.find("*/", i + 2);
            if (e == std::string::npos) break;
            i = e + 1;
            continue;
        }
        text += text0[i];
    }
    size_t i = 0;
    while (i < text.size()) {
        size_t o = text.find('{', i);
        if (o == std::string::npos) break;
        size_t c = text.find('}', o);
        if (c == std::string::npos) break;
        std::string sel = text.substr(i, o - i), body = text.substr(o + 1, c - o - 1);
        i = c + 1;
        Rule r;
        r.origin = origin;
        r.order = (int)rules.size();
        size_t p = 0;
        while (p <= sel.size()) {
            size_t q = sel.find(',', p);
            if (q == std::string::npos) q = sel.size();
            std::string one = trim(sel.substr(p, q - p));
            if (!one.empty()) r.sels.push_back(parse_selector(one));
            p = q + 1;
        }
        size_t d = 0;
        while (d < body.size()) {
            size_t semi = body.find(';', d);
            if (semi == std::string::npos) semi = body.size();
            std::string decl = body.substr(d, semi - d);
            d = semi + 1;
            size_t colon = decl.find(':');
            if (colon == std::string::npos) continue;
            std::string k = lower(trim(decl.substr(0, colon))), v = trim(decl.substr(colon + 1));
            size_t imp = v.find("!important");
            if (imp != std::string::npos) v = trim(v.substr(0, imp));
            if (!k.empty()) r.decls.push_back({k, v});
        }
        if (!r.sels.empty()) rules.push_back(r);
    }
}

// the widgets' default CSS: textual's DEFAULT_CSS of each (flattened), for the dark theme
static const char* DEFAULT_CSS = R"CSS(
Screen { layout: vertical; overflow-y: auto; background: $background; color: $foreground; }
ModalScreen { background: $background 60%; }
Container { width: 1fr; height: 1fr; layout: vertical; }
Vertical { width: 1fr; height: 1fr; layout: vertical; }
Horizontal { width: 1fr; height: 1fr; layout: horizontal; }
ScrollableContainer { width: 1fr; height: 1fr; layout: vertical; overflow-y: auto; overflow-x: auto; }
VerticalScroll { layout: vertical; overflow-x: hidden; overflow-y: auto; }
Static { height: auto; }
Label { width: auto; height: auto; min-height: 1; }
Input { background: $surface; color: $foreground; padding: 0 2; border: tall $border-blurred; width: 100%; height: 3; scrollbar-size-horizontal: 0; }
Input:focus { border: tall $border; background-tint: $foreground 5%; }
Input > .input--cursor { background: $input-cursor-background; color: $input-cursor-foreground; }
Input > .input--selection { background: $input-selection-background; color: $input-selection-foreground; }
Input > .input--placeholder { color: $text-disabled; }
Header { dock: top; width: 100%; background: $panel; color: $foreground; height: 1; }
Footer { layout: horizontal; color: $footer-foreground; background: $footer-background; dock: bottom; height: 1; }
Footer > .footer-key--key { color: $footer-key-foreground; background: $footer-key-background; text-style: bold; }
Footer > .footer-key--description { color: $footer-description-foreground; background: $footer-description-background; }
DataTable { background: $surface; color: $foreground; height: auto; max-height: 100%; }
DataTable:focus { background-tint: $foreground 5%; }
DataTable:focus > .datatable--cursor { background: $block-cursor-background; color: $block-cursor-foreground; text-style: $block-cursor-text-style; }
DataTable:focus > .datatable--header { background-tint: $foreground 5%; }
DataTable:dark > .datatable--even-row { background: $surface-darken-1 40%; }
DataTable > .datatable--header { text-style: bold; background: $panel; color: $foreground; }
DataTable > .datatable--even-row { background: $surface-lighten-1 50%; }
DataTable > .datatable--cursor { background: $block-cursor-blurred-background; color: $block-cursor-blurred-foreground; text-style: $block-cursor-blurred-text-style; }
TabbedContent { height: auto; }
TabPane { height: auto; }
ContentSwitcher { height: auto; }
Tabs { width: 100%; height: 2; }
Tabs:focus > .underline--bar { background: $foreground 30%; }
Tabs > .underline--bar { color: $block-cursor-background; background: $foreground 10%; }
Tab { width: auto; height: 1; padding: 0 1; color: $foreground 50%; }
Tab.-active { color: $foreground; }
Tabs:focus Tab.-active { text-style: $block-cursor-text-style; color: $block-cursor-foreground; background: $block-cursor-background; }
LoadingIndicator { width: 100%; height: 100%; min-height: 1; content-align: center middle; color: $primary; }
RichLog { background: $surface; color: $foreground; overflow-y: scroll; }
RichLog:focus { background-tint: $foreground 5%; }
CommandPalette { layout: vertical; color: $foreground; background: $background 60%; }
CommandPalette > .command-palette--highlight { text-style: bold underline; }
CommandPalette > .command-palette--help-text { text-style: dim; }
CommandPalette > Vertical { margin-top: 3; height: auto; background: $panel-darken-1; border-top: hkey black 50%; }
CommandPalette #--input { height: auto; padding: 0 1; }
CommandPalette #--input Input { width: 1fr; height: 3; padding: 0; border: tall $border-blurred; background: $surface; }
CommandPalette #--input Input:focus { border: tall $border; background-tint: transparent; }
CommandPalette SearchIcon { width: 2; height: 1; margin-top: 1; margin-left: 1; color: black; }
CommandList { height: auto; padding: 1 2 0 2; border-bottom: hkey black; }
)CSS";

// ------------------------------------------------------------------------------------------ the theme
struct ThemeVars {
    std::map<std::string, std::string> v;
};
static std::string hexa(const Color& c) {
    char b[16];
    if (c.a >= 1.0) snprintf(b, sizeof b, "#%02X%02X%02X", c.r, c.g, c.b);
    else snprintf(b, sizeof b, "#%02X%02X%02X%02X", c.r, c.g, c.b, (int)(c.a * 255));
    return b;
}
static Color tint_c(const Color& c, const Color& t) {
    return Color::rgb((int)(c.r + (t.r - c.r) * t.a), (int)(c.g + (t.g - c.g) * t.a), (int)(c.b + (t.b - c.b) * t.a), c.a);
}
static Color with_a(Color c, double a) { c.a = a; return c; }
// textual's ColorSystem._generate for a dark theme: the variables the CSS uses
static ThemeVars generate_theme(const var& theme) {
    ThemeVars t;
    auto col = [&](const char* name, const char* dflt) {
        Color c;
        var v = getattr(theme, new_str(name), None);
        if (is_str(v) && parse_color(str_utf8(v), c)) return c;
        if (dflt) parse_color(dflt, c);
        return c;
    };
    Color primary = col("primary", "#0178D4");
    Color secondary = col("secondary", nullptr);
    if (!secondary.set) secondary = primary;
    Color warning = col("warning", nullptr), error = col("error", nullptr), success = col("success", nullptr), accent = col("accent", nullptr);
    if (!warning.set) warning = primary;
    if (!error.set) error = secondary;
    if (!success.set) success = secondary;
    if (!accent.set) accent = primary;
    Color background = col("background", "#121212"), surface = col("surface", "#1e1e1e");
    Color foreground = col("foreground", nullptr);
    if (!foreground.set) foreground = Color::rgb(255 - background.r, 255 - background.g, 255 - background.b);
    Color boost = Color::rgb(0, 0, 0, 0);
    Color contrast = contrast_text(background, 1.0);
    auto& v = t.v;
    v["text-primary"] = hexa(tint_c(contrast, with_a(primary, 0.66)));
    v["text-secondary"] = hexa(tint_c(contrast, with_a(secondary, 0.66)));
    v["text-warning"] = hexa(tint_c(contrast, with_a(warning, 0.66)));
    v["text-error"] = hexa(tint_c(contrast, with_a(error, 0.66)));
    v["text-success"] = hexa(tint_c(contrast, with_a(success, 0.66)));
    v["text-accent"] = hexa(tint_c(contrast, with_a(accent, 0.66)));
    Color panel = col("panel", nullptr);
    if (!panel.set) {
        Color p = blend(surface, primary, 0.1);
        p.a = 1;
        boost = with_a(contrast, 0.04);
        panel = over(p, boost);
    }
    double spread = 0.15;
    var ls = getattr(theme, "luminosity_spread"_s, None);
    if (!ls.is_none() && !ls.is_unbound()) spread = ls.as_double();
    std::vector<std::pair<std::string, Color>> colors = {
        {"primary", primary}, {"secondary", secondary}, {"primary-background", primary}, {"secondary-background", secondary},
        {"background", background}, {"foreground", foreground}, {"panel", panel}, {"boost", boost}, {"surface", surface},
        {"warning", warning}, {"error", error}, {"success", success}, {"accent", accent}};
    for (auto& [name, c] : colors) {
        for (int n = -3; n <= 3; n++) {
            std::string key = name + (n < 0 ? "-darken-" + std::to_string(-n) : n > 0 ? "-lighten-" + std::to_string(n) : "");
            double delta = n * spread / 2;
            if (name == "primary-background" || name == "secondary-background") {
                Color dark = blend(background, c, 0.15);
                dark.a = 1;
                Color sh = blend(dark, Color::rgb(255, 255, 255), spread + delta);
                sh.a = 1;
                v[key] = hexa(sh);
            } else
                v[key] = n == 0 ? hexa(c) : hexa(lighten(c, delta));
        }
    }
    v["text"] = "auto 87%";
    v["text-muted"] = "auto 60%";
    v["text-disabled"] = "auto 38%";
    for (auto nm : {"primary", "secondary", "accent", "warning", "error", "success"}) {
        Color c = nm == std::string("primary") ? primary : nm == std::string("secondary") ? secondary : nm == std::string("accent") ? accent
                  : nm == std::string("warning") ? warning : nm == std::string("error") ? error : success;
        Color m = blend(c, background, 0.7);
        v[std::string(nm) + "-muted"] = hexa(m);
    }
    v["foreground-muted"] = hexa(with_a(foreground, 0.6));
    v["foreground-disabled"] = hexa(with_a(foreground, 0.38));
    v["block-cursor-foreground"] = v["text"];
    v["block-cursor-background"] = hexa(primary);
    v["block-cursor-text-style"] = "bold";
    v["block-cursor-blurred-foreground"] = hexa(foreground);
    v["block-cursor-blurred-background"] = hexa(with_a(primary, 0.3));
    v["block-cursor-blurred-text-style"] = "none";
    v["block-hover-background"] = hexa(with_a(boost, 0.1));
    v["border"] = hexa(primary);
    v["border-blurred"] = hexa(lighten(surface, -0.025));
    v["surface-active"] = hexa(lighten(surface, spread / 2.5));
    Color bgd1;
    parse_color(v["background-darken-1"], bgd1);
    v["scrollbar"] = hexa(over(bgd1, with_a(primary, 0.4)));
    v["scrollbar-hover"] = hexa(over(bgd1, with_a(primary, 0.5)));
    v["scrollbar-active"] = hexa(primary);
    v["scrollbar-background"] = v["background-darken-1"];
    v["scrollbar-corner-color"] = v["scrollbar-background"];
    v["footer-foreground"] = hexa(foreground);
    v["footer-background"] = hexa(panel);
    v["footer-key-foreground"] = hexa(accent);
    v["footer-key-background"] = "transparent";
    v["footer-description-foreground"] = hexa(foreground);
    v["footer-description-background"] = "transparent";
    v["footer-item-background"] = "transparent";
    v["input-cursor-background"] = hexa(foreground);
    v["input-cursor-foreground"] = hexa(background);
    v["input-cursor-text-style"] = "none";
    Color pl1;
    parse_color(v["primary-lighten-1"], pl1);
    v["input-selection-background"] = hexa(with_a(pl1, 0.4));
    v["input-selection-foreground"] = hexa(foreground);
    // the theme's own variables over the generated ones
    var vars = getattr(theme, "variables"_s, None);
    if (is_dict(vars))
        for (var k : each(vars)) v[str_utf8(k)] = str_(vars.getitem(k));
    return t;
}

// ------------------------------------------------------------------------------------------ the DOM
enum WT {
    W_APP, W_SCREEN, W_WIDGET, W_STATIC, W_INPUT, W_TABLE, W_HEADER, W_FOOTER, W_LOADING, W_LOG, W_TABBED, W_PANE,
    W_TABS, W_TAB, W_SWITCHER, W_CONTAINER, W_SCROLL, W_CMDLIST
};
struct Node;
struct ColumnD {
    var label, key;
    int width = -1;       // explicit, or -1: the content's
    int content_w = 0;
};
struct RowD {
    var key;
    std::vector<var> cells;
};
struct Node {
    Obj* self = nullptr;              // the Python object (its NodeObj holds this; borrowed)
    WT wt = W_WIDGET;
    Node* parent = nullptr;
    std::vector<var> children, pending;
    std::string id;
    std::vector<std::string> classes, type_names;
    bool mounted = false, removed = false, can_focus = false, display = true, is_modal = false;
    Styles st;
    Color bg, fg;                     // composited background, the text colour (resolved at paint)
    int x = 0, y = 0, w = 0, h = 0;   // the region (screen cells)
    int cx = 0, cy = 0, cw = 0, ch = 0;   // the content region
    int scroll_y = 0, virtual_h = 0;
    bool sb_shown = false;
    // Static / Label
    var content;
    uint64_t gen = 1, cache_gen = 0;
    int cache_w = -1;
    std::vector<Line> cache;
    bool markup = true;
    // Input
    std::u32string value, placeholder;
    int cursor = 0, sel = -1, view = 0;
    bool select_on_focus = true;
    // DataTable
    std::vector<ColumnD> cols;
    std::vector<RowD> rows;
    int cur_row = 0, cur_col = 0;
    std::string cursor_type = "cell";
    bool zebra = false, show_header = true, renderable_fg = false, scroll_to_cursor = false;
    // TabPane / Tab / TabbedContent
    std::string title, active;
    // RichLog
    std::vector<std::pair<var, std::vector<Line>>> log;
    int log_w = -1;
    bool wrap = false, highlight = false, auto_scroll = true;
    // LoadingIndicator
    double t0 = 0;
    // screens
    Node* focused = nullptr;
    var dismiss_cb;
    // the command list
    std::vector<var> hits;
    int hl = 0;
    ~Node();
};
static TypeObj* T_nodeobj = nullptr;
struct NodeObj : DataObj { Node n; };
Node::~Node() {
    for (auto& c : children) {
        var h = getattr(c, "_tui_node"_s, None);
        if (h.is_ref() && h.ref()->type == T_nodeobj) static_cast<NodeObj*>(h.ref())->n.parent = nullptr;
    }
}
static Node* node_of(const var& o, bool must = true) {
    if (o.is_ref()) {
        var h = getattr(o, "_tui_node"_s, None);
        if (h.is_ref() && h.ref()->type == T_nodeobj) return &static_cast<NodeObj*>(h.ref())->n;
    }
    if (must) raise(E_RuntimeError, "not a textual node: " + repr_(o));
    return nullptr;
}
static var obj(Node* n) { return n ? var(n->self) : var(None); }

// ------------------------------------------------------------------------------------------ the app's state
struct ThreadCall {
    var fn;
    CallArgs args;
    var result, exc;
    bool done = false;
};
struct TimerRec {
    var owner, cb;
    double interval = 0, next = 0;
    bool repeat = true, stopped = false;
    int64_t repeat_count = 0, fired = 0;
};
struct Toast {
    std::string message, title, severity;
    double until = 0;
    bool markup = true;
};
struct AppState {
    var app;
    Node* node = nullptr;
    std::vector<var> stack;
    std::deque<std::function<void()>> tasks;
    std::mutex mu;
    std::condition_variable cv;
    std::deque<std::shared_ptr<ThreadCall>> calls;
    std::vector<std::shared_ptr<TimerRec>> timers;
    std::vector<var> threads;   // the workers' threading.Thread objects (joined at the end)
    std::vector<Toast> toasts;
    std::vector<Rule> rules;
    ThemeVars vars;
    std::map<std::string, var> themes;
    bool running = false, exiting = false, headless = false, mono = false, toasts_on = true;
    std::atomic<bool> closed{false};
    var return_value;
    int return_code = 0;
    int W = 80, H = 24;
    var error;   // an exception out of a handler or a worker: the app ends, it is raised / shown after
    std::vector<Line> frame, last;
    bool dirty = true, full = true;
    double next_frame = 0;
    // the console
    HANDLE hin = INVALID_HANDLE_VALUE, hout = INVALID_HANDLE_VALUE;
    DWORD in_mode = 0, out_mode = 0;
    UINT out_cp = 0;
    int mouse_x = -1, mouse_y = -1;
};
static AppState* G = nullptr;
static DWORD g_main_thread = 0;
static std::vector<std::vector<var>> g_compose_stacks, g_composed;

static var g_cls[64];   // the native classes, by name below
enum {
    C_DOMNode, C_Widget, C_App, C_Screen, C_ModalScreen, C_Static, C_Label, C_Input, C_DataTable, C_Header, C_Footer,
    C_Loading, C_RichLog, C_Tabbed, C_TabPane, C_Tabs, C_ContentTabs, C_Tab, C_Switcher, C_Container, C_Vertical,
    C_Horizontal, C_Scrollable, C_VScroll, C_Message, C_InputChanged, C_InputSubmitted, C_RowHighlighted,
    C_RowSelected, C_TabActivated, C_Binding, C_Hit, C_DiscoveryHit, C_Provider, C_Matcher, C_Theme, C_Worker,
    C_Timer, C_Pilot, C_RunTest, C_NoMatches, C_WrongType, C_RowKey, C_ColumnKey, C_CellKey, C_Palette,
    C_CmdList, C_SearchIcon, C_SystemCommands, C_Notification, C_DismissResult, C_Underline, C_Query
};

static AppState* app_state() {
    if (!G) raise(E_RuntimeError, "textual: no app is running (App.run or App.run_test first)");
    return G;
}
static void request_redraw() {
    if (G) G->dirty = true;
}
static void fail(const error& e) {   // an exception out of a handler: textual ends the app and shows it
    if (!G) throw e;
    if (G->error.is_unbound() || G->error.is_none()) G->error = e.value;
    G->exiting = true;
}
template <class F> static void guarded(F f) {
    try {
        f();
    } catch (error& e) {
        if (e.matches(tv(E_SystemExit))) {
            if (G) G->exiting = true;
            throw;
        }
        fail(e);
    }
}
static void defer(std::function<void()> f) {
    if (G) G->tasks.push_back(std::move(f));
}

// a coroutine (or any awaitable) run to its end here: what terminal.py awaits this way never waits on anything
static var drive(const var& aw) {
    var it = aw;
    if (!aw.hasattr("send")) it = aw.call("__await__");
    for (int i = 0; i < 1000000; i++) {
        try {
            it.call("send", None);
        } catch (error& e) {
            if (e.matches(tv(E_StopIteration))) return getattr(e.value, "value"_s, None);
            throw;
        }
    }
    raise(E_RuntimeError, "textual: a coroutine awaited something that never finished");
}
static bool is_awaitable(const var& v) {
    if (!v.is_ref()) return false;
    std::string t = type_of(v)->name;
    return t == "coroutine" || v.hasattr("__await__");
}
static var finish(const var& r) { return is_awaitable(r) ? drive(r) : r; }

// ------------------------------------------------------------------------------------------ DOM helpers
static Node* screen_of(Node* n) {
    while (n && n->wt != W_SCREEN) n = n->parent;
    return n;
}
static Node* active_screen() {
    if (!G || G->stack.empty()) return nullptr;
    return node_of(G->stack.back());
}
static bool displayed(Node* n) {
    for (Node* p = n; p; p = p->parent)
        if (!p->display || p->st.display == "none") return false;
    return true;
}
static std::vector<Node*> kids(Node* n) {
    std::vector<Node*> out;
    for (auto& c : n->children) out.push_back(node_of(c));
    return out;
}
static void walk(Node* n, const std::function<void(Node*)>& f) {
    f(n);
    for (auto& c : n->children) walk(node_of(c), f);
}
static bool has_class(Node* n, const std::string& c) { return std::find(n->classes.begin(), n->classes.end(), c) != n->classes.end(); }
static bool is_focused(Node* n) {
    Node* s = screen_of(n);
    return s && s->focused == n;
}
static bool within(Node* n, Node* anc) {
    for (Node* p = n; p; p = p->parent)
        if (p == anc) return true;
    return false;
}

// ------------------------------------------------------------------------------------------ CSS: matching
static bool match_compound(const Compound& c, Node* n, const std::string* comp) {
    if (comp) {
        if (!c.type.empty() || !c.id.empty() || !c.pseudo.empty()) return false;
        for (auto& k : c.classes)
            if (k != *comp) return false;
        return !c.classes.empty();
    }
    if (!c.type.empty() && std::find(n->type_names.begin(), n->type_names.end(), c.type) == n->type_names.end()) return false;
    if (!c.id.empty() && c.id != n->id) return false;
    for (auto& k : c.classes)
        if (!has_class(n, k)) return false;
    for (auto& p : c.pseudo) {
        if (p == "focus") {
            if (!is_focused(n)) return false;
        } else if (p == "focus-within") {
            Node* s = screen_of(n);
            if (!s || !s->focused || !within(s->focused, n)) return false;
        } else if (p == "blur") {
            if (is_focused(n)) return false;
        } else if (p == "dark") {
        } else if (p == "can-focus") {
            if (!n->can_focus) return false;
        } else
            return false;   // hover, disabled, light, ansi, inline, nocolor ...
    }
    return true;
}
static bool match_sel(const Selector& s, int idx, Node* n, const std::string* comp) {
    if (!n || !match_compound(s.parts[idx], n, comp)) return false;
    if (idx == 0) return true;
    Node* up = comp ? n : n->parent;
    if (s.comb[idx - 1] == '>') return match_sel(s, idx - 1, up, nullptr);
    for (Node* a = up; a; a = a->parent)
        if (match_sel(s, idx - 1, a, nullptr)) return true;
    return false;
}
static bool matches(const Selector& s, Node* n, const std::string* comp = nullptr) {
    return !s.parts.empty() && match_sel(s, (int)s.parts.size() - 1, n, comp);
}

// a value with the theme's variables put in
static std::string subst(const std::string& v) {
    if (v.find('$') == std::string::npos || !G) return v;
    std::string out;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == '$') {
            size_t j = i + 1;
            while (j < v.size() && (isalnum((unsigned char)v[j]) || v[j] == '-' || v[j] == '_')) j++;
            auto it = G->vars.v.find(v.substr(i + 1, j - i - 1));
            out += it != G->vars.v.end() ? it->second : "transparent";
            i = j - 1;
        } else
            out += v[i];
    }
    return out;
}
static bool css_color(const std::string& v0, Color& out) {
    std::vector<std::string> w = split_ws(v0);
    if (w.empty()) return false;
    Color c;
    std::string k = lower(w[0]);
    if (k == "black") c = Color::rgb(0, 0, 0);          // (CSS's names are the web's, not the terminal's)
    else if (k == "white") c = Color::rgb(255, 255, 255);
    else if (k == "red") c = Color::rgb(255, 0, 0);
    else if (k == "green") c = Color::rgb(0, 128, 0);
    else if (!parse_color(w[0], c)) return false;
    for (size_t i = 1; i < w.size(); i++)
        if (!w[i].empty() && w[i].back() == '%') c.a *= atof(w[i].c_str()) / 100.0;
    out = c;
    return true;
}
static Scalar css_scalar(const std::string& v) {
    Scalar s;
    std::string t = trim(v);
    if (t == "auto") { s.u = 'a'; return s; }
    if (t.empty()) return s;
    char* end = nullptr;
    s.v = strtod(t.c_str(), &end);
    std::string unit = end ? std::string(end) : "";
    s.u = unit == "fr" ? 'f' : unit == "%" ? '%' : unit == "vw" ? 'w' : unit == "vh" ? 'h' : 'c';
    return s;
}
static void css_edges(const std::string& v, int* e) {
    std::vector<std::string> w = split_ws(v);
    std::vector<int> n;
    for (auto& x : w) n.push_back(atoi(x.c_str()));
    if (n.size() == 1) e[0] = e[1] = e[2] = e[3] = n[0];
    else if (n.size() == 2) e[0] = e[2] = n[0], e[1] = e[3] = n[1];
    else if (n.size() == 4) e[0] = n[0], e[1] = n[1], e[2] = n[2], e[3] = n[3];
}
static Edge css_edge(const std::string& v) {
    Edge e;
    std::vector<std::string> w = split_ws(v);
    if (w.empty() || w[0] == "none" || w[0] == "hidden") return e;
    e.type = w[0];
    std::string rest;
    for (size_t i = 1; i < w.size(); i++) rest += (i > 1 ? " " : "") + w[i];
    if (!css_color(rest, e.c)) e.c = Color::rgb(0x7f, 0x7f, 0x7f);   // (textual: the default border colour)
    return e;
}
static void apply_decl(Styles& s, const std::string& k, const std::string& v0) {
    std::string v = subst(v0);
    if (k == "background") { Color c; if (css_color(v, c)) s.bg = c; }
    else if (k == "color") { Color c; if (css_color(v, c)) s.fg = c; }
    else if (k == "background-tint") { Color c; if (css_color(v, c)) s.tint = c; }
    else if (k == "width") s.width = css_scalar(v);
    else if (k == "height") s.height = css_scalar(v);
    else if (k == "min-width") s.min_w = css_scalar(v);
    else if (k == "min-height") s.min_h = css_scalar(v);
    else if (k == "max-width") s.max_w = css_scalar(v);
    else if (k == "max-height") s.max_h = css_scalar(v);
    else if (k == "padding") css_edges(v, s.pad);
    else if (k == "margin") css_edges(v, s.mar);
    else if (k == "padding-top") s.pad[0] = atoi(v.c_str());
    else if (k == "padding-right") s.pad[1] = atoi(v.c_str());
    else if (k == "padding-bottom") s.pad[2] = atoi(v.c_str());
    else if (k == "padding-left") s.pad[3] = atoi(v.c_str());
    else if (k == "margin-top") s.mar[0] = atoi(v.c_str());
    else if (k == "margin-right") s.mar[1] = atoi(v.c_str());
    else if (k == "margin-bottom") s.mar[2] = atoi(v.c_str());
    else if (k == "margin-left") s.mar[3] = atoi(v.c_str());
    else if (k == "border") { Edge e = css_edge(v); for (int i = 0; i < 4; i++) s.border[i] = e; }
    else if (k == "border-top") s.border[0] = css_edge(v);
    else if (k == "border-right") s.border[1] = css_edge(v);
    else if (k == "border-bottom") s.border[2] = css_edge(v);
    else if (k == "border-left") s.border[3] = css_edge(v);
    else if (k == "text-style") {
        Style t = parse_style(v == "none" ? "not bold not dim not italic not underline not reverse not strike" : v);
        s.ts = s.ts + t;
    } else if (k == "dock") s.dock = trim(v);
    else if (k == "layout") s.layout = trim(v);
    else if (k == "display") s.display = trim(v);
    else if (k == "visibility") s.visibility = trim(v);
    else if (k == "align" || k == "content-align") {
        std::vector<std::string> w = split_ws(v);
        if (w.size() == 2) (k == "align" ? s.align_h : s.calign_h) = w[0], (k == "align" ? s.align_v : s.calign_v) = w[1];
    } else if (k == "align-horizontal") s.align_h = trim(v);
    else if (k == "align-vertical") s.align_v = trim(v);
    else if (k == "content-align-horizontal") s.calign_h = trim(v);
    else if (k == "content-align-vertical") s.calign_v = trim(v);
    else if (k == "overflow") {
        std::vector<std::string> w = split_ws(v);
        if (w.size() == 1) s.overflow_x = s.overflow_y = w[0];
        else if (w.size() == 2) s.overflow_x = w[0], s.overflow_y = w[1];
    } else if (k == "overflow-x") s.overflow_x = trim(v);
    else if (k == "overflow-y") s.overflow_y = trim(v);
    else if (k == "scrollbar-size-vertical") s.sb_v = atoi(v.c_str());
    else if (k == "scrollbar-size-horizontal") s.sb_h = atoi(v.c_str());
    else if (k == "scrollbar-size") {
        std::vector<std::string> w = split_ws(v);
        if (w.size() == 2) s.sb_h = atoi(w[0].c_str()), s.sb_v = atoi(w[1].c_str());
    }
}
// the computed styles of a node (or of one of its component classes): the matching rules, defaults first,
// by specificity, then in order
static Styles compute(Node* n, const std::string* comp = nullptr) {
    struct M { int origin, a, b, c, order; const Rule* r; };
    std::vector<M> ms;
    if (G)
        for (auto& r : G->rules) {
            int best = -1, ba = 0, bb = 0, bc = 0;
            for (auto& s : r.sels)
                if (matches(s, n, comp)) {
                    int sc = s.a * 10000 + s.b * 100 + s.c;
                    if (sc > best) best = sc, ba = s.a, bb = s.b, bc = s.c;
                }
            if (best >= 0) ms.push_back({r.origin, ba, bb, bc, r.order, &r});
        }
    std::stable_sort(ms.begin(), ms.end(), [](const M& x, const M& y) {
        if (x.origin != y.origin) return x.origin < y.origin;
        if (x.a != y.a) return x.a < y.a;
        if (x.b != y.b) return x.b < y.b;
        if (x.c != y.c) return x.c < y.c;
        return x.order < y.order;
    });
    Styles s;
    for (auto& m : ms)
        for (auto& [k, v] : m.r->decls) apply_decl(s, k, v);
    return s;
}
static void compute_tree(Node* n) {
    n->st = compute(n);
    for (auto& c : n->children) compute_tree(node_of(c));
}

// ------------------------------------------------------------------------------------------ messages
static std::unordered_map<Obj*, std::vector<std::pair<var, std::string>>> g_on;   // @on: function -> (message class, selector)
static bool sel_matches(const std::string& sel, Node* n) {
    if (sel.empty()) return true;
    size_t p = 0;
    while (p <= sel.size()) {
        size_t q = sel.find(',', p);
        if (q == std::string::npos) q = sel.size();
        std::string one = trim(sel.substr(p, q - p));
        if (!one.empty() && matches(parse_selector(one), n)) return true;
        p = q + 1;
    }
    return false;
}
static int arity(const var& f) {   // the parameters a handler takes besides self (-1: any)
    var fn = f;
    if (fn.is_ref() && fn.ref()->type == T_method) fn = static_cast<MethodObj*>(fn.ref())->func;
    if (fn.is_ref() && fn.ref()->type == T_function) {
        auto* fo = static_cast<FuncObj*>(fn.ref());
        if (fo->varargs >= 0) return -1;
        int n = fo->npos;
        return f.ref()->type == T_method ? n - 1 : n;
    }
    return -1;
}
static void call_handler(const var& bound, const var& msg) {
    int a = arity(bound);
    var r = a == 0 ? call0(bound) : call1(bound, msg);
    finish(r);
}
// a message through a node's handlers: on_<name> and the @on handlers whose message class and selector match;
// then up to its parent (when it bubbles), as textual's dispatch
static void dispatch(Node* target, const var& msg, bool bubble) {
    std::string handler = str_utf8(getattr(msg, "handler_name"_s, ""_s));
    var control = getattr(msg, "control"_s, None);
    Node* cn = control.is_none() ? nullptr : node_of(control, false);
    for (Node* n = target; n; n = n->parent) {
        var self = obj(n);
        TypeObj* t = type_of(self);
        std::set<Obj*> seen;
        for (TypeObj* k : t->mro) {
            if (!k->dict) continue;
            for (auto& e : k->dict->entries) {
                if (e.key.is_unbound() || !e.value.is_ref()) continue;
                auto it = g_on.find(e.value.ref());
                if (it == g_on.end() || seen.count(e.value.ref())) continue;
                // (a method a subclass overrides is the subclass's)
                const var* top = t->lookup(e.key);
                if (!top || !top->is_ref() || top->ref() != e.value.ref()) continue;
                seen.insert(e.value.ref());
                for (auto& [cls, sel] : it->second) {
                    if (!isinstance(msg, cls).truthy()) continue;
                    if (!sel.empty() && !(cn && sel_matches(sel, cn))) continue;
                    var bound = self.getattr(e.key);
                    call_handler(bound, msg);
                    break;
                }
            }
        }
        if (!handler.empty()) {
            const var* m = t->lookup(handler.c_str());
            if (m && !seen.count(m->ref())) call_handler(self.getattr(handler.c_str()), msg);
        }
        if (!bubble || getattr(msg, "_stop"_s, false).truthy()) break;
    }
}
static void post(Node* target, const var& msg, bool bubble = true) {
    if (!G || !target) return;
    var keep = obj(target);
    defer([keep, msg, bubble]() {
        Node* n = node_of(keep, false);
        if (n && !n->removed) guarded([&] { dispatch(n, msg, bubble); });
    });
}
static var make_msg(int cls, std::initializer_list<std::pair<const char*, var>> attrs) {
    var m = call0(g_cls[cls]);
    for (auto& [k, v] : attrs) m.setattr(k, v);
    return m;
}

// ------------------------------------------------------------------------------------------ timers, workers
static var add_timer(Node* owner, double interval, const var& cb, bool repeat, int64_t count = 0) {
    auto t = std::make_shared<TimerRec>();
    t->owner = obj(owner);
    t->cb = cb;
    t->interval = std::max(0.0, interval);
    t->next = now_s() + t->interval;
    t->repeat = repeat;
    t->repeat_count = count;
    app_state()->timers.push_back(t);
    var timer = call0(g_cls[C_Timer]);
    timer.setattr("_tui_timer", var((int64_t)(intptr_t)t.get()));
    return timer;
}
static void stop_timers_of(Node* n) {
    if (!G) return;
    for (auto& t : G->timers) {
        Node* o = node_of(t->owner, false);
        if (o && within(o, n)) t->stopped = true;
    }
}
static void run_timers() {
    double t = now_s();
    std::vector<std::shared_ptr<TimerRec>> due;
    for (auto& tm : G->timers)
        if (!tm->stopped && tm->next <= t) due.push_back(tm);
    for (auto& tm : due) {
        if (tm->stopped) continue;
        tm->fired++;
        if (tm->repeat && (!tm->repeat_count || tm->fired < tm->repeat_count)) {
            tm->next += tm->interval;
            if (tm->next < t) tm->next = t + tm->interval;   // (textual skips the ticks a slow callback missed)
        } else
            tm->stopped = true;
        guarded([&] { finish(call0(tm->cb)); });
        request_redraw();
        if (G->exiting) break;
    }
    G->timers.erase(std::remove_if(G->timers.begin(), G->timers.end(), [](const std::shared_ptr<TimerRec>& x) { return x->stopped; }), G->timers.end());
}
static double next_timer_in() {
    double t = now_s(), best = 1e9;
    for (auto& tm : G->timers)
        if (!tm->stopped) best = std::min(best, tm->next - t);
    return best;
}

// the calls workers made from their threads (call_from_thread), run here
static void run_thread_calls() {
    std::deque<std::shared_ptr<ThreadCall>> todo;
    {
        std::lock_guard<std::mutex> g(G->mu);
        todo.swap(G->calls);
    }
    for (auto& c : todo) {
        try {
            c->result = finish(call_obj(c->fn, c->args));
        } catch (error& e) {
            c->exc = e.value;
        }
        {
            std::lock_guard<std::mutex> g(G->mu);
            c->done = true;
        }
        G->cv.notify_all();
        request_redraw();
    }
}

// ------------------------------------------------------------------------------------------ compose and mount
static void mount(Node* n, Node* parent);
static void compose_children(Node* n);
static Node* ensure_node(const var& self);
static void add_child(Node* parent, const var& child) {
    Node* c = ensure_node(child);
    c->parent = parent;
    parent->children.push_back(child);
}
// node.compose() run with textual's compose stacks: `with Container(): yield w` puts w in the container
static std::vector<var> run_compose(Node* n) {
    var self = obj(n);
    g_compose_stacks.push_back({});
    g_composed.push_back({});
    std::vector<var> out;
    try {
        var gen = self.call("compose");
        if (!gen.is_none()) {
            var it = iter(gen);
            var child;
            while (next_(it, child)) {
                auto& stack = g_compose_stacks.back();
                if (!stack.empty()) node_of(stack.back())->pending.push_back(child);
                else g_composed.back().push_back(child);
            }
        }
    } catch (...) {
        g_compose_stacks.pop_back();
        g_composed.pop_back();
        throw;
    }
    out = g_composed.back();
    g_compose_stacks.pop_back();
    g_composed.pop_back();
    return out;
}
static void set_tab_active(Node* tabbed, const std::string& id, bool post_msg);
static void compose_children(Node* n) {
    std::vector<var> pend = n->pending;
    n->pending.clear();
    std::vector<var> composed = run_compose(n);
    if (n->wt == W_TABBED) {
        // TabbedContent composes ContentTabs (a Tab per pane) and a ContentSwitcher holding the panes
        std::vector<var> panes = pend;
        panes.insert(panes.end(), composed.begin(), composed.end());
        var tabs = call0(g_cls[C_ContentTabs]);
        Node* tn = ensure_node(tabs);
        tn->id = "--content-tabs";
        var sw = call0(g_cls[C_Switcher]);
        Node* sn = ensure_node(sw);
        for (auto& p : panes) {
            Node* pn = ensure_node(p);
            var tab = call0(g_cls[C_Tab]);
            Node* tb = ensure_node(tab);
            tb->id = "--content-tab-" + pn->id;
            tb->title = pn->title;
            tb->content = new_str(pn->title);
            add_child(tn, tab);
            add_child(sn, p);
        }
        add_child(n, tabs);
        add_child(n, sw);
        if (n->active.empty() && !panes.empty()) n->active = node_of(panes[0])->id;
        return;
    }
    for (auto& c : pend) add_child(n, c);
    for (auto& c : composed) add_child(n, c);
}
static void mount(Node* n, Node* parent) {
    if (n->mounted) return;
    n->parent = parent;
    n->mounted = true;
    compose_children(n);
    for (auto& c : std::vector<var>(n->children)) mount(node_of(c), n);
    if (n->wt == W_TABBED) set_tab_active(n, n->active, false);
    if (n->wt == W_LOADING) n->t0 = now_s();
    var self = obj(n);
    if (self.hasattr("on_mount")) guarded([&] { finish(self.call("on_mount")); });
    request_redraw();
}
static void unmount(Node* n) {
    walk(n, [](Node* x) { x->removed = true; });
    stop_timers_of(n);
    Node* s = screen_of(n);
    if (s && s->focused && within(s->focused, n)) s->focused = nullptr;
    if (n->parent) {
        auto& ch = n->parent->children;
        for (size_t i = 0; i < ch.size(); i++)
            if (ch[i].ref() == n->self) { ch.erase(ch.begin() + i); break; }
        n->parent = nullptr;
    }
    request_redraw();
}

// ------------------------------------------------------------------------------------------ focus
static std::vector<Node*> focus_chain(Node* screen) {
    std::vector<Node*> out;
    std::function<void(Node*)> rec = [&](Node* n) {
        if (!n->display || n->st.display == "none") return;
        if (n != screen && n->can_focus) out.push_back(n);
        for (auto& c : n->children) rec(node_of(c));
    };
    rec(screen);
    return out;
}
static void set_focus(Node* screen, Node* n) {
    if (!screen) return;
    if (screen->focused == n) return;
    screen->focused = n;
    if (n && n->wt == W_INPUT) {
        n->cursor = (int)n->value.size();
        n->sel = (n->select_on_focus && !n->value.empty()) ? 0 : -1;
    }
    request_redraw();
}
static void focus_move(int dir) {
    Node* s = active_screen();
    if (!s) return;
    compute_tree(s);
    std::vector<Node*> ch = focus_chain(s);
    if (ch.empty()) return;
    auto it = std::find(ch.begin(), ch.end(), s->focused);
    int i = it == ch.end() ? (dir > 0 ? -1 : 0) : (int)(it - ch.begin());
    i = ((i + dir) % (int)ch.size() + (int)ch.size()) % (int)ch.size();
    set_focus(s, ch[i]);
}
static void auto_focus(Node* s) {
    if (s->focused) return;
    compute_tree(s);
    std::vector<Node*> ch = focus_chain(s);
    if (!ch.empty()) set_focus(s, ch[0]);
}

// ------------------------------------------------------------------------------------------ screens
static void push_screen(const var& screen, const var& callback) {
    AppState* A = app_state();
    Node* sn = ensure_node(screen);
    sn->dismiss_cb = callback;
    sn->parent = A->node;
    A->stack.push_back(screen);
    mount(sn, A->node);
    auto_focus(sn);
    request_redraw();
}
static var pop_screen() {
    AppState* A = app_state();
    if (A->stack.size() <= 1) raise(E_RuntimeError, "Can't pop screen; there must be at least one screen on the stack");
    var top = A->stack.back();
    A->stack.pop_back();
    unmount(node_of(top));
    A->full = true;
    request_redraw();
    return top;
}

// ------------------------------------------------------------------------------------------ bindings and actions
struct BindingR {
    std::string key, action, description;
    bool show = true, priority = false;
    std::string key_display;
    var obj;
};
static std::vector<std::string> split_keys(const std::string& keys) {
    std::vector<std::string> out;
    size_t p = 0;
    while (p <= keys.size()) {
        size_t q = keys.find(',', p);
        if (q == std::string::npos) q = keys.size();
        std::string k = trim(keys.substr(p, q - p));
        if (!k.empty()) out.push_back(k);
        p = q + 1;
    }
    return out;
}
static std::string normalize_key(const std::string& k);
// a node's bindings: BINDINGS of each class in the MRO (a subclass's replace a base's for the same key)
static std::vector<std::pair<std::string, std::vector<BindingR>>> bindings_of(Node* n) {
    std::vector<std::pair<std::string, std::vector<BindingR>>> keys;
    var self = obj(n);
    TypeObj* t = type_of(self);
    for (auto it = t->mro.rbegin(); it != t->mro.rend(); ++it) {
        TypeObj* k = *it;
        if (!k->dict) continue;
        var* b = k->dict->get("BINDINGS");
        if (!b) continue;
        std::vector<std::pair<std::string, std::vector<BindingR>>> mine;
        for (var x : each(*b)) {
            std::vector<BindingR> rs;
            if (is_tuple(x)) {
                auto& it2 = tuple_items(x);
                BindingR r;
                r.key = str_utf8(it2.at(0));
                r.action = it2.size() > 1 ? str_utf8(it2[1]) : "";
                r.description = it2.size() > 2 ? str_utf8(it2[2]) : "";
                for (auto& kk : split_keys(r.key)) {
                    BindingR one = r;
                    one.key = normalize_key(kk);
                    rs.push_back(one);
                }
            } else {
                BindingR r;
                r.obj = x;
                r.action = str_utf8(x.getattr("action"));
                r.description = str_(x.getattr("description"));
                r.show = x.getattr("show").truthy();
                r.priority = x.getattr("priority").truthy();
                var kd = x.getattr("key_display");
                if (!kd.is_none()) r.key_display = str_utf8(kd);
                for (auto& kk : split_keys(str_utf8(x.getattr("key")))) {
                    BindingR one = r;
                    one.key = normalize_key(kk);
                    rs.push_back(one);
                }
            }
            for (auto& r : rs) {
                bool found = false;
                for (auto& [kk, v] : mine)
                    if (kk == r.key) { v.push_back(r); found = true; }
                if (!found) mine.push_back({r.key, {r}});
            }
        }
        for (auto& [kk, v] : mine) {
            bool found = false;
            for (auto& [k2, v2] : keys)
                if (k2 == kk) { v2 = v; found = true; }
            if (!found) keys.push_back({kk, v});
        }
    }
    if (n->wt == W_APP) {   // the command palette's binding, as textual adds it
        bool have = false;
        for (auto& [k, v] : keys)
            for (auto& b : v)
                if (b.action == "command_palette" || b.action == "app.command_palette") have = true;
        if (!have && getattr(self, "ENABLE_COMMAND_PALETTE"_s, true).truthy()) {
            BindingR r;
            r.key = "ctrl+p", r.action = "command_palette", r.description = "palette", r.show = false, r.priority = true;
            keys.push_back({"ctrl+p", {r}});
        }
    }
    return keys;
}
static bool consumes(Node* n, const std::string& key, char32_t ch) {   // an Input takes printable keys
    // (textual's key_to_character: one character, or a name unicodedata knows -- not slash, at, minus ...)
    static const std::set<std::string> unnamed = {"slash", "at", "minus", "plus", "backslash", "underscore", "less_than_sign", "greater_than_sign"};
    if (unnamed.count(key)) return false;
    return n->wt == W_INPUT && ch >= 0x20 && ch != 0x7f;
}
static char32_t key_char(const std::string& key);
// textual's Screen._binding_chain: the focused widget and its ancestors (else the screen and the app), the keys
// a node consumes taken out of the bindings of those above it
static std::vector<std::pair<Node*, std::vector<std::pair<std::string, std::vector<BindingR>>>>> binding_chain(Node* screen) {
    std::vector<std::pair<Node*, std::vector<std::pair<std::string, std::vector<BindingR>>>>> chain;
    Node* f = screen->focused;
    if (!f) {
        chain.push_back({screen, bindings_of(screen)});
        chain.push_back({G->node, bindings_of(G->node)});
    } else
        for (Node* n = f; n; n = n->parent) chain.push_back({n, bindings_of(n)});
    std::vector<Node*> filters;
    for (auto& [ns, bm] : chain) {
        for (Node* fl : filters)
            bm.erase(std::remove_if(bm.begin(), bm.end(), [&](const std::pair<std::string, std::vector<BindingR>>& kv) { return consumes(fl, kv.first, key_char(kv.first)); }), bm.end());
        filters.push_back(ns);
    }
    return chain;
}
static std::vector<std::pair<Node*, std::vector<std::pair<std::string, std::vector<BindingR>>>>> modal_chain(Node* screen) {
    auto chain = binding_chain(screen);
    for (size_t i = 0; i < chain.size(); i++)
        if (chain[i].first->is_modal) { chain.resize(i + 1); break; }
    return chain;
}
// a literal argument list: None, True, False, numbers, quoted strings
static std::vector<var> action_args(const std::string& s) {
    std::vector<var> out;
    size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && (isspace((unsigned char)s[i]) || s[i] == ',')) i++;
        if (i >= s.size()) break;
        if (s[i] == '"' || s[i] == '\'') {
            char q = s[i++];
            std::string v;
            while (i < s.size() && s[i] != q) {
                if (s[i] == '\\' && i + 1 < s.size()) i++;
                v += s[i++];
            }
            i++;
            out.push_back(new_str(v));
            continue;
        }
        size_t j = i;
        while (j < s.size() && s[j] != ',') j++;
        std::string w = trim(s.substr(i, j - i));
        i = j;
        if (w == "None") out.push_back(None);
        else if (w == "True") out.push_back(var(true));
        else if (w == "False") out.push_back(var(false));
        else if (w.find('.') != std::string::npos) out.push_back(var(atof(w.c_str())));
        else out.push_back(var((int64_t)atoll(w.c_str())));
    }
    return out;
}
// run "ns.name(args)" with `dflt` as the namespace: true when the namespace had the action
static bool run_action(const std::string& action, Node* dflt) {
    std::string a = trim(action), args;
    size_t paren = a.find('(');
    if (paren != std::string::npos) {
        size_t close = a.rfind(')');
        args = a.substr(paren + 1, close == std::string::npos ? std::string::npos : close - paren - 1);
        a = a.substr(0, paren);
    }
    Node* ns = dflt;
    size_t dot = a.find('.');
    if (dot != std::string::npos) {
        std::string pre = a.substr(0, dot);
        if (pre == "app") ns = G->node;
        else if (pre == "screen") ns = screen_of(dflt) ? screen_of(dflt) : active_screen();
        else if (pre == "focused") ns = active_screen() ? active_screen()->focused : nullptr;
        a = a.substr(dot + 1);
    }
    if (!ns) return false;
    var self = obj(ns);
    std::string m = "action_" + a;
    if (!self.hasattr(m.c_str())) return false;
    CallArgs ca;
    for (auto& x : action_args(args)) ca.pos.push_back(x);
    finish(call_obj(self.getattr(m.c_str()), ca));
    request_redraw();
    return true;
}

// ------------------------------------------------------------------------------------------ keys
static const std::map<char32_t, std::string>& char_names() {
    static const std::map<char32_t, std::string> m = {
        {U' ', "space"}, {U'!', "exclamation_mark"}, {U'"', "quotation_mark"}, {U'#', "number_sign"}, {U'$', "dollar_sign"},
        {U'%', "percent_sign"}, {U'&', "ampersand"}, {U'\'', "apostrophe"}, {U'(', "left_parenthesis"}, {U')', "right_parenthesis"},
        {U'*', "asterisk"}, {U'+', "plus"}, {U',', "comma"}, {U'-', "minus"}, {U'.', "full_stop"}, {U'/', "slash"},
        {U':', "colon"}, {U';', "semicolon"}, {U'<', "less_than_sign"}, {U'=', "equals_sign"}, {U'>', "greater_than_sign"},
        {U'?', "question_mark"}, {U'@', "at"}, {U'[', "left_square_bracket"}, {U'\\', "backslash"}, {U']', "right_square_bracket"},
        {U'^', "circumflex_accent"}, {U'_', "underscore"}, {U'`', "grave_accent"}, {U'{', "left_curly_bracket"},
        {U'|', "vertical_line"}, {U'}', "right_curly_bracket"}, {U'~', "tilde"}};
    return m;
}
static std::string char_to_key(char32_t c) {
    auto& m = char_names();
    auto it = m.find(c);
    if (it != m.end()) return it->second;
    return utf8(std::u32string(1, c));
}
static char32_t key_char(const std::string& key) {
    if (key.find('+') != std::string::npos && key.size() > 1) return 0;
    std::u32string u = u32(key);
    if (u.size() == 1) return u[0];
    for (auto& [c, n] : char_names())
        if (n == key) return c;
    return 0;
}
static std::string normalize_key(const std::string& k) {
    std::u32string u = u32(k);
    if (u.size() == 1 && u[0] != U' ') {
        auto& m = char_names();
        auto it = m.find(u[0]);
        if (it != m.end()) return it->second;
    }
    if (k == "esc") return "escape";
    if (k == "return") return "enter";
    return k;
}
static std::string key_display(const BindingR& b) {
    if (!b.key_display.empty()) return b.key_display;
    std::string key = b.key, prefix;
    std::vector<std::string> mods;
    for (;;) {
        size_t plus = key.find('+');
        if (plus == std::string::npos || plus == key.size() - 1) break;
        mods.push_back(key.substr(0, plus));
        key = key.substr(plus + 1);
    }
    static const std::map<std::string, std::string> alias = {
        {"up", "↑"}, {"down", "↓"}, {"left", "←"}, {"right", "→"}, {"backspace", "⌫"}, {"escape", "esc"},
        {"enter", "⏎"}, {"minus", "-"}, {"space", "space"}, {"pagedown", "pgdn"}, {"pageup", "pgup"}, {"delete", "del"}, {"tab", "⇥"}};
    auto it = alias.find(key);
    std::string disp;
    if (it != alias.end()) disp = it->second;
    else {
        char32_t c = key_char(key);
        disp = c ? utf8(std::u32string(1, c)) : key;
    }
    bool ctrl = false;
    std::vector<std::string> rest;
    for (auto& m : mods) {
        if (m == "ctrl") ctrl = true;
        else rest.push_back(m);
    }
    if (ctrl) disp = "^" + disp;
    std::string out;
    for (auto& m : rest) out += m + "+";
    return out + disp;
}

// ------------------------------------------------------------------------------------------ widgets: data
static void static_update(Node* n, const var& content) {
    n->content = content;
    n->gen++;
    request_redraw();
}
static std::vector<Line>& static_lines(Node* n, int width) {
    if (n->cache_w != width || n->cache_gen != n->gen) {
        var c = n->content;
        if (c.is_none() || c.is_unbound()) n->cache.clear();
        else if (is_str(c) && !n->markup) {
            TextData t;
            t.plain = u32(str_utf8(c));
            n->cache = render_text(t, width, Style());
        } else
            n->cache = render(c, width, Style());
        n->cache_w = width;
        n->cache_gen = n->gen;
    }
    return n->cache;
}
static void input_set(Node* n, const std::u32string& v, bool post_changed = true) {
    bool changed = v != n->value;
    n->value = v;
    if (n->cursor > (int)v.size()) n->cursor = (int)v.size();
    n->sel = -1;
    if (changed && post_changed)
        post(n, make_msg(C_InputChanged, {{"input", obj(n)}, {"value", new_str(utf8(v))}, {"validation_result", None}, {"_control", obj(n)}}));
    request_redraw();
}
static void input_delete_selection(Node* n) {
    if (n->sel < 0 || n->sel == n->cursor) { n->sel = -1; return; }
    int a = std::min(n->sel, n->cursor), b = std::max(n->sel, n->cursor);
    std::u32string v = n->value.substr(0, a) + n->value.substr(b);
    n->cursor = a;
    input_set(n, v);
}
static void input_insert(Node* n, const std::u32string& text) {
    if (n->sel >= 0) {
        int a = std::min(n->sel, n->cursor), b = std::max(n->sel, n->cursor);
        std::u32string v = n->value.substr(0, a) + text + n->value.substr(b);
        n->cursor = a + (int)text.size();
        input_set(n, v);
        return;
    }
    std::u32string v = n->value.substr(0, n->cursor) + text + n->value.substr(n->cursor);
    n->cursor += (int)text.size();
    input_set(n, v);
}
// DataTable: the cursor, and what moving it posts (textual's watch_cursor_coordinate / _highlight_row)
static void table_highlight(Node* n) {
    if (n->cursor_type != "row" && n->cursor_type != "cell") return;
    if (n->cur_row < (int)n->rows.size() && !n->cols.empty())
        post(n, make_msg(C_RowHighlighted, {{"data_table", obj(n)}, {"cursor_row", var((int64_t)n->cur_row)},
                                            {"row_key", n->rows[n->cur_row].key}, {"_control", obj(n)}}));
}
static void table_scroll_to_cursor(Node* n) {
    int view = std::max(1, n->ch - (n->show_header ? 1 : 0));
    if (n->cur_row < n->scroll_y) n->scroll_y = n->cur_row;
    if (n->cur_row >= n->scroll_y + view) n->scroll_y = n->cur_row - view + 1;
    if (n->scroll_y < 0) n->scroll_y = 0;
}
static void table_set_cursor(Node* n, int row, bool post_if_same = false) {
    int maxr = std::max(0, (int)n->rows.size() - 1);
    row = std::max(0, std::min(row, maxr));
    bool changed = row != n->cur_row;
    n->cur_row = row;
    if (changed || post_if_same) table_highlight(n);
    table_scroll_to_cursor(n);
    n->scroll_to_cursor = true;   // (again once laid out: the height may not be known yet)
    request_redraw();
}
static int cell_width_of(const var& cell) {
    int mn, mx;
    measure(cell, 10000, mn, mx);
    return mx;
}
static void set_tab_active(Node* tabbed, const std::string& id, bool post_msg) {
    tabbed->active = id;
    for (Node* k : kids(tabbed)) {
        if (k->wt == W_TABS) {
            for (Node* t : kids(k)) {
                auto& cl = t->classes;
                cl.erase(std::remove(cl.begin(), cl.end(), std::string("-active")), cl.end());
                if (t->id == "--content-tab-" + id) cl.push_back("-active");
            }
        } else if (k->wt == W_SWITCHER) {
            for (Node* p : kids(k)) p->display = p->id == id;
        }
    }
    if (post_msg) post(tabbed, make_msg(C_TabActivated, {{"tabbed_content", obj(tabbed)}, {"_control", obj(tabbed)}}));
    request_redraw();
}
static Node* tabbed_of_tabs(Node* tabs) { return tabs->parent && tabs->parent->wt == W_TABBED ? tabs->parent : nullptr; }

// ------------------------------------------------------------------------------------------ layout
static double resolve(const Scalar& s, double container, double fraction) {
    switch (s.u) {
    case 'c': return s.v;
    case 'f': return s.v * fraction;
    case '%': return s.v / 100.0 * container;
    case 'w': return s.v / 100.0 * (G ? G->W : 80);
    case 'h': return s.v / 100.0 * (G ? G->H : 24);
    default: return 0;
    }
}
static int gutter_w(const Styles& s) { return s.pad[1] + s.pad[3] + bw(s, 1) + bw(s, 3); }
static int gutter_h(const Styles& s) { return s.pad[0] + s.pad[2] + bw(s, 0) + bw(s, 2); }
static int content_height(Node* n, int width, int container_h);
static int content_width(Node* n, int container_w);
static std::vector<Node*> flow_kids(Node* n) {
    std::vector<Node*> out;
    for (Node* k : kids(n))
        if (k->display && k->st.display != "none" && k->st.dock.empty()) out.push_back(k);
    return out;
}
struct BoxM { double w = 0, h = 0; };
static BoxM box_model(Node* n, double cw, double ch, double frw, double frh) {
    const Styles& s = n->st;
    int gw = gutter_w(s), gh = gutter_h(s), mw = s.mar[1] + s.mar[3], mh = s.mar[0] + s.mar[2];
    BoxM b;
    if (!s.width.u) b.w = cw - mw;
    else if (s.width.u == 'a') b.w = content_width(n, (int)(cw - gw - mw)) + gw;
    else b.w = resolve(s.width, cw - mw, frw);
    if (s.min_w.u && s.min_w.u != 'a') b.w = std::max(b.w, resolve(s.min_w, cw - mw, frw));
    if (s.max_w.u && s.max_w.u != 'a') b.w = std::min(b.w, resolve(s.max_w, cw - mw, frw));
    b.w = std::max(0.0, b.w);
    if (!s.height.u) b.h = ch - mh;
    else if (s.height.u == 'a') b.h = content_height(n, std::max(0, (int)b.w - gw), (int)(ch - gh - mh)) + gh;
    else b.h = resolve(s.height, ch - mh, frh);
    if (s.min_h.u && s.min_h.u != 'a') b.h = std::max(b.h, resolve(s.min_h, ch - mh, frh));
    if (s.max_h.u && s.max_h.u != 'a') b.h = std::min(b.h, resolve(s.max_h, ch - mh, frh));
    b.h = std::max(0.0, b.h);
    return b;
}
static bool relative_h(Node* k) { return k->st.height.u == 'f' || k->st.height.u == '%'; }
static int content_height(Node* n, int width, int container_h) {
    switch (n->wt) {
    case W_STATIC: case W_TAB: {
        int h = (int)static_lines(n, std::max(1, width)).size();
        return h;
    }
    case W_INPUT: return 1;
    case W_HEADER: case W_FOOTER: case W_LOADING: return 1;
    case W_TABLE: return (n->show_header ? 1 : 0) + (int)n->rows.size();
    case W_LOG: return (int)n->log.size();
    case W_TABS: return 2;
    case W_CMDLIST: return (int)n->hits.size() * 2;
    default: break;
    }
    std::vector<Node*> fk = flow_kids(n);
    if (n->wt == W_TABBED) {
        int h = 0;
        for (Node* k : kids(n))
            if (k->display && k->st.display != "none") {
                BoxM b = box_model(k, width, std::max(0, container_h - h), width, std::max(0, container_h - h));
                if (relative_h(k) && container_h > 0) return container_h;
                h += (int)b.h + k->st.mar[0] + k->st.mar[2];
            }
        return h;
    }
    if (n->st.layout == "horizontal") {
        int h = 0;
        for (Node* k : fk) {
            if (relative_h(k) || !k->st.height.u) return std::max(h, container_h);
            BoxM b = box_model(k, width, container_h, width, container_h);
            h = std::max(h, (int)b.h + k->st.mar[0] + k->st.mar[2]);
        }
        return h;
    }
    int h = 0, prev = 0;
    bool first = true;
    for (Node* k : fk) {
        if (relative_h(k) && container_h > 0) return container_h;
        BoxM b = box_model(k, width, container_h, width, container_h);
        h += first ? k->st.mar[0] : std::max(prev, k->st.mar[0]);
        h += (int)b.h;
        prev = k->st.mar[2];
        first = false;
    }
    return h + prev;
}
static int content_width(Node* n, int container_w) {
    switch (n->wt) {
    case W_STATIC: case W_TAB: {
        if (n->content.is_none() || n->content.is_unbound()) return 0;
        int mn, mx;
        measure(n->content, std::max(1, container_w), mn, mx);
        return mx;
    }
    default: break;
    }
    int w = 0;
    for (Node* k : flow_kids(n)) {
        if (n->st.layout == "horizontal") w += (int)box_model(k, container_w, 1000, container_w, 1000).w + k->st.mar[1] + k->st.mar[3];
        else w = std::max(w, (int)box_model(k, container_w, 1000, container_w, 1000).w + k->st.mar[1] + k->st.mar[3]);
    }
    return w;
}
static void layout_node(Node* n, int x, int y, int w, int h);
static void place(Node* k, double x, double y, double w, double h) { layout_node(k, (int)x, (int)y, std::max(0, (int)w), std::max(0, (int)h)); }
static void hide_tree(Node* n) {
    n->w = n->h = n->cw = n->ch = 0;
    for (auto& c : n->children) hide_tree(node_of(c));
}
// textual's vertical / horizontal layout over the flow children of n in its content region (docked ones first)
static void layout_children(Node* n) {
    int ax = n->cx, ay = n->cy, aw = n->cw, ah = n->ch;
    for (Node* k : kids(n)) {
        if (!k->display || k->st.display == "none") { hide_tree(k); continue; }
        const std::string& d = k->st.dock;
        if (d.empty()) continue;
        BoxM b = box_model(k, aw, ah, aw, ah);
        int bw2 = (int)b.w, bh = (int)b.h;
        if (d == "top") { place(k, ax + k->st.mar[3], ay + k->st.mar[0], bw2, bh); ay += bh + k->st.mar[0] + k->st.mar[2]; ah -= bh + k->st.mar[0] + k->st.mar[2]; }
        else if (d == "bottom") { place(k, ax + k->st.mar[3], ay + ah - bh - k->st.mar[2], bw2, bh); ah -= bh + k->st.mar[0] + k->st.mar[2]; }
        else if (d == "left") { place(k, ax + k->st.mar[3], ay + k->st.mar[0], bw2, std::min(bh, ah)); ax += bw2 + k->st.mar[1] + k->st.mar[3]; aw -= bw2 + k->st.mar[1] + k->st.mar[3]; }
        else if (d == "right") { place(k, ax + aw - bw2 - k->st.mar[1], ay + k->st.mar[0], bw2, std::min(bh, ah)); aw -= bw2 + k->st.mar[1] + k->st.mar[3]; }
    }
    std::vector<Node*> fk = flow_kids(n);
    if (fk.empty()) return;
    bool scroll = n->wt == W_SCROLL || n->st.overflow_y == "auto" || n->st.overflow_y == "scroll";
    if (n->wt == W_SCREEN) scroll = false;
    int view_h = ah;
    bool horizontal = n->st.layout == "horizontal";
    struct P { Node* k; double x, y, w, h; };
    auto arrange = [&](int width, int height, double& total_main, double& cross_max) {
        std::vector<P> ps;
        std::vector<BoxM> bm(fk.size());
        std::vector<bool> fr(fk.size());
        double fixed = 0, frs = 0, margins = 0;
        int prev = 0;
        for (size_t i = 0; i < fk.size(); i++) {
            Node* k = fk[i];
            const Scalar& main = horizontal ? k->st.width : k->st.height;
            fr[i] = main.u == 'f';
            int m1 = horizontal ? k->st.mar[3] : k->st.mar[0], m2 = horizontal ? k->st.mar[1] : k->st.mar[2];
            margins += i == 0 ? m1 : std::max(prev, m1);
            prev = m2;
            if (fr[i]) frs += main.v;
            else {
                bm[i] = horizontal ? box_model(k, width, height, width, height) : box_model(k, width, height, width, height);
                fixed += horizontal ? (int)bm[i].w : (int)bm[i].h;
            }
        }
        margins += prev;
        double remaining = std::max(0.0, (horizontal ? width : height) - fixed - margins);
        double unit = frs > 0 ? remaining / frs : 0;
        for (size_t i = 0; i < fk.size(); i++)
            if (fr[i]) bm[i] = horizontal ? box_model(fk[i], width, height, unit, height) : box_model(fk[i], width, height, width, unit);
        double pos = horizontal ? fk[0]->st.mar[3] : fk[0]->st.mar[0];
        cross_max = 0;
        for (size_t i = 0; i < fk.size(); i++) {
            Node* k = fk[i];
            double next = pos + (horizontal ? bm[i].w : bm[i].h);
            P p;
            p.k = k;
            if (horizontal) {
                p.x = (int)pos, p.y = k->st.mar[0], p.w = (int)next - (int)pos, p.h = bm[i].h;
                cross_max = std::max(cross_max, bm[i].h + k->st.mar[0] + k->st.mar[2]);
            } else {
                p.x = k->st.mar[3], p.y = (int)pos, p.w = bm[i].w, p.h = (int)next - (int)pos;
                cross_max = std::max(cross_max, bm[i].w + k->st.mar[1] + k->st.mar[3]);
            }
            int m2 = horizontal ? k->st.mar[1] : k->st.mar[2];
            int m1n = i + 1 < fk.size() ? (horizontal ? fk[i + 1]->st.mar[3] : fk[i + 1]->st.mar[0]) : 0;
            pos = next + (i + 1 < fk.size() ? std::max(m2, m1n) : m2);
            ps.push_back(p);
        }
        total_main = pos;
        return ps;
    };
    double total = 0, cross = 0;
    std::vector<P> ps = arrange(aw, ah, total, cross);
    n->sb_shown = false;
    if (scroll && !horizontal) {
        n->virtual_h = (int)total;
        bool show = n->st.overflow_y == "scroll" || n->virtual_h > view_h;
        if (show && n->st.sb_v > 0) {
            n->sb_shown = true;
            aw -= n->st.sb_v;
            ps = arrange(aw, ah, total, cross);
            n->virtual_h = (int)total;
        }
        n->scroll_y = std::max(0, std::min(n->scroll_y, n->virtual_h - view_h));
    } else
        n->scroll_y = 0;
    // align
    double off_x = 0, off_y = 0;
    double used_w = horizontal ? total : cross, used_h = horizontal ? cross : total;
    if (n->st.align_h == "center") off_x = std::max(0.0, (double)(int)((aw - used_w) / 2));
    else if (n->st.align_h == "right") off_x = std::max(0.0, aw - used_w);
    if (n->st.align_v == "middle") off_y = std::max(0.0, (double)(int)((ah - used_h) / 2));
    else if (n->st.align_v == "bottom") off_y = std::max(0.0, ah - used_h);
    for (auto& p : ps) place(p.k, ax + off_x + p.x, ay + off_y + p.y - n->scroll_y, p.w, p.h);
}
static void layout_node(Node* n, int x, int y, int w, int h) {
    n->x = x, n->y = y, n->w = w, n->h = h;
    const Styles& s = n->st;
    n->cx = x + bw(s, 3) + s.pad[3];
    n->cy = y + bw(s, 0) + s.pad[0];
    n->cw = std::max(0, w - gutter_w(s));
    n->ch = std::max(0, h - gutter_h(s));
    switch (n->wt) {
    case W_STATIC: case W_INPUT: case W_TABLE: case W_HEADER: case W_FOOTER: case W_LOADING: case W_LOG: case W_CMDLIST:
        if (n->wt == W_TABLE) {
            if (n->scroll_to_cursor) table_scroll_to_cursor(n), n->scroll_to_cursor = false;
            int view = std::max(1, n->ch - (n->show_header ? 1 : 0));
            n->scroll_y = std::max(0, std::min(n->scroll_y, (int)n->rows.size() - view));
        }
        for (auto& c : n->children) hide_tree(node_of(c));
        return;
    case W_TABS: {
        int tx = n->cx;
        for (Node* t : kids(n)) {
            BoxM b = box_model(t, n->cw, 1, n->cw, 1);
            layout_node(t, tx, n->cy, (int)b.w, 1);
            tx += (int)b.w;
        }
        return;
    }
    default: layout_children(n);
    }
}

// ------------------------------------------------------------------------------------------ painting
struct Canvas {
    int W = 0, H = 0;
    std::vector<Line> rows;
    void reset(int w, int h, const Color& bg) {
        W = w, H = h;
        Cell c;
        c.s.bg = bg;
        rows.assign(h, Line(w, c));
    }
};
struct Clip {
    int x0, y0, x1, y1;
    Clip inter(int x, int y, int w, int h) const { return {std::max(x0, x), std::max(y0, y), std::min(x1, x + w), std::min(y1, y + h)}; }
    bool has(int x, int y) const { return x >= x0 && x < x1 && y >= y0 && y < y1; }
};
static Color mono(const Color& c) {   // (NO_COLOR: textual's luminance grey)
    if (!G || !G->mono || !c.set) return c;
    int g = (int)std::lround(c.r * 0.2126 + c.g * 0.7152 + c.b * 0.0722);
    return Color::rgb(g, g, g, c.a);
}
// one cell of content: its style over the widget's colours (bg composited, dim blended, reverse swapped)
static void put(Canvas& cv, const Clip& clip, int x, int y, const Cell& c, const Color& bg, const Color& fg, const Style& base_attrs) {
    if (!clip.has(x, y) || y < 0 || y >= cv.H || x < 0 || x >= cv.W) return;
    Cell& d = cv.rows[y][x];
    Style s = base_attrs + c.s;
    Color b = s.bg.set ? over(bg, s.bg) : bg;
    Color f = s.fg.set ? (s.fg.a < 1 || s.fg.automatic ? over(b, s.fg) : s.fg) : fg;
    if (!f.set) f = Color::rgb(0xd7, 0xda, 0xe0);
    if (s.reverse == 1) std::swap(f, b);
    if (s.dim == 1) f = blend(b, f, 0.66);
    f.a = b.a = 1;
    d.ch = c.ch;
    d.s = Style();
    d.s.fg = f, d.s.bg = b;
    d.s.bold = s.bold == 1 ? 1 : 0;
    d.s.italic = s.italic == 1 ? 1 : 0;
    d.s.underline = s.underline == 1 ? 1 : 0;
    d.s.strike = s.strike == 1 ? 1 : 0;
}
static void put_text(Canvas& cv, const Clip& clip, int x, int y, const std::u32string& t, const Style& s, const Color& bg, const Color& fg) {
    int cx = x;
    for (char32_t ch : t) {
        Cell c{ch, s};
        put(cv, clip, cx, y, c, bg, fg, Style());
        if (cwidth(ch) == 2) {
            c.ch = 0;
            put(cv, clip, cx + 1, y, c, bg, fg, Style());
        }
        cx += std::max(1, cwidth(ch));
    }
}
static void put_lines(Canvas& cv, const Clip& clip, int x, int y, int w, int h, const std::vector<Line>& lines, int skip, const Color& bg, const Color& fg, const Style& attrs) {
    for (int r = 0; r < h; r++) {
        int li = r + skip;
        if (li < 0 || li >= (int)lines.size()) continue;
        const Line& ln = lines[li];
        for (int c = 0; c < w && c < (int)ln.size(); c++) put(cv, clip, x + c, y + r, ln[c], bg, fg, attrs);   // (a wide character's right half too)
    }
}
// a border edge's character and colours (textual's BORDER_CHARS / BORDER_LOCATIONS for the types used)
static void border_cell(const std::string& type, int row, int col, char32_t& ch, int& loc) {
    struct B { const char* name; const char32_t* chars; int loc[9]; };
    static const B table[] = {
        {"round", U"╭─╮│ │╰─╯", {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {"solid", U"┌─┐│ │└─┘", {0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"heavy", U"┏━┓┃ ┃┗━┛", {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {"double", U"╔═╗║ ║╚═╝", {0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"ascii", U"+-+| |+-+", {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {"tall", U"▊▔▎▊ ▎▊▁▎", {2, 0, 1, 2, 0, 1, 2, 0, 1}},
        {"outer", U"▛▀▜▌ ▐▙▄▟", {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {"inner", U"▗▄▖▐ ▌▝▀▘", {1, 1, 1, 1, 1, 1, 1, 1, 1}},
        {"vkey", U"▏ ▕▏ ▕▏ ▕", {0, 0, 0, 0, 0, 0, 0, 0, 0}}, {"hkey", U"▔▔▔   ▁▁▁", {0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"wide", U"▁▁▁▎ ▊▔▔▔", {1, 1, 1, 0, 1, 3, 1, 1, 1}}, {"blank", U"         ", {0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {"panel", U"▊█▎▊ ▎▊▁▎", {2, 0, 1, 2, 0, 1, 2, 0, 1}},
    };
    const B* b = &table[1];
    for (auto& t : table)
        if (type == t.name) b = &t;
    ch = b->chars[row * 3 + col];
    loc = b->loc[row * 3 + col];
}
static void paint_borders(Canvas& cv, const Clip& clip, Node* n, const Color& outer_bg) {
    const Styles& s = n->st;
    if (!bw(s, 0) && !bw(s, 1) && !bw(s, 2) && !bw(s, 3)) return;
    auto cell = [&](int x, int y, const Edge& e, int row, int col) {
        char32_t ch;
        int loc;
        border_cell(e.type, row, col, ch, loc);
        Color ec = over(n->bg, e.c);
        Color f, b;
        switch (loc) {
        case 1: f = over(outer_bg, e.c), b = outer_bg; break;               // outer
        case 2: f = outer_bg, b = ec; break;                                // reversed: outer background on the border colour
        case 3: f = n->bg, b = over(outer_bg, e.c); break;
        default: f = ec, b = n->bg; break;                                  // inner
        }
        Cell c{ch, Style()};
        c.s.fg = f, c.s.bg = b;
        put(cv, clip, x, y, c, b, f, Style());
    };
    int x0 = n->x, y0 = n->y, x1 = n->x + n->w - 1, y1 = n->y + n->h - 1;
    if (n->w <= 0 || n->h <= 0) return;
    for (int x = x0; x <= x1; x++) {
        if (bw(s, 0)) {
            int col = x == x0 && bw(s, 3) ? 0 : x == x1 && bw(s, 1) ? 2 : 1;
            cell(x, y0, col == 0 ? s.border[3].type == s.border[0].type ? s.border[0] : s.border[0] : s.border[0], 0, col);
        }
        if (bw(s, 2)) {
            int col = x == x0 && bw(s, 3) ? 0 : x == x1 && bw(s, 1) ? 2 : 1;
            cell(x, y1, s.border[2], 2, col);
        }
    }
    for (int y = y0 + bw(s, 0); y <= y1 - bw(s, 2); y++) {
        if (bw(s, 3)) cell(x0, y, s.border[3], 1, 0);
        if (bw(s, 1)) cell(x1, y, s.border[1], 1, 2);
    }
}
// textual's ScrollBarRender.render_bar (vertical): the thumb in eighths of a cell, its ends as partial blocks
static void paint_scrollbar(Canvas& cv, const Clip& clip, int x, int y, int w, int h, int virtual_h, int scroll_y) {
    Color track, thumb;
    parse_color(G->vars.v["scrollbar-background"], track);
    parse_color(G->vars.v["scrollbar"], thumb);
    if (h <= 0) return;
    static const char32_t bars[8] = {U'▁', U'▂', U'▃', U'▄', U'▅', U'▆', U'▇', U' '};
    std::vector<Cell> seg(h, Cell{U' ', Style()});
    for (auto& c : seg) c.s.fg = track, c.s.bg = track;
    if (virtual_h > h) {
        double ratio = (double)virtual_h / h, thumb_size = std::max(1.0, h / ratio);
        double pos = (h - thumb_size) * ((double)scroll_y / (virtual_h - h));
        int start = (int)(pos * 8), end = start + (int)std::ceil(thumb_size * 8);
        int si = std::max(0, start) / 8, sb = std::max(0, start) % 8, ei = std::max(0, end) / 8, eb = std::max(0, end) % 8;
        for (int i = si; i < ei && i < h; i++) seg[i].s.fg = thumb, seg[i].s.bg = thumb;
        if (si < h && bars[7 - sb] != U' ') seg[si].ch = bars[7 - sb], seg[si].s.fg = thumb, seg[si].s.bg = track;
        if (ei < h && bars[7 - eb] != U' ') seg[ei].ch = bars[7 - eb], seg[ei].s.fg = track, seg[ei].s.bg = thumb;
    }
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++) put(cv, clip, x + c, y + r, seg[r], seg[r].s.bg, seg[r].s.fg, Style());
}
static Style comp_style(Node* n, const char* comp, Color& bg, Color& fg, const Color& under) {
    std::string c = comp;
    Styles s = compute(n, &c);
    bg = s.bg.set ? over(under, s.bg) : under;
    if (s.tint.set) bg = tint_c(bg, s.tint);
    fg = s.fg.set ? over(bg, s.fg) : Color();
    Style st = s.ts;
    return st;
}
static void paint_node(Canvas& cv, Clip clip, Node* n, const Color& parent_bg, const Color& parent_fg, const Style& parent_ts);
// rich's ReprHighlighter (what RichLog(highlight=True) does to a str): braces bold, True / False / None, "...",
// numbers (decimal, 0x hex), quoted strings -- with rich's default theme's styles (ANSI colours as textual shows them)
static void repr_highlight(TextData& t) {
    const std::u32string& s = t.plain;
    size_t n = s.size();
    auto word = [](char32_t c) { return is_ident_char_(c) || c >= 0x80; };
    Style brace = parse_style("bold"), num = parse_style("bold not italic cyan"), str = parse_style("not bold not italic green"),
          ell = parse_style("yellow"), yes = parse_style("italic bright_green"), no = parse_style("italic bright_red"), none = parse_style("italic magenta");
    for (size_t i = 0; i < n; i++)
        if (std::u32string(U"[]{}()").find(s[i]) != std::u32string::npos) t.spans.push_back({(int)i, (int)i + 1, brace});
    for (size_t i = 0; i < n; i++) {
        if (i && word(s[i - 1])) continue;
        for (auto [w, st] : {std::make_pair(std::u32string(U"True"), yes), std::make_pair(std::u32string(U"False"), no), std::make_pair(std::u32string(U"None"), none)})
            if (s.compare(i, w.size(), w) == 0 && (i + w.size() == n || !word(s[i + w.size()]))) t.spans.push_back({(int)i, (int)(i + w.size()), st});
    }
    for (size_t i = 0; i + 2 < n; i++)
        if (s[i] == U'.' && s[i + 1] == U'.' && s[i + 2] == U'.') t.spans.push_back({(int)i, (int)i + 3, ell}), i += 2;
    for (size_t i = 0; i < n;) {
        // 0x[0-9a-fA-F]*  |  (?<!\w)-?[0-9]+\.?[0-9]*(e-?\d+?)?\b
        if (s[i] == U'0' && i + 1 < n && s[i + 1] == U'x') {
            size_t j = i + 2;
            while (j < n && s[j] < 128 && isxdigit((int)s[j])) j++;
            t.spans.push_back({(int)i, (int)j, num});
            i = j;
            continue;
        }
        bool lb = i == 0 || !word(s[i - 1]);
        size_t j = i;
        if (lb && j < n && s[j] == U'-') j++;
        if (lb && j < n && s[j] >= U'0' && s[j] <= U'9') {
            size_t d0 = j;
            while (j < n && s[j] >= U'0' && s[j] <= U'9') j++;
            size_t intend = j;
            if (j < n && s[j] == U'.') {
                j++;
                while (j < n && s[j] >= U'0' && s[j] <= U'9') j++;
            }
            // \b at the end: back off to a place followed by no word character
            size_t e = j;
            while (e > d0 && e < n && word(s[e]) && word(s[e - 1])) e = e > intend ? intend : e - 1;
            if (e > d0 && (e == n || !word(s[e]) || !word(s[e - 1]))) {
                t.spans.push_back({(int)i, (int)e, num});
                i = e;
                continue;
            }
        }
        i++;
    }
    for (size_t i = 0; i < n; i++) {
        char32_t q = s[i];
        if (q != U'\'' && q != U'"') continue;
        if (i && (s[i - 1] == U'\\' || word(s[i - 1]))) continue;
        size_t j = i + 1;
        while (j < n && !(s[j] == q && s[j - 1] != U'\\')) j++;
        if (j >= n) continue;
        t.spans.push_back({(int)i, (int)j + 1, str});
        i = j;
    }
}
static void paint_widget(Canvas& cv, const Clip& clip, Node* n, const Style& ts) {
    int x = n->cx, y = n->cy, w = n->cw, h = n->ch;
    Clip cc = clip.inter(x, y, w, h);
    switch (n->wt) {
    case W_STATIC: case W_TAB: {
        std::vector<Line>& lines = static_lines(n, std::max(1, w));
        put_lines(cv, cc, x, y, w, h, lines, 0, n->bg, n->fg, ts);
        break;
    }
    case W_LOADING: {
        double elapsed = now_s() - n->t0;
        Color color = n->fg, background = n->bg;
        Color c0 = blend(background, color, 0.1), c2 = lighten(color, 0.1);
        std::u32string dots;
        int total = 9;
        int sx = x + std::max(0, (w - total) / 2), sy = y + std::max(0, (h - 1) / 2);
        for (int d = 0; d < 5; d++) {
            double b = std::fmod(elapsed * 0.8 - d / 8.0, 1.0);
            if (b < 0) b += 1;
            double p = (1 - b) * (1 - b);
            Color c = p <= 0.7 ? blend(c0, color, p / 0.7) : blend(color, c2, (p - 0.7) / 0.3);
            Cell cell{U'●', Style()};
            cell.s.fg = c;
            put(cv, cc, sx + d * 2, sy, cell, n->bg, n->fg, ts);
        }
        request_redraw();
        break;
    }
    case W_HEADER: {
        Node* scr = screen_of(n);
        var s = scr ? obj(scr) : var(None);
        var title = s.is_none() ? var(None) : getattr(s, "title"_s, None);
        if (title.is_none()) title = getattr(G->app, "title"_s, ""_s);
        var sub = s.is_none() ? var(None) : getattr(s, "sub_title"_s, None);
        if (sub.is_none()) sub = getattr(G->app, "sub_title"_s, ""_s);
        Cell icon{U'⭘', Style()};
        put(cv, cc, x + 1, y, icon, n->bg, n->fg, ts);
        std::u32string t = u32(str_(title)), st = u32(str_(sub));
        int tw = cell_len(t) + (st.empty() ? 0 : 3 + cell_len(st));
        int area = std::max(0, w - 18);
        int tx = x + 8 + std::max(0, (area - tw) / 2);
        put_text(cv, cc, tx, y, t, Style(), n->bg, n->fg);
        if (!st.empty()) {
            Style dim;
            dim.dim = 1;
            put_text(cv, cc, tx + cell_len(t), y, U" — ", dim, n->bg, n->fg);
            put_text(cv, cc, tx + cell_len(t) + 3, y, st, dim, n->bg, n->fg);
        }
        break;
    }
    case W_FOOTER: {
        Node* scr = screen_of(n);
        if (!scr) break;
        Color kbg, kfg, dbg, dfg;
        Style ks = comp_style(n, "footer-key--key", kbg, kfg, n->bg);
        Style ds = comp_style(n, "footer-key--description", dbg, dfg, n->bg);
        if (!kfg.set) kfg = n->fg;
        if (!dfg.set) dfg = n->fg;
        auto chain = modal_chain(scr);
        std::vector<std::pair<std::string, BindingR>> active;   // (key, binding) in the chain's order, first per key
        for (auto& [ns, bm] : chain)
            for (auto& [k, bs] : bm) {
                bool have = false;
                for (auto& [k2, b2] : active)
                    if (k2 == k) {
                        have = true;
                        if (bs[0].priority && !b2.priority) b2 = bs[0];
                    }
                if (!have) active.push_back({k, bs[0]});
            }
        std::vector<std::string> actions;
        int cx2 = x;
        for (auto& [k, b] : active) {
            if (!b.show) continue;
            if (std::find(actions.begin(), actions.end(), b.action) != actions.end()) continue;
            actions.push_back(b.action);
            std::u32string key = U" " + u32(key_display(b)) + U" ", desc = u32(b.description) + U" ";
            for (char32_t c : key) {
                Cell cell{c, ks};
                cell.s.fg = kfg;
                put(cv, cc, cx2++, y, cell, kbg, kfg, Style());
            }
            for (char32_t c : desc) {
                Cell cell{c, ds};
                cell.s.fg = dfg;
                put(cv, cc, cx2++, y, cell, dbg, dfg, Style());
            }
        }
        // the command palette's key, docked right
        for (auto& [k, b] : active)
            if (b.action == "command_palette" || b.action == "app.command_palette") {
                std::u32string key = u32(key_display(b)), desc = U" " + u32(b.description) + U" ";
                int total = 1 + cell_len(key) + cell_len(desc);
                int px = x + w - total;
                Color vk;
                css_color(subst("$foreground 20%"), vk);
                Cell bar{U'▏', Style()};
                bar.s.fg = over(n->bg, vk);
                put(cv, cc, px++, y, bar, n->bg, n->fg, Style());
                for (char32_t c : key) {
                    Cell cell{c, ks};
                    cell.s.fg = kfg;
                    put(cv, cc, px++, y, cell, kbg, kfg, Style());
                }
                for (char32_t c : desc) {
                    Cell cell{c, ds};
                    cell.s.fg = dfg;
                    put(cv, cc, px++, y, cell, dbg, dfg, Style());
                }
                break;
            }
        break;
    }
    case W_INPUT: {
        bool foc = is_focused(n);
        Color pbg, pfg, cbg, cfg, sbg, sfg;
        Style ps = comp_style(n, "input--placeholder", pbg, pfg, n->bg);
        Style cs = comp_style(n, "input--cursor", cbg, cfg, n->bg);
        Style ss = comp_style(n, "input--selection", sbg, sfg, n->bg);
        if (n->value.empty()) {
            Cell cell{U' ', ps};
            int cx2 = x;
            for (char32_t c : n->placeholder) {
                if (cx2 >= x + w) break;
                cell.ch = c;
                cell.s.fg = pfg.set ? pfg : n->fg;
                put(cv, cc, cx2++, y, cell, n->bg, n->fg, ts);
            }
            if (foc) {
                Cell cur{n->placeholder.empty() ? U' ' : n->placeholder[0], cs};
                cur.s.fg = cfg, cur.s.bg = cbg;
                put(cv, cc, x, y, cur, n->bg, n->fg, ts);
            }
            break;
        }
        int len = cell_len(n->value);
        if (n->cursor - n->view >= w) n->view = n->cursor - w + 1;
        if (n->cursor < n->view) n->view = n->cursor;
        if (len < w) n->view = 0;
        int a = n->sel >= 0 ? std::min(n->sel, n->cursor) : -1, b = n->sel >= 0 ? std::max(n->sel, n->cursor) : -1;
        for (int i = n->view; i <= (int)n->value.size() && i - n->view < w; i++) {
            char32_t c = i < (int)n->value.size() ? n->value[i] : U' ';
            Cell cell{c, Style()};
            if (foc && i >= a && i < b) cell.s = ss, cell.s.bg = sbg, cell.s.fg = sfg.set ? sfg : n->fg;
            if (foc && i == n->cursor) cell.s = cs, cell.s.bg = cbg, cell.s.fg = cfg;
            put(cv, cc, x + i - n->view, y, cell, n->bg, n->fg, ts);
        }
        break;
    }
    case W_TABLE: {
        bool foc = is_focused(n);
        Color hbg, hfg, ebg, efg, obg, ofg, cbg, cfg;
        Style hs = comp_style(n, "datatable--header", hbg, hfg, n->bg);
        Style es = comp_style(n, "datatable--even-row", ebg, efg, n->bg);
        Style os = comp_style(n, "datatable--odd-row", obg, ofg, n->bg);
        Style cs = comp_style(n, "datatable--cursor", cbg, cfg, n->bg);
        (void)foc;
        int view = h - (n->show_header ? 1 : 0);
        bool sb = (int)n->rows.size() > view && n->st.sb_v > 0;
        int tw = sb ? w - n->st.sb_v : w;
        Clip tc = cc.inter(x, y, tw, h);
        auto draw_row = [&](int ry, const std::vector<var>& cells, const Color& rbg, const Color& rfg, const Style& rs, bool cursor, bool header) {
            int cx2 = x;
            // the row's background across the table's width
            int total = 0;
            for (auto& c : n->cols) total += (c.width >= 0 ? c.width : c.content_w) + 2;
            for (size_t ci = 0; ci < n->cols.size(); ci++) {
                const ColumnD& col = n->cols[ci];
                int cwid = col.width >= 0 ? col.width : col.content_w;
                var cell = ci < cells.size() ? cells[ci] : var(None);
                Color fgc = rfg.set ? rfg : n->fg;
                Cell pad{U' ', rs};
                pad.s.fg = fgc;
                put(cv, tc, cx2, ry, pad, rbg, fgc, ts);
                std::vector<Line> lines;
                TextData td;
                if (is_str(cell)) lines = render_text(markup_text(str_utf8(cell)), cwid, Style(), false);
                else if (text_data(cell, td)) lines = render_text(td, cwid, Style(), false, td.justify);
                else if (is_int(cell) && !cell.is_bool()) {
                    TextData t;
                    t.plain = u32(str_(cell));
                    lines = render_text(t, cwid, Style(), false, "right");
                } else if (!cell.is_none())
                    lines = render(cell, cwid, Style(), false);
                Line ln = lines.empty() ? Line(cwid, Cell{U' ', Style()}) : lines[0];
                for (int k = 0; k < cwid; k++) {
                    Cell c = k < (int)ln.size() ? ln[k] : Cell{U' ', Style()};
                    if (c.ch == 0) continue;
                    Style st = rs + c.s;
                    if (cursor) {
                        if (!(n->renderable_fg && c.s.fg.set)) st.fg = fgc;
                        st.bg = Color();
                    } else if (!c.s.fg.set)
                        st.fg = fgc;
                    if (header) st.fg = fgc;
                    c.s = st;
                    put(cv, tc, cx2 + 1 + k, ry, c, rbg, fgc, ts);
                }
                put(cv, tc, cx2 + 1 + cwid, ry, pad, rbg, fgc, ts);
                cx2 += cwid + 2;
            }
            for (; cx2 < x + tw; cx2++) {
                Cell pad{U' ', rs};
                Color fgc = rfg.set ? rfg : n->fg;
                pad.s.fg = fgc;
                put(cv, tc, cx2, ry, pad, rbg, fgc, ts);
            }
            (void)total;
        };
        int ry = y;
        if (n->show_header) {
            std::vector<var> labels;
            for (auto& c : n->cols) labels.push_back(c.label);
            draw_row(ry++, labels, hbg, hfg, hs, false, true);
        }
        for (int r = 0; r < view; r++) {
            int ri = r + n->scroll_y;
            if (ri >= (int)n->rows.size()) break;
            bool cursor = ri == n->cur_row && (n->cursor_type == "row" || n->cursor_type == "cell");
            bool odd = ri % 2 == 1;
            if (cursor) draw_row(ry + r, n->rows[ri].cells, cbg, cfg, cs, true, false);
            else if (n->zebra) draw_row(ry + r, n->rows[ri].cells, odd ? obg : ebg, odd ? ofg : efg, odd ? os : es, false, false);
            else draw_row(ry + r, n->rows[ri].cells, n->bg, Color(), Style(), false, false);
        }
        if (sb) paint_scrollbar(cv, cc, x + tw, y + (n->show_header ? 1 : 0), n->st.sb_v, view, (int)n->rows.size(), n->scroll_y);
        break;
    }
    case W_LOG: {
        int sbw = n->st.sb_v;
        int tw = std::max(1, w - sbw);
        if (n->log_w != tw) {
            for (auto& [r, lines] : n->log) lines.clear();
            n->log_w = tw;
        }
        std::vector<Line> all;
        for (auto& [r, lines] : n->log) {
            if (lines.empty()) {
                if (is_str(r) && !n->highlight) {
                    TextData t;
                    t.plain = u32(str_utf8(r));
                    lines = render_text(t, tw, Style(), n->wrap);
                } else if (is_str(r)) {
                    TextData t;
                    t.plain = u32(str_utf8(r));
                    repr_highlight(t);
                    lines = render_text(t, tw, Style(), n->wrap);
                } else
                    lines = render(r, tw, Style(), n->wrap);
            }
            all.insert(all.end(), lines.begin(), lines.end());
        }
        n->virtual_h = (int)all.size();
        int maxs = std::max(0, n->virtual_h - h);
        if (n->auto_scroll) n->scroll_y = maxs;
        n->scroll_y = std::min(n->scroll_y, maxs);
        put_lines(cv, cc.inter(x, y, tw, h), x, y, tw, h, all, n->scroll_y, n->bg, n->fg, ts);
        paint_scrollbar(cv, cc, x + tw, y, sbw, h, std::max(n->virtual_h, h), n->scroll_y);
        break;
    }
    case W_TABS: {
        Color ubg, ufg;
        std::string c = "underline--bar";
        Styles us = compute(n, &c);
        Color hl = us.fg.set ? over(n->bg, us.fg) : n->fg;
        Color back = us.bg.set ? over(n->bg, us.bg) : n->bg;
        (void)ubg, (void)ufg;
        int start = 0, end = 0;
        for (Node* t : kids(n))
            if (has_class(t, "-active")) start = t->cx - n->cx, end = t->cx - n->cx + t->cw;
        std::u32string bar;
        std::vector<bool> lit;
        int width = w;
        if (start == 0 && end == 0) {
            bar.assign(width, U'━');
            lit.assign(width, false);
        } else {
            for (int i = 0; i < std::max(0, start - 1); i++) bar += U'━', lit.push_back(false);
            if (start > 0) bar += U'╸', lit.push_back(false);
            for (int i = start; i < end; i++) bar += U'━', lit.push_back(true);
            if (end != width) bar += U'╺', lit.push_back(false);
            for (int i = 0; i < width - end - 1; i++) bar += U'━', lit.push_back(false);
        }
        for (int i = 0; i < (int)bar.size() && i < width; i++) {
            Cell cell{bar[i], Style()};
            cell.s.fg = lit[i] ? hl : back;
            put(cv, cc, x + i, y + 1, cell, n->bg, n->fg, Style());
        }
        break;
    }
    case W_CMDLIST: {
        Color hbg, hfg, tbg, tfg;
        Node* pal = screen_of(n);
        comp_style(pal ? pal : n, "command-palette--highlight", hbg, hfg, n->bg);
        Style hts = comp_style(pal ? pal : n, "command-palette--help-text", tbg, tfg, n->bg);
        Color cur_bg;
        css_color(subst("$block-cursor-blurred-background"), cur_bg);
        Clip full = clip.inter(n->x, n->y, n->w, n->h - bw(n->st, 2));
        Style bold = parse_style("bold");
        int ry = y;
        for (size_t i = 0; i < n->hits.size() && ry < y + h; i++, ry += 2) {
            var hit = n->hits[i];
            Color rbg = (int)i == n->hl ? over(n->bg, cur_bg) : n->bg;
            for (int k = n->x; k < n->x + n->w; k++)
                for (int r = 0; r < 2; r++) {
                    Cell blank{U' ', Style()};
                    if ((int)i == n->hl) blank.s.bold = 1;
                    put(cv, full, k, ry + r, blank, rbg, n->fg, Style());
                }
            var disp = getattr(hit, "match_display"_s, None);
            std::vector<Line> l1 = render(disp, w, Style(), false);
            if (!l1.empty()) put_lines(cv, full, x, ry, w, 1, l1, 0, rbg, n->fg, bold);
            var help = getattr(hit, "help"_s, None);
            if (!help.is_none()) {
                int mn, mx;
                measure(help, w, mn, mx);
                std::vector<Line> l2 = render(help, mx, Style(), false);
                if (!l2.empty()) put_lines(cv, full, x, ry + 1, mx, 1, l2, 0, rbg, n->fg, hts);
            }
        }
        break;
    }
    default: break;
    }
}
static void paint_node(Canvas& cv, Clip clip, Node* n, const Color& parent_bg, const Color& parent_fg, const Style& parent_ts) {
    if (!n->display || n->st.display == "none" || n->w <= 0 || n->h <= 0) return;
    const Styles& s = n->st;
    Color bg = s.bg.set ? over(parent_bg, s.bg) : parent_bg;
    if (s.tint.set) bg = tint_c(bg, s.tint);
    n->bg = bg;
    n->fg = s.fg.set ? over(bg, s.fg) : parent_fg;
    Style ts = parent_ts + s.ts;
    bool visible = s.visibility != "hidden";
    Clip region = clip.inter(n->x, n->y, n->w, n->h);
    if (visible && n->wt != W_SCREEN) {
        for (int yy = region.y0; yy < region.y1; yy++)
            for (int xx = region.x0; xx < region.x1; xx++) {
                Cell c{U' ', Style()};
                put(cv, region, xx, yy, c, bg, n->fg, Style());
            }
        paint_borders(cv, region, n, parent_bg);
        paint_widget(cv, region, n, ts);
    }
    Clip inner = region.inter(n->cx, n->cy, n->cw, n->ch);
    if (n->sb_shown) {
        int sbx = n->cx + n->cw - n->st.sb_v;
        if (visible) paint_scrollbar(cv, inner, sbx, n->cy, n->st.sb_v, n->ch, n->virtual_h, n->scroll_y);
        inner = inner.inter(n->cx, n->cy, n->cw - n->st.sb_v, n->ch);
    }
    if (n->wt == W_TABS) inner = region;
    for (Node* k : kids(n)) paint_node(cv, k->st.dock.empty() ? inner : region, k, bg, n->fg, ts);
}

// ------------------------------------------------------------------------------------------ the frame
static void paint_toasts(Canvas& cv) {
    double t = now_s();
    G->toasts.erase(std::remove_if(G->toasts.begin(), G->toasts.end(), [t](const Toast& x) { return x.until <= t; }), G->toasts.end());
    int bottom = G->H - 1 - 1;   // (the rack's margin-bottom; the footer)
    Color panel_l1, bgc;
    parse_color(G->vars.v["panel-lighten-1"], panel_l1);
    for (int i = (int)G->toasts.size() - 1; i >= 0; i--) {
        const Toast& to = G->toasts[i];
        int w = std::min(60, G->W / 2);
        int inner = w - 1 - 2;   // the left border, padding 1 1
        TextData msg = to.markup ? markup_text(to.message) : TextData();
        if (!to.markup) msg.plain = u32(to.message);
        std::vector<Line> lines;
        if (!to.title.empty()) {
            Color tc;
            parse_color(G->vars.v[to.severity == "error" ? "text-error" : to.severity == "warning" ? "text-warning" : "text-success"], tc);
            TextData tt;
            tt.plain = u32(to.title);
            tt.base.bold = 1;
            tt.base.fg = tc;
            for (auto& l : render_text(tt, inner, Style())) lines.push_back(l);
        }
        for (auto& l : render_text(msg, inner, Style())) lines.push_back(l);
        int h = (int)lines.size() + 2;
        int y = bottom - h + 1, x = G->W - w;
        bottom = y - 1;   // (margin-top 1)
        if (y < 0) break;
        Clip clip{0, 0, G->W, G->H};
        Color edge;
        parse_color(G->vars.v[to.severity == "error" ? "error" : to.severity == "warning" ? "warning" : "success"], edge);
        Color fg;
        parse_color(G->vars.v["foreground"], fg);
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                Cell cell{U' ', Style()};
                put(cv, clip, x + c, y + r, cell, panel_l1, fg, Style());
            }
            Cell b{r == 0 ? U'▛' : r == h - 1 ? U'▙' : U'▌', Style()};
            b.s.fg = edge;
            put(cv, clip, x, y + r, b, panel_l1, edge, Style());
        }
        put_lines(cv, clip, x + 2, y + 1, inner, (int)lines.size(), lines, 0, panel_l1, fg, Style());
    }
}
static void render_frame() {
    AppState* A = G;
    Color bgc;
    parse_color(A->vars.v["background"], bgc);
    Canvas cv;
    cv.reset(A->W, A->H, bgc);
    // the screens to draw: the top one, and those under it while it is a modal (seen through its background)
    std::vector<Node*> show;
    for (int i = (int)A->stack.size() - 1; i >= 0; i--) {
        Node* s = node_of(A->stack[i]);
        show.insert(show.begin(), s);
        if (!s->is_modal) break;
    }
    Color fgc;
    parse_color(A->vars.v["foreground"], fgc);
    for (Node* s : show) {
        compute_tree(s);
        layout_node(s, 0, 0, A->W, A->H);
        Clip clip{0, 0, A->W, A->H};
        if (s->is_modal) {
            // the screen below seen through this one's background
            Color mb = s->st.bg.set ? s->st.bg : Color::rgb(0, 0, 0, 0);
            for (auto& row : cv.rows)
                for (auto& c : row) {
                    c.s.bg = over(c.s.bg, mb);
                    c.s.fg = over(c.s.fg, mb);
                }
            Color base = over(bgc, mb);
            s->bg = base;
            s->fg = s->st.fg.set ? over(base, s->st.fg) : fgc;
            for (Node* k : kids(s)) paint_node(cv, clip, k, base, s->fg, s->st.ts);
        } else {
            Color base = s->st.bg.set ? over(bgc, s->st.bg) : bgc;
            for (auto& row : cv.rows)
                for (auto& c : row) c.s.bg = base, c.s.fg = fgc;
            s->bg = base;
            s->fg = s->st.fg.set ? over(base, s->st.fg) : fgc;
            for (Node* k : kids(s)) paint_node(cv, clip, k, base, s->fg, s->st.ts);
        }
    }
    if (A->toasts_on) paint_toasts(cv);
    fix_halves(cv.rows);
    A->frame = cv.rows;
}
// the frame as text (each line right-stripped): what pilot dumps and the tests compare
static std::string frame_text() {
    std::string out;
    for (auto& row : G->frame) {
        std::u32string s;
        for (auto& c : row)
            if (c.ch) s += c.ch;
        while (!s.empty() && s.back() == U' ') s.pop_back();
        out += utf8(s) + "\n";
    }
    return out;
}

// ------------------------------------------------------------------------------------------ the console
static void write_console(const std::string& s) {
    if (G->headless || G->hout == INVALID_HANDLE_VALUE) return;
    std::wstring w;
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    w.resize(n);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &w[0], n);
    DWORD done = 0;
    size_t off = 0;
    while (off < w.size()) {
        DWORD chunk = (DWORD)std::min<size_t>(w.size() - off, 16384);
        if (!WriteConsoleW(G->hout, w.data() + off, chunk, &done, nullptr)) break;
        off += done ? done : chunk;
    }
}
static void sgr(std::string& o, const Style& s0) {
    Style s = s0;
    s.fg = mono(s.fg), s.bg = mono(s.bg);
    char b[96];
    snprintf(b, sizeof b, "\x1b[0;38;2;%d;%d;%d;48;2;%d;%d;%d", s.fg.r, s.fg.g, s.fg.b, s.bg.r, s.bg.g, s.bg.b);
    o += b;
    if (s.bold == 1) o += ";1";
    if (s.italic == 1) o += ";3";
    if (s.underline == 1) o += ";4";
    if (s.strike == 1) o += ";9";
    o += "m";
}
static void fix_halves(std::vector<Line>& rows) {
    for (auto& row : rows)
        for (size_t x = 0; x < row.size(); x++)
            if (row[x].ch == 0 && (x == 0 || row[x - 1].ch == 0 || cwidth(row[x - 1].ch) != 2)) row[x].ch = U' ';
}
static void flush_frame() {
    AppState* A = G;
    if (A->headless) return;
    std::string o = "\x1b[?2026h";
    for (int y = 0; y < (int)A->frame.size(); y++) {
        const Line& row = A->frame[y];
        bool same = !A->full && y < (int)A->last.size() && A->last[y].size() == row.size();
        if (same)
            for (size_t x = 0; x < row.size(); x++)
                if (row[x].ch != A->last[y][x].ch || !(row[x].s == A->last[y][x].s)) { same = false; break; }
        if (same) continue;
        char b[32];
        snprintf(b, sizeof b, "\x1b[%d;1H", y + 1);
        o += b;
        Style cur;
        bool first = true;
        std::u32string run;
        for (size_t x = 0; x < row.size(); x++) {
            if (row[x].ch == 0) continue;
            if (first || !(row[x].s == cur)) {
                if (!run.empty()) o += utf8(run), run.clear();
                sgr(o, row[x].s);
                cur = row[x].s;
                first = false;
            }
            run += row[x].ch;
        }
        o += utf8(run);
    }
    o += "\x1b[0m\x1b[?2026l";
    write_console(o);
    A->last = A->frame;
    A->full = false;
}
static void console_size(int& w, int& h) {
    CONSOLE_SCREEN_BUFFER_INFO bi;
    if (G->hout != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(G->hout, &bi)) {
        w = bi.srWindow.Right - bi.srWindow.Left + 1;
        h = bi.srWindow.Bottom - bi.srWindow.Top + 1;
    }
}
static void console_open() {
    AppState* A = G;
    A->hin = GetStdHandle(STD_INPUT_HANDLE);
    A->hout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD m;
    if (!GetConsoleMode(A->hout, &m) || !GetConsoleMode(A->hin, &A->in_mode))
        raise(E_RuntimeError, "the terminal interface needs a console (run it in a terminal)");
    GetConsoleMode(A->hout, &A->out_mode);
    SetConsoleMode(A->hout, A->out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT | DISABLE_NEWLINE_AUTO_RETURN);
    SetConsoleMode(A->hin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    write_console("\x1b[?1049h\x1b[?25l\x1b[2J");
    console_size(A->W, A->H);
}
static void console_close() {
    AppState* A = G;
    if (A->headless || A->hout == INVALID_HANDLE_VALUE) return;
    write_console("\x1b[0m\x1b[2J\x1b[?25h\x1b[?1049l");
    SetConsoleMode(A->hout, A->out_mode);
    SetConsoleMode(A->hin, A->in_mode);
}

// ------------------------------------------------------------------------------------------ input events
static void key_event(const std::string& key, char32_t ch);
static void palette_open();
// a key: priority bindings, the focused widget's own keys (an Input's characters), then the bindings up the chain
static void key_event(const std::string& key, char32_t ch) {
    Node* s = active_screen();
    if (!s) return;
    request_redraw();
    // priority bindings, the app first
    {
        auto chain = binding_chain(s);
        for (auto it = chain.rbegin(); it != chain.rend(); ++it)
            for (auto& [k, bs] : it->second)
                if (k == key)
                    for (auto& b : bs)
                        if (b.priority && run_action(b.action, it->first)) return;
    }
    Node* f = s->focused;
    if (f && f->wt == W_INPUT && ch >= 0x20 && ch != 0x7f) {
        input_insert(f, std::u32string(1, ch));
        return;
    }
    auto chain = modal_chain(s);
    for (auto& [ns, bm] : chain)
        for (auto& [k, bs] : bm)
            if (k == key)
                for (auto& b : bs)
                    if (!b.priority && run_action(b.action, ns)) return;
}
static void mouse_click(int mx, int my) {
    Node* s = active_screen();
    if (!s) return;
    Node* hit = nullptr;
    std::function<void(Node*)> rec = [&](Node* n) {
        if (!n->display || n->st.display == "none") return;
        if (mx >= n->x && mx < n->x + n->w && my >= n->y && my < n->y + n->h) hit = n;
        for (auto& c : n->children) rec(node_of(c));
    };
    for (auto& c : s->children) rec(node_of(c));
    if (!hit) return;
    Node* f = hit;
    while (f && !f->can_focus) f = f->parent;
    if (hit->wt == W_TAB && hit->parent) {
        Node* tb = tabbed_of_tabs(hit->parent);
        if (tb) set_tab_active(tb, hit->id.substr(std::string("--content-tab-").size()), true);
    }
    if (f) set_focus(s, f);
    if (hit->wt == W_TABLE) {
        int row = my - hit->cy - (hit->show_header ? 1 : 0) + hit->scroll_y;
        if (row >= 0 && row < (int)hit->rows.size()) {
            bool same = row == hit->cur_row;
            table_set_cursor(hit, row);
            if (same) post(hit, make_msg(C_RowSelected, {{"data_table", obj(hit)}, {"cursor_row", var((int64_t)row)}, {"row_key", hit->rows[row].key}, {"_control", obj(hit)}}));
        }
    }
    if (hit->wt == W_CMDLIST) {
        int i = (my - hit->cy) / 2;
        if (i >= 0 && i < (int)hit->hits.size()) {
            hit->hl = i;
            run_action("select_hit", screen_of(hit));
        }
    }
    request_redraw();
}
static void mouse_scroll(int mx, int my, int dir) {
    Node* s = active_screen();
    if (!s) return;
    Node* hit = nullptr;
    std::function<void(Node*)> rec = [&](Node* n) {
        if (!n->display || n->st.display == "none") return;
        if (mx >= n->x && mx < n->x + n->w && my >= n->y && my < n->y + n->h) {
            if (n->wt == W_TABLE || n->wt == W_LOG || n->sb_shown) hit = n;
        }
        for (auto& c : n->children) rec(node_of(c));
    };
    for (auto& c : s->children) rec(node_of(c));
    if (!hit) return;
    if (hit->wt == W_LOG) hit->auto_scroll = false;
    hit->scroll_y = std::max(0, hit->scroll_y + dir * 3);
    if (hit->wt == W_LOG && hit->scroll_y >= hit->virtual_h - hit->ch) hit->auto_scroll = true;
    if (hit->wt == W_TABLE) {
        int maxs = std::max(0, (int)hit->rows.size() - (hit->ch - 1));
        hit->scroll_y = std::min(hit->scroll_y, maxs);
    }
    request_redraw();
}
static std::string vk_name(WORD vk) {
    switch (vk) {
    case VK_UP: return "up";
    case VK_DOWN: return "down";
    case VK_LEFT: return "left";
    case VK_RIGHT: return "right";
    case VK_HOME: return "home";
    case VK_END: return "end";
    case VK_PRIOR: return "pageup";
    case VK_NEXT: return "pagedown";
    case VK_INSERT: return "insert";
    case VK_DELETE: return "delete";
    case VK_BACK: return "backspace";
    case VK_TAB: return "tab";
    case VK_RETURN: return "enter";
    case VK_ESCAPE: return "escape";
    default:
        if (vk >= VK_F1 && vk <= VK_F24) return "f" + std::to_string(vk - VK_F1 + 1);
        return "";
    }
}
static void read_console_events() {
    AppState* A = G;
    DWORD n = 0;
    if (!GetNumberOfConsoleInputEvents(A->hin, &n) || !n) return;
    std::vector<INPUT_RECORD> recs(n);
    DWORD got = 0;
    if (!ReadConsoleInputW(A->hin, recs.data(), n, &got)) return;
    static wchar_t high = 0;
    for (DWORD i = 0; i < got && !A->exiting; i++) {
        INPUT_RECORD& r = recs[i];
        if (r.EventType == KEY_EVENT && r.Event.KeyEvent.bKeyDown) {
            auto& k = r.Event.KeyEvent;
            DWORD st = k.dwControlKeyState;
            bool ctrl = st & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED), shift = st & SHIFT_PRESSED, alt = st & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED);
            wchar_t wc = k.uChar.UnicodeChar;
            for (WORD rep = 0; rep < std::max<WORD>(1, k.wRepeatCount); rep++) {
                std::string name = vk_name(k.wVirtualKeyCode);
                if (!name.empty()) {
                    if (name == "tab" && shift) name = "shift+tab";
                    else {
                        if (shift && name != "enter" && name != "escape" && name != "backspace") name = "shift+" + name;
                        if (alt) name = "alt+" + name;
                        if (ctrl) name = "ctrl+" + name;
                    }
                    key_event(name, 0);
                    continue;
                }
                if (wc >= 0xD800 && wc < 0xDC00) { high = wc; continue; }
                char32_t c = wc;
                if (wc >= 0xDC00 && wc < 0xE000 && high) c = 0x10000 + ((high - 0xD800) << 10) + (wc - 0xDC00), high = 0;
                if (!c) {
                    if (ctrl && k.wVirtualKeyCode >= 'A' && k.wVirtualKeyCode <= 'Z') key_event(std::string("ctrl+") + (char)tolower(k.wVirtualKeyCode), 0);
                    continue;
                }
                if (ctrl && c >= 1 && c <= 26) { key_event(std::string("ctrl+") + (char)('a' + c - 1), 0); continue; }
                if (ctrl && c == U' ') { key_event("ctrl+space", 0); continue; }
                if (c < 0x20) continue;
                std::string key = char_to_key(c);
                if (alt) key = "alt+" + key;
                key_event(key, alt ? 0 : c);
            }
        } else if (r.EventType == MOUSE_EVENT) {
            auto& m = r.Event.MouseEvent;
            CONSOLE_SCREEN_BUFFER_INFO bi;
            int top = 0;
            if (GetConsoleScreenBufferInfo(A->hout, &bi)) top = bi.srWindow.Top;
            int mx = m.dwMousePosition.X, my = m.dwMousePosition.Y - top;
            if (m.dwEventFlags == MOUSE_WHEELED) mouse_scroll(mx, my, (short)HIWORD(m.dwButtonState) > 0 ? -1 : 1);
            else if (m.dwEventFlags == 0 && (m.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)) mouse_click(mx, my);
        } else if (r.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            console_size(A->W, A->H);
            A->full = true;
            request_redraw();
        }
    }
}

// ------------------------------------------------------------------------------------------ the loop
static void process_tasks() {
    for (int guard = 0; guard < 100000 && !G->tasks.empty(); guard++) {
        auto f = std::move(G->tasks.front());
        G->tasks.pop_front();
        f();
        if (G->exiting) break;
    }
}
// one turn: the workers' calls, timers, the messages; then the frame when something changed
static void turn() {
    double t = now_s();
    for (auto& to : G->toasts)
        if (to.until <= t) request_redraw();
    run_thread_calls();
    process_tasks();
    run_timers();
    process_tasks();
    if (G->dirty && now_s() >= G->next_frame) {
        G->dirty = false;
        render_frame();
        flush_frame();
        G->next_frame = now_s() + 1.0 / 60;
    }
}
// wait (the global lock given up, so the workers run) for input, a call from a worker, or the next timer
static void wait_events(double max_s) {
    double t = std::min(max_s, std::max(0.0, next_timer_in()));
    if (G->dirty) t = std::min(t, std::max(0.0, G->next_frame - now_s()));
    if (!G->toasts.empty()) t = std::min(t, 0.25);
    if (t <= 0 && G->tasks.empty()) t = 0.001;
    if (!G->tasks.empty()) return;
    DWORD ms = (DWORD)std::max(1.0, std::min(t, 0.05) * 1000);
    gil_release g;
    if (!G->headless && G->hin != INVALID_HANDLE_VALUE) {
        WaitForSingleObject(G->hin, ms);
    } else {
        std::unique_lock<std::mutex> lk(G->mu);
        G->cv.wait_for(lk, std::chrono::milliseconds(ms), [] { return !G->calls.empty(); });
    }
}
static void run_for(double seconds) {
    double until = now_s() + seconds;
    do {
        turn();
        if (G->exiting) break;
        double left = until - now_s();
        if (left <= 0) break;
        wait_events(left);
    } while (true);
    turn();
}
static void settle() {   // what is waiting runs (textual's wait_for_idle)
    for (int i = 0; i < 4 && !G->exiting; i++) {
        turn();
        if (G->tasks.empty()) break;
    }
}
static void start_app(const var& app, bool headless, int w, int h) {
    if (G && G->running) raise(E_RuntimeError, "textual: an app is already running");
    auto* A = new AppState();
    A->app = app;
    A->node = node_of(app);
    A->headless = headless;
    A->W = w, A->H = h;
    const char* nc = getenv("NO_COLOR");
    A->mono = nc && *nc;
    G = A;
    g_main_thread = GetCurrentThreadId();
    A->running = true;
    // the CSS: the widgets' defaults, then the app's (CSS_PATH, CSS)
    parse_css(DEFAULT_CSS, 0, A->rules);
    var path = getattr(app, "CSS_PATH"_s, None);
    std::vector<var> paths;
    if (is_str(path) || (path.is_ref() && path.hasattr("__fspath__"))) paths.push_back(path);
    else if (!path.is_none()) for (var p : each(path)) paths.push_back(p);
    for (auto& p : paths) {
        std::string file = fs_path(p);
        FILE* f = fopen(file.c_str(), "rb");
        if (!f) {
            // relative to the app's module, as textual reads it
            var mod = getattr(type(app), "__module__"_s, None);
            var m = import("sys").getattr("modules").call("get", mod);
            var mf = m.is_none() ? var(None) : getattr(m, "__file__"_s, None);
            if (is_str(mf)) {
                std::string base = str_utf8(mf);
                base = base.substr(0, base.find_last_of("/\\") + 1);
                f = fopen((base + file).c_str(), "rb");
            }
        }
        if (!f) raise(E_FileNotFoundError, "textual: no stylesheet " + file);
        std::string css;
        char buf[65536];
        size_t k;
        while ((k = fread(buf, 1, sizeof buf, f)) > 0) css.append(buf, k);
        fclose(f);
        parse_css(css, 1, A->rules);
    }
    var css = getattr(app, "CSS"_s, ""_s);
    if (is_str(css)) parse_css(str_utf8(css), 1, A->rules);
    A->vars = generate_theme(getattr(app, "_tui_default_theme"_s, None));
    if (!headless) console_open();
}
static void shutdown_app(bool join) {
    AppState* A = G;
    A->running = false;
    A->closed = true;
    {
        std::lock_guard<std::mutex> g(A->mu);
        for (auto& c : A->calls) c->done = true, c->exc = make_exc(E_RuntimeError, "textual: the app is closed");
        A->calls.clear();
    }
    A->cv.notify_all();
    // the stack's screens go (their timers with them)
    for (auto it = A->stack.rbegin(); it != A->stack.rend(); ++it) unmount(node_of(*it));
    A->timers.clear();
    // the workers' threads end (a call_from_thread now raises in them); run_test waits for them, as asyncio.run
    // waits for its executor's threads
    if (join)
        for (auto& t : A->threads) {
            try {
                t.call("join", kw("timeout", var(120.0)));
            } catch (error&) {}
        }
    A->threads.clear();
    console_close();
}

// ------------------------------------------------------------------------------------------ nodes from Python
static Node* ensure_node(const var& self) {
    Node* n = node_of(self, false);
    if (n) return n;
    auto* no = new NodeObj();
    no->type = T_nodeobj;
    var h(static_cast<Obj*>(no));
    self.setattr("_tui_node", h);
    n = &no->n;
    n->self = self.ref();
    var kind = getattr(self, "_tui_kind"_s, var((int64_t)W_WIDGET));
    n->wt = (WT)kind.as_int64();
    for (TypeObj* t : type_of(self)->mro) {
        if (t->name == "object" || t->name == "DOMNode" || t->name == "MessagePump") continue;
        n->type_names.push_back(t->name);
        // (a class made with inherit_css=False is not matched by its bases' type selectors)
        if (t->dict && t->dict->get("_tui_inherit_css") && !t->dict->get("_tui_inherit_css")->truthy()) break;
    }
    n->can_focus = getattr(self, "can_focus"_s, false).truthy();
    n->is_modal = n->wt == W_SCREEN && getattr(self, "_tui_modal"_s, false).truthy();
    return n;
}
static void init_node_args(Node* n, const var& id, const var& classes, const var& name) {
    (void)name;
    if (is_str(id)) n->id = str_utf8(id);
    if (is_str(classes))
        for (auto& c : split_ws(str_utf8(classes))) n->classes.push_back(c);
    else if (!classes.is_none() && !classes.is_unbound())
        for (var c : each(classes)) n->classes.push_back(str_utf8(c));
}

// a query: the first descendant matching a selector (a str) or of a class
static void query_all(Node* root, const var& selector, std::vector<Node*>& out, bool first_only) {
    bool is_cls = !is_str(selector);
    std::vector<Selector> sels;
    if (!is_cls) {
        std::string s = str_utf8(selector);
        size_t p = 0;
        while (p <= s.size()) {
            size_t q = s.find(',', p);
            if (q == std::string::npos) q = s.size();
            std::string one = trim(s.substr(p, q - p));
            if (!one.empty()) sels.push_back(parse_selector(one));
            p = q + 1;
        }
    }
    std::function<bool(Node*)> rec = [&](Node* n) {
        for (auto& c : n->children) {
            Node* k = node_of(c);
            bool ok = false;
            if (is_cls) ok = isinstance(c, selector).truthy();
            else
                for (auto& s : sels)
                    if (matches(s, k)) { ok = true; break; }
            if (ok) {
                out.push_back(k);
                if (first_only) return true;
            }
            if (rec(k)) return true;
        }
        return false;
    };
    rec(root);
}
static var query_one(Node* root, const var& selector, const var& expect) {
    std::vector<Node*> found;
    query_all(root, selector, found, true);
    std::string sel = is_str(selector) ? str_utf8(selector) : str_(getattr(selector, "__name__"_s, selector));
    if (found.empty()) raise(make_exc_args(T(g_cls[C_NoMatches]), {new_str("No nodes match '" + sel + "' on " + repr_(obj(root)))}));
    var r = obj(found[0]);
    if (!expect.is_none() && !expect.is_unbound() && !isinstance(r, expect).truthy())
        raise(make_exc_args(T(g_cls[C_WrongType]), {new_str("Node matching '" + sel + "' is not of type " + str_(getattr(expect, "__name__"_s, expect)))}));
    return r;
}

// ------------------------------------------------------------------------------------------ the command palette
static var g_fuzzy_cache;
static void fuzzy(const std::u32string& q0, const std::u32string& c0, double& score, std::vector<int>& offsets) {
    std::u32string q = q0, c = c0;
    for (auto& ch : q) if (ch < 128) ch = (char32_t)tolower((int)ch);
    for (auto& ch : c) if (ch < 128) ch = (char32_t)tolower((int)ch);
    score = 0;
    offsets.clear();
    if (q.empty()) return;
    std::set<int> first;
    for (size_t i = 0; i < c.size(); i++) {
        bool w = is_ident_char_(c[i]) || c[i] >= 128;
        bool pw = i > 0 && (is_ident_char_(c[i - 1]) || c[i - 1] >= 128);
        if (w && !pw) first.insert((int)i);
    }
    auto sc = [&](const std::vector<int>& pos) {
        double s = (double)pos.size();
        for (int p : pos) s += first.count(p) ? 1 : 0;
        int groups = 1;
        for (size_t i = 1; i < pos.size(); i++)
            if (pos[i] != pos[i - 1] + 1) groups++;
        double ng = (pos.size() - (groups - 1)) / (double)pos.size();
        return s * (1 + ng * ng);
    };
    size_t at = c.find(q);
    if (at != std::u32string::npos) {
        for (size_t i = 0; i < q.size(); i++) offsets.push_back((int)(at + i));
        score = sc(offsets) * (c == q ? 2.0 : 1.5);
        return;
    }
    std::vector<std::vector<int>> letter_pos;
    size_t position = 0;
    for (size_t off = 0; off < q.size(); off++) {
        size_t last_index = c.size() - off;
        std::vector<int> pos;
        size_t index = position;
        for (;;) {
            size_t loc = c.find(q[off], index);
            if (loc == std::u32string::npos) break;
            pos.push_back((int)loc);
            index = loc + 1;
            if (index >= last_index) break;
        }
        if (pos.empty()) return;
        position = pos[0] + 1;
        letter_pos.push_back(pos);
    }
    int budget = 20000;
    std::vector<int> cur;
    std::function<void(size_t)> rec = [&](size_t li) {
        for (int o : letter_pos[li]) {
            if (budget <= 0) return;
            if (!cur.empty() && o <= cur.back()) continue;
            cur.push_back(o);
            if (cur.size() == q.size()) {
                budget--;
                double s = sc(cur);
                if (s > score) score = s, offsets = cur;
            } else
                rec(li + 1);
            cur.pop_back();
        }
    };
    rec(0);
}
static void palette_search(Node* pal) {
    Node* input = nullptr;
    Node* list = nullptr;
    walk(pal, [&](Node* n) {
        if (n->wt == W_INPUT && !input) input = n;
        if (n->wt == W_CMDLIST) list = n;
    });
    if (!input || !list) return;
    std::string q = utf8(input->value);
    std::vector<std::pair<double, var>> hits;
    var providers = getattr(obj(pal), "_tui_providers"_s, None);
    var stop_async = builtin("StopAsyncIteration");
    for (var p : each(providers)) {
        var gen = q.empty() ? p.call("discover") : p.call("search", new_str(q));
        if (gen.is_none()) continue;
        if (gen.hasattr("__anext__")) {
            for (int guard = 0; guard < 100000; guard++) {
                var h;
                try {
                    h = drive(gen.call("__anext__"));
                } catch (error& e) {
                    if (e.matches(stop_async)) break;
                    throw;
                }
                hits.push_back({getattr(h, "score"_s, var(1.0)).as_double(), h});
            }
        } else
            for (var h : each(gen)) hits.push_back({getattr(h, "score"_s, var(1.0)).as_double(), h});
    }
    if (q.empty())
        std::stable_sort(hits.begin(), hits.end(), [](const std::pair<double, var>& a, const std::pair<double, var>& b) {
            return str_(getattr(a.second, "text"_s, ""_s)) < str_(getattr(b.second, "text"_s, ""_s));
        });
    else
        std::stable_sort(hits.begin(), hits.end(), [](const std::pair<double, var>& a, const std::pair<double, var>& b) { return a.first > b.first; });
    list->hits.clear();
    for (auto& [s, h] : hits) list->hits.push_back(h);
    list->hl = 0;
    list->display = !list->hits.empty();
    request_redraw();
}

// ------------------------------------------------------------------------------------------ the classes
struct CB {
    var ns;
    const char* name;
    const char* module;
    CB(const char* n, const char* m) : ns(new_dict()), name(n), module(m) { ns.setitem("__module__"_s, new_str(m)); }
    CB& set(const char* k, const var& v) {
        ns.setitem(new_str(k), v);
        return *this;
    }
    var make(std::vector<var> bases) { return make_class(name, name, bases, ns, {}, module); }
};
static var binding(const char* key, const char* action, const char* desc, bool show = false, bool priority = false) {
    return g_cls[C_Binding](new_str(key), new_str(action), new_str(desc), kw("show", var(show)), kw("priority", var(priority)));
}
static var prop(var get, var set = None) { return property(get, set); }

static void def_dom(CB& c) {
    c.set("can_focus", var(false));
    c.set("BINDINGS", new_list());
    c.set("DEFAULT_CSS", ""_s);
    c.set("compose", def("compose", "self", {}, [](var) -> var { return new_list(); }));
    c.set("__enter__", def("__enter__", "self", {}, [](var self) -> var {
        if (g_compose_stacks.empty()) raise(E_RuntimeError, "textual: a container's `with` outside compose()");
        g_compose_stacks.back().push_back(self);
        return self;
    }));
    c.set("__exit__", def("__exit__", "self, *exc", {}, [](var self, var) -> var {
        auto& stack = g_compose_stacks.back();
        if (!stack.empty()) stack.pop_back();
        if (!stack.empty()) node_of(stack.back())->pending.push_back(self);
        else g_composed.back().push_back(self);
        return var(false);
    }));
    c.set("compose_add_child", def("compose_add_child", "self, widget", {}, [](var self, var w) -> var {
        ensure_node(self)->pending.push_back(w);
        return None;
    }));
    c.set("app", prop(def("app", "self", {}, [](var) -> var { return app_state()->app; })));
    c.set("screen", prop(def("screen", "self", {}, [](var self) -> var {
        Node* n = node_of(self);
        if (n->wt == W_APP) {
            if (!G || G->stack.empty()) raise(E_RuntimeError, "textual: no screen");
            return G->stack.back();
        }
        return obj(screen_of(n));
    })));
    c.set("id", prop(def("id", "self", {}, [](var self) -> var {
        Node* n = node_of(self);
        return n->id.empty() ? var(None) : new_str(n->id);
    }), def("id", "self, value", {}, [](var self, var v) -> var { node_of(self)->id = is_str(v) ? str_utf8(v) : ""; return None; })));
    c.set("parent", prop(def("parent", "self", {}, [](var self) -> var { return obj(node_of(self)->parent); })));
    c.set("children", prop(def("children", "self", {}, [](var self) -> var { return new_list(node_of(self)->children); })));
    c.set("is_mounted", prop(def("is_mounted", "self", {}, [](var self) -> var { return var(node_of(self)->mounted && !node_of(self)->removed); })));
    c.set("has_focus", prop(def("has_focus", "self", {}, [](var self) -> var { return var(is_focused(node_of(self))); })));
    c.set("display", prop(def("display", "self", {}, [](var self) -> var { return var(node_of(self)->display); }),
                          def("display", "self, v", {}, [](var self, var v) -> var { node_of(self)->display = v.truthy(); request_redraw(); return None; })));
    c.set("classes", prop(def("classes", "self", {}, [](var self) -> var {
        var s = new_set(true);
        std::vector<var> xs;
        for (auto& k : node_of(self)->classes) xs.push_back(new_str(k));
        return call1(builtin("frozenset"), new_list(xs));
    })));
    c.set("add_class", def("add_class", "self, *names", {}, [](var self, var names) -> var {
        Node* n = node_of(self);
        for (var x : each(names))
            for (auto& k : split_ws(str_utf8(x)))
                if (!has_class(n, k)) n->classes.push_back(k);
        request_redraw();
        return self;
    }));
    c.set("remove_class", def("remove_class", "self, *names", {}, [](var self, var names) -> var {
        Node* n = node_of(self);
        for (var x : each(names))
            for (auto& k : split_ws(str_utf8(x))) n->classes.erase(std::remove(n->classes.begin(), n->classes.end(), k), n->classes.end());
        request_redraw();
        return self;
    }));
    c.set("has_class", def("has_class", "self, *names", {}, [](var self, var names) -> var {
        for (var x : each(names))
            if (!has_class(node_of(self), str_utf8(x))) return var(false);
        return var(true);
    }));
    c.set("query_one", def("query_one", "self, selector, expect_type=", {None}, [](var self, var sel, var et) -> var {
        Node* n = node_of(self);
        if (n->wt == W_APP) n = active_screen();
        return query_one(n, sel, et);
    }));
    c.set("query", def("query", "self, selector=", {None}, [](var self, var sel) -> var {
        Node* n = node_of(self);
        if (n->wt == W_APP) n = active_screen();
        std::vector<Node*> found;
        if (sel.is_none()) walk(n, [&](Node* k) { if (k != n) found.push_back(k); });
        else query_all(n, sel, found, false);
        std::vector<var> xs;
        for (Node* k : found) xs.push_back(obj(k));
        var q = call0(g_cls[C_Query]);
        q.setattr("_nodes", new_list(xs));
        return q;
    }));
    c.set("set_interval", def("set_interval", "self, interval, callback=, *, name=, repeat=, pause=", {None, None, var(0), var(false)},
                              [](var self, var interval, var cb, var name, var repeat, var pause) -> var {
        (void)name, (void)pause;
        return add_timer(node_of(self), interval.as_double(), cb, true, repeat.as_int64());
    }));
    c.set("set_timer", def("set_timer", "self, delay, callback=, *, name=, pause=", {None, None, var(false)},
                           [](var self, var delay, var cb, var name, var pause) -> var {
        (void)name, (void)pause;
        return add_timer(node_of(self), delay.as_double(), cb, false);
    }));
    c.set("call_later", def("call_later", "self, callback, *args, **kwargs", {}, [](var self, var cb, var args, var kwargs) -> var {
        (void)self;
        defer([cb, args, kwargs]() {
            guarded([&] {
                CallArgs ca;
                ca.pos = collect(args);
                for (var k : each(kwargs)) ca.kw.push_back({k, kwargs.getitem(k)});
                finish(call_obj(cb, ca));
            });
        });
        return var(true);
    }));
    c.set("call_after_refresh", c.ns.getitem("call_later"_s));
    c.set("call_next", c.ns.getitem("call_later"_s));
    c.set("refresh", def("refresh", "self, *a, **k", {}, [](var self, var, var) -> var {
        Node* n = node_of(self);
        n->gen++;
        request_redraw();
        return self;
    }));
    c.set("focus", def("focus", "self, scroll_visible=", {var(true)}, [](var self, var) -> var {
        Node* n = node_of(self);
        if (n->can_focus) set_focus(screen_of(n), n);
        return self;
    }));
    c.set("blur", def("blur", "self", {}, [](var self) -> var {
        Node* n = node_of(self);
        Node* s = screen_of(n);
        if (s && s->focused == n) set_focus(s, nullptr);
        return self;
    }));
    c.set("remove", def("remove", "self", {}, [](var self) -> var {
        Node* n = node_of(self);
        if (n->wt == W_SCREEN) {
            if (G && !G->stack.empty() && G->stack.back().ref() == self.ref()) pop_screen();
        } else
            unmount(n);
        return None;
    }));
    c.set("mount", def("mount", "self, *widgets, before=, after=", {None, None}, [](var self, var widgets, var, var) -> var {
        Node* n = node_of(self);
        for (var w : each(widgets)) {
            add_child(n, w);
            if (n->mounted) mount(node_of(w), n);
        }
        request_redraw();
        return None;
    }));
    c.set("post_message", def("post_message", "self, message", {}, [](var self, var msg) -> var {
        post(node_of(self), msg);
        return var(true);
    }));
    c.set("notify", def("notify", "self, message, *, title=, severity=, timeout=, markup=", {""_s, "information"_s, None, var(true)},
                        [](var self, var message, var title, var severity, var timeout, var markup) -> var {
        (void)self;
        AppState* A = app_state();
        Toast t;
        t.message = str_(message);
        t.title = str_(title);
        t.severity = str_(severity);
        t.markup = markup.truthy();
        double to = timeout.is_none() ? getattr(A->app, "NOTIFICATION_TIMEOUT"_s, var(5)).as_double() : timeout.as_double();
        t.until = now_s() + to;
        A->toasts.push_back(t);
        request_redraw();
        return None;
    }));
    c.set("run_worker", def("run_worker", "self, work, name=, group=, description=, exit_on_error=, start=, exclusive=, thread=",
                            {""_s, "default"_s, ""_s, var(true), var(true), var(false), var(false)},
                            [](var self, var work, var name, var group, var desc, var eoe, var start, var excl, var thread) -> var {
        (void)name, (void)group, (void)desc, (void)eoe, (void)start, (void)excl;
        var wk = call0(g_cls[C_Worker]);
        wk.setattr("is_cancelled", var(false));
        if (thread.truthy()) {
            var t = import("threading").getattr("Thread")(kw("target", work), kw("daemon", var(true)));
            t.call("start");
            app_state()->threads.push_back(t);
        } else
            defer([work]() { guarded([&] { finish(call0(work)); }); });
        (void)self;
        return wk;
    }));
    c.set("scroll_home", def("scroll_home", "self, *a, **k", {}, [](var self, var, var) -> var { node_of(self)->scroll_y = 0; request_redraw(); return None; }));
    c.set("scroll_end", def("scroll_end", "self, *a, **k", {}, [](var self, var, var) -> var { node_of(self)->scroll_y = 1 << 30; request_redraw(); return None; }));
    c.set("log", def("log", "self, *a, **k", {}, [](var, var, var) -> var { return None; }));
    c.set("__repr__", def("__repr__", "self", {}, [](var self) -> var {
        Node* n = node_of(self, false);
        std::string s = type_of(self)->name + "(";
        if (n && !n->id.empty()) s += "id='" + n->id + "'";
        return new_str(s + ")");
    }));
}
static var def_widget_init(WT) {
    return def("__init__", "self, *children, name=, id=, classes=, disabled=, markup=", {None, None, None, var(false), var(true)},
               [](var self, var children, var name, var id, var classes, var disabled, var markup) -> var {
        (void)disabled;
        Node* n = ensure_node(self);
        init_node_args(n, id, classes, name);
        n->markup = markup.truthy();
        for (var c : each(children)) n->pending.push_back(c);
        return None;
    });
}

// ------------------------------------------------------------------------------------------ worker / on
static var make_worker_wrapper(const var& method, bool thread, bool exclusive, const std::string& group) {
    (void)exclusive, (void)group;
    return def("worker", "self, *args, **kwargs", {}, [method, thread](var self, var args, var kwargs) -> var {
        AppState* A = app_state();
        var wk = call0(g_cls[C_Worker]);
        wk.setattr("is_cancelled", var(false));
        wk.setattr("name", getattr(method, "__name__"_s, ""_s));
        CallArgs ca;
        ca.pos.push_back(self);
        for (var a : each(args)) ca.pos.push_back(a);
        for (var k : each(kwargs)) ca.kw.push_back({k, kwargs.getitem(k)});
        var body = natf("worker", [method, ca](const CallArgs&) -> var {
            try {
                finish(call_obj(method, ca));
            } catch (error& e) {
                if (G && G->running && !G->closed) {
                    // (textual: a worker's error ends the app, shown after)
                    var err = e.value;
                    std::lock_guard<std::mutex> g(G->mu);
                    auto c = std::make_shared<ThreadCall>();
                    c->fn = natf("worker_failed", [err](const CallArgs&) -> var {
                        if (G->error.is_unbound() || G->error.is_none()) G->error = err;
                        G->exiting = true;
                        return None;
                    });
                    G->calls.push_back(c);
                    G->cv.notify_all();
                }
            }
            return None;
        });
        if (thread) {
            var t = import("threading").getattr("Thread")(kw("target", body), kw("daemon", var(true)));
            t.call("start");
            A->threads.push_back(t);
        } else
            defer([body]() { guarded([&] { call0(body); }); });
        return wk;
    });
}

// ------------------------------------------------------------------------------------------ registration
static var C(int k) { return g_cls[k]; }
static void build_classes() {
    if (T_nodeobj) return;
    ensure_rich();
    T_nodeobj = new_type("_TuiNode", {}, Layout::NativeData, true, "textual");

    // ---- textual.css.query
    {
        CB nm("NoMatches", "textual.css.query");
        g_cls[C_NoMatches] = nm.make({tv(E_Exception)});
        CB wt("WrongType", "textual.css.query");
        g_cls[C_WrongType] = wt.make({tv(E_Exception)});
        CB q("DOMQuery", "textual.css.query");
        q.set("__iter__", def("__iter__", "self", {}, [](var self) -> var { return iter(self.getattr("_nodes")); }));
        q.set("__len__", def("__len__", "self", {}, [](var self) -> var { return len(self.getattr("_nodes")); }));
        q.set("__bool__", def("__bool__", "self", {}, [](var self) -> var { return var(len_(self.getattr("_nodes")) > 0); }));
        q.set("first", def("first", "self, expect_type=", {None}, [](var self, var) -> var {
            var xs = self.getattr("_nodes");
            if (!len_(xs)) raise(make_exc_args(T(g_cls[C_NoMatches]), {"No nodes match the query"_s}));
            return xs.getitem(var(0));
        }));
        q.set("last", def("last", "self, expect_type=", {None}, [](var self, var) -> var { return self.getattr("_nodes").getitem(var(-1)); }));
        q.set("remove", def("remove", "self", {}, [](var self) -> var {
            for (var x : each(self.getattr("_nodes"))) unmount(node_of(x));
            return self;
        }));
        q.set("add_class", def("add_class", "self, *names", {}, [](var self, var names) -> var {
            for (var x : each(self.getattr("_nodes"))) call_obj(x.getattr("add_class"), [&] { CallArgs a; a.pos = collect(names); return a; }());
            return self;
        }));
        q.set("remove_class", def("remove_class", "self, *names", {}, [](var self, var names) -> var {
            for (var x : each(self.getattr("_nodes"))) call_obj(x.getattr("remove_class"), [&] { CallArgs a; a.pos = collect(names); return a; }());
            return self;
        }));
        g_cls[C_Query] = q.make({});
    }
    // ---- textual.message
    {
        CB m("Message", "textual.message");
        m.set("handler_name", ""_s);
        m.set("bubble", var(true));
        m.set("__init__", def("__init__", "self, *a, **k", {}, [](var self, var, var) -> var {
            self.setattr("_stop", var(false));
            self.setattr("_control", None);
            return None;
        }));
        m.set("stop", def("stop", "self, stop=", {var(true)}, [](var self, var s) -> var { self.setattr("_stop", s); return None; }));
        m.set("prevent_default", def("prevent_default", "self, p=", {var(true)}, [](var self, var) -> var { return self; }));
        m.set("control", prop(def("control", "self", {}, [](var self) -> var { return getattr(self, "_control"_s, None); })));
        g_cls[C_Message] = m.make({});
        auto msgcls = [](int slot, const char* name, const char* handler) {
            CB c(name, "textual.widgets");
            c.set("handler_name", new_str(handler));
            g_cls[slot] = c.make({g_cls[C_Message]});
        };
        msgcls(C_InputChanged, "Changed", "on_input_changed");
        msgcls(C_InputSubmitted, "Submitted", "on_input_submitted");
        msgcls(C_RowHighlighted, "RowHighlighted", "on_data_table_row_highlighted");
        msgcls(C_RowSelected, "RowSelected", "on_data_table_row_selected");
        msgcls(C_TabActivated, "TabActivated", "on_tabbed_content_tab_activated");
    }
    // ---- textual.binding
    {
        CB b("Binding", "textual.binding");
        b.set("__init__", def("__init__", "self, key, action, description=, show=, key_display=, priority=, tooltip=, id=, system=, group=",
                              {""_s, var(true), None, var(false), ""_s, None, var(false), None},
                              [](var self, var key, var action, var desc, var show, var kd, var prio, var tip, var id, var sys, var grp) -> var {
            self.setattr("key", key);
            self.setattr("action", action);
            self.setattr("description", desc);
            self.setattr("show", show);
            self.setattr("key_display", kd);
            self.setattr("priority", prio);
            self.setattr("tooltip", tip);
            self.setattr("id", id);
            self.setattr("system", sys);
            self.setattr("group", grp);
            return None;
        }));
        b.set("__repr__", def("__repr__", "self", {}, [](var self) -> var {
            return new_str("Binding(key=" + repr_(self.getattr("key")) + ", action=" + repr_(self.getattr("action")) + ", description=" + repr_(self.getattr("description")) + ")");
        }));
        g_cls[C_Binding] = b.make({});
    }
    // ---- timers and workers
    {
        CB t("Timer", "textual.timer");
        auto rec = [](var self) -> TimerRec* { return (TimerRec*)(intptr_t)self.getattr("_tui_timer").as_int64(); };
        t.set("stop", def("stop", "self", {}, [rec](var self) -> var {
            TimerRec* r = rec(self);
            if (G)
                for (auto& x : G->timers)
                    if (x.get() == r) x->stopped = true;
            return None;
        }));
        t.set("pause", def("pause", "self", {}, [](var) -> var { return None; }));
        t.set("resume", def("resume", "self", {}, [](var) -> var { return None; }));
        t.set("reset", def("reset", "self", {}, [rec](var self) -> var {
            TimerRec* r = rec(self);
            if (G)
                for (auto& x : G->timers)
                    if (x.get() == r) x->next = now_s() + x->interval;
            return None;
        }));
        g_cls[C_Timer] = t.make({});
        CB w("Worker", "textual.worker");
        w.set("cancel", def("cancel", "self", {}, [](var self) -> var { self.setattr("is_cancelled", var(true)); return None; }));
        g_cls[C_Worker] = w.make({});
    }
    // ---- the DOM: DOMNode, Widget, Screen, App
    {
        CB d("DOMNode", "textual.dom");
        def_dom(d);
        g_cls[C_DOMNode] = d.make({});
        CB w("Widget", "textual.widget");
        w.set("_tui_kind", var((int64_t)W_WIDGET));
        w.set("__init__", def_widget_init(W_WIDGET));
        g_cls[C_Widget] = w.make({C(C_DOMNode)});
    }
    {
        CB s("Screen", "textual.screen");
        s.set("_tui_kind", var((int64_t)W_SCREEN));
        s.set("title", None);
        s.set("sub_title", None);
        s.set("AUTO_FOCUS", "*"_s);
        s.set("BINDINGS", new_list({binding("tab", "app.focus_next", "Focus Next"), binding("shift+tab", "app.focus_previous", "Focus Previous")}));
        s.set("__init__", def("__init__", "self, name=, id=, classes=", {None, None, None}, [](var self, var name, var id, var classes) -> var {
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            return None;
        }));
        s.set("focused", prop(def("focused", "self", {}, [](var self) -> var { return obj(node_of(self)->focused); })));
        s.set("set_focus", def("set_focus", "self, widget, scroll_visible=", {var(true)}, [](var self, var w, var) -> var {
            set_focus(node_of(self), w.is_none() ? nullptr : node_of(w));
            return None;
        }));
        s.set("dismiss", def("dismiss", "self, result=", {None}, [](var self, var result) -> var {
            AppState* A = app_state();
            Node* n = node_of(self);
            if (A->stack.empty() || A->stack.back().ref() != self.ref()) raise(E_RuntimeError, "textual: dismiss() of a screen that is not the active one");
            var cb = n->dismiss_cb;
            pop_screen();
            if (!cb.is_none() && !cb.is_unbound()) {
                int ar = arity(cb);
                finish(ar == 0 ? call0(cb) : call1(cb, result));
            }
            return None;
        }));
        s.set("action_dismiss", def("action_dismiss", "self, result=", {None}, [](var self, var result) -> var { return self.call("dismiss", result); }));
        s.set("action_focus_next", def("action_focus_next", "self", {}, [](var) -> var { focus_move(1); return None; }));
        s.set("action_focus_previous", def("action_focus_previous", "self", {}, [](var) -> var { focus_move(-1); return None; }));
        g_cls[C_Screen] = s.make({C(C_DOMNode)});
        CB m("ModalScreen", "textual.screen");
        m.set("_tui_modal", var(true));
        g_cls[C_ModalScreen] = m.make({C(C_Screen)});
    }
    // ---- widgets
    auto widget = [](int slot, const char* name, const char* module, WT kind, std::vector<var> bases, std::function<void(CB&)> more = nullptr) {
        CB c(name, module);
        c.set("_tui_kind", var((int64_t)kind));
        if (more) more(c);
        g_cls[slot] = c.make(bases);
    };
    widget(C_Static, "Static", "textual.widgets", W_STATIC, {C(C_Widget)}, [](CB& c) {
        c.set("__init__", def("__init__", "self, content=, *, expand=, shrink=, markup=, name=, id=, classes=, disabled=",
                              {""_s, var(false), var(false), var(true), None, None, None, var(false)},
                              [](var self, var content, var expand, var shrink, var markup, var name, var id, var classes, var disabled) -> var {
            (void)expand, (void)shrink, (void)disabled;
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->markup = markup.truthy();
            n->content = content;
            return None;
        }));
        c.set("update", def("update", "self, content=", {""_s}, [](var self, var content) -> var {
            static_update(node_of(self), content);
            return None;
        }));
        c.set("renderable", prop(def("renderable", "self", {}, [](var self) -> var { return node_of(self)->content; })));
        c.set("content", prop(def("content", "self", {}, [](var self) -> var { return node_of(self)->content; })));
        c.set("render", def("render", "self", {}, [](var self) -> var { return node_of(self)->content; }));
    });
    widget(C_Label, "Label", "textual.widgets", W_STATIC, {C(C_Static)});
    widget(C_SearchIcon, "SearchIcon", "textual.command", W_STATIC, {C(C_Static)}, [](CB& c) { c.set("_tui_inherit_css", var(false)); });
    widget(C_Input, "Input", "textual.widgets", W_INPUT, {C(C_Widget)}, [](CB& c) {
        c.set("can_focus", var(true));
        c.set("Changed", C(C_InputChanged));
        c.set("Submitted", C(C_InputSubmitted));
        c.set("BINDINGS", new_list({binding("left", "cursor_left", "Move cursor left"), binding("right", "cursor_right", "Move cursor right"),
                                    binding("backspace", "delete_left", "Delete character left"), binding("home,ctrl+a", "home", "Go to start"),
                                    binding("end,ctrl+e", "end", "Go to end"), binding("delete,ctrl+d", "delete_right", "Delete character right"),
                                    binding("enter", "submit", "Submit"), binding("ctrl+u", "delete_left_all", "Delete all to the left"),
                                    binding("ctrl+k", "delete_right_all", "Delete all to the right"), binding("ctrl+w", "delete_left_word", "Delete left to start of word")}));
        c.set("__init__", def("__init__", "self, value=, placeholder=, highlighter=, password=, *, restrict=, type=, max_length=, suggester=, validators=, validate_on=, valid_empty=, select_on_focus=, name=, id=, classes=, disabled=, tooltip=, compact=",
                              {None, ""_s, None, var(false), None, "text"_s, var(0), None, None, None, var(false), var(true), None, None, None, var(false), None, var(false)},
                              [](var self, var value, var placeholder, var hl, var pw, var restrict, var type_, var maxl, var sugg, var vals, var von, var vempty, var sof,
                                 var name, var id, var classes, var disabled, var tip, var compact) -> var {
            (void)hl, (void)pw, (void)restrict, (void)type_, (void)maxl, (void)sugg, (void)vals, (void)von, (void)vempty, (void)disabled, (void)tip, (void)compact;
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->value = is_str(value) ? u32(str_utf8(value)) : U"";
            n->cursor = (int)n->value.size();
            n->placeholder = u32(str_(placeholder));
            n->select_on_focus = sof.truthy();
            return None;
        }));
        c.set("value", prop(def("value", "self", {}, [](var self) -> var { return new_str(utf8(node_of(self)->value)); }),
                            def("value", "self, v", {}, [](var self, var v) -> var {
            Node* n = node_of(self);
            input_set(n, u32(str_(v)));
            n->cursor = (int)n->value.size();
            return None;
        })));
        c.set("placeholder", prop(def("placeholder", "self", {}, [](var self) -> var { return new_str(utf8(node_of(self)->placeholder)); }),
                                  def("placeholder", "self, v", {}, [](var self, var v) -> var { node_of(self)->placeholder = u32(str_(v)); request_redraw(); return None; })));
        c.set("cursor_position", prop(def("cursor_position", "self", {}, [](var self) -> var { return var((int64_t)node_of(self)->cursor); }),
                                      def("cursor_position", "self, v", {}, [](var self, var v) -> var {
            Node* n = node_of(self);
            n->cursor = std::max(0, std::min((int)v.as_int64(), (int)n->value.size()));
            n->sel = -1;
            return None;
        })));
        c.set("action_cursor_left", def("action_cursor_left", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            if (n->sel >= 0) n->cursor = std::min(n->sel, n->cursor), n->sel = -1;
            else if (n->cursor > 0) n->cursor--;
            request_redraw();
            return None;
        }));
        c.set("action_cursor_right", def("action_cursor_right", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            if (n->sel >= 0) n->cursor = std::max(n->sel, n->cursor), n->sel = -1;
            else if (n->cursor < (int)n->value.size()) n->cursor++;
            request_redraw();
            return None;
        }));
        c.set("action_home", def("action_home", "self", {}, [](var self) -> var { Node* n = node_of(self); n->cursor = 0; n->sel = -1; request_redraw(); return None; }));
        c.set("action_end", def("action_end", "self", {}, [](var self) -> var { Node* n = node_of(self); n->cursor = (int)n->value.size(); n->sel = -1; request_redraw(); return None; }));
        c.set("action_delete_left", def("action_delete_left", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            if (n->sel >= 0) { input_delete_selection(n); return None; }
            if (n->cursor > 0) {
                std::u32string v = n->value.substr(0, n->cursor - 1) + n->value.substr(n->cursor);
                n->cursor--;
                input_set(n, v);
            }
            return None;
        }));
        c.set("action_delete_right", def("action_delete_right", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            if (n->sel >= 0) { input_delete_selection(n); return None; }
            if (n->cursor < (int)n->value.size()) input_set(n, n->value.substr(0, n->cursor) + n->value.substr(n->cursor + 1));
            return None;
        }));
        c.set("action_delete_left_all", def("action_delete_left_all", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            std::u32string v = n->value.substr(n->cursor);
            n->cursor = 0;
            input_set(n, v);
            return None;
        }));
        c.set("action_delete_right_all", def("action_delete_right_all", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            input_set(n, n->value.substr(0, n->cursor));
            return None;
        }));
        c.set("action_delete_left_word", def("action_delete_left_word", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            int i = n->cursor;
            while (i > 0 && n->value[i - 1] == U' ') i--;
            while (i > 0 && n->value[i - 1] != U' ') i--;
            std::u32string v = n->value.substr(0, i) + n->value.substr(n->cursor);
            n->cursor = i;
            input_set(n, v);
            return None;
        }));
        c.set("action_submit", def("action_submit", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            post(n, make_msg(C_InputSubmitted, {{"input", self}, {"value", new_str(utf8(n->value))}, {"validation_result", None}, {"_control", self}}));
            return None;
        }));
        c.set("insert_text_at_cursor", def("insert_text_at_cursor", "self, text", {}, [](var self, var t) -> var { input_insert(node_of(self), u32(str_(t))); return None; }));
        c.set("clear", def("clear", "self", {}, [](var self) -> var { input_set(node_of(self), U""); node_of(self)->cursor = 0; return None; }));
    });
    // DataTable
    {
        CB rk("RowKey", "textual.widgets.data_table");
        rk.set("__init__", def("__init__", "self, value=", {None}, [](var self, var v) -> var { self.setattr("value", v); return None; }));
        rk.set("__eq__", def("__eq__", "self, other", {}, [](var self, var o) -> var {
            var v = self.getattr("value");
            if (is_str(o) || o.is_none()) return var(eq_(v, o));
            var ov = getattr(o, "value"_s, var());
            if (ov.is_unbound()) return var(false);
            return var(eq_(v, ov) && !v.is_none());
        }));
        rk.set("__hash__", def("__hash__", "self", {}, [](var self) -> var { return hash(self.getattr("value")); }));
        rk.set("__repr__", def("__repr__", "self", {}, [](var self) -> var { return new_str("<RowKey " + repr_(self.getattr("value")) + ">"); }));
        g_cls[C_RowKey] = rk.make({});
        CB ck("ColumnKey", "textual.widgets.data_table");
        ck.ns = new_dict();
        ck.set("__module__", "textual.widgets.data_table"_s);
        g_cls[C_ColumnKey] = ck.make({C(C_RowKey)});
        CB cell("CellKey", "textual.widgets.data_table");
        cell.set("__init__", def("__init__", "self, row_key, column_key", {}, [](var self, var r, var c) -> var {
            self.setattr("row_key", r);
            self.setattr("column_key", c);
            return None;
        }));
        cell.set("__iter__", def("__iter__", "self", {}, [](var self) -> var { return iter(new_tuple({self.getattr("row_key"), self.getattr("column_key")})); }));
        g_cls[C_CellKey] = cell.make({});
    }
    widget(C_DataTable, "DataTable", "textual.widgets", W_TABLE, {C(C_Widget)}, [](CB& c) {
        c.set("can_focus", var(true));
        c.set("RowHighlighted", C(C_RowHighlighted));
        c.set("RowSelected", C(C_RowSelected));
        c.set("BINDINGS", new_list({binding("enter", "select_cursor", "Select"), binding("up", "cursor_up", "Cursor up"),
                                    binding("down", "cursor_down", "Cursor down"), binding("right", "cursor_right", "Cursor right"),
                                    binding("left", "cursor_left", "Cursor left"), binding("pageup", "page_up", "Page up"),
                                    binding("pagedown", "page_down", "Page down"), binding("ctrl+home", "scroll_top", "Top"),
                                    binding("ctrl+end", "scroll_bottom", "Bottom"), binding("home", "scroll_home", "Home"),
                                    binding("end", "scroll_end", "End")}));
        c.set("__init__", def("__init__", "self, *, show_header=, show_row_labels=, fixed_rows=, fixed_columns=, zebra_stripes=, header_height=, show_cursor=, cursor_foreground_priority=, cursor_background_priority=, cursor_type=, cell_padding=, name=, id=, classes=, disabled=",
                              {var(true), var(true), var(0), var(0), var(false), var(1), var(true), "css"_s, "renderable"_s, "cell"_s, var(1), None, None, None, var(false)},
                              [](var self, var sh, var srl, var fr, var fc, var zebra, var hh, var sc, var cfp, var cbp, var ct, var cp, var name, var id, var classes, var dis) -> var {
            (void)srl, (void)fr, (void)fc, (void)hh, (void)sc, (void)cbp, (void)cp, (void)dis;
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->show_header = sh.truthy();
            n->zebra = zebra.truthy();
            n->cursor_type = str_(ct);
            n->renderable_fg = str_(cfp) == "renderable";
            return None;
        }));
        c.set("add_column", def("add_column", "self, label, *, width=, key=, default=", {None, None, None}, [](var self, var label, var width, var key, var dflt) -> var {
            (void)dflt;
            Node* n = node_of(self);
            ColumnD col;
            col.label = is_str(label) ? make_text(markup_text(str_utf8(label))) : label;
            col.key = g_cls[C_ColumnKey](key);
            col.width = width.is_none() ? -1 : (int)width.as_int64();
            col.content_w = cell_width_of(col.label);
            for (auto& r : n->rows) r.cells.push_back(None);
            n->cols.push_back(col);
            request_redraw();
            return col.key;
        }));
        c.set("add_columns", def("add_columns", "self, *labels", {}, [](var self, var labels) -> var {
            std::vector<var> keys;
            for (var l : each(labels)) {
                if (is_tuple(l)) keys.push_back(self.call("add_column", tuple_items(l)[0], kw("key", tuple_items(l)[1])));
                else keys.push_back(self.call("add_column", l));
            }
            return new_list(keys);
        }));
        c.set("add_row", def("add_row", "self, *cells, height=, key=, label=", {var(1), None, None}, [](var self, var cells, var height, var key, var label) -> var {
            (void)height, (void)label;
            Node* n = node_of(self);
            std::vector<var> cs = collect(cells);
            if (cs.size() > n->cols.size()) raise(E_ValueError, "More values provided than there are columns.");
            RowD r;
            r.key = g_cls[C_RowKey](key);
            for (auto& rr : n->rows)
                if (!key.is_none() && eq_(rr.key.getattr("value"), key)) raise(make_exc_args(T(tv(E_KeyError)), {new_str("The row key " + repr_(key) + " already exists.")}));
            cs.resize(n->cols.size(), None);
            for (size_t i = 0; i < cs.size(); i++)
                if (n->cols[i].width < 0) n->cols[i].content_w = std::max(n->cols[i].content_w, cell_width_of(cs[i]));
            r.cells = cs;
            n->rows.push_back(r);
            if (n->rows.size() == 1 && !n->cols.empty()) table_highlight(n);   // (a cursor where there was none)
            request_redraw();
            return r.key;
        }));
        c.set("clear", def("clear", "self, columns=", {var(false)}, [](var self, var columns) -> var {
            Node* n = node_of(self);
            n->rows.clear();
            if (columns.truthy()) n->cols.clear();
            else
                for (auto& col : n->cols) col.content_w = cell_width_of(col.label);
            n->cur_row = 0, n->cur_col = 0, n->scroll_y = 0;
            request_redraw();
            return self;
        }));
        c.set("move_cursor", def("move_cursor", "self, *, row=, column=, animate=, scroll=", {None, None, var(false), var(true)},
                                 [](var self, var row, var column, var, var) -> var {
            Node* n = node_of(self);
            if (!column.is_none()) n->cur_col = (int)column.as_int64();
            table_set_cursor(n, row.is_none() ? n->cur_row : (int)row.as_int64());
            return None;
        }));
        c.set("cursor_row", prop(def("cursor_row", "self", {}, [](var self) -> var { return var((int64_t)node_of(self)->cur_row); })));
        c.set("cursor_coordinate", prop(def("cursor_coordinate", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            return new_tuple({var((int64_t)n->cur_row), var((int64_t)n->cur_col)});
        })));
        c.set("row_count", prop(def("row_count", "self", {}, [](var self) -> var { return var((int64_t)node_of(self)->rows.size()); })));
        c.set("coordinate_to_cell_key", def("coordinate_to_cell_key", "self, coordinate", {}, [](var self, var coord) -> var {
            Node* n = node_of(self);
            std::vector<var> rc = collect(coord);
            int r = (int)rc.at(0).as_int64(), col = (int)rc.at(1).as_int64();
            if (r < 0 || r >= (int)n->rows.size() || col < 0 || col >= (int)n->cols.size())
                raise(make_exc_args(T(tv(E_KeyError)), {new_str("No cell exists at " + repr_(coord) + ".")}));
            return g_cls[C_CellKey](n->rows[r].key, n->cols[col].key);
        }));
        c.set("get_row_at", def("get_row_at", "self, row_index", {}, [](var self, var i) -> var { return new_list(node_of(self)->rows.at((size_t)i.as_int64()).cells); }));
        c.set("get_cell_at", def("get_cell_at", "self, coordinate", {}, [](var self, var coord) -> var {
            std::vector<var> rc = collect(coord);
            return node_of(self)->rows.at((size_t)rc.at(0).as_int64()).cells.at((size_t)rc.at(1).as_int64());
        }));
        c.set("action_cursor_up", def("action_cursor_up", "self", {}, [](var self) -> var { Node* n = node_of(self); table_set_cursor(n, n->cur_row - 1); return None; }));
        c.set("action_cursor_down", def("action_cursor_down", "self", {}, [](var self) -> var { Node* n = node_of(self); table_set_cursor(n, n->cur_row + 1); return None; }));
        c.set("action_cursor_left", def("action_cursor_left", "self", {}, [](var) -> var { return None; }));
        c.set("action_cursor_right", def("action_cursor_right", "self", {}, [](var) -> var { return None; }));
        c.set("action_page_up", def("action_page_up", "self", {}, [](var self) -> var { Node* n = node_of(self); table_set_cursor(n, n->cur_row - std::max(1, n->ch - 1)); return None; }));
        c.set("action_page_down", def("action_page_down", "self", {}, [](var self) -> var { Node* n = node_of(self); table_set_cursor(n, n->cur_row + std::max(1, n->ch - 1)); return None; }));
        c.set("action_scroll_top", def("action_scroll_top", "self", {}, [](var self) -> var { table_set_cursor(node_of(self), 0); return None; }));
        c.set("action_scroll_bottom", def("action_scroll_bottom", "self", {}, [](var self) -> var { table_set_cursor(node_of(self), 1 << 30); return None; }));
        c.set("action_scroll_home", def("action_scroll_home", "self", {}, [](var self) -> var { table_set_cursor(node_of(self), 0); return None; }));
        c.set("action_scroll_end", def("action_scroll_end", "self", {}, [](var self) -> var { table_set_cursor(node_of(self), 1 << 30); return None; }));
        c.set("action_select_cursor", def("action_select_cursor", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            if (n->cur_row < (int)n->rows.size())
                post(n, make_msg(C_RowSelected, {{"data_table", self}, {"cursor_row", var((int64_t)n->cur_row)}, {"row_key", n->rows[n->cur_row].key}, {"_control", self}}));
            return None;
        }));
    });
    widget(C_Header, "Header", "textual.widgets", W_HEADER, {C(C_Widget)}, [](CB& c) {
        c.set("__init__", def("__init__", "self, show_clock=, *, name=, id=, classes=, icon=, time_format=", {var(false), None, None, None, None, None},
                              [](var self, var, var name, var id, var classes, var, var) -> var {
            init_node_args(ensure_node(self), id, classes, name);
            return None;
        }));
    });
    widget(C_Footer, "Footer", "textual.widgets", W_FOOTER, {C(C_Widget)}, [](CB& c) {
        c.set("__init__", def("__init__", "self, *children, name=, id=, classes=, disabled=, show_command_palette=, compact=",
                              {None, None, None, var(false), var(true), var(false)}, [](var self, var, var name, var id, var classes, var, var, var) -> var {
            init_node_args(ensure_node(self), id, classes, name);
            return None;
        }));
    });
    widget(C_Loading, "LoadingIndicator", "textual.widgets", W_LOADING, {C(C_Widget)}, [](CB& c) { c.set("__init__", def_widget_init(W_LOADING)); });
    widget(C_RichLog, "RichLog", "textual.widgets", W_LOG, {C(C_Widget)}, [](CB& c) {
        c.set("can_focus", var(true));
        c.set("BINDINGS", new_list({binding("up", "scroll_up", "Scroll Up"), binding("down", "scroll_down", "Scroll Down"),
                                    binding("pageup", "page_up", "Page Up"), binding("pagedown", "page_down", "Page Down"),
                                    binding("home", "scroll_home", "Scroll Home"), binding("end", "scroll_end", "Scroll End")}));
        c.set("__init__", def("__init__", "self, *, max_lines=, min_width=, wrap=, highlight=, markup=, auto_scroll=, name=, id=, classes=, disabled=",
                              {None, var(78), var(false), var(false), var(false), var(true), None, None, None, var(false)},
                              [](var self, var ml, var mw, var wrap, var hl, var markup, var as, var name, var id, var classes, var dis) -> var {
            (void)ml, (void)mw, (void)dis;
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->wrap = wrap.truthy();
            n->highlight = hl.truthy();
            n->markup = markup.truthy();
            n->auto_scroll = as.truthy();
            return None;
        }));
        c.set("write", def("write", "self, content, width=, expand=, shrink=, scroll_end=, animate=", {None, var(false), var(true), None, var(false)},
                           [](var self, var content, var, var, var, var, var) -> var {
            Node* n = node_of(self);
            var r = content;
            if (is_str(content) && n->markup) r = make_text(markup_text(str_utf8(content)));
            n->log.push_back({r, {}});
            request_redraw();
            return self;
        }));
        c.set("clear", def("clear", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            n->log.clear();
            n->scroll_y = 0;
            request_redraw();
            return self;
        }));
        c.set("lines", prop(def("lines", "self", {}, [](var self) -> var {
            std::vector<var> xs;
            for (auto& [r, l] : node_of(self)->log) xs.push_back(r);
            return new_list(xs);
        })));
        c.set("action_scroll_up", def("action_scroll_up", "self", {}, [](var self) -> var { Node* n = node_of(self); n->auto_scroll = false; n->scroll_y = std::max(0, n->scroll_y - 1); request_redraw(); return None; }));
        c.set("action_scroll_down", def("action_scroll_down", "self", {}, [](var self) -> var {
            Node* n = node_of(self);
            n->scroll_y++;
            if (n->scroll_y >= n->virtual_h - n->ch) n->auto_scroll = true;
            request_redraw();
            return None;
        }));
        c.set("action_page_up", def("action_page_up", "self", {}, [](var self) -> var { Node* n = node_of(self); n->auto_scroll = false; n->scroll_y = std::max(0, n->scroll_y - n->ch); request_redraw(); return None; }));
        c.set("action_page_down", def("action_page_down", "self", {}, [](var self) -> var { Node* n = node_of(self); n->scroll_y += n->ch; request_redraw(); return None; }));
        c.set("action_scroll_home", def("action_scroll_home", "self", {}, [](var self) -> var { Node* n = node_of(self); n->auto_scroll = false; n->scroll_y = 0; request_redraw(); return None; }));
        c.set("action_scroll_end", def("action_scroll_end", "self", {}, [](var self) -> var { Node* n = node_of(self); n->auto_scroll = true; request_redraw(); return None; }));
    });
    // tabs
    widget(C_Tab, "Tab", "textual.widgets", W_TAB, {C(C_Widget)});
    widget(C_Underline, "Underline", "textual.widgets._tabs", W_WIDGET, {C(C_Widget)});
    widget(C_Tabs, "Tabs", "textual.widgets", W_TABS, {C(C_Widget)}, [](CB& c) {
        c.set("can_focus", var(true));
        c.set("BINDINGS", new_list({binding("left", "previous_tab", "Previous tab"), binding("right", "next_tab", "Next tab")}));
        auto step = [](var self, int d) {
            Node* n = node_of(self);
            Node* tb = tabbed_of_tabs(n);
            if (!tb) return;
            std::vector<Node*> ts = kids(n);
            if (ts.empty()) return;
            int i = 0;
            for (size_t k = 0; k < ts.size(); k++)
                if (has_class(ts[k], "-active")) i = (int)k;
            i = ((i + d) % (int)ts.size() + (int)ts.size()) % (int)ts.size();
            set_tab_active(tb, ts[i]->id.substr(std::string("--content-tab-").size()), true);
        };
        c.set("action_previous_tab", def("action_previous_tab", "self", {}, [step](var self) -> var { step(self, -1); return None; }));
        c.set("action_next_tab", def("action_next_tab", "self", {}, [step](var self) -> var { step(self, 1); return None; }));
    });
    widget(C_ContentTabs, "ContentTabs", "textual.widgets._tabbed_content", W_TABS, {C(C_Tabs)});
    widget(C_Switcher, "ContentSwitcher", "textual.widgets", W_SWITCHER, {C(C_Widget)});
    widget(C_TabPane, "TabPane", "textual.widgets", W_PANE, {C(C_Widget)}, [](CB& c) {
        c.set("__init__", def("__init__", "self, title, *children, name=, id=, classes=, disabled=", {None, None, None, var(false)},
                              [](var self, var title, var children, var name, var id, var classes, var) -> var {
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->title = str_(title);
            for (var ch : each(children)) n->pending.push_back(ch);
            return None;
        }));
    });
    widget(C_Tabbed, "TabbedContent", "textual.widgets", W_TABBED, {C(C_Widget)}, [](CB& c) {
        c.set("TabActivated", C(C_TabActivated));
        c.set("__init__", def("__init__", "self, *titles, initial=, name=, id=, classes=, disabled=", {""_s, None, None, None, var(false)},
                              [](var self, var titles, var initial, var name, var id, var classes, var) -> var {
            (void)titles;
            Node* n = ensure_node(self);
            init_node_args(n, id, classes, name);
            n->active = str_(initial);
            return None;
        }));
        c.set("active", prop(def("active", "self", {}, [](var self) -> var { return new_str(node_of(self)->active); }),
                             def("active", "self, v", {}, [](var self, var v) -> var {
            Node* n = node_of(self);
            std::string id = str_(v);
            bool found = false;
            for (Node* k : kids(n))
                if (k->wt == W_SWITCHER)
                    for (Node* p : kids(k))
                        if (p->id == id) found = true;
            if (!found && n->mounted) raise(E_ValueError, "No Tab with id '" + id + "'");
            if (n->mounted) set_tab_active(n, id, true);
            else n->active = id;
            return None;
        })));
    });
    // containers
    widget(C_Container, "Container", "textual.containers", W_CONTAINER, {C(C_Widget)});
    widget(C_Vertical, "Vertical", "textual.containers", W_CONTAINER, {C(C_Widget)});
    widget(C_Horizontal, "Horizontal", "textual.containers", W_CONTAINER, {C(C_Widget)});
    widget(C_Scrollable, "ScrollableContainer", "textual.containers", W_SCROLL, {C(C_Widget)}, [](CB& c) {
        c.set("can_focus", var(true));
        c.set("BINDINGS", new_list({binding("up", "scroll_up", "Scroll Up"), binding("down", "scroll_down", "Scroll Down"),
                                    binding("home", "scroll_home", "Scroll Home"), binding("end", "scroll_end", "Scroll End"),
                                    binding("pageup", "page_up", "Page Up"), binding("pagedown", "page_down", "Page Down")}));
        c.set("action_scroll_up", def("action_scroll_up", "self", {}, [](var self) -> var { Node* n = node_of(self); n->scroll_y = std::max(0, n->scroll_y - 1); request_redraw(); return None; }));
        c.set("action_scroll_down", def("action_scroll_down", "self", {}, [](var self) -> var { node_of(self)->scroll_y++; request_redraw(); return None; }));
        c.set("action_page_up", def("action_page_up", "self", {}, [](var self) -> var { Node* n = node_of(self); n->scroll_y = std::max(0, n->scroll_y - n->ch); request_redraw(); return None; }));
        c.set("action_page_down", def("action_page_down", "self", {}, [](var self) -> var { Node* n = node_of(self); n->scroll_y += n->ch; request_redraw(); return None; }));
        c.set("action_scroll_home", def("action_scroll_home", "self", {}, [](var self) -> var { node_of(self)->scroll_y = 0; request_redraw(); return None; }));
        c.set("action_scroll_end", def("action_scroll_end", "self", {}, [](var self) -> var { node_of(self)->scroll_y = 1 << 30; request_redraw(); return None; }));
    });
    widget(C_VScroll, "VerticalScroll", "textual.containers", W_SCROLL, {C(C_Scrollable)});
    widget(C_CmdList, "CommandList", "textual.command", W_CMDLIST, {C(C_Widget)});

    // ---- textual.theme
    {
        CB t("Theme", "textual.theme");
        t.set("__init__", def("__init__", "self, name, primary, secondary=, warning=, error=, success=, accent=, foreground=, background=, surface=, panel=, boost=, dark=, luminosity_spread=, text_alpha=, variables=",
                              {None, None, None, None, None, None, None, None, None, None, var(true), var(0.15), var(0.95), None},
                              [](var self, var name, var primary, var secondary, var warning, var error, var success, var accent, var fg, var bg, var surface,
                                 var panel, var boost, var dark, var spread, var alpha, var variables) -> var {
            self.setattr("name", name);
            self.setattr("primary", primary);
            self.setattr("secondary", secondary);
            self.setattr("warning", warning);
            self.setattr("error", error);
            self.setattr("success", success);
            self.setattr("accent", accent);
            self.setattr("foreground", fg);
            self.setattr("background", bg);
            self.setattr("surface", surface);
            self.setattr("panel", panel);
            self.setattr("boost", boost);
            self.setattr("dark", dark);
            self.setattr("luminosity_spread", spread);
            self.setattr("text_alpha", alpha);
            self.setattr("variables", variables.is_none() ? new_dict_obj() : variables);
            return None;
        }));
        g_cls[C_Theme] = t.make({});
    }
    // ---- textual.command
    {
        CB h("Hit", "textual.command");
        h.set("__init__", def("__init__", "self, score, match_display, command, text=, help=", {None, None},
                              [](var self, var score, var md, var cmd, var text, var help) -> var {
            self.setattr("score", score);
            self.setattr("match_display", md);
            self.setattr("command", cmd);
            if (text.is_none()) {
                TextData td;
                text = text_data(md, td) ? new_str(utf8(td.plain)) : (is_str(md) ? md : new_str(str_(md)));
            }
            self.setattr("text", text);
            self.setattr("help", help);
            self.setattr("prompt", md);
            return None;
        }));
        g_cls[C_Hit] = h.make({});
        CB dh("DiscoveryHit", "textual.command");
        dh.set("__init__", def("__init__", "self, display, command, text=, help=", {None, None}, [](var self, var disp, var cmd, var text, var help) -> var {
            self.setattr("score", var(0.0));
            self.setattr("match_display", disp);
            self.setattr("command", cmd);
            self.setattr("text", text.is_none() ? (is_str(disp) ? disp : new_str(str_(disp))) : text);
            self.setattr("help", help);
            self.setattr("prompt", disp);
            return None;
        }));
        g_cls[C_DiscoveryHit] = dh.make({});
        CB m("Matcher", "textual.fuzzy");
        m.set("__init__", def("__init__", "self, query, *, match_style=, case_sensitive=", {None, var(false)}, [](var self, var q, var ms, var cs) -> var {
            self.setattr("query", q);
            self.setattr("match_style", ms);
            self.setattr("case_sensitive", cs);
            return None;
        }));
        m.set("match", def("match", "self, candidate", {}, [](var self, var cand) -> var {
            double s;
            std::vector<int> o;
            fuzzy(u32(str_(self.getattr("query"))), u32(str_(cand)), s, o);
            return var(s);
        }));
        m.set("highlight", def("highlight", "self, candidate", {}, [](var self, var cand) -> var {
            double s;
            std::vector<int> o;
            TextData t = markup_text(str_(cand));
            fuzzy(u32(str_(self.getattr("query"))), u32(str_(cand)), s, o);
            if (s > 0) {
                Style hl = parse_style("bold underline");
                for (int off : o)
                    if (off < (int)t.plain.size() && t.plain[off] != U' ') t.spans.push_back({off, off + 1, hl});
            }
            return make_text(t);
        }));
        g_cls[C_Matcher] = m.make({});
        CB p("Provider", "textual.command");
        p.set("__init__", def("__init__", "self, screen, match_style=", {None}, [](var self, var screen, var ms) -> var {
            self.setattr("_Provider__screen", screen);
            self.setattr("_match_style", ms);
            return None;
        }));
        p.set("app", prop(def("app", "self", {}, [](var) -> var { return app_state()->app; })));
        p.set("screen", prop(def("screen", "self", {}, [](var self) -> var { return self.getattr("_Provider__screen"); })));
        p.set("focused", prop(def("focused", "self", {}, [](var self) -> var {
            Node* s = node_of(self.getattr("_Provider__screen"), false);
            return s ? obj(s->focused) : var(None);
        })));
        p.set("matcher", def("matcher", "self, user_input, case_sensitive=", {var(false)}, [](var self, var q, var cs) -> var {
            return g_cls[C_Matcher](q, kw("match_style", self.getattr("_match_style")), kw("case_sensitive", cs));
        }));
        p.set("discover", def_async_generator("discover", "self", {}, [](yielder& y, var self) -> var { (void)y, (void)self; return None; }));
        p.set("startup", def_coroutine("startup", "self", {}, [](yielder& y, var self) -> var { (void)y, (void)self; return None; }));
        p.set("shutdown", def_coroutine("shutdown", "self", {}, [](yielder& y, var self) -> var { (void)y, (void)self; return None; }));
        g_cls[C_Provider] = p.make({});
        // the system commands (textual's SystemCommandsProvider): what this app can do of them
        CB sc("SystemCommandsProvider", "textual.system_commands");
        auto commands = [](var app) {
            // Quit; Theme (the next registered theme -- textual opens a picker); Screenshot (the screen as text
            // -- textual writes an SVG). Keys and Maximize (textual's help panel, maximized widgets) are not here.
            var theme = natf("theme", [app](const CallArgs&) -> var {
                std::vector<var> names = {"textual-dark"_s};
                for (var k : each(app.getattr("_tui_themes"))) names.push_back(k);
                std::string cur = str_(app.getattr("_tui_theme"));
                size_t i = 0;
                for (size_t k = 0; k < names.size(); k++)
                    if (str_(names[k]) == cur) i = k;
                var next = names[(i + 1) % names.size()];
                app.setattr("theme", next);
                app.call("notify", new_str("Theme: " + str_(next)));
                return None;
            });
            var shot = natf("screenshot", [app](const CallArgs&) -> var {
                render_frame();
                std::string name = str_(getattr(app, "title"_s, "screenshot"_s)) + "_" + std::to_string((long long)time(nullptr)) + ".txt";
                FILE* f = fopen(name.c_str(), "wb");
                if (f) {
                    std::string t = frame_text();
                    fwrite(t.data(), 1, t.size(), f);
                    fclose(f);
                }
                app.call("notify", new_str("Saved screenshot to " + name));
                return None;
            });
            return std::vector<std::tuple<const char*, const char*, var>>{
                {"Quit", "Quit the application as soon as possible", app.getattr("action_quit")},
                {"Screenshot", "Save a text 'screenshot' of the current screen", shot},
                {"Theme", "Change the current theme", theme}};
        };
        sc.set("discover", def_async_generator("discover", "self", {}, [commands](yielder& y, var self) -> var {
            for (auto& [name, help, cb] : commands(app_state()->app)) y(g_cls[C_DiscoveryHit](new_str(name), cb, kw("help", new_str(help))));
            (void)self;
            return None;
        }));
        sc.set("search", def_async_generator("search", "self, query", {}, [commands](yielder& y, var self, var query) -> var {
            var m = self.call("matcher", query);
            for (auto& [name, help, cb] : commands(app_state()->app)) {
                var s = m.call("match", new_str(name));
                if (s.as_double() > 0) y(g_cls[C_Hit](s, m.call("highlight", new_str(name)), cb, kw("help", new_str(help))));
            }
            return None;
        }));
        g_cls[C_SystemCommands] = sc.make({C(C_Provider)});
        // the palette itself: a modal screen with a search input and the hits under it
        CB pal("CommandPalette", "textual.command");
        pal.set("_tui_inherit_css", var(false));
        pal.set("BINDINGS", new_list({binding("escape", "escape", "Exit the command palette"), binding("down", "cursor_down", "Next"),
                                      binding("up", "cursor_up", "Previous"), binding("pagedown", "page_down", "Next page"),
                                      binding("pageup", "page_up", "Previous page")}));
        pal.set("compose", def("compose", "self", {}, [](var self) -> var {
            (void)self;
            var outer = g_cls[C_Vertical](kw("id", "--container"_s));
            var row = g_cls[C_Horizontal](kw("id", "--input"_s));
            var icon = g_cls[C_SearchIcon]("🔎"_s);
            var input = g_cls[C_Input](kw("placeholder", "Search for commands…"_s), kw("select_on_focus", var(false)));
            var list = g_cls[C_CmdList](kw("id", "--results"_s));
            node_of(row)->pending = {icon, input};
            node_of(outer)->pending = {row, list};
            return new_list({outer});
        }));
        pal.set("on_mount", def("on_mount", "self", {}, [](var self) -> var {
            AppState* A = app_state();
            std::vector<var> ps;
            var cmds = getattr(A->app, "COMMANDS"_s, None);
            if (!cmds.is_none())
                for (var cls : each(cmds)) {
                    var p = cls(self);
                    finish(p.call("startup"));
                    ps.push_back(p);
                }
            self.setattr("_tui_providers", new_list(ps));
            Node* n = node_of(self);
            walk(n, [&](Node* k) { if (k->wt == W_INPUT) set_focus(n, k); });
            palette_search(n);
            return None;
        }));
        pal.set("on_input_changed", def("on_input_changed", "self, event", {}, [](var self, var) -> var {
            palette_search(node_of(self));
            return None;
        }));
        auto list_of = [](var self) {
            Node* l = nullptr;
            walk(node_of(self), [&](Node* k) { if (k->wt == W_CMDLIST) l = k; });
            return l;
        };
        pal.set("on_input_submitted", def("on_input_submitted", "self, event", {}, [](var self, var) -> var { return self.call("action_select_hit"); }));
        pal.set("action_select_hit", def("action_select_hit", "self", {}, [list_of](var self) -> var {
            Node* l = list_of(self);
            if (!l || l->hits.empty()) return None;
            var hit = l->hits[std::max(0, std::min(l->hl, (int)l->hits.size() - 1))];
            self.call("dismiss");
            var cmd = hit.getattr("command");
            defer([cmd]() { guarded([&] { finish(call0(cmd)); }); });
            return None;
        }));
        pal.set("action_escape", def("action_escape", "self", {}, [](var self) -> var { return self.call("dismiss"); }));
        pal.set("action_cursor_down", def("action_cursor_down", "self", {}, [list_of](var self) -> var {
            Node* l = list_of(self);
            if (l && l->hl + 1 < (int)l->hits.size()) l->hl++;
            request_redraw();
            return None;
        }));
        pal.set("action_cursor_up", def("action_cursor_up", "self", {}, [list_of](var self) -> var {
            Node* l = list_of(self);
            if (l && l->hl > 0) l->hl--;
            request_redraw();
            return None;
        }));
        pal.set("action_page_down", pal.ns.getitem("action_cursor_down"_s));
        pal.set("action_page_up", pal.ns.getitem("action_cursor_up"_s));
        g_cls[C_Palette] = pal.make({C(C_ModalScreen)});
    }
    // ---- Pilot and run_test
    {
        CB p("Pilot", "textual.pilot");
        auto co = [](const char* name, std::function<var()> f) { return def_coroutine(name, "", {}, [f](yielder& y) -> var { (void)y; return f(); })(); };
        p.set("__init__", def("__init__", "self, app", {}, [](var self, var app) -> var { self.setattr("_app", app); return None; }));
        p.set("app", prop(def("app", "self", {}, [](var self) -> var { return self.getattr("_app"); })));
        p.set("pause", def("pause", "self, delay=", {None}, [co](var self, var delay) -> var {
            (void)self;
            return co("pause", [delay]() -> var {
                if (delay.is_none()) settle();
                else run_for(delay.as_double());
                return None;
            });
        }));
        p.set("wait_for_scheduled_animations", def("wait_for_scheduled_animations", "self", {}, [co](var) -> var { return co("wait", []() -> var { settle(); return None; }); }));
        p.set("wait_for_animation", def("wait_for_animation", "self", {}, [co](var) -> var { return co("wait", []() -> var { settle(); return None; }); }));
        p.set("press", def("press", "self, *keys", {}, [co](var self, var keys) -> var {
            (void)self;
            std::vector<var> ks = collect(keys);
            return co("press", [ks]() -> var {
                for (auto& k : ks) {
                    if (G->exiting) break;
                    std::string key = str_utf8(k);
                    char32_t ch = key_char(key);
                    if (key.find('+') != std::string::npos && key.size() > 1) ch = 0;
                    key_event(normalize_key(key), ch);
                    settle();
                }
                return None;
            });
        }));
        p.set("click", def("click", "self, widget=, offset=, shift=, meta=, control=, times=", {None, new_tuple({var(0), var(0)}), var(false), var(false), var(false), var(1)},
                           [co](var self, var widget, var offset, var, var, var, var) -> var {
            (void)self;
            return co("click", [widget, offset]() -> var {
                render_frame();
                std::vector<var> o = collect(offset);
                int x = (int)o.at(0).as_int64(), y = (int)o.at(1).as_int64();
                if (!widget.is_none()) {
                    Node* s = active_screen();
                    var w = is_str(widget) ? query_one(s, widget, None) : widget;
                    Node* n = node_of(w);
                    x += n->x, y += n->y;
                }
                mouse_click(x, y);
                settle();
                return var(true);
            });
        }));
        p.set("resize_terminal", def("resize_terminal", "self, width, height", {}, [co](var self, var w, var h) -> var {
            (void)self;
            return co("resize", [w, h]() -> var {
                G->W = (int)w.as_int64(), G->H = (int)h.as_int64();
                request_redraw();
                settle();
                return None;
            });
        }));
        p.set("exit", def("exit", "self, result", {}, [co](var self, var result) -> var {
            return co("exit", [self, result]() -> var {
                self.getattr("_app").call("exit", result);
                settle();
                return None;
            });
        }));
        g_cls[C_Pilot] = p.make({});
        CB r("_RunTest", "textual.app");
        r.set("__aenter__", def("__aenter__", "self", {}, [co](var self) -> var {
            return co("__aenter__", [self]() -> var {
                var app = self.getattr("_app");
                std::vector<var> sz = collect(self.getattr("_size"));
                start_app(app, true, (int)sz.at(0).as_int64(), (int)sz.at(1).as_int64());
                G->toasts_on = self.getattr("_notifications").truthy();
                Node* an = G->node;
                an->mounted = true;
                guarded([&] {
                    var dflt = app.getattr("_tui_make_default_screen")();
                    push_screen(dflt, None);
                    if (app.hasattr("on_mount")) finish(app.call("on_mount"));
                });
                settle();
                return g_cls[C_Pilot](app);
            });
        }));
        r.set("__aexit__", def("__aexit__", "self, *exc", {}, [co](var self, var exc) -> var {
            (void)exc;
            return co("__aexit__", [self]() -> var {
                if (!G) return var(false);
                settle();
                AppState* A = G;
                shutdown_app(true);
                var err = A->error;
                G = nullptr;   // (the state stays: a worker still finishing may look at it)
                if (!err.is_unbound() && !err.is_none()) raise(err);
                return var(false);
            });
        }));
        g_cls[C_RunTest] = r.make({});
    }
    // ---- App
    {
        CB a("App", "textual.app");
        a.set("_tui_kind", var((int64_t)W_APP));
        a.set("TITLE", None);
        a.set("SUB_TITLE", None);
        a.set("CSS", ""_s);
        a.set("CSS_PATH", None);
        a.set("ENABLE_COMMAND_PALETTE", var(true));
        a.set("NOTIFICATION_TIMEOUT", var(5));
        a.set("COMMANDS", call1(builtin("set"), new_list({C(C_SystemCommands)})));
        a.set("BINDINGS", new_list({binding("ctrl+q", "quit", "Quit", false, true)}));
        a.set("__init__", def("__init__", "self, driver_class=, css_path=, watch_css=, ansi_color=", {None, None, var(false), var(false)},
                              [](var self, var, var css_path, var, var) -> var {
            Node* n = ensure_node(self);
            n->mounted = false;
            var t = getattr(self, "TITLE"_s, None);
            self.setattr("title", t.is_none() ? new_str(type_of(self)->name) : t);
            var st = getattr(self, "SUB_TITLE"_s, None);
            self.setattr("sub_title", st.is_none() ? ""_s : st);
            if (!css_path.is_none()) self.setattr("CSS_PATH", css_path);
            self.setattr("return_value", None);
            self.setattr("return_code", None);
            self.setattr("_tui_themes", new_dict_obj());
            self.setattr("_tui_theme", "textual-dark"_s);
            return None;
        }));
        a.set("_tui_default_theme", prop(def("_tui_default_theme", "self", {}, [](var self) -> var {
            var name = self.getattr("_tui_theme");
            var themes = self.getattr("_tui_themes");
            var t = themes.call("get", name);
            if (!t.is_none()) return t;
            // textual-dark
            return g_cls[C_Theme]("textual-dark"_s, "#0178D4"_s, kw("secondary", "#004578"_s), kw("accent", "#ffa62b"_s), kw("warning", "#ffa62b"_s),
                                  kw("error", "#ba3c5b"_s), kw("success", "#4EBF71"_s), kw("foreground", "#e0e0e0"_s), kw("background", "#121212"_s),
                                  kw("surface", "#1e1e1e"_s), kw("panel", "#24292f"_s));
        })));
        a.set("_tui_make_default_screen", def("_tui_make_default_screen", "self", {}, [](var) -> var {
            CB d("Screen", "textual.screen");
            static var cls;
            if (cls.is_unbound()) cls = d.make({C(C_Screen)});
            return call0(cls);
        }));
        a.set("register_theme", def("register_theme", "self, theme", {}, [](var self, var theme) -> var {
            self.getattr("_tui_themes").setitem(theme.getattr("name"), theme);
            return None;
        }));
        a.set("theme", prop(def("theme", "self", {}, [](var self) -> var { return self.getattr("_tui_theme"); }),
                            def("theme", "self, name", {}, [](var self, var name) -> var {
            if (!self.getattr("_tui_themes").call("__contains__", name).truthy() && str_(name) != "textual-dark")
                raise(make_exc_args(T(tv(E_ValueError)), {new_str("Theme " + repr_(name) + " has not been registered.")}));
            self.setattr("_tui_theme", name);
            if (G && G->app.ref() == self.ref()) {
                G->vars = generate_theme(self.getattr("_tui_default_theme"));
                G->full = true;
                request_redraw();
            }
            return None;
        })));
        a.set("screen_stack", prop(def("screen_stack", "self", {}, [](var self) -> var {
            (void)self;
            return G ? new_list(G->stack) : new_list();
        })));
        a.set("focused", prop(def("focused", "self", {}, [](var) -> var {
            Node* s = active_screen();
            return s ? obj(s->focused) : var(None);
        })));
        a.set("is_running", prop(def("is_running", "self", {}, [](var self) -> var { return var(G && G->app.ref() == self.ref() && G->running); })));
        a.set("push_screen", def("push_screen", "self, screen, callback=, wait_for_dismiss=", {None, var(false)}, [](var self, var screen, var cb, var) -> var {
            (void)self;
            push_screen(screen, cb);
            return None;
        }));
        a.set("pop_screen", def("pop_screen", "self", {}, [](var) -> var { return pop_screen(); }));
        a.set("switch_screen", def("switch_screen", "self, screen", {}, [](var, var screen) -> var {
            pop_screen();
            push_screen(screen, None);
            return None;
        }));
        a.set("exit", def("exit", "self, result=, return_code=, message=", {None, var(0), None}, [](var self, var result, var code, var message) -> var {
            (void)message;
            self.setattr("return_value", result);
            self.setattr("return_code", code);
            if (G) G->exiting = true;
            return None;
        }));
        a.set("call_from_thread", def("call_from_thread", "self, callback, *args, **kwargs", {}, [](var self, var cb, var args, var kwargs) -> var {
            (void)self;
            AppState* A = app_state();
            if (GetCurrentThreadId() == g_main_thread) raise(E_RuntimeError, "The `call_from_thread` method must run in a different thread from the app");
            if (A->closed) raise(E_RuntimeError, "App is not running");
            auto c = std::make_shared<ThreadCall>();
            c->fn = cb;
            c->args.pos = collect(args);
            for (var k : each(kwargs)) c->args.kw.push_back({k, kwargs.getitem(k)});
            {
                std::lock_guard<std::mutex> g(A->mu);
                A->calls.push_back(c);
            }
            A->cv.notify_all();
            {
                gil_release g;
                std::unique_lock<std::mutex> lk(A->mu);
                A->cv.wait(lk, [&] { return c->done || A->closed.load(); });
            }
            if (!c->done) raise(E_RuntimeError, "App is not running");
            if (!c->exc.is_unbound() && !c->exc.is_none()) raise(c->exc);
            return c->result;
        }));
        a.set("action_quit", def("action_quit", "self", {}, [](var self) -> var { return self.call("exit"); }));
        a.set("action_pop_screen", def("action_pop_screen", "self", {}, [](var) -> var { pop_screen(); return None; }));
        a.set("action_push_screen", def("action_push_screen", "self, screen", {}, [](var, var) -> var { return None; }));
        a.set("action_focus_next", def("action_focus_next", "self", {}, [](var) -> var { focus_move(1); return None; }));
        a.set("action_focus_previous", def("action_focus_previous", "self", {}, [](var) -> var { focus_move(-1); return None; }));
        a.set("action_bell", def("action_bell", "self", {}, [](var) -> var { return None; }));
        a.set("bell", def("bell", "self", {}, [](var) -> var { return None; }));
        a.set("action_command_palette", def("action_command_palette", "self", {}, [](var) -> var {
            Node* s = active_screen();
            if (s && type_of(obj(s))->name == "CommandPalette") return None;
            push_screen(call0(g_cls[C_Palette]), None);
            return None;
        }));
        a.set("get_key_display", def("get_key_display", "self, binding", {}, [](var, var b) -> var {
            BindingR r;
            r.key = normalize_key(str_utf8(b.getattr("key")));
            var kd = b.getattr("key_display");
            if (!kd.is_none()) r.key_display = str_utf8(kd);
            return new_str(key_display(r));
        }));
        a.set("set_focus", def("set_focus", "self, widget, scroll_visible=", {var(true)}, [](var, var w, var) -> var {
            Node* s = active_screen();
            set_focus(s, w.is_none() ? nullptr : node_of(w));
            return None;
        }));
        // (not textual's: the frame as it is drawn now -- rows of (text, fg, bg, attributes) runs -- what the
        // comparison with textual's screens reads; textual's own is its compositor's strips)
        a.set("_tui_layout", def("_tui_layout", "self", {}, [](var) -> var {
            render_frame();
            std::vector<var> out;
            Node* s = active_screen();
            std::function<void(Node*, int)> rec = [&](Node* n, int d) {
                out.push_back(new_tuple({var((int64_t)d), new_str(n->type_names.empty() ? "?" : n->type_names[0]), new_str(n->id), var((int64_t)n->x), var((int64_t)n->y), var((int64_t)n->w), var((int64_t)n->h)}));
                for (Node* k : kids(n)) rec(k, d + 1);
            };
            if (s) rec(s, 0);
            return new_list(out);
        }));
        a.set("_tui_frame", def("_tui_frame", "self", {}, [](var) -> var {
            AppState* A = app_state();
            render_frame();
            std::vector<var> rows;
            for (auto& row : A->frame) {
                std::vector<var> segs;
                std::u32string run;
                Style cur;
                auto flush = [&]() {
                    if (run.empty()) return;
                    std::string fl = std::string(cur.bold == 1 ? "b" : "");
                    segs.push_back(new_tuple({new_str(utf8(run)), new_str(lower(hex(mono(cur.fg)))), new_str(lower(hex(mono(cur.bg)))), new_str(fl)}));
                    run.clear();
                };
                for (auto& c : row) {
                    if (!c.ch) continue;
                    if (!(c.s == cur)) flush(), cur = c.s;
                    run += c.ch;
                }
                flush();
                rows.push_back(new_list(segs));
            }
            return new_list(rows);
        }));
        a.set("run_test", def("run_test", "self, *, headless=, size=, tooltips=, notifications=, message_hook=", {var(true), new_tuple({var(80), var(24)}), var(false), var(false), None},
                              [](var self, var, var size, var, var notifications, var) -> var {
            var r = call0(g_cls[C_RunTest]);
            r.setattr("_app", self);
            r.setattr("_size", size);
            r.setattr("_notifications", notifications);
            return r;
        }));
        a.set("run", def("run", "self, *, headless=, inline=, inline_no_clear=, mouse=, size=, auto_pilot=", {var(false), var(false), var(false), var(true), None, None},
                         [](var self, var headless, var, var, var, var size, var) -> var {
            int w = 80, h = 24;
            if (!size.is_none()) {
                std::vector<var> sz = collect(size);
                w = (int)sz.at(0).as_int64(), h = (int)sz.at(1).as_int64();
            }
            start_app(self, headless.truthy(), w, h);
            AppState* A = G;
            A->node->mounted = true;
            try {
                guarded([&] {
                    push_screen(self.getattr("_tui_make_default_screen")(), None);
                    if (self.hasattr("on_mount")) finish(self.call("on_mount"));
                });
                while (!A->exiting) {
                    turn();
                    if (A->exiting) break;
                    if (!A->headless) {
                        read_console_events();
                        int w2 = A->W, h2 = A->H;
                        console_size(w2, h2);
                        if (w2 != A->W || h2 != A->H) A->W = w2, A->H = h2, A->full = true, request_redraw();
                    }
                    if (A->exiting) break;
                    wait_events(0.05);
                }
            } catch (...) {
                shutdown_app(false);
                G = nullptr;
                throw;
            }
            shutdown_app(false);
            var err = A->error;
            G = nullptr;
            if (!err.is_unbound() && !err.is_none()) {
                print_exception_(err);
                self.setattr("return_code", var(1));
            }
            return self.getattr("return_value");
        }));
        g_cls[C_App] = a.make({C(C_DOMNode)});
    }
}

void init_tui() {
    register_native_module("textual", [](const var& m) {
        build_classes();
        mod_set(m, "__path__", new_list());
        mod_set(m, "__version__", "8.2.8"_s);
        // on(message_type, selector=None): the handler of a message (by type) from a widget matching the selector
        mod_def(m, "on", [](const CallArgs& a) -> var {
            var cls = a.pos.at(0);
            var sel = a.pos.size() > 1 ? a.pos[1] : kwarg(a, "selector", None);
            std::string s = sel.is_none() ? "" : str_utf8(sel);
            return natf("on", [cls, s](const CallArgs& b) -> var {
                var f = b.pos.at(0);
                g_on[f.ref()].push_back({cls, s});
                return f;
            });
        });
        // work(method=None, *, thread=False, exclusive=False, group="default", ...)
        mod_def(m, "work", [](const CallArgs& a) -> var {
            bool thread = kwarg(a, "thread", false).truthy(), exclusive = kwarg(a, "exclusive", false).truthy();
            std::string group = str_(kwarg(a, "group", "default"_s));
            if (!a.pos.empty() && callable(a.pos[0]).truthy()) return make_worker_wrapper(a.pos[0], thread, exclusive, group);
            return natf("work", [thread, exclusive, group](const CallArgs& b) -> var { return make_worker_wrapper(b.pos.at(0), thread, exclusive, group); });
        });
    });
    register_native_module("textual.app", [](const var& m) {
        build_classes();
        mod_set(m, "App", C(C_App));
        mod_set(m, "ComposeResult", builtin("object"));
        mod_set(m, "RenderResult", builtin("object"));
    });
    register_native_module("textual.binding", [](const var& m) {
        build_classes();
        mod_set(m, "Binding", C(C_Binding));
        mod_set(m, "BindingType", builtin("object"));
    });
    register_native_module("textual.command", [](const var& m) {
        build_classes();
        mod_set(m, "Hit", C(C_Hit));
        mod_set(m, "Hits", builtin("object"));
        mod_set(m, "DiscoveryHit", C(C_DiscoveryHit));
        mod_set(m, "Provider", C(C_Provider));
        mod_set(m, "CommandPalette", C(C_Palette));
    });
    register_native_module("textual.containers", [](const var& m) {
        build_classes();
        mod_set(m, "Container", C(C_Container));
        mod_set(m, "Horizontal", C(C_Horizontal));
        mod_set(m, "Vertical", C(C_Vertical));
        mod_set(m, "VerticalScroll", C(C_VScroll));
        mod_set(m, "ScrollableContainer", C(C_Scrollable));
    });
    register_native_module("textual.screen", [](const var& m) {
        build_classes();
        mod_set(m, "Screen", C(C_Screen));
        mod_set(m, "ModalScreen", C(C_ModalScreen));
    });
    register_native_module("textual.theme", [](const var& m) {
        build_classes(); mod_set(m, "Theme", C(C_Theme)); });
    register_native_module("textual.message", [](const var& m) {
        build_classes(); mod_set(m, "Message", C(C_Message)); });
    register_native_module("textual.widget", [](const var& m) {
        build_classes(); mod_set(m, "Widget", C(C_Widget)); });
    register_native_module("textual.dom", [](const var& m) {
        build_classes(); mod_set(m, "DOMNode", C(C_DOMNode)); });
    register_native_module("textual.pilot", [](const var& m) {
        build_classes(); mod_set(m, "Pilot", C(C_Pilot)); });
    register_native_module("textual.timer", [](const var& m) {
        build_classes(); mod_set(m, "Timer", C(C_Timer)); });
    register_native_module("textual.worker", [](const var& m) {
        build_classes(); mod_set(m, "Worker", C(C_Worker)); });
    register_native_module("textual.fuzzy", [](const var& m) {
        build_classes(); mod_set(m, "Matcher", C(C_Matcher)); });
    register_native_module("textual.css", [](const var& m) {
        build_classes(); mod_set(m, "__path__", new_list()); });
    register_native_module("textual.css.query", [](const var& m) {
        build_classes();
        mod_set(m, "NoMatches", C(C_NoMatches));
        mod_set(m, "WrongType", C(C_WrongType));
        mod_set(m, "DOMQuery", C(C_Query));
    });
    register_native_module("textual.widgets", [](const var& m) {
        build_classes();
        mod_set(m, "DataTable", C(C_DataTable));
        mod_set(m, "Footer", C(C_Footer));
        mod_set(m, "Header", C(C_Header));
        mod_set(m, "Input", C(C_Input));
        mod_set(m, "Label", C(C_Label));
        mod_set(m, "LoadingIndicator", C(C_Loading));
        mod_set(m, "RichLog", C(C_RichLog));
        mod_set(m, "Static", C(C_Static));
        mod_set(m, "TabbedContent", C(C_Tabbed));
        mod_set(m, "TabPane", C(C_TabPane));
        mod_set(m, "Tabs", C(C_Tabs));
        mod_set(m, "Tab", C(C_Tab));
        mod_set(m, "ContentSwitcher", C(C_Switcher));
    });
    register_native_module("textual.widgets.data_table", [](const var& m) {
        build_classes();
        mod_set(m, "RowKey", C(C_RowKey));
        mod_set(m, "ColumnKey", C(C_ColumnKey));
        mod_set(m, "CellKey", C(C_CellKey));
        mod_set(m, "DataTable", C(C_DataTable));
    });
}

}  // namespace tui

void init_tui() {
    tui::init_rich();
    tui::init_tui();
}

}  // namespace py
