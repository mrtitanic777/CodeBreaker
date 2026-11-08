from codebreaker.decompile.describe import Description
from codebreaker.translate.l2 import Made
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _code_segment_data(
    reports: Dict[str, None],
    where: Dict[str, Tuple[int, int]],
    d: Description,
    module_path: str
) -> Tuple[List[Any], Dict[Any, Any], Dict[Any, Any], Dict[Any, Any]]: ...


def _compilable16(module_path: str, outdir: <class 'Union'>[str, None] = ...) -> Made: ...


def _far_argument_widths(code: bytes, start: int, end: int, far_fn: bool) -> Dict[str, int]: ...


def _far_loads(code: bytes, start: int, end: int) -> Set[int]: ...


def _global_type(prefix: str, size: int, elem: None = ...) -> str: ...


def _keeps_frame(code: bytes, off: int) -> bool: ...


def _loads_dgroup(code: bytes, off: int, dgroup: int) -> bool: ...


def _more_helpers(have: str) -> str: ...


def _ram_type(prefix: str, size: int, called: bool) -> str: ...


def _selector_sites(module_path: str) -> Dict[Any, Any]: ...


def _shared_selector_structs(
    shared: Dict[str, Dict[str, Dict[int, str]]]
) -> <class 'Union'>[Tuple[List[Any], Dict[Any, Any]], Tuple[List[str], Dict[str, Dict[str, str]]]]: ...


def _source_stamp(*modules) -> str: ...


def _ss_store_places(code: bytes, start: int, end: int) -> List[<class 'Union'>[int, Any]]: ...


def _string_literal(chunk: bytes) -> <class 'Union'>[str, None]: ...


def _struct_decls(
    structs: List[<class 'Union'>[Any, List[<class 'Union'>]]],
    header: str,
    far16: bool = ...
) -> List[<class 'Union'>[Any, str]]: ...


def _struct_sizes(structs: List[Any]) -> Dict[Any, Any]: ...


def _write16(
    context: None,
    item: <class 'Union'>[Tuple[int, str, None, int, int, int, int, Dict[str, <class 'Union'>], bool, None, Dict[str, Dict[int, Tuple[str, str]]]], Tuple[int, str, str, int, int, int, int, Dict[str, <class 'Union'>], bool, None, Dict[str, Dict[int, Tuple[str, str]]]]]
) -> <class 'Union'>[Tuple[int, str, Set[Any], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]], Tuple[int, str, Set[str], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]]]: ...


def _write16_images(images: Dict[int, bytes]) -> None: ...


def call_counts(calls: Dict[str, Set[int]], names: Set[str]) -> Dict[str, int]: ...


def compilable16(module_path: str, outdir: <class 'Union'>[str, None] = ...) -> Made: ...


def compilable32(
    module_path: str,
    funcs: List[int],
    outdir: str
) -> <class 'Union'>[Tuple[str, Dict[str, str], List[str], Dict[int, str], List[str], Set[Any], Set[Any], Dict[str, <class 'Union'>], Dict[str, str]], Tuple[str, Dict[str, str], List[str], Dict[int, str], List[str], Set[Any], Set[Any], Dict[str, Dict[str, <class 'Union'>]], Dict[str, str]]]: ...


def generation() -> Tuple[str, Tuple[int, str, str]]: ...


def interrupt_helper_name(vector: int, regs: Tuple[str, Ellipsis]) -> str: ...


def interrupt_helpers(
    used: <class 'Union'>[Set[Tuple[int, Tuple[str, str]]], Set[<class 'Union'>]]
) -> List[<class 'Union'>[Any, str]]: ...


def interrupt_routines(vectors: Set[int]) -> Tuple[str, str]: ...


def module_facts16(
    code: bytes,
    off: int,
    end: int,
    distance: str,
    model: <class 'Union'>[str, None],
    dgroup: int,
    far_return: bool,
    seg: <class 'Union'>[int, None] = ...
) -> Dict[str, <class 'Union'>]: ...

