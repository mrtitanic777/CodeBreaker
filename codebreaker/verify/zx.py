"""zx.py -- what is particular to reconstructing a ZX Spectrum program: its reader (formats/zx.py),
its writer (source/zx_writer.py) and its 16 KB ROM at $0000. The lane itself is verify/z80_flat.py's."""
from codebreaker.formats import zx
from codebreaker.source import zx_writer
from codebreaker.verify.z80_flat import Lane

LANE = Lane("zx", 2, zx.read, zx_writer, (0x0000, 0x4000))
KIND, REV, SAME = LANE.KIND, LANE.REV, LANE.SAME


# Module-level functions, not the lane's bound methods: judge is handed to worker processes (jobs/pool.py), which pickle
# it by name.
def profile(module_path=None):
    return LANE.profile(module_path)


def prepare(module_path, work):
    return LANE.prepare(module_path, work)


def build(m, module_path, work):
    return LANE.build(m, module_path, work)


def judge(context, item):
    return LANE.judge(context, item)
