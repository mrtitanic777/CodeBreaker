from typing import (
    Callable,
    List,
    Optional,
)


def active() -> Profile: ...


def get(name: str) -> Profile: ...


def names() -> List[str]: ...


def on_change(fn: Callable) -> Callable: ...


def register(profile: Profile) -> Profile: ...


def use(name: str) -> Profile: ...


class Profile:
    def __init__(
        self,
        name: str,
        static_init_order: <class 'Union'>[None, List[str]] = ...,
        package_roles: <class 'Union'>[Dict[str, str], None] = ...,
        heavy_types: <class 'Union'>[Tuple[str, str, str, str], None] = ...,
        vcxproj_filters: <class 'Union'>[List[Tuple[str, str]], None] = ...,
        nullable: <class 'Union'>[Set[str], None] = ...,
        nullable_fields: <class 'Union'>[Set[str], None] = ...,
        nullable_getters: <class 'Union'>[Set[str], None] = ...,
        nullable_getter_return_type: <class 'Union'>[Dict[str, str], None] = ...,
        unsigned_byte_scalar_fields: None = ...,
        default_value_types: None = ...,
        dead_types: <class 'Union'>[str, None] = ...,
        optional_types: None = ...
    ) -> None: ...
