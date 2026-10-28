"""terminal.py -- CodeBreaker in the command prompt: a full-screen text interface, no window of its own.

    python -m codebreaker            (with no command, in a terminal)
    python -m codebreaker ui

The corpus at a glance (every module, its verdicts as a bar), a module's functions with the
reconstructed C, the assembly, who calls what and the judge's verdict beside each, and the long
work (measure, decompile) started as jobs and followed live. It shows what the function store
holds through the same calls the MCP server answers (ui/mcp_server.py), so the two never disagree.

Built on Textual (pip install textual). Every call that can take a while runs in a worker
thread; the screens say "working" and fill in when it is done.
"""
import importlib.util
import os

from rich.syntax import Syntax
from rich.table import Table
from rich.text import Text

from textual import on, work
from textual.app import App, ComposeResult
from textual.binding import Binding
from textual.command import Hit, Hits, Provider
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.screen import ModalScreen, Screen
from textual.theme import Theme
from textual.widgets import (DataTable, Footer, Header, Input, Label, LoadingIndicator, RichLog, Static,
                             TabbedContent, TabPane)

# ---------------------------------------------------------------------------------------------- look
# One colour per verdict, the same everywhere: the bars, the tables, the detail panes.
VERDICTS = ("verified", "weak", "unverified", "not-judged", "no-compile", "no-source", "identified")
COLOR = {"verified": "#4ec9a0", "weak": "#e5b454", "unverified": "#e06c75", "not-judged": "#7f8796",
         "no-compile": "#c678dd", "no-source": "#4b5263", "identified": "#61afef"}
TITLE = {"verified": "verified", "weak": "weak", "unverified": "unverified", "not-judged": "not judged",
         "no-compile": "does not compile", "no-source": "no source", "identified": "runtime library"}
MARK = {"verified": "●", "weak": "◐", "unverified": "✕", "not-judged": "○", "no-compile": "▲",
        "no-source": "·", "identified": "◆"}

THEME = Theme(
    name="codebreaker",
    primary="#61afef",
    secondary="#c678dd",
    accent="#4ec9a0",
    warning="#e5b454",
    error="#e06c75",
    success="#4ec9a0",
    foreground="#d7dae0",
    background="#16181d",
    surface="#1d2026",
    panel="#262a32",
    dark=True,
    variables={"block-cursor-background": "#2c3a4d", "block-cursor-foreground": "#ffffff",
               "footer-key-foreground": "#61afef", "input-selection-background": "#2c3a4d"},
)

LOGO = r"""[b #61afef]  ___         _     ___              _
 / __|___  __| |___| _ )_ _ ___ __ _| |_____ _ _
| (__/ _ \/ _` / -_) _ \ '_/ -_) _` | / / -_) '_|
 \___\___/\__,_\___|___/_| \___\__,_|_\_\___|_|[/]"""


def bar(counts, width=24, keys=VERDICTS):
    """Verdict counts as one bar of coloured blocks, each verdict its share of the width."""
    total = sum(counts.get(k, 0) or 0 for k in keys)
    out = Text()
    if not total:
        return out.append("─" * width, style="#3a3f4b")
    used = 0
    for i, k in enumerate(keys):
        n = counts.get(k, 0) or 0
        cells = round(width * n / total) if i < len(keys) - 1 else width - used
        cells = max(0, min(cells, width - used))
        if n and not cells and used < width:
            cells = 1                                    # (every verdict present gets at least a sliver)
        out.append("█" * cells, style=COLOR[k])
        used += cells
    if used < width:
        out.append("█" * (width - used), style="#3a3f4b")
    return out


def pct(n, total):
    return "%d%%" % round(100.0 * n / total) if total else "–"


def legend(counts, keys=VERDICTS):
    """`● 80 verified  ◐ 29 weak ...` for the verdicts present."""
    out = Text()
    for k in keys:
        n = counts.get(k, 0) or 0
        if n:
            out.append("%s %d %s   " % (MARK[k], n, TITLE[k]), style=COLOR[k])
    return out


def flat_address(addr):
    """A 16-bit `seg:off` as the flat address the analyses use (segment n at the made-up selector
    1000h + 8(n-1), as the description gives it); anything else as it is."""
    if ":" in addr:
        seg, off = addr.split(":", 1)
        try:
            return "%x" % (((0x1000 + 8 * (int(seg) - 1)) << 16) | int(off, 16))
        except ValueError:
            return addr
    return addr


