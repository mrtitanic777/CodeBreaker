from codebreaker.analyze.image import Image
from typing import (
    Any,
    Dict,
    List,
    Tuple,
    Union,
)


def _ascii(data: bytes, start: int, least: int) -> None: ...


def _ask(
    img: Image,
    what: str
) -> <class 'Union'>[Dict[str, <class 'Union'>], List[List[<class 'Union'>]]]: ...


def _assemble(stores: Dict[Tuple[str, int], Tuple[int, int]], least: int = ...) -> List[Tuple[str, int, str, bool]]: ...


def _formats(img: Image) -> Tuple[List[Dict[str, <class 'Union'>]], int, Dict[Any, Any]]: ...


def _table(img: Image) -> Dict[int, Tuple[str, str, bool]]: ...


def _wide(data: bytes, start: int, least: int) -> None: ...


def at(img: Image, va: int, least: int = ...) -> None: ...


def conversions(fmt: str, wide: bool = ...) -> List[str]: ...


def formats(img: Image) -> Tuple[List[Dict[str, <class 'Union'>]], int, Dict[Any, Any]]: ...


def literal(img: Image, args: List[<class 'Union'>], k: int) -> None: ...


def stack_strings(img: Image) -> List[Any]: ...


def table(img: Image) -> Dict[int, Tuple[str, str, bool]]: ...

from codebreaker.analyze.image import Image
from typing import (
    Any,
    Dict,
    List,
    Tuple,
    Union,
)


def _ascii(data: bytes, start: int, least: int) -> <class 'Union'>[str, None]: ...


def _assemble(stores: Dict[Tuple[str, int], Tuple[int, int]], least: int = ...) -> List[Tuple[str, int, str, bool]]: ...


def _formats(
    img: Image
) -> <class 'Union'>[Tuple[List[Any], int, Dict[Any, Any]], Tuple[List[Dict[str, <class 'Union'>]], int, Dict[Any, Any]]]: ...


def _formatter(name: str) -> <class 'Union'>[Tuple[int, bool], None]: ...


def _table(img: Image) -> Dict[int, Tuple[str, str, bool]]: ...


def _uses(img: Image) -> Dict[int, List[Tuple[int, int, str]]]: ...


def _wide(data: bytes, start: int, least: int) -> None: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def at(img: Image, va: int, least: int = ...) -> <class 'Union'>[Tuple[str, str], None]: ...


def conversions(fmt: str, wide: bool = ...) -> List[str]: ...


def formats(
    img: Image
) -> <class 'Union'>[Tuple[List[Dict[str, <class 'Union'>]], int, Dict[Any, Any]], Tuple[List[Any], int, Dict[Any, Any]]]: ...


def literal(
    img: Image,
    args: List[<class 'Union'>],
    k: int
) -> <class 'Union'>[Tuple[int, str, str], None]: ...


def stack_strings(img: Image) -> List[Any]: ...


def table(img: Image) -> Dict[int, Tuple[str, str, bool]]: ...


def uses(img: Image) -> Dict[int, List[Tuple[int, int, str]]]: ...
