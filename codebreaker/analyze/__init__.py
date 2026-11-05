"""What can be worked out ABOUT a module, beside decompiling it.

    image.py        a module as the analyses see it: its bytes at their addresses, its functions
                    decoded by following the code, its imports
    call_graph.py   who calls whom, references to data and functions, what exports reach
    conventions.py  how each function is called, its arguments, frame, result and shape
    globals.py      global variables and initialised tables of pointers
    strings.py      strings, strings built on the stack, format strings and their arguments
    structures.py   structures recovered from how pointers are used
    classes.py      virtual tables, run-time type information, MFC classes and message maps,
                    exception-handling tables
    dispatch.py     jump tables, compare chains, message and command dispatch, system calls
    resources.py    the resources code asks for
    observe.py      what running the functions shows: fields touched, indirect call targets,
                    functions that behave as strlen, memcpy, ...
    facts.py        the fact table of the function store
    report.py       python -m codebreaker analyze <module>
    native.py       the shim of cbanalyze.dll (native/analyze), where the image's decoding, the
                    call graph and the conventions are worked out in C++

Every analysis is `analyse(image) -> {"counts": {...}, "facts": [...], ...}`; a fact is a dict with
`addr` (None for a fact about the whole module), `kind` and whatever describes it.
"""
