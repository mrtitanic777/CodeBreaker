from typing import (
    Dict,
    Tuple,
)


def _kinds() -> Dict[str, str]: ...


def digests() -> Dict[str, str]: ...


def identity(module_path: str) -> Tuple[str, str]: ...


def kind_name(module_path: str) -> str: ...
