from typing import (
    Dict,
    Tuple,
    Union,
)


def _table() -> Dict[str, <class 'Union'>]: ...


def label(device: int, service: int) -> str: ...


def messages() -> Dict[int, str]: ...


def service_name(device: int, service: int) -> str: ...


def size() -> Tuple[int, int, int]: ...
