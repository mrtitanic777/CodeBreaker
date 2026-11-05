"""c.py -- the hub's C back end: an L2 function document as C (docs/IR_UNIFICATION.md, step 2).

The decompiler's PrintL2 writes each function as L2 (`/*@L2 {...} */`, CB_L2=1). This writes it
back as C in the decompiler's own dialect -- its type names (int2, uint4, xunknown2, code), its
calling-convention words (__cdecl16far, __stdcall), hexadecimal past 9 -- so what it writes goes
through decompile/c_text.py's passes as the decompiler's C does today, and is compiled and judged
the same way. tools/l2c.py holds it to that C, character for character, over the golden corpus.

The writer is C++ (native/l2: l2c in cbl2.dll, docs/NATIVE.md step 5): the layout (PrintC's
pretty printer), the writer, every c_text pass done on the tree and what the module decides. Each
entry point below is the DLL's, through translate/native_l2.py; the Python writer it was ported
from line by line is gone (the milestone of 2026-11-04). Without cbl2.dll nothing here works:
native_l2.why_not() says why.

    function(doc, moved=()) -> C text of one function          type_text(t, name='') -> a declaration
    module(doc) -> (C text, {name: assembly}) of a Module: its functions, and what stayed assembly

`moved` names the passes of decompile/c_text.py done here instead, on the tree (step 3): each is
written where the address spaces and types L2 carries say so, and c_text leaves its own out for C
written from L2. MOVED is every one there is. What they add is laid out as taking no room: c_text
adds the same to the printed C, after PrintC broke its lines.
"""

from codebreaker.translate import native_l2 as _native_l2

# c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order)
MOVED = ("far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers",
         "null_is_zero", "global_pointer_steps", "one_name_for_globals", "far_result",
         "model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
         "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
         "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields", "far_reads",
         "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots", "selector_imports",
         "segment_values", "code_segment_reads", "names_used", "heading_facts", "int_result", "register_arguments",
         "code_pointers", "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast",
         "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result")
# ... those of them that need what the module's facts say of the function (its model in the
# description, what its instructions read and start with): done only when `function` is given
# them, as c_text gives them when it makes the module (c_text._write16); the C the decompiler is
# run for at first (decompiler.c_from_l2) is written without them
IN_MODULE = ("model_word", "joined_arguments", "far_arguments", "void_result", "byte_result", "export", "loadds",
             "selector_structs", "struct_gaps", "own_segment_calls", "open_arguments", "interrupts", "far_addresses",
             "far_casts", "entry_sp", "stack_arguments", "far_locals", "far_concat_casts", "buffer_fields",
             "far_reads", "near_to_far", "stack_segment_stores", "callee_pops", "caller_frame_slots",
             "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
             "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame",
             "globals_through", "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address",
             "homed_arguments", "uint8_result", "open_arguments")
# ... those of them the 32-bit path has (compilable32)
FLAT = ("pieces", "null_is_zero", "struct_gaps", "int_result", "register_arguments", "code_pointers",
        "compare_as_numbers", "keep_frame", "globals_through", "global_widths", "same_cast", "fit_calls",
        "struct_fields")
# ... and those of the PowerPC path (c_text.compilable_powerpc; the facts' "lane": "powerpc")
POWERPC = ("wide_address", "struct_gaps", "pieces", "compare_as_numbers", "code_pointers",
           "homed_arguments", "uint8_result", "open_arguments")


class Decline(Exception):
    """A node this back end does not write."""


# (bytes of the decompiler's scalar type names)
SIZEOF = {"int1": 1, "uint1": 1, "xunknown1": 1, "char": 1, "bool": 1, "undefined": 1, "undefined1": 1,
          "int2": 2, "uint2": 2, "xunknown2": 2, "undefined2": 2,
          "int4": 4, "uint4": 4, "xunknown4": 4, "undefined4": 4, "float4": 4,
          "int8": 8, "uint8": 8, "xunknown8": 8, "undefined8": 8, "float8": 8, "float10": 10}


def _param_name(p):
    return (p.get('sym') or {}).get('name') or p.get('name', '')


# ---------------------------------------------------------------------------------------------- the entry points
# (each the DLL's: the same names, arguments, text, facts and exceptions the Python writer had)
_E = _native_l2._ENTRIES
function = _E["function"]                 # (doc, moved=(), far=None, facts=None) -> C text of one function
module = _E["module"]                     # (doc, moved=MOVED) -> (C text, {name: assembly})
fragment = _E["fragment"]                 # (node) -> a MachineFragment as the C of its module holds it
type_text = _E["type_text"]               # (t, name='', far=False) -> a declaration, or the type alone
expr = _E["expr"]                         # (e) -> C text of an expression
condition = _E["condition"]               # (e, inner=False) -> a branch's condition, bracketed as PrintC does
flags = _E["flags"]                       # (doc, facts) -> the function's flags in the module
signature = _E["signature"]               # (doc) -> `int2 __cdecl16far sub_1_0000(void)`
takes32 = _E["takes32"]                   # (signature) -> arguments a 32-bit definition takes, None for __cdecl
module_names32 = _E["module_names32"]     # (doc) -> {'names', 'calls', 'called_through'} of a 32-bit function
type_names = _E["type_names"]             # (doc) -> the decompiler's own type names a function's tree uses
parameter_names = _E["parameter_names"]   # (doc) -> its parameters' names as its heading writes them
struct_accesses = _E["struct_accesses"]   # (doc, param) -> [(base type, stars, offset)] through `param`
prototypes = _E["prototypes"]             # (headings, order) -> {name: a declaration with its parameters}
segment_plan = _E["segment_plan"]         # (functions, near, where, sites, code, data_segment, other_data, first_selector)
caller_frame_plan = _E["caller_frame_plan"]   # (reports, frames) -> the helpers that work in their callers' frames
seg_layout = _E["seg_layout"]             # (fields) -> a selector's structure as a declaration's body, None on overlap
arguments_at = _E["arguments_at"]         # (text, open_paren) -> ([argument text], index after `)`)
words = _E["words"]                       # (ty) -> the words a value of C type `ty` takes on the stack, or None
parameters = _E["parameters"]             # (text) -> (heading, [(name, type)]) of an older-style definition, or None
argument_words = _E["argument_words"]     # (arg, types) -> the words an argument takes on the stack, or None
del _E
