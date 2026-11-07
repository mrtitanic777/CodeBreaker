from typing import (
    List,
    Tuple,
)


def _arguments(body: str, shadow: str) -> str: ...


def _body(text: str) -> Tuple[str, str]: ...


def _comparison(body: str, shadow: str) -> str: ...


def _constant(body: str, shadow: str) -> str: ...


def _operator(body: str, shadow: str) -> str: ...


def _store(body: str, shadow: str) -> str: ...


def breaks(text: str) -> List[Tuple[str, str]]: ...
