import re
p = r"C:\Users\mrtit\Documents\GitHub\CodeBreaker\.claude\worktrees\agent-ae870db3b374ffca8\_stage"
s = open(p + r"\loadable_corpus_expected_tmp.py", encoding="utf-8").read()
s = s.replace("X.isolate_env(HERE)", "X.isolate_env(HERE, state=False)")
a = s.index("def save_case():")
b = s.index('K.add("save", save_case)')
new_save = '''def save_case():
    res = {"a": {"ok": True, "stage": None, "seconds": 0.25, "objects": 3, "device_id": 12, "device_id_ok": True, "identity": False, "resource_len": 9, "vxd_len": 4096},
           "b\\u00e9": {"ok": False, "stage": "exception", "log": "\\u00e9\\u4e2d", "trace": "line1\\nline2", "seconds": 3.0},
           "c": {"ok": False, "stage": "open", "log": "bad"}, "d": {"ok": False, "stage": None, "seconds": 1.0, "log": "L"},
           "e": {"ok": False, "stage": "exception", "log": "y", "trace": "t", "seconds": 0.0}, "f": {"ok": True, "stage": None, "seconds": 2.0, "objects": 1, "device_id": 1,
                                                                                                   "device_id_ok": False, "identity": True, "resource_len": 0, "vxd_len": 1}}
    C._save(res, "runs/save.json")
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        C._summary(res, 12.4)
        C._summary({}, 0.0)
        C._summary({"x": {"ok": True, "device_id_ok": True, "identity": True}}, 1.0)
    return [file_text("runs/save.json"), norm_text(buf.getvalue()), os.path.exists("runs/save.json.tmp")]


'''
s = s[:a] + new_save + s[b:]
s = s.replace('CORPUS = os.path.join(os.path.dirname(C.__file__), "corpus.py")', 'CORPUS = os.path.join(os.path.dirname(C.__file__), "loadable_corpus.py")')
s = s.replace("  consts                           _VXD_EXTS, _EXTRA_FILES, ROOT is the checkout root\n  is_le FILE                       _is_le_vxd(FILE)\n  one SPEC                         _one_module(name, v): the record (its seconds as 0)",
              "  consts                           ROOT is the checkout root\n  one SPEC                         _one(name, v, root): the entry (its seconds as 0)")
s = s.replace("(resume/redo/seeded_redo continue the file `fresh`/`seeded` wrote)", "(resume/redo/seeded_redo continue the file `fresh`/`seeded` wrote; `seeded` holds one entry of each kind)")
# main: the loadable's own args (no resume dirs of corpus)
open(p + r"\loadable\expected.py", "w", encoding="utf-8", newline="\n").write(s) if __import__("os").path.isdir(p + r"\loadable") else None
import os
os.makedirs(p + r"\loadable", exist_ok=True)
open(p + r"\loadable\expected.py", "w", encoding="utf-8", newline="\n").write(s)
print("ok", len(s))