# ---------------------------------------------------------------------------------------------- the store
class Store(object):
    """The calls the screens make, as the MCP server answers them (ui/mcp_server.py). Imported on
    first use: the server module sets up the corpus and the store."""

    def __init__(self):
        self._m = None

    @property
    def m(self):
        if self._m is None:
            from codebreaker.ui import mcp_server
            self._m = mcp_server
        return self._m

    def __getattr__(self, name):
        return getattr(self.m, name)


STORE = Store()


def ready(answer):
    """False while the server is still working a slow answer out in the background."""
    return not (isinstance(answer, dict) and answer.get("status") == "computing")


# ---------------------------------------------------------------------------------------------- small screens
class Help(ModalScreen):
    BINDINGS = [Binding("escape,q,question_mark", "dismiss", "Close")]

    def compose(self) -> ComposeResult:
        keys = Table.grid(padding=(0, 3))
        keys.add_column(style="bold #61afef", justify="right")
        keys.add_column()
        for k, what in (("everywhere", ""), ("ctrl+p", "command palette: open any module, run anything"),
                        ("j", "jobs: what is running, its output live"), ("?", "this help"),
                        ("q", "back, or quit on the first screen"),
                        ("", ""), ("modules", ""), ("/", "find a module (or type a path and press enter)"),
                        ("enter", "open the module"), ("r", "work the numbers out again"),
                        ("", ""), ("a module", ""), ("/", "find a function by name or address"),
                        ("f", "show only one verdict (again: the next)"), ("tab", "source, assembly, calls, verdict"),
                        ("m", "measure: decompile, build and judge the module (a job)"),
                        ("d", "decompile the function now, as it would be made today"),
                        ("n", "name the function (every module with the same code gets it)"),
                        ("i", "what the file is: format, sections, imports, exports")):
            keys.add_row(k, what)
        with Vertical(id="help"):
            yield Static(Text.from_markup(LOGO), id="help-logo")
            yield Static(keys)
            yield Label("[dim]escape to close[/]")


class Ask(ModalScreen):
    """One line of text from the user (a name), or None."""
    BINDINGS = [Binding("escape", "dismiss(None)", "Cancel")]

    def __init__(self, question, value=""):
        super().__init__()
        self.question, self.value = question, value

    def compose(self) -> ComposeResult:
        with Vertical(id="ask"):
            yield Label(self.question)
            yield Input(value=self.value, id="ask-input")
            yield Label("[dim]enter to save · escape to cancel[/]")

    @on(Input.Submitted)
    def done(self, event):
        self.dismiss(event.value.strip() or None)


class Info(ModalScreen):
    """What a file is: triage's summary, imports and exports."""
    BINDINGS = [Binding("escape,q,i", "dismiss", "Close")]

    def __init__(self, module):
        super().__init__()
        self.module = module

    def compose(self) -> ComposeResult:
        with Vertical(id="info"):
            yield Label("[b]%s[/]" % os.path.basename(self.module), id="info-title")
            yield LoadingIndicator()
            yield VerticalScroll(Static(id="info-body"))

    def on_mount(self):
        self.load()

    @work(thread=True)
    def load(self):
        try:
            t = STORE.triage(self.module)
        except Exception as e:                                  # noqa: BLE001 -- shown, not raised
            t = {"error": str(e)}
        self.app.call_from_thread(self.show, t)

    def show(self, t):
        self.query_one(LoadingIndicator).remove()
        if "error" in t:
            self.query_one("#info-body", Static).update(Text(t["error"], style="#e06c75"))
            return
        grid = Table.grid(padding=(0, 2))
        grid.add_column(style="#7f8796", justify="right")
        grid.add_column()
        for k, v in ((t.get("is") or {}).items()):
            grid.add_row(k, str(v))
        for k, v in ((t.get("summary") or {}).items()):
            if v not in ("", None):
                grid.add_row(k.replace("_", " "), str(v))
        exports = t.get("exports") or []
        imports = t.get("imports") or {}
        body = Table.grid()
        body.add_row(grid)
        if exports:
            body.add_row(Text("\nexports (%d)" % len(exports), style="bold #61afef"))
            body.add_row(Text("  ".join(map(str, exports[:400]))))
        if imports:
            body.add_row(Text("\nimports", style="bold #61afef"))
            for lib, names in imports.items():
                body.add_row(Text.assemble((lib + "  ", "bold"), ("  ".join(map(str, names)), "")))
        self.query_one("#info-body", Static).update(body)


