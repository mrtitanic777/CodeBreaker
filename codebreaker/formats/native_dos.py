"""native_dos.py -- formats/dos.py answered by the C++ (native/formats/dos).

While CB_NATIVE_FORMATS(_DOS) is on, dos.py's readers ask formats.dll (native.native_first); this
module turns the answers back into dos.Program and dos.Device objects. A program whose unpacker
must run in the emulator (anything packed but EXEPACK undone statically) is read by the Python,
which runs it. A temporary shim (docs/NATIVE.md, step 4).
"""
import os

from codebreaker.formats import dos


def file_args(path, *rest):
    """(the file's bytes, (its name, *rest)) -- as dos.py's path-taking readers read a file."""
    with open(path, "rb") as f:
        return f.read(), (os.path.basename(path),) + rest


def lower_name_args(path, *rest):
    with open(path, "rb") as f:
        return f.read(), rest + (os.path.basename(path).lower(),)


def devices(answer, *_):
    return [dos.Device(*t) for t in answer]


def program(path, answer):
    kind, state = answer
    p = dos.Program(path, kind)
    state["devices"] = devices(state["devices"])
    p.__dict__.update(state)
    return p


def program_result(answer, data, path, *rest):
    return program(path, answer)


def program_args(p, *rest):
    """unpacked_file(p): the program's image, and what is read of it besides."""
    return p.image, (p.kind, p.name, p.relocations, p.cs, p.ss, p.sp, p.ip, p.overlay)


def data_segment_args(p, entry):
    return p.image, (p.base, p.ip, p.kind, p.relocations, entry)
