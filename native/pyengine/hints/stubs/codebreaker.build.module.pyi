from capstone import CsInsn
from codebreaker.build.asm16 import Function
from codebreaker.build.definitions import Export
from codebreaker.formats.ne import NEFile
from codebreaker.translate.l2 import (
    Made,
    _Texts,
)
from typing import (
    Any,
    Callable,
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _assembled16(
    b: Built,
    nef: NEFile,
    at: Dict[Tuple[int, int], str],
    segs: Dict[int, Dict[str, <class 'Union'>]],
    stems: Dict[str, str],
    public: Dict[str, Tuple[str, str]],
    folder: str,
    env: Dict[str, str],
    carried: Dict[Any, Any] = ...,
    forced: Set[str] = ...,
    elsewhere: <class 'Union'>[Callable, None] = ...
) -> Dict[str, Tuple[str, int]]: ...


def _assembled_one(b: Built, name: str, stems: Dict[str, str], stem: str) -> None: ...


def _build16(b: Built, path: str, m: Made, folder: str) -> str: ...


def _build32(b: Built, path: str, m: Made, folder: str) -> str: ...


def _call(path: str, kind: str, export: Export) -> Tuple[str, int]: ...


def _dos_stub(path: str, folder: str) -> str: ...


def _filler(folder: str, wasm: str, env: Dict[str, str], seg: int, stem: str, n: int) -> str: ...


def _frame_pieces(
    b: Built,
    f: Function,
    seg: int,
    stems: Dict[str, str],
    folder: str,
    wasm: str,
    env: Dict[str, str],
    assemble_piece: Callable,
    elsewhere: <class 'Union'>[Callable, None] = ...
) -> List[str]: ...


def _leaves_ds(ins: List[CsInsn]) -> bool: ...


def _link(tool: str, env: Dict[str, str], folder: str, lines: List[str]) -> Tuple[str, str]: ...


def _link_resolving(
    b: Built,
    tool: str,
    env: Dict[str, str],
    folder: str,
    lines: List[str],
    stand_in: Callable,
    compile_extra: Callable
) -> str: ...


def _near_pointer_tables(nef: NEFile, at: Dict[Tuple[int, int], str]) -> Set[Any]: ...


def _pack_holes(
    holes: List[Tuple[int, int, str]],
    size: Dict[str, int],
    near: Set[str],
    aside: Tuple[()] = ...,
    limit: <class 'Union'>[int, None] = ...
) -> <class 'Union'>[Tuple[List[List[<class 'Union'>]], List[str], List[Any]], Tuple[List[List[<class 'Union'>]], List[Any], List[Any]]]: ...


def _publics(folder: str, stems: Dict[str, str]) -> Dict[str, Tuple[str, str]]: ...


def _publics_code() -> str: ...


def _quote(symbol: str) -> str: ...


def _register_argument_calls(nef: NEFile, at: Dict[Tuple[int, int], str]) -> Set[Any]: ...


def _registers_c_cannot_carry(
    nef: NEFile,
    segs: Dict[int, Dict[str, <class 'Union'>]],
    at: Dict[Tuple[int, int], str],
    texts: _Texts
) -> Dict[str, str]: ...


def _resources(b: Built, path: str, folder: str) -> str: ...


def _result_in_ax(code: bytes, start: int, end: int) -> None: ...


def _sections(path: str) -> List[Tuple[int, bytes]]: ...


def _segment_number_fixups(nef: NEFile) -> bool: ...


def _source_of(fn: Callable) -> str: ...


def _wasm(
    wasm: str,
    env: Dict[str, str],
    folder: str,
    stem: str,
    text: str,
    flags: <class 'Union'>[Tuple[str, str], Tuple[str]] = ...
) -> Tuple[int, str]: ...


def _whole_file_image(original: str, image: str) -> None: ...


def _write_definitions(path: str, folder: str, declaration_at: Callable) -> None: ...


def build(
    module: str,
    outdir: <class 'Union'>[str, None] = ...,
    call: bool = ...,
    log: Callable = ...,
    gated: None = ...,
    _gate: bool = ...
) -> Built: ...


def check(b: Built) -> Built: ...


def kept_as_asm(module: str) -> Dict[Any, Any]: ...


def module_document(
    module: str,
    m: Made,
    assembled: Dict[str, str],
    stems: Dict[str, str],
    folder: str,
    lengths: <class 'Union'>[Dict[int, int], None] = ...
) -> Dict[str, <class 'Union'>]: ...


def module_document32(module: str, m: Made) -> Dict[str, <class 'Union'>]: ...


def name_of_stem(stem: str, cstem: Dict[str, str]) -> str: ...


def run(b: Built, limit: int = ...) -> Built: ...


class Built:
    def __init__(self, original: str) -> None: ...
    @property
    def linked(self) -> bool: ...
    def report(self) -> str: ...