# ---------------------------------------------------------------------------------------------- jobs
class Jobs(Screen):
    """The jobs started from here: each a `python -m codebreaker ...` process, its output live."""
    BINDINGS = [Binding("escape,q,j", "app.pop_screen", "Back"), Binding("s", "stop", "Stop job")]

    def compose(self) -> ComposeResult:
        yield Header()
        with Horizontal():
            with Vertical(id="jobs-list"):
                yield Label("[b]Jobs[/]  [dim]started from here[/]", classes="pane-title")
                yield DataTable(id="jobs", cursor_type="row", zebra_stripes=True)
            with Vertical(id="jobs-log-pane"):
                yield Label("output", id="jobs-log-title", classes="pane-title")
                yield RichLog(id="jobs-log", wrap=False, highlight=True, markup=False)
        yield Footer()

    def on_mount(self):
        t = self.query_one("#jobs", DataTable)
        t.add_columns("job", "command", "module", "status", "time")
        self.shown, self.log_at = None, 0
        self.refresh_jobs()
        self.set_interval(1.0, self.refresh_jobs)

    def refresh_jobs(self):
        t = self.query_one("#jobs", DataTable)
        rows = STORE.jobs()["jobs"]
        keep = t.cursor_row
        t.clear()
        style = {"running": "#e5b454", "done": "#4ec9a0", "failed": "#e06c75"}
        for j in rows:
            t.add_row(j["id"], j["action"], j["module"] or "",
                      Text(("◷ " if j["status"] == "running" else "") + j["status"], style=style[j["status"]]),
                      "%dm %02ds" % divmod(j["seconds"], 60), key=j["id"])
        if rows:
            t.move_cursor(row=min(keep, len(rows) - 1))
            self.follow(rows[min(max(keep, 0), len(rows) - 1)]["id"])
        else:
            self.query_one("#jobs-log-title", Label).update("[dim]nothing started yet: m on a module measures it[/]")

    def follow(self, jid):
        log = self.query_one("#jobs-log", RichLog)
        if jid != self.shown:
            self.shown, self.log_at = jid, 0
            log.clear()
        got = STORE.job(jid, offset=self.log_at)
        if got.get("log"):
            for line in got["log"].splitlines():
                log.write(line)
        self.log_at = got.get("next", self.log_at)
        self.query_one("#jobs-log-title", Label).update("[b]%s %s[/]  [dim]%s[/]" % (got["action"], got["module"] or "", got["status"]))

    @on(DataTable.RowHighlighted, "#jobs")
    def picked(self, event):
        if event.row_key and event.row_key.value != self.shown:
            self.follow(event.row_key.value)

    def action_stop(self):
        if self.shown:
            STORE.stop_job(self.shown)
            self.notify("stopped %s" % self.shown)


# ---------------------------------------------------------------------------------------------- a module
FILTERS = (None, "verified", "weak", "unverified", "no-compile", "not-judged", "no-source")


