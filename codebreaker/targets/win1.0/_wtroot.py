"""_wtroot — resolve the win1.0 DATA root.

The win1.0 tool code lives under `engine/targets/win1.0/`, but its DATA
(build16/ reconstructed/ refs/ files/) is external. `data_root(here)` finds that
DATA root robustly across every way a tool runs:
  * dispatched via wt.py        -> $WT_DATA_ROOT is set (the win1.0 project dir)
  * run with cwd = the data dir -> cwd holds build16/ (e.g. the ledger's runner)
  * tool dir holds the data     -> HERE itself holds build16/
  * run directly, data nearby   -> a `win1.0/` with build16/ in an ancestor dir
"""
import os

_MARKER = "build16"          # the data root is the dir that contains build16/


def data_root(here):
    r = os.environ.get("WT_DATA_ROOT")
    if r and os.path.isdir(r):
        return r
    if os.path.isdir(os.path.join(here, _MARKER)):        # tool dir holds the data
        return here
    cwd = os.getcwd()
    if os.path.isdir(os.path.join(cwd, _MARKER)):         # runner set cwd to data
        return cwd
    d = here                                              # else walk up for a win1.0/ data root
    for _ in range(6):
        d = os.path.dirname(d)
        cand = os.path.join(d, "win1.0")
        if os.path.isdir(os.path.join(cand, _MARKER)):
            return cand
    return here
