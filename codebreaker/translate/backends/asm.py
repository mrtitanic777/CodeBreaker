"""asm.py -- L2 to assembly language: a Module whose parts stay machine code, as the file its assembler reads.

    from codebreaker.translate.backends import asm
    text = asm.module(doc)                  # the whole file
    text, first = asm.lines(doc)            # ... and the line each unit begins on (1-based), by unit

The back end of every lane whose output is assembly (docs/IR_UNIFICATION.md step 4): the whole-file
NASM writers (source/ne_writer.py, pe_writer.py, le_writer.py), the MASM writers (driver_writer.py,
dos_writer.py), the console and home-computer writers through source/image_writer.py, the 16-bit
port's assembly (port/asm16.py), and the head of a driver's source file (backends/module_file.py).
Each of those works out what to write -- which bytes are code, which instructions its assembler
gives back as they were, what each label is -- and builds the module as one L2 tree as it goes
(l2.Assembly): each part of the file a MachineFragment (a section, a segment, a function; its
origin, its name, the assembler it is written for). This writes the tree as the file:

    MachineFragment     its `asm`, as it is (a fragment `within` another has its text there)
    Comment             a comment line in the assembler's spelling (`; text`)

The parts are joined by a line break, nothing added and nothing taken away, so the file is the
text the writer laid out, whatever the assembler. A unit of any other kind is not assembly: it
is declined (Decline), and the lane that holds it writes it with the back end of its language
(backends/c.py for C).
"""


class Decline(Exception):
    """A node this back end does not write."""


COMMENT = {}                              # (syntax -> the comment's lead where it is not ';')


def comment(text, syntax=None):
    """One comment line: `;` alone for an empty one, else `; text`."""
    lead = COMMENT.get(syntax or "", ";")
    return lead + (" " + text if text else "")


def unit(u, syntax=None):
    """The text of one unit, or None for a fragment whose text another holds."""
    kind = u.get("node")
    if kind == "MachineFragment":
        if u.get("asm") is None:
            if u.get("within"):
                return None
            raise Decline("fragment %s has no text" % (u.get("name") or "?"))
        return u["asm"]
    if kind == "Comment":
        return comment(u.get("text") or "", syntax)
    raise Decline("document %s" % kind)


def lines(doc):
    """-> (the file, {unit index: the 1-based line it begins on})"""
    if doc.get("node") != "Module":
        raise Decline("document %s" % doc.get("node"))
    units = doc.get("units") or []
    parts, first, line = [], {}, 1
    for k, u in enumerate(units if isinstance(units, list) else [units]):
        text = unit(u, u.get("syntax"))
        if text is None:
            continue
        first[k] = line
        parts.append(text)
        line += text.count("\n") + 1
    return "\n".join(parts), first


def module(doc):
    """The whole file the Module stands for."""
    return lines(doc)[0]
