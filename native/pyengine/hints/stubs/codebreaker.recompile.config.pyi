from codebreaker.formats.powerpc_image import Image
from typing import (
    Dict,
    List,
    Optional,
    Tuple,
)


def _end_of_run(code: bytes, lo: int, a: int) -> int: ...


def _start_before(code: bytes, lo: int, a: int) -> int: ...


def extra_functions(
    xex_path: str,
    img: Image,
    switches: <class 'Union'>[Dict[int, List[int]], None] = ...
) -> List[Tuple[int, int]]: ...


def helpers(img: Image) -> Dict[str, int]: ...


def jump_buffers(img: Image, code: bytes) -> Dict[str, int]: ...

def _start_before(code: bytes, lo: int, a: int) -> int: ...
