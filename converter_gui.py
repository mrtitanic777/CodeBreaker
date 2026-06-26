#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
converter_gui.py - Desktop control panel for the Java -> Xbox 360 C++ converter.

PySide6 window over the existing tools:
  * SCAN     - syntax-check new .java (parser) + diff vs baseline -> color-coded tree + metric cards,
               plus a CLOBBER guard (warns if reconverting a changed file would lose live hand-fixes).
  * file     - click a changed file -> METHOD-level diff summary + unified diff (+/- colored).
  * CONVERT  - runs the converter IN-PROCESS (java2xbox360.convert_tree) on a worker thread -> works
               frozen; auto-runs the LINT afterward.
  * LINT     - scans converted output for known console-crash patterns -> Lint warnings tab.
  * BUILD    - MSBuild (Release|Xbox 360) via QProcess; parses errors into a clickable Build-errors tab.
  * PROMOTE  - rolls the baseline forward.

Run from source: python converter_gui.py     Build a .exe: build_gui.ps1
"""

import os
import re
import sys
import html
import logging
import difflib
import collections

HERE = os.path.dirname(os.path.abspath(__file__))
if getattr(sys, 'frozen', False):
    HERE = os.path.dirname(sys.executable)
REPO_ROOT = os.path.abspath(os.path.join(HERE, '..'))
sys.path.insert(0, HERE)
if getattr(sys, '_MEIPASS', None):
    sys.path.insert(0, sys._MEIPASS)

try:
    from PySide6.QtCore import Qt, QProcess, QThread, Signal, QSettings
    from PySide6.QtGui import QFont, QColor
    from PySide6.QtWidgets import (
        QApplication, QMainWindow, QWidget, QLabel, QPushButton, QComboBox, QLineEdit,
        QFileDialog, QTreeWidget, QTreeWidgetItem, QTextEdit, QPlainTextEdit, QSplitter,
        QHBoxLayout, QVBoxLayout, QFrame, QSizePolicy, QTabWidget, QListWidget, QListWidgetItem,
        QInputDialog,
    )
except ImportError:
    sys.stderr.write('PySide6 is required.  Install it with:\n    python -m pip install pyside6\n')
    sys.exit(1)

import java2xbox360 as conv            # noqa: E402
import update_version as upd           # noqa: E402
import converter_lint as lint          # noqa: E402

if sys.stdout is None:
    sys.stdout = open(os.devnull, 'w')
if sys.stderr is None:
    sys.stderr = open(os.devnull, 'w')

PROJECT_DIR = r'C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha'
CONVTOOL_DIR = os.path.join(PROJECT_DIR, 'ConversionTool')
LIVE_DIR = os.path.join(PROJECT_DIR, 'MinecraftAlpha')          # live VS project (hand-patched .cpp)
HANDWRITTEN_DIR = os.path.join(CONVTOOL_DIR, 'handwritten')
DEFAULT_NEW_JAVA = r'C:\Users\mrtit\Desktop\MinecraftAlpha'
DEFAULT_BASELINE = os.path.join(CONVTOOL_DIR, 'original_java')
DEFAULT_OUT = os.path.join(CONVTOOL_DIR, 'output')
DEFAULT_SLN = os.path.join(PROJECT_DIR, 'MinecraftAlpha.sln')
DEFAULT_MSBUILD = r'C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe'

OK, FAIL, ADD, DEL, CHG, INFO, MUTE = (
    '#4caf78', '#e24b4a', '#4caf78', '#e24b4a', '#ef9f27', '#378add', '#9a9a93')
SEV_COLOR = {'CRASH': '#e24b4a', 'LEAK': '#ef9f27', 'WARN': '#9a9a93'}

_ERR_RE = re.compile(r'^\s*(.*?)\((\d+)(?:,\d+)?\)\s*:\s*(fatal error|error|warning)\s+(\w+)\s*:\s*(.*)$')

STYLE = """
* { font-family: 'Segoe UI', sans-serif; font-size: 13px; color: #e2e0d8; }
QMainWindow, QWidget { background: #1c1c1a; }
QLabel#title { font-size: 16px; }
QFrame#card, QFrame#strip { background: #262624; border: 1px solid #3a3a37; border-radius: 8px; }
QLineEdit, QComboBox { background: #1c1c1a; border: 1px solid #3a3a37; border-radius: 6px; padding: 5px 8px; }
QPushButton { background: #2d2d2a; border: 1px solid #44443f; border-radius: 6px; padding: 6px 12px; }
QPushButton:hover { background: #383834; }
QPushButton#primary { background: #2f5d3f; border: 1px solid #3c7a52; }
QPushButton#primary:hover { background: #376e4a; }
QTreeWidget, QTextEdit, QPlainTextEdit, QListWidget { background: #161614; border: 1px solid #3a3a37; border-radius: 8px; }
QTreeWidget::item { padding: 3px 2px; }
QTreeWidget::item:selected, QListWidget::item:selected { background: #2f3a4a; }
QTabWidget::pane { border: 1px solid #3a3a37; border-radius: 8px; top: -1px; }
QTabBar::tab { background: #262624; color: #9a9a93; padding: 5px 14px; border: 1px solid #3a3a37; border-bottom: none; border-top-left-radius: 6px; border-top-right-radius: 6px; }
QTabBar::tab:selected { background: #161614; color: #e2e0d8; }
QSplitter::handle { background: #1c1c1a; }
"""


class _LogToSignal(logging.Handler):
    def __init__(self, sig):
        super().__init__()
        self.sig = sig

    def emit(self, record):
        try:
            self.sig.emit(self.format(record))
        except Exception:
            pass


class ConvertWorker(QThread):
    line = Signal(str)
    done = Signal(int, str)

    def __init__(self, newdir, outdir, version):
        super().__init__()
        self.newdir, self.outdir, self.version = newdir, outdir, version

    def run(self):
        h = _LogToSignal(self.line)
        h.setFormatter(logging.Formatter('%(message)s'))
        try:
            conv.convert_tree(self.newdir, self.outdir, self.version, log_handler=h)
            self.done.emit(0, 'convert complete')
        except SystemExit as e:
            self.done.emit(int(getattr(e, 'code', 1) or 0), 'convert exited')
        except Exception as e:                       # noqa: BLE001
            self.line.emit('ERROR: %s: %s' % (type(e).__name__, e))
            self.done.emit(1, 'convert failed')
        finally:
            try:
                conv.log.removeHandler(h)
            except Exception:
                pass


def card(label_text, color='#e2e0d8'):
    f = QFrame(); f.setObjectName('card')
    lay = QVBoxLayout(f); lay.setContentsMargins(12, 8, 12, 8); lay.setSpacing(2)
    cap = QLabel(label_text); cap.setStyleSheet('color:%s; font-size:12px;' % MUTE)
    val = QLabel('-'); val.setStyleSheet('color:%s; font-size:22px;' % color)
    lay.addWidget(cap); lay.addWidget(val)
    return f, val


class ConverterWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Minecraft 360 Converter')
        self.resize(1180, 820)
        self.setStyleSheet(STYLE)
        self.settings = QSettings('Minecraft360', 'ConverterGUI')
        self.new_files, self.base_files, self.changed = {}, {}, {}
        self.proc = None
        self.worker = None
        self._build_ui()

    def _build_ui(self):
        s = self.settings
        root = QWidget(); self.setCentralWidget(root)
        outer = QVBoxLayout(root); outer.setContentsMargins(14, 14, 14, 14); outer.setSpacing(10)

        head = QHBoxLayout()
        title = QLabel('Minecraft 360 Converter'); title.setObjectName('title')
        self.profile = QComboBox()
        try:
            self.profile.addItems(sorted(conv.VERSION_PROFILES.keys()))
        except Exception:
            self.profile.addItem('indev')
        self.profile.setCurrentText(s.value('profile', 'indev'))
        newp = QPushButton('New profile…'); newp.setFixedWidth(110); newp.clicked.connect(self.new_profile)
        newp.setToolTip('Create a custom version profile (clones the selected one as its base)')
        run = QPushButton('  Run update  '); run.setObjectName('primary'); run.clicked.connect(self.run_all)
        head.addWidget(title); head.addWidget(QLabel('profile:')); head.addWidget(self.profile)
        head.addWidget(newp)
        head.addStretch(1); head.addWidget(run)
        outer.addLayout(head)

        self.ed_new = QLineEdit(s.value('new', DEFAULT_NEW_JAVA))
        self.ed_new.setPlaceholderText('New version Java source tree...')
        self.ed_base = QLineEdit(s.value('base', DEFAULT_BASELINE))
        self.ed_out = QLineEdit(s.value('out', DEFAULT_OUT))
        outer.addLayout(self._path_row('New Java', self.ed_new))
        outer.addLayout(self._path_row('Baseline', self.ed_base))
        outer.addLayout(self._path_row('Output', self.ed_out))

        cards = QHBoxLayout(); cards.setSpacing(10)
        f1, self.m_ok = card('Parsed OK', OK)
        f2, self.m_fail = card('Failed', FAIL)
        f3, self.m_add = card('Added', ADD)
        f4, self.m_del = card('Deleted', DEL)
        f5, self.m_chg = card('Changed', CHG)
        f6, self.m_lint = card('Lint (crash)', FAIL)
        for f in (f1, f2, f3, f4, f5, f6):
            cards.addWidget(f)
        outer.addLayout(cards)

        strip = QFrame(); strip.setObjectName('strip')
        sl = QHBoxLayout(strip); sl.setContentsMargins(12, 8, 12, 8); sl.setSpacing(8)
        self.stages = {}
        names = ['Parse', 'Diff', 'Convert', 'Lint', 'Build XEX', 'Deploy']
        for i, n in enumerate(names):
            lab = QLabel(n); lab.setStyleSheet('color:%s;' % MUTE); self.stages[n] = lab
            sl.addWidget(lab)
            if i < len(names) - 1:
                sep = QLabel('>'); sep.setStyleSheet('color:#55554f;'); sl.addWidget(sep)
        sl.addStretch(1)
        outer.addWidget(strip)

        split = QSplitter(Qt.Horizontal)
        self.tree = QTreeWidget(); self.tree.setHeaderLabels(['Changed files', ''])
        self.tree.setColumnWidth(0, 300); self.tree.setRootIsDecorated(False)
        self.tree.currentItemChanged.connect(self.on_file_selected)
        self.diff = QTextEdit(); self.diff.setReadOnly(True); self.diff.setFont(QFont('Consolas', 10))
        split.addWidget(self.tree); split.addWidget(self.diff)
        split.setSizes([400, 740])
        split.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        outer.addWidget(split, 1)

        self.tabs = QTabWidget()
        self.log = QPlainTextEdit(); self.log.setReadOnly(True); self.log.setFont(QFont('Consolas', 9))
        self.lint_tree = QTreeWidget()
        self.lint_tree.setHeaderLabels(['Sev', 'File', 'Line', 'Issue'])
        self.lint_tree.setColumnWidth(0, 60); self.lint_tree.setColumnWidth(1, 170); self.lint_tree.setColumnWidth(2, 50)
        self.lint_tree.setRootIsDecorated(False)
        self.lint_tree.itemDoubleClicked.connect(self._open_lint_item)
        self.err_list = QListWidget(); self.err_list.itemDoubleClicked.connect(self._open_err_item)
        self.tabs.addTab(self.log, 'Log')
        self.tabs.addTab(self.lint_tree, 'Lint warnings')
        self.tabs.addTab(self.err_list, 'Build errors')
        self.tabs.setFixedHeight(200)
        outer.addWidget(self.tabs)

        btns = QHBoxLayout(); btns.setSpacing(8)
        for text, slot in [('Syntax check + diff', self.scan), ('Convert', self.run_convert),
                           ('Lint output', self.run_lint), ('Build XEX', self.run_build),
                           ('Open output', self.open_output)]:
            b = QPushButton(text); b.clicked.connect(slot); btns.addWidget(b)
        btns.addStretch(1)
        bp = QPushButton('Promote baseline'); bp.clicked.connect(self.promote); btns.addWidget(bp)
        outer.addLayout(btns)

    def _path_row(self, label, edit):
        row = QHBoxLayout()
        lab = QLabel(label); lab.setFixedWidth(70); lab.setStyleSheet('color:%s;' % MUTE)
        b = QPushButton('Browse'); b.setFixedWidth(80)
        b.clicked.connect(lambda: self._browse(edit))
        row.addWidget(lab); row.addWidget(edit, 1); row.addWidget(b)
        return row

    def _browse(self, edit):
        d = QFileDialog.getExistingDirectory(self, 'Select folder', edit.text() or HERE)
        if d:
            edit.setText(d)

    def closeEvent(self, e):
        s = self.settings
        s.setValue('new', self.ed_new.text()); s.setValue('base', self.ed_base.text())
        s.setValue('out', self.ed_out.text()); s.setValue('profile', self.profile.currentText())
        super().closeEvent(e)

    # ---- helpers --------------------------------------------------------
    def set_stage(self, name, state):
        colors = {'pending': MUTE, 'active': '#e2e0d8', 'done': OK, 'fail': FAIL}
        if name in self.stages:
            self.stages[name].setStyleSheet('color:%s;' % colors.get(state, MUTE))

    def logln(self, text):
        self.log.appendPlainText(text)
        self.log.verticalScrollBar().setValue(self.log.verticalScrollBar().maximum())

    @staticmethod
    def _open(path):
        try:
            if path and os.path.isfile(path):
                os.startfile(path)            # noqa: PLW1514 (Windows)
        except Exception:
            pass

    # ---- SCAN -----------------------------------------------------------
    def scan(self):
        newdir, basedir = self.ed_new.text().strip(), self.ed_base.text().strip()
        if not os.path.isdir(newdir):
            self.logln('! New Java dir not found: %s' % newdir); return
        try:
            conv.set_active_profile(self.profile.currentText()); conv.init_profile_globals()
        except Exception as e:
            self.logln('warn: profile init failed (%s)' % e)
        self.set_stage('Parse', 'active'); QApplication.processEvents()

        self.new_files = upd.list_java(newdir)
        self.base_files = upd.list_java(basedir)
        ok, fail = upd.syntax_check(self.new_files)
        self.m_ok.setText(str(len(ok))); self.m_fail.setText(str(len(fail)))
        self.set_stage('Parse', 'fail' if fail else 'done')

        ns, bs = set(self.new_files), set(self.base_files)
        added = sorted(ns - bs); deleted = sorted(bs - ns); common = sorted(ns & bs)
        self.changed = {}
        for rel in common:
            if upd.sha1_of(self.new_files[rel]) != upd.sha1_of(self.base_files[rel]):
                self.changed[rel] = upd.line_delta(self.base_files[rel], self.new_files[rel])
        self.m_add.setText(str(len(added))); self.m_del.setText(str(len(deleted)))
        self.m_chg.setText(str(len(self.changed)))
        self.set_stage('Diff', 'done')

        self.tree.clear()

        def item(rel, tag, color, note='', rel_data=True):
            it = QTreeWidgetItem(['%s  %s' % (tag, rel), note])
            it.setData(0, Qt.UserRole, rel if rel_data else None)
            it.setForeground(0, QColor(color))
            if note:
                it.setForeground(1, QColor(MUTE))
            return it

        for rel in added:
            self.tree.addTopLevelItem(item(rel, '+', ADD, rel_data=False))
        for rel in deleted:
            self.tree.addTopLevelItem(item(rel, '−', DEL, rel_data=False))
        for rel in sorted(self.changed):
            a, d = self.changed[rel]
            self.tree.addTopLevelItem(item(rel, '~', CHG, '+%d / -%d' % (a, d)))
        self.logln('scan: %d parsed OK, %d failed | +%d added, -%d deleted, ~%d changed'
                   % (len(ok), len(fail), len(added), len(deleted), len(self.changed)))
        for rel, why in fail:
            self.logln('  ! %s  (%s)' % (rel, why))

        # CLOBBER guard: changed files whose live C++ has hand-fixes a reconvert would drop.
        rep = upd.hand_fix_report(list(self.changed.keys()), LIVE_DIR, HANDWRITTEN_DIR)
        at_risk = [(c, n) for c, n, st in rep if st == 'at-risk']
        if at_risk:
            self.logln('CLOBBER WARNING: %d changed file(s) have live hand-fixes a reconvert would lose --' % len(at_risk))
            for c, n in at_risk:
                self.logln('  ~ %s.cpp: %d // PORT hand-fix(es); promote to handwritten/ before syncing output -> live' % (c, n))

    def on_file_selected(self, cur, _prev):
        if not cur:
            return
        rel = cur.data(0, Qt.UserRole)
        if not rel or rel not in self.changed:
            self.diff.setHtml('<span style="color:%s">(no diff &mdash; added / deleted / unchanged)</span>' % MUTE)
            return
        # method-level summary first
        hdr = ''
        a, r, ch = upd.method_diff(self.base_files[rel], self.new_files[rel])
        if a or r or ch:
            hdr = '<div style="color:%s">methods: +%d added / -%d removed / ~%d changed</div>' % (INFO, len(a), len(r), len(ch))
            for sgn, col, lst in (('+', ADD, a), ('−', DEL, r), ('~', CHG, ch)):
                for sig in lst:
                    hdr += '<div style="color:%s">%s %s</div>' % (col, sgn, html.escape(sig))
            hdr += '<div style="color:%s">&mdash;&mdash;&mdash;&mdash;&mdash;</div>' % MUTE
        # unified line diff
        a_lines = upd.read_text(self.base_files[rel]).splitlines(keepends=True)
        b_lines = upd.read_text(self.new_files[rel]).splitlines(keepends=True)
        rows = []
        for ln in difflib.unified_diff(a_lines, b_lines, 'baseline/' + rel, 'new/' + rel):
            esc = html.escape(ln.rstrip('\n'))
            if ln.startswith('+') and not ln.startswith('+++'):
                rows.append('<div style="background:#16341f;color:%s">%s</div>' % (ADD, esc))
            elif ln.startswith('-') and not ln.startswith('---'):
                rows.append('<div style="background:#3a1a1a;color:%s">%s</div>' % (DEL, esc))
            elif ln.startswith('@@'):
                rows.append('<div style="color:%s">%s</div>' % (INFO, esc))
            else:
                rows.append('<div style="color:#cfcdc6">%s</div>' % esc)
        self.diff.setHtml('<div style="font-family:Consolas,monospace;font-size:12px;line-height:1.5">%s%s</div>'
                          % (hdr, ''.join(rows)))

    # ---- LINT -----------------------------------------------------------
    def run_lint(self):
        outdir = self.ed_out.text().strip()
        if not os.path.isdir(outdir):
            self.logln('! output dir not found (convert first): %s' % outdir); return
        self.set_stage('Lint', 'active'); QApplication.processEvents()
        findings = lint.lint_tree(outdir)
        self.lint_tree.clear()
        for f in findings:
            it = QTreeWidgetItem([f.severity, os.path.basename(f.file), str(f.line), f.message])
            it.setForeground(0, QColor(SEV_COLOR.get(f.severity, MUTE)))
            it.setData(0, Qt.UserRole, (f.file, f.line))
            self.lint_tree.addTopLevelItem(it)
        c = collections.Counter(f.severity for f in findings)
        self.m_lint.setText(str(c.get('CRASH', 0)))
        self.set_stage('Lint', 'fail' if c.get('CRASH', 0) else 'done')
        self.logln('lint: %d findings (CRASH=%d LEAK=%d WARN=%d)'
                   % (len(findings), c.get('CRASH', 0), c.get('LEAK', 0), c.get('WARN', 0)))
        if findings:
            self.tabs.setCurrentWidget(self.lint_tree)

    def _open_lint_item(self, it, _col):
        data = it.data(0, Qt.UserRole)
        if data:
            self._open(data[0])

    # ---- CONVERT --------------------------------------------------------
    def run_convert(self):
        newdir, outdir = self.ed_new.text().strip(), self.ed_out.text().strip()
        if not os.path.isdir(newdir):
            self.logln('! New Java dir not found: %s' % newdir); return
        if self.worker and self.worker.isRunning():
            self.logln('! conversion already running'); return
        os.makedirs(outdir, exist_ok=True)
        self.set_stage('Convert', 'active')
        self.logln('converting %s -> %s ...' % (newdir, outdir))
        self.worker = ConvertWorker(newdir, outdir, self.profile.currentText())
        self.worker.line.connect(self.logln)
        self.worker.done.connect(self._convert_done)
        self.worker.start()

    def _convert_done(self, rc, msg):
        self.set_stage('Convert', 'done' if rc == 0 else 'fail')
        self.logln('[Convert] %s (rc=%d)' % (msg, rc))
        if rc == 0:
            self.run_lint()                # auto-lint the fresh output

    # ---- BUILD (QProcess -> MSBuild, parse errors) ----------------------
    def run_build(self):
        if self.proc is not None and self.proc.state() != QProcess.NotRunning:
            self.logln('! a build is already running'); return
        if not os.path.exists(DEFAULT_MSBUILD):
            self.logln('! MSBuild not found at %s' % DEFAULT_MSBUILD); self.set_stage('Build XEX', 'fail'); return
        self.err_list.clear()
        self.set_stage('Build XEX', 'active')
        self.logln('$ msbuild %s (Release|Xbox 360)' % os.path.basename(DEFAULT_SLN))
        self.proc = QProcess(self)
        self.proc.setWorkingDirectory(REPO_ROOT)
        self.proc.setProcessChannelMode(QProcess.MergedChannels)
        self.proc.readyReadStandardOutput.connect(self._read)
        self.proc.finished.connect(self._build_done)
        self.proc.start(DEFAULT_MSBUILD,
                        [DEFAULT_SLN, '/p:Configuration=Release', '/p:Platform=Xbox 360',
                         '/nologo', '/v:minimal', '/m'])

    def _read(self):
        if not self.proc:
            return
        data = bytes(self.proc.readAllStandardOutput()).decode('utf-8', 'replace')
        for line in data.splitlines():
            self.logln(line)
            m = _ERR_RE.match(line)
            if m and 'error' in m.group(3):
                path, ln, kind, code, msg = m.group(1).strip(), m.group(2), m.group(3), m.group(4), m.group(5)
                it = QListWidgetItem('%s(%s): %s %s: %s' % (os.path.basename(path), ln, kind, code, msg))
                it.setForeground(QColor(FAIL))
                it.setData(Qt.UserRole, (path, int(ln)))
                self.err_list.addItem(it)

    def _build_done(self, code, _status):
        self.set_stage('Build XEX', 'done' if code == 0 else 'fail')
        self.logln('[Build XEX] exit %d  (%d error line(s))' % (code, self.err_list.count()))
        if code != 0 and self.err_list.count():
            self.tabs.setCurrentWidget(self.err_list)
        self.proc = None

    def _open_err_item(self, it):
        data = it.data(Qt.UserRole)
        if not data:
            return
        path = data[0]
        if not os.path.isabs(path):
            path = os.path.join(LIVE_DIR, path)
        self._open(path)

    # ---- profiles -------------------------------------------------------
    def _refresh_profiles(self, select=None):
        """Reload custom profiles from disk and repopulate the combo (optionally select one)."""
        try:
            conv.load_external_profiles()
        except Exception as e:                       # noqa: BLE001
            self.logln('! profile reload failed: %s' % e)
        keep = select or self.profile.currentText()
        self.profile.blockSignals(True)
        self.profile.clear()
        self.profile.addItems(sorted(conv.VERSION_PROFILES.keys()))
        if keep in conv.VERSION_PROFILES:
            self.profile.setCurrentText(keep)
        self.profile.blockSignals(False)

    def new_profile(self):
        import json
        name, ok = QInputDialog.getText(self, 'New profile',
                                        'Profile name (lowercase, e.g. infdev):')
        if not ok:
            return
        name = name.strip().lower()
        if not name or not re.match(r'^[a-z0-9][a-z0-9_-]*$', name):
            self.logln('! invalid profile name (use lowercase letters/digits/_/-)'); return
        if name in conv.VERSION_PROFILES:
            self.logln('! profile "%s" already exists' % name); return
        base = self.profile.currentText() or 'indev'
        d = conv.profiles_dir()
        os.makedirs(d, exist_ok=True)
        path = os.path.join(d, name + '.json')
        template = {
            'base': base,
            '_help': "Inherits 'base'; list fields ADD to the base set, '<field>_remove' removes. "
                     'See profiles/README.txt for all keys.',
            'value_types': [], 'reference_types': [], 'interface_types': [],
            'skip_files': [], 'pure_logic_files': [], 'platform_file_classes': [],
            'required_output_files': [],
            'static_class_fields': {}, 'parent_fields': {},
            'extra_type_map': {}, 'extra_gl_map': {}, 'extra_math_map': {},
        }
        try:
            with open(path, 'w', encoding='utf-8') as f:
                json.dump(template, f, indent=2)
        except Exception as e:                       # noqa: BLE001
            self.logln('! could not write profile: %s' % e); return
        self.logln('created profile "%s" (base=%s) -> %s' % (name, base, path))
        self._refresh_profiles(select=name)
        self._open(path)                              # open in editor to fill in the deltas

    # ---- misc -----------------------------------------------------------
    def run_all(self):
        self.scan()
        if self.m_fail.text() not in ('-', '0'):
            self.logln('aborting run: syntax check has failures (use Convert to force)'); return
        self.run_convert()

    def promote(self):
        if not self.new_files:
            self.logln('! run a scan first'); return
        copied, removed = upd.promote_baseline(self.new_files, self.ed_base.text().strip())
        self.logln('promoted baseline: copied %d, removed %d' % (copied, removed))

    def open_output(self):
        out = self.ed_out.text().strip()
        if os.path.isdir(out):
            os.startfile(out)            # noqa: PLW1514 (Windows)
        else:
            self.logln('! output dir not found: %s' % out)


def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    w = ConverterWindow(); w.show()
    sys.exit(app.exec())


if __name__ == '__main__':
    main()
