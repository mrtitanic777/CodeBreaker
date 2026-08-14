# -*- coding: utf-8 -*-
"""
gui.py -- PySide6 desktop control panel for the j2x converter.

Wraps the full j2x pipeline (convert, report, signatures, worklist, vcxproj,
build) in a dark-themed window. Double-click j2x.exe to launch.
"""

import os
import re
import sys
import io
import contextlib

HERE = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(HERE)
if getattr(sys, 'frozen', False):
    REPO_ROOT = os.path.dirname(sys.executable)

try:
    from PySide6.QtCore import Qt, QProcess, QThread, Signal, QSettings
    from PySide6.QtGui import QFont, QColor
    from PySide6.QtWidgets import (
        QApplication, QMainWindow, QWidget, QLabel, QPushButton,
        QLineEdit, QFileDialog, QTreeWidget, QTreeWidgetItem,
        QPlainTextEdit, QTextEdit, QHBoxLayout, QVBoxLayout,
        QFrame, QSizePolicy, QTabWidget, QListWidget, QListWidgetItem,
        QHeaderView, QComboBox, QCheckBox, QScrollArea, QGridLayout,
        QButtonGroup, QStackedWidget, QDialog, QMessageBox,
    )
except ImportError:
    sys.stderr.write('PySide6 is required.  Install with:\n    pip install pyside6\n')
    sys.exit(1)

PROJECT_DIR = os.path.normpath(os.path.join(REPO_ROOT, '..'))
DEFAULT_JAVA = r'C:\Users\mrtit\Desktop\MinecraftAlpha\minecraft\src'
DEFAULT_OUT = os.path.join(REPO_ROOT, 'out')
DESKTOP = os.path.join(os.path.expanduser('~'), 'Desktop')    # browse dialogs start here by default
DEFAULT_SLN = os.path.join(PROJECT_DIR, 'MinecraftAlpha.sln')
DEFAULT_MSBUILD = r'C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe'

OK, FAIL, WARN, INFO, MUTE = '#4caf78', '#e24b4a', '#ef9f27', '#378add', '#9a9a93'

_CAT_COLORS = {
    'ERROR':      '#e24b4a',
    'GUESSED':    '#e88a3a',
    'STUBBED':    '#d46ab5',
    'BLOCKED':    '#e24b4a',
    'RISK':       '#ef9f27',
    'WARNING':    '#efcf27',
    'OVERRIDDEN': '#378add',
    'ANNOTATED':  '#7aafdd',
    'PORTED':     '#4caf78',
    'RULE':       '#9a9a93',
}

_CAT_LABELS = {
    'ERROR':      'Errors — compilation/emit failures',
    'GUESSED':    'Guessed — heuristic calls that could be wrong',
    'STUBBED':    'Stubbed — placeholder, needs hand implementation',
    'BLOCKED':    'Blocked — could not port',
    'RISK':       'Risk — matches known crash/leak pattern',
    'WARNING':    'Warning — suspicious, verify by hand',
    'OVERRIDDEN': 'Overridden — handwritten file used',
    'ANNOTATED':  'Annotated — explanatory notes',
    'PORTED':     'Ported — translated cleanly',
    'RULE':       'Rule — named fix/rule fired',
}

_ERR_RE = re.compile(
    r'^\s*(.*?)\((\d+)(?:,\d+)?\)\s*:\s*(fatal error|error|warning)\s+(\w+)\s*:\s*(.*)$')

STYLE = """
* { font-family: 'Segoe UI', 'Segoe UI Variable', sans-serif; font-size: 13px; color: #e7e5dd; }
QMainWindow { background: qlineargradient(x1:0, y1:0, x2:0.6, y2:1,
    stop:0 #191a20, stop:1 #141419); }
QWidget { background: transparent; }
QLabel#title { font-size: 17px; font-weight: bold; color: #f2f0e8; }
QLabel#hero { font-size: 25px; font-weight: bold; color: #f4f2ea; }

QFrame#card { background: #23242c; border: 1px solid #34353f; border-radius: 11px; }
QFrame#card:hover { border: 1px solid #4f8567; }
QFrame#strip { background: #23242c; border: 1px solid #34353f; border-radius: 11px; }

QLineEdit { background: #14141a; border: 1px solid #34353f; border-radius: 7px; padding: 6px 9px;
    selection-background-color: #2f5d3f; }
QLineEdit:focus { border: 1px solid #5cb37e; }
QLineEdit:hover { border: 1px solid #43454f; }

QPushButton { background: #2b2c34; border: 1px solid #45464f; border-radius: 7px; padding: 7px 15px;
    color: #dddbd3; }
QPushButton:hover { background: #34363f; border: 1px solid #5cb37e; }
QPushButton:pressed { background: #292a31; }
QPushButton:disabled { color: #55565e; border-color: #34353f; }
QPushButton#primary { background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
    stop:0 #3f855c, stop:1 #2f5d3f); border: 1px solid #57a577; color: #f0fbf4; font-weight: bold; }
QPushButton#primary:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
    stop:0 #4a9a6a, stop:1 #367049); border: 1px solid #6fc98e; }

QPushButton#choice { background: #1e1f26; border: 1px solid #34353f; border-radius: 7px;
    padding: 7px 13px; color: #bcbab2; text-align: left; }
QPushButton#choice:hover { background: #2a2b33; border: 1px solid #4f8567; }
QPushButton#choice:checked { background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
    stop:0 #327a52, stop:1 #2a5b3c); border: 1px solid #6fc98e; color: #eefaf1; font-weight: bold; }

QCheckBox { color: #d0cec6; spacing: 7px; }
QCheckBox::indicator { width: 16px; height: 16px; border: 1px solid #45464f; border-radius: 4px;
    background: #14141a; }
QCheckBox::indicator:checked { background: #3f855c; border: 1px solid #6fc98e; }

QComboBox { background: #14141a; border: 1px solid #34353f; border-radius: 7px; padding: 5px 9px;
    color: #e7e5dd; }
QComboBox:hover { border: 1px solid #4f8567; }
QComboBox::drop-down { border: none; width: 18px; }
QComboBox QAbstractItemView { background: #23242c; color: #e7e5dd; border: 1px solid #34353f;
    selection-background-color: #327a52; outline: none; }

QTreeWidget, QPlainTextEdit, QTextEdit, QListWidget {
    background: #121218; border: 1px solid #34353f; border-radius: 9px; }
QTreeWidget::item { padding: 3px 2px; }
QTreeWidget::item:selected, QListWidget::item:selected { background: #2b4a5e; }

QTabWidget::pane { border: 1px solid #34353f; border-radius: 9px; top: -1px; }
QTabBar::tab { background: #23242c; color: #9a9a93; padding: 6px 16px; margin-right: 2px;
    border: 1px solid #34353f; border-bottom: none;
    border-top-left-radius: 7px; border-top-right-radius: 7px; }
QTabBar::tab:hover { color: #d0cec6; }
QTabBar::tab:selected { background: #121218; color: #f2f0e8; border-bottom: 2px solid #5cb37e; }

QScrollBar:vertical { background: transparent; width: 11px; margin: 2px; }
QScrollBar::handle:vertical { background: #3a3b45; border-radius: 5px; min-height: 28px; }
QScrollBar::handle:vertical:hover { background: #4f8567; }
QScrollBar::add-line, QScrollBar::sub-line { height: 0; }
QSplitter::handle { background: #141419; }
QMessageBox, QDialog { background: #1a1b21; }
"""


def _card(label_text, color='#e2e0d8'):
    f = QFrame()
    f.setObjectName('card')
    lay = QVBoxLayout(f)
    lay.setContentsMargins(12, 8, 12, 8)
    lay.setSpacing(2)
    cap = QLabel(label_text)
    cap.setStyleSheet('color:%s; font-size:12px;' % MUTE)
    val = QLabel('-')
    val.setStyleSheet('color:%s; font-size:22px;' % color)
    lay.addWidget(cap)
    lay.addWidget(val)
    return f, val


class ConvertWorker(QThread):
    line = Signal(str)
    done = Signal(int, dict)

    def __init__(self, java_dir, out_dir, cpp_std='C++03', platform='xbox360',
                 emit_debug=True, sdk_override=None):
        super().__init__()
        self.java_dir = java_dir
        self.out_dir = out_dir
        self.cpp_std = cpp_std
        self.platform = platform
        self.emit_debug = emit_debug
        self.sdk_override = sdk_override

    def _progress(self, msg):
        self.line.emit(msg)
        self.msleep(1)  # yield so the GUI event loop can repaint live

    def run(self):
        try:
            from . import pipeline
            from .ledger import Ledger
            ledger = Ledger()
            self._progress('converting %s -> %s ...' % (self.java_dir, self.out_dir))
            s = pipeline.convert(
                self.java_dir, self.out_dir,
                ledger=ledger,
                cpp_std=self.cpp_std,
                platform=self.platform,
                emit_debug=self.emit_debug,
                sdk_override=self.sdk_override or None,
                progress_cb=self._progress)
            self.done.emit(0, s)
        except Exception as e:
            import traceback
            self._progress('ERROR: %s: %s' % (type(e).__name__, e))
            self._progress(traceback.format_exc())
            self.done.emit(1, {})


