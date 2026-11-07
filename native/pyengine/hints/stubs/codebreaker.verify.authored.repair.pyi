from typing import (
    Callable,
    Dict,
    Iterator,
    Optional,
    Tuple,
    Union,
)


def _blank(text: str) -> str: ...


def _knobs(source: str, target_frame: int, draft_frame: <class 'Union'>[int, None]) -> Iterator[Tuple[str, str]]: ...


def _split(source: str) -> <class 'Union'>[Tuple[str, List[str], str], Tuple[str, List[Any], str]]: ...


def _tests(shadow: str, body: str) -> Iterator[Tuple[int, int, str]]: ...


def _toggle_register(line: str) -> <class 'Union'>[str, None]: ...


def repair(compile: Callable, source: str, original: Tuple[bytes, bytes], bits: int = ...) -> Dict[str, <class 'Union'>]: ...


def score(
    built: Tuple[bytes, bytes],
    original: Tuple[bytes, bytes],
    bits: int = ...,
    target: None = ...
) -> Tuple[Tuple[int, int], Tuple[str, int, int]]: ...
