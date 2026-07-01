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
        QHeaderView, QComboBox,
    )
except ImportError:
    sys.stderr.write('PySide6 is required.  Install with:\n    pip install pyside6\n')
    sys.exit(1)

PROJECT_DIR = os.path.normpath(os.path.join(REPO_ROOT, '..'))
DEFAULT_JAVA = r'C:\Users\mrtit\Desktop\MinecraftAlpha\minecraft\src'
DEFAULT_OUT = os.path.join(REPO_ROOT, 'out')
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
* { font-family: 'Segoe UI', sans-serif; font-size: 13px; color: #e2e0d8; }
QMainWindow, QWidget { background: #1c1c1a; }
QLabel#title { font-size: 16px; font-weight: bold; }
QFrame#card { background: #262624; border: 1px solid #3a3a37; border-radius: 8px; }
QFrame#strip { background: #262624; border: 1px solid #3a3a37; border-radius: 8px; }
QLineEdit { background: #1c1c1a; border: 1px solid #3a3a37; border-radius: 6px; padding: 5px 8px; }
QPushButton { background: #2d2d2a; border: 1px solid #44443f; border-radius: 6px; padding: 6px 14px; }
QPushButton:hover { background: #383834; }
QPushButton:disabled { color: #55554f; }
QPushButton#primary { background: #2f5d3f; border: 1px solid #3c7a52; }
QPushButton#primary:hover { background: #376e4a; }
QTreeWidget, QPlainTextEdit, QTextEdit, QListWidget {
    background: #161614; border: 1px solid #3a3a37; border-radius: 8px; }
QTreeWidget::item { padding: 3px 2px; }
QTreeWidget::item:selected, QListWidget::item:selected { background: #2f3a4a; }
QTabWidget::pane { border: 1px solid #3a3a37; border-radius: 8px; top: -1px; }
QTabBar::tab { background: #262624; color: #9a9a93; padding: 5px 14px;
    border: 1px solid #3a3a37; border-bottom: none;
    border-top-left-radius: 6px; border-top-right-radius: 6px; }
QTabBar::tab:selected { background: #161614; color: #e2e0d8; }
QSplitter::handle { background: #1c1c1a; }
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

    def __init__(self, java_dir, out_dir, cpp_std='C++03', platform='xbox360'):
        super().__init__()
        self.java_dir = java_dir
        self.out_dir = out_dir
        self.cpp_std = cpp_std
        self.platform = platform

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
                progress_cb=self._progress)
            self.done.emit(0, s)
        except Exception as e:
            import traceback
            self._progress('ERROR: %s: %s' % (type(e).__name__, e))
            self._progress(traceback.format_exc())
            self.done.emit(1, {})


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
        s = self.settings
        root = QWidget()
        self.setCentralWidget(root)
        outer = QVBoxLayout(root)
        outer.setContentsMargins(14, 14, 14, 14)
        outer.setSpacing(10)

        # header
        head = QHBoxLayout()
        title = QLabel('j2x Converter')
        title.setObjectName('title')
        self.btn_run_all = QPushButton('  Run All  ')
        self.btn_run_all.setObjectName('primary')
        self.btn_run_all.setToolTip('Convert → Report → Regen vcxproj')
        self.btn_run_all.clicked.connect(self.run_all)
        head.addWidget(title)
        head.addStretch(1)
        head.addWidget(self.btn_run_all)
        outer.addLayout(head)

        # path rows
        self.ed_java = QLineEdit(s.value('java', DEFAULT_JAVA))
        self.ed_java.setPlaceholderText('Java source directory...')
        self.ed_out = QLineEdit(s.value('out', DEFAULT_OUT))
        self.ed_out.setPlaceholderText('Output directory...')
        from .platforms import PLATFORMS as _PLATS, default_sdk_dir as _default_sdk
        _saved_plat_label = s.value('platform', 'Xbox 360')
        _saved_plat_id = next((k for k, v in _PLATS.items() if v['label'] == _saved_plat_label), 'xbox360')
        _saved_sdk = s.value('sdk', '')
        if not _saved_sdk:
            _saved_sdk = _default_sdk(_saved_plat_id, REPO_ROOT)
        self.ed_sdk = QLineEdit(_saved_sdk)
        self.ed_sdk.setPlaceholderText('Platform SDK directory (auto-detected from PlatformSDKs/)...')
        outer.addLayout(self._path_row('Java src', self.ed_java))
        outer.addLayout(self._path_row('Output', self.ed_out))
        outer.addLayout(self._path_row('SDK', self.ed_sdk))

        # C++ standard + platform selectors
        _COMBO_STYLE = (
            'QComboBox { background: #1c1c1a; border: 1px solid #3a3a37; '
            'border-radius: 6px; padding: 5px 8px; color: #e2e0d8; } '
            'QComboBox::drop-down { border: none; } '
            'QComboBox QAbstractItemView { background: #262624; color: #e2e0d8; '
            'selection-background-color: #2f3a4a; }')

        settings_row = QHBoxLayout()

        std_lab = QLabel('C++ std')
        std_lab.setFixedWidth(55)
        std_lab.setStyleSheet('color:%s;' % MUTE)
        self.combo_std = QComboBox()
        self.combo_std.addItems(['C++03', 'C++98', 'C++11', 'C++14', 'C++17', 'C++20', 'C++23'])
        saved_std = s.value('cpp_std', 'C++03')
        idx = self.combo_std.findText(saved_std)
        if idx >= 0:
            self.combo_std.setCurrentIndex(idx)
        self.combo_std.setFixedWidth(100)
        self.combo_std.setStyleSheet(_COMBO_STYLE)
        std_desc = QLabel('')
        std_desc.setStyleSheet('color:%s;' % MUTE)
        self._std_desc = std_desc
        self.combo_std.currentTextChanged.connect(self._on_std_changed)
        self._on_std_changed(self.combo_std.currentText())

        plat_lab = QLabel('Platform')
        plat_lab.setFixedWidth(60)
        plat_lab.setStyleSheet('color:%s;' % MUTE)
        self.combo_platform = QComboBox()
        from .platforms import PLATFORMS
        self._platform_ids = list(PLATFORMS.keys())
        self._platform_labels = [PLATFORMS[k]['label'] for k in self._platform_ids]
        self.combo_platform.addItems(self._platform_labels)
        saved_plat = s.value('platform', 'Xbox 360')
        plat_idx = self.combo_platform.findText(saved_plat)
        if plat_idx >= 0:
            self.combo_platform.setCurrentIndex(plat_idx)
        self.combo_platform.setFixedWidth(170)
        self.combo_platform.setStyleSheet(_COMBO_STYLE)
        plat_desc = QLabel('')
        plat_desc.setStyleSheet('color:%s;' % MUTE)
        self._plat_desc = plat_desc
        self.combo_platform.currentIndexChanged.connect(self._on_platform_changed)
        self._on_platform_changed(self.combo_platform.currentIndex())

        settings_row.addWidget(std_lab)
        settings_row.addWidget(self.combo_std)
        settings_row.addSpacing(20)
        settings_row.addWidget(plat_lab)
        settings_row.addWidget(self.combo_platform)
        settings_row.addWidget(plat_desc, 1)
        outer.addLayout(settings_row)

        # metric cards — top row: counts, bottom row: categories
        cards_top = QHBoxLayout()
        cards_top.setSpacing(10)
        f1, self.m_parsed = _card('Parsed', OK)
        f2, self.m_headers = _card('Headers', INFO)
        f3, self.m_sources = _card('Sources', INFO)
        for f in (f1, f2, f3):
            cards_top.addWidget(f)
        outer.addLayout(cards_top)

        cards_cat = QHBoxLayout()
        cards_cat.setSpacing(10)
        self.m_ported_card = _card('Ported', _CAT_COLORS['PORTED'])
        self.m_guessed_card = _card('Guessed', _CAT_COLORS['GUESSED'])
        self.m_stubbed_card = _card('Stubbed', _CAT_COLORS['STUBBED'])
        self.m_risk_card = _card('Risk', _CAT_COLORS['RISK'])
        self.m_error_card = _card('Errors', _CAT_COLORS['ERROR'])
        self.m_rules_card = _card('Rules', _CAT_COLORS['RULE'])
        self._cat_cards = {
            'PORTED': self.m_ported_card,
            'GUESSED': self.m_guessed_card,
            'STUBBED': self.m_stubbed_card,
            'RISK': self.m_risk_card,
            'ERROR': self.m_error_card,
            'RULE': self.m_rules_card,
        }
        for key in ('PORTED', 'GUESSED', 'STUBBED', 'RISK', 'ERROR', 'RULE'):
            cards_cat.addWidget(self._cat_cards[key][0])
        outer.addLayout(cards_cat)

        # stage strip
        strip = QFrame()
        strip.setObjectName('strip')
        sl = QHBoxLayout(strip)
        sl.setContentsMargins(12, 8, 12, 8)
        sl.setSpacing(8)
        self.stages = {}
        stage_names = ['Convert', 'Report', 'vcxproj', 'Build XEX']
        for i, n in enumerate(stage_names):
            lab = QLabel(n)
            lab.setStyleSheet('color:%s;' % MUTE)
            self.stages[n] = lab
            sl.addWidget(lab)
            if i < len(stage_names) - 1:
                sep = QLabel('>')
                sep.setStyleSheet('color:#55554f;')
                sl.addWidget(sep)
        sl.addStretch(1)
        outer.addWidget(strip)

        # tabs
        self.tabs = QTabWidget()

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

        self.tabs.addTab(self.log, 'Log')
        self.tabs.addTab(self.diag_tree, 'Diagnostics')
        self.tabs.addTab(self.report_tree, 'Report')
        self.tabs.addTab(self.stubs_tree, 'Stubs')
        self.tabs.addTab(self.err_list, 'Build errors')
        self.tabs.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        outer.addWidget(self.tabs, 1)

        # action buttons
        btns = QHBoxLayout()
        btns.setSpacing(8)
        self._action_buttons = {}
        for text, slot, tip in [
            ('Convert',       self.run_convert,    'Parse Java + emit C++ (target std + platform)'),
            ('Report',        self.run_report,     'Crash-class analysis on converted output'),
            ('Scan Stubs',    self.run_stubs,      'Classify stubs by subsystem + match to SDK'),
            ('Regen vcxproj', self.run_vcxproj,    'Update Xbox .vcxproj file list'),
            ('Build XEX',     self.run_build,      'MSBuild the Xbox 360 project'),
            ('Signatures',    self.run_signatures, 'Generate signatures.md codebase map'),
            ('Worklist',      self.run_worklist,   'Compile-check output, write WORKLIST.md'),
            ('Open output',   self.open_output,    'Open the output folder in Explorer'),
        ]:
            b = QPushButton(text)
            b.setToolTip(tip)
            b.clicked.connect(slot)
            btns.addWidget(b)
            self._action_buttons[text] = b
        btns.addStretch(1)
        outer.addLayout(btns)

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
        d = QFileDialog.getExistingDirectory(self, 'Select folder', edit.text() or REPO_ROOT)
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
        html = _colorize_log_line(text)
        self.log.append(html)
        self.log.verticalScrollBar().setValue(self.log.verticalScrollBar().maximum())
        if self._log_file:
            try:
                self._log_file.write(text + '\n')
                self._log_file.flush()
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

    def _populate_diagnostics(self, ledger):
        self.diag_tree.clear()
        if ledger is None:
            return
        from .ledger import Category
        cats = ledger.totals_by_category()

        # update category cards
        card_map = {
            'PORTED': 'PORTED', 'GUESSED': 'GUESSED', 'STUBBED': 'STUBBED',
            'RISK': 'RISK', 'ERROR': 'ERROR', 'RULE': 'RULE',
        }
        for cat_key, card_key in card_map.items():
            if card_key in self._cat_cards:
                self._cat_cards[card_key][1].setText(str(cats.get(cat_key, 0)))

        # build tree: one top-level node per category (non-zero only)
        cat_order = [Category.ERROR, Category.GUESSED, Category.STUBBED, Category.BLOCKED,
                     Category.RISK, Category.WARNING, Category.OVERRIDDEN, Category.ANNOTATED,
                     Category.PORTED, Category.RULE]

        by_cat = {}
        for ev in ledger.events:
            by_cat.setdefault(ev.category, []).append(ev)

        for cat in cat_order:
            evs = by_cat.get(cat, [])
            if not evs:
                continue
            color = _CAT_COLORS.get(cat, MUTE)
            label = _CAT_LABELS.get(cat, cat)
            cat_node = QTreeWidgetItem([
                '%s (%d)' % (label, len(evs)), '', ''])
            cat_node.setForeground(0, QColor(color))
            cat_node.setExpanded(cat in (Category.ERROR, Category.GUESSED, Category.STUBBED,
                                         Category.BLOCKED, Category.RISK))

            # group events by file within category
            by_file = {}
            for ev in evs:
                by_file.setdefault(ev.file, []).append(ev)
            for fname in sorted(by_file.keys()):
                file_evs = by_file[fname]
                if len(file_evs) == 1:
                    ev = file_evs[0]
                    child = QTreeWidgetItem([
                        fname, ev.severity, ev.message])
                    child.setForeground(0, QColor(color))
                    child.setForeground(1, QColor(_sev_color(ev.severity)))
                    cat_node.addChild(child)
                else:
                    file_node = QTreeWidgetItem([
                        '%s (%d)' % (fname, len(file_evs)), '', ''])
                    file_node.setForeground(0, QColor(color))
                    for ev in file_evs:
                        line_lbl = fname
                        if ev.line:
                            line_lbl = '%s:%d' % (fname, ev.line)
                        child = QTreeWidgetItem([
                            line_lbl, ev.severity, ev.message])
                        child.setForeground(1, QColor(_sev_color(ev.severity)))
                        file_node.addChild(child)
                    cat_node.addChild(file_node)

            self.diag_tree.addTopLevelItem(cat_node)

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

    # ---- RUN ALL (Convert -> Report -> vcxproj) -------------------------
    def run_all(self):
        self._chain_after_convert = True
        self.run_convert()

    # ---- CONVERT --------------------------------------------------------
    def run_convert(self):
        java_dir = self.ed_java.text().strip()
        out_dir = self.ed_out.text().strip()
        if not os.path.isdir(java_dir):
            self.logln('ERROR: Java source dir not found: %s' % java_dir)
            self._chain_after_convert = False
            return
        if self.worker and self.worker.isRunning():
            self.logln('! conversion already running')
            self._chain_after_convert = False
            return
        os.makedirs(out_dir, exist_ok=True)
        self._reset_cards()
        self._set_stage('Convert', 'active')
        self._set_stage('Report', 'pending')
        self.report_tree.clear()
        self.diag_tree.clear()
        self._set_busy(True)
        self._open_log_file()

        cpp_std = self.combo_std.currentText()
        platform = self._selected_platform_id()
        sdk_dir = self.ed_sdk.text().strip()
        if sdk_dir and os.path.isdir(sdk_dir):
            self.logln('SDK: %s' % sdk_dir)
        self.worker = ConvertWorker(java_dir, out_dir, cpp_std=cpp_std, platform=platform)
        self.worker.line.connect(self.logln)
        self.worker.done.connect(self._convert_done)
        self.worker.start()

    def _convert_done(self, rc, stats):
        self._close_log_file()
        self._set_busy(False)
        if rc == 0:
            self._set_stage('Convert', 'done')
            self.m_parsed.setText(str(stats.get('parsed', 0)))
            self.m_headers.setText(str(stats.get('headers', 0)))
            self.m_sources.setText(str(stats.get('sources', 0)))
            ledger = stats.get('ledger')
            self._populate_diagnostics(ledger)
            self.logln('[Convert] done — %d total events.' % stats.get('events', 0))
            if ledger and len(ledger) > 0:
                self.tabs.setCurrentWidget(self.diag_tree)
            if self._chain_after_convert:
                self._chain_after_convert = False
                self.run_report()
                self.run_vcxproj()
        else:
            self._set_stage('Convert', 'fail')
            self._chain_after_convert = False
            self.logln('[Convert] FAILED (rc=%d)' % rc)

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

            self.stubs_tree.clear()

            # group by subsystem
            by_sys = {}
            for stub in stub_list:
                by_sys.setdefault(stub['subsystem'], []).append(stub)

            total_stubs = len(stub_list)
            total_methods = sum(s['method_count'] for s in stub_list)
            self.logln('  %d stubs, %d total stub methods across %d subsystems'
                       % (total_stubs, total_methods, len(by_sys)))

            subsystem_order = list(stubs_mod.SUBSYSTEMS.keys()) + ['unknown']
            for sid in subsystem_order:
                group = by_sys.get(sid)
                if not group:
                    continue

                info = stubs_mod.SUBSYSTEMS.get(sid)
                label = info['label'] if info else 'Unknown'
                sdk_hint = info['sdk_hint'] if info else ''
                desc = info['description'] if info else ''

                method_total = sum(s['method_count'] for s in group)
                sys_node = QTreeWidgetItem([
                    '%s (%d stubs, %d methods)' % (label, len(group), method_total),
                    '', sdk_hint])
                sys_node.setToolTip(0, desc)
                sys_node.setForeground(0, QColor(INFO))
                sys_node.setExpanded(True)

                for stub in sorted(group, key=lambda s: s['class_name']):
                    sdk_note = stub['sdk_hint']
                    matches = sdk_matches.get(stub['class_name'], [])
                    if matches:
                        headers = sorted(set(h for _, h, _ in matches))
                        sdk_note = 'SDK: %s' % ', '.join(headers[:3])

                    stub_node = QTreeWidgetItem([
                        stub['class_name'] + '.h',
                        str(stub['method_count']),
                        sdk_note])
                    stub_node.setForeground(0, QColor(_CAT_COLORS.get('STUBBED', MUTE)))

                    for method in stub['methods']:
                        sig = '%s %s(%s)' % (method['return'], method['name'], method['params'])
                        match_info = ''
                        for mname, mheader, mapi in matches:
                            if mname == method['name']:
                                match_info = '%s -> %s' % (mapi, mheader)
                                break
                        child = QTreeWidgetItem([sig, '', match_info])
                        if match_info:
                            child.setForeground(2, QColor(OK))
                        stub_node.addChild(child)

                    sys_node.addChild(stub_node)
                self.stubs_tree.addTopLevelItem(sys_node)

            self.tabs.setCurrentWidget(self.stubs_tree)
            self.logln('[Stubs] done.')
        except Exception as e:
            import traceback
            self.logln('ERROR: stubs scan failed: %s' % e)
            self.logln(traceback.format_exc())

    # ---- OPEN -----------------------------------------------------------
    def open_output(self):
        out = self.ed_out.text().strip()
        if os.path.isdir(out):
            os.startfile(out)
        else:
            self.logln('! output dir not found: %s' % out)


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
