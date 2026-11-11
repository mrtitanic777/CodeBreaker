import sys, os, traceback
sys.path.insert(0, r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\_ne_common')
import vxdcommon as X
os.chdir(r'C:\Users\mrtit\Documents\GitHub\CodeBreaker\state\work\typed-port\source.vxd.loadable_corpus')
from codebreaker.source.vxd import loadable_corpus as C
try:
    C.run(os.path.abspath("inputs/mini"), "runs/dbg.json", redo=None, workers=1, root=X.MAIN)
except Exception:
    traceback.print_exc()
