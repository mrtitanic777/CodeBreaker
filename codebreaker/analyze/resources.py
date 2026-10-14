"""resources.py -- the resources code asks for.

    from codebreaker.analyze import image, resources
    loads, controls = resources.uses(image.load("comctl32.dll"))
        loads      [{function, site, call, type, id, present}]
        controls   [{function, site, call, control}]

A module's resources are data; which function loads which one is in the code. Every call to a
function that takes a resource identifier (LoadString, LoadIcon, DialogBoxParam, FindResource,
CreateDialog, LoadMenu, LoadAccelerators, ...) is read for that argument: a small integer is an
identifier, an address is a resource name. Each is then looked up in the module's own resource
table -- `present` says whether the module really carries that dialog, that string, that icon --
so a reference to another module's resources shows as absent rather than being silently wrong.

Calls that name a dialog control (GetDlgItem, SetDlgItemText, CheckDlgButton, ...) are listed
by control identifier; matching those against the dialog templates needs the templates parsed,
which is the resource extractor's work, not this module's.

Where the identifier sits among the pushes differs by convention. 32-bit calls push the last
argument first, so argument k is the k-th push back from the call. 16-bit Pascal calls push the
first argument first and a far pointer as two words, segment then offset: the tables below give,
for each function, how many words back from the call the identifier's word is (and its segment
word, which is 0 for an integer identifier).

What a module carries is read by the format readers (formats/pe.py resource_tree, formats/ne.py
resources); strings are held in blocks of sixteen, so string N is present when block N/16+1 is.
"""
from codebreaker.analyze import call_graph, image, strings
from codebreaker.formats.ne import NEFile
from codebreaker.formats.pe import open_pe

_TYPES = {1: "CURSOR", 2: "BITMAP", 3: "ICON", 4: "MENU", 5: "DIALOG", 6: "STRING", 9: "ACCELERATOR", 10: "RCDATA",
          12: "GROUP_CURSOR", 14: "GROUP_ICON", 16: "VERSION"}
# function -> (resource type, 32-bit: argument number; 16-bit: words back to the identifier, to its segment)
_LOADERS = {"loadstring": ("STRING", 1, 3, None), "loadicon": ("GROUP_ICON", 1, 0, 1),
            "loadcursor": ("GROUP_CURSOR", 1, 0, 1), "loadbitmap": ("BITMAP", 1, 0, 1), "loadmenu": ("MENU", 1, 0, 1),
            "loadaccelerators": ("ACCELERATOR", 1, 0, 1), "dialogbox": ("DIALOG", 1, 3, 4),
            "createdialog": ("DIALOG", 1, 3, 4), "dialogboxparam": ("DIALOG", 1, 5, 6),
            "createdialogparam": ("DIALOG", 1, 5, 6), "findresource": (None, 1, 2, 3), "loadimage": (None, 1, None, None)}
# function -> (32-bit: argument number of the control identifier; 16-bit: words back to it)
_CONTROLS = {"getdlgitem": (1, 0), "setdlgitemtext": (1, 2), "getdlgitemtext": (1, 3), "setdlgitemint": (1, 2),
             "getdlgitemint": (1, 3), "senddlgitemmessage": (1, 4), "checkdlgbutton": (1, 1), "isdlgbuttonchecked": (1, 0)}


def carried(img):
    """{type name: set of identifiers and (upper-case) names} the module itself holds."""
    out = {}
    if img.kind == "pe":
        for t in getattr(open_pe(img.path), "resource_tree", lambda: [])():
            name = _TYPES.get(t["id"], t["type"] if t["id"] is None else "#%s" % t["id"])
            for item in t["items"]:
                out.setdefault(str(name), set()).add(item["id"] if item["id"] is not None else item["name"].upper())
    elif img.kind == "ne":
        for r in NEFile(img.path).resources():
            ident = r["id"]
            out.setdefault(str(r["type"]), set()).add(int(ident[1:]) if ident.startswith("#") and ident[1:].isdigit()
                                                      else ident.upper())
    return out


