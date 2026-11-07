from typing import (
    Dict,
    List,
    Optional,
    Tuple,
    Union,
)


class XenixC:
    def __init__(self, profile: Dict[str, <class 'Union'>], variant: None = ...) -> None: ...
    def compile(
        self,
        src: bytes,
        headers: None = ...,
        include: List[str] = ...,
        switches: None = ...,
        cache: bool = ...
    ) -> Tuple[bytes, str]: ...