class Module(Screen):
    """One module: its functions, and for the one picked its C, assembly, calls and verdict."""
    BINDINGS = [Binding("escape,q", "back", "Back"), Binding("slash", "find", "Find"),
                Binding("f", "filter", "Filter"), Binding("m", "measure", "Measure"),
                Binding("d", "decompile", "Decompile now"), Binding("n", "name", "Name it"),
                Binding("i", "info", "File info"), Binding("r", "reload", "Reload", show=False)]

    def __init__(self, module):
        super().__init__()
        self.module = module
        self.rows, self.shown_filter, self.picked_addr = [], None, None
        self.measuring = None

    def compose(self) -> ComposeResult:
        yield Header()
        yield Static(id="mod-head")
        with Horizontal(id="mod-body"):
            with Vertical(id="fn-pane"):
                yield Input(placeholder="find a function: name or address", id="fn-find")
                yield DataTable(id="functions", cursor_type="row", zebra_stripes=True)
                yield Label(id="fn-count", classes="dim-line")
            with TabbedContent(id="detail"):
                with TabPane("Source", id="tab-source"):
                    yield VerticalScroll(Static(id="source"))
                with TabPane("Assembly", id="tab-asm"):
                    yield VerticalScroll(Static(id="asm"))
                with TabPane("Calls", id="tab-calls"):
                    yield VerticalScroll(Static(id="calls"))
                with TabPane("Verdict", id="tab-verdict"):
                    yield VerticalScroll(Static(id="verdict"))
        yield Footer()

    def on_mount(self):
        self.title = "CodeBreaker"
        self.sub_title = os.path.basename(self.module)
        t = self.query_one("#functions", DataTable)
        t.focus()
        t.add_column("", key="mark", width=1)
        t.add_column("address", key="addr", width=10)
        t.add_column("name", key="name", width=24)
        t.add_column("verdict", key="verdict", width=17)
        self.query_one("#mod-head", Static).update(Text("  loading %s ..." % self.module, style="#7f8796"))
        self.load()

    @work(thread=True, exclusive=True, group="module")
    def load(self):
        try:
            got = STORE.functions(self.module, limit=5000)
            if got.get("sha"):
                from codebreaker.store import functions as F
                earlier = F.latest(got["sha"])
                for r in got.get("rows", []):
                    if r["class"] == "no-source" and earlier.get(r["addr"], "no-source") != "no-source":
                        r["earlier"] = earlier[r["addr"]]
        except Exception as e:                                  # noqa: BLE001
            got = {"error": str(e)}
        self.app.call_from_thread(self.loaded, got)

    def loaded(self, got):
        head = self.query_one("#mod-head", Static)
        if "error" in got:
            head.update(Text("  " + got["error"], style="#e06c75"))
            return
        self.got = got
        self.rows = got.get("rows", [])
        c = dict(got.get("counts", {}))
        total = c.get("functions") or len(self.rows)
        earlier = [r for r in self.rows if r.get("earlier")]
        for r in earlier:                                       # (counted by the verdict they last had)
            c["no-source"] = max(0, (c.get("no-source") or 0) - 1)
            c[r["earlier"]] = (c.get(r["earlier"]) or 0) + 1
        grid = Table.grid(padding=(0, 2), expand=True)
        grid.add_column(ratio=1)
        grid.add_column(justify="right")
        title = Text.assemble((" %s" % got["module"], "bold #ffffff"), ("   %s" % got.get("kind_title", ""), "#7f8796"),
                              ("   %d functions" % total, "#7f8796"))
        good = (c.get("verified", 0) or 0) + (c.get("weak", 0) or 0)
        right = Text("%s verified or weak" % pct(good, total), style="bold #4ec9a0")
        grid.add_row(title, right)
        grid.add_row(Text(" ").append(bar(c, 60)), Text(""))
        grid.add_row(Text(" ").append(legend(c)), Text(""))
        if earlier:
            grid.add_row(Text(" %d functions have no verdict under the judge in force: shown dim, by the last one "
                              "they had. m measures the module again." % len(earlier), style="#e5b454"), Text(""))
        head.update(grid)
        self.fill()

    def fill(self):
        t = self.query_one("#functions", DataTable)
        find = self.query_one("#fn-find", Input).value.strip().lower()
        t.clear()
        n = 0
        for r in self.rows:
            cls = r.get("earlier") or r["class"]
            if self.shown_filter and cls != self.shown_filter:
                continue
            if find and find not in r["addr"].lower() and find not in (r["name"] or "").lower():
                continue
            dim = " dim" if r.get("earlier") else ""
            t.add_row(Text(MARK.get(cls, "?"), style=COLOR.get(cls, "") + dim), Text(r["addr"], style="#9da5b4"),
                      r["name"] or Text("sub_" + r["addr"].replace(":", "_"), style="#7f8796"),
                      Text(TITLE.get(cls, cls), style=COLOR.get(cls, "") + dim), key=r["addr"])
            n += 1
        what = TITLE[self.shown_filter] if self.shown_filter else "all"
        self.query_one("#fn-count", Label).update(" %d of %d shown  ·  filter: %s" % (n, len(self.rows), what))
        if n:
            t.move_cursor(row=0)

    @on(Input.Changed, "#fn-find")
    def finding(self, _event):
        self.fill()

    @on(Input.Submitted, "#fn-find")
    def found(self, _event):
        self.query_one("#functions", DataTable).focus()

    @on(DataTable.RowHighlighted, "#functions")
    def picked(self, event):
        if event.row_key is None:
            return
        self.picked_addr = event.row_key.value
        for pane in ("#source", "#asm", "#calls", "#verdict"):
            self.query_one(pane, Static).update(Text("working ...", style="#7f8796"))
        self.detail(self.picked_addr)

    @work(thread=True, exclusive=True, group="detail")
    def detail(self, addr):
        flat = flat_address(addr)
        for key, call in (("function", lambda: STORE.function(self.module, addr)),
                          ("xrefs", lambda: STORE.xrefs(self.module, flat)),
                          ("asm", lambda: STORE.disasm(self.module, flat))):
            if self.picked_addr != addr:
                return                                          # (moved on: this one is not shown)
            try:
                got = call()
            except Exception as e:                              # noqa: BLE001
                got = {"error": str(e)}
            self.app.call_from_thread(self.show_detail, addr, key, got)

    def show_detail(self, addr, key, got):
        if addr != self.picked_addr:
            return
        if key == "function":
            # the source: the store's, or the latest reconstruction (with what earlier judges said of it)
            src = got.get("source") or next((c["source"] for c in got.get("candidates", []) if c.get("source")), "")
            source = self.query_one("#source", Static)
            if src:
                source.update(Syntax(src, "c", theme="monokai", line_numbers=True, word_wrap=False,
                                     background_color="#16181d"))
            else:
                source.update(Text(got.get("error") or "no reconstructed source stored: d decompiles it now",
                                   style="#7f8796"))
            self.query_one("#verdict", Static).update(self.verdict_view(got))
        elif key == "asm":
            self.query_one("#asm", Static).update(
                Syntax(got["listing"], "nasm", theme="monokai", background_color="#16181d")
                if got.get("listing") else Text(got.get("error", "no listing"), style="#7f8796"))
        else:
            self.query_one("#calls", Static).update(self.calls_view(got))

    @staticmethod
    def calls_view(xr):
        if "error" in xr:
            return Text(xr["error"], style="#e06c75")
        g = Table.grid(padding=(0, 2))
        g.add_column(style="#7f8796")
        g.add_column()
        g.add_column(style="#7f8796")
        g.add_row(Text("called from", style="bold #61afef"), "", "")
        for r in xr.get("from_code", []) or [{"site": "", "in": "(nothing in this module calls it)", "how": ""}]:
            g.add_row(r.get("site", ""), Text(r.get("in", ""), style="#d7dae0"), r.get("how", ""))
        if xr.get("in_data"):
            g.add_row(Text("\nin data", style="bold #61afef"), "", "")
            for r in xr["in_data"]:
                g.add_row(str(r.get("site", r)), str(r.get("in", "")), "")
        g.add_row(Text("\ncalls", style="bold #61afef"), "", "")
        for r in xr.get("calls", []) or [{"site": "", "to": "(no calls)", "kind": ""}]:
            to = r.get("to", "")
            g.add_row(r.get("site", ""), Text(to, style="#e5b454" if "!" in to else "#d7dae0"), r.get("kind", ""))
        return g

    @staticmethod
    def verdict_view(fn):
        """The verdict in force, or the last one the function had, and what was said of it."""
        if "error" in fn and len(fn) == 1:
            return Text(fn["error"], style="#e06c75")
        g = Table.grid(padding=(0, 2), expand=True)
        g.add_column(style="#7f8796", justify="right", width=14)
        g.add_column(ratio=1)
        cls = fn.get("class") or ""
        earlier = [(c, v) for c in fn.get("candidates", []) for v in c.get("older_verdicts", [])]
        if cls and cls != "no-source":
            g.add_row("verdict", Text("%s %s" % (MARK.get(cls, ""), TITLE.get(cls, cls)), style="bold " + COLOR.get(cls, "")))
        elif earlier:
            from codebreaker.store import functions as F
            last = F.classify(earlier[0][1]["verdict"], earlier[0][1].get("limits"))[0]
            g.add_row("last verdict", Text.assemble(("%s %s" % (MARK.get(last, ""), TITLE.get(last, last)),
                                                     "bold " + COLOR.get(last, "")),
                                                    ("   by an earlier judge", "#7f8796")))
            if earlier[0][1].get("reason"):
                g.add_row("why", Text(earlier[0][1]["reason"]))
        else:
            g.add_row("verdict", Text("%s %s" % (MARK["no-source"], TITLE["no-source"]), style="#7f8796"))
        for k in ("reason", "why", "detail", "limits", "engine", "name"):
            if fn.get(k):
                g.add_row(k, Text(str(fn[k])))
        for c in fn.get("candidates", [])[:3]:
            g.add_row("", "")
            g.add_row("made by", Text("%s, revision %s" % (c.get("engine"), c.get("rev"))))
            for v in c.get("older_verdicts", [])[:4]:
                g.add_row("earlier judge", Text.assemble((v["verdict"], "#9da5b4"), ("  " + (v.get("reason") or ""), "#7f8796")))
            if c.get("problem"):
                g.add_row("problem", Text(c["problem"], style="#e06c75"))
        if fn.get("say"):
            g.add_row("", "")
            g.add_row("", Text(fn["say"], style="#7f8796"))
        return g

    # -- actions
    def action_back(self):
        if self.query_one("#fn-find", Input).has_focus:
            self.query_one("#functions", DataTable).focus()
        else:
            self.app.pop_screen()

    def action_find(self):
        self.query_one("#fn-find", Input).focus()

    def action_filter(self):
        i = FILTERS.index(self.shown_filter)
        self.shown_filter = FILTERS[(i + 1) % len(FILTERS)]
        self.fill()

    def action_reload(self):
        self.load()

    def action_info(self):
        self.app.push_screen(Info(self.module))

    def action_measure(self):
        try:
            j = STORE.run_command("measure", [self.module])
        except ValueError as e:
            self.notify(str(e), severity="warning")
            return
        self.measuring = j["id"]
        self.notify("measuring %s: job %s (j shows it)" % (self.module, j["id"]), title="Started")
        self.set_interval(3.0, self.watch_measure)

    def watch_measure(self):
        if not self.measuring:
            return
        got = STORE.job(self.measuring, tail=200)
        if got["status"] != "running":
            ok = got["status"] == "done"
            self.notify("%s %s" % (self.module, "measured" if ok else "measure failed: j shows why"),
                        severity="information" if ok else "error")
            self.measuring = None
            self.load()

    def action_decompile(self):
        if self.picked_addr:
            self.query_one("#detail", TabbedContent).active = "tab-source"
            self.query_one("#source", Static).update(Text("decompiling ...", style="#7f8796"))
            self.decompile(self.picked_addr)

    @work(thread=True, exclusive=True, group="detail")
    def decompile(self, addr):
        for _ in range(8):                                      # (the server says "computing" past its wait)
            try:
                got = STORE.decompile_function(self.module, addr, wait=30)
            except Exception as e:                              # noqa: BLE001
                got = {"error": str(e)}
            if ready(got):
                break
        self.app.call_from_thread(self.decompiled, addr, got)

    def decompiled(self, addr, got):
        if addr != self.picked_addr:
            return
        src = self.query_one("#source", Static)
        if got.get("text"):
            src.update(Syntax(got["text"], "c", theme="monokai", line_numbers=True, background_color="#16181d"))
            self.notify("decompiled now: not built or judged", timeout=3)
        else:
            src.update(Text(got.get("error") or got.get("say") or "nothing came back", style="#e06c75"))

    def action_name(self):
        if not self.picked_addr:
            return
        addr = self.picked_addr
        now = next((r["name"] for r in self.rows if r["addr"] == addr), "") or ""

        def named(name):
            if not name:
                return
            try:
                STORE.name_function(self.module, addr, name)
            except ValueError as e:
                self.notify(str(e), severity="error")
                return
            for r in self.rows:
                if r["addr"] == addr:
                    r["name"] = name
            self.fill()
            self.notify("%s is %s now" % (addr, name))
        self.app.push_screen(Ask("Name the function at %s" % addr, now), named)