from codebreaker.decompile.describe import Description
from codebreaker.translate.l2 import Made
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _code_segment_data(
    reports: Dict[str, None],
    where: Dict[str, Tuple[int, int]],
    d: Description,
    module_path: str
) -> Tuple[List[Any], Dict[Any, Any], Dict[Any, Any], Dict[Any, Any]]: ...


def _compilable16(module_path: str, outdir: <class 'Union'>[str, None] = ...) -> Made: ...


def _far_argument_widths(code: bytes, start: int, end: int, far_fn: bool) -> Dict[str, int]: ...


def _far_loads(code: bytes, start: int, end: int) -> Set[int]: ...


def _global_type(prefix: str, size: int, elem: None = ...) -> str: ...


def _keeps_frame(code: bytes, off: int) -> bool: ...


def _loads_dgroup(code: bytes, off: int, dgroup: int) -> bool: ...


def _more_helpers(have: str) -> str: ...


def _selector_sites(module_path: str) -> Dict[Any, Any]: ...


def _shared_selector_structs(
    shared: Dict[str, Dict[str, Dict[int, str]]]
) -> <class 'Union'>[Tuple[List[Any], Dict[Any, Any]], Tuple[List[str], Dict[str, Dict[str, str]]]]: ...


def _ss_store_places(code: bytes, start: int, end: int) -> List[<class 'Union'>[Any, int]]: ...


def _string_literal(chunk: bytes) -> <class 'Union'>[str, None]: ...


def _struct_decls(
    structs: List[<class 'Union'>[Any, List[<class 'Union'>]]],
    header: str,
    far16: bool = ...
) -> List[<class 'Union'>[Any, str]]: ...


def _struct_sizes(structs: List[Any]) -> Dict[Any, Any]: ...


def _write16(
    context: None,
    item: <class 'Union'>[Tuple[int, str, None, int, int, int, int, Dict[str, <class 'Union'>], bool, None, Dict[str, Dict[int, Tuple[str, str]]]], Tuple[int, str, str, int, int, int, int, Dict[str, <class 'Union'>], bool, None, Dict[str, Dict[int, Tuple[str, str]]]]]
) -> <class 'Union'>[Tuple[int, str, Set[Any], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]], Tuple[int, str, Set[str], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]]]: ...


def _write16_images(images: Dict[int, bytes]) -> None: ...


def call_counts(calls: Dict[str, Set[int]], names: Set[str]) -> Dict[str, int]: ...


def compilable32(
    module_path: str,
    funcs: List[int],
    outdir: str
) -> Tuple[str, Dict[str, str], List[str], Dict[int, str], List[str], Set[Any], Set[Any], Dict[str, <class 'Union'>], Dict[str, str]]: ...


def interrupt_helper_name(
    vector: int,
    regs: <class 'Union'>[Tuple[str, str], Tuple[str, str, str, str], Tuple[str], Tuple[str, str, str]]
) -> str: ...


def interrupt_helpers(used: Set[<class 'Union'>]) -> List[str]: ...


def module_facts16(
    code: bytes,
    off: int,
    end: int,
    distance: str,
    model: <class 'Union'>[str, None],
    dgroup: int,
    far_return: bool,
    seg: <class 'Union'>[int, None] = ...
) -> Dict[str, <class 'Union'>]: ...

from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _far_loads(code: bytes, start: int, end: int) -> Set[int]: ...


def _global_type(prefix: str, size: int, elem: None = ...) -> str: ...


def _loads_dgroup(code: bytes, off: int, dgroup: int) -> bool: ...


def _more_helpers(have: str) -> str: ...


def _ram_type(prefix: str, size: int, called: bool) -> str: ...


def _struct_decls(structs: List[Any], header: str, far16: bool = ...) -> List[Any]: ...


def _struct_sizes(structs: List[Any]) -> Dict[Any, Any]: ...


def compilable32(
    module_path: str,
    funcs: List[int],
    outdir: str
) -> Tuple[str, Dict[str, str], List[str], Dict[int, str], List[str], Set[Any], Set[Any], Dict[str, <class 'Union'>], Dict[str, str]]: ...


