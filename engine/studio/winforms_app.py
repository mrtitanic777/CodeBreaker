#!/usr/bin/env python3
r"""studio/winforms_app.py -- CodeBreaker Workbench in native WinForms (visual styles ON).

The workbench built on real Windows Forms controls via pythonnet, rendered with the running
OS's own visual style -- so it looks native on whatever it runs on (Windows 7 / 10 / 11):
the OS-drawn title bar and window frame, themed buttons / TreeView / text boxes / status bar,
and the system UI font. No hand-painted chrome, no retro skinning.

Layout and flows are the same as the CLI's coverage: a quick toolbar, a left TreeView
navigator, an Analyze front door (drop a binary -> engine pipeline) with plain-language action
cards, a generic command runner that streams any `wt <subsystem> ...` into a console, and a
Settings/Doctor panel that reads config.json.

    wt ui                         # launch (falls back to the GDI app if pythonnet is absent)
    python winforms_app.py --selftest
    python winforms_app.py --shot out.png [analyze|command|settings]
"""
import os
import sys
import queue
import shlex
import threading
import subprocess

_HERE = os.path.dirname(os.path.abspath(__file__))
_ROOT = os.path.dirname(os.path.dirname(_HERE))
if _ROOT not in sys.path:
    sys.path.insert(0, _ROOT)

from engine.studio.app import NAV, ICONS, DEPS, CARDS, _load_cfg, _wt_argv   # noqa: E402

import clr  # noqa: E402  (pythonnet; ImportError here -> wt.py falls back to the GDI app)
clr.AddReference("System.Windows.Forms")
clr.AddReference("System.Drawing")

from System import Action                                                    # noqa: E402
from System.Drawing import (Color, Font, FontStyle, Bitmap, Rectangle, Point, Size,  # noqa: E402
                            SolidBrush, SystemColors, SystemFonts, ContentAlignment)
from System.Drawing.Imaging import ImageFormat                              # noqa: E402
from System.Windows.Forms import (Application, Form, Panel, Button, Label, TextBox,  # noqa: E402
                                  TreeView, TreeNode, TreeViewDrawMode, TreeNodeStates,
                                  ImageList, PictureBox, FlatStyle, FormBorderStyle, BorderStyle,
                                  DockStyle, AnchorStyles, TextImageRelation, PictureBoxSizeMode,
                                  DragDropEffects, DataFormats, FormStartPosition, ScrollBars,
                                  StatusStrip, ToolStripStatusLabel, ToolStripItemAlignment)
import ctypes                                                                # noqa: E402

ICON_PAL = {"K": (0, 0, 0), "W": (255, 255, 255), "LG": (200, 200, 200), "DG": (120, 120, 120),
            "NV": (0, 0, 130), "BL": (48, 72, 180), "LB": (190, 214, 250), "CY": (120, 180, 240),
            "YL": (232, 195, 74), "YP": (255, 242, 192), "GR": (16, 124, 16), "GN": (60, 200, 90),
            "RD": (193, 67, 67), "PU": (122, 92, 255), "PP": (200, 190, 250), "BK": (40, 40, 40)}


def _col(rgb):
    return Color.FromArgb(rgb[0], rgb[1], rgb[2])


def icon_bitmap(name, scale=1):
    """The colorful pixel-art icon rendered to a transparent WinForms Bitmap."""
    from System.Drawing import Graphics
    size = 16 * scale
    bmp = Bitmap(size, size)
    g = Graphics.FromImage(bmp)
    g.Clear(Color.Transparent)
    for (dx, dy, w, h, key) in ICONS.get(name, ()):
        br = SolidBrush(_col(ICON_PAL[key]))
        g.FillRectangle(br, dx * scale, dy * scale, w * scale, h * scale)
        br.Dispose()
    g.Dispose()
    return bmp