# ---------------------------------------------------------------------------------------------- the corpus
class Home(Screen):
    """Every module the store holds, its verdicts as a bar; the corpus totals beside."""
    BINDINGS = [Binding("q", "app.quit", "Quit"), Binding("slash", "find", "Find"),
                Binding("r", "refresh", "Refresh"), Binding("enter", "open", "Open", show=False),
                Binding("escape", "leave_find", "Back to the list", show=False)]

    def compose(self) -> ComposeResult:
        yield Header()
        with Horizontal():
            with Vertical(id="side"):
                yield Static(Text.from_markup(LOGO), id="logo")
                yield Static(id="totals")
                yield Static(id="sweep")
            with Vertical(id="main"):
                yield Input(placeholder="find a module, or type a path and press enter", id="find")
                yield DataTable(id="modules", cursor_type="row", zebra_stripes=True)
                yield Label(id="mod-count", classes="dim-line")
        yield Footer()

    def on_mount(self):
        self.title = "CodeBreaker"
        self.sub_title = "readable source from any binary"
        t = self.query_one("#modules", DataTable)
        t.add_column("module", key="name", width=24)
        t.add_column("kind", key="kind", width=8)
        t.add_column("functions", key="n", width=9)
        t.add_column("verified", key="v", width=8)
        t.add_column("verdicts", key="bar", width=26)
        self.rows = []
        t.focus()                                               # (the keys go to the list, not the search)
        self.query_one("#totals", Static).update(Text("reading the store ...", style="#7f8796"))
        self.load()
        self.set_interval(2.0, self.sweep_now)

    @work(thread=True, exclusive=True, group="home")
    def load(self, refresh=False):
        from codebreaker.store import functions as F
        try:
            mods = F.overview()
        except Exception as e:                                  # noqa: BLE001
            self.app.call_from_thread(self.query_one("#totals", Static).update, Text(str(e), style="#e06c75"))
            return
        rows = []
        for m in mods:
            r = dict(m["latest"], name=m["name"], sha=m["sha"], kind=m["kind"], path=m["path"],
                     functions=m["functions"], identified=m["identified"])
            r["no-source"] = max(0, r["no-source"] - r["identified"])   # (a library function: not ours to judge)
            rows.append(r)
        self.app.call_from_thread(self.loaded, {"rows": rows})
        self.sweep_now()

    def loaded(self, got):
        self.rows = got.get("rows", [])
        self.fill()
        totals = {k: sum(r.get(k) or 0 for r in self.rows) for k in VERDICTS + ("functions",)}
        g = Table.grid(padding=(0, 1))
        g.add_column(justify="right")
        g.add_column()
        g.add_row(Text("%d" % len(self.rows), style="bold #ffffff"), Text("modules in the store", style="#9da5b4"))
        g.add_row(Text("{:,}".format(totals["functions"]), style="bold #ffffff"), Text("functions", style="#9da5b4"))
        g.add_row("", "")
        for k in VERDICTS:
            if totals.get(k):
                g.add_row(Text("{:,}".format(totals[k]), style=COLOR[k]), Text("%s %s" % (MARK[k], TITLE[k]), style=COLOR[k]))
        g.add_row("", "")
        g.add_row("", bar(totals, 28))
        g.add_row("", Text("\neach function by the last verdict\nit had, under any judge", style="#7f8796"))
        self.query_one("#totals", Static).update(g)

    def sweep_now(self):
        """The jobs running, under the totals. (Not the server's sweep progress: reading it works
        out the whole corpus's ledger, minutes of work that would starve the screen.)"""
        out = Text()
        for j in [j for j in STORE.jobs()["jobs"] if j["status"] == "running"]:
            if not out:
                out.append("\nrunning\n", style="bold #e5b454")
            out.append("  ◷ %s %s  %dm %02ds\n" % ((j["action"], j["module"]) + divmod(j["seconds"], 60)),
                       style="#e5b454")
        self.query_one("#sweep", Static).update(out)

    def fill(self):
        t = self.query_one("#modules", DataTable)
        find = self.query_one("#find", Input).value.strip().lower()
        t.clear()
        n = 0
        for r in self.rows:
            if find and find not in (r["name"] or "").lower():
                continue
            total = r.get("functions") or 0
            t.add_row(Text(r["name"] or "?", style="#ffffff"),
                      Text(r.get("kind") or "", style="#7f8796"), str(total),
                      Text(pct(r.get("verified") or 0, total), style=COLOR["verified"]),
                      bar(r, 24), key=r["sha"])
            n += 1
        self.query_one("#mod-count", Label).update(" %d of %d modules  ·  enter opens one  ·  ctrl+p opens any"
                                                   % (n, len(self.rows)))

    @on(Input.Changed, "#find")
    def finding(self, _event):
        self.fill()

    def module_of(self, sha):
        """What to open for a row: its file where it is still there, else its name (two builds can
        share a name)."""
        r = next((r for r in self.rows if r["sha"] == sha), None)
        if not r:
            return sha
        return r["path"] if r.get("path") and os.path.exists(r["path"]) else r["name"]

    @on(Input.Submitted, "#find")
    def go(self, event):
        text = event.value.strip().strip('"')
        t = self.query_one("#modules", DataTable)
        if text and os.path.exists(text):
            self.app.push_screen(Module(text))
        elif t.row_count == 1:
            self.app.push_screen(Module(self.module_of(t.coordinate_to_cell_key((0, 0)).row_key.value)))
        else:
            t.focus()

    @on(DataTable.RowSelected, "#modules")
    def opened(self, event):
        self.app.push_screen(Module(self.module_of(event.row_key.value)))

    def action_find(self):
        self.query_one("#find", Input).focus()

    def action_leave_find(self):
        self.query_one("#modules", DataTable).focus()

    def action_refresh(self):
        self.notify("working the numbers out again", timeout=2)
        self.load(refresh=True)

    def action_open(self):
        t = self.query_one("#modules", DataTable)
        if t.row_count:
            self.app.push_screen(Module(self.module_of(t.coordinate_to_cell_key((t.cursor_row, 0)).row_key.value)))


