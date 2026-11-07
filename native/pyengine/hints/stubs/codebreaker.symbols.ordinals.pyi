from typing import (
    Any,
    Dict,
    Tuple,
)


def _from_corpus(stem: str) -> Tuple[Dict[int, str], str]: ...


def _from_file(stem: str) -> Tuple[Dict[Any, Any], str]: ...


def _named_modules() -> Dict[str, str]: ...


def _stem(module: str) -> str: ...


def identifier(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def linker_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...

from typing import Dict


def _from_corpus(stem: str) -> <class 'Union'>[Tuple[Dict[Any, Any], str], Tuple[Dict[int, str], str]]: ...


def _from_file(stem: str) -> <class 'Union'>[Tuple[Dict[Any, Any], str], Tuple[Dict[int, str], str]]: ...


def _named_modules() -> Dict[str, str]: ...


def _stem(module: str) -> str: ...


def identifier(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def linker_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...
