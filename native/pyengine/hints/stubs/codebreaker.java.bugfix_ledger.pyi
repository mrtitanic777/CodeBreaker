from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def _clean_comment(line: str) -> str: ...


def _scan_file(path: str) -> List[<class 'Union'>[Dict[str, <class 'Union'>], Any]]: ...


def collect(repo_root: str) -> List[Dict[str, <class 'Union'>]]: ...


def render(entries: List[Dict[str, <class 'Union'>]]) -> str: ...


def run(repo_root: None = ..., out_path: None = ...) -> List[Dict[str, <class 'Union'>]]: ...
