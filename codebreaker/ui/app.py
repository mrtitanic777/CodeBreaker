r"""app.py -- the desktop app: open a binary, look inside it, run the pipeline on it, read the results.

    python -m codebreaker app                    the window
    python -m codebreaker app <file-or-module>   the window, with that file open
    python -m codebreaker app --selftest [M]     build the window, open a module, report, close

One window on what the commands already do: the corpus on the left (or any file, by Browse or as
the argument -- which is also what dropping a file on a shortcut to this command gives); on the
right the inspector's description of it (formats/inspect.py), its functions with class and
reason and each one's reconstructed source (the function store), the coverage ledger, and the
log of the job being run. Decompile and Measure start the same background jobs as the web
console (ui/server.py); the bottom line is the progress reading (ui/progress.py).

Written with the standard library's tkinter. Where that is not installed, `app` serves the web
console and opens it in the default browser instead.
"""
import os
import queue
import threading
import webbrowser

from codebreaker.base import corpus
from codebreaker.coverage import ledger
from codebreaker.formats import inspect
from codebreaker.ui import progress, server

PARTS = ("summary", "headers", "sections", "exports", "imports", "resources", "relocations", "dos", "strings")


class App(object):
    def __init__(self, tk, ttk, filedialog):
        self.tk, self.filedialog = tk, filedialog
        self.root = root = tk.Tk()
        root.title("CodeBreaker")
        root.geometry("1180x760")
        self.done = queue.Queue()                                # (callback, result) from worker threads
        self.path = self.info = self.job = self.web = None
        self.job_at, self.rows, self.busy, self.ticks = 0, {}, 0, 0

        top = ttk.Frame(root, padding=(8, 8, 8, 4))
        top.pack(fill="x")
        self.entry = ttk.Entry(top)
        self.entry.pack(side="left", fill="x", expand=True)
        self.entry.bind("<Return>", lambda e: self.open(self.entry.get().strip()))
        for text, command in (("Open", lambda: self.open(self.entry.get().strip())), ("Browse...", self.browse),
                              ("Decompile", lambda: self.start("decompile")), ("Measure", lambda: self.start("measure")),
                              ("Web console", self.open_web)):
            ttk.Button(top, text=text, command=command).pack(side="left", padx=(6, 0))

        self.status = ttk.Label(root, text="", padding=(8, 3), anchor="w", relief="sunken")
        self.status.pack(side="bottom", fill="x")
        panes = ttk.Panedwindow(root, orient="horizontal")
        panes.pack(fill="both", expand=True, padx=8, pady=4)
        left = ttk.Frame(panes)
        self.filter = ttk.Entry(left)
        self.filter.pack(fill="x")
        self.filter.bind("<KeyRelease>", lambda e: self.fill_list())
        self.list = tk.Listbox(left, activestyle="none", exportselection=False, width=30)
        self.list.pack(fill="both", expand=True, pady=(4, 0))
        self.list.bind("<<ListboxSelect>>", self.pick_module)
        panes.add(left, weight=1)
        self.book = ttk.Notebook(panes)
        panes.add(self.book, weight=5)

        look = ttk.Frame(self.book)
        bar = ttk.Frame(look)
        bar.pack(fill="x")
        self.part = ttk.Combobox(bar, values=PARTS, state="readonly", width=16)
        self.part.set("summary")
        self.part.pack(side="left", padx=4, pady=4)
        self.part.bind("<<ComboboxSelected>>", lambda e: self.show_part())
        self.look = self.text(look)
        self.book.add(look, text="Inspect")

        fn = ttk.Panedwindow(self.book, orient="vertical")
        holder = ttk.Frame(fn)
        self.tree = ttk.Treeview(holder, columns=("addr", "name", "class", "why"), show="headings", height=12)
        for col, title, width in (("addr", "address", 110), ("name", "name", 240), ("class", "class", 130),
                                  ("why", "what was found", 520)):
            self.tree.heading(col, text=title)
            self.tree.column(col, width=width, stretch=col == "why")
        scroll = ttk.Scrollbar(holder, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scroll.set)
        scroll.pack(side="right", fill="y")
        self.tree.pack(fill="both", expand=True)
        self.tree.bind("<<TreeviewSelect>>", self.pick_function)
        fn.add(holder, weight=2)
        lower = ttk.Frame(fn)
        self.source = self.text(lower)
        fn.add(lower, weight=3)
        self.book.add(fn, text="Functions")

        cover = ttk.Frame(self.book)
        ttk.Button(cover, text="Read again", command=self.show_coverage).pack(anchor="w", padx=4, pady=4)
        self.cover = self.text(cover)
        self.book.add(cover, text="Coverage")
        logs = ttk.Frame(self.book)
        self.log = self.text(logs)
        self.book.add(logs, text="Log")

        root.protocol("WM_DELETE_WINDOW", self.close)
        root.after(100, self.pump)
        root.after(1000, self.tick)
        self.work(ledger.measure, self.got_ledger)

    # ---- plumbing ----------------------------------------------------------------------------
    def text(self, parent):
        box = self.tk.Text(parent, wrap="none", font=("Consolas", 10), undo=False)
        ys = self.tk.Scrollbar(parent, command=box.yview)
        xs = self.tk.Scrollbar(parent, orient="horizontal", command=box.xview)
        box.configure(yscrollcommand=ys.set, xscrollcommand=xs.set, state="disabled")
        ys.pack(side="right", fill="y")
        xs.pack(side="bottom", fill="x")
        box.pack(fill="both", expand=True)
        return box

    def put(self, box, content, append=False):
        box.configure(state="normal")
        if not append:
            box.delete("1.0", "end")
        box.insert("end", content)
        box.configure(state="disabled")
        if append:
            box.see("end")

    def work(self, make, then):
        """Run make() off the window's thread; hand its result (or the error) to then()."""
        self.busy += 1

        def run():
            try:
                result = make()
            except Exception as e:
                result = e
            self.done.put((then, result))
        threading.Thread(target=run, daemon=True).start()

    def pump(self):
        try:
            while True:
                then, result = self.done.get_nowait()
                self.busy -= 1
                if isinstance(result, Exception):
                    self.status.config(text="%s: %s" % (type(result).__name__, result))
                else:
                    then(result)
        except queue.Empty:
            pass
        self.root.after(100, self.pump)

    def tick(self):
        """Once a second: the running job's new output. Every fifth: the progress line."""
        if self.job:
            view = server.JOBS.view(self.job, self.job_at)
            if view["log"]:
                self.put(self.log, view["log"], append=True)
            self.job_at = view["next"]
            if view["status"] != "running":
                self.put(self.log, "\n[%s %s: %s after %d s]\n" % (view["action"], view["module"], view["status"],
                                                                 view["seconds"]), append=True)
                self.job = None
                if view["action"] == "measure":
                    self.work(ledger.measure, self.got_ledger)
                    self.load_functions()
                elif view["has_output"]:
                    with open(server.JOBS.get(view["id"])["output"], encoding="utf-8", errors="replace") as f:
                        self.put(self.source, f.read(4 << 20))
                    self.book.select(1)
        self.ticks += 1
        if self.ticks % 5 == 1:
            self.work(progress.snapshot, lambda s: self.status.config(text=progress.line(s)))
        self.root.after(1000, self.tick)

    def close(self):
        server.JOBS.stop_all()
        if self.web:
            self.web.shutdown()
        self.root.destroy()

    # ---- the corpus and the ledger -----------------------------------------------------------
    def got_ledger(self, led):
        self.rows = {r["name"]: r for r in led["modules"]}
        self.fill_list()
        self.put(self.cover, "\n".join(ledger.render(led)))

    def show_coverage(self):
        self.put(self.cover, "reading...")
        self.work(ledger.measure, self.got_ledger)

    def fill_list(self):
        want = self.filter.get().strip().lower()
        self.names = [n for n in sorted(self.rows) if want in n]
        self.list.delete(0, "end")
        for n in self.names:
            r = self.rows[n]
            self.list.insert("end", "%-14s %s" % (n, ("%3.0f%%" % (100.0 * r["verified"] / r["functions"]))
                                                  if r["functions"] else ""))

    def pick_module(self, _event):
        sel = self.list.curselection()
        if sel:
            self.open(self.names[sel[0]])

    # ---- one module --------------------------------------------------------------------------
    def browse(self):
        path = self.filedialog.askopenfilename(title="Open a binary")
        if path:
            self.open(path)

    def open(self, name):
        path = corpus.retail(name) if name else None
        if not path:
            self.status.config(text="no file or corpus module %r" % name)
            return
        self.path = path
        self.entry.delete(0, "end")
        self.entry.insert(0, path)
        self.root.title("CodeBreaker -- " + os.path.basename(path))
        self.put(self.look, "reading...")
        self.work(lambda: inspect.describe(path), self.got_info)
        self.load_functions()

    def got_info(self, info):
        if info["path"] == self.path:
            self.info = info
            self.show_part()

    def show_part(self):
        if not self.info:
            return
        part, path = self.part.get(), self.path
        if part == "strings":
            self.put(self.look, "reading...")
            self.work(lambda: inspect.strings(path, limit=20000), lambda rows: self.put(self.look, "\n".join(
                "%08x  %-5s %-8s %s" % (r["offset"], r["kind"], r["category"], r["text"].replace("\n", "\\n")[:160])
                for r in rows)))
            return
        lines = ["%s  (%s, %d bytes)" % (self.info["path"], self.info["format"] or "not an executable",
                                        self.info["size"])] + ["note: " + n for n in self.info["notes"]] + [""]
        self.put(self.look, "\n".join(lines + inspect.render(self.info.get(part) or {}, part)))

    def load_functions(self):
        name = os.path.basename(self.path)
        self.tree.delete(*self.tree.get_children())
        self.put(self.source, "")
        self.work(lambda: server.functions(name), self.got_functions)

    def got_functions(self, found):
        if not found or found["module"] != os.path.basename(self.path).lower():
            return
        if not found["in_store"]:
            self.put(self.source, "The store holds nothing for this module. Measure it to reconstruct and judge "
                                  "its functions, or Decompile it to read the C.")
            return
        for r in found["functions"]:
            self.tree.insert("", "end", iid=r["addr"], values=(r["addr"], r["name"], r["title"], r["why"]))

    def pick_function(self, _event):
        sel = self.tree.selection()
        if not sel:
            return
        name, addr = os.path.basename(self.path), sel[0]

        def show(found):
            f = found["function"]
            head = "/* [%s] %s @ %s -- %s\n   verdict %s; reason %s; limits %s\n   %s */\n\n" % (
                f["title"], f["name"] or "sub_" + addr, addr, f["why"], f.get("verdict"), f.get("reason") or "-",
                f.get("limits") or "-", f.get("detail") or "")
            self.put(self.source, head + (f.get("source") or "/* no reconstructed source */"))
        self.work(lambda: server.functions(name, addr), show)

    # ---- acting ------------------------------------------------------------------------------
    def start(self, action):
        if not self.path:
            self.status.config(text="open a module first")
            return
        try:
            view = server.JOBS.start(action, self.path)
        except ValueError as e:
            self.status.config(text=str(e))
            return
        self.job, self.job_at = server.JOBS.get(view["id"]), 0
        self.put(self.log, "[%s %s, process %d]\n" % (action, view["module"], view["pid"]), append=True)
        self.book.select(3)

    def open_web(self):
        if not self.web:
            self.web = server.make_server()
            threading.Thread(target=self.web.serve_forever, daemon=True).start()
        webbrowser.open("http://127.0.0.1:%d/" % self.web.server_address[1])


def main(argv):
    """python -m codebreaker app [file-or-module] [--selftest]

    The desktop app. Without tkinter it serves the web console and opens it in the browser."""
    names = [a for a in argv if not a.startswith("--")]
    try:
        import tkinter as tk
        from tkinter import filedialog, ttk
    except ImportError:
        print("this Python has no tkinter; opening the web console in the browser instead")
        return server.main(["--open"])
    app = App(tk, ttk, filedialog)
    if names:
        app.root.after(200, lambda: app.open(names[0]))
    if "--selftest" in argv:
        def report():
            if app.busy or not app.rows:                         # still reading
                app.root.after(200, report)
                return
            print("window built; %d modules listed; inspector shows %d characters; %d functions listed; "
                  "coverage shows %d lines" % (app.list.size(), len(app.look.get("1.0", "end")),
                                               len(app.tree.get_children()),
                                               int(app.cover.index("end").split(".")[0]) - 1))
            app.close()
        app.root.after(1500, report)
    app.root.mainloop()
    return 0
