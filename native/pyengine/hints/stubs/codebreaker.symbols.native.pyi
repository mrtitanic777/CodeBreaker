from ctypes import CDLL


def _load() -> CDLL: ...


def ask(question: str, text: str) -> <class 'Union'>[Dict[str, <class 'Union'>], str, None]: ...


def dll_path() -> str: ...

from ctypes import CDLL


def _load() -> CDLL: ...


def ask(question: str, text: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def dll_path() -> str: ...
