from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def _valid_root(spec: Dict[str, <class 'Union'>], root: str) -> <class 'Union'>[None, List[str]]: ...


def detect(
    platform: str,
    override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None]: ...


def require(platform: str, override: <class 'Union'>[str, None] = ...) -> SdkInfo: ...


def requires_sdk(platform: <class 'Union'>[str, None]) -> bool: ...


class SdkInfo:
    def __init__(
        self,
        platform: str,
        name: str,
        root: str,
        include_dirs: List[<class 'Union'>[Any, str]],
        source: str
    ) -> None: ...
    def _all_header_text(self) -> str: ...
    def has_symbol(self, symbol: str) -> bool: ...

from typing import (
    Dict,
    List,
    Optional,
    Union,
)


def _valid_root(spec: Dict[str, <class 'Union'>], root: str) -> <class 'Union'>[None, List[str]]: ...


def detect(
    platform: str,
    override: <class 'Union'>[str, None] = ...
) -> <class 'Union'>[codebreaker.java.console.sdk.SdkInfo, None]: ...


def require(platform: str, override: <class 'Union'>[str, None] = ...) -> SdkInfo: ...


def requires_sdk(platform: <class 'Union'>[str, None]) -> bool: ...


class SdkInfo:
    def __init__(
        self,
        platform: str,
        name: str,
        root: str,
        include_dirs: List[<class 'Union'>[Any, str]],
        source: str
    ) -> None: ...
    def _all_header_text(self) -> str: ...
    def has_symbol(self, symbol: str) -> bool: ...
