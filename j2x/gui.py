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
        QPlainTextEdit, QHBoxLayout, QVBoxLayout,
        QFrame, QSizePolicy, QTabWidget, QListWidget, QListWidgetItem,
    )
except ImportError:
    sys.stderr.write('PySide6 is required.  Install with:\n    pip install pyside6\n')
    sys.exit(1)

PROJECT_DIR = os.path.normpath(os.path.join(REPO_ROOT, '..'))
DEFAULT_JAVA = r'C:\Users\mrtit\Desktop\MinecraftAlpha\minecraft\src'
DEFAULT_OUT = os.path.join(REPO_ROOT, 'out')
DEFAULT_OVERRIDES = os.path.join(REPO_ROOT, 'overrides')
DEFAULT_SLN = os.path.join(PROJECT_DIR, 'MinecraftAlpha.sln')
DEFAULT_MSBUILD = r'C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe'

OK, FAIL, WARN, INFO, MUTE = '#4caf78', '#e24b4a', '#ef9f27', '#378add', '#9a9a93'

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
QTreeWidget, QPlainTextEdit, QListWidget {
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

    def __init__(self, java_dir, out_dir, overrides_dir):
        super().__init__()
        self.java_dir = java_dir
        self.out_dir = out_dir
        self.overrides_dir = overrides_dir

    def run(self):
        try:
            from . import pipeline
            self.line.emit('converting %s -> %s ...' % (self.java_dir, self.out_dir))
            s = pipeline.convert(
                self.java_dir, self.out_dir,
                overrides_dir=self.overrides_dir)
            self.line.emit('parsed=%d  headers=%d  sources=%d  overrides=%d'
                           % (s['parsed'], s['headers'], s['sources'], s['overrides']))
            self.line.emit('crash-class rule findings: %d' % s['rule_findings'])
            self.line.emit('ledger events=%d  (HIGH=%d)' % (s['events'], s['high']))
            self.done.emit(0, s)
        except Exception as e:
            self.line.emit('ERROR: %s: %s' % (type(e).__name__, e))
            self.done.emit(1, {})


class ConverterWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('j2x — Java → Xbox 360 Converter')
        self.resize(1100, 780)
        self.setStyleSheet(STYLE)
        self.settings = QSettings('j2x', 'ConverterGUI')
        self.worker = None
        self.proc = None
        self._chain_after_convert = False
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
        outer.addLayout(self._path_row('Java src', self.ed_java))
        outer.addLayout(self._path_row('Output', self.ed_out))

        # metric cards
        cards = QHBoxLayout()
        cards.setSpacing(10)
        f1, self.m_parsed = _card('Parsed', OK)
        f2, self.m_headers = _card('Headers', INFO)
        f3, self.m_sources = _card('Sources', INFO)
        f4, self.m_overrides = _card('Overrides', WARN)
        f5, self.m_rules = _card('Rule findings', FAIL)
        f6, self.m_high = _card('HIGH events', FAIL)
        for f in (f1, f2, f3, f4, f5, f6):
            cards.addWidget(f)
        outer.addLayout(cards)

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

        self.log = QPlainTextEdit()
        self.log.setReadOnly(True)
        self.log.setFont(QFont('Consolas', 9))

        self.report_tree = QTreeWidget()
        self.report_tree.setHeaderLabels(['Risk', 'File', 'Categories', 'Severities'])
        self.report_tree.setColumnWidth(0, 60)
        self.report_tree.setColumnWidth(1, 220)
        self.report_tree.setColumnWidth(2, 200)
        self.report_tree.setRootIsDecorated(False)

        self.err_list = QListWidget()
        self.err_list.itemDoubleClicked.connect(self._open_err_item)

        self.tabs.addTab(self.log, 'Log')
        self.tabs.addTab(self.report_tree, 'Report')
        self.tabs.addTab(self.err_list, 'Build errors')
        self.tabs.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        outer.addWidget(self.tabs, 1)

        # action buttons — every CLI command is here
        btns = QHBoxLayout()
        btns.setSpacing(8)
        self._action_buttons = {}
        for text, slot, tip in [
            ('Convert',       self.run_convert,    'Parse Java + emit C++03 (with overrides)'),
            ('Report',        self.run_report,     'Crash-class analysis on converted output'),
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
        super().closeEvent(e)

    def _set_stage(self, name, state):
        colors = {'pending': MUTE, 'active': '#e2e0d8', 'done': OK, 'fail': FAIL}
        if name in self.stages:
            self.stages[name].setStyleSheet('color:%s;' % colors.get(state, MUTE))

    def logln(self, text):
        self.log.appendPlainText(text)
        self.log.verticalScrollBar().setValue(self.log.verticalScrollBar().maximum())

    def _reset_cards(self):
        for w in (self.m_parsed, self.m_headers, self.m_sources,
                  self.m_overrides, self.m_rules, self.m_high):
            w.setText('-')

    def _set_busy(self, busy):
        for b in self._action_buttons.values():
            b.setEnabled(not busy)
        self.btn_run_all.setEnabled(not busy)

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
        self._set_busy(True)

        ov = DEFAULT_OVERRIDES if os.path.isdir(DEFAULT_OVERRIDES) else None
        self.worker = ConvertWorker(java_dir, out_dir, ov)
        self.worker.line.connect(self.logln)
        self.worker.done.connect(self._convert_done)
        self.worker.start()

    def _convert_done(self, rc, stats):
        self._set_busy(False)
        if rc == 0:
            self._set_stage('Convert', 'done')
            self.m_parsed.setText(str(stats.get('parsed', 0)))
            self.m_headers.setText(str(stats.get('headers', 0)))
            self.m_sources.setText(str(stats.get('sources', 0)))
            self.m_overrides.setText(str(stats.get('overrides', 0)))
            self.m_rules.setText(str(stats.get('rule_findings', 0)))
            self.m_high.setText(str(stats.get('high', 0)))
            self.logln('[Convert] done.')
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

            ov = DEFAULT_OVERRIDES if os.path.isdir(DEFAULT_OVERRIDES) else None
            ledger = Ledger()
            counts = analyze.analyze_all(
                ledger, out_dir,
                override_dir=ov,
                root=REPO_ROOT)

            meta = {
                'output_dir': os.path.abspath(out_dir),
                'build_log': counts.get('_build_log_path') or '(none)',
                'profile': 'alpha',
            }
            report_dir = report.write_reports(ledger, out_dir, meta=meta)

            sevs = ledger.totals_by_severity()
            self.logln('Report: markers=%d  rules=%d  lint=%d  overrides=%d'
                       % (counts['markers'], counts['rules'], counts['lint'],
                          counts['overrides']))
            self.logln('  events: %d  (HIGH=%d MED=%d LOW=%d)'
                       % (len(ledger), sevs.get(Severity.HIGH, 0),
                          sevs.get(Severity.MED, 0), sevs.get(Severity.LOW, 0)))

            # populate the report tab with the risk table
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

    # ---- OPEN -----------------------------------------------------------
    def open_output(self):
        out = self.ed_out.text().strip()
        if os.path.isdir(out):
            os.startfile(out)
        else:
            self.logln('! output dir not found: %s' % out)


def main():
    app = QApplication(sys.argv)
    app.setStyle('Fusion')
    w = ConverterWindow()
    w.show()
    sys.exit(app.exec())
