from typing import (
    Any,
    Dict,
    List,
    Optional,
    Union,
)


def _bare(name: str) -> str: ...


def _table(platform_: str, kind: str) -> Dict[str, Any]: ...


def api_model(
    platform_: str,
    api: str
) -> <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, int], Dict[str, str], None]: ...


def argument_bytes(platform_: str, api: str, module: <class 'Union'>[str, None] = ...) -> <class 'Union'>[int, None]: ...


def declared_argument_bytes(name: str) -> None: ...


def import_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def platform(name: str) -> str: ...


def prototype(platform_: str, name: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def signatures(platform_: str) -> List[List[<class 'Union'>]]: ...


def table(
    platform_: str,
    kind: str
) -> Dict[str, <class 'Union'>[str, List[str], <class 'Union'>, List[<class 'Union'>]]]: ...

from typing import (
    Any,
    Dict,
    Optional,
)


def _bare(name: str) -> str: ...


def _table(platform_: str, kind: str) -> Dict[str, Any]: ...


def api_model(platform_: str, api: str) -> <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, int], None]: ...


def argument_bytes(platform_: str, api: str, module: <class 'Union'>[str, None] = ...) -> <class 'Union'>[int, None]: ...


def import_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def platform(name: str) -> str: ...


def prototype(platform_: str, name: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def table(platform_: str, kind: str) -> Dict[str, <class 'Union'>[<class 'Union'>, List[str], str]]: ...

from typing import (
    Any,
    Dict,
    List,
    Optional,
    Union,
)


def _bare(name: str) -> str: ...


def _table(platform_: str, kind: str) -> Dict[str, Any]: ...


def api_model(
    platform_: str,
    api: str
) -> <class 'Union'>[Dict[str, str], None, Dict[str, int], Dict[str, <class 'Union'>]]: ...


def argument_bytes(platform_: str, api: str, module: <class 'Union'>[str, None] = ...) -> <class 'Union'>[int, None]: ...


def import_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def platform(name: str) -> str: ...


def prototype(platform_: str, name: str) -> <class 'Union'>[Dict[str, <class 'Union'>], None]: ...


def signatures(platform_: str) -> List[List[<class 'Union'>]]: ...


def table(
    platform_: str,
    kind: str
) -> Dict[str, <class 'Union'>[str, List[str], <class 'Union'>, List[<class 'Union'>]]]: ...

from typing import (
    Dict,
    Optional,
)


def _table(platform_: str, kind: str) -> Dict[str, <class 'Union'>[<class 'Union'>, str, Dict[str, <class 'Union'>]]]: ...


def api_model(platform_: str, api: str) -> <class 'Union'>[Dict[str, <class 'Union'>], Dict[str, int], None]: ...


def argument_bytes(platform_: str, api: str, module: None = ...) -> <class 'Union'>[int, None]: ...


def import_name(module: str, ordinal: int) -> <class 'Union'>[str, None]: ...


def platform(name: str) -> str: ...


def table(platform_: str, kind: str) -> Dict[str, str]: ...
