from codebreaker.analyze.image import Image
from typing import (
    Any,
    Dict,
    Iterator,
    List,
    Tuple,
    Union,
)


def _aligned_pointers(img: Image) -> Iterator[Tuple[int, int]]: ...


def _cxx_tables(img: Image) -> List[Any]: ...


def _decorated(name: str) -> str: ...


def _exception_tables(img: Image) -> Dict[str, List[Any]]: ...


def _hierarchy(img: Image) -> List[Any]: ...


def _message_maps(img: Image) -> List[Any]: ...


def _returned(img: Image, va: int) -> None: ...


def _returns_address(img: Image) -> Dict[Any, Any]: ...


def _runtime_classes(img: Image) -> Dict[Any, Any]: ...


def _vtables(img: Image) -> List[Any]: ...


def analyse(img: Image) -> Dict[str, <class 'Union'>]: ...


def exception_tables(img: Image) -> Dict[str, List[Any]]: ...


def hierarchy(img: Image) -> List[Any]: ...


def message_maps(img: Image) -> List[Any]: ...


def runtime_classes(img: Image) -> Dict[Any, Any]: ...


def vtables(img: Image) -> List[Any]: ...