class PlatformDialog(QDialog):
    """Modal shown after 'Convert a project': pick the target platform. Selecting one returns its id/
    label; 'Other' explains more are coming and keeps the picker open."""
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Choose a platform')
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(520)
        self.platform_id = None
        self.platform_label = None

        from .platforms import PLATFORMS
        lay = QVBoxLayout(self)
        lay.setContentsMargins(24, 24, 24, 24)
        lay.setSpacing(12)
        q = QLabel('Which platform would you like to convert your project to?')
        q.setStyleSheet('font-size: 16px; font-weight: bold; color: #e2e0d8;')
        q.setWordWrap(True)
        lay.addWidget(q)
        hint = QLabel('The conversion tools you get are tailored to the platform you pick.')
        hint.setStyleSheet('font-size: 12px; color: %s;' % MUTE)
        hint.setWordWrap(True)
        lay.addWidget(hint)

        emoji = {'xbox360': '🎮', 'pc_windows': '🖥️', 'pc_opengl': '🖥️', 'ps3': '🎮',
                 'wii': '🕹️', 'psp': '🎮', 'nds': '🎮'}
        grid = QGridLayout()
        grid.setSpacing(10)
        # xbox360 is the fully-supported target; others are selectable but get the generic toolset.
        items = [(pid, PLATFORMS[pid]['label'], PLATFORMS[pid].get('description', ''))
                 for pid in PLATFORMS]
        for i, (pid, label, desc) in enumerate(items):
            b = QPushButton('%s  %s%s' % (emoji.get(pid, '🎯'), label,
                                          '   ★' if pid == 'xbox360' else ''))
            b.setToolTip(desc)
            b.setMinimumHeight(46)
            b.setCursor(Qt.PointingHandCursor)
            if pid == 'xbox360':
                b.setObjectName('primary')
            b.clicked.connect(lambda _c=False, p=pid, l=label: self._pick(p, l))
            grid.addWidget(b, i // 2, i % 2)
        other = QPushButton('➕  Other…')
        other.setMinimumHeight(46)
        other.setCursor(Qt.PointingHandCursor)
        other.setToolTip('More platforms are on the way')
        other.clicked.connect(self._other)
        grid.addWidget(other, len(items) // 2, len(items) % 2)
        lay.addLayout(grid)

    def _pick(self, pid, label):
        self.platform_id = pid
        self.platform_label = label
        self.accept()

    def _other(self):
        QMessageBox.information(
            self, 'Other platforms',
            'More platforms are on the way. For now, pick one of the listed targets — Xbox 360 has the '
            'full toolset.')


# Target languages the user can pick. Only C++ is live; the rest are scaffolded skeletons so the full
# picture is visible (each shows its own real syntax versions). 'default' is pre-selected.
LANGUAGES = [
    {'id': 'cpp',    'label': 'C++',    'emoji': '⚙️', 'live': True,
     'versions': ['C++98', 'C++03', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23'], 'default': 'C++03'},
    {'id': 'c',      'label': 'C',      'emoji': '🔧', 'live': False,
     'versions': ['C89', 'C99', 'C11', 'C17', 'C23'], 'default': 'C99'},
    {'id': 'csharp', 'label': 'C#',     'emoji': '#️⃣', 'live': False,
     'versions': ['C# 7', 'C# 8', 'C# 9', 'C# 10', 'C# 11', 'C# 12'], 'default': 'C# 10'},
    {'id': 'rust',   'label': 'Rust',   'emoji': '🦀', 'live': False,
     'versions': ['2015', '2018', '2021'], 'default': '2021'},
]


class LanguageDialog(QDialog):
    """Modal after the platform pick: choose the target language + syntax version. C++ is live; the
    other languages are visible skeletons (their versions render, but picking one shows 'coming soon')."""
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Choose a language')
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(560)
        self.language = None
        self.version = None
        self._lang = LANGUAGES[0]
        self._version = self._lang['default']

        outer = QVBoxLayout(self)
        outer.setContentsMargins(24, 24, 24, 24)
        outer.setSpacing(12)
        q = QLabel('What language and version should the project convert to?')
        q.setStyleSheet('font-size: 16px; font-weight: bold; color: #e2e0d8;')
        q.setWordWrap(True)
        outer.addWidget(q)

        # language row
        lrow = QHBoxLayout()
        lrow.setSpacing(8)
        self._lang_btns = {}
        for lang in LANGUAGES:
            b = QPushButton('%s  %s%s' % (lang['emoji'], lang['label'], '' if lang['live'] else '  · soon'))
            b.setCheckable(True)
            b.setObjectName('choice')
            b.setMinimumHeight(40)
            b.setChecked(lang is self._lang)
            b.clicked.connect(lambda _c=False, l=lang: self._pick_lang(l))
            self._lang_btns[lang['id']] = b
            lrow.addWidget(b)
        lrow.addStretch(1)
        outer.addLayout(lrow)

        vlabel = QLabel('Syntax version')
        vlabel.setStyleSheet('color: %s; font-size: 12px;' % MUTE)
        outer.addWidget(vlabel)
        self._vrow_host = QWidget()
        self._vrow = QHBoxLayout(self._vrow_host)
        self._vrow.setContentsMargins(0, 0, 0, 0)
        self._vrow.setSpacing(6)
        outer.addWidget(self._vrow_host)
        self._note = QLabel('')
        self._note.setStyleSheet('color: #7bbf95; font-size: 12px;')
        self._note.setWordWrap(True)
        outer.addWidget(self._note)

        outer.addSpacing(6)
        brow = QHBoxLayout()
        brow.addStretch(1)
        self._continue = QPushButton('Continue')
        self._continue.setObjectName('primary')
        self._continue.setMinimumHeight(38)
        self._continue.clicked.connect(self._confirm)
        brow.addWidget(self._continue)
        outer.addLayout(brow)
        self._render_versions()

    def _pick_lang(self, lang):
        self._lang = lang
        self._version = lang['default']
        for lid, b in self._lang_btns.items():
            b.setChecked(lid == lang['id'])
        self._render_versions()

    def _render_versions(self):
        while self._vrow.count():
            item = self._vrow.takeAt(0)
            w = item.widget()
            if w:
                w.deleteLater()
        self._vbtns = {}
        for v in self._lang['versions']:
            b = QPushButton(v)
            b.setCheckable(True)
            b.setObjectName('choice')
            b.setChecked(v == self._version)
            b.clicked.connect(lambda _c=False, ver=v: self._pick_version(ver))
            self._vbtns[v] = b
            self._vrow.addWidget(b)
        self._vrow.addStretch(1)
        if self._lang['live']:
            self._note.setText('%s %s — full translation.' % (self._lang['label'], self._version))
            self._note.setStyleSheet('color: #7bbf95; font-size: 12px;')
        else:
            self._note.setText('%s %s — emits STRUCTURAL SKELETONS (types + stubbed methods) so you can '
                               'see the shape. C++ is the fully-translated target.'
                               % (self._lang['label'], self._version))
            self._note.setStyleSheet('color: %s; font-size: 12px;' % WARN)

    def _pick_version(self, ver):
        self._version = ver
        for v, b in self._vbtns.items():
            b.setChecked(v == ver)
        if self._lang['live']:
            self._note.setText('%s %s — ready to convert.' % (self._lang['label'], ver))

    def _confirm(self):
        self.language = self._lang['id']
        self.version = self._version
        self.live = self._lang['live']       # C++ = full translation; others = skeleton output
        self.accept()


class PathsDialog(QDialog):
    """Modal after the language pick: two questions (original project dir, output dir) + the SDK as
    gray subtext at the bottom. Returns .java_dir / .out_dir / .sdk_dir."""
    def __init__(self, java='', out='', sdk='', parent=None):
        super().__init__(parent)
        self.setWindowTitle('Project folders')
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(600)
        self.java_dir = self.out_dir = self.sdk_dir = None

        outer = QVBoxLayout(self)
        outer.setContentsMargins(24, 22, 24, 22)
        outer.setSpacing(8)

        q1 = QLabel('Please provide the directory for your original project.')
        q1.setStyleSheet('font-size: 15px; font-weight: bold; color: #e7e5dd;')
        q1.setWordWrap(True)
        outer.addWidget(q1)
        self._ed_java = QLineEdit(java)
        self._ed_java.setPlaceholderText('…\\your-project\\src')
        outer.addLayout(self._row(self._ed_java))
        outer.addSpacing(10)

        q2 = QLabel('Please provide an output directory for the converted project.')
        q2.setStyleSheet('font-size: 15px; font-weight: bold; color: #e7e5dd;')
        q2.setWordWrap(True)
        outer.addWidget(q2)
        self._ed_out = QLineEdit(out)
        self._ed_out.setPlaceholderText('…\\converted-output')
        outer.addLayout(self._row(self._ed_out))
        outer.addSpacing(14)

        # SDK as quiet subtext at the bottom
        sdklab = QLabel('Platform SDK (auto-detected; change only if needed):')
        sdklab.setStyleSheet('color: %s; font-size: 12px;' % MUTE)
        outer.addWidget(sdklab)
        self._ed_sdk = QLineEdit(sdk)
        self._ed_sdk.setPlaceholderText('auto-detected from your installed SDK')
        outer.addLayout(self._row(self._ed_sdk))

        outer.addSpacing(10)
        brow = QHBoxLayout()
        brow.addStretch(1)
        cont = QPushButton('Continue')
        cont.setObjectName('primary')
        cont.setMinimumHeight(38)
        cont.clicked.connect(self._confirm)
        brow.addWidget(cont)
        outer.addLayout(brow)

    def _row(self, edit):
        r = QHBoxLayout()
        r.addWidget(edit, 1)
        b = QPushButton('Browse')
        b.setFixedWidth(80)
        b.clicked.connect(lambda: self._browse(edit))
        r.addWidget(b)
        return r

    def _browse(self, edit):
        d = QFileDialog.getExistingDirectory(self, 'Select folder', edit.text() or DESKTOP)
        if d:
            edit.setText(d)

    def _confirm(self):
        j = self._ed_java.text().strip()
        if not os.path.isdir(j):
            QMessageBox.warning(self, 'Folder not found',
                                'The original project directory does not exist:\n%s' % j)
            return
        self.java_dir = j
        self.out_dir = self._ed_out.text().strip()
        self.sdk_dir = self._ed_sdk.text().strip()
        self.accept()


class ToolStepDialog(QDialog):
    """One wizard step = one tool, in its own modal. The caller builds the tool's card into `self.body`
    via a builder callback; this frames it with a title + Skip / Next buttons."""
    def __init__(self, title, subtitle, build_body, parent=None, last=False):
        super().__init__(parent)
        self.setWindowTitle(title)
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(640)
        outer = QVBoxLayout(self)
        outer.setContentsMargins(20, 18, 20, 18)
        outer.setSpacing(10)
        head = QLabel(title)
        head.setStyleSheet('font-size: 16px; font-weight: bold; color: #f2f0e8;')
        outer.addWidget(head)
        if subtitle:
            sub = QLabel(subtitle)
            sub.setStyleSheet('color: %s; font-size: 12px;' % MUTE)
            sub.setWordWrap(True)
            outer.addWidget(sub)
        area = QScrollArea()
        area.setWidgetResizable(True)
        host = QWidget()
        self.body = QVBoxLayout(host)
        self.body.setContentsMargins(0, 0, 0, 0)
        build_body(self.body)
        area.setWidget(host)
        outer.addWidget(area, 1)
        brow = QHBoxLayout()
        skip = QPushButton('Skip')
        skip.clicked.connect(self.reject)
        brow.addWidget(skip)
        brow.addStretch(1)
        nxt = QPushButton('Finish' if last else 'Next')
        nxt.setObjectName('primary')
        nxt.clicked.connect(self.accept)
        brow.addWidget(nxt)
        outer.addLayout(brow)


class ReviewDialog(QDialog):
    """The overview step, shown in the MIDDLE of the wizard (right after the folders modal, before the
    tool questionnaires) so the user sees the whole setup at a glance before configuring the tools."""
    def __init__(self, rows, parent=None):
        super().__init__(parent)
        self.setWindowTitle('Review your setup')
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(560)
        outer = QVBoxLayout(self)
        outer.setContentsMargins(24, 22, 24, 22)
        outer.setSpacing(12)
        q = QLabel('Here’s your setup so far.')
        q.setStyleSheet('font-size: 16px; font-weight: bold; color: #e7e5dd;')
        outer.addWidget(q)
        card = QFrame(); card.setObjectName('card')
        cl = QVBoxLayout(card); cl.setContentsMargins(16, 14, 16, 14); cl.setSpacing(8)
        for label, value in rows:
            r = QHBoxLayout()
            lab = QLabel(label); lab.setStyleSheet('color:%s; font-size:12px;' % MUTE); lab.setFixedWidth(80)
            val = QLabel(value); val.setStyleSheet('color:#e7e5dd; font-size:13px;'); val.setWordWrap(True)
            r.addWidget(lab); r.addWidget(val, 1)
            cl.addLayout(r)
        outer.addWidget(card)
        outer.addStretch(1)
        brow = QHBoxLayout()
        back = QPushButton('Back'); back.clicked.connect(self.reject)
        brow.addWidget(back); brow.addStretch(1)
        cont = QPushButton('Continue to the tools'); cont.setObjectName('primary'); cont.clicked.connect(self.accept)
        brow.addWidget(cont)
        outer.addLayout(brow)


class ConvertingDialog(QDialog):
    """The LAST window: 'Converting now…'. Runs the actual conversion (C++ pipeline or the skeleton
    scaffolder), streams progress, then shows the result stats + Open output / Open reports / Done."""
    def __init__(self, win, parent=None):
        super().__init__(parent)
        self._win = win
        self._worker = None
        self.setWindowTitle('Converting')
        self.setStyleSheet(STYLE)
        self.setMinimumWidth(560)
        outer = QVBoxLayout(self)
        outer.setContentsMargins(24, 22, 24, 22)
        outer.setSpacing(12)
        self._title = QLabel('Converting now…')
        self._title.setStyleSheet('font-size: 18px; font-weight: bold; color: #7bcf99;')
        outer.addWidget(self._title)
        self._status = QLabel('Starting…')
        self._status.setStyleSheet('color: %s; font-size: 12px;' % MUTE)
        self._status.setWordWrap(True)
        outer.addWidget(self._status)
        self._stats = QLabel('')
        self._stats.setStyleSheet('color:#e7e5dd; font-size:13px;')
        self._stats.setWordWrap(True)
        outer.addWidget(self._stats)
        outer.addStretch(1)
        brow = QHBoxLayout()
        self._btn_out = QPushButton('Open output')
        self._btn_out.clicked.connect(self._open_out); self._btn_out.setEnabled(False)
        self._btn_rep = QPushButton('Open reports')
        self._btn_rep.clicked.connect(win.open_reports); self._btn_rep.setEnabled(False)
        self._btn_done = QPushButton('Done'); self._btn_done.setObjectName('primary')
        self._btn_done.clicked.connect(self.accept); self._btn_done.setEnabled(False)
        brow.addWidget(self._btn_out); brow.addWidget(self._btn_rep); brow.addStretch(1)
        brow.addWidget(self._btn_done)
        outer.addLayout(brow)
        from PySide6.QtCore import QTimer
        QTimer.singleShot(60, self._start)

    def _open_out(self):
        out = self._win.ed_out.text().strip()
        if os.path.isdir(out):
            os.startfile(out)

    def _start(self):
        w = self._win
        java = w.ed_java.text().strip()
        out = w.ed_out.text().strip()
        if not os.path.isdir(java):
            self._fail('Source folder not found:\n%s' % java); return
        os.makedirs(out, exist_ok=True)
        w._open_log_file()
        if getattr(w, '_target_language', 'cpp') != 'cpp':      # skeleton languages
            from . import scaffold
            try:
                r = scaffold.run(java, out, w._target_language)
                self._stats.setText('%d %s skeleton file(s) written.'
                                    % (len(r['written']), w._LANG_LABEL.get(w._target_language,
                                                                            w._target_language)))
                self._ready('Skeletons generated.')
            except Exception as e:
                self._fail('Failed: %s' % e)
            finally:
                w._close_log_file()
            return
        self._worker = ConvertWorker(java, out, cpp_std=w.combo_std.currentText(),
                                     platform=w._selected_platform_id(),
                                     emit_debug=w._emit_debug_enabled(),
                                     sdk_override=w.ed_sdk.text().strip() or None)
        self._worker.line.connect(self._on_line)
        self._worker.done.connect(self._on_done)
        self._worker.start()

    def _on_line(self, msg):
        self._status.setText(msg[-100:])
        self._win.logln(msg)

    def _on_done(self, rc, stats):
        w = self._win
        out = w.ed_out.text().strip()
        if rc == 0:
            ledger = w._finalize_conversion(stats, out)
            self._run_deferred_utils()                    # optional dev utilities, now that output exists
            from .ledger import Category
            cats = ledger.totals_by_category() if ledger else {}
            self._stats.setText(
                'Parsed %s   ·   Headers %s   ·   Sources %s\n'
                'Ported %s · Guessed %s · Stubbed %s · Risk %s · Errors %s · Rules %s'
                % (stats.get('parsed', 0), stats.get('headers', 0), stats.get('sources', 0),
                   cats.get(Category.PORTED, 0), cats.get(Category.GUESSED, 0),
                   cats.get(Category.STUBBED, 0), cats.get(Category.RISK, 0),
                   cats.get(Category.ERROR, 0), cats.get(Category.RULE, 0)))
            self._ready('Conversion complete.')
        else:
            self._fail('Conversion failed. See the log (Open reports).')
        w._close_log_file()

    def _run_deferred_utils(self):
        w = self._win
        w._auto_open_reports = False                            # don't pop reports open at convert-end
        runners = {'Scan Stubs': w.run_stubs, 'Regen vcxproj': w.run_vcxproj,
                   'Signatures': w.run_signatures, 'Worklist': w.run_worklist}
        for name in getattr(w, '_devutil_sel', []):
            self._status.setText('Running %s…' % name)
            QApplication.processEvents()
            try:
                runners[name]()
            except Exception as e:
                w.logln('%s failed: %s' % (name, e))
        w._auto_open_reports = True                            # restore for manual tool clicks

    def _ready(self, title):
        self._title.setText(title)
        self._status.setText('Output:  %s' % self._win.ed_out.text().strip())
        for b in (self._btn_out, self._btn_rep, self._btn_done):
            b.setEnabled(True)

    def _fail(self, msg):
        self._title.setText('Conversion failed')
        self._title.setStyleSheet('font-size: 18px; font-weight: bold; color: %s;' % FAIL)
        self._status.setText(msg)
        self._btn_rep.setEnabled(True)
        self._btn_done.setEnabled(True)


class ConverterWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('j2x — Java → C++ Converter')
        self.resize(1100, 820)
        self.setStyleSheet(STYLE)
        self.settings = QSettings('j2x', 'ConverterGUI')
        self.worker = None
        self.proc = None
        self._chain_after_convert = False
        self._log_file = None
        self._build_ui()

    def _build_ui(self):
        # Landing screen + an IN-WINDOW wizard (no pop-up dialogs). A stacked widget swaps between the
        # landing (0) and the wizard page (1); the wizard renders each step in place.
        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)
        self.stack.addWidget(self._build_landing())        # index 0
        self._workspace = self._build_workspace()          # builds shared STATE widgets (not shown)
        self.stack.addWidget(self._build_wizard())         # index 1 = the in-window wizard
        self.stack.setCurrentIndex(0)

    def _build_workspace(self):
        s = self.settings
        root = QWidget()
        outer = QVBoxLayout(root)
        outer.setContentsMargins(14, 14, 14, 14)
        outer.setSpacing(10)

        # header
        head = QHBoxLayout()
        btn_home = QPushButton('‹ Home')
        btn_home.setToolTip('Back to the start screen')
        btn_home.clicked.connect(self.go_home)
        title = QLabel('Convert a project')
        title.setObjectName('title')
        self.btn_run_all = QPushButton('  Run All  ')
        self.btn_run_all.setObjectName('primary')
        self.btn_run_all.setToolTip('Convert → Report → Regen vcxproj')
        self.btn_run_all.clicked.connect(self.run_all)
        head.addWidget(btn_home)
        head.addSpacing(8)
        head.addWidget(title)
        head.addStretch(1)
        head.addWidget(self.btn_run_all)
        outer.addLayout(head)

        # path rows
        self.ed_java = QLineEdit(s.value('java', DEFAULT_JAVA))
        # ed_java/out/sdk are hidden STATE holders now -- the paths are chosen in the PathsDialog modal
        # and shown as subtext. (The old input rows are gone.)
        self.ed_out = QLineEdit(s.value('out', DEFAULT_OUT))
        from .platforms import PLATFORMS as _PLATS, default_sdk_dir as _default_sdk
        _saved_plat_label = s.value('platform', 'Xbox 360')
        _saved_plat_id = next((k for k, v in _PLATS.items() if v['label'] == _saved_plat_label), 'xbox360')
        _saved_sdk = s.value('sdk', '') or _default_sdk(_saved_plat_id, REPO_ROOT)
        self.ed_sdk = QLineEdit(_saved_sdk)
        self.lbl_paths = QLabel('')
        self.lbl_paths.setStyleSheet('color:%s; font-size:12px;' % MUTE)
        self.lbl_paths.setWordWrap(True)
        outer.addWidget(self.lbl_paths)

        # The language/version and platform were already chosen in the setup modals, so we don't show
        # selectors here -- we keep the combos as hidden STATE (existing logic reads them) and just
        # display the chosen target as gray subtext.
        from .platforms import PLATFORMS
        self.combo_std = QComboBox()          # hidden state holder
        self.combo_std.addItems(['C++03', 'C++98', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23'])
        _idx = self.combo_std.findText(s.value('cpp_std', 'C++03'))
        if _idx >= 0:
            self.combo_std.setCurrentIndex(_idx)
        self._std_desc = QLabel('')           # hidden; kept so _on_std_changed has a target
        self.combo_std.currentTextChanged.connect(self._on_std_changed)
        self.combo_std.currentTextChanged.connect(lambda *_: self._update_target_label())

        self.combo_platform = QComboBox()     # hidden state holder
        self._platform_ids = list(PLATFORMS.keys())
        self._platform_labels = [PLATFORMS[k]['label'] for k in self._platform_ids]
        self.combo_platform.addItems(self._platform_labels)
        _pidx = self.combo_platform.findText(s.value('platform', 'Xbox 360'))
        if _pidx >= 0:
            self.combo_platform.setCurrentIndex(_pidx)
        self._plat_desc = QLabel('')          # hidden; kept for _on_platform_changed
        self.combo_platform.currentIndexChanged.connect(self._on_platform_changed)

        self._target_language = 'cpp'
        self._target_version = self.combo_std.currentText()
        self._target_live = True

        target_row = QHBoxLayout()
        self.lbl_target = QLabel('')
        self.lbl_target.setStyleSheet('color:%s; font-size:12px;' % MUTE)
        target_row.addWidget(self.lbl_target)
        target_row.addStretch(1)
        outer.addLayout(target_row)
        self._on_platform_changed(self.combo_platform.currentIndex())
        self._update_target_label()

        # one slim stats strip (replaces nine big metric cards + the stage strip): small label:value
        # chips that stay quiet until a conversion fills them in.
        def _stat(label, color):
            w = QFrame()
            hl = QHBoxLayout(w)
            hl.setContentsMargins(0, 0, 0, 0)
            hl.setSpacing(5)
            cap = QLabel(label)
            cap.setStyleSheet('color:%s; font-size:11px;' % MUTE)
            val = QLabel('–')
            val.setStyleSheet('color:%s; font-size:13px; font-weight:bold;' % color)
            hl.addWidget(cap)
            hl.addWidget(val)
            return w, val

        stats = QFrame()
        stats.setObjectName('strip')
        srow = QHBoxLayout(stats)
        srow.setContentsMargins(12, 6, 12, 6)
        srow.setSpacing(16)
        sp, self.m_parsed = _stat('Parsed', OK)
        sh, self.m_headers = _stat('Headers', INFO)
        ss, self.m_sources = _stat('Sources', INFO)
        cp, v_ported = _stat('Ported', _CAT_COLORS['PORTED'])
        cg, v_guessed = _stat('Guessed', _CAT_COLORS['GUESSED'])
        cs, v_stubbed = _stat('Stubbed', _CAT_COLORS['STUBBED'])
        cr, v_risk = _stat('Risk', _CAT_COLORS['RISK'])
        ce, v_errors = _stat('Errors', _CAT_COLORS['ERROR'])
        cru, v_rules = _stat('Rules', _CAT_COLORS['RULE'])
        self._cat_cards = {'PORTED': (cp, v_ported), 'GUESSED': (cg, v_guessed),
                           'STUBBED': (cs, v_stubbed), 'RISK': (cr, v_risk),
                           'ERROR': (ce, v_errors), 'RULE': (cru, v_rules)}
        for wdg in (sp, sh, ss):
            srow.addWidget(wdg)
        sep0 = QLabel('|')
        sep0.setStyleSheet('color:#44443f;')
        srow.addWidget(sep0)
        for key in ('PORTED', 'GUESSED', 'STUBBED', 'RISK', 'ERROR', 'RULE'):
            srow.addWidget(self._cat_cards[key][0])
        srow.addStretch(1)
        self.stages = {}                                  # stage strip removed; _set_stage no-ops
        outer.addWidget(stats)

        # tabs
        self.tabs = QTabWidget()
        self._action_buttons = {}                          # populated by _reg_btn (bottom row + Tools)

        self.log = QTextEdit()
        self.log.setReadOnly(True)
        self.log.setFont(QFont('Consolas', 9))

        # diagnostics tree — grouped by category, expandable
        self.diag_tree = QTreeWidget()
        self.diag_tree.setHeaderLabels(['Category / File', 'Severity', 'Message'])
        self.diag_tree.setColumnWidth(0, 300)
        self.diag_tree.setColumnWidth(1, 70)
        self.diag_tree.setColumnWidth(2, 500)
        self.diag_tree.setRootIsDecorated(True)
        self.diag_tree.setAlternatingRowColors(False)
        self.diag_tree.header().setStretchLastSection(True)

        self.report_tree = QTreeWidget()
        self.report_tree.setHeaderLabels(['Risk', 'File', 'Categories', 'Severities'])
        self.report_tree.setColumnWidth(0, 60)
        self.report_tree.setColumnWidth(1, 220)
        self.report_tree.setColumnWidth(2, 200)
        self.report_tree.setRootIsDecorated(False)

        self.err_list = QListWidget()
        self.err_list.itemDoubleClicked.connect(self._open_err_item)

        # stubs tree — grouped by subsystem
        self.stubs_tree = QTreeWidget()
        self.stubs_tree.setHeaderLabels(['Subsystem / Stub', 'Methods', 'SDK Hint'])
        self.stubs_tree.setColumnWidth(0, 350)
        self.stubs_tree.setColumnWidth(1, 60)
        self.stubs_tree.setColumnWidth(2, 400)
        self.stubs_tree.setRootIsDecorated(True)
        self.stubs_tree.header().setStretchLastSection(True)

        # The whole setup + conversion now runs as a linear modal wizard (folders → review → tool
        # questions → 'Converting now'), so this page is only a builder for the shared state widgets
        # above (ed_java/out/sdk, combos, stat cards) and is never shown.
        outer.addStretch(1)
        return root

    # ==== LANDING SCREEN ================================================
    def _build_landing(self):
        root = QWidget()
        outer = QVBoxLayout(root)
        outer.setContentsMargins(40, 40, 40, 40)
        outer.addStretch(1)

        title = QLabel('What would you like to do today?')
        title.setObjectName('hero')
        title.setAlignment(Qt.AlignCenter)
        outer.addWidget(title)
        sub = QLabel('Pick a starting point.')
        sub.setAlignment(Qt.AlignCenter)
        sub.setStyleSheet('font-size: 14px; color: %s;' % MUTE)
        outer.addWidget(sub)
        outer.addSpacing(30)

        choices = QHBoxLayout()
        choices.setSpacing(22)
        choices.addStretch(1)
        choices.addWidget(self._landing_card(
            '📄', 'Convert a file',
            'Pick a single source file and convert it right away.', self.on_convert_file))
        choices.addWidget(self._landing_card(
            '📦', 'Convert a project',
            'Choose a platform and language, then the full conversion toolset.', self.on_convert_project,
            primary=True))
        choices.addStretch(1)
        outer.addLayout(choices)
        outer.addStretch(2)
        return root

    def _landing_card(self, emoji, title, desc, on_click, primary=False):
        card = QFrame()
        card.setObjectName('card')
        card.setFixedSize(310, 210)
        card.setCursor(Qt.PointingHandCursor)
        lay = QVBoxLayout(card)
        lay.setContentsMargins(22, 20, 22, 20)
        lay.setSpacing(7)
        em = QLabel(emoji)
        em.setStyleSheet('font-size: 30px;')
        lay.addWidget(em)
        t = QLabel(title)
        t.setStyleSheet('font-size: 18px; font-weight: bold; color: %s;'
                        % ('#7bcf99' if primary else '#f2f0e8'))
        d = QLabel(desc)
        d.setWordWrap(True)
        d.setStyleSheet('font-size: 13px; color: %s;' % MUTE)
        btn = QPushButton(title)
        if primary:
            btn.setObjectName('primary')
        btn.setMinimumHeight(38)
        btn.setCursor(Qt.PointingHandCursor)
        btn.clicked.connect(on_click)
        lay.addWidget(t)
        lay.addWidget(d)
        lay.addStretch(1)
        lay.addWidget(btn)
        return card

    def _build_wizard(self):
        page = QWidget()
        v = QVBoxLayout(page)
        v.setContentsMargins(20, 16, 20, 16)
        v.setSpacing(12)
        head = QHBoxLayout()
        home = QPushButton('‹ Home'); home.setToolTip('Back to the start screen')
        home.clicked.connect(self.go_home)
        self._wiz_title = QLabel(''); self._wiz_title.setObjectName('title')
        self._wiz_step_lbl = QLabel(''); self._wiz_step_lbl.setStyleSheet('color:%s;' % MUTE)
        head.addWidget(home); head.addSpacing(8); head.addWidget(self._wiz_title)
        head.addStretch(1); head.addWidget(self._wiz_step_lbl)
        v.addLayout(head)
        self._wiz_scroll = QScrollArea(); self._wiz_scroll.setWidgetResizable(True)
        self._wiz_scroll.setFrameShape(QFrame.NoFrame)
        v.addWidget(self._wiz_scroll, 1)
        nav = QHBoxLayout()
        self._wiz_back = QPushButton('‹ Back'); self._wiz_back.clicked.connect(self._wiz_go_back)
        self._wiz_skip = QPushButton('Skip'); self._wiz_skip.clicked.connect(self._wiz_go_skip)
        self._wiz_next = QPushButton('Next ›'); self._wiz_next.setObjectName('primary')
        self._wiz_next.clicked.connect(self._wiz_go_next)
        nav.addWidget(self._wiz_back); nav.addWidget(self._wiz_skip); nav.addStretch(1)
        nav.addWidget(self._wiz_next)
        v.addLayout(nav)
        return page

    def go_home(self):
        self.stack.setCurrentIndex(0)

    _LANG_LABEL = {'cpp': 'C++', 'c': 'C', 'csharp': 'C#', 'rust': 'Rust'}

    def _update_target_label(self):
        lang = self._LANG_LABEL.get(getattr(self, '_target_language', 'cpp'), 'C++')
        ver = getattr(self, '_target_version', None) or self.combo_std.currentText()
        plat = self.combo_platform.currentText()
        skel = '   ·   skeleton output' if not getattr(self, '_target_live', True) else ''
        self.lbl_target.setText('Target:   %s %s   ·   %s%s' % (lang, ver, plat, skel))

    def _on_platform_tools(self):
        # The tools are answered as modals in the wizard, which is rebuilt per platform each run --
        # nothing to live-rebuild in the workspace body.
        pass

    def _effective_out(self):
        """The dir holding the converted sources/reports. After the solution layout runs they live in
        the <root>/<Name>/ project subfolder; otherwise directly in the chosen output dir."""
        out = self.ed_out.text().strip()
        name = os.path.basename(os.path.normpath(out))
        proj = os.path.join(out, name)
        if os.path.isfile(os.path.join(proj, name + '.vcxproj')):
            return proj
        return out

    def open_reports(self):
        """Open the pretty report files the conversion writes (DIAGNOSTICS.md, then STUBS.md, then the
        log) -- whichever exist."""
        out = self._effective_out()
        opened = False
        for name in ('DIAGNOSTICS.md', 'STUBS.md', 'conversion.log'):
            p = os.path.join(out, name)
            if os.path.isfile(p):
                os.startfile(p)
                opened = True
        if not opened:
            QMessageBox.information(self, 'No reports yet',
                                    'Reports appear after you run a conversion:\n\n'
                                    'DIAGNOSTICS.md, STUBS.md, conversion.log in\n%s' % out)

    # ---- CONVERT A FILE (single file, quick) ----------------------------
    def on_convert_file(self):
        path, _ = QFileDialog.getOpenFileName(
            self, 'Choose a source file to convert', self.ed_java.text() or DESKTOP,
            'Java source (*.java);;All files (*.*)')
        if not path:
            return
        import tempfile, shutil
        out_dir = os.path.join(os.path.dirname(path), 'converted')
        os.makedirs(out_dir, exist_ok=True)
        tmp = tempfile.mkdtemp(prefix='j2x_file_')
        try:
            shutil.copy(path, tmp)
            from . import pipeline
            self.logln('converting single file: %s' % path)
            s = pipeline.convert(tmp, out_dir, write_report=False, platform=None,
                                 emit_debug=False, progress_cb=self.logln)
            self.logln('[done] %d header(s), %d source(s) -> %s'
                       % (s.get('headers', 0), s.get('sources', 0), out_dir))
            QMessageBox.information(self, 'Converted',
                                    'Converted %s\n\nOutput: %s'
                                    % (os.path.basename(path), out_dir))
            if os.path.isdir(out_dir):
                os.startfile(out_dir)
        except Exception as e:
            self.logln('ERROR: %s: %s' % (type(e).__name__, e))
            QMessageBox.critical(self, 'Conversion failed', '%s: %s' % (type(e).__name__, e))
        finally:
            shutil.rmtree(tmp, ignore_errors=True)

    # ==== IN-WINDOW WIZARD ===============================================
    def on_convert_project(self):
        self._wiz_start()

    def _wiz_start(self):
        # fresh run: blank source/output (no stale context), keep the auto-detected SDK
        self.ed_java.setText('')
        self.ed_out.setText('')
        self._target_language, self._target_version, self._target_live = 'cpp', 'C++03', True
        self._wiz_data = {'platform_id': 'xbox360', 'platform_label': 'Xbox 360',
                          'language': 'cpp', 'version': 'C++03', 'live': True}
        self._decision_values = {}
        self._devutil_sel = []
        self._cfg = {}                                     # captured per-tool config
        self._tool_include = {}                            # step id -> included? (default True)
        self._resolved = {}
        self._steps = [
            {'id': 'platform',     'title': 'Choose a platform'},
            {'id': 'language',     'title': 'Choose a language & version'},
            {'id': 'folders',      'title': 'Project folders'},
            {'id': 'assets',       'title': 'Assets', 'tool': self._tc_assets,
             'action': 'extract the game textures/resources into Content\\'},
            {'id': 'audio',        'title': 'Audio', 'tool': self._tc_audio, 'needs360': True,
             'action': 'match the sounds and build the audio banks'},
            {'id': 'decisions',    'title': 'Platform decisions', 'tool': self._tc_decisions,
             'action': 'emit the platform config from your answers'},
            {'id': 'uimark',       'title': 'UI marker', 'tool': self._tc_uimark,
             'action': 'tag every UI function with a greppable marker + write UI_INDEX.md'},
            {'id': 'achievements', 'title': 'Achievements', 'tool': self._tc_achievements, 'needs360': True,
             'action': 'map achievements to the console (XLAST config + unlock seam)'},
            {'id': 'coverage',     'title': 'Coverage & SDK', 'tool': self._tc_coverage,
             'action': 'write the platform coverage report'},
            {'id': 'devutils',     'title': 'Developer utilities', 'tool': self._tc_devutils,
             'action': 'run the selected developer utilities'},
            {'id': 'summary',      'title': 'Review & convert'},
            {'id': 'converting',   'title': 'Converting'},
        ]
        self._wiz_index = 0
        self.stack.setCurrentIndex(1)
        self._wiz_render()

    def _wiz_applies(self, step):
        return not (step.get('needs360') and self._wiz_data.get('platform_id') != 'xbox360')

    def _wiz_render(self):
        step = self._steps[self._wiz_index]
        self._wiz_title.setText(step['title'])
        applicable = [s for s in self._steps if self._wiz_applies(s)]
        pos = applicable.index(step) + 1 if step in applicable else self._wiz_index + 1
        self._wiz_step_lbl.setText('Step %d of %d' % (pos, len(applicable)))
        host = QWidget()
        lay = QVBoxLayout(host)
        lay.setContentsMargins(2, 2, 2, 2)
        lay.setSpacing(10)
        sid = step['id']
        builder = {'platform': self._wstep_platform, 'language': self._wstep_language,
                   'folders': self._wstep_folders, 'summary': self._wstep_summary,
                   'converting': self._wstep_converting}.get(sid)
        if builder:
            builder(lay)
        else:                                              # a tool step: gray subtext + config card
            inc = self._tool_include.get(sid, True)
            note = QLabel('This step WILL %s when you convert — press “Skip” to exclude it.'
                          % step['action'] if inc
                          else 'This step will be SKIPPED at convert. Press “Include” to add it back.')
            note.setStyleSheet('color:%s; font-size:12px;' % (MUTE if inc else WARN))
            note.setWordWrap(True)
            lay.addWidget(note)
            step['tool'](lay)                              # config-only card (no action button)
        lay.addStretch(1)
        self._wiz_scroll.setWidget(host)
        conv = sid == 'converting'
        self._wiz_back.setVisible(self._wiz_index > 0 and not conv)
        self._wiz_back.setText('‹ Adjust settings' if sid == 'summary' else '‹ Back')
        self._wiz_skip.setVisible('tool' in step)
        if 'tool' in step:
            self._wiz_skip.setText('Include' if not self._tool_include.get(sid, True) else 'Skip')
        self._wiz_next.setVisible(not conv)
        self._wiz_next.setText('Convert' if sid == 'summary' else 'Next ›')

    def _wiz_capture(self):
        sid = self._steps[self._wiz_index]['id']
        if sid == 'platform':
            i = self.combo_platform.findText(self._wiz_data['platform_label'])
            if i >= 0:
                self.combo_platform.setCurrentIndex(i)
        elif sid == 'language':
            self._target_language = self._wiz_data['language']
            self._target_version = self._wiz_data['version']
            self._target_live = self._wiz_data['live']
            if self._target_live:
                i = self.combo_std.findText(self._target_version)
                if i >= 0:
                    self.combo_std.setCurrentIndex(i)
        elif sid == 'folders':
            j = self._wiz_java.text().strip()
            if not os.path.isdir(j):
                QMessageBox.warning(self, 'Folder not found',
                                    'The original project directory does not exist:\n%s' % j)
                return False
            # auto-resolve the rest of the project from this one folder (jar, resources, src, lang)
            from . import project_scan
            self._resolved = project_scan.resolve(j)
            self.ed_java.setText(self._resolved.get('java_src') or j)   # use the src root if found
            self.ed_out.setText(self._wiz_out.text().strip())
            self.ed_sdk.setText(self._wiz_sdk.text().strip())
        elif 'tool' in self._steps[self._wiz_index]:
            self._capture_tool_cfg()
        return True

    def _capture_tool_cfg(self):
        """Capture the current tool step's inputs into self._cfg (widgets are destroyed on re-render)."""
        sid = self._steps[self._wiz_index]['id']
        if sid == 'assets':
            self._cfg['assets'] = {'jar': self.ed_jar.text().strip(),
                                   'content': self.ed_content.text().strip()}
        elif sid == 'audio':
            self._cfg['audio'] = {'resources': self.ed_res.text().strip(),
                                  'audio_out': self.ed_audio_out.text().strip()}
        elif sid == 'decisions':
            self._decision_values = dict((did, g['value']())
                                         for did, g in getattr(self, '_decision_groups', {}).items())
            self._cfg['invert_y'] = self.chk_invy.isChecked()
        elif sid == 'uimark':
            self._cfg['uimark'] = {'dir': self.ed_uidir.text().strip()}
        elif sid == 'achievements':
            self._cfg['achievements'] = {'lang': self.ed_ach_lang.text().strip(),
                                         'title_id': self.ed_ach_titleid.text().strip(),
                                         'profile': self.combo_ach_profile.currentText()}
        elif sid == 'devutils':
            self._devutil_sel = [n for n, cb in getattr(self, '_devutil_checks', {}).items()
                                 if cb.isChecked()]

    def _wiz_go_next(self):
        if not self._wiz_capture():
            return
        i = self._wiz_index + 1
        while i < len(self._steps) and not self._wiz_applies(self._steps[i]):
            i += 1
        if i < len(self._steps):
            self._wiz_index = i
            self._wiz_render()

    def _wiz_go_skip(self):
        # Skip TOGGLES whether this tool runs at convert; it does not advance (Next does).
        step = self._steps[self._wiz_index]
        if 'tool' not in step:
            return
        self._capture_tool_cfg()                           # keep whatever they typed
        self._tool_include[step['id']] = not self._tool_include.get(step['id'], True)
        self._wiz_render()

    def _wiz_go_back(self):
        if self._steps[self._wiz_index]['id'] == 'summary':
            self._wiz_index = 0                            # "Adjust settings" -> back to the start
            self._wiz_render()
            return
        i = self._wiz_index - 1
        while i >= 0 and not self._wiz_applies(self._steps[i]):
            i -= 1
        if i >= 0:
            self._wiz_index = i
            self._wiz_render()

    # ---- step content builders ----
    def _wstep_platform(self, lay):
        from .platforms import PLATFORMS
        emoji = {'xbox360': '🎮', 'pc_windows': '🖥️', 'pc_opengl': '🖥️', 'ps3': '🎮',
                 'wii': '🕹️', 'psp': '🎮', 'nds': '🎮'}
        q = QLabel('Which platform would you like to convert your project to?')
        q.setStyleSheet('font-size:16px; font-weight:bold; color:#e7e5dd;'); q.setWordWrap(True)
        lay.addWidget(q)
        hint = QLabel('The tools you get are tailored to the platform you pick.')
        hint.setStyleSheet('color:%s; font-size:12px;' % MUTE); lay.addWidget(hint)
        gw = QWidget(); grid = QGridLayout(gw); grid.setSpacing(10)
        bg = QButtonGroup(gw); bg.setExclusive(True)
        cur = self._wiz_data.get('platform_id') or 'xbox360'
        for i, (pid, info) in enumerate(PLATFORMS.items()):
            b = QPushButton('%s  %s%s' % (emoji.get(pid, '🎯'), info['label'],
                                          '   ★' if pid == 'xbox360' else ''))
            b.setObjectName('choice'); b.setCheckable(True); b.setMinimumHeight(44)
            b.setChecked(pid == cur)
            b.clicked.connect(lambda _c=False, p=pid, l=info['label']:
                              self._wiz_data.update(platform_id=p, platform_label=l))
            bg.addButton(b); grid.addWidget(b, i // 2, i % 2)
        lay.addWidget(gw)

    def _wstep_language(self, lay):
        q = QLabel('What language and version should the project convert to?')
        q.setStyleSheet('font-size:16px; font-weight:bold; color:#e7e5dd;'); q.setWordWrap(True)
        lay.addWidget(q)
        lhost = QWidget(); lhl = QHBoxLayout(lhost); lhl.setContentsMargins(0, 0, 0, 0)
        self._wiz_lg = QButtonGroup(lhost); self._wiz_lg.setExclusive(True)
        vhost = QWidget(); vlay = QHBoxLayout(vhost); vlay.setContentsMargins(0, 0, 0, 0)
        note = QLabel(''); note.setWordWrap(True)

        def render_versions(lang):
            while vlay.count():
                w = vlay.takeAt(0).widget()
                if w:
                    w.deleteLater()
            self._wiz_vg = QButtonGroup(vhost); self._wiz_vg.setExclusive(True)
            for v in lang['versions']:
                vb = QPushButton(v); vb.setObjectName('choice'); vb.setCheckable(True)
                vb.setChecked(v == self._wiz_data['version'])
                vb.clicked.connect(lambda _c=False, ver=v: self._wiz_data.update(version=ver))
                self._wiz_vg.addButton(vb); vlay.addWidget(vb)
            vlay.addStretch(1)
            if lang['live']:
                note.setText('%s %s — full translation.' % (lang['label'], self._wiz_data['version']))
                note.setStyleSheet('color:#7bbf95; font-size:12px;')
            else:
                note.setText('%s emits STRUCTURAL SKELETONS (types + stubbed methods). C++ is the '
                             'fully-translated target.' % lang['label'])
                note.setStyleSheet('color:%s; font-size:12px;' % WARN)

        def set_language(lang):
            self._wiz_data.update(language=lang['id'], live=lang['live'], version=lang['default'])
            render_versions(lang)

        cur_id = self._wiz_data.get('language', 'cpp')
        for lang in LANGUAGES:
            lb = QPushButton('%s  %s%s' % (lang['emoji'], lang['label'],
                                           '' if lang['live'] else '  · skeleton'))
            lb.setObjectName('choice'); lb.setCheckable(True); lb.setMinimumHeight(40)
            lb.setChecked(lang['id'] == cur_id)
            lb.clicked.connect(lambda _c=False, l=lang: set_language(l))
            self._wiz_lg.addButton(lb); lhl.addWidget(lb)
        lhl.addStretch(1)
        lay.addWidget(lhost)
        vl = QLabel('Syntax version'); vl.setStyleSheet('color:%s; font-size:12px;' % MUTE)
        lay.addWidget(vl)
        lay.addWidget(vhost)
        lay.addWidget(note)
        render_versions(next(l for l in LANGUAGES if l['id'] == cur_id))

    def _wstep_folders(self, lay):
        q1 = QLabel('Please provide the directory for your original project.')
        q1.setStyleSheet('font-size:15px; font-weight:bold; color:#e7e5dd;'); q1.setWordWrap(True)
        lay.addWidget(q1)
        self._wiz_java = QLineEdit(self.ed_java.text())
        self._wiz_java.setPlaceholderText('…\\your-project\\src')
        lay.addLayout(self._folder_row(self._wiz_java))
        q2 = QLabel('Please provide an output directory for the converted project.')
        q2.setStyleSheet('font-size:15px; font-weight:bold; color:#e7e5dd;'); q2.setWordWrap(True)
        lay.addWidget(q2)
        self._wiz_out = QLineEdit(self.ed_out.text())
        self._wiz_out.setPlaceholderText('…\\converted-output')
        lay.addLayout(self._folder_row(self._wiz_out))
        oh = QLabel('This folder becomes the solution root: a same-named project subfolder and a '
                    '.sln are created inside it (like the reference port). Pick the folder, not a '
                    'subfolder within it.')
        oh.setStyleSheet('color:%s; font-size:11px;' % MUTE); oh.setWordWrap(True)
        lay.addWidget(oh)
        sl = QLabel('Platform SDK (auto-detected; change only if needed):')
        sl.setStyleSheet('color:%s; font-size:12px;' % MUTE); lay.addWidget(sl)
        self._wiz_sdk = QLineEdit(self.ed_sdk.text())
        lay.addLayout(self._folder_row(self._wiz_sdk))

    def _folder_row(self, edit):
        r = QHBoxLayout(); r.addWidget(edit, 1)
        b = QPushButton('Browse'); b.setFixedWidth(80); b.clicked.connect(lambda: self._browse(edit))
        r.addWidget(b)
        return r

    def _wstep_summary(self, lay):
        q = QLabel('Review your selections, then convert.')
        q.setStyleSheet('font-size:16px; font-weight:bold; color:#e7e5dd;'); lay.addWidget(q)
        card = QFrame(); card.setObjectName('card'); cl = QVBoxLayout(card)
        cl.setContentsMargins(16, 14, 16, 14); cl.setSpacing(6)
        for label, value in self._summary_rows():
            r = QHBoxLayout()
            lb = QLabel(label); lb.setStyleSheet('color:%s; font-size:12px;' % MUTE); lb.setFixedWidth(90)
            vv = QLabel(value); vv.setStyleSheet('color:#e7e5dd; font-size:13px;'); vv.setWordWrap(True)
            r.addWidget(lb); r.addWidget(vv, 1); cl.addLayout(r)
        lay.addWidget(card)
        t = QLabel('Tools that will run at convert:')
        t.setStyleSheet('color:%s; font-size:12px;' % MUTE); lay.addWidget(t)
        tcard = QFrame(); tcard.setObjectName('card'); tl = QVBoxLayout(tcard)
        tl.setContentsMargins(16, 10, 16, 10); tl.setSpacing(4)
        for step in self._steps:
            if 'tool' not in step or not self._wiz_applies(step):
                continue
            inc = self._tool_include.get(step['id'], True)
            row = QLabel(('✓  %s' % step['title']) if inc else ('—  %s  (skipped)' % step['title']))
            row.setStyleSheet('color:%s; font-size:12px;' % ('#7bcf99' if inc else MUTE))
            tl.addWidget(row)
        lay.addWidget(tcard)
        hint = QLabel('“Convert” runs the conversion and every included tool. “Adjust settings” goes '
                      'back to change anything.')
        hint.setStyleSheet('color:%s; font-size:11px;' % MUTE); hint.setWordWrap(True)
        lay.addWidget(hint)

    def _wstep_converting(self, lay):
        self._cv_title = QLabel('Converting now…')
        self._cv_title.setStyleSheet('font-size:18px; font-weight:bold; color:#7bcf99;')
        lay.addWidget(self._cv_title)
        self._cv_status = QLabel('Starting…')
        self._cv_status.setStyleSheet('color:%s; font-size:12px;' % MUTE); self._cv_status.setWordWrap(True)
        lay.addWidget(self._cv_status)
        self._cv_stats = QLabel(''); self._cv_stats.setStyleSheet('color:#e7e5dd; font-size:13px;')
        self._cv_stats.setWordWrap(True)
        lay.addWidget(self._cv_stats)
        brow = QHBoxLayout()
        self._cv_out = QPushButton('Open output'); self._cv_out.clicked.connect(self.open_output)
        self._cv_rep = QPushButton('Open reports'); self._cv_rep.clicked.connect(self.open_reports)
        self._cv_done = QPushButton('Done'); self._cv_done.setObjectName('primary')
        self._cv_done.clicked.connect(self.go_home)
        for b in (self._cv_out, self._cv_rep, self._cv_done):
            b.setEnabled(False)
        brow.addWidget(self._cv_out); brow.addWidget(self._cv_rep); brow.addStretch(1)
        brow.addWidget(self._cv_done)
        w = QWidget(); w.setLayout(brow); lay.addWidget(w)
        from PySide6.QtCore import QTimer
        QTimer.singleShot(80, self._cv_start)

    def _cv_start(self):
        java, out = self.ed_java.text().strip(), self.ed_out.text().strip()
        if not os.path.isdir(java):
            self._cv_fail('Source folder not found:\n%s' % java); return
        os.makedirs(out, exist_ok=True)
        self._open_log_file()
        if getattr(self, '_target_language', 'cpp') != 'cpp':
            from . import scaffold
            try:
                r = scaffold.run(java, out, self._target_language)
                self._cv_stats.setText('%d %s skeleton file(s) written.'
                                       % (len(r['written']),
                                          self._LANG_LABEL.get(self._target_language, self._target_language)))
                self._cv_ready('Skeletons generated.')
            except Exception as e:
                self._cv_fail('Failed: %s' % e)
            finally:
                self._close_log_file()
            return
        self._cv_worker = ConvertWorker(java, out, cpp_std=self.combo_std.currentText(),
                                        platform=self._selected_platform_id(),
                                        emit_debug=self._emit_debug_enabled(),
                                        sdk_override=self.ed_sdk.text().strip() or None)
        self._cv_worker.line.connect(self._cv_on_line)
        self._cv_worker.done.connect(self._cv_on_done)
        self._cv_worker.start()

    def _cv_on_line(self, msg):
        self._cv_status.setText(msg[-100:]); self.logln(msg)

    def _cv_on_done(self, rc, stats):
        out = self.ed_out.text().strip()
        if rc == 0:
            ledger = self._finalize_conversion(stats, out)
            self._cv_run_tools(out)                        # run every INCLUDED tool, in order
            self._close_log_file()                         # release the log before restructuring
            self._apply_project_layout(out)                # <root>/<Name>.sln + <root>/<Name>/ (Alpha layout)
            from .ledger import Category
            cats = ledger.totals_by_category() if ledger else {}
            self._cv_stats.setText(
                'Parsed %s   ·   Headers %s   ·   Sources %s\n'
                'Ported %s · Guessed %s · Stubbed %s · Risk %s · Errors %s · Rules %s'
                % (stats.get('parsed', 0), stats.get('headers', 0), stats.get('sources', 0),
                   cats.get(Category.PORTED, 0), cats.get(Category.GUESSED, 0),
                   cats.get(Category.STUBBED, 0), cats.get(Category.RISK, 0),
                   cats.get(Category.ERROR, 0), cats.get(Category.RULE, 0)))
            self._cv_ready('Conversion complete.')
        else:
            self._cv_fail('Conversion failed. See the log (Open reports).')
        self._close_log_file()

    def _cv_run_tools(self, out):
        """Run each INCLUDED tool (in order) using the captured config, now that the output exists."""
        self._auto_open_reports = False                        # don't pop reports open at convert-end
        order = ['assets', 'audio', 'decisions', 'uimark', 'achievements', 'coverage', 'devutils']
        labels = {'assets': 'Extracting assets', 'audio': 'Building audio', 'decisions': 'Emitting config',
                  'uimark': 'Marking UI', 'achievements': 'Mapping achievements',
                  'coverage': 'Writing coverage', 'devutils': 'Dev utilities'}
        for sid in order:
            step = next((s for s in self._steps if s['id'] == sid), None)
            if step is None or not self._wiz_applies(step) or not self._tool_include.get(sid, True):
                continue
            self._cv_status.setText('%s…' % labels.get(sid, sid))
            QApplication.processEvents()
            try:
                self._run_tool_backend(sid, out)
            except Exception as e:
                self.logln('%s failed: %s' % (sid, e))
        self._auto_open_reports = True                         # restore for manual tool clicks

    def _run_tool_backend(self, sid, out):
        cfg = getattr(self, '_cfg', {})
        if sid == 'assets':
            from . import assets
            a = cfg.get('assets', {})
            jar = a.get('jar')
            if jar and os.path.isfile(jar):
                assets.run(jar, a.get('content') or os.path.join(out, 'Content'), code_dir=out)
        elif sid == 'audio':
            from . import xact
            a = cfg.get('audio', {})
            res = a.get('resources')
            if res and os.path.isdir(res):
                m = xact.match_sources(self.ed_java.text().strip(), res)
                ao = a.get('audio_out') or os.path.join(out, 'Content', 'Audio')
                os.makedirs(ao, exist_ok=True)
                xact.write_ffmpeg_script(m, ao, res)
        elif sid == 'decisions':
            from . import platform_config, sdk
            updates = {'invert_y': cfg.get('invert_y', False)}
            for did, key in (getattr(self, '_decision_values', {}) or {}).items():
                if did == 'controller_layout':
                    updates['layout'] = key
                elif did in ('keyboard', 'storage'):
                    updates[did] = key
                else:
                    updates['subsystem.%s' % did] = key
            try:
                platform_config.run(out, updates=updates, platform=self._selected_platform_id(),
                                    sdk_override=self.ed_sdk.text().strip() or None)
            except sdk.SdkNotFound as e:
                self.logln('platform-config: %s' % e)
        elif sid == 'uimark':
            from . import ui_mark
            d = cfg.get('uimark', {}).get('dir') or out
            if os.path.isdir(d):
                ui_mark.run(d)
        elif sid == 'achievements':
            from . import achievements
            a = cfg.get('achievements', {})
            deliv = (getattr(self, '_decision_values', {}) or {}).get('achievements', 'online')
            achievements.run(out, out, lang_file=a.get('lang') or None,
                             profile=a.get('profile', 'retail'), delivery=deliv,
                             title_id=a.get('title_id') or None)
        elif sid == 'coverage':
            from . import platform_coverage
            ref = os.path.join(REPO_ROOT, 'docs', 'platform', 'reference')
            platform_coverage.write_report(out, ref if os.path.isdir(ref) else None)
        elif sid == 'devutils':
            runners = {'Scan Stubs': self.run_stubs, 'Regen vcxproj': self.run_vcxproj,
                       'Signatures': self.run_signatures, 'Worklist': self.run_worklist}
            for name in getattr(self, '_devutil_sel', []):
                try:
                    runners[name]()
                except Exception as e:
                    self.logln('%s failed: %s' % (name, e))

    def _apply_project_layout(self, out):
        """Restructure the output into the standard VS solution layout (<root>/<Name>.sln +
        <root>/<Name>/…), so pointing the converter at a folder never leaves a flat dump or a
        duplicate copy. Xbox360 C++ only -- other targets don't use a .vcxproj solution."""
        if self._selected_platform_id() != 'xbox360' or getattr(self, '_target_language', 'cpp') != 'cpp':
            return
        try:
            from . import project_layout
            r = project_layout.layout(out)
            if r['moved']:
                self.logln('[layout] moved %d entr%s into %s\\ (solution subfolder)'
                           % (r['moved'], 'y' if r['moved'] == 1 else 'ies', r['name']))
            self.logln('[layout] solution ready: %s  +  %s\\' % (r['sln'], r['proj_dir']))
        except Exception as e:
            self.logln('[layout] skipped: %s' % e)

    def _cv_ready(self, title):
        self._cv_title.setText(title)
        self._cv_status.setText('Output:  %s' % self.ed_out.text().strip())
        for b in (self._cv_out, self._cv_rep, self._cv_done):
            b.setEnabled(True)

    def _cv_fail(self, msg):
        self._cv_title.setText('Conversion failed')
        self._cv_title.setStyleSheet('font-size:18px; font-weight:bold; color:%s;' % FAIL)
        self._cv_status.setText(msg)
        self._cv_rep.setEnabled(True); self._cv_done.setEnabled(True)

    def _summary_rows(self):
        rows = [('Platform', self.combo_platform.currentText()),
                ('Language', '%s %s' % (self._LANG_LABEL.get(getattr(self, '_target_language', 'cpp'), 'C++'),
                                        getattr(self, '_target_version', None) or self.combo_std.currentText())),
                ('Source', self.ed_java.text().strip() or '(unset)'),
                ('Output', self.ed_out.text().strip() or '(unset)'),
                ('SDK', self.ed_sdk.text().strip() or '(auto-detected)')]
        # what the auto-scan found from the project folder (so it's clear it was prefilled)
        r = getattr(self, '_resolved', {})
        if r.get('jar'):
            rows.append(('Assets jar', r['jar'] + '   (auto)'))
        if r.get('resources'):
            rows.append(('Resources', r['resources'] + '   (auto)'))
        if r.get('lang'):
            rows.append(('Lang', r['lang'] + '   (auto)'))
        return rows

    def _finalize_conversion(self, stats, out_dir):
        """Merge the report analysis into the ledger, refresh stat cards, write DIAGNOSTICS.md.
        Returns the ledger (for the Converting window to show its totals)."""
        ledger = stats.get('ledger')
        try:
            from . import analyze
            if ledger is not None:
                analyze.analyze_all(ledger, out_dir, root=REPO_ROOT, platform=self._selected_platform_id())
        except Exception as e:
            self.logln('[report] analysis skipped: %s' % e)
        self._refresh_stat_cards(ledger)
        self._write_diagnostics_md(ledger, out_dir)
        return ledger

    def _reg_btn(self, text, slot, tip, primary=False):
        b = QPushButton(text)
        if primary:
            b.setObjectName('primary')
        b.setToolTip(tip)
        b.clicked.connect(slot)
        self._action_buttons[text] = b
        return b

    def _path_row(self, label, edit):
        row = QHBoxLayout()
        lab = QLabel(label)
        lab.setFixedWidth(70)
        lab.setStyleSheet('color:%s;' % MUTE)
        b = QPushButton('Browse')
        b.setFixedWidth(80)
        b.clicked.connect(lambda: self._browse(edit))
        row.addWidget(lab)
        row.addWidget(edit, 1)
        row.addWidget(b)
        return row

    def _browse(self, edit):
        d = QFileDialog.getExistingDirectory(self, 'Select folder', edit.text() or DESKTOP)
        if d:
            edit.setText(d)

    def closeEvent(self, e):
        self.settings.setValue('java', self.ed_java.text())
        self.settings.setValue('out', self.ed_out.text())
        self.settings.setValue('sdk', self.ed_sdk.text())
        self.settings.setValue('cpp_std', self.combo_std.currentText())
        self.settings.setValue('platform', self.combo_platform.currentText())
        super().closeEvent(e)

    def _set_stage(self, name, state):
        colors = {'pending': MUTE, 'active': '#e2e0d8', 'done': OK, 'fail': FAIL}
        if name in self.stages:
            self.stages[name].setStyleSheet('color:%s;' % colors.get(state, MUTE))

    def logln(self, text):
        # logging goes to the log FILE only -- there is no log panel in the UI. (self.log is a hidden
        # in-memory buffer kept so any code that appends still works.)
        if self._log_file:
            try:
                self._log_file.write(text + '\n')
                self._log_file.flush()
            except Exception:
                pass
        try:
            self.log.append(text)
        except Exception:
            pass
        QApplication.processEvents()

    def _open_log_file(self):
        out_dir = self.ed_out.text().strip()
        if out_dir:
            os.makedirs(out_dir, exist_ok=True)
            path = os.path.join(out_dir, 'conversion.log')
            try:
                self._log_file = open(path, 'w', encoding='utf-8')
                self.logln('logging to %s' % path)
            except Exception as e:
                self.logln('WARNING: could not open log file: %s' % e)
                self._log_file = None

    def _close_log_file(self):
        if self._log_file:
            try:
                self._log_file.close()
            except Exception:
                pass
            self._log_file = None

    def _reset_cards(self):
        for w in (self.m_parsed, self.m_headers, self.m_sources):
            w.setText('-')
        for key, (frame, val) in self._cat_cards.items():
            val.setText('-')

    def _set_busy(self, busy):
        for b in self._action_buttons.values():
            b.setEnabled(not busy)
        self.btn_run_all.setEnabled(not busy)

    def _refresh_stat_cards(self, ledger):
        """Update the stats strip (the report at a glance) from the ledger categories."""
        if ledger is None:
            return
        cats = ledger.totals_by_category()
        for key in ('PORTED', 'GUESSED', 'STUBBED', 'RISK', 'ERROR', 'RULE'):
            if key in self._cat_cards:
                self._cat_cards[key][1].setText(str(cats.get(key, 0)))

    def _write_diagnostics_md(self, ledger, out_dir):
        """Write DIAGNOSTICS.md -- the pretty, grouped version of what the old Diagnostics panel showed
        (events by category, then by file). Returns the path or None."""
        if ledger is None or not out_dir:
            return None
        from .ledger import Category
        cat_order = [Category.ERROR, Category.BLOCKED, Category.GUESSED, Category.STUBBED,
                     Category.RISK, Category.WARNING, Category.RULE, Category.OVERRIDDEN,
                     Category.ANNOTATED, Category.PORTED]
        by_cat = {}
        for ev in ledger.events:
            by_cat.setdefault(ev.category, []).append(ev)
        totals = ledger.totals_by_category()
        L = ['# DIAGNOSTICS.md — conversion diagnostics', '',
             'Every event the conversion recorded, grouped by category then file. The summary counts '
             'match the stats strip in the tool.', '',
             '| Category | Count |', '|---|---:|']
        for cat in cat_order:
            n = totals.get(cat, 0)
            if n:
                L.append('| %s | %d |' % (_CAT_LABELS.get(cat, cat), n))
        L.append('')
        for cat in cat_order:
            evs = by_cat.get(cat, [])
            if not evs:
                continue
            L += ['## %s (%d)' % (_CAT_LABELS.get(cat, cat), len(evs)), '']
            by_file = {}
            for ev in evs:
                by_file.setdefault(ev.file, []).append(ev)
            for fname in sorted(by_file):
                L.append('### `%s`' % fname)
                for ev in by_file[fname]:
                    loc = ':%d' % ev.line if ev.line else ''
                    L.append('- **%s**%s — %s' % (ev.severity, loc, ev.message))
                L.append('')
        path = os.path.join(out_dir, 'DIAGNOSTICS.md')
        try:
            with open(path, 'w', encoding='utf-8') as f:
                f.write('\n'.join(L) + '\n')
        except (IOError, OSError):
            return None
        return path

    # ---- C++ STANDARD SELECTOR ------------------------------------------
    _STD_HINTS = {
        'C++98': 'ISO C++98 — NULL, no auto, no range-for',
        'C++03': 'ISO C++03 — NULL, no auto, no range-for',
        'C++11': 'C++11 — nullptr, auto, range-for, initializer_list',
        'C++14': 'C++14 — generic lambdas, relaxed constexpr',
        'C++17': 'C++17 — structured bindings, if-init, std::optional',
        'C++20': 'C++20 — concepts, ranges, coroutines',
        'C++23': 'C++23 — deducing this, std::expected',
    }

    def _on_std_changed(self, text):
        self._std_desc.setText(self._STD_HINTS.get(text, ''))

    def _on_platform_changed(self, index):
        from .platforms import PLATFORMS, default_sdk_dir
        pid = self._platform_ids[index] if 0 <= index < len(self._platform_ids) else None
        plat = PLATFORMS.get(pid, {}) if pid else {}
        self._plat_desc.setText(plat.get('description', ''))
        label = plat.get('label', '')
        if label:
            self.setWindowTitle('j2x — Java → %s Converter' % label)
        sdk = default_sdk_dir(pid, REPO_ROOT) if pid else ''
        cur = self.ed_sdk.text().strip()
        if sdk and (not cur or self._is_platform_default_sdk(cur)):
            self.ed_sdk.setText(sdk)

    def _is_platform_default_sdk(self, path):
        """True if `path` is the default SDK dir for any platform."""
        from .platforms import PLATFORMS, default_sdk_dir
        norm = os.path.normpath(path)
        for pid in PLATFORMS:
            d = default_sdk_dir(pid, REPO_ROOT)
            if d and os.path.normpath(d) == norm:
                return True
        return False

    def _selected_platform_id(self):
        idx = self.combo_platform.currentIndex()
        if 0 <= idx < len(self._platform_ids):
            return self._platform_ids[idx]
        return 'xbox360'

    def _emit_debug_enabled(self):
        """Debug support is the 'Debug support' question in the Platform-decisions modal. Read the
        captured value (the modal's widgets are gone by convert time); default on."""
        vals = getattr(self, '_decision_values', None)
        if vals and 'debug' in vals:
            return vals['debug'] == 'on'
        grp = getattr(self, '_decision_groups', {}).get('debug')
        return grp['value']() == 'on' if grp else True

    def run_all(self):
        self.run_convert()

    # ---- CONVERT --------------------------------------------------------
    def run_convert(self):
        java_dir = self.ed_java.text().strip()
        out_dir = self.ed_out.text().strip()
        if not os.path.isdir(java_dir):
            QMessageBox.warning(self, 'Missing source', 'Source directory not found:\n%s' % java_dir)
            return
        os.makedirs(out_dir, exist_ok=True)
        # skeleton languages (C / C# / Rust) -> the scaffold emitter, not the full C++ pipeline
        if getattr(self, '_target_language', 'cpp') != 'cpp':
            self._run_scaffold(java_dir, out_dir)
            return
        if self.worker and self.worker.isRunning():
            self.logln('! conversion already running')
            return
        self._reset_cards()
        self.diag_tree.clear()
        self._set_busy(True)
        self._open_log_file()
        platform = self._selected_platform_id()
        sdk_dir = self.ed_sdk.text().strip()
        if sdk_dir and os.path.isdir(sdk_dir):
            self.logln('SDK: %s' % sdk_dir)
        self.worker = ConvertWorker(java_dir, out_dir, cpp_std=self.combo_std.currentText(),
                                    platform=platform, emit_debug=self._emit_debug_enabled(),
                                    sdk_override=sdk_dir or None)
        self.worker.line.connect(self.logln)
        self.worker.done.connect(self._convert_done)
        self.worker.start()

    def _run_scaffold(self, java_dir, out_dir):
        from . import scaffold
        lang = self._target_language
        self._reset_cards()
        self._open_log_file()
        self.logln('scaffolding %s skeletons from %s ...' % (lang, java_dir))
        try:
            r = scaffold.run(java_dir, out_dir, lang)
            self.m_parsed.setText(str(r['classes']))
            self.m_sources.setText(str(len(r['written'])))
            self.logln('[done] %d %s skeleton file(s) -> %s' % (len(r['written']), lang, out_dir))
            QMessageBox.information(self, 'Skeletons generated',
                                    '%d %s skeleton file(s) written to\n\n%s'
                                    % (len(r['written']), self._LANG_LABEL.get(lang, lang), out_dir))
            os.startfile(out_dir)
        except Exception as e:
            self.logln('ERROR: %s: %s' % (type(e).__name__, e))
            QMessageBox.critical(self, 'Scaffold failed', str(e))
        finally:
            self._close_log_file()

    def _convert_done(self, rc, stats):
        self._set_busy(False)
        if rc == 0:
            self.m_parsed.setText(str(stats.get('parsed', 0)))
            self.m_headers.setText(str(stats.get('headers', 0)))
            self.m_sources.setText(str(stats.get('sources', 0)))
            ledger = stats.get('ledger')
            out_dir = self.ed_out.text().strip()
            # merge the report analysis (markers / rules / lint) into the SAME ledger so the stats
            # strip IS the report; the detail goes to DIAGNOSTICS.md (no on-screen panel).
            try:
                from . import analyze
                if ledger is not None:
                    c = analyze.analyze_all(ledger, out_dir, root=REPO_ROOT,
                                            platform=self._selected_platform_id())
                    self.logln('[report] markers=%d rules=%d lint=%d'
                               % (c.get('markers', 0), c.get('rules', 0), c.get('lint', 0)))
            except Exception as e:
                self.logln('[report] analysis skipped: %s' % e)
            self._refresh_stat_cards(ledger)
            dpath = self._write_diagnostics_md(ledger, out_dir)
            if dpath:
                self.logln('[diagnostics] wrote %s' % dpath)
            self.logln('[done] %d event(s). See DIAGNOSTICS.md / the log for details.'
                       % (len(ledger) if ledger else 0))
        else:
            self.logln('[Convert] FAILED (rc=%d)' % rc)
            QMessageBox.critical(self, 'Conversion failed',
                                 'The conversion failed. See the log file (Open log).')
        self._close_log_file()

    # ---- REPORT ---------------------------------------------------------
    def run_report(self):
        out_dir = self.ed_out.text().strip()
        if not os.path.isdir(out_dir):
            self.logln('ERROR: output dir not found (convert first): %s' % out_dir)
            return
        self._set_stage('Report', 'active')
        QApplication.processEvents()
        try:
            from . import analyze, report
            from .ledger import Ledger, Severity

            ledger = Ledger()
            platform = self._selected_platform_id()
            counts = analyze.analyze_all(
                ledger, out_dir,
                root=REPO_ROOT,
                platform=platform)

            meta = {
                'output_dir': os.path.abspath(out_dir),
                'build_log': counts.get('_build_log_path') or '(none)',
                'profile': 'alpha',
            }
            report_dir = report.write_reports(ledger, out_dir, meta=meta)

            sevs = ledger.totals_by_severity()
            self.logln('Report: markers=%d  rules=%d  lint=%d'
                       % (counts['markers'], counts['rules'], counts['lint']))
            self.logln('  events: %d  (HIGH=%d MED=%d LOW=%d)'
                       % (len(ledger), sevs.get(Severity.HIGH, 0),
                          sevs.get(Severity.MED, 0), sevs.get(Severity.LOW, 0)))

            self.report_tree.clear()
            for fname, score, cats, sevlist in ledger.file_risk_table():
                if score == 0:
                    continue
                it = QTreeWidgetItem([
                    str(score), fname,
                    str(cats) if cats else '',
                    str(sevlist) if sevlist else ''])
                color = FAIL if score >= 10 else (WARN if score >= 3 else MUTE)
                it.setForeground(0, QColor(color))
                self.report_tree.addTopLevelItem(it)

            self.logln('Report written to %s' % os.path.abspath(report_dir))
            self._set_stage('Report', 'done')
            if self.report_tree.topLevelItemCount() > 0:
                self.tabs.setCurrentWidget(self.report_tree)
        except Exception as e:
            self.logln('ERROR: report failed: %s' % e)
            self._set_stage('Report', 'fail')

    # ---- VCXPROJ --------------------------------------------------------
    def run_vcxproj(self):
        self._set_stage('vcxproj', 'active')
        QApplication.processEvents()
        try:
            from . import vcxproj
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf):
                vcxproj.run()
            self.logln(buf.getvalue().strip())
            self._set_stage('vcxproj', 'done')
        except Exception as e:
            self.logln('ERROR: vcxproj failed: %s' % e)
            self._set_stage('vcxproj', 'fail')

    # ---- BUILD XEX (QProcess -> MSBuild) --------------------------------
    def run_build(self):
        if self.proc is not None and self.proc.state() != QProcess.NotRunning:
            self.logln('! a build is already running')
            return
        if not os.path.exists(DEFAULT_MSBUILD):
            self.logln('! MSBuild not found: %s' % DEFAULT_MSBUILD)
            self._set_stage('Build XEX', 'fail')
            return
        self.err_list.clear()
        self._set_stage('Build XEX', 'active')
        self.logln('$ msbuild %s (Release|Xbox 360)' % os.path.basename(DEFAULT_SLN))
        self.proc = QProcess(self)
        self.proc.setWorkingDirectory(PROJECT_DIR)
        self.proc.setProcessChannelMode(QProcess.MergedChannels)
        self.proc.readyReadStandardOutput.connect(self._read_build)
        self.proc.finished.connect(self._build_done)
        self.proc.start(DEFAULT_MSBUILD,
                        [DEFAULT_SLN, '/p:Configuration=Release', '/p:Platform=Xbox 360',
                         '/nologo', '/v:minimal', '/m'])

    def _read_build(self):
        if not self.proc:
            return
        data = bytes(self.proc.readAllStandardOutput()).decode('utf-8', 'replace')
        for line in data.splitlines():
            self.logln(line)
            m = _ERR_RE.match(line)
            if m and 'error' in m.group(3):
                path, ln = m.group(1).strip(), m.group(2)
                kind, code, msg = m.group(3), m.group(4), m.group(5)
                it = QListWidgetItem('%s(%s): %s %s: %s'
                                     % (os.path.basename(path), ln, kind, code, msg))
                it.setForeground(QColor(FAIL))
                it.setData(Qt.UserRole, (path, int(ln)))
                self.err_list.addItem(it)

    def _build_done(self, code, _status):
        self._set_stage('Build XEX', 'done' if code == 0 else 'fail')
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
            path = os.path.join(PROJECT_DIR, 'MinecraftAlpha', path)
        if os.path.isfile(path):
            os.startfile(path)

    # ---- SIGNATURES -----------------------------------------------------
    def run_signatures(self):
        java_dir = self.ed_java.text().strip()
        if not os.path.isdir(java_dir):
            self.logln('ERROR: Java source dir not found: %s' % java_dir)
            return
        self.logln('generating signatures.md ...')
        QApplication.processEvents()
        try:
            from . import signatures
            out_path = os.path.join(REPO_ROOT, 'signatures.md')
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf):
                signatures.run(java_dir, out_path)
            self.logln(buf.getvalue().strip())
        except Exception as e:
            self.logln('ERROR: signatures failed: %s' % e)

    # ---- WORKLIST -------------------------------------------------------
    def run_worklist(self):
        out_dir = self.ed_out.text().strip()
        if not os.path.isdir(out_dir):
            self.logln('ERROR: output dir not found (convert first): %s' % out_dir)
            return
        self.logln('compiling worklist ...')
        QApplication.processEvents()
        try:
            from . import worklist
            buf = io.StringIO()
            with contextlib.redirect_stdout(buf):
                worklist.run(out_dir)
            self.logln(buf.getvalue().strip())
        except Exception as e:
            self.logln('ERROR: worklist failed: %s' % e)

    # ---- STUBS ----------------------------------------------------------
    def run_stubs(self):
        out_dir = self.ed_out.text().strip()
        if not os.path.isdir(out_dir):
            self.logln('ERROR: output dir not found (convert first): %s' % out_dir)
            return
        sdk_dir = self.ed_sdk.text().strip() or None
        self.logln('scanning stubs in %s ...' % out_dir)
        if sdk_dir:
            self.logln('  SDK dir: %s' % sdk_dir)
        QApplication.processEvents()
        try:
            from . import stubs as stubs_mod
            stub_list = stubs_mod.scan_stubs(out_dir)

            sdk_apis = {}
            sdk_matches = {}
            if sdk_dir and os.path.isdir(sdk_dir):
                self.logln('  scanning SDK headers ...')
                QApplication.processEvents()
                sdk_apis = stubs_mod.scan_sdk(sdk_dir)
                self.logln('  found %d SDK headers' % len(sdk_apis))
                sdk_matches = stubs_mod.match_sdk_to_stubs(stub_list, sdk_apis)

            # group by subsystem and render STUBS.md (pretty), then open it
            by_sys = {}
            for stub in stub_list:
                by_sys.setdefault(stub['subsystem'], []).append(stub)
            total_methods = sum(s['method_count'] for s in stub_list)
            self.logln('  %d stubs, %d methods across %d subsystems'
                       % (len(stub_list), total_methods, len(by_sys)))

            L = ['# STUBS.md — placeholder surface by subsystem', '',
                 'Every emitted stub, grouped by subsystem, with the SDK hint / matched header. '
                 'A stub is fine if its effect is cosmetic; verify the load-bearing ones.', '',
                 '- stubs: %d   methods: %d   subsystems: %d' % (len(stub_list), total_methods, len(by_sys)), '']
            for sid in list(stubs_mod.SUBSYSTEMS.keys()) + ['unknown']:
                group = by_sys.get(sid)
                if not group:
                    continue
                info = stubs_mod.SUBSYSTEMS.get(sid)
                label = info['label'] if info else 'Unknown'
                sdk_hint = info['sdk_hint'] if info else ''
                mtot = sum(s['method_count'] for s in group)
                L += ['## %s — %d stub(s), %d method(s)' % (label, len(group), mtot), '']
                if sdk_hint:
                    L += ['_SDK: %s_' % sdk_hint, '']
                for stub in sorted(group, key=lambda s: s['class_name']):
                    matches = sdk_matches.get(stub['class_name'], [])
                    hdrs = sorted(set(h for _, h, _ in matches))
                    note = ' — SDK: %s' % ', '.join(hdrs[:3]) if hdrs else ''
                    L.append('### `%s.h` (%d)%s' % (stub['class_name'], stub['method_count'], note))
                    for method in stub['methods']:
                        sig = '%s %s(%s)' % (method['return'], method['name'], method['params'])
                        mi = ''
                        for mname, mheader, mapi in matches:
                            if mname == method['name']:
                                mi = '  → `%s` in %s' % (mapi, mheader)
                                break
                        L.append('- `%s`%s' % (sig, mi))
                    L.append('')
            path = os.path.join(out_dir, 'STUBS.md')
            with open(path, 'w', encoding='utf-8') as f:
                f.write('\n'.join(L) + '\n')
            self.logln('[stubs] wrote %s' % path)
            if getattr(self, '_auto_open_reports', True):      # off during a conversion run
                os.startfile(path)
        except Exception as e:
            import traceback
            self.logln('ERROR: stubs scan failed: %s' % e)
            self.logln(traceback.format_exc())
            QMessageBox.critical(self, 'Stub scan failed', str(e))

    # ---- OPEN -----------------------------------------------------------
    def open_output(self):
        out = self.ed_out.text().strip()
        if os.path.isdir(out):
            os.startfile(out)
        else:
            self.logln('! output dir not found: %s' % out)

    # ==== TOOLS TAB ======================================================
    # Every CLI tool, surfaced as a UI panel so the GUI is the primary interface.
    def _tool(self, fn, *a, **k):
        """Run a backend function, echo its stdout to the Log tab, report errors inline."""
        self.tabs.setCurrentWidget(self.log)
        QApplication.processEvents()
        buf = io.StringIO()
        try:
            with contextlib.redirect_stdout(buf):
                rv = fn(*a, **k)
            txt = buf.getvalue().strip()
            if txt:
                self.logln(txt)
            return rv
        except Exception as e:
            import traceback
            self.logln('ERROR: %s: %s' % (type(e).__name__, e))
            self.logln(traceback.format_exc())
            return None

    def _tool_field(self, grid, row, label, edit, browse=True, tip='', file_filter=None):
        lab = QLabel(label)
        lab.setStyleSheet('color:%s;' % MUTE)
        lab.setFixedWidth(90)
        if tip:
            edit.setToolTip(tip)
        grid.addWidget(lab, row, 0)
        grid.addWidget(edit, row, 1)
        if browse:
            b = QPushButton('Browse')
            b.setFixedWidth(74)
            if file_filter:                               # a FILE picker (e.g. the jar), not a folder
                b.clicked.connect(lambda: self._browse_file(edit, file_filter))
            else:
                b.clicked.connect(lambda: self._browse(edit))
            grid.addWidget(b, row, 2)

    def _browse_file(self, edit, file_filter):
        cur = edit.text().strip()
        start = os.path.dirname(cur) if cur else DESKTOP
        p, _ = QFileDialog.getOpenFileName(self, 'Select file', start, file_filter)
        if p:
            edit.setText(p)

    def _combo(self, items, current=None, width=150):
        c = QComboBox()
        c.addItems(items)
        if current and current in items:
            c.setCurrentIndex(items.index(current))
        c.setFixedWidth(width)
        c.setStyleSheet(
            'QComboBox { background:#1c1c1a; border:1px solid #3a3a37; border-radius:6px; '
            'padding:4px 8px; color:#e2e0d8; } QComboBox QAbstractItemView { background:#262624; '
            'color:#e2e0d8; selection-background-color:#2f3a4a; }')
        return c

    def _choice_group(self, decision, current):
        """A decision posed as a QUESTION + why, then an exclusive multiple-choice BUTTON GROUP.
        `decision` is a platform_config.DECISIONS dict. Below the buttons, the chosen option's
        consequence is shown and updates live. Returns {'widget','group','keys','value'}."""
        options = decision['options']
        w = QFrame()
        w.setObjectName('card')
        v = QVBoxLayout(w)
        v.setContentsMargins(11, 9, 11, 9)
        v.setSpacing(4)
        # the question
        q = QLabel(decision['question'])
        q.setStyleSheet('font-weight:bold; font-size:13px; color:#e2e0d8;')
        q.setWordWrap(True)
        v.addWidget(q)
        # why it matters
        why = QLabel(decision['why'])
        why.setStyleSheet('color:%s; font-size:11px;' % MUTE)
        why.setWordWrap(True)
        v.addWidget(why)
        # recommendation line
        rec_key, rec_reason = decision['recommend']
        rec = QLabel('Recommended: %s — %s' % (rec_key, rec_reason))
        rec.setStyleSheet('color:#7bbf95; font-size:11px;')
        rec.setWordWrap(True)
        v.addWidget(rec)
        # the buttons
        roww = QWidget()
        row = QHBoxLayout(roww)
        row.setContentsMargins(0, 3, 0, 3)
        row.setSpacing(6)
        bg = QButtonGroup(w)
        bg.setExclusive(True)
        keys, descs = [], {}
        for key, olabel, odesc in options:
            tag = '  ★' if key == rec_key else ''
            btn = QPushButton(olabel + tag)
            btn.setObjectName('choice')
            btn.setCheckable(True)
            btn.setToolTip(odesc)
            if key == current or (current is None and not keys):
                btn.setChecked(True)
            bg.addButton(btn, len(keys))
            row.addWidget(btn)
            descs[len(keys)] = odesc
            keys.append(key)
        row.addStretch(1)
        v.addWidget(roww)
        # live consequence of the current pick
        conseq = QLabel('')
        conseq.setStyleSheet('color:#c7c5bc; font-size:11px;')
        conseq.setWordWrap(True)

        def _update(_=None):
            idx = bg.checkedId()
            conseq.setText('→ ' + descs.get(idx, ''))
        bg.idClicked.connect(_update) if hasattr(bg, 'idClicked') else bg.buttonClicked.connect(_update)
        _update()
        v.addWidget(conseq)

        def value():
            idx = bg.checkedId()
            return keys[idx] if 0 <= idx < len(keys) else (keys[0] if keys else None)
        return {'widget': w, 'group': bg, 'keys': keys, 'value': value}

    def _tool_card(self, title, desc):
        card = QFrame()
        card.setObjectName('card')
        lay = QVBoxLayout(card)
        lay.setContentsMargins(12, 10, 12, 10)
        lay.setSpacing(6)
        t = QLabel(title)
        t.setStyleSheet('font-weight:bold; font-size:14px;')
        d = QLabel(desc)
        d.setStyleSheet('color:%s;' % MUTE)
        d.setWordWrap(True)
        lay.addWidget(t)
        lay.addWidget(d)
        return card, lay

    # ==== TOOL CARDS (each is one wizard step / modal) ===================
    def _tc_assets(self, col):
        out_default = self.ed_out.text().strip()
        card, lay = self._tool_card(
            'Assets — extract textures/resources',
            'Copy textures into Content\\ (exact paths); palettized PNGs are fixed to RGBA for the 360.')
        g = QGridLayout()
        ac = self._cfg.get('assets', {})
        self.ed_jar = QLineEdit(ac.get('jar') or getattr(self, '_resolved', {}).get('jar')
                                or self.settings.value('jar', ''))
        self.ed_jar.setPlaceholderText('deobfuscated.jar')
        self.ed_content = QLineEdit(ac.get('content') or os.path.join(out_default, 'Content'))
        self._tool_field(g, 0, 'Jar', self.ed_jar, tip='Game jar with the textures.',
                         file_filter='Java archive (*.jar);;All files (*.*)')
        self._tool_field(g, 1, 'Content dir', self.ed_content, tip='Destination Content\\ folder.')
        lay.addLayout(g)
        col.addWidget(card)

    def _tc_audio(self, col):
        out_default = self.ed_out.text().strip()
        ac = self._cfg.get('audio', {})
        self.ed_res = QLineEdit(ac.get('resources') or getattr(self, '_resolved', {}).get('resources')
                                or self.settings.value('resources', ''))
        self.ed_audio_out = QLineEdit(ac.get('audio_out') or os.path.join(out_default, 'Content', 'Audio'))
        card, lay = self._tool_card(
            'Audio (XACT) — banks & sources',
            'Match sound cues to the original .ogg and build the XACT banks.')
        g = QGridLayout()
        self.ed_res.setPlaceholderText('...\\game\\resources')
        self._tool_field(g, 0, 'Resources', self.ed_res, tip='Folder of original .ogg sounds.')
        self._tool_field(g, 1, 'Audio out', self.ed_audio_out, tip='Where to write banks / scripts.')
        lay.addLayout(g)
        col.addWidget(card)

    def _tc_decisions(self, col):
        card, lay = self._tool_card(
            'Platform decisions',
            'Answer each question, then emit. Every choice is explained inline.')
        from . import platform_config as _pc
        self._decision_groups = {}
        cfg = _pc.load_config(os.path.join(self.ed_out.text().strip() or REPO_ROOT,
                                           'platform.config.json'))
        dv = getattr(self, '_decision_values', {}) or {}
        for d in _pc.DECISIONS:
            cur = dv.get(d['id']) if d['id'] in dv else _pc.decision_value(cfg, d['id'])
            grp = self._choice_group(d, cur)
            self._decision_groups[d['id']] = grp
            lay.addWidget(grp['widget'])
        self.chk_invy = QCheckBox('Invert look Y')
        self.chk_invy.setChecked(self._cfg.get('invert_y', bool(cfg.get('controller', {}).get('invert_y'))))
        lay.addWidget(self.chk_invy)
        col.addWidget(card)

    def _tc_uimark(self, col):
        card, lay = self._tool_card(
            'UI marker — tag every UI function',
            'Tag every UI function with a greppable marker + write UI_INDEX.md. Re-run after a UI merge.')
        g = QGridLayout()
        self.ed_uidir = QLineEdit(self._cfg.get('uimark', {}).get('dir') or self.ed_out.text().strip())
        self._tool_field(g, 0, 'Code dir', self.ed_uidir, tip='Converted C++ tree to mark.')
        lay.addLayout(g)
        col.addWidget(card)

    def _tc_achievements(self, col):
        ac = self._cfg.get('achievements', {})
        self.ed_ach_lang = QLineEdit(ac.get('lang') or getattr(self, '_resolved', {}).get('lang') or '')
        self.ed_ach_titleid = QLineEdit(ac.get('title_id') or '00000000')
        self.combo_ach_profile = self._combo(['retail', 'arcade'], ac.get('profile', 'retail'), width=110)
        card, lay = self._tool_card(
            'Achievements — map to the console system',
            'Map achievements to the console: gamerscore, XLAST config, tile art, and the unlock seam.')
        g = QGridLayout()
        self.ed_ach_lang.setPlaceholderText('optional .lang for names (…\\Content\\lang\\en_US.lang)')
        self._tool_field(g, 0, 'Lang file', self.ed_ach_lang, tip='Optional — real names/descriptions.')
        self.ed_ach_titleid.setToolTip('32-bit hex. Self-issued; must not collide + must match the XEX.')
        lab = QLabel('Title ID')
        lab.setStyleSheet('color:%s;' % MUTE); lab.setFixedWidth(90)
        sug = QPushButton('Suggest')           # a helper (fills a value), not a run action
        sug.setFixedWidth(74)
        sug.clicked.connect(self._suggest_title_id)
        g.addWidget(lab, 1, 0); g.addWidget(self.ed_ach_titleid, 1, 1); g.addWidget(sug, 1, 2)
        lay.addLayout(g)
        row = QHBoxLayout()
        row.addWidget(QLabel('Budget'))
        row.addWidget(self.combo_ach_profile)
        row.addStretch(1)
        lay.addLayout(row)
        col.addWidget(card)

    def _tc_coverage(self, col):
        card, _lay = self._tool_card(
            'Platform coverage & SDK',
            'Writes PLATFORM_COVERAGE.md (REUSABLE / STUB / MISSING per subsystem + known considerations '
            'like the D3D9 half-pixel offset) using the located SDK. No inputs needed.')
        col.addWidget(card)

    def _tc_devutils(self, col):
        card, lay = self._tool_card(
            'Developer utilities',
            'Optional build-support commands. Tick any you want — they run AFTER the conversion, when '
            'the output exists. (None are needed for a normal conversion.)')
        self._devutil_checks = {}
        for name, tip in [
            ('Scan Stubs', 'Classify stubs by subsystem → STUBS.md (also scans the SDK; can take a moment)'),
            ('Regen vcxproj', 'Update the Xbox .vcxproj file list'),
            ('Signatures', 'Generate signatures.md codebase map'),
            ('Worklist', 'Compile-check the output → WORKLIST.md')]:
            cb = QCheckBox(name)
            cb.setToolTip(tip)
            cb.setChecked(name in getattr(self, '_devutil_sel', []))
            self._devutil_checks[name] = cb
            lay.addWidget(cb)
        col.addWidget(card)

    def _tool_steps(self):
        """The ordered wizard steps: (title, subtitle, builder, needs360)."""
        return [
            ('Assets', 'Extract the game textures/resources into the project.', self._tc_assets, False),
            ('Audio', 'Match sounds and build the audio banks.', self._tc_audio, True),
            ('Platform decisions', 'Answer the platform questions (controller, storage, audio, …).',
             self._tc_decisions, False),
            ('UI marker', 'Tag every UI function so a merge is auditable.', self._tc_uimark, False),
            ('Achievements', 'Map achievements onto the console system.', self._tc_achievements, True),
            ('Coverage & SDK', 'Check platform coverage and the SDK.', self._tc_coverage, False),
            ('Developer utilities', 'Optional build-support commands.', self._tc_devutils, False),
        ]

    def _run_tool_wizard(self):
        """Walk each tool as its own modal (Skip / Next). Decision values are captured so Convert can
        still read them after the modals close."""
        is360 = self._selected_platform_id() == 'xbox360'
        steps = [s for s in self._tool_steps() if is360 or not s[3]]
        for i, (title, subtitle, builder, _n) in enumerate(steps):
            dlg = ToolStepDialog(title, subtitle, builder, parent=self, last=(i == len(steps) - 1))
            accepted = dlg.exec() == QDialog.Accepted
            if title == 'Platform decisions':
                # capture the picks before the dialog's widgets are destroyed
                self._decision_values = dict((did, g['value']())
                                             for did, g in getattr(self, '_decision_groups', {}).items())
            elif title == 'Developer utilities':
                self._devutil_sel = [n for n, cb in getattr(self, '_devutil_checks', {}).items()
                                     if cb.isChecked()] if accepted else []
            dlg.deleteLater()
            if not accepted:
                continue    # Skip just moves on; there is no cancel-the-whole-wizard here

    # ---- tool handlers ----
    def tool_assets(self):
        from . import assets
        jar, content = self.ed_jar.text().strip(), self.ed_content.text().strip()
        code = self.ed_out.text().strip() or None
        if not os.path.isfile(jar):
            self.logln('! jar not found: %s' % jar); return
        self.settings.setValue('jar', jar); self.settings.setValue('content', content)
        res = self._tool(assets.run, jar, content, code_dir=code, only_needed=False)
        if res:
            self.logln('[assets] %d written, %d PNG(s) normalized to RGBA, %d missing'
                       % (len(res['written']), len(res['converted']),
                          len(res['coverage']['missing']) if res['coverage'] else 0))

    def tool_xact_match(self):
        from . import xact
        java, res, out = (self.ed_java.text().strip(), self.ed_res.text().strip(),
                          self.ed_audio_out.text().strip())
        if not os.path.isdir(res):
            self.logln('! resources dir not found: %s' % res); return
        self.settings.setValue('resources', res); self.settings.setValue('audio_out', out)
        os.makedirs(out, exist_ok=True)
        m = self._tool(xact.match_sources, java, res)
        if m:
            self._tool(xact.write_ffmpeg_script, m, out, res)
            self.logln('[xact] matched=%d missing=%d -> convert_audio.bat in %s'
                       % (len(m['matched']), len(m['missing']), out))

    def tool_xact_build(self):
        from . import xact
        java, out = self.ed_java.text().strip(), self.ed_audio_out.text().strip()
        if not os.path.isdir(java):
            self.logln('! Java src not found: %s' % java); return
        os.makedirs(out, exist_ok=True)
        r = self._tool(xact.build, java, out)
        if r:
            self.logln('[xact] built %s' % r[0])

    def tool_platform_config(self):
        from . import platform_config, sdk
        out = self.ed_out.text().strip()
        # gather every decision from its button group
        sel = dict((did, grp['value']()) for did, grp in self._decision_groups.items())
        updates = {'invert_y': self.chk_invy.isChecked()}
        for did, key in sel.items():
            if did == 'controller_layout':
                updates['layout'] = key
            elif did == 'keyboard':
                updates['keyboard'] = key
            elif did == 'storage':
                updates['storage'] = key
            else:
                updates['subsystem.%s' % did] = key      # audio/video/network/memory/user/debug
        plat = self._selected_platform_id()
        sdk_dir = self.ed_sdk.text().strip() or None
        try:
            cfg, written, info, unverified = platform_config.run(
                out, updates=updates, platform=plat, sdk_override=sdk_dir)
        except sdk.SdkNotFound as e:
            QMessageBox.warning(self, 'SDK required', str(e))
            return
        self.logln('[platform-config] wrote %d file(s) to %s' % (len(written), out))
        self.logln('  decisions: ' + '  '.join('%s=%s' % (d, k) for d, k in sorted(sel.items())))
        if unverified:
            self.logln('  WARNING: symbols not in located SDK: %s' % ', '.join(unverified))

    def tool_debug(self, on):
        from . import debug
        out = self.ed_out.text().strip()
        if not os.path.isdir(out):
            self.logln('! output dir not found: %s' % out); return
        paths = self._tool(debug.write_debug, out, on)
        if paths:
            self.logln('[debug] emitted %s (DEBUG_ENABLED=%d)'
                       % (', '.join(os.path.basename(p) for p in paths), 1 if on else 0))

    def tool_ui_mark(self, dry):
        from . import ui_mark
        d = self.ed_uidir.text().strip()
        if not os.path.isdir(d):
            self.logln('! code dir not found: %s' % d); return
        res = self._tool(ui_mark.run, d, dry_run=dry)
        if res:
            total = sum(len(v) for v in res['marked'].values())
            self.logln('[ui-mark] %d UI classes, %d methods %s (skipped %d)%s'
                       % (len(res['classes']), total, 'to mark' if dry else 'marked',
                          res['skipped'], '' if dry else ' -> UI_INDEX.md'))

    def _suggest_title_id(self):
        from . import achievements
        seed = os.path.basename(self.ed_out.text().rstrip('/\\')) or 'title'
        self.ed_ach_titleid.setText(achievements.suggest_title_id(seed))

    def tool_achievements(self):
        from . import achievements
        out = self.ed_out.text().strip()
        if not os.path.isdir(out):
            self.logln('! output dir not found: %s' % out); return
        lang = self.ed_ach_lang.text().strip() or None
        # tie delivery to the Achievements decision if that group exists
        delivery = 'online'
        if getattr(self, '_decision_groups', None) and 'achievements' in self._decision_groups:
            delivery = self._decision_groups['achievements']['value']()
        res = self._tool(achievements.run, out, out, lang_file=lang,
                         profile=self.combo_ach_profile.currentText(), delivery=delivery,
                         title_id=self.ed_ach_titleid.text().strip() or None)
        if res:
            m = res['mapping']
            self.logln('[achievements] %d found — %s, %d gamerscore, delivery=%s'
                       % (res['count'], m['profile'], m['total_score'], delivery))
            self.logln('  icons: %d wired, %d manual | title id 0x%s%s'
                       % (len(res['icons']['wired']), len(res['icons']['unresolved']),
                          res['title_id'], ' (suggested)' if res['title_suggested'] else ''))
            for w in m['warnings']:
                self.logln('  WARNING: %s' % w)

    def tool_coverage(self):
        from . import platform_coverage
        out = self.ed_out.text().strip()
        if not os.path.isdir(out):
            self.logln('! output dir not found: %s' % out); return
        ref = os.path.join(REPO_ROOT, 'docs', 'platform', 'reference')
        r = self._tool(platform_coverage.write_report, out,
                       ref if os.path.isdir(ref) else None)
        if r:
            self.logln('[coverage] wrote %s' % r[0])

    def tool_sdk(self):
        from . import sdk
        plat = self._selected_platform_id()
        override = self.ed_sdk.text().strip() or None
        msg = sdk.describe(plat, override=override)
        self.logln('[sdk] ' + msg)
        QMessageBox.information(self, 'SDK', msg)


def _sev_color(sev):
    return {'HIGH': FAIL, 'MED': WARN, 'LOW': MUTE}.get(sev, MUTE)


import html as _html

_CAT_TAG_RE = re.compile(
    r'\b(ERROR|GUESSED|STUBBED|BLOCKED|RISK|WARNING|OVERRIDDEN|ANNOTATED|PORTED|RULE|OVERRIDE|FAIL)\b')


def _colorize_log_line(text):
    escaped = _html.escape(text)
    def _repl(m):
        tag = m.group(1)
        lookup = tag
        if tag == 'OVERRIDE':
            lookup = 'OVERRIDDEN'
        elif tag == 'FAIL':
            lookup = 'ERROR'
        color = _CAT_COLORS.get(lookup, MUTE)
        return '<span style="color:%s; font-weight:bold;">%s</span>' % (color, tag)
    colored = _CAT_TAG_RE.sub(_repl, escaped)
    return '<pre style="margin:0; white-space:pre-wrap;">%s</pre>' % colored


def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    w = ConverterWindow()
    w.show()
    sys.exit(app.exec())
