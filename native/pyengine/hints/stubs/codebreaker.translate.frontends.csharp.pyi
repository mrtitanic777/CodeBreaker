from typing import (
    Any,
    Dict,
    List,
    Optional,
)


def _tmp() -> str: ...


def diagnostics(out_dir: str) -> List[<class 'Union'>[Dict[str, <class 'Union'>], Any]]: ...


def dump(
    files: List[str],
    out_dir: str,
    langversion: None = ...
) -> Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, <class 'Union'>]]]: ...


def errors(out_dir: str) -> List[Any]: ...


def load(out_dir: str) -> Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, <class 'Union'>]]]: ...


def run(files: List[str], out_dir: str, langversion: None = ...) -> int: ...


def tool() -> str: ...

from typing import (
    Any,
    Dict,
    List,
    Optional,
)


def _tmp() -> str: ...


def diagnostics(out_dir: str) -> List[<class 'Union'>[Any, Dict[str, <class 'Union'>]]]: ...


def dump(
    files: List[str],
    out_dir: str,
    langversion: None = ...
) -> Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, <class 'Union'>]]]: ...


def errors(out_dir: str) -> List[Any]: ...


def load(out_dir: str) -> Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, <class 'Union'>]]]: ...


def run(files: List[str], out_dir: str, langversion: None = ...) -> int: ...


def tool() -> str: ...