# ---------------------------------------------------------------------------------------------- the palette
class Modules(Provider):
    """ctrl+p: open any module of the corpus by name, or run a command on the one shown."""

    async def search(self, query: str) -> Hits:
        app = self.app
        matcher = self.matcher(query)
        home = next((s for s in app.screen_stack if isinstance(s, Home)), None)
        for r in (home.rows if home else [])[:5000]:
            score = matcher.match(r["name"] or "")
            if score > 0:
                yield Hit(score, matcher.highlight("open " + r["name"]),
                          lambda sha=r["sha"]: app.push_screen(Module(home.module_of(sha))),
                          help="%s functions" % (r.get("functions") or 0))
        for words, act, why in (("jobs", app.action_jobs, "what is running, its output"),
                                ("help", app.action_help, "every key")):
            score = matcher.match(words)
            if score > 0:
                yield Hit(score, matcher.highlight(words), act, help=why)
        if isinstance(app.screen, Module):
            for words, act in (("measure this module", app.screen.action_measure),
                               ("file info", app.screen.action_info)):
                score = matcher.match(words)
                if score > 0:
                    yield Hit(score, matcher.highlight(words), act)


# ---------------------------------------------------------------------------------------------- the app
class CodeBreaker(App):
    CSS_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "terminal.tcss")   # (wherever the app is run or subclassed from)
    TITLE = "CodeBreaker"
    COMMANDS = App.COMMANDS | {Modules}
    BINDINGS = [Binding("j", "jobs", "Jobs"), Binding("question_mark", "help", "Help"),
                Binding("ctrl+c", "quit", "Quit", show=False, priority=True)]

    def __init__(self, module=None):
        super().__init__()
        self.first = module

    def on_mount(self):
        self.register_theme(THEME)
        self.theme = "codebreaker"
        self.push_screen(Home())
        if self.first:
            self.push_screen(Module(self.first))

    def action_jobs(self):
        if not isinstance(self.screen, Jobs):
            self.push_screen(Jobs())

    def action_help(self):
        self.push_screen(Help())

    async def action_quit(self):
        """Quit; a job still running is said first (a second q stops it with the interface)."""
        from codebreaker.ui import server
        running = [j for j in server.JOBS.all() if j["status"] == "running"]
        if running and not getattr(self, "warned", False):
            self.warned = True
            self.notify("%s still running: q again stops it and quits" % ", ".join(
                "%s %s" % (j["action"], j["module"]) for j in running), severity="warning", timeout=6)
            return
        server.JOBS.stop_all()                                  # (only the processes started here)
        self.exit()


def main(argv):
    """`python -m codebreaker ui [module]`"""
    if not importlib.util.find_spec("textual"):
        print("The terminal interface needs Textual:  pip install textual")
        return 2
    CodeBreaker(argv[0] if argv else None).run()
    return 0


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv[1:]))
