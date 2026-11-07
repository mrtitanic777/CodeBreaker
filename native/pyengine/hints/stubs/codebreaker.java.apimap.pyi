from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _bridge_equiv(reg: Registry) -> None: ...


def _leaf(target: str) -> str: ...


def _norm(name: str) -> List[str]: ...


def extract_symbols(text: str) -> Set[str]: ...


def extract_symbols_from_dir(java_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> Set[str]: ...


def load(force: bool = ...) -> Registry: ...


def render_coverage_md(
    names: <class 'Union'>[Set[str], List[str]],
    src_lang: str,
    dst_lang: str,
    dialect: <class 'Union'>[str, None] = ...,
    reg: None = ...,
    title: None = ...
) -> str: ...


class Mapping:
    def __init__(
        self,
        name: str,
        src_lang: str,
        dst_lang: str,
        target: str,
        kind: str = ...,
        note: str = ...,
        adapter: None = ...,
        since: <class 'Union'>[str, None] = ...,
        subsystem: str = ...,
        reverse: bool = ...,
        dialect: <class 'Union'>[str, None] = ...
    ) -> None: ...
    @property
    def supported(self) -> bool: ...


class Registry:
    def __init__(self) -> None: ...
    def _add_symbol(
        self,
        src_lang: str,
        name: str,
        entry: Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, Dict[str, str]], Dict[str, Dict[str, <class 'Union'>]]]],
        subsystem: str
    ) -> None: ...
    def _select_version(
        self,
        m: Dict[str, <class 'Union'>[str, <class 'Union'>]],
        dst_lang: str,
        dialect: <class 'Union'>[str, None]
    ) -> <class 'Union'>[Tuple[str, str, str, None, None, None], Tuple[str, str, str, None, str, str]]: ...
    def add_subsystem_doc(self, sub_id: str, language: str, description: str) -> None: ...
    def coverage(
        self,
        names: <class 'Union'>[Set[str], List[str]],
        src_lang: str,
        dst_lang: str,
        dialect: <class 'Union'>[str, None] = ...
    ) -> Dict[str, <class 'Union'>[List[Any], List[<class 'Union'>]]]: ...
    def dialect_order(self, lang: str) -> List[str]: ...
    def resolve(
        self,
        name: str,
        src_lang: str,
        dst_lang: str,
        dialect: <class 'Union'>[str, None] = ...
    ) -> <class 'Union'>[codebreaker.java.apimap.Mapping, None]: ...
    def reverse(self, target: str, dst_lang: str, src_lang: str) -> Mapping: ...
    def symbols_in(self, subsystem: str) -> List[Tuple[str, Dict[str, <class 'Union'>]]]: ...

from typing import (
    Dict,
    List,
    Optional,
    Set,
    Tuple,
    Union,
)


def _bridge_equiv(reg: Registry) -> None: ...


def _leaf(target: str) -> str: ...


def _norm(name: str) -> List[str]: ...


def extract_symbols(text: str) -> Set[str]: ...


def extract_symbols_from_dir(java_dir: <class 'Union'>[str, pathlib.WindowsPath]) -> Set[str]: ...


def load(force: bool = ...) -> Registry: ...


def render_coverage_md(
    names: <class 'Union'>[Set[str], List[str]],
    src_lang: str,
    dst_lang: str,
    dialect: <class 'Union'>[str, None] = ...,
    reg: None = ...,
    title: None = ...
) -> str: ...


class Mapping:
    def __init__(
        self,
        name: str,
        src_lang: str,
        dst_lang: str,
        target: str,
        kind: str = ...,
        note: str = ...,
        adapter: None = ...,
        since: <class 'Union'>[str, None] = ...,
        subsystem: str = ...,
        reverse: bool = ...,
        dialect: <class 'Union'>[str, None] = ...
    ) -> None: ...
    @property
    def supported(self) -> bool: ...


class Registry:
    def __init__(self) -> None: ...
    def _add_symbol(
        self,
        src_lang: str,
        name: str,
        entry: Dict[str, <class 'Union'>[<class 'Union'>, Dict[str, Dict[str, str]], Dict[str, Dict[str, <class 'Union'>]]]],
        subsystem: str
    ) -> None: ...
    def _select_version(
        self,
        m: Dict[str, <class 'Union'>[<class 'Union'>, str]],
        dst_lang: str,
        dialect: <class 'Union'>[str, None]
    ) -> <class 'Union'>[Tuple[str, str, str, None, None, None], Tuple[str, str, str, None, str, str]]: ...
    def add_subsystem_doc(self, sub_id: str, language: str, description: str) -> None: ...
    def coverage(
        self,
        names: <class 'Union'>[Set[str], List[str]],
        src_lang: str,
        dst_lang: str,
        dialect: <class 'Union'>[str, None] = ...
    ) -> Dict[str, <class 'Union'>[List[Any], List[<class 'Union'>]]]: ...
    def dialect_order(self, lang: str) -> List[str]: ...
    def resolve(
        self,
        name: str,
        src_lang: str,
        dst_lang: str,
        dialect: <class 'Union'>[str, None] = ...
    ) -> <class 'Union'>[codebreaker.java.apimap.Mapping, None]: ...
    def reverse(self, target: str, dst_lang: str, src_lang: str) -> Mapping: ...
    def symbols_in(self, subsystem: str) -> List[Tuple[str, Dict[str, <class 'Union'>]]]: ...
