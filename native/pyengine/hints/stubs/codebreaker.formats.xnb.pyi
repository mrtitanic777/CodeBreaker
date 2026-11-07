from PIL.Image import Image
from typing import (
    Callable,
    Dict,
    Tuple,
    Union,
)


def _7bit(b: bytes, o: int) -> Tuple[int, int]: ...


def _7bit_bytes(value: int) -> bytes: ...


def _riff(fmt: bytes, samples: bytes) -> bytes: ...


def _string(b: bytes, o: int) -> Tuple[str, int]: ...


def convert(path: str, out_dir: str) -> Dict[str, <class 'Union'>]: ...


def read(path: str) -> Xnb: ...


def rename_readers(raw: bytes, rename: Callable) -> <class 'Union'>[bytes, None]: ...


def short(reader: str) -> str: ...


def texture_image(tex: Dict[str, <class 'Union'>], platform: str) -> Image: ...


def write_wav(sound: Dict[str, <class 'Union'>], out: str) -> None: ...


class Xnb:
    def __init__(self, raw: bytes) -> None: ...

from PIL.Image import Image
from typing import (
    Callable,
    Dict,
    Tuple,
    Union,
)


def _7bit(b: bytes, o: int) -> Tuple[int, int]: ...


def _7bit_bytes(value: int) -> bytes: ...


def _riff(fmt: bytes, samples: bytes) -> bytes: ...


def _string(b: bytes, o: int) -> Tuple[str, int]: ...


def convert(path: str, out_dir: str) -> Dict[str, <class 'Union'>]: ...


def read(path: str) -> Xnb: ...


def rename_readers(raw: bytes, rename: Callable) -> <class 'Union'>[bytes, None]: ...


def short(reader: str) -> str: ...


def texture_image(tex: Dict[str, <class 'Union'>], platform: str) -> Image: ...


def write_wav(sound: Dict[str, <class 'Union'>], out: str) -> None: ...


class Xnb:
    def __init__(self, raw: bytes) -> None: ...

from PIL.Image import Image
from typing import (
    Callable,
    Dict,
    List,
    Tuple,
    Union,
)


def _7bit(b: bytes, o: int) -> Tuple[int, int]: ...


def _7bit_bytes(value: int) -> bytes: ...


def _manifest(data: bytes) -> Tuple[List[str], int, int]: ...


def _string(b: bytes, o: int) -> Tuple[str, int]: ...


def _texture(data: bytes, o: int) -> Tuple[Dict[str, <class 'Union'>], int]: ...


def convert(path: str, out_dir: str) -> Dict[str, <class 'Union'>]: ...


def read(path: str) -> Xnb: ...


def rename_readers(raw: bytes, rename: Callable) -> <class 'Union'>[bytes, None]: ...


def short(reader: str) -> str: ...


def texture_image(tex: Dict[str, <class 'Union'>], platform: str) -> Image: ...


class Xnb:
    def __init__(self, raw: bytes) -> None: ...