def interrupt_helper_name(vector: int, regs: Tuple[str, Ellipsis]) -> str: ...


def interrupt_helpers(
    used: <class 'Union'>[Set[Tuple[int, Tuple[str, str]]], Set[<class 'Union'>]]
) -> List[<class 'Union'>[Any, str]]: ...


def interrupt_routines(vectors: Set[int]) -> Tuple[str, str]: ...

from codebreaker.decompile.describe import Description
from codebreaker.translate.l2 import Made
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _code_bytes(image: bytes, starts: List[int]) -> bytearray: ...


def _code_segment_data(
    reports: Dict[str, <class 'Union'>[<class 'Union'>, None]],
    where: Dict[str, Tuple[int, int]],
    d: Description,
    module_path: str
) -> <class 'Union'>[Tuple[List[Any], Dict[Any, Any], Dict[int, Dict[str, <class 'Union'>]], Dict[Any, Any]], Tuple[List[Any], Dict[Any, Any], Dict[Any, Any], Dict[Any, Any]], Tuple[List[str], Dict[int, str], Dict[int, <class 'Union'>], Dict[str, Dict[str, Dict[int, str]]]], Tuple[List[str], Dict[int, str], Dict[int, Dict[str, <class 'Union'>]], Dict[str, Dict[str, Dict[int, str]]]]]: ...


def _compilable16(module_path: str, outdir: <class 'Union'>[str, None] = ...) -> Made: ...


def _data_runs(code: bytearray) -> List[Tuple[int, int]]: ...


def _far_argument_widths(code: bytes, start: int, end: int, far_fn: bool) -> Dict[str, int]: ...


def _far_loads(code: bytes, start: int, end: int) -> Set[int]: ...


def _global_type(prefix: str, size: int, elem: <class 'Union'>[int, None] = ...) -> str: ...


def _keeps_frame(code: bytes, off: int) -> bool: ...


def _loads_dgroup(code: bytes, off: int, dgroup: int) -> bool: ...


def _more_helpers(have: str) -> str: ...


def _rows(items: List[str], per: int) -> str: ...


def _selector_sites(module_path: str) -> Dict[int, List[Tuple[int, int]]]: ...


def _shared_selector_structs(
    shared: Dict[str, Dict[str, Dict[int, str]]]
) -> <class 'Union'>[Tuple[List[Any], Dict[Any, Any]], Tuple[List[str], Dict[Any, Any]]]: ...


def _source_stamp(*modules) -> str: ...


def _ss_store_places(code: bytes, start: int, end: int) -> List[<class 'Union'>[Any, int]]: ...


def _string_literal(chunk: bytes) -> <class 'Union'>[str, None]: ...


def _struct_decls(
    structs: List[<class 'Union'>[Any, List[<class 'Union'>]]],
    header: str,
    far16: bool = ...
) -> List[<class 'Union'>[Any, str]]: ...


def _write16(
    context: None,
    item: Any
) -> <class 'Union'>[Tuple[int, str, Set[str], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]], Tuple[int, str, Set[Any], Dict[str, <class 'Union'>], Dict[str, <class 'Union'>]]]: ...


def _write16_images(images: Dict[int, bytes]) -> None: ...


def compilable16(module_path: str, outdir: <class 'Union'>[str, None] = ...) -> Made: ...


def generation() -> Tuple[str, Tuple[int, str, str]]: ...


def interrupt_helper_name(vector: int, regs: Tuple[str, Ellipsis]) -> str: ...


def interrupt_helpers(used: Set[<class 'Union'>]) -> List[<class 'Union'>[Any, str]]: ...


def module_facts16(
    code: bytes,
    off: int,
    end: int,
    distance: <class 'Union'>[str, None],
    model: <class 'Union'>[str, None],
    dgroup: <class 'Union'>[int, None],
    far_return: bool,
    seg: <class 'Union'>[int, None] = ...
) -> Dict[str, <class 'Union'>]: ...