class Workbench(object):
    W, H = 1000, 636

    def __init__(self):
        self.cfg = _load_cfg()
        self.target = "win95"
        self.running = False
        self.cmd = None
        self.panel_name = "analyze"
        self._q = queue.Queue()
        self.uifont = SystemFonts.MessageBoxFont                 # native UI font (Segoe UI on 10/11)
        self.boldfont = Font(self.uifont, FontStyle.Bold)
        self.headfont = Font(self.uifont.FontFamily, 13.5, FontStyle.Bold)
        self.monofont = Font("Consolas", 9.5)
        self._build()

    # ------------------------------------------------------------------ build
    def _build(self):
        f = Form(); self.form = f
        f.Text = "CodeBreaker"
        f.FormBorderStyle = FormBorderStyle.Sizable              # native OS frame, resizable
        f.ClientSize = Size(self.W, self.H)
        f.MinimumSize = f.Size                                   # can grow, not shrink below design
        f.StartPosition = FormStartPosition.CenterScreen
        f.Font = self.uifont

        # ----- toolbar (docked top) -----
        tool = Panel(); tool.Dock = DockStyle.Top; tool.Height = 40
        tool.BackColor = SystemColors.Control
        f.Controls.Add(tool)
        x = 6
        for label, ic, act in (("Open", "folder", ("analyze_browse",)),
                               ("Analyze", "lens", ("nav", "Analyze a binary")),
                               ("Diff", "diff", ("nav", "Compare versions (Diff)")),
                               ("Xbox", "xbox", ("nav", "Xbox 360")),
                               ("Stop", "stop", ("stop",))):
            b = Button()
            b.Text = "  " + label
            b.Image = icon_bitmap(ic)
            b.TextImageRelation = TextImageRelation.ImageBeforeText
            b.ImageAlign = ContentAlignment.MiddleLeft
            b.FlatStyle = FlatStyle.Standard
            b.SetBounds(x, 7, 84, 26)
            b.Tag = act
            b.Click += self._on_button
            tool.Controls.Add(b)
            x += b.Width + 4

        # ----- status bar (native StatusStrip, docked bottom) -----
        ss = StatusStrip(); self.status = ss
        self.sl_status = ToolStripStatusLabel("Ready"); self.sl_status.Spring = True
        self.sl_status.TextAlign = ContentAlignment.MiddleLeft
        self.sl_oracle = ToolStripStatusLabel("Oracle: -")
        self.sl_target = ToolStripStatusLabel("Target: win95")
        self.sl_target.IsLink = True; self.sl_target.Click += self._cycle_target
        ss.Items.Add(self.sl_status); ss.Items.Add(self.sl_oracle); ss.Items.Add(self.sl_target)
        f.Controls.Add(ss)

        midtop, midbot = 40, self.H - 24

        # ----- nav tree (left) -----
        tv = TreeView()
        tv.SetBounds(0, midtop, 228, midbot - midtop)
        tv.BorderStyle = BorderStyle.FixedSingle
        tv.HideSelection = False; tv.FullRowSelect = True; tv.ItemHeight = 22
        tv.ShowLines = False; tv.ShowRootLines = False
        tv.DrawMode = TreeViewDrawMode.OwnerDrawText; tv.DrawNode += self._draw_node
        il = ImageList(); il.ImageSize = Size(16, 16); il.TransparentColor = Color.Transparent
        self._icon_index = {}
        for nm in ("binary", "disasm", "cfile", "inverse", "chip", "gear", "diff", "xbox",
                   "backbone", "sdk", "settings"):
            self._icon_index[nm] = il.Images.Count
            il.Images.Add(icon_bitmap(nm))
        blank_idx = il.Images.Count; il.Images.Add(Bitmap(16, 16))
        tv.ImageList = il
        group = None
        for entry in NAV:
            if entry[0] == "grp":
                group = TreeNode(entry[1]); group.ForeColor = Color.FromArgb(90, 90, 90)
                group.ImageIndex = group.SelectedImageIndex = blank_idx
                tv.Nodes.Add(group)
            else:
                _, label, icon, action = entry
                n = TreeNode(label)
                n.ImageIndex = n.SelectedImageIndex = self._icon_index.get(icon, 0)
                n.Tag = action
                (group.Nodes if group is not None else tv.Nodes).Add(n)
        tv.ExpandAll()
        tv.AfterSelect += self._on_nav
        tv.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left
        f.Controls.Add(tv); self.nav = tv

        # ----- main host + panels -----
        host = Panel(); host.SetBounds(232, midtop, self.W - 232, midbot - midtop)
        host.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
        f.Controls.Add(host); self.host = host
        self._panels = {"analyze": self._build_analyze(host),
                        "command": self._build_command(host),
                        "settings": self._build_settings(host)}
        for nm, p in self._panels.items():
            p.Visible = (nm == "analyze")

        f.AllowDrop = True
        f.DragEnter += self._drag_enter; f.DragDrop += self._drag_drop
        self._refresh_status()

    # --------------------------------------------------------- analyze panel
    def _build_analyze(self, host):
        p = Panel(); p.Dock = DockStyle.Fill; self._an_panel = p
        cwid = host.Width - 20
        head = Label(); head.Text = "Take a program apart"; head.Font = self.headfont
        head.SetBounds(10, 10, cwid, 26); head.AutoSize = True; p.Controls.Add(head)
        self.an_blurb = Label(); self.an_blurb.SetBounds(10, 42, cwid, 34)
        self.an_blurb.Text = ("Drop a Windows or Xbox 360 program below. CodeBreaker works out what it is on "
                              "its own, then shows you - in plain terms - what it can do with it. You don't "
                              "need to know the file format.")
        p.Controls.Add(self.an_blurb)
        drop = Panel(); drop.SetBounds(10, 82, cwid, 96); self.an_drop = drop
        drop.BorderStyle = BorderStyle.FixedSingle; drop.BackColor = Color.White
        drop.AllowDrop = True; drop.Click += self._analyze_click
        drop.DragEnter += self._drag_enter; drop.DragDrop += self._drag_drop
        self.an_pic = PictureBox(); self.an_pic.Image = icon_bitmap("floppy", 2)
        self.an_pic.SizeMode = PictureBoxSizeMode.AutoSize; self.an_pic.BackColor = Color.White
        self.an_pic.SetBounds(drop.Width // 2 - 16, 12, 32, 32)
        self.an_pic.Click += self._analyze_click; drop.Controls.Add(self.an_pic)
        self.drop_title = Label(); self.drop_title.Text = "Drop a program here to take it apart"
        self.drop_title.Font = self.boldfont; self.drop_title.TextAlign = ContentAlignment.MiddleCenter
        self.drop_title.SetBounds(0, 48, drop.Width, 18); self.drop_title.BackColor = Color.White
        self.drop_title.Click += self._analyze_click; drop.Controls.Add(self.drop_title)
        self.an_hint = Label(); self.an_hint.Text = "or click to browse    .exe  .dll  .drv  .vxd  .386  default.xex"
        self.an_hint.ForeColor = Color.Gray; self.an_hint.TextAlign = ContentAlignment.MiddleCenter
        self.an_hint.SetBounds(0, 68, drop.Width, 16); self.an_hint.BackColor = Color.White
        self.an_hint.Click += self._analyze_click; drop.Controls.Add(self.an_hint)
        p.Controls.Add(drop)
        self.detected = Panel(); self.detected.SetBounds(10, 186, cwid, 26)
        self.detected.BorderStyle = BorderStyle.FixedSingle; self.detected.Visible = False
        self.det_lbl = Label(); self.det_lbl.SetBounds(8, 5, 420, 16); self.det_lbl.Font = self.boldfont
        self.detected.Controls.Add(self.det_lbl)
        self.det_tag = Label(); self.det_tag.SetBounds(440, 4, 130, 18); self.det_tag.BackColor = _col((0, 90, 200))
        self.det_tag.ForeColor = Color.White; self.det_tag.TextAlign = ContentAlignment.MiddleCenter
        self.det_tag.Anchor = AnchorStyles.Top | AnchorStyles.Right
        self.detected.Controls.Add(self.det_tag)
        p.Controls.Add(self.detected)
        self._cards = []
        cw = (cwid - 8) // 2
        for idx, (ct, cd, cic, cact) in enumerate(CARDS):
            card = self._make_card(ct, cd, cic, cact, cw, 58)
            card.Location = Point(10 + (idx % 2) * (cw + 8), 220 + (idx // 2) * 66)
            p.Controls.Add(card); self._cards.append(card)
        self.prog = Label(); self.prog.SetBounds(10, 354, cwid, 16); self.prog.ForeColor = Color.Gray
        self.prog.Visible = False; p.Controls.Add(self.prog)
        p.Resize += self._reflow
        host.Controls.Add(p)
        return p

    def _reflow(self, sender=None, e=None):
        if not hasattr(self, "_an_panel"):
            return
        cwid = self._an_panel.Width - 20
        if cwid < 120:
            return
        for c in (self.an_blurb, self.an_drop, self.detected, self.prog):
            c.Width = cwid
        self.drop_title.Width = self.an_hint.Width = self.an_drop.Width
        self.an_pic.Left = self.an_drop.Width // 2 - 16
        cw = (cwid - 8) // 2
        for idx, card in enumerate(self._cards):
            card.SetBounds(10 + (idx % 2) * (cw + 8), 220 + (idx // 2) * 66, cw, 58)

    def _make_card(self, title, desc, icon, action, w, h):
        card = Panel(); card.Size = Size(w, h); card.Tag = action
        card.BorderStyle = BorderStyle.FixedSingle; card.BackColor = Color.White
        card.Cursor = self._hand(); card.Click += self._on_card
        pic = PictureBox(); pic.Image = icon_bitmap(icon); pic.SizeMode = PictureBoxSizeMode.AutoSize
        pic.SetBounds(12, 13, 16, 16); pic.BackColor = Color.White; pic.Tag = action
        pic.Click += self._on_card; card.Controls.Add(pic)
        t = Label(); t.Text = title; t.Font = self.boldfont; t.SetBounds(36, 10, w - 46, 18)
        t.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right
        t.BackColor = Color.White; t.Tag = action; t.Click += self._on_card; card.Controls.Add(t)
        d = Label(); d.Text = desc; d.SetBounds(36, 30, w - 46, 22); d.ForeColor = Color.FromArgb(80, 80, 80)
        d.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right
        d.BackColor = Color.White; d.Tag = action; d.Click += self._on_card; card.Controls.Add(d)
        return card

    # --------------------------------------------------------- command panel
    def _build_command(self, host):
        p = Panel(); p.Dock = DockStyle.Fill
        cwid = host.Width - 20
        self.cmd_head = Label(); self.cmd_head.Font = self.headfont; self.cmd_head.SetBounds(10, 10, 600, 24)
        self.cmd_head.AutoSize = True; p.Controls.Add(self.cmd_head)
        self.cmd_blurb = Label(); self.cmd_blurb.SetBounds(10, 40, cwid, 30)
        self.cmd_blurb.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right; p.Controls.Add(self.cmd_blurb)
        lin = Label(); lin.Text = "Input"; lin.SetBounds(10, 78, 54, 20); p.Controls.Add(lin)
        self.cmd_input = TextBox(); self.cmd_input.SetBounds(66, 76, cwid - 66 - 88, 23)
        self.cmd_input.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right; p.Controls.Add(self.cmd_input)
        bin = Button(); bin.Text = "Browse"; bin.SetBounds(10 + cwid - 82, 75, 82, 25)
        bin.Anchor = AnchorStyles.Top | AnchorStyles.Right; bin.Click += self._cmd_browse; p.Controls.Add(bin)
        la = Label(); la.Text = "Arguments"; la.SetBounds(10, 108, 70, 20); p.Controls.Add(la)
        self.cmd_args = TextBox(); self.cmd_args.SetBounds(86, 106, cwid - 86, 23)
        self.cmd_args.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right; p.Controls.Add(self.cmd_args)
        run = Button(); run.Text = "Run"; run.Font = self.boldfont; run.SetBounds(10 + cwid - 176, 138, 84, 26)
        run.Anchor = AnchorStyles.Top | AnchorStyles.Right; run.Click += self._run_command; p.Controls.Add(run)
        stop = Button(); stop.Text = "Stop"; stop.SetBounds(10 + cwid - 86, 138, 86, 26)
        stop.Anchor = AnchorStyles.Top | AnchorStyles.Right; stop.Click += self._stop; p.Controls.Add(stop)
        lo = Label(); lo.Text = "Output"; lo.Font = self.boldfont; lo.SetBounds(10, 170, 200, 16); p.Controls.Add(lo)
        self.console = TextBox(); self.console.Multiline = True; self.console.ReadOnly = True
        self.console.ScrollBars = ScrollBars.Both; self.console.BackColor = Color.White
        self.console.Font = self.monofont
        self.console.SetBounds(10, 190, cwid, host.Height - 190 - 10)
        self.console.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Bottom
        p.Controls.Add(self.console)
        host.Controls.Add(p)
        return p

    # --------------------------------------------------------- settings panel
    def _build_settings(self, host):
        p = Panel(); p.Dock = DockStyle.Fill
        cwid = host.Width - 20
        head = Label(); head.Text = "Attach your tools"; head.Font = self.headfont
        head.SetBounds(10, 10, 400, 24); head.AutoSize = True; p.Controls.Add(head)
        blurb = Label(); blurb.SetBounds(10, 40, cwid, 32)
        blurb.Text = ("CodeBreaker ships as the tool only - point it at the compilers you have. "
                      "Everything works without these except the byte-for-byte rebuild loop.")
        blurb.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right; p.Controls.Add(blurb)
        self._dep_dots = []
        y = 78
        for key, disp, url, optional, note in DEPS:
            dot = Panel(); dot.SetBounds(12, y + 4, 11, 11); dot.BorderStyle = BorderStyle.FixedSingle
            p.Controls.Add(dot); self._dep_dots.append((dot, key, optional))
            nm = Label(); nm.Text = disp; nm.Font = self.boldfont; nm.SetBounds(32, y, 168, 18); p.Controls.Add(nm)
            path = TextBox(); path.ReadOnly = True; path.SetBounds(204, y - 2, cwid - 204 - 78, 23)
            path.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right
            path.Text = self.cfg.get(key, "") or note; p.Controls.Add(path)
            g = Button(); g.Text = "Get it"; g.SetBounds(10 + cwid - 72, y - 3, 72, 24)
            g.Anchor = AnchorStyles.Top | AnchorStyles.Right
            g.Tag = url; g.Click += self._on_geturl; p.Controls.Add(g)
            y += 30
        rd = Button(); rd.Text = "Run Doctor"; rd.Font = self.boldfont; rd.SetBounds(10, y + 8, 116, 26)
        rd.Click += self._run_doctor; p.Controls.Add(rd)
        oc = Button(); oc.Text = "Open config"; oc.SetBounds(134, y + 8, 116, 26)
        oc.Tag = os.path.join(_ROOT, "config.json"); oc.Click += self._on_openpath; p.Controls.Add(oc)
        self.set_console = TextBox(); self.set_console.Multiline = True; self.set_console.ReadOnly = True
        self.set_console.ScrollBars = ScrollBars.Vertical; self.set_console.BackColor = Color.White
        self.set_console.Font = self.monofont
        self.set_console.SetBounds(10, y + 42, cwid, host.Height - (y + 42) - 10)
        self.set_console.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Bottom
        p.Controls.Add(self.set_console)
        host.Controls.Add(p)
        self._refresh_dep_dots()
        return p

    def _refresh_dep_dots(self):
        import shutil
        for dot, key, optional in self._dep_dots:
            val = self.cfg.get(key, "")
            ok = bool(val) and (os.path.exists(val) or (shutil.which(val) if val else None))
            dot.BackColor = _col((0, 160, 0)) if ok else (_col((220, 150, 30)) if optional else _col((200, 50, 50)))

    def _hand(self):
        from System.Windows.Forms import Cursors
        return Cursors.Hand

    # ----------------------------------------------------- owner-drawn nav text
    def _draw_node(self, sender, e):
        node = e.Node
        if e.Bounds.Width <= 0:
            e.DrawDefault = True; return
        is_group = node.Tag is None
        font = self.boldfont if is_group else self.uifont
        selected = e.State.HasFlag(TreeNodeStates.Selected)
        col = (SystemColors.HighlightText if selected else
               (node.ForeColor if not node.ForeColor.IsEmpty else SystemColors.WindowText))
        e.Graphics.DrawString(node.Text, font, SolidBrush(col), float(e.Bounds.X), float(e.Bounds.Y + 2))

    # --------------------------------------------------------------- events
    def _show_panel(self, name):
        self.panel_name = name
        for nm, p in self._panels.items():
            p.Visible = (nm == name)

    def _on_nav(self, sender, e):
        if e.Node.Tag is not None:
            self._dispatch(e.Node.Tag)

    def _select_nav(self, label):
        def walk(nodes):
            for n in nodes:
                if n.Text == label and n.Tag is not None:
                    return n
                r = walk(n.Nodes)
                if r:
                    return r
            return None
        n = walk(self.nav.Nodes)
        if n:
            self.nav.SelectedNode = n

    def _on_button(self, sender, e):
        self._dispatch(sender.Tag)

    def _on_card(self, sender, e):
        self._dispatch(sender.Tag)

    def _on_geturl(self, sender, e):
        self._open(sender.Tag)

    def _on_openpath(self, sender, e):
        self._open(sender.Tag)

    def _cycle_target(self, sender, e):
        eras = ["win1.0", "win3.1", "win95", "win98"]
        self.target = eras[(eras.index(self.target) + 1) % len(eras)] if self.target in eras else "win95"
        self._refresh_status()

    def _dispatch(self, action):
        k = action[0]
        if k == "analyze":
            self._show_panel("analyze")
        elif k == "settings":
            self.cfg = _load_cfg(); self._refresh_dep_dots(); self._show_panel("settings")
        elif k == "cmd":
            self.cmd = (action[1], action[3])
            self.cmd_head.Text = "wt " + action[1]; self.cmd_blurb.Text = action[3]
            self.cmd_args.Text = action[2]; self.cmd_input.Text = ""; self.console.Text = ""
            self._show_panel("command")
        elif k == "nav":
            self._select_nav(action[1])
        elif k == "analyze_browse":
            self._show_panel("analyze"); self._analyze(self._browse(
                "Windows binaries|*.exe;*.dll;*.drv;*.vxd;*.386;*.ocx;*.cpl;*.sys|Xbox 360|*.xex|All files|*.*"))
        elif k == "stop":
            self.running = False; self._set_status("Stopped.")

    def _browse(self, filt="All files|*.*"):
        from System.Windows.Forms import OpenFileDialog, DialogResult
        dlg = OpenFileDialog(); dlg.Filter = filt; dlg.Title = "Select a file"
        return dlg.FileName if dlg.ShowDialog() == DialogResult.OK else None

    def _cmd_browse(self, sender, e):
        p = self._browse()
        if p:
            self.cmd_input.Text = p

    def _analyze_click(self, sender, e):
        self._analyze(self._browse(
            "Windows binaries|*.exe;*.dll;*.drv;*.vxd;*.386;*.ocx;*.cpl;*.sys|Xbox 360|*.xex|All files|*.*"))

    def _open(self, path):
        try:
            os.startfile(path)
        except Exception:
            pass

    def _drag_enter(self, sender, e):
        if e.Data.GetDataPresent(DataFormats.FileDrop):
            e.Effect = DragDropEffects.Copy

    def _drag_drop(self, sender, e):
        files = e.Data.GetData(DataFormats.FileDrop)
        if files is not None and len(files) > 0:
            self._show_panel("analyze"); self._analyze(files[0])

    # ----- UI-thread marshaling + status -----
    def _ui(self, func):
        try:
            if self.form.IsHandleCreated:
                self.form.BeginInvoke(Action(func))
        except Exception:
            pass

    def _set_status(self, text):
        self._ui(lambda: setattr(self.sl_status, "Text", text))

    def _refresh_status(self):
        self.sl_target.Text = "Target: " + self.target
        ok = bool(self.cfg.get("msc203_oracle"))
        self.sl_oracle.Text = "Oracle: connected" if ok else "Oracle: not set"
        self.sl_oracle.ForeColor = _col((0, 130, 0)) if ok else _col((150, 30, 30))

    def _desktop(self):
        home = os.path.expanduser("~")
        for d in (os.path.join(home, "Desktop"), os.path.join(home, "OneDrive", "Desktop")):
            if os.path.isdir(d):
                return d
        return home

    def _analyze(self, path):
        if not path or self.running:
            return
        try:
            from engine.studio import pipeline
        except Exception as ex:
            self._set_status("pipeline unavailable: %s" % ex); return
        self.running = True
        self.drop_title.Text = os.path.basename(path)
        self._set_status("Analyzing " + os.path.basename(path))
        self._ui(lambda: setattr(self.prog, "Visible", True))

        def prog(kind, *a):
            if kind == "stage":
                cap = (a[1] if len(a) > 1 else "")
                self._ui(lambda: setattr(self.prog, "Text", "  " + cap))

        def worker():
            try:
                rep = pipeline.reverse(path, progress=prog, outbase=self._desktop())
            except Exception as ex:
                rep = {"name": os.path.basename(path), "error": str(ex)}
            self.running = False
            self._ui(lambda: self._analyzed(rep))
        threading.Thread(target=worker, daemon=True).start()

    def _analyzed(self, rep):
        self.prog.Visible = False; self._set_status("Ready")
        if rep.get("format"):
            self.det_lbl.Text = "  " + rep.get("name", "") + "      just analyzed"
            self.det_tag.Text = rep.get("format", ""); self.detected.Visible = True
        elif rep.get("error"):
            self.det_lbl.Text = "  " + rep.get("name", "") + " - error"
            self.det_tag.Text = "error"; self.detected.Visible = True

    def _stream(self, box, argv, done="Done."):
        box.Text = ""; self.running = True
        self._set_status("Running: " + " ".join(argv[-3:]))

        def worker():
            try:
                p = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     cwd=_ROOT, text=True, bufsize=1, creationflags=0x08000000)
                for line in iter(p.stdout.readline, ""):
                    self._ui(lambda l=line: box.AppendText(l))
                p.wait()
            except Exception as ex:
                self._ui(lambda: box.AppendText("[error] %s\r\n" % ex))
            self.running = False; self._set_status(done)
        threading.Thread(target=worker, daemon=True).start()

    def _run_command(self, sender, e):
        if self.running or not self.cmd:
            return
        rest = [self.cmd[0]]
        if self.cmd_input.Text:
            rest.append(self.cmd_input.Text)
        try:
            rest += shlex.split(self.cmd_args.Text)
        except Exception:
            rest += self.cmd_args.Text.split()
        self._stream(self.console, _wt_argv(rest))

    def _run_doctor(self, sender, e):
        if not self.running:
            self._stream(self.set_console, _wt_argv(["doctor"]))

    def _stop(self, sender, e):
        self.running = False; self._set_status("Stopped.")

    # -------------------------------------------------------------- run/shot
    def run(self):
        Application.Run(self.form)

    def shot(self, path, panel):
        if panel == "command":
            self._dispatch(("cmd", "msc6", "list", "Reconstruct 16-bit NE DLLs to source that re-links to the exact bytes."))
            self.console.Text = ("$ wt msc6 list\r\n\r\nmsc6-engine commands (engine/msc6):\r\n"
                                 "  detect_recipe\r\n  ne_normalize\r\n  skeleton\r\n  tree\r\n")
        elif panel == "settings":
            self._dispatch(("settings",))
        elif panel == "analyze":
            self._analyzed({"name": "SHELL.DLL", "format": "NE (16-bit)"})
        self.form.Show(); Application.DoEvents()
        bmp = Bitmap(self.form.Width, self.form.Height)
        self.form.DrawToBitmap(bmp, Rectangle(0, 0, self.form.Width, self.form.Height))
        bmp.Save(path, ImageFormat.Png)
        self.form.Hide(); self.form.Dispose()
        print("winforms: rendered %dx%d -> %s" % (self.form.Width, self.form.Height, path))


def _configure():
    try:
        Application.EnableVisualStyles()                         # native OS theme
        Application.SetCompatibleTextRenderingDefault(False)
    except Exception:
        pass
    try:
        from System.Windows.Forms import UnhandledExceptionMode
        Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException)
        Application.ThreadException += (lambda s, e: sys.stderr.write("UI exception: %s\n" % e.Exception))
    except Exception:
        pass


def main(argv=None):
    argv = argv or []
    if sys.platform != "win32":
        print("CodeBreaker Workbench is a native Windows UI."); return

    def body():
        _configure()
        if "--selftest" in argv:
            wb = Workbench(); wb.form.Dispose()
            print("winforms: form + controls built/disposed OK (WinForms plumbing sound)"); return
        if "--shot" in argv:
            i = argv.index("--shot")
            path = argv[i + 1] if i + 1 < len(argv) else "codebreaker.png"
            panel = argv[i + 2] if i + 2 < len(argv) and not argv[i + 2].startswith("-") else "analyze"
            Workbench().shot(path, panel); return
        Workbench().run()

    # WinForms (OLE drag-drop, clipboard, dialogs) requires a Single-Threaded-Apartment UI thread.
    from System.Threading import Thread, ApartmentState, ThreadStart
    t = Thread(ThreadStart(body)); t.SetApartmentState(ApartmentState.STA)
    t.Start(); t.Join()


if __name__ == "__main__":
    main(sys.argv[1:])
