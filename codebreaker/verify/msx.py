"""msx.py -- what is particular to reconstructing an MSX cartridge: its reader (formats/msx.py), its
writer (source/msx_writer.py) and the BIOS ROM at $0000. The lane itself is verify/z80_flat.py's."""
from codebreaker.formats import msx
from codebreaker.source import msx_writer
from codebreaker.verify.z80_flat import Lane

LANE = Lane("msx", 2, msx.read, msx_writer, (0x0000, 0x4000))
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
