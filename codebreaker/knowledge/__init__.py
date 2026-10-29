"""knowledge -- one knowledge base: what CodeBreaker knows about each platform, kept in one place.

    from codebreaker import knowledge as kb
    kb.prototype("win16", "KERNEL.GLOBALALLOC")     how the headers declare it: {returns, convention, parameters, header}
    kb.declaration("win16", "LZRead")               "int WINAPI LZRead(HFILE, void FAR *, int)"
    kb.spelling("GETDLGITEM")                       "GetDlgItem"
    kb.argument_bytes("win16", "USER.@171")         bytes of arguments the export removes (its code says)
    kb.argument_bytes("win32", "FindWindowA", "USER32")
    kb.import_name("USER", 91)                      "GetDlgItem": a numbered import's name, as a C name
    kb.name(body_id)                                (name, where it is from) of a function's body, in any module
    kb.name_body(body_id, "Name")                   ... given once, seen in every module with that body
    kb.signature_matches("win32", code)             (name, library) of the runtime-library function `code` is
    kb.struct("win32", "RECT")                      a structure's layout (symbols/sdk_structs.Struct)
    kb.api_model("win16", "GLOBALLOCK")             what the emulator's stand-in for it does
    kb.api_model("vxd", "Get_Cur_VM_Handle")
    kb.known(platform, kind, key) / kb.learn(platform, kind, key, value, source)   anything else

    python -m codebreaker knowledge                       every platform's kinds: how many, how big, from where
    python -m codebreaker knowledge <platform> <kind> [key]

The base is one SQLite file keyed by platform (knowledge/store.py: state/db/knowledge.sqlite).
Its platforms:

    win16     16-bit Windows: prototypes (the 16-bit headers), argument bytes read from exports'
              code, runtime-library signatures, the emulator's stand-ins, KERNEL's numbers
    win32     32-bit Windows: prototypes, argument bytes (the headers' table, and exports' code),
              runtime-library signatures, structure layouts, the stand-ins' argument counts
    windows   both: numbered exports' names from their makers' definition files
    vxd       ring-0 drivers: devices, their services in order, control messages, stand-ins
    dos       DOS and BIOS interrupts
    xbox360   the console's system libraries' numbered exports, the XDK's library signatures
    (bodies)  the name store: a body's digest and the names given to it, whatever its platform

Where each kind comes from, and how a change reaches it, is knowledge/sources.py (the files
people write: kept where they are, loaded again when they change) and the module that makes
it (symbols/prototypes.py from the headers, symbols/api_arguments.py from an export's code,
symbols/names.py the bodies). Nothing of it is kept anywhere else: the state folder's
prototypes.json, api16.*.json and xdk/ pickles, and the corpus's apiarity/ folder are gone
(an apiarity file still there is read into the base once).

Platform names may be given as the lanes say them: ne, pe, le, mz, powerpc, 16, 32.
"""
import re

from codebreaker.knowledge import store

PLATFORMS = {"win16": "16-bit Windows", "win32": "32-bit Windows", "windows": "16- and 32-bit Windows",
             "vxd": "ring-0 drivers", "dos": "DOS", "xbox360": "Xbox 360"}
_ALIASES = {"ne": "win16", "pe": "win32", "le": "vxd", "mz": "dos", "powerpc": "xbox360", "16": "win16",
            "32": "win32", 16: "win16", 32: "win32"}


def platform(name):
    """The base's name for a platform, from the lane's (ne -> win16, pe -> win32, ...)."""
    return _ALIASES.get(name, name)


def open(path=None):                                    # noqa: A001
    return store.open(path)


# ---- anything ------------------------------------------------------------------------------------
def known(platform_, kind, key, default=None):
    return store.open().get(platform(platform_), kind, key, default)


def learn(platform_, kind, key, value, source=""):
    """Keep something found: written once, seen by every later ask in any process."""
    store.open().put(platform(platform_), kind, key, value, source, store.LEARNED)


def table(platform_, kind):
    """{key: value} of a kind (an authored kind is loaded from its file first when the file changed)."""
    from codebreaker.knowledge import sources
    p = platform(platform_)
    if (p, kind) in sources.AUTHORED:
        return sources.authored(p, kind)
    return store.open().table(p, kind)


# ---- prototypes ----------------------------------------------------------------------------------
def _bare(name):
    """`KERNEL.GLOBALALLOC`, `USER32!FindWindowA` -> the function's own name; anything else as it is."""
    m = re.fullmatch(r"\w+[.!](\w+)", name or "")
    return m.group(1) if m else name


def prototype(platform_, name):
    """How the headers declare a Windows function, or None. Capitals do not matter."""
    from codebreaker.symbols import prototypes
    return prototypes.of(_bare(name), bits=16 if platform(platform_) == "win16" else 32)


def declaration(platform_, name):
    from codebreaker.symbols import prototypes
    return prototypes.declaration(_bare(name), 16 if platform(platform_) == "win16" else 32)


def spelling(name):
    from codebreaker.symbols import prototypes
    return prototypes.spelling(name)


def argument_bytes(platform_, api, module=None):
    """Bytes of arguments an imported function removes, or None when unknown. win16: `api` as
    the emulator reports it ('USER.@171', 'USER_171'). win32: `api` the name (or '#N') and
    `module` the DLL exporting it -- the headers' size first, else what its code does."""
    from codebreaker.symbols import api_arguments
    if platform(platform_) == "win16":
        return api_arguments.of(api)
    return api_arguments.stack_bytes(module, api)


def declared_argument_bytes(name):
    """Bytes of arguments a 32-bit function takes as the headers declare it, or None."""
    from codebreaker.symbols import api_arguments
    return api_arguments.header_bytes(name)


def import_name(module, ordinal):
    """A numbered import's name as a C identifier, or None (unknown, or a C++ name)."""
    from codebreaker.symbols import ordinals
    return ordinals.identifier(module, ordinal)


# ---- names of bodies -----------------------------------------------------------------------------
def name(body_id, kb=None):
    """(name, where it is from) of a function's body (symbols/names.py digests it), or None."""
    return (kb or store.open()).name(body_id)


def name_body(body_id, name_, source="named by hand", kb=None):
    (kb or store.open()).name_body(body_id, name_, source)


# ---- signatures, structures, behaviour -----------------------------------------------------------
def signatures(platform_):
    """[(leading bytes, size, mask, digest, name, library)] of the runtime-library functions."""
    return list(table(platform_, "signature").values())


def signature_matches(platform_, code):
    """(name, library) of the runtime-library function `code` starts with, or None."""
    from codebreaker.symbols import runtime_library
    return runtime_library.match(16 if platform(platform_) == "win16" else 32, code)


def struct(platform_, name_):
    """The layout of a structure the headers declare (sdk_structs.Struct), or None."""
    from codebreaker.symbols import sdk_structs
    return sdk_structs.layout(name_) if platform(platform_) == "win32" else None


def api_model(platform_, api):
    """What the emulator's stand-in for a system function does ({ax, dx, pop} for win16, with
    "arg<N>" and "heap" as knowledge/models/win16.json says; {argc} for win32; {answers_in} for a
    ring-0 service), or None when it is not modelled."""
    model = table(platform_, "api_model").get(api)
    return {k: v for k, v in model.items() if not k.startswith("_")} if model else None
