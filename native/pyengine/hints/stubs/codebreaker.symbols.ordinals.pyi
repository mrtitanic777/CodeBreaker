from typing import (
    Dict,
    Tuple,
)


def _coff_import(obj: bytes) -> Tuple[str, int, str]: ...


def _from_corpus(stem: str) -> <class 'Union'>[Tuple[Dict[Any, Any], str], Tuple[Dict[int, str], str]]: ...


def _from_file(stem: str) -> <class 'Union'>[Tuple[Dict[Any, Any], str], Tuple[Dict[int, str], str]]: ...


def _named_modules() -> Dict[str, str]: ...


def _stem(module: str) -> str: ...


def identifier(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def imports16(module_path: str) -> Dict[Tuple[str, int], str]: ...


def linker_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def read_definition_file(path: str) -> Dict[str, Dict[int, str]]: ...


def read_import_library(path: str) -> Dict[str, Dict[int, str]]: ...


def source(module: str, ordinal: int) -> str: ...


def table(module: str) -> Dict[int, str]: ...

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

from typing import (
    Dict,
    Tuple,
)


def _from_corpus(stem: str) -> Tuple[Dict[int, str], str]: ...


def _from_file(stem: str) -> Tuple[Dict[int, str], str]: ...


def _stem(module: str) -> str: ...


def identifier(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def linker_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...
