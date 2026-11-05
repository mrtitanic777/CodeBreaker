from codebreaker.formats.native_pe import PEFile
from typing import (
    Any,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _flat_thunk_stubs(
    module_path: str,
    pe: PEFile,
    sections: List[Tuple[int, bytes, str]]
) -> List[int]: ...


def _offsets(mask_text: str) -> List[<class 'Union'>[int, Any]]: ...


def _rich(data: bytes, end: int) -> List[Any]: ...


def _signatures() -> Dict[str, Dict[bytes, List[<class 'Union'>]]]: ...


def compiler(
    module_path: str,
    identified: <class 'Union'>[None, Dict[int, Tuple[str, str]]] = ...
) -> Dict[str, <class 'Union'>]: ...


def identify(module_path: str) -> Dict[int, Tuple[str, str]]: ...


def main(argv: List[str]) -> int: ...


def match(bits: int, code: bytes) -> None: ...
