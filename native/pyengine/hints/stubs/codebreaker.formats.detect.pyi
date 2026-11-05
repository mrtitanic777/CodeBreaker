from typing import (
    Callable,
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


def _match(**kw) -> Dict[str, str]: ...


def dos_header(data: bytes) -> Dict[str, <class 'Union'>]: ...


def flat(kind: str) -> bool: ...


def format_of(path: str) -> <class 'Union'>[str, None]: ...


def holds_drivers(path: str, bundles: Tuple[bytes] = ...) -> bool: ...


def identify(path: str) -> List[<class 'Union'>[Any, Dict[str, str]]]: ...


def kind(path: str) -> <class 'Union'>[str, None]: ...


def lane(path: str) -> <class 'Union'>[str, None]: ...


def signature(path: str) -> <class 'Union'>[bytes, None]: ...


def signature_of_bytes(head: bytes, read_at: <class 'Union'>[Callable, None] = ...) -> <class 'Union'>[bytes, None]: ...