def _stem(api):
    low = api.split("!")[-1].lower()
    return low[:-1] if low[-1:] in "aw" and (low[:-1] in _LOADERS or low[:-1] in _CONTROLS) else low


def _identifier(img, args, at, seg, text):
    """The resource identifier among a call's pushes: an integer, or a name read from the
    module's strings. None when it is not a constant in sight."""
    if at is None or at >= len(args) or args[at][0] != "i":
        return None
    value = args[at][1]
    if img.bits == 32:
        if value < 0x10000:
            return value
        return text[value][0].upper() if value in text else None
    if seg is None:
        return value
    if seg >= len(args):
        return None
    s = args[seg]
    if s[0] == "i" and s[1] == 0:
        return value
    where = img.data_segment + value if s[0] == "r" and s[1] == "ds" else None
    return text[where][0].upper() if where in text else None


def _uses(img):
    graph = call_graph.build(img)
    have, text = carried(img), strings.table(img)
    loads, controls = [], []
    for f in img.functions:
        fn = img.function(f)
        for site, _kind, dest in graph.calls[f]:
            if dest not in img.imports:
                continue
            stem = _stem(img.imports[dest])
            if stem not in _LOADERS and stem not in _CONTROLS:
                continue
            args = image.arguments(fn, fn.at(site), 8)
            if stem in _CONTROLS:
                ident = _identifier(img, args, _CONTROLS[stem][0 if img.bits == 32 else 1], None, {})
                if isinstance(ident, int) and ident < 0x10000:
                    controls.append({"function": f, "site": site, "call": stem, "control": ident})
                continue
            kind, pos32, pos16, seg16 = _LOADERS[stem]
            ident = _identifier(img, args, pos32 if img.bits == 32 else pos16, seg16, text)
            if ident is None:
                continue
            if stem == "findresource":                  # the type is an argument too
                t = _identifier(img, args, 2 if img.bits == 32 else 0, 1, text)
                kind = _TYPES.get(t, "#%d" % t) if isinstance(t, int) else t
            pool = have.get(kind or "", set())
            if kind == "STRING" and isinstance(ident, int):
                present = (ident // 16 + 1) in pool
            elif kind in ("GROUP_ICON", "GROUP_CURSOR") and isinstance(ident, int) and ident >= 32000:
                present = None                          # one of the system's stock icons and cursors
            else:
                present = ident in pool
            loads.append({"function": f, "site": site, "call": stem, "type": kind, "id": ident, "present": present})
    return loads, controls


def uses(img):
    return img.memo("resources", _uses)


def analyse(img):
    found, controls = uses(img)
    have = carried(img)
    facts = [dict(u, addr=u["function"], kind="resource_use") for u in found]
    facts += [dict(c, addr=c["function"], kind="dialog_control_use") for c in controls]
    facts.append({"addr": None, "kind": "resources_carried", "types": {t: len(v) for t, v in sorted(have.items())}})
    referenced = {}
    for u in found:
        if u["present"]:
            referenced.setdefault(u["type"], set()).add(u["id"])
    counts = {"resource_types": len(have), "resources_carried": sum(len(v) for v in have.values()),
              "loads_read": len(found), "present_in_module": sum(1 for u in found if u["present"]),
              "absent_from_module": sum(1 for u in found if u["present"] is False),
              "system_stock": sum(1 for u in found if u["present"] is None),
              "distinct_resources_referenced": sum(len(v) for v in referenced.values()),
              "functions_loading_resources": len({u["function"] for u in found}),
              "dialog_control_uses": len(controls), "distinct_controls": len({c["control"] for c in controls})}
    by = {}
    for u in found:
        by.setdefault(u["type"] or "?", set()).add(u["id"])
    text = ["%s: %s" % (t, ", ".join(str(x) for x in sorted(v, key=str)[:14])) for t, v in sorted(by.items())]
    return {"counts": counts, "facts": facts, "text": text}
