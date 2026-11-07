from ctypes import CDLL


def _load() -> CDLL: ...


def ask(question: str, text: str) -> <class 'Union'>[Dict[str, <class 'Union'>], str, None]: ...


def dll_path() -> str: ...

from ctypes import CDLL


def _load() -> CDLL: ...


def ask(question: str, text: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def dll_path() -> str: ...

from ctypes import CDLL
from typing import (
    Dict,
    Union,
)


def _load() -> CDLL: ...


def ask(question: str, text: str) -> Dict[str, Dict[str, <class 'Union'>]]: ...


def dll_path() -> str: ...
